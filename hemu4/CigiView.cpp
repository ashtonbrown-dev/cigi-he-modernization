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
 *  FILENAME:   CigiView.cpp
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
#include "hemu4.h"
#include "hemumsg.h"
#include "CigiView.h"
#include "RTXCompatibility.h"
#include "globals.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CCigiView, CObject, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCigiView::CCigiView()
{
    CDebugTrace trace("CCigiView::CCigiView()");

    m_SharedHandle = m_SharedObject.Create();

    // If this is a temporary object, we don't want to send a message
    // to the driver in the destructor.
    m_bNotifyDriver = FALSE;

    memset(&m_InitialState, 0, sizeof(m_InitialState));
}

CCigiView::CCigiView(const int id)
{
    CDebugTrace trace("CCigiView::CCigiView(int)");

    m_bNotifyDriver = TRUE;

    m_SharedHandle = m_SharedObject.Create();

    // Initialize the CMap.
    m_Components.RemoveAll();
    m_Components.InitHashTable(521);

    // Lock the object. We should never have to sleep because the driver
    // process does not yet know about this object.
    VIEW defaults = {0};
    while (!m_SharedObject.GetAndLock(&defaults))
        Sleep(1);

    defaults.viewdef.view_id = id;
    m_SharedObject.SetLocked(defaults);
    Unlock();

    m_InitialState = defaults;

    // Set the name since we aren't pulling it from the config template.
    m_Name.Format("View %d", id);

    // Send a message to the driver to create the view on that side.
    MESSAGE_ADD_VIEW msg_add;
    strcpy(msg_add.sharedname, GetSharedName());
    PostDriverMsg(msg_add);
}

CCigiView::CCigiView(const TEMPL_VIEW *templ)
{
    CDebugTrace trace("CCigiView::CCigiView(TEMPL_VIEW *)");

    m_bNotifyDriver = TRUE;

    m_SharedHandle = m_SharedObject.Create();

    // Initialize the CMap.
    m_Components.RemoveAll();
    m_Components.InitHashTable(521);

    // Apply the template.  This will also populate the
    // components list.
    ApplyTemplate(templ);

    // Set the initial state after applying the template.
    // Lock the object. We should never have to sleep because the driver
    // process does not yet know about this object.
    VIEW defaults = {0};
    while (!m_SharedObject.GetAndLock(&defaults))
        Sleep(1);
    Unlock();

    m_InitialState = defaults;

    // Send a message to the driver to create the view on that side.
    MESSAGE_ADD_VIEW msg;
    strcpy(msg.sharedname, GetSharedName());
    PostDriverMsg(msg);
}

CCigiView::~CCigiView()
{
    CDebugTrace trace("CCigiView::~CCigiView()");

    ClearComponents();

    // Nofity the driver to delete the object on its side.
    if (m_bNotifyDriver) {
        MESSAGE_DEL_VIEW msg;
        msg.id = (unsigned short)GetViewID();
        PostDriverMsg(msg);
    }

    // Close the handle on this side.
    RtCloseHandle(m_SharedHandle);
}

