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

// PacketDlgComponentControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "CigiView.h"
#include "ViewGroup.h"
#include "TerrainDB.h"
#include "Weather.h"
#include "PacketDlgComponentControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgComponentControl dialog

CPacketDlgComponentControl::CPacketDlgComponentControl(CWnd *pParent /*=NULL*/)
    : OldDialog(CPacketDlgComponentControl::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgComponentControl)
    m_CompID = 0;
    m_CompState = 0;
    m_InstID = 0;
    m_CompClass = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgComponentControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgComponentControl)
    DDX_Text(pDX, IDC_EDIT_COMPONENT_ID, m_CompID);
    DDX_Text(pDX, IDC_EDIT_COMPONENT_STATE, m_CompState);
    DDX_Text(pDX, IDC_EDIT_INSTANCE_ID, m_InstID);
    DDX_CBIndex(pDX, IDC_COMBOBOXEX_COMPONENT_CLASS, m_CompClass);
    DDX_Control(pDX, IDC_COMBO1, m_cboData1);
    DDX_Control(pDX, IDC_COMBO2, m_cboData2);
    DDX_Control(pDX, IDC_COMBO3, m_cboData3);
    DDX_Control(pDX, IDC_COMBO4, m_cboData4);
    DDX_Control(pDX, IDC_COMBO5, m_cboData5);
    DDX_Control(pDX, IDC_COMBO6, m_cboData6);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgComponentControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgComponentControl)
    ON_MESSAGE(CBV_SELCHANGE, OnUserSelDatatype)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgComponentControl message handlers

BOOL CPacketDlgComponentControl::OnInitDialog()
{
    OldDialog::OnInitDialog();

    // Delete the 64-bit datatypes from the last data item.
    m_cboData6.DeleteString(5);
    m_cboData6.DeleteString(4);
    m_cboData6.DeleteString(3);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgComponentControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgComponentControl::OnOK()
{
    unsigned long temp[2] = {0};
    bool is32 = false;

    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_COMPONENT_CONTROL cc = {0};

    cc.packet_id = CIGI_COMPONENT_CONTROL_OPCODE;
    cc.packet_size = sizeof(CIGI_COMPONENT_CONTROL);
    cc.instance_id = m_InstID;
    cc.component_id = m_CompID;
    cc.component_state = m_CompState;
    
	auto PopulateData = [&temp]( CComboBoxVariant& combo_box, uint32_t* data_out, uint32_t* next_data_out ) -> bool
	{
		switch (combo_box.GetDataType()) {
		case 0: // CComboBoxVariant::DataTypeInt32
			*data_out = combo_box.GetValueInt32();
			return true; // 32-bit

		case 1: // CComboBoxVariant::DataTypeUInt32
			*data_out = combo_box.GetValueUInt32();
			return true; // 32-bit

		case 2: // CComboBoxVariant::DataTypeFloat
			*((float *)temp) = combo_box.GetValueFloat();
			*data_out = temp[0];
			return true; // 32-bit

		case 3: // CComboBoxVariant::DataTypeInt64
		case 4: // CComboBoxVariant::DataTypeUInt64
			if ( next_data_out )
			{
				*((unsigned __int64 *)temp) = combo_box.GetValueUInt64();
					 *data_out = temp[1];
				*next_data_out = temp[0];
			}
			return false; // not 32-bit

		case 5: // CComboBoxVariant::DataTypeDouble
			if ( next_data_out )
			{
				*((double *)temp) = combo_box.GetValueDouble();
					 *data_out = temp[1];
				*next_data_out = temp[0];
			}
			return false; // not 32-bit

		default:
			break;
		}
	};

	            is32 = PopulateData( m_cboData1, &cc.data1, &cc.data2 );
	
	if ( is32 ) is32 = PopulateData( m_cboData2, &cc.data2, &cc.data3 );
	else        is32 = false;

	if ( is32 ) is32 = PopulateData( m_cboData3, &cc.data3, &cc.data4 );
	else        is32 = false;

	if ( is32 ) is32 = PopulateData( m_cboData4, &cc.data4, &cc.data5 );
	else        is32 = false;

	if ( is32 ) is32 = PopulateData( m_cboData5, &cc.data5, &cc.data6 );
	else        is32 = false;
    
	if ( is32 )        PopulateData( m_cboData6, &cc.data6, nullptr );
  

	cc.component_class = m_CompClass;

    SendImmedCigiMessage(&cc);

    ApplyComponent();
}

void CPacketDlgComponentControl::ApplyComponent()
{
    // Changes made to this function should also be made to
    //CPacketDlgComponentControl::ApplyComponent().

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

    //if (comp) {
    //    long data;
    //    comp->SetState(m_CompState);
    //    sscanf((LPCTSTR)m_Data1, "%X", &data);
    //    comp->SetData(0, data);
    //    sscanf((LPCTSTR)m_Data2, "%X", &data);
    //    comp->SetData(1, data);
    //    sscanf((LPCTSTR)m_Data3, "%X", &data);
    //    comp->SetData(2, data);
    //    sscanf((LPCTSTR)m_Data4, "%X", &data);
    //    comp->SetData(3, data);
    //    sscanf((LPCTSTR)m_Data5, "%X", &data);
    //    comp->SetData(4, data);
    //    sscanf((LPCTSTR)m_Data6, "%X", &data);
    //    comp->SetData(5, data);
    //}
}

/// This function will detect the type (size) of data that the user has selected for
/// component data fields. When the user selects a 64 bit sized type for a field,
/// the field immediately following will be disabled. (Since fields are 32 bits, a 64
/// bit value will of course use the combined space of two fields.)
LRESULT CPacketDlgComponentControl::OnUserSelDatatype(WPARAM wParam, LPARAM lParam)
{
    CComboBoxVariant *cbo = (CComboBoxVariant *)lParam;
    CComboBoxVariant *nextDataField = NULL;

    // Determine the data field immediately following the current, if it exists.
    if (cbo == &m_cboData1)
        nextDataField = &m_cboData2;
    if (cbo == &m_cboData2)
        nextDataField = &m_cboData3;
    if (cbo == &m_cboData3)
        nextDataField = &m_cboData4;
    if (cbo == &m_cboData4)
        nextDataField = &m_cboData5;
    if (cbo == &m_cboData5)
        nextDataField = &m_cboData6;

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
