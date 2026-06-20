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
 *  FILENAME:   ConfigWnd.cpp
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

// ConfigWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ConfigWnd.h"
#include "DirDialog.h"
#include "globals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MENU_ITEM_LOAD      0
#define MENU_ITEM_SAVE      1
#define MENU_ITEM_EXPAND    4
#define MENU_ITEM_COLLAPSE  5
#define MENU_ITEM_ADD       7
#define MENU_ITEM_DELETE    8

/////////////////////////////////////////////////////////////////////////////
// CConfigWnd

CConfigWnd::CConfigWnd()
{
    m_hMenu = NULL;
}

CConfigWnd::~CConfigWnd()
{
}

BEGIN_MESSAGE_MAP(CConfigWnd, CWnd)
    //{{AFX_MSG_MAP(CConfigWnd)
    ON_WM_CLOSE()
    ON_WM_CREATE()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] = {
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CConfigWnd message handlers

void CConfigWnd::OnClose()
{
    // We don't want to close the window.  We want to hide it.
    // The window is destroyed automatically when CMainFrame is destroyed.
    ShowWindow(SW_HIDE);
}

int CConfigWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rect(0, 0, 1, 1);
    if (!m_ToolBarCtrl.Create(WS_CHILD | WS_VISIBLE | CCS_TOP | CCS_NORESIZE | TBSTYLE_FLAT,
                              rect, this, ID_CONFIG_TOOLBAR)) {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }

    // Create the toolbar buttons.
    TBBUTTON tbbuttons[] = {{0, ID_FILE_LOAD_TEMPLATE, TBSTATE_ENABLED, TBSTYLE_BUTTON, NULL, NULL},
        {1, ID_FILE_SAVE_TEMPLATE, TBSTATE_ENABLED, TBSTYLE_BUTTON, NULL, NULL},
        {0, 0, 0, TBSTYLE_SEP, NULL, NULL},
        {2, ID_VIEW_EXPAND_ALL, TBSTATE_ENABLED, TBSTYLE_BUTTON, NULL, NULL},
        {3, ID_VIEW_COLLAPSE_ALL, TBSTATE_ENABLED, TBSTYLE_BUTTON, NULL, NULL},
        {0, 0, 0, TBSTYLE_SEP, NULL, NULL},
        {4, ID_CONFIG_ADD_ITEM, TBSTATE_ENABLED, TBSTYLE_BUTTON, NULL, NULL},
        {5, ID_CONFIG_DELETE_ITEM, TBSTATE_ENABLED, TBSTYLE_BUTTON, NULL, NULL}
    };

    CBitmap bmpConfigToolbar;
    bmpConfigToolbar.LoadBitmap(IDB_CONFIG_TOOLBAR);
    m_ButtonImages.Create(16, 15, ILC_COLOR16 | ILC_MASK, 6, 6);
    m_ButtonImages.Add(&bmpConfigToolbar, RGB(255, 230, 255));
    m_ButtonImages.SetBkColor(CLR_NONE);    // make transparent
    m_ToolBarCtrl.SetImageList(&m_ButtonImages);
    m_ToolBarCtrl.AddButtons(8, tbbuttons);

    // Set the icon.
    SetIcon(AfxGetApp()->LoadIcon(IDI_CONFIG), FALSE);

    // Create the client window.
    m_DlgTempl = LockDlgRes(MAKEINTRESOURCE(IDD_VIEW_CONFIGURATION));
    m_DlgClient.CreateIndirect(m_DlgTempl, this);

    CMenu menu;
    menu.LoadMenu(IDR_CONFIGURATION);
    this->SetMenu(&menu);
    m_hMenu = menu.Detach();

    return 0;
}

void CConfigWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    CRect itemrect;
    m_ToolBarCtrl.GetItemRect(0, &itemrect);
    m_ToolBarCtrl.MoveWindow(0, 0, cx, itemrect.Height() + 2);

    CRect rect;
    m_ToolBarCtrl.GetClientRect(&rect);

    m_DlgClient.SetWindowPos(NULL, 0, rect.Height() + 2, cx, cy - rect.Height() - 3,
                             SWP_FRAMECHANGED | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_SHOWWINDOW);
}

BOOL CConfigWnd::PreCreateWindow(CREATESTRUCT &cs)
{
    BOOL retval = CFrameWnd::PreCreateWindow(cs);

    // Remove the inner client edge.
    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

    // Register a window class with a gray background (because the flat toolbar
    // is transparent).
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
                                       ::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_3DFACE + 1), NULL);

    return retval;
}