void CCigiView::Serialize(CArchive &ar)
{
    CDebugTrace trace("CCigiView::Serialize(CArchive &)");

    int length = 0;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)m_Name);
        ar << length;
        ar.Write((LPCTSTR)m_Name, length);  // NULL terminator not copied

        // Store the shared data.  First, lock the object and get a copy
        // of the data.
        VIEW view = GetAndLock();
        Unlock();

        // Then, populate a VIEW_SAVE_STATE struct to maintain file
        // compatibility.
        VIEW_SAVE_STATE saved = {0};
        saved.view_id = view.viewdef.view_id;
        saved.group_id = view.viewdef.group_id;
        saved.view_type = view.viewdef.view_type;
        saved.projection = view.viewdef.projection;
        saved.replication = view.viewdef.replication;
        saved.reorder = view.viewdef.reorder;
        saved.entity_id = view.viewctrl.entity_id;
        saved.mirror_mode = view.viewdef.mirror_mode;
        saved.fov_top_enable = view.viewdef.fov_top_enable;
        saved.fov_bottom_enable = view.viewdef.fov_bottom_enable;
        saved.fov_left_enable = view.viewdef.fov_left_enable;
        saved.fov_right_enable = view.viewdef.fov_right_enable;
        saved.fov_near_enable = view.viewdef.fov_near_enable;
        saved.fov_far_enable = view.viewdef.fov_far_enable;
        saved.offset_x_enable = view.viewctrl.offset_x_enable;
        saved.offset_y_enable = view.viewctrl.offset_y_enable;
        saved.offset_z_enable = view.viewctrl.offset_z_enable;
        saved.yaw_enable = view.viewctrl.yaw_enable;
        saved.pitch_enable = view.viewctrl.pitch_enable;
        saved.roll_enable = view.viewctrl.roll_enable;
        saved.def_has_changed = view.noncigi.def_has_changed;
        saved.camera_has_changed = view.noncigi.ctrl_has_changed;
        saved.offset_x = view.viewctrl.offset_x;
        saved.offset_y = view.viewctrl.offset_y;
        saved.offset_z = view.viewctrl.offset_z;
        saved.yaw = view.viewctrl.yaw;
        saved.pitch = view.viewctrl.pitch;
        saved.roll = view.viewctrl.roll;
        saved.dx = view.noncigi.dx;
        saved.dy = view.noncigi.dy;
        saved.dz = view.noncigi.dz;
        saved.dyaw = view.noncigi.dyaw;
        saved.dpitch = view.noncigi.dpitch;
        saved.droll = view.noncigi.droll;
        saved.fov_top = view.viewdef.fov_top;
        saved.fov_bottom = view.viewdef.fov_bottom;
        saved.fov_left = view.viewdef.fov_left;
        saved.fov_right = view.viewdef.fov_right;
        saved.fov_near = view.viewdef.fov_near;
        saved.fov_far = view.viewdef.fov_far;
        ar.Write(&saved, sizeof(saved));

        // Store the components.
        m_Components.Serialize(ar);

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("	length = %d\n", length);
        TRACE1("	Name = \"%s\"\n", (LPCTSTR)m_Name);
        TRACE1("	view_id = %d\n", view.viewdef.view_id);
        TRACE1("	group_id = %d\n", view.viewdef.group_id);
    } else {
        // Reinitialize the flags.
        m_bNotifyDriver = true;

        // Read the name.
        ar >> length;
        ar.Read(m_Name.GetBuffer(length), length);
        m_Name.ReleaseBuffer(length);

        // Read into a VIEW_SAVE_STATE struct to maintain file
        // compatibility.
        VIEW_SAVE_STATE saved = {0};
        ar.Read(&saved, sizeof(saved));

        // Lock the object and update the shared data.
        VIEW view = GetAndLock();

        // Populate the real structure.
        view.viewdef.view_id = saved.view_id;
        view.viewdef.group_id = saved.group_id;
        view.viewdef.view_type = saved.view_type;
        view.viewdef.projection = saved.projection;
        view.viewdef.replication = saved.replication;
        view.viewdef.reorder = saved.reorder;
        view.viewctrl.entity_id = saved.entity_id;
        view.viewdef.mirror_mode = saved.mirror_mode;
        view.viewdef.fov_top_enable = saved.fov_top_enable;
        view.viewdef.fov_bottom_enable = saved.fov_bottom_enable;
        view.viewdef.fov_left_enable = saved.fov_left_enable;
        view.viewdef.fov_right_enable = saved.fov_right_enable;
        view.viewdef.fov_near_enable = saved.fov_near_enable;
        view.viewdef.fov_far_enable = saved.fov_far_enable;
        view.viewctrl.offset_x_enable = saved.offset_x_enable;
        view.viewctrl.offset_y_enable = saved.offset_y_enable;
        view.viewctrl.offset_z_enable = saved.offset_z_enable;
        view.viewctrl.yaw_enable = saved.yaw_enable;
        view.viewctrl.pitch_enable = saved.pitch_enable;
        view.viewctrl.roll_enable = saved.roll_enable;
        view.noncigi.def_has_changed = saved.def_has_changed;
        view.noncigi.ctrl_has_changed = saved.camera_has_changed;
        view.viewctrl.offset_x = saved.offset_x;
        view.viewctrl.offset_y = saved.offset_y;
        view.viewctrl.offset_z = saved.offset_z;
        view.viewctrl.yaw = saved.yaw;
        view.viewctrl.pitch = saved.pitch;
        view.viewctrl.roll = saved.roll;
        view.noncigi.dx = saved.dx;
        view.noncigi.dy = saved.dy;
        view.noncigi.dz = saved.dz;
        view.noncigi.dyaw = saved.dyaw;
        view.noncigi.dpitch = saved.dpitch;
        view.noncigi.droll = saved.droll;
        view.viewdef.fov_top = saved.fov_top;
        view.viewdef.fov_bottom = saved.fov_bottom;
        view.viewdef.fov_left = saved.fov_left;
        view.viewdef.fov_right = saved.fov_right;
        view.viewdef.fov_near = saved.fov_near;
        view.viewdef.fov_far = saved.fov_far;

        // Write it back and unlock the object.
        SetLocked(view, true);
        Unlock();

        // Read the components.
        m_Components.Serialize(ar);

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("	length = %d\n", length);
        TRACE1("	Name = \"%s\"\n", (LPCTSTR)m_Name);
        TRACE1("	view_id = %d\n", view.viewdef.view_id);
        TRACE1("	group_id = %d\n", view.viewdef.group_id);
    }
}

