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

// RecPlaybackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "Hemu4.h"
#include "hemumsg.h"
#include "regutils.h"
#include "RecPlaybackDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static BOOL DirectoryExists(LPCTSTR path)
{
    DWORD attributes = GetFileAttributes(path);

    return ((attributes != INVALID_FILE_ATTRIBUTES) &&
            ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0));
}

static CString GetExecutableDirectory()
{
    TCHAR modulePath[MAX_PATH] = {0};
    DWORD modulePathLength = GetModuleFileName(NULL, modulePath, MAX_PATH);

    if ((modulePathLength == 0) || (modulePathLength >= MAX_PATH))
        return "";

    CString executablePath(modulePath);
    int separator = executablePath.ReverseFind('\\');

    if (separator < 0)
        return "";

    return executablePath.Left(separator + 1);
}

static CString GetRecentDirectoryFromRegistry(LPCTSTR keyFormat)
{
    for (int i = 0; i < 10; i++) {
        CString key;
        key.Format(keyFormat, i);

        CString recentPath;
        if (GetRegEntryStr("CIGI Host Emulator 4", (LPCTSTR)key, &recentPath) != ERROR_SUCCESS)
            continue;

        int separator = recentPath.ReverseFind('\\');
        if (separator < 0)
            continue;

        CString recentDirectory = recentPath.Left(separator + 1);
        if (DirectoryExists((LPCTSTR)recentDirectory))
            return recentDirectory;
    }

    return "";
}

static CString GetInitialDirectoryForAppFolder(LPCTSTR folderName,
                                               LPCTSTR recentRegistryKeyFormat,
                                               BOOL createFolderIfMissing)
{
    CString executableDirectory = GetExecutableDirectory();
    if (executableDirectory == "")
        return "";

    CString appDirectory = executableDirectory + folderName;

    if (!DirectoryExists((LPCTSTR)appDirectory) && createFolderIfMissing)
        CreateDirectory((LPCTSTR)appDirectory, NULL);

    if (DirectoryExists((LPCTSTR)appDirectory))
        return appDirectory;

    CString recentDirectory = GetRecentDirectoryFromRegistry(recentRegistryKeyFormat);
    if (recentDirectory != "")
        return recentDirectory;

    return executableDirectory;
}

/////////////////////////////////////////////////////////////////////////////
// CRecPlaybackDlg dialog


CRecPlaybackDlg::CRecPlaybackDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CRecPlaybackDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CRecPlaybackDlg)
    //}}AFX_DATA_INIT

    m_ProgressCtr = 0;
    m_TotalFrames = 0;
}

void CRecPlaybackDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CRecPlaybackDlg)
    DDX_Control(pDX, IDC_LABEL_TIME, m_CurrentTime);
    DDX_Control(pDX, IDC_LABEL_END_TIME, m_EndTime);
    DDX_Control(pDX, IDC_PROGRESS_METER, m_ProgressMeter);
    DDX_Control(pDX, IDC_COMBO_FILENAME, m_ComboFilename);
    DDX_Control(pDX, IDC_BUTTON_STOP, m_ButtonStop);
    DDX_Control(pDX, IDC_BUTTON_PAUSE, m_ButtonPause);
    DDX_Control(pDX, IDC_BUTTON_PLAY, m_ButtonPlay);
    DDX_Control(pDX, IDC_BUTTON_RECORD, m_ButtonRecord);
    DDX_Control(pDX, IDC_BUTTON_LOOP, m_ButtonLoop);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRecPlaybackDlg, CDialog)
    //{{AFX_MSG_MAP(CRecPlaybackDlg)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
    ON_BN_CLICKED(IDC_BUTTON_RECORD, OnButtonRecord)
    ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
    ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnButtonPause)
    ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
    ON_BN_CLICKED(IDC_BUTTON_LOOP, OnButtonLoop)
    ON_CBN_SELCHANGE(IDC_COMBO_FILENAME, OnSelchangeFilename)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecPlaybackDlg message handlers

