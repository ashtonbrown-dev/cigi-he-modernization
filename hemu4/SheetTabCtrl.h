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

#if !defined(AFX_SHEETTABCTRL_H__6E61A73E_BBC0_4252_A608_27CBF593137A__INCLUDED_)
#define AFX_SHEETTABCTRL_H__6E61A73E_BBC0_4252_A608_27CBF593137A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SheetTabCtrl.h : header file
//

#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// CSheetTabCtrl window

class CSheetTabCtrl : public CTabCtrl
{
public:
    CSheetTabCtrl();
    virtual ~CSheetTabCtrl();
    void SetBkgndColor(COLORREF color);
    void SetSheetColor(COLORREF color);
    void SetTabColor(COLORREF color);
    void SetHilightColor(COLORREF color);
    void SetShadowColor(COLORREF color);

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSheetTabCtrl)
    //}}AFX_VIRTUAL

protected:
    COLORREF m_BkgndColor;
    COLORREF m_SheetColor;
    COLORREF m_TabColor;
    COLORREF m_HilightColor;
    COLORREF m_ShadowColor;

    CSize FormatText(CString &sText, CDC *pDC, int nWidth);
    virtual void DrawMainBorder(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void DrawItemBorder(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void PreSubclassWindow();

    //{{AFX_MSG(CSheetTabCtrl)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    afx_msg BOOL OnEraseBkgnd(CDC *pDC);
    afx_msg void OnPaint();

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETTABCTRL_H__6E61A73E_BBC0_4252_A608_27CBF593137A__INCLUDED_)
