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

#if !defined(AFX_CONFIGDLG_H__2A102755_99D8_426B_97EC_EF9AF7B1CBE5__INCLUDED_)
#define AFX_CONFIGDLG_H__2A102755_99D8_426B_97EC_EF9AF7B1CBE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDlg.h : header file
//

#include "ConfigTempl.h"
#include "DlgConfigBase.h"
#include "DlgConfigEntities.h"
#include "DlgConfigEnv.h"
#include "DlgConfigTerrain.h"
#include "DlgConfigViews.h"
#include "Resource.h"
#include "SheetTabCtrl.h"

#define NUM_CONFIGTAB_PAGES 4

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog

class CConfigDlg : public CDialog
{
    // Construction
public:
    CConfigDlg(CWnd *pParent = NULL);   // standard constructor
    CDlgConfigBase *GetTabPage(const int id);
    CDlgConfigEntities *GetEntitiesTabPage(void);
    CDlgConfigEnv *GetEnvTabPage(void);
    CDlgConfigTerrain *GetTerrainTabPage(void);
    CDlgConfigViews *GetViewsTabPage(void);

    // Dialog Data
    //{{AFX_DATA(CConfigDlg)
    enum { IDD = IDD_VIEW_CONFIGURATION };
    CSheetTabCtrl   m_Tab;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CConfigDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CDlgConfigEntities      m_DlgEntities;
    CDlgConfigEnv           m_DlgEnv;
    CDlgConfigTerrain       m_DlgTerrain;
    CDlgConfigViews         m_DlgViews;
    CDlgConfigBase          *m_TabDlg[NUM_CONFIGTAB_PAGES];
    DLGTEMPLATE             *m_DlgTempl[NUM_CONFIGTAB_PAGES];

    void InitTabs(void);

    // Generated message map functions
    //{{AFX_MSG(CConfigDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline CDlgConfigBase *CConfigDlg::GetTabPage(const int id)
{
    if ((unsigned int)id < NUM_CONFIGTAB_PAGES)
        return m_TabDlg[id];
    else
        return NULL;
}

inline CDlgConfigEntities *CConfigDlg::GetEntitiesTabPage(void)
{
    return &m_DlgEntities;
}

inline CDlgConfigEnv *CConfigDlg::GetEnvTabPage(void)
{
    return &m_DlgEnv;
}

inline CDlgConfigTerrain *CConfigDlg::GetTerrainTabPage(void)
{
    return &m_DlgTerrain;
}

inline CDlgConfigViews *CConfigDlg::GetViewsTabPage(void)
{
    return &m_DlgViews;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGDLG_H__2A102755_99D8_426B_97EC_EF9AF7B1CBE5__INCLUDED_)