void CRecPlaybackDlg::OnOK()
{
}

void CRecPlaybackDlg::OnCancel()
{
    OnButtonStop();

    CDialog::OnCancel();
}

BOOL CRecPlaybackDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_ButtonStop.SetIcon(theApp.LoadIcon(IDI_STOP));
    m_ButtonPause.SetIcon(theApp.LoadIcon(IDI_PAUSE));
    m_ButtonPlay.SetIcon(theApp.LoadIcon(IDI_PLAY));
    m_ButtonRecord.SetIcon(theApp.LoadIcon(IDI_RECORD));
    m_ButtonLoop.SetIcon(theApp.LoadIcon(IDI_LOOP));

    ReadPathsFromRegistry();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CRecPlaybackDlg::StopPlayback(void)
{
    m_ButtonPlay.SetCheck(0);
    m_ButtonRecord.SetCheck(0);
    m_ButtonRecord.EnableWindow(TRUE);
    m_ButtonPause.SetCheck(0);
    m_ButtonPause.EnableWindow(FALSE);
    m_ButtonStop.EnableWindow(FALSE);
    m_ComboFilename.EnableWindow(TRUE);

    if (g_RecPlaybackState & RECPLAYBACK_STATE_RECORD)
        FinishCaptureProgress();
    else if (g_RecPlaybackState & RECPLAYBACK_STATE_PLAY)
        FinishPlaybackProgress();

    g_RecPlaybackState = RECPLAYBACK_STATE_STOP;
}

void CRecPlaybackDlg::IncrementFrame(void)
{
    if (g_RecPlaybackState == RECPLAYBACK_STATE_RECORD)
        IncrementCaptureProgress();
    else if (g_RecPlaybackState == RECPLAYBACK_STATE_PLAY)
        IncrementPlaybackProgress();
}

void CRecPlaybackDlg::OnButtonBrowse()
{
    CFileDialog dlg(TRUE, "cap", NULL,
                    OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_NOCHANGEDIR,
                    "Capture Files (*.cap)|*.cap|All Files (*.*)|*.*||");

    CString initialDirectory = GetInitialDirectoryForAppFolder("captures", "CapFile%d", TRUE);
    if (initialDirectory != "")
        dlg.m_ofn.lpstrInitialDir = (LPCTSTR)initialDirectory;

    if (dlg.DoModal() == IDCANCEL)
        return;

    InitializePlayback((LPCTSTR)dlg.GetPathName());
}

void CRecPlaybackDlg::InitializePlayback(LPCTSTR path)
{
    HANDLE hfile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ,
                              NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,
                              NULL);
    ResetCaptureProgress();

    // Figure out the creation date, length, and size of the file.
    BY_HANDLE_FILE_INFORMATION fileinfo;
    if (GetFileInformationByHandle(hfile, &fileinfo)) {
        // Get the length of the recording.
        m_TotalFrames = GetCaptureFileLength(hfile);
        DisplayProgressRange();

        m_ButtonPlay.EnableWindow(TRUE);
        m_ButtonPause.EnableWindow(FALSE);
        m_ButtonStop.EnableWindow(FALSE);
        m_ButtonRecord.EnableWindow(TRUE);
        m_ButtonLoop.EnableWindow(TRUE);
    } else {
        m_EndTime.SetWindowText("00:00:00");
        m_CurrentTime.SetWindowText("00:00:00");

        m_ButtonPlay.EnableWindow(FALSE);
        m_ButtonPause.EnableWindow(FALSE);
        m_ButtonStop.EnableWindow(FALSE);
        m_ButtonRecord.EnableWindow(TRUE);  // can still record
        m_ButtonLoop.EnableWindow(FALSE);
    }

    CloseHandle(hfile);

    // Then save the path to the combo and the registry.
    SavePathToCombo(path);
    SavePathToRegistry(path);
}

