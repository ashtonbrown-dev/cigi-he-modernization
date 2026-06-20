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

// ScriptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "Hemu4.h"
#include "hemumsg.h"
#include "Script.h"
#include "regutils.h"
#include "ScriptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptDlg dialog

CScriptDlg::CScriptDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CScriptDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CScriptDlg)
    //}}AFX_DATA_INIT

    m_ProgressCtr = 0;
    m_TotalLines = 0;
}

void CScriptDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CScriptDlg)
    DDX_Control(pDX, IDC_LABEL_WAITTIME, m_WaitValue);
    DDX_Control(pDX, IDC_LABEL_WAIT, m_WaitLabel);
    DDX_Control(pDX, IDC_BUTTON_EDIT, m_ButtonEdit);
    DDX_Control(pDX, IDC_LABEL_PROGRESS, m_CurrentPos);
    DDX_Control(pDX, IDC_PROGRESS_METER, m_ProgressMeter);
    DDX_Control(pDX, IDC_COMBO_FILENAME, m_ComboFilename);
    DDX_Control(pDX, IDC_BUTTON_STOP, m_ButtonStop);
    DDX_Control(pDX, IDC_BUTTON_PLAY, m_ButtonPlay);
    DDX_Control(pDX, IDC_BUTTON_PAUSE, m_ButtonPause);
    DDX_Control(pDX, IDC_BUTTON_BROWSE, m_ButtonBrowse);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CScriptDlg, CDialog)
    //{{AFX_MSG_MAP(CScriptDlg)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
    ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnButtonPause)
    ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
    ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
    ON_CBN_SELCHANGE(IDC_COMBO_FILENAME, OnSelchangeFilename)
    ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptDlg message handlers

void CScriptDlg::OnOK()
{
}

void CScriptDlg::OnCancel()
{
    OnButtonStop();
    CDialog::OnCancel();
}

BOOL CScriptDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_ButtonStop.SetIcon(theApp.LoadIcon(IDI_STOP));
    m_ButtonPause.SetIcon(theApp.LoadIcon(IDI_PAUSE));
    m_ButtonPlay.SetIcon(theApp.LoadIcon(IDI_PLAY));

    ReadPathsFromRegistry();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CScriptDlg::OnButtonBrowse()
{
    CFileDialog dlg(TRUE, "scp", NULL, OFN_HIDEREADONLY | OFN_CREATEPROMPT,
                    "Script Files (*.scp)|*.scp|All Files (*.*)|*.*||");

    if (dlg.DoModal() != IDCANCEL) {
        InitializePlayback((LPCTSTR)dlg.GetPathName());
        m_ButtonEdit.EnableWindow(TRUE);
    }
}

void CScriptDlg::InitializePlayback(LPCTSTR path)
{
    HANDLE hfile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ,
                              NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,
                              NULL);

    // Figure out the creation date, length, and size of the file.
    BY_HANDLE_FILE_INFORMATION fileinfo;
    if (GetFileInformationByHandle(hfile, &fileinfo)) {
        // Get the length of the recording.
        m_TotalLines = GetScriptFileLength(hfile);
        m_ProgressMeter.SetRange32(0, m_TotalLines);

        m_ButtonPlay.EnableWindow(TRUE);
        m_ButtonPause.EnableWindow(FALSE);
        m_ButtonStop.EnableWindow(FALSE);
    } else {
        m_CurrentPos.SetWindowText("0%");

        m_ButtonPlay.EnableWindow(FALSE);
        m_ButtonPause.EnableWindow(FALSE);
        m_ButtonStop.EnableWindow(FALSE);
    }

    CloseHandle(hfile);

    // Then save the path to the combo and the registry.
    SavePathToCombo(path);
    SavePathToRegistry(path);
}

void CScriptDlg::StopScript(void)
{
    m_ButtonPlay.SetCheck(0);
    m_ButtonPause.SetCheck(0);
    m_ButtonPause.EnableWindow(FALSE);
    m_ButtonStop.EnableWindow(FALSE);
    m_ComboFilename.EnableWindow(TRUE);

    if (g_ScriptState != SCRIPT_STATE_STOP)
        FinishScriptProgress();

    g_ScriptState = SCRIPT_STATE_STOP;
    g_TestScriptTranspDelayRunning = 0;


    m_ButtonEdit.EnableWindow(TRUE);
    m_WaitLabel.EnableWindow(FALSE);
    m_WaitValue.EnableWindow(FALSE);
    m_WaitValue.SetWindowText("0 sec");
}

