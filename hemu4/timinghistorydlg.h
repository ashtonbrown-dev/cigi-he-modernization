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

#if !defined(AFX_TIMINGHISTORYDLG1_H__4E308DDD_7405_4713_9B1D_5FD1F525D34A__INCLUDED_)
#define AFX_TIMINGHISTORYDLG1_H__4E308DDD_7405_4713_9B1D_5FD1F525D34A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimingHistoryDlg1.h : header file
//

#include "HistoryGraph.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTimingHistoryDlg dialog

class CTimingHistoryDlg : public CDialog
{
public:
    unsigned int m_Threshold;

    CTimingHistoryDlg(CWnd *pParent = NULL);   // standard constructor
    void OnOK();
    void OnCancel();
    void SetLineValue(const int index, const int value);
    void Scroll();
    void RecalcLimits(void);
    void PositionMedianLabel(void);

    //{{AFX_DATA(CTimingHistoryDlg)
    enum { IDD = IDD_DIALOG_HISTORY };
    CStatic m_WidthRef;
    CScrollBar  m_ScrollBar;
    CStatic m_Min;
    CStatic m_Median;
    CStatic m_Max;
    CHistoryGraph   m_Graph;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTimingHistoryDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

protected:
    int m_ScrollPos;

    int FindTargetFrameRate(const int actual_rate);

    // Generated message map functions
    //{{AFX_MSG(CTimingHistoryDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnClose();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline void CTimingHistoryDlg::SetLineValue(const int index, const int value)
{
    if (m_Graph.GetSafeHwnd())
        m_Graph.SetLineValue(index, value);
}

inline void CTimingHistoryDlg::Scroll()
{
    if (m_Graph.GetSafeHwnd())
        m_Graph.Scroll();
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMINGHISTORYDLG1_H__4E308DDD_7405_4713_9B1D_5FD1F525D34A__INCLUDED_)
