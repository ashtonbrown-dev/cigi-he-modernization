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

// PacketDlgWeatherControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "hemu4.h"
#include "PacketDlgWeatherControl.h"
#include "WeatherStateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgWeatherControl dialog

CPacketDlgWeatherControl::CPacketDlgWeatherControl(CWnd *pParent /*=NULL*/)
    : OldDialog(CPacketDlgWeatherControl::IDD, pParent)
{
    m_EntityID = -1;
    m_LayerID = -1;

    //{{AFX_DATA_INIT(CPacketDlgWeatherControl)
    m_EnableLightning = FALSE;
    m_EnableTopScud = FALSE;
    m_EnableBottomScud = FALSE;
    m_EnableWeather = FALSE;
    m_EnableWinds = FALSE;
    m_Scope = 0;
    m_Severity = 0;
    m_AirTemp = 0.0f;
    m_BaroPressure = 0.0f;
    m_Humidity = 0;
    m_Visibility = 0.0f;
    m_WindDirection = 0.0f;
    m_WindSpeedHorz = 0.0f;
    m_WindSpeedVert = 0.0f;
    m_CloudType = 0;
    m_Aerosol = 0.0f;
    m_BaseElev = 0.0f;
    m_Coverage = 0.0f;
    m_BottomScudFreq = 0.0f;
    m_Thickness = 0.0f;
    m_BottomTransitionBand = 0.0f;
    //}}AFX_DATA_INIT
}

void CPacketDlgWeatherControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgWeatherControl)
    DDX_Control(pDX, IDC_COMBOBOXEX_LAYER_ID, m_ComboLayerID);
    DDX_Control(pDX, IDC_COMBOBOXEX_REGION_ENTITY_ID, m_ComboRegionEntityID);
    DDX_Control(pDX, IDC_LABEL_REGION_ENTITY_ID, m_LabelRegionEntityID);
    DDX_Control(pDX, IDC_SPIN_SEVERITY, m_SpinSeverity);
    DDX_Check(pDX, IDC_CHECK_ENABLE_LIGHTNING, m_EnableLightning);
    DDX_Check(pDX, IDC_CHECK_ENABLE_TOP_SCUD, m_EnableTopScud);
    DDX_Check(pDX, IDC_CHECK_ENABLE_BOTTOM_SCUD, m_EnableBottomScud);
    DDX_Check(pDX, IDC_CHECK_ENABLE_WEATHER, m_EnableWeather);
    DDX_Check(pDX, IDC_CHECK_ENABLE_WINDS, m_EnableWinds);
    DDX_Radio(pDX, IDC_RADIO_GLOBAL, m_Scope);
    DDX_Text(pDX, IDC_EDIT_SEVERITY, m_Severity);
    DDX_Text(pDX, IDC_EDIT_AIR_TEMP, m_AirTemp);
    DDX_Text(pDX, IDC_EDIT_BARO_PRESSURE, m_BaroPressure);
    DDX_Text(pDX, IDC_EDIT_HUMIDITY, m_Humidity);
    DDX_Text(pDX, IDC_EDIT_VISIBILITY_RANGE, m_Visibility);
    DDX_Text(pDX, IDC_EDIT_WIND_DIRECTION, m_WindDirection);
    DDX_Text(pDX, IDC_EDIT_WIND_SPEED_HORZ, m_WindSpeedHorz);
    DDX_Text(pDX, IDC_EDIT_WIND_SPEED_VERT, m_WindSpeedVert);
    DDX_CBIndex(pDX, IDC_COMBOBOXEX_CLOUD_TYPE, m_CloudType);
    DDX_Text(pDX, IDC_EDIT_AEROSOL, m_Aerosol);
    DDX_Text(pDX, IDC_EDIT_BASE_ELEV, m_BaseElev);
    DDX_Text(pDX, IDC_EDIT_COVERAGE, m_Coverage);
    DDV_MinMaxFloat(pDX, m_Coverage, 0.f, 100.f);
    DDX_Text(pDX, IDC_EDIT_THICKNESS, m_Thickness);
    DDX_Text(pDX, IDC_EDIT_BOTTOM_SCUD_FREQ, m_BottomScudFreq);
    DDV_MinMaxFloat(pDX, m_BottomScudFreq, 0.f, 100.f);
    DDX_Text(pDX, IDC_EDIT_BOTTOM_TRANSITION_BAND, m_BottomTransitionBand);
    DDX_Text(pDX, IDC_EDIT_TOP_SCUD_FREQ, m_TopScudFreq);
    DDV_MinMaxFloat(pDX, m_TopScudFreq, 0.f, 100.f);
    DDX_Text(pDX, IDC_EDIT_TOP_TRANSITION_BAND, m_TopTransitionBand);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgWeatherControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgWeatherControl)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_REGION_ENTITY_ID, OnSelchangeEntityID)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_REGION_ENTITY_ID, OnEndeditEntityID)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_LAYER_ID, OnSelchangeLayerID)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_LAYER_ID, OnEndeditLayerID)
    ON_BN_CLICKED(IDC_RADIO_ENTITY, OnRadioEntity)
    ON_BN_CLICKED(IDC_RADIO_GLOBAL, OnRadioGlobal)
    ON_BN_CLICKED(IDC_RADIO_REGIONAL, OnRadioRegional)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgWeatherControl message handlers

