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

// RadioList.cpp : implementation file
//

#include "stdafx.h"
#include "hemu4.h"
#include "RadioList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRadioList

CRadioList::CRadioList()
{
    m_BtnCount = 0;
    m_BtnHeight = 0;
    m_WndWidth = 0;
    m_WndHeight = 0;
    m_PageHeight = 0;
    m_BorderHeight = 1;
    m_BorderWidth = 1;
    m_ScrollPos = 0;
    m_LastItem = -1;
}

CRadioList::~CRadioList()
{
    int i;
    for (i = 0; i < m_BtnArray.GetSize(); i++)
        delete m_BtnArray[i];
}

BEGIN_MESSAGE_MAP(CRadioList, CWnd)
    //{{AFX_MSG_MAP(CRadioList)
    ON_WM_CREATE()
    ON_WM_VSCROLL()
    ON_WM_SIZE()
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRadioList message handlers

int CRadioList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    Init();

    return 0;
}

void CRadioList::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    m_WndHeight = cy;
    m_WndWidth = cx;

    m_PageHeight = m_WndHeight - (2 * m_BorderHeight);

    SCROLLINFO si;
    si.fMask = SIF_RANGE | SIF_PAGE;
    si.nMin = 0;
    si.nMax = ((m_BtnCount * m_BtnHeight) + m_BtnHeight - 1 > m_PageHeight) ? m_BtnCount * m_BtnHeight : 0;
    si.nPage = m_PageHeight;

    SetScrollInfo(SB_VERT, &si, TRUE);
}

void CRadioList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
    int delta;

    switch (nSBCode) {
    case SB_LINEUP:
        delta = -m_BtnHeight;
        break;

    case SB_PAGEUP:
        delta = -m_PageHeight / m_BtnHeight;
        break;

    case SB_THUMBTRACK:
        delta = (int)nPos - m_ScrollPos;
        break;

    case SB_PAGEDOWN:
        delta = m_PageHeight / m_BtnHeight;
        break;

    case SB_LINEDOWN:
        delta = m_BtnHeight;
        break;

    default:
        return;
    }

    int scrollpos = m_ScrollPos + delta;
    int max = (m_BtnCount * m_BtnHeight) - m_PageHeight;

    if (scrollpos < 0)
        delta = -m_ScrollPos;
    else if (scrollpos > max)
        delta = max - m_ScrollPos;

    if (delta) {
        m_ScrollPos += delta;
        SetScrollPos(SB_VERT, m_ScrollPos, TRUE);
        ScrollWindow(0, -delta);
    }

    CWnd::OnVScroll(nSBCode, m_ScrollPos, pScrollBar);
}

void CRadioList::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // Erase the background using the same color as the button face.
    CBrush brush;
    brush.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
    CPen pen(PS_SOLID, 0, ::GetSysColor(COLOR_3DFACE));

    dc.SelectObject(&brush);
    dc.SelectObject(&pen);

    CRect rect;
    GetClientRect(&rect);
    dc.Rectangle(&rect);
}

void CRadioList::Init(void)
{
    CClientDC dc(this);
    CSize size = dc.GetWindowExt();
    m_WndWidth = size.cx;
    m_WndHeight = size.cy;

    // Add the WS_VSCROLL and WS_CLIPCHILDREN styles to the window if
    // they arent' there already.
    long style = ::GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
    ::SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, style | WS_VSCROLL | WS_CLIPCHILDREN);
    SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

    // Make the default button height the same as that of a menu bar.
    m_BtnHeight = ::GetSystemMetrics(SM_CYMENU);    // SM_CYCAPTION

    // Save the border width and height to use as offsets when placing buttons.
    m_BorderWidth = ::GetSystemMetrics(SM_CXBORDER);
    m_BorderHeight = ::GetSystemMetrics(SM_CYBORDER);

    // Hide the scroll bar and set the page size.
    CRect clrect;
    GetClientRect(&clrect);
    m_PageHeight = clrect.Height() - (2 * m_BorderHeight);
    SCROLLINFO si;
    si.fMask = SIF_PAGE | SIF_RANGE;
    si.nMin = 0;
    si.nMax = 0;
    si.nPage = m_PageHeight;
    SetScrollInfo(SB_VERT, &si, TRUE);

    // Create the default font (8pt normal MS Sans Serif).
    LOGFONT logfont = { 80,         // height in 10ths of a pt
                        0,          // width -- 0 means find closest
                        0,          // escapement in 10ths of a degree
                        0,          // orientation in 10ths of a degree
                        FW_NORMAL,  // weight
                        FALSE,      // italic
                        FALSE,      // underline
                        FALSE,      // strikeout,
                        ANSI_CHARSET,
                        OUT_DEFAULT_PRECIS,     // default output precision
                        CLIP_DEFAULT_PRECIS,    // default clipping precision
                        DEFAULT_QUALITY,
                        DEFAULT_PITCH | FF_SWISS,
                        "MS Sans Serif"
                      };
    m_Font.CreatePointFontIndirect(&logfont, NULL);

    int i;
    m_BtnArray.SetSize(20, 20);
    for (i = 0; i < 20; i++)
        m_BtnArray[i] = NULL;

    m_ItemData.SetSize(20, 20);
}

