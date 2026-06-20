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

// ViewGroupStateView.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "entity.h"
#include "ViewGroup.h"
#include "ViewGroupStateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewGroupStateView dialog

CViewGroupStateView::CViewGroupStateView(CWnd *pParent /*=NULL*/)
    : CBaseStateView(CViewGroupStateView::IDD, pParent)
{
    //{{AFX_DATA_INIT(CViewGroupStateView)
    m_Yaw = 0.0f;
    m_Pitch = 0.0f;
    m_Roll = 0.0f;
    m_X = 0.0f;
    m_Y = 0.0f;
    m_Z = 0.0f;
    m_GroupID = 0;
    //}}AFX_DATA_INIT
}

void CViewGroupStateView::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CViewGroupStateView)
    DDX_Control(pDX, IDC_TAB1, m_Tab);
    DDX_Control(pDX, IDC_EDIT_VIEWPOS_Z, m_EditZ);
    DDX_Control(pDX, IDC_EDIT_VIEWPOS_Y, m_EditY);
    DDX_Control(pDX, IDC_EDIT_VIEWPOS_X, m_EditX);
    DDX_Control(pDX, IDC_EDIT_VIEW_ID, m_EditGroupID);
    DDX_Control(pDX, IDC_EDIT_CAMERA_ROLL, m_EditRoll);
    DDX_Control(pDX, IDC_EDIT_CAMERA_PITCH, m_EditPitch);
    DDX_Control(pDX, IDC_EDIT_CAMERA_HEADING, m_EditYaw);
    DDX_Control(pDX, IDC_COMBO_ENTITY, m_ComboEntity);
    DDX_Control(pDX, IDC_BUTTON_SLEW, m_ButtonSlew);
    DDX_Text(pDX, IDC_EDIT_VIEW_ID, m_GroupID);
    //}}AFX_DATA_MAP

    DDX_TextPrecision(pDX, IDC_EDIT_CAMERA_HEADING, m_Yaw, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_CAMERA_PITCH, m_Pitch, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_CAMERA_ROLL, m_Roll, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_VIEWPOS_X, m_X, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_VIEWPOS_Y, m_Y, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_VIEWPOS_Z, m_Z, 5);
}

