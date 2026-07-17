#if !defined(AFX_EXTERNALTOOLHOSTPAGE_H__INCLUDED_)
#define AFX_EXTERNALTOOLHOSTPAGE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "ExternalTabConfig.h"

class CExternalToolHostPage : public CWnd
{
public:
    CExternalToolHostPage();
    virtual ~CExternalToolHostPage();

    void Configure(const CExternalTabDefinition &definition);
    BOOL Create(CWnd *pParent);
    void Activate();
    void Shutdown();

protected:
    CExternalTabDefinition m_Definition;
    CStatic m_Status;
    PROCESS_INFORMATION m_ProcessInfo;
    HANDLE m_JobHandle;
    HWND m_HostedWindow;
    DWORD m_LaunchTick;
    BOOL m_LaunchAttempted;
    BOOL m_Embedded;

    void SetStatus(LPCTSTR text);
    BOOL Launch();
    void PollForWindow();
    HWND FindTopLevelProcessWindow() const;
    BOOL EmbedWindow(HWND window);
    void ResizeHostedWindow();
    void DetachHostedWindow();

    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnDestroy();
    DECLARE_MESSAGE_MAP()
};

#endif