BOOL CPacketDlgWeatherControl::OnInitDialog()
{
    OldDialog::OnInitDialog();

    m_ComboRegionEntityID.SetImageList(&g_ImgListTypes);

    m_SpinSeverity.SetRange(0, 5);
    m_SpinSeverity.SetPos(0);

    PopulateEntityIDCombo();
    PopulateLayerIDCombo();
    RefreshView();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgWeatherControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgWeatherControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_WEATHER_CONTROL wc = {0};
    wc.packet_id = CIGI_WEATHER_CONTROL_OPCODE;
    wc.packet_size = sizeof(CIGI_WEATHER_CONTROL);
	if( m_Scope == 0 ) 
	    wc.entity_region_id = 0;
	else
	    wc.entity_region_id = m_EntityID;
    wc.layer_id = m_LayerID;
    wc.weather_enable = m_EnableWeather;
    wc.bottom_scud_enable = m_EnableBottomScud;
    wc.top_scud_enable = m_EnableTopScud;
    wc.random_winds = m_EnableWinds;
    wc.random_lightning = m_EnableLightning;
    wc.cloud_type = m_CloudType;
    wc.scope = m_Scope;
    wc.severity = m_Severity;
    wc.bottom_scud_freq = m_BottomScudFreq;
    wc.top_scud_freq = m_TopScudFreq;
    wc.coverage = m_Coverage;
    wc.base_elevation = m_BaseElev;
    wc.thickness = m_Thickness;
    wc.bottom_transition_band = m_BottomTransitionBand;
    wc.top_transition_band = m_TopTransitionBand;
    wc.humidity = m_Humidity;
    wc.air_temp = m_AirTemp;
    wc.visibility = m_Visibility;
    wc.windspeed_horz = m_WindSpeedHorz;
    wc.windspeed_vert = m_WindSpeedVert;
    wc.wind_direction = m_WindDirection;
    wc.baro_pressure = m_BaroPressure;
    wc.aerosol_concentration = m_Aerosol;

    SendImmedCigiMessage(&wc);

    ApplyWeather();

    // Update the view.
    theApp.GetMainFrame().GetWeatherStateView().RefreshView();
}

void CPacketDlgWeatherControl::OnSelchangeEntityID()
{
    // Get the ID of the selected entity.
    int idx = m_ComboRegionEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboRegionEntityID.GetItemData(idx));

    RefreshView();
}

void CPacketDlgWeatherControl::OnSelchangeLayerID()
{
    // Get the ID of the selected entity.
    int idx = m_ComboLayerID.GetCurSel();
    if (idx > -1)
        m_LayerID = MAKE_ID(m_ComboLayerID.GetItemData(idx));

    RefreshView();
}

void CPacketDlgWeatherControl::OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult)
{
    int new_id = atoi(((NMCBEENDEDIT *)pNMHDR)->szText);
    if (new_id == m_EntityID)
        return;

    m_EntityID = new_id;
    int idx = FindComboItem(m_ComboRegionEntityID, m_EntityID);
    if (idx > -1) {
        // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
        // select no item after it has finished its stuff.
        ::PostMessage(m_ComboRegionEntityID.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

        // Post a CBN_SELCHANGE notification so OnSelchangeEntityID() will be called.
        // The function must be called after the CB_SELCURSEL message is processed
        // so the new item will be selected.
        ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                      MAKEWPARAM(IDC_COMBOBOXEX_REGION_ENTITY_ID, CBN_SELCHANGE),
                      (LPARAM)m_ComboRegionEntityID.GetSafeHwnd());
    }

    *pResult = 0;
}

