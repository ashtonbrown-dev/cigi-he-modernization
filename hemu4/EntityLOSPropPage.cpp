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
 *  FILENAME:   EntityLOSPropPage.cpp
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
#include "Entity.h"
#include "EntityLOSPropPage.h"
#include "globals.h"
#include "hemumsg.h"
#include "Los.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityLOSPropPage dialog

CEntityLOSPropPage::CEntityLOSPropPage(CWnd *pParent /*=NULL*/)
    : CDialog(CEntityLOSPropPage::IDD, pParent)
{
    //{{AFX_DATA_INIT(CEntityLOSPropPage)
    m_RespValid = FALSE;
    m_szRespCount = _T("0");
    m_szRespAlt = _T("");
    m_szRespLat = _T("");
    m_szRespLon = _T("");
    m_szRespEntityID = _T("");
    m_szRespAz = _T("");
    m_szRespEl = _T("");
    m_szRespRange = _T("");
    m_szRespMaterialCode = _T("");
    m_szRespColor = _T("");
    //}}AFX_DATA_INIT
}

void CEntityLOSPropPage::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEntityLOSPropPage)
    DDX_Control(pDX, IDC_LABEL_REQ_EL, m_LabelReqEl);
    DDX_Control(pDX, IDC_LABEL_REQ_AZ, m_LabelReqAz);
    DDX_Control(pDX, IDC_LABEL_RESP_LONGITUDE_UNITS, m_LabelRespLonOrYUnits);
    DDX_Control(pDX, IDC_LABEL_RESP_LONGITUDE, m_LabelRespLonOrY);
    DDX_Control(pDX, IDC_LABEL_RESP_LATITUDE_UNITS, m_LabelRespLatOrXUnits);
    DDX_Control(pDX, IDC_LABEL_RESP_LATITUDE, m_LabelRespLatOrX);
    DDX_Control(pDX, IDC_LABEL_RESP_ALTITUDE, m_LabelRespAltOrZ);
    DDX_Control(pDX, IDC_LABEL_DESTINATION, m_LabelDestination);
    DDX_Control(pDX, IDC_LABEL_DEST_ALT, m_LabelDestAltOrZ);
    DDX_Control(pDX, IDC_LABEL_DEST_LAT_OR_MIN_RANGE, m_LabelDestLatOrXOrMinRange);
    DDX_Control(pDX, IDC_LABEL_DEST_LON_OR_MAX_RANGE, m_LabelDestLonOrYOrMaxRange);
    DDX_Control(pDX, IDC_RADIO_LOS_TYPE_SEGMENT, m_RadioSegment);
    DDX_Control(pDX, IDC_EDIT_LOS_SOURCE_Z, m_EditSourceZ);
    DDX_Control(pDX, IDC_EDIT_LOS_SOURCE_Y, m_EditSourceY);
    DDX_Control(pDX, IDC_EDIT_LOS_SOURCE_X, m_EditSourceX);
    DDX_Control(pDX, IDC_EDIT_LOS_MAT_MASK, m_EditMaterialMask);
    DDX_Control(pDX, IDC_EDIT_LOS_EL, m_EditReqEl);
    DDX_Control(pDX, IDC_EDIT_LOS_DEST_Z, m_EditDestZ);
    DDX_Control(pDX, IDC_EDIT_LOS_DEST_X_MIN_RANGE, m_EditDestXorMinRange);
    DDX_Control(pDX, IDC_EDIT_LOS_DEST_Y_MAX_RANGE, m_EditDestYorMaxRange);
    DDX_Control(pDX, IDC_EDIT_LOS_AZ, m_EditReqAz);
    DDX_Control(pDX, IDC_EDIT_LOS_ALPHA_THRESH, m_EditAlphaThreshold);
    DDX_Control(pDX, IDC_COMBO_LOS_ID_RANGE, m_ComboID);
    DDX_Control(pDX, IDC_CHECK_LOS_REQ_GDC, m_CheckReqGDC);
    DDX_Control(pDX, IDC_CHECK_LOS_DEST_GDC, m_CheckDestGDC);
    DDX_Control(pDX, IDC_CHECK_EXT_RESPONSE, m_CheckExtResponse);
    DDX_Control(pDX, IDC_CHECK_CONTINUOUS, m_CheckContinuous);
    DDX_Control(pDX, IDC_BUTTON_REMOVE_LOS, m_ButtonRemove);
    DDX_Check(pDX, IDC_CHECK_LOS_VALID, m_RespValid);
    DDX_Text(pDX, IDC_LABEL_RESPONSE_COUNT, m_szRespCount);
    DDX_Text(pDX, IDC_STATIC_RESP_ALTITUDE, m_szRespAlt);
    DDX_Text(pDX, IDC_STATIC_RESP_LATITUDE, m_szRespLat);
    DDX_Text(pDX, IDC_STATIC_RESP_LONGITUDE, m_szRespLon);
    DDX_Text(pDX, IDC_STATIC_RESP_ENTITY_ID, m_szRespEntityID);
    DDX_Text(pDX, IDC_STATIC_RESP_AZIMUTH, m_szRespAz);
    DDX_Text(pDX, IDC_STATIC_RESP_ELEVATION, m_szRespEl);
    DDX_Text(pDX, IDC_STATIC_RESP_RANGE, m_szRespRange);
    DDX_Text(pDX, IDC_STATIC_RESP_MATERIAL_CODE, m_szRespMaterialCode);
    DDX_Text(pDX, IDC_STATIC_RESP_COLOR, m_szRespColor);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEntityLOSPropPage, CDialog)
    //{{AFX_MSG_MAP(CEntityLOSPropPage)
    ON_BN_CLICKED(IDC_BUTTON_ADD_LOS, OnButtonAddLos)
    ON_BN_CLICKED(IDC_BUTTON_REMOVE_LOS, OnButtonRemoveLos)
    ON_BN_CLICKED(IDC_BUTTON_APPLY_LOS, OnButtonApply)
    ON_BN_CLICKED(IDC_CHECK_LOS_DEST_GDC, OnCheckLosDestGdc)
    ON_CBN_SELCHANGE(IDC_COMBO_LOS_ID_RANGE, OnSelchangeRange)
    ON_BN_CLICKED(IDC_RADIO_LOS_TYPE_SEGMENT, OnRadioLosTypeSegment)
    ON_BN_CLICKED(IDC_RADIO_LOS_TYPE_VECTOR, OnRadioLosTypeVector)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityLOSPropPage message handlers

