/** <pre>
 *  Copyright 2004 The Boeing Company
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
 *
 *  FILENAME:   DlgConfigViews.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface Host Emulator (CIGI HE)
 *
 *  PROGRAM DESCRIPTION:
 *  The Host Emulator (HE) acts as a surrogate in the absence of a simulation host.
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME
 *  DESCRIPTION OF CHANGE
 *
 *  11/11/2004 Lance Durham
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  Version 3.1.1
 */

#include "stdafx.h"
#include "DlgConfigViews.h"
#include "globals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConfigViews dialog

void CDlgConfigViews::OnOK()
{
}

void CDlgConfigViews::OnCancel()
{
}

CDlgConfigViews::CDlgConfigViews(CWnd *pParent /*=NULL*/)
    : CDlgConfigBase(CDlgConfigViews::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgConfigViews)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CDlgConfigViews::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgConfigViews)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgConfigViews, CDialog)
    //{{AFX_MSG_MAP(CDlgConfigViews)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConfigViews message handlers


void CDlgConfigViews::ClearData(void)
{
}

int CDlgConfigViews::LoadData(CString IN &filename)
{
    CStdioFile file;
    if (!file.Open((LPCTSTR)filename, CFile::modeRead | CFile::typeText)) {
        CString errmsg;
        errmsg.Format("Cannot open the file \"%s\"", (LPCTSTR)filename);
        MessageBox((LPCTSTR)errmsg, "File Error", MB_ICONEXCLAMATION);
        return 0;
    }

    SetFilenameForConfigErrorMessages((LPCTSTR) filename);

    g_DataManager.LoadViewGroupTemplates(file);
    g_DataManager.LoadViewTemplates(file);

    return 0;
}

int CDlgConfigViews::SaveData(CString IN &filename)
{
    CStdioFile file;
    if (!file.Open((LPCTSTR)filename, CFile::modeCreate | CFile::modeWrite | CFile::typeText)) {
        CString errmsg;
        errmsg.Format("Cannot open the file \"%s\"", (LPCTSTR)filename);
        MessageBox((LPCTSTR)errmsg, "File Error", MB_ICONEXCLAMATION);
        return 0;
    }

    g_DataManager.WriteViewGroupTemplates(file);
    g_DataManager.WriteViewTemplates(file);

    return 0;
}