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

// ViewGroup.cpp: implementation of the CViewGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "globals.h"
#include "hemumsg.h"
#include "ViewGroup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CViewGroup, CObject, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CViewGroup::CViewGroup()
{
    CDebugTrace trace("CViewGroup::CViewGroup()");

    m_SharedHandle = m_SharedObject.Create();

    // If this is a temporary object, we don't want to send a message
    // to the driver in the destructor.
    m_bNotifyDriver = FALSE;

    memset(&m_InitialState, 0, sizeof(m_InitialState));
}

CViewGroup::CViewGroup(const int id)
{
    CDebugTrace trace("CViewGroup::CViewGroup(int)");

    m_bNotifyDriver = TRUE;

    m_SharedHandle = m_SharedObject.Create();

    // Lock the object. We should never have to sleep because the driver
    // process does not yet know about this object.
    VIEWGROUP defaults = {0};
    while (!m_SharedObject.GetAndLock(&defaults))
        Sleep(1);

    defaults.cigi.group_id = id;
    m_SharedObject.SetLocked(defaults);
    Unlock();

    // Initialize the CMap.
    m_Components.RemoveAll();
    m_Components.InitHashTable(521);

    m_InitialState = defaults;

    // Set the name since we aren't pulling it from the config template.
    m_Name.Format("Group %d", id);

    // Send a message to the driver to create the view on that side.
    MESSAGE_ADD_VIEWGROUP msg_add;
    strcpy(msg_add.sharedname, GetSharedName());
    PostDriverMsg(msg_add);
}

CViewGroup::CViewGroup(const TEMPL_VIEWGROUP *templ)
{
    CDebugTrace trace("CViewGroup::CViewGroup(TEMPL_VIEWGROUP *)");

    m_bNotifyDriver = TRUE;

    m_SharedHandle = m_SharedObject.Create();

    // Initialize the CMap.
    m_Components.RemoveAll();
    m_Components.InitHashTable(521);

    // Now we can apply the template.  This will also populate the
    // components list.
    ApplyTemplate(templ);

    // Set the initial state after applying the template.
    // Lock the object. We should never have to sleep because the driver
    // process does not yet know about this object.
    VIEWGROUP defaults = {0};
    while (!m_SharedObject.GetAndLock(&defaults))
        Sleep(1);
    Unlock();

    m_InitialState = defaults;

    // Send a message to the driver to create the view on that side.
    MESSAGE_ADD_VIEWGROUP msg;
    strcpy(msg.sharedname, GetSharedName());
    PostDriverMsg(msg);
}

CViewGroup::~CViewGroup()
{
    CDebugTrace trace("CViewGroup::~CViewGroup()");

    ClearComponents();

    // Nofity the driver to delete the object on its side.
    if (m_bNotifyDriver) {
        MESSAGE_DEL_VIEWGROUP msg;
        msg.id = (unsigned short)GetGroupID();
        PostDriverMsg(msg);
    }

    // m_SharedObject owns and closes the shared-memory handle.
}

void CViewGroup::SynchronizeToDriver(void)
{
    CDebugTrace trace("CViewGroup::SynchronizeToDriver()");

    MESSAGE_ADD_VIEWGROUP msg;
    strcpy(msg.sharedname, GetSharedName());
    PostDriverMsg(msg);

    // Future deletes or scenario reloads must remove this runtime driver
    // object. Serialization-created view groups are only registered after
    // CViewGroup::Serialize() has restored their shared-memory state.
    m_bNotifyDriver = TRUE;
}

