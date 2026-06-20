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

// SituationDispCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SituationDispCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTooltipWnd

CTooltipWnd::CTooltipWnd()
{
    m_XOffset = 20;
    m_YOffset = -10;
    m_Width = 20;
    m_Height = 20;
}

BEGIN_MESSAGE_MAP(CTooltipWnd, CWnd)
    ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CTooltipWnd::Create(CWnd *pParentWnd, DWORD dwStyle)
{
    return CWnd::CreateEx(NULL, TOOLTIPS_CLASS, NULL,
                          WS_POPUP | dwStyle, // force WS_POPUP
                          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, pParentWnd->GetSafeHwnd(),
                          NULL, NULL);
}

void CTooltipWnd::OnPaint()
{
    CPaintDC paintdc(this); // device context for painting

    // Calculate the size of the window.
    CalcWindowSize(&paintdc);
    CRect BkgndRect(0, 0, m_Width, m_Height);

    // Resize the window.
    SetWindowPos(NULL, 0, 0, m_Width, m_Height, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);

    // Draw the background.
    CBrush *pBrush = CBrush::FromHandle((GetSysColorBrush(COLOR_INFOBK)));
    paintdc.FillRect(&BkgndRect, pBrush);

    // Draw the text.
    BkgndRect.DeflateRect(4, 3, 0, 0);      // shrink the window
    paintdc.SetBkColor(GetSysColor(COLOR_INFOBK));
    paintdc.SetTextColor(GetSysColor(COLOR_INFOTEXT));
    paintdc.DrawText(m_Text, &BkgndRect, DT_LEFT);
}

int CTooltipWnd::GetNumTextLines(void)
{
    int count = 0;
    int start = 0;

    while ((start = m_Text.Find('\n', start + 1)) > -1)
        count++;

    return count + 1;
}

CString CTooltipWnd::GetLongestTextLine(CString &string)
{
    int start = 0;
    int end = 0;
    int length = 0;
    CString substring;
    CString teststring;

    // Compare all but the last line.
    while ((end = m_Text.Find('\n', end + 1)) > -1) {
        teststring = m_Text.Mid(start, end - start);

        if (teststring.GetLength() > substring.GetLength())
            substring = teststring;

        start = end;
    }

    // Now handle the last (and usually the only) line.
    teststring = m_Text.Mid(start);
    if (teststring.GetLength() > substring.GetLength())
        substring = teststring;

    return substring;
}

void CTooltipWnd::CalcWindowSize(CDC *pDC)
{
    // Get the longest line in the string.
    CString tempstr = GetLongestTextLine(m_Text);

    // Calculate the size of the window.
    CSize size = pDC->GetTextExtent(tempstr);
    m_Width = size.cx + 10;                             // leave some margin
    m_Height = (size.cy + 1) * GetNumTextLines() + 6;   // leave some margin
}

void CTooltipWnd::Popup(const int cx, const int cy)
{
    // Position the window.
    CPoint point(cx + m_XOffset, cy + m_YOffset);
    GetOwner()->ClientToScreen(&point);
    SetWindowPos(&wndTopMost, point.x, point.y, m_Width, m_Height, SWP_NOACTIVATE);

    // Now show the window.
    if (GetSafeHwnd() && !IsWindowVisible())
        ShowWindow(SW_SHOWNOACTIVATE);
}