void CEntityLOSPropPage::OnOK()
{
}

void CEntityLOSPropPage::OnCancel()
{
}

void CEntityLOSPropPage::OnButtonAddLos()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Find the next available index.
    int los_id = entity->GetNextLosID();

    // Create a new LOS and add it to the entity.
    CLos *los = new CLos(los_id, entity->GetID());
    entity->AddLos(los);

    // Add the LOS to the combo box list.
    int idx = m_ComboID.AddString((LPCTSTR)(los->GetName()));
    m_ComboID.SetItemData(idx, los->GetID());

    // Select the new item.
    m_ComboID.SetCurSel(idx);
    OnSelchangeRange();
}

void CEntityLOSPropPage::OnButtonRemoveLos()
{
    CLos *los = GetSelLos();
    if (!los)
        return;

    // First remove the item from the list box.
    int id = los->GetID();
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

    OnSelchangeRange();

    // Send a message to the driver.
    // LWD

    // Finally, delete the LOS from the CEntity.
    g_DataManager.GetSelectedEntity()->DeleteLos(id);
}

void CEntityLOSPropPage::OnButtonApply()
{
    CString str;
    int tempi = 0;
    unsigned int tempui = 0;
    float tempf = 0.0;
    double tempd = 0.0;
    CLos *los = GetSelLos();
    if (!los) {
        // If for some reason we have an invalid item in the list, just
        // get rid of it.  This should never happen, though.
        m_ComboID.DeleteString(m_ComboID.GetCurSel());
        EnableFields(FALSE);
        return;
    }

    los->SetContinuous(m_CheckContinuous.GetCheck() ? TRUE : FALSE);
    los->SetRequestExtended(m_CheckExtResponse.GetCheck() ? TRUE : FALSE);
    //los->SetDestCoordSystem(m_CheckDestGDC.GetCheck() ? 1 : 0);
    los->SetDesiredRespCoordSystem(m_CheckReqGDC.GetCheck() ? 1 : 0);

    if (m_RadioSegment.GetCheck()) {
        los->SetRequestType(LOS_TYPE_SEGMENT);

        m_EditDestXorMinRange.GetWindowText(str);
        sscanf((LPCTSTR)str, "%lf", &tempd);
        los->SetDestLatX(tempd);

        m_EditDestYorMaxRange.GetWindowText(str);
        sscanf((LPCTSTR)str, "%lf", &tempd);
        los->SetDestLonY(tempd);
    } else {
        los->SetRequestType(LOS_TYPE_VECTOR);

        m_EditDestXorMinRange.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        los->SetRequestMinRange(tempf);

        m_EditDestYorMaxRange.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        los->SetRequestMaxRange(tempf);
    }

    m_EditAlphaThreshold.GetWindowText(str);
    sscanf((LPCTSTR)str, "%u", &tempui);
    los->SetAlphaThreshold(tempui);

    m_EditMaterialMask.GetWindowText(str);
    sscanf((LPCTSTR)str, "%x", &tempui);
    los->SetMaterialMask(tempui);

    m_EditReqAz.GetWindowText(str);
    sscanf((LPCTSTR)str, "%f", &tempf);
    los->SetRequestAzimuth(tempf);

    m_EditReqEl.GetWindowText(str);
    sscanf((LPCTSTR)str, "%f", &tempf);
    los->SetRequestElevation(tempf);

    m_EditSourceX.GetWindowText(str);
    sscanf((LPCTSTR)str, "%lf", &tempd);
    los->SetSourceXOffset(tempd);

    m_EditSourceY.GetWindowText(str);
    sscanf((LPCTSTR)str, "%lf", &tempd);
    los->SetSourceYOffset(tempd);

    m_EditSourceZ.GetWindowText(str);
    sscanf((LPCTSTR)str, "%lf", &tempd);
    los->SetSourceZOffset(tempd);

    m_EditDestZ.GetWindowText(str);
    sscanf((LPCTSTR)str, "%lf", &tempd);
    los->SetDestAltZ(tempd);

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (entity) {
        DOF entity_dofs = entity->GetDofs();
        los->SetSourceLat(entity_dofs.latitude);
        los->SetSourceLon(entity_dofs.longitude);
        los->SetSourceAlt(entity_dofs.altitude);
    }

    SendLosRequestToDriver(los);
}

