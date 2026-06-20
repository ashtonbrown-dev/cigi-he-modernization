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
 *  FILENAME:   EntityStateView.cpp
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
#include "DebugTrace.h"
#include "Entity.h"
#include "EntityStateView.h"
#include "EntityTreeView.h"
#include "globals.h"
#include "Hemu4.h"
#include "SituationDispCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityStateView dialog

const char CEntityStateView::m_StrLookup[4][13] = { "Normal",
                                                    "Magic Carpet",
                                                    "Roll/Pitch",
                                                    "Yaw/Z"
                                                  };

CEntityStateView::CEntityStateView(CWnd *pParent /*=NULL*/)
    : CBaseStateView(CEntityStateView::IDD, pParent)
{
    //{{AFX_DATA_INIT(CEntityStateView)
    m_bAnimLoop = FALSE;
    m_bAnimRev = FALSE;
    m_bFlyWaypoints = FALSE;
    m_bHideGeometry = FALSE;
    m_bInheritAlpha = FALSE;
    m_bSendRates = FALSE;
    m_Alpha = 0;
    m_Altitude = 0.0;
    m_Bearing = 0.0;
    m_EntityID = 0;
    m_Latitude = 0.0;
    m_Longitude = 0.0;
    m_Pitch = 0.0;
    m_Range = 0.0;
    m_Roll = 0.0;
    m_Speed = 0.0;
    m_EntityType = 0;
    m_Yaw = 0.0;
    m_strAltitudeUnit = _T("m");
    m_strAltitudeLabel = _T("Altitude:");
    m_strLatitudeUnit = _T("deg");
    m_strLatitudeLabel = _T("Latitude:");
    m_strLongitudeUnit = _T("deg");
    m_strLongitudeLabel = _T("Longitude:");
    m_strSpeedUnit = _T("knots");
    m_strSpeedLabel = _T("Speed:");
    m_GroundClamp = 0;
    m_strFlyMode = _T(m_StrLookup[0]);
    m_RatesCoordSystem = -1;
    //}}AFX_DATA_INIT

    m_PrevEntityID = -1;
    m_ParentID = -1;
    m_RefEntityID = -1;
    m_bHoldAnimLoop = FALSE;
    m_bHoldAnimRev = FALSE;
    m_bHoldFlyWaypoints = FALSE;
    m_bHoldHideGeometry = FALSE;
    m_bHoldInheritAlpha = FALSE;
    m_bHoldRatesCoordSystem = FALSE;
    m_bHoldSendRates = FALSE;
    m_bHoldClampMode = FALSE;
}