void CRecPlaybackDlg::OnButtonRecord()
{
    if ((g_RecPlaybackState & RECPLAYBACK_STATE_RECORD) ||
        (g_RecPlaybackState == RECPLAYBACK_STATE_PLAY)) {
        if (g_RecPlaybackState == RECPLAYBACK_STATE_RECORD)
            m_ButtonRecord.SetCheck(1);

        return;
    }

    m_ButtonRecord.SetCheck(1);
    m_ButtonPlay.SetCheck(1);
    m_ButtonPause.EnableWindow(TRUE);
    m_ButtonStop.EnableWindow(TRUE);

    // If we are currently paused, unpause us.
    if (g_RecPlaybackState & RECPLAYBACK_STATE_PAUSE) {
        MESSAGE_RESUME_RECPLAYBACK msg;
        PostDriverMsg(msg);
    } else { // Otherwise, open the file and begin recording.
        MESSAGE_BEGIN_RECORD msg;

        m_ComboFilename.GetWindowText(msg.filename, 256);

        if (msg.filename[0] != '\0')
            PostDriverMsg(msg);

        ResetCaptureProgress();
    }

    g_RecPlaybackState = RECPLAYBACK_STATE_RECORD;

    m_ComboFilename.EnableWindow(FALSE);
}

void CRecPlaybackDlg::OnButtonPlay()
{
    if ((g_RecPlaybackState & RECPLAYBACK_STATE_RECORD) ||
        (g_RecPlaybackState == RECPLAYBACK_STATE_PLAY)) {
        m_ButtonPlay.SetCheck(1);
        return;
    }

    m_ButtonRecord.EnableWindow(FALSE);
    m_ButtonPlay.SetCheck(1);
    m_ButtonPause.SetCheck(0);
    m_ButtonPause.EnableWindow(TRUE);
    m_ButtonStop.EnableWindow(TRUE);

    // If we are currently paused, unpause us.
    if (g_RecPlaybackState & RECPLAYBACK_STATE_PAUSE) {
        MESSAGE_RESUME_RECPLAYBACK msg;
        PostDriverMsg(msg);
    } else { // Otherwise, open the file and begin playback.
        MESSAGE_BEGIN_PLAYBACK msg;

        m_ComboFilename.GetWindowText(msg.filename, 256);

        if (msg.filename[0] != '\0')
            PostDriverMsg(msg);

        ResetPlaybackProgress();
    }

    g_RecPlaybackState = RECPLAYBACK_STATE_PLAY;

    m_ComboFilename.EnableWindow(FALSE);
}

void CRecPlaybackDlg::OnButtonPause()
{
    if (GetFreezeFlag()) {
        m_ButtonPause.SetCheck((g_RecPlaybackState & RECPLAYBACK_STATE_PAUSE)
                               ? 1 : 0);
        return;
    }

    if (g_RecPlaybackState) {
        g_RecPlaybackState ^= RECPLAYBACK_STATE_PAUSE;

        int ispaused = g_RecPlaybackState & RECPLAYBACK_STATE_PAUSE;
        m_ButtonPause.SetCheck(ispaused);

        if (ispaused) {
            MESSAGE_PAUSE_RECPLAYBACK msg;
            PostDriverMsg(msg);
        } else {
            MESSAGE_RESUME_RECPLAYBACK msg;
            PostDriverMsg(msg);
        }
    }
}

void CRecPlaybackDlg::OnButtonStop()
{
    // Make the button act as a push button.
    m_ButtonStop.SetCheck(0);

    m_ButtonPlay.SetCheck(0);
    m_ButtonPlay.EnableWindow(TRUE);
    m_ButtonRecord.SetCheck(0);
    m_ButtonRecord.EnableWindow(TRUE);
    m_ButtonPause.SetCheck(0);
    m_ButtonPause.EnableWindow(FALSE);
    m_ButtonStop.EnableWindow(FALSE);

    if (g_RecPlaybackState & RECPLAYBACK_STATE_RECORD) {
        MESSAGE_END_RECORD msg;
        PostDriverMsg(msg);

        FinishCaptureProgress();
    } else if (g_RecPlaybackState & RECPLAYBACK_STATE_PLAY) {
        MESSAGE_END_PLAYBACK msg;
        PostDriverMsg(msg);
    }

    g_RecPlaybackState = RECPLAYBACK_STATE_STOP;

    m_ComboFilename.EnableWindow(TRUE);
}

