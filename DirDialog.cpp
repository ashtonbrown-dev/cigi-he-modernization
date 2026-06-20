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

// DirDialog.cpp - implements class CDirDialog

// The CDirDialog class implements a directory selection dialog by deriving from the file
// selection dialog class (CFileDialog).  This dialog has the advantages of the standard
// file open dialog (resizeable, ability to create/delete directories etc) but is
// customized for the selection of a directory rather than a file.
//
// USER INTERFACE
// --------------
//
// For example, the user can double click on a directory name or type it into the
// edit control to open it in the directory listing display.  To say that this is
// the directory they want the user must click the "Open" button.  When the user
// enters a backslash (\) as the last character in the edit control the display is
// changed to show the contents of the directory if it exists.  The same is done if
// the user presses the Enter key except that if the directory does not exist the
// user is asked if they want to create it.
//
// When the user enters other characters and the contents don't end in a backslash
// then automatic directory name completion is attempted.  If the contents of the
// edit box are the first character(s) of one unique existing directory then the
// rest of the directory name is added to the end of the edit box.  These characters
// are selected so that the user can type something else and it they will be ignored.
//
// When selecting a directory you would normally not want to see files, but you may on
// occasion.  The normal "Files of Type:" drop down list is available but it has an
// extra entry "Show Folders Only" that is selected by default.  When files are
// displayed double clicking of them is ignored (unlike the normal File Open dialog).
// Double-clicking a directory name changes to that directory as normal.
//
// PROGRAMMER INTERFACE
// --------------------
//
// Add DirDialog.cpp and DirDialog.h to your project.  Include DirDialog.h
// in the source file(s) where you want to use the CDirDialog class.
//
// Create a CDirDialog object using the constructor described below.  If necessary
// you may then modify values in the m_ofn member of the CFileDialog base class
// (see the Win32 documentation for OPENFILENAME).  For example, to change the
// text that appears in the title bar of the dialog use m_ofn.lpstrTitle.
//
// Call DoModal() to activate the dialog.  If DoModal() return IDOK you can then
// call GetPath() to obtain the name of the directory that the user selected.
//
//
// CDirDialog::CDirDialog(LPCTSTR lpstrInitialDir = NULL,
//                        LPCTSTR lpszFilter = NULL, CWnd* pParentWnd = NULL);
//
//      lpstrInitialDir
//          The initial directory.  If NULL then the current directory is used.
//          See lpstrInitialDir in the Win32 documentation for OPENFILENAME for more info.
//
//      lpszFilter
//          The string pairs that specify the file filters to use.  See lpszFilter
//          in the documentation for the CFileDialog constructor for more info.
//          Note that an extra entry is always added that allows the user to hide the
//          display of all files.  If NULL is used (the default) then only the
//          "no files" entry and an "all files" entry are provided.
//
//      pParentWnd
//          A pointer to the dialog parent window.
//
// virtual int CDirDialog::DoModal()
//
//      see CFileDialog::DoModal().
//
// CString CDirDialog::GetPath()
//
//      return value
//          The full path name chosen by the user.
//
// Example:
//
//      // Called when the Browse button is clicked in CMyDialog
//      void CMyDialog::OnBrowseDir()
//      {
//          if (!UpdateData(TRUE))          // Update current value of m_dir from control
//              return;
//
//          CDirDialog dlg(m_dir,
//                        "JPEG Files (*.jpg; *.jpeg)|*.jpg;*.jpeg|All Files (*.*)|*.*||",
//                        this);
//          dlg.m_ofn.lpstrTitle = "Select Folder to Store Images";
//
//          if (dlg.DoModal() == IDOK)
//          {
//              m_dir = dlg.GetPath();      // Store selected directory name back into the control
//              UpdateData(FALSE);
//          }
//      }
//
//
// INTERNAL DESIGN
// ---------------
//
// LWD: Note that as of VS .NET 7.1, the control IDs for the File Open dialog
//      have changed somewhat.  The ID for the edt1 control shows up as
//      0x047C in Spy++, but this is defined as cmb13 in dlgs.h.  This file
//      has been updated accordingly.
//
// The following changes are made to the controls in the standard file open dialog:
//
// The "Open" button is hidden and replaced with another button (IDC_OPEN).
// The normal edit control (edt1) where the file name is entered is hidden and replaced
// by a "subclassed" edit control (IDC_DIR) of class CDirEdit (derived from CEdit).
// By hiding and replacing these controls we can manipulate the behaviour
// of the dialog in ways not provided for in any other way.  For example, by changing
// the contents of the hidden edit control (edt1) and simulating a click of the hidden
// Open button (IDOK) we can force the contents of a directory to be displayed.
//
// An extra entry is added to the file types drop down combo called "Show Folders Only"
// that causes no files to be displayed.  (If no filters are supplied at all by using
// the default value of NULL, then an "All Files" filter type is also added.)
// The filter string is a single dot (full-stop) which will match no files.
//
// The new edit control (IDC_DIR) is subclassed so that the contents are monitored and
// the some keys can be intercepted.  When the contents are changed and they end with
// a backslash the current display is changed to point to the directory entered (if it
// exists).  When return is pressed the directory is also changed, but if it doesn't
// exist then the user is asked if he wants to create it.  The way the directory is
// changed (ie. the files of that directory are shown in the display) is by putting the
// directory name into the original edit control (edt1) and simulating a click of the
// original Open button (IDOK).  Directory name completion is also performed as the
// user types in a directory name.
//
// The IDC_OPEN button is used as a replacement for the IDOK button while still allowing
// the hidden IDOK button to be used to change the displayed directory.
//
// The CDirDialog class is derived from CFileDialog. The following base class members
// are overridden:
// - OnInitDone: so that the dialog controls can be reorganized
// - OnFolderChange: so that when the user clicks on a folder name the edit control can
//   be updated to reflect the name of the currently selected directory
// - OnFileNameOK: always returns TRUE so that the user can't select files by
//   double-clicking them (this is a DIRECTORY selection dialog after all)