CComponent *CCigiView::GetComponent(const int id)
{
    CDebugTrace trace("CCigiView::GetComponent(int)");

    CComponent *search = NULL;
    m_Components.Lookup(id, search);

    return search;
}

BOOL CCigiView::AddComponent(CComponent *comp)
{
    CDebugTrace trace("CCigiView::AddComponent(CComponent *)");

    BOOL retval = FALSE;
    int id = comp->GetID();
    CComponent *search = NULL;

    // If the component already exists, delete it.
    if (m_Components.Lookup(id, search)) {
        //LWD: we need to preserve the states and revert to default if they don't match

        delete search;
        retval = TRUE;      // return TRUE if component exists
    }

    m_Components.SetAt(id, comp);

    return retval;
}

BOOL CCigiView::DeleteComponent(const int id)
{
    CDebugTrace trace("CCigiView::DeleteComponent(int)");

    CComponent *search = NULL;

    if (m_Components.Lookup(id, search)) {
        if (search)
            delete search;

        m_Components.RemoveKey(id);

        return TRUE;
    }

    return FALSE;
}

void CCigiView::ClearComponents(void)
{
    CDebugTrace trace("CCigiView::ClearComponents()");

    int id = 0;
    CComponent *comp = NULL;
    POSITION pos = m_Components.GetStartPosition();
    while (pos) {
        m_Components.GetNextAssoc(pos, id, comp);
        delete comp;
    }

    m_Components.RemoveAll();
}

BOOL CCigiView::ApplyTemplate(const TEMPL_VIEW *templ)
{
    CDebugTrace trace("CCigiView::ApplyTemplate(TEMPL_VIEW *)");

    // Set the name.
    m_Name.Format("View %d: %s", templ->ViewID, templ->Name);

    // Set the initial state from the template and the set the shared data.
    VIEW view = GetAndLock();
    memset(&view, 0, sizeof(VIEW));
    view.viewdef.view_id = templ->ViewID;
    view.viewdef.group_id = templ->GroupID;
    view.viewdef.view_type = templ->ViewType;
    view.viewdef.projection = templ->ProjectionType;
    view.viewdef.replication = templ->ReplicationMode;
    view.viewdef.mirror_mode = templ->MirrorMode;
    view.viewdef.fov_top = templ->Top;
    view.viewdef.fov_bottom = templ->Bottom;
    view.viewdef.fov_left = templ->Left;
    view.viewdef.fov_right = templ->Right;
    view.viewdef.fov_near = templ->Near;
    view.viewdef.fov_far = templ->Far;
    view.viewctrl.offset_x = templ->XOffset;
    view.viewctrl.offset_y = templ->YOffset;
    view.viewctrl.offset_z = templ->ZOffset;
    view.viewctrl.yaw = templ->Yaw;
    view.viewctrl.pitch = templ->Pitch;
    view.viewctrl.roll = templ->Roll;
    SetLocked(view);
    Unlock();

    // Build the component list.
    unsigned long handle = 0;
    TEMPL_COMPONENT *comp_templ = templ->ComponentList.GetHead(&handle);
    while (comp_templ) {
        CComponent *comp = new CComponent(COMP_CLASS_VIEW, comp_templ);

        // Set the instance ID of the component to the entity id.
        comp->SetInstanceID(GetViewID());

        AddComponent(comp);

        comp_templ = templ->ComponentList.GetNext(&handle);
    }

    return TRUE;
}

