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
 *  FILENAME:   EntityTreeView.cpp
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
#include "EntityStateView.h"
#include "EntityTreeView.h"
#include "Entity.h"
#include "globals.h"
#include "Hemu4.h"
#include "SituationDispCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityTreeView dialog

CEntityTreeView::CEntityTreeView(CWnd *pParent /*=NULL*/)
    : CDialog(CEntityTreeView::IDD, pParent)
{
    //{{AFX_DATA_INIT(CEntityTreeView)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    m_IsDragging = FALSE;
}

void CEntityTreeView::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEntityTreeView)
    DDX_Control(pDX, IDC_TREE, m_TreeCtrl);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEntityTreeView, CDialog)
    //{{AFX_MSG_MAP(CEntityTreeView)
    ON_WM_SIZE()
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
    ON_WM_SHOWWINDOW()
    ON_NOTIFY(TVN_SELCHANGING, IDC_TREE, OnSelchangingTree)
    ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE, OnBegindrag)
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityTreeView message handlers

void CEntityTreeView::OnOK()
{
}

void CEntityTreeView::OnCancel()
{
}

void CEntityTreeView::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // Resize the control.
    if (m_TreeCtrl.GetSafeHwnd())
        m_TreeCtrl.MoveWindow(0, 0, cx, cy);
}

BOOL CEntityTreeView::OnInitDialog()
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

void CEntityTreeView::OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    CDebugTrace trace("CEntityTreeView::OnSelchangedTree(NMHDR *, LRESULT *)");

    NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pNMHDR;
    HTREEITEM hitem_new = pNMTreeView->itemNew.hItem;

    if (hitem_new) {
        int viewnum = MAKE_TYPE(m_TreeCtrl.GetItemData(hitem_new));
        int new_entity = MAKE_ID(m_TreeCtrl.GetItemData(hitem_new));

        // Show the correct view.
        theApp.GetMainFrame().SetActiveStateView(viewnum);

        // Select the entity in the situational display.
        theApp.GetMainFrame().SelectSituationViewEntity(new_entity);

        // Child/property nodes carry the owning entity ID. The helper also
        // suppresses duplicate publications within the same entity.
        NotifySelectedEntityChanged(new_entity);
    }

    RepopulateEntityStateParentList();
    theApp.GetMainFrame().GetEntityStateView().PopulateRefEntityList();
    theApp.GetMainFrame().GetEntityStateView().RefreshView();

    *pResult = 0;
}

void CEntityTreeView::OnSelchangingTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    CDebugTrace trace("CEntityTreeView::OnSelchangingTree(NMHDR *, LRESULT *)");

    *pResult = 0;
    NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pNMHDR;
    HTREEITEM hitem_old = pNMTreeView->itemOld.hItem;
    if (hitem_old == NULL)
        return;

    int old_entity = MAKE_ID(m_TreeCtrl.GetItemData(hitem_old));
}

void CEntityTreeView::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDebugTrace trace("CEntityTreeView::OnShowWindow(BOOL, UINT)");

    CDialog::OnShowWindow(bShow, nStatus);

    HTREEITEM hitem = m_TreeCtrl.GetSelectedItem();
    if (hitem) {
        int viewnum = MAKE_TYPE(m_TreeCtrl.GetItemData(hitem));
        theApp.GetMainFrame().SetActiveStateView(viewnum);
    } else {
        theApp.GetMainFrame().SetActiveStateView(-1);
    }
}

void CEntityTreeView::SelectEntityByID(const int id)
{
    CDebugTrace trace("CEntityTreeView::SelectEntityByID(int)");

    CEntity *entity = g_DataManager.GetEntity(id);
    if (entity)
        m_TreeCtrl.SelectItem(entity->GetHtree());
}

void CEntityTreeView::SelectEntityBySituationCtrlHandle(POSITION hdl)
{
    CDebugTrace trace("CEntityTreeView::SelectEntityBySituationCtrlHandle(POSITION)");

    int id;
    CEntity *entity = NULL;
    CEntityMap &map = g_DataManager.GetEntityMap();
    POSITION pos = map.GetStartPosition();

    while (pos) {
        map.GetNextAssoc(pos, id, entity);
        if (entity) {
            if (entity->GetSituationHandle() == hdl) {
                m_TreeCtrl.SelectItem(entity->GetHtree());
                return;
            }
        }
    }
}

void CEntityTreeView::OnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
    CDebugTrace trace("CEntityTreeView::OnBegindrag(NMHDR *, LRESULT *)");

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
    g_ImgListTypes.BeginDrag(img, offset);
    g_ImgListTypes.DragEnter(&(GetTreeCtrl()), cursorpos);

    *pResult = 0;
}