void CEntityLOSPropPage::SendLosRequestToDriver(CLos *los)
{
    MESSAGE_ADD_LOS msg;

    msg.id_base = los->GetID();
    msg.entity_id = los->GetOwnerID();
    msg.target_entity_id = los->GetTargetEntityID();
    msg.req_type = los->GetRequestType();
    msg.ext_response = los->GetRequestExtended();
    msg.src_coord_system = los->GetSourceCoordSystem();
    msg.dest_coord_system = los->GetDestCoordSystem();
    msg.resp_coord_system = los->GetDesiredRespCoordSystem();
    msg.continuous = los->GetContinuous();
    msg.alpha_thresh = los->GetAlphaThreshold();
    msg.material_mask = los->GetMaterialMask();
    msg.azimuth = los->GetRequestAzimuth();
    msg.elevation = los->GetRequestElevation();
    msg.min_range = los->GetRequestMinRange();
    msg.max_range = los->GetRequestMaxRange();
    msg.src_lat = los->GetSourceLat();
    msg.src_lon = los->GetSourceLon();
    msg.src_alt = los->GetSourceAlt();
    msg.src_x_offset = los->GetSourceXOffset();
    msg.src_y_offset = los->GetSourceYOffset();
    msg.src_z_offset = los->GetSourceZOffset();
    msg.dest_lat_x = los->GetDestLatX();
    msg.dest_lon_y = los->GetDestLonY();
    msg.dest_alt_z = los->GetDestAltZ();

    msg.use_target_entity = 0;          // Add this capability later.

    PostDriverMsg(msg);
}

