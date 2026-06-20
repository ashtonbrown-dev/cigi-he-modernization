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

// PacketDlgCelestialSphereControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "Hemu4.h"
#include "PacketDlgCelestialSphereControl.h"
#include "SkyStateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgCelestialSphereControl dialog

CPacketDlgCelestialSphereControl::CPacketDlgCelestialSphereControl(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgCelestialSphereControl::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgCelestialSphereControl)
    m_MoonEnable = FALSE;
    m_SunEnable = FALSE;
    m_StarFieldEnable = FALSE;
    m_EphemerisEnable = FALSE;
    m_Hour = 0;
    m_Minute = 0;
	m_Seconds = 0.0;
    m_Month = 1;
    m_Day = 1;
    m_Year = 2016;
    m_StarFieldIntensity = 0.0f;
    m_DateTimeValid = TRUE;
    //}}AFX_DATA_INIT
}

void CPacketDlgCelestialSphereControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgCelestialSphereControl)
    DDX_Check(pDX, IDC_CHECK_MOON_ENABLE, m_MoonEnable);
    DDX_Check(pDX, IDC_CHECK_SUN_ENABLE, m_SunEnable);
    DDX_Check(pDX, IDC_CHECK_STARFIELD_ENABLE, m_StarFieldEnable);
    DDX_Check(pDX, IDC_CHECK_EPHEMERIS_ENABLE, m_EphemerisEnable);
    DDX_Text(pDX, IDC_EDIT_HOUR, m_Hour);
    DDV_MinMaxInt(pDX, m_Hour, 0, 23);
    DDX_Text(pDX, IDC_EDIT_MINUTE, m_Minute);
    DDV_MinMaxInt(pDX, m_Minute, 0, 59);
    DDX_Text(pDX, IDC_EDIT_SECONDS, m_Seconds);
    DDV_MinMaxFloat(pDX, m_Seconds, 0.0f, 59.9f);
    DDX_Text(pDX, IDC_EDIT_MONTH, m_Month);
    DDV_MinMaxInt(pDX, m_Month, 1, 12);
    DDX_Text(pDX, IDC_EDIT_DAY, m_Day);
    DDV_MinMaxInt(pDX, m_Day, 1, 31);
    DDX_Text(pDX, IDC_EDIT_YEAR, m_Year);
    DDX_Text(pDX, IDC_EDIT_STARFIELD_INTENSITY, m_StarFieldIntensity);
    DDV_MinMaxFloat(pDX, m_StarFieldIntensity, 0.f, 100.f);
    DDX_Check(pDX, IDC_CHECK_DATETIME_VALID, m_DateTimeValid);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgCelestialSphereControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgCelestialSphereControl)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgCelestialSphereControl message handlers

BOOL CPacketDlgCelestialSphereControl::OnInitDialog()
{
    CDialog::OnInitDialog();

    RefreshView();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgCelestialSphereControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgCelestialSphereControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    ApplyCelestialSphere();

    CIGI_CELESTIAL_SPHERE_CONTROL csc = {0};
    csc.packet_id = CIGI_CELESTIAL_SPHERE_CONTROL_OPCODE;
    csc.packet_size = sizeof(CIGI_CELESTIAL_SPHERE_CONTROL);
    csc.hour = m_Hour;
    csc.minute = m_Minute;
	csc.seconds = m_Seconds;
    csc.date = m_Month * 1000000 + m_Day * 10000 + m_Year;
    csc.datetime_valid = m_DateTimeValid;
    csc.ephemeris_enable = m_EphemerisEnable;
    csc.sun_enable = m_SunEnable;
    csc.moon_enable = m_MoonEnable;
    csc.starfield_enable = m_StarFieldEnable;
    csc.starfield_intensity = m_StarFieldIntensity;

    SendImmedCigiMessage(&csc);

    // Update the view.
    theApp.GetMainFrame().GetSkyStateView().RefreshView();
}

void CPacketDlgCelestialSphereControl::RefreshView(void)
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    CTime time = env->GetDateTime();
    m_Month = time.GetMonth();
    m_Day = time.GetDay();
    m_Year = time.GetYear();
    m_Hour = time.GetHour();
    m_Minute = time.GetMinute();
	m_Seconds = ( float ) time.GetSecond();
    m_EphemerisEnable = env->GetEphemerisEnable();
    m_SunEnable = env->GetSunEnable();
    m_MoonEnable = env->GetMoonEnable();
    m_StarFieldEnable = env->GetStarfieldEnable();
    m_StarFieldIntensity = env->GetStarfieldIntensity();

    UpdateData(FALSE);
}

void CPacketDlgCelestialSphereControl::ApplyCelestialSphere(void)
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    CTime time(m_Year, m_Month, m_Day, m_Hour, m_Minute, (int) m_Seconds);
    env->SetDateTime(time);
    env->SetEphemerisEnable(m_EphemerisEnable);
    env->SetSunEnable(m_SunEnable);
    env->SetMoonEnable(m_MoonEnable);
    env->SetStarfieldEnable(m_StarFieldEnable);
    env->SetStarfieldIntensity(m_StarFieldIntensity);
}