void CTooltipWnd::Hide(void)
{
    if (GetSafeHwnd() && IsWindowVisible())
        ShowWindow(SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////
// CSituationDispCtrl

CSituationDispCtrl::CSituationDispCtrl()
{
    m_AutoSizeArea = FALSE;
    m_EntityCount = 0;
    m_NumCircles = 2;
    m_CircleInterval = 250;
    m_GamingAreaLength = 1000;
    m_MinGamingAreaLength = 1000;
    m_ActualAreaX = 0;
    m_ActualAreaY = 0;
    m_OriginX = 0;
    m_OriginY = 0;
    m_SelectedEntity = NULL;
    m_BkColor = RGB(0, 0, 0);
    m_CircleColor = RGB(0, 127, 0);
    m_SelectedColor = RGB(255, 255, 0);
}

CSituationDispCtrl::~CSituationDispCtrl()
{
    m_EntityList.RemoveAll();
}

BEGIN_MESSAGE_MAP(CSituationDispCtrl, CWnd)
    //{{AFX_MSG_MAP(CSituationDispCtrl)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_MOUSEMOVE()
    ON_WM_CREATE()
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSituationDispCtrl message handlers

BOOL CSituationDispCtrl::PreCreateWindow(CREATESTRUCT &cs)
{
    // Clear the WS_EX_NOPARENTNOTIFY style.  If this control is on a dialog,
    // we will have to set this manually through dialog editor.
    cs.dwExStyle &= ~WS_EX_NOPARENTNOTIFY;

    return CWnd::PreCreateWindow(cs);
}

int CSituationDispCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_Tooltip.Create(this);

    return 0;
}

void CSituationDispCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    CString text;
    static CString oldtext;

    // Go through our list and find every entity that the mouse is over.
    POSITION pos = NULL;
    pos = m_EntityList.GetHeadPosition();
    while (pos) {
        DISPCTRL_POS_AND_HDG entity = m_EntityList.GetNext(pos);
        int x = LogicalToViewportX(entity.x);
        if (entity.visible && (x - point.x < 6) && (x - point.x > -6)) {
            int y = LogicalToViewportY(entity.y);

            if ((y - point.y < 6) && (y - point.y > -6))
                text += CString(entity.name) + CString("\n");
        }
    }

    text.TrimRight();

    if (text != "") {
        // Handle the situation when we have to change the text.
        if (text != oldtext)
            HidePopup();

        DisplayPopup(point, text);
    } else
        HidePopup();

    oldtext = text;

    CWnd::OnMouseMove(nFlags, point);
}

void CSituationDispCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    // Go through our list and find every entity that the mouse is over.
    POSITION pos = NULL;
    POSITION entity_hdl = NULL;
    pos = m_EntityList.GetHeadPosition();
    while (pos) {
        entity_hdl = pos;
        DISPCTRL_POS_AND_HDG entity = m_EntityList.GetNext(pos);

        int x = LogicalToViewportX(entity.x);
        if (entity.visible && (x - point.x < 6) && (x - point.x > -6)) {
            int y = LogicalToViewportY(entity.y);

            if ((y - point.y < 6) && (y - point.y > -6)) {
                GetParent()->PostMessage(SDC_PICK, (WPARAM)entity_hdl, 0);
                return;
            }
        }
    }

    CWnd::OnLButtonDown(nFlags, point);
}

// Initialization for when using with a dialog resource.
void CSituationDispCtrl::InitDlgControl(CWnd *pParent)
{
    // Call Create() for the tooltip window.
    m_Tooltip.Create(this);

    m_Tooltip.SetOwner(this);

    // Post a resize message.
    CRect rect;
    pParent->GetClientRect(&rect);
    this->SendMessage(WM_SIZE, SIZE_RESTORED, MAKELPARAM(rect.Width(), rect.Height()));
}

void CSituationDispCtrl::DisplayPopup(CPoint point, CString text)
{
    m_Tooltip.Popup(point.x, point.y, (LPCTSTR)text);
}

void CSituationDispCtrl::HidePopup(void)
{
    m_Tooltip.Hide();
}

double CSituationDispCtrl::ViewportToLogicalX(double x)
{
    // Get the size of the window.
    CRect rect;
    GetClientRect(&rect);

    return (m_ActualAreaX * (double)(x - (rect.Width() / 2)) / (double)rect.Width()) + m_OriginX;
}

double CSituationDispCtrl::ViewportToLogicalY(double y)
{
    // Get the size of the window.
    CRect rect;
    GetClientRect(&rect);

    return (m_ActualAreaY * (double)(y - (rect.Height() / 2)) / (double)rect.Height()) + m_OriginY;
}

int CSituationDispCtrl::LogicalToViewportX(double x)
{
    // Get the size of the window.
    CRect rect;
    GetClientRect(&rect);

    return (rect.Width() / 2) + (int)((x - m_OriginX) * rect.Width() / m_ActualAreaX);
}

int CSituationDispCtrl::LogicalToViewportY(double y)
{
    // Get the size of the window.
    CRect rect;
    GetClientRect(&rect);

    return (rect.Height() / 2) - (int)((y - m_OriginY) * rect.Height() / m_ActualAreaY);
}

int CSituationDispCtrl::LogicalToViewportWidth(double x)
{
    // Get the size of the window.
    CRect rect;
    GetClientRect(&rect);

    return (rect.Width() / 2) + (int)(x * rect.Width() / m_ActualAreaX);
}

int CSituationDispCtrl::LogicalToViewportHeight(double y)
{
    // Get the size of the window.
    CRect rect;
    GetClientRect(&rect);

    return (rect.Height() / 2) - (int)(y * rect.Height() / m_ActualAreaY);
}

