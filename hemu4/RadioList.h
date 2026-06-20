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

#if !defined(AFX_RADIOLIST_H__28A690E2_CC74_11D5_BBCD_00609419F883__INCLUDED_)
#define AFX_RADIOLIST_H__28A690E2_CC74_11D5_BBCD_00609419F883__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RadioList.h : header file
//

// Define a message that will get sent to the parent window when the
// item is changed.
#define RL_SELCHANGED       (WM_APP + 2)

/////////////////////////////////////////////////////////////////////////////
// CRadioList window

class CRadioList : public CWnd
{
public:
    CRadioList();
    virtual ~CRadioList();
    void Init(void);
    int AddItem(LPCTSTR text);
    int GetItemCount(void);
    void Clear(void);
    int GetCurSel(void);
    void SetCurSel(const int item);
    BOOL SetItemData(int item, DWORD data);
    DWORD GetItemData(int item);

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CRadioList)
public:
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo);
    //}}AFX_VIRTUAL

protected:
    int m_WndWidth;
    int m_WndHeight;
    int m_PageHeight;
    int m_BorderWidth;
    int m_BorderHeight;
    int m_BtnHeight;
    int m_BtnCount;
    int m_ScrollPos;
    int m_LastItem;
    CDWordArray m_ItemData;
    CArray<CButton *, CButton *&> m_BtnArray;
    CFont m_Font;

    //{{AFX_MSG(CRadioList)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline int CRadioList::GetItemCount(void)
{
    return m_BtnCount;
}

inline DWORD CRadioList::GetItemData(int item)
{
    return (item > 0) ? m_ItemData[item] : 0;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RADIOLIST_H__28A690E2_CC74_11D5_BBCD_00609419F883__INCLUDED_)
