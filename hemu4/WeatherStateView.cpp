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

// WeatherStateView.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "WeatherStateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWeatherStateView dialog

CWeatherStateView::CWeatherStateView(CWnd *pParent /*=NULL*/)
    : CBaseStateView(CWeatherStateView::IDD, pParent)
{
    //{{AFX_DATA_INIT(CWeatherStateView)
    m_EnableLightning = FALSE;
    m_EnableTopScud = FALSE;
    m_EnableBottomScud = FALSE;
    m_EnableWeather = FALSE;
    m_EnableWinds = FALSE;
    m_Aerosol = 0.0f;
    m_BaroPressure = 0.0f;
    m_BaseElevation = 0.0f;
    m_Coverage = 0.0f;
    m_HorzWindspeed = 0.0f;
    m_VertWindspeed = 0.0f;
    m_Humidity = 0;
    m_LayerID = 0;
    m_TopScudFreq = 0.0f;
    m_BottomScudFreq = 0.0f;
    m_Severity = 0;
    m_AirTemp = 0.0f;
    m_Thickness = 0.0f;
    m_TopTransitionBand = 0.0f;
    m_BottomTransitionBand = 0.0f;
    m_Visibility = 0.0f;
    m_WindDirection = 0.0f;
    //}}AFX_DATA_INIT

    m_bHoldEnableWeather = FALSE;
    m_bHoldEnableWinds = FALSE;
    m_bHoldEnableLightning = FALSE;
    m_bHoldEnableTopScud = FALSE;
    m_bHoldEnableBottomScud = FALSE;

    m_PrevWeather = NULL;
}