void CEntityStateView::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(CEntityStateView)
    DDX_Control(pDX, IDC_RADIO_RATES_PARENT, m_RadioRatesParent);
    DDX_Control(pDX, IDC_RADIO_RATES_LOCAL, m_RadioRatesLocal);
    DDX_Control(pDX, IDC_RADIO_ENTITY_GROUND_CLAMP_NONE, m_RadioClampNone);
    DDX_Control(pDX, IDC_RADIO_ENTITY_GROUND_CLAMP_ALT_AND_ATT, m_RadioClampConformal);
    DDX_Control(pDX, IDC_RADIO_ENTITY_GROUND_CLAMP_ALT, m_RadioClampNonconformal);
    DDX_Control(pDX, IDC_CHECK_ENTITY_RATES, m_CheckSendRates);
    DDX_Control(pDX, IDC_CHECK_ENTITY_INHERIT_ALPHA, m_CheckInheritAlpha);
    DDX_Control(pDX, IDC_CHECK_ENTITY_HIDE_GEOMETRY, m_CheckHideGeometry);
    DDX_Control(pDX, IDC_CHECK_ENTITY_FLY_WAYPOINTS, m_CheckFlyWaypoints);
    DDX_Control(pDX, IDC_CHECK_ENTITY_ANIMATION_REVERSE, m_CheckAnimRev);
    DDX_Control(pDX, IDC_CHECK_ENTITY_ANIMATION_LOOP, m_CheckAnimLoop);
    DDX_Control(pDX, IDC_EDIT_ENTITY_YAW, m_EditYaw);
    DDX_Control(pDX, IDC_EDIT_ENTITY_TYPE, m_EditType);
    DDX_Control(pDX, IDC_EDIT_ENTITY_SPEED, m_EditSpeed);
    DDX_Control(pDX, IDC_EDIT_ENTITY_ROLL, m_EditRoll);
    DDX_Control(pDX, IDC_EDIT_ENTITY_RANGE, m_EditRange);
    DDX_Control(pDX, IDC_EDIT_ENTITY_PITCH, m_EditPitch);
    DDX_Control(pDX, IDC_EDIT_ENTITY_LONGITUDE, m_EditLongitude);
    DDX_Control(pDX, IDC_EDIT_ENTITY_LATITUDE, m_EditLatitude);
    DDX_Control(pDX, IDC_EDIT_ENTITY_ID, m_EditID);
    DDX_Control(pDX, IDC_EDIT_ENTITY_BEARING, m_EditBearing);
    DDX_Control(pDX, IDC_EDIT_ENTITY_ALTITUDE, m_EditAltitude);
    DDX_Control(pDX, IDC_EDIT_ENTITY_ALPHA, m_EditAlpha);
    DDX_Control(pDX, IDC_TAB1, m_Tab);
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_RELATIVE_TO, m_cboRefEntity);
    DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_PARENT, m_cboParent);
    DDX_Check(pDX, IDC_CHECK_ENTITY_ANIMATION_LOOP, m_bAnimLoop);
    DDX_Check(pDX, IDC_CHECK_ENTITY_ANIMATION_REVERSE, m_bAnimRev);
    DDX_Check(pDX, IDC_CHECK_ENTITY_FLY_WAYPOINTS, m_bFlyWaypoints);
    DDX_Check(pDX, IDC_CHECK_ENTITY_HIDE_GEOMETRY, m_bHideGeometry);
    DDX_Check(pDX, IDC_CHECK_ENTITY_INHERIT_ALPHA, m_bInheritAlpha);
    DDX_Check(pDX, IDC_CHECK_ENTITY_RATES, m_bSendRates);
    DDX_Text(pDX, IDC_EDIT_ENTITY_ALPHA, m_Alpha);
    DDX_Text(pDX, IDC_EDIT_ENTITY_ID, m_EntityID);
    DDX_Text(pDX, IDC_EDIT_ENTITY_TYPE, m_EntityType);
    DDX_Text(pDX, IDC_LABEL_ENTITY_ALT_UNIT, m_strAltitudeUnit);
    DDX_Text(pDX, IDC_LABEL_ENTITY_ALTITUDE, m_strAltitudeLabel);
    DDX_Text(pDX, IDC_LABEL_ENTITY_LAT_UNIT, m_strLatitudeUnit);
    DDX_Text(pDX, IDC_LABEL_ENTITY_LATITUDE, m_strLatitudeLabel);
    DDX_Text(pDX, IDC_LABEL_ENTITY_LON_UNIT, m_strLongitudeUnit);
    DDX_Text(pDX, IDC_LABEL_ENTITY_LONGITUDE, m_strLongitudeLabel);
    DDX_Text(pDX, IDC_LABEL_SPEED_UNIT, m_strSpeedUnit);
    DDX_Text(pDX, IDC_LABEL_SPEED, m_strSpeedLabel);
    DDX_Radio(pDX, IDC_RADIO_ENTITY_GROUND_CLAMP_NONE, m_GroundClamp);
    DDX_Text(pDX, IDC_LABEL_FLYMODE, m_strFlyMode);
    DDX_Radio(pDX, IDC_RADIO_RATES_PARENT, m_RatesCoordSystem);
    DDX_TextPrecision(pDX, IDC_EDIT_ENTITY_ALTITUDE, m_Altitude, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_ENTITY_BEARING, m_Bearing, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_ENTITY_LATITUDE, m_Latitude, 8);
    DDX_TextPrecision(pDX, IDC_EDIT_ENTITY_LONGITUDE, m_Longitude, 8);
    DDX_TextPrecision(pDX, IDC_EDIT_ENTITY_PITCH, m_Pitch, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_ENTITY_RANGE, m_Range, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_ENTITY_ROLL, m_Roll, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_ENTITY_SPEED, m_Speed, 5);
    DDX_TextPrecision(pDX, IDC_EDIT_ENTITY_YAW, m_Yaw, 5);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEntityStateView, CBaseStateView)
    //{{AFX_MSG_MAP(CEntityStateView)
    ON_BN_CLICKED(IDC_CHECK_ENTITY_ANIMATION_LOOP, OnCheckAnimationLoop)
    ON_BN_CLICKED(IDC_CHECK_ENTITY_ANIMATION_REVERSE, OnCheckAnimationReverse)
    ON_BN_CLICKED(IDC_CHECK_ENTITY_FLY_WAYPOINTS, OnCheckFlyWaypoints)
    ON_BN_CLICKED(IDC_CHECK_ENTITY_HIDE_GEOMETRY, OnCheckHideGeometry)
    ON_BN_CLICKED(IDC_CHECK_ENTITY_INHERIT_ALPHA, OnCheckInheritAlpha)
    ON_BN_CLICKED(IDC_CHECK_ENTITY_RATES, OnCheckSendRates)
    ON_BN_CLICKED(IDC_RADIO_ENTITY_GROUND_CLAMP_NONE, OnRadioClampNone)
    ON_BN_CLICKED(IDC_RADIO_ENTITY_GROUND_CLAMP_ALT_AND_ATT, OnRadioClampConformal)
    ON_BN_CLICKED(IDC_RADIO_ENTITY_GROUND_CLAMP_ALT, OnRadioClampNonconformal)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_PARENT, OnSelchangeParent)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_RELATIVE_TO, OnSelchangeRelativeTo)
    ON_CBN_DROPDOWN(IDC_COMBOBOXEX_ENTITY_RELATIVE_TO, OnDropdownRelativeTo)
    ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnSelchangingTab)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab)
    ON_BN_CLICKED(IDC_RADIO_RATES_LOCAL, OnRadioRatesLocal)
    ON_BN_CLICKED(IDC_RADIO_RATES_PARENT, OnRadioRatesParent)
    ON_MESSAGE(WM_USEREDIT, OnUserEditControl)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityStateView message handlers

void CEntityStateView::OnOK()
{
}

void CEntityStateView::OnCancel()
{
}

