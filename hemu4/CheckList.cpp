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
 *  FILENAME:   CheckList.cpp
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
#include "hemu4.h"
#include "CheckList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CCheckList, CWnd)
    //{{AFX_MSG_MAP(CCheckList)
    ON_WM_PAINT()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCheckList::CCheckList()
    : CRadioList()
{
}

CCheckList::~CCheckList()
{
}

void CCheckList::Init(void)
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

    m_BtnArray.SetSize(20, 20);
    for (int i = 0; i < 20; i++)
        m_BtnArray[i] = NULL;

    m_ItemData.SetSize(20, 20);
}

int CCheckList::AddItem(LPCTSTR text)
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
    int style = WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX;    // | (m_BtnCount ? 0 : WS_GROUP);

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

void CCheckList::OnPaint()
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

void CCheckList::OnSize(UINT nType, int cx, int cy)
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
