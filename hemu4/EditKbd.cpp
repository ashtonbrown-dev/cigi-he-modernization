/** <pre>
 *  Copyright 2004 The Boeing Company
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
 *
 *  FILENAME:   EditKbd.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface Host Emulator (CIGI HE)
 *
 *  PROGRAM DESCRIPTION:
 *  The Host Emulator (HE) acts as a surrogate in the absence of a simulation host.
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME
 *  DESCRIPTION OF CHANGE
 *
 *  11/11/2004 Lance Durham
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  Version 3.1.1
 */

#include "stdafx.h"
#include "EditKbd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditKbd

CEditKbd::CEditKbd()
{
}

CEditKbd::~CEditKbd()
{
}

BEGIN_MESSAGE_MAP(CEditKbd, CEdit)
    //{{AFX_MSG_MAP(CEditKbd)
    ON_WM_KEYUP()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEACTIVATE()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_SETFOCUS()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditKbd message handlers

void CEditKbd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nFlags & 256) // Extended key bit
        GetParent()->PostMessage(WM_USEREXTKEY, (WPARAM)nChar, (LPARAM)this);
    else
        GetParent()->PostMessage(WM_USEREDIT, (WPARAM)nChar, (LPARAM)this);

    CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CEditKbd::OnLButtonUp(UINT nFlags, CPoint point)
{
    GetParent()->PostMessage(WM_USERMOUSE, (WPARAM)0, (LPARAM)this);

    CEdit::OnLButtonUp(nFlags, point);
}

int CEditKbd::OnMouseActivate(CWnd *pDesktopWnd, UINT nHitTest, UINT message)
{
    GetParent()->PostMessage(WM_USERFOCUS, (WPARAM)0, (LPARAM)this);

    return CEdit::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CEditKbd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    GetParent()->PostMessage(WM_USERMOUSE, (WPARAM)0, (LPARAM)this);

    CEdit::OnLButtonDblClk(nFlags, point);
}

void CEditKbd::OnSetFocus(CWnd *pOldWnd)
{
    CEdit::OnSetFocus(pOldWnd);

    GetParent()->PostMessage(WM_USERFOCUS, (WPARAM)0, (LPARAM)this);
}
