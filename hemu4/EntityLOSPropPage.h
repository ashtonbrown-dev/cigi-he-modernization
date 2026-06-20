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

#if !defined(AFX_ENTITYLOSPROPPAGE_H__5DB2DCC4_621C_406F_B7B5_1C9DD6C40C42__INCLUDED_)
#define AFX_ENTITYLOSPROPPAGE_H__5DB2DCC4_621C_406F_B7B5_1C9DD6C40C42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityLOSPropPage.h : header file
//

#include "resource.h"

class CLos;

/////////////////////////////////////////////////////////////////////////////
// CEntityLOSPropPage dialog

class CEntityLOSPropPage : public CDialog
{
    // Construction
public:
    CEntityLOSPropPage(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    BOOL AddLosItem(CLos *los);
    BOOL RemoveLosItem(CLos *los);
    CLos *FindLos(const int id);
    CLos *GetSelLos(void);
    void ClearLos(void);
    void RefreshView(void);

    // Dialog Data
    //{{AFX_DATA(CEntityLOSPropPage)
    enum { IDD = IDD_PROPPAGE_ENTITY_LOS };
    CStatic m_LabelReqEl;
    CStatic m_LabelReqAz;
    CStatic m_LabelRespLonOrYUnits;
    CStatic m_LabelRespLonOrY;
    CStatic m_LabelRespLatOrXUnits;
    CStatic m_LabelRespLatOrX;
    CStatic m_LabelRespAltOrZ;
    CStatic m_LabelDestination;
    CStatic m_LabelDestAltOrZ;
    CStatic m_LabelDestLatOrXOrMinRange;
    CStatic m_LabelDestLonOrYOrMaxRange;
    CButton m_RadioSegment;
    CSpinButtonCtrl m_SpinRespNum;
    CEdit   m_EditSourceZ;
    CEdit   m_EditSourceY;
    CEdit   m_EditSourceX;
    CEdit   m_EditMaterialMask;
    CEdit   m_EditReqEl;
    CEdit   m_EditDestZ;
    CEdit   m_EditDestYorMaxRange;
    CEdit   m_EditDestXorMinRange;
    CEdit   m_EditReqAz;
    CEdit   m_EditAlphaThreshold;
    CComboBox   m_ComboID;
    CButton m_CheckReqGDC;
    CButton m_CheckDestGDC;
    CButton m_CheckExtResponse;
    CButton m_CheckContinuous;
    CButton m_ButtonRemove;
    BOOL    m_RespValid;
    CString m_szRespCount;
    CString m_szRespAlt;
    CString m_szRespLat;
    CString m_szRespLon;
    CString m_szRespEntityID;
    CString m_szRespAz;
    CString m_szRespEl;
    CString m_szRespRange;
    CString m_szRespMaterialCode;
    CString m_szRespColor;
    //}}AFX_DATA

    CButton m_RadioVector;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEntityLOSPropPage)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    void EnableFields(BOOL enable);
    void PopulateReqFields(CLos *los);
    void PopulateRespFields(CLos *los);
    void SendLosRequestToDriver(CLos *los);

    // Generated message map functions
    //{{AFX_MSG(CEntityLOSPropPage)
    afx_msg void OnButtonAddLos();
    afx_msg void OnButtonRemoveLos();
    afx_msg void OnButtonApply();
    afx_msg void OnCheckLosDestGdc();
    afx_msg void OnSelchangeRange();
    afx_msg void OnRadioLosTypeSegment();
    afx_msg void OnRadioLosTypeVector();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYLOSPROPPAGE_H__5DB2DCC4_621C_406F_B7B5_1C9DD6C40C42__INCLUDED_)
