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

#if !defined(AFX_DBDIALOGBAR_H__663D77D4_BF45_4D22_8142_8D385968BA0B__INCLUDED_)
#define AFX_DBDIALOGBAR_H__663D77D4_BF45_4D22_8142_8D385968BA0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DBDialogBar.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDBDialogBar dialog

class CDBDialogBar : public CDialog
{
    // Construction
public:
    CDBDialogBar(CWnd *pParent = NULL);   // standard constructor
    void PopulateDatabaseCombo(void);
    void ClearDatabaseCombo(void);
    void SelectDatabaseItem(const int db_id);
    void UpdateDatabaseCombo(void);
    int FindComboItem(const int id);
    int FindComboInsertPoint(const int item_id);

    friend class CMainFrame;

    // Dialog Data
    //{{AFX_DATA(CDBDialogBar)
    enum { IDD = IDD_DIALOGBAR };
    CComboBox   m_ComboDatabase;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDBDialogBar)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDBDialogBar)
    afx_msg void OnSelchangeDatabase();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline void CDBDialogBar::ClearDatabaseCombo(void)
{
    m_ComboDatabase.ResetContent();
}

inline void CDBDialogBar::SelectDatabaseItem(const int db_id)
{
    m_ComboDatabase.SetCurSel(FindComboItem(db_id));
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBDIALOGBAR_H__663D77D4_BF45_4D22_8142_8D385968BA0B__INCLUDED_)
