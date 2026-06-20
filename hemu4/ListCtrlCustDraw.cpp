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
 *  FILENAME:   ListCtrlCustDraw.cpp
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
#include "ListCtrlCustDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlCustDraw

CListCtrlCustDraw::CListCtrlCustDraw()
{
    m_DrawColumnLines = TRUE;
    m_ColumnLineStyle = PS_SOLID;
    m_ColumnLineColor = ::GetSysColor(COLOR_BTNSHADOW);
    m_FullRowSelect = TRUE;
}

CListCtrlCustDraw::~CListCtrlCustDraw()
{
}

BEGIN_MESSAGE_MAP(CListCtrlCustDraw, CListCtrl)
    //{{AFX_MSG_MAP(CListCtrlCustDraw)
    ON_WM_PAINT()
    ON_WM_HSCROLL()
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlCustDraw message handlers

void CListCtrlCustDraw::OnPaint()
{
    // First let the control do its default drawing.
    const MSG *msg = GetCurrentMessage();
    DefWindowProc(msg->message, msg->wParam, msg->lParam);

    if (!m_DrawColumnLines) return;

    CClientDC dc(this);

    // Draw the lines only for LVS_REPORT mode
    if ((GetStyle() & LVS_TYPEMASK) == LVS_REPORT) {
        CHeaderCtrl *header = GetHeaderCtrl();
        // Get the number of columns
        int ColCount = header->GetItemCount();

        CRect rect;
        GetClientRect(&rect);

        CRect hdrect;
        header->GetClientRect(&hdrect);

        // The border of the column is offset by the horz scroll
        int borderx = 0 - GetScrollPos(SB_HORZ);
        int bordery = hdrect.Height();

        CPen *pOldPen;
        CPen pen;
        CGdiObject *pOldBrush;
        pen.CreatePen(m_ColumnLineStyle, 0, m_ColumnLineColor);
        pOldPen = dc.SelectObject(&pen);
        pOldBrush = dc.SelectStockObject(NULL_BRUSH);

        for (int i = 0; i < ColCount; i++) {
            // Get the next border
            borderx += GetColumnWidth(i);

            // If next border is outside client area, stop.
            if (borderx >= rect.right)
                break;

            // Draw the line.
            dc.MoveTo(borderx - 1, bordery);
            dc.LineTo(borderx - 1, rect.bottom);
        }

        dc.SelectObject(pOldPen);
        dc.SelectObject(pOldBrush);
    }

    // Do the default painting.
    CListCtrl::OnPaint();
}

void CListCtrlCustDraw::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
    CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);

    // We have a couple of rows of pixels immediately under the header
    // that are not getting redrawn.  We can take care of this here.
    CRect rect;
    GetHeaderCtrl()->GetClientRect(&rect);
    rect.bottom += 2;
    RedrawWindow(&rect);
}

