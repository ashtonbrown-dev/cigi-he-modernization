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

// PacketDlgViewDef.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "hemu4.h"
#include "PacketDlgViewDef.h"
#include "ViewGroup.h"
#include "CigiView.h"
#include "ViewStateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgViewDef dialog

CPacketDlgViewDef::CPacketDlgViewDef(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgViewDef::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgViewDef)
    m_EnableBottom = FALSE;
    m_EnableFar = FALSE;
    m_EnableLeft = FALSE;
    m_EnableNear = FALSE;
    m_EnableRight = FALSE;
    m_EnableTop = FALSE;
    m_Reorder = FALSE;
    m_Bottom = 0.0f;
    m_Far = 0.0f;
    m_Left = 0.0f;
    m_Near = 0.0f;
    m_Right = 0.0f;
    m_Top = 0.0f;
    m_ViewType = 0;
    m_MirrorMode = 0;
    m_PixelRepMode = 0;
    m_ProjectionType = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgViewDef::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgViewDef)
    DDX_Control(pDX, IDC_COMBOBOXEX_VIEW_ID, m_ComboViewID);
    DDX_Control(pDX, IDC_COMBOBOXEX_GROUP_ID, m_ComboGroupID);
    DDX_Check(pDX, IDC_CHECK_ENABLE_BOTTOM, m_EnableBottom);
    DDX_Check(pDX, IDC_CHECK_ENABLE_FAR, m_EnableFar);
    DDX_Check(pDX, IDC_CHECK_ENABLE_LEFT, m_EnableLeft);
    DDX_Check(pDX, IDC_CHECK_ENABLE_NEAR, m_EnableNear);
    DDX_Check(pDX, IDC_CHECK_ENABLE_RIGHT, m_EnableRight);
    DDX_Check(pDX, IDC_CHECK_ENABLE_TOP, m_EnableTop);
    DDX_Check(pDX, IDC_CHECK_REORDER, m_Reorder);
    DDX_Text(pDX, IDC_EDIT_BOTTOM, m_Bottom);
    DDX_Text(pDX, IDC_EDIT_FAR, m_Far);
    DDX_Text(pDX, IDC_EDIT_LEFT, m_Left);
    DDX_Text(pDX, IDC_EDIT_NEAR, m_Near);
    DDX_Text(pDX, IDC_EDIT_RIGHT, m_Right);
    DDX_Text(pDX, IDC_EDIT_TOP, m_Top);
    DDX_Text(pDX, IDC_EDIT_VIEW_TYPE, m_ViewType);
    DDV_MinMaxInt(pDX, m_ViewType, 0, 7);
    DDX_Radio(pDX, IDC_RADIO_MIRROR_NONE, m_MirrorMode);
    DDX_Radio(pDX, IDC_RADIO_PIXELREP_NONE, m_PixelRepMode);
    DDX_Radio(pDX, IDC_RADIO_PROJECTION_PERSPECTIVE, m_ProjectionType);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgViewDef, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgViewDef)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_VIEW_ID, OnSelchangeViewId)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_VIEW_ID, OnEndeditViewId)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_GROUP_ID, OnSelchangeGroupId)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_GROUP_ID, OnEndeditGroupId)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgViewDef message handlers

void CPacketDlgViewDef::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgViewDef::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_VIEW_DEF vd = {0};
    vd.packet_id = CIGI_VIEW_DEF_OPCODE;
    vd.packet_size = sizeof(CIGI_VIEW_DEF);
    vd.view_id = m_ViewID;
    vd.group_id = m_GroupID;
    vd.view_type = m_ViewType;
    vd.left_enable = m_EnableLeft;
    vd.right_enable = m_EnableRight;
    vd.top_enable = m_EnableTop;
    vd.bottom_enable = m_EnableBottom;
    vd.near_enable = m_EnableNear;
    vd.far_enable = m_EnableFar;
    vd.fov_left = m_Left;
    vd.fov_right = m_Right;
    vd.fov_top = m_Top;
    vd.fov_bottom = m_Bottom;
    vd.fov_near = m_Near;
    vd.fov_far = m_Far;
    vd.reorder = m_Reorder;
    vd.projection_type = m_ProjectionType;
    vd.pixel_replication_mode = m_PixelRepMode;
    vd.mirror_mode = m_MirrorMode;

    SendImmedCigiMessage(&vd);

//    ApplyView();
}

