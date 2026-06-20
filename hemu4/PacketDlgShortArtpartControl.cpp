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

// PacketDlgShortArtpartControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "PacketDlgShortArtpartControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgShortArtpartControl dialog

CPacketDlgShortArtpartControl::CPacketDlgShortArtpartControl(CWnd *pParent /*=NULL*/)
    : OldDialog(CPacketDlgShortArtpartControl::IDD, pParent)
{
    m_EntityID = -1;
    m_PartID1 = -1;
    m_PartID2 = -1;

    //{{AFX_DATA_INIT(CPacketDlgShortArtpartControl)
    m_Enable1 = FALSE;
    m_Enable2 = FALSE;
    m_Dof1 = 0;
    m_Dof2 = 0;
    m_Value1 = 0.0f;
    m_Value2 = 0.0f;
    //}}AFX_DATA_INIT
}

void CPacketDlgShortArtpartControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgShortArtpartControl)
    DDX_Control(pDX, IDC_COMBOBOXEX_PART_ID2, m_ComboPartID2);
    DDX_Control(pDX, IDC_COMBOBOXEX_PART_ID1, m_ComboPartID1);
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_ID, m_ComboEntityID);
    DDX_Check(pDX, IDC_CHECK_ENABLE_ARTPART1, m_Enable1);
    DDX_Check(pDX, IDC_CHECK_ENABLE_ARTPART2, m_Enable2);
    DDX_CBIndex(pDX, IDC_COMBOBOXEX_DOF1, m_Dof1);
    DDX_CBIndex(pDX, IDC_COMBOBOXEX_DOF2, m_Dof2);
    DDX_Text(pDX, IDC_EDIT_VALUE1, m_Value1);
    DDX_Text(pDX, IDC_EDIT_VALUE2, m_Value2);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgShortArtpartControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgShortArtpartControl)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_ENTITY_ID, OnEndeditEntityID)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_ID, OnSelchangeEntityID)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_PART_ID1, OnSelchangePartID1)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_PART_ID1, OnEndeditPartID1)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_PART_ID2, OnSelchangePartID2)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_PART_ID2, OnEndeditPartID2)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgShortArtpartControl message handlers

BOOL CPacketDlgShortArtpartControl::OnInitDialog()
{
    OldDialog::OnInitDialog();

    // Set the image lists for the combo boxes.
    m_ComboEntityID.SetImageList(&g_ImgListTypes);

    PopulateEntityIDCombo();
    SelectEntityIDComboItem();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgShortArtpartControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgShortArtpartControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_ARTPART_CONTROL_S ac = {0};
    ac.packet_id = CIGI_ARTPART_CONTROL_S_OPCODE;
    ac.packet_size = sizeof(CIGI_ARTPART_CONTROL_S);
    ac.entity_id = m_EntityID;
    ac.artpart1_id = m_PartID1;
    ac.artpart1_enable = m_Enable1;
    ac.dof1_select = m_Dof1;
    ac.dof1 = m_Value1;
    ac.artpart2_id = m_PartID2;
    ac.artpart2_enable = m_Enable2;
    ac.dof2_select = m_Dof2;
    ac.dof2 = m_Value2;

    SendImmedCigiMessage(&ac);

    // Update the articulated part object.
    ApplyArtPart1();
    ApplyArtPart2();
}

void CPacketDlgShortArtpartControl::OnSelchangeEntityID()
{
    PopulatePartIDCombos();

    // Get the ID of the selected entity.
    int idx = m_ComboEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboEntityID.GetItemData(idx));
}

void CPacketDlgShortArtpartControl::OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult)
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
        PopulatePartIDCombos(); // Normally done by OnSelchangeEntityID().

    *pResult = 0;
}

void CPacketDlgShortArtpartControl::OnSelchangePartID1()
{
    // Get the type of the selected parent.
    int idx = m_ComboPartID1.GetCurSel();
    if (idx > -1)
        m_PartID1 = MAKE_ID(m_ComboPartID1.GetItemData(idx));
}

void CPacketDlgShortArtpartControl::OnEndeditPartID1(NMHDR *pNMHDR, LRESULT *pResult)
{
    int new_id = atoi(((NMCBEENDEDIT *)pNMHDR)->szText);
    if (new_id == m_PartID1)
        return;

    m_PartID1 = new_id;
    int idx = FindComboItem(m_ComboPartID1, m_PartID1);
    if ((idx == -1) && (m_PartID1 == -1))
        idx = 0;

    // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
    // select no item after it has finished its stuff.
    if (idx > -1)
        ::PostMessage(m_ComboPartID1.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

    // Post a CBN_SELCHANGE notification so OnSelchangePartID() will be called.
    // The function must be called after the CB_SELCURSEL message is processed
    // so the new item will be selected.
    ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                  MAKEWPARAM(IDC_COMBOBOXEX_PART_ID1, CBN_SELCHANGE),
                  (LPARAM)m_ComboPartID1.GetSafeHwnd());

    *pResult = 0;
}

