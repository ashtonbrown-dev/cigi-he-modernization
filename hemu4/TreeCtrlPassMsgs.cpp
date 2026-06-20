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

// TreeCtrlPassMsgs.cpp : implementation file
//

#include "stdafx.h"
#include "hemu4.h"
#include "TreeCtrlPassMsgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlPassMsgs

CTreeCtrlPassMsgs::CTreeCtrlPassMsgs()
{
}

CTreeCtrlPassMsgs::~CTreeCtrlPassMsgs()
{
}

BEGIN_MESSAGE_MAP(CTreeCtrlPassMsgs, CTreeCtrl)
    //{{AFX_MSG_MAP(CTreeCtrlPassMsgs)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlPassMsgs message handlers

void CTreeCtrlPassMsgs::OnMouseMove(UINT nFlags, CPoint point)
{
    CWnd *parent = GetParent();
    if (parent) {
        CRect rect;
        rect.left = point.x;
        rect.top = point.y;
        MapWindowPoints(parent, &rect);
        short x = (short)rect.left;
        short y = (short)rect.top;
        parent->SendMessage(WM_MOUSEMOVE, (WPARAM)nFlags, MAKELPARAM(x, y));
    }

    // Call the default handler last, or the control won't process the
    // message correctly.
    CTreeCtrl::OnMouseMove(nFlags, point);
}

void CTreeCtrlPassMsgs::OnLButtonUp(UINT nFlags, CPoint point)
{
    CWnd *parent = GetParent();
    if (parent) {
        CRect rect;
        rect.left = point.x;
        rect.top = point.y;
        MapWindowPoints(parent, &rect);
        short x = (short)rect.left;
        short y = (short)rect.top;
        parent->SendMessage(WM_LBUTTONUP, (WPARAM)nFlags, MAKELPARAM(x, y));
    }

    // Call the default handler last, or the control won't process the
    // message correctly.
    CTreeCtrl::OnLButtonUp(nFlags, point);
}