void CEntityLOSPropPage::OnCheckLosDestGdc()
{
    // We will assume that since we are checking/unchecking the
    // button, it is visible so we are defining a segment.
    if (m_CheckDestGDC.GetCheck()) {
        m_LabelDestLatOrXOrMinRange.SetWindowText("Latitude:");
        m_LabelDestLonOrYOrMaxRange.SetWindowText("Longitude:");
        m_LabelDestAltOrZ.SetWindowText("Altitude:");
    } else {
        m_LabelDestLatOrXOrMinRange.SetWindowText("X Offset:");
        m_LabelDestLonOrYOrMaxRange.SetWindowText("Y Offset:");
        m_LabelDestAltOrZ.SetWindowText("Z Offset:");
    }
}

void CEntityLOSPropPage::OnSelchangeRange()
{
    CLos *los = GetSelLos();
    if (!los) {
        // If for some reason we have an invalid item in the list, just
        // get rid of it.  This should never happen, though.
        m_ComboID.DeleteString(m_ComboID.GetCurSel());
        EnableFields(FALSE);
        return;
    }

    EnableFields(TRUE);
    PopulateReqFields(los);
}

BOOL CEntityLOSPropPage::AddLosItem(CLos *los)
{
    if (!los)
        return FALSE;

    int index = m_ComboID.AddString((LPCTSTR)(los->GetName()));
    m_ComboID.SetItemData(index, los->GetID());

    return TRUE;
}

BOOL CEntityLOSPropPage::RemoveLosItem(CLos *los)
{
    if (!los)
        return FALSE;

    // Remove the item from the combo box.
    int id = los->GetID();
    int item_id = -1;
    int idx = 0;
    do {
        item_id = m_ComboID.GetItemData(idx);
        idx++;
    } while (item_id != id);
    m_ComboID.DeleteString(idx);

    return TRUE;
}

CLos *CEntityLOSPropPage::FindLos(const int id)
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return NULL;

    int mapidx = -1;
    CLos *los = NULL;
    CLosMap &map = entity->GetLosMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, mapidx, los);
        if ((los) && los->GetID() == id)
            return los;
    }

    return NULL;
}

CLos *CEntityLOSPropPage::GetSelLos(void)
{
    int idx = m_ComboID.GetCurSel();
    if (idx == -1)
        return NULL;

    int los_id = m_ComboID.GetItemData(idx);

    return FindLos(los_id);
}

void CEntityLOSPropPage::ClearLos(void)
{
    m_ComboID.ResetContent();
    RefreshView();
}

void CEntityLOSPropPage::RefreshView(void)
{
    CLos *los = GetSelLos();
    if (!los) {
        // If for some reason we have an invalid item in the list, just
        // get rid of it.  This should never happen, though.
        m_ComboID.DeleteString(m_ComboID.GetCurSel());
        return;
    }

    if (!m_ComboID.GetDroppedState())
        PopulateRespFields(los);
}

