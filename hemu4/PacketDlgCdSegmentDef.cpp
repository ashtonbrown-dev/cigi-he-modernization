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

// PacketDlgCdSegmentDef.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "PacketDlgCdSegmentDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgCdSegmentDef dialog


CPacketDlgCdSegmentDef::CPacketDlgCdSegmentDef(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgCdSegmentDef::IDD, pParent)
{
    m_EntityID = -1;

    //{{AFX_DATA_INIT(CPacketDlgCdSegmentDef)
    m_SegmentEnable = FALSE;
    m_X1 = 0.0f;
    m_X2 = 0.0f;
    m_Y1 = 0.0f;
    m_Y2 = 0.0f;
    m_Z1 = 0.0f;
    m_Z2 = 0.0f;
    m_SegmentID = 0;
    m_MaterialMask = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgCdSegmentDef::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgCdSegmentDef)
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_ID, m_ComboEntityID);
    DDX_Check(pDX, IDC_CHECK_ENABLE_SEGMENT, m_SegmentEnable);
    DDX_Text(pDX, IDC_EDIT_X1, m_X1);
    DDX_Text(pDX, IDC_EDIT_X2, m_X2);
    DDX_Text(pDX, IDC_EDIT_Y1, m_Y1);
    DDX_Text(pDX, IDC_EDIT_Y2, m_Y2);
    DDX_Text(pDX, IDC_EDIT_Z1, m_Z1);
    DDX_Text(pDX, IDC_EDIT_Z2, m_Z2);
    DDX_Text(pDX, IDC_EDIT_SEGMENT_ID, m_SegmentID);
    DDV_MinMaxInt(pDX, m_SegmentID, 0, 255);
    DDX_Text(pDX, IDC_EDIT_MATERIAL_MASK, m_MaterialMask);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgCdSegmentDef, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgCdSegmentDef)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_ID, OnSelchangeEntityId)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_ENTITY_ID, OnEndeditEntityId)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgCdSegmentDef message handlers

void CPacketDlgCdSegmentDef::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgCdSegmentDef::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_COLL_SEGMENT_DEF cd = {0};
    cd.packet_id = CIGI_COLL_SEGMENT_DEF_OPCODE;
    cd.packet_size = sizeof(CIGI_COLL_SEGMENT_DEF);
    cd.entity_id = GetEntityIDFromCombo();
    cd.segment_id = m_SegmentID;
    cd.segment_enable = m_SegmentEnable;
    cd.x1 = m_X1;
    cd.y1 = m_Y1;
    cd.z1 = m_Z1;
    cd.x2 = m_X2;
    cd.y2 = m_Y2;
    cd.z2 = m_Z2;
    cd.material_mask = m_MaterialMask;

    SendImmedCigiMessage(&cd);

    ApplySegment();
}

BOOL CPacketDlgCdSegmentDef::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_ComboEntityID.SetImageList(&g_ImgListTypes);
    PopulateEntityIDCombo();

    RefreshView();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgCdSegmentDef::RefreshView(void)
{
    CEntity *entity = g_DataManager.GetEntity(m_EntityID);
    if (!entity)
        return;

    CCollisionSeg *seg = entity->GetCollisionSeg(m_SegmentID);
    if (!seg)
        return;

    m_SegmentEnable = seg->GetEnable();
    m_X1 = seg->GetStartX();
    m_Y1 = seg->GetStartY();
    m_Z1 = seg->GetStartZ();
    m_X2 = seg->GetEndX();
    m_Y2 = seg->GetEndY();
    m_Z2 = seg->GetEndZ();
    m_MaterialMask = seg->GetMaterialMask();

    UpdateData(FALSE);
}

void CPacketDlgCdSegmentDef::ApplySegment(void)
{
    // Because our collision segment numbering is slightly different
    // from the actual IDs, don't worry about doing this.  We might
    // update it later if there is time.
}

void CPacketDlgCdSegmentDef::OnSelchangeEntityId()
{
    // Get the ID of the selected entity.
    int idx = m_ComboEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboEntityID.GetItemData(idx));
}

void CPacketDlgCdSegmentDef::OnEndeditEntityId(NMHDR *pNMHDR, LRESULT *pResult)
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

void CPacketDlgCdSegmentDef::PopulateEntityIDCombo(void)
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

        if (m_EntityID == entity_id) {
            // Select the item in the combo box.
            m_ComboEntityID.SetCurSel(cbo_idx);
        }
    }
}

int CPacketDlgCdSegmentDef::GetEntityIDFromCombo(void)
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

int CPacketDlgCdSegmentDef::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgCdSegmentDef::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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
