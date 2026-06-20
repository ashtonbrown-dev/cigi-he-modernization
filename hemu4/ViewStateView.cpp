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

// ViewStateView.cpp : implementation file
//

#include "stdafx.h"
#include "CigiView.h"
#include "Entity.h"
#include "globals.h"
#include "Hemu4.h"
#include "ViewGroup.h"
#include "ViewStateView.h"
#include "ViewTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewStateView dialog

CViewStateView::CViewStateView(CWnd *pParent /*=NULL*/)
    : CBaseStateView(CViewStateView::IDD, pParent)
{
    //{{AFX_DATA_INIT(CViewStateView)
    m_MirrorMode = 0;
    m_PixelReplication = 0;
    m_Yaw = 0.0f;
    m_Pitch = 0.0f;
    m_Roll = 0.0f;
    m_X = 0.0f;
    m_Y = 0.0f;
    m_Z = 0.0f;
    m_ViewType = 0;
    m_ViewID = 0;
    //}}AFX_DATA_INIT

    m_bHoldPixelrep = FALSE;
    m_bHoldMirror = FALSE;
}

void CViewStateView::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CViewStateView)
    DDX_Control(pDX, IDC_RADIO_PIXELREP_NONE, m_RadioPixelRepNone);
    DDX_Control(pDX, IDC_RADIO_PIXELREP_1_2, m_RadioPixelRep1x2);
    DDX_Control(pDX, IDC_RADIO_PIXELREP_2_1, m_RadioPixelRep2x1);
    DDX_Control(pDX, IDC_RADIO_PIXELREP_2_2, m_RadioPixelRep2x2);
    DDX_Control(pDX, IDC_RADIO_MIRROR_NONE, m_RadioMirrorNone);
    DDX_Control(pDX, IDC_RADIO_MIRROR_HORZ, m_RadioMirrorHorz);
    DDX_Control(pDX, IDC_RADIO_MIRROR_VERT, m_RadioMirrorVert);
    DDX_Control(pDX, IDC_RADIO_MIRROR_HORZ_VERT, m_RadioMirrorHorzVert);
    DDX_Control(pDX, IDC_TAB1, m_Tab);
    DDX_Control(pDX, IDC_EDIT_VIEW_TYPE, m_EditViewType);
    DDX_Control(pDX, IDC_EDIT_VIEWPOS_Z, m_EditZ);
    DDX_Control(pDX, IDC_EDIT_VIEWPOS_Y, m_EditY);
    DDX_Control(pDX, IDC_EDIT_VIEWPOS_X, m_EditX);
    DDX_Control(pDX, IDC_EDIT_VIEW_ID, m_EditViewID);
    DDX_Control(pDX, IDC_EDIT_CAMERA_ROLL, m_EditRoll);
    DDX_Control(pDX, IDC_EDIT_CAMERA_PITCH, m_EditPitch);
    DDX_Control(pDX, IDC_EDIT_CAMERA_HEADING, m_EditYaw);
    DDX_Control(pDX, IDC_COMBO_VIEW_GROUP, m_ComboViewGroup);
    DDX_Control(pDX, IDC_COMBO_ENTITY, m_ComboEntity);
    DDX_Control(pDX, IDC_BUTTON_SLEW, m_ButtonSlew);
    DDX_Control(pDX, IDC_BUTTON_REORDER_TO_TOP, m_ButtonReorder);
    DDX_Radio(pDX, IDC_RADIO_MIRROR_NONE, m_MirrorMode);
    DDX_Radio(pDX, IDC_RADIO_PIXELREP_NONE, m_PixelReplication);
    DDX_Text(pDX, IDC_EDIT_VIEW_TYPE, m_ViewType);
    DDX_Text(pDX, IDC_EDIT_VIEW_ID, m_ViewID);
    //}}AFX_DATA_MAP

    DDX_TextPrecision(pDX, IDC_EDIT_CAMERA_HEADING, m_Yaw, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_CAMERA_PITCH, m_Pitch, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_CAMERA_ROLL, m_Roll, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_VIEWPOS_X, m_X, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_VIEWPOS_Y, m_Y, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_VIEWPOS_Z, m_Z, 5);
}

