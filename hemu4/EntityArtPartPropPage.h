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

#if !defined(AFX_ENTITYARTPARTPROPPAGE_H__90072DE6_0CEA_47F6_8101_CD26825AF270__INCLUDED_)
#define AFX_ENTITYARTPARTPROPPAGE_H__90072DE6_0CEA_47F6_8101_CD26825AF270__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityArtPartPropPage.h : header file
//

#include "EditKbd.h"
#include "resource.h"

class CArtPart;

/////////////////////////////////////////////////////////////////////////////
// CEntityArtPartPropPage dialog

class CEntityArtPartPropPage : public CDialog
{
    // Construction
public:
    CEntityArtPartPropPage(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    void RefreshView(void);
    BOOL AddArtPart(CArtPart *artpart);
    BOOL RemoveArtPart(CArtPart *artpart);
    CArtPart *FindArtPart(const int id);
    CArtPart *GetSelArtPart(void);
    void ClearArtParts(void);

    // Dialog Data
    //{{AFX_DATA(CEntityArtPartPropPage)
    enum { IDD = IDD_PROPPAGE_ENTITY_ARTPARTS };
    CSpinButtonCtrl m_SpinZ;
    CSpinButtonCtrl m_SpinY;
    CSpinButtonCtrl m_SpinX;
    CSpinButtonCtrl m_SpinYaw;
    CSpinButtonCtrl m_SpinRoll;
    CSpinButtonCtrl m_SpinPitch;
    CListBox    m_ListBox;
    CEditKbd    m_EditZ;
    CEditKbd    m_EditY;
    CEditKbd    m_EditX;
    CEditKbd    m_EditYaw;
    CEditKbd    m_EditRoll;
    CEditKbd    m_EditPitch;
    CButton m_CheckAutoApply;
    CButton m_CheckZ;
    CButton m_CheckY;
    CButton m_CheckX;
    CButton m_CheckYaw;
    CButton m_CheckRoll;
    CButton m_CheckPitch;
    CButton m_CheckEnable;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEntityArtPartPropPage)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CLinkedList<CArtPart *>     m_ArtPartList;

    void SendPacketToDriver(CArtPart *artpart);
    void EnableFields(BOOL enable);
    void PopulateFields(CArtPart *artpart);

    LRESULT OnUserEditControl(WPARAM wParam, LPARAM lParam);

    // Generated message map functions
    //{{AFX_MSG(CEntityArtPartPropPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnDeltaposPitch(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposRoll(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposYaw(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposX(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposY(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposZ(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnApply();
    afx_msg void OnCheckArtpartEnable();
    afx_msg void OnCheckPitchEnable();
    afx_msg void OnCheckRollEnable();
    afx_msg void OnCheckYawEnable();
    afx_msg void OnCheckXEnable();
    afx_msg void OnCheckYEnable();
    afx_msg void OnCheckZEnable();
    afx_msg void OnSelchangeArtpart();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYARTPARTPROPPAGE_H__90072DE6_0CEA_47F6_8101_CD26825AF270__INCLUDED_)
