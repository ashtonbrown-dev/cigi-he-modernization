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

#if !defined(AFX_LISTCTRLEX_H__A82C50D2_A476_47B7_A82A_C627E2F0BE4C__INCLUDED_)
#define AFX_LISTCTRLEX_H__A82C50D2_A476_47B7_A82A_C627E2F0BE4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketViewListCtrl.h : header file
//

#include "ListCtrlCustDraw.h"

#define COLUMN_DELTA_T_SOF      0
#define COLUMN_RESPONSE_TIME    1
#define COLUMN_IG_TO_HOST       2
#define COLUMN_HOST_TO_IG       3

/////////////////////////////////////////////////////////////////////////////
// CPacketViewListCtrl window

class CPacketViewListCtrl : public CListCtrlCustDraw
{
public:
    CPacketViewListCtrl();
    virtual ~CPacketViewListCtrl();
    BOOL DeleteAllItems(void);
    int AddItem(LPCTSTR str, int column, unsigned long framectr, BOOL severe = FALSE);
    void SetColorFlag(BOOL flag);
    BOOL GetColorFlag(void);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketViewListCtrl)
    //}}AFX_VIRTUAL

protected:
    BOOL m_ColorFlag;
    CBrush  m_HighlightBrush;

    virtual BOOL OnDraw(CDC *pDC, const CRect &rect);
    virtual BOOL IsNotifyItemDraw();
    virtual BOOL IsNotifySubItemDraw(int nItem, UINT nState, LPARAM lParam);
    virtual COLORREF TextColorForSubItem(int nItem, int nSubItem, UINT nState,
                                         LPARAM lParam);
    virtual COLORREF BkColorForSubItem(int nItem, int nSubItem, UINT nState,
                                       LPARAM lParam);
    virtual BOOL IsItemDraw(int nItem, UINT nState, LPARAM lParam);
    virtual BOOL OnItemDraw(CDC *pDC, int nItem, UINT nState, LPARAM lParam);
    virtual BOOL IsSubItemDraw(int nItem, int nSubItem, UINT nState,
                               LPARAM lParam);
    virtual BOOL OnSubItemDraw(CDC *pDC, int nItem, int nSubItem, UINT nState,
                               LPARAM lParam);


    //{{AFX_MSG(CPacketViewListCtrl)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

inline void CPacketViewListCtrl::SetColorFlag(BOOL flag)
{
    m_ColorFlag = flag;
}

inline BOOL CPacketViewListCtrl::GetColorFlag(void)
{
    return m_ColorFlag;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEX_H__A82C50D2_A476_47B7_A82A_C627E2F0BE4C__INCLUDED_)
