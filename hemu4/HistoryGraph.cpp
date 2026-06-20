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
 *  FILENAME:   HistoryGraph.cpp
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
#include "HistoryGraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistoryGraph

CHistoryGraph::CHistoryGraph()
{
    m_Median = 0;
    m_MinValue = -10;
    m_MaxValue = 10;
    m_LineSegCount = 20;
    m_AllocSize = 40;
    m_UnitsPerPixelY = 0;
    m_PixelsPerSegmentX = 0;
    m_ArrayItemIdx = 0;
    m_BkColor = RGB(0, 0, 0);
    m_MedianColor = RGB(127, 127, 127);
}

CHistoryGraph::~CHistoryGraph()
{
    const int size = m_LineData.GetSize();

    for (int i = 0; i < size; i++)
        if (m_LineData.GetAt(i))
            delete [] m_LineData.GetAt(i);
}

BOOL CHistoryGraph::SetNumPoints(const unsigned int num)
{
    // If we try to have less than one pixel per segment, fail.
    CRect rect;
    GetClientRect(&rect);
    float temp = rect.Width() / (float)(m_LineSegCount - 1);
    if (temp < 1.0f)
        return FALSE;

    // If we need to grow the array, allocate enough memory for twice the requested
    // number of elements and copy the old array to the new one.
    if (num > m_AllocSize) {
        unsigned int k = 0;
        unsigned int sz = m_LineData.GetSize();

        m_AllocSize = 2 * num;

        for (unsigned int i = 0; i < sz; i++) {
            int *ptr = new int[m_AllocSize];

            for (unsigned int j = m_ArrayItemIdx; j < m_LineSegCount; j++)
                ptr[k++] = ((int *)(m_LineData[i]))[j];

            for (unsigned int j = 0; j < m_ArrayItemIdx; j++)
                ptr[k++] = ((int *)(m_LineData[i]))[j];

            delete [] m_LineData.GetAt(i);
            m_LineData[i] = (void *)ptr;
        }
    }

    m_LineColors.SetSize(num);
    m_LineSegCount = num;
    m_ArrayItemIdx = 0;
    m_PixelsPerSegmentX = temp;

    return TRUE;
}

unsigned int CHistoryGraph::AddLine(const COLORREF rgb)
{
    int sz = m_LineData.GetSize();

    int *ptr = new int[m_LineSegCount];
    for (unsigned int i = 0; i < m_LineSegCount; i++)
        ptr[i] = m_Median;

    m_LineData.SetAtGrow(sz, (void *)ptr);
    m_LineColors.SetAtGrow(sz, (DWORD)rgb);

    return sz;
}

void CHistoryGraph::Clear(void)
{
    unsigned int sz = m_LineData.GetSize();

    for (unsigned int i = 0; i < sz; i++)
        for (unsigned int j = 0; j < m_LineSegCount; j++)
            ((int *)(m_LineData[i]))[j] = m_Median;
}

void CHistoryGraph::Scroll(void)
{

    // Increment or wrap the array index.
    m_ArrayItemIdx++;
    if (m_ArrayItemIdx >= m_LineSegCount)
        m_ArrayItemIdx = 0;

    Invalidate(FALSE);
}

int CHistoryGraph::GetCurrentYPos(const int linenum)
{
    int ypos = 0;

    if (linenum > (int)m_LineSegCount)
        return 0;

    int idx = m_ArrayItemIdx - 1;
    if (idx < 0)
        idx = m_LineSegCount - 1;

    ypos = (m_MaxValue - ((int *)(m_LineData[linenum]))[idx]);
    ypos = (int)((float)ypos / m_UnitsPerPixelY);

    return ypos;
}

int CHistoryGraph::GetAvgYPos(const int linenum)
{
    int count = 0;
    int ypos = 0;
    float temp = 0.0f;

    if (linenum > (int)m_LineSegCount)
        return 0;

    // First do the end of the array.
    for (unsigned int i = m_ArrayItemIdx + 1; i < m_LineSegCount; i++) {
        count++;
        ypos += m_MaxValue - ((int *)(m_LineData[linenum]))[i];
    }

    // Next, go back to the beginning and do the front of the array.
    for (unsigned int i = 0; i < m_ArrayItemIdx; i++) {
        count++;
        ypos += m_MaxValue - ((int *)(m_LineData[linenum]))[i];
    }

    temp = ypos / (float)count;
    ypos = (int)(temp / m_UnitsPerPixelY);

    return ypos;
}

