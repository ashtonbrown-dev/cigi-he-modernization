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

// PacketDlgArtpartControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "PacketDlgArtpartControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgArtpartControl dialog

CPacketDlgArtpartControl::CPacketDlgArtpartControl(CWnd *pPart /*=NULL*/)
    : CDialog(CPacketDlgArtpartControl::IDD, pPart)
{
    m_EntityID = -1;
    m_PartID = -1;

    //{{AFX_DATA_INIT(CPacketDlgArtpartControl)
    m_EnableArtPart = FALSE;
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

void CPacketDlgArtpartControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgArtpartControl)
    DDX_Control(pDX, IDC_COMBOBOXEX_PART_ID, m_ComboPartID);
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_ID, m_ComboEntityID);
    DDX_Check(pDX, IDC_CHECK_ENABLE_ARTPART, m_EnableArtPart);
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

BEGIN_MESSAGE_MAP(CPacketDlgArtpartControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgArtpartControl)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_ID, OnSelchangeEntityID)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_ENTITY_ID, OnEndeditEntityID)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_PART_ID, OnSelchangePartID)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_PART_ID, OnEndeditPartID)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgArtpartControl message handlers

BOOL CPacketDlgArtpartControl::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the image lists for the combo boxes.
    m_ComboEntityID.SetImageList(&g_ImgListTypes);

    PopulateEntityIDCombo();
    SelectEntityIDComboItem();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgArtpartControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgArtpartControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_ARTPART_CONTROL ac = {0};
    ac.packet_id = CIGI_ARTPART_CONTROL_OPCODE;
    ac.packet_size = sizeof(CIGI_ARTPART_CONTROL);
    ac.entity_id = m_EntityID;
    ac.artpart_id = m_PartID;
    ac.artpart_enable = m_EnableArtPart;
    ac.pitch_enable = m_EnablePitch;
    ac.roll_enable = m_EnableRoll;
    ac.yaw_enable = m_EnableYaw;
    ac.xoffset_enable = m_EnableX;
    ac.yoffset_enable = m_EnableY;
    ac.zoffset_enable = m_EnableZ;
    ac.pitch = m_Pitch;
    ac.roll = m_Roll;
    ac.yaw = m_Yaw;
    ac.xoffset = m_X;
    ac.yoffset = m_Y;
    ac.zoffset = m_Z;

    SendImmedCigiMessage(&ac);

    // Update the articulated part object.
    ApplyArtPart();
}

void CPacketDlgArtpartControl::RefreshView(void)
{
    CEntity *entity = g_DataManager.GetEntity(m_EntityID);
    if (!entity)
        return;

    CArtPart *artpart = entity->GetArtPart(m_PartID);
    if (!artpart)
        return;

    m_EnableArtPart = artpart->GetEnableFlag();
    m_EnablePitch = artpart->GetPitchEnable();
    m_EnableRoll = artpart->GetRollEnable();
    m_EnableYaw = artpart->GetYawEnable();
    m_EnableX = artpart->GetXEnable();
    m_EnableY = artpart->GetYEnable();
    m_EnableZ = artpart->GetZEnable();
    m_Pitch = artpart->GetPitch();
    m_Roll = artpart->GetRoll();
    m_Yaw = artpart->GetYaw();
    m_X = artpart->GetXOffset();
    m_Y = artpart->GetYOffset();
    m_Z = artpart->GetZOffset();

    UpdateData(FALSE);
}

void CPacketDlgArtpartControl::OnSelchangeEntityID()
{
    PopulatePartIDCombo();

    // Get the ID of the selected entity.
    int idx = m_ComboEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboEntityID.GetItemData(idx));

    RefreshView();
}

void CPacketDlgArtpartControl::OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult)
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
        PopulatePartIDCombo();  // Normally done by OnSelchangeEntityID().

    *pResult = 0;
}

void CPacketDlgArtpartControl::OnSelchangePartID()
{
    // Get the type of the selected parent.
    int idx = m_ComboPartID.GetCurSel();
    if (idx > -1)
        m_PartID = MAKE_ID(m_ComboPartID.GetItemData(idx));
}

