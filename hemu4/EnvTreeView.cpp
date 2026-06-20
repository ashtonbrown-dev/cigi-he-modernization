/** <pre>
 *  Copyright 2004 The Boeing Company
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
 *
 *  FILENAME:   EnvTreeView.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface Host Emulator (CIGI HE)
 *
 *  PROGRAM DESCRIPTION:
 *  The Host Emulator (HE) acts as a surrogate in the absence of a simulation host.
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME
 *  DESCRIPTION OF CHANGE
 *
 *  11/11/2004 Lance Durham
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  Version 3.1.1
 */

#include "stdafx.h"
#include "DebugTrace.h"
#include "Hemu4.h"
#include "EntityTreeView.h"
#include "EnvTreeView.h"
#include "ObjectStateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnvTreeView dialog

CEnvTreeView::CEnvTreeView(CWnd *pParent /*=NULL*/)
    : CDialog(CEnvTreeView::IDD, pParent)
{
    //{{AFX_DATA_INIT(CEnvTreeView)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CEnvTreeView::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEnvTreeView)
    DDX_Control(pDX, IDC_TREE, m_TreeCtrl);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEnvTreeView, CDialog)
    //{{AFX_MSG_MAP(CEnvTreeView)
    ON_WM_SIZE()
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
    ON_WM_SHOWWINDOW()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnvTreeView message handlers

void CEnvTreeView::OnOK()
{
}

void CEnvTreeView::OnCancel()
{
}

void CEnvTreeView::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // Resize the control.
    if (m_TreeCtrl.GetSafeHwnd())
        m_TreeCtrl.MoveWindow(0, 0, cx, cy);
}

BOOL CEnvTreeView::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Modify the style so that the selected item is always highlighted,
    // the items have buttons, and there are lines.
    m_TreeCtrl.ModifyStyle(0, TVS_SHOWSELALWAYS | TVS_HASLINES |
                           TVS_LINESATROOT | TVS_HASBUTTONS, 0);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CEnvTreeView::OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    CDebugTrace trace("CEnvTreeView::OnSelchangedTree(NMHDR *, LRESULT *)");

    NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pNMHDR;;
    HTREEITEM hitem = pNMTreeView->itemNew.hItem;
    DWORD itemdata = m_TreeCtrl.GetItemData(hitem);
    int object_type = MAKE_TYPE(itemdata);
    int entity_id = MAKE_ID(itemdata);

    theApp.GetMainFrame().SetActiveStateView(object_type);

    // If we have selected a weather entity, simulate selection in the
    // Entity tree.
    if (object_type == OBJECT_TYPE_ENTITY) {
        theApp.GetMainFrame().GetEntityTreeView().SelectEntityByID(entity_id);

        // And select the Weather tab.
        theApp.GetMainFrame().GetEntityStateView().SelectTab(6);
    }

    // Since our environment property pages aren't updated by the timer,
    // force a refresh here.
    theApp.GetMainFrame().GetObjectStateView().RefreshActiveView();

    *pResult = 0;
}

void CEnvTreeView::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDebugTrace trace("CEnvTreeView::OnShowWindow(BOOL, UINT)");

    CDialog::OnShowWindow(bShow, nStatus);

    HTREEITEM hitem = m_TreeCtrl.GetSelectedItem();
    if (hitem) {
        int viewnum = MAKE_TYPE(m_TreeCtrl.GetItemData(hitem));
        theApp.GetMainFrame().SetActiveStateView(viewnum);
    } else {
        theApp.GetMainFrame().SetActiveStateView(-1);
    }
}
