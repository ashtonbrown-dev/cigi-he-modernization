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

#if !defined(AFX_ADDENTITYDLG_H__7D13D88F_6C58_4798_AC06_BE03F940E535__INCLUDED_)
#define AFX_ADDENTITYDLG_H__7D13D88F_6C58_4798_AC06_BE03F940E535__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CAddEntityDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CAddEntityDlg dialog

class CAddEntityDlg : public CDialog
{
    // Construction
public:
    CAddEntityDlg(CWnd *pParent = NULL);   // standard constructor
    int GetID();
    int GetType();

    // Dialog Data
    //{{AFX_DATA(CAddEntityDlg)
    enum { IDD = IDD_DIALOG_NEW_ENTITY };
    CButton m_ButtonOK;
    CListCtrl   m_TypesList;
    UINT    m_ID;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAddEntityDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_Type;

    void PopulateTypeList(void);
    int FindListInsertPoint(const int type);

    // Generated message map functions
    //{{AFX_MSG(CAddEntityDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnItemchangedListTypes(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline int CAddEntityDlg::GetID()
{
    return m_ID;
}

inline int CAddEntityDlg::GetType()
{
    return m_Type;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDENTITYDLG_H__7D13D88F_6C58_4798_AC06_BE03F940E535__INCLUDED_)
