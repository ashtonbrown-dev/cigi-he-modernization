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

// PacketDlgLosSegmentReq.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "PacketDlgLosSegmentReq.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgLosSegmentReq dialog

CPacketDlgLosSegmentReq::CPacketDlgLosSegmentReq(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgLosSegmentReq::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgLosSegmentReq)
    m_AlphaThreshold = 0;
    m_MaterialMask = 0;
    m_ReqType = 0;
    m_RespCoordSystem = 0;
    m_SrcCoordSystem = 0;
    m_DestCoordSystem = 0;
    m_SrcLatX = 0.0;
    m_SrcLonY = 0.0;
    m_SrcAltZ = 0.0;
    m_DestLatX = 0.0;
    m_DestLonY = 0.0;
    m_DestAltZ = 0.0;
    m_LosID = 0;
    m_UpdatePeriod = 0;
    m_DestEntityID = 0;
    m_DestEntityIDValid = FALSE;
    //}}AFX_DATA_INIT
}

void CPacketDlgLosSegmentReq::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgLosSegmentReq)
    DDX_Control(pDX, IDC_CHECK_DEST_VALID, m_CheckDestEntityIDValid);
    DDX_Control(pDX, IDC_EDIT_DEST_ENTITY_ID, m_EditDestEntityID);
    DDX_Control(pDX, IDC_EDIT_UPDATE_PERIOD, m_EditUpdatePeriod);
    DDX_Control(pDX, IDC_LABEL_SRC_LON_Y_UNITS, m_LabelSrcLonYUnits);
    DDX_Control(pDX, IDC_LABEL_SRC_LAT_X_UNITS, m_LabelSrcLatXUnits);
    DDX_Control(pDX, IDC_LABEL_DEST_LON_Y_UNITS, m_LabelDestLonYUnits);
    DDX_Control(pDX, IDC_LABEL_DEST_LAT_X_UNITS, m_LabelDestLatXUnits);
    DDX_Control(pDX, IDC_LABEL_SRC_ALT_Z, m_LabelSrcAltZ);
    DDX_Control(pDX, IDC_LABEL_SRC_LON_Y, m_LabelSrcLonY);
    DDX_Control(pDX, IDC_LABEL_SRC_LAT_X, m_LabelSrcLatX);
    DDX_Control(pDX, IDC_LABEL_DEST_LON_Y, m_LabelDestLonY);
    DDX_Control(pDX, IDC_LABEL_DEST_LAT_X, m_LabelDestLatX);
    DDX_Control(pDX, IDC_LABEL_DEST_ALT_Z, m_LabelDestAltZ);
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_ID, m_ComboEntityID);
    DDX_Control(pDX, IDC_COMBOBOXEX_DESTINATION_ID, m_ComboDestinationEntityID);
    DDX_Text(pDX, IDC_EDIT_ALPHA_THRESHOLD, m_AlphaThreshold);
    DDV_MinMaxInt(pDX, m_AlphaThreshold, 0, 255);
    DDX_Text(pDX, IDC_EDIT_MATERIAL_MASK, m_MaterialMask);
    DDX_Radio(pDX, IDC_RADIO_TYPE_BASIC, m_ReqType);
    DDX_Radio(pDX, IDC_RADIO_RESP_GEODETIC, m_RespCoordSystem);
    DDX_Radio(pDX, IDC_RADIO_SRC_GEODETIC, m_SrcCoordSystem);
    DDX_Radio(pDX, IDC_RADIO_DEST_GEODETIC, m_DestCoordSystem);
    DDX_Text(pDX, IDC_EDIT_SRC_LAT_X, m_SrcLatX);
    DDX_Text(pDX, IDC_EDIT_SRC_LON_Y, m_SrcLonY);
    DDX_Text(pDX, IDC_EDIT_SRC_ALT_Z, m_SrcAltZ);
    DDX_Text(pDX, IDC_EDIT_DEST_LAT_X, m_DestLatX);
    DDX_Text(pDX, IDC_EDIT_DEST_LON_Y, m_DestLonY);
    DDX_Text(pDX, IDC_EDIT_DEST_ALT_Z, m_DestAltZ);
    DDX_Text(pDX, IDC_EDIT_LOS_ID, m_LosID);
    DDV_MinMaxInt(pDX, m_LosID, 0, 65535);
    DDX_Text(pDX, IDC_EDIT_UPDATE_PERIOD, m_UpdatePeriod);
    DDX_Text(pDX, IDC_EDIT_DEST_ENTITY_ID, m_DestEntityID);
    DDX_Check(pDX, IDC_CHECK_DEST_VALID, m_DestEntityIDValid);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgLosSegmentReq, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgLosSegmentReq)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_ID, OnSelchangeEntityId)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_ENTITY_ID, OnEndeditEntityId)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_DESTINATION_ID, OnSelchangeDestinationEntityId)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_DESTINATION_ID, OnEndeditDestinationEntityId)
    ON_BN_CLICKED(IDC_RADIO_DEST_ENTITY, OnRadioDestEntity)
    ON_BN_CLICKED(IDC_RADIO_DEST_GEODETIC, OnRadioDestGeodetic)
    ON_BN_CLICKED(IDC_RADIO_RESP_ENTITY, OnRadioRespEntity)
    ON_BN_CLICKED(IDC_RADIO_RESP_GEODETIC, OnRadioRespGeodetic)
    ON_BN_CLICKED(IDC_RADIO_SRC_ENTITY, OnRadioSrcEntity)
    ON_BN_CLICKED(IDC_RADIO_SRC_GEODETIC, OnRadioSrcGeodetic)
    ON_BN_CLICKED(IDC_RADIO_TYPE_BASIC, OnRadioTypeBasic)
    ON_BN_CLICKED(IDC_RADIO_TYPE_EXT, OnRadioTypeExt)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgLosSegmentReq message handlers

