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
 *  FILENAME:   FlyDlg.cpp
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

#include <math.h>

#include "stdafx.h"
#include "Entity.h"
#include "EntityStateView.h"
#include "FlyDlg.h"
#include "globals.h"
#include "hemu4.h"
#include "hemumsg.h"

#ifndef TBM_SETBKCOLOR
#define TBM_SETBKCOLOR (WM_USER + 19)
#endif

namespace
{
const double FixedWingThrottlePickupToleranceKnots = 5.0;
const double OtherThrottlePickupTolerance = 5.0;
const double RotorcraftThrottlePickupTolerance = 2.0;
const double KeyboardRollRateDegreesPerSecond = 50.0;
const double KeyboardPitchRateDegreesPerSecond = 20.0;
const double KeyboardYawRateDegreesPerSecond = 15.0;
const double KeyboardSpeedRatePerSecond = 50.0;
const double KeyboardCollectiveRatePerSecond = 25.0;
const COLORREF AcquiredThrottleBackground = RGB(202, 238, 202);
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlyDlg dialog
IMPLEMENT_DYNCREATE(CFlyDlg, CDialog)

CFlyDlg::CFlyDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CFlyDlg::IDD, pParent)
{
    m_dX = 0.0f;
    m_dY = 0.0f;
    m_dZ = 0.0f;
    m_dYaw = 0.0f;
    m_dPitch = 0.0f;
    m_dRoll = 0.0f;
    m_bSetRates = FALSE;
    m_HardwarePrecision = 0;
    ZeroMemory(&m_PreviousHardwareState, sizeof(m_PreviousHardwareState));
    m_HasPreviousHardwareState = FALSE;
    m_PovDirection = 0;
    m_PovHoldStarted = 0;
    m_PovLastRepeat = 0;
    m_PovImmediate = FALSE;
    m_HardwareEntityId = -1;
    m_LastHardwarePovDelta = 0;
    m_HardwareThrottleAvailable = FALSE;
    m_HardwareThrottleAcquired = FALSE;
    m_HasLastHardwareThrottleValue = FALSE;
    m_LastHardwareThrottleValue = 0.0;
    m_KeyboardAxesActive = FALSE;
    m_KeyboardEntityId = -1;
    m_KeyboardThrottleActive = FALSE;
    m_HasKeyboardThrottleValue = FALSE;
    m_KeyboardThrottleEntityId = -1;
    m_KeyboardThrottleValue = 0.0;

    //{{AFX_DATA_INIT(CFlyDlg)
    //}}AFX_DATA_INIT
}

void CFlyDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFlyDlg)
    DDX_Control(pDX, IDC_CHECK_PRECISION_50, m_CheckPrecision50);
    DDX_Control(pDX, IDC_CHECK_PRECISION_100, m_CheckPrecision100);
    DDX_Control(pDX, IDC_CHECK_PRECISION_10, m_CheckPrecision10);
    DDX_Control(pDX, IDC_LABEL_KNOTS, m_LabelKnots);
    DDX_Control(pDX, IDC_LABEL_AIRSPEED, m_LabelSpeed);
    DDX_Control(pDX, IDC_LABEL_THROTTLE_TOP, m_ThrottleLabelMax);
    DDX_Control(pDX, IDC_LABEL_THROTTLE_BOTTOM, m_ThrottleLabelMin);
    DDX_Control(pDX, IDC_LABEL_THROTTLE_MIDDLE, m_ThrottleLabelMiddle);
    DDX_Control(pDX, IDC_COMBO_FLY_WEAPON, m_ComboMissile);
    DDX_Control(pDX, IDC_COMBO_FLY_TARGET, m_ComboTarget);
    DDX_Control(pDX, IDC_BUTTON_RESET, m_ResetButton);
    DDX_Control(pDX, IDC_JOYBALL, m_JoyWindow);
    DDX_Control(pDX, IDC_HTRIM, m_HTrim);
    DDX_Control(pDX, IDC_VTRIM, m_VTrim);
    DDX_Control(pDX, IDC_HEADING, m_Heading);
    DDX_Control(pDX, IDC_SLIDER_THROTTLE, m_SpeedSlider);
    DDX_Control(pDX, IDC_SLIDER_ROLL, m_RollSlider);
    DDX_Control(pDX, IDC_SLIDER_PITCH, m_PitchSlider);
    DDX_Control(pDX, IDC_ROLL, m_Roll);
    DDX_Control(pDX, IDC_PITCH, m_Pitch);
    DDX_Control(pDX, IDC_LONGITUDE, m_Longitude);
    DDX_Control(pDX, IDC_LATITUDE, m_Latitude);
    DDX_Control(pDX, IDC_COMBO_MODE, m_FlyModeCombo);
    DDX_Control(pDX, IDC_BUTTON_FREEZE_RESUME, m_FreezeResumeButton);
    DDX_Control(pDX, IDC_BUTTON_FIRE, m_FireButton);
    DDX_Control(pDX, IDC_ALTITUDE, m_Altitude);
    DDX_Control(pDX, IDC_AIRSPEED, m_Speed);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlyDlg, CDialog)
    //{{AFX_MSG_MAP(CFlyDlg)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON_FREEZE_RESUME, OnButtonFreezeResume)
    ON_WM_MOUSEWHEEL()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin)
    ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
    ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_ROLL, OnReleasedcaptureSliderRoll)
    ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PITCH, OnReleasedcaptureSliderPitch)
    ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
    ON_BN_CLICKED(IDC_BUTTON_FIRE, OnButtonFire)
    ON_CBN_CLOSEUP(IDC_COMBO_FLY_TARGET, OnCloseupComboTarget)
    ON_CBN_CLOSEUP(IDC_COMBO_FLY_WEAPON, OnCloseupComboMissile)
    ON_CBN_SELCHANGE(IDC_COMBO_FLY_WEAPON, OnSelchangeMissile)
    ON_CBN_SELCHANGE(IDC_COMBO_FLY_TARGET, OnSelchangeTarget)
    ON_BN_CLICKED(IDC_CHECK_PRECISION_10, OnCheckPrecision10)
    ON_BN_CLICKED(IDC_CHECK_PRECISION_50, OnCheckPrecision50)
    ON_BN_CLICKED(IDC_CHECK_PRECISION_100, OnCheckPrecision100)
    ON_MESSAGE(WM_CHANGEJOYPOS, OnChangeJoyPos)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CFlyDlg::OnOK()
{
}