void CEntityTreeView::OnLButtonUp(UINT nFlags, CPoint point)
{
    CDebugTrace trace("CEntityTreeView::OnLButtonUp(UINT, CPoint)");

    if (m_IsDragging) {
        UINT flags = 0;
        HTREEITEM dest = GetTreeCtrl().HitTest(point, &flags);
        CEntity *child = g_DataManager.GetSelectedEntity();
        int child_id = child->GetID();

        if (flags & TVHT_ONITEM) {
            // We have a hit.  If we aren't trying to make a circular
            // relationship (i.e., a node is a descendant of itself,
            // create the parent-child relationship and select the parent (dest).
            CEntity *parent = g_DataManager.GetEntity(dest);
            if ((child != parent) && (!child->IsDescendantOf(parent))) {
                // If we've set a parent for a missile, no longer track the target.
                // Do this immediately before we set the new parent so that we
                // only get one Entity Control packet.
                child->SetTrackTarget(FALSE);

                // Set the parent.
                child->SetParent(parent->GetID(), TRUE);

                // After we've set the parent, update the tree control
                // and the entity's HTREEITEM.
                MoveTreeItem(child->GetHtree(), parent->GetHtree());

                // Repopulate the entity state view's parent list.
                RepopulateEntityStateParentList();

                // Expand the destination and child items.
                GetTreeCtrl().Expand(dest, TVE_EXPAND);
                GetTreeCtrl().Expand(child->GetHtree(), TVE_EXPAND);

                // Select the item.
                GetTreeCtrl().SelectItem(child->GetHtree());
            }

            // Make sure the target item isn't highlighted.
            GetTreeCtrl().SetItem(dest, TVIF_STATE, NULL, 0, 0, 0, TVIS_DROPHILITED, 0);

            // Show or hide the entity in the situtational display control.
            theApp.GetMainFrame().GetSituationCtrl().SetEntityVisible(
                child->GetSituationHandle(), FALSE);
        } else {
            // We have a miss.  Destroy the parent-child relationship.
            child->SetParent(-1, TRUE);

            // After we've set the parent, update the tree control
            // and the entity's HTREEITEM.
            MoveTreeItem(child->GetHtree(), NULL);

            // Repopulate the entity state view's parent list.
            RepopulateEntityStateParentList();

            GetTreeCtrl().SelectItem(child->GetHtree());
            GetTreeCtrl().Expand(child->GetHtree(), TVE_EXPAND);

            // Select the item.
            GetTreeCtrl().SelectItem(child->GetHtree());

            // Show or hide the entity in the situtational display control.
            theApp.GetMainFrame().GetSituationCtrl().SetEntityVisible(
                child->GetSituationHandle(), TRUE);
        }

        m_IsDragging = FALSE;
        g_ImgListTypes.EndDrag();
        RedrawWindow();
    }

    CDialog::OnLButtonUp(nFlags, point);
}

void CEntityTreeView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_IsDragging) {
        static HTREEITEM prev_dest = NULL;
        UINT flags = 0;
        HTREEITEM dest = GetTreeCtrl().HitTest(point, &flags);
        CRect update_rect;

        // Highlight the target if there is one and un-highlight
        // the previous target.
        if (flags & TVHT_ONITEM) {
            g_ImgListTypes.DragLeave(&(GetTreeCtrl()));

            GetTreeCtrl().SetItem(dest, TVIF_STATE, NULL, 0, 0,
                                  TVIS_DROPHILITED, TVIS_DROPHILITED, 0);

            GetTreeCtrl().GetItemRect(dest, &update_rect, TRUE);
            GetTreeCtrl().RedrawWindow(&update_rect);
            g_ImgListTypes.DragEnter(&(GetTreeCtrl()), point);
        }

        if (prev_dest != dest) {
            g_ImgListTypes.DragLeave(&(GetTreeCtrl()));

            GetTreeCtrl().SetItem(prev_dest, TVIF_STATE, NULL, 0, 0,
                                  0, TVIS_DROPHILITED, 0);

            GetTreeCtrl().GetItemRect(prev_dest, &update_rect, TRUE);
            GetTreeCtrl().RedrawWindow(&update_rect);
            g_ImgListTypes.DragEnter(&(GetTreeCtrl()), point);
        }

        // Move the drag image.
        g_ImgListTypes.DragMove(point);

        prev_dest = dest;
    }

    CDialog::OnMouseMove(nFlags, point);
}