void CPacketDlgLosSegmentReq::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgLosSegmentReq::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_LOS_SEGMENT_REQUEST los = {0};
    los.packet_id = CIGI_LOS_SEGMENT_REQUEST_OPCODE;
    los.packet_size = sizeof(CIGI_LOS_SEGMENT_REQUEST);
    los.source_entity_id = GetEntityIDFromCombo();
    los.los_id = m_LosID;
    los.request_type = m_ReqType;
    los.response_coord_system = m_RespCoordSystem;
    los.source_coord_system = m_SrcCoordSystem;
    los.dest_entity_id_valid = m_DestEntityIDValid ? 1 : 0;
    los.source_lat_x = m_SrcLatX;
    los.source_lon_y = m_SrcLonY;
    los.source_alt_z = m_SrcAltZ;
    los.dest_coord_system = m_DestCoordSystem;
    los.dest_lat_x = m_DestLatX;
    los.dest_lon_y = m_DestLonY;
    los.dest_alt_z = m_DestAltZ;
    los.alpha_threshold = m_AlphaThreshold;
    los.material_mask = m_MaterialMask;
    los.update_period = (unsigned char)m_UpdatePeriod;
    los.dest_entity_id = GetDestinationEntityIDFromCombo();

    SendImmedCigiMessage(&los);

    ApplyLos();
}

BOOL CPacketDlgLosSegmentReq::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_ComboEntityID.SetImageList(&g_ImgListTypes);
    PopulateEntityIDCombo();

    OnRadioRespGeodetic();
    OnRadioSrcGeodetic();
    OnRadioDestGeodetic();

        m_EditDestEntityID.EnableWindow(TRUE);
        m_EditUpdatePeriod.EnableWindow(TRUE);
        m_CheckDestEntityIDValid.EnableWindow(TRUE);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgLosSegmentReq::OnRadioDestEntity()
{
    m_LabelDestLatX.SetWindowText("X Offset:");
    m_LabelDestLonY.SetWindowText("Y Offset:");
    m_LabelDestAltZ.SetWindowText("Z Offset:");
    m_LabelDestLatXUnits.SetWindowText("m");
    m_LabelDestLonYUnits.SetWindowText("m");
}

