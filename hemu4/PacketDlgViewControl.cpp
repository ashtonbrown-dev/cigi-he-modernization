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

// PacketDlgViewControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "hemu4.h"
#include "PacketDlgViewControl.h"
#include "CigiView.h"
#include "ViewGroup.h"
#include "ViewStateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgViewControl dialog


CPacketDlgViewControl::CPacketDlgViewControl(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgViewControl::IDD, pParent)
{
    m_EntityID = -1;
    m_ViewID = -1;
    m_GroupID = -1;

    //{{AFX_DATA_INIT(CPacketDlgViewControl)
    m_EnablePitch = FALSE;
    m_EnableRoll = FALSE;
    m_EnableYaw = FALSE;
    m_EnableX = FALSE;
    m_EnableY = FALSE;
    m_EnableZ = FALSE;
    m_Pitch = 0.0f;
    m_Roll = 0.0f;
    m_Yaw = 0.0f;
    m_X = 0.0f;
    m_Y = 0.0f;
    m_Z = 0.0f;
    //}}AFX_DATA_INIT
}

void CPacketDlgViewControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgViewControl)
    DDX_Control(pDX, IDC_COMBOBOXEX_GROUP_ID, m_ComboGroupID);
    DDX_Control(pDX, IDC_COMBOBOXEX_VIEW_ID, m_ComboViewID);
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_ID, m_ComboEntityID);
    DDX_Check(pDX, IDC_CHECK_ENABLE_PITCH, m_EnablePitch);
    DDX_Check(pDX, IDC_CHECK_ENABLE_ROLL, m_EnableRoll);
    DDX_Check(pDX, IDC_CHECK_ENABLE_YAW, m_EnableYaw);
    DDX_Check(pDX, IDC_CHECK_ENABLE_X, m_EnableX);
    DDX_Check(pDX, IDC_CHECK_ENABLE_Y, m_EnableY);
    DDX_Check(pDX, IDC_CHECK_ENABLE_Z, m_EnableZ);
    DDX_Text(pDX, IDC_EDIT_PITCH, m_Pitch);
    DDX_Text(pDX, IDC_EDIT_ROLL, m_Roll);
    DDX_Text(pDX, IDC_EDIT_YAW, m_Yaw);
    DDX_Text(pDX, IDC_EDIT_X, m_X);
    DDX_Text(pDX, IDC_EDIT_Y, m_Y);
    DDX_Text(pDX, IDC_EDIT_Z, m_Z);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgViewControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgViewControl)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_ID, OnSelchangeEntityId)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_ENTITY_ID, OnEndeditEntityId)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_GROUP_ID, OnSelchangeGroupId)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_GROUP_ID, OnEndeditGroupId)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_VIEW_ID, OnSelchangeViewId)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_VIEW_ID, OnEndeditViewId)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgViewControl message handlers

BOOL CPacketDlgViewControl::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the image lists for the combo boxes.
    m_ComboEntityID.SetImageList(&g_ImgListTypes);
    m_ComboViewID.SetImageList(&g_ImgListIcons);
    m_ComboGroupID.SetImageList(&g_ImgListIcons);

    PopulateEntityIDCombo();
    PopulateViewIDCombo();
    PopulateGroupIDCombo();
    m_ComboGroupID.SetCurSel(0);
    m_ComboViewID.SetCurSel(0);
    RefreshView();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgViewControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgViewControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_VIEW_CONTROL vc = {0};
    vc.packet_id = CIGI_VIEW_CONTROL_OPCODE;
    vc.packet_size = sizeof(CIGI_VIEW_CONTROL);
    vc.view_id = m_ViewID;
    vc.group_id = m_GroupID;
    vc.entity_id = m_EntityID;
    vc.xoffset_enable = m_EnableX;
    vc.yoffset_enable = m_EnableY;
    vc.zoffset_enable = m_EnableZ;
    vc.roll_enable = m_EnableRoll;
    vc.pitch_enable = m_EnablePitch;
    vc.yaw_enable = m_EnableYaw;
    vc.xoffset = m_X;
    vc.yoffset = m_Y;
    vc.zoffset = m_Z;
    vc.roll = m_Roll;
    vc.pitch = m_Pitch;
    vc.yaw = m_Yaw;

    SendImmedCigiMessage(&vc);

