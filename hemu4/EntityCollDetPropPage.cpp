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
 *  FILENAME:   EntityCollDetPropPage.cpp
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
#include "CollisionSeg.h"
#include "CollisionVol.h"
#include "cigi4types.h"
#include "Entity.h"
#include "EntityCollDetPropPage.h"
#include "globals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityCollDetPropPage dialog

CEntityCollDetPropPage::CEntityCollDetPropPage(CWnd *pParent /*=NULL*/)
    : CDialog(CEntityCollDetPropPage::IDD, pParent)
{
    //{{AFX_DATA_INIT(CEntityCollDetPropPage)
    m_NotificationData = _T("");
    //}}AFX_DATA_INIT

    m_bHoldEnableCD = FALSE;
}

void CEntityCollDetPropPage::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEntityCollDetPropPage)
    DDX_Control(pDX, IDC_LABEL_Z1, m_LabelZ1);
    DDX_Control(pDX, IDC_LABEL_Y1, m_LabelY1);
    DDX_Control(pDX, IDC_LABEL_X1, m_LabelX1);
    DDX_Control(pDX, IDC_LABEL_Y_WIDTH_UNITS, m_LabelY2WidthUnits);
    DDX_Control(pDX, IDC_LABEL_Z_DEPTH_UNITS, m_LabelZ2DepthUnits);
    DDX_Control(pDX, IDC_LABEL_Z_DEPTH, m_LabelZ2Depth);
    DDX_Control(pDX, IDC_LABEL_YAW_UNITS, m_LabelYawUnits);
    DDX_Control(pDX, IDC_LABEL_ROLL_UNITS, m_LabelRollUnits);
    DDX_Control(pDX, IDC_LABEL_PITCH_UNITS, m_LabelPitchUnits);
    DDX_Control(pDX, IDC_LABEL_Y_WIDTH, m_LabelY2Width);
    DDX_Control(pDX, IDC_LABEL_X_HEIGHT_RADIUS, m_LabelX2HeightRadius);
    DDX_Control(pDX, IDC_LABEL_START_OR_TYPE, m_LabelStartOrType);
    DDX_Control(pDX, IDC_LABEL_YAW, m_LabelYaw);
    DDX_Control(pDX, IDC_LABEL_ROLL, m_LabelRoll);
    DDX_Control(pDX, IDC_LABEL_PITCH, m_LabelPitch);
    DDX_Control(pDX, IDC_LABEL_MASK, m_LabelMask);
    DDX_Control(pDX, IDC_LABEL_END, m_LabelEnd);
    DDX_Control(pDX, IDC_RADIO_SPHERE, m_RadioSphere);
    DDX_Control(pDX, IDC_GROUPBOX_SEGMENT, m_GroupBox);
    DDX_Control(pDX, IDC_EDIT_Z2_DEPTH, m_EditZ2Depth);
    DDX_Control(pDX, IDC_EDIT_Y2_WIDTH, m_EditY2Width);
    DDX_Control(pDX, IDC_EDIT_X2_HEIGHT_RADIUS, m_EditX2HeightRadius);
    DDX_Control(pDX, IDC_EDIT_Z1, m_EditZ1);
    DDX_Control(pDX, IDC_EDIT_Y1, m_EditY1);
    DDX_Control(pDX, IDC_EDIT_X1, m_EditX1);
    DDX_Control(pDX, IDC_EDIT_YAW, m_EditYaw);
    DDX_Control(pDX, IDC_EDIT_ROLL, m_EditRoll);
    DDX_Control(pDX, IDC_EDIT_PITCH, m_EditPitch);
    DDX_Control(pDX, IDC_EDIT_MASK, m_EditMask);
    DDX_Control(pDX, IDC_COMBO_COLLISION_ID, m_ComboID);
    DDX_Control(pDX, IDC_CHECK_SEG_OR_VOL_ENABLE, m_CheckEnableSegOrVol);
    DDX_Control(pDX, IDC_CHECK_COLL_ENABLE, m_CheckEnableCD);
    DDX_Control(pDX, IDC_BUTTON_REMOVE_COLL, m_ButtonRemove);
    DDX_Text(pDX, IDC_EDIT_NOTIFICATION, m_NotificationData);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEntityCollDetPropPage, CDialog)
    //{{AFX_MSG_MAP(CEntityCollDetPropPage)
    ON_BN_CLICKED(IDC_RADIO_CUBOID, OnRadioCuboid)
    ON_BN_CLICKED(IDC_RADIO_SPHERE, OnRadioSphere)
    ON_BN_CLICKED(IDC_BUTTON_ADD_SEGMENT, OnButtonAddSegment)
    ON_BN_CLICKED(IDC_BUTTON_ADD_VOLUME, OnButtonAddVolume)
    ON_BN_CLICKED(IDC_BUTTON_REMOVE_COLL, OnButtonRemoveColl)
    ON_BN_CLICKED(IDC_BUTTON_APPLY_COLL, OnButtonApplyColl)
    ON_CBN_SELCHANGE(IDC_COMBO_COLLISION_ID, OnSelchangeComboID)
    ON_BN_CLICKED(IDC_CHECK_COLL_ENABLE, OnCheckCollEnable)
    ON_BN_CLICKED(IDC_CHECK_SEG_OR_VOL_ENABLE, OnCheckSegOrVolEnable)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityCollDetPropPage message handlers