BEGIN_MESSAGE_MAP(CViewStateView, CBaseStateView)
    //{{AFX_MSG_MAP(CViewStateView)
    ON_BN_CLICKED(IDC_RADIO_PIXELREP_NONE, OnRadioPixelrepNone)
    ON_BN_CLICKED(IDC_RADIO_PIXELREP_1_2, OnRadioPixelrep1x2)
    ON_BN_CLICKED(IDC_RADIO_PIXELREP_2_1, OnRadioPixelrep2x1)
    ON_BN_CLICKED(IDC_RADIO_PIXELREP_2_2, OnRadioPixelrep2x2)
    ON_BN_CLICKED(IDC_RADIO_MIRROR_NONE, OnRadioMirrorNone)
    ON_BN_CLICKED(IDC_RADIO_MIRROR_HORZ, OnRadioMirrorHorz)
    ON_BN_CLICKED(IDC_RADIO_MIRROR_VERT, OnRadioMirrorVert)
    ON_BN_CLICKED(IDC_RADIO_MIRROR_HORZ_VERT, OnRadioMirrorHorzVert)
    ON_CBN_SELCHANGE(IDC_COMBO_ENTITY, OnSelchangeComboEntity)
    ON_CBN_SELCHANGE(IDC_COMBO_VIEW_GROUP, OnSelchangeComboViewGroup)
    ON_BN_CLICKED(IDC_BUTTON_REORDER_TO_TOP, OnButtonReorderToTop)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab)
    ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnSelchangingTab)
    //}}AFX_MSG_MAP

    ON_MESSAGE(WM_USEREDIT, OnUserEditControl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewStateView message handlers

void CViewStateView::OnOK()
{
}

void CViewStateView::OnCancel()
{
}

BOOL CViewStateView::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_ComboEntity.SetImageList(&g_ImgListTypes);
    m_ComboViewGroup.SetImageList(&g_ImgListIcons);

    InitTabs();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewStateView::InitTabs(void)
{
    int i;

    // Add a tab for each of the child dialog boxes.
    m_Tab.InsertItem(0, "Projection");
    m_Tab.InsertItem(1, "Components");

    // Lock the resources for the child dialog boxes.
    m_DlgTempl[0] = LockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE_VIEW_FRUSTUM));
    m_DlgTempl[1] = LockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE_ENTITY_COMPONENTS));

    // Create an array of pointers to the child dialog boxes.
    m_TabDlg[0] = (CDialog *)&m_DlgFrustum;
    m_TabDlg[1] = (CDialog *)&m_DlgComponents;

    // Create child dialog boxes.
    int tabcount = m_Tab.GetItemCount();
    ASSERT(tabcount == NUM_VIEWTAB_PAGES);

    for (i = 0; i < tabcount; i++) {
        m_TabDlg[i]->CreateIndirect(m_DlgTempl[i], &m_Tab);
        m_TabDlg[i]->SetWindowPos(NULL, 5, 25, 0, 0, SWP_NOSIZE);
    }

    // Simulate selection of the first item.
    SelectTab(0);
}

void CViewStateView::PopulateCompPropPage(CCigiView *view)
{
    m_DlgComponents.ClearComponents();

    int id = 0;
    CComponent *compptr = NULL;
    CComponentMap &compmap = view->GetComponentMap();
    POSITION pos = compmap.GetStartPosition();
    while (pos) {
        compmap.GetNextAssoc(pos, id, compptr);
        m_DlgComponents.AddComponent(compptr);
    }
}

