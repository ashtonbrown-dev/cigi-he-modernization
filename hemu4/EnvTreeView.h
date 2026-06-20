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

#if !defined(AFX_ENVIRONMENTTREEVIEW_H__64F0475D_BA94_4AD6_AF80_D7AB39DD8781__INCLUDED_)
#define AFX_ENVIRONMENTTREEVIEW_H__64F0475D_BA94_4AD6_AF80_D7AB39DD8781__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnvTreeView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnvTreeView dialog

class CEnvTreeView : public CDialog
{
    // Construction
public:
    CEnvTreeView(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    CTreeCtrl &GetTreeCtrl();

    // Dialog Data
    //{{AFX_DATA(CEnvTreeView)
    enum { IDD = IDD_BLANK_TREE_VIEW };
    CTreeCtrl   m_TreeCtrl;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEnvTreeView)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CEnvTreeView)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline CTreeCtrl &CEnvTreeView::GetTreeCtrl()
{
    return m_TreeCtrl;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENVIRONMENTTREEVIEW_H__64F0475D_BA94_4AD6_AF80_D7AB39DD8781__INCLUDED_)