BEGIN_MESSAGE_MAP(CHistoryGraph, CWnd)
    //{{AFX_MSG_MAP(CHistoryGraph)
    ON_WM_SIZE()
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHistoryGraph message handlers

BOOL CHistoryGraph::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext)
{
    return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CHistoryGraph::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    m_UnitsPerPixelY = (m_MaxValue - m_MinValue) / (float)(cy);
    m_PixelsPerSegmentX = cx / (float)(m_LineSegCount - 1);
}

void CHistoryGraph::OnPaint()
{
    CPaintDC paintdc(this);
    CDC memdc;
    memdc.CreateCompatibleDC(&paintdc);

    // Get the size of the window.
    CRect BkgndRect;
    GetClientRect(&BkgndRect);

    // Create a bitmap that we can draw to.  We will blit this to the paintdc
    // at the end.
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(&paintdc, BkgndRect.Width(), BkgndRect.Height());
    CBitmap *pOldBitmap = memdc.SelectObject(&bitmap);

    // Set the clipping region so we don't draw outside the client area.
    CRgn rgn;
    rgn.CreateRectRgnIndirect(&BkgndRect);
    memdc.SelectClipRgn(&rgn, RGN_COPY);

    // Draw the background.
    CBrush brush;
    brush.CreateSolidBrush(m_BkColor);
    memdc.FillRect(&BkgndRect, &brush);

    // Set the background color to match in case we have to draw dashed lines.
    memdc.SetBkColor(m_BkColor);

    // Draw the median line.
    int median_y = int((m_MaxValue - m_Median) / m_UnitsPerPixelY);
    CPen median_pen(PS_SOLID, 1, m_MedianColor);
    CPen *pOldPen = memdc.SelectObject(&median_pen);
    memdc.MoveTo(0, median_y);
    memdc.LineTo(BkgndRect.right, median_y);

    // Now draw each line.
    unsigned int k = 0;
    const unsigned int sz = m_LineData.GetSize();

    for (unsigned int i = 0; i < sz; i++) {
        unsigned int x1 = 0;
        unsigned int x2 = 0;
        int y1 = 0;
        int y2 = 0;

        // Counter to figure out the new x position.
        k = 0;

        // Create a pen with the specified color.
        CPen pen_solid(PS_SOLID, 1, (COLORREF)m_LineColors[i]);
        memdc.SelectObject(&pen_solid);

        // Do this in two steps.  First, do the end of the array.
        y1 = -int((((int *)(m_LineData[i]))[m_ArrayItemIdx] - m_Median) / m_UnitsPerPixelY) + median_y;
        for (unsigned int j = m_ArrayItemIdx + 1; j < m_LineSegCount; j++) {
            // Original code is simple but does not draw along the edge if the line is out of bounds:
            //      x += (int)pixels_per_segment;
            //      y = -int((((int *)(m_LineData[i]))[j] - m_Median) / m_UnitsPerPixelY) + median_y;
            //      dc.LineTo(x, y);

            // These two lines give the same result:
            //      y2 = -int((((int *)(m_LineData[i]))[j] - m_Median) / m_UnitsPerPixelY) + median_y;
            y2 = (int)((m_MaxValue - ((int *)(m_LineData[i]))[j]) / m_UnitsPerPixelY);

            x2 = (int)(++k * m_PixelsPerSegmentX);
            DrawSegment(&memdc, x1, y1, x2, y2, m_LineColors[i]);
            x1 = x2;
            y1 = y2;
        }

        // Next, go back to the beginning and do the front of the array.
        for (unsigned int j = 0; j <= m_ArrayItemIdx; j++) {
            // Original code is simple but does not draw along the edge if the line is out of bounds:
            //      x += (int)pixels_per_segment;
            //      y = -int((((int *)(m_LineData[i]))[j] - m_Median) / m_UnitsPerPixelY) + median_y;
            //      dc.LineTo(x, y);

            // These two lines give the same result:
            //      y2 = -int((((int *)(m_LineData[i]))[j] - m_Median) / m_UnitsPerPixelY) + median_y;
            y2 = (int)((m_MaxValue - ((int *)(m_LineData[i]))[j]) / m_UnitsPerPixelY);

            x2 = (int)(++k * m_PixelsPerSegmentX);
            DrawSegment(&memdc, x1, y1, x2, y2, m_LineColors[i]);
            x1 = x2;
            y1 = y2;
        }

        // Select the previous pen before we destroy the current one on the stack.
        memdc.SelectObject(&median_pen);
    }

    // Now copy from the mem DC to the paint DC.
    paintdc.BitBlt(0, 0, BkgndRect.Width(), BkgndRect.Height(), &memdc, 0, 0, SRCCOPY);
}