void CEntityCollDetPropPage::OnOK()
{
}

void CEntityCollDetPropPage::OnCancel()
{
}

BOOL CEntityCollDetPropPage::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Simulate selection of the Sphere radio button.
    m_RadioSphere.SetCheck(TRUE);
    OnRadioSphere();

    // Subclass the radio button that ClassWizard won't let us get
    // a variable to.
    m_RadioCuboid.SubclassDlgItem(IDC_RADIO_CUBOID, this);

    EnableFields(FALSE);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

BOOL CEntityCollDetPropPage::AddCollisionSegItem(CCollisionSeg *seg)
{
    if (!seg)
        return FALSE;

    CString str;
    str.Format("Segment %d \"%s\"", seg->GetID(), (LPCTSTR)(seg->GetName()));
    int index = m_ComboID.AddString((LPCTSTR)str);
    m_ComboID.SetItemData(index, MAKELONG((short)seg->GetID(), 0));

    return TRUE;
}

BOOL CEntityCollDetPropPage::RemoveCollisionSegItem(CCollisionSeg *seg)
{
    if (!seg)
        return FALSE;

    // Remove the item from the combo box.
    int id = seg->GetID();
    int item_data = 0;
    int item_type = -1;
    int item_id = -1;
    int idx = 0;
    do {
        // The high word contains the type (0) and the low word contains
        // the segment ID.
        item_data = m_ComboID.GetItemData(idx);
        item_id = (int)LOWORD(item_data);
        item_type = (int)HIWORD(item_data);
        idx++;
    } while ((item_type != 0) || (item_id != id));
    m_ComboID.DeleteString(idx);

    return TRUE;
}

BOOL CEntityCollDetPropPage::AddCollisionVolItem(CCollisionVol *vol)
{
    if (!vol)
        return FALSE;

    CString str;
    str.Format("Volume %d \"%s\"", vol->GetID(), (LPCTSTR)(vol->GetName()));
    int index = m_ComboID.AddString((LPCTSTR)str);
    m_ComboID.SetItemData(index, MAKELONG((short)vol->GetID(), 1));

    return TRUE;
}

BOOL CEntityCollDetPropPage::RemoveCollisionVolItem(CCollisionVol *vol)
{
    if (!vol)
        return FALSE;

    // Remove the item from the combo box.
    int id = vol->GetID();
    int item_data = 0;
    int item_type = -1;
    int item_id = -1;
    int idx = 0;
    do {
        // The high word contains the type (0) and the low word contains
        // the segment ID.
        item_data = m_ComboID.GetItemData(idx);
        item_id = (int)LOWORD(item_data);
        item_type = (int)HIWORD(item_data);
        idx++;
    } while ((item_type != 1) || (item_id != id));
    m_ComboID.DeleteString(idx);

    return TRUE;
}

CCollisionSeg *CEntityCollDetPropPage::FindSegment(const int id)
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return NULL;

    int mapidx = -1;
    CCollisionSeg *seg = NULL;
    CCollisionSegMap &map = entity->GetCollisionSegMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, mapidx, seg);
        if ((seg) && seg->GetID() == id)
            return seg;
    }

    return NULL;
}