BOOL CCigiView::GetDataFromTemplate(VIEW *view, const TEMPL_VIEW *templ)
{
    CDebugTrace trace("CCigiView::GetDataFromTemplate(VIEW *, TEMPL_VIEW *)");

    ASSERT(m_SharedObject.IsLocked());

    if (templ->ViewID != GetViewID())
        return FALSE;

    // Set the name.
    if (templ->Name == "")
        m_Name.Format("View %d", GetViewID());
    else
        m_Name.Format("View %d: %s", GetViewID(), templ->Name);

    view->viewdef.view_id = templ->ViewID;
    view->viewdef.group_id = templ->GroupID;
    view->viewdef.view_type = templ->ViewType;
    view->viewdef.projection = templ->ProjectionType;
    view->viewdef.replication = templ->ReplicationMode;
    view->viewdef.mirror_mode = templ->MirrorMode;
    view->viewdef.fov_top = templ->Top;
    view->viewdef.fov_bottom = templ->Bottom;
    view->viewdef.fov_left = templ->Left;
    view->viewdef.fov_right = templ->Right;
    view->viewdef.fov_near = templ->Near;
    view->viewdef.fov_far = templ->Far;
    view->viewctrl.offset_x = templ->XOffset;
    view->viewctrl.offset_y = templ->YOffset;
    view->viewctrl.offset_z = templ->ZOffset;
    view->viewctrl.yaw = templ->Yaw;
    view->viewctrl.pitch = templ->Pitch;
    view->viewctrl.roll = templ->Roll;

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Gets
//
///////////////////////////////////////////////////////////////////////////////

VIEW CCigiView::GetAndLock(void)
{
    VIEW view = {0};
    GetView(&view, TRUE);
    return view;
}

VIEW_DEF_DATA CCigiView::GetAndLockViewDef(void)
{
    VIEW view = {0};
    GetView(&view, TRUE);
    return view.viewdef;
}

VIEW_CONTROL_DATA CCigiView::GetAndLockViewCtrl(void)
{
    VIEW view = {0};
    GetView(&view, TRUE);
    return view.viewctrl;
}

VIEW_NONCIGI_DATA CCigiView::GetAndLockNonCigi(void)
{
    VIEW view = {0};
    GetView(&view, TRUE);
    return view.noncigi;
}

int CCigiView::GetViewID(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.view_id;
}

int CCigiView::GetGroupID(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.group_id;
}

int CCigiView::GetEntityID(void)
{
    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data);
    return data.entity_id;
}

int CCigiView::GetViewType(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.view_type;
}

int CCigiView::GetProjectionType(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.projection;
}

int CCigiView::GetReplicationMode(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.replication;
}

BOOL CCigiView::GetReorderFlag(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.reorder;
}

int CCigiView::GetMirrorMode(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.mirror_mode;
}

float CCigiView::GetFovTop(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.fov_top;
}

float CCigiView::GetFovBottom(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.fov_bottom;
}

float CCigiView::GetFovLeft(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.fov_left;
}

float CCigiView::GetFovRight(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.fov_right;
}

float CCigiView::GetFovNear(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.fov_near;
}

float CCigiView::GetFovFar(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.fov_far;
}

float CCigiView::GetXOffset(void)
{
    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data);
    return data.offset_x;
}

float CCigiView::GetYOffset(void)
{
    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data);
    return data.offset_y;
}

float CCigiView::GetZOffset(void)
{
    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data);
    return data.offset_z;
}

