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
 *  FILENAME:   EntityHATPropPage.cpp
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
#include "entity.h"
#include "EntityHATPropPage.h"
#include "globals.h"
#include "Hat.h"
#include "hemumsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityHATPropPage dialog

CEntityHATPropPage::CEntityHATPropPage(CWnd *pParent /*=NULL*/)
    : CDialog(CEntityHATPropPage::IDD, pParent)
{
    //{{AFX_DATA_INIT(CEntityHATPropPage)
    m_HatValid = FALSE;
    m_Azimuth = _T("");
    m_Elevation = _T("");
    m_HAT = _T("");
    m_HOT = _T("");
    m_MaterialCode = _T("");
    //}}AFX_DATA_INIT
}

void CEntityHATPropPage::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEntityHATPropPage)
    DDX_Control(pDX, IDC_EDIT_HAT_Z, m_EditZ);
    DDX_Control(pDX, IDC_EDIT_HAT_Y, m_EditY);
    DDX_Control(pDX, IDC_EDIT_HAT_X, m_EditX);
    DDX_Control(pDX, IDC_COMBO_HAT_ID_RANGE, m_ComboID);
    DDX_Control(pDX, IDC_CHECK_REQUEST_HOT, m_CheckRequestHot);
    DDX_Control(pDX, IDC_CHECK_REQUEST_HAT, m_CheckRequestHat);
    DDX_Control(pDX, IDC_CHECK_CONTINUOUS, m_CheckContinuous);
    DDX_Control(pDX, IDC_BUTTON_APPLY_HAT, m_ButtonApply);
    DDX_Control(pDX, IDC_BUTTON_REMOVE_HAT, m_ButtonRemoveHat);
    DDX_Control(pDX, IDC_BUTTON_ADD_HAT, m_ButtonAddHat);
    DDX_Check(pDX, IDC_CHECK_HAT_VALID, m_HatValid);
    DDX_Text(pDX, IDC_STATIC_RESP_AZIMUTH, m_Azimuth);
    DDX_Text(pDX, IDC_STATIC_RESP_ELEVATION, m_Elevation);
    DDX_Text(pDX, IDC_STATIC_RESP_HAT, m_HAT);
    DDX_Text(pDX, IDC_STATIC_RESP_HOT, m_HOT);
    DDX_Text(pDX, IDC_STATIC_RESP_MATERIAL, m_MaterialCode);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEntityHATPropPage, CDialog)
    //{{AFX_MSG_MAP(CEntityHATPropPage)
    ON_BN_CLICKED(IDC_BUTTON_ADD_HAT, OnButtonAddHat)
    ON_BN_CLICKED(IDC_BUTTON_REMOVE_HAT, OnButtonRemoveHat)
    ON_BN_CLICKED(IDC_BUTTON_APPLY_HAT, OnButtonApplyHat)
    ON_CBN_SELCHANGE(IDC_COMBO_HAT_ID_RANGE, OnSelchangeComboIdRange)
    ON_BN_CLICKED(IDC_CHECK_REQUEST_HAT, OnCheckRequestHat)
    ON_BN_CLICKED(IDC_CHECK_REQUEST_HOT, OnCheckRequestHot)
    ON_BN_CLICKED(IDC_CHECK_CONTINUOUS, OnCheckContinuous)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityHATPropPage message handlers

void CEntityHATPropPage::OnOK()
{
}

void CEntityHATPropPage::OnCancel()
{
}

void CEntityHATPropPage::OnButtonAddHat()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Find the next available index.
    int hat_hot_id = entity->GetNextHatID();

    // Create a new HAT and add it to the entity.
    CHat *hat = new CHat(hat_hot_id, entity->GetID());
    entity->AddHat(hat);

    // Add the HAT to the combo box list.
    int idx = m_ComboID.AddString((LPCTSTR)(hat->GetName()));
    m_ComboID.SetItemData(idx, hat->GetID());

    // Select the new item.
    m_ComboID.SetCurSel(idx);
    OnSelchangeComboIdRange();
}