CCollisionVol *CEntityCollDetPropPage::FindVolume(const int id)
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return NULL;

    int mapidx = -1;
    CCollisionVol *vol = NULL;
    CCollisionVolMap &map = entity->GetCollisionVolMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, mapidx, vol);
        if ((vol) && vol->GetID() == id)
            return vol;
    }

    return NULL;
}

CCollisionSeg *CEntityCollDetPropPage::GetSelSegment(void)
{
    int idx = m_ComboID.GetCurSel();
    if (idx == -1)
        return NULL;

    DWORD data = m_ComboID.GetItemData(idx);

    // We only want segments.  These will have a 0 in the high 16 bits.
    if (HIWORD(data) != 0)
        return NULL;

    // The id will be in the low word.
    short seg_id = LOWORD(data);

    return FindSegment((int)seg_id);
}

CCollisionVol *CEntityCollDetPropPage::GetSelVolume(void)
{
    int idx = m_ComboID.GetCurSel();
    if (idx == -1)
        return NULL;

    DWORD data = m_ComboID.GetItemData(idx);

    // We only want volumes.  These will have a 1 in the high 16 bits.
    if (HIWORD(data) != 1)
        return NULL;

    // The id will be in the low word.
    short vol_id = LOWORD(data);

    return FindVolume((int)vol_id);
}

void CEntityCollDetPropPage::ClearSegsAndVols(void)
{
    m_ComboID.ResetContent();
    RefreshView();
}

void CEntityCollDetPropPage::RefreshView(void)
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Show whether collision detection is enabled for the entity.
    // We don't want to update the check box the frame
    // immediately following user action because the 1-frame
    // lag from the driver may cause flutter.
    if (!m_bHoldEnableCD)
        m_CheckEnableCD.SetCheck(entity->GetCollDetectEnable() ? 1 : 0);
    else
        m_bHoldEnableCD = FALSE;

    CCollisionSeg *seg = GetSelSegment();
    if (seg) {
        PopulateReqFields(seg);
        return;
    }

    CCollisionVol *vol = GetSelVolume();
    if (vol) {
        PopulateReqFields(vol);
        return;
    }

    // If for some reason we have an invalid item in the list, just
    // get rid of it.  This should never happen, though.
    m_ComboID.DeleteString(m_ComboID.GetCurSel());
}

void CEntityCollDetPropPage::EnableFields(BOOL enable)
{
    if (enable) {
        m_RadioSphere.EnableWindow(TRUE);
        m_RadioCuboid.EnableWindow(TRUE);
        m_EditZ2Depth.EnableWindow(TRUE);
        m_EditY2Width.EnableWindow(TRUE);
        m_EditX2HeightRadius.EnableWindow(TRUE);
        m_EditZ1.EnableWindow(TRUE);
        m_EditY1.EnableWindow(TRUE);
        m_EditX1.EnableWindow(TRUE);
        m_EditYaw.EnableWindow(TRUE);
        m_EditRoll.EnableWindow(TRUE);
        m_EditPitch.EnableWindow(TRUE);
        m_EditMask.EnableWindow(TRUE);
        m_CheckEnableSegOrVol.EnableWindow(TRUE);
        m_ButtonRemove.EnableWindow(TRUE);
    } else {
        m_EditZ2Depth.SetWindowText("");
        m_EditY2Width.SetWindowText("");
        m_EditX2HeightRadius.SetWindowText("");
        m_EditZ1.SetWindowText("");
        m_EditY1.SetWindowText("");
        m_EditX1.SetWindowText("");
        m_EditYaw.SetWindowText("");
        m_EditRoll.SetWindowText("");
        m_EditPitch.SetWindowText("");
        m_EditMask.SetWindowText("");
        m_CheckEnableSegOrVol.SetCheck(0);
        m_RadioSphere.EnableWindow(FALSE);
        m_RadioCuboid.EnableWindow(FALSE);
        m_EditZ2Depth.EnableWindow(FALSE);
        m_EditY2Width.EnableWindow(FALSE);
        m_EditX2HeightRadius.EnableWindow(FALSE);
        m_EditZ1.EnableWindow(FALSE);
        m_EditY1.EnableWindow(FALSE);
        m_EditX1.EnableWindow(FALSE);
        m_EditYaw.EnableWindow(FALSE);
        m_EditRoll.EnableWindow(FALSE);
        m_EditPitch.EnableWindow(FALSE);
        m_EditMask.EnableWindow(FALSE);
        m_CheckEnableSegOrVol.EnableWindow(FALSE);
        m_ButtonRemove.EnableWindow(FALSE);
    }
}

