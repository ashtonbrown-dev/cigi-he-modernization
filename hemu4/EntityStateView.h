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

#if !defined(AFX_ENTITYSTATEVIEW_H__5EF92C2C_4A8A_4D58_88EC_EDD56D803559__INCLUDED_)
#define AFX_ENTITYSTATEVIEW_H__5EF92C2C_4A8A_4D58_88EC_EDD56D803559__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityStateView.h : header file
//

#include "BaseCompPropPage.h"
#include "BaseStateView.h"
#include "EntityArtPartPropPage.h"
#include "EntityCollDetPropPage.h"
#include "EntityHATPropPage.h"
#include "EntityLOSPropPage.h"
#include "EntityMissilePropPage.h"
#include "EntityWeatherPropPage.h"
#include "EditKbd.h"

#define NUM_ENTITYTAB_PAGES 7

class CEntity;

/////////////////////////////////////////////////////////////////////////////
// CEntityStateView dialog

class CEntityStateView : public CBaseStateView
{
    // Construction
public:
    CEntityStateView(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    virtual void RefreshView(void);
    void ClearParentList(void);
    void AddToParentList(CEntity *entity);
    void RemoveFromParentList(const int id);
    void ClearRefEntityList(void);
    void PopulateRefEntityList(void);
    void PopulateCompPropPage(CEntity *entity);
    void PopulateArtPartPropPage(CEntity *entity);
    void PopulateHATPropPage(CEntity *entity);
    void PopulateLOSPropPage(CEntity *entity);
    void PopulateCollDetPropPage(CEntity *entity);
    void PopulateMissilePropPage(CEntity *entity);
    void PopulateWeatherPropPage(CEntity *entity);
    void RefreshCompPage(void);
    void RefreshArtPartPage(void);
    void RefreshFlyMode(int flymode);
    int FindEntityComboItem(CComboBoxEx &combo, const int entity_id);
    int FindEntityComboInsertPoint(CComboBoxEx &combo, const int entity_id);
    void SelectTab(const int index);

    CBaseCompPropPage &GetCompPropPage(void);
    CEntityArtPartPropPage &GetArtPartPropPage(void);
    CEntityHATPropPage &GetHATPropPage(void);
    CEntityLOSPropPage &GetLOSPropPage(void);
    CEntityCollDetPropPage &GetCollDetPropPage(void);
    CEntityMissilePropPage &GetMissilePropPage(void);
    CEntityWeatherPropPage &GetWeatherPropPage(void);

    // Dialog Data
    //{{AFX_DATA(CEntityStateView)
    enum { IDD = IDD_ENTITY_STATE_VIEW };
    CButton m_RadioRatesParent;
    CButton m_RadioRatesLocal;
    CButton m_RadioClampNone;
    CButton m_RadioClampConformal;
    CButton m_RadioClampNonconformal;
    CButton m_CheckSendRates;
    CButton m_CheckInheritAlpha;
    CButton m_CheckHideGeometry;
    CButton m_CheckFlyWaypoints;
    CButton m_CheckAnimRev;
    CButton m_CheckAnimLoop;
    CEditKbd    m_EditYaw;
    CEditKbd    m_EditType;
    CEditKbd    m_EditSpeed;
    CEditKbd    m_EditRoll;
    CEditKbd    m_EditRange;
    CEditKbd    m_EditPitch;
    CEditKbd    m_EditLongitude;
    CEditKbd    m_EditLatitude;
    CEditKbd    m_EditID;
    CEditKbd    m_EditBearing;
    CEditKbd    m_EditAltitude;
    CEditKbd    m_EditAlpha;
    CTabCtrl    m_Tab;
    CComboBoxEx m_cboRefEntity;
    CComboBoxEx m_cboParent;
    BOOL    m_bAnimLoop;
    BOOL    m_bAnimRev;
    BOOL    m_bFlyWaypoints;
    BOOL    m_bHideGeometry;
    BOOL    m_bInheritAlpha;
    BOOL    m_bSendRates;
    BYTE    m_Alpha;
    double  m_Altitude;
    double  m_Bearing;
    UINT    m_EntityID;
    double  m_Latitude;
    double  m_Longitude;
    double  m_Pitch;
    double  m_Range;
    double  m_Roll;
    double  m_Speed;
    UINT    m_EntityType;
    double  m_Yaw;
    CString m_strAltitudeUnit;
    CString m_strAltitudeLabel;
    CString m_strLatitudeUnit;
    CString m_strLatitudeLabel;
    CString m_strLongitudeUnit;
    CString m_strLongitudeLabel;
    CString m_strSpeedUnit;
    CString m_strSpeedLabel;
    int     m_GroundClamp;
    CString m_strFlyMode;
    int     m_RatesCoordSystem;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEntityStateView)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    static const char m_StrLookup[4][13];
    int m_PrevEntityID;
    int m_ParentID;
    int m_RefEntityID;
    BOOL m_bHoldAnimLoop;
    BOOL m_bHoldAnimRev;
    BOOL m_bHoldFlyWaypoints;
    BOOL m_bHoldHideGeometry;
    BOOL m_bHoldInheritAlpha;
    BOOL m_bHoldRatesCoordSystem;
    BOOL m_bHoldSendRates;
    BOOL m_bHoldClampMode;
    CEntityArtPartPropPage  m_DlgArtParts;
    CEntityCollDetPropPage  m_DlgCollDet;
    CBaseCompPropPage       m_DlgComponents;
    CEntityHATPropPage      m_DlgHAT;
    CEntityLOSPropPage      m_DlgLOS;
    CEntityMissilePropPage  m_DlgMissile;
    CEntityWeatherPropPage  m_DlgWeather;
    CDialog *m_TabDlg[NUM_ENTITYTAB_PAGES];
    DLGTEMPLATE *m_DlgTempl[NUM_ENTITYTAB_PAGES];

