/** <pre>
 *  Copyright 2004 The Boeing Company
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
 *
 *  FILENAME:   EntityWeatherPropPage.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface Host Emulator (CIGI HE)
 *
 *  PROGRAM DESCRIPTION:
 *  The Host Emulator (HE) acts as a surrogate in the absence of a simulation host.
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME
 *  DESCRIPTION OF CHANGE
 *
 *  11/11/2004 Lance Durham
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  Version 3.1.1
 */

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "EntityWeatherPropPage.h"
#include "globals.h"
#include "Weather.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityWeatherPropPage dialog

CEntityWeatherPropPage::CEntityWeatherPropPage(CWnd *pParent /*=NULL*/)
    : OldDialog(CEntityWeatherPropPage::IDD, pParent)
{
    //{{AFX_DATA_INIT(CEntityWeatherPropPage)
    m_EnableLightning = FALSE;
    m_EnableTopScud = FALSE;
    m_EnableBottomScud = FALSE;
    m_EnableWeather = FALSE;
    m_EnableWinds = FALSE;
    m_CloudType = 0;
    m_Aerosol = 0.0f;
    m_BaroPressure = 0.0f;
    m_Coverage = 0.0f;
    m_WindspeedHorz = 0.0f;
    m_WindspeedVert = 0.0f;
    m_Humidity = 0;
    m_TopScudFreq = 0.0f;
    m_BottomScudFreq = 0.0f;
    m_Severity = 0;
    m_Temperature = 0.0f;
    m_TopTransitionBand = 0.0f;
    m_BottomTransitionBand = 0.0f;
    m_WindDirection = 0.0f;
    m_Visibility = 0.0f;
    //}}AFX_DATA_INIT
}

void CEntityWeatherPropPage::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEntityWeatherPropPage)
    DDX_Control(pDX, IDC_EDIT_VISIBILITY, m_EditVisibility);
    DDX_Control(pDX, IDC_EDIT_WIND_DIRECTION, m_EditWindDirection);
    DDX_Control(pDX, IDC_EDIT_TOP_TRANSITION_BAND, m_EditTopTransitionBand);
    DDX_Control(pDX, IDC_EDIT_BOTTOM_TRANSITION_BAND, m_EditBottomTransitionBand);
    DDX_Control(pDX, IDC_EDIT_TEMPERATURE, m_EditTemperature);
    DDX_Control(pDX, IDC_EDIT_SEVERITY, m_EditSeverity);
    DDX_Control(pDX, IDC_EDIT_TOP_SCUD_FREQ, m_EditTopScudFreq);
    DDX_Control(pDX, IDC_EDIT_BOTTOM_SCUD_FREQ, m_EditBottomScudFreq);
    DDX_Control(pDX, IDC_EDIT_HUMIDITY, m_EditHumidity);
    DDX_Control(pDX, IDC_EDIT_VERT_WINDSPEED, m_EditWindspeedVert);
    DDX_Control(pDX, IDC_EDIT_HORZ_WINDSPEED, m_EditWindspeedHorz);
    DDX_Control(pDX, IDC_EDIT_COVERAGE, m_EditCoverage);
    DDX_Control(pDX, IDC_EDIT_BARO_PRESSURE, m_EditBaroPressure);
    DDX_Control(pDX, IDC_EDIT_AEROSOL, m_EditAerosol);
    DDX_Control(pDX, IDC_COMBOBOXEX_CLOUD_TYPE, m_ComboCloudType);
    DDX_Control(pDX, IDC_CHECK_ENABLE_WINDS, m_CheckEnableWinds);
    DDX_Control(pDX, IDC_CHECK_ENABLE_WEATHER, m_CheckEnableWeather);
    DDX_Control(pDX, IDC_CHECK_ENABLE_TOP_SCUD, m_CheckEnableTopScud);
    DDX_Control(pDX, IDC_CHECK_ENABLE_BOTTOM_SCUD, m_CheckEnableBottomScud);
    DDX_Control(pDX, IDC_CHECK_ENABLE_LIGHTNING, m_CheckEnableLightning);
    DDX_Control(pDX, IDC_SPIN_SEVERITY, m_SpinSeverity);
    DDX_Check(pDX, IDC_CHECK_ENABLE_LIGHTNING, m_EnableLightning);
    DDX_Check(pDX, IDC_CHECK_ENABLE_TOP_SCUD, m_EnableTopScud);
    DDX_Check(pDX, IDC_CHECK_ENABLE_BOTTOM_SCUD, m_EnableBottomScud);
    DDX_Check(pDX, IDC_CHECK_ENABLE_WEATHER, m_EnableWeather);
    DDX_Check(pDX, IDC_CHECK_ENABLE_WINDS, m_EnableWinds);
    DDX_CBIndex(pDX, IDC_COMBOBOXEX_CLOUD_TYPE, m_CloudType);

    DDX_Text(pDX, IDC_EDIT_AEROSOL, m_Aerosol);
    DDX_Text(pDX, IDC_EDIT_BARO_PRESSURE, m_BaroPressure);
    DDX_Text(pDX, IDC_EDIT_COVERAGE, m_Coverage);
    DDV_MinMaxFloat(pDX, m_Coverage, 0.f, 100.f);
    DDX_Text(pDX, IDC_EDIT_HORZ_WINDSPEED, m_WindspeedHorz);
    DDX_Text(pDX, IDC_EDIT_VERT_WINDSPEED, m_WindspeedVert);
    DDX_Text(pDX, IDC_EDIT_HUMIDITY, m_Humidity);
    DDV_MinMaxInt(pDX, m_Humidity, 0, 100);
    DDX_Text(pDX, IDC_EDIT_TOP_SCUD_FREQ, m_TopScudFreq);
    DDX_Text(pDX, IDC_EDIT_SEVERITY, m_Severity);
    DDV_MinMaxInt(pDX, m_Severity, 0, 5);
    DDX_Text(pDX, IDC_EDIT_TEMPERATURE, m_Temperature);
    DDX_Text(pDX, IDC_EDIT_TOP_TRANSITION_BAND, m_TopTransitionBand);
    DDX_Text(pDX, IDC_EDIT_BOTTOM_TRANSITION_BAND, m_BottomTransitionBand);
    DDX_Text(pDX, IDC_EDIT_WIND_DIRECTION, m_WindDirection);
    DDV_MinMaxFloat(pDX, m_WindDirection, 0.f, 360.f);
    DDX_Text(pDX, IDC_EDIT_VISIBILITY, m_Visibility);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEntityWeatherPropPage, CDialog)
    //{{AFX_MSG_MAP(CEntityWeatherPropPage)
    ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityWeatherPropPage message handlers