void CEntityCollDetPropPage::PopulateReqFields(CCollisionSeg *seg)
{
    CString str;

    if (m_ComboID.GetDroppedState())
        return;

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity) {
        EnableFields(FALSE);
        return;
    }

    m_CheckEnableCD.SetCheck(entity->GetCollDetectEnable());
    m_CheckEnableSegOrVol.SetCheck(seg->GetEnable());
    m_CheckEnableSegOrVol.SetWindowText("Enable Segment");

    m_LabelX1.SetWindowText("   X:");
    m_LabelY1.SetWindowText("   Y:");
    m_LabelZ1.SetWindowText("   Z:");
    m_LabelX2HeightRadius.SetWindowText("   X:");
    m_LabelY2Width.SetWindowText("   Y:");
    m_LabelZ2Depth.SetWindowText("   Z:");
    m_LabelY2Width.ShowWindow(SW_SHOW);
    m_LabelZ2Depth.ShowWindow(SW_SHOW);
    m_LabelY2WidthUnits.ShowWindow(SW_SHOW);
    m_LabelZ2DepthUnits.ShowWindow(SW_SHOW);
    m_LabelStartOrType.SetWindowText("Start:");
    m_LabelEnd.ShowWindow(SW_SHOW);
    m_LabelYaw.ShowWindow(SW_HIDE);
    m_LabelRoll.ShowWindow(SW_HIDE);
    m_LabelPitch.ShowWindow(SW_HIDE);
    m_LabelYawUnits.ShowWindow(SW_HIDE);
    m_LabelRollUnits.ShowWindow(SW_HIDE);
    m_LabelPitchUnits.ShowWindow(SW_HIDE);
    m_LabelMask.ShowWindow(SW_SHOW);
    m_LabelMask.SetWindowText("Material Mask:");
    m_EditMask.ShowWindow(SW_SHOW);
    m_EditY2Width.ShowWindow(SW_SHOW);
    m_EditZ2Depth.ShowWindow(SW_SHOW);
    m_EditYaw.ShowWindow(SW_HIDE);
    m_EditPitch.ShowWindow(SW_HIDE);
    m_EditRoll.ShowWindow(SW_HIDE);
    m_RadioSphere.ShowWindow(SW_HIDE);
    m_RadioCuboid.ShowWindow(SW_HIDE);

    str.Format("%.8X", seg->GetMaterialMask());
    m_EditMask.SetWindowText((LPCTSTR)str);
    str.Format("%f", seg->GetEndZ());
    m_EditZ2Depth.SetWindowText((LPCTSTR)str);
    str.Format("%f", seg->GetEndY());
    m_EditY2Width.SetWindowText((LPCTSTR)str);
    str.Format("%f", seg->GetEndX());
    m_EditX2HeightRadius.SetWindowText((LPCTSTR)str);
    str.Format("%f", seg->GetStartZ());
    m_EditZ1.SetWindowText((LPCTSTR)str);
    str.Format("%f", seg->GetStartY());
    m_EditY1.SetWindowText((LPCTSTR)str);
    str.Format("%f", seg->GetStartX());
    m_EditX1.SetWindowText((LPCTSTR)str);
}