void CEntityLOSPropPage::EnableFields(BOOL enable)
{
    if (enable) {
        m_EditSourceZ.EnableWindow(TRUE);
        m_EditSourceY.EnableWindow(TRUE);
        m_EditSourceX.EnableWindow(TRUE);
        m_EditMaterialMask.EnableWindow(TRUE);
        m_EditReqEl.EnableWindow(TRUE);
        m_EditDestZ.EnableWindow(TRUE);
        m_EditDestYorMaxRange.EnableWindow(TRUE);
        m_EditDestXorMinRange.EnableWindow(TRUE);
        m_EditReqAz.EnableWindow(TRUE);
        m_EditAlphaThreshold.EnableWindow(TRUE);
        m_RadioSegment.EnableWindow(TRUE);
        m_RadioVector.EnableWindow(TRUE);
        m_CheckReqGDC.EnableWindow(TRUE);
        m_CheckExtResponse.EnableWindow(TRUE);
        m_CheckContinuous.EnableWindow(TRUE);
        m_ButtonRemove.EnableWindow(TRUE);
    } else {
        m_EditSourceZ.SetWindowText("");
        m_EditSourceY.SetWindowText("");
        m_EditSourceX.SetWindowText("");
        m_EditMaterialMask.SetWindowText("");
        m_EditReqEl.SetWindowText("");
        m_EditDestZ.SetWindowText("");
        m_EditDestYorMaxRange.SetWindowText("");
        m_EditDestXorMinRange.SetWindowText("");
        m_EditReqAz.SetWindowText("");
        m_EditAlphaThreshold.SetWindowText("");
        m_RadioSegment.SetCheck(1);
        m_RadioVector.SetCheck(0);
        m_CheckReqGDC.SetCheck(0);
        m_CheckExtResponse.SetCheck(0);
        m_CheckContinuous.SetCheck(0);

        m_EditSourceZ.EnableWindow(FALSE);
        m_EditSourceY.EnableWindow(FALSE);
        m_EditSourceX.EnableWindow(FALSE);
        m_EditMaterialMask.EnableWindow(FALSE);
        m_EditReqEl.EnableWindow(FALSE);
        m_EditDestZ.EnableWindow(FALSE);
        m_EditDestYorMaxRange.EnableWindow(FALSE);
        m_EditDestXorMinRange.EnableWindow(FALSE);
        m_EditReqAz.EnableWindow(FALSE);
        m_EditAlphaThreshold.EnableWindow(FALSE);
        m_RadioSegment.EnableWindow(FALSE);
        m_RadioVector.EnableWindow(FALSE);
        m_CheckReqGDC.EnableWindow(FALSE);
        m_CheckExtResponse.EnableWindow(FALSE);
        m_CheckContinuous.EnableWindow(FALSE);
        m_ButtonRemove.EnableWindow(FALSE);

        m_RespValid = FALSE;
        m_szRespCount = "0";
        m_szRespAlt = "";
        m_szRespLat = "";
        m_szRespLon = "";
        m_szRespEntityID = "";
        m_szRespAz = "";
        m_szRespEl = "";
        m_szRespRange = "";
        m_szRespMaterialCode = "";
        m_szRespColor = "";
        UpdateData(FALSE);
    }
}

void CEntityLOSPropPage::PopulateReqFields(CLos *los)
{
    CString str;

    str.Format("%lf", los->GetSourceXOffset());
    m_EditSourceX.SetWindowText((LPCTSTR)str);
    str.Format("%lf", los->GetSourceYOffset());
    m_EditSourceY.SetWindowText((LPCTSTR)str);
    str.Format("%lf", los->GetSourceZOffset());
    m_EditSourceZ.SetWindowText((LPCTSTR)str);
    str.Format("%.8X", los->GetMaterialMask());
    m_EditMaterialMask.SetWindowText((LPCTSTR)str);
    str.Format("%f", los->GetRequestAzimuth());
    m_EditReqAz.SetWindowText((LPCTSTR)str);
    str.Format("%f", los->GetRequestElevation());
    m_EditReqEl.SetWindowText((LPCTSTR)str);
    str.Format("%lf", los->GetDestAltZ());
    m_EditDestZ.SetWindowText((LPCTSTR)str);
    str.Format("%d", los->GetAlphaThreshold());
    m_EditAlphaThreshold.SetWindowText((LPCTSTR)str);

    if (los->GetRequestType() == LOS_TYPE_SEGMENT) {
        str.Format("%lf", los->GetDestLatX());
        m_EditDestXorMinRange.SetWindowText((LPCTSTR)str);
        str.Format("%lf", los->GetDestLonY());
        m_EditDestYorMaxRange.SetWindowText((LPCTSTR)str);

        m_RadioSegment.SetCheck(1);
        m_RadioVector.SetCheck(0);
    } else {
        str.Format("%f", los->GetRequestMinRange());
        m_EditDestXorMinRange.SetWindowText((LPCTSTR)str);
        str.Format("%f", los->GetRequestMaxRange());
        m_EditDestYorMaxRange.SetWindowText((LPCTSTR)str);

        m_RadioSegment.SetCheck(0);
        m_RadioVector.SetCheck(1);
    }

    m_CheckReqGDC.SetCheck(los->GetDesiredRespCoordSystem());
    m_CheckExtResponse.SetCheck(los->GetRequestExtended());
    m_CheckContinuous.SetCheck(los->GetContinuous());
}