void CFlyDlg::OnCancel()
{
    // Hide the window when the user presses Escape.
    ShowWindow(SW_HIDE);
}

void CFlyDlg::RefreshView(void)
{
    static int prev_entity_id = -1;
    static int prev_missile_id = -1;
    static int prev_entity_class = 1000;    // initilize to invalid value
    BOOL child_selected = FALSE;
    CString temp;
    CEntity *entity = g_DataManager.GetSelectedEntity();

    if (!entity)
        return;

    // If the selected entity is a child, find the top-level ancestor.
    while (entity->GetParent() != -1) {
        child_selected = TRUE;
        entity = g_DataManager.GetEntity(entity->GetParent());
    }

    if (m_HardwareThrottleAcquired
        && entity->GetID() == m_HardwareEntityId
        && m_HasLastHardwareThrottleValue
        && fabs(GetEntityThrottleValue(entity)
                - m_LastHardwareThrottleValue) > 0.1) {
        DisengageHardwareThrottle();
    }

    if (prev_entity_class != entity->GetClass())
        ConfigureSpeedSlider();

    if (child_selected) {
        m_ComboMissile.EnableWindow(FALSE);
        m_ComboTarget.EnableWindow(FALSE);
    } else {
        m_ComboMissile.EnableWindow(TRUE);
        m_ComboTarget.EnableWindow(TRUE);

        if (prev_entity_id != entity->GetID()) {
            prev_entity_id = entity->GetID();
            UpdateMissileCombo();
            UpdateTargetCombo();
        }

        if (prev_missile_id != (int)(MAKE_ID(m_ComboMissile.GetItemData(m_ComboMissile.GetCurSel())))) {
            prev_missile_id = (int)(MAKE_ID(m_ComboMissile.GetItemData(m_ComboMissile.GetCurSel())));
            OnSelchangeMissile();
        }
    }

    temp.Format("Fly (Entity %d)", entity->GetID());
    SetWindowText((LPCTSTR)temp);

    if (entity->GetClass() == ENTITY_CLASS_ROTORCRAFT) {
        m_SpeedSlider.SetPos(100 - entity->GetCollective());
        temp.Format("%d", entity->GetCollective());
    } else if (entity->GetClass() == ENTITY_CLASS_FIXEDWING) {
        const double entitySpd = MPSToKnots(entity->GetSpeed());
        const int roundedEntitySpd = (entitySpd > 0) ? (int)floor(entitySpd + 0.5) : (int)ceil(entitySpd - 0.5);
        m_SpeedSlider.SetPos(1200 - roundedEntitySpd);
        temp.Format("%d", roundedEntitySpd);
    } else {
        const double entitySpd = entity->GetSpeed();
        const int roundedEntitySpd = (entitySpd > 0) ? (int)floor(entitySpd + 0.5) : (int)ceil(entitySpd - 0.5);
        m_SpeedSlider.SetPos(1200 - roundedEntitySpd);
        temp.Format("%d", roundedEntitySpd);
    }
    m_Speed.SetWindowText((LPCTSTR)temp);
    UpdateHardwareThrottleVisual();

    DOF dof = entity->GetDofs();

    temp.Format("%.6lf", dof.latitude);
    m_Latitude.SetWindowText((LPCTSTR)temp);

    temp.Format("%.6lf", dof.longitude);
    m_Longitude.SetWindowText((LPCTSTR)temp);

    temp.Format("%.2lf", dof.altitude);
    m_Altitude.SetWindowText((LPCTSTR)temp);

    temp.Format("%.2lf", dof.pitch);
    m_Pitch.SetWindowText((LPCTSTR)temp);

    temp.Format("%.2lf", dof.yaw);
    m_Heading.SetWindowText((LPCTSTR)temp);

    temp.Format("%.2lf", dof.roll);
    m_Roll.SetWindowText((LPCTSTR)temp);

    SetFreezeResumeButtonText();
}

void CFlyDlg::OnClose()
{
    // Just hide the window so the user can still use the joystick.
    ShowWindow(SW_HIDE);
}

void CFlyDlg::OnButtonReset()
{
    g_DataManager.ResetScenario();

    // Resetting the scenario will reset all entity state, including
    // its current fly mode. We'll want to restore the fly mode currently
    // selected in the GUI though, for consistency in the GUI.
    OnSelchangeComboMode();
}

void CFlyDlg::OnButtonFreezeResume()
{
    if (GetFreezeFlag()) {
        SetFreezeFlag(FALSE);
        SetResetIGFlag(FALSE);

        // Send a message to the RT driver.
        MESSAGE_RUN_EXERCISE msg;
        PostDriverMsg(msg);
    } else {
        SetFreezeFlag(TRUE);

        // Send a message to the RT driver.
        MESSAGE_PAUSE_EXERCISE msg;
        PostDriverMsg(msg);
    }

    SetFreezeResumeButtonText();

    // Refresh the active state view, so that the edit boxes can be
    // enabled/disabled appropriately.
    CBaseStateView *activeStateView = theApp.GetMainFrame().GetActiveStateView();

    // Ensure that there's actually a state view loaded before refreshing it.
    // (It's possible the user could open the Fly window and toggle the button
    // before any state view has been loaded at all.)
    if (activeStateView)
        activeStateView->RefreshView();
}

void CFlyDlg::SetFreezeResumeButtonText(void)
{
    const LPCSTR buttonText = GetFreezeFlag() ? "Resume" : "Freeze";

    if (m_FreezeResumeButton.GetSafeHwnd())
        m_FreezeResumeButton.SetWindowText(buttonText);
}