void CEntityHATPropPage::OnButtonRemoveHat()
{
    CHat *hat = GetSelHat();
    if (!hat)
        return;

    // First remove the item from the list box.
    int id = hat->GetID();
    int item_id = -1;
    int idx = 0;
    do {
        item_id = m_ComboID.GetItemData(idx);
        idx++;
    } while (item_id != id);
    idx--;                          // idx has been incremented past the item
    m_ComboID.DeleteString(idx);

    // Select the previous item.
    if (idx > 1)
        m_ComboID.SetCurSel(idx - 1);
    else
        m_ComboID.SetCurSel(0);

    OnSelchangeComboIdRange();

    // Send a message to the driver.
    // LWD

    // Finally, delete the HAT from the CEntity.
    g_DataManager.GetSelectedEntity()->DeleteHat(id);
}

void CEntityHATPropPage::OnButtonApplyHat()
{
    CHat *hat = GetSelHat();

    if (!hat) {
        // If for some reason we have an invalid item in the list, just
        // get rid of it.  This should never happen, though.
        m_ComboID.DeleteString(m_ComboID.GetCurSel());
        EnableFields(FALSE);
        return;
    }

    hat->SetContinuous(m_CheckContinuous.GetCheck() ? TRUE : FALSE);

    unsigned int requestType = 0;

    // Check if mode is HAT, HOT, or HAT/HOT.
    if (m_CheckRequestHot.GetCheck())
        requestType = (m_CheckRequestHat.GetCheck() ? 2 : 1);

    hat->SetRequestType(requestType);

    CString str;
    float pos = 0.0;

    m_EditX.GetWindowText(str);
    sscanf((LPCTSTR)str, "%f", &pos);
    hat->SetXOffset(pos);

    m_EditY.GetWindowText(str);
    sscanf((LPCTSTR)str, "%f", &pos);
    hat->SetYOffset(pos);

    m_EditZ.GetWindowText(str);
    sscanf((LPCTSTR)str, "%f", &pos);
    hat->SetZOffset(pos);

    SendHatRequestToDriver(hat);
}

void CEntityHATPropPage::SendHatRequestToDriver(CHat *hat)
{
    MESSAGE_ADD_HAT msg;

    msg.id_base = hat->GetID();
    msg.entity_id = hat->GetOwnerID();
    msg.request_type = hat->GetRequestType();
    msg.continuous = hat->GetContinuous();
    msg.lat = hat->GetLat();
    msg.lon = hat->GetLon();
    msg.alt = hat->GetAlt();
    msg.x_offset = hat->GetXOffset();
    msg.y_offset = hat->GetYOffset();
    msg.z_offset = hat->GetZOffset();

    PostDriverMsg(msg);
}

void CEntityHATPropPage::OnSelchangeComboIdRange()
{
    CHat *hat = GetSelHat();
    if (!hat) {
        // If for some reason we have an invalid item in the list, just
        // get rid of it.  This should never happen, though.
        m_ComboID.DeleteString(m_ComboID.GetCurSel());
        EnableFields(FALSE);
        return;
    }

    EnableFields(TRUE);
    PopulateReqFields(hat);
}

BOOL CEntityHATPropPage::AddHatItem(CHat *hat)
{
    if (!hat)
        return FALSE;

    int index = m_ComboID.AddString((LPCTSTR)(hat->GetName()));
    m_ComboID.SetItemData(index, hat->GetID());

    return TRUE;
}

BOOL CEntityHATPropPage::RemoveHatItem(CHat *hat)
{
    if (!hat)
        return FALSE;

    // Remove the item from the combo box.
    int id = hat->GetID();
    int item_id = -1;
    int idx = 0;
    do {
        item_id = m_ComboID.GetItemData(idx);
        idx++;
    } while (item_id != id);
    m_ComboID.DeleteString(idx);

    return TRUE;
}

CHat *CEntityHATPropPage::FindHat(const int id)
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return NULL;

    int mapidx = -1;
    CHat *hat = NULL;
    CHatMap &map = entity->GetHatMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, mapidx, hat);
        if ((hat) && hat->GetID() == id)
            return hat;
    }

    return NULL;
}

CHat *CEntityHATPropPage::GetSelHat(void)
{
    int idx = m_ComboID.GetCurSel();
    if (idx == -1)
        return NULL;

    int hat_hot_id = m_ComboID.GetItemData(idx);

    return FindHat(hat_hot_id);
}

void CEntityHATPropPage::ClearHats(void)
{
    m_ComboID.ResetContent();
    RefreshView();
}