BOOL CPacketDlgViewDef::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the image lists for the combo boxes.
    m_ComboViewID.SetImageList(&g_ImgListIcons);
    m_ComboGroupID.SetImageList(&g_ImgListIcons);

    PopulateViewIDCombo();
    PopulateGroupIDCombo();
    m_ComboViewID.SetCurSel(0);
    m_ViewID = GetViewIDFromCombo();
    SelectGroupIDComboItem();
    RefreshView();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgViewDef::RefreshView(void)
{
    CCigiView *view = g_DataManager.GetView(m_ViewID);
    if (!view)
        return;

    m_GroupID = view->GetGroupID();
    m_ComboGroupID.SetCurSel(FindComboItem(m_ComboGroupID, m_GroupID));

    m_ViewType = view->GetViewType();
    m_ProjectionType = view->GetProjectionType();
    m_Left = view->GetFovLeft();
    m_Right = view->GetFovRight();
    m_Top = view->GetFovTop();
    m_Bottom = view->GetFovBottom();
    m_Near = view->GetFovNear();
    m_Far = view->GetFovFar();
    m_MirrorMode = view->GetMirrorMode();
    m_PixelRepMode = view->GetReplicationMode();
    m_EnableLeft = 0;
    m_EnableRight = 0;
    m_EnableTop = 0;
    m_EnableBottom = 0;
    m_EnableNear = 0;
    m_EnableFar = 0;

    UpdateData(FALSE);
}

void CPacketDlgViewDef::ApplyView(void)
{
    UpdateData(TRUE);

    m_ViewID = GetViewIDFromCombo();
    m_GroupID = GetGroupIDFromCombo();

    CCigiView *view = g_DataManager.GetView(m_ViewID);
    if (!view)
        return;

    view->SetViewType(m_ViewType);
    view->SetProjectionType(m_ProjectionType);
    view->SetMirrorMode(m_MirrorMode);
    view->SetReplicationMode(m_PixelRepMode);

    if (m_EnableLeft)
        view->SetFovLeft(m_Left);

    if (m_EnableRight)
        view->SetFovRight(m_Right);

    if (m_EnableTop)
        view->SetFovTop(m_Top);

    if (m_EnableBottom)
        view->SetFovBottom(m_Bottom);

    if (m_EnableNear)
        view->SetFovNear(m_Near);

    if (m_EnableFar)
        view->SetFovFar(m_Far);

    theApp.GetMainFrame().GetViewStateView().RefreshView();
}

void CPacketDlgViewDef::OnSelchangeViewId()
{
    m_ViewID = GetViewIDFromCombo();
    SelectGroupIDComboItem();
    RefreshView();
}

void CPacketDlgViewDef::OnEndeditViewId(NMHDR *pNMHDR, LRESULT *pResult)
{
    CString temp = ((NMCBEENDEDIT *)pNMHDR)->szText;
    if (temp.Left(5) == "View ")
        temp.Delete(0, 5);      // Delete "View " so the number is first.

    int new_id = atoi((LPCTSTR)temp);
    if (new_id == m_ViewID)
        return;

    m_ViewID = new_id;
    int idx = FindComboItem(m_ComboViewID, m_ViewID);
    if ((idx == -1) && (m_ViewID == -1))
        idx = 0;

    if (idx != -1) {
        // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
        // select no item after it has finished its stuff.
        ::PostMessage(m_ComboViewID.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

        // Post a CBN_SELCHANGE notification so OnSelchangeParentID() will be called.
        // The function must be called after the CB_SELCURSEL message is processed
        // so the new item will be selected.
        ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                      MAKEWPARAM(IDC_COMBOBOXEX_VIEW_ID, CBN_SELCHANGE),
                      (LPARAM)m_ComboViewID.GetSafeHwnd());
    }

    *pResult = 0;
}

void CPacketDlgViewDef::OnSelchangeGroupId()
{
    m_GroupID = GetGroupIDFromCombo();
    RefreshView();
}

void CPacketDlgViewDef::OnEndeditGroupId(NMHDR *pNMHDR, LRESULT *pResult)
{
    CString temp = ((NMCBEENDEDIT *)pNMHDR)->szText;
    if (temp.Left(6) == "Group ")
        temp.Delete(0, 6);      // Delete "Group " so the number is first.

    int new_id = atoi((LPCTSTR)temp);
    if (new_id == m_GroupID)
        return;

    m_GroupID = new_id;
    int idx = FindComboItem(m_ComboGroupID, m_GroupID);
    if ((idx == -1) && (m_GroupID == -1))
        idx = 0;

    if (idx != -1) {
        // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
        // select no item after it has finished its stuff.
        ::PostMessage(m_ComboGroupID.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

        // Post a CBN_SELCHANGE notification so OnSelchangeParentID() will be called.
        // The function must be called after the CB_SELCURSEL message is processed
        // so the new item will be selected.
        ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                      MAKEWPARAM(IDC_COMBOBOXEX_GROUP_ID, CBN_SELCHANGE),
                      (LPARAM)m_ComboGroupID.GetSafeHwnd());
    }

    *pResult = 0;
}