void CHistoryGraph::DrawSegment(CDC *dc, int x1, int y1, int x2, int y2, COLORREF rgb)
{
    CPen *old_pen = dc->GetCurrentPen();
    CRect BkgndRect;
    GetClientRect(&BkgndRect);

    int bottom = BkgndRect.bottom - 1;

    // Windows won't draw segment if start point is beyond clipping region, so adjust
    // y1 if necessary.
    if (y1 < 0)
        dc->MoveTo(x1, 0);
    else if (y1 > bottom)
        dc->MoveTo(x1, bottom);
    else
        dc->MoveTo(x1, y1);

    // If the point is off the window, draw a dotted line along that edge.
    if (y1 < 0) {
        // If both ends are off the window, draw the line.
        if (y2 <= 0) {
            // Create the pen
            CPen pen_dotted(PS_DOT, 1, rgb);
            dc->SelectObject(&pen_dotted);

            dc->LineTo(x2, 0);

            // Reset to the solid pen.
            dc->SelectObject(old_pen);
        } else {
            // If the new end is within the window, we must divide the
            // line into two parts (dotted and solid).

            // Find the intersection with the edge.  It will be at (x_intersect, 0).
            double m = (y2 - y1) / (double)(x2 - x1);
            int x_intersect = (int)((-y1 / m) + x1);

            // Draw the dotted line.
            CPen pen_dotted(PS_DOT, 1, rgb);
            dc->SelectObject(&pen_dotted);
            dc->LineTo(x_intersect, 0);

            // Draw the dotted line
            dc->SelectObject(old_pen);
            dc->LineTo(x2, y2);
        }
    } else if (y1 > bottom) {
        // If both ends are off the window, draw the line.
        if (y2 >= bottom) {
            // Create the pen
            CPen pen_dotted(PS_DOT, 1, rgb);
            dc->SelectObject(&pen_dotted);

            dc->LineTo(x2, bottom);

            // Reset to the solid pen.
            dc->SelectObject(old_pen);
        } else {
            // If the new end is within the window, we must divide the
            // line into two parts (dotted and solid).

            // Find the intersection with the edge.  It will be at (x_intersect, bottom).
            double m = (y2 - y1) / (double)(x2 - x1);
            int x_intersect = (int)(((bottom - y1) / m) + x1);

            // Draw the dotted line.
            CPen pen_dotted(PS_DOT, 1, rgb);
            dc->SelectObject(&pen_dotted);
            dc->LineTo(x_intersect, bottom);

            // Draw the dotted line
            dc->SelectObject(old_pen);
            dc->LineTo(x2, y2);
        }
    } else {
        if (y2 < 0) {
            // We have to divide the line into two parts (solid and dotted).

            // Find the intersection with the edge.  It will be at (x_intersect, 0).
            double m = (y2 - y1) / (double)(x2 - x1);
            int x_intersect = (int)((-y1 / m) + x1);

            // Draw the solid line.
            dc->LineTo(x_intersect, 0);

            CPen pen_dotted(PS_DOT, 1, rgb);
            dc->SelectObject(&pen_dotted);
            dc->LineTo(x2, 0);
            dc->SelectObject(old_pen);
        } else if (y2 > bottom) {
            // We have to divide the line into two parts (solid and dotted).

            // Find the intersection with the edge.  It will be at (x_intersect, bottom).
            double m = (y2 - y1) / (double)(x2 - x1);
            int x_intersect = (int)(((bottom - y1) / m) + x1);

            // Draw the solid line.
            dc->LineTo(x_intersect, bottom);

            CPen pen_dotted(PS_DOT, 1, rgb);
            dc->SelectObject(&pen_dotted);
            dc->LineTo(x2, bottom);
            dc->SelectObject(old_pen);

        } else {
            // If the segment is within the window, just draw the line.
            dc->LineTo(x2, y2);
        }
    }
}
