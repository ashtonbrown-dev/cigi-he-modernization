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
 *  FILENAME:   EntityMissilePropPage.cpp
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
#include "EntityMissilePropPage.h"
#include "Entity.h"
#include "globals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityMissilePropPage dialog

CEntityMissilePropPage::CEntityMissilePropPage(CWnd *pParent /*=NULL*/)
    : CDialog(CEntityMissilePropPage::IDD, pParent)
{
    //{{AFX_DATA_INIT(CEntityMissilePropPage)
    //}}AFX_DATA_INIT
}

void CEntityMissilePropPage::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEntityMissilePropPage)
    DDX_Control(pDX, IDC_BUTTON_FIRE, m_ButtonFire);
    DDX_Control(pDX, IDC_BUTTON_APPLY_COMP, m_ButtonApply);
    DDX_Control(pDX, IDC_LIST_DETONATING, m_ListDetonation);
    DDX_Control(pDX, IDC_LIST_PERSISTENT, m_ListPersistent);
    DDX_Control(pDX, IDC_LIST_TERMINATING, m_ListTerminating);
    DDX_Control(pDX, IDC_COMBOBOXEX_TARGET, m_ComboTarget);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEntityMissilePropPage, CDialog)
    //{{AFX_MSG_MAP(CEntityMissilePropPage)
    ON_BN_CLICKED(IDC_BUTTON_APPLY_COMP, OnButtonApply)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_TARGET, OnSelchangeTarget)
    ON_BN_CLICKED(IDC_BUTTON_FIRE, OnButtonFire)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityMissilePropPage message handlers

void CEntityMissilePropPage::OnOK()
{
}

void CEntityMissilePropPage::OnCancel()
{
}

BOOL CEntityMissilePropPage::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_ListTerminating.Init();
    m_ListPersistent.Init();
    m_ListDetonation.Init();

    m_ComboTarget.SetImageList(&g_ImgListTypes);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CEntityMissilePropPage::OnButtonApply()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (entity) {
        int idx = m_ComboTarget.GetCurSel();
        int id = MAKE_ID(m_ComboTarget.GetItemData(idx));
        entity->SetTarget(id);
        entity->SetInitialTarget(id);
    }

    AddItemsToLinkedList(m_ListTerminating, entity->GetLaunchAnimList());
    AddItemsToLinkedList(m_ListPersistent, entity->GetPersistentLaunchAnimList());
    AddItemsToLinkedList(m_ListDetonation, entity->GetDetonationAnimList());
}

void CEntityMissilePropPage::AddItemsToLinkedList(CCheckList &ctrl, CList<long, long> &list)
{
    list.RemoveAll();

    int count = ctrl.GetItemCount();

    for (int i = 0; i < count; i++) {
        if (ctrl.GetItemCheck(i))
            list.AddTail(ctrl.GetItemData(i));
    }
}

void CEntityMissilePropPage::PopulateTargetCombo(CEntity *entity)
{
    int entity_id = entity->GetID();
    int target_id = -1;
    int cbo_idx = -1;
    CEntityMap &map = g_DataManager.GetEntityMap();
    CEntity *target = NULL;
    POSITION pos = map.GetStartPosition();

    char text[256] = {0};
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_GENERIC, IMG_GENERIC, 0, 0, -1
                             };

    m_ButtonFire.EnableWindow(FALSE);

    m_ComboTarget.ResetContent();
    m_ComboTarget.SetCurSel(-1);

    if (!entity || (entity->GetClass() != ENTITY_CLASS_MISSILE)) {
        m_ComboTarget.EnableWindow(FALSE);
        return;
    }

    m_ComboTarget.EnableWindow(TRUE);

    while (pos) {
        map.GetNextAssoc(pos, target_id, target);
        if (target && (target_id != entity_id)  && (target->GetParent() == -1) &&
            (!entity->IsDescendantOf(target))) {
            strcpy(text, target->GetName());
            cboitem.pszText = text;
            cboitem.cchTextMax = strlen(text + 1);
            cboitem.iImage = target->GetClass();
            cboitem.iSelectedImage = cboitem.iImage;
            cboitem.lParam = MAKE_TREE_DATA(target->GetID(), target->GetType(),
                                            target->GetClass());

            // Add a new item to the list.
            cboitem.iItem = FindEntityComboInsertPoint(m_ComboTarget, target->GetID());
            cbo_idx = m_ComboTarget.InsertItem(&cboitem);

            if (target->GetID() == entity->GetTarget()) {
                // Select the item in the combo box.
                m_ComboTarget.SetCurSel(cbo_idx);

                m_ButtonFire.EnableWindow(TRUE);
            }
        }
    }
}

