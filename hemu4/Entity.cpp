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
 *  FILENAME:   Entity.cpp
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
#include "cigi4types.h"
#include "DebugTrace.h"
#include "Entity.h"
#include "globals.h"
#include "hemumsg.h"
#include "Weather.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#ifndef STATE_MASK_CLASS
#define STATE_MASK_CLASS        0x80000000
#endif

CIDManager CEntity::m_HatIDManager;         // shared
CIDManager CEntity::m_LosIDManager;         // shared

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CEntity, CObject, 0)

CEntity::CEntity()
{
    CDebugTrace trace("CEntity::CEntity()");

    memcpy(&m_InitialCigiData,    0, sizeof(m_InitialCigiData));
    memcpy(&m_InitialNonCigiData, 0, sizeof(m_InitialNonCigiData));

    SetAlpha(255);
    SetInitialAlpha(255);

    CommonEntityInit();
}

CEntity::CEntity(const int id, const int type, const int entity_class, const DOF &dof)
{
    CDebugTrace trace("CEntity::CEntity(int, int, int)");

    CommonEntityInit(id, type, entity_class, TRUE, TRUE, &dof);
}

CEntity::CEntity(const int id, const TEMPL_ENTITY *templ, const DOF &dof)
{
    CDebugTrace trace("CEntity::CEntity(int, TEMPL_ENTITY *)");

    CommonEntityInit(id, -1, -1, TRUE, TRUE, &dof, templ);
}

CEntity::~CEntity()
{
    CDebugTrace trace("CEntity::~CEntity()");

    ClearAttachPoints();
    ClearArtParts();
    ClearComponents();
    ClearHat();
    ClearLos();
    ClearCollisionSegs();
    ClearCollisionVols();

    // Nofity the driver to delete the object on its side.
    if (m_bNotifyDriver) {
        MESSAGE_DEL_ENTITY msg;
        msg.id = (unsigned short)GetID();
        PostDriverMsg(msg);
    }

    // Close the handle on this side.
    RtCloseHandle(m_SharedHandle);
}

void CEntity::CommonEntityInit(int entityId,
                               int entityType,
                               int entityClass,
                               BOOL notifyDriver,
                               BOOL applyTemplate,
                               const DOF *dof,
                               const TEMPL_ENTITY *templ)
{
    m_EntityID        = entityId;
    m_EntityType      = entityType;
    m_RefEntityID     = -1;
    m_HtreeEntity     = NULL;
    m_HtreeEnv        = NULL;
    m_SituationHandle = NULL;
    m_SharedHandle    = m_SharedObject.Create();

    m_EntityClass = entityClass;
    if (entityClass == ENTITY_CLASS_CLOUD) {
        m_WeatherAttributes = new CWeather;
        m_WeatherAttributes->SetOwnerID(entityId);
        m_WeatherAttributes->SetScope(SCOPE_ENTITY);
    } else {
        m_WeatherAttributes = NULL;
    }

    // Initialize the CMaps.
    m_AttachPoints.RemoveAll();
    m_AttachPoints.InitHashTable(521);
    m_ArtParts.RemoveAll();
    m_ArtParts.InitHashTable(521);
    m_Components.RemoveAll();
    m_Components.InitHashTable(521);
    m_Hat.RemoveAll();
    m_Hat.InitHashTable(521);
    m_Los.RemoveAll();
    m_Los.InitHashTable(521);
    m_CollSegs.RemoveAll();
    m_CollSegs.InitHashTable(521);
    m_CollVols.RemoveAll();
    m_CollVols.InitHashTable(521);

    m_Waypoints.SetGrowBy(16);

    if (applyTemplate) {
        const TEMPL_ENTITY *t = templ;

        if (!t)
            t = ::GetEntityTempl(entityType);
        if (t)
            ApplyTemplate(t);

        // Set the rest of the default values not provided by the template.
        // Lock the object. We should never have to sleep because the driver
        // process does not yet know about this object.
        ENTITY defaults = {0};
        while (!m_SharedObject.GetAndLock(&defaults))
            Sleep(1);

        defaults.cigi.id = entityId;
        defaults.cigi.active = 1;
        defaults.cigi.parent_id = -1;
        defaults.cigi.dofs = *dof;
        // ApplyTemplate will set defaults.cigi.type = type

        defaults.noncigi.target_id = -1;
        defaults.noncigi.collective = 50;
        m_SharedObject.SetLocked(defaults);
        Unlock();

        m_InitialCigiData = defaults.cigi;
        m_InitialNonCigiData = defaults.noncigi;
    } else {
        m_InitialNonCigiData.collective = 50;
    }

    m_bNotifyDriver = notifyDriver;
    if (notifyDriver) {
        // Send a message to the driver to create the entity on that side.
        MESSAGE_ADD_ENTITY msg;
        strcpy(msg.sharedname, GetSharedName());
        PostDriverMsg(msg);
    }
}

