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

// PacketDlgTerrestrialCondControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "PacketDlgTerrestrialCondControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgTerrestrialCondControl dialog

CPacketDlgTerrestrialCondControl::CPacketDlgTerrestrialCondControl(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgTerrestrialCondControl::IDD, pParent)
{
    m_EntityID = -1;

    //{{AFX_DATA_INIT(CPacketDlgTerrestrialCondControl)
    m_EnableConditions = FALSE;
    m_ConditionID = 0;
    m_Coverage = 0;
    m_Severity = 0;
    m_Scope = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgTerrestrialCondControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgTerrestrialCondControl)
    DDX_Control(pDX, IDC_LABEL_REGION_ENTITY_ID, m_LabelRegionEntityID);
    DDX_Control(pDX, IDC_COMBOBOXEX_REGION_ENTITY_ID, m_ComboRegionEntityID);
    DDX_Control(pDX, IDC_SPIN_SEVERITY, m_SpinSeverity);
    DDX_Check(pDX, IDC_CHECK_ENABLE_CONDITIONS, m_EnableConditions);
    DDX_Text(pDX, IDC_EDIT_COND_ID, m_ConditionID);
    DDX_Text(pDX, IDC_EDIT_COVERAGE, m_Coverage);
    DDX_Text(pDX, IDC_EDIT_SEVERITY, m_Severity);
    DDV_MinMaxInt(pDX, m_Severity, 0, 5);
    DDX_Radio(pDX, IDC_RADIO_GLOBAL, m_Scope);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgTerrestrialCondControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgTerrestrialCondControl)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_REGION_ENTITY_ID, OnSelchangeEntityID)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_REGION_ENTITY_ID, OnEndeditEntityID)
    ON_BN_CLICKED(IDC_RADIO_ENTITY, OnRadioEntity)
    ON_BN_CLICKED(IDC_RADIO_GLOBAL, OnRadioGlobal)
    ON_BN_CLICKED(IDC_RADIO_REGIONAL, OnRadioRegional)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgTerrestrialCondControl message handlers

BOOL CPacketDlgTerrestrialCondControl::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_SpinSeverity.SetRange(0, 5);
    m_SpinSeverity.SetPos(0);

    m_ComboRegionEntityID.SetImageList(&g_ImgListTypes);

    PopulateEntityIDCombo();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgTerrestrialCondControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgTerrestrialCondControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_TERRESTRIAL_CONDITIONS_CONTROL tc = {0};
    tc.packet_id = CIGI_TERRESTRIAL_CONDITIONS_CONTROL_OPCODE;
    tc.packet_size = sizeof(CIGI_TERRESTRIAL_CONDITIONS_CONTROL);
	if( m_Scope == 0 ) 
	    tc.entity_region_id = 0;
	else
	    tc.entity_region_id = m_EntityID;
    tc.condition_id = m_ConditionID;
    tc.condition_enable = m_EnableConditions;
    tc.scope = m_Scope;
    tc.coverage = m_Coverage;
    tc.severity = m_Severity;

    SendImmedCigiMessage(&tc);

    ApplyTerrestrialCond();
}

void CPacketDlgTerrestrialCondControl::OnSelchangeEntityID()
{
    // Get the ID of the selected entity.
    int idx = m_ComboRegionEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboRegionEntityID.GetItemData(idx));
}

void CPacketDlgTerrestrialCondControl::OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult)
{
    int new_id = atoi(((NMCBEENDEDIT *)pNMHDR)->szText);
    if (new_id == m_EntityID)
        return;

    m_EntityID = new_id;
    int idx = FindComboItem(m_ComboRegionEntityID, m_EntityID);
    if (idx > -1) {
        // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
        // select no item after it has finished its stuff.
        ::PostMessage(m_ComboRegionEntityID.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

        // Post a CBN_SELCHANGE notification so OnSelchangeEntityID() will be called.
        // The function must be called after the CB_SELCURSEL message is processed
        // so the new item will be selected.
        ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                      MAKEWPARAM(IDC_COMBOBOXEX_REGION_ENTITY_ID, CBN_SELCHANGE),
                      (LPARAM)m_ComboRegionEntityID.GetSafeHwnd());
    }

    *pResult = 0;
}

void CPacketDlgTerrestrialCondControl::PopulateEntityIDCombo(void)
{
    char text[256] = {0};
    int cbo_idx = -1;
    int entity_id = -1;
    CEntity *entity = NULL;
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_GENERIC, IMG_GENERIC, 0, 0, -1
                             };

    m_ComboRegionEntityID.ResetContent();

    switch (m_Scope) {
    case 0:     // Global
        break;

    case 1:     // Regional
        break;

    case 2: {   // Entity
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

            if (entity->GetClass() == ENTITY_CLASS_TERRAIN_COND) {
                // Add a new item to the list.
                cboitem.iItem = FindComboInsertPoint(m_ComboRegionEntityID, entity->GetID());
                cbo_idx = m_ComboRegionEntityID.InsertItem(&cboitem);
            }
        }

        break;
    }
    }

    if (m_ComboRegionEntityID.GetCount()) {
        m_ComboRegionEntityID.SetCurSel(0);
        m_EntityID = MAKE_ID(m_ComboRegionEntityID.GetItemData(0));
    }


    m_ComboRegionEntityID.RedrawWindow();
}

int CPacketDlgTerrestrialCondControl::GetEntityIDFromCombo(void)
{
    int itemdata = m_ComboRegionEntityID.GetItemData(m_ComboRegionEntityID.GetCurSel());
    if (itemdata != -1)
        return MAKE_ID(itemdata);
    else {
        CString text;
        m_ComboRegionEntityID.GetWindowText(text);
        return atoi((LPCTSTR)text);
    }
}

int CPacketDlgTerrestrialCondControl::FindComboItem(CComboBoxEx &combo, const int id)
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

int CPacketDlgTerrestrialCondControl::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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

void CPacketDlgTerrestrialCondControl::SelectEntityIDComboItem(void)
{
    int id = FindComboItem(m_ComboRegionEntityID, m_EntityID);
    m_ComboRegionEntityID.SetCurSel(id);
    if (id == -1) {
        CString text;
        text.Format("%d", m_EntityID);
        m_ComboRegionEntityID.SetWindowText(text);
    }

    OnSelchangeEntityID();
}

void CPacketDlgTerrestrialCondControl::ApplyTerrestrialCond(void)
{
}

void CPacketDlgTerrestrialCondControl::OnRadioGlobal()
{
    m_Scope = 0;
    m_ComboRegionEntityID.SetCurSel(-1);
    PopulateEntityIDCombo();
    SelectEntityIDComboItem();
}

void CPacketDlgTerrestrialCondControl::OnRadioRegional()
{
    m_Scope = 1;
    m_LabelRegionEntityID.SetWindowText("Region ID:");
    m_ComboRegionEntityID.SetCurSel(-1);
    PopulateEntityIDCombo();
    SelectEntityIDComboItem();
}

void CPacketDlgTerrestrialCondControl::OnRadioEntity()
{
    m_Scope = 2;
    m_LabelRegionEntityID.SetWindowText("Entity ID:");
    PopulateEntityIDCombo();
    SelectEntityIDComboItem();
}