//    ApplyView();
}

void CPacketDlgViewControl::RefreshView(void)
{
    CViewGroup *group = g_DataManager.GetViewGroup(m_GroupID);
    if (group) {
        //m_ViewID = -1;
        //m_ComboViewID.SetCurSel(-1);  // No view selected
        m_ComboViewID.EnableWindow(FALSE);

        m_EntityID = group->GetEntityID();
        m_Pitch = group->GetPitch();
        m_Roll = group->GetRoll();
        m_Yaw = group->GetYaw();
        m_X = group->GetXOffset();
        m_Y = group->GetYOffset();
        m_Z = group->GetZOffset();
    } else {
        CCigiView *view = g_DataManager.GetView(m_ViewID);
        if (!view)
            return;

        m_ComboViewID.EnableWindow(TRUE);

        m_GroupID = 0;
        m_ComboGroupID.SetCurSel(0);    // "None" selected

        m_EntityID = view->GetEntityID();
        m_Pitch = view->GetPitch();
        m_Roll = view->GetRoll();
        m_Yaw = view->GetYaw();
        m_X = view->GetXOffset();
        m_Y = view->GetYOffset();
        m_Z = view->GetZOffset();
    }

    m_EnableX = 0;
    m_EnableY = 0;
    m_EnableZ = 0;
    m_EnableYaw = 0;
    m_EnablePitch = 0;
    m_EnableRoll = 0;

    UpdateData(FALSE);
}

void CPacketDlgViewControl::ApplyView(void)
{
    UpdateData(TRUE);

    m_ViewID = GetViewIDFromCombo();
    m_GroupID = GetGroupIDFromCombo();
    m_EntityID = GetEntityIDFromCombo();

    if (m_GroupID == 0) {
        CCigiView *view = g_DataManager.GetView(m_ViewID);
        if (!view)
            return;

        m_GroupID = 0;
        m_ComboGroupID.SetCurSel(0);    // "None" selected

        view->SetEntityID(m_EntityID);

        if (m_EnablePitch)
            view->SetPitch(m_Pitch);

        if (m_EnableRoll)
            view->SetRoll(m_Roll);

        if (m_EnableYaw)
            view->SetYaw(m_Yaw);

        if (m_EnableX)
            view->SetXOffset(m_X);

        if (m_EnableY)
            view->SetYOffset(m_Y);

        if (m_EnableZ)
            view->SetZOffset(m_Z);
    } else {
        CViewGroup *group = g_DataManager.GetViewGroup(m_GroupID);
        if (!group)
            return;

        group->SetEntityID(m_EntityID);

        if (m_EnablePitch)
            group->SetPitch(m_Pitch);

        if (m_EnableRoll)
            group->SetRoll(m_Roll);

        if (m_EnableYaw)
            group->SetYaw(m_Yaw);

        if (m_EnableX)
            group->SetXOffset(m_X);

        if (m_EnableY)
            group->SetYOffset(m_Y);

        if (m_EnableZ)
            group->SetZOffset(m_Z);
    }

    theApp.GetMainFrame().GetViewStateView().RefreshView();
}

void CPacketDlgViewControl::OnSelchangeEntityId()
{
    // Get the ID of the selected entity.
    int idx = m_ComboEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboEntityID.GetItemData(idx));
}

void CPacketDlgViewControl::OnEndeditEntityId(NMHDR *pNMHDR, LRESULT *pResult)
{
    int new_id = atoi(((NMCBEENDEDIT *)pNMHDR)->szText);
    if (new_id == m_EntityID)
        return;

    m_EntityID = new_id;
    int idx = FindComboItem(m_ComboEntityID, m_EntityID);
    if (idx > -1) {
        // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
        // select no item after it has finished its stuff.
        ::PostMessage(m_ComboEntityID.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

        // Post a CBN_SELCHANGE notification so OnSelchangeEntityID() will be called.
        // The function must be called after the CB_SELCURSEL message is processed
        // so the new item will be selected.
        ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                      MAKEWPARAM(IDC_COMBOBOXEX_ENTITY_ID, CBN_SELCHANGE),
                      (LPARAM)m_ComboEntityID.GetSafeHwnd());
    }

    *pResult = 0;
}

