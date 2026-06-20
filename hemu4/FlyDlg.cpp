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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Callback function for enumerating joystick axes.  We will be setting
// the range for each axis.
BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
    DIPROPRANGE rangeinfo;
    rangeinfo.diph.dwSize = sizeof(DIPROPRANGE);
    rangeinfo.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    rangeinfo.diph.dwHow = DIPH_BYID;
    rangeinfo.diph.dwObj = lpddoi->dwType;

    if (lpddoi->guidType == GUID_XAxis) {
        rangeinfo.lMin = -100;
        rangeinfo.lMax = 100;
    } else if (lpddoi->guidType == GUID_YAxis) {
        rangeinfo.lMin = -100;
        rangeinfo.lMax = 100;
    } else if (lpddoi->guidType == GUID_ZAxis) {    // ???
        rangeinfo.lMin = -100;
        rangeinfo.lMax = 100;
    } else if (lpddoi->guidType == GUID_RzAxis) {   // Rudder
        rangeinfo.lMin = -100;
        rangeinfo.lMax = 100;
    } else if (lpddoi->guidType == GUID_Slider) {   // Throttle
        rangeinfo.lMin = -1200;
		rangeinfo.lMax = 300;
//		rangeinfo.lMax = 0;
	}

    // Set the range.
    g_pJoystick->SetProperty(DIPROP_RANGE, &rangeinfo.diph);

    return DIENUM_CONTINUE;
}

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
    ON_WM_TIMER()
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

    if (g_pJoystick) {
        // Set up the joystick device.
        DIDEVCAPS caps;
        caps.dwSize = sizeof(DIDEVCAPS);
        g_pJoystick->SetCooperativeLevel(m_hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND);
        g_pJoystick->GetCapabilities(&caps);    // Get number of axes, buttons, etc.
        g_pJoystick->EnumObjects(EnumAxesCallback, NULL, DIDFT_AXIS);
        g_pJoystick->Acquire();
    }

    // Set the timer.
    SetTimer(0, 20, NULL);

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

