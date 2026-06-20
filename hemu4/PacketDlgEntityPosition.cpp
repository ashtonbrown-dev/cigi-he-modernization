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

// PacketDlgEntityPosition.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "hemu4.h"
#include "PacketDlgEntityPosition.h"
#include "EntityStateView.h"
#include "EntityTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgEntityPosition dialog

CPacketDlgEntityPosition::CPacketDlgEntityPosition(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgEntityPosition::IDD, pParent)
{
    m_EntityID = -1;
    m_ParentID = -1;

    //{{AFX_DATA_INIT(CPacketDlgEntityPosition)
    m_ClampMode = 0;
    m_AltZ = 0.0;
    m_LatX = 0.0;
    m_LonY = 0.0;
    m_Pitch = 0.0;
    m_Roll = 0.0;
    m_Yaw = 0.0;
    m_LabelAltZ = _T("Altitude:");
    m_LabelLatX = _T("Latitude:");
    m_LabelLonY = _T("Longitude:");
    m_LabelLatXUnits = _T("degrees");
    m_LabelLonYUnits = _T("degrees");
    m_LabelAltZUnits = _T("meters");
    m_LabelYaw = _T("Yaw");
    m_LabelPitch = _T("Pitch");
    m_LabelRoll = _T("Roll");

    //}}AFX_DATA_INIT
}

void CPacketDlgEntityPosition::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgEntityPosition)
    DDX_Control(pDX, IDC_COMBOBOXEX_PARENT_ID, m_ComboParentID);
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_ID, m_ComboEntityID);
    DDX_Radio(pDX, IDC_RADIO_CLAMP_NONE, m_ClampMode);
    DDX_Text(pDX, IDC_EDIT_ALTZ, m_AltZ);
    DDX_Text(pDX, IDC_EDIT_LATX, m_LatX);
    DDX_Text(pDX, IDC_EDIT_LONY, m_LonY);
    DDX_Text(pDX, IDC_EDIT_PITCH, m_Pitch);
    DDX_Text(pDX, IDC_EDIT_ROLL, m_Roll);
    DDX_Text(pDX, IDC_EDIT_YAW, m_Yaw);
    DDX_Text(pDX, IDC_LABEL_ALTZ, m_LabelAltZ);
    DDX_Text(pDX, IDC_LABEL_LATX, m_LabelLatX);
    DDX_Text(pDX, IDC_LABEL_LONY, m_LabelLonY);
    DDX_Text(pDX, IDC_LABEL_UNITS_X, m_LabelLatXUnits);
    DDX_Text(pDX, IDC_LABEL_UNITS_Y, m_LabelLonYUnits);
    DDX_Text(pDX, IDC_LABEL_UNITS_Z, m_LabelAltZUnits);
    DDX_Text(pDX, IDC_LABEL_YAW, m_LabelYaw);
    DDX_Text(pDX, IDC_LABEL_PITCH, m_LabelPitch);
    DDX_Text(pDX, IDC_LABEL_ROLL, m_LabelRoll);
    //DDX_Text(pDX, IDC_LABEL_YAW_UNITS, m_LabelYawUnits);
    //DDX_Text(pDX, IDC_LABEL_PITCH_UNITS, m_LabelPitchUnits);
    //DDX_Text(pDX, IDC_LABEL_ROLL_UNITS, m_LabelRollUnits);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgEntityPosition, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgEntityPosition)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_ID, OnSelchangeEntityID)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_ENTITY_ID, OnEndeditEntityID)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_PARENT_ID, OnSelchangeParentID)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_PARENT_ID, OnEndeditParentID)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgEntityPosition message handlers