void CPacketDlgArtpartControl::OnEndeditPartID(NMHDR *pNMHDR, LRESULT *pResult)
{
    int new_id = atoi(((NMCBEENDEDIT *)pNMHDR)->szText);
    if (new_id == m_PartID)
        return;

    m_PartID = new_id;
    int idx = FindComboItem(m_ComboPartID, m_PartID);
    if ((idx == -1) && (m_PartID == -1))
        idx = 0;

    // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
    // select no item after it has finished its stuff.
    if (idx > -1)
        ::PostMessage(m_ComboPartID.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

    // Post a CBN_SELCHANGE notification so OnSelchangePartID() will be called.
    // The function must be called after the CB_SELCURSEL message is processed
    // so the new item will be selected.
    ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                  MAKEWPARAM(IDC_COMBOBOXEX_PART_ID, CBN_SELCHANGE),
                  (LPARAM)m_ComboPartID.GetSafeHwnd());

    *pResult = 0;
}

void CPacketDlgArtpartControl::PopulateEntityIDCombo(void)
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

        if (entity->GetID() == entity_id) {
            // Select the item in the combo box.
            m_ComboEntityID.SetCurSel(cbo_idx);
        }
    }
}

void CPacketDlgArtpartControl::SelectEntityIDComboItem(void)
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity) {
        m_ComboEntityID.SetCurSel(-1);
        m_ComboPartID.SetCurSel(-1);
        return;
    }

    m_EntityID = entity->GetID();
    m_ComboEntityID.SetCurSel(FindComboItem(m_ComboEntityID, m_EntityID));
    OnSelchangeEntityID();
}

void CPacketDlgArtpartControl::PopulatePartIDCombo(void)
{
    char text[256] = {"None"};
    int cbo_idx = -1;
    int entity_id = -1;
    int part_id = -1;
    CArtPart *artpart = NULL;
    CEntity *entity = g_DataManager.GetEntity(GetEntityIDFromCombo());
    COMBOBOXEXITEM cboitem = {  CBEIF_LPARAM | CBEIF_TEXT,
                                0, NULL, 1, 0, 0, 0, 0, -1
                             };

    m_ComboPartID.ResetContent();

    if (!entity)
        return;

    entity_id = entity->GetID();

    CArtPartMap &map = entity->GetArtPartMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, part_id, artpart);

        sprintf(text, "%d: %s", artpart->GetID(), artpart->GetName());
        cboitem.pszText = text;
        cboitem.cchTextMax = strlen(text + 1);
        cboitem.lParam = MAKE_TREE_DATA(artpart->GetID(), 0, 0);

        // Add a new item to the list.
        cboitem.iItem = FindComboInsertPoint(m_ComboPartID, artpart->GetID());
        cbo_idx = m_ComboPartID.InsertItem(&cboitem);
    }
}

int CPacketDlgArtpartControl::GetEntityIDFromCombo(void)
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

int CPacketDlgArtpartControl::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgArtpartControl::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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

void CPacketDlgArtpartControl::ApplyArtPart(void)
{
    CEntity *entity = g_DataManager.GetEntity(m_EntityID);
    if (entity) {
        CArtPart *artpart = entity->GetArtPart(m_PartID);
        if (artpart) {
            artpart->SetEnableFlag(m_EnableArtPart);
            artpart->SetPitchEnable(m_EnablePitch);
            artpart->SetRollEnable(m_EnableRoll);
            artpart->SetYawEnable(m_EnableYaw);
            artpart->SetXEnable(m_EnableX);
            artpart->SetYEnable(m_EnableY);
            artpart->SetZEnable(m_EnableZ);
            artpart->SetPitch(m_Pitch);
            artpart->SetRoll(m_Roll);
            artpart->SetYaw(m_Yaw);
            artpart->SetXOffset(m_X);
            artpart->SetYOffset(m_Y);
            artpart->SetZOffset(m_Z);
        }
    }
}

