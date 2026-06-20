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

#if !defined(AFX_NOMWCOMBOBOX_H__5E2D8EC0_20D3_462F_B2F2_E9F10255D6BA__INCLUDED_)
#define AFX_NOMWCOMBOBOX_H__5E2D8EC0_20D3_462F_B2F2_E9F10255D6BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NoMWComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNoMWComboBox window

class CNoMWComboBox : public CComboBox
{
    // Construction
public:
    CNoMWComboBox();
    virtual ~CNoMWComboBox();

    // Generated message map functions
protected:
    //{{AFX_MSG(CNoMWComboBox)
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOMWCOMBOBOX_H__5E2D8EC0_20D3_462F_B2F2_E9F10255D6BA__INCLUDED_)
