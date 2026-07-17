#if !defined(AFX_JOYSTICKINPUT_H__INCLUDED_)
#define AFX_JOYSTICKINPUT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define WM_HEMU_JOYSTICK_INPUT (WM_APP + 20)

struct HEMU_JOYSTICK_STATE
{
    LONG x;
    LONG y;
    LONG rudder;
    DWORD pov;
    BYTE buttons[32];
};

class CJoystickInput
{
public:
    CJoystickInput();
    ~CJoystickInput();

    BOOL Start(HINSTANCE instance, HWND notificationWindow);
    void Stop(void);
    void SetEnabled(const BOOL enabled, const BOOL persist = TRUE);
    BOOL IsEnabled(void) const;
    int GetPollRateHz(void) const;
    BOOL ConsumeLatestState(HEMU_JOYSTICK_STATE *state);

private:
    CJoystickInput(const CJoystickInput &);
    CJoystickInput &operator=(const CJoystickInput &);

    static DWORD WINAPI ThreadProc(LPVOID context);
    static BOOL CALLBACK EnumDevicesCallback(
        LPCDIDEVICEINSTANCE instance, LPVOID context);
    static BOOL CALLBACK EnumAxesCallback(
        LPCDIDEVICEOBJECTINSTANCE object, LPVOID context);

    DWORD Run(void);
    BOOL InitializeDirectInput(void);
    BOOL FindJoystick(void);
    void ReleaseJoystick(void);
    void ReleaseDirectInput(void);
    void PublishState(const DIJOYSTATE2 &state);
    HANDLE CreatePollTimer(void) const;
    void LoadSettings(void);
    void StoreEnabledSetting(void) const;

    HINSTANCE m_Instance;
    HWND m_NotificationWindow;
    HANDLE m_Thread;
    HANDLE m_StopEvent;
    HANDLE m_SettingsChangedEvent;
    IDirectInput8 *m_DirectInput;
    IDirectInputDevice8 *m_Joystick;
    volatile LONG m_Enabled;
    volatile LONG m_UpdatePosted;
    int m_PollRateHz;
    CRITICAL_SECTION m_StateLock;
    HEMU_JOYSTICK_STATE m_LatestState;
    BOOL m_HasLatestState;
};

#endif