void CPacketDlgLosSegmentReq::OnRadioDestGeodetic()
{
    m_LabelDestLatX.SetWindowText("Latitude:");
    m_LabelDestLonY.SetWindowText("Longitude:");
    m_LabelDestAltZ.SetWindowText("Altitude:");
    m_LabelDestLatXUnits.SetWindowText("deg");
    m_LabelDestLonYUnits.SetWindowText("deg");
}

void CPacketDlgLosSegmentReq::OnRadioRespEntity()
{
}

void CPacketDlgLosSegmentReq::OnRadioRespGeodetic()
{
}

void CPacketDlgLosSegmentReq::OnRadioSrcEntity()
{
    m_LabelSrcLatX.SetWindowText("X Offset:");
    m_LabelSrcLonY.SetWindowText("Y Offset:");
    m_LabelSrcAltZ.SetWindowText("Z Offset:");
    m_LabelSrcLatXUnits.SetWindowText("m");
    m_LabelSrcLonYUnits.SetWindowText("m");
}

void CPacketDlgLosSegmentReq::OnRadioSrcGeodetic()
{
    m_LabelSrcLatX.SetWindowText("Latitude:");
    m_LabelSrcLonY.SetWindowText("Longitude:");
    m_LabelSrcAltZ.SetWindowText("Altitude:");
    m_LabelSrcLatXUnits.SetWindowText("deg");
    m_LabelSrcLonYUnits.SetWindowText("deg");
}

void CPacketDlgLosSegmentReq::OnRadioTypeBasic()
{
}

void CPacketDlgLosSegmentReq::OnRadioTypeExt()
{
}

void CPacketDlgLosSegmentReq::ApplyLos(void)
{
    // Because our numbering is slightly different
    // from the actual IDs, don't worry about doing
    // this.  We mightupdate it later if there is time.
}

void CPacketDlgLosSegmentReq::OnSelchangeEntityId()
{
    // Get the ID of the selected entity.
    int idx = m_ComboEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboEntityID.GetItemData(idx));
}

void CPacketDlgLosSegmentReq::OnEndeditEntityId(NMHDR *pNMHDR, LRESULT *pResult)
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

void CPacketDlgLosSegmentReq::PopulateEntityIDCombo(void)
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

int CPacketDlgLosSegmentReq::GetEntityIDFromCombo(void)
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

int CPacketDlgLosSegmentReq::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgLosSegmentReq::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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
/*
 * Destination stuff
 */
void CPacketDlgLosSegmentReq::OnSelchangeDestinationEntityId()
{
    // Get the ID of the selected entity.
    int idx = m_ComboDestinationEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboDestinationEntityID.GetItemData(idx));
}

void CPacketDlgLosSegmentReq::OnEndeditDestinationEntityId(NMHDR *pNMHDR, LRESULT *pResult)
{
    int new_id = atoi(((NMCBEENDEDIT *)pNMHDR)->szText);
    if (new_id == m_EntityID)
        return;

    m_EntityID = new_id;
    int idx = FindComboItem(m_ComboDestinationEntityID, m_EntityID);
    if (idx > -1) {
        // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
        // select no item after it has finished its stuff.
        ::PostMessage(m_ComboDestinationEntityID.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

        // Post a CBN_SELCHANGE notification so OnSelchangeEntityID() will be called.
        // The function must be called after the CB_SELCURSEL message is processed
        // so the new item will be selected.
        ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                      MAKEWPARAM(IDC_COMBOBOXEX_ENTITY_ID, CBN_SELCHANGE),
                      (LPARAM)m_ComboDestinationEntityID.GetSafeHwnd());
    }

    *pResult = 0;
}

int CPacketDlgLosSegmentReq::GetDestinationEntityIDFromCombo(void)
{
    int itemdata = m_ComboDestinationEntityID.GetItemData(m_ComboDestinationEntityID.GetCurSel());
    if (itemdata != -1)
        return MAKE_ID(itemdata);
    else {
        CString text;
        m_ComboDestinationEntityID.GetWindowText(text);
        return atoi((LPCTSTR)text);
    }
}