void CRecPlaybackDlg::OnButtonLoop()
{
    g_bRecPlaybackLoop = !g_bRecPlaybackLoop;
    m_ButtonLoop.SetCheck(g_bRecPlaybackLoop);
}

void CRecPlaybackDlg::OnSelchangeFilename()
{
    int idx = m_ComboFilename.GetCurSel();
    if (idx > -1) {
        CString path;
        m_ComboFilename.GetLBText(idx, path);
        InitializePlayback((LPCTSTR)path);
    }
}

void CRecPlaybackDlg::SavePathToCombo(LPCTSTR path)
{
    // Add the string to the top of the combo box.
    int i = m_ComboFilename.FindString(-1, path);
    if (i > CB_ERR)
        m_ComboFilename.DeleteString(i);
    m_ComboFilename.InsertString(0, path);
    m_ComboFilename.SetCurSel(0);
}

void CRecPlaybackDlg::SavePathToRegistry(LPCTSTR path)
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
        key.Format("CapFile%d", i);
        SetRegEntryStr("CIGI Host Emulator 4", (LPCTSTR)key, m_PathArray[i]);
    }
}

void CRecPlaybackDlg::ReadPathsFromRegistry(void)
{
    for (int i = 9; i >= 0; i--) {
        CString key;
        key.Format("CapFile%d", i);
        if (GetRegEntryStr("CIGI Host Emulator 4", (LPCTSTR)key, &m_PathArray[i]) == ERROR_SUCCESS) {
            if (m_PathArray[i] != "")
                m_ComboFilename.InsertString(0, m_PathArray[i]);
        } else {
            m_PathArray[i] = "";
        }
    }

    PackPathArray();
}

int CRecPlaybackDlg::FindPathInArray(LPCTSTR path)
{
    for (int i = 0; i < 10; i++)
        if (m_PathArray[i] == path)
            return i;

    return -1;
}

void CRecPlaybackDlg::PackPathArray(void)
{
    for (int i = 0; i < 9; i++) {
        if (m_PathArray[0] == "") {
            m_PathArray[i] = m_PathArray[i + 1];
            m_PathArray[i + 1] = "";
        }
    }
}

void CRecPlaybackDlg::DisplayProgressRange(void)
{
    int TotalSecs = 0;

    // Round up just a hair to make things "look" more accurate.
    if (g_AvgFrameRate)
        TotalSecs = (int)((m_TotalFrames + (g_AvgFrameRate / 3.0)) / g_AvgFrameRate);

    int hour = TotalSecs / 3600;
    int min = (TotalSecs - (hour * 3600)) / 60;
    int sec = TotalSecs - (hour * 3600) - (min * 60);
    CString str;

    str.Format("%02d:%02d:%02d", hour, min, sec);
    m_EndTime.SetWindowText((LPCTSTR)str);

    m_ProgressMeter.SetRange32(0, m_TotalFrames);
}

int CRecPlaybackDlg::GetCaptureFileLength(HANDLE hfile)
{
    unsigned int framecount = 0;
    unsigned int msglength = 0;
    OVERLAPPED offset = {0};

    while (ReadFile(hfile, &msglength, sizeof(msglength), NULL, &offset)) {
        // Move the read position to the end of the message
        offset.Offset += sizeof(msglength) + msglength;
        framecount++;
    }

    return framecount;
}