void CEntity::Serialize(CArchive &ar)
{
    CDebugTrace trace("CEntity::Serialize(CArchive &)");

    unsigned long junk;
    int length = 0;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)m_Name);
        ar << length;
        ar.Write((LPCTSTR)m_Name, length);  // NULL terminator not copied

        // Store the non-shared data.
        ar << m_RefEntityID;

        // Store the initial states. To maintain file compatibility with
        // Version 3.1.2 and earlier, populate a ENTITY_SAVE_STATE structure.
        ENTITY_SAVE_STATE SaveStruct = {0};
        SaveStruct.core.id = m_InitialCigiData.id;
        SaveStruct.core.type = m_InitialCigiData.type;
        SaveStruct.core.parent_id = m_InitialCigiData.parent_id;
        SaveStruct.core.target_id = m_InitialNonCigiData.target_id;
        SaveStruct.core.entity_class = m_InitialNonCigiData.entity_class;
        SaveStruct.core.active = m_InitialCigiData.active;
        SaveStruct.core.alpha = m_InitialCigiData.alpha;
        SaveStruct.core.anim_state = m_InitialCigiData.anim_state;
        SaveStruct.core.anim_dir = m_InitialCigiData.anim_dir;
        SaveStruct.core.anim_loop = m_InitialCigiData.anim_loop;
        SaveStruct.core.inherit_alpha = m_InitialCigiData.inherit_alpha;
        SaveStruct.core.clamp_mode = m_InitialCigiData.clamp_mode;
        SaveStruct.core.coll_detect = m_InitialCigiData.coll_detect;
        SaveStruct.core.flymode = m_InitialNonCigiData.flymode;
        SaveStruct.core.send_rates = m_InitialNonCigiData.send_rates;
        SaveStruct.core.use_attach_point = m_InitialNonCigiData.use_attach_point;
        SaveStruct.core.attach_point = m_InitialNonCigiData.attach_point;
        SaveStruct.core.speed = m_InitialNonCigiData.speed;
        SaveStruct.core.dofs = m_InitialCigiData.dofs;
        SaveStruct.core.rates = m_InitialNonCigiData.rates;
        ar.Write(&SaveStruct.core, sizeof(SaveStruct.core));

        m_AttachPoints.Serialize(ar);
        m_ArtParts.Serialize(ar);
        m_Components.Serialize(ar);
        m_Hat.Serialize(ar);
        m_Los.Serialize(ar);
        m_CollSegs.Serialize(ar);
        m_CollVols.Serialize(ar);
        m_CollSegIDManager.Serialize(ar);
        m_CollVolIDManager.Serialize(ar);
        m_Waypoints.Serialize(ar);

        // Store the launch, persistent launch, and detonation animations.
        m_LaunchAnimIDs.Serialize(ar);
        m_PersistentLaunchAnimIDs.Serialize(ar);
        m_DetonationAnimIDs.Serialize(ar);

        // m_HatIDManager and m_LosIDManager are stored by the data manager.

        if (m_WeatherAttributes) {
            ar << 1;
            m_WeatherAttributes->Serialize(ar);
        } else
            ar << 0;

        // Store the shared data.
        ENTITY entitydata = GetAndLock();
        Unlock();           // unlock before possibly long file I/O

        // Populate an ENTITY_SAVE_DATA structure.
        SaveStruct.core.id = entitydata.cigi.id;
        SaveStruct.core.type = entitydata.cigi.type;
        SaveStruct.core.parent_id = entitydata.cigi.parent_id;
        SaveStruct.core.target_id = entitydata.noncigi.target_id;
        SaveStruct.core.entity_class = entitydata.noncigi.entity_class;
        SaveStruct.core.active = entitydata.cigi.active;
        SaveStruct.core.alpha = entitydata.cigi.alpha;
        SaveStruct.core.anim_state = entitydata.cigi.anim_state;
        SaveStruct.core.anim_dir = entitydata.cigi.anim_dir;
        SaveStruct.core.anim_loop = entitydata.cigi.anim_loop;
        SaveStruct.core.inherit_alpha = entitydata.cigi.inherit_alpha;
        SaveStruct.core.clamp_mode = entitydata.cigi.clamp_mode;
        SaveStruct.core.coll_detect = entitydata.cigi.coll_detect;
        SaveStruct.core.flymode = entitydata.noncigi.flymode;
        SaveStruct.core.send_rates = entitydata.noncigi.send_rates;
        SaveStruct.core.use_attach_point = entitydata.noncigi.use_attach_point;
        SaveStruct.core.attach_point = entitydata.noncigi.attach_point;
        SaveStruct.core.speed = entitydata.noncigi.speed;
        SaveStruct.core.dofs = entitydata.cigi.dofs;
        SaveStruct.core.rates = entitydata.noncigi.rates;
        ar.Write(&SaveStruct, sizeof(SaveStruct));

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("	length = %d\n", length);
        TRACE1("	Name = \"%s\"\n", (LPCTSTR)m_Name);
        TRACE1("	ID = %d\n", entitydata.cigi.id);
    } else {
        // Read the name.
        ar >> length;
        ar.Read(m_Name.GetBuffer(length), length);
        m_Name.ReleaseBuffer(length);

        // Read the non-shared data.
        ar >> m_RefEntityID;

        // Read the initial states. To maintain file compatibility with
        // Version 3.1.2 and earlier, use an ENTITY_SAVE_STATE structure.
        ENTITY_SAVE_STATE SaveStruct = {0};

        ar.Read(&SaveStruct.core, sizeof(SaveStruct.core));
        m_InitialCigiData.id = SaveStruct.core.id;
        m_InitialCigiData.type = SaveStruct.core.type;
        m_InitialCigiData.parent_id = SaveStruct.core.parent_id;
        m_InitialNonCigiData.target_id = SaveStruct.core.target_id;
        m_InitialNonCigiData.entity_class = SaveStruct.core.entity_class;
        m_InitialCigiData.active = SaveStruct.core.active;
        m_InitialCigiData.alpha = SaveStruct.core.alpha;
        m_InitialCigiData.anim_state = SaveStruct.core.anim_state;
        m_InitialCigiData.anim_dir = SaveStruct.core.anim_dir;
        m_InitialCigiData.anim_loop = SaveStruct.core.anim_loop;
        m_InitialCigiData.inherit_alpha = SaveStruct.core.inherit_alpha;
        m_InitialCigiData.clamp_mode = SaveStruct.core.clamp_mode;
        m_InitialCigiData.coll_detect = SaveStruct.core.coll_detect;
        m_InitialNonCigiData.flymode = SaveStruct.core.flymode;
        m_InitialNonCigiData.send_rates = SaveStruct.core.send_rates;
        m_InitialNonCigiData.use_attach_point = SaveStruct.core.use_attach_point;
        m_InitialNonCigiData.attach_point = SaveStruct.core.attach_point;
        m_InitialNonCigiData.speed = SaveStruct.core.speed;
        m_InitialCigiData.dofs = SaveStruct.core.dofs;
        m_InitialNonCigiData.rates = SaveStruct.core.rates;

        // LWD: Make a guess at this.  We may add file support in spare later.
        m_InitialNonCigiData.collective = 50;

        m_AttachPoints.Serialize(ar);
        m_ArtParts.Serialize(ar);
        m_Components.Serialize(ar);
        m_Hat.Serialize(ar);
        m_Los.Serialize(ar);
        m_CollSegs.Serialize(ar);
        m_CollVols.Serialize(ar);
        m_CollSegIDManager.Serialize(ar);
        m_CollVolIDManager.Serialize(ar);
        m_Waypoints.Serialize(ar);

        // Read the launch, persistent launch, and detonation animations.
        m_LaunchAnimIDs.Serialize(ar);
        m_PersistentLaunchAnimIDs.Serialize(ar);
        m_DetonationAnimIDs.Serialize(ar);

        // m_HatIDManager and m_LosIDManager are loaded by the data manager.

        int weatherflag = 0;
        ar >> weatherflag;
        if (weatherflag) {
            m_WeatherAttributes = new CWeather;
            m_WeatherAttributes->Serialize(ar);
        } else
            m_WeatherAttributes = NULL;

        // Read the shared data.
        ENTITY entitydata = GetAndLock();

        ar.Read(&SaveStruct, sizeof(SaveStruct));
        entitydata.cigi.id = SaveStruct.core.id;
        entitydata.cigi.type = SaveStruct.core.type;
        entitydata.cigi.parent_id = SaveStruct.core.parent_id;
        entitydata.noncigi.target_id = SaveStruct.core.target_id;
        entitydata.noncigi.entity_class = SaveStruct.core.entity_class;
        entitydata.cigi.active = SaveStruct.core.active;
        entitydata.cigi.alpha = SaveStruct.core.alpha;
        entitydata.cigi.anim_state = SaveStruct.core.anim_state;
        entitydata.cigi.anim_dir = SaveStruct.core.anim_dir;
        entitydata.cigi.anim_loop = SaveStruct.core.anim_loop;
        entitydata.cigi.inherit_alpha = SaveStruct.core.inherit_alpha;
        entitydata.cigi.clamp_mode = SaveStruct.core.clamp_mode;
        entitydata.cigi.coll_detect = SaveStruct.core.coll_detect;
        entitydata.noncigi.flymode = SaveStruct.core.flymode;
        entitydata.noncigi.send_rates = SaveStruct.core.send_rates;
        entitydata.noncigi.use_attach_point = SaveStruct.core.use_attach_point;
        entitydata.noncigi.attach_point = SaveStruct.core.attach_point;
        entitydata.noncigi.speed = SaveStruct.core.speed;
        entitydata.cigi.dofs = SaveStruct.core.dofs;
        entitydata.noncigi.rates = SaveStruct.core.rates;

        // LWD: Make a guess at this.  We may add file support in spare later.
        entitydata.noncigi.collective = 50;

        SetLocked(entitydata);
        Unlock();           // unlock after writing

        // Set the local id, type, and class to match.
        m_EntityID = SaveStruct.core.id;
        m_EntityType = SaveStruct.core.type;
        m_EntityClass = SaveStruct.core.entity_class;

        // Read the padding.
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("	length = %d\n", length);
        TRACE1("	Name = \"%s\"\n", (LPCTSTR)m_Name);
        TRACE1("	ID = %d\n", entitydata.cigi.id);
    }
}

