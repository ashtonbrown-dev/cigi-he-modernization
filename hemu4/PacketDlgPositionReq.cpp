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

// PacketDlgPositionReq.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "CigiView.h"
#include "Entity.h"
#include "globals.h"
#include "PacketDlgPositionReq.h"
#include "ViewGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgPositionReq dialog

CPacketDlgPositionReq::CPacketDlgPositionReq(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgPositionReq::IDD, pParent)
{
    m_ObjectID = -1;
    m_PartID = -1;

    //{{AFX_DATA_INIT(CPacketDlgPositionReq)
    m_LabelObjectID = _T("");
    m_ObjectClass = 0;
    m_CoordSystem = 0;
    m_ReqMode = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgPositionReq::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgPositionReq)
    DDX_Control(pDX, IDC_COMBOBOXEX_OBJECT_ID, m_ComboObjectID);
    DDX_Control(pDX, IDC_COMBOBOXEX_PART_ID, m_ComboPartID);
    DDX_Text(pDX, IDC_LABEL_OBJECT_ID, m_LabelObjectID);
    DDX_Radio(pDX, IDC_RADIO_CLASS_ENTITY, m_ObjectClass);
    DDX_Radio(pDX, IDC_RADIO_COORDSYSTEM_GEODETIC, m_CoordSystem);
    DDX_Radio(pDX, IDC_RADIO_MODE_ONESHOT, m_ReqMode);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgPositionReq, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgPositionReq)
    ON_BN_CLICKED(IDC_RADIO_CLASS_ARTPART, OnRadioClassArtpart)
    ON_BN_CLICKED(IDC_RADIO_CLASS_ENTITY, OnRadioClassEntity)
    ON_BN_CLICKED(IDC_RADIO_CLASS_TRACKER, OnRadioClassTracker)
    ON_BN_CLICKED(IDC_RADIO_CLASS_VIEW, OnRadioClassView)
    ON_BN_CLICKED(IDC_RADIO_CLASS_VIEWGROUP, OnRadioClassViewgroup)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_OBJECT_ID, OnSelchangeObjectId)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_OBJECT_ID, OnEndeditObjectId)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_PART_ID, OnSelchangePartId)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_PART_ID, OnEndeditPartId)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgPositionReq message handlers

void CPacketDlgPositionReq::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgPositionReq::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_POSITION_REQUEST pr = {0};
    pr.packet_id = CIGI_POSITION_REQUEST_OPCODE;
    pr.packet_size = sizeof(CIGI_POSITION_REQUEST);
    pr.object_id = m_ObjectID;
    pr.artpart_id = m_PartID;
    pr.coord_system = m_CoordSystem;
    pr.object_class = m_ObjectClass;
    pr.update_mode = m_ReqMode;

    SendImmedCigiMessage(&pr);
}

BOOL CPacketDlgPositionReq::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the image lists for the combo boxes.
    m_ComboObjectID.SetImageList(&g_ImgListTypes);

    PopulateObjectIDCombo();
    OnRadioClassEntity();
    OnSelchangeObjectId();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgPositionReq::OnRadioClassArtpart()
{
    m_ObjectClass = 1;
    m_LabelObjectID = "Entity ID:";
    UpdateData(FALSE);

    m_ComboObjectID.SetImageList(&g_ImgListTypes);
    PopulateObjectIDCombo();
    OnSelchangeObjectId();
    PopulatePartIDCombo();
    OnSelchangePartId();
    m_ComboPartID.SetCurSel(0);
    m_ComboPartID.EnableWindow(TRUE);
}

void CPacketDlgPositionReq::OnRadioClassEntity()
{
    m_ObjectClass = 0;
    m_LabelObjectID = "Entity ID:";
    UpdateData(FALSE);

    m_ComboObjectID.SetImageList(&g_ImgListTypes);
    PopulateObjectIDCombo();
    OnSelchangeObjectId();
    m_ComboPartID.SetCurSel(-1);
    m_ComboPartID.EnableWindow(FALSE);
}

void CPacketDlgPositionReq::OnRadioClassTracker()
{
    m_ObjectClass = 4;
    m_LabelObjectID = "Tracker ID:";
    UpdateData(FALSE);

    m_ComboObjectID.SetImageList(&g_ImgListIcons);
    PopulateObjectIDCombo();
    OnSelchangeObjectId();
    m_ComboPartID.SetCurSel(-1);
    m_ComboPartID.EnableWindow(FALSE);
}

void CPacketDlgPositionReq::OnRadioClassView()
{
    m_ObjectClass = 2;
    m_LabelObjectID = "View ID:";
    UpdateData(FALSE);

    m_ComboObjectID.SetImageList(&g_ImgListIcons);
    PopulateObjectIDCombo();
    OnSelchangeObjectId();
    m_ComboPartID.SetCurSel(-1);
    m_ComboPartID.EnableWindow(FALSE);
}

