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

// PacketDlgCdVolumeDef.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "PacketDlgCdVolumeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgCdVolumeDef dialog

CPacketDlgCdVolumeDef::CPacketDlgCdVolumeDef(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgCdVolumeDef::IDD, pParent)
{
    m_EntityID = -1;

    //{{AFX_DATA_INIT(CPacketDlgCdVolumeDef)
    m_VolumeEnable = FALSE;
    m_VolumeID = 0;
    m_Depth = 0.0f;
    m_HeightOrRadius = 0.0f;
    m_Width = 0.0f;
    m_Pitch = 0.0f;
    m_Roll = 0.0f;
    m_Yaw = 0.0f;
    m_X = 0.0f;
    m_Y = 0.0f;
    m_Z = 0.0f;
    m_VolumeType = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgCdVolumeDef::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgCdVolumeDef)
    DDX_Control(pDX, IDC_LABEL_HEIGHT_RADIUS, m_LabelHeightOrRadius);
    DDX_Control(pDX, IDC_EDIT_DEPTH, m_EditDepth);
    DDX_Control(pDX, IDC_EDIT_WIDTH, m_EditWidth);
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_ID, m_ComboEntityID);
    DDX_Check(pDX, IDC_CHECK_ENABLE_VOLUME, m_VolumeEnable);
    DDX_Text(pDX, IDC_EDIT_VOLUME_ID, m_VolumeID);
    DDV_MinMaxInt(pDX, m_VolumeID, 0, 255);
    DDX_Text(pDX, IDC_EDIT_DEPTH, m_Depth);
    DDX_Text(pDX, IDC_EDIT_HEIGHT_RADIUS, m_HeightOrRadius);
    DDX_Text(pDX, IDC_EDIT_WIDTH, m_Width);
    DDX_Text(pDX, IDC_EDIT_PITCH, m_Pitch);
    DDX_Text(pDX, IDC_EDIT_ROLL, m_Roll);
    DDX_Text(pDX, IDC_EDIT_YAW, m_Yaw);
    DDX_Text(pDX, IDC_EDIT_X, m_X);
    DDX_Text(pDX, IDC_EDIT_Y, m_Y);
    DDX_Text(pDX, IDC_EDIT_Z, m_Z);
    DDX_Radio(pDX, IDC_RADIO_TYPE_SPHERE, m_VolumeType);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgCdVolumeDef, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgCdVolumeDef)
    ON_BN_CLICKED(IDC_RADIO_TYPE_SPHERE, OnRadioTypeSphere)
    ON_BN_CLICKED(IDC_RADIO_TYPE_CUBOID, OnRadioTypeCuboid)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_ID, OnSelchangeEntityId)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_ENTITY_ID, OnEndeditEntityId)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgCdVolumeDef message handlers

void CPacketDlgCdVolumeDef::OnRadioTypeSphere()
{
    m_LabelHeightOrRadius.SetWindowText("Radius:");
    m_EditDepth.EnableWindow(FALSE);
    m_EditWidth.EnableWindow(FALSE);
}

void CPacketDlgCdVolumeDef::OnRadioTypeCuboid()
{
    m_LabelHeightOrRadius.SetWindowText("Height:");
    m_EditDepth.EnableWindow(TRUE);
    m_EditWidth.EnableWindow(TRUE);
}

void CPacketDlgCdVolumeDef::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgCdVolumeDef::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_COLL_VOLUME_DEF cd = {0};
    cd.packet_id = CIGI_COLL_VOLUME_DEF_OPCODE;
    cd.packet_size = sizeof(CIGI_COLL_VOLUME_DEF);
    cd.entity_id = GetEntityIDFromCombo();
    cd.volume_id = m_VolumeID;
    cd.volume_type = m_VolumeType;
    cd.volume_enable = m_VolumeEnable;
    cd.x = m_X;
    cd.y = m_Y;
    cd.z = m_Z;
    cd.height_radius = m_HeightOrRadius;
    cd.width = m_Width;
    cd.depth = m_Depth;
    cd.yaw = m_Yaw;
    cd.pitch = m_Pitch;
    cd.roll = m_Roll;

    SendImmedCigiMessage(&cd);

    ApplyVolume();
}

BOOL CPacketDlgCdVolumeDef::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_ComboEntityID.SetImageList(&g_ImgListTypes);
    PopulateEntityIDCombo();

    OnRadioTypeSphere();

    RefreshView();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CPacketDlgCdVolumeDef::RefreshView(void)
{
    CEntity *entity = g_DataManager.GetEntity(m_EntityID);
    if (!entity)
        return;

    CCollisionVol *vol = entity->GetCollisionVol(m_VolumeID);
    if (!vol)
        return;

    m_VolumeEnable = vol->GetEnable();
    m_Depth = vol->GetDepth();;
    m_HeightOrRadius = vol->GetHeightOrRadius();
    m_Width = vol->GetWidth();
    m_Pitch = vol->GetPitch();
    m_Roll = vol->GetRoll();
    m_Yaw = vol->GetYaw();
    m_X = vol->GetX();
    m_Y = vol->GetY();
    m_Z = vol->GetZ();
    m_VolumeType = vol->GetType();

    UpdateData(FALSE);
}

void CPacketDlgCdVolumeDef::ApplyVolume(void)
{
    // Because our collision volume numbering is slightly different
    // from the actual IDs, don't worry about doing this.  We might
    // update it later if there is time.
}

void CPacketDlgCdVolumeDef::OnSelchangeEntityId()
{
    // Get the ID of the selected entity.
    int idx = m_ComboEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboEntityID.GetItemData(idx));
}

void CPacketDlgCdVolumeDef::OnEndeditEntityId(NMHDR *pNMHDR, LRESULT *pResult)
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

void CPacketDlgCdVolumeDef::PopulateEntityIDCombo(void)
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

int CPacketDlgCdVolumeDef::GetEntityIDFromCombo(void)
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

int CPacketDlgCdVolumeDef::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgCdVolumeDef::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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
