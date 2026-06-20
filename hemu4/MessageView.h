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

#if !defined(AFX_MESSAGEVIEW_H__2518EAE8_8B87_4552_BE0A_5D962C114DB3__INCLUDED_)
#define AFX_MESSAGEVIEW_H__2518EAE8_8B87_4552_BE0A_5D962C114DB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageView dialog

class CMessageView : public CDialog
{
    // Construction
public:
    CMessageView(CWnd *pParent = NULL);   // standard constructor
    void ClearMessageText(void);
    void PrintMessage(LPCTSTR str);

    // Dialog Data
    //{{AFX_DATA(CMessageView)
    enum { IDD = IDD_MESSAGE_VIEW };
    CEdit   m_Edit;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMessageView)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CFont m_MessageFont;

    // Generated message map functions
    //{{AFX_MSG(CMessageView)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline void CMessageView::ClearMessageText(void)
{
    m_Edit.SetWindowText("");
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEVIEW_H__2518EAE8_8B87_4552_BE0A_5D962C114DB3__INCLUDED_)
