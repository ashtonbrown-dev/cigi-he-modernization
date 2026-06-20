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

#if !defined(AFX_DLGCONFIGENV_H__D6251AC3_E5B4_4803_9955_E859305EABA1__INCLUDED_)
#define AFX_DLGCONFIGENV_H__D6251AC3_E5B4_4803_9955_E859305EABA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfigEnv.h : header file
//

#include "DlgConfigBase.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgConfigEnv dialog

class CDlgConfigEnv : public CDlgConfigBase
{
    // Construction
public:
    CDlgConfigEnv(CWnd *pParent = NULL);   // standard constructor
    void OnOK();
    void OnCancel();
    int LoadTree(CArchive &ar);
    int SaveTree(CArchive &ar);
    virtual void ClearData(void);
    virtual int LoadData(CString IN &filename);
    virtual int SaveData(CString IN &filename);

    // Dialog Data
    //{{AFX_DATA(CDlgConfigEnv)
    enum { IDD = IDD_BLANK_STATE_VIEW };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgConfigEnv)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgConfigEnv)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONFIGENV_H__D6251AC3_E5B4_4803_9955_E859305EABA1__INCLUDED_)
