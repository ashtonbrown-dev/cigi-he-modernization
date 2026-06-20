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

// ViewTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "Hemu4.h"
#include "hemumsg.h"
#include "CigiView.h"
#include "ViewGroup.h"
#include "ViewStateView.h"
#include "ViewTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTreeView dialog

CViewTreeView::CViewTreeView(CWnd *pParent /*=NULL*/)
    : CDialog(CViewTreeView::IDD, pParent)
{
    //{{AFX_DATA_INIT(CViewTreeView)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    m_IsDragging = FALSE;
}

void CViewTreeView::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CViewTreeView)
    DDX_Control(pDX, IDC_TREE, m_TreeCtrl);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CViewTreeView, CDialog)
    //{{AFX_MSG_MAP(CViewTreeView)
    ON_WM_SIZE()
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
    ON_WM_SHOWWINDOW()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE, OnBegindragTree)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTreeView message handlers

void CViewTreeView::OnOK()
{
}

void CViewTreeView::OnCancel()
{
}

void CViewTreeView::ClearItems(void)
{
    m_TreeCtrl.DeleteAllItems();
}

void CViewTreeView::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // Resize the control.
    if (m_TreeCtrl.GetSafeHwnd())
        m_TreeCtrl.MoveWindow(0, 0, cx, cy);
}

BOOL CViewTreeView::OnInitDialog()
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

void CViewTreeView::OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    CDebugTrace trace("CViewTreeView::OnSelchangedTree(NMHDR *, LRESULT *)");

    NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pNMHDR;;
    HTREEITEM hitem = pNMTreeView->itemNew.hItem;
    int viewnum = MAKE_TYPE(m_TreeCtrl.GetItemData(hitem));

    theApp.GetMainFrame().SetActiveStateView(viewnum);

    *pResult = 0;
}

void CViewTreeView::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDebugTrace trace("CViewTreeView::OnShowWindow(BOOL, UINT)");

    CDialog::OnShowWindow(bShow, nStatus);

    HTREEITEM hitem = m_TreeCtrl.GetSelectedItem();
    if (hitem) {
        int viewnum = MAKE_TYPE(m_TreeCtrl.GetItemData(hitem));
        theApp.GetMainFrame().SetActiveStateView(viewnum);
    } else {
        theApp.GetMainFrame().SetActiveStateView(-1);
    }
}

void CViewTreeView::SelectViewByID(const int id)
{
    CDebugTrace trace("CViewTreeView::SelectViewByID(int)");

    CCigiView *view = g_DataManager.GetView(id);
    if (view)
        m_TreeCtrl.SelectItem(view->GetHtree());
}

void CViewTreeView::SelectViewGroupByID(const int id)
{
    CDebugTrace trace("CViewTreeView::SelectViewGroupByID(int)");

    CViewGroup *group = g_DataManager.GetViewGroup(id);
    if (group)
        m_TreeCtrl.SelectItem(group->GetHtree());
}


void CViewTreeView::OnBegindragTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    CDebugTrace trace("CViewTreeView::OnBegindragTree(NMHDR *, LRESULT *)");

    NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pNMHDR;
    HTREEITEM htree = pNMTreeView->itemNew.hItem;

    // To make things simple, select the item we are dragging.
    GetTreeCtrl().SelectItem(htree);

    // Set the flag.
    m_IsDragging = TRUE;

    // Get the offset from the top left of the image to the cursor.
    CRect rect;
    GetTreeCtrl().GetItemRect(htree, &rect, TRUE);
    CPoint imagepos(rect.left, rect.top);
    CPoint cursorpos;
    GetCursorPos(&cursorpos);
    GetTreeCtrl().ScreenToClient(&cursorpos);
    CPoint offset = cursorpos - imagepos;

    // Set the image list to begin dragging.
    int img, sel_img;
    GetTreeCtrl().GetItemImage(htree, img, sel_img);
    g_ImgListIcons.BeginDrag(img, offset);
    g_ImgListIcons.DragEnter(&(GetTreeCtrl()), cursorpos);

    *pResult = 0;
}

