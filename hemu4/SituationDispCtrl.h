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

#if !defined(AFX_SITUATIONDISPCTRL_H__CE4F497C_CE7D_4DA8_BD4A_8176FA6A75A8__INCLUDED_)
#define AFX_SITUATIONDISPCTRL_H__CE4F497C_CE7D_4DA8_BD4A_8176FA6A75A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SituationDispCtrl.h : header file
//

#include <math.h>

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

// Define a message that will get sent to the parent window when the
// item is changed.
#define SDC_PICK        (WM_APP + 3)

/////////////////////////////////////////////////////////////////////////////
// CTooltipWnd window
class CTooltipWnd : public CWnd
{
public:
    CTooltipWnd();
    virtual BOOL Create(CWnd *pParentWnd, DWORD dwStyle = 0);
    void SetOffset(const int cx, const int cy);
    void Popup(const int cx, const int cy);
    void Popup(const int cx, const int cy, LPCTSTR lpszString);
    void Hide(void);

protected:
    int m_XOffset;
    int m_YOffset;
    int m_Width;
    int m_Height;
    CString m_Text;

    int GetNumTextLines(void);
    CString GetLongestTextLine(CString &string);
    void CalcWindowSize(CDC *pDC);

    afx_msg void OnPaint();

    DECLARE_MESSAGE_MAP()
};

inline void CTooltipWnd::SetOffset(const int cx, const int cy)
{
    m_XOffset = cx;
    m_YOffset = cy;
}

inline void CTooltipWnd::Popup(const int cx, const int cy, LPCTSTR lpszString)
{
    m_Text = lpszString;
    Popup(cx, cy);
}

/////////////////////////////////////////////////////////////////////////////
// CSituationDispCtrl window

class CSituationDispCtrl : public CWnd
{
    // Construction
public:
    struct DISPCTRL_POS_AND_HDG {
        double x;
        double y;
        double hdg;
        COLORREF rgb;
        BOOL visible;
        char name[64];
    };

    CSituationDispCtrl();
    virtual ~CSituationDispCtrl();
    void InitDlgControl(CWnd *pParent);
    void SetTooltipOffset(const int cx, const int cy);
    POSITION AddEntity(double x, double y, double heading, COLORREF rgb, const char *name);
    void SetEntity(POSITION handle, double x, double y, double heading, BOOL visible);
    void RemoveEntity(POSITION handle);
    void RemoveAllEntities(void);
    void SelectEntity(POSITION handle);
    void SetEntityVisible(POSITION handle, BOOL visible);
    BOOL GetEntityVisible(POSITION handle);
    void SetEntityColor(POSITION handle, COLORREF rgb);
    COLORREF GetEntityColor(POSITION handle);
    void SetBkgndColor(COLORREF rgb);
    COLORREF GetBkgndColor(void);
    void SetCircleColor(COLORREF rgb);
    COLORREF GetCircleColor(void);
    void SetSelectColor(COLORREF rgb);
    COLORREF GetSelectColor(void);
    void SetCircleInterval(double dr);
    double GetCircleInterval(void);
    void SetGamingAreaSize(double min_length, BOOL autosize = FALSE);
    double GetGamingAreaSize(void);
    void GetActualGamingAreaSize(double *x, double *y);
    void ResizeGamingArea(void);

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSituationDispCtrl)
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT &cs);
    //}}AFX_VIRTUAL

protected:
    BOOL m_AutoSizeArea;
    int m_EntityCount;
    int m_NumCircles;
    double m_CircleInterval;
    double m_GamingAreaLength;
    double m_MinGamingAreaLength;
    double m_ActualAreaX;
    double m_ActualAreaY;
    double m_OriginX;
    double m_OriginY;
    POSITION m_SelectedEntity;
    COLORREF m_BkColor;
    COLORREF m_CircleColor;
    COLORREF m_SelectedColor;
    CTooltipWnd m_Tooltip;
    CList<DISPCTRL_POS_AND_HDG, DISPCTRL_POS_AND_HDG &> m_EntityList;

    void DisplayPopup(CPoint point, CString text);
    void HidePopup(void);
    double ViewportToLogicalX(double x);
    double ViewportToLogicalY(double y);
    int LogicalToViewportX(double x);
    int LogicalToViewportY(double y);
    int LogicalToViewportWidth(double x);
    int LogicalToViewportHeight(double y);
    POINT GetPoint1(int x, int y, double heading);
    POINT GetPoint2(int x, int y, double heading);
    POINT GetPoint3(int x, int y, double heading);

    //{{AFX_MSG(CSituationDispCtrl)
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline void CSituationDispCtrl::RemoveAllEntities(void)
{
    m_EntityList.RemoveAll();
    m_EntityCount = 0;
}

inline void CSituationDispCtrl::SetTooltipOffset(const int cx, const int cy)
{
    m_Tooltip.SetOffset(cx, cy);
}

inline void CSituationDispCtrl::SetEntityVisible(POSITION handle, BOOL visible)
{
    m_EntityList.GetAt(handle).visible = visible;
    ResizeGamingArea();
}

inline BOOL CSituationDispCtrl::GetEntityVisible(POSITION handle)
{
    return m_EntityList.GetAt(handle).visible;
}

inline void CSituationDispCtrl::SetEntityColor(POSITION handle, COLORREF rgb)
{
    m_EntityList.GetAt(handle).rgb = rgb;
    Invalidate(FALSE);
}

inline COLORREF CSituationDispCtrl::GetEntityColor(POSITION handle)
{
    return m_EntityList.GetAt(handle).rgb;
}

inline void CSituationDispCtrl::SetBkgndColor(COLORREF rgb)
{
    m_BkColor = rgb;

    if (GetSafeHwnd())
        Invalidate(FALSE);
}

inline COLORREF CSituationDispCtrl::GetBkgndColor(void)
{
    return m_BkColor;
}

inline void CSituationDispCtrl::SetCircleColor(COLORREF rgb)
{
    m_CircleColor = rgb;

    if (GetSafeHwnd())
        Invalidate(FALSE);
}

inline COLORREF CSituationDispCtrl::GetCircleColor(void)
{
    return m_CircleColor;
}

inline void CSituationDispCtrl::SetSelectColor(COLORREF rgb)
{
    m_SelectedColor = rgb;

    if (GetSafeHwnd())
        Invalidate(FALSE);
}

inline COLORREF CSituationDispCtrl::GetSelectColor(void)
{
    return m_SelectedColor;
}

inline void CSituationDispCtrl::SetCircleInterval(double dr)
{
    if (dr > 0)
        m_CircleInterval = dr;


    if (GetSafeHwnd())
        Invalidate(FALSE);
}

inline double CSituationDispCtrl::GetCircleInterval(void)
{
    return m_CircleInterval;
}

inline double CSituationDispCtrl::GetGamingAreaSize(void)
{
    return m_GamingAreaLength;
}

inline void CSituationDispCtrl::GetActualGamingAreaSize(double *x, double *y)
{
    *x = m_ActualAreaX;
    *y = m_ActualAreaY;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SITUATIONDISPCTRL_H__CE4F497C_CE7D_4DA8_BD4A_8176FA6A75A8__INCLUDED_)