void CViewGroup::Serialize(CArchive &ar)
{
    CDebugTrace trace("CViewGroup::Serialize(CArchive &)");

    unsigned long junk;
    int length = 0;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)m_Name);
        ar << length;
        ar.Write((LPCTSTR)m_Name, length);  // NULL terminator not copied

        // Store the shared data.  First, lock the object and get a copy
        // of the data.
        VIEWGROUP group = GetAndLock();
        Unlock();

        // Then, populate a VIEWGROUP_SAVE_STATE struct to maintain file
        // compatibility.
        VIEWGROUP_SAVE_STATE saved = {0};
        saved.group_id = group.cigi.group_id;
        saved.offset_x_enable = group.cigi.offset_x_enable;
        saved.offset_y_enable = group.cigi.offset_y_enable;
        saved.offset_z_enable = group.cigi.offset_z_enable;
        saved.yaw_enable = group.cigi.yaw_enable;
        saved.pitch_enable = group.cigi.pitch_enable;
        saved.roll_enable = group.cigi.roll_enable;
        saved.entity_id = group.cigi.entity_id;
        saved.offset_x = group.cigi.offset_x;
        saved.offset_y = group.cigi.offset_y;
        saved.offset_z = group.cigi.offset_z;
        saved.yaw = group.cigi.yaw;
        saved.pitch = group.cigi.pitch;
        saved.roll = group.cigi.roll;
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
        TRACE1("	Group ID = %d\n", group.cigi.group_id);
    } else {
        // Reinitialize the flags. Post-load scenario synchronization will
        // register the fully deserialized view group with the driver.
        m_bNotifyDriver = FALSE;

        // Read the name.
        ar >> length;
        ar.Read(m_Name.GetBuffer(length), length);
        m_Name.ReleaseBuffer(length);

        // Read into a VIEW_SAVE_STATE struct to maintain file
        // compatibility.
        VIEWGROUP_SAVE_STATE saved = {0};
        ar.Read(&saved, sizeof(saved));

        // Lock the object and update the shared data.
        VIEWGROUP group = GetAndLock();

        // Populate the real structure.
        group.cigi.group_id = saved.group_id;
        group.cigi.offset_x_enable = saved.offset_x_enable;
        group.cigi.offset_y_enable = saved.offset_y_enable;
        group.cigi.offset_z_enable = saved.offset_z_enable;
        group.cigi.yaw_enable = saved.yaw_enable;
        group.cigi.pitch_enable = saved.pitch_enable;
        group.cigi.roll_enable = saved.roll_enable;
        group.cigi.entity_id = saved.entity_id;
        group.cigi.offset_x = saved.offset_x;
        group.cigi.offset_y = saved.offset_y;
        group.cigi.offset_z = saved.offset_z;
        group.cigi.yaw = saved.yaw;
        group.cigi.pitch = saved.pitch;
        group.cigi.roll = saved.roll;

        // Write it back and unlock the object.
        SetLocked(group, true);
        Unlock();

        // Read the components.
        m_Components.Serialize(ar);

        // Read the padding.
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("	length = %d\n", length);
        TRACE1("	Name = \"%s\"\n", (LPCTSTR)m_Name);
        TRACE1("	Group ID = %d\n", group.cigi.group_id);
    }
}


CComponent *CViewGroup::GetComponent(const int id)
{
    CDebugTrace trace("CViewGroup::GetComponent(int)");

    CComponent *search = NULL;

    m_Components.Lookup(id, search);

    return search;
}

