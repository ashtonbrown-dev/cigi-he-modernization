#include "stdafx.h"
#include "JoystickInput.h"
#include <limits.h>

namespace
{
const int DefaultPollRateHz = 240;
const int MinimumPollRateHz = 30;
const int MaximumPollRateHz = 1000;
const DWORD HighResolutionTimerFlag = 0x00000002;

typedef HANDLE(WINAPI *CREATE_WAITABLE_TIMER_EX_PROC)(
    LPSECURITY_ATTRIBUTES, LPCTSTR, DWORD, DWORD);

CString GetDirectoryName(const CString &path)
{
    const int slash = path.ReverseFind(_T('\\'));
    return (slash < 0) ? _T("") : path.Left(slash);
}

BOOL DirectoryExists(const CString &path)
{
    const DWORD attributes = GetFileAttributes(path);
    return attributes != INVALID_FILE_ATTRIBUTES
           && (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

BOOL FileExists(const CString &path)
{
    const DWORD attributes = GetFileAttributes(path);
    return attributes != INVALID_FILE_ATTRIBUTES
           && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

CString GetApplicationDirectory(void)
{
    TCHAR modulePath[MAX_PATH] = {0};
    GetModuleFileName(NULL, modulePath, MAX_PATH);

    const CString moduleDirectory = GetDirectoryName(modulePath);
    if (DirectoryExists(moduleDirectory + _T("\\config")))
        return moduleDirectory;

    TCHAR currentDirectory[MAX_PATH] = {0};
    if (GetCurrentDirectory(MAX_PATH, currentDirectory) > 0) {
        const CString workingDirectory(currentDirectory);
        if (DirectoryExists(workingDirectory + _T("\\config")))
            return workingDirectory;
    }

    return moduleDirectory.IsEmpty() ? _T(".") : moduleDirectory;
}

BOOL TryReadInt(const CString &filePath, LPCTSTR section, LPCTSTR key,
                int *value)
{
    if (!value || !FileExists(filePath))
        return FALSE;

    const int MissingValue = INT_MIN;
    const int configuredValue = GetPrivateProfileInt(
        section, key, MissingValue, filePath);
    if (configuredValue == MissingValue)
        return FALSE;

    *value = configuredValue;
    return TRUE;
}
}

CJoystickInput::CJoystickInput()
    : m_Instance(NULL),
      m_NotificationWindow(NULL),
      m_Thread(NULL),
      m_StopEvent(NULL),
      m_SettingsChangedEvent(NULL),
      m_DirectInput(NULL),
      m_Joystick(NULL),
      m_ThrottleAxisOffset((DWORD)-1),
      m_Enabled(TRUE),
      m_UpdatePosted(0),
      m_PollRateHz(DefaultPollRateHz),
      m_HasLatestState(FALSE)
{
    ZeroMemory(&m_LatestState, sizeof(m_LatestState));
    InitializeCriticalSection(&m_StateLock);
}

CJoystickInput::~CJoystickInput()
{
    Stop();
    DeleteCriticalSection(&m_StateLock);
}

BOOL CJoystickInput::Start(HINSTANCE instance, HWND notificationWindow)
{
    Stop();

    m_Instance = instance;
    m_NotificationWindow = notificationWindow;
    LoadSettings();

    m_StopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_SettingsChangedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (!m_StopEvent || !m_SettingsChangedEvent) {
        Stop();
        return FALSE;
    }

    m_Thread = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
    if (!m_Thread) {
        Stop();
        return FALSE;
    }

    return TRUE;
}

void CJoystickInput::Stop(void)
{
    if (m_StopEvent)
        SetEvent(m_StopEvent);

    if (m_Thread) {
        WaitForSingleObject(m_Thread, 5000);
        CloseHandle(m_Thread);
        m_Thread = NULL;
    }

    if (m_SettingsChangedEvent) {
        CloseHandle(m_SettingsChangedEvent);
        m_SettingsChangedEvent = NULL;
    }

    if (m_StopEvent) {
        CloseHandle(m_StopEvent);
        m_StopEvent = NULL;
    }

    m_Instance = NULL;
    m_NotificationWindow = NULL;
    InterlockedExchange(&m_UpdatePosted, 0);
}

void CJoystickInput::SetEnabled(const BOOL enabled, const BOOL persist)
{
    InterlockedExchange(&m_Enabled, enabled ? TRUE : FALSE);

    if (!enabled) {
        EnterCriticalSection(&m_StateLock);
        m_HasLatestState = FALSE;
        InterlockedExchange(&m_UpdatePosted, 0);
        LeaveCriticalSection(&m_StateLock);
    }

    if (persist)
        StoreEnabledSetting();

    if (m_SettingsChangedEvent)
        SetEvent(m_SettingsChangedEvent);
}

BOOL CJoystickInput::IsEnabled(void) const
{
    return InterlockedCompareExchange(
        const_cast<volatile LONG *>(&m_Enabled), 0, 0) ? TRUE : FALSE;
}

int CJoystickInput::GetPollRateHz(void) const
{
    return m_PollRateHz;
}

BOOL CJoystickInput::ConsumeLatestState(HEMU_JOYSTICK_STATE *state)
{
    if (!state)
        return FALSE;

    EnterCriticalSection(&m_StateLock);
    const BOOL hasState = m_HasLatestState;
    if (hasState)
        *state = m_LatestState;
    InterlockedExchange(&m_UpdatePosted, 0);
    LeaveCriticalSection(&m_StateLock);

    return hasState;
}

DWORD WINAPI CJoystickInput::ThreadProc(LPVOID context)
{
    CJoystickInput *input = static_cast<CJoystickInput *>(context);
    return input ? input->Run() : 0;
}

BOOL CALLBACK CJoystickInput::EnumDevicesCallback(
    LPCDIDEVICEINSTANCE instance, LPVOID context)
{
    CJoystickInput *input = static_cast<CJoystickInput *>(context);
    if (!input || !input->m_DirectInput)
        return DIENUM_STOP;

    if (SUCCEEDED(input->m_DirectInput->CreateDevice(
            instance->guidInstance, &input->m_Joystick, NULL))) {
        return DIENUM_STOP;
    }

    return DIENUM_CONTINUE;
}

BOOL CALLBACK CJoystickInput::EnumAxesCallback(
    LPCDIDEVICEOBJECTINSTANCE object, LPVOID context)
{
    CJoystickInput *input = static_cast<CJoystickInput *>(context);
    if (!input || !input->m_Joystick)
        return DIENUM_STOP;

    DIPROPRANGE range;
    ZeroMemory(&range, sizeof(range));
    range.diph.dwSize = sizeof(DIPROPRANGE);
    range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    range.diph.dwHow = DIPH_BYID;
    range.diph.dwObj = object->dwType;

    if (object->guidType == GUID_Slider
        || object->guidType == GUID_ZAxis) {
        range.lMin = -1200;
        range.lMax = 300;

        // Prefer a dedicated slider, but accept the Z axis as a fallback for
        // joysticks that expose their throttle there.
        if (object->guidType == GUID_Slider
            || input->m_ThrottleAxisOffset == (DWORD)-1) {
            input->m_ThrottleAxisOffset = object->dwOfs;
        }
    } else {
        range.lMin = -100;
        range.lMax = 100;
    }

    input->m_Joystick->SetProperty(DIPROP_RANGE, &range.diph);
    return DIENUM_CONTINUE;
}

DWORD CJoystickInput::Run(void)
{
    HANDLE pollTimer = CreatePollTimer();
    HANDLE waitHandles[3] = {m_StopEvent, m_SettingsChangedEvent, pollTimer};

    while (WaitForSingleObject(m_StopEvent, 0) != WAIT_OBJECT_0) {
        if (!IsEnabled()) {
            ReleaseJoystick();
            WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);
            continue;
        }

        if (!m_Joystick && !FindJoystick()) {
            const DWORD result = WaitForMultipleObjects(
                2, waitHandles, FALSE, 1000);
            if (result == WAIT_OBJECT_0)
                break;
            continue;
        }

        HRESULT result = m_Joystick->Poll();
        if (FAILED(result)) {
            result = m_Joystick->Acquire();
            if (SUCCEEDED(result))
                result = m_Joystick->Poll();
        }

        DIJOYSTATE2 state;
        ZeroMemory(&state, sizeof(state));
        if (SUCCEEDED(result))
            result = m_Joystick->GetDeviceState(sizeof(state), &state);

        if (SUCCEEDED(result))
            PublishState(state);
        else
            ReleaseJoystick();

        if (!pollTimer) {
            const DWORD delay = max(1, 1000 / m_PollRateHz);
            if (WaitForMultipleObjects(2, waitHandles, FALSE, delay)
                == WAIT_OBJECT_0) {
                break;
            }
            continue;
        }

        LARGE_INTEGER dueTime;
        dueTime.QuadPart = -(10000000LL / m_PollRateHz);
        SetWaitableTimer(pollTimer, &dueTime, 0, NULL, NULL, FALSE);
        if (WaitForMultipleObjects(3, waitHandles, FALSE, INFINITE)
            == WAIT_OBJECT_0) {
            break;
        }
    }

    if (pollTimer) {
        CancelWaitableTimer(pollTimer);
        CloseHandle(pollTimer);
    }

    ReleaseDirectInput();
    return 0;
}

BOOL CJoystickInput::InitializeDirectInput(void)
{
    if (m_DirectInput)
        return TRUE;

    return DirectInput8Create(
               m_Instance, DIRECTINPUT_VERSION, IID_IDirectInput8,
               reinterpret_cast<void **>(&m_DirectInput), NULL) == DI_OK;
}

BOOL CJoystickInput::FindJoystick(void)
{
    ReleaseJoystick();
    if (!InitializeDirectInput())
        return FALSE;

    m_DirectInput->EnumDevices(
        DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, this,
        DIEDFL_ATTACHEDONLY);
    if (!m_Joystick)
        return FALSE;

    if (FAILED(m_Joystick->SetDataFormat(&c_dfDIJoystick2))
        || FAILED(m_Joystick->SetCooperativeLevel(
            m_NotificationWindow, DISCL_EXCLUSIVE | DISCL_BACKGROUND))) {
        ReleaseJoystick();
        return FALSE;
    }

    DIPROPDWORD property;
    ZeroMemory(&property, sizeof(property));
    property.diph.dwSize = sizeof(DIPROPDWORD);
    property.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    property.diph.dwHow = DIPH_DEVICE;

    property.dwData = 0;
    m_Joystick->SetProperty(DIPROP_BUFFERSIZE, &property.diph);
    property.dwData = DIPROPAUTOCENTER_ON;
    m_Joystick->SetProperty(DIPROP_AUTOCENTER, &property.diph);
    property.dwData = DIPROPAXISMODE_ABS;
    m_Joystick->SetProperty(DIPROP_AXISMODE, &property.diph);
    property.dwData = 1000;
    m_Joystick->SetProperty(DIPROP_DEADZONE, &property.diph);

    m_ThrottleAxisOffset = (DWORD)-1;
    m_Joystick->EnumObjects(EnumAxesCallback, this, DIDFT_AXIS);
    m_Joystick->Acquire();
    return TRUE;
}

void CJoystickInput::ReleaseJoystick(void)
{
    if (!m_Joystick)
        return;

    m_Joystick->Unacquire();
    m_Joystick->Release();
    m_Joystick = NULL;
    m_ThrottleAxisOffset = (DWORD)-1;
}

void CJoystickInput::ReleaseDirectInput(void)
{
    ReleaseJoystick();
    if (m_DirectInput) {
        m_DirectInput->Release();
        m_DirectInput = NULL;
    }
}

void CJoystickInput::PublishState(const DIJOYSTATE2 &state)
{
    HEMU_JOYSTICK_STATE published;
    ZeroMemory(&published, sizeof(published));
    published.x = state.lX;
    published.y = state.lY;
    published.rudder = state.lRz;
    published.hasThrottle = m_ThrottleAxisOffset != (DWORD)-1;
    if (m_ThrottleAxisOffset == DIJOFS_Z)
        published.throttle = -state.lZ;
    else if (m_ThrottleAxisOffset == DIJOFS_SLIDER(1))
        published.throttle = -state.rglSlider[1];
    else if (published.hasThrottle)
        published.throttle = -state.rglSlider[0];
    published.pov = state.rgdwPOV[0];
    CopyMemory(published.buttons, state.rgbButtons,
               sizeof(published.buttons));

    EnterCriticalSection(&m_StateLock);
    m_LatestState = published;
    m_HasLatestState = TRUE;
    LeaveCriticalSection(&m_StateLock);

    if (InterlockedCompareExchange(&m_UpdatePosted, 1, 0) == 0) {
        if (!PostMessage(m_NotificationWindow, WM_HEMU_JOYSTICK_INPUT, 0, 0))
            InterlockedExchange(&m_UpdatePosted, 0);
    }
}

HANDLE CJoystickInput::CreatePollTimer(void) const
{
    HMODULE kernel = GetModuleHandle(_T("kernel32.dll"));
    CREATE_WAITABLE_TIMER_EX_PROC createTimerEx = kernel
        ? reinterpret_cast<CREATE_WAITABLE_TIMER_EX_PROC>(
            GetProcAddress(kernel, "CreateWaitableTimerExA"))
        : NULL;

    HANDLE timer = NULL;
    if (createTimerEx) {
        timer = createTimerEx(NULL, NULL, HighResolutionTimerFlag,
                              TIMER_ALL_ACCESS);
    }

    return timer ? timer : CreateWaitableTimer(NULL, FALSE, NULL);
}

void CJoystickInput::LoadSettings(void)
{
    const CString configDirectory = GetApplicationDirectory() + _T("\\config");
    const CString defaultPath = configDirectory + _T("\\setup_profiles.ini");
    const CString userPath = configDirectory + _T("\\setup_profiles.user.ini");

    int enabled = TRUE;
    int pollRate = DefaultPollRateHz;
    TryReadInt(defaultPath, _T("Joystick"), _T("Enabled"), &enabled);
    TryReadInt(defaultPath, _T("Joystick"), _T("PollRateHz"), &pollRate);
    TryReadInt(userPath, _T("Joystick"), _T("Enabled"), &enabled);
    TryReadInt(userPath, _T("Joystick"), _T("PollRateHz"), &pollRate);

    if (pollRate < MinimumPollRateHz || pollRate > MaximumPollRateHz)
        pollRate = DefaultPollRateHz;

    m_PollRateHz = pollRate;
    InterlockedExchange(&m_Enabled, enabled ? TRUE : FALSE);
}

void CJoystickInput::StoreEnabledSetting(void) const
{
    const CString configDirectory = GetApplicationDirectory() + _T("\\config");
    if (!DirectoryExists(configDirectory))
        CreateDirectory(configDirectory, NULL);

    const CString userPath = configDirectory + _T("\\setup_profiles.user.ini");
    const CString defaultPath = configDirectory + _T("\\setup_profiles.ini");
    if (!FileExists(userPath) && FileExists(defaultPath))
        CopyFile(defaultPath, userPath, FALSE);

    WritePrivateProfileString(
        _T("Joystick"), _T("Enabled"),
        IsEnabled() ? _T("1") : _T("0"), userPath);
}
