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

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <Dlgs.h>           // For file dialog control IDs
#include <Imagehlp.h>       // For ::MakeSureDirectoryPathExists()

#include "DlgWnd.h"

// Class CDlgWnd
BEGIN_MESSAGE_MAP(CDlgWnd, CWnd)
    ON_BN_CLICKED(IDC_OPEN, OnOpen)
END_MESSAGE_MAP()

void CDlgWnd::OnOpen()
{
    // Get the text and check whether it is a valid directory
    CString ss;
    CEdit *pEdit = (CEdit *)GetDlgItem(IDC_DIR);
    ASSERT(pEdit != NULL);
    pEdit->GetWindowText(ss);
    int len = ss.GetLength();

    if (len == 2 && ss[0] == '\\' && ss[1] == '\\') {
        AfxMessageBox(ss + _T("\nThis is not a valid folder."));
        pEdit->SetFocus();
        return;
    } else if (len == 0 || len == 1 && ss[0] == '\\') {
        // Current directory or root of the current drive (therefore must be valid)
        ;
    } else if ((len == 2 && ss[1] == ':') ||
               (len == 3 && ss[1] == ':' && ss[2] == '\\')) {
        _TCHAR rootdir[4] = _T("?:\\");
        rootdir[0] = ss[0];

        if (GetDriveType(rootdir) <= DRIVE_NO_ROOT_DIR) {
            AfxMessageBox(ss + _T("\nThe drive is invalid."));
            pEdit->SetFocus();
            return;
        }
    } else {
        // Check that it's a valid directory
        if (ss[len - 1] == '\\')
            ss = ss.Left(--len);
        DWORD attr = GetFileAttributes(ss);
        if (attr == 0xFFFFFFFF) {
            const char *ss2;

            // Directory not found but maybe it's an invalid drive
            _TCHAR rootdir[4] = _T("?:\\");
            rootdir[0] = ss[0];

            if (len > 1 && ss[1] == ':' && GetDriveType(rootdir) <= DRIVE_NO_ROOT_DIR) {
                AfxMessageBox(ss + _T("\nThe drive is invalid."));
                pEdit->SetFocus();
                return;
            } else if (len >= 2 && ss[0] == '\\' && ss[1] == '\\' &&
                       ((ss2 = strchr((const char *)ss + 2, '\\')) == NULL || strchr(ss2 + 1, '\\') == NULL)) {
                AfxMessageBox(ss + _T("\nThis is not a valid folder."));
                pEdit->SetFocus();
                return;
            } else {
                // Appears to be a valid drive (or relative path)
                CString mess(ss);
                mess += _T("\nThis folder does not exist.\n\n"
                           "Do you want to create it?");
                if (AfxMessageBox(mess, MB_YESNO) == IDYES) {
                    // MakeSureDirectoryPathExists is not part of Windows but is
                    // in the IMAGHLP.DLL which is always present.  This call
                    // requires linking with IMAGHLP.LIB.
                    if (!::MakeSureDirectoryPathExists(ss + _T("\\"))) {
                        switch (GetDriveType(rootdir)) {
                        case DRIVE_CDROM:
                            AfxMessageBox(_T("You cannot create this folder\n"
                                             "as the CD ROM medium is read-only."));
                            break;
                        case DRIVE_REMOVABLE:
                            AfxMessageBox(_T("You cannot create this folder.\n"
                                             "The medium may be write-protected."));
                            break;
                        case DRIVE_REMOTE:
                            AfxMessageBox(_T("You do not have permission to create\n"
                                             "this folder on the network."));
                            break;
                        default:
                            AfxMessageBox(_T("You do not have permission\n"
                                             "to create this folder."));
                            break;
                        }
                        pEdit->SetFocus();
                        return;         // Directory could not be created
                    }
                    // directory was created, so continue
                } else {
                    pEdit->SetFocus();
                    return;             // User did not want to create directory
                }
            }
        } else if ((attr & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            AfxMessageBox(ss + _T("\nThis is a file not a directory."));
            pEdit->SetFocus();
            return;
        }
    }

    // We have now selected a directory and will return from the dialog
    CheckDir(ss);

    ::EndDialog(m_hWnd, IDOK);
}

// This routine updates the directory/file list display using the directory
// name given.  It does this by putting the name in the (hidden) edit control
// and simulating a press of the (hidden) IDOK button.  If the directory is
// invalid in some way the currently displayed list will not be changed and
// some sort of error message may be displayed.
void CDlgWnd::CheckDir(const CString &ss)
{
    // Put the new directory into the old (hidden) edit box
    CEdit *pOld = (CEdit *)GetDlgItem(cmb13);   // edt1
    ASSERT(pOld != NULL);
    pOld->SetWindowText(ss);

    // Save the current text/selection in the edit control
    CString strSaved;                       // Current text in edit box
    int start, end;                         // Current selection in edit box
    CEdit *pEdit = (CEdit *)GetDlgItem(IDC_DIR);
    ASSERT(pEdit != NULL);
    pEdit->GetWindowText(strSaved);
    pEdit->GetSel(start, end);

    CWnd *pOK = GetDlgItem(IDOK);
    pOK->SendMessage(WM_LBUTTONDOWN);
    pOK->SendMessage(WM_LBUTTONUP);

    CString strNew;
    pEdit->GetWindowText(strNew);

    // Usually we want to keep what the user has typed (strSaved) rather than what has been
    // put in the edit control due to OnFolderChange.  One exception is if the user has
    // used "..", "..." etc to change to an ancestor directory in which case we don't want to
    // leave this the same as it will cause repeated changes to ancestor directories whenever
    // the user types backslash (\).  Also don;t set the edit string back to what the user
    // typed if it would be empty or unchanged except for case (as the case probably looks
    // better the way it was filled in).
    if (strSaved.IsEmpty() || strSaved[0] == '.' ||
        strNew.CompareNoCase(strSaved) == 0 || strNew.CompareNoCase(strSaved + '\\') == 0) {
        pEdit->SetSel(strNew.GetLength(), -1);
    } else {
        // Restore the edit control the way the user typed it
        pEdit->SetWindowText(strSaved);
        pEdit->SetSel(start, end);
    }
}
