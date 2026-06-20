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

#if !defined(AFX_DLGCONFIGVIEWS_H__DE353F23_37E8_44E5_8F34_9636B093957F__INCLUDED_)
#define AFX_DLGCONFIGVIEWS_H__DE353F23_37E8_44E5_8F34_9636B093957F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfigViews.h : header file
//

#include "DlgConfigBase.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgConfigViews dialog

class CDlgConfigViews : public CDlgConfigBase
{
    // Construction
public:
    CDlgConfigViews(CWnd *pParent = NULL);   // standard constructor
    void OnOK();
    void OnCancel();
    int LoadTree(CArchive &ar);
    int SaveTree(CArchive &ar);
    virtual void ClearData(void);
    virtual int LoadData(CString IN &filename);
    virtual int SaveData(CString IN &filename);

    // Dialog Data
    //{{AFX_DATA(CDlgConfigViews)
    enum { IDD = IDD_BLANK_STATE_VIEW };
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgConfigViews)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgConfigViews)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONFIGVIEWS_H__DE353F23_37E8_44E5_8F34_9636B093957F__INCLUDED_)