void CEntityWeatherPropPage::OnOK()
{
}

void CEntityWeatherPropPage::OnCancel()
{
}

void CEntityWeatherPropPage::RefreshView(CWeather *weather)
{
    if (weather) {
        m_CheckEnableLightning.EnableWindow(TRUE);
        m_CheckEnableTopScud.EnableWindow(TRUE);
        m_CheckEnableBottomScud.EnableWindow(TRUE);
        m_CheckEnableWeather.EnableWindow(TRUE);
        m_CheckEnableWinds.EnableWindow(TRUE);
        m_SpinSeverity.EnableWindow(TRUE);
        m_ComboCloudType.EnableWindow(TRUE);
        m_EditAerosol.EnableWindow(TRUE);
        m_EditBaroPressure.EnableWindow(TRUE);
        m_EditCoverage.EnableWindow(TRUE);
        m_EditWindspeedHorz.EnableWindow(TRUE);
        m_EditWindspeedVert.EnableWindow(TRUE);
        m_EditHumidity.EnableWindow(TRUE);
        m_EditTopScudFreq.EnableWindow(TRUE);
        m_EditBottomScudFreq.EnableWindow(TRUE);
        m_EditSeverity.EnableWindow(TRUE);
        m_EditTemperature.EnableWindow(TRUE);
        m_EditVisibility.EnableWindow(TRUE);
        m_EditTopTransitionBand.EnableWindow(TRUE);
        m_EditBottomTransitionBand.EnableWindow(TRUE);
        m_EditWindDirection.EnableWindow(TRUE);

        m_EnableLightning = weather->GetRandomLightningEnable();
        m_EnableTopScud = weather->GetTopScudEnable();
        m_EnableBottomScud = weather->GetBottomScudEnable();
        m_EnableWeather = weather->GetWeatherEnable();
        m_EnableWinds = weather->GetRandomWindEnable();
        m_CloudType = weather->GetCloudType();
        m_Aerosol = weather->GetAerosolConcentration();
        m_BaroPressure = weather->GetBaroPressure();
        m_Coverage = weather->GetCoverage();
        m_WindspeedHorz = weather->GetHorzWindspeed();
        m_WindspeedVert = weather->GetVertWindspeed();
        m_Humidity = weather->GetHumidity();
        m_TopScudFreq = weather->GetTopScudFreq();
        m_BottomScudFreq = weather->GetBottomScudFreq();
        m_Severity = weather->GetSeverity();
        m_Temperature = weather->GetTemperature();
        m_Visibility = weather->GetVisibility();
        m_TopTransitionBand = weather->GetTopTransitionBand();
        m_BottomTransitionBand = weather->GetBottomTransitionBand();
        m_WindDirection = weather->GetWindDirection();

        UpdateData(FALSE);
    } else {
        m_CheckEnableLightning.EnableWindow(FALSE);
        m_CheckEnableTopScud.EnableWindow(FALSE);
        m_CheckEnableBottomScud.EnableWindow(FALSE);
        m_CheckEnableWeather.EnableWindow(FALSE);
        m_CheckEnableWinds.EnableWindow(FALSE);
        m_SpinSeverity.EnableWindow(FALSE);
        m_ComboCloudType.EnableWindow(FALSE);
        m_EditAerosol.EnableWindow(FALSE);
        m_EditBaroPressure.EnableWindow(FALSE);
        m_EditCoverage.EnableWindow(FALSE);
        m_EditWindspeedHorz.EnableWindow(FALSE);
        m_EditWindspeedVert.EnableWindow(FALSE);
        m_EditHumidity.EnableWindow(FALSE);
        m_EditTopScudFreq.EnableWindow(FALSE);
        m_EditBottomScudFreq.EnableWindow(FALSE);
        m_EditSeverity.EnableWindow(FALSE);
        m_EditTemperature.EnableWindow(FALSE);
        m_EditVisibility.EnableWindow(FALSE);
        m_EditTopTransitionBand.EnableWindow(FALSE);
        m_EditBottomTransitionBand.EnableWindow(FALSE);
        m_EditWindDirection.EnableWindow(FALSE);

        m_CheckEnableLightning.SetCheck(FALSE);
        m_CheckEnableTopScud.SetCheck(FALSE);
        m_CheckEnableBottomScud.SetCheck(FALSE);
        m_CheckEnableWeather.SetCheck(FALSE);
        m_CheckEnableWinds.SetCheck(FALSE);
        m_ComboCloudType.SetCurSel(-1);
        m_EditAerosol.SetWindowText("");
        m_EditBaroPressure.SetWindowText("");
        m_EditCoverage.SetWindowText("");
        m_EditWindspeedHorz.SetWindowText("");
        m_EditWindspeedVert.SetWindowText("");
        m_EditHumidity.SetWindowText("");
        m_EditTopScudFreq.SetWindowText("");
        m_EditBottomScudFreq.SetWindowText("");
        m_EditSeverity.SetWindowText("");
        m_EditTemperature.SetWindowText("");
        m_EditVisibility.SetWindowText("");
        m_EditTopTransitionBand.SetWindowText("");
        m_EditBottomTransitionBand.SetWindowText("");
        m_EditWindDirection.SetWindowText("");
    }
}