void CPacketDlgWeatherControl::OnEndeditLayerID(NMHDR *pNMHDR, LRESULT *pResult)
{
    int new_id = atoi(((NMCBEENDEDIT *)pNMHDR)->szText);
    if (new_id == m_LayerID)
        return;

    m_LayerID = new_id;
    int idx = FindComboItem(m_ComboLayerID, m_LayerID);
    if (idx > -1) {
        // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
        // select no item after it has finished its stuff.
        ::PostMessage(m_ComboLayerID.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

        // Post a CBN_SELCHANGE notification so OnSelchangeLayerID() will be called.
        // The function must be called after the CB_SELCURSEL message is processed
        // so the new item will be selected.
        ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                      MAKEWPARAM(IDC_COMBOBOXEX_LAYER_ID, CBN_SELCHANGE),
                      (LPARAM)m_ComboLayerID.GetSafeHwnd());
    }

    *pResult = 0;
}

void CPacketDlgWeatherControl::PopulateEntityIDCombo(void)
{
    char text[256] = {0};
    int cbo_idx = -1;
    int entity_id = -1;
    CEntity *entity = NULL;
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_GENERIC, IMG_GENERIC, 0, 0, -1
                             };

    m_ComboRegionEntityID.ResetContent();

    switch (m_Scope) {
    case 0:     // Global
        break;

    case 1:     // Regional
        break;

    case 2: {   // Entity
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

            if (entity->GetClass() == ENTITY_CLASS_CLOUD) {
                // Add a new item to the list.
                cboitem.iItem = FindComboInsertPoint(m_ComboRegionEntityID, entity->GetID());
                cbo_idx = m_ComboRegionEntityID.InsertItem(&cboitem);
            }
        }

        break;
    }
    }

    if (m_ComboRegionEntityID.GetCount()) {
        m_ComboRegionEntityID.SetCurSel(0);
        m_EntityID = MAKE_ID(m_ComboRegionEntityID.GetItemData(0));
    }


    m_ComboRegionEntityID.RedrawWindow();
}

void CPacketDlgWeatherControl::PopulateLayerIDCombo(void)
{
    char text[256] = {0};
    int cbo_idx = -1;
    int layer_id = -1;
    CWeather *weather = NULL;
    COMBOBOXEXITEM cboitem = {  CBEIF_LPARAM | CBEIF_TEXT,
                                0, NULL, 1, 0, 0, 0, 0, -1
                             };

    m_ComboLayerID.ResetContent();

    switch (m_Scope) {
    case 0: {   // Global
        CWeatherMap &map = g_DataManager.GetGlobalEnv()->GetWeatherLayerMap();
        POSITION pos = map.GetStartPosition();
        while (pos) {
            map.GetNextAssoc(pos, layer_id, weather);

            sprintf(text, "%d: ", layer_id);
            strcat(text, weather->GetName());
            cboitem.pszText = text;
            cboitem.cchTextMax = strlen(text + 1);
            cboitem.lParam = MAKE_TREE_DATA(weather->GetLayerID(), 0, 0);

            // Add a new item to the list.
            cboitem.iItem = FindComboInsertPoint(m_ComboLayerID, weather->GetLayerID());
            cbo_idx = m_ComboLayerID.InsertItem(&cboitem);

            if (weather->GetLayerID() == layer_id) {
                // Select the item in the combo box.
                m_ComboLayerID.SetCurSel(cbo_idx);
            }
        }

        break;
    }

    case 1:     // Regional
        break;

    case 2:     // Entity
        break;
    }

    if (m_ComboLayerID.GetCount()) {
        m_ComboLayerID.SetCurSel(0);
        m_LayerID = MAKE_ID(m_ComboLayerID.GetItemData(0));
    }

    m_ComboLayerID.RedrawWindow();
}

int CPacketDlgWeatherControl::GetEntityIDFromCombo(void)
{
    int itemdata = m_ComboRegionEntityID.GetItemData(m_ComboRegionEntityID.GetCurSel());
    if (itemdata != -1)
        return MAKE_ID(itemdata);
    else {
        CString text;
        m_ComboRegionEntityID.GetWindowText(text);
        return atoi((LPCTSTR)text);
    }
}

int CPacketDlgWeatherControl::GetLayerIDFromCombo(void)
{
    int itemdata = m_ComboLayerID.GetItemData(m_ComboLayerID.GetCurSel());
    return MAKE_ID(itemdata);
}

int CPacketDlgWeatherControl::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgWeatherControl::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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

