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

// ObjectStateView.cpp : implementation file
//

#include "stdafx.h"
#include "hemu4.h"
#include "ObjectStateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectStateView

IMPLEMENT_DYNCREATE(CObjectStateView, CFormView)

CObjectStateView::CObjectStateView()
    : CFormView(CObjectStateView::IDD)
{
    //{{AFX_DATA_INIT(CObjectStateView)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    m_ActiveViewNum = -1;
    m_Views[OBJECT_TYPE_ENTITY] = &m_EntityView;
    m_Views[OBJECT_TYPE_VIEW] = &m_ViewView;
    m_Views[OBJECT_TYPE_VIEWGROUP] = &m_ViewgroupView;
    m_Views[OBJECT_TYPE_WEATHER] = &m_WeatherView;
    m_Views[OBJECT_TYPE_SKY] = &m_SkyView;
    m_Views[OBJECT_TYPE_DATABASE] = &m_TerrainView;

    m_Initialized = FALSE;
}

CObjectStateView::~CObjectStateView()
{
}

void CObjectStateView::DoDataExchange(CDataExchange *pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CObjectStateView)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CObjectStateView, CFormView)
    //{{AFX_MSG_MAP(CObjectStateView)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectStateView diagnostics

#ifdef _DEBUG
void CObjectStateView::AssertValid() const
{
    CFormView::AssertValid();
}

void CObjectStateView::Dump(CDumpContext &dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CObjectStateView message handlers

int CObjectStateView::SetActiveView(const int view)
{
    int retval = m_ActiveViewNum;

    if (m_ActiveViewNum != view) {
        if (m_ActiveViewNum >= 0)
            m_Views[m_ActiveViewNum]->ShowWindow(SW_HIDE);

        if ((view >= 0) && (view < VIEW_COUNT))
            m_Views[view]->ShowWindow(SW_SHOW);

        m_ActiveViewNum = view;
    }

    // Refresh the view upon select.
    if ((view >= 0) && (view < VIEW_COUNT))
        m_Views[view]->RefreshView();

    return retval;
}

void CObjectStateView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    if (m_Initialized)
        return;

    // Lock the resources for the child dialog boxes.
    m_DlgTempl[OBJECT_TYPE_ENTITY] = LockDlgRes(MAKEINTRESOURCE(IDD_ENTITY_STATE_VIEW));
    m_DlgTempl[OBJECT_TYPE_VIEW] = LockDlgRes(MAKEINTRESOURCE(IDD_VIEW_STATE_VIEW));
    m_DlgTempl[OBJECT_TYPE_VIEWGROUP] = LockDlgRes(MAKEINTRESOURCE(IDD_VIEWGROUP_STATE_VIEW));
    m_DlgTempl[OBJECT_TYPE_WEATHER] = LockDlgRes(MAKEINTRESOURCE(IDD_WEATHER_STATE_VIEW));
    m_DlgTempl[OBJECT_TYPE_SKY] = LockDlgRes(MAKEINTRESOURCE(IDD_SKY_STATE_VIEW));
    m_DlgTempl[OBJECT_TYPE_DATABASE] = LockDlgRes(MAKEINTRESOURCE(IDD_TERRAIN_STATE_VIEW));

    // Create the child windows.
    for (int i = 0; i < VIEW_COUNT; i++) {
        m_Views[i]->CreateIndirect(m_DlgTempl[i], this);
        m_Views[i]->SetWindowPos(NULL, 1, 1, 0, 0, SWP_NOSIZE);
    }

    m_Initialized = TRUE;
}