float CCigiView::GetYaw(void)
{
    VIEW view = {0};
    GetView(&view);
    return view.viewctrl.yaw;
}

float CCigiView::GetPitch(void)
{
    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data);
    return data.pitch;
}

float CCigiView::GetRoll(void)
{
    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data);
    return data.roll;
}

float CCigiView::GetXRate(void)
{
    VIEW_NONCIGI_DATA data = {0};
    GetViewNonCigiData(&data);
    return data.dx;
}

float CCigiView::GetYRate(void)
{
    VIEW_NONCIGI_DATA data = {0};
    GetViewNonCigiData(&data);
    return data.dy;
}

float CCigiView::GetZRate(void)
{
    VIEW_NONCIGI_DATA data = {0};
    GetViewNonCigiData(&data);
    return data.dz;
}

float CCigiView::GetYawRate(void)
{
    VIEW_NONCIGI_DATA data = {0};
    GetViewNonCigiData(&data);
    return data.dyaw;
}

float CCigiView::GetPitchRate(void)
{
    VIEW_NONCIGI_DATA data = {0};
    GetViewNonCigiData(&data);
    return data.dpitch;
}

float CCigiView::GetRollRate(void)
{
    VIEW_NONCIGI_DATA data = {0};
    GetViewNonCigiData(&data);
    return data.droll;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Sets
//
///////////////////////////////////////////////////////////////////////////////

void CCigiView::SetGroupID(const int group, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetGroupID(int, BOOL)");

    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data, TRUE);

    data.group_id = group;
    m_SharedObject.SetLockedViewDef(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewdef.group_id = group;
}

void CCigiView::SetEntityID(const int entity, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetEntityID(int, BOOL)");

    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data, TRUE);

    data.entity_id = entity;
    m_SharedObject.SetLockedViewCtrl(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewctrl.entity_id = entity;
}

void CCigiView::SetViewType(const int type, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetViewType(int, BOOL)");

    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data, TRUE);

    data.view_type = type;
    m_SharedObject.SetLockedViewDef(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewdef.view_type = type;
}

void CCigiView::SetProjectionType(const int type, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetProjectionType(int, BOOL)");

    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data, TRUE);

    data.projection = type;
    m_SharedObject.SetLockedViewDef(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewdef.projection = type;
}

void CCigiView::SetReplicationMode(const int mode, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetReplicationMode(int, BOOL)");

    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data, TRUE);

    data.replication = mode;
    m_SharedObject.SetLockedViewDef(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewdef.replication = mode;
}

void CCigiView::SetReorderFlag(const BOOL reorder, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetReorderFlag(BOOL, BOOL)");

    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data, TRUE);

    data.reorder = reorder;
    m_SharedObject.SetLockedViewDef(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewdef.reorder = reorder;
}

void CCigiView::SetMirrorMode(const int mode, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetMirrorMode(int, BOOL)");

    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data, TRUE);

    data.mirror_mode = mode;
    m_SharedObject.SetLockedViewDef(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewdef.mirror_mode = mode;
}

void CCigiView::SetFovTop(const float top, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetFovTop(float, BOOL)");

    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data, TRUE);

    data.fov_top = top;
    data.fov_top_enable = 1;
    m_SharedObject.SetLockedViewDef(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewdef = data;
}

void CCigiView::SetFovBottom(const float bottom, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetFovBottom(float, BOOL)");

    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data, TRUE);

    data.fov_bottom = bottom;
    data.fov_bottom_enable = 1;
    m_SharedObject.SetLockedViewDef(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewdef = data;
}

void CCigiView::SetFovLeft(const float left, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetFovLeft(float, BOOL)");

    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data, TRUE);

    data.fov_left = left;
    data.fov_left_enable = 1;
    m_SharedObject.SetLockedViewDef(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewdef = data;
}

void CCigiView::SetFovRight(const float right, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetFovRight(float, BOOL)");

    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data, TRUE);

    data.fov_right = right;
    data.fov_right_enable = 1;
    m_SharedObject.SetLockedViewDef(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewdef = data;
}

void CCigiView::SetFovNear(const float near_dist, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetFovNear(float, BOOL)");

    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data, TRUE);

    data.fov_near = near_dist;
    data.fov_near_enable = 1;
    m_SharedObject.SetLockedViewDef(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewdef = data;
}

void CCigiView::SetFovFar(const float far_dist, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetFovFar(float, BOOL)");

    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data, TRUE);

    data.fov_far = far_dist;
    data.fov_far_enable = 1;
    m_SharedObject.SetLockedViewDef(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewdef = data;
}

void CCigiView::SetXOffset(const float x, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetXOffset(float, BOOL)");

    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data, TRUE);

    data.offset_x = x;
    data.offset_x_enable = 1;
    m_SharedObject.SetLockedViewCtrl(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewctrl = data;
}

void CCigiView::SetYOffset(const float y, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetYOffset(float, BOOL)");

    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data, TRUE);

    data.offset_y = y;
    data.offset_y_enable = 1;
    m_SharedObject.SetLockedViewCtrl(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewctrl = data;
}

void CCigiView::SetZOffset(const float z, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetZOffset(float, BOOL)");

    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data, TRUE);

    data.offset_z = z;
    data.offset_z_enable = 1;
    m_SharedObject.SetLockedViewCtrl(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewctrl = data;
}

void CCigiView::SetYaw(const float yaw, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetYaw(float, BOOL)");

    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data, TRUE);

    data.yaw = yaw;
    data.yaw_enable = 1;
    m_SharedObject.SetLockedViewCtrl(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewctrl = data;
}

void CCigiView::SetPitch(const float pitch, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetPitch(float, BOOL)");

    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data, TRUE);

    data.pitch = pitch;
    data.pitch_enable = 1;
    m_SharedObject.SetLockedViewCtrl(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewctrl = data;
}

void CCigiView::SetRoll(const float roll, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetRoll(float, BOOL)");

    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data, TRUE);

    data.roll = roll;
    data.roll_enable = 1;
    m_SharedObject.SetLockedViewCtrl(data);

    Unlock();

    if (set_initial)
        m_InitialState.viewctrl = data;
}

