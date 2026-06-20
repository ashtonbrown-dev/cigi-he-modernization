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

// PacketDlgShortComponentControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "CigiView.h"
#include "Entity.h"
#include "globals.h"
#include "PacketDlgShortComponentControl.h"
#include "TerrainDB.h"
#include "ViewGroup.h"
#include "Weather.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgShortComponentControl dialog

CPacketDlgShortComponentControl::CPacketDlgShortComponentControl(CWnd *pParent /*=NULL*/)
    : OldDialog(CPacketDlgShortComponentControl::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgShortComponentControl)
    m_CompClass = 0;
    m_CompID = 0;
    m_CompState = 0;
    m_InstID = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgShortComponentControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgShortComponentControl)
    DDX_Control(pDX, IDC_COMBO2, m_cboData2);
    DDX_Control(pDX, IDC_COMBO1, m_cboData1);
    DDX_CBIndex(pDX, IDC_COMBOBOXEX_COMPONENT_CLASS, m_CompClass);
    DDX_Text(pDX, IDC_EDIT_COMPONENT_ID, m_CompID);
    DDX_Text(pDX, IDC_EDIT_COMPONENT_STATE, m_CompState);
    DDX_Text(pDX, IDC_EDIT_INSTANCE_ID, m_InstID);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgShortComponentControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgShortComponentControl)
    ON_MESSAGE(CBV_SELCHANGE, OnUserSelDatatype)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgShortComponentControl message handlers