void CEntityStateView::RefreshView(void)
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Gray out the numeric fields during Run mode, so that the user
    // cannot edit the values.
    m_EditAlpha.EnableWindow(g_bFreezeFlag);
    m_EditAltitude.EnableWindow(g_bFreezeFlag);
    m_EditBearing.EnableWindow(g_bFreezeFlag);
    m_EditLatitude.EnableWindow(g_bFreezeFlag);
    m_EditLongitude.EnableWindow(g_bFreezeFlag);
    m_EditPitch.EnableWindow(g_bFreezeFlag);
    m_EditRange.EnableWindow(g_bFreezeFlag);
    m_EditRoll.EnableWindow(g_bFreezeFlag);
    m_EditSpeed.EnableWindow(g_bFreezeFlag);
    m_EditYaw.EnableWindow(g_bFreezeFlag);

    // If an edit box has the focus, store the selection.
    int start = 0, end = 0;
    CEdit *edit = DYNAMIC_DOWNCAST(CEdit, GetFocus());
    if (edit)
        edit->GetSel(start, end);

    ENTITY state = entity->GetAndLock();
    entity->Unlock();

    m_EntityID    = entity->GetID();
    m_EntityType  = entity->GetType();
    m_RefEntityID = entity->GetRefEntityID();
    m_Alpha       = state.cigi.alpha;
    m_ParentID    = state.cigi.parent_id;
    m_strFlyMode  = m_StrLookup[state.noncigi.flymode];

    if ((entity->GetClass() == ENTITY_CLASS_FIXEDWING) ||
        (entity->GetClass() == ENTITY_CLASS_ROTORCRAFT) ||
        (entity->GetClass() == ENTITY_CLASS_BOAT)) {
        m_Speed = MPSToKnots(state.noncigi.speed);
        m_strSpeedLabel = "Airspeed:";
        m_strSpeedUnit = "knots";
    } else {
        m_Speed = state.noncigi.speed;
        m_strSpeedLabel = "Speed:";
        m_strSpeedUnit = "m/s";
    }

    // Set entity state information based on whether or not the entity
    // is attached to a parent.
    const bool hasParent = (m_ParentID != -1);
    const DOF &dofs = state.cigi.dofs;

    m_Latitude          = hasParent ? dofs.offset_x  : dofs.latitude;
    m_Longitude         = hasParent ? dofs.offset_y  : dofs.longitude;
    m_Altitude          = hasParent ? dofs.offset_z  : dofs.altitude;
    m_Yaw               = hasParent ? dofs.rel_yaw   : dofs.yaw;
    m_Pitch             = hasParent ? dofs.rel_pitch : dofs.pitch;
    m_Roll              = hasParent ? dofs.rel_roll  : dofs.roll;
    m_strLatitudeLabel  = hasParent ? "X Offset:"    : "Latitude:";
    m_strLongitudeLabel = hasParent ? "Y Offset:"    : "Longitude:";
    m_strAltitudeLabel  = hasParent ? "Z Offset:"    : "Altitude:";
    m_strLatitudeUnit   = hasParent ? "m"            : "deg";
    m_strLongitudeUnit  = hasParent ? "m"            : "deg";

    // We don't want to update the following fields the frame
    // immediately following user action because the .25-sec
    // lag from the driver may cause flutter.
    if (!m_bHoldAnimLoop)
        m_bAnimLoop = state.cigi.anim_loop;

    if (!m_bHoldAnimRev)
        m_bAnimRev = state.cigi.anim_dir;

    if (!m_bHoldHideGeometry)
        m_bHideGeometry = (state.cigi.active == 1) ? FALSE : TRUE;

    if (!m_bHoldInheritAlpha)
        m_bInheritAlpha = state.cigi.inherit_alpha;

    if (!m_bHoldSendRates)
        m_bSendRates = state.noncigi.send_rates;

    if (!m_bHoldRatesCoordSystem)
        m_RatesCoordSystem = state.noncigi.rates_coordsys;

    if (!m_bHoldClampMode)
        m_GroundClamp = state.cigi.clamp_mode;

    if (!m_bHoldFlyWaypoints)
        m_bFlyWaypoints = state.unsaved.fly_waypoints;

    // Reset the holds.
    m_bHoldAnimLoop         = FALSE;
    m_bHoldAnimRev          = FALSE;
    m_bHoldHideGeometry     = FALSE;
    m_bHoldInheritAlpha     = FALSE;
    m_bHoldSendRates        = FALSE;
    m_bHoldRatesCoordSystem = FALSE;
    m_bHoldClampMode        = FALSE;
    m_bHoldFlyWaypoints     = FALSE;

    // Select the parent.
    if (!m_cboParent.GetDroppedState()) {
        int id = FindEntityComboItem(m_cboParent, m_ParentID);
        m_cboParent.SetCurSel(id);
    }

    // Select the reference entity.
    if (!m_cboRefEntity.GetDroppedState()) {
        int id = FindEntityComboItem(m_cboRefEntity, m_RefEntityID);
        m_cboRefEntity.SetCurSel(id);
    }

    // Now calculate the bearing and range.
    entity->CalcRefBearingAndRange(&m_Bearing, &m_Range);

    UpdateData(FALSE);

    // Enable/disable version-specific fields.
    if (state.noncigi.send_rates == 0) {
        m_RadioRatesLocal.EnableWindow(FALSE);
        m_RadioRatesParent.EnableWindow(FALSE);
    } else {
        m_RadioRatesLocal.EnableWindow(TRUE);
        m_RadioRatesParent.EnableWindow(TRUE);
    }

    // If an edit box has the focus, restore the selection.
    if (edit)
        edit->SetSel(start, end);

    // Now update the property pages that require it (mission function
    // responses).  If we have changed entities, just repopulate everything.
    if ((int)m_EntityID == m_PrevEntityID) {
        // Refresh the mission function tabs.
        m_DlgHAT.RefreshView();
        m_DlgLOS.RefreshView();
        m_DlgCollDet.RefreshView();
    } else {
        PopulateCompPropPage(entity);
        PopulateArtPartPropPage(entity);
        PopulateHATPropPage(entity);
        PopulateLOSPropPage(entity);
        PopulateCollDetPropPage(entity);
        PopulateMissilePropPage(entity);
        PopulateWeatherPropPage(entity);

        m_PrevEntityID = m_EntityID;
    }

    // Finally, if the Fly window is valid, update it.
    CFlyDlg &flydlg = theApp.GetMainFrame().GetFlyDlg();
    if (flydlg.GetSafeHwnd())
        flydlg.RefreshView();
}

