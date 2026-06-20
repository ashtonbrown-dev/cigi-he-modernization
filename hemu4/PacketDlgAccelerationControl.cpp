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

// PacketDlgTrajectoryDef.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "PacketDlgAccelerationControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgAccelerationControl dialog

CPacketDlgAccelerationControl::CPacketDlgAccelerationControl(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgAccelerationControl::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgAccelerationControl)
    m_ApplyToArtPart = FALSE;
    m_AccelX = 0.0f;
    m_AccelY = 0.0f;
    m_AccelZ = 0.0f;
    m_AccelYaw = 0.0f;
    m_AccelPitch = 0.0f;
    m_AccelRoll = 0.0f;
    m_CoordSystem = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgAccelerationControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgAccelerationControl)
    DDX_Control(pDX, IDC_COMBOBOXEX_PART_ID, m_ComboPartID);
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_ID, m_ComboEntityID);
    DDX_Check(pDX, IDC_CHECK_ENABLE_ARTPART, m_ApplyToArtPart);
    DDX_Text(pDX, IDC_EDIT_YAW_ACCEL, m_AccelYaw);
    DDX_Text(pDX, IDC_EDIT_PITCH_ACCEL, m_AccelPitch);
    DDX_Text(pDX, IDC_EDIT_ROLL_ACCEL, m_AccelRoll);
    DDX_Text(pDX, IDC_EDIT_X, m_AccelX);
    DDX_Text(pDX, IDC_EDIT_Y, m_AccelY);
    DDX_Text(pDX, IDC_EDIT_Z, m_AccelZ);
    DDX_Radio(pDX, IDC_RADIO_PARENT, m_CoordSystem);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgAccelerationControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgAccelerationControl)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgAccelerationControl message handlers

void CPacketDlgAccelerationControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgAccelerationControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_ACCELERATION_CONTROL_DEF td = {0};
    td.packet_id = CIGI_ACCELERATION_CONTROL_DEF_OPCODE;
    td.packet_size = sizeof(CIGI_ACCELERATION_CONTROL_DEF);
    td.entity_id = GetEntityIDFromCombo();
	td.artpart_id = GetPartIDFromCombo();
	td.artpart_apply = m_ApplyToArtPart;
    td.accel_x = m_AccelX;
    td.accel_y = m_AccelY;
    td.accel_z = m_AccelZ;
    td.accel_yaw = m_AccelYaw;
    td.accel_pitch = m_AccelPitch;
    td.accel_roll = m_AccelRoll;
	td.coord_system = m_CoordSystem;

    SendImmedCigiMessage(&td);
}

BOOL CPacketDlgAccelerationControl::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the image lists for the combo boxes.
    m_ComboEntityID.SetImageList(&g_ImgListTypes);

    PopulateEntityIDCombo();
    m_ComboEntityID.SetCurSel(0);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgAccelerationControl::PopulateEntityIDCombo(void)
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

int CPacketDlgAccelerationControl::GetEntityIDFromCombo(void)
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

int CPacketDlgAccelerationControl::GetPartIDFromCombo(void)
{
    int itemdata = m_ComboPartID.GetItemData(m_ComboPartID.GetCurSel());
    if (itemdata != -1)
        return MAKE_ID(itemdata);
    else {
        CString text;
        m_ComboPartID.GetWindowText(text);
        return atoi((LPCTSTR)text);
    }
}

int CPacketDlgAccelerationControl::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgAccelerationControl::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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