void CViewStateView::RefreshView(void)
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)      // if we have a view group, do nothing
        return;

    const int viewId = view->GetViewID();

    // Gray out the numeric fields during Run mode, so that the user
    // cannot edit the values.
    m_EditViewType.EnableWindow(g_bFreezeFlag);
    m_EditZ.EnableWindow(g_bFreezeFlag);
    m_EditY.EnableWindow(g_bFreezeFlag);
    m_EditX.EnableWindow(g_bFreezeFlag);
    m_EditViewID.EnableWindow(g_bFreezeFlag);
    m_EditRoll.EnableWindow(g_bFreezeFlag);
    m_EditPitch.EnableWindow(g_bFreezeFlag);
    m_EditYaw.EnableWindow(g_bFreezeFlag);

    m_ViewID = viewId;
    m_ViewType = view->GetViewType();
    m_Yaw = view->GetYaw();
    m_Pitch = view->GetPitch();
    m_Roll = view->GetRoll();
    m_X = view->GetXOffset();
    m_Y = view->GetYOffset();
    m_Z = view->GetZOffset();

    if (!m_bHoldPixelrep)
        m_PixelReplication = view->GetReplicationMode();
    else
        m_bHoldPixelrep = FALSE;

    if (!m_bHoldMirror)
        m_MirrorMode = view->GetMirrorMode();
    else
        m_bHoldMirror = FALSE;

    UpdateData(FALSE);

    // If the view belongs to a group, we don't want to allow the
    // user to select entities.  Force the view's entity assignment
    // to that of the group.
    int entity_id = view->GetEntityID();
    int group_id = view->GetGroupID();
    if (group_id) {
        CViewGroup *group = g_DataManager.GetViewGroup(group_id);
        if (group) {
            // Set the view's entity ID to that of the group.  We
            // will call SelectEntityComboItem() with this ID
            // because the driver won't have had time to update the
            // view object.
            entity_id = group->GetEntityID();
            if (entity_id != view->GetEntityID())
                view->SetEntityID(entity_id);

            m_ComboEntity.EnableWindow(FALSE);
        } else {
            // Maybe the user used a script or Send View Definition packet to
            // assign a view to a bogus view group.
            m_ComboEntity.EnableWindow(TRUE);
        }
    } else {
        m_ComboEntity.EnableWindow(TRUE);
    }

    // Select the group to which the view is assigned.
    SelectViewGroupComboItem(group_id);

    // Select the entity to which the view is attached.
    PopulateEntityList();
    SelectEntityComboItem(entity_id);

    m_DlgFrustum.RefreshView();

    // Update the Components tab.
    if (m_PrevViewId != viewId) {
        PopulateCompPropPage(view);
        m_PrevViewId = viewId;
    }
}

void CViewStateView::ClearEntityList(void)
{
    m_ComboEntity.ResetContent();
}

void CViewStateView::PopulateEntityList(void)
{
    m_ComboEntity.ResetContent();

    CEntity *entity = NULL;
    CCigiView *view = g_DataManager.GetSelectedView();

    char text[256] = {0};
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_GENERIC, IMG_GENERIC, 0, 0, -1
                             };

    int attached_to = -1;
    int cbo_idx = -1;
    int entity_id = -1;

    if (view)
        view->GetEntityID();

    CEntityMap &map = g_DataManager.GetEntityMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, entity_id, entity);

        strcpy(text, entity->GetName());
        cboitem.pszText = text;
        cboitem.cchTextMax = strlen(text + 1);
        cboitem.iImage = entity->GetClass();
        cboitem.iSelectedImage = cboitem.iImage;
        cboitem.lParam = MAKE_TREE_DATA(entity_id, entity->GetType(),
                                        entity->GetClass());

        // Add a new item to the list.
        cboitem.iItem = FindComboInsertPoint(m_ComboEntity, entity_id);
        cbo_idx = m_ComboEntity.InsertItem(&cboitem);

        if (entity->GetID() == attached_to) {
            // Select the item in the combo box.
            m_ComboEntity.SetCurSel(cbo_idx);
        }
    }
}

void CViewStateView::SelectViewGroupComboItem(const int group_id)
{
    int id = FindComboItem(m_ComboViewGroup, group_id);
    if (id == -1)
        m_ComboViewGroup.SetCurSel(0);  // "None"
    else
        m_ComboViewGroup.SetCurSel(id);
}