void CEntityLOSPropPage::PopulateRespFields(CLos *los)
{
    if (los->RespEntityIsValid())
        m_szRespEntityID.Format("%d", los->GetRespEntityID());
    else
        m_szRespEntityID = "";

    if (los->GetRespCoordSystem() == LOS_COORD_SYSTEM_WORLD) {
        m_LabelRespLatOrXUnits.SetWindowText("deg");
        m_LabelRespLonOrYUnits.SetWindowText("deg");
        m_LabelRespLatOrX.SetWindowText("Latitude:");
        m_LabelRespLonOrY.SetWindowText("Longitude:");
        m_LabelRespAltOrZ.SetWindowText("Altitude:");
    } else {
        m_LabelRespLatOrXUnits.SetWindowText("m");
        m_LabelRespLonOrYUnits.SetWindowText("m");
        m_LabelRespLatOrX.SetWindowText("X Offset:");
        m_LabelRespLonOrY.SetWindowText("Y Offset:");
        m_LabelRespAltOrZ.SetWindowText("Z Offset:");
    }

    if (los->RespIsExtended()) {
        m_szRespLat.Format("%.5lf", los->GetRespLatX());
        m_szRespLon.Format("%.5lf", los->GetRespLonY());
        m_szRespAlt.Format("%.5lf", los->GetRespAltZ());
        m_szRespAz.Format("%f", los->GetRespAzimuth());
        m_szRespEl.Format("%f", los->GetRespElevation());
        m_szRespMaterialCode.Format("%.8X", los->GetRespMaterialCode());
        m_szRespColor.Format("%d %d %d %d", los->GetRespRed(), los->GetRespGreen(),
                             los->GetRespBlue(), los->GetRespAlpha());
    } else {
        m_szRespLat = "";
        m_szRespLon = "";
        m_szRespAlt = "";
        m_szRespAz = "";
        m_szRespEl = "";
        m_szRespMaterialCode = "";
        m_szRespColor = "";
    }

    m_szRespCount.Format("%d", los->GetRespCount());
    m_szRespEntityID.Format("%d", los->GetRespEntityID());
    m_szRespRange.Format("%.5lf", los->GetRespRange());

    m_RespValid = los->RespIsValid() ? TRUE : FALSE;

    UpdateData(FALSE);
}

void CEntityLOSPropPage::OnRadioLosTypeSegment()
{
    m_LabelDestination.ShowWindow(SW_SHOW);
    m_LabelDestAltOrZ.ShowWindow(SW_SHOW);
    m_EditDestZ.ShowWindow(SW_SHOW);
    m_LabelReqAz.ShowWindow(SW_HIDE);
    m_LabelReqEl.ShowWindow(SW_HIDE);
    m_EditReqAz.ShowWindow(SW_HIDE);
    m_EditReqEl.ShowWindow(SW_HIDE);
    OnCheckLosDestGdc();
}

void CEntityLOSPropPage::OnRadioLosTypeVector()
{
    m_LabelDestination.ShowWindow(SW_HIDE);
    m_LabelDestAltOrZ.ShowWindow(SW_HIDE);
    m_EditDestZ.ShowWindow(SW_HIDE);
    m_LabelReqAz.ShowWindow(SW_SHOW);
    m_LabelReqEl.ShowWindow(SW_SHOW);
    m_EditReqAz.ShowWindow(SW_SHOW);
    m_EditReqEl.ShowWindow(SW_SHOW);
    m_LabelDestLatOrXOrMinRange.SetWindowText("Min Range:");
    m_LabelDestLonOrYOrMaxRange.SetWindowText("Max Range:");
}

BOOL CEntityLOSPropPage::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Simulate selection of the Segment radio button.
    m_RadioSegment.SetCheck(TRUE);
    OnRadioLosTypeSegment();

    // Subclass the radio button that ClassWizard won't let us get
    // a variable to.
    m_RadioVector.SubclassDlgItem(IDC_RADIO_LOS_TYPE_VECTOR, this);

    // Destination coord system is Geodetic right now.
    m_CheckDestGDC.SetCheck(1);
    OnCheckLosDestGdc();

    EnableFields(FALSE);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}