void CEntityStateView::PopulateCompPropPage(CEntity *entity)
{
    CDebugTrace trace("CEntityStateView::PopulateCompPropPage(CEntity *)");

    m_DlgComponents.ClearComponents();

    int id = 0;
    CComponent *compptr = NULL;
    CComponentMap &compmap = entity->GetComponentMap();
    POSITION pos = compmap.GetStartPosition();
    while (pos) {
        compmap.GetNextAssoc(pos, id, compptr);
        m_DlgComponents.AddComponent(compptr);
    }
}

void CEntityStateView::PopulateArtPartPropPage(CEntity *entity)
{
    CDebugTrace trace("CEntityStateView::PopulateArtPartPropPage(CEntity *)");

    m_DlgArtParts.ClearArtParts();

    int id = 0;
    CArtPart *artpartptr = NULL;
    CArtPartMap &artpartmap = entity->GetArtPartMap();
    POSITION pos = artpartmap.GetStartPosition();
    while (pos) {
        artpartmap.GetNextAssoc(pos, id, artpartptr);
        m_DlgArtParts.AddArtPart(artpartptr);
    }
}

void CEntityStateView::PopulateHATPropPage(CEntity *entity)
{
    CDebugTrace trace("CEntityStateView::PopulateHATPropPage(CEntity *)");

    m_DlgHAT.ClearHats();

    int id = 0;
    CHat *hatptr = NULL;
    CHatMap &hatmap = entity->GetHatMap();
    POSITION pos = hatmap.GetStartPosition();
    while (pos) {
        hatmap.GetNextAssoc(pos, id, hatptr);
        m_DlgHAT.AddHatItem(hatptr);
    }
}

void CEntityStateView::PopulateLOSPropPage(CEntity *entity)
{
    CDebugTrace trace("CEntityStateView::PopulateLOSPropPage(CEntity *)");

    m_DlgLOS.ClearLos();

    int id = 0;
    CLos *losptr = NULL;
    CLosMap &losmap = entity->GetLosMap();
    POSITION pos = losmap.GetStartPosition();
    while (pos) {
        losmap.GetNextAssoc(pos, id, losptr);
        m_DlgLOS.AddLosItem(losptr);
    }
}

void CEntityStateView::PopulateCollDetPropPage(CEntity *entity)
{
    CDebugTrace trace("CEntityStateView::PopulateCollDetPropPage(CEntity *)");

    m_DlgCollDet.ClearSegsAndVols();

    int id = 0;
    POSITION pos = NULL;

    CCollisionSeg *segptr = NULL;
    CCollisionSegMap &segmap = entity->GetCollisionSegMap();
    pos = segmap.GetStartPosition();
    while (pos) {
        segmap.GetNextAssoc(pos, id, segptr);
        m_DlgCollDet.AddCollisionSegItem(segptr);
    }

    CCollisionVol *volptr = NULL;
    CCollisionVolMap &volmap = entity->GetCollisionVolMap();
    pos = volmap.GetStartPosition();
    while (pos) {
        volmap.GetNextAssoc(pos, id, volptr);
        m_DlgCollDet.AddCollisionVolItem(volptr);
    }
}

void CEntityStateView::PopulateMissilePropPage(CEntity *entity)
{
    CDebugTrace trace("CEntityStateView::PopulateMissilePropPage(CEntity *)");

    m_DlgMissile.RefreshView(entity);
}

void CEntityStateView::PopulateWeatherPropPage(CEntity *entity)
{
    CDebugTrace trace("CEntityStateView::PopulateWeatherPropPage(CEntity *)");

    m_DlgWeather.RefreshView(entity->GetWeatherAttributes());
}

void CEntityStateView::ClearParentList(void)
{
    CDebugTrace trace("CEntityStateView::ClearParentList()");

    m_cboParent.ResetContent();

    // Add a "None" item.
    COMBOBOXEXITEM cboitem = {  CBEIF_LPARAM | CBEIF_TEXT,
                                0, "None", 5, -1, -1, 0, 0, -1
                             };
    m_cboParent.InsertItem(&cboitem);
}

