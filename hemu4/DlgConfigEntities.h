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

#if !defined(AFX_DLGCONFIGENTITIES_H__E100F1AE_A833_490F_AF7B_444D57B7C83A__INCLUDED_)
#define AFX_DLGCONFIGENTITIES_H__E100F1AE_A833_490F_AF7B_444D57B7C83A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfigEntities.h : header file
//

#include "DlgConfigBase.h"
#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgConfigEntities dialog

class CDlgConfigEntities : public CDlgConfigBase
{
    // Construction
public:
    CDlgConfigEntities(CWnd *pParent = NULL);   // standard constructor
    void OnOK();
    void OnCancel();
    virtual void ClearData(void) {}
    virtual int LoadData(CString IN &filename);
    virtual int SaveData(CString IN &filename);

    // Dialog Data
    //{{AFX_DATA(CDlgConfigEntities)
    enum { IDD = IDD_BLANK_STATE_VIEW };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgConfigEntities)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CDlgConfigEntities)
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONFIGENTITIES_H__E100F1AE_A833_490F_AF7B_444D57B7C83A__INCLUDED_)