BOOL CPacketDlgEntityPosition::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the image lists for the combo boxes.
    m_ComboEntityID.SetImageList(&g_ImgListTypes);
    m_ComboParentID.SetImageList(&g_ImgListTypes);

    PopulateEntityIDCombo();
    SelectEntityIDComboItem();
	PopulateParentIDCombo();
    SelectParentIDComboItem();
    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgEntityPosition::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgEntityPosition::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_ENTITY_POSITION ec = {0};
    ec.packet_id = CIGI_ENTITY_POSITION_OPCODE;
    ec.packet_size = sizeof(CIGI_ENTITY_POSITION);
    ec.attach_state = (m_ParentID > -1) ? 1 : 0;
    ec.clamp_mode = m_ClampMode;
    ec.entity_id = m_EntityID;
    ec.parent_id = m_ParentID;
    ec.roll = (float)m_Roll;
    ec.pitch = (float)m_Pitch;
    ec.yaw = (float)m_Yaw;
    ec.lat_x = m_LatX;
    ec.lon_y = m_LonY;
    ec.alt_z = m_AltZ;

    SendImmedCigiMessage(&ec);

    ApplyEntity();
}

void CPacketDlgEntityPosition::ApplyEntity(void)
{
//
// Find the entity and update its data.
//
CEntity *entity = g_DataManager.GetEntity(m_EntityID);
    if( entity ) {

		ENTITY state = entity->GetAndLock();
		state.cigi.clamp_mode = m_ClampMode;
		if ((short)m_ParentID == -1) {
			state.cigi.dofs.latitude = m_LatX;
			state.cigi.dofs.longitude = m_LonY;
			state.cigi.dofs.altitude = m_AltZ;
			state.cigi.dofs.yaw = m_Yaw;
			state.cigi.dofs.pitch = m_Pitch;
			state.cigi.dofs.roll = m_Roll;
		} else {
			state.cigi.dofs.offset_x  = m_LatX;
			state.cigi.dofs.offset_y  = m_LonY;
			state.cigi.dofs.offset_z  = m_AltZ;
			state.cigi.dofs.rel_yaw   = m_Yaw;
			state.cigi.dofs.rel_pitch = m_Pitch;
			state.cigi.dofs.rel_roll  = m_Roll;
		}

		state.cigi.parent_id = (short)m_ParentID;

		// Set the shared object and unlock.
		entity->SetLocked(state);
		entity->ClearLockedChangeFlag();  // chas added to prevent drv from sending entityctrl packet.
		entity->Unlock();

		CEntity *parent = g_DataManager.GetEntity(m_ParentID);
		if ((state.cigi.parent_id != (short)m_ParentID) && (parent || (m_ParentID == -1))) {
			// Update the tree view.
			HTREEITEM htreeParent;
			CEntityTreeView &treeview = theApp.GetMainFrame().GetEntityTreeView();

			if (parent)
				htreeParent = parent->GetHtree();
			else
				htreeParent = NULL;

			HTREEITEM htreeItem = treeview.MoveTreeItem(entity->GetHtree(), htreeParent);
			entity->SetHtree(htreeItem);
			treeview.GetTreeCtrl().Expand(htreeParent, TVE_EXPAND);
			treeview.GetTreeCtrl().SelectItem(htreeItem);

			// Repopulate the parent and reference entity lists.
			treeview.RepopulateEntityStateParentList();
			theApp.GetMainFrame().GetEntityStateView().PopulateRefEntityList();
		}

		// Update the entity view.
		theApp.GetMainFrame().GetEntityStateView().RefreshView();

	} // end if( entity )
}

void CPacketDlgEntityPosition::RefreshView(void)
{
    CEntity *entity = g_DataManager.GetEntity(m_EntityID);
    if (!entity)
        return;

    ENTITY state = entity->GetAndLock();

    if (state.cigi.parent_id < 0) {
        m_AltZ = state.cigi.dofs.altitude;
        m_LatX = state.cigi.dofs.latitude;
        m_LonY = state.cigi.dofs.longitude;
        m_Pitch = state.cigi.dofs.pitch;
        m_Roll = state.cigi.dofs.roll;
        m_Yaw = state.cigi.dofs.yaw;
        m_LabelAltZ = "Altitude:";
        m_LabelLatX = "Latitude:";
        m_LabelLonY = "Longitude:";
        m_LabelLatXUnits = "degrees";
        m_LabelLonYUnits = "degrees";
        m_LabelAltZUnits = "meters";
    } else {
        m_AltZ = state.cigi.dofs.offset_z;
        m_LatX = state.cigi.dofs.offset_y;
        m_LonY = state.cigi.dofs.offset_x;
        m_Pitch = state.cigi.dofs.rel_pitch;
        m_Roll = state.cigi.dofs.rel_roll;
        m_Yaw = state.cigi.dofs.rel_yaw;
        m_LabelAltZ = "Z Offset:";
        m_LabelLatX = "X Offset:";
        m_LabelLonY = "Y Offset:";
        m_LabelLatXUnits = "meters";
        m_LabelLonYUnits = "meters";
        m_LabelAltZUnits = "meters";
    }

    entity->Unlock();

    UpdateData(FALSE);
}

