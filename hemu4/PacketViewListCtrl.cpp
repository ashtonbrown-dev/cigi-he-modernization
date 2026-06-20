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

// PacketViewListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlCustDraw.h"
#include "PacketViewListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketViewListCtrl

CPacketViewListCtrl::CPacketViewListCtrl()
{
    m_HighlightBrush.CreateSolidBrush(RGB(192, 192, 192));
}

CPacketViewListCtrl::~CPacketViewListCtrl()
{
}

BEGIN_MESSAGE_MAP(CPacketViewListCtrl, CListCtrlCustDraw)
    //{{AFX_MSG_MAP(CPacketViewListCtrl)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketViewListCtrl message handlers

BOOL CPacketViewListCtrl::OnDraw(CDC *pDC, const CRect &rect)
{
    // Erase the ugly vertical lines from resizing the header.
    CBrush brush(RGB(255, 255, 255));
    pDC->FillRect(rect, &brush);
    return FALSE;   // do default drawing as well
}

BOOL CPacketViewListCtrl::IsNotifyItemDraw()
{
    return TRUE;
}

BOOL CPacketViewListCtrl::IsNotifySubItemDraw(int nItem, UINT nState, LPARAM lParam)
{
    return TRUE;
}

COLORREF CPacketViewListCtrl::TextColorForSubItem(int nItem, int nSubItem, UINT nState,
                                                  LPARAM lParam)
{
    // Use the state image bits to decide which color we need.  We only want
    // to make the item in the IG-to-Host column red.
    if (GetItemState(nItem, LVIS_OVERLAYMASK) && (nSubItem == 0))
        return RGB(255, 0, 0);
    else
        return CLR_DEFAULT;
}

COLORREF CPacketViewListCtrl::BkColorForSubItem(int nItem, int nSubItem, UINT nState,
                                                LPARAM lParam)
{
    // Use the state image bits to decide which color we need.
    if (GetItemState(nItem, LVIS_STATEIMAGEMASK))
        return RGB(197, 255, 255);          //RGB(192, 255, 192);
    else
        return CLR_DEFAULT;
}

BOOL CPacketViewListCtrl::IsItemDraw(int nItem, UINT nState, LPARAM lParam)
{
    return (nState == CDIS_SELECTED);
}

BOOL CPacketViewListCtrl::OnItemDraw(CDC *pDC, int nItem, UINT nState, LPARAM lParam)
{
    pDC->SelectObject(&m_HighlightBrush);
    return FALSE;                   // Do default drawing as well.
}

BOOL CPacketViewListCtrl::IsSubItemDraw(int nItem, int nSubItem, UINT nState,
                                        LPARAM lParam)
{
    return (nState == CDIS_SELECTED);
}

BOOL CPacketViewListCtrl::OnSubItemDraw(CDC *pDC, int nItem, int nSubItem, UINT nState,
                                        LPARAM lParam)
{
    pDC->SelectObject(&m_HighlightBrush);
    return FALSE;                   // Do default drawing as well.
}

BOOL CPacketViewListCtrl::DeleteAllItems(void)
{
    return CListCtrl::DeleteAllItems();
}

int CPacketViewListCtrl::AddItem(LPCTSTR str, int column, unsigned long framectr, BOOL severe)
{
    static int i = 0;
    int search_idx, idx, new_idx;

    // We need to search back until we find a row with a matching frame counter
    // (lParam). If we don't find one, we need to insert a new row after the
    // first smaller lParam.
    search_idx = GetItemCount() - 1;
    while ((search_idx >= 0) && (framectr < GetItemData(search_idx)))
        search_idx--;

    // We have found one or more rows with a matching lParam.
    // search backward until we find an empty "slot" in the
    // appropriate column.
    idx = search_idx;
    while ((idx >= 0) && (framectr == GetItemData(idx)) && (GetItemText(idx, column) == ""))
        idx--;
    idx++;      // Increment back to last empty string.

    if (idx - 1 == search_idx) { // we found no empty slots, so insert a row
        // Determine if we need to switch colors.
        int colorflag = 0;
        if (idx) {
            colorflag = GetItemState(idx - 1, LVIS_STATEIMAGEMASK);

            if (framectr != (unsigned long)GetItemData(idx - 1))
                colorflag = (colorflag ? 0 : 1);
        }

        if (column == 0) {
            new_idx = InsertItem(idx, str);
            SetItemData(new_idx, (DWORD)framectr);
        } else {
            new_idx = InsertItem(idx, "");
            SetItemText(new_idx, column, str);
            SetItemData(new_idx, (DWORD)framectr);
        }

        idx = new_idx;

        // Set the state image bits to zero or non-zero to specify which color
        // the row should be.  Set the overlay image bits to specify if the
        // text should be red.
        LVITEM lvitem;
        lvitem.mask = LVIF_STATE;
        lvitem.state = INDEXTOOVERLAYMASK(severe) |
                       INDEXTOSTATEIMAGEMASK(colorflag ? 1 : 0);
        lvitem.stateMask = LVIS_OVERLAYMASK | LVIS_STATEIMAGEMASK;
        SetItemState(idx, &lvitem);
    } else {    // we found an empty slot
        SetItemText(idx, column, str);

        // Set the overlay image bits to specify if the text should be red.
        if (severe) {
            LVITEM lvitem;
            lvitem.mask = LVIF_STATE;
            lvitem.state = INDEXTOOVERLAYMASK(1);
            lvitem.stateMask = LVIS_OVERLAYMASK;
            SetItemState(idx, &lvitem);
        }
    }

    return idx;
}