void CEntityStateView::AddToParentList(CEntity *entity)
{
    CDebugTrace trace("CEntityStateView::AddToParentList(CEntity *)");

    char text[256] = {0};
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_GENERIC, IMG_GENERIC, 0, 0, -1
                             };

    strcpy(text, entity->GetName());
    cboitem.pszText = text;
    cboitem.cchTextMax = strlen(text + 1);
    cboitem.iImage = entity->GetClass();
    cboitem.iSelectedImage = cboitem.iImage;
    cboitem.lParam = MAKE_TREE_DATA(entity->GetID(), entity->GetType(), entity->GetClass());

    // If the item exists, replace it.  Otherwise, insert it.
    cboitem.iItem = FindEntityComboItem(m_cboParent, entity->GetID());
    if (cboitem.iItem > -1) {
        m_cboParent.SetItem(&cboitem);
    } else {
        cboitem.iItem = FindEntityComboInsertPoint(m_cboParent, entity->GetID());
        m_cboParent.InsertItem(&cboitem);
    }
}

void CEntityStateView::RemoveFromParentList(const int id)
{
    CDebugTrace trace("CEntityStateView::RemoveFromParentList(int)");

    m_cboParent.DeleteItem(FindEntityComboItem(m_cboParent, id));
}

void CEntityStateView::ClearRefEntityList(void)
{
    CDebugTrace trace("CEntityStateView::ClearRefEntityList()");

    m_cboRefEntity.ResetContent();
}

void CEntityStateView::PopulateRefEntityList(void)
{
    CDebugTrace trace("CEntityStateView::PopulateRefEntityList()");

    m_cboRefEntity.ResetContent();

    CEntity *ref = NULL;
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    if (entity->GetParent() != -1)
        return;

    char text[256] = {0};
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_GENERIC, IMG_GENERIC, 0, 0, -1
                             };

    int cbo_idx = -1;
    int entity_id = -1;
    int ref_id = entity->GetRefEntityID();

    CEntityMap &map = g_DataManager.GetEntityMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, entity_id, ref);

        if ((ref->GetParent() == -1) && (ref->GetID() != entity->GetID())) {
            strcpy(text, ref->GetName());
            cboitem.pszText = text;
            cboitem.cchTextMax = strlen(text + 1);
            cboitem.iImage = ref->GetClass();
            cboitem.iSelectedImage = cboitem.iImage;
            cboitem.lParam = MAKE_TREE_DATA(ref->GetID(), ref->GetType(),
                                            ref->GetClass());

            // Add a new item to the list.
            cboitem.iItem = FindEntityComboInsertPoint(m_cboRefEntity, ref->GetID());
            cbo_idx = m_cboRefEntity.InsertItem(&cboitem);

            if (ref->GetID() == ref_id) {
                // Select the item in the combo box.
                m_cboRefEntity.SetCurSel(cbo_idx);
            }
        }
    }
}

int CEntityStateView::FindEntityComboItem(CComboBoxEx &combo, const int entity_id)
{
    CDebugTrace trace("CEntityStateView::FindEntityComboItem(CComboBoxEx &, int)");

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

int CEntityStateView::FindEntityComboInsertPoint(CComboBoxEx &combo, const int entity_id)
{
    CDebugTrace trace("CEntityStateView::FindEntityComboInsertPoint(CComboBoxEx &, int)");

    int i = 0;
    COMBOBOXEXITEM cboitem = { CBEIF_LPARAM, 0, NULL, 1, 0, 0, 0, 0, -1 };

    while (combo.GetItem(&cboitem)) {
        if ((short)MAKE_ID(cboitem.lParam) > entity_id)
            return i;

        cboitem.iItem = ++i;
    }

    return i;
}

BOOL CEntityStateView::OnInitDialog()
{
    CBaseStateView::OnInitDialog();

    // Set the image lists for the combo boxes.
    m_cboRefEntity.SetImageList(&g_ImgListTypes);
    m_cboParent.SetImageList(&g_ImgListTypes);

    // Insert a "None" item in the parent combo.
    ClearParentList();

    // Configure the tab control.
    InitTabs();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CEntityStateView::InitTabs(void)
{
    // Add a tab for each of the child dialog boxes.
    m_Tab.InsertItem(0, "Components");
    m_Tab.InsertItem(1, "Articulated Parts");
    m_Tab.InsertItem(2, "HAT/HOT");
    m_Tab.InsertItem(3, "LOS");
    m_Tab.InsertItem(4, "Collision Detection");
    m_Tab.InsertItem(5, "Missile Properties");
    m_Tab.InsertItem(6, "Weather Attributes");

    // Lock the resources for the child dialog boxes.
    m_DlgTempl[0] = LockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE_ENTITY_COMPONENTS));
    m_DlgTempl[1] = LockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE_ENTITY_ARTPARTS));
    m_DlgTempl[2] = LockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE_ENTITY_HATHOT));
    m_DlgTempl[3] = LockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE_ENTITY_LOS));
    m_DlgTempl[4] = LockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE_ENTITY_COLLDET));
    m_DlgTempl[5] = LockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE_ENTITY_MISSILE));
    m_DlgTempl[6] = LockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE_ENTITY_WEATHER));

    // Create an array of pointers to the child dialog boxes.
    m_TabDlg[0] = (CDialog *)&m_DlgComponents;
    m_TabDlg[1] = (CDialog *)&m_DlgArtParts;
    m_TabDlg[2] = (CDialog *)&m_DlgHAT;
    m_TabDlg[3] = (CDialog *)&m_DlgLOS;
    m_TabDlg[4] = (CDialog *)&m_DlgCollDet;
    m_TabDlg[5] = (CDialog *)&m_DlgMissile;
    m_TabDlg[6] = (CDialog *)&m_DlgWeather;

    // Create child dialog boxes.
    int tabcount = m_Tab.GetItemCount();
    ASSERT(tabcount == NUM_ENTITYTAB_PAGES);

    for (int i = 0; i < tabcount; i++) {
        m_TabDlg[i]->CreateIndirect(m_DlgTempl[i], &m_Tab);
        m_TabDlg[i]->SetWindowPos(NULL, 5, 25, 0, 0, SWP_NOSIZE);
    }

    // Simulate selection of the first item.
    SelectTab(0);
}