void CPacketDlgPositionReq::OnRadioClassViewgroup()
{
    m_ObjectClass = 3;
    m_LabelObjectID = "View Group ID:";
    UpdateData(FALSE);

    m_ComboObjectID.SetImageList(&g_ImgListIcons);
    PopulateObjectIDCombo();
    OnSelchangeObjectId();
    m_ComboPartID.SetCurSel(-1);
    m_ComboPartID.EnableWindow(FALSE);
}

void CPacketDlgPositionReq::OnSelchangeObjectId()
{
    if (m_ObjectClass == 1)     // Articulated Part
        PopulatePartIDCombo();

    // Get the ID of the selected entity.
    int idx = m_ComboObjectID.GetCurSel();
    if (idx > -1)
        m_ObjectID = MAKE_ID(m_ComboObjectID.GetItemData(idx));
}

void CPacketDlgPositionReq::OnEndeditObjectId(NMHDR *pNMHDR, LRESULT *pResult)
{
    int new_id = atoi(((NMCBEENDEDIT *)pNMHDR)->szText);
    if (new_id == m_ObjectID)
        return;

    m_ObjectID = new_id;
    int idx = FindComboItem(m_ComboObjectID, m_ObjectID);
    if (idx > -1) {
        // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
        // select no item after it has finished its stuff.
        ::PostMessage(m_ComboObjectID.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

        // Post a CBN_SELCHANGE notification so OnSelchangeObjectID() will be called.
        // The function must be called after the CB_SELCURSEL message is processed
        // so the new item will be selected.
        ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                      MAKEWPARAM(IDC_COMBOBOXEX_OBJECT_ID, CBN_SELCHANGE),
                      (LPARAM)m_ComboObjectID.GetSafeHwnd());
    } else if (m_ObjectClass == 1)
        PopulatePartIDCombo();  // Normally done by OnSelchangeObjectID().

    *pResult = 0;
}

void CPacketDlgPositionReq::OnSelchangePartId()
{
    // Get the type of the selected parent.
    int idx = m_ComboPartID.GetCurSel();
    if (idx > -1)
        m_PartID = MAKE_ID(m_ComboPartID.GetItemData(idx));
}

void CPacketDlgPositionReq::OnEndeditPartId(NMHDR *pNMHDR, LRESULT *pResult)
{
    int new_id = atoi(((NMCBEENDEDIT *)pNMHDR)->szText);
    if (new_id == m_PartID)
        return;

    m_PartID = new_id;
    int idx = FindComboItem(m_ComboPartID, m_PartID);
    if ((idx == -1) && (m_PartID == -1))
        idx = 0;

    // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
    // select no item after it has finished its stuff.
    if (idx > -1)
        ::PostMessage(m_ComboPartID.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

    // Post a CBN_SELCHANGE notification so OnSelchangePartID() will be called.
    // The function must be called after the CB_SELCURSEL message is processed
    // so the new item will be selected.
    ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                  MAKEWPARAM(IDC_COMBOBOXEX_PART_ID, CBN_SELCHANGE),
                  (LPARAM)m_ComboPartID.GetSafeHwnd());

    *pResult = 0;
}

void CPacketDlgPositionReq::PopulateObjectIDCombo(void)
{
    switch (m_ObjectClass) {
    case 0:     // Entity;
    case 1:     // Articulated Part
        PopulateEntityIDCombo();
        break;

    case 2:     // View
        PopulateViewIDCombo();
        break;

    case 3:     // View Group
        PopulateViewGroupIDCombo();
        break;

    case 4:     // Motion Tracker;
        PopulateTrackerIDCombo();
        break;

    default:
        break;
    }

    m_ComboObjectID.SetCurSel(0);
}

void CPacketDlgPositionReq::PopulateEntityIDCombo(void)
{
    char text[256] = {0};
    int cbo_idx = -1;
    int entity_id = -1;
    CEntity *entity = NULL;
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_GENERIC, IMG_GENERIC, 0, 0, -1
                             };

    m_ComboObjectID.ResetContent();

    CEntityMap &map = g_DataManager.GetEntityMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, entity_id, entity);

        strcpy(text, entity->GetName());
        cboitem.pszText = text;
        cboitem.cchTextMax = strlen(text + 1);
        cboitem.iImage = entity->GetClass();
        cboitem.iSelectedImage = cboitem.iImage;
        cboitem.lParam = MAKE_TREE_DATA(entity->GetID(), entity->GetType(),
                                        entity->GetClass());

        // Add a new item to the list.
        cboitem.iItem = FindComboInsertPoint(m_ComboObjectID, entity->GetID());
        cbo_idx = m_ComboObjectID.InsertItem(&cboitem);
    }
}

