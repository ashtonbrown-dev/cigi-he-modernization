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

// PacketDlgLosVectorReq.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "PacketDlgLosVectorReq.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgLosVectorReq dialog


CPacketDlgLosVectorReq::CPacketDlgLosVectorReq(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgLosVectorReq::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgLosVectorReq)
    m_AlphaThreshold = 0;
    m_Azimuth = 0.0f;
    m_Elevation = 0.0f;
    m_LosID = 0;
    m_MaterialMask = 0;
    m_MaxRange = 0.0f;
    m_MinRange = 0.0f;
    m_SrcAltZ = 0.0;
    m_SrcLatX = 0.0;
    m_SrcLonY = 0.0;
    m_RespCoordSystem = 0;
    m_SrcCoordSystem = 0;
    m_ReqType = 0;
    m_UpdatePeriod = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgLosVectorReq::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgLosVectorReq)
    DDX_Control(pDX, IDC_EDIT_UPDATE_PERIOD, m_EditUpdatePeriod);
    DDX_Control(pDX, IDC_LABEL_SRC_LON_Y_UNITS, m_LabelSrcLonYUnits);
    DDX_Control(pDX, IDC_LABEL_SRC_LAT_X_UNITS, m_LabelSrcLatXUnits);
    DDX_Control(pDX, IDC_LABEL_SRC_LON_Y, m_LabelSrcLonY);
    DDX_Control(pDX, IDC_LABEL_SRC_LAT_X, m_LabelSrcLatX);
    DDX_Control(pDX, IDC_LABEL_SRC_ALT_Z, m_LabelSrcAltZ);
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_ID, m_ComboEntityID);
    DDX_Text(pDX, IDC_EDIT_ALPHA_THRESHOLD, m_AlphaThreshold);
    DDV_MinMaxInt(pDX, m_AlphaThreshold, 0, 255);
    DDX_Text(pDX, IDC_EDIT_AZIMUTH, m_Azimuth);
    DDX_Text(pDX, IDC_EDIT_ELEVATION, m_Elevation);
    DDX_Text(pDX, IDC_EDIT_LOS_ID, m_LosID);
    DDV_MinMaxInt(pDX, m_LosID, 0, 65535);
    DDX_Text(pDX, IDC_EDIT_MATERIAL_MASK, m_MaterialMask);
    DDX_Text(pDX, IDC_EDIT_MAX_RANGE, m_MaxRange);
    DDX_Text(pDX, IDC_EDIT_MIN_RANGE, m_MinRange);
    DDX_Text(pDX, IDC_EDIT_SRC_ALT_Z, m_SrcAltZ);
    DDX_Text(pDX, IDC_EDIT_SRC_LAT_X, m_SrcLatX);
    DDX_Text(pDX, IDC_EDIT_SRC_LON_Y, m_SrcLonY);
    DDX_Radio(pDX, IDC_RADIO_RESP_GEODETIC, m_RespCoordSystem);
    DDX_Radio(pDX, IDC_RADIO_SRC_GEODETIC, m_SrcCoordSystem);
    DDX_Radio(pDX, IDC_RADIO_TYPE_BASIC, m_ReqType);
    DDX_Text(pDX, IDC_EDIT_UPDATE_PERIOD, m_UpdatePeriod);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgLosVectorReq, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgLosVectorReq)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_ID, OnSelchangeEntityId)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_ENTITY_ID, OnEndeditEntityId)
    ON_BN_CLICKED(IDC_RADIO_RESP_ENTITY, OnRadioRespEntity)
    ON_BN_CLICKED(IDC_RADIO_RESP_GEODETIC, OnRadioRespGeodetic)
    ON_BN_CLICKED(IDC_RADIO_SRC_ENTITY, OnRadioSrcEntity)
    ON_BN_CLICKED(IDC_RADIO_SRC_GEODETIC, OnRadioSrcGeodetic)
    ON_BN_CLICKED(IDC_RADIO_TYPE_BASIC, OnRadioTypeBasic)
    ON_BN_CLICKED(IDC_RADIO_TYPE_EXT, OnRadioTypeExt)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgLosVectorReq message handlers

void CPacketDlgLosVectorReq::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgLosVectorReq::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_LOS_VECTOR_REQUEST los = {0};
    los.packet_id = CIGI_LOS_VECTOR_REQUEST_OPCODE;
    los.packet_size = sizeof(CIGI_LOS_VECTOR_REQUEST);
    los.entity_id = GetEntityIDFromCombo();
    los.los_id = m_LosID;
    los.request_type = m_ReqType;
    los.resp_coord_system = m_RespCoordSystem;
    los.source_coord_system = m_SrcCoordSystem;
    los.source_lat_x = m_SrcLatX;
    los.source_lon_y = m_SrcLonY;
    los.source_alt_z = m_SrcAltZ;
    los.azimuth = m_Azimuth;
    los.elevation = m_Elevation;
    los.min_range = m_MinRange;
    los.max_range = m_MaxRange;
    los.alpha_threshold = m_AlphaThreshold;
    los.material_mask = m_MaterialMask;
    los.update_period = (unsigned char)m_UpdatePeriod;

    SendImmedCigiMessage(&los);

    ApplyLos();
}

BOOL CPacketDlgLosVectorReq::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_ComboEntityID.SetImageList(&g_ImgListTypes);
    PopulateEntityIDCombo();

    OnRadioRespGeodetic();
    OnRadioSrcGeodetic();

    m_EditUpdatePeriod.EnableWindow(TRUE);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgLosVectorReq::OnRadioRespEntity()
{
}

void CPacketDlgLosVectorReq::OnRadioRespGeodetic()
{
}

void CPacketDlgLosVectorReq::OnRadioSrcEntity()
{
    m_LabelSrcLatX.SetWindowText("X Offset:");
    m_LabelSrcLonY.SetWindowText("Y Offset:");
    m_LabelSrcAltZ.SetWindowText("Z Offset:");
    m_LabelSrcLatXUnits.SetWindowText("m");
    m_LabelSrcLonYUnits.SetWindowText("m");
}

void CPacketDlgLosVectorReq::OnRadioSrcGeodetic()
{
    m_LabelSrcLatX.SetWindowText("Latitude:");
    m_LabelSrcLonY.SetWindowText("Longitude:");
    m_LabelSrcAltZ.SetWindowText("Altitude:");
    m_LabelSrcLatXUnits.SetWindowText("deg");
    m_LabelSrcLonYUnits.SetWindowText("deg");
}

void CPacketDlgLosVectorReq::OnRadioTypeBasic()
{
}

void CPacketDlgLosVectorReq::OnRadioTypeExt()
{
}

void CPacketDlgLosVectorReq::ApplyLos(void)
{
    // Because our numbering is slightly different
    // from the actual IDs, don't worry about doing
    // this.  We mightupdate it later if there is time.
}

void CPacketDlgLosVectorReq::OnSelchangeEntityId()
{
    // Get the ID of the selected entity.
    int idx = m_ComboEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboEntityID.GetItemData(idx));
}

void CPacketDlgLosVectorReq::OnEndeditEntityId(NMHDR *pNMHDR, LRESULT *pResult)
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

void CPacketDlgLosVectorReq::PopulateEntityIDCombo(void)
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

int CPacketDlgLosVectorReq::GetEntityIDFromCombo(void)
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

int CPacketDlgLosVectorReq::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgLosVectorReq::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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