LRESULT CEntityStateView::OnUserEditControl(WPARAM wParam, LPARAM lParam)
{
    CDebugTrace trace("CEntityStateView::OnUserEditControl(WPARAM, LPARAM)");

    CEditKbd *ctrl = (CEditKbd *)lParam;

    if (ctrl == &m_EditSpeed)
        OnUserChangeSpeed();
    else if (ctrl == &m_EditAlpha)
        OnUserChangeAlpha();
    else if (ctrl == &m_EditAltitude)
        OnUserChangeAltitude();
    else if (ctrl == &m_EditBearing)
        OnUserChangeBearing();
    else if (ctrl == &m_EditLatitude)
        OnUserChangeLatitude();
    else if (ctrl == &m_EditLongitude)
        OnUserChangeLongitude();
    else if (ctrl == &m_EditPitch)
        OnUserChangePitch();
    else if (ctrl == &m_EditRange)
        OnUserChangeRange();
    else if (ctrl == &m_EditRoll)
        OnUserChangeRoll();
    else if (ctrl == &m_EditYaw)
        OnUserChangeYaw();

    return 0;
}

void CEntityStateView::OnUserChangeSpeed()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    UpdateData(TRUE);

    if ((entity->GetClass() == ENTITY_CLASS_FIXEDWING) ||
        (entity->GetClass() == ENTITY_CLASS_ROTORCRAFT) ||
        (entity->GetClass() == ENTITY_CLASS_BOAT)) {
        entity->SetSpeed(KnotsToMPS(m_Speed), TRUE);
    } else {
        entity->SetSpeed(m_Speed, TRUE);
    }
}

void CEntityStateView::OnUserChangeAlpha()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    UpdateData(TRUE);

    entity->SetAlpha(m_Alpha, TRUE);

    theApp.GetMainFrame().GetSituationCtrl().SetEntityVisible(entity->GetSituationHandle(),
                                                              (m_Alpha > 0) ? TRUE : FALSE);
}

void CEntityStateView::OnUserChangeAltitude()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    UpdateData(TRUE);

    DOF dof = entity->GetAndLockDofs();
    if (m_ParentID == -1)
        dof.altitude = m_Altitude;
    else
        dof.offset_z = m_Altitude;

    entity->SetLockedDofs(dof, TRUE);
    entity->Unlock();
}

void CEntityStateView::OnUserChangeBearing()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    UpdateData(TRUE);

    CEntity *refentity = g_DataManager.GetEntity(entity->GetRefEntityID());
    if (refentity) {
        // Set the position relative to the reference entity.
        entity->SetRelativePosition(m_Bearing, m_Range, TRUE);

        // Update the situational view.  Because the driver hasn't had
        // time to update the entity's position, we will have to
        // calculate the new lat and lon here.
        ENTITY state = entity->GetAndLock();
        entity->Unlock();
        BOOL visible = ((state.cigi.parent_id == -1) && (state.cigi.active)
                        && (state.cigi.alpha > 0)) ? TRUE : FALSE;

        double x, y;
        DOF dof = state.cigi.dofs;
        DOF refdof = refentity->GetDofs();

        waypoint(&g_ERM, refdof.latitude, refdof.longitude,
                 m_Range, m_Bearing + refdof.yaw,
                 &(dof.latitude), &(dof.longitude));

        GDCtoTM(dof.latitude, dof.longitude, &y, &x);

        (((CMainFrame *)AfxGetMainWnd())->GetSituationCtrl()).
        SetEntity(entity->GetSituationHandle(), x, y, dof.yaw,
                  visible);
    }
}

void CEntityStateView::OnUserChangePitch()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    UpdateData(TRUE);

    DOF dof = entity->GetAndLockDofs();
    if (m_ParentID == -1)
        dof.pitch = m_Pitch;
    else
        dof.rel_pitch = m_Pitch;

    entity->SetLockedDofs(dof, TRUE);
    entity->Unlock();
}

void CEntityStateView::OnUserChangeRoll()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    UpdateData(TRUE);

    DOF dof = entity->GetAndLockDofs();
    if (m_ParentID == -1)
        dof.roll = m_Roll;
    else
        dof.rel_roll = m_Roll;

    entity->SetLockedDofs(dof, TRUE);
    entity->Unlock();
}

