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

#if !defined(AFX_JOYWND_H__A95C152F_B3EC_11D4_BB9A_00609419F883__INCLUDED_)
#define AFX_JOYWND_H__A95C152F_B3EC_11D4_BB9A_00609419F883__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JoyWnd.h : header file
//

#include <afxwin.h>         // MFC core and standard components

// Define a message that will get sent to the parent window when the
// value is changed.
#define WM_CHANGEJOYPOS     (WM_APP + 1)

/////////////////////////////////////////////////////////////////////////////
// CJoyWnd window

class CJoyWnd : public CWnd
{
public:
    CJoyWnd();
    virtual ~CJoyWnd();
    void Init(void);
    void AutoCenter(const BOOL state);
    void CenterBall(void);
    void SetBackColor(const COLORREF color);
    void SetBallColor(const COLORREF color);
    COLORREF GetBackColor(void);
    COLORREF GetBallColor(void);
    long GetPosX(void);
    long GetPosY(void);
    void SetPosX(const long x);
    void SetPosY(const long y);
    long GetMaxX(void);
    long GetMaxY(void);
    void SetMaxX(long x);
    void SetMaxY(long y);

    //{{AFX_VIRTUAL(CJoyWnd)
public:
    virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext = NULL);
    //}}AFX_VIRTUAL

protected:
    //{{AFX_MSG(CJoyWnd)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnCaptureChanged(CWnd *pWnd);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG

    long CalcLogicalX(const long wnd_x);
    long CalcLogicalY(const long wnd_y);
    long CalcWindowX(const long x);
    long CalcWindowY(const long y);
    void DrawBall(void);

    BOOL m_AutoCenter;          // Joystick "snaps" back to center
    long m_BorderSize;          // Border width/height in pixels
    long m_BallRadiusPixels;
    long m_MinX;                // Logical units
    long m_MaxX;                // Logical units
    long m_MinY;                // Logical units
    long m_MaxY;                // Logical units
    long m_PosX;                // Logical units
    long m_PosY;                // Logical units
    CPoint m_BallPos;           // Window coordinates
    double m_BallRadiusRatio;
    CRect m_BallMBR;
    CRect m_WindowRect;
    COLORREF m_BkColor;
    COLORREF m_BallColor;
    CBrush m_BkBrush;
    CBrush m_BallBrush;

    DECLARE_MESSAGE_MAP()
};

inline void CJoyWnd::AutoCenter(const BOOL state = TRUE)
{
    m_AutoCenter = (BOOL)state;
}

inline void CJoyWnd::SetBackColor(const COLORREF color)
{
    m_BkColor = color;
    Invalidate();
}

inline void CJoyWnd::SetBallColor(const COLORREF color)
{
    m_BallColor = color;
    Invalidate();
}

inline COLORREF CJoyWnd::GetBackColor(void)
{
    return m_BkColor;
}

inline COLORREF CJoyWnd::GetBallColor(void)
{
    return m_BallColor;
}

inline long CJoyWnd::GetPosX(void)
{
    return m_PosX;
}

inline long CJoyWnd::GetPosY(void)
{
    return m_PosY;
}

inline long CJoyWnd::GetMaxX(void)
{
    return m_MaxX;
}

inline long CJoyWnd::GetMaxY(void)
{
    return m_MaxY;
}

inline long CJoyWnd::CalcLogicalX(const long wnd_x)
{
    // Not accounting for radius of ball:
    //      return (long)((2 * m_MaxX * wnd_x) / (double)m_WindowRect.right) - m_MaxX;
    //
    // Not accounting for border:
    //      return (long)((2 * m_MaxX * (wnd_x - m_BallRadiusPixels))
    //          / (double)(m_WindowRect.Width() - m_BallRadiusPixels - m_BallRadiusPixels))
    //          - m_MaxX;

    return (long)((wnd_x - (m_WindowRect.Width() / 2.0f)) *
                  ((2 * m_MaxX) / (double)(m_WindowRect.Width() - (2 * (m_BallRadiusPixels + m_BorderSize)))));
}

inline long CJoyWnd::CalcLogicalY(const long wnd_y)
{
    // Not accounting for radius of ball:
    //      return -((long)((2 * m_MaxY * wnd_y) / (double)m_WindowRect.bottom) - m_MaxY);
    //
    // Not accounting for border:
    //      return -((long)((2 * m_MaxY * (wnd_y - m_BallRadiusPixels))
    //              / (double)(m_WindowRect.Height() - m_BallRadiusPixels - m_BallRadiusPixels))
    //              - m_MaxY);

    return (long)(((m_WindowRect.Height() / 2.0f) - wnd_y) *
                  ((2 * m_MaxY) / (double)(m_WindowRect.Height() - (2 * (m_BallRadiusPixels + m_BorderSize)))));
}

inline long CJoyWnd::CalcWindowX(const long x)
{
    // Not accounting for radius of ball:
    //      return (long)((x + m_MaxX) * 0.5 * (m_WindowRect.right / (double)m_MaxX));
    //
    // Not accounting for border:
    //      return (long)(((x + m_MaxX) * (m_WindowRect.Width())) / (2.0f * m_MaxX)) + m_BallRadiusPixels;

    return (long)((x * (m_WindowRect.Width() - (2 * (m_BallRadiusPixels + m_BorderSize)))
                   / (double)(2 * m_MaxX)) + (m_WindowRect.Width() / 2.0f));
}

inline long CJoyWnd::CalcWindowY(const long y)
{
    // Not accounting for radius of ball:
    //      return -((long)((y + m_MaxY) * 0.5 * (m_WindowRect.bottom / (double)m_MaxY)));

    // Not accounting for border:
    //      return (long)(((y + m_MaxY) * (m_WindowRect.Height())) / (-2.0f * m_MaxY)) + m_BallRadiusPixels;

    return (long)((y * ((2 * (m_BallRadiusPixels + m_BorderSize)) - m_WindowRect.Height())
                   / (double)(2 * m_MaxY)) + (m_WindowRect.Height() / 2.0f));
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOYWND_H__A95C152F_B3EC_11D4_BB9A_00609419F883__INCLUDED_)