//#include "stdafx.h"

// If you don't want this as part of your project (eg to put in a library) remove
// the above #include "stdafx.h" and uncomment the following 3 lines:
#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <Dlgs.h>           // For file dialog control IDs
#include <Imagehlp.h>       // For ::MakeSureDirectoryPathExists()

#include "DirDialog.h"      // Our own header file

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Starting with Vista, CFileDialogs use Vista styling and functionality by
// default. This causes the HE to crash though, so we want to disable it via
// constructor parameter.
const BOOL vistaStyleDialog = FALSE;

// class CDirDialog
CDirDialog::CDirDialog(LPCTSTR initial, LPCTSTR filter, CWnd *pParentWnd)
    : CFileDialog(TRUE, NULL, NULL,
                  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
                  NULL, pParentWnd, 0, vistaStyleDialog),
    m_strPath(initial)
{
    // Note: m_strFilter is a member variable so it doesn't disappear because
    // it is used later internally by the file open dialog (via m_ofn.lpstrFilter).
    if (filter != NULL)
        m_strFilter = CString(_T("Show Folders Only|.|")) + filter;
    else
        m_strFilter = _T("Show Folders Only|.|All Files (*.*)|*.*||");
    m_strFilter.Replace('|', '\0');
    m_ofn.lpstrFilter = m_strFilter;

    m_ofn.lpstrInitialDir = initial;

    m_ofn.lpstrTitle = _T("Select Folder");
}

