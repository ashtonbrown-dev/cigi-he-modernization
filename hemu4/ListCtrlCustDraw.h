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

#if !defined(AFX_LISTCTRLEX_H__41C162E3_99EF_4F4D_9DC1_8C3FB5FDA0B0__INCLUDED_)
#define AFX_LISTCTRLEX_H__41C162E3_99EF_4F4D_9DC1_8C3FB5FDA0B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListCtrlCustDraw window

class CListCtrlCustDraw : public CListCtrl
{
public:
    CListCtrlCustDraw();
    virtual ~CListCtrlCustDraw();
    void SetFullRowSelect(BOOL enable);
    BOOL GetFullRowSelect(void);
    void EnableColumnLines(BOOL enable = TRUE);
    BOOL ColumnLinesAreEnabled(void);
    void SetColumnLineStyle(int style);
    int GetColumnLineStyle(void);
    void SetColumnLineColor(COLORREF rgb);
    COLORREF GetColumnLineColor(void);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CListCtrlCustDraw)
    //}}AFX_VIRTUAL

protected:
    int             m_ColumnLineStyle;
    COLORREF        m_ColumnLineColor;
    BOOL            m_FullRowSelect;
    BOOL            m_DrawColumnLines;
    CFont           *m_pOldItemFont;
    CFont           *m_pOldSubItemFont;

    //////////////////////////////////////////////////////////
    // Overrideable Virtual Functions:

    // Callbacks for the control:

    // Specifies whether we want to do the drawing ourselves.
    virtual BOOL IsDraw() {
        return (m_DrawColumnLines ? TRUE : FALSE);
    }

    // Place custom drawing code here and return TRUE if we did do the
    // drawing ourselves.
    virtual BOOL OnDraw(CDC *pDC, const CRect &rect) {
        return FALSE;
    }

    // Specifies whether we want to handle custom draw for individual items.
    virtual BOOL IsNotifyItemDraw() {
        return FALSE;
    }

    // Specifies whether we want to be notified when the painting has finished.
    virtual BOOL IsNotifyPostPaint() {
        return FALSE;
    }

    // Specifies whether we want to do any drawing after the list control is
    // finished.
    virtual BOOL IsPostDraw() {
        return FALSE;
    }

    // Place post-drawing drawing code here.  The return value is not used.
    virtual BOOL OnPostDraw(CDC *pDC, const CRect &rect) {
        return FALSE;
    }

    // Callbacks for each item:

    // Return a pointer to the font to use for this item, or NULL for the
    // default font.
    virtual CFont *FontForItem(int nItem, UINT nState, LPARAM lParam) {
        return NULL;
    }

    // Return the text color to use for this item, or CLR_DEFAULT for default.
    virtual COLORREF TextColorForItem(int nItem, UINT nState, LPARAM lParam) {
        return CLR_DEFAULT;
    }

    // Return the background color to use for this item, or CLR_DEFAULT for
    // the default.
    virtual COLORREF BkColorForItem(int nItem, UINT nState, LPARAM lParam) {
        return CLR_DEFAULT;
    }

    // Specify whether we want to do the drawing for this item ourselves.
    virtual BOOL IsItemDraw(int nItem, UINT nState, LPARAM lParam) {
        return FALSE;
    }

    // Place item-specific drawing code here and return TRUE if we did do
    // all the drawing ourselves
    virtual BOOL OnItemDraw(CDC *pDC, int nItem, UINT nState, LPARAM lParam) {
        return FALSE;
    }

    // Specify whether we want to handle custom draw for individual sub items.
    virtual BOOL IsNotifySubItemDraw(int nItem, UINT nState, LPARAM lParam) {
        return FALSE;
    }

    // Specify whether we want to be notified when the painting has finished.
    virtual BOOL IsNotifyItemPostPaint(int nItem, UINT nState, LPARAM lParam) {
        return FALSE;
    }

    // Specify whether we want to do any drawing after the list control
    // drawing is finished.
    virtual BOOL IsItemPostDraw() {
        return FALSE;
    }

    // Place item post-drawing drawind code here.  The return value is not
    // used.
    virtual BOOL OnItemPostDraw(CDC *pDC, int nItem, UINT nState,
                                LPARAM lParam) {
        return FALSE;
    }

    // Callbacks for each sub item

    // Return a pointer to the font to use for this sub item, or NULL for the
    // default font.
    virtual CFont *FontForSubItem(int nItem, int nSubItem, UINT nState,
                                  LPARAM lParam) {
        return NULL;
    }

    // Return the text color to use for this sub item, or CLR_DEFAULT for
    // the default.
    virtual COLORREF TextColorForSubItem(int nItem, int nSubItem, UINT nState,
                                         LPARAM lParam) {
        return CLR_DEFAULT;
    }

    // Return the background color to use for this sub item, or CLR_DEFAULT
    // to use the default
    virtual COLORREF BkColorForSubItem(int nItem, int nSubItem, UINT nState,
                                       LPARAM lParam) {
        return CLR_DEFAULT;
    }

    // Specify whether we want to do the drawing for this sub item ourselves.
    virtual BOOL IsSubItemDraw(int nItem, int nSubItem, UINT nState,
                               LPARAM lParam) {
        return FALSE;
    }

    // Place custom drawing code here.  Return TRUE if we did do all the
    // drawing ourselves.
    virtual BOOL OnSubItemDraw(CDC *pDC, int nItem, int nSubItem, UINT nState,
                               LPARAM lParam) {
        return FALSE;
    }

    // Specify whether we want to be notified when the painting has finished.
    virtual BOOL IsNotifySubItemPostPaint(int nItem, int nSubItem, UINT nState,
                                          LPARAM lParam) {
        return FALSE;
    }

    // Specify whether we want to do any drawing after the list control is
    // finished.
    virtual BOOL IsSubItemPostDraw() {
        return FALSE;
    }

    // Place subitem post-drawing drawing code here.  The return value is not
    // used .
    virtual BOOL OnSubItemPostDraw(CDC *pDC, int nItem, int nSubItem,
                                   UINT nState, LPARAM lParam) {
        return FALSE;
    }

    //{{AFX_MSG(CListCtrlCustDraw)
    afx_msg void OnPaint();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    afx_msg void OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

inline void CListCtrlCustDraw::SetFullRowSelect(BOOL enable)
{
    m_FullRowSelect = enable;

    SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT,
                (LPARAM)(m_FullRowSelect ? LVS_EX_FULLROWSELECT : 0));
}

inline BOOL CListCtrlCustDraw::GetFullRowSelect(void)
{
    return m_FullRowSelect;
}

inline void CListCtrlCustDraw::EnableColumnLines(BOOL enable)
{
    m_DrawColumnLines = enable;
    UpdateWindow();
}

inline BOOL CListCtrlCustDraw::ColumnLinesAreEnabled(void)
{
    return m_DrawColumnLines;
}

inline void CListCtrlCustDraw::SetColumnLineStyle(int style)
{
    m_ColumnLineStyle = style;
    UpdateWindow();
}

inline int CListCtrlCustDraw::GetColumnLineStyle(void)
{
    return m_ColumnLineStyle;
}

inline void CListCtrlCustDraw::SetColumnLineColor(COLORREF rgb)
{
    m_ColumnLineColor = rgb;
    UpdateWindow();
}

inline COLORREF CListCtrlCustDraw::GetColumnLineColor(void)
{
    return m_ColumnLineColor;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEX_H__41C162E3_99EF_4F4D_9DC1_8C3FB5FDA0B0__INCLUDED_)
