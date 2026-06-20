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

#if !defined(AFX_NAVIGATORVIEW_H__53DADED9_264D_4754_8BA8_084EB0B6E0D3__INCLUDED_)
#define AFX_NAVIGATORVIEW_H__53DADED9_264D_4754_8BA8_084EB0B6E0D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectTreeView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjectTreeView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "SheetTabCtrl.h"
#include "EntityTreeView.h"
#include "EnvTreeView.h"
#include "ViewTreeView.h"

#define NUM_NAVTAB_PAGES    3

class CObjectTreeView : public CFormView
{
public:
    CEntityTreeView &GetEntityTreeView(void);
    CEnvTreeView &GetEnvTreeView(void);
    CViewTreeView &GetViewTreeView(void);
    CTreeCtrl &GetEntityTree(void);
    CTreeCtrl &GetEnvTree(void);
    CTreeCtrl &GetViewTree(void);
    int GetSelectedTabNum(void);
    void SelectTab(const int index);

    //{{AFX_DATA(CObjectTreeView)
    enum { IDD = IDD_OBJECT_TREE_VIEW };
    CSheetTabCtrl   m_Tab;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CObjectTreeView)
public:
    virtual void OnInitialUpdate();
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

protected:
    BOOL m_Initialized;
    CEntityTreeView         m_DlgEntityTree;
    CEnvTreeView            m_DlgEnvTree;
    CViewTreeView           m_DlgViewTree;
    CDialog *m_TabDlg[NUM_NAVTAB_PAGES];
    DLGTEMPLATE *m_DlgTempl[NUM_NAVTAB_PAGES];

    CObjectTreeView();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CObjectTreeView)
    virtual ~CObjectTreeView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext &dc) const;
#endif
    void InitTabs(void);

    // Generated message map functions
    //{{AFX_MSG(CObjectTreeView)
    afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline CEntityTreeView &CObjectTreeView::GetEntityTreeView(void)
{
    return m_DlgEntityTree;
}

inline CEnvTreeView &CObjectTreeView::GetEnvTreeView(void)
{
    return m_DlgEnvTree;
}

inline CViewTreeView &CObjectTreeView::GetViewTreeView(void)
{
    return m_DlgViewTree;
}

inline CTreeCtrl &CObjectTreeView::GetEntityTree(void)
{
    return m_DlgEntityTree.m_TreeCtrl;
}

inline CTreeCtrl &CObjectTreeView::GetEnvTree(void)
{
    return m_DlgEnvTree.m_TreeCtrl;
}

inline CTreeCtrl &CObjectTreeView::GetViewTree(void)
{
    return m_DlgViewTree.m_TreeCtrl;
}

inline int CObjectTreeView::GetSelectedTabNum(void)
{
    return m_Tab.GetCurSel();
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAVIGATORVIEW_H__53DADED9_264D_4754_8BA8_084EB0B6E0D3__INCLUDED_)