void CCigiView::SetXRate(const float dx)
{
    CDebugTrace trace("CCigiView::SetXRate(float)");

    VIEW_NONCIGI_DATA data = {0};
    GetViewNonCigiData(&data, TRUE);

    data.dx = dx;
    m_SharedObject.SetLockedNonCigi(data);

    Unlock();
}

void CCigiView::SetYRate(const float dy)
{
    CDebugTrace trace("CCigiView::SetYRate(float)");

    VIEW_NONCIGI_DATA data = {0};
    GetViewNonCigiData(&data, TRUE);

    data.dy = dy;
    m_SharedObject.SetLockedNonCigi(data);

    Unlock();
}

void CCigiView::SetZRate(const float dz)
{
    CDebugTrace trace("CCigiView::SetZRate(float)");

    VIEW_NONCIGI_DATA data = {0};
    GetViewNonCigiData(&data, TRUE);

    data.dz = dz;
    m_SharedObject.SetLockedNonCigi(data);

    Unlock();
}

void CCigiView::SetYawRate(const float dyaw)
{
    CDebugTrace trace("CCigiView::SetYawRate(float)");

    VIEW_NONCIGI_DATA data = {0};
    GetViewNonCigiData(&data, TRUE);

    data.dyaw = dyaw;
    m_SharedObject.SetLockedNonCigi(data);

    Unlock();
}

void CCigiView::SetPitchRate(const float dpitch)
{
    CDebugTrace trace("CCigiView::SetPitchRate(float)");

    VIEW_NONCIGI_DATA data = {0};
    GetViewNonCigiData(&data, TRUE);

    data.dpitch = dpitch;
    m_SharedObject.SetLockedNonCigi(data);

    Unlock();
}

void CCigiView::SetRollRate(const float droll)
{
    CDebugTrace trace("CCigiView::SetRollRate(float)");

    VIEW_NONCIGI_DATA data = {0};
    GetViewNonCigiData(&data, TRUE);

    data.droll = droll;
    m_SharedObject.SetLockedNonCigi(data);

    Unlock();
}

