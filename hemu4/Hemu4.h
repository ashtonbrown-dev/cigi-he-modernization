/** <pre>
 *  Copyright 2004 The Boeing Company
 *  Author: Lance Durham
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2.1 of the License, or (at
 *  your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * </pre>
 */

// Hemu4.h : main header file for the HEMU3 application
//

#if !defined(AFX_HEMU3_H__848408BD_74DC_444E_9C7F_4F3A049DF5E8__INCLUDED_)
#define AFX_HEMU3_H__848408BD_74DC_444E_9C7F_4F3A049DF5E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
//#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "MainFrm.h"
#include "JoystickInput.h"

/////////////////////////////////////////////////////////////////////////////
// CHemuApp:
// See Hemu4.cpp for the implementation of this class
//

class CHemuApp : public CWinApp
{
public:
    CHemuApp();
    CMainFrame &GetMainFrame(void);
    BOOL ShowSetupDlg(void);
    void InitializeCIGI(void);
    void InitializeTrees(void);
    void EnsureDefaultOwnship(void);
    CJoystickInput &GetJoystickInput(void);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHemuApp)
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    //}}AFX_VIRTUAL

    // Implementation
    //{{AFX_MSG(CHemuApp)
    afx_msg void OnAppAbout();
    afx_msg void OnHelpContents();
    afx_msg void OnHelpIndex();
    afx_msg void OnFileOpen();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    // Create a handle to a mutex that the Windows driver can use to
    // detect if this process has died.
    HANDLE m_ForceShutdownMutexHandle;
    HANDLE m_DriverProcessHandle;
    BOOL m_IpcInitialized;
    BOOL m_InitializationComplete;
    CJoystickInput m_JoystickInput;

    int LoadDriver(void);
    void ShutdownDriver(void);
    BOOL CheckForMultiProcessor(void);
    BOOL SetupDBFromRegistry(void);
    BOOL SetupCommFromRegistry(void);
    BOOL SetupCigiOptionsFromRegistry(void);
};

inline CMainFrame &CHemuApp::GetMainFrame(void)
{
    return *((CMainFrame *)m_pMainWnd);
}

inline CJoystickInput &CHemuApp::GetJoystickInput(void)
{
    return m_JoystickInput;
}

extern CHemuApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEMU3_H__848408BD_74DC_444E_9C7F_4F3A049DF5E8__INCLUDED_)