void CPacketDlgPositionReq::PopulateViewIDCombo(void)
{
    char text[256] = {0};
    int cbo_idx = -1;
    int view_id = -1;
    CCigiView *view = NULL;
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_VIEW, IMG_VIEW, 0, 0, -1
                             };

    m_ComboObjectID.ResetContent();

    CViewMap &map = g_DataManager.GetViewMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, view_id, view);

        strcpy(text, view->GetName());
        cboitem.pszText = text;
        cboitem.cchTextMax = strlen(text + 1);
        cboitem.lParam = MAKE_TREE_DATA(view->GetViewID(), 0, 0);

        // Add a new item to the list.
        cboitem.iItem = FindComboInsertPoint(m_ComboObjectID, view->GetViewID());
        cbo_idx = m_ComboObjectID.InsertItem(&cboitem);
    }
}

void CPacketDlgPositionReq::PopulateViewGroupIDCombo(void)
{
    char text[256] = {"None"};
    int cbo_idx = -1;
    int group_id = -1;
    int i;
    CViewGroup *group = NULL;
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_EX, IMG_EX, 0, 0, -1
                             };

    m_ComboObjectID.ResetContent();

    // Initialize the structure.
    cboitem.pszText = text;
    cboitem.cchTextMax = strlen(text + 1);
    cboitem.lParam = 0;
    cboitem.iItem = 0;
    cboitem.iImage = IMG_VIEWGROUP;
    cboitem.iSelectedImage = IMG_VIEWGROUP;

    CViewGroup **array = g_DataManager.GetViewGroupArray();
    for (i = 1; i < VIEWGROUP_ARRAY_SIZE; i++) {
        group = array[i];
        if (group) {
            group_id = group->GetGroupID();

            strcpy(text, group->GetName());
            cboitem.cchTextMax = strlen(text + 1);
            cboitem.lParam = MAKE_TREE_DATA(group_id, 0, 0);

            // Add a new item to the list.
            cboitem.iItem = FindComboInsertPoint(m_ComboObjectID, group_id);
            cbo_idx = m_ComboObjectID.InsertItem(&cboitem);
        }
    }
}

void CPacketDlgPositionReq::PopulateTrackerIDCombo(void)
{
    m_ComboObjectID.ResetContent();
}

void CPacketDlgPositionReq::PopulatePartIDCombo(void)
{
    char text[256] = {"None"};
    int cbo_idx = -1;
    int entity_id = -1;
    int part_id = -1;
    CArtPart *artpart = NULL;
    CEntity *entity = g_DataManager.GetEntity(GetObjectIDFromCombo());
    COMBOBOXEXITEM cboitem = {  CBEIF_LPARAM | CBEIF_TEXT,
                                0, NULL, 1, 0, 0, 0, 0, -1
                             };

    m_ComboPartID.ResetContent();

    if (!entity)
        return;

    entity_id = entity->GetID();

    CArtPartMap &map = entity->GetArtPartMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, part_id, artpart);

        sprintf(text, "%d: %s", artpart->GetID(), artpart->GetName());
        cboitem.pszText = text;
        cboitem.cchTextMax = strlen(text + 1);
        cboitem.lParam = MAKE_TREE_DATA(artpart->GetID(), 0, 0);

        // Add a new item to the list.
        cboitem.iItem = FindComboInsertPoint(m_ComboPartID, artpart->GetID());
        cbo_idx = m_ComboPartID.InsertItem(&cboitem);
    }

    m_ComboPartID.SetCurSel(0);
}

int CPacketDlgPositionReq::GetObjectIDFromCombo(void)
{
    int itemdata = m_ComboObjectID.GetItemData(m_ComboObjectID.GetCurSel());
    if (itemdata != -1)
        return MAKE_ID(itemdata);
    else {
        CString text;
        m_ComboObjectID.GetWindowText(text);
        return atoi((LPCTSTR)text);
    }
}

int CPacketDlgPositionReq::GetPartIDFromCombo(void)
{
    int itemdata = m_ComboPartID.GetItemData(m_ComboPartID.GetCurSel());
    if (itemdata != -1)
        return MAKE_ID(itemdata);
    else {
        CString text;
        m_ComboPartID.GetWindowText(text);
        return atoi((LPCTSTR)text);
    }
}

int CPacketDlgPositionReq::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgPositionReq::FindComboInsertPoint(CComboBoxEx &combo, const int id)
{
    int i = 0;
    COMBOBOXEXITEM cboitem = { CBEIF_LPARAM, 0, NULL, 1, 0, 0, 0, 0, -1 };

    while (combo.GetItem(&cboitem)) {
        if ((short)MAKE_ID(cboitem.lParam) > id)
            return i;

        cboitem.iItem = ++i;
    }

    return i;
}