void CViewTreeView::OnLButtonUp(UINT nFlags, CPoint point)
{
    CDebugTrace trace("CViewTreeView::OnLButtonUp(NMHDR *, LRESULT *)");

    if (m_IsDragging) {
        UINT flags = 0;
        HTREEITEM dest = GetTreeCtrl().HitTest(point, &flags);
        CCigiView *view = g_DataManager.GetSelectedView();
        if (view) {
            int view_id = view->GetViewID();

            if (flags & TVHT_ONITEM) {
                // We have a hit.  If we aren't trying to make a circular
                // relationship (i.e., a node is a descendant of itself,
                // create the parent-child relationship and select the parent (dest).
                CViewGroup *group = g_DataManager.GetViewGroup(dest);
                if (group) {
                    int group_id = group->GetGroupID();

                    if (view->GetGroupID() != group_id) {
                        // Set the parent.
                        view->SetGroupID(group_id);

                        // After we've set the parent, update the tree control
                        // and the view's HTREEITEM.
                        view->SetHtree(MoveTreeItem(view->GetHtree(), group->GetHtree()));

                        // Expand the destination and child items.
                        GetTreeCtrl().Expand(dest, TVE_EXPAND);
                        GetTreeCtrl().Expand(view->GetHtree(), TVE_EXPAND);

                        // Select the item.
                        GetTreeCtrl().SelectItem(view->GetHtree());

                        // Make sure the correct group is selected from the list, since the
                        // driver will not have had time to update the object.
                        theApp.GetMainFrame().GetViewStateView().
                        SelectViewGroupComboItem(group_id);

                        // Also, disable the entity combo for the same reason.
                        theApp.GetMainFrame().GetViewStateView().
                        m_ComboEntity.EnableWindow(FALSE);

                        // And force the view's entity ID to be the same as the group's.
                        view->SetEntityID(group->GetEntityID());
                        theApp.GetMainFrame().GetViewStateView().
                        SelectEntityComboItem(group->GetEntityID());
                    }
                }

                // Make sure the target item isn't highlighted.
                GetTreeCtrl().SetItem(dest, TVIF_STATE, NULL, 0, 0, 0, TVIS_DROPHILITED, 0);
            } else {
                // We have a miss.  Destroy the parent-child relationship.
                view->SetGroupID(0);

                // After we've set the parent, update the tree control
                // and the entity's HTREEITEM.
                view->SetHtree(MoveTreeItem(view->GetHtree(), NULL));

                GetTreeCtrl().SelectItem(view->GetHtree());
                GetTreeCtrl().Expand(view->GetHtree(), TVE_EXPAND);

                // Select the item.
                GetTreeCtrl().SelectItem(view->GetHtree());

                // Make sure the "None" group is selected from the list, since the
                // driver will not have had time to update the object.
                theApp.GetMainFrame().GetViewStateView().
                SelectViewGroupComboItem(0);

                // Also, disable the entity combo for the same reason.
                theApp.GetMainFrame().GetViewStateView().
                m_ComboEntity.EnableWindow(TRUE);
            }
        } else { // We're trying to move a group.
            // Make sure the target item isn't highlighted.
            GetTreeCtrl().SetItem(dest, TVIF_STATE, NULL, 0, 0, 0, TVIS_DROPHILITED, 0);
        }

        m_IsDragging = FALSE;
        g_ImgListIcons.EndDrag();
        RedrawWindow();
    }

    CDialog::OnLButtonUp(nFlags, point);
}

