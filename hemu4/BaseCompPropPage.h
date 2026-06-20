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

#if !defined(AFX_BASECOMPPROPPAGE_H__7BE90B53_7B7D_4A13_990D_524D10F58B9B__INCLUDED_)
#define AFX_BASECOMPPROPPAGE_H__7BE90B53_7B7D_4A13_990D_524D10F58B9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseCompPropPage.h : header file
//

#include "Resource.h"
#include "component.h"
#include "ComboBoxVariant.h"
#include "RadioList.h"

/////////////////////////////////////////////////////////////////////////////
// CBaseCompPropPage dialog

class CBaseCompPropPage : public CDialog
{
    // Construction
public:
    CBaseCompPropPage(CWnd *pParent = NULL);   // standard constructor
    void OnOK();
    void OnCancel();
    virtual void RefreshView(void);
    virtual BOOL AddComponent(CComponent *component);
    virtual BOOL RemoveComponent(CComponent *component);
    CComponent *FindComponent(int compId, int instId);
    CComponent *GetSelComponent(void);
    virtual void ClearComponents(void);

    // Dialog Data
    //{{AFX_DATA(CBaseCompPropPage)
    enum { IDD = IDD_PROPPAGE_ENTITY_COMPONENTS };
    CButton m_CheckAutoApply;
    CRadioList  m_rlStates;
    CListBox    m_lbComponents;
    CComboBoxVariant    m_cbvarData6;
    CComboBoxVariant    m_cbvarData5;
    CComboBoxVariant    m_cbvarData4;
    CComboBoxVariant    m_cbvarData3;
    CComboBoxVariant    m_cbvarData2;
    CComboBoxVariant    m_cbvarData1;
    CButton m_ButtonUndo;
    CButton m_ButtonApply;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CBaseCompPropPage)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CLinkedList<CComponent *>   m_ComponentList;

    void SendPacketToDriver(CComponent *comp);
    void PopulateStateList(CComponent *comp);
    void PopulateDataFields(CComponent *comp);
    void SetDataField(CComboBoxVariant &cbo, const int datatype, void *data);
    void EnableFields(BOOL enable = TRUE);

    afx_msg LRESULT OnUserEditData(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUserSelDatatype(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSelchangeState(WPARAM wParam, LPARAM lParam);

    // Generated message map functions
    //{{AFX_MSG(CBaseCompPropPage)
    afx_msg void OnButtonApply();
    afx_msg void OnSelchangeComponent();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASECOMPPROPPAGE_H__7BE90B53_7B7D_4A13_990D_524D10F58B9B__INCLUDED_)