void CEntityCollDetPropPage::PopulateReqFields(CCollisionVol *vol)
{
    CString str;

    if (m_ComboID.GetDroppedState())
        return;

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity) {
        EnableFields(FALSE);
        return;
    }

    m_CheckEnableCD.SetCheck(entity->GetCollDetectEnable());
    m_CheckEnableSegOrVol.SetCheck(vol->GetEnable());
    m_CheckEnableSegOrVol.SetWindowText("Enable Volume");
    m_LabelMask.ShowWindow(SW_HIDE);
    m_EditMask.ShowWindow(SW_HIDE);
    m_LabelMask.SetWindowText("Material Mask:");
    m_LabelStartOrType.SetWindowText("Volume Type:");
    m_LabelEnd.ShowWindow(SW_HIDE);
    m_RadioSphere.ShowWindow(SW_SHOW);
    m_RadioCuboid.ShowWindow(SW_SHOW);
    m_LabelX1.SetWindowText("X:");
    m_LabelY1.SetWindowText("Y:");
    m_LabelZ1.SetWindowText("Z:");

    if (vol->GetType() == COLLISION_VOLUME_SPHERE) {
        m_RadioSphere.SetCheck(1);
        m_RadioCuboid.SetCheck(0);
        m_LabelX2HeightRadius.SetWindowText("Radius:");
        m_LabelY2Width.ShowWindow(SW_HIDE);
        m_LabelZ2Depth.ShowWindow(SW_HIDE);
        m_LabelY2WidthUnits.ShowWindow(SW_HIDE);
        m_LabelZ2DepthUnits.ShowWindow(SW_HIDE);
        m_LabelYaw.ShowWindow(SW_HIDE);
        m_LabelRoll.ShowWindow(SW_HIDE);
        m_LabelPitch.ShowWindow(SW_HIDE);
        m_LabelYawUnits.ShowWindow(SW_HIDE);
        m_LabelPitchUnits.ShowWindow(SW_HIDE);
        m_LabelRollUnits.ShowWindow(SW_HIDE);
        m_EditY2Width.ShowWindow(SW_HIDE);
        m_EditZ2Depth.ShowWindow(SW_HIDE);
        m_EditYaw.ShowWindow(SW_HIDE);
        m_EditPitch.ShowWindow(SW_HIDE);
        m_EditRoll.ShowWindow(SW_HIDE);
    } else {
        m_RadioSphere.SetCheck(0);
        m_RadioCuboid.SetCheck(1);
        m_LabelX2HeightRadius.SetWindowText("Height:");
        m_LabelY2Width.SetWindowText("Width:");
        m_LabelZ2Depth.SetWindowText("Depth:");
        m_LabelY2Width.ShowWindow(SW_SHOW);
        m_LabelZ2Depth.ShowWindow(SW_SHOW);
        m_LabelY2WidthUnits.ShowWindow(SW_SHOW);
        m_LabelZ2DepthUnits.ShowWindow(SW_SHOW);
        m_LabelYaw.ShowWindow(SW_SHOW);
        m_LabelRoll.ShowWindow(SW_SHOW);
        m_LabelPitch.ShowWindow(SW_SHOW);
        m_LabelYawUnits.ShowWindow(SW_SHOW);
        m_LabelPitchUnits.ShowWindow(SW_SHOW);
        m_LabelRollUnits.ShowWindow(SW_SHOW);
        m_EditY2Width.ShowWindow(SW_SHOW);
        m_EditZ2Depth.ShowWindow(SW_SHOW);
        m_EditYaw.ShowWindow(SW_SHOW);
        m_EditPitch.ShowWindow(SW_SHOW);
        m_EditRoll.ShowWindow(SW_SHOW);
    }

    str.Format("%f", vol->GetDepth());
    m_EditZ2Depth.SetWindowText((LPCTSTR)str);
    str.Format("%f", vol->GetWidth());
    m_EditY2Width.SetWindowText((LPCTSTR)str);
    str.Format("%f", vol->GetHeightOrRadius());
    m_EditX2HeightRadius.SetWindowText((LPCTSTR)str);
    str.Format("%f", vol->GetZ());
    m_EditZ1.SetWindowText((LPCTSTR)str);
    str.Format("%f", vol->GetY());
    m_EditY1.SetWindowText((LPCTSTR)str);
    str.Format("%f", vol->GetX());
    m_EditX1.SetWindowText((LPCTSTR)str);
    str.Format("%f", vol->GetYaw());
    m_EditYaw.SetWindowText((LPCTSTR)str);
    str.Format("%f", vol->GetPitch());
    m_EditPitch.SetWindowText((LPCTSTR)str);
    str.Format("%f", vol->GetRoll());
    m_EditRoll.SetWindowText((LPCTSTR)str);
}