void CFlyDlg::ConfigureSpeedSlider(void)
{
    CEntity *entity = g_DataManager.GetSelectedEntity();

    if (!entity)
        return;

    switch (entity->GetClass()) {
    case ENTITY_CLASS_FIXEDWING:
        m_SpeedSlider.SetRange(0, 1500, FALSE);
        m_SpeedSlider.ClearTics();
        m_SpeedSlider.SetTic(1300);
        m_SpeedSlider.SetTic(1200);
        m_SpeedSlider.SetTic(1100);
        m_SpeedSlider.SetTic(900);
        m_SpeedSlider.SetTic(500);

        m_LabelSpeed.SetWindowText("Airspeed:");
        m_LabelKnots.SetWindowText("knots");
        m_ThrottleLabelMin.SetWindowText("-300");
        m_ThrottleLabelMax.SetWindowText("1200");
        m_ThrottleLabelMiddle.ShowWindow(SW_SHOW);

        break;

    case ENTITY_CLASS_ROTORCRAFT:
        m_SpeedSlider.SetRange(0, 100, FALSE);
        m_SpeedSlider.ClearTics();
        m_SpeedSlider.SetTic(100);
        m_SpeedSlider.SetTic(90);
        m_SpeedSlider.SetTic(80);
        m_SpeedSlider.SetTic(70);
        m_SpeedSlider.SetTic(60);
        m_SpeedSlider.SetTic(50);
        m_SpeedSlider.SetTic(40);
        m_SpeedSlider.SetTic(30);
        m_SpeedSlider.SetTic(20);
        m_SpeedSlider.SetTic(10);

        m_LabelSpeed.SetWindowText("Collective:");
        m_LabelKnots.SetWindowText("%");
        m_ThrottleLabelMin.SetWindowText("0");
        m_ThrottleLabelMax.SetWindowText("100");
        m_ThrottleLabelMiddle.ShowWindow(SW_HIDE);

        break;

    default:
        m_SpeedSlider.SetRange(0, 1500, FALSE);
        m_SpeedSlider.ClearTics();
        m_SpeedSlider.SetTic(1300);
        m_SpeedSlider.SetTic(1200);
        m_SpeedSlider.SetTic(1100);
        m_SpeedSlider.SetTic(900);
        m_SpeedSlider.SetTic(500);

        m_LabelSpeed.SetWindowText("Speed:");
        m_LabelKnots.SetWindowText("m/s");
        m_ThrottleLabelMin.SetWindowText("-300");
        m_ThrottleLabelMax.SetWindowText("1200");
        m_ThrottleLabelMiddle.ShowWindow(SW_SHOW);

        break;
    }
}

BOOL CFlyDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_JoyWindow.Init();
    m_JoyWindow.SetMaxX(100);
    m_JoyWindow.SetMaxY(100);

    m_RollSlider.SetRange(-100, 100, FALSE);
    m_RollSlider.SetTicFreq(25);
    m_RollSlider.SetPos(0);
    m_PitchSlider.SetRange(-100, 100, FALSE);
    m_PitchSlider.SetTicFreq(25);
    m_PitchSlider.SetPos(0);

    ConfigureSpeedSlider();
    m_SpeedSlider.SetPos(100);

    m_FlyModeCombo.SetCurSel(0);

    m_ComboTarget.SetImageList(&g_ImgListTypes);
    m_ComboMissile.SetImageList(&g_ImgListTypes);
    UpdateMissileCombo();
    UpdateTargetCombo();

    UpdatePrecisionControls();

    SetFreezeResumeButtonText();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

LRESULT CFlyDlg::OnChangeJoyPos(WPARAM wParam, LPARAM lParam)
{
    long x = (long)wParam;
    long y = (long)lParam;
    double theta, translatefactor, rollfactor, pitchfactor, yawfactor;
    CEntity *curr_entity = g_DataManager.GetSelectedEntity();
    if (!curr_entity)
        return 0;

    if (m_CheckPrecision10.GetCheck()) {
        translatefactor = 1.0f;
        rollfactor = 0.2f;
        pitchfactor = -0.03f;
        yawfactor = 0.02f;      // For taxiing only
    } else if (m_CheckPrecision50.GetCheck()) {
        translatefactor = 0.5f;
        rollfactor = 0.1f;
        pitchfactor = -0.015f;
        yawfactor = 0.01f;      // For taxiing only
    } else if (m_CheckPrecision100.GetCheck()) {
        translatefactor = 0.1f;
        rollfactor = 0.02f;
        pitchfactor = -0.003f;
        yawfactor = 0.002f;     // For taxiing only
    } else {
        translatefactor = 10.0f;
        rollfactor = 2.0f;
        pitchfactor = -0.3f;
        yawfactor = 0.2f;       // For taxiing only
    }

    CString temp;
    temp.Format("%d", y);
    m_VTrim.SetWindowText((LPCTSTR)temp);
    temp.Format("%d", x);
    m_HTrim.SetWindowText((LPCTSTR)temp);

    // These won't send scroll messages, so we don't have
    // to worry about infinite recursion.
    m_RollSlider.SetPos(x);
    m_PitchSlider.SetPos(-y);

    switch (curr_entity->GetFlyMode()) {
    case ENTITY_FLYMODE_FLY:
        switch (curr_entity->GetClass()) {
        case ENTITY_CLASS_ROTORCRAFT:
            // These are also used as desired (final) pitch and roll.
            m_dX = 0.25f * x;   // phi_max = +/- 25 degrees
            m_dY = 0.30f * y;   // theta_max = +/- 30 degrees
            m_dYaw *= (float)yawfactor; // m_dYaw was previously set in OnTimer()
            m_bSetRates = TRUE;
            break;

        default:
            m_dX = 0.0f;
            m_dY = 0.0f;
            m_dZ = 0.0f;

            if (curr_entity->GetClampMode() == ENTITY_CLAMP_CONFORMAL) {
                m_dRoll = 0;
                m_dPitch = 0;
                m_dYaw = (float)(yawfactor * x);        // left/right axis is yaw
            } else {
                m_dRoll = (float)(rollfactor * x);  // degrees per sec.
                m_dPitch = (float)(pitchfactor * y);    // degrees per sec.
                m_dYaw = 0.0f;              // ignore joystick rudder
            }

            m_bSetRates = TRUE;

            break;
        }

        break;

    case ENTITY_FLYMODE_MAGIC_CARPET:
        theta = DegToRad(curr_entity->GetDofs().yaw);
        m_dX = (float)(2 * translatefactor * ((y * cos(theta)) - (x * sin(theta))));
        m_dY = (float)(2 * translatefactor * ((x * cos(theta)) + (y * sin(theta))));
        m_dZ = 0.0f;
        m_dRoll = 0.0f;
        m_dPitch = 0.0f;
        // m_dYaw is set in OnTimer().

        m_bSetRates = TRUE;

        break;

    case ENTITY_FLYMODE_YAW_Z:
        m_dX = 0.0f;
        m_dY = 0.0f;
        m_dZ = (float)(-translatefactor * y);
        m_dRoll = 0.0f;
        m_dPitch = 0.0f;
        m_dYaw = x * 2.0f;  // degrees per sec.

        m_bSetRates = TRUE;

        break;

    case ENTITY_FLYMODE_ROLL_PITCH:
    default:
        m_dX = 0.0f;
        m_dY = 0.0f;
        m_dZ = 0.0f;
        m_dRoll = (float)(rollfactor * x);  // degrees per sec.
        m_dPitch = (float)(pitchfactor * y);    // degrees per sec.
        m_dYaw = 0.0f;              // ignore joystick rudder

        m_bSetRates = TRUE;

        break;
    }

    if (m_bSetRates) {
        RATES rates;
        rates.dx = m_dX;
        rates.dy = m_dY;
        rates.dz = m_dZ;
        rates.dyaw = m_dYaw;
        rates.dpitch = m_dPitch;
        rates.droll = m_dRoll;

        const RATES oldRates = curr_entity->GetAndLockRates();
        if (memcmp(&rates, &oldRates, sizeof(RATES)) != 0)
            curr_entity->SetLockedRates(rates);
        curr_entity->Unlock();
        m_bSetRates = FALSE;
    }

    return 0;
}

