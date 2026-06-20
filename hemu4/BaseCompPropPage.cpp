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
 *  FILENAME:   BaseCompPropPage.cpp
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
#include "BaseCompPropPage.h"
#include "cigi4types.h"
#include "globals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseCompPropPage dialog

CBaseCompPropPage::CBaseCompPropPage(CWnd *pParent /*=NULL*/)
    : CDialog(CBaseCompPropPage::IDD, pParent)
{
    //{{AFX_DATA_INIT(CBaseCompPropPage)
    //}}AFX_DATA_INIT
}

void CBaseCompPropPage::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CBaseCompPropPage)
    DDX_Control(pDX, IDC_CHECK_ENTITY_COMP_AUTOAPPLY, m_CheckAutoApply);
    DDX_Control(pDX, IDC_LIST_ENTITY_COMP_STATES, m_rlStates);
    DDX_Control(pDX, IDC_LIST_ENTITY_COMPONENTS, m_lbComponents);
    DDX_Control(pDX, IDC_COMBO_ENTITY_COMP6, m_cbvarData6);
    DDX_Control(pDX, IDC_COMBO_ENTITY_COMP5, m_cbvarData5);
    DDX_Control(pDX, IDC_COMBO_ENTITY_COMP4, m_cbvarData4);
    DDX_Control(pDX, IDC_COMBO_ENTITY_COMP3, m_cbvarData3);
    DDX_Control(pDX, IDC_COMBO_ENTITY_COMP2, m_cbvarData2);
    DDX_Control(pDX, IDC_COMBO_ENTITY_COMP1, m_cbvarData1);
    DDX_Control(pDX, IDC_BUTTON_APPLY_COMP, m_ButtonApply);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBaseCompPropPage, CDialog)
    //{{AFX_MSG_MAP(CBaseCompPropPage)
    ON_BN_CLICKED(IDC_BUTTON_APPLY_COMP, OnButtonApply)
    ON_LBN_SELCHANGE(IDC_LIST_ENTITY_COMPONENTS, OnSelchangeComponent)
    //}}AFX_MSG_MAP

    ON_MESSAGE(CBV_EDITCHANGE, OnUserEditData)
    ON_MESSAGE(CBV_SELCHANGE, OnUserSelDatatype)
    ON_MESSAGE(RL_SELCHANGED, OnSelchangeState)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseCompPropPage message handlers

void CBaseCompPropPage::OnOK()
{
}

void CBaseCompPropPage::OnCancel()
{
}

BOOL CBaseCompPropPage::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_rlStates.Init();
    m_CheckAutoApply.SetCheck(TRUE);

    // Delete the 64-bit datatypes from the last data item.
    m_cbvarData6.DeleteString(5);
    m_cbvarData6.DeleteString(4);
    m_cbvarData6.DeleteString(3);

    // Return TRUE unless you set the focus to a control.
    // EXCEPTION: OCX Property Pages should return FALSE.
    return TRUE;
}

void CBaseCompPropPage::OnButtonApply()
{
    BOOL autoapply = m_CheckAutoApply.GetCheck();
    m_CheckAutoApply.SetCheck(TRUE);

    // Simulate selection of the state.
    OnSelchangeState((WPARAM)m_rlStates.GetCurSel(), (LPARAM)m_rlStates.m_hWnd);

    // Simulate keyboard edit for each data field.  Do this in reverse order
    // so that the "Jacob's Ladder" cascading effect will work correctly.
    OnUserEditData((WPARAM)m_cbvarData6.GetDataType(), (LPARAM)&m_cbvarData6);
    OnUserEditData((WPARAM)m_cbvarData5.GetDataType(), (LPARAM)&m_cbvarData5);
    OnUserEditData((WPARAM)m_cbvarData4.GetDataType(), (LPARAM)&m_cbvarData4);
    OnUserEditData((WPARAM)m_cbvarData3.GetDataType(), (LPARAM)&m_cbvarData3);
    OnUserEditData((WPARAM)m_cbvarData2.GetDataType(), (LPARAM)&m_cbvarData2);
    OnUserEditData((WPARAM)m_cbvarData1.GetDataType(), (LPARAM)&m_cbvarData1);

    m_CheckAutoApply.SetCheck(autoapply);

    // Send a CIGI packet.
    CComponent *comp = GetSelComponent();
    if (comp)
        SendPacketToDriver(comp);
}

