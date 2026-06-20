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

#if !defined(AFX_VIEWFRUSTUMPROPPAGE_H__2704DA4E_975E_4C2C_BABA_EF0C762B7FCB__INCLUDED_)
#define AFX_VIEWFRUSTUMPROPPAGE_H__2704DA4E_975E_4C2C_BABA_EF0C762B7FCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewFrustumPropPage.h : header file
//

#include "BaseStateView.h"
#include "TranspStatic.h"
#include "EditKbd.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CViewFrustumPropPage dialog

class CViewFrustumPropPage : public CBaseStateView
{
    // Construction
public:
    CViewFrustumPropPage(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    virtual void RefreshView(void);

    // Dialog Data
    //{{AFX_DATA(CViewFrustumPropPage)
    enum { IDD = IDD_PROPPAGE_VIEW_FRUSTUM };
    CButton m_RadioPerspective;
    CButton m_RadioOrthographic;
    CEditKbd    m_EditFOVFar;
    CEditKbd    m_EditFOVNear;
    CEditKbd    m_EditFOVRight;
    CEditKbd    m_EditFOVLeft;
    CEditKbd    m_EditFOVTop;
    CEditKbd    m_EditFOVBottom;
    CTranspStatic   m_PictureCtrlOrtho;
    CTranspStatic   m_PictureCtrlPerspective;
    int     m_ProjectionType;
    float   m_FOVBottom;
    float   m_FOVTop;
    float   m_FOVLeft;
    float   m_FOVRight;
    float   m_FOVNear;
    float   m_FOVFar;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CViewFrustumPropPage)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    afx_msg LRESULT OnUserEditControl(WPARAM wParam, LPARAM lParam);

    void OnUserChangeFovNear(void);
    void OnUserChangeFovFar(void);
    void OnUserChangeFovLeft(void);
    void OnUserChangeFovRight(void);
    void OnUserChangeFovTop(void);
    void OnUserChangeFovBottom(void);

    // Generated message map functions
    //{{AFX_MSG(CViewFrustumPropPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnRadioPerspective();
    afx_msg void OnRadioOrthographic();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWFRUSTUMPROPPAGE_H__2704DA4E_975E_4C2C_BABA_EF0C762B7FCB__INCLUDED_)
