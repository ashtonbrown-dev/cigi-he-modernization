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

// TranspStatic.cpp : implementation file
//

#include "TranspStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTranspStatic

CTranspStatic::CTranspStatic()
{
}

CTranspStatic::~CTranspStatic()
{
}

BEGIN_MESSAGE_MAP(CTranspStatic, CStatic)
    //{{AFX_MSG_MAP(CTranspStatic)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

HBITMAP CTranspStatic::LoadBitmap(UINT nIDResource)
{
    COLORMAP colormap;
    colormap.from = RGB(192, 192, 192);
    colormap.to = GetSysColor(COLOR_BTNFACE);
    m_Bitmap.LoadMappedBitmap(nIDResource, 0, &colormap, 1);

    return SetBitmap(HBITMAP(m_Bitmap));
}

void CTranspStatic::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    BITMAP bm;

    m_Bitmap.GetBitmap(&bm);
    CPoint size(bm.bmWidth, bm.bmHeight);
    dc.DPtoLP(&size);

    CPoint org(0, 0);
    dc.DPtoLP(&org);

    CDC dcMem;
    dcMem.CreateCompatibleDC(&dc);
    CBitmap *pOldBitmap = dcMem.SelectObject(&m_Bitmap);
    dcMem.SetMapMode(dc.GetMapMode());
    dc.BitBlt(0, 0, size.x, size.y, &dcMem, org.x, org.y, SRCCOPY);
    dcMem.SelectObject(pOldBitmap);

    // Do not call CStatic::OnPaint() for painting messages
}
