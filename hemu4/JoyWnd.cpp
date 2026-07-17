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
 *  FILENAME:   JoyWnd.cpp
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

#include "JoyWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJoyWnd

CJoyWnd::CJoyWnd()
{
    m_AutoCenter = TRUE;
    m_BorderSize = 1;
    m_BallRadiusRatio = 0.08;
    m_BallRadiusPixels = 10;
    m_BkColor = GetSysColor(COLOR_BTNFACE);
    m_BallColor = GetSysColor(COLOR_BTNTEXT);
    m_BkBrush.CreateSolidBrush(m_BkColor);
    m_BallBrush.CreateSolidBrush(m_BallColor);
    m_MaxX = 100;
    m_MinX = 0;
    m_MaxY = 100;
    m_MinY = 0;
}

CJoyWnd::~CJoyWnd()
{
}

BEGIN_MESSAGE_MAP(CJoyWnd, CWnd)
    //{{AFX_MSG_MAP(CJoyWnd)
    ON_WM_MOUSEMOVE()
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_LBUTTONUP()
    ON_WM_CAPTURECHANGED()
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJoyWnd message handlers

void CJoyWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    // If the left mouse button is not down, do nothing.
    if (!(nFlags & MK_LBUTTON))
        return;

    // Adjust point so the ball will not go beyond the client area.
    long margin = m_BallRadiusPixels + m_BorderSize;
    if (point.x <= m_WindowRect.left + margin)
        point.x = m_WindowRect.left + margin;
    if (point.x > m_WindowRect.right - margin)
        point.x = m_WindowRect.right - margin;
    if (point.y <= m_WindowRect.top + margin)
        point.y = m_WindowRect.top + margin;
    if (point.y > m_WindowRect.bottom - margin)
        point.y = m_WindowRect.bottom - margin;

    m_BallPos = point;
    DrawBall();

    // Update the position and post a message.
    m_PosX = CalcLogicalX(point.x);
    m_PosY = CalcLogicalY(point.y);
    GetParent()->PostMessage(WM_CHANGEJOYPOS, (WPARAM)m_PosX, (LPARAM)m_PosY);

    CWnd::OnMouseMove(nFlags, point);
}

void CJoyWnd::SetPosX(const long x, const BOOL notify)
{
    if (x == m_PosX)
        return;

    m_PosX = x;
    m_BallPos.x = CalcWindowX(x);
    DrawBall();

    if (notify)
        GetParent()->PostMessage(WM_CHANGEJOYPOS, (WPARAM)m_PosX,
                                 (LPARAM)m_PosY);
}

void CJoyWnd::SetPosY(const long y, const BOOL notify)
{
    if (y == m_PosY)
        return;

    m_PosY = y;
    m_BallPos.y = CalcWindowY(y);
    DrawBall();

    if (notify)
        GetParent()->PostMessage(WM_CHANGEJOYPOS, (WPARAM)m_PosX,
                                 (LPARAM)m_PosY);
}

void CJoyWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // Fill with background color.
    CBrush brush(m_BkColor);
    dc.FillRect(&m_WindowRect, &brush);

    // Draw a 3D border.
    CPen *savepen;
    CPen shadow(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));
    CPen hilight(PS_SOLID, 0, GetSysColor(COLOR_BTNHIGHLIGHT));
    savepen = dc.SelectObject(&shadow);
    dc.MoveTo(m_WindowRect.left, m_WindowRect.bottom);
    dc.LineTo(m_WindowRect.left, m_WindowRect.top);
    dc.LineTo(m_WindowRect.right, m_WindowRect.top);
    dc.SelectObject(&hilight);
    dc.LineTo(m_WindowRect.right, m_WindowRect.bottom);
    dc.LineTo(m_WindowRect.left, m_WindowRect.bottom);
    dc.SelectObject(savepen);

    // Draw the ball.
    dc.SelectObject(&m_BallBrush);
    dc.Ellipse(m_BallPos.x - m_BallRadiusPixels, m_BallPos.y - m_BallRadiusPixels,
               m_BallPos.x + m_BallRadiusPixels, m_BallPos.y + m_BallRadiusPixels);

    // Do not call CWnd::OnPaint() for painting messages
}

void CJoyWnd::SetMaxX(const long x)
{
    m_MaxX = (long)x;
    m_MinX = (long) - x;

    // Set the ball radius based on the smaller of the two dimensions.
    long length = m_MaxX - m_MinX;
    if ((m_MaxY - m_MinY) < length)
        length = m_MaxY - m_MinY;
    m_BallRadiusPixels = (long)(length * m_BallRadiusRatio);

    DrawBall();
}

void CJoyWnd::SetMaxY(const long y)
{
    m_MaxY = (long)y;
    m_MinY = (long) - y;

    // Set the ball radius based on the smaller of the two dimensions.
    long length = m_MaxX - m_MinX;
    if ((m_MaxY - m_MinY) < length)
        length = m_MaxY - m_MinY;
    m_BallRadiusPixels = (long)(length * m_BallRadiusRatio);

    DrawBall();
}