void CListCtrlCustDraw::OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVCUSTOMDRAW *custdraw = (NMLVCUSTOMDRAW *)pNMHDR;

    // Copy the device context into hdc, but don't convert it to a pDC* until
    // (and if) we need it as this requires a bit of work internally for MFC
    // to create temporary CDC objects.
    HDC hdc = custdraw->nmcd.hdc;
    CDC *pDC = NULL;

    // Get the item info.  Note that we don't get the subitem number here,
    // since this may not be valid data except when we are handling a sub
    // item notification.  We'll do that separately in the appropriate
    // case statements below.
    int nItem = custdraw->nmcd.dwItemSpec;
    UINT nState = custdraw->nmcd.uItemState;
    LPARAM lParam = custdraw->nmcd.lItemlParam;

    // Set up flags that will control the return value for *pResult.
    BOOL bNotifyPostPaint = FALSE;
    BOOL bNotifyItemDraw = FALSE;
    BOOL bNotifySubItemDraw = FALSE;
    BOOL bSkipDefault = FALSE;
    BOOL bNewFont = FALSE;

    // What we do next depends on the drawing stage we are processing.
    switch (custdraw->nmcd.dwDrawStage) {
    case CDDS_PREPAINT: {   // PrePaint
        m_pOldItemFont = NULL;
        m_pOldSubItemFont = NULL;

        bNotifyPostPaint = IsNotifyPostPaint();
        bNotifyItemDraw = IsNotifyItemDraw();

        // Do we want to draw the control ourselves?
        if (IsDraw()) {
            if (!pDC)
                pDC = CDC::FromHandle(hdc);

            CRect rect(custdraw->nmcd.rc);

            // Call the overridden OnDraw().
            if (OnDraw(pDC, rect))
                bSkipDefault = TRUE;    // don't do default drawing
        }

        break;
    }

    case CDDS_ITEMPREPAINT: {   // Item PrePaint
        m_pOldItemFont = NULL;

        bNotifyPostPaint = IsNotifyItemPostPaint(nItem, nState, lParam);
        bNotifySubItemDraw = IsNotifySubItemDraw(nItem, nState, lParam);

        // Set up the colors to use at this stage.
        custdraw->clrText = TextColorForItem(nItem, nState, lParam);
        custdraw->clrTextBk = BkColorForItem(nItem, nState, lParam);

        // Set up a different font to use, if any.
        CFont *pNewFont = FontForItem(nItem, nState, lParam);
        if (pNewFont) {
            if (!pDC)
                pDC = CDC::FromHandle(hdc);

            m_pOldItemFont = pDC->SelectObject(pNewFont);

            bNotifyPostPaint = TRUE;    // need to restore font
        }

        // Do we want to draw the item ourselves?
        if (IsItemDraw(nItem, nState, lParam)) {
            if (! pDC) pDC = CDC::FromHandle(hdc);

            // Call the overridden OnDraw() (don't do the default drawing).
            if (OnItemDraw(pDC, nItem, nState, lParam))
                bSkipDefault = TRUE;
        }

        break;
    }

    case CDDS_ITEMPREPAINT | CDDS_SUBITEM: {    // Sub Item PrePaint
        // Set sub item number (data will be valid now).
        int nSubItem = custdraw->iSubItem;

        m_pOldSubItemFont = NULL;

        bNotifyPostPaint = IsNotifySubItemPostPaint(nItem, nSubItem, nState, lParam);

        // Set up the colors to use.
        custdraw->clrText = TextColorForSubItem(nItem, nSubItem, nState, lParam);
        custdraw->clrTextBk = BkColorForSubItem(nItem, nSubItem, nState, lParam);

        // Set up a different font to use, if any.
        CFont *pNewFont = FontForSubItem(nItem, nSubItem, nState, lParam);

        if (pNewFont) {
            if (!pDC)
                pDC = CDC::FromHandle(hdc);

            m_pOldSubItemFont = pDC->SelectObject(pNewFont);

            bNotifyPostPaint = TRUE;    // need to restore font
        }

        // do we want to draw the item ourselves?
        if (IsSubItemDraw(nItem, nSubItem, nState, lParam)) {
            if (!pDC)
                pDC = CDC::FromHandle(hdc);

            if (OnSubItemDraw(pDC, nItem, nSubItem, nState, lParam))
                bSkipDefault = TRUE;
        }

        break;
    }

    case CDDS_ITEMPOSTPAINT | CDDS_SUBITEM: {   // Sub Item PostPaint
        // Set sub item number (data will be valid now).
        int nSubItem = custdraw->iSubItem;

        // restore old font if any
        if (m_pOldSubItemFont) {
            if (!pDC)
                pDC = CDC::FromHandle(hdc);

            pDC->SelectObject(m_pOldSubItemFont);

            m_pOldSubItemFont = NULL;
        }

        // Do we want to do any extra drawing?
        if (IsSubItemPostDraw()) {
            if (!pDC)
                pDC = CDC::FromHandle(hdc);

            OnSubItemPostDraw(pDC, nItem, nSubItem, nState, lParam);
        }

        break;
    }

    case CDDS_ITEMPOSTPAINT: {      // Item PostPaint
        // Restore old font if any
        if (m_pOldItemFont) {
            if (!pDC)
                pDC = CDC::FromHandle(hdc);

            pDC->SelectObject(m_pOldItemFont);
            m_pOldItemFont = NULL;
        }

        // Do we want to do any extra drawing?
        if (IsItemPostDraw()) {
            if (!pDC)
                pDC = CDC::FromHandle(hdc);

            OnItemPostDraw(pDC, nItem, nState, lParam);
        }

        break;
    }

    case CDDS_POSTPAINT: {      // Item PostPaint
        // do we want to do any extra drawing?
        if (IsPostDraw()) {
            if (!pDC)
                pDC = CDC::FromHandle(hdc);

            CRect rect(custdraw->nmcd.rc);

            OnPostDraw(pDC, rect);
        }

        break;
    }
    }

    *pResult = 0;

    if (bNotifyPostPaint)
        *pResult |= CDRF_NOTIFYPOSTPAINT;

    if (bNotifyItemDraw)
        *pResult |= CDRF_NOTIFYITEMDRAW;

    if (bNotifySubItemDraw)
        *pResult |= CDRF_NOTIFYSUBITEMDRAW;

    if (bNewFont)
        *pResult |= CDRF_NEWFONT;

    if (bSkipDefault)
        *pResult |= CDRF_SKIPDEFAULT;

    // Redundant, since CDRF_DODEFAULT == 0, but we'll set it anyway.
    if (*pResult == 0)
        *pResult = CDRF_DODEFAULT;
}