void CPacketDlgViewControl::OnSelchangeViewId()
{
    m_ViewID = GetViewIDFromCombo();
    SelectEntityIDComboItem();
    RefreshView();
}

void CPacketDlgViewControl::OnEndeditViewId(NMHDR *pNMHDR, LRESULT *pResult)
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

void CPacketDlgViewControl::OnSelchangeGroupId()
{
    m_GroupID = GetGroupIDFromCombo();

    if (m_GroupID == 0)
        SelectViewIDComboItem();

    SelectEntityIDComboItem();
    RefreshView();
}

void CPacketDlgViewControl::OnEndeditGroupId(NMHDR *pNMHDR, LRESULT *pResult)
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

void CPacketDlgViewControl::PopulateEntityIDCombo(void)
{
    char text[256] = {0};
    int cbo_idx = -1;
    int entity_id = -1;
    CEntity *entity = NULL;
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_GENERIC, IMG_GENERIC, 0, 0, -1
                             };

    m_ComboEntityID.ResetContent();

    CEntityMap &map = g_DataManager.GetEntityMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, entity_id, entity);

        strcpy(text, entity->GetName());
        cboitem.pszText = text;
        cboitem.cchTextMax = strlen(text + 1);
        cboitem.iImage = entity->GetClass();
        cboitem.iSelectedImage = cboitem.iImage;
        cboitem.lParam = MAKE_TREE_DATA(entity->GetID(), entity->GetType(),
                                        entity->GetClass());

        // Add a new item to the list.
        cboitem.iItem = FindComboInsertPoint(m_ComboEntityID, entity->GetID());
        cbo_idx = m_ComboEntityID.InsertItem(&cboitem);

        if (m_EntityID == entity_id) {
            // Select the item in the combo box.
            m_ComboEntityID.SetCurSel(cbo_idx);
        }
    }
}

void CPacketDlgViewControl::SelectEntityIDComboItem(void)
{
    int entity_id = -1;

    if (m_GroupID) {
        CViewGroup *group = g_DataManager.GetViewGroup(m_GroupID);
        if (!group)
            return;

        entity_id = group->GetEntityID();
    } else {
        CCigiView *view = g_DataManager.GetView(m_ViewID);
        if (!view)
            return;

        // If the view is part of a group, get that group's entity ID.
        CViewGroup *group = g_DataManager.GetViewGroup(view->GetGroupID());
        if (group)
            entity_id = group->GetEntityID();
        else
            entity_id = view->GetEntityID();
    }

    int idx = FindComboItem(m_ComboEntityID, entity_id);
    m_ComboEntityID.SetCurSel(idx);
}

void CPacketDlgViewControl::PopulateViewIDCombo(void)
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

void CPacketDlgViewControl::SelectViewIDComboItem(void)
{
    m_ViewID = GetViewIDFromCombo();
    int idx = FindComboItem(m_ComboViewID, m_ViewID);
    m_ComboViewID.SetCurSel(idx);
}

void CPacketDlgViewControl::PopulateGroupIDCombo(void)
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

void CPacketDlgViewControl::SelectGroupIDComboItem(void)
{
    int idx = FindComboItem(m_ComboGroupID, m_GroupID);
    m_ComboGroupID.SetCurSel(idx);
}

int CPacketDlgViewControl::GetEntityIDFromCombo(void)
{
    int itemdata = m_ComboEntityID.GetItemData(m_ComboEntityID.GetCurSel());
    if (itemdata != -1)
        return MAKE_ID(itemdata);
    else {
        CString text;
        m_ComboEntityID.GetWindowText(text);
        return atoi((LPCTSTR)text);
    }
}

int CPacketDlgViewControl::GetViewIDFromCombo(void)
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

int CPacketDlgViewControl::GetGroupIDFromCombo(void)
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

int CPacketDlgViewControl::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgViewControl::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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