int CRadioList::AddItem(LPCTSTR text)
{
    CButton *radio = m_BtnArray[m_BtnCount];

    // Create the CButton object if one hasn't been created already.
    if (radio == NULL) {
        radio = new CButton;
        m_BtnArray.SetAtGrow(m_BtnCount, radio);
    }

    CRect rect;
    rect.top = m_BtnCount * m_BtnHeight;
    rect.bottom = rect.top + m_BtnHeight - 1;
    rect.left = m_BorderWidth;
    rect.right = m_WndWidth - 2 * m_BorderWidth;

    // Include WS_GROUP in the style if this is the first button.
    int style = WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | (m_BtnCount ? 0 : WS_GROUP);

    radio->Create(text, style, rect, this, m_BtnCount + 100);
    radio->SetFont(&m_Font, FALSE);

    m_BtnCount++;

    // Adjust the scroll bar's upper range if necessary.
    if (rect.bottom > m_PageHeight) {
        SCROLLINFO si;
        si.fMask = SIF_RANGE | SIF_PAGE;
        si.nMin = 0;
        si.nMax = m_BtnCount * m_BtnHeight;
        si.nPage = m_PageHeight;

        SetScrollInfo(SB_VERT, &si, TRUE);
    }

    CScrollBar *bar = GetScrollBarCtrl(SB_VERT);

    return m_BtnCount - 1;
}

void CRadioList::Clear(void)
{
    // Destroy the CButton windows but do not delete the objects.
    for (int i = 0; i < m_BtnCount; i++) {
        m_BtnArray[i]->DestroyWindow();
        delete m_BtnArray[i];
        m_BtnArray[i] = NULL;
    }

    m_BtnCount = 0;

    // Hide the scroll bar.
    SCROLLINFO si;
    si.fMask = SIF_RANGE;
    si.nMin = 0;
    si.nMax = 0;
    SetScrollInfo(SB_VERT, &si, TRUE);

    // Unset the "last selected item" latch, so that the next set of items
    // loaded into the radio list will not inherit old state and cause
    // unexpected behavior.
    m_LastItem = -1;
}

int CRadioList::GetCurSel(void)
{
    for (int i = 0; i < m_BtnCount; i++)
        if (m_BtnArray[i]->GetCheck() == 1)
            return i;

    return -1;
}

void CRadioList::SetCurSel(const int item)
{
    if ((item > m_BtnArray.GetUpperBound()) || (item < 0)) {
        for (int i = 0; i <= m_BtnArray.GetUpperBound(); i++)
            m_BtnArray[i]->SetCheck(FALSE);
        return;
    }

    m_BtnArray[item]->SetCheck(TRUE);
    m_LastItem = item;
}

BOOL CRadioList::SetItemData(int item, DWORD data)
{
    if ((item < 0) || (item > m_ItemData.GetUpperBound())) {
        return FALSE;
    } else {
        m_ItemData[item] = data;
        return TRUE;
    }
}

BOOL CRadioList::OnCmdMsg(UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo)
{
    // If the button has changed, post a message to the parent.
    // The message will look the same as the CListBox message.
    int item = GetCurSel();
    if (item != m_LastItem) {
        GetParent()->SendMessage(RL_SELCHANGED, (WPARAM)item, (LPARAM)m_hWnd);
        m_LastItem = item;
    }

    return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
