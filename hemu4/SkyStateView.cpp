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

// SkyStateView.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "SkyStateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkyStateView dialog

CSkyStateView::CSkyStateView(CWnd *pParent /*=NULL*/)
    : CBaseStateView(CSkyStateView::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSkyStateView)
    m_EnableAtmospherics = FALSE;
    m_EnableEphemeris = FALSE;
    m_EnableMoon = FALSE;
    m_EnableStarfield = FALSE;
    m_EnableSun = FALSE;
    m_Date = 0;
    m_Time = 0;
    m_GlobalBaroPressure = 0.0f;
    m_GlobalHumidity = 0;
    m_GlobalTemperature = 0.0f;
    m_GlobalVisibility = 0.0f;
    m_GlobalWindDirection = 0.0f;
    m_GlobalWindHorizontal = 0.0f;
    m_GlobalWindVertical = 0.0f;
    m_StarfieldIntensity = 0.0f;
    //}}AFX_DATA_INIT
}

void CSkyStateView::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSkyStateView)
    DDX_Control(pDX, IDC_EDIT_INTENSITY, m_EditStarfieldIntensity);
    DDX_Control(pDX, IDC_TIME, m_TimeCtrl);
    DDX_Control(pDX, IDC_DATE, m_DateCtrl);
    DDX_Control(pDX, IDC_TAB1, m_Tab);
    DDX_Control(pDX, IDC_EDIT_GLOBAL_WIND_VERT, m_EditWindVertical);
    DDX_Control(pDX, IDC_EDIT_GLOBAL_WIND_HORZ, m_EditWindHorizontal);
    DDX_Control(pDX, IDC_EDIT_GLOBAL_WIND_DIR, m_EditWindDirection);
    DDX_Control(pDX, IDC_EDIT_GLOBAL_VISIBILITY, m_EditVisibility);
    DDX_Control(pDX, IDC_EDIT_GLOBAL_TEMP, m_EditTemperature);
    DDX_Control(pDX, IDC_EDIT_GLOBAL_HUMIDITY, m_EditHumidity);
    DDX_Control(pDX, IDC_EDIT_GLOBAL_BARO_PRESSURE, m_EditBaroPressure);
    DDX_Control(pDX, IDC_CHECK_SUN, m_CheckSun);
    DDX_Control(pDX, IDC_CHECK_STARFIELD, m_CheckStarfield);
    DDX_Control(pDX, IDC_CHECK_MOON, m_CheckMoon);
    DDX_Control(pDX, IDC_CHECK_EPHEMERIS, m_CheckEphemeris);
    DDX_Control(pDX, IDC_CHECK_ENABLE_ATMOSPHERICS, m_CheckAtmospherics);
    DDX_Check(pDX, IDC_CHECK_ENABLE_ATMOSPHERICS, m_EnableAtmospherics);
    DDX_Check(pDX, IDC_CHECK_EPHEMERIS, m_EnableEphemeris);
    DDX_Check(pDX, IDC_CHECK_MOON, m_EnableMoon);
    DDX_Check(pDX, IDC_CHECK_STARFIELD, m_EnableStarfield);
    DDX_Check(pDX, IDC_CHECK_SUN, m_EnableSun);
    DDX_DateTimeCtrl(pDX, IDC_DATE, m_Date);
    DDX_DateTimeCtrl(pDX, IDC_TIME, m_Time);
    DDX_Text(pDX, IDC_EDIT_GLOBAL_BARO_PRESSURE, m_GlobalBaroPressure);
    DDX_Text(pDX, IDC_EDIT_GLOBAL_HUMIDITY, m_GlobalHumidity);
    DDV_MinMaxUInt(pDX, m_GlobalHumidity, 0, 100);
    DDX_Text(pDX, IDC_EDIT_GLOBAL_TEMP, m_GlobalTemperature);
    DDX_Text(pDX, IDC_EDIT_GLOBAL_VISIBILITY, m_GlobalVisibility);
    DDX_Text(pDX, IDC_EDIT_GLOBAL_WIND_DIR, m_GlobalWindDirection);
    DDV_MinMaxFloat(pDX, m_GlobalWindDirection, -360.f, 360.f);
    DDX_Text(pDX, IDC_EDIT_GLOBAL_WIND_HORZ, m_GlobalWindHorizontal);
    DDX_Text(pDX, IDC_EDIT_GLOBAL_WIND_VERT, m_GlobalWindVertical);
    DDX_Text(pDX, IDC_EDIT_INTENSITY, m_StarfieldIntensity);
    DDV_MinMaxFloat(pDX, m_StarfieldIntensity, 0.f, 100.f);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSkyStateView, CDialog)
    //{{AFX_MSG_MAP(CSkyStateView)
    ON_BN_CLICKED(IDC_CHECK_ENABLE_ATMOSPHERICS, OnCheckEnableAtmospherics)
    ON_BN_CLICKED(IDC_CHECK_EPHEMERIS, OnCheckEphemeris)
    ON_BN_CLICKED(IDC_CHECK_MOON, OnCheckMoon)
    ON_BN_CLICKED(IDC_CHECK_STARFIELD, OnCheckStarfield)
    ON_BN_CLICKED(IDC_CHECK_SUN, OnCheckSun)
    ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE, OnDatetimechangeDate)
    ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIME, OnDatetimechangeTime)
    ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnSelchangingTab)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab)
    //}}AFX_MSG_MAP

    ON_MESSAGE(WM_USEREDIT, OnUserEditControl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkyStateView message handlers

void CSkyStateView::OnOK()
{
}

void CSkyStateView::OnCancel()
{
}

BOOL CSkyStateView::OnInitDialog()
{
    CDialog::OnInitDialog();

    CTime earliest(1979, 1, 1, 0, 0, 0);
    CTime latest(2037, 12, 31, 23, 59, 59);
    m_DateCtrl.SetRange(&earliest, &latest);
    m_TimeCtrl.SetFormat("HH':'mm");

    InitTabs();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CSkyStateView::InitTabs(void)
{
    // Add a tab for each of the child dialog boxes.
    m_Tab.InsertItem(0, "Celestial Sphere Components");
    m_Tab.InsertItem(1, "Atmospheric Components");

    // Lock the resources for the child dialog boxes.
    m_DlgTempl[0] = LockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE_ENTITY_COMPONENTS));
    m_DlgTempl[1] = LockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE_ENTITY_COMPONENTS));

    // Create an array of pointers to the child dialog boxes.
    m_TabDlg[0] = (CDialog *)&m_DlgCelSphereComponents;
    m_TabDlg[1] = (CDialog *)&m_DlgAtmosphericComponents;

    // Create child dialog boxes.
    const int tabcount = m_Tab.GetItemCount();
    ASSERT(tabcount == NUM_SKYTAB_PAGES);

    for (int i = 0; i < tabcount; i++) {
        m_TabDlg[i]->CreateIndirect(m_DlgTempl[i], &m_Tab);
        m_TabDlg[i]->SetWindowPos(NULL, 5, 25, 0, 0, SWP_NOSIZE);
    }

    // Simulate selection of the first item.
    SelectTab(0);
}