POINT CSituationDispCtrl::GetPoint1(int x, int y, double heading)
{
    double hdg = -heading * PI / 180.0f;
    double sin_hdg = sin(hdg);
    double cos_hdg = cos(hdg);
    POINT p = { (int)(x + (4 * sin_hdg) - (4 * cos_hdg)),
                (int)(y + (4 * cos_hdg) + (4 * sin_hdg))
              };
    return p;
}

POINT CSituationDispCtrl::GetPoint2(int x, int y, double heading)
{
    double hdg = -heading * PI / 180.0f;
    double sin_hdg = sin(hdg);
    double cos_hdg = cos(hdg);
    POINT p = { (int)(x + (-4 * sin_hdg) + cos_hdg),
                (int)(y + (-4 * cos_hdg) - sin_hdg)
              };
    return p;
}

POINT CSituationDispCtrl::GetPoint3(int x, int y, double heading)
{
    double hdg = -heading * PI / 180.0f;
    double sin_hdg = sin(hdg);
    double cos_hdg = cos(hdg);
    POINT p = { (int)(x + (5 * sin_hdg) + (5 * cos_hdg)),
                (int)(y + (5 * cos_hdg) - (5 * sin_hdg))
              };
    return p;
}

POSITION CSituationDispCtrl::AddEntity(double x, double y, double heading, COLORREF rgb, const char *name)
{
    DISPCTRL_POS_AND_HDG entity = {x, y, heading, rgb, FALSE, {0}};

    if (strlen(name) < 64)
        strcpy(entity.name, name);
    else
        memcpy(entity.name, name, 63);

    POSITION pos = m_EntityList.AddTail(entity);

    if (m_EntityList.GetCount() == 1) {
        m_SelectedEntity = pos;
        m_OriginX = x;
        m_OriginY = y;
    }

    if (m_AutoSizeArea)
        ResizeGamingArea();
    else
        Invalidate(FALSE);

    return pos;
}

void CSituationDispCtrl::SetEntity(POSITION handle, double x, double y, double heading, BOOL visible)
{
    DISPCTRL_POS_AND_HDG &pos = m_EntityList.GetAt(handle);
    pos.x = x;
    pos.y = y;
    pos.hdg = heading;
    pos.visible = visible;

    if (handle == m_SelectedEntity) {
        m_OriginX = x;
        m_OriginY = y;
    }

    if (m_AutoSizeArea)
        ResizeGamingArea();
    else
        Invalidate(FALSE);
}

void CSituationDispCtrl::SelectEntity(POSITION handle)
{
    m_SelectedEntity = handle;

    m_OriginX = m_EntityList.GetAt(handle).x;
    m_OriginY = m_EntityList.GetAt(handle).y;

    if (m_AutoSizeArea)
        ResizeGamingArea();
    else
        Invalidate(FALSE);
}

void CSituationDispCtrl::RemoveEntity(POSITION handle)
{
    if (handle == m_SelectedEntity)
        m_SelectedEntity = NULL;

    m_EntityList.RemoveAt(handle);

    if (m_AutoSizeArea)
        ResizeGamingArea();
    else
        Invalidate(FALSE);
}

void CSituationDispCtrl::ResizeGamingArea(void)
{
    int n = 0;
    POSITION pos = NULL;
    double cx = 0.0;
    double cy = 0.0;
    double dx = 0.0;
    double dy = 0.0;
    double x = 0.0;
    double y = 0.0;
    double dist = 0;
    DISPCTRL_POS_AND_HDG entity;

    if (m_SelectedEntity == NULL)
        return;

    // Get the position of the selected entity.
    entity = m_EntityList.GetAt(m_SelectedEntity);
    cx = entity.x;
    cy = entity.y;

    // Iterate through the linked list and find the greatest
    // distance from the selected entity.
    pos = m_EntityList.GetHeadPosition();
    while (pos) {
        // Don't test the selected entity against itself.
        if (pos == m_SelectedEntity) {
            m_EntityList.GetNext(pos);
            continue;
        }

        entity = m_EntityList.GetNext(pos);
        if (entity.visible) {
            x = fabs(cx - entity.x);
            y = fabs(cy - entity.y);

            if (x > dx)
                dx = x;

            if (y > dy)
                dy = y;
        }
    }

    // Find the diameter that is equal to the next largest circle.
    dist = sqrt((dx * dx) + (dy * dy));
    n = (int)(dist / m_CircleInterval) + 1;
    m_GamingAreaLength = 2 * n * m_CircleInterval;
    if (m_GamingAreaLength < m_MinGamingAreaLength)
        m_GamingAreaLength = m_MinGamingAreaLength;

    // Force a resize to set m_ActualAreaX and m_ActualAreaY.
    CRect rect;
    GetWindowRect(rect);
    OnSize(SIZE_RESTORED, rect.Width(), rect.Height());
}

