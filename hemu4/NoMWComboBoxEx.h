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

#if !defined(AFX_NOMWCOMBOBOXEX_H__6058FA9C_E4A7_4085_9408_4F4F1F883001__INCLUDED_)
#define AFX_NOMWCOMBOBOXEX_H__6058FA9C_E4A7_4085_9408_4F4F1F883001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NoMWComboBoxEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNoMWComboBoxEx window

class CNoMWComboBoxEx : public CComboBoxEx
{
    // Construction
public:
    CNoMWComboBoxEx();
    virtual ~CNoMWComboBoxEx();

    // Generated message map functions
protected:
    //{{AFX_MSG(CNoMWComboBoxEx)
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOMWCOMBOBOXEX_H__6058FA9C_E4A7_4085_9408_4F4F1F883001__INCLUDED_)