void CEntityMissilePropPage::PopulateAnimLists(CEntity *entity)
{
    int listidx = -1;
    int id = -1;
    int entity_id = -1;
    CEntity *item = NULL;
    CEntityMap &map = g_DataManager.GetEntityMap();
    POSITION pos = map.GetStartPosition();

    m_ListTerminating.Clear();
    m_ListPersistent.Clear();
    m_ListDetonation.Clear();

    if (!entity) {
        m_ListTerminating.EnableWindow(FALSE);
        m_ListPersistent.EnableWindow(FALSE);
        m_ListDetonation.EnableWindow(FALSE);

        return;
    }

    m_ListTerminating.EnableWindow(TRUE);
    m_ListPersistent.EnableWindow(TRUE);
    m_ListDetonation.EnableWindow(TRUE);

    entity_id = entity->GetID();

    while (pos) {
        map.GetNextAssoc(pos, id, item);

        if (item && (item != entity) && (item->GetClass() == ENTITY_CLASS_ANIMATION) &&
            (item->GetParent() == entity_id)) {
            listidx = m_ListTerminating.AddItem((LPCTSTR)(item->GetName()));
            m_ListTerminating.SetItemData(listidx, id);

            listidx = m_ListPersistent.AddItem((LPCTSTR)(item->GetName()));
            m_ListPersistent.SetItemData(listidx, id);

            listidx = m_ListDetonation.AddItem((LPCTSTR)(item->GetName()));
            m_ListDetonation.SetItemData(listidx, id);
        }
    }
}

void CEntityMissilePropPage::UpdateAnimList(CList<long, long> &list, CCheckList &ctrl)
{
    int id = -1;
    CEntity *entity = NULL;

    ClearCheckList(ctrl);

    POSITION pos = list.GetHeadPosition();
    while (pos) {
        id = list.GetNext(pos);
        entity = g_DataManager.GetEntity(id);
        if (entity && (entity->GetClass() == ENTITY_CLASS_ANIMATION)) {
            ctrl.SetItemCheck(GetCheckListItem(ctrl, id), TRUE);
        }
    }
}

void CEntityMissilePropPage::ClearCheckList(CCheckList &list)
{
    int count = list.GetItemCount();

    for (int i = 0; i < count; i++)
        list.SetItemCheck(i, FALSE);
}

int CEntityMissilePropPage::GetCheckListItem(CCheckList &list, const int id)
{
    int count = list.GetItemCount();

    for (int i = 0; i < count; i++)
        if ((unsigned int)id == list.GetItemData(i))
            return i;

    return -1;
}

void CEntityMissilePropPage::RefreshView(CEntity *entity)
{
    PopulateTargetCombo(entity);
    if (entity->GetTarget() == -1) {
        m_ButtonFire.EnableWindow(FALSE);
        m_ButtonApply.EnableWindow(FALSE);
    } else {
        m_ButtonFire.EnableWindow(TRUE);
        m_ButtonApply.EnableWindow(TRUE);
    }

    PopulateAnimLists(entity);
    UpdateAnimList(entity->GetLaunchAnimList(), m_ListTerminating);
    UpdateAnimList(entity->GetPersistentLaunchAnimList(), m_ListPersistent);
    UpdateAnimList(entity->GetDetonationAnimList(), m_ListDetonation);
}

int CEntityMissilePropPage::FindEntityComboItem(CComboBoxEx &combo, const int entity_id)
{
    int i = 0;
    COMBOBOXEXITEM cboitem = { CBEIF_LPARAM, 0, NULL, 1, 0, 0, 0, 0, -1 };

    // Test the selected item first.
    i = combo.GetCurSel();
    if (i > -1) {
        if ((short)MAKE_ID(combo.GetItemData(i)) == (short)entity_id)
            return i;
    }

    i = 0;

    while (combo.GetItem(&cboitem)) {
        if ((short)MAKE_ID(cboitem.lParam) == (short)entity_id)
            return i;

        cboitem.iItem = ++i;
    }

    return -1;
}

int CEntityMissilePropPage::FindEntityComboInsertPoint(CComboBoxEx &combo, const int entity_id)
{
    int i = 0;
    COMBOBOXEXITEM cboitem = { CBEIF_LPARAM, 0, NULL, 1, 0, 0, 0, 0, -1 };

    while (combo.GetItem(&cboitem)) {
        if ((short)MAKE_ID(cboitem.lParam) > entity_id)
            return i;

        cboitem.iItem = ++i;
    }

    return i;
}

void CEntityMissilePropPage::OnSelchangeTarget()
{
    if (m_ComboTarget.GetCurSel() > -1) {
        m_ButtonApply.EnableWindow(TRUE);
        m_ButtonFire.EnableWindow(TRUE);
    }

    //OnButtonApply();
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (entity) {
        int idx = m_ComboTarget.GetCurSel();
        int id = MAKE_ID(m_ComboTarget.GetItemData(idx));
        entity->SetTarget(id);
        entity->SetInitialTarget(id);
    }
}

void CEntityMissilePropPage::OnButtonFire()
{
    m_ButtonApply.EnableWindow(FALSE);
    m_ButtonFire.EnableWindow(FALSE);

    OnButtonApply();
    FireMissile(g_DataManager.GetSelectedEntity());
}