BOOL CConfigWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // Handle the toolbar and menu commands.
    switch ((int)wParam) {
    case ID_FILE_LOAD_TEMPLATE:
        OnLoadTemplate();
        break;

    case ID_FILE_SAVE_TEMPLATE:
        OnSaveTemplate();
        break;

    case ID_VIEW_EXPAND_ALL:
        OnExpandAll();
        break;

    case ID_VIEW_COLLAPSE_ALL:
        OnCollapseAll();
        break;

    default:
        break;
    }

    return CWnd::OnCommand(wParam, lParam);
}

void CConfigWnd::OnLoadTemplate()
{
    CDirDialog dlg(".\\Default", "Definition Files (*.def)|*.def|All Files (*.*)|*.*||");
    if (dlg.DoModal() == IDOK) {
        CString strDir = dlg.GetPath();
        CString path = strDir + "\\entities.def";
        ClearAll();
        ((CDlgConfigEntities *)GetTabPage(0))->LoadData(path);
    }
}

void CConfigWnd::OnSaveTemplate()
{
    CDirDialog dlg(".\\Default", "Definition Files (*.def)|*.def|All Files (*.*)|*.*||");
    if (dlg.DoModal() == IDOK) {
        CString strDir = dlg.GetPath();
        CString path = strDir + "\\entities.def";
        ((CDlgConfigEntities *)GetTabPage(0))->SaveData(path);
    }
}

void CConfigWnd::OnExpandAll()
{
}

void CConfigWnd::OnCollapseAll()
{
}

void CConfigWnd::OnAddItem()
{
    CRect rectmenu, rectbutton;
    CSize size;
    CMenu menu;
    menu.CreatePopupMenu();

    switch (GetActiveTabID()) {
    case 0:     // Entities

        // Build the menu.
        menu.AppendMenu(MF_ENABLED | MF_STRING, ID_CONFIG_ADD_ITEM_ENTITY, "Entity Type");
        menu.AppendMenu(MF_ENABLED | MF_STRING, ID_CONFIG_ADD_ITEM_COMPONENT, "Component");
        menu.AppendMenu(MF_ENABLED | MF_STRING, ID_CONFIG_ADD_ITEM_COMPSTATE, "Component State");
        menu.AppendMenu(MF_ENABLED | MF_STRING, ID_CONFIG_ADD_ITEM_ARTPART, "Articulated Part");
        menu.AppendMenu(MF_ENABLED | MF_STRING, ID_CONFIG_ADD_ITEM_ATTACHPOINT, "Attach Point");

        // Display the menu below the button.
        m_ToolBarCtrl.GetRect(ID_CONFIG_ADD_ITEM, &rectbutton);
        m_ToolBarCtrl.GetWindowRect(&rectmenu);
        size.cx = rectbutton.left;
        size.cy = rectbutton.top;
        rectmenu += size;
        menu.TrackPopupMenu(TPM_LEFTALIGN, rectmenu.left, rectmenu.bottom, this, NULL);

        break;

    case 1:     // Environment
        break;

    case 2:     // Terrain
        break;

    case 3:     // Views
        break;

    default:
        break;
    }
}

void CConfigWnd::OnDeleteItem()
{
    CRect rectmenu, rectbutton;
    CSize size;
    CMenu menu;
    menu.CreatePopupMenu();

    switch (GetActiveTabID()) {
    case 0:     // Entities

        // Build the menu.
        menu.AppendMenu(MF_ENABLED | MF_STRING, ID_CONFIG_DELETE_ITEM_ENTITY, "Entity Type");
        menu.AppendMenu(MF_ENABLED | MF_STRING, ID_CONFIG_DELETE_ITEM_COMPONENT, "Component");
        menu.AppendMenu(MF_ENABLED | MF_STRING, ID_CONFIG_DELETE_ITEM_COMPSTATE, "Component State");
        menu.AppendMenu(MF_ENABLED | MF_STRING, ID_CONFIG_DELETE_ITEM_ARTPART, "Articulated Part");
        menu.AppendMenu(MF_ENABLED | MF_STRING, ID_CONFIG_DELETE_ITEM_ATTACHPOINT, "Attach Point");

        // Display the menu below the button.
        m_ToolBarCtrl.GetRect(ID_CONFIG_DELETE_ITEM, &rectbutton);
        m_ToolBarCtrl.GetWindowRect(&rectmenu);
        size.cx = rectbutton.left;
        size.cy = rectbutton.top;
        rectmenu += size;
        menu.TrackPopupMenu(TPM_LEFTALIGN, rectmenu.left, rectmenu.bottom, this, NULL);

        break;

    case 1:     // Environment
        break;

    case 2:     // Terrain
        break;

    case 3:     // Views
        break;

    default:
        break;
    }
}

void CConfigWnd::ClearAll(void)
{
    ((CDlgConfigEntities *)GetTabPage(0))->ClearData();
}
