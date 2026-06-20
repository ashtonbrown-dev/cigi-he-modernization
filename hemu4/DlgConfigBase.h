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

#if !defined(AFX_DLGCONFIGBASE_H__8FC90CF8_CAEF_43AD_9D51_01C03966A9B5__INCLUDED_)
#define AFX_DLGCONFIGBASE_H__8FC90CF8_CAEF_43AD_9D51_01C03966A9B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfigBase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgConfigBase dialog

class CDlgConfigBase : public CDialog
{
    // Construction
public:
    virtual void ClearData(void) = 0;
    virtual int LoadData(CString IN &filename) = 0;
    virtual int SaveData(CString IN &filename) = 0;
    CDlgConfigBase(UINT nIDTemplate, CWnd *pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgConfigBase)
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgConfigBase)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgConfigBase)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONFIGBASE_H__8FC90CF8_CAEF_43AD_9D51_01C03966A9B5__INCLUDED_)