void CSituationDispCtrl::SetGamingAreaSize(double min_length, BOOL autosize)
{
    long wnd_x, wnd_y;
    CRect wndrect;

    m_GamingAreaLength = min_length;
    m_MinGamingAreaLength = min_length;
    m_AutoSizeArea = autosize;

    if (GetSafeHwnd()) {
        GetWindowRect(&wndrect);
        wnd_x = wndrect.Width();
        wnd_y = wndrect.Height();

        if (wnd_x < wnd_y) { // tall
            m_ActualAreaX = min_length;
            m_ActualAreaY = min_length * (wnd_y / (double)wnd_x);
        } else { // long
            m_ActualAreaY = min_length;
            m_ActualAreaX = min_length * (wnd_x / (double)wnd_y);
        }

        Invalidate(FALSE);
    }
}

void CSituationDispCtrl::OnPaint()
{
    char buffer[16] = {0};
    int x1, y1, x2, y2;
    int i;
    CPaintDC paintdc(this); // device context for painting
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

    // Set the background color and brush to match.
    memdc.SetBkColor(m_BkColor);
    memdc.SelectObject(brush);

    // Draw the circles (from the outside in, since we have a solid brush).
    memdc.SetTextColor(m_CircleColor);
    CPen circle_pen(PS_SOLID, 1, m_CircleColor);
    CPen *pOldPen = memdc.SelectObject(&circle_pen);
    for (i = m_NumCircles; i > 0; i--) {
        x1 = LogicalToViewportWidth(i * -m_CircleInterval);
        y1 = LogicalToViewportHeight(i * m_CircleInterval);
        x2 = LogicalToViewportWidth(i * m_CircleInterval);
        y2 = LogicalToViewportHeight(i * -m_CircleInterval);
        memdc.Ellipse(x1, y1, x2, y2);

        // Label the circles.
        CRect rect(x1 + ((x2 - x1)) / 2, y2, x2, y2);   // initial rect
        sprintf(buffer, "%5.lfm", m_CircleInterval * i);
        int len = strlen(buffer);

        // Resize and nudge the rect.
        memdc.DrawText(buffer, len, &rect, DT_CALCRECT);
        int dx = rect.Width() / 2;
        rect.left -= (dx + 2);
        rect.right -= (dx + 2);
        rect.top -= 2;
        rect.bottom -= 2;

        // Make the background transparent and draw the text.
        int oldmode = memdc.SetBkMode(TRANSPARENT);
        memdc.DrawText(buffer, len, &rect, DT_LEFT);
        memdc.SetBkMode(oldmode);
    }


    // Next, draw the entities.
    CPen selectpen(PS_SOLID, 1, m_SelectedColor);
    POSITION pos = m_EntityList.GetHeadPosition();
    POSITION currpos = pos;
    while (pos) {
        currpos = pos;
        DISPCTRL_POS_AND_HDG entity = m_EntityList.GetNext(pos);

        if (entity.visible) {
            CPen entitypen(PS_SOLID, 1, entity.rgb);
            x1 = LogicalToViewportX(entity.x);
            y1 = LogicalToViewportY(entity.y);

            if (currpos == m_SelectedEntity)
                pOldPen = memdc.SelectObject(&selectpen);
            else
                pOldPen = memdc.SelectObject(&entitypen);

            memdc.MoveTo(GetPoint1(x1, y1, entity.hdg));
            memdc.LineTo(GetPoint2(x1, y1, entity.hdg));
            memdc.LineTo(GetPoint3(x1, y1, entity.hdg));
        }
    }

    // Finally, copy from the mem DC to the paint DC.  This will eliminate any flicker
    // that would appear if we simply drew to the paint DC.
    paintdc.BitBlt(0, 0, BkgndRect.Width(), BkgndRect.Height(), &memdc, 0, 0, SRCCOPY);

    // Do not call CWnd::OnPaint() for painting messages
}

void CSituationDispCtrl::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    if (cx < cy) {  // tall
        m_ActualAreaX = m_GamingAreaLength;
        m_ActualAreaY = m_GamingAreaLength * (cy / (double)cx);
    } else { // long
        m_ActualAreaY = m_GamingAreaLength;
        m_ActualAreaX = m_GamingAreaLength * (cx / (double)cy);
    }

    Invalidate(FALSE);
}
