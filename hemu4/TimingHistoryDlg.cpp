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

// TimingHistoryDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "TimingHistoryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimingHistoryDlg dialog


CTimingHistoryDlg::CTimingHistoryDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CTimingHistoryDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTimingHistoryDlg)
    //}}AFX_DATA_INIT

    m_Threshold = 50;
    m_ScrollPos = 5;
}

void CTimingHistoryDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTimingHistoryDlg)
    DDX_Control(pDX, IDC_WIDTH_REFERENCE, m_WidthRef);
    DDX_Control(pDX, IDC_SCROLLBAR1, m_ScrollBar);
    DDX_Control(pDX, IDC_MIN_PERIOD, m_Min);
    DDX_Control(pDX, IDC_MEDIAN, m_Median);
    DDX_Control(pDX, IDC_MAX_PERIOD, m_Max);
    DDX_Control(pDX, IDC_GRAPH, m_Graph);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTimingHistoryDlg, CDialog)
    //{{AFX_MSG_MAP(CTimingHistoryDlg)
    ON_WM_SIZE()
    ON_WM_CLOSE()
    ON_WM_VSCROLL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimingHistoryDlg message handlers

BOOL CTimingHistoryDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_Max.SetWindowText("");
    m_Min.SetWindowText("");
    m_Median.SetWindowText("0ms");

    m_Graph.SetNumPoints(100);
    m_Graph.SetMax(int((1 / (double)g_FrameRate) * 10000) + 50);
    m_Graph.SetMin(int((1 / (double)g_FrameRate) * 10000) - 50);
    m_Graph.AddLine(RGB(127, 127, 255));        // sample max
    m_Graph.AddLine(RGB(127, 127, 255));        // sample min
    m_Graph.AddLine(RGB(0, 255, 0));            // sample avg

    RecalcLimits();

    m_ScrollBar.SetScrollRange(1, 20, FALSE);
    m_ScrollBar.SetScrollPos(m_ScrollPos, TRUE);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CTimingHistoryDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if (m_Graph.GetSafeHwnd()) {
        Invalidate(TRUE);

        // Position the scroll bar.
        CRect scrollrect;
        m_ScrollBar.GetWindowRect(&scrollrect);
        ScreenToClient(&scrollrect);
        m_ScrollBar.SetWindowPos(NULL, cx - scrollrect.Width(), 0,
                                 scrollrect.Width(), cy, SWP_NOZORDER);
        // Get the new position.
        m_ScrollBar.GetWindowRect(&scrollrect);
        ScreenToClient(&scrollrect);

        // Position our CStatic to use as a reference for the labels.
        CRect rect_ref;
        m_WidthRef.GetWindowRect(&rect_ref);
        ScreenToClient(&rect_ref);
        int w = rect_ref.Width();
        int h = rect_ref.Height();
        rect_ref.left = scrollrect.left - w;
        rect_ref.right = scrollrect.left;   //cx;
        rect_ref.top = (cy - h) / 2;
        rect_ref.bottom = rect_ref.top + h;
        m_WidthRef.MoveWindow(&rect_ref);

        // Position the time labels.
        m_Median.SetWindowPos(NULL, rect_ref.left + 3, rect_ref.top,
                              rect_ref.Width() - 3, rect_ref.Height(),
                              SWP_NOZORDER);

        m_Max.SetWindowPos(NULL, rect_ref.left + 3, 0,
                           rect_ref.Width() - 3, rect_ref.Height(),
                           SWP_NOZORDER);

        m_Min.SetWindowPos(NULL, rect_ref.left + 3, cy - h,
                           rect_ref.Width() - 3, rect_ref.Height(),
                           SWP_NOZORDER);

        // Position the graph window.
        CRect graph_rect(0, 0, rect_ref.left, cy);
        m_Graph.MoveWindow(&graph_rect, FALSE);
    }
}

void CTimingHistoryDlg::OnOK()
{
}

// User clicks "X" button or presses ESC.
void CTimingHistoryDlg::OnCancel()
{
    m_Graph.Clear();

    CDialog::OnCancel();
}

void CTimingHistoryDlg::OnClose()
{
    m_Graph.Clear();

    CDialog::OnClose();
}

void CTimingHistoryDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
    int delta;

    switch (nSBCode) {
    case SB_LINEUP:
    case SB_PAGEUP:
        delta = -1;
        break;

    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        delta = (int)nPos - m_ScrollPos;
        break;

    case SB_PAGEDOWN:
    case SB_LINEDOWN:
        delta = 1;
        break;

    case SB_BOTTOM:
        if (m_ScrollPos == 0)
            return;

    case SB_TOP:
        if (m_ScrollPos == m_ScrollBar.GetScrollLimit())
            return;

    case SB_ENDSCROLL:
    default:
        return;
    }

    double target_fr = (double)FindTargetFrameRate(g_AvgFrameRate);
    double T_10msec = (1 / target_fr) * 10000;

    if ((m_ScrollPos + delta <= 0)  || (m_ScrollPos + delta >= m_ScrollBar.GetScrollLimit()))
        return;

    // Add the proposed change to the current values.
    m_ScrollPos += delta;
    m_Threshold += (delta * 10);

    // Set the graph's range.
    int min = int(T_10msec) - m_Threshold;
    int max = int(T_10msec) + m_Threshold;
    if (min < 0) {
        max = 2 * int(T_10msec);
        min = 0;
    }
    m_Graph.SetMax(max);
    m_Graph.SetMin(min);

    // Set the min, max, and median labels.
    CString txt;
    txt.Format("%.1fms", (min + ((max - min) / 2)) / 10.0);
    m_Median.SetWindowText((LPCTSTR)txt);
    txt.Format("%.1fms", max / 10.0);
    m_Max.SetWindowText((LPCTSTR)txt);
    txt.Format("%.1fms", min / 10.0);
    m_Min.SetWindowText((LPCTSTR)txt);

    pScrollBar->SetScrollPos(m_ScrollPos, TRUE);

    CDialog::OnVScroll(nSBCode, m_ScrollPos, pScrollBar);
}

void CTimingHistoryDlg::RecalcLimits(void)
{
    if (g_FrameRate == 0.0f)
        return;

    double target_fr = (double)FindTargetFrameRate(g_AvgFrameRate);

    double T_10msec = (1 / target_fr) * 10000;
    int min = int(T_10msec) - m_Threshold;
    int max = int(T_10msec) + m_Threshold;
    if (min < 0) {
        max = 2 * int(T_10msec);
        min = 0;
    }
    m_Graph.SetMax(max);
    m_Graph.SetMin(min);

    CString txt;
    txt.Format("%.1fms", (min + ((max - min) / 2)) / 10.0);
    m_Median.SetWindowText((LPCTSTR)txt);
    txt.Format("%.1fms", max / 10.0);
    m_Max.SetWindowText((LPCTSTR)txt);
    txt.Format("%.1fms", min / 10.0);
    m_Min.SetWindowText((LPCTSTR)txt);
}

int CTimingHistoryDlg::FindTargetFrameRate(const int actual_rate)
{
    if (actual_rate <= 20)
        return actual_rate;

    if (actual_rate <= 23)
        return 20;

    if (actual_rate <= 27)
        return 25;

    if (actual_rate <= 37)
        return 30;

    if (actual_rate <= 44)
        return 40;

    if (actual_rate <= 54)
        return 50;

    if (actual_rate <= 70)
        return 60;

    if (actual_rate <= 78)
        return 75;

    if (actual_rate <= 88)
        return 80;

    if (actual_rate <= 110)
        return 100;

    if (actual_rate <= 130)
        return 120;

    if (actual_rate <= 180)
        return 150;

    if (actual_rate <= 220)
        return 200;

    return actual_rate;
}

void CTimingHistoryDlg::PositionMedianLabel(void)
{
    // Get the vertical position (in pixels) of the "avg" line.
    int ypos = m_Graph.GetAvgYPos(2);

    // Get the graph's position and height (both in pixels and logical coords).
    CRect graphrect;
    m_Median.GetWindowRect(&graphrect);
    int height_pixels = graphrect.Height();
    this->ScreenToClient(&graphrect);
    int height_logical = graphrect.Height();

    // Get the label's position.
    CRect textrect;
    m_Median.GetWindowRect(&textrect);
    this->ScreenToClient(&textrect);

    // Calculate the new logical vertical position of the text.
    int new_y = (int)(ypos * height_logical / (float)height_pixels);
    new_y -= (int)(textrect.Height() / 2.0);

    // Set the new position.
    m_Median.SetWindowPos(NULL, textrect.left, new_y, textrect.Width(),
                          textrect.Height(), SWP_NOZORDER | SWP_NOSIZE);
}
