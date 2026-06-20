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

// PacketDlgCCEntityControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "Hemu4.h"
#include "PacketDlgCCEntityPosition.h"
#include "EntityStateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgCCEntityPosition dialog

CPacketDlgCCEntityPosition::CPacketDlgCCEntityPosition(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgCCEntityPosition::IDD, pParent)
{
    m_EntityID = -1;

    //{{AFX_DATA_INIT(CPacketDlgCCEntityPosition)
    m_Latitude = 0.0;
    m_Longitude = 0.0;
    m_Yaw = 0.0f;
    //}}AFX_DATA_INIT
}

void CPacketDlgCCEntityPosition::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgCCEntityPosition)
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_ID, m_ComboEntityID);
    DDX_Text(pDX, IDC_EDIT_LAT, m_Latitude);
    DDX_Text(pDX, IDC_EDIT_LON, m_Longitude);
    DDX_Text(pDX, IDC_EDIT_YAW, m_Yaw);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgCCEntityPosition, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgCCEntityPosition)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_ID, OnSelchangeEntityID)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_ENTITY_ID, OnEndeditEntityID)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgCCEntityPosition message handlers

void CPacketDlgCCEntityPosition::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_ENTITY_POSITION_CC ec = {0};
    ec.packet_id = CIGI_ENTITY_POSITION_CC_OPCODE;
    ec.packet_size = sizeof(CIGI_ENTITY_POSITION_CC);
    ec.entity_id = m_EntityID;
    ec.yaw = m_Yaw;
    ec.latitude = m_Latitude;
    ec.longitude = m_Longitude;

    SendImmedCigiMessage(&ec);

    ApplyEntity();
}

void CPacketDlgCCEntityPosition::ApplyEntity(void)
{
    // Find the entity and update its data.
    CEntity *entity = g_DataManager.GetEntity(m_EntityID);
    if (!entity)
        return;

    ENTITY state = entity->GetAndLock();
    state.cigi.dofs.latitude = m_Latitude;
    state.cigi.dofs.longitude = m_Longitude;
    state.cigi.dofs.yaw = m_Yaw;
    entity->SetLockedEntityPosition(state);
    entity->Unlock();

    // Update the entity view.
    theApp.GetMainFrame().GetEntityStateView().RefreshView();
}

void CPacketDlgCCEntityPosition::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

BOOL CPacketDlgCCEntityPosition::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the image lists for the combo boxes.
    m_ComboEntityID.SetImageList(&g_ImgListTypes);

    PopulateEntityIDCombo();
    SelectEntityIDComboItem();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgCCEntityPosition::RefreshView(void)
{
    CEntity *entity = g_DataManager.GetEntity(m_EntityID);
    if (!entity)
        return;

    DOF dof = entity->GetDofs();
    m_Latitude = dof.latitude;
    m_Longitude = dof.longitude;
    m_Yaw = (float)dof.yaw;

    UpdateData(FALSE);
}

void CPacketDlgCCEntityPosition::OnSelchangeEntityID()
{
    // Get the ID of the selected entity.
    int idx = m_ComboEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboEntityID.GetItemData(idx));

    RefreshView();
}

void CPacketDlgCCEntityPosition::OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult)
{
    int new_id = atoi(((NMCBEENDEDIT *)pNMHDR)->szText);
    if (new_id == m_EntityID)
        return;

    m_EntityID = new_id;
    int idx = FindComboItem(m_ComboEntityID, m_EntityID);
    if (idx > -1) {
        // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
        // select no item after it has finished its stuff.
        ::PostMessage(m_ComboEntityID.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

        // Post a CBN_SELCHANGE notification so OnSelchangeEntityID() will be called.
        // The function must be called after the CB_SELCURSEL message is processed
        // so the new item will be selected.
        ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                      MAKEWPARAM(IDC_COMBOBOXEX_ENTITY_ID, CBN_SELCHANGE),
                      (LPARAM)m_ComboEntityID.GetSafeHwnd());
    }

    *pResult = 0;
}

void CPacketDlgCCEntityPosition::PopulateEntityIDCombo(void)
{
    char text[256] = {0};
    int cbo_idx = -1;
    int entity_id = -1;
    CEntity *entity = NULL;
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_GENERIC, IMG_GENERIC, 0, 0, -1
                             };

    m_ComboEntityID.ResetContent();

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
        cboitem.iItem = FindComboInsertPoint(m_ComboEntityID, entity->GetID());
        cbo_idx = m_ComboEntityID.InsertItem(&cboitem);

        if (entity->GetID() == entity_id) {
            // Select the item in the combo box.
            m_ComboEntityID.SetCurSel(cbo_idx);
        }
    }
}

void CPacketDlgCCEntityPosition::SelectEntityIDComboItem(void)
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity) {
        m_ComboEntityID.SetCurSel(-1);
        return;
    }

    m_EntityID = entity->GetID();
    m_ComboEntityID.SetCurSel(FindComboItem(m_ComboEntityID, m_EntityID));
    OnSelchangeEntityID();
}

int CPacketDlgCCEntityPosition::GetEntityIDFromCombo(void)
{
    int itemdata = m_ComboEntityID.GetItemData(m_ComboEntityID.GetCurSel());
    if (itemdata != -1)
        return MAKE_ID(itemdata);
    else {
        CString text;
        m_ComboEntityID.GetWindowText(text);
        return atoi((LPCTSTR)text);
    }
}

int CPacketDlgCCEntityPosition::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgCCEntityPosition::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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
