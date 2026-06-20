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

// ViewFrustumPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "CigiView.h"
#include "globals.h"
#include "ViewFrustumPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewFrustumPropPage dialog

CViewFrustumPropPage::CViewFrustumPropPage(CWnd *pParent /*=NULL*/)
    : CBaseStateView(CViewFrustumPropPage::IDD, pParent)
{
    //{{AFX_DATA_INIT(CViewFrustumPropPage)
    m_ProjectionType = 0;
    m_FOVBottom = 0.0f;
    m_FOVTop = 0.0f;
    m_FOVLeft = 0.0f;
    m_FOVRight = 0.0f;
    m_FOVNear = 0.0f;
    m_FOVFar = 0.0f;
    //}}AFX_DATA_INIT
}

void CViewFrustumPropPage::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CViewFrustumPropPage)
    DDX_Control(pDX, IDC_RADIO_PERSPECTIVE, m_RadioPerspective);
    DDX_Control(pDX, IDC_RADIO_ORTHOGRAPHIC, m_RadioOrthographic);
    DDX_Control(pDX, IDC_EDIT_FAR_FOV, m_EditFOVFar);
    DDX_Control(pDX, IDC_EDIT_NEAR_FOV, m_EditFOVNear);
    DDX_Control(pDX, IDC_EDIT_RIGHT_FOV, m_EditFOVRight);
    DDX_Control(pDX, IDC_EDIT_LEFT_FOV, m_EditFOVLeft);
    DDX_Control(pDX, IDC_EDIT_TOP_FOV, m_EditFOVTop);
    DDX_Control(pDX, IDC_EDIT_BOTTOM_FOV, m_EditFOVBottom);
    DDX_Control(pDX, IDC_PICTURE_ORTHO, m_PictureCtrlOrtho);
    DDX_Control(pDX, IDC_PICTURE_PERSPECTIVE, m_PictureCtrlPerspective);
    DDX_Radio(pDX, IDC_RADIO_PERSPECTIVE, m_ProjectionType);
    DDX_Text(pDX, IDC_EDIT_BOTTOM_FOV, m_FOVBottom);
    DDX_Text(pDX, IDC_EDIT_TOP_FOV, m_FOVTop);
    DDX_Text(pDX, IDC_EDIT_LEFT_FOV, m_FOVLeft);
    DDX_Text(pDX, IDC_EDIT_RIGHT_FOV, m_FOVRight);
    DDX_Text(pDX, IDC_EDIT_NEAR_FOV, m_FOVNear);
    DDX_Text(pDX, IDC_EDIT_FAR_FOV, m_FOVFar);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CViewFrustumPropPage, CDialog)
    //{{AFX_MSG_MAP(CViewFrustumPropPage)
    ON_BN_CLICKED(IDC_RADIO_PERSPECTIVE, OnRadioPerspective)
    ON_BN_CLICKED(IDC_RADIO_ORTHOGRAPHIC, OnRadioOrthographic)
    //}}AFX_MSG_MAP

    ON_MESSAGE(WM_USEREDIT, OnUserEditControl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewFrustumPropPage message handlers

void CViewFrustumPropPage::OnOK()
{
}

void CViewFrustumPropPage::OnCancel()
{
}

BOOL CViewFrustumPropPage::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_PictureCtrlPerspective.LoadBitmap(IDB_PERSPECTIVE);
    m_PictureCtrlOrtho.LoadBitmap(IDB_ORTHOGRAPHIC);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CViewFrustumPropPage::RefreshView(void)
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)      // if we have a view group, do nothing
        return;

    // Gray out the numeric fields during Run mode, so that the user
    // cannot edit the values.
    m_EditFOVFar.EnableWindow(g_bFreezeFlag);
    m_EditFOVNear.EnableWindow(g_bFreezeFlag);
    m_EditFOVRight.EnableWindow(g_bFreezeFlag);
    m_EditFOVLeft.EnableWindow(g_bFreezeFlag);
    m_EditFOVTop.EnableWindow(g_bFreezeFlag);
    m_EditFOVBottom.EnableWindow(g_bFreezeFlag);

    m_ProjectionType = view->GetProjectionType();
    m_FOVBottom = view->GetFovBottom();
    m_FOVTop = view->GetFovTop();
    m_FOVLeft = view->GetFovLeft();
    m_FOVRight = view->GetFovRight();
    m_FOVNear = view->GetFovNear();
    m_FOVFar = view->GetFovFar();

    UpdateData(FALSE);

    if (m_ProjectionType == PROJECTION_TYPE_PARALLEL) {
        m_PictureCtrlOrtho.ShowWindow(SW_SHOW);
        m_PictureCtrlPerspective.ShowWindow(SW_HIDE);
    } else {
        m_PictureCtrlPerspective.ShowWindow(SW_SHOW);
        m_PictureCtrlOrtho.ShowWindow(SW_HIDE);
    }
}

void CViewFrustumPropPage::OnRadioPerspective()
{
    m_PictureCtrlPerspective.ShowWindow(SW_SHOW);
    m_PictureCtrlOrtho.ShowWindow(SW_HIDE);

    CCigiView *view = g_DataManager.GetSelectedView();
    if (view)
        view->SetProjectionType(PROJECTION_TYPE_PERSPECTIVE);
}

void CViewFrustumPropPage::OnRadioOrthographic()
{
    m_PictureCtrlOrtho.ShowWindow(SW_SHOW);
    m_PictureCtrlPerspective.ShowWindow(SW_HIDE);

    CCigiView *view = g_DataManager.GetSelectedView();
    if (view)
        view->SetProjectionType(PROJECTION_TYPE_PARALLEL);
}

LRESULT CViewFrustumPropPage::OnUserEditControl(WPARAM wParam, LPARAM lParam)
{
    CEditKbd *ctrl = (CEditKbd *)lParam;

    if (ctrl == &m_EditFOVFar)
        OnUserChangeFovFar();
    else if (ctrl == &m_EditFOVNear)
        OnUserChangeFovNear();
    else if (ctrl == &m_EditFOVRight)
        OnUserChangeFovRight();
    else if (ctrl == &m_EditFOVLeft)
        OnUserChangeFovLeft();
    else if (ctrl == &m_EditFOVTop)
        OnUserChangeFovTop();
    else if (ctrl == &m_EditFOVBottom)
        OnUserChangeFovBottom();

    return 0;
}

void CViewFrustumPropPage::OnUserChangeFovNear()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    UpdateData(TRUE);

    view->SetFovNear(m_FOVNear);
}

void CViewFrustumPropPage::OnUserChangeFovFar()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    UpdateData(TRUE);

    view->SetFovFar(m_FOVFar);
}

void CViewFrustumPropPage::OnUserChangeFovLeft()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    UpdateData(TRUE);

    view->SetFovLeft(m_FOVLeft);
}

void CViewFrustumPropPage::OnUserChangeFovRight()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    UpdateData(TRUE);

    view->SetFovRight(m_FOVRight);
}

void CViewFrustumPropPage::OnUserChangeFovTop()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    UpdateData(TRUE);

    view->SetFovTop(m_FOVTop);
}

void CViewFrustumPropPage::OnUserChangeFovBottom()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    UpdateData(TRUE);

    view->SetFovBottom(m_FOVBottom);
}
