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

#if !defined(AFX_SITUATIONVIEW_H__A817776A_FFD1_4F8B_B0BC_B4FA29318B15__INCLUDED_)
#define AFX_SITUATIONVIEW_H__A817776A_FFD1_4F8B_B0BC_B4FA29318B15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SituationView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSituationView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "SituationDispCtrl.h"

class CSituationView : public CFormView
{
public:
    virtual void OnOK();
    virtual void OnCancel();

    //{{AFX_DATA(CSituationView)
    enum { IDD = IDD_SITUATIONVIEW };
    CSituationDispCtrl  m_SituationCtrl;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSituationView)
public:
    virtual void OnInitialUpdate();
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    BOOL m_Initialized;
    CSituationView();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CSituationView)
    virtual ~CSituationView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext &dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CSituationView)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    afx_msg LRESULT OnPick(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SITUATIONVIEW_H__A817776A_FFD1_4F8B_B0BC_B4FA29318B15__INCLUDED_)
