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
#include "ArtPart.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "hemumsg.h"

extern void ScriptPostTextMessage(const char *text);

long ScriptDoArtPartEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long artpart_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu %lu", keyword, &entity_id, &artpart_id, &enable);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CArtPart *artpart = entity->GetArtPart(artpart_id);
    if (!artpart) {
        CString errmsg;
        errmsg.Format("Error: Invalid articulated part specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    artpart->SetEnableFlag(enable);

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoArtPartEnableXYZ(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long artpart_id = -1;
    unsigned long x_enable = 0;
    unsigned long y_enable = 0;
    unsigned long z_enable = 0;

    sscanf(buffer, "%s %lu %lu %lu %lu %lu", keyword, &entity_id, &artpart_id,
           &x_enable, &y_enable, &z_enable);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CArtPart *artpart = entity->GetArtPart(artpart_id);
    if (!artpart) {
        CString errmsg;
        errmsg.Format("Error: Invalid articulated part specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    artpart->SetXEnable(x_enable);
    artpart->SetYEnable(y_enable);
    artpart->SetZEnable(z_enable);

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoArtPartEnableYPR(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long artpart_id = -1;
    unsigned long yaw_enable = 0;
    unsigned long pitch_enable = 0;
    unsigned long roll_enable = 0;

    sscanf(buffer, "%s %lu %lu %lu %lu %lu", keyword, &entity_id, &artpart_id,
           &yaw_enable, &pitch_enable, &roll_enable);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CArtPart *artpart = entity->GetArtPart(artpart_id);
    if (!artpart) {
        CString errmsg;
        errmsg.Format("Error: Invalid articulated part specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    artpart->SetYawEnable(yaw_enable);
    artpart->SetPitchEnable(pitch_enable);
    artpart->SetRollEnable(roll_enable);

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoArtPartPitch(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long artpart_id = -1;
    float pitch = 0.0;

    sscanf(buffer, "%s %lu %lu %f", keyword, &entity_id, &artpart_id,
           &pitch);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CArtPart *artpart = entity->GetArtPart(artpart_id);
    if (!artpart) {
        CString errmsg;
        errmsg.Format("Error: Invalid articulated part specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    artpart->SetPitch(pitch);

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoArtPartRoll(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long artpart_id = -1;
    float roll = 0.0;

    sscanf(buffer, "%s %lu %lu %f", keyword, &entity_id, &artpart_id,
           &roll);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CArtPart *artpart = entity->GetArtPart(artpart_id);
    if (!artpart) {
        CString errmsg;
        errmsg.Format("Error: Invalid articulated part specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    artpart->SetRoll(roll);

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoArtPartX(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long artpart_id = -1;
    float x = 0.0;

    sscanf(buffer, "%s %lu %lu %f", keyword, &entity_id, &artpart_id,
           &x);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CArtPart *artpart = entity->GetArtPart(artpart_id);
    if (!artpart) {
        CString errmsg;
        errmsg.Format("Error: Invalid articulated part specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the DOF, but keep the enable flag.
    artpart->SetXOffset(x, artpart->GetXEnable());

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoArtPartXYZ(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long artpart_id = -1;
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;

    sscanf(buffer, "%s %lu %lu %f %f %f", keyword, &entity_id, &artpart_id,
           &x, &y, &z);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CArtPart *artpart = entity->GetArtPart(artpart_id);
    if (!artpart) {
        CString errmsg;
        errmsg.Format("Error: Invalid articulated part specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the DOFs, but keep the enable flags.
    artpart->SetXOffset(x, artpart->GetXEnable());
    artpart->SetYOffset(y, artpart->GetYEnable());
    artpart->SetZOffset(z, artpart->GetZEnable());

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoArtPartY(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long artpart_id = -1;
    float y = 0.0;

    sscanf(buffer, "%s %lu %lu %f", keyword, &entity_id, &artpart_id, &y);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CArtPart *artpart = entity->GetArtPart(artpart_id);
    if (!artpart) {
        CString errmsg;
        errmsg.Format("Error: Invalid articulated part specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the DOF, but keep the enable flag.
    artpart->SetYOffset(y, artpart->GetYEnable());

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoArtPartYaw(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long artpart_id = -1;
    float yaw = 0.0;

    sscanf(buffer, "%s %lu %lu %f", keyword, &entity_id, &artpart_id, &yaw);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CArtPart *artpart = entity->GetArtPart(artpart_id);
    if (!artpart) {
        CString errmsg;
        errmsg.Format("Error: Invalid articulated part specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the DOF, but keep the enable flag.
    artpart->SetYaw(yaw, artpart->GetYawEnable());

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoArtPartYPR(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long artpart_id = -1;
    float yaw = 0.0;
    float pitch = 0.0;
    float roll = 0.0;

    sscanf(buffer, "%s %lu %lu %f %f %f",
           keyword, &entity_id, &artpart_id, &yaw, &pitch, &roll);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CArtPart *artpart = entity->GetArtPart(artpart_id);
    if (!artpart) {
        CString errmsg;
        errmsg.Format("Error: Invalid articulated part specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the DOFs, but keep the enable flags.
    artpart->SetYaw(yaw, artpart->GetYawEnable());
    artpart->SetPitch(pitch, artpart->GetPitchEnable());
    artpart->SetRoll(roll, artpart->GetRollEnable());

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoArtPartZ(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long artpart_id = -1;
    float z = 0.0;

    sscanf(buffer, "%s %lu %lu %f", keyword, &entity_id, &artpart_id, &z);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CArtPart *artpart = entity->GetArtPart(artpart_id);
    if (!artpart) {
        CString errmsg;
        errmsg.Format("Error: Invalid articulated part specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Set the DOF, but keep the enable flag.
    artpart->SetZOffset(z, artpart->GetZEnable());

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoAtmModelEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu", keyword, &enable);

    // Update the environment data.
    g_DataManager.GetGlobalEnv()->SetAtmosphericsEnable(enable);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}
