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

// NoMWComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "hemu4.h"
#include "FlyDlg.h"
#include "NoMWComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoMWComboBox

CNoMWComboBox::CNoMWComboBox()
{
}

CNoMWComboBox::~CNoMWComboBox()
{
}

BEGIN_MESSAGE_MAP(CNoMWComboBox, CComboBox)
    //{{AFX_MSG_MAP(CNoMWComboBox)
    ON_WM_MOUSEWHEEL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoMWComboBox message handlers

BOOL CNoMWComboBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    ((CFlyDlg *)GetParent())->DoMouseWheelThrottle(nFlags, zDelta, pt);

    return 1;   // Disable Mouse Wheel processing for this window.
}
