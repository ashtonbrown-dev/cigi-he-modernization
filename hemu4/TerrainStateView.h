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

#if !defined(AFX_TERRAINSTATEVIEW_H__86FAF1CA_344D_4C54_BEF2_170EC2178903__INCLUDED_)
#define AFX_TERRAINSTATEVIEW_H__86FAF1CA_344D_4C54_BEF2_170EC2178903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TerrainStateView.h : header file
//

#include "BaseCompPropPage.h"
#include "BaseStateView.h"

#define NUM_TERRAINTAB_PAGES 1

class CTerrainDB;

/////////////////////////////////////////////////////////////////////////////
// CTerrainStateView dialog

class CTerrainStateView : public CBaseStateView
{
    // Construction
public:
    CTerrainStateView(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    virtual void RefreshView(void);
    void PopulateCompPropPage(CTerrainDB *terrain);
    void RefreshCompPage(void);
    void ClearDatabaseList(void);
    void PopulateDatabaseList(void);
    int FindComboItem(const int id);
    int FindComboInsertPoint(const int database_id);

    CBaseCompPropPage &GetCompPropPage(void);

    // Dialog Data
    //{{AFX_DATA(CTerrainStateView)
    enum { IDD = IDD_TERRAIN_STATE_VIEW };
    CTabCtrl    m_Tab;
    CComboBox   m_ComboDatabase;
    CString m_strAltitude;
    CString m_strERM;
    CString m_strLatitude;
    CString m_strLongitude;
    CString m_strRadius;
    CString m_strFlattening;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTerrainStateView)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CBaseCompPropPage m_DlgComponents;
    CDialog *m_TabDlg[NUM_TERRAINTAB_PAGES];
    DLGTEMPLATE *m_DlgTempl[NUM_TERRAINTAB_PAGES];

    void InitTabs(void);

    int m_PrevDatabaseID;

    // Generated message map functions
    //{{AFX_MSG(CTerrainStateView)
    afx_msg void OnSelchangeDatabase();
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void SelectTab(const int index);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline CBaseCompPropPage &CTerrainStateView::GetCompPropPage(void)
{
    return m_DlgComponents;
}

inline void CTerrainStateView::RefreshCompPage(void)
{
    m_DlgComponents.RefreshView();
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERRAINSTATEVIEW_H__86FAF1CA_344D_4C54_BEF2_170EC2178903__INCLUDED_)