void CRecPlaybackDlg::IncrementPlaybackProgress(void)
{
    if (g_RecPlaybackState != RECPLAYBACK_STATE_PLAY)
        return;

    if (m_ProgressCtr > m_TotalFrames)
        return;

    m_ProgressCtr++;

    m_ProgressMeter.SetPos(m_ProgressCtr);

    CString str;
    int ProgressSecs = 0;

    // Round up just a hair to make things "look" more accurate.
    if (g_AvgFrameRate)
        ProgressSecs = (int)((m_ProgressCtr + (g_AvgFrameRate / 3.0)) / g_AvgFrameRate);

    int hour = ProgressSecs / 3600;
    int min = (ProgressSecs - (hour * 3600)) / 60;
    int sec = ProgressSecs - (hour * 3600) - (min * 60);

    str.Format("%02d:%02d:%02d", hour, min, sec);
    m_CurrentTime.SetWindowText((LPCTSTR)str);
}

void CRecPlaybackDlg::ResetPlaybackProgress(void)
{
    m_ProgressCtr = 0;

    m_CurrentTime.SetWindowText("00:00:00");
    m_ProgressMeter.SetPos(0);
}

void CRecPlaybackDlg::FinishPlaybackProgress(void)
{
    CString str;
    m_TotalFrames = m_ProgressCtr;

    int TotalSecs = 0;

    // Round up just a hair to make things "look" more accurate.
    if (g_AvgFrameRate)
        TotalSecs = (int)((m_TotalFrames + (g_AvgFrameRate / 3.0)) / g_AvgFrameRate);

    int hour = TotalSecs / 3600;
    int min = (TotalSecs - (hour * 3600)) / 60;
    int sec = TotalSecs - (hour * 3600) - (min * 60);

    str.Format("%02d:%02d:%02d", hour, min, sec);
    m_EndTime.SetWindowText((LPCTSTR)str);
    m_ProgressMeter.SetRange32(0, m_TotalFrames);
    m_ProgressMeter.SetPos(0);

    m_ComboFilename.EnableWindow(TRUE);
}

void CRecPlaybackDlg::IncrementCaptureProgress(void)
{
    if (g_RecPlaybackState != RECPLAYBACK_STATE_RECORD)
        return;

    m_ProgressCtr++;

    CString str;
    int ProgressSecs = 0;
    if (g_AvgFrameRate)
        ProgressSecs = m_ProgressCtr / g_AvgFrameRate;
    int hour = ProgressSecs / 3600;
    int min = (ProgressSecs - (hour * 3600)) / 60;
    int sec = ProgressSecs - (hour * 3600) - (min * 60);
    str.Format("%02d:%02d:%02d", hour, min, sec);
    m_CurrentTime.SetWindowText((LPCTSTR)str);
    m_EndTime.SetWindowText((LPCTSTR)str);
}

void CRecPlaybackDlg::ResetCaptureProgress(void)
{
    m_ProgressCtr = 0;

    m_CurrentTime.SetWindowText("00:00:00");
    m_EndTime.SetWindowText("00:00:00");
    m_ProgressMeter.SetPos(0);
}

void CRecPlaybackDlg::FinishCaptureProgress(void)
{
    CString str;
    m_TotalFrames = m_ProgressCtr;
    int TotalSecs = 0;

    // Round up just a hair to make things "look" more accurate.
    if (g_AvgFrameRate)
        TotalSecs = (int)((m_TotalFrames + (g_AvgFrameRate / 3.0)) / g_AvgFrameRate);

    int hour = TotalSecs / 3600;
    int min = (TotalSecs - (hour * 3600)) / 60;
    int sec = TotalSecs - (hour * 3600) - (min * 60);

    str.Format("%02d:%02d:%02d", hour, min, sec);
    m_EndTime.SetWindowText((LPCTSTR)str);
    m_ProgressMeter.SetRange32(0, m_TotalFrames);

    m_ComboFilename.EnableWindow(TRUE);
}
