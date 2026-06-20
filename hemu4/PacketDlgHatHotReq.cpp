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

// PacketDlgHatHotReq.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "PacketDlgHatHotReq.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgHatHotReq dialog

CPacketDlgHatHotReq::CPacketDlgHatHotReq(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgHatHotReq::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgHatHotReq)
    m_AltZ = 0.0;
    m_LatX = 0.0;
    m_LonY = 0.0;
    m_HatHotID = 0;
    m_ReqType = 0;
    m_CoordSystem = 0;
    m_UpdatePeriod = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgHatHotReq::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgHatHotReq)
    DDX_Control(pDX, IDC_EDIT_UPDATE_PERIOD, m_EditUpdatePeriod);
    DDX_Control(pDX, IDC_LABEL_LON_Y_UNITS, m_LabelLonYUnits);
    DDX_Control(pDX, IDC_LABEL_LAT_X_UNITS, m_LabelLatXUnits);
    DDX_Control(pDX, IDC_LABEL_LON_Y, m_LabelLonY);
    DDX_Control(pDX, IDC_LABEL_LAT_X, m_LabelLatX);
    DDX_Control(pDX, IDC_LABEL_ALT_Z, m_LabelAltZ);
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_ID, m_ComboEntityID);
    DDX_Text(pDX, IDC_EDIT_ALT_Z, m_AltZ);
    DDX_Text(pDX, IDC_EDIT_LAT_X, m_LatX);
    DDX_Text(pDX, IDC_EDIT_LON_Y, m_LonY);
    DDX_Text(pDX, IDC_EDIT_HAT_ID, m_HatHotID);
    DDV_MinMaxInt(pDX, m_HatHotID, 0, 65535);
    DDX_Radio(pDX, IDC_RADIO_TYPE_HAT, m_ReqType);
    DDX_Radio(pDX, IDC_RADIO_CS_GEODETIC, m_CoordSystem);
    DDX_Text(pDX, IDC_EDIT_UPDATE_PERIOD, m_UpdatePeriod);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgHatHotReq, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgHatHotReq)
    ON_BN_CLICKED(IDC_RADIO_CS_ENTITY, OnRadioCoordSysEntity)
    ON_BN_CLICKED(IDC_RADIO_CS_GEODETIC, OnRadioCoordSysGeodetic)
    ON_BN_CLICKED(IDC_RADIO_TYPE_HAT, OnRadioTypeHat)
    ON_BN_CLICKED(IDC_RADIO_TYPE_HOT, OnRadioTypeHot)
    ON_BN_CLICKED(IDC_RADIO_TYPE_EXT, OnRadioTypeExt)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_ID, OnSelchangeEntityId)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_ENTITY_ID, OnEndeditEntityId)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgHatHotReq message handlers

void CPacketDlgHatHotReq::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgHatHotReq::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_HAT_HOT_REQUEST hr = {0};
    hr.packet_id = CIGI_HAT_HOT_REQUEST_OPCODE;
    hr.packet_size = sizeof(CIGI_HAT_HOT_REQUEST);
    hr.entity_id = GetEntityIDFromCombo();
    hr.hat_hot_id = m_HatHotID;
    hr.request_type = m_ReqType;
    hr.coord_system = m_CoordSystem;
    hr.update_period = (unsigned char)m_UpdatePeriod;
    hr.lat_x = m_LatX;
    hr.lon_y = m_LonY;
    hr.alt_z = m_AltZ;

    SendImmedCigiMessage(&hr);

    ApplyHatHot();
}

BOOL CPacketDlgHatHotReq::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_ComboEntityID.SetImageList(&g_ImgListTypes);
    PopulateEntityIDCombo();

    OnRadioCoordSysGeodetic();

    m_EditUpdatePeriod.EnableWindow(TRUE);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgHatHotReq::OnRadioCoordSysEntity()
{
    m_LabelLatX.SetWindowText("X Offset:");
    m_LabelLonY.SetWindowText("Y Offset:");
    m_LabelAltZ.SetWindowText("Z Offset:");
    m_LabelLatXUnits.SetWindowText("m");
    m_LabelLonYUnits.SetWindowText("m");
}

void CPacketDlgHatHotReq::OnRadioCoordSysGeodetic()
{
    m_LabelLatX.SetWindowText("Latitude:");
    m_LabelLonY.SetWindowText("Longitude:");
    m_LabelAltZ.SetWindowText("Altitude:");
    m_LabelLatXUnits.SetWindowText("deg");
    m_LabelLonYUnits.SetWindowText("deg");
}

void CPacketDlgHatHotReq::OnRadioTypeHat()
{
}

void CPacketDlgHatHotReq::OnRadioTypeHot()
{
}

void CPacketDlgHatHotReq::OnRadioTypeExt()
{
}

void CPacketDlgHatHotReq::ApplyHatHot(void)
{
    // Because our numbering is slightly different
    // from the actual IDs, don't worry about doing
    // this.  We mightupdate it later if there is time.
}

void CPacketDlgHatHotReq::OnSelchangeEntityId()
{
    // Get the ID of the selected entity.
    int idx = m_ComboEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboEntityID.GetItemData(idx));
}

void CPacketDlgHatHotReq::OnEndeditEntityId(NMHDR *pNMHDR, LRESULT *pResult)
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

void CPacketDlgHatHotReq::PopulateEntityIDCombo(void)
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

int CPacketDlgHatHotReq::GetEntityIDFromCombo(void)
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

int CPacketDlgHatHotReq::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgHatHotReq::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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