void CCigiView::SetLocked(const VIEW &view, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetLocked(VIEW, BOOL)");

    m_SharedObject.SetLocked(view);

    if (set_initial)
        m_InitialState = view;
}

void CCigiView::SetLockedViewDef(const VIEW_DEF_DATA &viewdef, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetLockedViewDef(VIEW_DEF_DATA, BOOL)");

    m_SharedObject.SetLockedViewDef(viewdef);

    if (set_initial)
        m_InitialState.viewdef = viewdef;
}

void CCigiView::SetLockedViewCtrl(const VIEW_CONTROL_DATA &viewctrl, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetLockedViewCtrl(VIEW_CONTROL_DATA, BOOL)");

    m_SharedObject.SetLockedViewCtrl(viewctrl);

    if (set_initial)
        m_InitialState.viewctrl = viewctrl;
}

void CCigiView::SetLockedNonCigi(const VIEW_NONCIGI_DATA &noncigi, const BOOL set_initial)
{
    CDebugTrace trace("CCigiView::SetLockedNonCigi(VIEW, BOOL)");

    m_SharedObject.SetLockedNonCigi(noncigi);

    if (set_initial)
        m_InitialState.noncigi = noncigi;
}

BOOL CCigiView::FOVTopHasChanged(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.fov_top_enable;
}

BOOL CCigiView::FOVBottomHasChanged(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.fov_bottom_enable;
}

BOOL CCigiView::FOVLeftHasChanged(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.fov_left_enable;
}

BOOL CCigiView::FOVRightHasChanged(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.fov_right_enable;
}

BOOL CCigiView::FOVNearHasChanged(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.fov_near_enable;
}

BOOL CCigiView::FOVFarHasChanged(void)
{
    VIEW_DEF_DATA data = {0};
    GetViewDefData(&data);
    return data.fov_far_enable;
}

BOOL CCigiView::XOffsetHasChanged(void)
{
    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data);
    return data.offset_x_enable;
}

BOOL CCigiView::YOffsetHasChanged(void)
{
    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data);
    return data.offset_y_enable;
}

BOOL CCigiView::ZOffsetHasChanged(void)
{
    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data);
    return data.offset_z_enable;
}

BOOL CCigiView::YawHasChanged(void)
{
    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data);
    return data.yaw_enable;
}

BOOL CCigiView::PitchHasChanged(void)
{
    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data);
    return data.pitch_enable;
}

BOOL CCigiView::RollHasChanged(void)
{
    VIEW_CONTROL_DATA data = {0};
    GetViewControlData(&data);
    return data.roll_enable;
}

void CCigiView::GetView(VIEW *view, BOOL keepLock)
{
    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLock(view))
        Sleep(1);

    // Immediately unlock if we don't need it.
    if (!keepLock)
        Unlock();
}

void CCigiView::GetViewControlData(VIEW_CONTROL_DATA *viewControlData, BOOL keepLock)
{
    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockViewCtrl(viewControlData))
        Sleep(1);

    // Immediately unlock if we don't need it.
    if (!keepLock)
        Unlock();
}

void CCigiView::GetViewDefData(VIEW_DEF_DATA *viewDefData, BOOL keepLock)
{
    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockViewDef(viewDefData))
        Sleep(1);

    // Immediately unlock if we don't need it.
    if (!keepLock)
        Unlock();
}

void CCigiView::GetViewNonCigiData(VIEW_NONCIGI_DATA *viewNonCigiData, BOOL keepLock)
{
    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockNonCigi(viewNonCigiData))
        Sleep(1);

    // Immediately unlock if we don't need it.
    if (!keepLock)
        Unlock();
}

template <> void AFXAPI SerializeElements<CCigiView *>(CArchive &ar, CCigiView **pElements, int nCount)
{
    CDebugTrace trace("SerializeElements<CCigiView *>(CArchive &, CCigiView **, int)");

    if (ar.IsStoring()) {
        for (int i = 0; i < nCount; i++) {
            if (pElements[i])
                pElements[i]->Serialize(ar);
        }
    } else {
        for (int i = 0; i < nCount; i++) {
            pElements[i] = new CCigiView(-1);
            pElements[i]->Serialize(ar);
        }
    }
}
