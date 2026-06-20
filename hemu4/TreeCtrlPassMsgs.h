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

#if !defined(AFX_TREECTRLPASSMSGS_H__64655ED7_2C7A_4D7B_B079_803786052525__INCLUDED_)
#define AFX_TREECTRLPASSMSGS_H__64655ED7_2C7A_4D7B_B079_803786052525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeCtrlPassMsgs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlPassMsgs window

class CTreeCtrlPassMsgs : public CTreeCtrl
{
    // Construction
public:
    CTreeCtrlPassMsgs();

    // Attributes
public:

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTreeCtrlPassMsgs)
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CTreeCtrlPassMsgs();

    // Generated message map functions
protected:
    //{{AFX_MSG(CTreeCtrlPassMsgs)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREECTRLPASSMSGS_H__64655ED7_2C7A_4D7B_B079_803786052525__INCLUDED_)