void CViewStateView::SelectEntityComboItem(const int entity_id)
{
    int id = FindComboItem(m_ComboEntity, entity_id);
    m_ComboEntity.SetCurSel(id);
}

void CViewStateView::PopulateViewGroupList(void)
{
    char text[256] = {"None"};
    int i = 0;
    CViewGroup *group = NULL;
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_EX, IMG_EX, 0, 0, -1
                             };

    m_ComboViewGroup.ResetContent();

    // Add the "None" item to the top of the list.
    cboitem.pszText = text;
    m_ComboViewGroup.InsertItem(&cboitem);

    // Now add an item for each group.
    cboitem.iImage = IMG_VIEWGROUP;
    cboitem.iSelectedImage = IMG_VIEWGROUP;
    for (i = 1; i < 256; i++) {
        group = g_DataManager.GetViewGroup(i);

        if (group) {
            strcpy(text, group->GetName());
            cboitem.pszText = text;
            cboitem.cchTextMax = strlen(text + 1);
            cboitem.lParam = MAKE_TREE_DATA(group->GetGroupID(), 0, 0);

            // Add a new item to the list.
            cboitem.iItem = FindComboInsertPoint(m_ComboViewGroup, group->GetGroupID());
            m_ComboViewGroup.InsertItem(&cboitem);
        }
    }
}

int CViewStateView::FindComboItem(CComboBoxEx &combo, const int id)
{
    int i = 0;
    COMBOBOXEXITEM cboitem = { CBEIF_LPARAM, 0, NULL, 1, 0, 0, 0, 0, -1 };

    // Test the selected item first.
    i = combo.GetCurSel();
    if (i > -1) {
        if ((short)MAKE_ID(combo.GetItemData(i)) == (short)id)
            return i;
    }

    i = 0;

    while (combo.GetItem(&cboitem)) {
        if ((short)MAKE_ID(cboitem.lParam) == (short)id)
            return i;

        cboitem.iItem = ++i;
    }

    return -1;
}

int CViewStateView::FindComboInsertPoint(CComboBoxEx &combo, const int item_id)
{
    int i = 0;
    COMBOBOXEXITEM cboitem = { CBEIF_LPARAM, 0, NULL, 1, 0, 0, 0, 0, -1 };

    while (combo.GetItem(&cboitem)) {
        if ((short)MAKE_ID(cboitem.lParam) > item_id)
            return i;

        cboitem.iItem = ++i;
    }

    return i;
}

void CViewStateView::OnRadioPixelrepNone()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldPixelrep = TRUE;

    view->SetReplicationMode(PIXEL_REPLICATION_MODE_NONE);
}

void CViewStateView::OnRadioPixelrep1x2()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldPixelrep = TRUE;

    view->SetReplicationMode(PIXEL_REPLICATION_MODE_1X2);
}

void CViewStateView::OnRadioPixelrep2x1()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldPixelrep = TRUE;

    view->SetReplicationMode(PIXEL_REPLICATION_MODE_2X1);
}

void CViewStateView::OnRadioPixelrep2x2()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldPixelrep = TRUE;

    view->SetReplicationMode(PIXEL_REPLICATION_MODE_2X2);
}

void CViewStateView::OnRadioMirrorNone()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldMirror = TRUE;

    view->SetMirrorMode(MIRROR_MODE_NONE);
}

void CViewStateView::OnRadioMirrorHorz()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldMirror = TRUE;

    view->SetMirrorMode(MIRROR_MODE_HORZ);
}

void CViewStateView::OnRadioMirrorVert()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldMirror = TRUE;

    view->SetMirrorMode(MIRROR_MODE_VERT);
}

void CViewStateView::OnRadioMirrorHorzVert()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldMirror = TRUE;

    view->SetMirrorMode(MIRROR_MODE_BOTH);
}

