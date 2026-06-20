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

#if !defined(AFX_CONFIGWND_H__75CC6BDC_F2EA_49B5_8DF5_3AAAAC426E7F__INCLUDED_)
#define AFX_CONFIGWND_H__75CC6BDC_F2EA_49B5_8DF5_3AAAAC426E7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigWnd.h : header file
//

#include "ConfigDlg.h"
#include "DlgConfigBase.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigWnd window

class CConfigWnd : public CFrameWnd
{
    // Construction
public:
    CConfigWnd();
    virtual ~CConfigWnd();
    int GetActiveTabID(void);
    CDlgConfigEntities *GetEntitiesTabPage(void);
    CDlgConfigEnv *GetEnvTabPage(void);
    CDlgConfigTerrain *GetTerrainTabPage(void);
    CDlgConfigViews *GetViewsTabPage(void);
    CDlgConfigBase *GetTabPage(const int id);
    CDlgConfigBase *GetActiveTabPage(void);
    void OnLoadTemplate();
    void OnSaveTemplate();
    void OnExpandAll();
    void OnCollapseAll();
    void OnAddItem();
    void OnDeleteItem();
    void ClearAll(void);

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CConfigWnd)
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT &cs);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

protected:
    CToolBarCtrl m_ToolBarCtrl;
    CImageList m_ButtonImages;
    CConfigDlg m_DlgClient;
    DLGTEMPLATE *m_DlgTempl;
    HMENU m_hMenu;

    //{{AFX_MSG(CConfigWnd)
    afx_msg void OnClose();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline int CConfigWnd::GetActiveTabID(void)
{
    return m_DlgClient.m_Tab.GetCurSel();
}

inline CDlgConfigEntities *CConfigWnd::GetEntitiesTabPage(void)
{
    return m_DlgClient.GetEntitiesTabPage();
}

inline CDlgConfigEnv *CConfigWnd::GetEnvTabPage(void)
{
    return m_DlgClient.GetEnvTabPage();
}

inline CDlgConfigTerrain *CConfigWnd::GetTerrainTabPage(void)
{
    return m_DlgClient.GetTerrainTabPage();
}

inline CDlgConfigViews *CConfigWnd::GetViewsTabPage(void)
{
    return m_DlgClient.GetViewsTabPage();
}

inline CDlgConfigBase *CConfigWnd::GetTabPage(const int id)
{
    return m_DlgClient.GetTabPage(id);
}

inline CDlgConfigBase *CConfigWnd::GetActiveTabPage(void)
{
    return m_DlgClient.GetTabPage(m_DlgClient.m_Tab.GetCurSel());
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGWND_H__75CC6BDC_F2EA_49B5_8DF5_3AAAAC426E7F__INCLUDED_)