void CFlyDlg::DoMouseWheelThrottle(UINT nFlags, short zDelta, CPoint pt)
{
    short d = zDelta / WHEEL_DELTA;
    short factor = 1;
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    switch (entity->GetClass()) {
    case ENTITY_CLASS_FIXEDWING: {
        if (nFlags & MK_SHIFT)
            factor = 100;

        m_SpeedSlider.SetPos(((m_SpeedSlider.GetPos() / factor) - d) * factor);

        // Set the rates.
        int nPos = m_SpeedSlider.GetPos();
        entity->SetSpeed(KnotsToMPS(1200 - nPos));

        break;
    }

    case ENTITY_CLASS_ROTORCRAFT: {
        if (nFlags & MK_SHIFT)
            factor = 10;

        // Save the collective to a signed int to limit to 0.
        int coll = entity->GetCollective() + (d * factor);
        if (coll > 100)
            coll = 100;
        else if (coll < 0)
            coll = 0;
        entity->SetCollective(coll);

        m_SpeedSlider.SetPos(100 - coll);

        break;
    }

    default: {
        if (nFlags & MK_SHIFT)
            factor = 100;

        m_SpeedSlider.SetPos(((m_SpeedSlider.GetPos() / factor) - d) * factor);

        // Set the rates.
        int nPos = m_SpeedSlider.GetPos();
        entity->SetSpeed(1200 - nPos);

        break;
    }
    }

    DisengageHardwareThrottle();
}

BOOL CFlyDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    DoMouseWheelThrottle(nFlags, zDelta, pt);
    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CFlyDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
    if (pScrollBar->m_hWnd == m_RollSlider.m_hWnd) {
        // Set the position of the joystick ball.  We can't just
        // pass the value of nPos because when we release the slider,
        // we will get a WM_ON_HSCROLL message and nPos will be zero.
        m_JoyWindow.SetPosX(m_RollSlider.GetPos());

        // If we are using the keyboard, nPos = 0.  We need to read the
        // slider to find the correct new value.
        if (nSBCode != SB_THUMBPOSITION && nSBCode != SB_THUMBTRACK)
            nPos = m_RollSlider.GetPos();

        CString trim;
        trim.Format("%d", (int)nPos);
        m_HTrim.SetWindowText((LPCTSTR)trim);
    }

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CFlyDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
    if (pScrollBar->m_hWnd == m_PitchSlider.m_hWnd) {
        // Set the position of the joystick ball.  We can't just
        // pass the value of nPos because when we release the slider,
        // we will get a WM_ON_VSCROLL message and nPos will be zero.
        m_JoyWindow.SetPosY(-m_PitchSlider.GetPos());


        // If we are using the keyboard, nPos = 0.  We need to read the
        // slider to find the correct new value.
        if (nSBCode != SB_THUMBPOSITION && nSBCode != SB_THUMBTRACK)
            nPos = m_PitchSlider.GetPos();

        CString trim;
        trim.Format("%d", -(int)nPos);
        m_VTrim.SetWindowText((LPCTSTR)trim);
    } else if (pScrollBar->m_hWnd == m_SpeedSlider.m_hWnd) {
        // If we are using the keyboard, nPos = 0.  We need to read the
        // slider to find the correct new value.
        if (nSBCode != SB_THUMBPOSITION && nSBCode != SB_THUMBTRACK)
            nPos = m_SpeedSlider.GetPos();

        CEntity *entity = g_DataManager.GetSelectedEntity();
        if (!entity)
            return;

        // Update the entity.state.
        switch (entity->GetClass()) {
        case ENTITY_CLASS_FIXEDWING: {
            entity->SetSpeed(KnotsToMPS(1200 - (int)nPos));

            break;
        }

        case ENTITY_CLASS_ROTORCRAFT: {
            entity->SetCollective(100 - nPos);

            break;
        }

        default: {
            entity->SetSpeed(1200 - (int)nPos);

            break;
        }
        }

        DisengageHardwareThrottle();
    }

    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CFlyDlg::OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
    int max = m_SpeedSlider.GetRangeMax();
    int min = m_SpeedSlider.GetRangeMin();
    int pos = m_SpeedSlider.GetPos();
    int delta = ((NM_UPDOWN *)pNMHDR)->iDelta;

    // Add the proposed change to the current value.
    if ((pos + delta <= max) && (pos + delta >= min))
        pos += delta;

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Update the entity.state.
    switch (entity->GetClass()) {
		case ENTITY_CLASS_FIXEDWING:
			entity->SetSpeed(KnotsToMPS(1200 - pos));
			break;

		case ENTITY_CLASS_ROTORCRAFT:
			entity->SetCollective(100 - pos);
			break;

		default:
			entity->SetSpeed(1200 - pos);
			break;
    }

    DisengageHardwareThrottle();

    *pResult = 0;
}

void CFlyDlg::OnReleasedcaptureSliderRoll(NMHDR *pNMHDR, LRESULT *pResult)
{
    m_SpeedSlider.SetFocus();

    *pResult = 0;
}

void CFlyDlg::OnReleasedcaptureSliderPitch(NMHDR *pNMHDR, LRESULT *pResult)
{
    m_SpeedSlider.SetFocus();

    *pResult = 0;
}

void CFlyDlg::OnSelchangeComboMode()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();

    if (entity) {
        // Signal the driver to change the fly mode.
        entity->SetFlyMode(m_FlyModeCombo.GetCurSel());

        // Reset the joystick after changing fly mode.
        m_JoyWindow.SetPosX(0);
        m_JoyWindow.SetPosY(0);
    }
}

