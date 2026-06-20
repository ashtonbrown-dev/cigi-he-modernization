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

#if !defined(AFX_WEATHERSTATEVIEW_H__245929C3_E53C_48FE_BC1E_9E6878BE5040__INCLUDED_)
#define AFX_WEATHERSTATEVIEW_H__245929C3_E53C_48FE_BC1E_9E6878BE5040__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WeatherStateView.h : header file
//

#include "BaseStateView.h"
#include "BaseCompPropPage.h"
#include "EditKbd.h"

#define NUM_WEATHERTAB_PAGES    1

class CWeather;

/////////////////////////////////////////////////////////////////////////////
// CWeatherStateView dialog

class CWeatherStateView : public CBaseStateView
{
    // Construction
public:
    CWeatherStateView(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    virtual void RefreshView(void);
    void SelectTab(const int index);
    void PopulateCompPropPage(CWeather *weather);
    void RefreshCompPage(void);

    // Dialog Data
    //{{AFX_DATA(CWeatherStateView)
    enum { IDD = IDD_WEATHER_STATE_VIEW };
    CButton m_CheckEnableWinds;
    CButton m_CheckEnableWeather;
    CButton m_CheckEnableTopScud;
    CButton m_CheckEnableBottomScud;
    CButton m_CheckEnableLightning;
    CComboBoxEx m_ComboCloudType;
    CEditKbd    m_EditWindDirection;
    CEditKbd    m_EditVisibility;
    CEditKbd    m_EditTopTransitionBand;
    CEditKbd    m_EditBottomTransitionBand;
    CEditKbd    m_EditThickness;
    CEditKbd    m_EditAirTemp;
    CEditKbd    m_EditSeverity;
    CEditKbd    m_EditTopScudFreq;
    CEditKbd    m_EditBottomScudFreq;
    CEditKbd    m_EditHumidity;
    CEditKbd    m_EditVertWindspeed;
    CEditKbd    m_EditHorzWindspeed;
    CEditKbd    m_EditCoverage;
    CEditKbd    m_EditBaseElevation;
    CEditKbd    m_EditBaroPressure;
    CEditKbd    m_EditAerosol;
    CTabCtrl    m_Tab;
    CSpinButtonCtrl m_SpinSeverity;
    BOOL    m_EnableLightning;
    BOOL    m_EnableTopScud;
    BOOL    m_EnableBottomScud;
    BOOL    m_EnableWeather;
    BOOL    m_EnableWinds;
    float   m_Aerosol;
    float   m_BaroPressure;
    float   m_BaseElevation;
    float   m_Coverage;
    float   m_HorzWindspeed;
    float   m_VertWindspeed;
    int     m_Humidity;
    int     m_LayerID;
    float   m_TopScudFreq;
    float   m_BottomScudFreq;
    int     m_Severity;
    float   m_AirTemp;
    float   m_Thickness;
    float   m_TopTransitionBand;
    float   m_BottomTransitionBand;
    float   m_Visibility;
    float   m_WindDirection;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWeatherStateView)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    //}}AFX_VIRTUAL

    // Implementation
protected:
    BOOL m_bHoldEnableWeather;
    BOOL m_bHoldEnableWinds;
    BOOL m_bHoldEnableLightning;
    BOOL m_bHoldEnableTopScud;
    BOOL m_bHoldEnableBottomScud;
    CDialog *m_TabDlg[NUM_WEATHERTAB_PAGES];
    DLGTEMPLATE *m_DlgTempl[NUM_WEATHERTAB_PAGES];
    CBaseCompPropPage        m_DlgComponents;
    CWeather *m_PrevWeather;

    void InitTabs(void);
    afx_msg LRESULT OnUserEditControl(WPARAM wParam, LPARAM lParam);

    void OnUserChangeWindDirection();
    void OnUserChangeVisibility();
    void OnUserChangeTopTransitionBand();
    void OnUserChangeBottomTransitionBand();
    void OnUserChangeThickness();
    void OnUserChangeAirTemp();
    void OnUserChangeSeverity();
    void OnUserChangeTopScudFreq();
    void OnUserChangeBottomScudFreq();
    void OnUserChangeHumidity();
    void OnUserChangeRange();
    void OnUserChangeVertWindspeed();
    void OnUserChangeHorzWindspeed();
    void OnUserChangeCoverage();
    void OnUserChangeBaseElevation();
    void OnUserChangeBaroPressure();
    void OnUserChangeAerosol();

    // Generated message map functions
    //{{AFX_MSG(CWeatherStateView)
    afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnCheckEnableLightning();
    afx_msg void OnCheckEnableTopScud();
    afx_msg void OnCheckEnableBottomScud();
    afx_msg void OnCheckEnableWeather();
    afx_msg void OnCheckEnableWinds();
    afx_msg void OnSelchangeComboboxexCloudType();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void SendPacketToDriver(CWeather *weather);
};

inline void CWeatherStateView::RefreshCompPage(void)
{
    m_DlgComponents.RefreshView();
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEATHERSTATEVIEW_H__245929C3_E53C_48FE_BC1E_9E6878BE5040__INCLUDED_)