void CJoyWnd::Init(void)
{
    // Set the ball radius based on the smaller of the two dimensions.
    GetClientRect(&m_WindowRect);
    long length = m_WindowRect.Width();
    if (m_WindowRect.Height() < length)
        length = m_WindowRect.Height();
    m_BallRadiusPixels = (long)(length * m_BallRadiusRatio);

    // Set the initial MBR of the ball so it will be valid when we call
    // CenterBall() below.
    m_BallMBR = CRect((length / 2) - m_BallRadiusPixels,
                      (m_WindowRect.Height() / 2) - m_BallRadiusPixels,
                      (length / 2) + m_BallRadiusPixels,
                      (m_WindowRect.Height() / 2) + m_BallRadiusPixels);

    CenterBall();
}

void CJoyWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    // Set the ball radius based on the smaller of the two dimensions.
    GetClientRect(&m_WindowRect);
    long length = m_WindowRect.right - m_WindowRect.left;
    if (m_WindowRect.bottom - m_WindowRect.top < length)
        length = m_WindowRect.bottom - m_WindowRect.top;
    m_BallRadiusPixels = (long)(length * m_BallRadiusRatio);
}

BOOL CJoyWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext)
{
    m_WindowRect = rect;

    // Set the ball size.
    long length = rect.right - rect.left;
    if (rect.bottom - rect.top < length)
        length = rect.bottom - rect.top;
    m_BallRadiusPixels = (long)(length * m_BallRadiusRatio);

    CenterBall();

    return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CJoyWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    // Capture the mouse so we still get messages even if we drag the mouse
    // outside the CJoyWnd area.
    SetCapture();

    // Everything else is pretty much like OnMouseMove():

    // Adjust point so the ball will not go beyond the client area.
    if (point.x <= m_WindowRect.left + m_BallRadiusPixels)
        point.x = m_WindowRect.left + m_BallRadiusPixels + 1;
    if (point.x > m_WindowRect.right - m_BallRadiusPixels)
        point.x = m_WindowRect.right - m_BallRadiusPixels;
    if (point.y <= m_WindowRect.top + m_BallRadiusPixels)
        point.y = m_WindowRect.top + m_BallRadiusPixels + 1;
    if (point.y > m_WindowRect.bottom - m_BallRadiusPixels)
        point.y = m_WindowRect.bottom - m_BallRadiusPixels;

    m_BallPos = point;
    DrawBall();

    // Update the position and post a message.
    m_PosX = CalcLogicalX(point.x);
    m_PosY = CalcLogicalY(point.y);
    GetParent()->PostMessage(WM_CHANGEJOYPOS, (WPARAM)m_PosX, (LPARAM)m_PosY);

    CWnd::OnLButtonDown(nFlags, point);
}

void CJoyWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    ReleaseCapture();

    // Automatically snap the ball to the center if we need to.
    if (m_AutoCenter) {
        CenterBall();
    }

    CWnd::OnLButtonUp(nFlags, point);
}

void CJoyWnd::OnCaptureChanged(CWnd *pWnd)
{
    CWnd::OnCaptureChanged(pWnd);
}

void CJoyWnd::CenterBall(void)
{
    CRect rect;
    GetClientRect(&rect);

    // Center the ball.
    CPoint point = rect.CenterPoint();
    m_BallPos = point;
    DrawBall();

    // Update the position and post a message.
    m_PosX = CalcLogicalX(point.x);
    m_PosY = CalcLogicalY(point.y);
    GetParent()->PostMessage(WM_CHANGEJOYPOS, (WPARAM)m_PosX, (LPARAM)m_PosY);
}

void CJoyWnd::DrawBall()
{
    CClientDC dc(this);

    // Erase the old ball.
    CRgn rgn;
    rgn.CreateRectRgnIndirect(&m_BallMBR);
    dc.FillRgn(&rgn, &m_BkBrush);

    // Draw a new ball.
    dc.SelectObject(&m_BallBrush);
    dc.Ellipse(m_BallPos.x - m_BallRadiusPixels, m_BallPos.y - m_BallRadiusPixels,
               m_BallPos.x + m_BallRadiusPixels, m_BallPos.y + m_BallRadiusPixels);

    // Save the current MBR so we can erase next time.
    m_BallMBR.left = m_BallPos.x - m_BallRadiusPixels;
    m_BallMBR.right = m_BallPos.x + m_BallRadiusPixels;
    m_BallMBR.top = m_BallPos.y - m_BallRadiusPixels;
    m_BallMBR.bottom = m_BallPos.y + m_BallRadiusPixels;

    // Delete the region we created.
    rgn.DeleteObject();
}
