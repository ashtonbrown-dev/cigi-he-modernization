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

#if !defined(AFX_PACKETDLGWEATHERCONTROL_H__430A9522_5066_470F_859D_CA6C95AA0FA7__INCLUDED_)
#define AFX_PACKETDLGWEATHERCONTROL_H__430A9522_5066_470F_859D_CA6C95AA0FA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgWeatherControl.h : header file
//

#include "resource.h"
#include "OldDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgWeatherControl dialog

class CPacketDlgWeatherControl : public OldDialog
{
    // Construction
public:
    CPacketDlgWeatherControl(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);
    void PopulateLayerIDCombo(void);
    void RefreshView(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgWeatherControl)
    enum { IDD = IDD_PACKET_WEATHER_CONTROL };
    CComboBoxEx m_ComboLayerID;
    CComboBoxEx m_ComboRegionEntityID;
    CStatic m_LabelRegionEntityID;
    CSpinButtonCtrl m_SpinSeverity;
    BOOL    m_EnableWeather;
    BOOL    m_EnableBottomScud;
    BOOL    m_EnableWinds;
    BOOL    m_EnableLightning;
    BOOL    m_EnableTopScud;
    int     m_Scope;
    int     m_Severity;
    float   m_AirTemp;
    float   m_BaroPressure;
    int     m_Humidity;
    float   m_Visibility;
    float   m_WindDirection;
    float   m_WindSpeedHorz;
    float   m_WindSpeedVert;
    int     m_CloudType;
    float   m_Aerosol;
    float   m_BaseElev;
    float   m_Coverage;
    float   m_Thickness;
    float   m_TopScudFreq;
    float   m_TopTransitionBand;
    float   m_BottomScudFreq;
    float   m_BottomTransitionBand;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgWeatherControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_EntityID;
    int m_LayerID;

    void ApplyWeather(void);
    int GetEntityIDFromCombo(void);
    int GetLayerIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgWeatherControl)
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnSelchangeEntityID();
    afx_msg void OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangeLayerID();
    afx_msg void OnEndeditLayerID(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnRadioEntity();
    afx_msg void OnRadioGlobal();
    afx_msg void OnRadioRegional();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGWEATHERCONTROL_H__430A9522_5066_470F_859D_CA6C95AA0FA7__INCLUDED_)