void CEntityCollDetPropPage::PopulateRespFields(void)
{
    // Clear the existing text.
    m_NotificationData = "";

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity) {
        UpdateData(FALSE);
        return;
    }

    CString temp1;
    CString temp2;
    POSITION pos = NULL;
    int id = -1;
    CCollisionSeg *seg = NULL;
    CCollisionVol *vol = NULL;
    COLLISION_SEG_NOTIFICATION *seg_resp = NULL;
    COLLISION_VOL_NOTIFICATION *vol_resp = NULL;
    CCollisionSegMap &segmap = entity->GetCollisionSegMap();
    CCollisionVolMap &volmap = entity->GetCollisionVolMap();

    // Iterate through the segment map and build the text for each item.
    pos = segmap.GetStartPosition();
    while (pos) {
        segmap.GetNextAssoc(pos, id, seg);
        if (seg) {
            seg_resp = seg->GetFirstNotification();
            while (seg_resp) {
                temp1.Format("Segment %d -> ", seg_resp->identity.id);
                if (seg_resp->type == 0)
                    temp2 = "Non-entity\r\n";
                else
                    temp2.Format("Entity %d\r\n", seg_resp->contacted_entity_id);

                m_NotificationData += temp1 + temp2;

                seg_resp = seg->GetNextNotification(seg_resp);
            }
        }
    }

    // Now do the same thing for each volume response.
    pos = volmap.GetStartPosition();
    while (pos) {
        volmap.GetNextAssoc(pos, id, vol);
        if (vol) {
            vol_resp = vol->GetFirstNotification();
            while (vol_resp) {
                temp1.Format("Volume %d -> ", vol_resp->identity.id);
                if (vol_resp->type == 0)
                    temp2 = "Non-entity\r\n";
                else
                    temp2.Format("Entity %d\r\n", vol_resp->contacted_entity_id);

                m_NotificationData += temp1 + temp2;

                vol_resp = vol->GetNextNotification(vol_resp);
            }
        }
    }

    UpdateData(FALSE);
}

void CEntityCollDetPropPage::OnRadioCuboid()
{
    CCollisionVol *vol = GetSelVolume();
    if (!vol)
        return;

    vol->SetType(COLLISION_VOLUME_CUBOID);
    PopulateReqFields(vol);
}

void CEntityCollDetPropPage::OnRadioSphere()
{
    CCollisionVol *vol = GetSelVolume();
    if (!vol)
        return;

    vol->SetType(COLLISION_VOLUME_SPHERE);
    PopulateReqFields(vol);
}

void CEntityCollDetPropPage::OnButtonAddSegment()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Find the next available index.
    int seg_id = entity->GetNextCollisionSegID();

    // Create a new LOS and add it to the entity.
    CCollisionSeg *seg = new CCollisionSeg(seg_id, entity->GetID());
    entity->AddCollisionSeg(seg);

    // Add the segment to the combo box list.  Segments will have a
    // 0 in the high 16 bits
    int idx = m_ComboID.AddString((LPCTSTR)(seg->GetName()));
    m_ComboID.SetItemData(idx, MAKELONG((short)seg->GetID(), 0));

    // Select the new item.
    m_ComboID.SetCurSel(idx);
    OnSelchangeComboID();

    // Send a message to the driver.
    // LWD
}

void CEntityCollDetPropPage::OnButtonAddVolume()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Find the next available index.
    int vol_id = entity->GetNextCollisionVolID();

    // Create a new LOS and add it to the entity.
    CCollisionVol *vol = new CCollisionVol(vol_id, entity->GetID());
    entity->AddCollisionVol(vol);

    // Add the segment to the combo box list.  Volumes will have a
    // 1 in the high 16 bits
    int idx = m_ComboID.AddString((LPCTSTR)(vol->GetName()));
    m_ComboID.SetItemData(idx, MAKELONG((short)vol->GetID(), 1));

    // Select the new item.
    m_ComboID.SetCurSel(idx);
    OnSelchangeComboID();

    // Send a message to the driver.
    // LWD
}

