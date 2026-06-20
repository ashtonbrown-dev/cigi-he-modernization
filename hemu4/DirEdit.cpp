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

#include "DirEdit.h"
#include "DlgWnd.h"

// CDirEdit control class
BEGIN_MESSAGE_MAP(CDirEdit, CEdit)
    ON_WM_CHAR()
    ON_WM_KEYDOWN()
    ON_WM_GETDLGCODE()
END_MESSAGE_MAP()

void CDirEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CDlgWnd *pp;                           // Parent = the dialog itself
    VERIFY(pp = (CDlgWnd *)GetParent());

    if (nChar == '\t') {
        // Because we are getting all keys (see OnGetDlgCode()) so that we can get the Return key,
        // we also get the tab key as a side-effect.  This means that the tabbing between controls
        // in the dialog will stop at the edit control unless we force it to go to the next control.
        CWnd *pWnd = pp->GetDlgItem(IDC_OPEN);
        ASSERT(pWnd != NULL);
        pWnd->SetFocus();                       // Set focus to Open button
    } else if (nChar == '\r' || nChar == '\n') {
        // If return key is pressed we change to the directory specified OR
        // if the directory name appears valid but does not exist we ask the
        // user if they want to create it.  Note that the string is not
        // validated (although some validation may be done by Windows
        // via the CheckDir() call).  The name is only checked to see if
        // it is possible that a directory needs to be created.
        // Full validation is deferred till the "Open" button is clicked.

        CString ss;
        GetWindowText(ss);
        int len = ss.GetLength();

        // Remove trailing backslash unless root directory or network root
        if (strcmp(ss, "\\") != 0 && strcmp(ss, "\\\\") != 0 && strcmp((const char *)ss + 1, ":\\") != 0 &&
            len > 0 && ss[len - 1] == '\\') {
            ss = ss.Left(--len);
        }

        if (len == 0 ||
            len == 1 && ss[0] == '\\' ||
            len >= 2 && ss[0] == '\\' && ss[1] == '\\' && strchr((const char *)ss + 2, '\\') == NULL ||
            len == 2 && ss[1] == ':' ||
            len == 3 && ss[1] == ':' && ss[2] == '\\') {
            // Definitely not a createable directory
            pp->CheckDir(ss);
        } else {
            // Check if it's an existing directory
            CFileStatus fs;

            DWORD attr = GetFileAttributes(ss);
            if (attr == 0xFFFFFFFF) {
                // Directory not found but maybe it's an invalid drive
                _TCHAR rootdir[4] = _T("?:\\");
                rootdir[0] = ss[0];

                if (len == 1 || (len > 1 && ss[1] != ':') ||
                    GetDriveType(rootdir) > DRIVE_NO_ROOT_DIR) {
                    // Appears to be a valid drive (or relative path)
                    CString mess(ss);
                    mess += _T("\nThis folder does not exist.\n\n"
                               "Do you want to create it?");
                    if (AfxMessageBox(mess, MB_YESNO) == IDYES) {
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
                                AfxMessageBox(_T("You do not have permission or\n"
                                                 "otherwise cannot create this folder."));
                                break;
                            }
                            return;
                        }
                    } else
                        return;
                }
            }
            pp->CheckDir(ss);
            // Make sure the directory name ends with backslash so user can type sub-drectory name
            GetWindowText(ss);
            if (ss[ss.GetLength() - 1] != '\\') {
                ss += "\\";
                SetWindowText(ss);
            }
            SetSel(ss.GetLength(), -1);
        }
        SetFocus();                         // Make sure caret stays in this edit control
    } else {
        CEdit::OnChar(nChar, nRepCnt, nFlags);

        // Get the text and check whether it is a valid directory
        CString ss;                         // Current text in the edit control
        GetWindowText(ss);

        int len = ss.GetLength();
        int start, end;                     // Current selection
        GetSel(start, end);

        if (ss.Compare(_T("\\\\")) == 0) {
            // Don't check \\ else we get a message about "\\" being an invalid filename
            ;
        } else if (ss.Compare(_T("\\")) == 0) {
            // Show root directory
            pp->CheckDir(ss);
        } else if (len == 3 && ss[1] == ':' && ss[2] == '\\') {
            // Check that it's a valid drive
            if (GetDriveType(ss) > DRIVE_NO_ROOT_DIR) {
                pp->CheckDir(ss);
            }
        } else if (len > 0 && ss[len - 1] == '\\') {
            // Check that it's a valid directory
            // xxx does not handle "\\anwar\"
            DWORD attr = GetFileAttributes(ss);
            if (attr != 0xFFFFFFFF && (attr & FILE_ATTRIBUTE_DIRECTORY) != 0) {
                pp->CheckDir(ss);
            }
        } else if (start == len && nChar != '\b') {
            // Try to do completion of the directory name
            CFileFind ff;                   // Used to find directory names that start with ss
            int count = 0;                  // Number of matching directory names
            CString strMatch;               // The last directory found that matches

            BOOL bContinue = ff.FindFile(ss + "*");

            while (bContinue) {
                // At least one match - check them all
                bContinue = ff.FindNextFile();

                if (ff.IsDirectory()) {
                    // Found a matching directory
                    ++count;
                    strMatch = ff.GetFileName();
                }
            }

            // If there was exactly one matching directory use it
            if (count == 1) {
                int ii;
                // The file open dialog changes all uppercase names to lower case with an initial
                // capital (eg WINDOWS displays as Windows).  We do the same so things look nicer.
                for (ii = 0; ii < strMatch.GetLength(); ++ii) {
                    // Don't change if it contains spaces or lowercase letters
                    if (isspace(strMatch[ii]) || islower(strMatch[ii]))
                        break;
                }

                ASSERT(ii <= strMatch.GetLength());
                if (!strMatch.IsEmpty() && ii == strMatch.GetLength()) {
                    CString temp = strMatch.Mid(1);
                    temp.MakeLower();
                    strMatch = strMatch.Left(1) + temp;
                }


                // Get the bit of the directory name that the user has not yet typed
                int lb_len;             // Length of last bit (after \ or :)
                lb_len = ss.ReverseFind('\\');
                if (lb_len == -1) lb_len = ss.ReverseFind('/');
                if (lb_len == -1) lb_len = ss.ReverseFind(':');
                if (lb_len == -1)
                    lb_len = ss.GetLength();
                else
                    lb_len = ss.GetLength() - (lb_len + 1);

                // Check if the last char is the same case as the same char in the matched name
                if (!ss.IsEmpty() && lb_len > 0 && strMatch[lb_len - 1] != ss[ss.GetLength() - 1]) {
                    // The user used different case to that of the corresponding character in
                    // the matched directory so change the matched name to be the user's case.
                    if (isupper(ss[ss.GetLength() - 1]))
                        strMatch.MakeUpper();
                    else
                        strMatch.MakeLower();
                }

#ifdef _DEBUG
                CString temp = strMatch.Left(lb_len);
                ASSERT(temp.CompareNoCase(ss.Right(lb_len)) == 0);
#endif
                end += strMatch.GetLength() - lb_len;
                SetWindowText(ss + strMatch.Mid(lb_len));
                SetSel(start, end);
            }

            // else if (count > 1) pop-up some sort of selection list???
        }
        SetFocus();                         // Make sure caret stays in this edit control
    }
}

void CDirEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);

    if (nChar != VK_DELETE)
        return;

    CDlgWnd *pp;                           // Parent = the dialog itself
    VERIFY(pp = (CDlgWnd *)GetParent());

    // Get the current text and check whether it is a valid directory
    CString ss;
    GetWindowText(ss);
    int len = ss.GetLength();

    if (ss.Compare(_T("\\\\")) == 0) {
        // Don't check \\ else we get a message about "\\" being an invalid filename
        ;
    } else if (ss.Compare(_T("\\")) == 0) {
        // Show root directory
        pp->CheckDir(ss);
    } else if (len == 3 && ss[1] == ':' && ss[2] == '\\') {
        // Check that it's a valid drive
        if (GetDriveType(ss) > DRIVE_NO_ROOT_DIR) {
            pp->CheckDir(ss);
        }
    } else if (len > 0 && ss[len - 1] == '\\') {
        // Check that it's a valid directory
        DWORD attr = GetFileAttributes(ss);
        if (attr != 0xFFFFFFFF && (attr & FILE_ATTRIBUTE_DIRECTORY) != 0) {
            pp->CheckDir(ss);
        }
    }
    SetFocus();                         // Make sure caret stays in this edit control
}

UINT CDirEdit::OnGetDlgCode()
{
    // Get all keys so that we see CR
    return CEdit::OnGetDlgCode() | DLGC_WANTALLKEYS;
}
