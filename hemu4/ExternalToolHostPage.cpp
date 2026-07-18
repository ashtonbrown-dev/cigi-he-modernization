#include "stdafx.h"
#include "ExternalToolHostPage.h"

namespace
{
const UINT_PTR FindWindowTimerId = 1;
const UINT FindWindowPollMilliseconds = 250;
const DWORD FindWindowTimeoutMilliseconds = 10000;
const DWORD CloseProcessWaitMilliseconds = 2000;
const DWORD TerminateProcessWaitMilliseconds = 1000;

struct CWindowSearchContext
{
    DWORD ProcessId;
    HWND Window;
    int Score;
};

BOOL CALLBACK FindProcessWindow(HWND window, LPARAM parameter)
{
    CWindowSearchContext *context =
        reinterpret_cast<CWindowSearchContext *>(parameter);

    DWORD processId = 0;
    GetWindowThreadProcessId(window, &processId);
    if (processId != context->ProcessId
        || GetWindow(window, GW_OWNER) != NULL)
        return TRUE;

    const LONG_PTR extendedStyle = GetWindowLongPtr(window, GWL_EXSTYLE);
    if (extendedStyle & WS_EX_TOOLWINDOW)
        return TRUE;

    int score = 0;
    if (IsWindowVisible(window))
        score += 2;
    if (extendedStyle & WS_EX_APPWINDOW)
        score += 1;

    if (score > context->Score) {
        context->Window = window;
        context->Score = score;
    }

    return score < 3;
}

CString QuoteCommandArgument(const CString &argument)
{
    CString quoted(_T("\""));
    for (int index = 0; index < argument.GetLength(); ++index) {
        if (argument[index] == _T('\"'))
            quoted += _T("\\\"");
        else
            quoted += argument[index];
    }
    quoted += _T("\"");
    return quoted;
}

CString FormatWindowsError(const DWORD error)
{
    LPTSTR message = NULL;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
                      | FORMAT_MESSAGE_FROM_SYSTEM
                      | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  reinterpret_cast<LPTSTR>(&message), 0, NULL);

    CString result;
    if (message != NULL) {
        result = message;
        result.TrimRight();
        LocalFree(message);
    } else {
        result.Format(_T("Windows error %lu"), error);
    }
    return result;
}
}

BEGIN_MESSAGE_MAP(CExternalToolHostPage, CWnd)
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

CExternalToolHostPage::CExternalToolHostPage()
    : m_JobHandle(NULL), m_HostedWindow(NULL), m_LaunchTick(0),
      m_LaunchAttempted(FALSE), m_Embedded(FALSE)
{
    ZeroMemory(&m_ProcessInfo, sizeof(m_ProcessInfo));
}

CExternalToolHostPage::~CExternalToolHostPage()
{
    Shutdown();
}

void CExternalToolHostPage::Configure(
    const CExternalTabDefinition &definition)
{
    m_Definition = definition;
}

BOOL CExternalToolHostPage::Create(CWnd *pParent)
{
    const CString className = AfxRegisterWndClass(
        CS_HREDRAW | CS_VREDRAW, LoadCursor(NULL, IDC_ARROW),
        reinterpret_cast<HBRUSH>(COLOR_3DFACE + 1), NULL);

    if (!CWnd::CreateEx(WS_EX_CONTROLPARENT, className, _T(""),
                        WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                        CRect(0, 0, 0, 0), pParent, 0))
        return FALSE;

    if (!m_Status.Create(_T(""),
                         WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
                         CRect(0, 0, 0, 0), this, 1)) {
        DestroyWindow();
        return FALSE;
    }

    CFont *font = pParent->GetFont();
    if (font != NULL)
        m_Status.SetFont(font);

    if (m_Definition.LaunchOnSelect)
        SetStatus(_T("Select this tab to launch the external application."));
    else
        SetStatus(_T("LaunchOnSelect is disabled for this tab."));

    return TRUE;
}

void CExternalToolHostPage::Activate()
{
    if (m_Definition.LaunchOnSelect && !m_LaunchAttempted)
        Launch();
}