void CBaseCompPropPage::SendPacketToDriver(CComponent *comp)
{
    int size = 0;
    unsigned char buffer[sizeof(CIGI_COMPONENT_CONTROL)] = {0};

    // Create either a Component or Short Component packet.
    comp->CreateCigiPacket(buffer);

    // SendImmedCigiMessage() will figure out the packet size.
    SendImmedCigiMessage(buffer);
}

LRESULT CBaseCompPropPage::OnUserEditData(WPARAM wParam, LPARAM lParam)
{
    if (!m_CheckAutoApply.GetCheck())
        return 0;

    int slot = 0;
    int datatype = (int)wParam;
    CComboBoxVariant *cbo = (CComboBoxVariant *)lParam;

    CComponent *comp = GetSelComponent();
    if (!comp)
        return 0;

    if (cbo == &m_cbvarData1)
        slot = 1;
    else if (cbo == &m_cbvarData2)
        slot = 2;
    else if (cbo == &m_cbvarData3)
        slot = 3;
    else if (cbo == &m_cbvarData4)
        slot = 4;
    else if (cbo == &m_cbvarData5)
        slot = 5;
    else if (cbo == &m_cbvarData6)
        slot = 6;
    else
        return 0;

    CString temp;
    cbo->GetWindowText(temp);
    temp.TrimLeft();
    temp.TrimRight();
    if (temp == "") {
        // No text, so mark as unused.
        comp->SetData(slot, -1, NULL);
    } else {
        switch (datatype) {
        case 0: //DataTypeInt32
            comp->SetData(slot, cbo->GetValueInt32());
            break;

        case 1: //DataTypeUInt32
            comp->SetData(slot, cbo->GetValueUInt32());
            break;

        case 2: //DataTypeFloat
            comp->SetData(slot, cbo->GetValueFloat());
            break;

        case 3: //DataTypeInt64
            comp->SetData(slot, cbo->GetValueInt64());
            break;

        case 4: //DataTypeUInt64
            comp->SetData(slot, cbo->GetValueUInt64());
            break;

        case 5: //DataTypeDouble
            comp->SetData(slot, cbo->GetValueDouble());
            break;

        default:
            break;
        }
    }

    return 0;
}

/// This function will detect the type (size) of data that the user has selected for
/// component data fields. When the user selects a 64 bit sized type for a field,
/// the field immediately following will be disabled. (Since fields are 32 bits, a 64
/// bit value will of course use the combined space of two fields.)
LRESULT CBaseCompPropPage::OnUserSelDatatype(WPARAM wParam, LPARAM lParam)
{
    // Simulate keyboard entry.  This will set the type within the component.
    OnUserEditData(wParam, lParam);

    CComboBoxVariant *cbo = (CComboBoxVariant *)lParam;
    CComboBoxVariant *nextDataField = NULL;

    // Determine the data field immediately following the current, if it exists.
    if (cbo == &m_cbvarData1)
        nextDataField = &m_cbvarData2;
    else if (cbo == &m_cbvarData2)
        nextDataField = &m_cbvarData3;
    else if (cbo == &m_cbvarData3)
        nextDataField = &m_cbvarData4;
    else if (cbo == &m_cbvarData4)
        nextDataField = &m_cbvarData5;
    else if (cbo == &m_cbvarData5)
        nextDataField = &m_cbvarData6;

    if (nextDataField) {
        int datatype = (int)wParam;
        BOOL enableNextDataField;

        switch (datatype) {
        case 3: // DataTypeInt64
        case 4: // DataTypeUInt64
        case 5: // DataTypeDouble
            // If the current combo item is disabled, then we want to enable
            // the combo item that follows. Otherwise, the current combo item
            // would be an enabled 64 bit field, and we therefore would
            // need the disable the field that follows.
            enableNextDataField = !cbo->IsWindowEnabled();
            break;

        default:
            // 32 bit types do not affect the fields after them.
            enableNextDataField = TRUE;
            break;
        }

        nextDataField->EnableWindow(enableNextDataField);
        OnUserSelDatatype((WPARAM)nextDataField->GetDataType(), (LPARAM)nextDataField);
    }

    return 0;
}

