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

#if !defined(AFX_RecPlaybackDlg_H__BBF885E6_7050_4F42_94BB_DA25E30536CE__INCLUDED_)
#define AFX_RecPlaybackDlg_H__BBF885E6_7050_4F42_94BB_DA25E30536CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecPlaybackDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecPlaybackDlg dialog

#include "resource.h"

class CRecPlaybackDlg : public CDialog
{
    // Construction
public:
    CRecPlaybackDlg(CWnd *pParent = NULL);   // standard constructor
    void DisplayProgressRange(void);
    void IncrementPlaybackProgress(void);
    void ResetPlaybackProgress(void);
    void FinishPlaybackProgress(void);
    void IncrementCaptureProgress(void);
    void ResetCaptureProgress(void);
    void FinishCaptureProgress(void);
    void IncrementFrame(void);
    void StopPlayback(void);
    void BeginPlayback(void);

    // Dialog Data
    //{{AFX_DATA(CRecPlaybackDlg)
    enum { IDD = IDD_DIALOG_RECPLAYBACK };
    CStatic m_CurrentTime;
    CStatic m_EndTime;
    CProgressCtrl   m_ProgressMeter;
    CComboBox   m_ComboFilename;
    CButton m_ButtonStop;
    CButton m_ButtonPause;
    CButton m_ButtonPlay;
    CButton m_ButtonRecord;
    CButton m_ButtonLoop;
    CString m_strEndTime;
    CString m_strTime;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CRecPlaybackDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    unsigned int m_ProgressCtr;
    unsigned int m_TotalFrames;
    CString m_PathArray[10];

    void InitializePlayback(LPCTSTR path);
    int GetCaptureFileLength(HANDLE hfile);
    void SavePathToCombo(LPCTSTR path);
    void SavePathToRegistry(LPCTSTR path);
    void ReadPathsFromRegistry(void);
    int FindPathInArray(LPCTSTR path);
    void PackPathArray(void);

    // Generated message map functions
    //{{AFX_MSG(CRecPlaybackDlg)
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnButtonBrowse();
    afx_msg void OnButtonRecord();
    afx_msg void OnButtonPlay();
    afx_msg void OnButtonPause();
    afx_msg void OnButtonStop();
    afx_msg void OnButtonLoop();
    afx_msg void OnSelchangeFilename();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline void CRecPlaybackDlg::BeginPlayback(void)
{
    OnButtonPlay();
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RecPlaybackDlg_H__BBF885E6_7050_4F42_94BB_DA25E30536CE__INCLUDED_)