void CEntityCollDetPropPage::OnButtonRemoveColl()
{
    int id = -1;
    int item_data = 0;
    int item_type = -1;
    int item_id = -1;
    int idx = 0;
    CCollisionSeg *seg = GetSelSegment();
    CCollisionVol *vol = GetSelVolume();

    if (seg) {
        // First remove the item from the list box.
        id = seg->GetID();
        do {
            // The high word contains the type (0) and the low word contains
            // the segment ID.
            item_data = m_ComboID.GetItemData(idx);
            item_id = (int)LOWORD(item_data);
            item_type = (int)HIWORD(item_data);
            idx++;
        } while ((item_type != 0) || (item_id != id));
        idx--;                          // idx has been incremented past the item
        m_ComboID.DeleteString(idx);

        // Select the previous item.
        if (idx > 1)
            m_ComboID.SetCurSel(idx - 1);
        else
            m_ComboID.SetCurSel(0);

        OnSelchangeComboID();

        // Send a message to the driver.
        // LWD

        // Finally, delete the segment from the CEntity.
        g_DataManager.GetSelectedEntity()->DeleteCollisionSeg(id);
    } else if (vol) {
        // First remove the item from the list box.
        id = vol->GetID();
        do {
            // The high word contains the type (1) and the low word contains
            // the volume ID.
            item_data = m_ComboID.GetItemData(idx);
            item_id = (int)LOWORD(item_data);
            item_type = (int)HIWORD(item_data);
            idx++;
        } while ((item_type != 1) || (item_id != id));
        idx--;                          // idx has been incremented past the item
        m_ComboID.DeleteString(idx);

        // Select the previous item.
        if (idx > 1)
            m_ComboID.SetCurSel(idx - 1);
        else
            m_ComboID.SetCurSel(0);

        OnSelchangeComboID();

        // Send a message to the driver.
        // LWD

        // Finally, delete the volume from the CEntity.
        g_DataManager.GetSelectedEntity()->DeleteCollisionVol(id);
    }
}

void CEntityCollDetPropPage::OnButtonApplyColl()
{
    CString str;
    unsigned int tempui = 0;
    float tempf = 0.0;
    CCollisionSeg *seg = GetSelSegment();
    CCollisionVol *vol = GetSelVolume();

    if (seg) {
        m_EditX1.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        seg->SetStartX(tempf);

        m_EditY1.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        seg->SetStartY(tempf);

        m_EditZ1.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        seg->SetStartZ(tempf);

        m_EditX2HeightRadius.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        seg->SetEndX(tempf);

        m_EditY2Width.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        seg->SetEndY(tempf);

        m_EditZ2Depth.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        seg->SetEndZ(tempf);

        m_EditMask.GetWindowText(str);
        sscanf((LPCTSTR)str, "%X", &tempui);
        seg->SetMaterialMask(tempui);

        SendCollisionSegDefToDriver(seg);
    } else if (vol) {
        vol->SetType(m_RadioCuboid.GetCheck() ? COLLISION_VOLUME_CUBOID
                     : COLLISION_VOLUME_SPHERE);

        m_EditX1.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        vol->SetX(tempf);

        m_EditY1.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        vol->SetY(tempf);

        m_EditZ1.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        vol->SetZ(tempf);

        m_EditX2HeightRadius.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        vol->SetHeightOrRadius(tempf);

        m_EditY2Width.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        vol->SetWidth(tempf);

        m_EditZ2Depth.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        vol->SetDepth(tempf);

        m_EditYaw.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        vol->SetYaw(tempf);

        m_EditPitch.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        vol->SetPitch(tempf);

        m_EditRoll.GetWindowText(str);
        sscanf((LPCTSTR)str, "%f", &tempf);
        vol->SetRoll(tempf);

        SendCollisionVolDefToDriver(vol);
    } else {
        // If for some reason we have an invalid item in the list, just
        // get rid of it.  This should never happen, though.
        m_ComboID.DeleteString(m_ComboID.GetCurSel());
        EnableFields(FALSE);
    }
}

