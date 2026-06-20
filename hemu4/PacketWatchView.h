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

#if !defined(AFX_PACKETWATCHVIEW_H__89BB4C1F_1972_477A_BD41_98FA2CE0FEF5__INCLUDED_)
#define AFX_PACKETWATCHVIEW_H__89BB4C1F_1972_477A_BD41_98FA2CE0FEF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketWatchView.h : header file
//

#include "PacketViewListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketWatchView dialog

class CPacketWatchView : public CDialog
{
    // Construction
public:
    CPacketWatchView(CWnd *pParent = NULL);   // standard constructor
    BOOL DeleteAllItems(void);
    int AddItem(LPCTSTR str, int column, unsigned long framectr, BOOL severe = FALSE);

    // Dialog Data
    //{{AFX_DATA(CPacketWatchView)
    enum { IDD = IDD_WATCH_VIEW };
    CPacketViewListCtrl     m_List;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketWatchView)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CPacketWatchView)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline BOOL CPacketWatchView::DeleteAllItems(void)
{
    return m_List.DeleteAllItems();
}

inline int CPacketWatchView::AddItem(LPCTSTR str, int column, unsigned long framectr, BOOL severe)
{
    return m_List.AddItem(str, column, framectr, severe);
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETWATCHVIEW_H__89BB4C1F_1972_477A_BD41_98FA2CE0FEF5__INCLUDED_)