BOOL CViewGroup::AddComponent(CComponent *comp)
{
    CDebugTrace trace("CViewGroup::AddComponent(CComponent *)");

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

BOOL CViewGroup::DeleteComponent(const int id)
{
    CDebugTrace trace("CViewGroup::DeleteComponent(int)");

    CComponent *search = NULL;

    if (m_Components.Lookup(id, search)) {
        if (search)
            delete search;

        m_Components.RemoveKey(id);

        return TRUE;
    }

    return FALSE;
}

void CViewGroup::ClearComponents(void)
{
    CDebugTrace trace("CViewGroup::CClearComponents()");

    int id = 0;
    CComponent *comp = NULL;
    POSITION pos = m_Components.GetStartPosition();
    while (pos) {
        m_Components.GetNextAssoc(pos, id, comp);
        delete comp;
    }

    m_Components.RemoveAll();
}

BOOL CViewGroup::ApplyTemplate(const TEMPL_VIEWGROUP *templ)
{
    CDebugTrace trace("CViewGroup::ApplyTemplate(TEMPL_VIEWGROUP *)");

    // Set the name.
    if (templ->Name == "")
        m_Name.Format("Group %d", templ->GroupID);
    else
        m_Name.Format("Group %d: %s", templ->GroupID, templ->Name);

    // Set the initial state from the template and the set the shared data.
    VIEWGROUP group = GetAndLock();
    memset(&group, 0, sizeof(VIEWGROUP));
    group.cigi.group_id = templ->GroupID;
    group.cigi.offset_x = templ->XOffset;
    group.cigi.offset_y = templ->YOffset;
    group.cigi.offset_z = templ->ZOffset;
    group.cigi.yaw = templ->Yaw;
    group.cigi.pitch = templ->Pitch;
    group.cigi.roll = templ->Roll;
    SetLocked(group);
    Unlock();

    // Build the component list.
    unsigned long handle = 0;
    TEMPL_COMPONENT *comp_templ = templ->ComponentList.GetHead(&handle);
    while (comp_templ) {
        CComponent *comp = new CComponent(COMP_CLASS_VIEWGROUP, comp_templ);

        // Set the instance ID of the component to the entity id.
        comp->SetInstanceID(GetGroupID());

        AddComponent(comp);

        comp_templ = templ->ComponentList.GetNext(&handle);
    }

    return TRUE;
}

BOOL CViewGroup::GetDataFromTemplate(VIEWGROUP *group, const TEMPL_VIEWGROUP *templ)
{
    CDebugTrace trace("CViewGroup::GetDataFromTemplate(VIEWGROUP *, TEMPL_VIEWGROUP *)");

    ASSERT(m_SharedObject.IsLocked());

    if (templ->GroupID != GetGroupID())
        return FALSE;

    // Set the name.
    m_Name.Format("Group %d: %s", GetGroupID(), templ->Name);

    group->cigi.group_id = templ->GroupID;
    group->cigi.offset_x = templ->XOffset;
    group->cigi.offset_y = templ->YOffset;
    group->cigi.offset_z = templ->ZOffset;
    group->cigi.yaw = templ->Yaw;
    group->cigi.pitch = templ->Pitch;
    group->cigi.roll = templ->Roll;

    return TRUE;
}

int CViewGroup::GetGroupID(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.group_id;
}

int CViewGroup::GetEntityID(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.entity_id;
}

float CViewGroup::GetXOffset(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.offset_x;
}

float CViewGroup::GetYOffset(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.offset_y;
}

float CViewGroup::GetZOffset(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.offset_z;
}

float CViewGroup::GetYaw(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.yaw;
}

float CViewGroup::GetPitch(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.pitch;
}

float CViewGroup::GetRoll(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.roll;
}

float CViewGroup::GetXRate(void)
{
    VIEWGROUP_NONCIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockNonCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.dx;
}

float CViewGroup::GetYRate(void)
{
    VIEWGROUP_NONCIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockNonCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.dy;
}

float CViewGroup::GetZRate(void)
{
    VIEWGROUP_NONCIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockNonCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.dz;
}

float CViewGroup::GetYawRate(void)
{
    VIEWGROUP_NONCIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockNonCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.dyaw;
}

float CViewGroup::GetPitchRate(void)
{
    VIEWGROUP_NONCIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockNonCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.dpitch;
}

float CViewGroup::GetRollRate(void)
{
    VIEWGROUP_NONCIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockNonCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.droll;
}

VIEWGROUP CViewGroup::GetAndLock(void)
{
    VIEWGROUP group = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLock(&group)) {
        Sleep(1);
    }

    return group;
}

VIEWGROUP_CIGI_DATA CViewGroup::GetAndLockCigi(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    return data;
}

VIEWGROUP_NONCIGI_DATA CViewGroup::GetAndLockNonCigi(void)
{
    VIEWGROUP_NONCIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockNonCigi(&data)) {
        Sleep(1);
    }

    return data;
}

void CViewGroup::SetEntityID(const int entity, BOOL set_initial)
{
    CDebugTrace trace("CViewGroup::SetEntityID(int, BOOL)");

    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    data.entity_id = entity;
    m_SharedObject.SetLockedCigi(data);

    Unlock();

    if (set_initial)
        m_InitialState.cigi.entity_id = entity;
}

void CViewGroup::SetXOffset(float x, BOOL set_initial)
{
    CDebugTrace trace("CViewGroup::SetXOffset(float, BOOL)");

    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    data.offset_x = x;
    data.offset_x_enable = 1;
    m_SharedObject.SetLockedCigi(data);

    Unlock();

    if (set_initial)
        m_InitialState.cigi.offset_x = x;
}

void CViewGroup::SetYOffset(float y, BOOL set_initial)
{
    CDebugTrace trace("CViewGroup::SetYOffset(float, BOOL)");

    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    data.offset_y = y;
    data.offset_y_enable = 1;
    m_SharedObject.SetLockedCigi(data);

    Unlock();

    if (set_initial)
        m_InitialState.cigi.offset_y = y;
}

void CViewGroup::SetZOffset(float z, BOOL set_initial)
{
    CDebugTrace trace("CViewGroup::SetZOffset(float, BOOL)");

    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    data.offset_z = z;
    data.offset_z_enable = 1;
    m_SharedObject.SetLockedCigi(data);

    Unlock();

    if (set_initial)
        m_InitialState.cigi.offset_z = z;
}

void CViewGroup::SetYaw(float yaw, BOOL set_initial)
{
    CDebugTrace trace("CViewGroup::SetYaw(float, BOOL)");

    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    data.yaw = yaw;
    data.yaw_enable = 1;
    m_SharedObject.SetLockedCigi(data);

    Unlock();

    if (set_initial)
        m_InitialState.cigi.yaw = yaw;
}