void CWeatherStateView::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CWeatherStateView)
    DDX_Control(pDX, IDC_CHECK_ENABLE_WINDS, m_CheckEnableWinds);
    DDX_Control(pDX, IDC_CHECK_ENABLE_WEATHER, m_CheckEnableWeather);
    DDX_Control(pDX, IDC_CHECK_ENABLE_TOP_SCUD, m_CheckEnableTopScud);
    DDX_Control(pDX, IDC_CHECK_ENABLE_BOTTOM_SCUD, m_CheckEnableBottomScud);
    DDX_Control(pDX, IDC_CHECK_ENABLE_LIGHTNING, m_CheckEnableLightning);
    DDX_Control(pDX, IDC_COMBOBOXEX_CLOUD_TYPE, m_ComboCloudType);
    DDX_Control(pDX, IDC_EDIT_WIND_DIRECTION, m_EditWindDirection);
    DDX_Control(pDX, IDC_EDIT_VISIBILITY, m_EditVisibility);
    DDX_Control(pDX, IDC_EDIT_TOP_TRANSITION_BAND, m_EditTopTransitionBand);
    DDX_Control(pDX, IDC_EDIT_BOTTOM_TRANSITION_BAND, m_EditBottomTransitionBand);
    DDX_Control(pDX, IDC_EDIT_THICKNESS, m_EditThickness);
    DDX_Control(pDX, IDC_EDIT_TEMPERATURE, m_EditAirTemp);
    DDX_Control(pDX, IDC_EDIT_SEVERITY, m_EditSeverity);
    DDX_Control(pDX, IDC_EDIT_TOP_SCUD_FREQ, m_EditTopScudFreq);
    DDX_Control(pDX, IDC_EDIT_BOTTOM_SCUD_FREQ, m_EditBottomScudFreq);
    DDX_Control(pDX, IDC_EDIT_HUMIDITY, m_EditHumidity);
    DDX_Control(pDX, IDC_EDIT_VERT_WINDSPEED, m_EditVertWindspeed);
    DDX_Control(pDX, IDC_EDIT_HORZ_WINDSPEED, m_EditHorzWindspeed);
    DDX_Control(pDX, IDC_EDIT_COVERAGE, m_EditCoverage);
    DDX_Control(pDX, IDC_EDIT_BASE_ELEV, m_EditBaseElevation);
    DDX_Control(pDX, IDC_EDIT_BARO_PRESSURE, m_EditBaroPressure);
    DDX_Control(pDX, IDC_EDIT_AEROSOL, m_EditAerosol);
    DDX_Control(pDX, IDC_TAB1, m_Tab);
    DDX_Control(pDX, IDC_SPIN_SEVERITY, m_SpinSeverity);
    DDX_Check(pDX, IDC_CHECK_ENABLE_LIGHTNING, m_EnableLightning);
    DDX_Check(pDX, IDC_CHECK_ENABLE_TOP_SCUD, m_EnableTopScud);
    DDX_Check(pDX, IDC_CHECK_ENABLE_BOTTOM_SCUD, m_EnableBottomScud);
    DDX_Check(pDX, IDC_CHECK_ENABLE_WEATHER, m_EnableWeather);
    DDX_Check(pDX, IDC_CHECK_ENABLE_WINDS, m_EnableWinds);
    DDX_Text(pDX, IDC_EDIT_AEROSOL, m_Aerosol);
    DDX_Text(pDX, IDC_EDIT_BARO_PRESSURE, m_BaroPressure);
    DDX_Text(pDX, IDC_EDIT_BASE_ELEV, m_BaseElevation);
    DDX_Text(pDX, IDC_EDIT_COVERAGE, m_Coverage);
    DDV_MinMaxFloat(pDX, m_Coverage, 0.f, 100.f);
    DDX_Text(pDX, IDC_EDIT_HORZ_WINDSPEED, m_HorzWindspeed);
    DDX_Text(pDX, IDC_EDIT_VERT_WINDSPEED, m_VertWindspeed);
    DDX_Text(pDX, IDC_EDIT_HUMIDITY, m_Humidity);
    DDV_MinMaxInt(pDX, m_Humidity, 0, 100);
    DDX_Text(pDX, IDC_EDIT_LAYER_ID, m_LayerID);
    DDX_Text(pDX, IDC_EDIT_TOP_SCUD_FREQ, m_TopScudFreq);
    DDV_MinMaxFloat(pDX, m_TopScudFreq, 0.f, 100.f);
    DDX_Text(pDX, IDC_EDIT_BOTTOM_SCUD_FREQ, m_BottomScudFreq);
    DDV_MinMaxFloat(pDX, m_BottomScudFreq, 0.f, 100.f);
    DDX_Text(pDX, IDC_EDIT_SEVERITY, m_Severity);
    DDV_MinMaxInt(pDX, m_Severity, 0, 5);
    DDX_Text(pDX, IDC_EDIT_TEMPERATURE, m_AirTemp);
    DDX_Text(pDX, IDC_EDIT_THICKNESS, m_Thickness);
    DDX_Text(pDX, IDC_EDIT_TOP_TRANSITION_BAND, m_TopTransitionBand);
    DDX_Text(pDX, IDC_EDIT_BOTTOM_TRANSITION_BAND, m_BottomTransitionBand);
    DDX_Text(pDX, IDC_EDIT_VISIBILITY, m_Visibility);
    DDX_Text(pDX, IDC_EDIT_WIND_DIRECTION, m_WindDirection);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWeatherStateView, CBaseStateView)
    //{{AFX_MSG_MAP(CWeatherStateView)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab)
    ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnSelchangingTab)
    ON_BN_CLICKED(IDC_CHECK_ENABLE_LIGHTNING, OnCheckEnableLightning)
    ON_BN_CLICKED(IDC_CHECK_ENABLE_TOP_SCUD, OnCheckEnableTopScud)
    ON_BN_CLICKED(IDC_CHECK_ENABLE_BOTTOM_SCUD, OnCheckEnableBottomScud)
    ON_BN_CLICKED(IDC_CHECK_ENABLE_WEATHER, OnCheckEnableWeather)
    ON_BN_CLICKED(IDC_CHECK_ENABLE_WINDS, OnCheckEnableWinds)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_CLOUD_TYPE, OnSelchangeComboboxexCloudType)
    //}}AFX_MSG_MAP

    ON_MESSAGE(WM_USEREDIT, OnUserEditControl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeatherStateView message handlers

void CWeatherStateView::OnOK()
{
}

void CWeatherStateView::OnCancel()
{
}

BOOL CWeatherStateView::OnInitDialog()
{
    CBaseStateView::OnInitDialog();

    m_SpinSeverity.SetRange(0, 5);
    m_SpinSeverity.SetPos(0);

    // Configure the tab control.
    InitTabs();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CWeatherStateView::InitTabs(void)
{
    // Add a tab for each of the child dialog boxes.
    m_Tab.InsertItem(0, "Components");

    // Lock the resources for the child dialog boxes.
    m_DlgTempl[0] = LockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE_ENTITY_COMPONENTS));

    // Create an array of pointers to the child dialog boxes.
    m_TabDlg[0] = (CDialog *)&m_DlgComponents;

    // Create child dialog boxes.
    int tabcount = m_Tab.GetItemCount();
    ASSERT(tabcount == NUM_WEATHERTAB_PAGES);

    for (int i = 0; i < tabcount; i++) {
        m_TabDlg[i]->CreateIndirect(m_DlgTempl[i], &m_Tab);
        m_TabDlg[i]->SetWindowPos(NULL, 5, 25, 0, 0, SWP_NOSIZE);
    }

    // Simulate selection of the first item.
    SelectTab(0);
}