LRESULT CViewStateView::OnUserEditControl(WPARAM wParam, LPARAM lParam)
{
    //long key = (long)wParam;
    CEditKbd *ctrl = (CEditKbd *)lParam;

    if (ctrl == &m_EditViewType)
        OnUserChangeType();
    else if (ctrl == &m_EditZ)
        OnUserChangeZOffset();
    else if (ctrl == &m_EditY)
        OnUserChangeYOffset();
    else if (ctrl == &m_EditX)
        OnUserChangeXOffset();
    else if (ctrl == &m_EditRoll)
        OnUserChangeRoll();
    else if (ctrl == &m_EditPitch)
        OnUserChangePitch();
    else if (ctrl == &m_EditYaw)
        OnUserChangeYaw();

    return 0;
}

void CViewStateView::OnUserChangeType()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    UpdateData(TRUE);

    view->SetViewType(m_ViewType);
}

void CViewStateView::OnUserChangeXOffset()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    UpdateData(TRUE);

    view->SetXOffset(m_X);
}

void CViewStateView::OnUserChangeYOffset()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    UpdateData(TRUE);

    view->SetYOffset(m_Y);
}

void CViewStateView::OnUserChangeZOffset()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    UpdateData(TRUE);

    view->SetZOffset(m_Z);
}

void CViewStateView::OnUserChangeYaw()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    UpdateData(TRUE);

    view->SetYaw(m_Yaw);
}

void CViewStateView::OnUserChangePitch()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    UpdateData(TRUE);

    view->SetPitch(m_Pitch);
}

void CViewStateView::OnUserChangeRoll()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    UpdateData(TRUE);

    view->SetRoll(m_Roll);
}

void CViewStateView::OnSelchangeComboEntity()
{
    //CEntity *entity = g_DataManager.GetSelectedEntity();
    //if (!entity)
    //  return;

    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    DWORD lParam = m_ComboEntity.GetItemData(m_ComboEntity.GetCurSel());

    view->SetEntityID(MAKE_ID(lParam));
}

void CViewStateView::OnSelchangeComboViewGroup()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (!view)
        return;

    DWORD lParam = m_ComboViewGroup.GetItemData(m_ComboViewGroup.GetCurSel());
    int group_id = MAKE_ID(lParam);
    if (group_id > 255)
        group_id = 0;

    view->SetGroupID(group_id);

    // Update the tree view.
    HTREEITEM htreeGroup;
    CViewTreeView &treeview = theApp.GetMainFrame().GetViewTreeView();
    CViewGroup *group = g_DataManager.GetViewGroup(group_id);
    if (group)
        htreeGroup = group->GetHtree();
    else
        htreeGroup = NULL;

    HTREEITEM htreeItem = treeview.MoveTreeItem(view->GetHtree(), htreeGroup);
    view->SetHtree(htreeItem);
    treeview.GetTreeCtrl().Expand(htreeGroup, TVE_EXPAND);
    treeview.GetTreeCtrl().SelectItem(htreeItem);

    // Make sure the correct group is selected from the list, since the
    // driver will not have had time to update the object.
    SelectViewGroupComboItem(group_id);

    // Also, enable/disable the entity combo for the same reason.
    if (group_id == 0)
        m_ComboEntity.EnableWindow(TRUE);
    else
        m_ComboEntity.EnableWindow(FALSE);

    // And force the view's entity ID to be the same as the group's.
    if (group) {
        view->SetEntityID(group->GetEntityID());
        SelectEntityComboItem(group->GetEntityID());
    }
}

void CViewStateView::OnButtonReorderToTop()
{
    CCigiView *view = g_DataManager.GetSelectedView();
    if (view)
        view->SetReorderFlag(TRUE);
}


void CViewStateView::OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Hide the current window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_HIDE);
    }

    *pResult = 0;
}

void CViewStateView::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Show the appropriate window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_SHOW);
    }

    *pResult = 0;
}

void CViewStateView::SelectTab(const int index)
{
    // Simulate selection of the item.
    LRESULT result;
    OnSelchangingTab(NULL, &result);

    m_Tab.SetCurSel(index);

    OnSelchangeTab(NULL, &result);
}
