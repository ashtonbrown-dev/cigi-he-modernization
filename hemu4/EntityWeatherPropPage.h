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

#if !defined(AFX_ENTITYWEATHERPROPPAGE_H__318B5F79_65E3_412E_8D1C_E9903ACACD60__INCLUDED_)
#define AFX_ENTITYWEATHERPROPPAGE_H__318B5F79_65E3_412E_8D1C_E9903ACACD60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityWeatherPropPage.h : header file
//

#include "Resource.h"
#include "OldDialog.h"

class CWeather;

/////////////////////////////////////////////////////////////////////////////
// CEntityWeatherPropPage dialog

class CEntityWeatherPropPage : public OldDialog
{
    // Construction
public:
    CEntityWeatherPropPage(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    void RefreshView(CWeather *weather);
    void SendPacketToDriver(CWeather *weather);

    // Dialog Data
    //{{AFX_DATA(CEntityWeatherPropPage)
    enum { IDD = IDD_PROPPAGE_ENTITY_WEATHER };
    CEdit   m_EditVisibility;
    CEdit   m_EditWindDirection;
    CEdit   m_EditTopTransitionBand;
    CEdit   m_EditBottomTransitionBand;
    CEdit   m_EditTemperature;
    CEdit   m_EditSeverity;
    CEdit   m_EditTopScudFreq;
    CEdit   m_EditBottomScudFreq;
    CEdit   m_EditHumidity;
    CEdit   m_EditWindspeedVert;
    CEdit   m_EditWindspeedHorz;
    CEdit   m_EditCoverage;
    CEdit   m_EditBaroPressure;
    CEdit   m_EditAerosol;
    CComboBoxEx m_ComboCloudType;
    CButton m_CheckEnableWinds;
    CButton m_CheckEnableWeather;
    CButton m_CheckEnableTopScud;
    CButton m_CheckEnableBottomScud;
    CButton m_CheckEnableLightning;
    CSpinButtonCtrl m_SpinSeverity;
    BOOL    m_EnableLightning;
    BOOL    m_EnableTopScud;
    BOOL    m_EnableBottomScud;
    BOOL    m_EnableWeather;
    BOOL    m_EnableWinds;
    int     m_CloudType;
    float   m_Aerosol;
    float   m_BaroPressure;
    float   m_Coverage;
    float   m_WindspeedHorz;
    float   m_WindspeedVert;
    int     m_Humidity;
    float   m_TopScudFreq;
    float   m_BottomScudFreq;
    int     m_Severity;
    float   m_Temperature;
    float   m_TopTransitionBand;
    float   m_BottomTransitionBand;
    float   m_WindDirection;
    float   m_Visibility;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEntityWeatherPropPage)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CEntityWeatherPropPage)
    afx_msg void OnButtonApply();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYWEATHERPROPPAGE_H__318B5F79_65E3_412E_8D1C_E9903ACACD60__INCLUDED_)
