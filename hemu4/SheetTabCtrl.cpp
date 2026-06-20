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

// SheetTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Hemu4.h"
#include "SheetTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSheetTabCtrl

CSheetTabCtrl::CSheetTabCtrl()
{
    m_BkgndColor = ::GetSysColor(COLOR_BTNFACE);
    m_SheetColor = RGB(255, 255, 255);  // white
    m_TabColor = ::GetSysColor(COLOR_BTNFACE);
    m_HilightColor = ::GetSysColor(COLOR_BTNHILIGHT);
    m_ShadowColor = ::GetSysColor(COLOR_3DSHADOW);
}

CSheetTabCtrl::~CSheetTabCtrl()
{
}

BEGIN_MESSAGE_MAP(CSheetTabCtrl, CTabCtrl)
    //{{AFX_MSG_MAP(CSheetTabCtrl)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheetTabCtrl message handlers

void CSheetTabCtrl::SetBkgndColor(COLORREF color)
{
    // set new color
    m_BkgndColor = color;

    // redraw
    if (GetSafeHwnd())
        Invalidate();
}

void CSheetTabCtrl::SetSheetColor(COLORREF color)
{
    // set new color
    m_SheetColor = color;

    // redraw
    if (GetSafeHwnd())
        Invalidate();
}

void CSheetTabCtrl::SetTabColor(COLORREF color)
{
    // set new color
    m_TabColor = color;

    // redraw
    if (GetSafeHwnd())
        Invalidate();
}

void CSheetTabCtrl::SetHilightColor(COLORREF color)
{
    // set new color
    m_HilightColor = color;

    // redraw
    if (GetSafeHwnd())
        Invalidate();
}

void CSheetTabCtrl::SetShadowColor(COLORREF color)
{
    // set new color
    m_ShadowColor = color;

    // redraw
    if (GetSafeHwnd())
        Invalidate();
}

CSize CSheetTabCtrl::FormatText(CString &sText, CDC *pDC, int nWidth)
{
    CRect rect(0, 0, nWidth, 20);
    UINT uFlags = DT_CALCRECT | DT_SINGLELINE | DT_MODIFYSTRING | DT_END_ELLIPSIS;

    pDC->DrawText((LPCTSTR)sText, -1, rect, uFlags);

    return pDC->GetTextExtent(sText);
}

void CSheetTabCtrl::DrawMainBorder(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CRect border(lpDrawItemStruct->rcItem);
    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

    CPen blackpen(PS_SOLID, 0, RGB(0, 0, 0));
    CPen *oldpen = pDC->SelectObject(&blackpen);
    pDC->MoveTo(0, border.bottom);
    pDC->LineTo(border.right + 1, border.bottom);

    pDC->SelectObject(oldpen);
}

void CSheetTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    BOOL bSelected = (lpDrawItemStruct->itemID == (UINT)GetCurSel());
    CRect itemrect = lpDrawItemStruct->rcItem;

    POINT points[4] = { {itemrect.left, itemrect.top},
        {itemrect.left + 8, itemrect.bottom},
        {itemrect.right - 7, itemrect.bottom},
        {itemrect.right + 1, itemrect.top}
    };

    // Create a new fill brush and a pen to match (to erase the top border).
    CBrush whitebrush(bSelected ? m_SheetColor : m_TabColor);
    CBrush *oldbrush = pDC->SelectObject(&whitebrush);
    CPen newpen(PS_SOLID, 0, bSelected ? m_SheetColor : RGB(0, 0, 0));
    CPen *oldpen = pDC->SelectObject(&newpen);

    // Draw the polygon.
    pDC->Polygon(points, 4);

    // Draw the point in the upper-right corner.
    pDC->SetPixel(itemrect.right + 1, itemrect.top, RGB(0, 0, 0));

    // Restore the old pen and brush.
    pDC->SelectObject(oldpen);
    pDC->SelectObject(oldbrush);


    // Draw the text.
    itemrect.left += 4;
    itemrect.right -= 4;
    itemrect.top += 1;

    pDC->SetBkMode(TRANSPARENT);

    CString tempstr;
    TC_ITEM tcitem;
    tcitem.mask = TCIF_TEXT | TCIF_IMAGE;
    tcitem.pszText = tempstr.GetBuffer(100);
    tcitem.cchTextMax = 99;
    GetItem(lpDrawItemStruct->itemID, &tcitem);
    tempstr.ReleaseBuffer();

    FormatText(tempstr, pDC, itemrect.Width());
    pDC->DrawText(tempstr, itemrect, DT_NOPREFIX | DT_CENTER);
}