void CEntityHATPropPage::RefreshView(void)
{
    CHat *hat = GetSelHat();
    if (!hat) {
        // If for some reason we have an invalid item in the list, just
        // get rid of it.  This should never happen, though.
        m_ComboID.DeleteString(m_ComboID.GetCurSel());
        return;
    }

    if (!m_ComboID.GetDroppedState())
        PopulateRespFields(hat);
}

void CEntityHATPropPage::EnableFields(BOOL enable)
{
    if (enable) {
        m_EditX.EnableWindow(TRUE);
        m_EditY.EnableWindow(TRUE);
        m_EditZ.EnableWindow(TRUE);
        m_CheckRequestHat.EnableWindow(TRUE);
        m_CheckRequestHot.EnableWindow(TRUE);
        m_CheckContinuous.EnableWindow(TRUE);
        m_ButtonRemoveHat.EnableWindow(TRUE);
    } else {
        m_EditX.SetWindowText("");
        m_EditY.SetWindowText("");
        m_EditZ.SetWindowText("");
        m_CheckRequestHat.SetCheck(0);
        m_CheckRequestHot.SetCheck(0);
        m_CheckContinuous.SetCheck(0);
        m_EditX.EnableWindow(FALSE);
        m_EditY.EnableWindow(FALSE);
        m_EditZ.EnableWindow(FALSE);
        m_CheckRequestHat.EnableWindow(FALSE);
        m_CheckRequestHot.EnableWindow(FALSE);
        m_CheckContinuous.EnableWindow(FALSE);
        m_ButtonRemoveHat.EnableWindow(FALSE);

        m_Azimuth = "";
        m_Elevation = "";
        m_HAT = "";
        m_HOT = "";
        m_MaterialCode = "";
        m_HatValid = FALSE;
        UpdateData(FALSE);
    }
}

void CEntityHATPropPage::PopulateReqFields(CHat *hat)
{
    CString str;

    m_CheckRequestHat.SetCheck(hat->GetRequestType() != 1);
    m_CheckRequestHot.SetCheck(hat->GetRequestType() > 0);
    m_CheckContinuous.SetCheck(hat->GetContinuous());

    str.Format("%.1f", hat->GetXOffset());
    m_EditX.SetWindowText((LPCTSTR)str);
    str.Format("%.1f", hat->GetYOffset());
    m_EditY.SetWindowText((LPCTSTR)str);
    str.Format("%.1f", hat->GetZOffset());
    m_EditZ.SetWindowText((LPCTSTR)str);
}

void CEntityHATPropPage::PopulateRespFields(CHat *hat)
{
    if (hat->RespIsHat())
        m_HAT.Format("%f", hat->GetRespHat());
    else
        m_HAT = "";

    if (hat->RespIsHot())
        m_HOT.Format("%f", hat->GetRespHot());
    else
        m_HOT = "";

    if (hat->RespIsHat() && hat->RespIsHot()) {
        m_Azimuth.Format("%f", hat->GetRespAzimuth());
        m_Elevation.Format("%f", hat->GetRespElevation());
        m_MaterialCode.Format("%.8X", hat->GetRespMaterialCode());
    } else {
        m_Azimuth = "";
        m_Elevation = "";
        m_MaterialCode = "";
    }

    m_HatValid = hat->RespIsValid() ? TRUE : FALSE;

    UpdateData(FALSE);
}

void CEntityHATPropPage::OnCheckRequestHat()
{
    CHat *hat = GetSelHat();
    if (!hat)
        return;

    int checked = m_CheckRequestHat.GetCheck();

    // If we are unchecking the box, make sure the other one is checked.
    if ((!checked) && (!m_CheckRequestHot.GetCheck())) {
        m_CheckRequestHot.SetCheck(1);
        OnCheckRequestHot();
    }
}

void CEntityHATPropPage::OnCheckRequestHot()
{
    CHat *hat = GetSelHat();
    if (!hat)
        return;

    int checked = m_CheckRequestHot.GetCheck();

    // If we are unchecking the box, make sure the other one is checked.
    if ((!checked) && (!m_CheckRequestHat.GetCheck())) {
        m_CheckRequestHat.SetCheck(1);
        OnCheckRequestHat();
    }
}

void CEntityHATPropPage::OnCheckContinuous()
{
}

BOOL CEntityHATPropPage::OnInitDialog()
{
    CDialog::OnInitDialog();

    EnableFields(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