void CEntityCollDetPropPage::SendCollisionSegDefToDriver(CCollisionSeg *seg)
{
    // Unlike HAT/HOT and LOS, where we send a message to the driver and
    // the driver and the driver creates the CIGI message, we can just
    // create the CIGI message here.
    CIGI_COLL_SEGMENT_DEF packet;
    packet.packet_id = CIGI_COLL_SEGMENT_DEF_OPCODE;
    packet.packet_size = sizeof(CIGI_COLL_SEGMENT_DEF);
    packet.entity_id = seg->GetOwnerID();
    packet.segment_id = seg->GetID();
    packet.segment_enable = seg->GetEnable();
    packet.x1 = seg->GetStartX();
    packet.y1 = seg->GetStartY();
    packet.z1 = seg->GetStartZ();
    packet.x2 = seg->GetEndX();
    packet.y2 = seg->GetEndY();
    packet.z2 = seg->GetEndZ();
    packet.material_mask = seg->GetMaterialMask();

    SendImmedCigiMessage(&packet);
}

void CEntityCollDetPropPage::SendCollisionVolDefToDriver(CCollisionVol *vol)
{
    // Unlike HAT/HOT and LOS, where we send a message to the driver and
    // the driver and the driver creates the CIGI message, we can just
    // create the CIGI message here.
    CIGI_COLL_VOLUME_DEF packet;
    packet.packet_id = CIGI_COLL_VOLUME_DEF_OPCODE;
    packet.packet_size = sizeof(CIGI_COLL_VOLUME_DEF);
    packet.entity_id = vol->GetOwnerID();
    packet.volume_id = vol->GetID();
    packet.volume_enable = vol->GetEnable();
    packet.volume_type = vol->GetType();
    packet.x = vol->GetX();
    packet.y = vol->GetY();
    packet.z = vol->GetZ();
    packet.height_radius = vol->GetHeightOrRadius();
    packet.width = vol->GetWidth();
    packet.depth = vol->GetDepth();
    packet.roll = vol->GetRoll();
    packet.pitch = vol->GetPitch();
    packet.yaw = vol->GetYaw();

    SendImmedCigiMessage(&packet);
}

void CEntityCollDetPropPage::OnSelchangeComboID()
{
    CCollisionSeg *seg = GetSelSegment();
    CCollisionVol *vol = GetSelVolume();
    if (seg) {
        EnableFields(TRUE);
        PopulateReqFields(seg);
    } else if (vol) {
        EnableFields(TRUE);
        PopulateReqFields(vol);
    } else {
        // If for some reason we have an invalid item in the list, just
        // get rid of it.  This should never happen, though.
        m_ComboID.DeleteString(m_ComboID.GetCurSel());
        EnableFields(FALSE);
    }
}

void CEntityCollDetPropPage::OnCheckCollEnable()
{
    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldEnableCD = TRUE;

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (entity)
        entity->SetCollDetectEnable(m_CheckEnableCD.GetCheck() ? TRUE : FALSE);
}

void CEntityCollDetPropPage::OnCheckSegOrVolEnable()
{
    CCollisionSeg *seg = GetSelSegment();
    CCollisionVol *vol = GetSelVolume();

    if (seg) {
        EnableFields(TRUE);
        seg->SetEnable(m_CheckEnableSegOrVol.GetCheck() ? TRUE : FALSE);
    } else if (vol) {
        EnableFields(TRUE);
        vol->SetEnable(m_CheckEnableSegOrVol.GetCheck() ? TRUE : FALSE);
    } else {
        EnableFields(FALSE);
    }
}

void CEntityCollDetPropPage::AddCollisionDataItem(CIGI_COLL_SEGMENT_NOTIFICATION *packet)
{
    CString str;
    if (packet->collision_type) {
        str.Format("Segment %d --> Entity %d\r\n",
                   packet->segment_id, packet->contacted_entity_id);
    } else {
        str.Format("Segment %d -- No entity%d\r\n", packet->segment_id);
    }

    m_NotificationData += str;
    UpdateData(FALSE);
}

void CEntityCollDetPropPage::AddCollisionDataItem(CIGI_COLL_VOLUME_NOTIFICATION *packet)
{
    CString str;
    if (packet->collision_type) {
        str.Format("Volume %d --> Entity %d, Volume %d\r\n",
                   packet->volume_id, packet->contacted_entity_id,
                   packet->contacted_volume_id);
    } else {
        str.Format("Volume %d -- No entity, Segment %d\r\n",
                   packet->volume_id, packet->contacted_volume_id);
    }

    m_NotificationData += str;
    UpdateData(FALSE);
}
