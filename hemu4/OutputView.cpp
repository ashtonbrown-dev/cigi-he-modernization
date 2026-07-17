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
#include "ExternalTabConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputView

IMPLEMENT_DYNCREATE(COutputView, CFormView)

COutputTabPage::COutputTabPage()
    : Window(NULL), ExternalPage(NULL), Owned(FALSE)
{
}

COutputView::COutputView()
    : CFormView(COutputView::IDD), m_MessageDlgTemplate(NULL),
      m_WatchDlgTemplate(NULL)
{
    //{{AFX_DATA_INIT(COutputView)
    //}}AFX_DATA_INIT

    m_Initialized = FALSE;
}

COutputView::~COutputView()
{
    for (int index = 0; index < m_TabPages.GetSize(); ++index) {
        COutputTabPage &page = m_TabPages[index];
        if (!page.Owned || page.Window == NULL)
            continue;

        if (page.Window->GetSafeHwnd())
            page.Window->DestroyWindow();
        delete page.Window;
        page.Window = NULL;
        page.ExternalPage = NULL;
    }
    m_TabPages.RemoveAll();
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
        ResizeTabPages();
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
    // Lock the resources for the child dialog boxes.
    m_MessageDlgTemplate = LockDlgRes(MAKEINTRESOURCE(IDD_MESSAGE_VIEW));
    m_WatchDlgTemplate = LockDlgRes(MAKEINTRESOURCE(IDD_WATCH_VIEW));

    // Keep the two built-in pages first and in their existing order.
    if (m_DlgMessages.CreateIndirect(m_MessageDlgTemplate, &m_Tab))
        AddTabPage(_T("Messages"), &m_DlgMessages, FALSE);
    if (m_DlgWatch.CreateIndirect(m_WatchDlgTemplate, &m_Tab))
        AddTabPage(_T("Capture"), &m_DlgWatch, FALSE);

    CExternalTabDefinitionArray definitions;
    CStringArray diagnostics;
    CString startupTabTitle;
    LoadExternalTabDefinitions(definitions, diagnostics, startupTabTitle);

    for (int diagnosticIndex = 0;
         diagnosticIndex < diagnostics.GetSize(); ++diagnosticIndex)
        PrintExternalTabDiagnostic(diagnostics[diagnosticIndex]);

    for (int definitionIndex = 0;
         definitionIndex < definitions.GetSize(); ++definitionIndex) {
        CExternalToolHostPage *externalPage =
            new CExternalToolHostPage;
        externalPage->Configure(definitions[definitionIndex]);
        if (!externalPage->Create(&m_Tab)) {
            CString diagnostic;
            diagnostic.Format(
                _T("External tabs: could not create the '%s' host page."),
                (LPCTSTR)definitions[definitionIndex].Title);
            PrintExternalTabDiagnostic(diagnostic);
            delete externalPage;
            continue;
        }

        AddTabPage(definitions[definitionIndex].Title, externalPage, TRUE,
                   externalPage);
    }

    ResizeTabPages();

    int startupTabIndex = FindTabPage(startupTabTitle);
    if (startupTabIndex < 0) {
        CString diagnostic;
        diagnostic.Format(
            _T("External tabs: StartupTab '%s' was not found; using Messages."),
            (LPCTSTR)startupTabTitle);
        PrintExternalTabDiagnostic(diagnostic);
        startupTabIndex = 0;
    }

    m_Tab.SetCurSel(startupTabIndex);
    LRESULT result;
    OnSelchangeTab(NULL, &result);
}

void COutputView::AddTabPage(LPCTSTR title, CWnd *window, BOOL owned,
                             CExternalToolHostPage *externalPage)
{
    if (window == NULL || !window->GetSafeHwnd())
        return;

    COutputTabPage page;
    page.Title = title;
    page.Window = window;
    page.ExternalPage = externalPage;
    page.Owned = owned;

    m_Tab.InsertItem(m_TabPages.GetSize(), title);
    m_TabPages.Add(page);
    window->SetWindowPos(NULL, 1, 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    window->ShowWindow(SW_HIDE);
}

int COutputView::FindTabPage(LPCTSTR title) const
{
    if (!title || !title[0])
        return -1;

    for (int index = 0; index < m_TabPages.GetSize(); ++index) {
        if (m_TabPages[index].Title.CompareNoCase(title) == 0)
            return index;
    }

    return -1;
}

void COutputView::ResizeTabPages(void)
{
    if (!m_Tab.GetSafeHwnd() || m_TabPages.GetSize() == 0)
        return;

    CRect tabRect;
    CRect itemRect;
    m_Tab.GetClientRect(&tabRect);
    if (m_Tab.GetItemRect(0, &itemRect))
        tabRect.bottom -= itemRect.Height();

    for (int index = 0; index < m_TabPages.GetSize(); ++index) {
        CWnd *window = m_TabPages[index].Window;
        if (window != NULL && window->GetSafeHwnd())
            window->MoveWindow(&tabRect);
    }
}

void COutputView::PrintExternalTabDiagnostic(LPCTSTR diagnostic)
{
    if (m_DlgMessages.GetSafeHwnd()
        && m_DlgMessages.m_Edit.GetSafeHwnd())
        m_DlgMessages.PrintMessage(diagnostic);
}

void COutputView::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Show the appropriate window.
    int index = m_Tab.GetCurSel();

    if (index >= 0 && index < m_TabPages.GetSize()) {
        COutputTabPage &page = m_TabPages[index];
        if (page.Window != NULL && page.Window->GetSafeHwnd())
            page.Window->ShowWindow(SW_SHOW);
        if (page.ExternalPage != NULL)
            page.ExternalPage->Activate();
    }

    *pResult = 0;
}

void COutputView::OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Hide the current window.
    int index = m_Tab.GetCurSel();

    if (index >= 0 && index < m_TabPages.GetSize()
        && m_TabPages[index].Window != NULL
        && m_TabPages[index].Window->GetSafeHwnd())
        m_TabPages[index].Window->ShowWindow(SW_HIDE);

    *pResult = 0;
}