void CBaseCompPropPage::OnSelchangeComponent()
{
    CComponent *comp = GetSelComponent();
    const BOOL compExists = (comp != NULL);

    if (compExists) {
        PopulateStateList(comp);
        PopulateDataFields(comp);
    }

    EnableFields(compExists);
}

LRESULT CBaseCompPropPage::OnSelchangeState(WPARAM wParam, LPARAM lParam)
{
    if (m_CheckAutoApply.GetCheck()) {
        int item = (int)wParam;
        CComponent *comp = GetSelComponent();
        if (comp) {
            // Set the state of the actual component object.
            comp->SetState(m_rlStates.GetItemData(item));
            SendPacketToDriver(comp);
        }
    }

    return 0;
}

void CBaseCompPropPage::RefreshView(void)
{
    CComponent *comp = GetSelComponent();
    const BOOL compExists = (comp != NULL);

    if (compExists) {
        // Added this line for updates from scripts.  If I encounter problems, I may
        // have to add a routine to SELECT the item from the list rather than
        // REPOPULATE the entire list.
        PopulateStateList(comp);
        PopulateDataFields(comp);
    } else {
        // If for some reason we have an invalid item in the list, just
        // get rid of it.  This should never happen, though.
        m_lbComponents.DeleteString(m_lbComponents.GetCurSel());
    }

    EnableFields(compExists);
}

void CBaseCompPropPage::EnableFields(BOOL enable)
{
    m_rlStates.EnableWindow(enable);
    m_cbvarData6.EnableWindow(enable);
    m_cbvarData5.EnableWindow(enable);
    m_cbvarData4.EnableWindow(enable);
    m_cbvarData3.EnableWindow(enable);
    m_cbvarData2.EnableWindow(enable);
    m_cbvarData1.EnableWindow(enable);

    if (!enable) {
        m_rlStates.Clear();
        m_cbvarData6.SetWindowText("");
        m_cbvarData5.SetWindowText("");
        m_cbvarData4.SetWindowText("");
        m_cbvarData3.SetWindowText("");
        m_cbvarData2.SetWindowText("");
        m_cbvarData1.SetWindowText("");
    }
}

void CBaseCompPropPage::PopulateStateList(CComponent *comp)
{
    m_rlStates.Clear();

    unsigned long handle = 0;
    CComponentState *state = comp->GetFirstState(&handle);

    while (state) {
        int idx = m_rlStates.AddItem(state->Name);
        m_rlStates.SetItemData(idx, state->Value);

        // If the item represents the current state, select it.
        if (comp->GetState() == state->Value)
            m_rlStates.SetCurSel(idx);

        state = comp->GetNextState(&handle);
    }
}

void CBaseCompPropPage::PopulateDataFields(CComponent *comp)
{
    BOOL skipnext = FALSE;
    int datatype = -1;
    void *data = NULL;

    // Do the first data field.
    data = comp->GetData(1, &datatype);
    SetDataField(m_cbvarData1, datatype, data);
    skipnext = (datatype >= CComboBoxVariant::DataTypeInt64); // if 64-bit

    // Do the second data field.
    if (skipnext) {
        m_cbvarData2.EnableWindow(FALSE);
        skipnext = FALSE;
    } else {
        data = comp->GetData(2, &datatype);
        SetDataField(m_cbvarData2, datatype, data);
        skipnext = (datatype >= CComboBoxVariant::DataTypeInt64); // if 64-bit
    }

    // Do the third data field.
    if (skipnext) {
        m_cbvarData3.EnableWindow(FALSE);
        skipnext = FALSE;
    } else {
        data = comp->GetData(3, &datatype);
        SetDataField(m_cbvarData3, datatype, data);
        skipnext = (datatype >= CComboBoxVariant::DataTypeInt64); // if 64-bit
    }

    // Do the fourth data field.
    if (skipnext) {
        m_cbvarData4.EnableWindow(FALSE);
        skipnext = FALSE;
    } else {
        data = comp->GetData(4, &datatype);
        SetDataField(m_cbvarData4, datatype, data);
        skipnext = (datatype >= CComboBoxVariant::DataTypeInt64); // if 64-bit
    }

    // Do the fifth data field.
    if (skipnext) {
        m_cbvarData5.EnableWindow(FALSE);
        skipnext = FALSE;
    } else {
        data = comp->GetData(5, &datatype);
        SetDataField(m_cbvarData5, datatype, data);
        skipnext = (datatype >= CComboBoxVariant::DataTypeInt64); // if 64-bit
    }

    // Do the sixth data field.
    if (skipnext) {
        m_cbvarData6.EnableWindow(FALSE);
        skipnext = FALSE;
    } else {
        data = comp->GetData(6, &datatype);
        SetDataField(m_cbvarData6, datatype, data);
    }
}

