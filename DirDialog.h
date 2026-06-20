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

#if !defined(DIRDIALOG_INCLUDED_)
#define DIRDIALOG_INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "DirEdit.h"
#include "DlgWnd.h"

// CDirDialog - directory selection dialog
class CDirDialog : public CFileDialog
{
public:
    CDirDialog(LPCTSTR initial = NULL,
               LPCTSTR filter = NULL, CWnd *pParentWnd = NULL);

    CString GetPath() {
        return m_strPath;
    }

    // Overriden members of CFileDialog
    virtual void OnInitDone();
    virtual void OnFolderChange();

    // Disallow selection of files (since we're only selecting directories)
    virtual BOOL OnFileNameOK() {
        return TRUE;
    }

private:
    CString m_strPath;                  // Current directory
    CString m_strFilter;                // The current file filters string (used for string storage
    // for internal use of File Open dialog)

    CDlgWnd m_DlgWnd;                   // Subclassed dialog window (parent of CDirDialog window)
    CDirEdit m_Edit;                    // Edit control where you can type in a dir. name
    CButton m_Open;                     // "Open" button (replaces OK button)
};

#endif // DIRDIALOG_INCLUDED_