BEGIN_MESSAGE_MAP(CViewGroupStateView, CBaseStateView)
    //{{AFX_MSG_MAP(CViewGroupStateView)
    ON_CBN_SELCHANGE(IDC_COMBO_ENTITY, OnSelchangeComboEntity)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab)
    ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnSelchangingTab)
    //}}AFX_MSG_MAP

    ON_MESSAGE(WM_USEREDIT, OnUserEditControl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewGroupStateView message handlers

void CViewGroupStateView::OnOK()
{
}

void CViewGroupStateView::OnCancel()
{
}

BOOL CViewGroupStateView::OnInitDialog()
{
    CDialog::OnInitDialog();

    InitTabs();

    m_ComboEntity.SetImageList(&g_ImgListTypes);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewGroupStateView::RefreshView(void)
{
    CViewGroup *group = g_DataManager.GetSelectedViewGroup();
    if (!group)     // if we have a view, do nothing
        return;

    const int groupId = group->GetGroupID();

    // Gray out the numeric fields during Run mode, so that the user
    // cannot edit the values.
    m_EditZ.EnableWindow(g_bFreezeFlag);
    m_EditY.EnableWindow(g_bFreezeFlag);
    m_EditX.EnableWindow(g_bFreezeFlag);
    m_EditGroupID.EnableWindow(g_bFreezeFlag);
    m_EditRoll.EnableWindow(g_bFreezeFlag);
    m_EditPitch.EnableWindow(g_bFreezeFlag);
    m_EditYaw.EnableWindow(g_bFreezeFlag);

    m_GroupID = groupId;
    m_Yaw = group->GetYaw();
    m_Pitch = group->GetPitch();
    m_Roll = group->GetRoll();
    m_X = group->GetXOffset();
    m_Y = group->GetYOffset();
    m_Z = group->GetZOffset();

    UpdateData(FALSE);

    // Select the entity to which the view group is attached.
    PopulateEntityList();
    int id = FindComboItem(m_ComboEntity, group->GetEntityID());
    m_ComboEntity.SetCurSel(id);

    // Update the Components tab.
    if (m_PrevGroupId != groupId) {
        PopulateCompPropPage(group);
        m_PrevGroupId = groupId;
    }
}

void CViewGroupStateView::ClearEntityList(void)
{
    m_ComboEntity.ResetContent();
}

void CViewGroupStateView::PopulateEntityList(void)
{
    m_ComboEntity.ResetContent();

    CEntity *entity = NULL;
    CViewGroup *group = g_DataManager.GetSelectedViewGroup();

    char text[256] = {0};
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_GENERIC, IMG_GENERIC, 0, 0, -1
                             };

    int attached_to = -1;
    int cbo_idx = -1;
    int entity_id = -1;

    if (group)
        group->GetEntityID();

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

int CViewGroupStateView::FindComboItem(CComboBoxEx &combo, const int id)
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

int CViewGroupStateView::FindComboInsertPoint(CComboBoxEx &combo, const int item_id)
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

LRESULT CViewGroupStateView::OnUserEditControl(WPARAM wParam, LPARAM lParam)
{
    //long key = (long)wParam;
    CEditKbd *ctrl = (CEditKbd *)lParam;

    if (ctrl == &m_EditZ)
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

void CViewGroupStateView::OnUserChangeXOffset()
{
    CViewGroup *group = g_DataManager.GetSelectedViewGroup();
    if (!group)
        return;

    UpdateData(TRUE);

    group->SetXOffset(m_X);
}

void CViewGroupStateView::OnUserChangeYOffset()
{
    CViewGroup *group = g_DataManager.GetSelectedViewGroup();
    if (!group)
        return;

    UpdateData(TRUE);

    group->SetYOffset(m_Y);
}

void CViewGroupStateView::OnUserChangeZOffset()
{
    CViewGroup *group = g_DataManager.GetSelectedViewGroup();
    if (!group)
        return;

    UpdateData(TRUE);

    group->SetZOffset(m_Z);
}

void CViewGroupStateView::OnUserChangeYaw()
{
    CViewGroup *group = g_DataManager.GetSelectedViewGroup();
    if (!group)
        return;

    UpdateData(TRUE);

    group->SetYaw(m_Yaw);
}

void CViewGroupStateView::OnUserChangePitch()
{
    CViewGroup *group = g_DataManager.GetSelectedViewGroup();
    if (!group)
        return;

    UpdateData(TRUE);

    group->SetPitch(m_Pitch);
}

void CViewGroupStateView::OnUserChangeRoll()
{
    CViewGroup *group = g_DataManager.GetSelectedViewGroup();
    if (!group)
        return;

    UpdateData(TRUE);

    group->SetRoll(m_Roll);
}

void CViewGroupStateView::OnSelchangeComboEntity()
{
    //CEntity *entity = g_DataManager.GetSelectedEntity();
    //if (!entity)
    //  return;

    CViewGroup *group = g_DataManager.GetSelectedViewGroup();
    if (!group)
        return;

    DWORD lParam = m_ComboEntity.GetItemData(m_ComboEntity.GetCurSel());

    group->SetEntityID(MAKE_ID(lParam));
}

void CViewGroupStateView::InitTabs(void)
{
    // Add a tab for each of the child dialog boxes.
    m_Tab.InsertItem(0, "Components");

    // Lock the resources for the child dialog boxes.
    m_DlgTempl[0] = LockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE_ENTITY_COMPONENTS));

    // Create an array of pointers to the child dialog boxes.
    m_TabDlg[0] = (CDialog *)&m_DlgComponents;

    // Create child dialog boxes.
    int tabcount = m_Tab.GetItemCount();
    ASSERT(tabcount == NUM_VIEWGROUPTAB_PAGES);

    for (int i = 0; i < tabcount; i++) {
        m_TabDlg[i]->CreateIndirect(m_DlgTempl[i], &m_Tab);
        m_TabDlg[i]->SetWindowPos(NULL, 5, 25, 0, 0, SWP_NOSIZE);
    }

    // Simulate selection of the first item.
    SelectTab(0);
}

void CViewGroupStateView::PopulateCompPropPage(CViewGroup *group)
{
    m_DlgComponents.ClearComponents();

    int id = 0;
    CComponent *compptr = NULL;
    CComponentMap &compmap = group->GetComponentMap();
    POSITION pos = compmap.GetStartPosition();
    while (pos) {
        compmap.GetNextAssoc(pos, id, compptr);
        m_DlgComponents.AddComponent(compptr);
    }
}


void CViewGroupStateView::OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Hide the current window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_HIDE);
    }

    *pResult = 0;
}

void CViewGroupStateView::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Show the appropriate window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_SHOW);
    }

    *pResult = 0;
}

void CViewGroupStateView::SelectTab(const int index)
{
    // Simulate selection of the item.
    LRESULT result;
    OnSelchangingTab(NULL, &result);

    m_Tab.SetCurSel(index);

    OnSelchangeTab(NULL, &result);
}