void CSkyStateView::RefreshView(void)
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();

    m_EnableAtmospherics = env->GetAtmosphericsEnable();
    m_EnableEphemeris = env->GetEphemerisEnable();
    m_EnableMoon = env->GetMoonEnable();
    m_EnableStarfield = env->GetStarfieldEnable();
    m_EnableSun = env->GetSunEnable();
    m_GlobalBaroPressure = env->GetBaroPressure();
    m_GlobalHumidity = env->GetHumidity();
    m_GlobalTemperature = env->GetTemperature();
    m_GlobalVisibility = env->GetVisibility();
    m_GlobalWindDirection = env->GetWindDirection();
    m_GlobalWindHorizontal = env->GetHorzWindspeed();
    m_GlobalWindVertical = env->GetVertWindspeed();
    m_StarfieldIntensity = env->GetStarfieldIntensity();
    m_Date = env->GetDateTime();
    m_Time = env->GetDateTime();

    UpdateData(FALSE);

    // Update the Components pages.
    PopulateCompPropPages(env);
}

void CSkyStateView::PopulateCompPropPages(CGlobalEnv *env)
{
    m_DlgCelSphereComponents.ClearComponents();
    m_DlgAtmosphericComponents.ClearComponents();

    int id = 0;
    CComponent *compptr = NULL;

    // Populate Celestial Sphere components.
    CComponentMap &celsphere_map = env->GetCelestialComponentMap();
    POSITION pos = celsphere_map.GetStartPosition();
    while (pos) {
        celsphere_map.GetNextAssoc(pos, id, compptr);

        m_DlgCelSphereComponents.AddComponent(compptr);
    }

    // Populate Atmosphere components.
    CComponentMap &atm_map = env->GetAtmosphereComponentMap();
    pos = atm_map.GetStartPosition();
    while (pos) {
        atm_map.GetNextAssoc(pos, id, compptr);

        m_DlgAtmosphericComponents.AddComponent(compptr);
    }
}

