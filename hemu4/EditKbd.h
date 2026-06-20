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

#if !defined(AFX_EDITKBD_H__A7434AEC_E971_48CD_9546_ABEFA60CD3A3__INCLUDED_)
#define AFX_EDITKBD_H__A7434AEC_E971_48CD_9546_ABEFA60CD3A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditKbd.h : header file
//

// Define a message that will get sent to the parent window when the
// user strikes a key.
#define WM_USEREDIT     (WM_APP + 4)
#define WM_USEREXTKEY   (WM_APP + 5)
#define WM_USERMOUSE    (WM_APP + 6)
#define WM_USERFOCUS    (WM_APP + 6)

/////////////////////////////////////////////////////////////////////////////
// CEditKbd window

class CEditKbd : public CEdit
{
    // Construction
public:
    CEditKbd();
    virtual ~CEditKbd();

    // Generated message map functions
protected:
    //{{AFX_MSG(CEditKbd)
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg int OnMouseActivate(CWnd *pDesktopWnd, UINT nHitTest, UINT message);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnSetFocus(CWnd *pOldWnd);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITKBD_H__A7434AEC_E971_48CD_9546_ABEFA60CD3A3__INCLUDED_)