void CViewGroup::SetPitch(float pitch, BOOL set_initial)
{
    CDebugTrace trace("CViewGroup::SetPitch(float, BOOL)");

    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    data.pitch = pitch;
    data.pitch_enable = 1;
    m_SharedObject.SetLockedCigi(data);

    Unlock();

    if (set_initial)
        m_InitialState.cigi.pitch = pitch;
}

void CViewGroup::SetRoll(float roll, BOOL set_initial)
{
    CDebugTrace trace("CViewGroup::SetRoll(float, BOOL)");

    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    data.roll = roll;
    data.roll_enable = 1;
    m_SharedObject.SetLockedCigi(data);

    Unlock();

    if (set_initial)
        m_InitialState.cigi.roll = roll;
}

void CViewGroup::SetXRate(float dx)
{
    CDebugTrace trace("CViewGroup::SetXRate(float)");

    VIEWGROUP_NONCIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockNonCigi(&data)) {
        Sleep(1);
    }

    data.dx = dx;
    m_SharedObject.SetLockedNonCigi(data);

    Unlock();
}

void CViewGroup::SetYRate(float dy)
{
    CDebugTrace trace("CViewGroup::SetYRate(float)");

    VIEWGROUP_NONCIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockNonCigi(&data)) {
        Sleep(1);
    }

    data.dy = dy;
    m_SharedObject.SetLockedNonCigi(data);

    Unlock();
}

void CViewGroup::SetZRate(float dz)
{
    CDebugTrace trace("CViewGroup::SetZRate(float)");

    VIEWGROUP_NONCIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockNonCigi(&data)) {
        Sleep(1);
    }

    data.dz = dz;
    m_SharedObject.SetLockedNonCigi(data);

    Unlock();
}

void CViewGroup::SetYawRate(float dyaw)
{
    CDebugTrace trace("CViewGroup::SetYawRate(float)");

    VIEWGROUP_NONCIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockNonCigi(&data)) {
        Sleep(1);
    }

    data.dyaw = dyaw;
    m_SharedObject.SetLockedNonCigi(data);

    Unlock();
}

void CViewGroup::SetPitchRate(float dpitch)
{
    CDebugTrace trace("CViewGroup::SetPitchRate(float)");

    VIEWGROUP_NONCIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockNonCigi(&data)) {
        Sleep(1);
    }

    data.dpitch = dpitch;
    m_SharedObject.SetLockedNonCigi(data);

    Unlock();
}

void CViewGroup::SetRollRate(float droll)
{
    CDebugTrace trace("CViewGroup::SetRollRate(float)");

    VIEWGROUP_NONCIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockNonCigi(&data)) {
        Sleep(1);
    }

    data.droll = droll;
    m_SharedObject.SetLockedNonCigi(data);

    Unlock();
}

void CViewGroup::SetLocked(const VIEWGROUP &group, BOOL set_initial)
{
    CDebugTrace trace("CViewGroup::SetLocked(VIEWGROUP, BOOL)");

    m_SharedObject.SetLocked(group);

    if (set_initial)
        m_InitialState = group;
}

void CViewGroup::SetLockedCigi(const VIEWGROUP_CIGI_DATA &data, BOOL set_initial)
{
    CDebugTrace trace("CViewGroup::SetLockedCigi(VIEWGROUP_CIGI_DATA &, BOOL)");

    m_SharedObject.SetLockedCigi(data);

    if (set_initial)
        m_InitialState.cigi = data;
}

void CViewGroup::SetLockedNonCigi(const VIEWGROUP_NONCIGI_DATA &data, BOOL set_initial)
{
    CDebugTrace trace("CViewGroup::SetLockedNonCigi(VIEWGROUP_NONCIGI_DATA &, BOOL)");

    m_SharedObject.SetLockedNonCigi(data);

    if (set_initial)
        m_InitialState.noncigi = data;
}


BOOL CViewGroup::XOffsetHasChanged(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.offset_x_enable;
}

BOOL CViewGroup::YOffsetHasChanged(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.offset_y_enable;
}

BOOL CViewGroup::ZOffsetHasChanged(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.offset_z_enable;
}

BOOL CViewGroup::YawHasChanged(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.yaw_enable;
}

BOOL CViewGroup::PitchHasChanged(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.pitch_enable;
}

BOOL CViewGroup::RollHasChanged(void)
{
    VIEWGROUP_CIGI_DATA data = {0};

    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLockCigi(&data)) {
        Sleep(1);
    }

    // Immediately unlock.
    Unlock();

    return data.roll_enable;
}
