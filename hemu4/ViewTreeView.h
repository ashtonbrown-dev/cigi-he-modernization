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

#if !defined(AFX_VIEWTREEVIEW_H__DEB469B1_12E8_415B_B352_3F48675B50E3__INCLUDED_)
#define AFX_VIEWTREEVIEW_H__DEB469B1_12E8_415B_B352_3F48675B50E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewTreeView.h : header file
//

#include "TreeCtrlPassMsgs.h"

class CCigiView;

/////////////////////////////////////////////////////////////////////////////
// CViewTreeView dialog

class CViewTreeView : public CDialog
{
    // Construction
public:
    CViewTreeView(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    CTreeCtrl &GetTreeCtrl();
    void ClearItems(void);
    void SelectViewByID(const int id);
    void SelectViewGroupByID(const int id);
    HTREEITEM MoveTreeItem(const HTREEITEM item, const HTREEITEM parent);
    void MoveTreeChildren(const HTREEITEM src, const HTREEITEM dest);
    HTREEITEM MoveViewToGroup(CCigiView *view, const int group_id);

    // Dialog Data
    //{{AFX_DATA(CViewTreeView)
    enum { IDD = IDD_BLANK_TREE_VIEW };
    CTreeCtrlPassMsgs   m_TreeCtrl;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CViewTreeView)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    BOOL m_IsDragging;

    // Generated message map functions
    //{{AFX_MSG(CViewTreeView)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnBegindragTree(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline CTreeCtrl &CViewTreeView::GetTreeCtrl()
{
    return m_TreeCtrl;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWTREEVIEW_H__DEB469B1_12E8_415B_B352_3F48675B50E3__INCLUDED_)