void CEntityWeatherPropPage::OnButtonApply()
{
    UpdateData(TRUE);

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather)
        return;

    weather->SetRandomLightningEnable(m_EnableLightning ? TRUE : FALSE);
    weather->SetTopScudEnable(m_EnableTopScud ? TRUE : FALSE);
    weather->SetBottomScudEnable(m_EnableBottomScud ? TRUE : FALSE);
    weather->SetWeatherEnable(m_EnableWeather ? TRUE : FALSE);
    weather->SetRandomWindEnable(m_EnableWinds ? TRUE : FALSE);
    weather->SetCloudType(m_CloudType);
    weather->SetAerosolConcentration(m_Aerosol);
    weather->SetBaroPressure(m_BaroPressure);
    weather->SetCoverage(m_Coverage);
    weather->SetHorzWindspeed(m_WindspeedHorz);
    weather->SetVertWindspeed(m_WindspeedVert);
    weather->SetHumidity(m_Humidity);
    weather->SetTopScudFreq(m_TopScudFreq);
    weather->SetBottomScudFreq(m_BottomScudFreq);
    weather->SetSeverity(m_Severity);
    weather->SetTemperature(m_Temperature);
    weather->SetVisibility(m_Visibility);
    weather->SetTopTransitionBand(m_TopTransitionBand);
    weather->SetBottomTransitionBand(m_BottomTransitionBand);
    weather->SetWindDirection(m_WindDirection);

    // Send the packet.
    SendPacketToDriver(weather);
}

BOOL CEntityWeatherPropPage::OnInitDialog()
{
    OldDialog::OnInitDialog();

    m_SpinSeverity.SetRange(0, 5);
    m_SpinSeverity.SetPos(0);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CEntityWeatherPropPage::SendPacketToDriver(CWeather *weather)
{
    unsigned char buffer[sizeof(CIGI_WEATHER_CONTROL)] = {0};

    // Create a Weather Control packet.
    weather->CreateCigiPacket(buffer);

    // SendImmedCigiMessage() will figure out the packet size.
    SendImmedCigiMessage(buffer);
}