void CEntityStateView::OnUserChangeLatitude()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    UpdateData(TRUE);

    ENTITY state = entity->GetAndLock();
    if (m_ParentID == -1)
        state.cigi.dofs.latitude = m_Latitude;
    else
        state.cigi.dofs.offset_x = m_Latitude;

    entity->SetLockedEntityPosition(state, TRUE);
    entity->Unlock();

    // Update the situational view.
    BOOL visible = ((state.cigi.parent_id == -1) && (state.cigi.active)
                    && (state.cigi.alpha > 0)) ? TRUE : FALSE;
    double x, y;
    GDCtoTM(state.cigi.dofs.latitude, state.cigi.dofs.longitude, &y, &x);
    (((CMainFrame *)AfxGetMainWnd())->GetSituationCtrl()).
    SetEntity(entity->GetSituationHandle(), x, y, state.cigi.dofs.yaw,
              visible);
}

void CEntityStateView::OnUserChangeLongitude()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    UpdateData(TRUE);

    ENTITY state = entity->GetAndLock();
    if (m_ParentID == -1)
        state.cigi.dofs.longitude = m_Longitude;
    else
        state.cigi.dofs.offset_y = m_Longitude;

    entity->SetLockedEntityPosition(state, TRUE);
    entity->Unlock();

    // Update the situational view.
    BOOL visible = ((state.cigi.parent_id == -1) && (state.cigi.active)
                    && (state.cigi.alpha > 0)) ? TRUE : FALSE;
    double x, y;
    GDCtoTM(state.cigi.dofs.latitude, state.cigi.dofs.longitude, &y, &x);
    (((CMainFrame *)AfxGetMainWnd())->GetSituationCtrl()).
    SetEntity(entity->GetSituationHandle(), x, y, state.cigi.dofs.yaw,
              visible);
}

void CEntityStateView::OnUserChangeRange()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    UpdateData(TRUE);

    CEntity *refentity = g_DataManager.GetEntity(entity->GetRefEntityID());
    if (refentity) {
        // Set the position relative to the reference entity.
        entity->SetRelativePosition(m_Bearing, m_Range, TRUE);

        // Update the situational view.  Because the driver hasn't had
        // time to update the entity's position, we will have to
        // calculate the new lat and lon here.
        ENTITY state = entity->GetAndLock();
        entity->Unlock();
        BOOL visible = ((state.cigi.parent_id == -1) && (state.cigi.active)
                        && (state.cigi.alpha > 0)) ? TRUE : FALSE;

        double x, y;
        DOF dof = state.cigi.dofs;
        DOF refdof = refentity->GetDofs();

        waypoint(&g_ERM, refdof.latitude, refdof.longitude,
                 m_Range, m_Bearing + refdof.yaw,
                 &(dof.latitude), &(dof.longitude));

        GDCtoTM(dof.latitude, dof.longitude, &y, &x);

        (((CMainFrame *)AfxGetMainWnd())->GetSituationCtrl()).
        SetEntity(entity->GetSituationHandle(), x, y, dof.yaw,
                  visible);
    }
}

void CEntityStateView::OnUserChangeYaw()
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    UpdateData(TRUE);

    ENTITY state = entity->GetAndLock();
    if (m_ParentID == -1)
        state.cigi.dofs.yaw = m_Yaw;
    else
        state.cigi.dofs.rel_yaw = m_Yaw;

    entity->SetLockedEntityPosition(state, TRUE);
    entity->Unlock();

    // Update the situational view.
    BOOL visible = ((state.cigi.parent_id == -1) && (state.cigi.active)
                    && (state.cigi.alpha > 0)) ? TRUE : FALSE;
    double x, y;
    GDCtoTM(state.cigi.dofs.latitude, state.cigi.dofs.longitude, &y, &x);
    (((CMainFrame *)AfxGetMainWnd())->GetSituationCtrl()).
    SetEntity(entity->GetSituationHandle(), x, y, state.cigi.dofs.yaw,
              visible);
}