BOOL CPacketDlgShortComponentControl::OnInitDialog()
{
	OldDialog::OnInitDialog();

    // Delete the 64-bit datatypes from the last data item.
    m_cboData2.DeleteString(5);
    m_cboData2.DeleteString(4);
    m_cboData2.DeleteString(3);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgShortComponentControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgShortComponentControl::OnOK()
{
    unsigned long temp[2] = {0};
    bool is32 = false;

    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_COMPONENT_CONTROL_S cc = {0};

    cc.packet_id = CIGI_COMPONENT_CONTROL_S_OPCODE;
    cc.packet_size = sizeof(CIGI_COMPONENT_CONTROL_S);
    cc.instance_id = m_InstID;
    cc.component_id = m_CompID;
    cc.component_state = m_CompState;

    switch (m_cboData1.GetDataType()) {
    case 0: // CComboBoxVariant::DataTypeInt32
        cc.data1 = m_cboData1.GetValueInt32();
        is32 = true;
        break;

    case 1: // CComboBoxVariant::DataTypeUInt32
        cc.data1 = m_cboData1.GetValueUInt32();
        is32 = true;
        break;

    case 2: // CComboBoxVariant::DataTypeFloat
        *((float *)temp) = m_cboData1.GetValueFloat();
        cc.data1 = temp[0];
        is32 = true;
        break;

    case 3: // CComboBoxVariant::DataTypeInt64
    case 4: // CComboBoxVariant::DataTypeUInt64
        *((unsigned __int64 *)temp) = m_cboData1.GetValueUInt64();
        cc.data1 = temp[1];
        cc.data2 = temp[0];
        break;

    case 5: // CComboBoxVariant::DataTypeDouble
        *((double *)temp) = m_cboData1.GetValueDouble();
        cc.data1 = temp[1];
        cc.data2 = temp[0];
        break;

    default:
        break;
    }

    if (is32) {
        switch (m_cboData2.GetDataType()) {
        case 0: // CComboBoxVariant::DataTypeInt32
            cc.data2 = m_cboData2.GetValueInt32();
            break;

        case 1: // CComboBoxVariant::DataTypeUInt32
            cc.data2 = m_cboData2.GetValueUInt32();
            break;

        case 2: // CComboBoxVariant::DataTypeFloat
            *((float *)temp) = m_cboData2.GetValueFloat();
            cc.data2 = temp[0];
            break;

        default:
            break;
        }
    }

    cc.component_class = m_CompClass;

    SendImmedCigiMessage(&cc);
}

void CPacketDlgShortComponentControl::ApplyComponent()
{
    // Changes made to this function should also be made to:
    //      CPacketDlgComponentControl::ApplyComponent().

    CComponent *comp = NULL;

    switch (m_CompClass) {
    case COMP_CLASS_ENTITY: {
        CEntity *entity = g_DataManager.GetEntity(m_InstID);
        if (entity)
            comp = entity->GetComponent(m_CompID);

        break;
    }

    case COMP_CLASS_VIEW: {
        CCigiView *view = g_DataManager.GetView(m_InstID);
        if (view)
            comp = view->GetComponent(m_CompID);

        break;
    }

    case COMP_CLASS_VIEWGROUP: {
        CViewGroup *group = g_DataManager.GetViewGroup(m_InstID);
        if (group)
            comp = group->GetComponent(m_CompID);

        break;
    }

    case COMP_CLASS_SENSOR:
        break;

    case COMP_CLASS_R_SEA:
        break;

    case COMP_CLASS_R_TERRAIN:
        break;

    case COMP_CLASS_R_WEATHER:
        break;

    case COMP_CLASS_G_SEA: {
        comp = g_DataManager.GetGlobalEnv()->GetSeaSurfaceComponent(m_CompID, m_InstID);
        break;
    }

    case COMP_CLASS_G_TERRAIN: {
        // First search the database-specific components.
        CTerrainDB *db = g_DataManager.GetSelectedDatabase();
        if (db)
            comp = db->GetComponent(m_CompID, m_InstID);

        // Then search the universal terrain components.
        if (comp == NULL)
            comp = g_DataManager.GetGlobalEnv()->GetTerrainComponent(m_CompID, m_InstID);

        break;
    }

    case COMP_CLASS_G_WEATHER: {
        CWeather *weather = g_DataManager.GetWeatherLayer(m_InstID);
        if (weather)
            comp = weather->GetComponent(m_CompID);

        break;
    }

    case COMP_CLASS_ATMOSPHERE: {
        comp = g_DataManager.GetGlobalEnv()->GetAtmosphereComponent(m_CompID, m_InstID);
        break;
    }

    case COMP_CLASS_CELSPHERE: {
        comp = g_DataManager.GetGlobalEnv()->GetCelestialComponent(m_CompID, m_InstID);
        break;
    }

    case COMP_CLASS_EVENT:
        break;

    case COMP_CLASS_SYSTEM:
        break;

    default:
        break;
    }
}

/// This function will detect the type (size) of data that the user has selected for
/// component data fields. When the user selects a 64 bit sized type for a field,
/// the field immediately following will be disabled. (Since fields are 32 bits, a 64
/// bit value will of course use the combined space of two fields.)
LRESULT CPacketDlgShortComponentControl::OnUserSelDatatype(WPARAM wParam, LPARAM lParam)
{
    CComboBoxVariant *cbo = (CComboBoxVariant *)lParam;
    CComboBoxVariant *nextDataField = NULL;

    // Determine the data field immediately following the current, if it exists.
    if (cbo == &m_cboData1)
        nextDataField = &m_cboData2;

    if (nextDataField) {
        int datatype = (int)wParam;
        BOOL enableNextDataField;

        switch (datatype) {
        case 3: // DataTypeInt64
        case 4: // DataTypeUInt64
        case 5: // DataTypeDouble
            // If the current combo item is disabled, then we want to enable
            // the combo item that follows. Otherwise, the current combo item
            // would be an enabled 64 bit field, and we therefore would
            // need the disable the field that follows.
            enableNextDataField = !cbo->IsWindowEnabled();
            break;

        default:
            // 32 bit types do not affect the fields after them.
            enableNextDataField = TRUE;
            break;
        }

        nextDataField->EnableWindow(enableNextDataField);
        OnUserSelDatatype((WPARAM)nextDataField->GetDataType(), (LPARAM)nextDataField);
    }

    return 0;
}