void CFlyDlg::ProcessHardwareJoystickState(
    const HEMU_JOYSTICK_STATE &state)
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    const DWORD now = GetTickCount();

    const int selectedEntityId = entity ? entity->GetID() : -1;
    const BOOL entityChanged = selectedEntityId != m_HardwareEntityId;
    if (entityChanged) {
        CEntity *previousEntity = g_DataManager.GetEntity(m_HardwareEntityId);
        if (previousEntity) {
            HEMU_JOYSTICK_STATE neutralState;
            ZeroMemory(&neutralState, sizeof(neutralState));
            neutralState.pov = (DWORD)-1;
            ApplyHardwareRates(previousEntity, neutralState, 0);
        }
        m_HardwareEntityId = selectedEntityId;
        m_HardwareThrottleAcquired = FALSE;
        m_HasLastHardwareThrottleValue = FALSE;
        UpdateHardwareThrottleVisual();
    }
    m_HardwareThrottleAvailable = state.hasThrottle;

    int povDirection = 0;
    if (state.pov == 0)
        povDirection = 1;
    else if (state.pov == 18000)
        povDirection = -1;

    if (povDirection != m_PovDirection) {
        m_PovDirection = povDirection;
        m_PovHoldStarted = now;
        m_PovLastRepeat = now;
        m_PovImmediate = povDirection ? TRUE : FALSE;
    }

    DWORD heldMilliseconds = m_PovDirection ? now - m_PovHoldStarted : 0;
    int povMagnitude = m_PovDirection ? 1 : 0;
    if (heldMilliseconds >= 2000)
        ++povMagnitude;
    if (heldMilliseconds >= 6000)
        ++povMagnitude;
    const int povDelta = m_PovDirection * povMagnitude;
    const BOOL hardwareAxesChanged = !m_HasPreviousHardwareState
        || state.x != m_PreviousHardwareState.x
        || state.y != m_PreviousHardwareState.y
        || state.rudder != m_PreviousHardwareState.rudder;
    const BOOL povRateChanged = povDelta != m_LastHardwarePovDelta;

    if (GetSafeHwnd()) {
        if (!m_HasPreviousHardwareState
            || state.x != m_PreviousHardwareState.x) {
            m_JoyWindow.SetPosX(state.x, FALSE);
            m_RollSlider.SetPos(state.x);
        }
        if (!m_HasPreviousHardwareState
            || state.y != m_PreviousHardwareState.y) {
            m_JoyWindow.SetPosY(-state.y, FALSE);
            m_PitchSlider.SetPos(state.y);
        }
    }

    if (entity) {
        if (state.hasThrottle && !m_KeyboardThrottleActive) {
            const double hardwareThrottle =
                GetHardwareThrottleValue(entity, state);
            const double entityThrottle = GetEntityThrottleValue(entity);

            if (!m_HardwareThrottleAcquired
                && fabs(hardwareThrottle - entityThrottle)
                   <= GetHardwareThrottleTolerance(entity)) {
                AcquireHardwareThrottle(entity);
            } else if (entityChanged && !m_HardwareThrottleAcquired) {
                ShowHardwareThrottleWaitingStatus(entity);
            }

            if (m_HardwareThrottleAcquired) {
                if (fabs(hardwareThrottle - entityThrottle) > 0.01)
                    SetEntityThrottleValue(entity, hardwareThrottle);
                m_LastHardwareThrottleValue = hardwareThrottle;
                m_HasLastHardwareThrottleValue = TRUE;
            }
        } else if (m_HardwareThrottleAcquired) {
            DisengageHardwareThrottle();
        }

        // Hardware rates persist in the entity. Reapplying an unchanged,
        // centered device here would overwrite the on-screen joystick at the
        // hardware polling rate. Apply only when the physical input changes.
        if (!m_KeyboardAxesActive
            && (entityChanged || hardwareAxesChanged || povRateChanged)) {
            ApplyHardwareRates(entity, state, povDelta);
        }

        const BOOL repeatPov = m_PovDirection
            && heldMilliseconds >= 220
            && now - m_PovLastRepeat >= 20;
        if (m_PovImmediate || repeatPov) {
            if (entity->GetFlyMode() == ENTITY_FLYMODE_FLY) {
                if (entity->GetClass() == ENTITY_CLASS_ROTORCRAFT) {
                    int collective = entity->GetCollective();
                    const int collectiveDelta = repeatPov
                        ? m_PovDirection * 5 : m_PovDirection;
                    collective = max(0, min(100,
                        collective + collectiveDelta));
                    entity->SetCollective(collective);
                    if (GetSafeHwnd())
                        m_SpeedSlider.SetPos(100 - collective);
                } else {
                    if (entity->GetClass() == ENTITY_CLASS_FIXEDWING) {
                        double speed =
                            MPSToKnots(entity->GetSpeed()) + povDelta;
                        speed = max(0.0, min(1200.0, speed));
                        entity->SetSpeed(KnotsToMPS(speed));
                    } else {
                        double speed = entity->GetSpeed() + povDelta;
                        speed = max(0.0, min(1200.0, speed));
                        entity->SetSpeed(speed);
                    }
                }
                DisengageHardwareThrottle();
            }

            m_PovImmediate = FALSE;
            m_PovLastRepeat = now;
        }
    }

    const BOOL button0Pressed = (state.buttons[0] & 0x80) != 0;
    const BOOL button1Pressed = (state.buttons[1] & 0x80) != 0;
    const BOOL button2Pressed = (state.buttons[2] & 0x80) != 0;
    const BOOL button4Pressed = (state.buttons[4] & 0x80) != 0;

    if (button0Pressed && (!m_HasPreviousHardwareState
        || !(m_PreviousHardwareState.buttons[0] & 0x80))) {
        if (GetSafeHwnd())
            OnButtonFire();
    }

    if (button1Pressed && (!m_HasPreviousHardwareState
        || !(m_PreviousHardwareState.buttons[1] & 0x80))) {
        if (GetSafeHwnd() && m_TargetCount > 1) {
            m_ComboTarget.SetCurSel(
                (m_ComboTarget.GetCurSel() + 1) % m_TargetCount);
            OnSelchangeTarget();
        }
    }

    if (button2Pressed && (!m_HasPreviousHardwareState
        || !(m_PreviousHardwareState.buttons[2] & 0x80))) {
        OnButtonFreezeResume();
    }

    if (button4Pressed && (!m_HasPreviousHardwareState
        || !(m_PreviousHardwareState.buttons[4] & 0x80))) {
        m_HardwarePrecision = (m_HardwarePrecision + 1) % 4;
        UpdatePrecisionControls();
        if (entity && !m_KeyboardAxesActive)
            ApplyHardwareRates(entity, state, povDelta);
    }

    m_LastHardwarePovDelta = povDelta;
    m_PreviousHardwareState = state;
    m_HasPreviousHardwareState = TRUE;
}

