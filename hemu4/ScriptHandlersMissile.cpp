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
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "hemumsg.h"

extern void ScriptPostTextMessage(const char *text);

long ScriptDoMissileFire(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long missile_id = -1;

    sscanf(buffer, "%s %lu", keyword, &missile_id);

    CEntity *pMissile = g_DataManager.GetEntity(missile_id);
    if (!pMissile) {
        CString errmsg;
        errmsg.Format("Warning: Entity %d not found. (line %lu)", missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Make sure the entity is a missile.
    if (pMissile->GetClass() != ENTITY_CLASS_MISSILE) {
        CString errmsg;
        errmsg.Format("Warning: Entity %d is not a missile. (line %lu)", missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Make sure the missile has a valid target.
    if (!g_DataManager.GetEntity(pMissile->GetTarget())) {
        CString errmsg;
        errmsg.Format("Warning: Missile %d does not have a valid target. (line %lu)", missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_SCRIPT_FIRE_MISSILE msg;
    msg.pMissile = pMissile;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoMissileHitEffect(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long missile_id = -1;
    unsigned long effect_id = -1;

    sscanf(buffer, "%s %lu %lu", keyword, &missile_id, &effect_id);

    CEntity *pMissile = g_DataManager.GetEntity(missile_id);
    if (!pMissile) {
        CString errmsg;
        errmsg.Format("Warning: Entity %d not found. (line %lu)", missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Make sure the entity is a missile.
    if (pMissile->GetClass() != ENTITY_CLASS_MISSILE) {
        CString errmsg;
        errmsg.Format("Warning: Entity %d is not a missile. (line %lu)", missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Make sure a valid effect was specified.
    CEntity *pEffect = g_DataManager.GetEntity(effect_id);
    if (!pEffect) {
        CString errmsg;
        errmsg.Format("Warning: Invalid animation entity %d specified for missile %d. (line %lu)", effect_id, missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    if (pEffect->GetClass() != ENTITY_CLASS_ANIMATION) {
        CString errmsg;
        errmsg.Format("Warning: Entity %d is not an animation. (line %lu)", effect_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -4;
    }

    pMissile->GetDetonationAnimList().AddTail(effect_id);

    return 0;
}

long ScriptDoMissilePersistentLaunchEffect(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long missile_id = -1;
    unsigned long effect_id = -1;

    sscanf(buffer, "%s %lu %lu", keyword, &missile_id, &effect_id);

    CEntity *pMissile = g_DataManager.GetEntity(missile_id);
    if (!pMissile) {
        CString errmsg;
        errmsg.Format("Warning: Entity %d not found. (line %lu)", missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Make sure the entity is a missile.
    if (pMissile->GetClass() != ENTITY_CLASS_MISSILE) {
        CString errmsg;
        errmsg.Format("Warning: Entity %d is not a missile. (line %lu)", missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Make sure a valid effect was specified.
    CEntity *pEffect = g_DataManager.GetEntity(effect_id);
    if (!pEffect) {
        CString errmsg;
        errmsg.Format("Warning: Invalid animation entity %d specified for missile %d. (line %lu)", effect_id, missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    if (pEffect->GetClass() != ENTITY_CLASS_ANIMATION) {
        CString errmsg;
        errmsg.Format("Warning: Entity %d is not an animation. (line %lu)", effect_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -4;
    }

    pMissile->GetPersistentLaunchAnimList().AddTail(effect_id);

    return 0;
}

long ScriptDoMissileTerminatingLaunchEffect(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long missile_id = -1;
    unsigned long effect_id = -1;

    sscanf(buffer, "%s %lu %lu", keyword, &missile_id, &effect_id);

    CEntity *pMissile = g_DataManager.GetEntity(missile_id);
    if (!pMissile) {
        CString errmsg;
        errmsg.Format("Warning: Entity %d not found. (line %lu)", missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Make sure the entity is a missile.
    if (pMissile->GetClass() != ENTITY_CLASS_MISSILE) {
        CString errmsg;
        errmsg.Format("Warning: Entity %d is not a missile. (line %lu)", missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Make sure a valid effect was specified.
    CEntity *pEffect = g_DataManager.GetEntity(effect_id);
    if (!pEffect) {
        CString errmsg;
        errmsg.Format("Warning: Invalid animation entity %d specified for missile %d. (line %lu)", effect_id, missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    if (pEffect->GetClass() != ENTITY_CLASS_ANIMATION) {
        CString errmsg;
        errmsg.Format("Warning: Entity %d is not an animation. (line %lu)", effect_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -4;
    }

    pMissile->GetLaunchAnimList().AddTail(effect_id);

    return 0;
}

long ScriptDoMissileTarget(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long missile_id = -1;
    unsigned long target_id = -1;

    sscanf(buffer, "%s %lu %lu", keyword, &missile_id, &target_id);

    CEntity *pMissile = g_DataManager.GetEntity(missile_id);
    if (!pMissile) {
        CString errmsg;
        errmsg.Format("Warning: Entity %d not found. (line %lu)", missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Make sure the entity is a missile.
    if (pMissile->GetClass() != ENTITY_CLASS_MISSILE) {
        CString errmsg;
        errmsg.Format("Warning: Entity %d is not a missile. (line %lu)", missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Make sure a valid target was specified.
    CEntity *pTarget = g_DataManager.GetEntity(target_id);
    if (!pTarget) {
        CString errmsg;
        errmsg.Format("Warning: Invalid target %d specified for missile %d. (line %lu)", target_id, missile_id, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    pMissile->SetTarget(target_id, FALSE);

    PostGUIMsg(MESSAGE_SCRIPT_UPDATE_MISSILE_TARGET_COMBO());

    return 0;
}
