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
 *  FILENAME:   ConfigDlg.cpp
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
#include "ConfigDlg.h"
#include "globals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog

CConfigDlg::CConfigDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CConfigDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CConfigDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CConfigDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CConfigDlg)
    DDX_Control(pDX, IDC_TAB1, m_Tab);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
    //{{AFX_MSG_MAP(CConfigDlg)
    ON_WM_SIZE()
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab)
    ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnSelchangingTab)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

BOOL CConfigDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    InitTabs();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlg::InitTabs(void)
{
    // Add a tab for each of the child dialog boxes.
    m_Tab.InsertItem(0, "Entities");
    m_Tab.InsertItem(1, "Environment");
    m_Tab.InsertItem(2, "Terrain");
    m_Tab.InsertItem(3, "Views");

    // Lock the resources for the child dialog boxes.
    m_DlgTempl[0] = LockDlgRes(MAKEINTRESOURCE(IDD_BLANK_STATE_VIEW));
    m_DlgTempl[1] = LockDlgRes(MAKEINTRESOURCE(IDD_BLANK_STATE_VIEW));
    m_DlgTempl[2] = LockDlgRes(MAKEINTRESOURCE(IDD_BLANK_STATE_VIEW));
    m_DlgTempl[3] = LockDlgRes(MAKEINTRESOURCE(IDD_BLANK_STATE_VIEW));

    // Create an array of pointers to the child dialog boxes.
    m_TabDlg[0] = (CDlgConfigBase *)&m_DlgEntities;
    m_TabDlg[1] = (CDlgConfigBase *)&m_DlgEnv;
    m_TabDlg[2] = (CDlgConfigBase *)&m_DlgTerrain;
    m_TabDlg[3] = (CDlgConfigBase *)&m_DlgViews;

    // Create child dialog boxes.
    int tabcount = m_Tab.GetItemCount();
    ASSERT(tabcount == NUM_CONFIGTAB_PAGES);

    for (int i = 0; i < tabcount; i++) {
        m_TabDlg[i]->CreateIndirect(m_DlgTempl[i], &m_Tab);
        m_TabDlg[i]->SetWindowPos(NULL, 1, 1, 0, 0, SWP_NOSIZE);
    }

    // Simulate selection of the first item.
    LRESULT result;
    OnSelchangeTab(NULL, &result);
}

void CConfigDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if (m_Tab.GetSafeHwnd()) {
        ((CSplitterWnd *)GetParent())->RecalcLayout();
        m_Tab.MoveWindow(0, 0, cx, cy);

        // Resize each of the tab pages.
        int tabcount = m_Tab.GetItemCount();

        ASSERT(tabcount == NUM_CONFIGTAB_PAGES);
        CRect tabrect, itemrect;
        m_Tab.GetClientRect(&tabrect);

        if (m_Tab.GetItemRect(0, &itemrect))
            tabrect.bottom -= itemrect.Height();

        for (int i = 0; i < tabcount; i++) {
            m_TabDlg[i]->MoveWindow(&tabrect);  // don't care about parameters
            m_TabDlg[i]->PostMessage(WM_SIZE, SIZE_RESTORED, MAKELPARAM(cx, cy));
        }
    }
}

void CConfigDlg::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Show the appropriate window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_SHOW);
    }

    *pResult = 0;
}

void CConfigDlg::OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Hide the current window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_HIDE);
    }

    *pResult = 0;
}