void CScriptDlg::OnButtonPause()
{
    if (g_ScriptState == SCRIPT_STATE_PAUSE) {
        // Freeze the host emulator.
        ::SetFreezeFlag(TRUE);

        // Send a message to the RT driver.
        MESSAGE_PAUSE_EXERCISE msg;
        PostDriverMsg(msg);

        g_ScriptState = SCRIPT_STATE_PLAY;
    } else {
        g_ScriptState = SCRIPT_STATE_PAUSE;
    }
}

void CScriptDlg::OnButtonPlay()
{
    // If we are currently paused, unpause us.
    if (g_ScriptState == SCRIPT_STATE_PAUSE) {
        // Freeze the host emulator.
        ::SetFreezeFlag(TRUE);

        // Send a message to the RT driver.
        MESSAGE_PAUSE_EXERCISE msg;
        PostDriverMsg(msg);

        g_ScriptState = SCRIPT_STATE_PLAY;
        g_ScriptParsePause = FALSE;
    } else { // Otherwise, play the script from the beginning.
        CString filename;
        m_ComboFilename.GetWindowText(filename);
        if ((filename == "") && (m_TotalLines == 0))
            return;

        ResetScriptProgress();

        // Set the state to play BEFORE calling ExecuteScript().
        g_ScriptState = SCRIPT_STATE_PLAY;
        g_ScriptParsePause = FALSE;

        ExecuteScript((LPCTSTR)filename);
    }

    m_ButtonPlay.SetCheck(1);
    m_ButtonPause.SetCheck(0);
    m_ButtonPause.EnableWindow(TRUE);
    m_ButtonStop.EnableWindow(TRUE);
    m_ButtonEdit.EnableWindow(FALSE);
    m_ComboFilename.EnableWindow(FALSE);
}

void CScriptDlg::OnButtonStop()
{
    // Make the button act as a push button.
    m_ButtonStop.SetCheck(0);

    m_ButtonPlay.SetCheck(0);
    m_ButtonPlay.EnableWindow(TRUE);
    m_ButtonPause.SetCheck(0);
    m_ButtonPause.EnableWindow(FALSE);
    m_ButtonStop.EnableWindow(FALSE);
    m_ButtonEdit.EnableWindow(TRUE);
    m_WaitLabel.EnableWindow(FALSE);
    m_WaitValue.EnableWindow(FALSE);
    m_WaitValue.SetWindowText("0 sec");

    g_ScriptState = SCRIPT_STATE_STOP;
    g_ScriptParsePause = FALSE;

    // Deactivate the wait countdown timer if enabled.
    MESSAGE_SCRIPT_END_WAIT_TIMER end_msg;
    PostGUIMsg(end_msg);

    // Turn off waypoint flight for all entities.
    g_DataManager.DisableWaypointFlight();

    // Stop the Transport Delay Test if it is running.
    MESSAGE_TDTEST_STOP tdtestmsg;
    PostDriverMsg(tdtestmsg);

    g_TestScriptTranspDelayRunning = 0;
}

void CScriptDlg::OnSelchangeFilename()
{
    int idx = m_ComboFilename.GetCurSel();

    if (idx > -1) {
        CString path;
        m_ComboFilename.GetLBText(idx, path);
        InitializePlayback((LPCTSTR)path);
        m_ButtonEdit.EnableWindow(TRUE);
    }
}

void CScriptDlg::SavePathToCombo(LPCTSTR path)
{
    // Add the string to the top of the combo box.
    int i = m_ComboFilename.FindString(-1, path);
    if (i > CB_ERR)
        m_ComboFilename.DeleteString(i);
    m_ComboFilename.InsertString(0, path);
    m_ComboFilename.SetCurSel(0);

    UpdateComboListWidth(path);
}

void CScriptDlg::SavePathToRegistry(LPCTSTR path)
{
    // Move the path to the top.
    int idx = FindPathInArray(path);
    if (idx == -1)
        idx = 9;    // If we can't find it, just start at the bottom.

    // Move each item up.
    for (int i = idx; i > 0; i--)
        m_PathArray[i] = m_PathArray[i - 1];

    m_PathArray[0] = path;

    // Save each item to the registry.
    for (int i = 0; i < 10; i++) {
        CString key;
        key.Format("ScriptFile%d", i);
        SetRegEntryStr("CIGI Host Emulator 4", (LPCTSTR)key, m_PathArray[i]);
    }
}

