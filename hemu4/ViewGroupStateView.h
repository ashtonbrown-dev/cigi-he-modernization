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

#if !defined(AFX_VIEWGROUPSTATEVIEW_H__9B38C3A8_04C1_4361_9729_6D043467A7DD__INCLUDED_)
#define AFX_VIEWGROUPSTATEVIEW_H__9B38C3A8_04C1_4361_9729_6D043467A7DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewGroupStateView.h : header file
//

#include "BaseCompPropPage.h"
#include "BaseStateView.h"
#include "EditKbd.h"
#include "resource.h"

#define NUM_VIEWGROUPTAB_PAGES  1

class CViewGroup;

/////////////////////////////////////////////////////////////////////////////
// CViewGroupStateView dialog

class CViewGroupStateView : public CBaseStateView
{
    // Construction
public:
    CViewGroupStateView(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    virtual void RefreshView(void);
    void SelectTab(const int index);
    void PopulateCompPropPage(CViewGroup *group);
    void RefreshCompPage(void);
    void ClearEntityList(void);
    void PopulateEntityList(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int entity_id);

    // Dialog Data
    //{{AFX_DATA(CViewGroupStateView)
    enum { IDD = IDD_VIEWGROUP_STATE_VIEW };
    CTabCtrl    m_Tab;
    CEditKbd    m_EditZ;
    CEditKbd    m_EditY;
    CEditKbd    m_EditX;
    CEditKbd    m_EditGroupID;
    CEditKbd    m_EditRoll;
    CEditKbd    m_EditPitch;
    CEditKbd    m_EditYaw;
    CComboBoxEx m_ComboEntity;
    CButton m_ButtonSlew;
    float   m_Yaw;
    float   m_Pitch;
    float   m_Roll;
    float   m_X;
    float   m_Y;
    float   m_Z;
    UINT    m_GroupID;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CViewGroupStateView)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CBaseCompPropPage m_DlgComponents;
    CDialog *m_TabDlg[NUM_VIEWGROUPTAB_PAGES];
    DLGTEMPLATE *m_DlgTempl[NUM_VIEWGROUPTAB_PAGES];
    int m_PrevGroupId;

    void InitTabs(void);
    afx_msg LRESULT OnUserEditControl(WPARAM wParam, LPARAM lParam);

    void OnUserChangeXOffset(void);
    void OnUserChangeYOffset(void);
    void OnUserChangeZOffset(void);
    void OnUserChangeYaw(void);
    void OnUserChangePitch(void);
    void OnUserChangeRoll(void);

    // Generated message map functions
    //{{AFX_MSG(CViewGroupStateView)
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeComboEntity();
    afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline void CViewGroupStateView::RefreshCompPage(void)
{
    m_DlgComponents.RefreshView();
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWGROUPSTATEVIEW_H__9B38C3A8_04C1_4361_9729_6D043467A7DD__INCLUDED_)
