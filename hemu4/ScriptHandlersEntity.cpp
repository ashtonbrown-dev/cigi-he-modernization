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

#include "StdAfx.h"
#include "globals.h"
#include "Entity.h"
#include "hemumsg.h"

extern void ScriptPostTextMessage(const char *text);

long ScriptDoAddEntity(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long id = -1;
    unsigned long type = -1;

    sscanf(buffer, "%s %lu %lu", keyword, &id, &type);

    if (g_DataManager.GetEntity(id)) {
        CString errmsg;
        errmsg.Format("Warning: Entity %d already exists. (line %lu)", id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);

        return 0;   // return success
    }

    // If we are creating an entity of type 0 and the user hasn't
    // defined the type, create a default template.
    TEMPL_ENTITY default_templ;
    TEMPL_ENTITY *templ = GetEntityTempl(type);
    if (!templ) {
        if (type == 0) {
            default_templ.Type = 0;
            default_templ.Name = "None";
            default_templ.Class = ENTITY_CLASS_FIXEDWING;

            templ = &default_templ;
        } else {
            CString errmsg;
            errmsg.Format("Error: Invalid entity type. (line %lu)", linenum);
            ScriptPostTextMessage((LPCTSTR)errmsg);
            return -1;
        }
    }

    // Create the entity and send a message to the main thread so it
    // will add everything to the GUI because we're not thread-safe.
    if (g_DataManager.CreateEntity(id, templ)) {
        MESSAGE_SCRIPT_ADD_ENTITY_TO_GUI msg_add;
        msg_add.entity_id = id;
        PostGUIMsg(msg_add);
    } else {
        CString errmsg;
        errmsg.Format("Error: Cannot create entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoRemoveEntity(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long id = -1;

    sscanf(buffer, "%s %lu", keyword, &id);

    CEntity *entity = g_DataManager.GetEntity(id);
    if (entity) {
        // Send a message to the main thread so it will delete
        // everything from the GUI (we're not thread-safe).
        MESSAGE_SCRIPT_DEL_ENTITY_FROM_GUI msg;
        msg.entity_id = id;
        msg.htree = (unsigned long)entity->GetHtree();
        msg.htree_env = (unsigned long)entity->GetHtreeEnv();
        msg.situation_hdl = (unsigned long)entity->GetSituationHandle();

        // If we're not deleting in the same frame we are creating, post
        // the "delete" message.  Otherwise, the object will have been
        // deleted, so the "create" message that is now queued will fail.
        if (msg.htree)
            PostGUIMsg(msg);

        // And delete the actual entity object.
        g_DataManager.DeleteEntity(entity);
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    return 0;
}

long ScriptDoEntityAirspeed(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double knots = 0.0;

    sscanf(buffer, "%s %lu %lf", keyword, &entity_id, &knots);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    ENTITY_NONCIGI_DATA data = entity->GetAndLockNonCigi();
    data.speed = KnotsToMPS(knots);
    entity->SetLockedNonCigi(data);
    entity->Unlock();

    return 0;
}

long ScriptDoEntityAddWaypoint(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long waypoint_id = -1;
    int airspeed = 0;
    double accel = 0.0;
    double turn_rate = 0.0;

    sscanf(buffer, "%s %lu %lu %d %lf %lf", keyword, &entity_id, &waypoint_id,
           &airspeed, &accel, &turn_rate);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWaypoint *waypoint = g_DataManager.GetWaypoint(waypoint_id);
    if (!waypoint) {
        CString errmsg;
        errmsg.Format("Error: Invalid waypoint specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // These properties will be overwritten as needed.
    waypoint->SetAirspeed(airspeed);
    waypoint->SetAccel(accel);
    waypoint->SetTurnRate(turn_rate);

    entity->AddWaypoint(waypoint);

    return 0;
}

long ScriptDoEntityAlpha(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long alpha = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &alpha);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (alpha > 255) {
        CString errmsg;
        errmsg.Format("Error: Invalid value specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the entity properties and initial state.
    ENTITY_CIGI_DATA data = entity->GetAndLockCigi();
    data.alpha = (unsigned char)alpha;
    entity->SetLockedCigi(data);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialAlpha((unsigned char)alpha);

    return 0;
}

long ScriptDoEntityAltitude(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double alt = 0.0;

    sscanf(buffer, "%s %lu %lf", keyword, &entity_id, &alt);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dof = entity->GetAndLockDofs();
    dof.altitude = alt;
    entity->SetLockedDofs(dof);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dof);

    return 0;
}

long ScriptDoEntityAltitudeRelative(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long ref_id = -1;
    double delta = 0.0;

    sscanf(buffer, "%s %lu %lu %lf", keyword, &entity_id, &ref_id, &delta);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CEntity *ref_entity = g_DataManager.GetEntity(ref_id);
    if (!ref_entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid reference entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the entity properties and initial state.
    DOF refDof = ref_entity->GetDofs();
    DOF dof = entity->GetAndLockDofs();
    dof.altitude = refDof.altitude + delta;
    entity->SetLockedDofs(dof);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dof);

    return 0;
}

long ScriptDoEntityAnimDirection(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long direction = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &direction);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (direction > 1) {
        CString errmsg;
        errmsg.Format("Error: Invalid value specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the entity properties and initial state.
    ENTITY_CIGI_DATA data = entity->GetAndLockCigi();
    data.anim_dir = direction;
    entity->SetLockedCigi(data);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialAnimDir(direction);

    return 0;
}

long ScriptDoEntityAnimLoopmode(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id =  -1;
    unsigned long mode = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &mode);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (mode > 1) {
        CString errmsg;
        errmsg.Format("Error: Invalid value specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the entity properties and initial state.
    ENTITY_CIGI_DATA data = entity->GetAndLockCigi();
    data.anim_loop = mode;
    entity->SetLockedCigi(data);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialAnimLoop(mode);

    return 0;
}

long ScriptDoEntityAnimState(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long state = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &state);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (state > 3) {
        CString errmsg;
        errmsg.Format("Error: Invalid value specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the entity properties and initial state.
    ENTITY_CIGI_DATA data = entity->GetAndLockCigi();
    data.anim_state = state;
    entity->SetLockedCigi(data);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialAnimState(state);

    return 0;
}

long ScriptDoEntityAttach(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long parent_id = -1;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &parent_id);

    CEntity *child = g_DataManager.GetEntity(entity_id);
    if (!child) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if ((parent_id != -1) && (!g_DataManager.GetEntity(parent_id))) {
        CString errmsg;
        errmsg.Format("Error: Invalid parent specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (entity_id == parent_id) {
        CString errmsg;
        errmsg.Format("Error: Cannot attach entity to itself. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    // Set the entity properties.
    ENTITY_CIGI_DATA data = child->GetAndLockCigi();
    data.parent_id = parent_id;
    child->SetLockedCigi(data);
    child->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to interact with the GUI.
    child->SetInitialParent(parent_id);

    // Now notify the main thread that it should update the view.
    MESSAGE_SCRIPT_SET_PARENT_IN_GUI msg;
    msg.entity_id = entity_id;
    msg.parent_id = parent_id;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityClampMode(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long mode = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &mode);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    ENTITY_CIGI_DATA data = entity->GetAndLockCigi();
    data.clamp_mode = mode;
    entity->SetLockedCigi(data);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to interact with the GUI.
    entity->SetInitialClampMode(mode);

    return 0;
}

long ScriptDoEntityClearWaypoints(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;

    sscanf(buffer, "%s %lu", keyword, &entity_id);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    entity->ClearWaypoints();

    return 0;
}

long ScriptDoEntityCdEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &enable);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (enable > 1) {
        CString errmsg;
        errmsg.Format("Error: Invalid value specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the entity properties.
    ENTITY_CIGI_DATA data = entity->GetAndLockCigi();
    data.coll_detect = enable;
    entity->SetLockedCigi(data);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to interact with the GUI.
    entity->SetInitialCollDetectEnable(enable);

    return 0;
}

long ScriptDoEntityDetach(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;

    sscanf(buffer, "%s %lu", keyword, &entity_id);

    CEntity *child = g_DataManager.GetEntity(entity_id);
    if (!child) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties.
    ENTITY_CIGI_DATA data = child->GetAndLockCigi();
    data.parent_id = -1;
    child->SetLockedCigi(data);
    child->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to interact with the GUI.
    child->SetInitialParent(-1);

    // Now notify the main thread that it should update the view.
    MESSAGE_SCRIPT_SET_PARENT_IN_GUI msg;
    msg.entity_id = entity_id;
    msg.parent_id = -1;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityFly(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;

    sscanf(buffer, "%s %lu", keyword, &entity_id);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties.
    ENTITY_NONCIGI_DATA data = entity->GetAndLockNonCigi();
    data.flymode = ENTITY_FLYMODE_FLY;
    entity->SetLockedNonCigi(data);
    entity->Unlock();

    return 0;
}

long ScriptDoEntityFlyWaypoints(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;

    sscanf(buffer, "%s %lu", keyword, &entity_id);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    entity->FlyWaypoints(TRUE);

    return 0;
}

long ScriptDoEntityInheritAlpha(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &enable);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (enable > 1) {
        CString errmsg;
        errmsg.Format("Error: Invalid value specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the entity properties.
    ENTITY_CIGI_DATA data = entity->GetAndLockCigi();
    data.inherit_alpha = enable;
    entity->SetLockedCigi(data);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to interact with the GUI.
    entity->SetInitialInheritAlpha(enable);

    return 0;
}

long ScriptDoEntityLatitude(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double lat = 0.0;

    sscanf(buffer, "%s %lu %lf", keyword, &entity_id, &lat);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.latitude = lat;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    return 0;
}

long ScriptDoEntityLongitude(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double lon = 0.0;

    sscanf(buffer, "%s %lu %lf", keyword, &entity_id, &lon);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.longitude = lon;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    return 0;
}

long ScriptDoEntityPitch(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double pitch = 0.0;

    sscanf(buffer, "%s %lu %lf", keyword, &entity_id, &pitch);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.pitch = pitch;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    return 0;
}

long ScriptDoEntityPitchOffset(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double pitch = 0.0;

    sscanf(buffer, "%s %lu %lf", keyword, &entity_id, &pitch);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.rel_pitch = pitch;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    return 0;
}

long ScriptDoEntityPitchRelative(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long ref_id = -1;
    double delta = 0.0;

    sscanf(buffer, "%s %lu %lu %lf", keyword, &entity_id, &ref_id, &delta);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CEntity *ref_entity = g_DataManager.GetEntity(ref_id);
    if (!ref_entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid reference entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the entity properties and initial state.
    DOF refDofs = ref_entity->GetDofs();
    DOF dof = entity->GetAndLockDofs();
    dof.pitch = refDofs.pitch + delta;

    // Check the bounds.
    if (dof.pitch > 90.0) {
        dof.pitch = 180.0 - dof.pitch;
        dof.yaw += 180.0;
    } else if (dof.pitch < -90.0) {
        dof.pitch = -180.0 - dof.pitch;
        dof.yaw += 180.0;
    }

    if (dof.yaw > 180.0f)
        dof.yaw -= 360.0f;
    else if (dof.yaw <= -180)
        dof.yaw += 360.0f;

    entity->SetLockedDofs(dof);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dof);

    return 0;
}

long ScriptDoEntityPos(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double lat = 0.0;
    double lon = 0.0;
    double alt = 0.0;

    sscanf(buffer, "%s %lu %lf %lf %lf", keyword, &entity_id, &lat, &lon, &alt);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.latitude = lat;
    dofs.longitude = lon;
    dofs.altitude = alt;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityPosRelative(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long ref_id = -1;
    double bearing = 0.0;
    double range = 0.0;

    sscanf(buffer, "%s %lu %lu %lf %lf", keyword, &entity_id, &ref_id, &bearing, &range);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CEntity *ref_entity = g_DataManager.GetEntity(ref_id);
    if (!ref_entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid reference entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the entity properties and initial state.
    double lat = 0.0;
    double lon = 0.0;
    DOF ref = ref_entity->GetDofs();
    GetRelativePos(ref.latitude, ref.longitude, ref.yaw, bearing, range, &lat, &lon);

    DOF dofs = entity->GetAndLockDofs();
    dofs.latitude = lat;
    dofs.longitude = lon;
    entity->SetLockedDofs(dofs, true);
    entity->Unlock();

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityRoll(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double roll = 0.0;

    sscanf(buffer, "%s %lu %lf", keyword, &entity_id, &roll);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.roll = roll;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityRollOffset(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double roll = 0.0;

    sscanf(buffer, "%s %lu %lf", keyword, &entity_id, &roll);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.rel_roll = roll;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityRollRelative(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long ref_id = -1;
    double delta = 0.0;

    sscanf(buffer, "%s %lu %lu %lf", keyword, &entity_id, &ref_id, &delta);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CEntity *ref_entity = g_DataManager.GetEntity(ref_id);
    if (!ref_entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid reference entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the entity properties and initial state.
    DOF refDofs = ref_entity->GetDofs();
    DOF dofs = entity->GetAndLockDofs();
    dofs.roll = refDofs.roll + delta;

    // Check the bounds.
    if (dofs.roll > 180.0)
        dofs.roll -= 360.0;
    else if (dofs.roll < -180.0)
        dofs.roll += 360.0;

    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntitySpeed(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double mps = 0.0;

    sscanf(buffer, "%s %lu %lf", keyword, &entity_id, &mps);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    ENTITY_NONCIGI_DATA data = entity->GetAndLockNonCigi();
    data.speed = mps;
    entity->SetLockedNonCigi(data);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialSpeed(mps);

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityState(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long state = -1;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &state);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (state > 2) {
        CString errmsg;
        errmsg.Format("Error: Invalid value specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // If the state is set to "Destroyed," delete the entity.
    if (state == 2) {
        // Send a message to the main thread so it will delete
        // everything from the GUI (we're not thread-safe).
        MESSAGE_SCRIPT_DEL_ENTITY_FROM_GUI msg;
        msg.entity_id = entity_id;
        msg.htree = (unsigned long)entity->GetHtree();
        msg.htree_env = (unsigned long)entity->GetHtreeEnv();
        msg.situation_hdl = (unsigned long)entity->GetSituationHandle();

        // If we're not deleting in the same frame we are creating, post
        // the "delete" message.  Otherwise, the object will have been
        // deleted, so the "create" message that is now queued will fail.
        if (msg.htree)
            PostGUIMsg(msg);

        // And delete the actual entity object.
        g_DataManager.DeleteEntity(entity);
    } else {
        // Set the entity properties and initial state.
        ENTITY_CIGI_DATA data = entity->GetAndLockCigi();
        data.active = state;
        entity->SetLockedCigi(data);
        entity->Unlock();

        // Set the initial value so the user doesn't have to re-run the entire
        // script to return to this point if he wants to go into free-flight.
        entity->SetInitialActiveState(state);
    }

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityXOffset(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double x = 0.0;

    sscanf(buffer, "%s %lu %lf", keyword, &entity_id, &x);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.offset_x = x;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityXYZOffset(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    sscanf(buffer, "%s %lu %lf %lf %lf", keyword, &entity_id, &x, &y, &z);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.offset_x = x;
    dofs.offset_y = y;
    dofs.offset_z = z;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityYOffset(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double y = 0.0;

    sscanf(buffer, "%s %lu %lf", keyword, &entity_id, &y);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.offset_y = y;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityYaw(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double yaw = 0.0;

    sscanf(buffer, "%s %lu %lf", keyword, &entity_id, &yaw);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.yaw = yaw;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityYawOffset(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double yaw = 0.0;

    sscanf(buffer, "%s %lu %lf", keyword, &entity_id, &yaw);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.rel_yaw = yaw;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityYawRelative(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long ref_id = -1;
    double delta = 0.0;

    sscanf(buffer, "%s %lu %lu %lf", keyword, &entity_id, &ref_id, &delta);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CEntity *ref_entity = g_DataManager.GetEntity(ref_id);
    if (!ref_entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid reference entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the entity properties and initial state.
    DOF refDof = ref_entity->GetDofs();
    DOF dof = entity->GetAndLockDofs();
    dof.yaw = refDof.yaw + delta;

    // Check the bounds.
    if (dof.yaw > 180.0)
        dof.yaw -= 360.0;
    else if (dof.yaw <= -180.0)
        dof.yaw += 360.0;

    entity->SetLockedDofs(dof);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dof);

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityYPR(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double yaw = 0.0;
    double pitch = 0.0;
    double roll = 0.0;

    sscanf(buffer, "%s %lu %lf %lf %lf", keyword, &entity_id, &yaw, &pitch, &roll);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.yaw = yaw;
    dofs.pitch = pitch;
    dofs.roll = roll;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityYPROffset(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double yaw = 0.0;
    double pitch = 0.0;
    double roll = 0.0;

    sscanf(buffer, "%s %lu %lf %lf %lf", keyword, &entity_id, &yaw, &pitch, &roll);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.rel_yaw = yaw;
    dofs.rel_pitch = pitch;
    dofs.rel_roll = roll;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityZOffset(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    double z = 0.0;

    sscanf(buffer, "%s %lu %lf", keyword, &entity_id, &z);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the entity properties and initial state.
    DOF dofs = entity->GetAndLockDofs();
    dofs.offset_z = z;
    entity->SetLockedDofs(dofs);
    entity->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this point if he wants to go into free-flight.
    entity->SetInitialDofs(dofs);

    MESSAGE_UPDATE_ENTITIES_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}