void CViewTreeView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_IsDragging) {
        static HTREEITEM prev_dest = NULL;
        UINT flags = 0;
        HTREEITEM dest = GetTreeCtrl().HitTest(point, &flags);
        CRect update_rect;

        // Highlight the target if there is one and un-highlight
        // the previous target.
        if (flags & TVHT_ONITEM) {
            g_ImgListIcons.DragLeave(&(GetTreeCtrl()));

            GetTreeCtrl().SetItem(dest, TVIF_STATE, NULL, 0, 0,
                                  TVIS_DROPHILITED, TVIS_DROPHILITED, 0);

            GetTreeCtrl().GetItemRect(dest, &update_rect, TRUE);
            GetTreeCtrl().RedrawWindow(&update_rect);
            g_ImgListIcons.DragEnter(&(GetTreeCtrl()), point);
        }

        if (prev_dest != dest) {
            g_ImgListIcons.DragLeave(&(GetTreeCtrl()));

            GetTreeCtrl().SetItem(prev_dest, TVIF_STATE, NULL, 0, 0,
                                  0, TVIS_DROPHILITED, 0);

            GetTreeCtrl().GetItemRect(prev_dest, &update_rect, TRUE);
            GetTreeCtrl().RedrawWindow(&update_rect);
            g_ImgListIcons.DragEnter(&(GetTreeCtrl()), point);
        }

        // Move the drag image.
        g_ImgListIcons.DragMove(point);

        prev_dest = dest;
    }

    CDialog::OnMouseMove(nFlags, point);
}

HTREEITEM CViewTreeView::MoveTreeItem(const HTREEITEM item, const HTREEITEM parent)
{
    CDebugTrace trace("CViewTreeView::MoveTreeItem(HTREEITEM, HTREEITEM)");

    HTREEITEM hitemParent = parent;
    if (hitemParent == NULL)
        hitemParent = TVI_ROOT;

    // Don't do anything if the parent hasn't changed.
    if (GetTreeCtrl().GetParentItem(item) == parent)
        return item;

    // Clone the item in the tree.
    char str[64];
    TVITEM tvItem = { TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE |
                      TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_TEXT,
                      item, 0, 0, str, 64, 0, 0, 0, 0
                    };
    GetTreeCtrl().GetItem(&tvItem);
    HTREEITEM hitemNodeNew = GetTreeCtrl().InsertItem(str, tvItem.iImage,
                                                      tvItem.iSelectedImage, hitemParent);
    GetTreeCtrl().SetItemData(hitemNodeNew, tvItem.lParam);

    // Move any children to the new node.
    MoveTreeChildren(item, hitemNodeNew);

    // Delete the old item.
    GetTreeCtrl().DeleteItem(item);

    return hitemNodeNew;
}

void CViewTreeView::MoveTreeChildren(const HTREEITEM src, const HTREEITEM dest)
{
    CDebugTrace trace("CViewTreeView::MoveTreeChildren(HTREEITEM, HTREEITEM)");

    if (GetTreeCtrl().ItemHasChildren(src)) {
        HTREEITEM hNext;
        HTREEITEM hChild = GetTreeCtrl().GetChildItem(src);

        while (hChild) {
            hNext = GetTreeCtrl().GetNextSiblingItem(hChild);
            MoveTreeItem(hChild, dest);
            hChild = hNext;
        }
    }
}

HTREEITEM CViewTreeView::MoveViewToGroup(CCigiView *view, const int group_id)
{
    CDebugTrace trace("CViewTreeView::MoveViewToGroup(CCigiView *, int)");

    if (!view)
        return NULL;

    // Update the tree view.
    HTREEITEM htreeGroup;
    CViewGroup *group = g_DataManager.GetViewGroup(group_id);
    if (group)
        htreeGroup = group->GetHtree();
    else
        htreeGroup = NULL;

    HTREEITEM htreeItem = MoveTreeItem(view->GetHtree(), htreeGroup);
    view->SetHtree(htreeItem);
    GetTreeCtrl().Expand(htreeGroup, TVE_EXPAND);
    GetTreeCtrl().SelectItem(htreeItem);

    MESSAGE_SYNC_UPDATE_VIEWS_VIEW msg;
    PostDriverMsg(msg);

    return htreeItem;
}