void CFlyDlg::ApplyHardwareRates(CEntity *entity,
                                 const HEMU_JOYSTICK_STATE &state,
                                 const int povDelta)
{
    if (!entity)
        return;

    double translateFactor = 10.0;
    double rollFactor = 2.0;
    double pitchFactor = -0.3;
    double yawFactor = 0.2;
    if (m_HardwarePrecision == 1) {
        translateFactor = 1.0;
        rollFactor = 0.2;
        pitchFactor = -0.03;
        yawFactor = 0.02;
    } else if (m_HardwarePrecision == 2) {
        translateFactor = 0.5;
        rollFactor = 0.1;
        pitchFactor = -0.015;
        yawFactor = 0.01;
    } else if (m_HardwarePrecision == 3) {
        translateFactor = 0.1;
        rollFactor = 0.02;
        pitchFactor = -0.003;
        yawFactor = 0.002;
    }

    const long x = state.x;
    const long y = -state.y;
    const double rudder = state.rudder * 0.5;

    m_dX = 0.0f;
    m_dY = 0.0f;
    m_dZ = 0.0f;
    m_dYaw = 0.0f;
    m_dPitch = 0.0f;
    m_dRoll = 0.0f;

    switch (entity->GetFlyMode()) {
    case ENTITY_FLYMODE_FLY:
        if (entity->GetClass() == ENTITY_CLASS_ROTORCRAFT) {
            m_dX = (float)(0.25 * x);
            m_dY = (float)(0.30 * y);
            m_dYaw = (float)(rudder * yawFactor);
        } else if (entity->GetClampMode() == ENTITY_CLAMP_CONFORMAL) {
            m_dYaw = (float)(yawFactor * x);
        } else {
            m_dRoll = (float)(rollFactor * x);
            m_dPitch = (float)(pitchFactor * y);
        }
        break;

    case ENTITY_FLYMODE_MAGIC_CARPET: {
        const double theta = DegToRad(entity->GetDofs().yaw);
        m_dX = (float)(2 * translateFactor
                       * ((y * cos(theta)) - (x * sin(theta))));
        m_dY = (float)(2 * translateFactor
                       * ((x * cos(theta)) + (y * sin(theta))));
        m_dZ = (float)(translateFactor * povDelta * 10);
        m_dYaw = (float)(rudder * yawFactor);
        break;
    }

    case ENTITY_FLYMODE_YAW_Z:
        m_dZ = (float)(-translateFactor * y);
        m_dYaw = (float)(x * 2.0);
        break;

    case ENTITY_FLYMODE_ROLL_PITCH:
    default:
        m_dRoll = (float)(rollFactor * x);
        m_dPitch = (float)(pitchFactor * y);
        break;
    }

    RATES rates;
    rates.dx = m_dX;
    rates.dy = m_dY;
    rates.dz = m_dZ;
    rates.dyaw = m_dYaw;
    rates.dpitch = m_dPitch;
    rates.droll = m_dRoll;

    const RATES oldRates = entity->GetAndLockRates();
    if (memcmp(&rates, &oldRates, sizeof(RATES)) != 0)
        entity->SetLockedRates(rates);
    entity->Unlock();
}

void CFlyDlg::ProcessKeyboardFlightState(
    const int rollDirection,
    const int pitchDirection,
    const int yawDirection)
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    const int selectedEntityId = entity ? entity->GetID() : -1;
    const BOOL axesActive =
        rollDirection || pitchDirection || yawDirection;

    if (selectedEntityId != m_KeyboardEntityId) {
        CEntity *previousEntity =
            g_DataManager.GetEntity(m_KeyboardEntityId);
        if (previousEntity)
            ApplyKeyboardRates(previousEntity, 0, 0, 0);
        m_KeyboardEntityId = selectedEntityId;
    }

    m_KeyboardAxesActive = axesActive && entity;
    if (entity)
        ApplyKeyboardRates(
            entity, rollDirection, pitchDirection, yawDirection);

    if (!m_KeyboardAxesActive)
        m_KeyboardEntityId = -1;
}

void CFlyDlg::ApplyKeyboardRates(
    CEntity *entity,
    const int rollDirection,
    const int pitchDirection,
    const int yawDirection)
{
    if (!entity)
        return;

    RATES rates;
    ZeroMemory(&rates, sizeof(rates));

    if (entity->GetClass() == ENTITY_CLASS_ROTORCRAFT
        && entity->GetFlyMode() == ENTITY_FLYMODE_FLY) {
        rates.dx = (float)(25.0 * rollDirection);
        rates.dy = (float)(30.0 * pitchDirection);
        rates.dyaw =
            (float)(KeyboardYawRateDegreesPerSecond * yawDirection);
    } else {
        rates.droll =
            (float)(KeyboardRollRateDegreesPerSecond * rollDirection);
        rates.dpitch =
            (float)(-KeyboardPitchRateDegreesPerSecond * pitchDirection);
        rates.dyaw =
            (float)(KeyboardYawRateDegreesPerSecond * yawDirection);
    }

    const RATES oldRates = entity->GetAndLockRates();
    if (memcmp(&rates, &oldRates, sizeof(RATES)) != 0)
        entity->SetLockedRates(rates);
    entity->Unlock();
}

void CFlyDlg::AdjustKeyboardThrottle(
    const int direction, const double elapsedSeconds)
{
    if (!direction) {
        m_KeyboardThrottleActive = FALSE;
        m_HasKeyboardThrottleValue = FALSE;
        m_KeyboardThrottleEntityId = -1;
        return;
    }

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    const int selectedEntityId = entity->GetID();
    if (!m_HasKeyboardThrottleValue
        || selectedEntityId != m_KeyboardThrottleEntityId) {
        m_KeyboardThrottleValue = GetEntityThrottleValue(entity);
        m_HasKeyboardThrottleValue = TRUE;
        m_KeyboardThrottleEntityId = selectedEntityId;
    }

    const double rate =
        entity->GetClass() == ENTITY_CLASS_ROTORCRAFT
            ? KeyboardCollectiveRatePerSecond
            : KeyboardSpeedRatePerSecond;
    const double minimum = 0.0;
    const double maximum =
        entity->GetClass() == ENTITY_CLASS_ROTORCRAFT ? 100.0 : 1200.0;

    m_KeyboardThrottleValue = max(minimum, min(maximum,
        m_KeyboardThrottleValue
            + direction * rate * max(0.0, elapsedSeconds)));
    m_KeyboardThrottleActive = TRUE;
    SetEntityThrottleValue(entity, m_KeyboardThrottleValue);
    DisengageHardwareThrottle(FALSE);
}

