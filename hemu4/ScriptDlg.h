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

#if !defined(AFX_SCRIPTDLG_H__14921441_F5C3_40C9_AD7A_474CD120CA21__INCLUDED_)
#define AFX_SCRIPTDLG_H__14921441_F5C3_40C9_AD7A_474CD120CA21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CScriptDlg dialog

class CScriptDlg : public CDialog
{
    // Construction
public:
    CScriptDlg(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    void IncrementScriptProgress(const int linenum);
    void ResetScriptProgress(void);
    void FinishScriptProgress(void);
    void StopScript(void);
    void CheckPauseButton(const BOOL state = TRUE);
    void SetWaitTime(const int time);

    // Dialog Data
    //{{AFX_DATA(CScriptDlg)
    enum { IDD = IDD_DIALOG_SCRIPT };
    CStatic m_WaitValue;
    CStatic m_WaitLabel;
    CButton m_ButtonEdit;
    CStatic m_CurrentPos;
    CProgressCtrl   m_ProgressMeter;
    CComboBox   m_ComboFilename;
    CButton m_ButtonStop;
    CButton m_ButtonPlay;
    CButton m_ButtonPause;
    CButton m_ButtonBrowse;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CScriptDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    unsigned int m_ProgressCtr;
    unsigned int m_TotalLines;
    CString m_PathArray[10];

    void InitializePlayback(LPCTSTR path);
    int GetScriptFileLength(HANDLE hfile);
    void SavePathToCombo(LPCTSTR path);
    void SavePathToRegistry(LPCTSTR path);
    void ReadPathsFromRegistry(void);
    int FindPathInArray(LPCTSTR path);
    void PackPathArray(void);

    // Generated message map functions
    //{{AFX_MSG(CScriptDlg)
    afx_msg void OnButtonBrowse();
    afx_msg void OnButtonPause();
    afx_msg void OnButtonPlay();
    afx_msg void OnButtonStop();
    afx_msg void OnSelchangeFilename();
    virtual BOOL OnInitDialog();
    afx_msg void OnButtonEdit();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void UpdateComboListWidth(LPCTSTR path);
};

inline void CScriptDlg::CheckPauseButton(const BOOL state)
{
    m_ButtonPause.SetCheck(state);
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTDLG_H__14921441_F5C3_40C9_AD7A_474CD120CA21__INCLUDED_)