void CPacketDlgEntityPosition::OnSelchangeEntityID()
{
    PopulateParentIDCombo();
    SelectParentIDComboItem();

    // Get the ID of the selected entity.
    int idx = m_ComboEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboEntityID.GetItemData(idx));

    RefreshView();
}

void CPacketDlgEntityPosition::OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult)
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
    } else
        PopulateParentIDCombo();    // Normally done by OnSelchangeEntityID().

    *pResult = 0;
}


void CPacketDlgEntityPosition::OnSelchangeParentID()
{
#if 1
    CEntity *entity = g_DataManager.GetEntity(m_EntityID);
    if (!entity)
        return;

    ENTITY state = entity->GetAndLock();

    // If we are attaching to a parent, the position is relative.  If not,
    // the position is absolute.
    if (m_ComboParentID.GetCurSel() > 0) {
        m_LabelLatX = "X Offset:";
        m_LabelLonY = "Y Offset:";
        m_LabelAltZ = "Z Offset:";
        m_LabelLatXUnits = "meters";
        m_LabelLonYUnits = "meters";
        m_LabelAltZUnits = "meters";
		m_LabelYaw = _T("Yaw Rel.");
		m_LabelPitch = _T("Pitch Rel.");
		m_LabelRoll = _T("Roll Rel.");

        m_LatX = state.cigi.dofs.offset_x;
        m_LonY = state.cigi.dofs.offset_y;
        m_AltZ = state.cigi.dofs.offset_z;
        m_Yaw = state.cigi.dofs.rel_yaw;
        m_Pitch = state.cigi.dofs.rel_pitch;
        m_Roll = state.cigi.dofs.rel_roll;
    } else {
        m_LabelLatX = "Latitude:";
        m_LabelLonY = "Longitude:";
        m_LabelAltZ = "Altitude:";
        m_LabelLatXUnits = "degrees";
        m_LabelLonYUnits = "degrees";
        m_LabelAltZUnits = "meters";
		m_LabelYaw = _T("Yaw");
		m_LabelPitch = _T("Pitch");
		m_LabelRoll = _T("Roll");

        m_AltZ = state.cigi.dofs.altitude;
        m_LatX = state.cigi.dofs.latitude;
        m_LonY = state.cigi.dofs.longitude;
        m_Pitch = state.cigi.dofs.pitch;
        m_Roll = state.cigi.dofs.roll;
        m_Yaw = state.cigi.dofs.yaw;
    }
#endif

    entity->Unlock();
    UpdateData(FALSE);

    // Get the type of the selected parent.
    int idx = m_ComboParentID.GetCurSel();
    if (idx > -1)
        m_ParentID = (short)MAKE_ID(m_ComboParentID.GetItemData(idx));
}

void CPacketDlgEntityPosition::OnEndeditParentID(NMHDR *pNMHDR, LRESULT *pResult)
{
    CString temp = ((NMCBEENDEDIT *)pNMHDR)->szText;
    if (temp == "None")
        m_ParentID = -1;
    else
        m_ParentID = atoi((LPCTSTR)temp);

    int idx = FindComboItem(m_ComboParentID, m_ParentID);
    if ((idx == -1) && ((short)m_ParentID == -1))
        idx = 0;

    if (idx != -1) {
        // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
        // select no item after it has finished its stuff.
        ::PostMessage(m_ComboParentID.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

        // Post a CBN_SELCHANGE notification so OnSelchangeParentID() will be called.
        // The function must be called after the CB_SELCURSEL message is processed
        // so the new item will be selected.
        ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                      MAKEWPARAM(IDC_COMBOBOXEX_PARENT_ID, CBN_SELCHANGE),
                      (LPARAM)m_ComboParentID.GetSafeHwnd());
    }

    *pResult = 0;
}