void CBaseCompPropPage::SetDataField(CComboBoxVariant &cbo, const int datatype, void *data)
{
    if (((unsigned int)datatype > 5) || (!data)) {
        cbo.SetValueInt32(0);
        cbo.SetWindowText("");
        return;
    }

    switch (datatype) {
    case 0: //DataTypeInt32
        cbo.SetValueInt32(*((__int32 *)data));
        break;

    case 1: //DataTypeUInt32
        cbo.SetValueUInt32(*((unsigned __int32 *)data));
        break;

    case 2: //DataTypeFloat
        cbo.SetValueFloat(*((float *)data));
        break;

    case 3: //DataTypeInt64
        cbo.SetValueInt64(*((__int64 *)data));
        break;

    case 4: //DataTypeUInt64
        cbo.SetValueUInt64(*((unsigned __int64 *)data));
        break;

    case 5: //DataTypeDouble
        cbo.SetValueDouble(*((double *)data));
        break;

    default:
        break;
    }
}

BOOL CBaseCompPropPage::AddComponent(CComponent *component)
{
    if (!component)
        return FALSE;

    int index = m_lbComponents.AddString((LPCTSTR)(component->GetName()));
    m_lbComponents.SetItemData(index, MakeKey(component->GetID(), component->GetInstanceID()));

    m_ComponentList.InsertTail(component);

    return TRUE;
}

BOOL CBaseCompPropPage::RemoveComponent(CComponent *component)
{
    if (!component)
        return FALSE;

    // First remove the item from the list box.
    const int id = MakeKey(component->GetID(), component->GetInstanceID());
    int item_id = -1;
    int idx = 0;

    do {
        item_id = m_lbComponents.GetItemData(idx);
        idx++;
    } while (item_id != id);

    m_lbComponents.DeleteString(idx);

    // Now remove the item from the linked list.
    unsigned long handle = 0;
    CComponent **comp = m_ComponentList.GetHead(&handle);

    while (comp && *comp) {
        if ((*comp)->GetID() == id) {
            m_ComponentList.Remove(&handle);
            return TRUE;
        }

        comp = m_ComponentList.GetNext(&handle);
    }

    return FALSE;
}

void CBaseCompPropPage::ClearComponents(void)
{
    m_ComponentList.RemoveAll();
    m_lbComponents.ResetContent();
    RefreshView();
}

CComponent *CBaseCompPropPage::FindComponent(int componentId, int instID)
{
    unsigned long handle = 0;
    CComponent **comp = m_ComponentList.GetHead(&handle);

    while (comp && *comp) {
        if (((*comp)->GetID() == componentId) && ((*comp)->GetInstanceID() == instID))
            return *comp;

        comp = m_ComponentList.GetNext(&handle);
    }

    return NULL;
}

CComponent *CBaseCompPropPage::GetSelComponent(void)
{
    int idx = m_lbComponents.GetCurSel();

    if (idx == -1)
        return NULL;

    const uint32_t key = m_lbComponents.GetItemData(idx);
    const uint16_t compId = key >> 16;
    const uint16_t instId = key & 0xFFFF;

#ifdef _DEBUG
    // Ensure that the component ID and instance ID we've extracted
    // combine correctly back into the original key.
    const uint32_t assertKey = MakeKey(compId, instId);
    ASSERT(assertKey == key);
#endif

    return FindComponent(compId, instId);
}
