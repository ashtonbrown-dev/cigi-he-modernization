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

// PacketDlgRateControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "PacketDlgVelocityControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgVelocityControl dialog

CPacketDlgVelocityControl::CPacketDlgVelocityControl(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgVelocityControl::IDD, pParent)
{
    m_EntityID = -1;
    m_PartID = -1;

    //{{AFX_DATA_INIT(CPacketDlgVelocityControl)
    m_ApplyToArtPart = FALSE;
    m_Pitch = 0.0f;
    m_Roll = 0.0f;
    m_Yaw = 0.0f;
    m_X = 0.0f;
    m_Y = 0.0f;
    m_Z = 0.0f;
    m_CoordSystem = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgVelocityControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgVelocityControl)
    DDX_Control(pDX, IDC_COMBOBOXEX_PART_ID, m_ComboPartID);
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_ID, m_ComboEntityID);
    DDX_Check(pDX, IDC_CHECK_ENABLE_ARTPART, m_ApplyToArtPart);
    DDX_Text(pDX, IDC_EDIT_PITCH, m_Pitch);
    DDX_Text(pDX, IDC_EDIT_ROLL, m_Roll);
    DDX_Text(pDX, IDC_EDIT_YAW, m_Yaw);
    DDX_Text(pDX, IDC_EDIT_X, m_X);
    DDX_Text(pDX, IDC_EDIT_Y, m_Y);
    DDX_Text(pDX, IDC_EDIT_Z, m_Z);
    DDX_Radio(pDX, IDC_RADIO_PARENT, m_CoordSystem);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgVelocityControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgVelocityControl)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_ID, OnSelchangeEntityID)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_ENTITY_ID, OnEndeditEntityID)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_PART_ID, OnSelchangePartID)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_PART_ID, OnEndeditPartID)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgVelocityControl message handlers

BOOL CPacketDlgVelocityControl::OnInitDialog()
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

void CPacketDlgVelocityControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgVelocityControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_VELOCITY_CONTROL rc = {0};
    rc.packet_id = CIGI_VELOCITY_CONTROL_OPCODE;
    rc.packet_size = sizeof(CIGI_VELOCITY_CONTROL);
    rc.entity_id = m_EntityID;
    rc.artpart_id = m_PartID;
    rc.apply_artpart = m_ApplyToArtPart;
    rc.coord_system = m_CoordSystem;
    rc.pitch_rate = m_Pitch;
    rc.roll_rate = m_Roll;
    rc.yaw_rate = m_Yaw;
    rc.x_rate = m_X;
    rc.y_rate = m_Y;
    rc.z_rate = m_Z;

    SendImmedCigiMessage(&rc);
}

void CPacketDlgVelocityControl::OnSelchangeEntityID()
{
    PopulatePartIDCombo();

    // Get the ID of the selected entity.
    int idx = m_ComboEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboEntityID.GetItemData(idx));
}

void CPacketDlgVelocityControl::OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult)
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

void CPacketDlgVelocityControl::OnSelchangePartID()
{
    // Get the type of the selected parent.
    int idx = m_ComboPartID.GetCurSel();
    if (idx > -1) {
        m_PartID = MAKE_ID(m_ComboPartID.GetItemData(idx));

        m_ApplyToArtPart = TRUE;
        UpdateData(FALSE);
    }
}

void CPacketDlgVelocityControl::OnEndeditPartID(NMHDR *pNMHDR, LRESULT *pResult)
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

void CPacketDlgVelocityControl::PopulateEntityIDCombo(void)
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

void CPacketDlgVelocityControl::SelectEntityIDComboItem(void)
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

void CPacketDlgVelocityControl::PopulatePartIDCombo(void)
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

int CPacketDlgVelocityControl::GetEntityIDFromCombo(void)
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

int CPacketDlgVelocityControl::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgVelocityControl::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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