void CEntity::SerializeStaticVars(CArchive &ar)
{
    CDebugTrace trace("CEntity::SerializeStaticVars(CArchive &)");

    CEntity::m_HatIDManager.Serialize(ar);
    CEntity::m_LosIDManager.Serialize(ar);
}

BOOL CEntity::ApplyTemplate(const TEMPL_ENTITY *templ)
{
    CDebugTrace trace("CEntity::ApplyTemplate(TEMPL_ENTITY *)");

    unsigned long handle = 0;

    // Set the local versions of the type and class.
    m_EntityType = templ->Type;
    m_EntityClass = templ->Class;

    // Create the name.
    if (GetID() == 0) {
        m_Name = "0: Ownship";
    } else {
        m_Name.Format("%d: %s", GetID(), templ->Name);
    }

    // Set the initial state from the template and the set the shared data.
    ENTITY entity = GetAndLock();
    entity.noncigi.entity_class = templ->Class;
    entity.cigi.type = templ->Type;
    entity.cigi.alpha = templ->Alpha;
    entity.cigi.inherit_alpha = templ->InheritAlpha;
    entity.cigi.clamp_mode = templ->ClampMode;
    entity.cigi.anim_dir = templ->AnimDirection;
    entity.cigi.anim_loop = templ->AnimLoop;
    SetLocked(entity);
    Unlock();

    // Build the component list.
    TEMPL_COMPONENT *comp_templ = templ->ComponentList.GetHead(&handle);
    while (comp_templ) {
        CComponent *comp = new CComponent(COMP_CLASS_ENTITY, comp_templ);

        // Set the instance ID of the component to the entity id.
        comp->SetInstanceID(GetID());

        AddComponent(comp);

        comp_templ = templ->ComponentList.GetNext(&handle);
    }

    // Build the articulated part list.
    TEMPL_ARTPART *artpart_templ = templ->ArtPartList.GetHead(&handle);
    while (artpart_templ) {
        CArtPart *artpart = new CArtPart(artpart_templ);
        AddArtPart(artpart);

        artpart_templ = templ->ArtPartList.GetNext(&handle);
    }

    // Build the attachment point list.
    TEMPL_ATTACHPOINT *attach_templ = templ->AttachPointList.GetHead(&handle);
    while (attach_templ) {
        CAttachPoint *attach = new CAttachPoint(attach_templ);
        AddAttachPoint(attach);

        attach_templ = templ->AttachPointList.GetNext(&handle);
    }

    // Build the collision segment list.
    TEMPL_COLLISION_SEG *seg_templ = templ->CollisionSegList.GetHead(&handle);
    while (seg_templ) {
        CCollisionSeg *seg = new CCollisionSeg(seg_templ);
        seg->SetEntityID(GetID());
        AddCollisionSeg(seg);

        seg_templ = templ->CollisionSegList.GetNext(&handle);
    }

    // Build the collision volume list.
    TEMPL_COLLISION_VOL *vol_templ = templ->CollisionVolList.GetHead(&handle);
    while (vol_templ) {
        CCollisionVol *vol = new CCollisionVol(vol_templ);
        vol->SetEntityID(GetID());
        AddCollisionVol(vol);

        vol_templ = templ->CollisionVolList.GetNext(&handle);
    }

    // If this is a weather entity, apply the weather properties.
    if (templ->Class == ENTITY_CLASS_CLOUD) {
        if (templ->WeatherAttributes)
            m_WeatherAttributes = new CWeather(templ->WeatherAttributes);
        else
            m_WeatherAttributes = new CWeather;

        m_WeatherAttributes->SetOwnerID(GetID());
        m_WeatherAttributes->SetScope(SCOPE_ENTITY);

        // Send a Weather Control packet to the driver to set
        // the initial weather conditions.
        unsigned char buffer[sizeof(CIGI_WEATHER_CONTROL)] = {0};

        // Create a Weather Control packet.
        m_WeatherAttributes->CreateCigiPacket(buffer);

        // QueueCigiMessage() will figure out the packet size.
        //QueueCigiMessage(buffer);
        SendImmedCigiMessage(buffer);
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Gets
//
///////////////////////////////////////////////////////////////////////////////

void CEntity::GetEntity(ENTITY *entity, BOOL keepLock)
{
    // Attempt to get the object until it can be locked.
    while (!m_SharedObject.GetAndLock(entity))
        Sleep(1);

    // Immediately unlock if we don't need it.
    if (!keepLock)
        Unlock();
}

int CEntity::GetActiveState(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.cigi.active;
}

int CEntity::GetParent(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.cigi.parent_id;
}

int CEntity::GetTarget(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.noncigi.target_id;
}

unsigned char CEntity::GetAlpha(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.cigi.alpha;
}

int CEntity::GetAnimState(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.cigi.anim_state;
}

int CEntity::GetAnimDir(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.cigi.anim_dir;
}

int CEntity::GetAnimLoop(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.cigi.anim_loop;
}

BOOL CEntity::GetInheritAlpha(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.cigi.inherit_alpha;
}

int CEntity::GetClampMode(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.cigi.clamp_mode;
}

BOOL CEntity::GetCollDetectEnable(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.cigi.coll_detect ? TRUE : FALSE;
}

int CEntity::GetFlyMode(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.noncigi.flymode;
}

int CEntity::GetAttachPoint(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.noncigi.attach_point;
}

double CEntity::GetSpeed(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.noncigi.speed;
}

long CEntity::GetCollective(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.noncigi.collective;
}

BOOL CEntity::GetFlyWaypoints(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.unsaved.fly_waypoints ? TRUE : FALSE;
}

BOOL CEntity::GetTrackTarget(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.unsaved.track_target ? TRUE : FALSE;
}

BOOL CEntity::GetRollDisable(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.unsaved.roll_disable ? TRUE : FALSE;
}

BOOL CEntity::GetSendRates(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.noncigi.send_rates;
}

int CEntity::GetRatesCoordSystem(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.noncigi.rates_coordsys;
}

ENTITY CEntity::GetAndLock(void)
{
    ENTITY entity;
    GetEntity(&entity, TRUE);
    return entity;
}

ENTITY_CIGI_DATA CEntity::GetAndLockCigi(void)
{
    ENTITY entity;
    GetEntity(&entity, TRUE);
    return entity.cigi;
}

ENTITY_NONCIGI_DATA CEntity::GetAndLockNonCigi(void)
{
    ENTITY entity;
    GetEntity(&entity, TRUE);
    return entity.noncigi;
}

ENTITY_TEMP_DATA CEntity::GetAndLockUnsaved(void)
{
    ENTITY entity;
    GetEntity(&entity, TRUE);
    return entity.unsaved;
}

DOF CEntity::GetAndLockDofs(void)
{
    ENTITY entity;
    GetEntity(&entity, TRUE);
    return entity.cigi.dofs;
}

DOF CEntity::GetDofs(void)
{
    ENTITY entity;
    GetEntity(&entity);
    return entity.cigi.dofs;
}

RATES CEntity::GetAndLockRates(void)
{
    ENTITY entity;
    GetEntity(&entity, TRUE);
    return entity.noncigi.rates;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Sets
//
///////////////////////////////////////////////////////////////////////////////

void CEntity::SetActiveState(const int state, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetActiveState(int, BOOL)");

    if (set_initial)
        SetInitialActiveState(state);

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.unsaved.entityctrl_has_changed = 1;
    entity.cigi.active = state ? 1 : 0;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetParent(const int parent, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetParent(int, BOOL)");

    if (set_initial)
        SetInitialParent(parent);

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.unsaved.entityposition_has_changed = 1;
    entity.cigi.parent_id = parent;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetTarget(const int target, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetTarget(int, BOOL)");

    if (set_initial)
        SetInitialTarget(target);

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.noncigi.target_id = target;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetAlpha(const unsigned char alpha, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetAlpha(unsigned char, BOOL)");

    if (set_initial)
        SetInitialAlpha(alpha);

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.unsaved.entityctrl_has_changed = 1;
    entity.cigi.alpha = alpha;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetAnimState(const int state, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetAnimState(int, BOOL)");

    if ((unsigned int)state > 3)
        return;

    if (set_initial)
        SetInitialAnimState(state);

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.unsaved.entityanimation_has_changed = 1;
    entity.cigi.anim_state = state;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetAnimDir(const int direction, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetAnimDir(int, BOOL)");

    if (set_initial)
        SetInitialAnimDir(direction);

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.unsaved.entityanimation_has_changed = 1;
    entity.cigi.anim_dir = direction;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetAnimLoop(const int loopmode, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetAnimLoop(int, BOOL)");

    if (set_initial)
        SetInitialAnimLoop(loopmode);

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.unsaved.entityanimation_has_changed = 1;
    entity.cigi.anim_loop = loopmode;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetInheritAlpha(const BOOL inherit, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetInheritAlpha(BOOL, BOOL)");

    if (set_initial)
        SetInitialInheritAlpha(inherit);

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.unsaved.entityctrl_has_changed = 1;
    entity.cigi.inherit_alpha = inherit ? 1 : 0;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetClampMode(const int mode, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetClampMode(int, BOOL)");

    if ((unsigned int)mode > 2)
        return;

    if (set_initial)
        SetInitialClampMode(mode);

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.unsaved.entityposition_has_changed = 1;
    entity.cigi.clamp_mode = mode;
    m_SharedObject.SetLockedEntityPosition(entity);

    Unlock();
}

void CEntity::SetCollDetectEnable(const BOOL enable, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetCollDetectEnable(BOOL, BOOL)");

    if (set_initial)
        SetInitialCollDetectEnable(enable);

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.unsaved.entityctrl_has_changed = 1;
    entity.cigi.coll_detect = enable ? 1 : 0;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetFlyMode(const int mode, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetFlyMode(int, BOOL)");

    if ((unsigned int)mode > 4)
        return;

    if (set_initial)
        SetInitialFlyMode(mode);

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.noncigi.flymode = mode;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetAttachPoint(const int attachpt, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetAttachPoint(int, BOOL)");

    if (set_initial)
        SetInitialAttachPoint(attachpt);

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.noncigi.attach_point = attachpt;
    entity.noncigi.use_attach_point = (attachpt >= 0) ? 1 : 0;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetSpeed(const double speed, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetSpeed(double, BOOL)");

    if (set_initial)
        SetInitialSpeed(speed);

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.noncigi.speed = speed;
    m_SharedObject.SetLockedEntityPosition(entity);

    Unlock();
}

void CEntity::SetCollective(const long collective, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetCollective(long, BOOL)");

    if (set_initial)
        SetInitialCollective(collective);

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.noncigi.collective = collective;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetFlyWaypoints(const BOOL enable)
{
    CDebugTrace trace("CEntity::SetFlyWaypoints(BOOL)");

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.unsaved.fly_waypoints = enable;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetTrackTarget(const BOOL enable)
{
    CDebugTrace trace("CEntity::SetTrackTarget(BOOL)");

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.unsaved.track_target = enable;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetRollDisable(const BOOL disable)
{
    CDebugTrace trace("CEntity::SetRollDisable(BOOL)");

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.unsaved.roll_disable = disable;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetLocked(const ENTITY &entity, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetLocked(ENTITY &)");

    if (set_initial) {
        SetInitialCigi(entity.cigi);
        SetInitialNonCigi(entity.noncigi);
    }

    // Assume the object is already locked.
    m_SharedObject.SetLocked(entity);
}

void CEntity::SetLockedEntityPosition(const ENTITY &entity, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetLocked(ENTITY &)");

    if (set_initial) {
        SetInitialCigi(entity.cigi);
        SetInitialNonCigi(entity.noncigi);
    }

    // Assume the object is already locked.
    m_SharedObject.SetLockedEntityPosition(entity);
}

void CEntity::SetLockedCigi(const ENTITY_CIGI_DATA &data, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetLockedCigiData(ENTITY_CIGI_DATA &, const BOOL)");

    if (set_initial)
        SetInitialCigi(data);

    // Assume the object is already locked.
    m_SharedObject.SetLockedCigiData(data);
}

void CEntity::SetLockedNonCigi(const ENTITY_NONCIGI_DATA &data, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetLockedNonCigiData(ENTITY_NONCIGI_DATA &, const BOOL)");

    if (set_initial)
        SetInitialNonCigi(data);

    // Assume the object is already locked.
    m_SharedObject.SetLockedNonCigiData(data);
}

void CEntity::SetLockedUnsaved(const ENTITY_TEMP_DATA &data)
{
    CDebugTrace trace("CEntity::SetUnsavedData(ENTITY_TEMP_DATA &)");

    // Assume the object is already locked.
    m_SharedObject.SetLockedUnsavedData(data);
}

void CEntity::SetSendRates(const BOOL send)
{
    CDebugTrace trace("CEntity::SetSendRates(BOOL)");

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.noncigi.send_rates = send;
    m_SharedObject.SetLockedEntityPosition(entity);

    Unlock();
}

void CEntity::SetRatesCoordSystem(const int cs)
{
    CDebugTrace trace("CEntity::SetSendRates(BOOL)");

    ENTITY entity = {0};
    GetEntity(&entity, TRUE);

    entity.noncigi.rates_coordsys = cs;
    m_SharedObject.SetLocked(entity);

    Unlock();
}

void CEntity::SetLockedRates(const RATES &rates, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetRates(RATES &, BOOL)");

    if (set_initial)
        SetInitialRates(rates);

    // Assume the object is locked.
    m_SharedObject.SetLockedRates(rates);
}

void CEntity::SetLockedDofs(const DOF &dof, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetPosAtt(DOF &, BOOL)");

    if (set_initial)
        SetInitialDofs(dof);

    // Assume the object is locked.
    m_SharedObject.SetLockedDofs(dof);
}

CComponent *CEntity::GetComponent(const int id)
{
    CDebugTrace trace("CEntity::GetComponent(int)");

    CComponent *search = NULL;

    m_Components.Lookup(id, search);

    return search;
}

BOOL CEntity::AddComponent(CComponent *comp)
{
    CDebugTrace trace("CEntity::AddComponent(CComponent *)");

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

BOOL CEntity::DeleteComponent(const int id)
{
    CDebugTrace trace("CEntity::DeleteComponent(int)");

    CComponent *search = NULL;

    if (m_Components.Lookup(id, search)) {
        if (search)
            delete search;

        m_Components.RemoveKey(id);

        return TRUE;
    }

    return FALSE;
}

void CEntity::ClearComponents(void)
{
    CDebugTrace trace("CEntity::ClearComponents()");

    int id = 0;
    CComponent *comp = NULL;
    POSITION pos = m_Components.GetStartPosition();
    while (pos) {
        m_Components.GetNextAssoc(pos, id, comp);
        delete comp;
    }

    m_Components.RemoveAll();
}

CArtPart *CEntity::GetArtPart(const int id)
{
    CDebugTrace trace("CEntity::GetArtPart(int)");

    CArtPart *search = NULL;

    m_ArtParts.Lookup(id, search);

    return search;
}

BOOL CEntity::AddArtPart(CArtPart *artpart)
{
    CDebugTrace trace("CEntity::AddArtPart(CArtPart *)");

    BOOL retval = FALSE;
    int id = artpart->GetID();
    CArtPart *search = NULL;
    artpart->SetEntityID(GetID());

    // If it already exists, delete it.
    if (m_ArtParts.Lookup(id, search)) {
        // Maintain the positions of the existing articulated part.
        artpart->SetXOffset(search->GetXOffset());
        artpart->SetYOffset(search->GetYOffset());
        artpart->SetZOffset(search->GetZOffset());
        artpart->SetRoll(search->GetRoll());
        artpart->SetPitch(search->GetPitch());
        artpart->SetYaw(search->GetYaw());

        delete search;
        retval = TRUE;  // return TRUE if part exists
    }

    m_ArtParts.SetAt(id, artpart);

    return retval;
}

BOOL CEntity::DeleteArtPart(const int id)
{
    CDebugTrace trace("CEntity::DeleteArtPart(int)");

    CArtPart *search = NULL;

    if (m_ArtParts.Lookup(id, search)) {
        if (search)
            delete search;

        m_ArtParts.RemoveKey(id);

        return TRUE;
    }

    return FALSE;
}

void CEntity::ClearArtParts(void)
{
    CDebugTrace trace("CEntity::ClearArtParts()");

    int id = 0;
    CArtPart *part = NULL;
    POSITION pos = m_ArtParts.GetStartPosition();
    while (pos) {
        m_ArtParts.GetNextAssoc(pos, id, part);
        delete part;
    }

    m_ArtParts.RemoveAll();
}

CAttachPoint *CEntity::GetAttachPoint(const int id)
{
    CDebugTrace trace("CEntity::GetAttachPoint(int)");

    CAttachPoint *search = NULL;

    m_AttachPoints.Lookup(id, search);

    return search;
}

BOOL CEntity::AddAttachPoint(CAttachPoint *attach)
{
    CDebugTrace trace("CEntity::AddAttachPoint(CAttachPoint *)");

    BOOL retval = FALSE;
    int id = attach->GetID();
    CAttachPoint *search = NULL;

    // If the attach point already exists, delete it.
    if (m_AttachPoints.Lookup(id, search)) {
        //LWD: we need to move the attached child entities to the new positions or
        //     remove from the attach point and keep at the same position

        delete search;
        retval = TRUE;      // return TRUE if the attachpoint exists
    }

    m_AttachPoints.SetAt(id, attach);

    return retval;
}

void CEntity::ClearAttachPoints(void)
{
    CDebugTrace trace("CEntity::ClearAttachPoints()");

    int id = 0;
    CAttachPoint *attach = NULL;
    POSITION pos = m_AttachPoints.GetStartPosition();
    while (pos) {
        m_AttachPoints.GetNextAssoc(pos, id, attach);
        delete attach;
    }

    m_AttachPoints.RemoveAll();
}

BOOL CEntity::DeleteAttachPoint(const int id)
{
    CDebugTrace trace("CEntity::DeleteAttachPoint(int)");

    CAttachPoint *search = NULL;

    if (m_AttachPoints.Lookup(id, search)) {
        if (search)
            delete search;

        m_AttachPoints.RemoveKey(id);

        return TRUE;
    }

    return FALSE;
}

CHat *CEntity::GetHat(const int id)
{
    CDebugTrace trace("CEntity::GetHat(int)");

    CHat *search = NULL;

    m_Hat.Lookup(id, search);

    return search;
}

BOOL CEntity::AddHat(CHat *hat)
{
    CDebugTrace trace("CEntity::AddHat(CHat *)");

    BOOL retval = FALSE;
    int id = hat->GetID();
    CHat *search = NULL;

    // If the HAT already exists, delete it.
    if (m_Hat.Lookup(id, search)) {
        delete search;
        retval = TRUE;      // return TRUE if it exists
    }

    m_Hat.SetAt(id, hat);
    m_HatIDManager.MarkID(id);

    // Add to the data manager's lookup table.
    g_DataManager.AddHatLookup(id, hat);

    return retval;
}

void CEntity::ClearHat(void)
{
    CDebugTrace trace("CEntity::ClearHat()");

    int id = 0;
    CHat *hat = NULL;
    POSITION pos = m_Hat.GetStartPosition();
    while (pos) {
        m_Hat.GetNextAssoc(pos, id, hat);
        delete hat;
    }

    m_Hat.RemoveAll();
    g_DataManager.RemoveHatLookupItems();
}

BOOL CEntity::DeleteHat(const int id)
{
    CDebugTrace trace("CEntity::DeleteHat(int)");

    CHat *search = NULL;

    if (m_Hat.Lookup(id, search)) {
        if (search)
            delete search;

        m_Hat.RemoveKey(id);
        m_HatIDManager.UnmarkID(id);

        // Remove from the data manager's lookup table.
        g_DataManager.RemoveHatLookup(id);

        return TRUE;
    }

    return FALSE;
}

CLos *CEntity::GetLos(const int id)
{
    CDebugTrace trace("CEntity::GetLos(int)");

    CLos *search = NULL;

    m_Los.Lookup(id, search);

    return search;
}

BOOL CEntity::AddLos(CLos *los)
{
    CDebugTrace trace("CEntity::AddLos(CLos *)");

    BOOL retval = FALSE;
    int id = los->GetID();
    CLos *search = NULL;

    // If the HAT already exists, delete it.
    if (m_Los.Lookup(id, search)) {
        delete search;
        retval = TRUE;      // return TRUE if it exists
    }

    m_Los.SetAt(id, los);
    m_LosIDManager.MarkID(id);

    // Add to the data manager's lookup table.
    g_DataManager.AddLosLookup(id, los);

    return retval;
}

void CEntity::ClearLos(void)
{
    CDebugTrace trace("CEntity::ClearLos()");

    int id = 0;
    CLos *los = NULL;
    POSITION pos = m_Los.GetStartPosition();
    while (pos) {
        m_Los.GetNextAssoc(pos, id, los);
        delete los;
    }

    m_Los.RemoveAll();
    g_DataManager.RemoveLosLookupItems();
}

BOOL CEntity::DeleteLos(const int id)
{
    CDebugTrace trace("CEntity::DeleteLos(int)");

    CLos *search = NULL;

    if (m_Los.Lookup(id, search)) {
        if (search)
            delete search;

        m_Los.RemoveKey(id);
        m_LosIDManager.UnmarkID(id);

        // Remove from the data manager's lookup table.
        g_DataManager.RemoveLosLookup(id);

        return TRUE;
    }

    return FALSE;
}

CCollisionSeg *CEntity::GetCollisionSeg(const int id)
{
    CDebugTrace trace("CEntity::GetCollisionSeg(int)");

    CCollisionSeg *search = NULL;

    m_CollSegs.Lookup(id, search);

    return search;
}

BOOL CEntity::AddCollisionSeg(CCollisionSeg *seg)
{
    CDebugTrace trace("CEntity::AddCollisionSeg(CCollisionSeg *)");

    BOOL retval = FALSE;
    int id = seg->GetID();
    CCollisionSeg *search = NULL;

    // If the segment already exists, delete it.
    if (m_CollSegs.Lookup(id, search)) {
        delete search;
        retval = TRUE;      // return TRUE if it exists
    }

    m_CollSegs.SetAt(id, seg);
    m_CollSegIDManager.MarkID(id);

    return retval;
}

void CEntity::ClearCollisionSegs(void)
{
    CDebugTrace trace("CEntity::ClearCollisionSegs()");

    int id = 0;
    CCollisionSeg *seg = NULL;
    POSITION pos = m_CollSegs.GetStartPosition();
    while (pos) {
        m_CollSegs.GetNextAssoc(pos, id, seg);
        delete seg;
    }

    m_CollSegs.RemoveAll();
}

BOOL CEntity::DeleteCollisionSeg(const int id)
{
    CDebugTrace trace("CEntity::DeleteCollisionSeg(int)");

    CCollisionSeg *search = NULL;

    if (m_CollSegs.Lookup(id, search)) {
        if (search)
            delete search;

        m_CollSegs.RemoveKey(id);
        m_CollSegIDManager.UnmarkID(id);

        return TRUE;
    }

    return FALSE;
}

CCollisionVol *CEntity::GetCollisionVol(const int id)
{
    CDebugTrace trace("CEntity::GetCollisionVol(int)");

    CCollisionVol *search = NULL;

    m_CollVols.Lookup(id, search);

    return search;
}

BOOL CEntity::AddCollisionVol(CCollisionVol *vol)
{
    CDebugTrace trace("CEntity::AddCollisionVol(CCollisionVol *)");

    BOOL retval = FALSE;
    int id = vol->GetID();
    CCollisionVol *search = NULL;

    // If the volume already exists, delete it.
    if (m_CollVols.Lookup(id, search)) {
        delete search;
        retval = TRUE;      // return TRUE if it exists
    }

    m_CollVols.SetAt(id, vol);
    m_CollVolIDManager.MarkID(id);

    return retval;
}

void CEntity::ClearCollisionVols(void)
{
    CDebugTrace trace("CEntity::ClearCollisionVols()");

    int id = 0;
    CCollisionVol *vol = NULL;
    POSITION pos = m_CollVols.GetStartPosition();
    while (pos) {
        m_CollVols.GetNextAssoc(pos, id, vol);
        delete vol;
    }

    m_CollVols.RemoveAll();
}

BOOL CEntity::DeleteCollisionVol(const int id)
{
    CDebugTrace trace("CEntity::DeleteCollisionVol(int)");

    CCollisionVol *search = NULL;

    if (m_CollVols.Lookup(id, search)) {
        if (search)
            delete search;

        m_CollVols.RemoveKey(id);
        m_CollVolIDManager.UnmarkID(id);

        return TRUE;
    }

    return FALSE;
}

BOOL CEntity::CalcRefBearingAndRange(double *bearing, double *range)
{
    if (m_RefEntityID < 0)
        return FALSE;

    CEntity *refentity = g_DataManager.GetEntity(m_RefEntityID);
    if (!refentity)
        return FALSE;

    DOF refdof = refentity->GetDofs();
    DOF dof = GetDofs();

    double b = 0.0;
    double d = 0.0;

    // Get the distance and azimuth from the reference entity to this entity.
    d = distAzimuth(&g_ERM, refdof.latitude, refdof.longitude,
                    dof.latitude, dof.longitude, &b);

    if (bearing) {
        b = b - refdof.yaw;

        // +/-180 seems to be more intuitive than 0-360.
        if (b > 180.0)
            *bearing = b - 360.0;
        else
            *bearing = b;
    }

    if (range)
        *range = d;

    return TRUE;
}

BOOL CEntity::SetRelativePosition(const double bearing, const double range, const BOOL set_initial)
{
    CDebugTrace trace("CEntity::SetRelativePosition(double, double, BOOL)");

    if (m_RefEntityID < 0)
        return FALSE;

    CEntity *refentity = g_DataManager.GetEntity(m_RefEntityID);
    if (!refentity)
        return FALSE;

    DOF refdof = refentity->GetDofs();
    DOF dof = GetAndLockDofs();

    waypoint(&g_ERM, refdof.latitude, refdof.longitude,
             range, bearing + refdof.yaw,
             &(dof.latitude), &(dof.longitude));

    SetLockedDofs(dof);
    Unlock();

    if (set_initial)
        SetInitialDofs(dof);

    return TRUE;
}

BOOL CEntity::IsDescendantOf(const int ancestor_id)
{
    CDebugTrace trace("CEntity::IsDescendantOf(int)");

    int parent_id = GetParent();

    if (parent_id == -1)
        return FALSE;

    if (parent_id == ancestor_id)
        return TRUE;
    else {
        CEntity *parent = g_DataManager.GetEntity(parent_id);
        if (!parent)
            return FALSE;

        return parent->IsDescendantOf(ancestor_id);
    }
}

void CEntity::AddWaypoint(CWaypoint *waypoint)
{
    CDebugTrace trace("CEntity::AddWayoint(CWaypoint *)");

    if (waypoint) {
        // A copy of the waypoint will be created.
        m_Waypoints.Push(*waypoint);
    }
}

int CEntity::AcquireNextWaypoint(const BOOL enable)
{
    CDebugTrace trace("CEntity::AcquireNextWaypoint(BOOL)");

    int retval = -1;
    CWaypoint waypoint;

    if (m_Waypoints.Pop(&waypoint) >= 0) {
        ENTITY entity = GetAndLock();

        // Get the relative coordinates based on the entity's current
        // position.  (This is the entity's own copy of the waypoint.)
        DOF dof = entity.cigi.dofs;
        waypoint.MakeAbsolute(dof.latitude, dof.longitude, dof.altitude);

        entity.unsaved.fly_waypoints = enable ? 1 : 0;
        entity.unsaved.waypoint_valid = 1;
        entity.unsaved.waypoint_reached = 0;
        entity.unsaved.waypoint_id = waypoint.GetID();
        entity.unsaved.waypoint_type = waypoint.GetType();
        entity.unsaved.yaw_ctr = 0;
        entity.unsaved.pitch_ctr = 0;
        entity.unsaved.roll_ctr = 0;
        entity.unsaved.waypoint_x = waypoint.GetX();
        entity.unsaved.waypoint_y = waypoint.GetY();
        entity.unsaved.waypoint_z = waypoint.GetZ();
        entity.unsaved.accel = waypoint.GetAccel();
        entity.unsaved.turn_rate = waypoint.GetTurnRate();
        entity.unsaved.speed_final = KnotsToMPS(waypoint.GetAirspeed());
        entity.unsaved.speed_init = entity.noncigi.speed;
        SetLocked(entity);
        Unlock();

        CString str;
        str.Format("Next waypoint at %lf°%c, %lf°%c, %lfm",
                   fabs(waypoint.GetLatitude()),
                   (waypoint.GetLatitude() < 0) ? 'S' : 'N',
                   fabs(waypoint.GetLongitude()),
                   (waypoint.GetLongitude() < 0) ? 'W' : 'E',
                   waypoint.GetAltitude());
        PrintMessageText((LPCTSTR)str);

        retval = waypoint.GetID();
    } else {
        ENTITY entity = GetAndLock();
        entity.unsaved.fly_waypoints = 0;
        entity.unsaved.waypoint_valid = 0;
        SetLocked(entity);
        Unlock();

        PrintMessageText("No more waypoints.");
    }

    return retval;
}

void CEntity::ClearWaypoints(void)
{
    CDebugTrace trace("CEntity::ClearWaypoints()");

    CWaypoint waypoint;
    while (m_Waypoints.Pop(&waypoint) >= 0);
}

void CEntity::FlyWaypoints(const BOOL enable)
{
    CDebugTrace trace("CEntity::FlyWaypoints(BOOL)");

    ENTITY_TEMP_DATA unsaved = GetAndLockUnsaved();

    if (enable && !unsaved.waypoint_valid) {
        // AcquireNextWaypoint() locks object.
        Unlock();
        AcquireNextWaypoint();
    } else {
        unsaved.fly_waypoints = enable;
        SetLockedUnsaved(unsaved);
        Unlock();
    }

}

BOOL CEntity::Unlock(void)
{
    int retval = m_SharedObject.Unlock();

    // The only way this will fail is if it is not locked. We
    // should check for this in DEBUG mode.
    ASSERT(retval);

    return (BOOL)retval;
}

long CEntity::GetID(void)
{
    return m_EntityID;
}

long CEntity::GetType(void)
{
    return m_EntityType;
}

long CEntity::GetClass(void)
{
    return m_EntityClass;
}

const char *const CEntity::GetSharedName(void)
{
    CDebugTrace trace("CEntity::GetSharedName()");

    return m_SharedObject.GetInstanceName();
}

HANDLE CEntity::GetSharedHandle(void)
{
    CDebugTrace trace("CEntity::GetSharedHandle()");

    return m_SharedHandle;
}

void CEntity::SetHtree(const HTREEITEM htree)
{
    CDebugTrace trace("CEntity::SetHtree(HTREEITEM)");

    m_HtreeEntity = htree;
}

HTREEITEM CEntity::GetHtree(void)
{
    return m_HtreeEntity;
}

void CEntity::SetHtreeEnv(const HTREEITEM htree)
{
    CDebugTrace trace("CEntity::SetHtreeEnv(HTREEITEM)");

    m_HtreeEnv = htree;
}

HTREEITEM CEntity::GetHtreeEnv(void)
{
    CDebugTrace trace("CEntity::GetHtreeEnv()");

    return m_HtreeEnv;
}

void CEntity::SetSituationHandle(const POSITION pos)
{
    CDebugTrace trace("CEntity::SetSituationHandle(POSITION)");

    m_SituationHandle = pos;
}

POSITION CEntity::GetSituationHandle(void)
{
    CDebugTrace trace("CEntity::GetSituationHandle()");

    return m_SituationHandle;
}

long CEntity::GetRefEntityID(void)
{
    return m_RefEntityID;
}

void CEntity::SetRefEntityID(const long id)
{
    m_RefEntityID = id;
}

void CEntity::SetName(LPCTSTR name)
{
    m_Name = name;
}

void CEntity::SetName(CString &name)
{
    m_Name = name;
}

CString CEntity::GetName(void)
{
    return m_Name;
}

void CEntity::SetInitialActiveState(const int state)
{
    m_InitialCigiData.active = state;
}

int CEntity::GetInitialActiveState(void)
{
    return m_InitialCigiData.active;
}

void CEntity::SetInitialParent(const int parent)
{
    m_InitialCigiData.parent_id = parent;
}

int CEntity::GetInitialParent(void)
{
    return m_InitialCigiData.parent_id;
}

void CEntity::SetInitialTarget(const int target)
{
    m_InitialNonCigiData.target_id = target;
}

int CEntity::GetInitialTarget(void)
{
    return m_InitialNonCigiData.target_id;
}

void CEntity::SetInitialAlpha(const unsigned char alpha)
{
    m_InitialCigiData.alpha = alpha;
}

unsigned char CEntity::GetInitialAlpha(void)
{
    return m_InitialCigiData.alpha;
}

void CEntity::SetInitialAnimState(const int state)
{
    m_InitialCigiData.anim_state = state;
}

int CEntity::GetInitialAnimState(void)
{
    return m_InitialCigiData.anim_state;
}

void CEntity::SetInitialAnimDir(const int direction)
{
    m_InitialCigiData.anim_dir = direction;
}

int CEntity::GetInitialAnimDir(void)
{
    return m_InitialCigiData.anim_dir;
}

void CEntity::SetInitialAnimLoop(const int loopmode)
{
    m_InitialCigiData.anim_loop = loopmode;
}

int CEntity::GetInitialAnimLoop(void)
{
    return m_InitialCigiData.anim_loop;
}

void CEntity::SetInitialInheritAlpha(const BOOL inherit)
{
    m_InitialCigiData.inherit_alpha = inherit ? 1 : 0;
}

BOOL CEntity::GetInitialInheritAlpha(void)
{
    return m_InitialCigiData.inherit_alpha ? TRUE : FALSE;
}

void CEntity::SetInitialClampMode(const int mode)
{
    m_InitialCigiData.clamp_mode = mode;
}

int CEntity::GetInitialClampMode(void)
{
    return m_InitialCigiData.clamp_mode;
}

void CEntity::SetInitialCollDetectEnable(const BOOL enable)
{
    m_InitialCigiData.coll_detect = enable ? 1 : 0;
}

BOOL CEntity::GetInitialCollDetectEnable(void)
{
    return m_InitialCigiData.coll_detect ? TRUE : FALSE;
}

void CEntity::SetInitialFlyMode(const int mode)
{
    m_InitialNonCigiData.flymode = mode;
}

int CEntity::GetInitialFlyMode(void)
{
    return m_InitialNonCigiData.flymode;
}

void CEntity::SetInitialAttachPoint(const int attachpt)
{
    m_InitialNonCigiData.attach_point = attachpt;
}

int CEntity::GetInitialAttachPoint(void)
{
    return m_InitialNonCigiData.attach_point;
}

void CEntity::SetInitialSpeed(const double speed)
{
    m_InitialNonCigiData.speed = speed;
}

double CEntity::GetInitialSpeed(void)
{
    return m_InitialNonCigiData.speed;
}

void CEntity::SetInitialCollective(const long collective)
{
    m_InitialNonCigiData.collective = collective;
}

long CEntity::GetInitialCollective(void)
{
    return m_InitialNonCigiData.collective;
}

void CEntity::SetInitialSendRates(const BOOL send)
{
    m_InitialNonCigiData.send_rates = send ? 1 : 0;
}

BOOL CEntity::GetInitialSendRates(void)
{
    return m_InitialNonCigiData.send_rates ? TRUE : FALSE;
}

void CEntity::SetInitialRatesCoordSystem(const int cs)
{
    m_InitialNonCigiData.rates_coordsys = cs;
}

int CEntity::GetInitialRatesCoordSystem(void)
{
    return m_InitialNonCigiData.rates_coordsys;
}

void CEntity::SetInitialDofs(const DOF &dofs)
{
    m_InitialCigiData.dofs = dofs;
}

DOF CEntity::GetInitialDofs(void)
{
    return m_InitialCigiData.dofs;
}

void CEntity::SetInitialRates(const RATES &rates)
{
    m_InitialNonCigiData.rates = rates;
}

RATES CEntity::GetInitialRates(void)
{
    return m_InitialNonCigiData.rates;
}

void CEntity::SetInitialCigi(const ENTITY_CIGI_DATA &data)
{
    m_InitialCigiData = data;
}

ENTITY_CIGI_DATA CEntity::GetInitialCigi(void)
{
    return m_InitialCigiData;
}

void CEntity::SetInitialNonCigi(const ENTITY_NONCIGI_DATA &data)
{
    m_InitialNonCigiData = data;
}

ENTITY_NONCIGI_DATA CEntity::GetInitialNonCigi(void)
{
    return m_InitialNonCigiData;
}

CComponentMap &CEntity::GetComponentMap(void)
{
    return m_Components;
}

CArtPartMap &CEntity::GetArtPartMap(void)
{
    return m_ArtParts;
}

CAttachPointMap &CEntity::GetAttachPointMap(void)
{
    return m_AttachPoints;
}

CHatMap &CEntity::GetHatMap(void)
{
    return m_Hat;
}

CLosMap &CEntity::GetLosMap(void)
{
    return m_Los;
}

CCollisionSegMap &CEntity::GetCollisionSegMap(void)
{
    return m_CollSegs;
}

CCollisionVolMap &CEntity::GetCollisionVolMap(void)
{
    return m_CollVols;
}

void CEntity::SendCigiPacket(void)
{
    CDebugTrace trace("CEntity::SendCigiPacket()");

    // Cause the driver to set the changed flag.  This will cause the
    // driver to create and send a CIGI packet.  Setting the active
    // state will accomplish this.
    SetActiveState(GetActiveState());
}

BOOL CEntity::IsDescendantOf(CEntity *ancestor)
{
    CDebugTrace trace("CEntity::IsDescendantOf(CEntity *)");

    return IsDescendantOf(ancestor->GetID());
}

int CEntity::GetNextHatID(void)
{
    CDebugTrace trace("CEntity::GetNextHatID()");

    return m_HatIDManager.GetNextID();
}

int CEntity::GetNextLosID(void)
{
    CDebugTrace trace("CEntity::GetNextLosID()");

    return m_LosIDManager.GetNextID();
}

int CEntity::GetNextCollisionSegID(void)
{
    CDebugTrace trace("CEntity::GetNextCollisionSegID)");

    return m_CollSegIDManager.GetNextID();
}

int CEntity::GetNextCollisionVolID(void)
{
    CDebugTrace trace("CEntity::GetNextCollisionVolID()");

    return m_CollVolIDManager.GetNextID();
}

CWeather *CEntity::GetWeatherAttributes(void)
{
    return m_WeatherAttributes;
}

CList<long, long> &CEntity::GetLaunchAnimList(void)
{
    return m_LaunchAnimIDs;
}

CList<long, long> &CEntity::GetPersistentLaunchAnimList(void)
{
    return m_PersistentLaunchAnimIDs;
}

CList<long, long> &CEntity::GetDetonationAnimList(void)
{
    return m_DetonationAnimIDs;
}
