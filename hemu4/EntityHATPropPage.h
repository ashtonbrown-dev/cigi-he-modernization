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

#if !defined(AFX_ENTITYHATPROPPAGE_H__EB4E3175_B88F_4152_B0B3_2F9344E9D56F__INCLUDED_)
#define AFX_ENTITYHATPROPPAGE_H__EB4E3175_B88F_4152_B0B3_2F9344E9D56F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityHATPropPage.h : header file
//

#include "resource.h"

class CHat;

/////////////////////////////////////////////////////////////////////////////
// CEntityHATPropPage dialog

class CEntityHATPropPage : public CDialog
{
    // Construction
public:
    CEntityHATPropPage(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    BOOL AddHatItem(CHat *hat);
    BOOL RemoveHatItem(CHat *hat);
    CHat *FindHat(const int id);
    CHat *GetSelHat(void);
    void ClearHats(void);
    void RefreshView(void);

    // Dialog Data
    //{{AFX_DATA(CEntityHATPropPage)
    enum { IDD = IDD_PROPPAGE_ENTITY_HATHOT };
    CEdit   m_EditZ;
    CEdit   m_EditY;
    CEdit   m_EditX;
    CComboBox   m_ComboID;
    CButton m_CheckRequestHot;
    CButton m_CheckRequestHat;
    CButton m_CheckContinuous;
    CButton m_ButtonApply;
    CButton m_ButtonRemoveHat;
    CButton m_ButtonAddHat;
    BOOL    m_HatValid;
    CString m_Azimuth;
    CString m_Elevation;
    CString m_HAT;
    CString m_HOT;
    CString m_MaterialCode;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEntityHATPropPage)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    void EnableFields(BOOL enable);
    void PopulateReqFields(CHat *hat);
    void PopulateRespFields(CHat *hat);
    void SendHatRequestToDriver(CHat *hat);

    // Generated message map functions
    //{{AFX_MSG(CEntityHATPropPage)
    afx_msg void OnButtonAddHat();
    afx_msg void OnButtonRemoveHat();
    afx_msg void OnButtonApplyHat();
    afx_msg void OnSelchangeComboIdRange();
    afx_msg void OnCheckRequestHat();
    afx_msg void OnCheckRequestHot();
    afx_msg void OnCheckContinuous();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYHATPROPPAGE_H__EB4E3175_B88F_4152_B0B3_2F9344E9D56F__INCLUDED_)
