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

// ObjectTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "ObjectTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectTreeView

IMPLEMENT_DYNCREATE(CObjectTreeView, CFormView)

CObjectTreeView::CObjectTreeView()
    : CFormView(CObjectTreeView::IDD)
{
    //{{AFX_DATA_INIT(CObjectTreeView)
    //}}AFX_DATA_INIT

    m_Initialized = FALSE;
}

CObjectTreeView::~CObjectTreeView()
{
}

void CObjectTreeView::DoDataExchange(CDataExchange *pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CObjectTreeView)
    DDX_Control(pDX, IDC_TAB_NAV, m_Tab);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CObjectTreeView, CFormView)
    //{{AFX_MSG_MAP(CObjectTreeView)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_NAV, OnSelchangeTab)
    ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_NAV, OnSelchangingTab)
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectTreeView diagnostics

#ifdef _DEBUG
void CObjectTreeView::AssertValid() const
{
    CFormView::AssertValid();
}

void CObjectTreeView::Dump(CDumpContext &dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CObjectTreeView message handlers

void CObjectTreeView::InitTabs(void)
{
    // Add a tab for each of the child dialog boxes.
    m_Tab.InsertItem(0, "Entities");
    m_Tab.InsertItem(1, "Environment");
    m_Tab.InsertItem(2, "Views");

    // Lock the resources for the child dialog boxes.
    m_DlgTempl[0] = LockDlgRes(MAKEINTRESOURCE(IDD_NAVTREEVIEW));
    m_DlgTempl[1] = LockDlgRes(MAKEINTRESOURCE(IDD_NAVTREEVIEW));
    m_DlgTempl[2] = LockDlgRes(MAKEINTRESOURCE(IDD_NAVTREEVIEW));

    // Create an array of pointers to the child dialog boxes.
    m_TabDlg[0] = (CDialog *)&m_DlgEntityTree;
    m_TabDlg[1] = (CDialog *)&m_DlgEnvTree;
    m_TabDlg[2] = (CDialog *)&m_DlgViewTree;

    // Create child dialog boxes.
    int tabcount = m_Tab.GetItemCount();
    ASSERT(tabcount == NUM_NAVTAB_PAGES);

    for (int i = 0; i < tabcount; i++) {
        m_TabDlg[i]->CreateIndirect(m_DlgTempl[i], &m_Tab);
        m_TabDlg[i]->SetWindowPos(NULL, 1, 1, 0, 0, SWP_NOSIZE);
    }

    // Simulate selection of the first item.
    LRESULT result;
    OnSelchangeTab(NULL, &result);
}

void CObjectTreeView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    if (m_Initialized)
        return;

    InitTabs();

    // Force a resize.
    CRect rect;
    GetWindowRect(&rect);
    SendMessage(WM_SIZE, SIZE_RESTORED, MAKELPARAM(rect.Width(), rect.Height()));

    m_Initialized = TRUE;
}

void CObjectTreeView::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);

    if (m_Tab.GetSafeHwnd()) {
        ((CSplitterWnd *)GetParent())->RecalcLayout();
        m_Tab.MoveWindow(0, 0, cx, cy);

        // Resize each of the tab pages.
        int tabcount = m_Tab.GetItemCount();
        ASSERT(tabcount == NUM_NAVTAB_PAGES);
        CRect tabrect, itemrect;
        m_Tab.GetClientRect(&tabrect);
        if (m_Tab.GetItemRect(0, &itemrect))
            tabrect.bottom -= itemrect.Height();
        int i;
        for (i = 0; i < tabcount; i++) {
            m_TabDlg[i]->MoveWindow(&tabrect);  // don't care about parameters
        }
    }
}

void CObjectTreeView::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Show the appropriate window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_SHOW);
    }

    *pResult = 0;
}

void CObjectTreeView::OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Hide the current window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_HIDE);
    }

    *pResult = 0;
}

void CObjectTreeView::SelectTab(const int index)
{
    // Simulate selection of the item.
    LRESULT result;
    OnSelchangingTab(NULL, &result);

    m_Tab.SetCurSel(index);

    OnSelchangeTab(NULL, &result);
}
