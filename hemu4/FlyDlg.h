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

#if !defined(AFX_FLYDLG_H__CF321AD2_90C3_11D5_BBC9_00609419F883__INCLUDED_)
#define AFX_FLYDLG_H__CF321AD2_90C3_11D5_BBC9_00609419F883__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlyDlg.h : header file
//

#include "resource.h"
#include "NoMWComboBox.h" // CComboBox that passes WM_MOUSEWHEEL messages to parent.
#include "NoMWComboBoxEx.h" // CComboBox that passes WM_MOUSEWHEEL messages to parent.
#include "JoyWnd.h"
#include "JoystickInput.h"

class CEntity;

/////////////////////////////////////////////////////////////////////////////
// CFlyDlg dialog

class CFlyDlg : public CDialog
{
    // Construction
public:
    DECLARE_DYNCREATE(CFlyDlg)

    CFlyDlg(CWnd *pParent = NULL);
    virtual void OnOK();
    virtual void OnCancel();
    void RefreshView(void);
    void SetFreezeResumeButtonText(void);
    void DoMouseWheelThrottle(UINT nFlags, short zDelta, CPoint pt);
    void UpdateMissileCombo(void);
    void UpdateTargetCombo(void);
    void ConfigureSpeedSlider(void);
    void ProcessHardwareJoystickState(const HEMU_JOYSTICK_STATE &state);
    void DisableHardwareJoystick(void);
    void DisengageHardwareThrottle(const BOOL showWaitingStatus = TRUE);
    void ProcessKeyboardFlightState(
        const int rollDirection,
        const int pitchDirection,
        const int yawDirection);
    void AdjustKeyboardThrottle(
        const int direction, const double elapsedSeconds);
    void DisableKeyboardFlight(void);

    // Dialog Data
    //{{AFX_DATA(CFlyDlg)
    enum { IDD = IDD_DIALOG_FLY };
    CButton m_CheckPrecision50;
    CButton m_CheckPrecision100;
    CButton m_CheckPrecision10;
    CStatic m_LabelKnots;
    CStatic m_LabelSpeed;
    CStatic m_ThrottleLabelMax;
    CStatic m_ThrottleLabelMin;
    CStatic m_ThrottleLabelMiddle;
    CNoMWComboBoxEx m_ComboMissile;
    CNoMWComboBoxEx m_ComboTarget;
    CButton m_ResetButton;
    CJoyWnd m_JoyWindow;
    CStatic m_HTrim;
    CStatic m_VTrim;
    CStatic m_Heading;
    CSliderCtrl m_SpeedSlider;
    CSliderCtrl m_RollSlider;
    CSliderCtrl m_PitchSlider;
    CStatic m_Roll;
    CStatic m_Pitch;
    CStatic m_Longitude;
    CStatic m_Latitude;
    CNoMWComboBox   m_FlyModeCombo;
    CButton m_FreezeResumeButton;
    CButton m_FireButton;
    CStatic m_Altitude;
    CStatic m_Speed;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFlyDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_TargetCount;
    BOOL m_bSetRates;
    float m_dX;
    float m_dY;
    float m_dZ;
    float m_dYaw;
    float m_dPitch;
    float m_dRoll;
    int m_HardwarePrecision;
    HEMU_JOYSTICK_STATE m_PreviousHardwareState;
    BOOL m_HasPreviousHardwareState;
    int m_PovDirection;
    DWORD m_PovHoldStarted;
    DWORD m_PovLastRepeat;
    BOOL m_PovImmediate;
    int m_HardwareEntityId;
    int m_LastHardwarePovDelta;
    BOOL m_HardwareThrottleAvailable;
    BOOL m_HardwareThrottleAcquired;
    BOOL m_HasLastHardwareThrottleValue;
    double m_LastHardwareThrottleValue;
    BOOL m_KeyboardAxesActive;
    int m_KeyboardEntityId;
    BOOL m_KeyboardThrottleActive;
    BOOL m_HasKeyboardThrottleValue;
    int m_KeyboardThrottleEntityId;
    double m_KeyboardThrottleValue;

    void SelectTargetItem(void);
    void ApplyHardwareRates(CEntity *entity,
                            const HEMU_JOYSTICK_STATE &state,
                            const int povDelta);
    void ApplyKeyboardRates(
        CEntity *entity,
        const int rollDirection,
        const int pitchDirection,
        const int yawDirection);
    double GetEntityThrottleValue(CEntity *entity) const;
    double GetHardwareThrottleValue(
        CEntity *entity, const HEMU_JOYSTICK_STATE &state) const;
    double GetHardwareThrottleTolerance(CEntity *entity) const;
    void SetEntityThrottleValue(CEntity *entity, const double value);
    void UpdateHardwareThrottleVisual(void);
    void ShowHardwareThrottleWaitingStatus(CEntity *entity);
    void AcquireHardwareThrottle(CEntity *entity);
    void UpdatePrecisionControls(void);

    afx_msg LRESULT OnChangeJoyPos(WPARAM wParam, LPARAM lParam);

    // Generated message map functions
    //{{AFX_MSG(CFlyDlg)
    afx_msg void OnClose();
    afx_msg void OnButtonFreezeResume();
    virtual BOOL OnInitDialog();
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    afx_msg void OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnButtonReset();
    afx_msg void OnReleasedcaptureSliderRoll(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnReleasedcaptureSliderPitch(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangeComboMode();
    afx_msg void OnButtonFire();
    afx_msg void OnCloseupComboTarget();
    afx_msg void OnCloseupComboMissile();
    afx_msg void OnSelchangeMissile();
    afx_msg void OnSelchangeTarget();
    afx_msg void OnCheckPrecision10();
    afx_msg void OnCheckPrecision50();
    afx_msg void OnCheckPrecision100();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLYDLG_H__CF321AD2_90C3_11D5_BBC9_00609419F883__INCLUDED_)