void CExternalToolHostPage::SetStatus(LPCTSTR text)
{
    if (m_Status.GetSafeHwnd()) {
        m_Status.SetWindowText(text);
        m_Status.ShowWindow(SW_SHOW);
    }
}

BOOL CExternalToolHostPage::Launch()
{
    m_LaunchAttempted = TRUE;

    const DWORD attributes = GetFileAttributes(m_Definition.Executable);
    if (attributes == INVALID_FILE_ATTRIBUTES
        || (attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        CString status;
        status.Format(_T("Executable not found:\r\n%s"),
                      (LPCTSTR)m_Definition.Executable);
        SetStatus(status);
        return FALSE;
    }

    CString commandLine = QuoteCommandArgument(m_Definition.Executable);
    if (!m_Definition.Arguments.IsEmpty()) {
        commandLine += _T(" ");
        commandLine += m_Definition.Arguments;
    }

    STARTUPINFO startupInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESHOWWINDOW;
    startupInfo.wShowWindow = SW_HIDE;
    ZeroMemory(&m_ProcessInfo, sizeof(m_ProcessInfo));

    DWORD creationFlags = 0;
    if (m_Definition.CloseWithHemu) {
        m_JobHandle = CreateJobObject(NULL, NULL);
        if (m_JobHandle == NULL) {
            CString status;
            status.Format(_T("Could not create external application job:\r\n%s"),
                          (LPCTSTR)FormatWindowsError(GetLastError()));
            SetStatus(status);
            return FALSE;
        }

        JOBOBJECT_EXTENDED_LIMIT_INFORMATION jobLimits;
        ZeroMemory(&jobLimits, sizeof(jobLimits));
        jobLimits.BasicLimitInformation.LimitFlags =
            JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        if (!SetInformationJobObject(
                m_JobHandle, JobObjectExtendedLimitInformation,
                &jobLimits, sizeof(jobLimits))) {
            const DWORD error = GetLastError();
            CloseHandle(m_JobHandle);
            m_JobHandle = NULL;

            CString status;
            status.Format(_T("Could not configure external application job:\r\n%s"),
                          (LPCTSTR)FormatWindowsError(error));
            SetStatus(status);
            return FALSE;
        }
        creationFlags = CREATE_SUSPENDED;
    }

    LPTSTR mutableCommandLine =
        commandLine.GetBuffer(commandLine.GetLength() + 1);
    const BOOL created = CreateProcess(
        m_Definition.Executable, mutableCommandLine, NULL, NULL, FALSE,
        creationFlags,
        NULL,
        m_Definition.WorkingDirectory.IsEmpty()
            ? NULL
            : (LPCTSTR)m_Definition.WorkingDirectory,
        &startupInfo, &m_ProcessInfo);
    const DWORD error = created ? ERROR_SUCCESS : GetLastError();
    commandLine.ReleaseBuffer();

    if (!created) {
        if (m_JobHandle != NULL) {
            CloseHandle(m_JobHandle);
            m_JobHandle = NULL;
        }
        CString status;
        status.Format(_T("Could not launch the external application:\r\n%s"),
                      (LPCTSTR)FormatWindowsError(error));
        SetStatus(status);
        return FALSE;
    }

    if (m_Definition.CloseWithHemu) {
        if (!AssignProcessToJobObject(m_JobHandle, m_ProcessInfo.hProcess)) {
            const DWORD assignError = GetLastError();
            TerminateProcess(m_ProcessInfo.hProcess, 0);
            WaitForSingleObject(m_ProcessInfo.hProcess,
                                TerminateProcessWaitMilliseconds);
            CloseHandle(m_ProcessInfo.hThread);
            CloseHandle(m_ProcessInfo.hProcess);
            CloseHandle(m_JobHandle);
            ZeroMemory(&m_ProcessInfo, sizeof(m_ProcessInfo));
            m_JobHandle = NULL;

            CString status;
            status.Format(
                _T("Could not contain the external application process:\r\n%s"),
                (LPCTSTR)FormatWindowsError(assignError));
            SetStatus(status);
            return FALSE;
        }

        if (ResumeThread(m_ProcessInfo.hThread) == (DWORD)-1) {
            const DWORD resumeError = GetLastError();
            TerminateProcess(m_ProcessInfo.hProcess, 0);
            WaitForSingleObject(m_ProcessInfo.hProcess,
                                TerminateProcessWaitMilliseconds);
            CloseHandle(m_ProcessInfo.hThread);
            CloseHandle(m_ProcessInfo.hProcess);
            CloseHandle(m_JobHandle);
            ZeroMemory(&m_ProcessInfo, sizeof(m_ProcessInfo));
            m_JobHandle = NULL;

            CString status;
            status.Format(
                _T("Could not start the contained external application:\r\n%s"),
                (LPCTSTR)FormatWindowsError(resumeError));
            SetStatus(status);
            return FALSE;
        }
    }

    if (m_ProcessInfo.hThread != NULL) {
        CloseHandle(m_ProcessInfo.hThread);
        m_ProcessInfo.hThread = NULL;
    }

    m_LaunchTick = GetTickCount();
    SetStatus(_T("Application launched; waiting for its window..."));
    SetTimer(FindWindowTimerId, FindWindowPollMilliseconds, NULL);
    PollForWindow();
    return TRUE;
}

HWND CExternalToolHostPage::FindTopLevelProcessWindow() const
{
    if (m_ProcessInfo.dwProcessId == 0)
        return NULL;

    CWindowSearchContext context;
    context.ProcessId = m_ProcessInfo.dwProcessId;
    context.Window = NULL;
    context.Score = -1;
    EnumWindows(FindProcessWindow, reinterpret_cast<LPARAM>(&context));
    return context.Window;
}

void CExternalToolHostPage::PollForWindow()
{
    if (m_Embedded || m_ProcessInfo.hProcess == NULL)
        return;

    const HWND window = FindTopLevelProcessWindow();
    if (window != NULL && EmbedWindow(window)) {
        KillTimer(FindWindowTimerId);
        return;
    }

    if (WaitForSingleObject(m_ProcessInfo.hProcess, 0) == WAIT_OBJECT_0) {
        KillTimer(FindWindowTimerId);
        SetStatus(_T("The external application exited before a window was found."));
        return;
    }

    if (GetTickCount() - m_LaunchTick >= FindWindowTimeoutMilliseconds) {
        KillTimer(FindWindowTimerId);
        SetStatus(_T("The application is running detached; no embeddable window was found."));
    }
}

BOOL CExternalToolHostPage::EmbedWindow(HWND window)
{
    CWnd *mainWindow = AfxGetMainWnd();
    const HWND mainWindowHandle =
        mainWindow ? mainWindow->GetSafeHwnd() : NULL;
    const HWND foregroundWindow = ::GetForegroundWindow();
    DWORD foregroundProcessId = 0;
    if (foregroundWindow)
        GetWindowThreadProcessId(
            foregroundWindow, &foregroundProcessId);
    const BOOL restoreHemuFocus =
        mainWindowHandle
        && (foregroundWindow == mainWindowHandle
            || (foregroundWindow
                && ::IsChild(mainWindowHandle, foregroundWindow))
            || foregroundProcessId == m_ProcessInfo.dwProcessId);

    ::ShowWindow(window, SW_HIDE);

    SetLastError(ERROR_SUCCESS);
    const HWND previousParent = ::SetParent(window, m_hWnd);
    const DWORD error = GetLastError();
    if (previousParent == NULL && error != ERROR_SUCCESS) {
        CString status;
        status.Format(_T("The application is running detached:\r\n%s"),
                      (LPCTSTR)FormatWindowsError(error));
        SetStatus(status);
        return FALSE;
    }

    LONG_PTR style = GetWindowLongPtr(window, GWL_STYLE);
    style &= ~(WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX
               | WS_MAXIMIZEBOX | WS_SYSMENU | WS_VISIBLE);
    style |= WS_CHILD;
    SetWindowLongPtr(window, GWL_STYLE, style);

    LONG_PTR extendedStyle = GetWindowLongPtr(window, GWL_EXSTYLE);
    extendedStyle &= ~WS_EX_APPWINDOW;
    SetWindowLongPtr(window, GWL_EXSTYLE, extendedStyle);

    m_HostedWindow = window;
    m_Embedded = TRUE;
    m_Status.ShowWindow(SW_HIDE);
    ResizeHostedWindow();

    // Embedding occurs asynchronously after normal application startup. A
    // cooperative tool starts hidden, but a tool or window framework may still
    // briefly become foreground while creating its native window. Restore HEMU
    // only when HEMU or this tool owned the foreground; never steal focus from
    // another application the user selected while the tool was launching.
    if (restoreHemuFocus)
        mainWindow->PostMessage(WM_HEMU_EXTERNAL_TOOL_EMBEDDED);

    return TRUE;
}

void CExternalToolHostPage::ResizeHostedWindow()
{
    if (!m_Embedded || !::IsWindow(m_HostedWindow))
        return;

    CRect clientRect;
    GetClientRect(&clientRect);
    ::SetWindowPos(m_HostedWindow, NULL, 0, 0, clientRect.Width(),
                   clientRect.Height(),
                   SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED
                       | SWP_SHOWWINDOW);
}

void CExternalToolHostPage::DetachHostedWindow()
{
    if (!m_Embedded || !::IsWindow(m_HostedWindow))
        return;

    ::SetParent(m_HostedWindow, NULL);
    LONG_PTR style = GetWindowLongPtr(m_HostedWindow, GWL_STYLE);
    style &= ~WS_CHILD;
    style |= WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    SetWindowLongPtr(m_HostedWindow, GWL_STYLE, style);
    ::SetWindowPos(m_HostedWindow, NULL, 100, 100, 800, 600,
                   SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED
                       | SWP_SHOWWINDOW);

    m_HostedWindow = NULL;
    m_Embedded = FALSE;
}

void CExternalToolHostPage::Shutdown()
{
    if (GetSafeHwnd())
        KillTimer(FindWindowTimerId);

    if (m_Definition.CloseWithHemu) {
        HWND window = m_HostedWindow;
        if (!::IsWindow(window))
            window = FindTopLevelProcessWindow();
        if (::IsWindow(window))
            ::PostMessage(window, WM_CLOSE, 0, 0);

        if (m_ProcessInfo.hProcess != NULL) {
            const DWORD waitResult = WaitForSingleObject(
                m_ProcessInfo.hProcess, CloseProcessWaitMilliseconds);
            if (waitResult == WAIT_TIMEOUT) {
                TerminateProcess(m_ProcessInfo.hProcess, 0);
                WaitForSingleObject(m_ProcessInfo.hProcess,
                                    TerminateProcessWaitMilliseconds);
            }
        }
    } else {
        DetachHostedWindow();
    }

    if (m_ProcessInfo.hThread != NULL) {
        CloseHandle(m_ProcessInfo.hThread);
        m_ProcessInfo.hThread = NULL;
    }
    if (m_ProcessInfo.hProcess != NULL) {
        CloseHandle(m_ProcessInfo.hProcess);
        m_ProcessInfo.hProcess = NULL;
    }
    if (m_JobHandle != NULL) {
        CloseHandle(m_JobHandle);
        m_JobHandle = NULL;
    }
    m_ProcessInfo.dwProcessId = 0;
    m_ProcessInfo.dwThreadId = 0;
    m_HostedWindow = NULL;
    m_Embedded = FALSE;
}

void CExternalToolHostPage::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    if (m_Status.GetSafeHwnd())
        m_Status.MoveWindow(0, 0, cx, cy);
    ResizeHostedWindow();
}

void CExternalToolHostPage::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == FindWindowTimerId) {
        PollForWindow();
        return;
    }

    CWnd::OnTimer(nIDEvent);
}

void CExternalToolHostPage::OnDestroy()
{
    Shutdown();
    CWnd::OnDestroy();
}