void CFlyDlg::DisableKeyboardFlight(void)
{
    CEntity *entity = g_DataManager.GetEntity(m_KeyboardEntityId);
    if (entity)
        ApplyKeyboardRates(entity, 0, 0, 0);

    m_KeyboardAxesActive = FALSE;
    m_KeyboardEntityId = -1;
    m_KeyboardThrottleActive = FALSE;
    m_HasKeyboardThrottleValue = FALSE;
    m_KeyboardThrottleEntityId = -1;
}

void CFlyDlg::DisableHardwareJoystick(void)
{
    m_HasPreviousHardwareState = FALSE;
    m_PovDirection = 0;
    m_PovImmediate = FALSE;
    m_LastHardwarePovDelta = 0;
    m_HardwareThrottleAvailable = FALSE;
    m_HardwareThrottleAcquired = FALSE;
    m_HasLastHardwareThrottleValue = FALSE;

    HEMU_JOYSTICK_STATE neutralState;
    ZeroMemory(&neutralState, sizeof(neutralState));
    neutralState.pov = (DWORD)-1;

    CEntity *entity = g_DataManager.GetEntity(m_HardwareEntityId);
    if (entity)
        ApplyHardwareRates(entity, neutralState, 0);
    m_HardwareEntityId = -1;

    if (GetSafeHwnd()) {
        m_JoyWindow.SetPosX(0, FALSE);
        m_JoyWindow.SetPosY(0, FALSE);
        m_RollSlider.SetPos(0);
        m_PitchSlider.SetPos(0);
        UpdateHardwareThrottleVisual();
    }
}

double CFlyDlg::GetEntityThrottleValue(CEntity *entity) const
{
    if (!entity)
        return 0.0;

    if (entity->GetClass() == ENTITY_CLASS_ROTORCRAFT)
        return entity->GetCollective();
    if (entity->GetClass() == ENTITY_CLASS_FIXEDWING)
        return MPSToKnots(entity->GetSpeed());
    return entity->GetSpeed();
}

double CFlyDlg::GetHardwareThrottleValue(
    CEntity *entity, const HEMU_JOYSTICK_STATE &state) const
{
    const double rawThrottle =
        max(-300.0, min(1200.0, (double)state.throttle));
    const double normalizedThrottle =
        (rawThrottle + 300.0) / 1500.0;
    if (entity && entity->GetClass() == ENTITY_CLASS_ROTORCRAFT) {
        return max(0.0, min(100.0,
            normalizedThrottle * 100.0));
    }

    return max(0.0, min(1200.0, normalizedThrottle * 1200.0));
}

double CFlyDlg::GetHardwareThrottleTolerance(CEntity *entity) const
{
    if (entity && entity->GetClass() == ENTITY_CLASS_ROTORCRAFT)
        return RotorcraftThrottlePickupTolerance;
    if (entity && entity->GetClass() == ENTITY_CLASS_FIXEDWING)
        return FixedWingThrottlePickupToleranceKnots;
    return OtherThrottlePickupTolerance;
}

void CFlyDlg::SetEntityThrottleValue(
    CEntity *entity, const double value)
{
    if (!entity)
        return;

    if (entity->GetClass() == ENTITY_CLASS_ROTORCRAFT) {
        const double safeValue = max(0.0, min(100.0, value));
        entity->SetCollective((long)floor(safeValue + 0.5));
        if (GetSafeHwnd())
            m_SpeedSlider.SetPos(100 - (int)floor(safeValue + 0.5));
    } else if (entity->GetClass() == ENTITY_CLASS_FIXEDWING) {
        const double safeValue = max(0.0, min(1200.0, value));
        entity->SetSpeed(KnotsToMPS(safeValue));
        if (GetSafeHwnd())
            m_SpeedSlider.SetPos(1200 - (int)floor(safeValue + 0.5));
    } else {
        const double safeValue = max(0.0, min(1200.0, value));
        entity->SetSpeed(safeValue);
        if (GetSafeHwnd())
            m_SpeedSlider.SetPos(1200 - (int)floor(safeValue + 0.5));
    }
}

void CFlyDlg::DisengageHardwareThrottle(const BOOL showWaitingStatus)
{
    if (!m_HardwareThrottleAcquired)
        return;

    m_HardwareThrottleAcquired = FALSE;
    m_HasLastHardwareThrottleValue = FALSE;
    UpdateHardwareThrottleVisual();

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (showWaitingStatus && entity && m_HardwareThrottleAvailable)
        ShowHardwareThrottleWaitingStatus(entity);
}

void CFlyDlg::UpdateHardwareThrottleVisual(void)
{
    if (!GetSafeHwnd())
        return;

    CEntity *entity = g_DataManager.GetSelectedEntity();
    const BOOL acquired = m_HardwareThrottleAcquired
        && entity && entity->GetID() == m_HardwareEntityId;
    const COLORREF background = acquired
        ? AcquiredThrottleBackground : GetSysColor(COLOR_BTNFACE);
    m_SpeedSlider.SendMessage(TBM_SETBKCOLOR, 0, background);
    m_SpeedSlider.Invalidate();
}

void CFlyDlg::ShowHardwareThrottleWaitingStatus(CEntity *entity)
{
    if (!entity)
        return;

    CString text;
    if (entity->GetClass() == ENTITY_CLASS_ROTORCRAFT) {
        text.Format(
            "Throttle waiting: move to %d%% to control Entity %d",
            (int)floor(GetEntityThrottleValue(entity) + 0.5),
            entity->GetID());
    } else {
        text.Format(
            "Throttle waiting: move to %d %s to control Entity %d",
            (int)floor(GetEntityThrottleValue(entity) + 0.5),
            entity->GetClass() == ENTITY_CLASS_FIXEDWING ? "knots" : "m/s",
            entity->GetID());
    }
    theApp.GetMainFrame().ShowTransientStatus(text, 3500);
}