void CPacketDlgShortArtpartControl::OnSelchangePartID2()
{
    // Get the type of the selected parent.
    int idx = m_ComboPartID2.GetCurSel();
    if (idx > -1)
        m_PartID2 = MAKE_ID(m_ComboPartID2.GetItemData(idx));
}

void CPacketDlgShortArtpartControl::OnEndeditPartID2(NMHDR *pNMHDR, LRESULT *pResult)
{
    int new_id = atoi(((NMCBEENDEDIT *)pNMHDR)->szText);
    if (new_id == m_PartID2)
        return;

    m_PartID2 = new_id;
    int idx = FindComboItem(m_ComboPartID2, m_PartID2);
    if ((idx == -1) && (m_PartID2 == -1))
        idx = 0;

    // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
    // select no item after it has finished its stuff.
    if (idx > -1)
        ::PostMessage(m_ComboPartID2.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

    // Post a CBN_SELCHANGE notification so OnSelchangePartID() will be called.
    // The function must be called after the CB_SELCURSEL message is processed
    // so the new item will be selected.
    ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                  MAKEWPARAM(IDC_COMBOBOXEX_PART_ID2, CBN_SELCHANGE),
                  (LPARAM)m_ComboPartID2.GetSafeHwnd());

    *pResult = 0;
}

void CPacketDlgShortArtpartControl::PopulateEntityIDCombo(void)
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

void CPacketDlgShortArtpartControl::SelectEntityIDComboItem(void)
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity) {
        m_ComboEntityID.SetCurSel(-1);
        m_ComboPartID1.SetCurSel(-1);
        m_ComboPartID2.SetCurSel(-1);

        return;
    }

    m_EntityID = entity->GetID();
    m_ComboEntityID.SetCurSel(FindComboItem(m_ComboEntityID, m_EntityID));
    OnSelchangeEntityID();
}

void CPacketDlgShortArtpartControl::PopulatePartIDCombos(void)
{
    char text[256] = {"None"};
    int entity_id = -1;
    int part_id = -1;
    CArtPart *artpart = NULL;
    CEntity *entity = g_DataManager.GetEntity(GetEntityIDFromCombo());
    COMBOBOXEXITEM cboitem = {  CBEIF_LPARAM | CBEIF_TEXT,
                                0, NULL, 1, 0, 0, 0, 0, -1
                             };

    m_ComboPartID1.ResetContent();
    m_ComboPartID2.ResetContent();

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
        cboitem.iItem = FindComboInsertPoint(m_ComboPartID1, artpart->GetID());
        m_ComboPartID1.InsertItem(&cboitem);
        m_ComboPartID2.InsertItem(&cboitem);
    }
}

int CPacketDlgShortArtpartControl::GetEntityIDFromCombo(void)
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

int CPacketDlgShortArtpartControl::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgShortArtpartControl::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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

void CPacketDlgShortArtpartControl::ApplyArtPart1(void)
{
    CEntity *entity = g_DataManager.GetEntity(m_EntityID);
    if (entity) {
        CArtPart *artpart = entity->GetArtPart(m_PartID1);
        if (artpart) {
            switch (m_Dof1) {
            case 1:     // X Offset
                artpart->SetXOffset(m_Value1);
                artpart->SetXEnable(m_Enable1);
                break;

            case 2:     // Y Offset
                artpart->SetYOffset(m_Value1);
                artpart->SetYEnable(m_Enable1);
                break;

            case 3:     // Z Offset
                artpart->SetZOffset(m_Value1);
                artpart->SetZEnable(m_Enable1);
                break;

            case 4:     // Yaw
                artpart->SetYaw(m_Value1);
                artpart->SetYawEnable(m_Enable1);
                break;

            case 5:     // Pitch
                artpart->SetPitch(m_Value1);
                artpart->SetPitchEnable(m_Enable1);
                break;

            case 6:     // Roll
                artpart->SetRoll(m_Value1);
                artpart->SetRollEnable(m_Enable1);
                break;

            case 0:     // Not used
            default:
                break;
            }
        }
    }
}

void CPacketDlgShortArtpartControl::ApplyArtPart2(void)
{
    CEntity *entity = g_DataManager.GetEntity(m_EntityID);
    if (entity) {
        CArtPart *artpart = entity->GetArtPart(m_PartID2);
        if (artpart) {
            switch (m_Dof2) {
            case 1:     // X Offset
                artpart->SetXOffset(m_Value2);
                artpart->SetXEnable(m_Enable2);
                break;

            case 2:     // Y Offset
                artpart->SetYOffset(m_Value2);
                artpart->SetYEnable(m_Enable2);
                break;

            case 3:     // Z Offset
                artpart->SetZOffset(m_Value2);
                artpart->SetZEnable(m_Enable2);
                break;

            case 4:     // Yaw
                artpart->SetYaw(m_Value2);
                artpart->SetYawEnable(m_Enable2);
                break;

            case 5:     // Pitch
                artpart->SetPitch(m_Value2);
                artpart->SetPitchEnable(m_Enable2);
                break;

            case 6:     // Roll
                artpart->SetRoll(m_Value2);
                artpart->SetRollEnable(m_Enable2);
                break;

            case 0:     // Not used
            default:
                break;
            }
        }
    }
}