void CEntityStateView::OnCheckAnimationLoop()
{
    CDebugTrace trace("CEntityStateView::OnCheckAnimationLoop()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // .25-sec latency from the driver.
    m_bHoldAnimLoop = TRUE;

    m_bAnimLoop = m_CheckAnimLoop.GetCheck();

    entity->SetAnimLoop(m_bAnimLoop, TRUE);
}

void CEntityStateView::OnCheckAnimationReverse()
{
    CDebugTrace trace("CEntityStateView::OnCheckAnimationReverse()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // .25-sec latency from the driver.
    m_bHoldAnimRev = TRUE;

    m_bAnimRev = m_CheckAnimRev.GetCheck();

    entity->SetAnimDir(m_bAnimRev, TRUE);
}

void CEntityStateView::OnCheckFlyWaypoints()
{
    CDebugTrace trace("CEntityStateView::OnCheckFlyWaypoints()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // .25-sec latency from the driver.
    m_bHoldFlyWaypoints = TRUE;

    m_bFlyWaypoints = m_CheckFlyWaypoints.GetCheck();

    entity->SetFlyWaypoints(m_bFlyWaypoints ? TRUE : FALSE);
}

void CEntityStateView::OnCheckHideGeometry()
{
    CDebugTrace trace("CEntityStateView::OnCheckHideGeometry()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // .25-sec latency from the driver.
    m_bHoldHideGeometry = TRUE;

    m_bHideGeometry = m_CheckHideGeometry.GetCheck();

    entity->SetActiveState(m_bHideGeometry ? 0 : 1);
}

void CEntityStateView::OnCheckInheritAlpha()
{
    CDebugTrace trace("CEntityStateView::OnCheckInheritAlpha()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // .25-sec latency from the driver.
    m_bHoldInheritAlpha = TRUE;

    m_bInheritAlpha = m_CheckInheritAlpha.GetCheck();

    entity->SetInheritAlpha(m_bInheritAlpha ? TRUE : FALSE, TRUE);
}

void CEntityStateView::OnCheckSendRates()
{
    CDebugTrace trace("CEntityStateView::OnCheckSendRates()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // .25-sec latency from the driver.
    m_bHoldSendRates = TRUE;

    m_bSendRates = m_CheckSendRates.GetCheck();

    entity->SetSendRates(m_bSendRates ? TRUE : FALSE);

    m_RadioRatesLocal.EnableWindow(m_bSendRates ? TRUE : FALSE);
    m_RadioRatesParent.EnableWindow(m_bSendRates ? TRUE : FALSE);
}

void CEntityStateView::OnRadioClampNone()
{
    CDebugTrace trace("CEntityStateView::OnRadioClampNone()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // .25-sec latency from the driver.
    m_bHoldClampMode = TRUE;

    m_GroundClamp = 0;

    entity->SetClampMode(m_GroundClamp, TRUE);
}

void CEntityStateView::OnRadioClampNonconformal()
{
    CDebugTrace trace("CEntityStateView::OnRadioClampNonconformal()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // .25-sec latency from the driver.
    m_bHoldClampMode = TRUE;

    m_GroundClamp = 1;

    entity->SetClampMode(m_GroundClamp, TRUE);
}

void CEntityStateView::OnRadioClampConformal()
{
    CDebugTrace trace("CEntityStateView::OnRadioClampConformal()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // .25-sec latency from the driver.
    m_bHoldClampMode = TRUE;

    m_GroundClamp = 2;

    entity->SetClampMode(m_GroundClamp, TRUE);
}

void CEntityStateView::OnSelchangeParent()
{
    CDebugTrace trace("CEntityStateView::OnSelchangeParent()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    DWORD lParam = m_cboParent.GetItemData(m_cboParent.GetCurSel());
    m_ParentID = (short)MAKE_ID(lParam);

    entity->SetParent(m_ParentID, TRUE);    // force a wait

    // Update the tree view.
    theApp.GetMainFrame().GetEntityTreeView().MoveEntityToParent(entity, m_ParentID);

    // Show or hide the entity in the situtational display control.
    theApp.GetMainFrame().GetSituationCtrl().SetEntityVisible(
        entity->GetSituationHandle(),
        (m_ParentID == -1 ? TRUE : FALSE));

    // Repopulate the reference entity list.
    PopulateRefEntityList();
}

void CEntityStateView::OnSelchangeRelativeTo()
{
    CDebugTrace trace("CEntityStateView::OnSelchangeRelativeTo()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    int id = m_cboRefEntity.GetCurSel();
    if (id == -1) {
        entity->SetRefEntityID(-1);
        m_RefEntityID = -1;
    } else {
        int ref_id = MAKE_ID(m_cboRefEntity.GetItemData(id));
        entity->SetRefEntityID(ref_id);
        m_RefEntityID = MAKE_ID(ref_id);
    }

    RefreshView();
}

void CEntityStateView::OnDropdownRelativeTo()
{
    CDebugTrace trace("CEntityStateView::OnDropdownRelativeTo()");

    PopulateRefEntityList();
}

void CEntityStateView::OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    CDebugTrace trace("CEntityStateView::OnSelchangingTab(NMHDR *, LRESULT *)");

    // Hide the current window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_HIDE);
    }

    *pResult = 0;
}

void CEntityStateView::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    CDebugTrace trace("CEntityStateView::OnSelchangeTab(NMHDR *, LRESULT *)");

    // Show the appropriate window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_SHOW);
    }

    *pResult = 0;
}

void CEntityStateView::SelectTab(const int index)
{
    CDebugTrace trace("CEntityStateView::SelectTab(int)");

    // Simulate selection of the item.
    LRESULT result;
    OnSelchangingTab(NULL, &result);

    m_Tab.SetCurSel(index);

    OnSelchangeTab(NULL, &result);
}

void CEntityStateView::RefreshFlyMode(int flymode)
{
    m_strFlyMode = m_StrLookup[flymode];
    UpdateData(FALSE);
}

void CEntityStateView::OnRadioRatesLocal()
{
    CDebugTrace trace("CEntityStateView::OnRadioRatesLocal()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // .25-sec latency from the driver.
    m_bHoldRatesCoordSystem = TRUE;

    m_RatesCoordSystem = 1;

    entity->SetRatesCoordSystem(m_RatesCoordSystem);
}

void CEntityStateView::OnRadioRatesParent()
{
    CDebugTrace trace("CEntityStateView::OnRadioRatesLocal()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    // Set the hold flag so we don't get "flutter" from the
    // .25-sec latency from the driver.
    m_bHoldRatesCoordSystem = TRUE;

    m_RatesCoordSystem = 0;

    entity->SetRatesCoordSystem(m_RatesCoordSystem);
}