HTREEITEM CEntityTreeView::MoveTreeItem(const HTREEITEM item, const HTREEITEM parent)
{
    CDebugTrace trace("CEntityTreeView::MoveTreeitem(HTREEITEM, HTREEITEM)");

    // Don't do anything if the parent hasn't changed.
    if (parent == GetTreeCtrl().GetParentItem(item))
        return item;

    HTREEITEM hitemParent = parent;
    if (hitemParent == NULL)
        hitemParent = TVI_ROOT;

    // Clone the item in the tree.
    char str[64] = {'\0'};
    TVITEM tvItem = { TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE |
                      TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_TEXT,
                      item, 0, 0, str, 64, 0, 0, 0, 0
                    };

    if (GetTreeCtrl().GetItem(&tvItem) == 0)
        return NULL;

    HTREEITEM hitemNodeNew = GetTreeCtrl().InsertItem(str, tvItem.iImage,
                                                      tvItem.iSelectedImage, hitemParent);
    GetTreeCtrl().SetItemData(hitemNodeNew, tvItem.lParam);

    CEntity *entity = g_DataManager.GetEntity(item);
    if (entity)
        entity->SetHtree(hitemNodeNew);

    // Move any children to the new node.
    MoveTreeChildren(item, hitemNodeNew);

    // Delete the old item.
    GetTreeCtrl().DeleteItem(item);

    return hitemNodeNew;
}

void CEntityTreeView::MoveTreeChildren(const HTREEITEM src, const HTREEITEM dest)
{
    CDebugTrace trace("CEntityTreeView::MoveTreeChildren(HTREEITEM HTREEITEM)");

    if (src == dest)
        return;

    if (GetTreeCtrl().ItemHasChildren(src)) {
        HTREEITEM hNext;
        HTREEITEM hChild = GetTreeCtrl().GetChildItem(src);

        while (hChild) {
            hNext = GetTreeCtrl().GetNextSiblingItem(hChild);
            HTREEITEM hNew = MoveTreeItem(hChild, dest);

            CEntity *entity = g_DataManager.GetEntity(hChild);
            if (entity)
                entity->SetHtree(hNew);

            hChild = hNext;
        }
    }
}

// This is easiest done here since the tree stores the relationships
// and the HTREEITEM data.  This way we don't have to search the
// CEntityMap in the data manager.
void CEntityTreeView::RepopulateEntityStateParentList(void)
{
    CDebugTrace trace("CEntityTreeView::RepopulateEntityStateParentList()");

    HTREEITEM hNext;
    HTREEITEM hCurrent = GetTreeCtrl().GetNextItem(TVI_ROOT, TVGN_ROOT);

    theApp.GetMainFrame().GetEntityStateView().ClearParentList();

    while (hCurrent) {
        hNext = GetTreeCtrl().GetNextSiblingItem(hCurrent);
        AddItemToEntityStateParentList(hCurrent);
        hCurrent = hNext;
    }
}

void CEntityTreeView::AddItemToEntityStateParentList(const HTREEITEM hItem)
{
    CDebugTrace trace("CEntityTreeView::AddItemToEntityStateParentList(HTREEITEM)");

    HTREEITEM hSelected = GetTreeCtrl().GetSelectedItem();

    if (hItem != hSelected) {
        // Add the item.
        CEntity *entity = g_DataManager.GetEntity(hItem);
        if (entity)
            theApp.GetMainFrame().GetEntityStateView().AddToParentList(entity);

        // Add its children.
        AddChildrenToEntityStateParentList(hItem);
    }
}

HTREEITEM CEntityTreeView::MoveEntityToParent(CEntity *entity, const int parent_id)
{
    CDebugTrace trace("CEntityTreeView::MoveEntityToParent(CEntity *, int)");

    if (!entity)
        return NULL;

    // Update the tree view.
    CEntity *parent = g_DataManager.GetEntity(parent_id);
    HTREEITEM htreeParent = (parent ? parent->GetHtree() : NULL);
    HTREEITEM htreeItem = MoveTreeItem(entity->GetHtree(), htreeParent);

    GetTreeCtrl().Expand(htreeParent, TVE_EXPAND);
    GetTreeCtrl().SelectItem(htreeItem);
    RepopulateEntityStateParentList();

    return htreeItem;
}

void CEntityTreeView::AddChildrenToEntityStateParentList(const HTREEITEM hParent)
{
    CDebugTrace trace("CEntityTreeView::AddChildrenToEntityStateParentList(HTREEITEM)");

    HTREEITEM hSelected = GetTreeCtrl().GetSelectedItem();
    HTREEITEM hCurrent = GetTreeCtrl().GetChildItem(hParent);

    while (hCurrent) {
        HTREEITEM hNext = GetTreeCtrl().GetNextSiblingItem(hCurrent);

        if (hCurrent != hSelected)
            AddItemToEntityStateParentList(hCurrent);

        hCurrent = hNext;
    }
}