void CFlyDlg::AcquireHardwareThrottle(CEntity *entity)
{
    if (!entity)
        return;

    m_HardwareThrottleAcquired = TRUE;
    UpdateHardwareThrottleVisual();

    CString text;
    text.Format(
        "Throttle matched: joystick now controls Entity %d",
        entity->GetID());
    theApp.GetMainFrame().ShowTransientStatus(text, 3000);
}

void CFlyDlg::UpdatePrecisionControls(void)
{
    if (!GetSafeHwnd())
        return;

    m_CheckPrecision10.SetCheck(m_HardwarePrecision == 1);
    m_CheckPrecision50.SetCheck(m_HardwarePrecision == 2);
    m_CheckPrecision100.SetCheck(m_HardwarePrecision == 3);
}

void CFlyDlg::OnButtonFire()
{
    int missile_id = MAKE_ID(m_ComboMissile.GetItemData(m_ComboMissile.GetCurSel()));
    if (missile_id < 0)
        return;

    int target_id = MAKE_ID(m_ComboTarget.GetItemData(m_ComboTarget.GetCurSel()));
    if (target_id < 0)
        return;

    CEntity *missile = g_DataManager.GetEntity(missile_id);
    FireMissile(missile);

    UpdateMissileCombo();
}

void CFlyDlg::OnCloseupComboTarget()
{
    // Since I can't get the WM_MOUSEWHEEL handler to work in CNoMWComboBoxEx,
    // I have to do this to return the mousewheel focus to the throttle slider.
    m_SpeedSlider.SetFocus();
}

void CFlyDlg::OnCloseupComboMissile()
{
    // Since I can't get the WM_MOUSEWHEEL handler to work in CNoMWComboBoxEx,
    // I have to do this to return the mousewheel focus to the throttle slider.
    m_SpeedSlider.SetFocus();
}

void CFlyDlg::UpdateMissileCombo(void)
{
    int item_id = 0;
    int current_id = -1;
    POSITION pos = NULL;
    CEntityMap &map = g_DataManager.GetEntityMap();
    CEntity *entity = NULL;
    CEntity *current = g_DataManager.GetSelectedEntity();

    if (current)
        current_id = current->GetID();

    // Clear the list and add an empty item.
    m_ComboMissile.ResetContent();

    char text[256] = {0};
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_GENERIC, IMG_GENERIC, 0, 0, -1
                             };
    cboitem.pszText = text;
    cboitem.cchTextMax = strlen(text + 1);

    // Now add items from the DataManager's linked list.
    pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, item_id, entity);
        if ((entity->GetParent() == current_id)
            && ((entity->GetClass() == ENTITY_CLASS_MISSILE) || (entity->GetClass() == ENTITY_CLASS_BOMB))) {
            strcpy(text, entity->GetName());
            cboitem.iImage = entity->GetClass();
            cboitem.iSelectedImage = cboitem.iImage;
            cboitem.lParam = MAKE_TREE_DATA(entity->GetID(), entity->GetType(), entity->GetClass());
            m_ComboMissile.InsertItem(&cboitem);
        }
    }

    m_ComboMissile.SetCurSel(0);
}

void CFlyDlg::UpdateTargetCombo(void)
{
    int item_id = 0;
    POSITION pos = NULL;
    CEntityMap &map = g_DataManager.GetEntityMap();
    CEntity *entity = NULL;
    int current_id = -1;
    CEntity *current = g_DataManager.GetSelectedEntity();
    if (current)
        current_id = current->GetID();

    // Clear the list and add an empty item.
    m_ComboTarget.ResetContent();

    char text[256] = {0};
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_GENERIC, IMG_GENERIC, 0, 0, -1
                             };
    cboitem.pszText = text;
    cboitem.cchTextMax = strlen(text + 1);

    // Now add items from the DataManager's linked list.
    pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, item_id, entity);

        if ((entity->GetParent() == -1) && (entity->GetID() != current_id)
            && (entity->GetClass() != ENTITY_CLASS_MISSILE)
            && (entity->GetClass() != ENTITY_CLASS_BOMB)) {
            strcpy(text, entity->GetName());
            cboitem.iImage = entity->GetClass();
            cboitem.iSelectedImage = cboitem.iImage;
            cboitem.lParam = MAKE_TREE_DATA(entity->GetID(), entity->GetType(), entity->GetClass());
            m_ComboTarget.InsertItem(&cboitem);
        }
    }

    SelectTargetItem();
}

void CFlyDlg::SelectTargetItem(void)
{
    int itemidx = MAKE_ID(m_ComboMissile.GetItemData(m_ComboMissile.GetCurSel()));
    CEntity *missile = g_DataManager.GetEntity(itemidx);
    CEntity *target = NULL;

    if (missile)
        target = g_DataManager.GetEntity(missile->GetTarget());

    if (target) {
        int comboSelection = -1;
        const int count = m_ComboTarget.GetCount();

        for (int i = 0; i < count; i++) {
            int id = MAKE_ID(m_ComboTarget.GetItemData(i));
            if (id == target->GetID()) {
                comboSelection = i;
                break;
            }
        }

        m_ComboTarget.SetCurSel(comboSelection);
    }
}

void CFlyDlg::OnSelchangeMissile()
{
    SelectTargetItem();
}

void CFlyDlg::OnSelchangeTarget()
{
    CEntity *missile = g_DataManager.GetEntity(MAKE_ID(m_ComboMissile.GetItemData(m_ComboMissile.GetCurSel())));

    if (missile) {
        missile->SetTarget(MAKE_ID(m_ComboTarget.GetItemData(m_ComboTarget.GetCurSel())));
        g_DataManager.SetEntity(missile);
    }
}

void CFlyDlg::OnCheckPrecision10()
{
    m_CheckPrecision50.SetCheck(FALSE);
    m_CheckPrecision100.SetCheck(FALSE);
    m_HardwarePrecision = m_CheckPrecision10.GetCheck() ? 1 : 0;
}

void CFlyDlg::OnCheckPrecision50()
{
    m_CheckPrecision10.SetCheck(FALSE);
    m_CheckPrecision100.SetCheck(FALSE);
    m_HardwarePrecision = m_CheckPrecision50.GetCheck() ? 2 : 0;
}

void CFlyDlg::OnCheckPrecision100()
{
    m_CheckPrecision10.SetCheck(FALSE);
    m_CheckPrecision50.SetCheck(FALSE);
    m_HardwarePrecision = m_CheckPrecision100.GetCheck() ? 3 : 0;
}