LRESULT CSkyStateView::OnUserEditControl(WPARAM wParam, LPARAM lParam)
{
    CEditKbd *ctrl = (CEditKbd *)lParam;

    if (ctrl == &m_EditStarfieldIntensity)
        OnUserChangeStarfieldIntensity();
    else if (ctrl == &m_EditWindVertical)
        OnUserChangeWindVertical();
    else if (ctrl == &m_EditWindHorizontal)
        OnUserChangeWindHorizontal();
    else if (ctrl == &m_EditWindDirection)
        OnUserChangeWindDirection();
    else if (ctrl == &m_EditVisibility)
        OnUserChangeVisibility();
    else if (ctrl == &m_EditTemperature)
        OnUserChangeTemperature();
    else if (ctrl == &m_EditHumidity)
        OnUserChangeHumidity();
    else if (ctrl == &m_EditBaroPressure)
        OnUserChangeBaroPressure();

    return 0;
}

void CSkyStateView::OnUserChangeStarfieldIntensity()
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    env->SetStarfieldIntensity(m_StarfieldIntensity);
}

void CSkyStateView::OnUserChangeWindVertical()
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    env->SetVertWindspeed(m_GlobalWindVertical);
}

void CSkyStateView::OnUserChangeWindHorizontal()
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    env->SetHorzWindspeed(m_GlobalWindHorizontal);
}

void CSkyStateView::OnUserChangeWindDirection()
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    env->SetWindDirection(m_GlobalWindDirection);
}

void CSkyStateView::OnUserChangeVisibility()
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    env->SetVisibility(m_GlobalVisibility);
}

void CSkyStateView::OnUserChangeTemperature()
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    env->SetTemperature(m_GlobalTemperature);
}

void CSkyStateView::OnUserChangeHumidity()
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    env->SetHumidity(m_GlobalHumidity);
}

void CSkyStateView::OnUserChangeBaroPressure()
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    env->SetBaroPressure(m_GlobalBaroPressure);
}

void CSkyStateView::OnCheckEnableAtmospherics()
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    env->SetAtmosphericsEnable(m_EnableAtmospherics ? TRUE : FALSE);
}

void CSkyStateView::OnCheckEphemeris()
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    env->SetEphemerisEnable(m_EnableEphemeris ? TRUE : FALSE);
}

void CSkyStateView::OnCheckMoon()
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    env->SetMoonEnable(m_EnableMoon ? TRUE : FALSE);
}

void CSkyStateView::OnCheckStarfield()
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    env->SetStarfieldEnable(m_EnableStarfield ? TRUE : FALSE);
}

void CSkyStateView::OnCheckSun()
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env)
        return;

    UpdateData(TRUE);

    env->SetSunEnable(m_EnableSun ? TRUE : FALSE);
}

void CSkyStateView::OnDatetimechangeDate(NMHDR *pNMHDR, LRESULT *pResult)
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (env) {
        UpdateData(TRUE);

        env->SetDate(m_Date);

        // Keep these synchronized.
        m_Time = m_Date;
    }

    *pResult = 0;
}

void CSkyStateView::OnDatetimechangeTime(NMHDR *pNMHDR, LRESULT *pResult)
{
    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (env) {
        UpdateData(TRUE);

        env->SetTime(m_Time);

        // Keep these synchronized.
        m_Date = m_Time;
    }

    *pResult = 0;
}

void CSkyStateView::OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Hide the current window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_HIDE);
    }

    *pResult = 0;
}

void CSkyStateView::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Show the appropriate window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_SHOW);
    }

    *pResult = 0;
}


void CSkyStateView::SelectTab(const int index)
{
    // Simulate selection of the item.
    LRESULT result;
    OnSelchangingTab(NULL, &result);

    m_Tab.SetCurSel(index);

    OnSelchangeTab(NULL, &result);
}