void CPacketDlgWeatherControl::RefreshView(void)
{
    CWeather *weather = NULL;

    switch (m_Scope) {
    case 0: {   // Global
        CGlobalEnv *env = g_DataManager.GetGlobalEnv();
        if (!env)
            return;

        weather = env->GetWeatherLayer(m_LayerID);

        break;
    }

    case 1: {   // Regional
        break;
    }

    case 2: {   // Entity
        CEntity *entity = g_DataManager.GetEntity(m_EntityID);
        if (!entity)
            return;

        weather = entity->GetWeatherAttributes();

        break;
    }

    default:
        break;
    }

    if (!weather)
        return;

    m_EnableLightning = weather->GetRandomLightningEnable();
    m_EnableTopScud = weather->GetTopScudEnable();
    m_EnableBottomScud = weather->GetBottomScudEnable();
    m_EnableWeather = weather->GetWeatherEnable();
    m_EnableWinds = weather->GetRandomWindEnable();
    m_Humidity = weather->GetHumidity();
    m_AirTemp = weather->GetTemperature();
    m_Visibility = weather->GetVisibility();
    m_WindSpeedHorz = weather->GetHorzWindspeed();
    m_WindSpeedVert = weather->GetVertWindspeed();
    m_WindDirection = weather->GetWindDirection();
    m_BaroPressure = weather->GetBaroPressure();
    m_Scope = weather->GetScope();
    m_Severity = weather->GetSeverity();
    m_CloudType = weather->GetCloudType();
    m_Aerosol = weather->GetAerosolConcentration();
    m_BaseElev = weather->GetBaseElevation();
    m_Coverage = weather->GetCoverage();
    m_TopScudFreq = weather->GetTopScudFreq();
    m_BottomScudFreq = weather->GetBottomScudFreq();
    m_Thickness = weather->GetThickness();
    m_TopTransitionBand = weather->GetTopTransitionBand();
    m_BottomTransitionBand = weather->GetBottomTransitionBand();

    UpdateData(FALSE);
}

void CPacketDlgWeatherControl::ApplyWeather(void)
{
    CWeather *weather = NULL;

    UpdateData(TRUE);

    switch (m_Scope) {
    case 0: {   // Global
        CGlobalEnv *env = g_DataManager.GetGlobalEnv();
        if (!env)
            return;

        weather = env->GetWeatherLayer(m_LayerID);

        break;
    }

    case 1: {   // Regional
        break;
    }

    case 2: {   // Entity
        CEntity *entity = g_DataManager.GetEntity(m_EntityID);
        if (!entity)
            return;

        weather = entity->GetWeatherAttributes();

        break;
    }

    default:
        break;
    }

    if (!weather)
        return;

    weather->SetRandomLightningEnable(m_EnableLightning);
    weather->SetTopScudEnable(m_EnableTopScud);
    weather->SetBottomScudEnable(m_EnableBottomScud);
    weather->SetWeatherEnable(m_EnableWeather);
    weather->SetRandomWindEnable(m_EnableWinds);
    weather->SetHumidity(m_Humidity);
    weather->SetTemperature(m_AirTemp);
    weather->SetVisibility(m_Visibility);
    weather->SetHorzWindspeed(m_WindSpeedHorz);
    weather->SetVertWindspeed(m_WindSpeedVert);
    weather->SetWindDirection(m_WindDirection);
    weather->SetBaroPressure(m_BaroPressure);
    weather->SetScope(m_Scope);
    weather->SetSeverity(m_Severity);
    weather->SetCloudType(m_CloudType);
    weather->SetAerosolConcentration(m_Aerosol);
    weather->SetBaseElevation(m_BaseElev);
    weather->SetCoverage(m_Coverage);
    weather->SetTopScudFreq(m_TopScudFreq);
    weather->SetBottomScudFreq(m_BottomScudFreq);
    weather->SetThickness(m_Thickness);
    weather->SetTopTransitionBand(m_TopTransitionBand);
    weather->SetBottomTransitionBand(m_BottomTransitionBand);
}


void CPacketDlgWeatherControl::OnRadioGlobal()
{
    m_Scope = 0;
    m_ComboRegionEntityID.SetCurSel(-1);
    PopulateEntityIDCombo();
    PopulateLayerIDCombo();

    RefreshView();
}

void CPacketDlgWeatherControl::OnRadioRegional()
{
    m_Scope = 1;
    m_LabelRegionEntityID.SetWindowText("Region ID:");
    m_ComboRegionEntityID.SetCurSel(-1);
    PopulateEntityIDCombo();
    PopulateLayerIDCombo();

    RefreshView();
}

void CPacketDlgWeatherControl::OnRadioEntity()
{
    m_Scope = 2;
    m_LabelRegionEntityID.SetWindowText("Entity ID:");
    m_ComboLayerID.SetCurSel(-1);
    PopulateEntityIDCombo();
    PopulateLayerIDCombo();

    RefreshView();
}