void CPacketDlgEntityPosition::PopulateEntityIDCombo(void)
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
        cboitem.lParam = MAKE_TREE_DATA(entity->GetID(), OBJECT_TYPE_ENTITY,
                                        entity->GetClass());

        // Add a new item to the list.
        cboitem.iItem = FindComboInsertPoint(m_ComboEntityID, entity->GetID());
        cbo_idx = m_ComboEntityID.InsertItem(&cboitem);

        if (entity->GetID() == entity_id) {
            // Select the item in the combo box.
            m_ComboEntityID.SetCurSel(cbo_idx);
        }
    }
}

void CPacketDlgEntityPosition::SelectEntityIDComboItem(void)
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity) {
        m_ComboEntityID.SetCurSel(-1);
        m_ComboParentID.SetCurSel(-1);

        return;
    }

    m_EntityID = entity->GetID();
    m_ComboEntityID.SetCurSel(FindComboItem(m_ComboEntityID, m_EntityID));
    OnSelchangeEntityID();
}


void CPacketDlgEntityPosition::PopulateParentIDCombo(void)
{
    char text[256] = {"None"};
    int cbo_idx = -1;
    int entity_id = -1;
    int parent_id = -1;
    int curr_entity_id = -1;
    CEntity *entity = NULL;
    CEntity *curr_entity = g_DataManager.GetEntity(GetEntityIDFromCombo());
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_GENERIC, IMG_GENERIC, 0, 0, -1
                             };

    m_ComboParentID.ResetContent();

    // Insert a "None" item.
    cboitem.pszText = text;
    cboitem.cchTextMax = strlen(text + 1);
    cboitem.iImage = -1;
    cboitem.iSelectedImage = -1;
    cboitem.lParam = MAKE_TREE_DATA(-1, -1, -1);
    cbo_idx = m_ComboParentID.InsertItem(&cboitem);
    m_ComboParentID.SetCurSel(cbo_idx);

    if (curr_entity) {
        curr_entity_id = curr_entity->GetID();
        parent_id = curr_entity->GetParent();
    }

    CEntityMap &map = g_DataManager.GetEntityMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, entity_id, entity);

        if (entity_id != curr_entity_id) {
            strcpy(text, entity->GetName());
            cboitem.pszText = text;
            cboitem.cchTextMax = strlen(text + 1);
            cboitem.iImage = entity->GetClass();
            cboitem.iSelectedImage = cboitem.iImage;
            cboitem.lParam = MAKE_TREE_DATA(entity->GetID(), OBJECT_TYPE_ENTITY,
                                            entity->GetClass());

            // Add a new item to the list.
            cboitem.iItem = FindComboInsertPoint(m_ComboParentID, entity->GetID());
            cbo_idx = m_ComboParentID.InsertItem(&cboitem);

            if (entity->GetID() == parent_id) {
                // Select the item in the combo box.
                m_ComboParentID.SetCurSel(cbo_idx);
            }
        }
    }
}

void CPacketDlgEntityPosition::SelectParentIDComboItem(void)
{
    CEntity *entity = g_DataManager.GetEntity(GetEntityIDFromCombo());
    if (!entity) {
        m_ComboParentID.SetCurSel(-1);

        return;
    }

    int parent_id = entity->GetParent();
    m_ComboParentID.SetCurSel(FindComboItem(m_ComboParentID, parent_id));
    OnSelchangeParentID();
}

int CPacketDlgEntityPosition::GetEntityIDFromCombo(void)
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

int CPacketDlgEntityPosition::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgEntityPosition::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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
