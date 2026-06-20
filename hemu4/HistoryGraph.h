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

#if !defined(AFX_HISTORYGRAPH_H__5A29BE95_E1C0_4C0F_A3E2_71DBE21F7E52__INCLUDED_)
#define AFX_HISTORYGRAPH_H__5A29BE95_E1C0_4C0F_A3E2_71DBE21F7E52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistoryGraph.h : header file
//

#include "afxtempl.h"

/////////////////////////////////////////////////////////////////////////////
// CHistoryGraph window

class CHistoryGraph : public CWnd
{
public:
    CHistoryGraph();
    virtual ~CHistoryGraph();
    void ForceSize(void);                   // Forces OnSize() to be called.
    BOOL SetNumPoints(const unsigned int num);
    void SetBkColor(const COLORREF rgb);
    COLORREF GetBkColor(void);
    void SetMedianColor(const COLORREF rgb);
    COLORREF GetMedianColor(void);
    void SetLineColor(const unsigned int id, const COLORREF rgb);
    COLORREF GetLineColor(const unsigned int id);
    unsigned int AddLine(const COLORREF rgb);
    void RemoveLines(void);
    void Clear(void);
    void SetMedian(const int median);
    int GetMedian(void);
    void SetMax(const int max);
    int GetMax(void);
    void SetMin(const int min);
    int GetMin(void);
    void SetLineValue(const int index, const int value);
    void Scroll(void);
    int GetCurrentYPos(const int linenum);
    int GetAvgYPos(const int linenum);

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHistoryGraph)
public:
    virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext = NULL);
    //}}AFX_VIRTUAL

protected:
    int m_Median;
    int m_MinValue;
    int m_MaxValue;
    unsigned int m_LineSegCount;
    unsigned int m_AllocSize;
    unsigned int m_ArrayItemIdx;
    COLORREF m_BkColor;
    COLORREF m_MedianColor;
    float m_UnitsPerPixelY;
    float m_PixelsPerSegmentX;
    CDWordArray m_LineColors;
    CPtrArray m_LineData;

    void DrawSegment(CDC *dc, int x1, int y1, int x2, int y2, COLORREF rgb);

    //{{AFX_MSG(CHistoryGraph)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline void CHistoryGraph::ForceSize(void)
{
    if (!GetSafeHwnd())
        return;

    CRect rect;
    GetClientRect(&rect);
    OnSize(SIZE_RESTORED, rect.right, rect.bottom);
}

inline void CHistoryGraph::SetBkColor(const COLORREF rgb)
{
    m_BkColor = rgb;
}

inline COLORREF CHistoryGraph::GetBkColor(void)
{
    return m_BkColor;
}

inline void CHistoryGraph::SetMedianColor(const COLORREF rgb)
{
    m_MedianColor = rgb;
}

inline COLORREF CHistoryGraph::GetMedianColor(void)
{
    return m_MedianColor;
}

inline void CHistoryGraph::SetLineColor(const unsigned int id, const COLORREF rgb)
{
    m_LineColors.SetAtGrow(id, (DWORD)rgb);
}

inline COLORREF CHistoryGraph::GetLineColor(const unsigned int id)
{
    return (COLORREF)m_LineColors.GetAt(id);
}

inline void CHistoryGraph::RemoveLines(void)
{
    m_LineColors.RemoveAll();
}

inline void CHistoryGraph::SetMax(const int max)
{
    m_MaxValue = max;
    m_Median = max - ((max - m_MinValue) / 2);
    ForceSize();
}

inline int CHistoryGraph::GetMax(void)
{
    return m_MaxValue;
}

inline void CHistoryGraph::SetMin(const int min)
{
    m_MinValue = min;
    m_Median = m_MaxValue - ((m_MaxValue - min) / 2);
    ForceSize();
}

inline int CHistoryGraph::GetMin(void)
{
    return m_MinValue;
}

inline void CHistoryGraph::SetLineValue(const int index, const int value)
{
    ((int *)m_LineData[index])[m_ArrayItemIdx] = value;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYGRAPH_H__5A29BE95_E1C0_4C0F_A3E2_71DBE21F7E52__INCLUDED_)
