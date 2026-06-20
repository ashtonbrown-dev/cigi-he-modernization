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

#if !defined(AFX_SKYSTATEVIEW_H__955CA46F_77A6_4EA7_BE63_8424D77D6B3C__INCLUDED_)
#define AFX_SKYSTATEVIEW_H__955CA46F_77A6_4EA7_BE63_8424D77D6B3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkyStateView.h : header file
//

#include "BaseStateView.h"
#include "BaseCompPropPage.h"
#include "EditKbd.h"

#define NUM_SKYTAB_PAGES    2

class CGlobalEnv;

/////////////////////////////////////////////////////////////////////////////
// CSkyStateView dialog

class CSkyStateView : public CBaseStateView
{
    // Construction
public:
    CSkyStateView(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    virtual void RefreshView(void);
    void SelectTab(const int index);
    void PopulateCompPropPages(CGlobalEnv *env);
    void RefreshCompPages(void);

    // Dialog Data
    //{{AFX_DATA(CSkyStateView)
    enum { IDD = IDD_SKY_STATE_VIEW };
    CDateTimeCtrl   m_TimeCtrl;
    CDateTimeCtrl   m_DateCtrl;
    CEditKbd    m_EditStarfieldIntensity;
    CEditKbd    m_EditWindVertical;
    CEditKbd    m_EditWindHorizontal;
    CEditKbd    m_EditWindDirection;
    CEditKbd    m_EditVisibility;
    CEditKbd    m_EditTemperature;
    CEditKbd    m_EditHumidity;
    CEditKbd    m_EditBaroPressure;
    CButton m_CheckSun;
    CButton m_CheckStarfield;
    CButton m_CheckMoon;
    CButton m_CheckEphemeris;
    CButton m_CheckAtmospherics;
    CTabCtrl    m_Tab;
    BOOL    m_EnableAtmospherics;
    BOOL    m_EnableEphemeris;
    BOOL    m_EnableMoon;
    BOOL    m_EnableStarfield;
    BOOL    m_EnableSun;
    CTime   m_Date;
    CTime   m_Time;
    float   m_GlobalBaroPressure;
    UINT    m_GlobalHumidity;
    float   m_GlobalTemperature;
    float   m_GlobalVisibility;
    float   m_GlobalWindDirection;
    float   m_GlobalWindHorizontal;
    float   m_GlobalWindVertical;
    float   m_StarfieldIntensity;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSkyStateView)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CBaseCompPropPage m_DlgCelSphereComponents;
    CBaseCompPropPage m_DlgAtmosphericComponents;
    CDialog *m_TabDlg[NUM_SKYTAB_PAGES];
    DLGTEMPLATE *m_DlgTempl[NUM_SKYTAB_PAGES];

    void InitTabs(void);

    afx_msg LRESULT OnUserEditControl(WPARAM wParam, LPARAM lParam);

    void OnUserChangeStarfieldIntensity();
    void OnUserChangeWindVertical();
    void OnUserChangeWindHorizontal();
    void OnUserChangeWindDirection();
    void OnUserChangeVisibility();
    void OnUserChangeTemperature();
    void OnUserChangeHumidity();
    void OnUserChangeBaroPressure();

    // Generated message map functions
    //{{AFX_MSG(CSkyStateView)
    virtual BOOL OnInitDialog();
    afx_msg void OnCheckEnableAtmospherics();
    afx_msg void OnCheckEphemeris();
    afx_msg void OnCheckMoon();
    afx_msg void OnCheckStarfield();
    afx_msg void OnCheckSun();
    afx_msg void OnDatetimechangeDate(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDatetimechangeTime(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline void CSkyStateView::RefreshCompPages(void)
{
    m_DlgCelSphereComponents.RefreshView();
    m_DlgAtmosphericComponents.RefreshView();
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKYSTATEVIEW_H__955CA46F_77A6_4EA7_BE63_8424D77D6B3C__INCLUDED_)
