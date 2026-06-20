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

#if !defined(AFX_BASESTATEVIEW_H__366D4D35_97EE_42B7_A952_F6A180DB8C6E__INCLUDED_)
#define AFX_BASESTATEVIEW_H__366D4D35_97EE_42B7_A952_F6A180DB8C6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseStateView.h : header file
//

#define TIMER_ID_SUSPEND    1

#include "OldDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CBaseStateView dialog

class CBaseStateView : public OldDialog
{
public:
    CBaseStateView(UINT nIDTemplate, CWnd *pParentWnd = NULL);
    virtual void OnOK();
    virtual void OnCancel();
    virtual void RefreshView(void) = 0; // Pure virtual

protected:
    UINT m_Timer;

    virtual BOOL OnInitDialog();
    //{{AFX_MSG(CBaseStateView)
    afx_msg void OnTimer(UINT nIDEvent);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASESTATEVIEW_H__366D4D35_97EE_42B7_A952_F6A180DB8C6E__INCLUDED_)