void CSheetTabCtrl::DrawItemBorder(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    BOOL bSelected = (lpDrawItemStruct->itemID == (UINT)GetCurSel());
    CRect itemrect = lpDrawItemStruct->rcItem;

    // Draw the shadow.
    CPen shadowpen(PS_SOLID, 0, m_ShadowColor);
    CPen *oldpen = pDC->SelectObject(&shadowpen);
    pDC->MoveTo(itemrect.left + 8, itemrect.bottom - 1);
    pDC->LineTo(itemrect.right - 7, itemrect.bottom - 1);
    pDC->LineTo(itemrect.right, itemrect.top);

    // Draw the outline.
    CPen blackpen(PS_SOLID, 0, RGB(0, 0, 0));
    pDC->SelectObject(&blackpen);
    pDC->MoveTo(itemrect.left, itemrect.top);
    pDC->LineTo(itemrect.left + 8, itemrect.bottom);
    pDC->LineTo(itemrect.right - 7, itemrect.bottom);
    pDC->LineTo(itemrect.right + 1, itemrect.top);

    pDC->SelectObject(oldpen);
}

void CSheetTabCtrl::PreSubclassWindow()
{
    CTabCtrl::PreSubclassWindow();

    // Make sure the control has OWNERDRAW and BOTTOM properties.
    ModifyStyle(0, TCS_OWNERDRAWFIXED | TCS_RIGHT);
}

BOOL CSheetTabCtrl::OnEraseBkgnd(CDC *pDC)
{
    CRect rClient, rTab, rTotalTab, rBkgnd, rEdge;
    int nTab, nTabHeight = 0;

    CTabCtrl::OnEraseBkgnd(pDC);

    // calc total tab width
    GetClientRect(rClient);
    nTab = GetItemCount();
    rTotalTab.SetRectEmpty();

    while (nTab--) {
        GetItemRect(nTab, rTab);
        rTotalTab.UnionRect(rTab, rTotalTab);
    }

    nTabHeight = rTotalTab.Height();

    // add a bit
    rTotalTab.InflateRect(2, 3);
    rEdge = rTotalTab;

    // full width of tab ctrl above top of tabs
    rBkgnd = rClient;
    rBkgnd.bottom = rTotalTab.top + 5;
    pDC->SetBkColor(m_SheetColor);
    pDC->ExtTextOut(rBkgnd.left, rBkgnd.top, ETO_CLIPPED | ETO_OPAQUE, rBkgnd, "", NULL);

    // width of tab ctrl visible bkgnd including bottom pixel of tabs to left of tabs
    rBkgnd = rClient;
    rBkgnd.right = 2;
    rBkgnd.bottom = rBkgnd.top + (nTabHeight + 2);
    pDC->ExtTextOut(rBkgnd.left, rBkgnd.top, ETO_CLIPPED | ETO_OPAQUE, rBkgnd, "", NULL);

    // to right of tabs
    rBkgnd = rClient;
    rBkgnd.left += rTotalTab.Width() - 2;
    rBkgnd.bottom = rBkgnd.top + (nTabHeight + 2);
    pDC->ExtTextOut(rBkgnd.left, rBkgnd.top, ETO_CLIPPED | ETO_OPAQUE, rBkgnd, "", NULL);

    return TRUE;
}

void CSheetTabCtrl::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // Prepare DC
    CFont *pFont = dc.SelectObject(GetFont());

    DRAWITEMSTRUCT dis;
    dis.CtlType = ODT_TAB;
    dis.CtlID = GetDlgCtrlID();
    dis.hwndItem = GetSafeHwnd();
    dis.hDC = dc.GetSafeHdc();
    dis.itemAction = ODA_DRAWENTIRE;

    // paint the tabs first and then the borders
    int nTab = GetItemCount();
    int nSel = GetCurSel();

    if (!nTab) // no pages added
        return;

    while (nTab--) {
        if (nTab != nSel) {
            VERIFY(GetItemRect(nTab, &dis.rcItem));

            dis.rcItem.top += 2;
            dis.rcItem.left -= 2;
            dis.rcItem.right += 2;
            dis.itemID = nTab;
            dis.itemState = 0;

            DrawItem(&dis);
            DrawItemBorder(&dis);
        }
    }

    // Draw the lower border
    CRect rect;
    GetClientRect(&dis.rcItem);
    rect = dis.rcItem;
    AdjustRect(FALSE, rect);
    dis.rcItem.bottom = rect.bottom + 4;

    DrawMainBorder(&dis);

    // Now draw the selected tab
    dis.itemID = nSel;
    dis.itemState = ODS_SELECTED;

    VERIFY(GetItemRect(nSel, &dis.rcItem));

    dis.rcItem.top += 2;
    dis.rcItem.left -= 2;
    dis.rcItem.right += 2;
    DrawItem(&dis);
    DrawItemBorder(&dis);

    dc.SelectObject(pFont);
}
