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

#if !defined(AFX_VIEWSTATEVIEW_H__664F8394_913E_4587_9675_D46F798C4246__INCLUDED_)
#define AFX_VIEWSTATEVIEW_H__664F8394_913E_4587_9675_D46F798C4246__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewStateView.h : header file
//

#include "BaseCompPropPage.h"
#include "BaseStateView.h"
#include "ViewFrustumPropPage.h"
#include "EditKbd.h"
#include "resource.h"

#define NUM_VIEWTAB_PAGES   2

class CCigiView;

/////////////////////////////////////////////////////////////////////////////
// CViewStateView dialog

class CViewStateView : public CBaseStateView
{
    // Construction
public:
    CViewStateView(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    virtual void RefreshView(void);
    void SelectTab(const int index);
    void PopulateCompPropPage(CCigiView *view);
    void RefreshCompPage(void);
    void ClearEntityList(void);
    void PopulateEntityList(void);
    void PopulateViewGroupList(void);
    void SelectEntityComboItem(const int entity_id);
    void SelectViewGroupComboItem(const int group_id);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int entity_id);

    // Dialog Data
    //{{AFX_DATA(CViewStateView)
    enum { IDD = IDD_VIEW_STATE_VIEW };
    CButton m_RadioPixelRepNone;
    CButton m_RadioPixelRep1x2;
    CButton m_RadioPixelRep2x1;
    CButton m_RadioPixelRep2x2;
    CButton m_RadioMirrorNone;
    CButton m_RadioMirrorHorz;
    CButton m_RadioMirrorVert;
    CButton m_RadioMirrorHorzVert;
    CTabCtrl    m_Tab;
    CEditKbd    m_EditViewType;
    CEditKbd    m_EditZ;
    CEditKbd    m_EditY;
    CEditKbd    m_EditX;
    CEditKbd    m_EditViewID;
    CEditKbd    m_EditRoll;
    CEditKbd    m_EditPitch;
    CEditKbd    m_EditYaw;
    CComboBoxEx m_ComboViewGroup;
    CComboBoxEx m_ComboEntity;
    CButton m_ButtonSlew;
    CButton m_ButtonReorder;
    CButton m_ButtonTrackerEnable;
    CButton m_ButtonTrackerBoresight;
    int     m_MirrorMode;
    int     m_PixelReplication;
    float   m_Yaw;
    float   m_Pitch;
    float   m_Roll;
    float   m_X;
    float   m_Y;
    float   m_Z;
    UINT    m_ViewType;
    UINT    m_ViewID;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CViewStateView)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    BOOL m_bHoldPixelrep;
    BOOL m_bHoldMirror;
    CDialog *m_TabDlg[NUM_VIEWTAB_PAGES];
    DLGTEMPLATE *m_DlgTempl[NUM_VIEWTAB_PAGES];
    CBaseCompPropPage       m_DlgComponents;
    CViewFrustumPropPage    m_DlgFrustum;
    int m_PrevViewId;

    void InitTabs(void);
    afx_msg LRESULT OnUserEditControl(WPARAM wParam, LPARAM lParam);

    void OnUserChangeType(void);
    void OnUserChangeXOffset(void);
    void OnUserChangeYOffset(void);
    void OnUserChangeZOffset(void);
    void OnUserChangeYaw(void);
    void OnUserChangePitch(void);
    void OnUserChangeRoll(void);

    // Generated message map functions
    //{{AFX_MSG(CViewStateView)
    virtual BOOL OnInitDialog();
    afx_msg void OnRadioPixelrepNone();
    afx_msg void OnRadioPixelrep1x2();
    afx_msg void OnRadioPixelrep2x1();
    afx_msg void OnRadioPixelrep2x2();
    afx_msg void OnRadioMirrorNone();
    afx_msg void OnRadioMirrorHorz();
    afx_msg void OnRadioMirrorVert();
    afx_msg void OnRadioMirrorHorzVert();
    afx_msg void OnSelchangeComboEntity();
    afx_msg void OnSelchangeComboViewGroup();
    afx_msg void OnButtonReorderToTop();
    afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline void CViewStateView::RefreshCompPage(void)
{
    m_DlgComponents.RefreshView();
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSTATEVIEW_H__664F8394_913E_4587_9675_D46F798C4246__INCLUDED_)