void CPacketDlgViewDef::PopulateViewIDCombo(void)
{
    char text[256] = {0};
    int cbo_idx = -1;
    int view_id = -1;
    CCigiView *view = NULL;
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_VIEW, IMG_VIEW, 0, 0, -1
                             };

    m_ComboViewID.ResetContent();

    CViewMap &map = g_DataManager.GetViewMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, view_id, view);

        strcpy(text, view->GetName());
        cboitem.pszText = text;
        cboitem.cchTextMax = strlen(text + 1);
        cboitem.lParam = MAKE_TREE_DATA(view->GetViewID(), 0, 0);

        // Add a new item to the list.
        cboitem.iItem = FindComboInsertPoint(m_ComboViewID, view->GetViewID());
        cbo_idx = m_ComboViewID.InsertItem(&cboitem);

        if (m_ViewID == view_id) {
            // Select the item in the combo box.
            m_ComboViewID.SetCurSel(cbo_idx);
        }
    }
}

void CPacketDlgViewDef::SelectViewIDComboItem(void)
{
}

void CPacketDlgViewDef::PopulateGroupIDCombo(void)
{
    char text[256] = {"None"};
    int cbo_idx = -1;
    int group_id = -1;
    int i;
    CViewGroup *group = NULL;
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_EX, IMG_EX, 0, 0, -1
                             };

    m_ComboGroupID.ResetContent();

    // Create a None item.
    cboitem.pszText = text;
    cboitem.cchTextMax = strlen(text + 1);
    cboitem.lParam = 0;
    cboitem.iItem = 0;
    cbo_idx = m_ComboGroupID.InsertItem(&cboitem);

    // Set the group images.
    cboitem.iImage = IMG_VIEWGROUP;
    cboitem.iSelectedImage = IMG_VIEWGROUP;

    CViewGroup **array = g_DataManager.GetViewGroupArray();
    for (i = 1; i < VIEWGROUP_ARRAY_SIZE; i++) {
        group = array[i];
        if (group) {
            group_id = group->GetGroupID();

            strcpy(text, group->GetName());
            cboitem.cchTextMax = strlen(text + 1);
            cboitem.lParam = MAKE_TREE_DATA(group_id, 0, 0);

            // Add a new item to the list.
            cboitem.iItem = FindComboInsertPoint(m_ComboGroupID, group_id);
            cbo_idx = m_ComboGroupID.InsertItem(&cboitem);

            if (m_GroupID == group_id) {
                // Select the item in the combo box.
                m_ComboGroupID.SetCurSel(cbo_idx);
            }
        }
    }
}

void CPacketDlgViewDef::SelectGroupIDComboItem(void)
{
    CCigiView *view = g_DataManager.GetView(m_ViewID);
    if (!view) {
        m_ComboGroupID.SetCurSel(0);
        return;
    }

    int idx = FindComboItem(m_ComboGroupID, view->GetGroupID());
    m_ComboGroupID.SetCurSel(idx);
}

int CPacketDlgViewDef::GetViewIDFromCombo(void)
{
    int itemdata = m_ComboViewID.GetItemData(m_ComboViewID.GetCurSel());
    if (itemdata != -1)
        return MAKE_ID(itemdata);
    else {
        CString text;
        m_ComboViewID.GetWindowText(text);
        return atoi((LPCTSTR)text);
    }
}

int CPacketDlgViewDef::GetGroupIDFromCombo(void)
{
    int itemdata = m_ComboGroupID.GetItemData(m_ComboGroupID.GetCurSel());
    if (itemdata != -1)
        return MAKE_ID(itemdata);
    else {
        CString text;
        m_ComboGroupID.GetWindowText(text);
        return atoi((LPCTSTR)text);
    }
}

int CPacketDlgViewDef::FindComboItem(CComboBoxEx &combo, const int id)
{
    int i = 0;
    COMBOBOXEXITEM cboitem = { CBEIF_LPARAM, 0, NULL, 1, 0, 0, 0, 0, -1 };

    // Test the selected item first.
    i = combo.GetCurSel();
    if (i > -1) {
        if ((short)MAKE_ID(combo.GetItemData(i)) == (short)id)
            return i;
    }

    i = 0;

    while (combo.GetItem(&cboitem)) {
        if ((short)MAKE_ID(cboitem.lParam) == (short)id)
            return i;

        cboitem.iItem = ++i;
    }

    return -1;
}

int CPacketDlgViewDef::FindComboInsertPoint(CComboBoxEx &combo, const int id)
{
    int i = 0;
    COMBOBOXEXITEM cboitem = { CBEIF_LPARAM, 0, NULL, 1, 0, 0, 0, 0, -1 };

    while (combo.GetItem(&cboitem)) {
        if ((short)MAKE_ID(cboitem.lParam) > id)
            return i;

        cboitem.iItem = ++i;
    }

    return i;
}