void CWeatherStateView::RefreshView(void)
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    // Gray out the numeric fields during Run mode, so that the user
    // cannot edit the values.
    m_EditAerosol.EnableWindow(g_bFreezeFlag);
    m_EditAirTemp.EnableWindow(g_bFreezeFlag);
    m_EditBaroPressure.EnableWindow(g_bFreezeFlag);
    m_EditBaseElevation.EnableWindow(g_bFreezeFlag);
    m_EditCoverage.EnableWindow(g_bFreezeFlag);
    m_EditHorzWindspeed.EnableWindow(g_bFreezeFlag);
    m_EditHumidity.EnableWindow(g_bFreezeFlag);
    m_EditTopScudFreq.EnableWindow(g_bFreezeFlag);
    m_EditBottomScudFreq.EnableWindow(g_bFreezeFlag);
    m_EditSeverity.EnableWindow(g_bFreezeFlag);
    m_EditThickness.EnableWindow(g_bFreezeFlag);
    m_EditTopTransitionBand.EnableWindow(g_bFreezeFlag);
    m_EditBottomTransitionBand.EnableWindow(g_bFreezeFlag);
    m_EditVertWindspeed.EnableWindow(g_bFreezeFlag);
    m_EditVisibility.EnableWindow(g_bFreezeFlag);
    m_EditWindDirection.EnableWindow(g_bFreezeFlag);

    m_Aerosol = weather->GetAerosolConcentration();
    m_BaroPressure = weather->GetBaroPressure();
    m_BaseElevation = weather->GetBaseElevation();
    m_Coverage = weather->GetCoverage();
    m_HorzWindspeed = weather->GetHorzWindspeed();
    m_VertWindspeed = weather->GetVertWindspeed();
    m_Humidity = weather->GetHumidity();
    m_LayerID = weather->GetLayerID();
    m_TopScudFreq = weather->GetTopScudFreq();
    m_BottomScudFreq = weather->GetBottomScudFreq();
    m_Severity = weather->GetSeverity();
    m_AirTemp = weather->GetTemperature();
    m_Thickness = weather->GetThickness();
    m_TopTransitionBand = weather->GetTopTransitionBand();
    m_BottomTransitionBand = weather->GetBottomTransitionBand();
    m_Visibility = weather->GetVisibility();
    m_WindDirection = weather->GetWindDirection();

    if (m_bHoldEnableWeather == FALSE)
        m_EnableWeather = weather->GetWeatherEnable();

    if (m_bHoldEnableWinds == FALSE)
        m_EnableWinds = weather->GetRandomWindEnable();

    if (m_bHoldEnableLightning == FALSE)
        m_EnableLightning = weather->GetRandomLightningEnable();

    if (m_bHoldEnableTopScud == FALSE)
        m_EnableTopScud = weather->GetTopScudEnable();

    if (m_bHoldEnableBottomScud == FALSE)
        m_EnableBottomScud = weather->GetBottomScudEnable();

    UpdateData(FALSE);

    if (!m_ComboCloudType.GetDroppedState())
        m_ComboCloudType.SetCurSel(weather->GetCloudType());

    // Update the Components tab.
    if (m_PrevWeather != weather) {
        PopulateCompPropPage(weather);
        m_PrevWeather = weather;
    }
}

void CWeatherStateView::PopulateCompPropPage(CWeather *weather)
{
    m_DlgComponents.ClearComponents();

    int id = 0;
    CComponent *compptr = NULL;
    CComponentMap &compmap = weather->GetComponentMap();
    POSITION pos = compmap.GetStartPosition();
    while (pos) {
        compmap.GetNextAssoc(pos, id, compptr);
        m_DlgComponents.AddComponent(compptr);
    }
}

void CWeatherStateView::OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Hide the current window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_HIDE);
    }

    *pResult = 0;
}

void CWeatherStateView::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Show the appropriate window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_SHOW);
    }

    *pResult = 0;
}

void CWeatherStateView::SelectTab(const int index)
{
    // Simulate selection of the item.
    LRESULT result;
    OnSelchangingTab(NULL, &result);

    m_Tab.SetCurSel(index);

    OnSelchangeTab(NULL, &result);
}