void CDirDialog::OnInitDone()
{
    CRect rct;                          // Used to move/resize controls
    CWnd *pp;                           // Parent = the dialog window itself
    VERIFY(pp = GetParent());

    ASSERT(pp->GetDlgItem(stc3) != NULL);
    pp->GetDlgItem(stc3)->SetWindowText(_T("Folder:"));

    // Create a new CDlgWnd so we can catch dialog control notifications
    VERIFY(m_DlgWnd.SubclassWindow(pp->m_hWnd));

    // Create a new edit control where edt1 now is
    ASSERT(pp->GetDlgItem(cmb13) != NULL);      // VC6 was edt1, VC7.1 is cmb13
    pp->GetDlgItem(cmb13)->GetWindowRect(rct); //Get edt1 rectangle
    pp->ScreenToClient(rct);

    VERIFY(m_Edit.Create(WS_TABSTOP | WS_VISIBLE | WS_CHILD,
                         rct, pp, IDC_DIR));
    if (m_ofn.lpstrInitialDir  != NULL)
        m_Edit.SetWindowText(m_ofn.lpstrInitialDir);
    m_Edit.SetFont(pp->GetDlgItem(cmb13)->GetFont());   // edt1
    m_Edit.ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);
    m_Edit.SetWindowPos(pp->GetDlgItem(stc3), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    CWnd *pCancel = pp->GetDlgItem(IDCANCEL);
    ASSERT(pCancel != NULL);

    // Create a new button where the OK button now is
    ASSERT(pp->GetDlgItem(IDOK) != NULL);
    pp->GetDlgItem(IDOK)->GetWindowRect(rct); //Get OK button rectangle
    pp->ScreenToClient(rct);

    m_Open.Create(_T("Select"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                  rct, pp, IDC_OPEN);
    m_Open.SetFont(pp->GetDlgItem(IDOK)->GetFont());
    m_Open.SetWindowPos(&m_Edit, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    pCancel->SetWindowPos(&m_Open, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    // Change default push button
    pp->GetDlgItem(IDOK)->ModifyStyle(BS_DEFPUSHBUTTON, 0);
    pp->SendMessage(DM_SETDEFID, IDC_OPEN);

#ifdef DIRDIALOG_TESTING
    // Move controls (rather than hide them) for testing

    // Increase size of dialog
    pp->GetWindowRect(rct);
    pp->SetWindowPos(NULL, 0, 0, rct.Width(), rct.Height() + 70, SWP_NOZORDER | SWP_NOMOVE);

    // Move the replaced controls down
    ASSERT(pp->GetDlgItem(IDOK) != NULL);
    pp->GetDlgItem(IDOK)->GetWindowRect(rct);
    pp->ScreenToClient(rct);
    pp->GetDlgItem(IDOK)->SetWindowPos(NULL, rct.left, rct.top + 70,
                                       0, 0, SWP_NOZORDER | SWP_NOSIZE);

    ASSERT(pp->GetDlgItem(cmb13) != NULL);  // edt1
    pp->GetDlgItem(cmb13)->GetWindowRect(rct);  // edt1
    pp->ScreenToClient(rct);
    pp->GetDlgItem(cmb13)->SetWindowPos(NULL, rct.left, rct.top + 70,
                                        0, 0, SWP_NOZORDER | SWP_NOSIZE);    // edt1

#else
    // Hide the controls we don't want the user to use
    HideControl(IDOK);
    HideControl(cmb13); // edt1
#endif

    CFileDialog::OnInitDone();
}

void CDirDialog::OnFolderChange()
{
    CWnd *pp;                           // Parent window = the dialog itself
    VERIFY(pp = GetParent());
    ASSERT(::IsWindow(pp->m_hWnd));

    ASSERT(pp->GetDlgItem(IDC_DIR) != NULL);
    m_strPath = GetFolderPath();
    int len = m_strPath.GetLength();
    if (len > 0 && m_strPath[len - 1] != '\\') {
        m_strPath += "\\";
        ++len;
    }
    pp->GetDlgItem(IDC_DIR)->SetWindowText(m_strPath);
    m_Edit.SetSel(len, len);

    CFileDialog::OnFolderChange();

    m_Edit.SetFocus();
}
