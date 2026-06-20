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

// OutputView.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "OutputView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputView

IMPLEMENT_DYNCREATE(COutputView, CFormView)

COutputView::COutputView()
    : CFormView(COutputView::IDD)
{
    //{{AFX_DATA_INIT(COutputView)
    //}}AFX_DATA_INIT

    m_Initialized = FALSE;
}

COutputView::~COutputView()
{
}

void COutputView::DoDataExchange(CDataExchange *pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COutputView)
    DDX_Control(pDX, IDC_TAB1, m_Tab);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COutputView, CFormView)
    //{{AFX_MSG_MAP(COutputView)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab)
    ON_WM_SIZE()
    ON_WM_CREATE()
    ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnSelchangingTab)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputView diagnostics

#ifdef _DEBUG
void COutputView::AssertValid() const
{
    CFormView::AssertValid();
}

void COutputView::Dump(CDumpContext &dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COutputView message handlers

void COutputView::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);

    if (m_Tab.GetSafeHwnd()) {
        ((CSplitterWnd *)GetParent())->RecalcLayout();
        m_Tab.MoveWindow(0, 0, cx, cy);

        // Resize each of the tab pages.
        int tabcount = m_Tab.GetItemCount();

        ASSERT(tabcount == NUM_OUTPUTTAB_PAGES);

        for (int i = 0; i < tabcount; i++) {
            CRect tabrect, itemrect;
            m_Tab.GetClientRect(&tabrect);
            if (m_Tab.GetItemRect(0, &itemrect))
                tabrect.bottom -= itemrect.Height();
            m_TabDlg[i]->MoveWindow(&tabrect);  // don't care about parameters
        }
    }
}

int COutputView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFormView::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}

void COutputView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    if (m_Initialized)
        return;

    InitTabs();

    m_Initialized = TRUE;
}

void COutputView::InitTabs(void)
{
    // Add a tab for each of the child dialog boxes.
    m_Tab.InsertItem(0, "Messages");
    m_Tab.InsertItem(1, "Capture");

    // Lock the resources for the child dialog boxes.
    m_DlgTempl[0] = LockDlgRes(MAKEINTRESOURCE(IDD_MESSAGE_VIEW));
    m_DlgTempl[1] = LockDlgRes(MAKEINTRESOURCE(IDD_WATCH_VIEW));

    // Create an array of pointers to the child dialog boxes.
    m_TabDlg[0] = (CDialog *)&m_DlgMessages;
    m_TabDlg[1] = (CDialog *)&m_DlgWatch;

    // Create child dialog boxes.
    int tabcount = m_Tab.GetItemCount();
    ASSERT(tabcount == NUM_OUTPUTTAB_PAGES);

    for (int i = 0; i < tabcount; i++)
        m_TabDlg[i]->CreateIndirect(m_DlgTempl[i], &m_Tab);

    for (int i = 0; i < tabcount; i++)
        m_TabDlg[i]->SetWindowPos(NULL, 1, 1, 0, 0, SWP_NOSIZE);

    // Simulate selection of the first item.
    LRESULT result;
    OnSelchangeTab(NULL, &result);
}

void COutputView::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Show the appropriate window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_SHOW);
    }

    *pResult = 0;
}

void COutputView::OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Hide the current window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_HIDE);
    }

    *pResult = 0;
}