void CFlyDlg::OnTimer(UINT nIDEvent)
{
    // Flags to test for button switch bounce.  For some reason, we get a bounce
    // with the retail version of the DirectX DLLs.
    static int b0flag = 0;
    static int b1flag = 0;
    static int b2flag = 0;
    static int b3flag = 0;
    static int b4flag = 0;

    // Flag indicating we need to trap the first throttle slider message.  We do
    // this so the hardware position doesn't override the current speed value
    // whenever we launch the Fly dialog.
    static long do_throttle = 0;
    static long prev_throttle = 0;

    // Since buffered DirectInput messages don't tell us if the user holds the
    // POV hat in one position, we have to keep track of this ourselves.  At
    // the end of the while() loop, we will add the delta to the position.
    static int holdcount = -20;
    static int delta = 0;
    static int max = 1200;
    static int min = -300;

    static DIJOYSTATE2 prev_data = {0};
    DIJOYSTATE2 data = {0};

    // Get the selected entity.
    CEntity *entity = g_DataManager.GetSelectedEntity();

    if (g_pJoystick) {
        // Poll the joystick to get the current state.
        if (FAILED(g_pJoystick->Poll())) {
            // Attempt to reacquire the joystick.
            if (SUCCEEDED(g_pJoystick->Acquire()))
                g_pJoystick->Poll();
        }

        g_pJoystick->GetDeviceState(sizeof(DIJOYSTATE2), &data);

        // X and Y axes
        if (data.lX != prev_data.lX) {
            m_JoyWindow.SetPosX(data.lX);
            prev_data.lX = data.lX;
        }

        if (data.lY != prev_data.lY) {
            m_JoyWindow.SetPosY(-data.lY);
            prev_data.lY = data.lY;
        }

        // Rudder
        if (data.lRz != prev_data.lRz) {
            if (entity) {
                // Yaw for all modes except Fly.
                if ((entity->GetFlyMode() == ENTITY_FLYMODE_FLY)
                    && (entity->GetClass() != ENTITY_CLASS_ROTORCRAFT)) {
                    m_dYaw = 0.0f;
                } else {
                    // Rate will be scaled to precision in OnChangeJoyPos().
                    m_dYaw = (long)(data.lRz) * 0.5f;
                    m_bSetRates = TRUE;
                }
            }

            prev_data.lRz = data.lRz;
        }

        // POV Hat
        if (data.rgdwPOV[0] != prev_data.rgdwPOV[0]) {
            if (data.rgdwPOV[0] == 0) {         // Forward (0 degrees)
                holdcount = 0;
                delta = 1;
            } else if (data.rgdwPOV[0] == 18000) { // Back (180 degrees)
                holdcount = 0;
                delta = -1;
            } else if (data.rgdwPOV[0] == -1) { // Center
                holdcount = -1;
                delta = 0;
            }

            prev_data.rgdwPOV[0] = data.rgdwPOV[0];
        }

        // Buttons 0 - 5
        if (data.rgbButtons[0] != prev_data.rgbButtons[0]) {
            if (data.rgbButtons[0] & 0x80) { // If button is down
                if (!b0flag) {
                    OnButtonFire();
                    b0flag = 1;
                }
            } else
                b0flag = 0;

            prev_data.rgbButtons[0] = data.rgbButtons[0];
        }

        if (data.rgbButtons[1] != prev_data.rgbButtons[1]) {
            if (data.rgbButtons[1] & 0x80) { // If button is down
                if (!b1flag) {
                    if (m_TargetCount > 1) {
                        // If we call m_ComboTarget.GetCount() in the above if(),
                        // the % operator in this line causes the joystick to flutter
                        // in Release mode with the DirectX 8.1 retail drivers,
                        // even if the if() fails.  It's probably because of some
                        // compiler optimizations.  It doesn't do it if we use the
                        // m_TargetCount member variable, though.
                        m_ComboTarget.SetCurSel((m_ComboTarget.GetCurSel() + 1) % m_TargetCount);
                        OnSelchangeTarget();
                    }

                    b1flag = 1;
                }

            } else
                b1flag = 0;

            prev_data.rgbButtons[1] = data.rgbButtons[1];
        }

        if (data.rgbButtons[2] != prev_data.rgbButtons[2]) {
            if (data.rgbButtons[2] & 0x80) { // If button is down
                if (!b3flag) {
                    OnButtonFreezeResume();
                    b3flag = 1;
                }
            } else
                b3flag = 0;

            prev_data.rgbButtons[2] = data.rgbButtons[2];
        }

        if (data.rgbButtons[3] != prev_data.rgbButtons[3]) {
            if (data.rgbButtons[3] & 0x80) { // If button is down
                if (!b2flag) {
                    int newmode = (m_FlyModeCombo.GetCurSel() + 1) % 4;
                    m_FlyModeCombo.SetCurSel(newmode);

                    if (entity)
                        entity->SetFlyMode(newmode);

                    b2flag = 1;

                    // Update the Fly Mode indicator in the Entity State window.
                    theApp.GetMainFrame().GetEntityStateView().RefreshFlyMode(newmode);
                }
            } else
                b2flag = 0;

            prev_data.rgbButtons[3] = data.rgbButtons[3];
        }

        if (data.rgbButtons[4] != prev_data.rgbButtons[4]) {
            if (data.rgbButtons[4] & 0x80) { // If button is down
                if (!b4flag) {
                    if (m_CheckPrecision10.GetCheck()) {
                        m_CheckPrecision10.SetCheck(FALSE);
                        m_CheckPrecision50.SetCheck(TRUE);
                    } else if (m_CheckPrecision50.GetCheck()) {
                        m_CheckPrecision50.SetCheck(FALSE);
                        m_CheckPrecision100.SetCheck(TRUE);
                    } else if (m_CheckPrecision100.GetCheck()) {
                        m_CheckPrecision100.SetCheck(FALSE);
                    } else {
                        m_CheckPrecision10.SetCheck(TRUE);
                    }

                    b4flag = 1;
                }
            } else
                b4flag = 0;

            prev_data.rgbButtons[4] = data.rgbButtons[4];
        }
    }


    if (entity) {
        switch (entity->GetFlyMode()) {
        case ENTITY_FLYMODE_FLY: {
            switch (entity->GetClass()) {
            case ENTITY_CLASS_ROTORCRAFT:
                if (delta && ((holdcount == 0) || (holdcount > 10))) {
                    int d_coll = delta;
                    if (holdcount > 10)
                        d_coll *= 5;

                    int pos = 100 - m_SpeedSlider.GetPos();

                    // Add the delta to the current value.
                    if ((pos + d_coll <= 100) && (pos + d_coll >= 0))
                        pos += d_coll;

                    entity->SetCollective(pos);
                }

                break;

            default:
                // Update the entity.state.
                if (delta && ((holdcount == 0) || (holdcount > 10))) {
                    int pos = (int)(entity->GetSpeed());

                    // Add the delta to the current value.
                    if ((pos + delta <= max) && (pos + delta >= min))
                        pos += delta;

                    entity->SetSpeed((float)pos);
                }
            }

            if (holdcount >= 0) {
                if (holdcount == 100) {
                    if (delta > 0)
                        delta += 1;
                    else
                        delta -= 1;
                } else if (holdcount == 300) {
                    if (delta > 0)
                        delta += 1;
                    else
                        delta -= 1;
                }

                holdcount++;
            }

            break;
        }

        case ENTITY_FLYMODE_MAGIC_CARPET: {
            double translatefactor;

            if (m_CheckPrecision10.GetCheck())
                translatefactor = 1.0f;
            else if (m_CheckPrecision10.GetCheck())
                translatefactor = 0.5f;
            else if (m_CheckPrecision10.GetCheck())
                translatefactor = 0.1f;
            else
                translatefactor = 10.0f;

            m_dZ = (float)(translatefactor * delta * 10);   //LWD: 100
            m_bSetRates = TRUE;
        }

        default:
            break;
        }
    }

    if (m_bSetRates) {
CString str;
str.Format( "%6.2f %6.2f %6.2f %6.2f %6.2f %6.2f\r", m_dX, m_dY, m_dZ, m_dYaw, m_dPitch, m_dRoll );
OutputDebugString( str );
        RATES old_rates = entity->GetAndLockRates();
        RATES rates;
        rates.dx = m_dX;
        rates.dy = m_dY;
        rates.dz = m_dZ;
        rates.dyaw = m_dYaw;
        rates.dpitch = m_dPitch;
        rates.droll = m_dRoll;

        if (memcmp(&rates, &old_rates, sizeof(RATES)) != 0)
            entity->SetLockedRates(rates);

        entity->Unlock();

        m_bSetRates = FALSE;
    }


    CDialog::OnTimer(nIDEvent);
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
}

void CFlyDlg::OnCheckPrecision50()
{
    m_CheckPrecision10.SetCheck(FALSE);
    m_CheckPrecision100.SetCheck(FALSE);
}

void CFlyDlg::OnCheckPrecision100()
{
    m_CheckPrecision10.SetCheck(FALSE);
    m_CheckPrecision50.SetCheck(FALSE);
}