    void InitTabs(void);
    afx_msg LRESULT OnUserEditControl(WPARAM wParam, LPARAM lParam);

    void OnUserChangeSpeed();
    void OnUserChangeAlpha();
    void OnUserChangeAltitude();
    void OnUserChangeBearing();
    void OnUserChangeLatitude();
    void OnUserChangeLongitude();
    void OnUserChangePitch();
    void OnUserChangeRange();
    void OnUserChangeRoll();
    void OnUserChangeYaw();

    // Generated message map functions
    //{{AFX_MSG(CEntityStateView)
    afx_msg void OnCheckAnimationLoop();
    afx_msg void OnCheckAnimationReverse();
    afx_msg void OnCheckFlyWaypoints();
    afx_msg void OnCheckHideGeometry();
    afx_msg void OnCheckInheritAlpha();
    afx_msg void OnCheckSendRates();
    afx_msg void OnRadioClampNone();
    afx_msg void OnRadioClampConformal();
    afx_msg void OnRadioClampNonconformal();
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeParent();
    afx_msg void OnSelchangeRelativeTo();
    afx_msg void OnDropdownRelativeTo();
    afx_msg void OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnRadioRatesLocal();
    afx_msg void OnRadioRatesParent();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline CBaseCompPropPage &CEntityStateView::GetCompPropPage(void)
{
    return m_DlgComponents;
}

inline CEntityArtPartPropPage &CEntityStateView::GetArtPartPropPage(void)
{
    return m_DlgArtParts;
}

inline CEntityHATPropPage &CEntityStateView::GetHATPropPage(void)
{
    return m_DlgHAT;
}

inline CEntityLOSPropPage &CEntityStateView::GetLOSPropPage(void)
{
    return m_DlgLOS;
}

inline CEntityCollDetPropPage &CEntityStateView::GetCollDetPropPage(void)
{
    return m_DlgCollDet;
}

inline CEntityMissilePropPage &CEntityStateView::GetMissilePropPage(void)
{
    return m_DlgMissile;
}

inline void CEntityStateView::RefreshCompPage(void)
{
    m_DlgComponents.RefreshView();
}

inline void CEntityStateView::RefreshArtPartPage(void)
{
    m_DlgArtParts.RefreshView();
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYSTATEVIEW_H__5EF92C2C_4A8A_4D58_88EC_EDD56D803559__INCLUDED_)