LRESULT CWeatherStateView::OnUserEditControl(WPARAM wParam, LPARAM lParam)
{
    //long key = (long)wParam;
    CEditKbd *ctrl = (CEditKbd *)lParam;

    if (ctrl == &m_EditWindDirection)
        OnUserChangeWindDirection();
    else if (ctrl == &m_EditVisibility)
        OnUserChangeVisibility();
    else if (ctrl == &m_EditTopTransitionBand)
        OnUserChangeTopTransitionBand();
    else if (ctrl == &m_EditBottomTransitionBand)
        OnUserChangeBottomTransitionBand();
    else if (ctrl == &m_EditThickness)
        OnUserChangeThickness();
    else if (ctrl == &m_EditAirTemp)
        OnUserChangeAirTemp();
    else if (ctrl == &m_EditSeverity)
        OnUserChangeSeverity();
    else if (ctrl == &m_EditTopScudFreq)
        OnUserChangeTopScudFreq();
    else if (ctrl == &m_EditBottomScudFreq)
        OnUserChangeBottomScudFreq();
    else if (ctrl == &m_EditHumidity)
        OnUserChangeHumidity();
    else if (ctrl == &m_EditVisibility)
        OnUserChangeVisibility();
    else if (ctrl == &m_EditVertWindspeed)
        OnUserChangeVertWindspeed();
    else if (ctrl == &m_EditHorzWindspeed)
        OnUserChangeHorzWindspeed();
    else if (ctrl == &m_EditCoverage)
        OnUserChangeCoverage();
    else if (ctrl == &m_EditBaseElevation)
        OnUserChangeBaseElevation();
    else if (ctrl == &m_EditBaroPressure)
        OnUserChangeBaroPressure();
    else if (ctrl == &m_EditAerosol)
        OnUserChangeAerosol();

    return 0;
}

void CWeatherStateView::OnUserChangeWindDirection()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetWindDirection(m_WindDirection);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeVisibility()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetVisibility(m_Visibility);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeTopTransitionBand()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetTopTransitionBand(m_TopTransitionBand);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeBottomTransitionBand()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetBottomTransitionBand(m_BottomTransitionBand);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeThickness()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetThickness(m_Thickness);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeAirTemp()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetTemperature(m_AirTemp);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeSeverity()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetSeverity(m_Severity);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeTopScudFreq()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetTopScudFreq(m_TopScudFreq);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeBottomScudFreq()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetBottomScudFreq(m_BottomScudFreq);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeHumidity()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetHumidity(m_Humidity);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeRange()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetVisibility(m_Visibility);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeVertWindspeed()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetVertWindspeed(m_VertWindspeed);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeHorzWindspeed()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetHorzWindspeed(m_HorzWindspeed);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeCoverage()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetCoverage(m_Coverage);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeBaseElevation()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetBaseElevation(m_BaseElevation);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeBaroPressure()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetBaroPressure(m_BaroPressure);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnUserChangeAerosol()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    UpdateData(TRUE);

    weather->SetAerosolConcentration(m_Aerosol);

    SendPacketToDriver(weather);
}


void CWeatherStateView::OnCheckEnableLightning()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldEnableLightning = TRUE;

    UpdateData(TRUE);

    weather->SetRandomLightningEnable(m_EnableLightning ? TRUE : FALSE);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnCheckEnableTopScud()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldEnableTopScud = TRUE;

    UpdateData(TRUE);

    weather->SetTopScudEnable(m_EnableTopScud ? TRUE : FALSE);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnCheckEnableBottomScud()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldEnableBottomScud = TRUE;

    UpdateData(TRUE);

    weather->SetBottomScudEnable(m_EnableBottomScud ? TRUE : FALSE);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnCheckEnableWeather()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldEnableWeather = TRUE;

    UpdateData(TRUE);

    weather->SetWeatherEnable(m_EnableWeather ? TRUE : FALSE);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnCheckEnableWinds()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldEnableWinds = TRUE;

    UpdateData(TRUE);

    weather->SetRandomWindEnable(m_EnableWinds ? TRUE : FALSE);

    SendPacketToDriver(weather);
}

void CWeatherStateView::OnSelchangeComboboxexCloudType()
{
    CWeather *weather = g_DataManager.GetSelectedWeatherLayer();
    if (!weather)
        return;

    weather->SetCloudType(m_ComboCloudType.GetCurSel());

    SendPacketToDriver(weather);
}

void CWeatherStateView::SendPacketToDriver(CWeather *weather)
{
    unsigned char buffer[sizeof(CIGI_WEATHER_CONTROL)] = {0};

    // Create a Weather Control packet.
    weather->CreateCigiPacket(buffer);

    // SendImmedCigiMessage() will figure out the packet size.
    SendImmedCigiMessage(buffer);
}
