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

// PacketDlgAtmosphereControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "Hemu4.h"
#include "PacketDlgAtmosphereControl.h"
#include "SkyStateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgAtmosphereControl dialog


CPacketDlgAtmosphereControl::CPacketDlgAtmosphereControl(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgAtmosphereControl::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgAtmosphereControl)
    m_AtmosphericsEnable = TRUE;
    m_AirTemperature = 0.0f;
    m_BaroPressure = 0.0f;
    m_Humidity = 0;
    m_VisibilityRange = 0.0f;
    m_WindDirection = 0.0f;
    m_WindSpeedHorz = 0.0f;
    m_WindSpeedVert = 0.0f;
    //}}AFX_DATA_INIT
}

void CPacketDlgAtmosphereControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgAtmosphereControl)
    DDX_Check(pDX, IDC_CHECK_ATMOSPHERICS_ENABLE, m_AtmosphericsEnable);
    DDX_Text(pDX, IDC_EDIT_AIR_TEMP, m_AirTemperature);
    DDX_Text(pDX, IDC_EDIT_BARO_PRESSURE, m_BaroPressure);
    DDX_Text(pDX, IDC_EDIT_HUMIDITY, m_Humidity);
    DDV_MinMaxInt(pDX, m_Humidity, 0, 100);
    DDX_Text(pDX, IDC_EDIT_VISIBILITY_RANGE, m_VisibilityRange);
    DDX_Text(pDX, IDC_EDIT_WIND_DIRECTION, m_WindDirection);
    DDX_Text(pDX, IDC_EDIT_WIND_SPEED_HORZ, m_WindSpeedHorz);
    DDX_Text(pDX, IDC_EDIT_WIND_SPEED_VERT, m_WindSpeedVert);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgAtmosphereControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgAtmosphereControl)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgAtmosphereControl message handlers

BOOL CPacketDlgAtmosphereControl::OnInitDialog()
{
    CDialog::OnInitDialog();

    RefreshView();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgAtmosphereControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgAtmosphereControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_ATMOSPHERE_CONTROL ac = {0};
    ac.packet_id = CIGI_ATMOSPHERE_CONTROL_OPCODE;
    ac.packet_size = sizeof(CIGI_ATMOSPHERE_CONTROL);
    ac.atm_model_enable = m_AtmosphericsEnable;
    ac.global_humidity = m_Humidity;
    ac.global_air_temp = m_AirTemperature;
    ac.global_visibility = m_VisibilityRange;
    ac.global_windspeed_horz = m_WindSpeedHorz;
    ac.global_windspeed_vert = m_WindSpeedVert;
    ac.global_wind_direction = m_WindDirection;
    ac.global_baro_pressure = m_BaroPressure;

    SendImmedCigiMessage(&ac);

    ApplyAtmosphere();

    // Update the view.
    theApp.GetMainFrame().GetSkyStateView().RefreshView();
}

void CPacketDlgAtmosphereControl::RefreshView(void)
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    m_AtmosphericsEnable = env->GetAtmosphericsEnable();
    m_Humidity = env->GetHumidity();
    m_AirTemperature = env->GetTemperature();
    m_VisibilityRange = env->GetVisibility();
    m_WindSpeedHorz = env->GetHorzWindspeed();
    m_WindSpeedVert = env->GetVertWindspeed();
    m_WindDirection = env->GetWindDirection();
    m_BaroPressure = env->GetBaroPressure();

    UpdateData(FALSE);
}

void CPacketDlgAtmosphereControl::ApplyAtmosphere(void)
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    env->SetAtmosphericsEnable(m_AtmosphericsEnable);
    env->SetHumidity(m_Humidity);
    env->SetTemperature(m_AirTemperature);
    env->SetVisibility(m_VisibilityRange);
    env->SetHorzWindspeed(m_WindSpeedHorz);
    env->SetVertWindspeed(m_WindSpeedVert);
    env->SetWindDirection(m_WindDirection);
    env->SetBaroPressure(m_BaroPressure);
}
