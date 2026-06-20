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

#if !defined(AFX_OUTPUTVIEW_H__53A5BF27_C668_496B_B7C1_1AC81929357B__INCLUDED_)
#define AFX_OUTPUTVIEW_H__53A5BF27_C668_496B_B7C1_1AC81929357B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputView.h : header file
//

#include "resource.h"
#include "PacketWatchView.h"
#include "MessageView.h"
#include "SheetTabCtrl.h"

#define NUM_OUTPUTTAB_PAGES     2

/////////////////////////////////////////////////////////////////////////////
// COutputView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class COutputView : public CFormView
{
public:
    CMessageView &GetMessageView(void);
    CPacketWatchView &GetWatchView(void);

    //{{AFX_DATA(COutputView)
    enum { IDD = IDD_OUTPUT_VIEW };
    CSheetTabCtrl   m_Tab;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COutputView)
public:
    virtual void OnInitialUpdate();
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    void InitTabs(void);

protected:
    BOOL m_Initialized;
    CPacketWatchView        m_DlgWatch;
    CMessageView            m_DlgMessages;
    CDialog *m_TabDlg[NUM_OUTPUTTAB_PAGES];
    DLGTEMPLATE *m_DlgTempl[NUM_OUTPUTTAB_PAGES];

    DECLARE_DYNCREATE(COutputView)

    COutputView();           // protected constructor used by dynamic creation
    virtual ~COutputView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext &dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(COutputView)
    afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline CMessageView &COutputView::GetMessageView(void)
{
    return m_DlgMessages;
}

inline CPacketWatchView &COutputView::GetWatchView(void)
{
    return m_DlgWatch;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTVIEW_H__53A5BF27_C668_496B_B7C1_1AC81929357B__INCLUDED_)