void CScriptDlg::ReadPathsFromRegistry(void)
{
    for (int i = 9; i >= 0; i--) {
        CString key;
        key.Format("ScriptFile%d", i);
        if (GetRegEntryStr("CIGI Host Emulator 4", (LPCTSTR)key, &m_PathArray[i]) == ERROR_SUCCESS) {
            if (m_PathArray[i] != "") {
                m_ComboFilename.InsertString(0, m_PathArray[i]);
                UpdateComboListWidth(m_PathArray[i]);
            }
        } else {
            m_PathArray[i] = "";
        }
    }

    PackPathArray();
}

int CScriptDlg::FindPathInArray(LPCTSTR path)
{
    for (int i = 0; i < 10; i++)
        if (m_PathArray[i] == path)
            return i;

    return -1;
}

void CScriptDlg::PackPathArray(void)
{
    for (int i = 0; i < 9; i++) {
        if (m_PathArray[0] == "") {
            m_PathArray[i] = m_PathArray[i + 1];
            m_PathArray[i + 1] = "";
        }
    }
}

int CScriptDlg::GetScriptFileLength(HANDLE hfile)
{
    unsigned char buffer[1024];
    unsigned long linecount = 0;
    unsigned long bytesread = 0;
    OVERLAPPED offset = {0};

    while (ReadFile(hfile, buffer, sizeof(buffer), &bytesread, &offset)) {
        // Count the number of newline characters.
        for (unsigned long i = 0; i < bytesread; i++)
            if (buffer[i] == '\n')
                linecount++;

        // Move the read position to the end of the block
        offset.Offset += bytesread;
    }

    // If we have read any characters from the file, add a line since
    // there will be one more line than the number of CR/LF characters.
    if ((linecount > 0) || (bytesread > 0))
        linecount++;

    return linecount;
}

void CScriptDlg::IncrementScriptProgress(const int linenum)
{
    if (m_ProgressCtr > m_TotalLines)
        return;

    m_ProgressCtr = linenum;

    m_ProgressMeter.SetPos(m_ProgressCtr);

    CString str;
    str.Format("%d%%", (m_ProgressCtr * 100) / m_TotalLines);
    m_CurrentPos.SetWindowText((LPCTSTR)str);
}

void CScriptDlg::ResetScriptProgress(void)
{
    m_ProgressCtr = 0;

    m_CurrentPos.SetWindowText("0%");
    m_ProgressMeter.SetPos(0);
}

void CScriptDlg::FinishScriptProgress(void)
{
    m_ProgressMeter.SetPos(m_TotalLines);
    m_CurrentPos.SetWindowText("100%");

    Sleep(500);

    m_ProgressMeter.SetPos(0);
    m_CurrentPos.SetWindowText("0%");

    m_ComboFilename.EnableWindow(TRUE);
}

void CScriptDlg::OnButtonEdit()
{
    CString filename;
    m_ComboFilename.GetLBText(m_ComboFilename.GetCurSel(), filename);

    if (filename != "")
        _spawnlp(_P_NOWAIT, "notepad.exe", "notepad.exe", (LPCTSTR)filename, NULL);
}

void CScriptDlg::SetWaitTime(const int time)
{
    if (time < 0) {
        m_WaitValue.SetWindowText("0 sec");
        m_WaitValue.EnableWindow(FALSE);
        m_WaitLabel.EnableWindow(FALSE);
    } else {
        CString str;
        str.Format("%d sec", time);
        m_WaitValue.SetWindowText((LPCTSTR)str);
        m_WaitValue.EnableWindow(TRUE);
        m_WaitLabel.EnableWindow(TRUE);
    }
}

// Sets the width of the dropdown list to match the longest script path.
// This helps greatly for longer path names, where the name would otherwise
// be cut off.
void CScriptDlg::UpdateComboListWidth(LPCTSTR path)
{
    TEXTMETRIC tm   = { 0 };
    CDC *pDC        = m_ComboFilename.GetDC();
    CFont *pFont    = m_ComboFilename.GetFont();
    CFont *pOldFont = pDC->SelectObject(pFont);
    CSize pathExtent;

    pDC->GetTextMetrics(&tm);
    pathExtent = pDC->GetTextExtent(path);
    pathExtent.cx += tm.tmAveCharWidth;
    pathExtent.cx += GetSystemMetrics(SM_CXVSCROLL + 2 * GetSystemMetrics(SM_CXEDGE));

    pDC->SelectObject(pOldFont);
    m_ComboFilename.ReleaseDC(pDC);

    if (pathExtent.cx > m_ComboFilename.GetDroppedWidth())
        m_ComboFilename.SetDroppedWidth(pathExtent.cx);
}
