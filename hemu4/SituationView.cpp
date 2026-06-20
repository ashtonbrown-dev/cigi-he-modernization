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

// SituationView.cpp : implementation file
//

#include "stdafx.h"
#include "EntityTreeView.h"
#include "Hemu4.h"
#include "ObjectTreeView.h"
#include "SituationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSituationView

IMPLEMENT_DYNCREATE(CSituationView, CFormView)

CSituationView::CSituationView()
    : CFormView(CSituationView::IDD)
{
    //{{AFX_DATA_INIT(CSituationView)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    m_Initialized = FALSE;
}

CSituationView::~CSituationView()
{
}

void CSituationView::DoDataExchange(CDataExchange *pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSituationView)
    DDX_Control(pDX, IDC_SITUATION_CTRL, m_SituationCtrl);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSituationView, CFormView)
    //{{AFX_MSG_MAP(CSituationView)
    ON_WM_SIZE()
    //}}AFX_MSG_MAP

    ON_MESSAGE(SDC_PICK, OnPick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSituationView diagnostics

#ifdef _DEBUG
void CSituationView::AssertValid() const
{
    CFormView::AssertValid();
}

void CSituationView::Dump(CDumpContext &dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSituationView message handlers


void CSituationView::OnOK()
{
}

void CSituationView::OnCancel()
{
}

void CSituationView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    if (m_Initialized)
        return;

    m_SituationCtrl.InitDlgControl(this);

    m_Initialized = TRUE;
}

void CSituationView::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);

    if (m_SituationCtrl && m_SituationCtrl.GetSafeHwnd()) {
        // Resize the control.
        CRect rect(0, 0, cx, cy);
        m_SituationCtrl.MoveWindow(&rect, TRUE);
    }
}

LRESULT CSituationView::OnPick(WPARAM wParam, LPARAM lParam)
{
    POSITION entity_hdl = (POSITION)wParam;

    theApp.GetMainFrame().GetObjectTreeView().SelectTab(0);
    theApp.GetMainFrame().GetEntityTreeView().SelectEntityBySituationCtrlHandle(entity_hdl);

    return 0;
}
