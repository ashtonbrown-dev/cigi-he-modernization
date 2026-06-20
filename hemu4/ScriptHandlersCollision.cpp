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
#include "CollisionSeg.h"
#include "CollisionVol.h"
#include "Entity.h"
#include "globals.h"
#include "hemumsg.h"

extern void ScriptPostTextMessage(const char *text);

long ScriptDoCDSegment(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long segment_id = -1;
    int mask = -1;
    float x1 = 0.0;
    float y1 = 0.0;
    float z1 = 0.0;
    float x2 = 0.0;
    float y2 = 0.0;
    float z2 = 0.0;

    sscanf(buffer, "%s %lu %lu %f %f %f %f %f %f %x",
           keyword, &entity_id, &segment_id, &x1, &y1, &z1, &x2, &y2, &z2, &mask);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CCollisionSeg *seg = entity->GetCollisionSeg(segment_id);
    if (!seg) {
        seg = new CCollisionSeg(segment_id, entity_id);
        entity->AddCollisionSeg(seg);
    }

    seg->SetEnable(1);      // Enabled by default    LWD: note this in documentation
    seg->SetStartX(x1);
    seg->SetStartY(y1);
    seg->SetStartZ(z1);
    seg->SetEndX(x2);
    seg->SetEndY(y2);
    seg->SetEndZ(z2);
    seg->SetMaterialMask(mask);

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoCDSegmentEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long segment_id = -1;
    unsigned long enable = 1;

    sscanf(buffer, "%s %lu %lu %lu", keyword, &entity_id, &segment_id, &enable);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CCollisionSeg *seg = entity->GetCollisionSeg(segment_id);
    if (!seg) {
        CString errmsg;
        errmsg.Format("Error: Invalid collision detection segment specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    seg->SetEnable(enable);

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoCDVolumeCuboid(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long volume_id = -1;
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    float height = 0.0;
    float width = 0.0;
    float depth = 0.0;
    float yaw = 0.0;
    float pitch = 0.0;
    float roll = 0.0;

    sscanf(buffer, "%s %lu %lu %f %f %f %f %f %f %f %f %f",
           keyword, &entity_id, &volume_id, &x, &y, &z,
           &height, &width, &depth, &yaw, &pitch, &roll);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CCollisionVol *vol = entity->GetCollisionVol(volume_id);
    if (!vol) {
        vol = new CCollisionVol(volume_id, entity_id);
        vol->SetType(COLLISION_VOLUME_CUBOID);
        entity->AddCollisionVol(vol);
    }

    if (vol->GetType() != COLLISION_VOLUME_CUBOID) {
        CString errmsg;
        errmsg.Format("Error: Specified collision detection volume is not a cuboid. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    vol->SetEnable(1);      // Enabled by default    LWD: note this in documentation
    vol->SetX(x);
    vol->SetY(y);
    vol->SetZ(z);
    vol->SetHeightOrRadius(height);
    vol->SetWidth(width);
    vol->SetDepth(depth);
    vol->SetYaw(yaw);
    vol->SetPitch(pitch);
    vol->SetRoll(roll);

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoCDVolumeSphere(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long volume_id = -1;
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    float radius = 0.0;

    sscanf(buffer, "%s %lu %lu %f %f %f %f",
           keyword, &entity_id, &volume_id, &x, &y, &z, &radius);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CCollisionVol *vol = entity->GetCollisionVol(volume_id);
    if (!vol) {
        vol = new CCollisionVol(volume_id, entity_id);
        vol->SetType(COLLISION_VOLUME_SPHERE);
        entity->AddCollisionVol(vol);
    }

    if (vol->GetType() != COLLISION_VOLUME_SPHERE) {
        CString errmsg;
        errmsg.Format("Error: Specified collision detection volume is not a sphere. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    vol->SetEnable(1);      // Enabled by default    LWD: note this in documentation
    vol->SetX(x);
    vol->SetY(y);
    vol->SetZ(z);
    vol->SetHeightOrRadius(radius);

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoCDVolumeEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long volume_id = -1;
    unsigned long enable = 1;

    sscanf(buffer, "%s %lu %lu %lu", keyword, &entity_id, &volume_id, &enable);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CCollisionSeg *vol = entity->GetCollisionSeg(volume_id);
    if (!vol) {
        CString errmsg;
        errmsg.Format("Error: Invalid collision detection volume specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    vol->SetEnable(enable);

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}
