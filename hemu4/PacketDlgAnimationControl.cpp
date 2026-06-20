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

// PacketDlgEntityControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "hemu4.h"
#include "PacketDlgAnimationControl.h"
#include "EntityStateView.h"
#include "EntityTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgAnimationControl dialog

CPacketDlgAnimationControl::CPacketDlgAnimationControl(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgAnimationControl::IDD, pParent)
{
    m_EntityID = -1;
	m_AnimationID = -1;

    //{{AFX_DATA_INIT(CPacketDlgAnimationControl)
    m_AnimLoop = FALSE;
    m_InheritAlpha = FALSE;
    m_AnimDirection = 0;
    m_AnimState = 0;
	m_AnimFrameReset = 0;
    m_Alpha = 0;
    m_Speed = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgAnimationControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgAnimationControl)
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_ID, m_ComboEntityID);
    DDX_Control(pDX, IDC_COMBOBOXEX_ANIMATION_ID, m_ComboAnimationID);
    DDX_Check(pDX, IDC_CHECK_ANIM_LOOP, m_AnimLoop);
    DDX_Check(pDX, IDC_CHECK_INHERIT_ALPHA, m_InheritAlpha);
    DDX_Radio(pDX, IDC_RADIO_ANIM_STATE_STOP, m_AnimState);
    DDX_Radio(pDX, IDC_RADIO_ANIM_STATE_CONTINUE, m_AnimFrameReset);


    DDX_Text(pDX, IDC_EDIT_ALPHA, m_Alpha);
    DDX_Text(pDX, IDC_EDIT_SPEED, m_Speed);

    DDV_MinMaxUInt(pDX, m_Alpha, 0, 255);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgAnimationControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgAnimationControl)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_ID, OnSelchangeEntityID)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_ENTITY_ID, OnEndeditEntityID)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ANIMATION_ID, OnSelchangeAnimationID)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_ANIMATION_ID, OnEndeditAnimationID)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgAnimationControl message handlers

BOOL CPacketDlgAnimationControl::OnInitDialog()
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

void CPacketDlgAnimationControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgAnimationControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_ANIMATION_CONTROL ac = {0};
    ac.packet_id = CIGI_ANIMATION_CONTROL_OPCODE;
    ac.packet_size = sizeof(CIGI_ANIMATION_CONTROL);
    ac.entity_id = m_EntityID;
	ac.animation_id = m_AnimationID;
    ac.animation_state = m_AnimState;
	ac.animation_frame_reset = m_AnimFrameReset;
    ac.animation_loop_mode = m_AnimLoop;
    ac.inherit_alpha = m_InheritAlpha;
    ac.alpha = m_Alpha;
	ac.animation_speed = m_Speed;

    SendImmedCigiMessage(&ac);

}


void CPacketDlgAnimationControl::RefreshView(void)
{
    CEntity *entity = g_DataManager.GetEntity(m_EntityID);
    if (!entity)
        return;

    ENTITY state = entity->GetAndLock();
    entity->Unlock();

    m_AnimLoop = state.cigi.anim_loop;
    m_InheritAlpha = state.cigi.inherit_alpha;
    m_AnimDirection = state.cigi.anim_dir;
    m_AnimState = state.cigi.anim_state;
    m_Alpha = state.cigi.alpha;


    UpdateData(FALSE);
}

void CPacketDlgAnimationControl::OnSelchangeEntityID()
{
    // Get the ID of the selected entity.
    int idx = m_ComboEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboEntityID.GetItemData(idx));

    RefreshView();
}

void CPacketDlgAnimationControl::OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult)
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

void CPacketDlgAnimationControl::OnSelchangeAnimationID()
{
    // Get the ID of the selected animation.
    int idx = m_ComboAnimationID.GetCurSel();
    if (idx > -1)
        m_AnimationID = MAKE_ID(m_ComboAnimationID.GetItemData(idx));

    RefreshView();
}

void CPacketDlgAnimationControl::OnEndeditAnimationID(NMHDR *pNMHDR, LRESULT *pResult)
{
    int new_id = atoi(((NMCBEENDEDIT *)pNMHDR)->szText);
    if (new_id == m_AnimationID)
        return;

    m_AnimationID = new_id;
    int idx = FindComboItem(m_ComboAnimationID, m_AnimationID);
    if (idx > -1) {
        // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
        // select no item after it has finished its stuff.
        ::PostMessage(m_ComboAnimationID.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

        // Post a CBN_SELCHANGE notification so OnSelchangeEntityID() will be called.
        // The function must be called after the CB_SELCURSEL message is processed
        // so the new item will be selected.
        ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                      MAKEWPARAM(IDC_COMBOBOXEX_ANIMATION_ID, CBN_SELCHANGE),
                      (LPARAM)m_ComboAnimationID.GetSafeHwnd());
	}
    *pResult = 0;
}


void CPacketDlgAnimationControl::PopulateEntityIDCombo(void)
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
        cboitem.lParam = MAKE_TREE_DATA(entity->GetID(), OBJECT_TYPE_ENTITY,
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

void CPacketDlgAnimationControl::SelectEntityIDComboItem(void)
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

int CPacketDlgAnimationControl::GetEntityIDFromCombo(void)
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

int CPacketDlgAnimationControl::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgAnimationControl::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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
