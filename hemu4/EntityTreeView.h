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

#if !defined(AFX_ENTITYTREEVIEW_H__D69D76EB_89BC_495B_B2AC_0D107D1797BA__INCLUDED_)
#define AFX_ENTITYTREEVIEW_H__D69D76EB_89BC_495B_B2AC_0D107D1797BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityTreeView.h : header file
//

#include "TreeCtrlPassMsgs.h"
#include "resource.h"

class CEntity;

/////////////////////////////////////////////////////////////////////////////
// CEntityTreeView dialog

class CEntityTreeView : public CDialog
{
    // Construction
public:
    CEntityTreeView(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    CTreeCtrl &GetTreeCtrl();
    void SelectEntityByID(const int id);
    void SelectEntityBySituationCtrlHandle(POSITION hdl);
    HTREEITEM MoveTreeItem(const HTREEITEM item, const HTREEITEM parent);
    HTREEITEM MoveEntityToParent(CEntity *entity, const int parent_id);
    void RepopulateEntityStateParentList(void);

    // Dialog Data
    //{{AFX_DATA(CEntityTreeView)
    enum { IDD = IDD_BLANK_TREE_VIEW };
    CTreeCtrlPassMsgs   m_TreeCtrl;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEntityTreeView)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    BOOL m_IsDragging;

    void MoveTreeChildren(const HTREEITEM src, const HTREEITEM dest);
    void AddItemToEntityStateParentList(const HTREEITEM hItem);
    void AddChildrenToEntityStateParentList(const HTREEITEM hParent);

    // Generated message map functions
    //{{AFX_MSG(CEntityTreeView)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnSelchangingTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline CTreeCtrl &CEntityTreeView::GetTreeCtrl()
{
    return m_TreeCtrl;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYTREEVIEW_H__D69D76EB_89BC_495B_B2AC_0D107D1797BA__INCLUDED_)
