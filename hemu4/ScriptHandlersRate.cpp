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

extern void ScriptPostTextMessage(const char *text);

long ScriptDoAngularRate(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    short part_id = -1;
    float yaw_rate = 0.0f;
    float pitch_rate = 0.0f;
    float roll_rate = 0.0f;
    float x_rate = 0.0f;
    float y_rate = 0.0f;
    float z_rate = 0.0f;

    sscanf(buffer, "%s %lu %hd %f %f %f",
           keyword, &entity_id, &part_id, &yaw_rate, &pitch_rate, &roll_rate);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (part_id == -1) {
        // Set the new entity rates.
        RATES rates = entity->GetAndLockRates();
        rates.dyaw = yaw_rate;
        rates.dpitch = pitch_rate;
        rates.droll = roll_rate;
        entity->SetLockedRates(rates);
        entity->Unlock();

        // Get the existing linear rates so we don't overwrite.
        x_rate = (float)rates.dx;
        y_rate = (float)rates.dy;
        z_rate = (float)rates.dz;
    } else {
        CArtPart *artpart = entity->GetArtPart(part_id);
        if (!artpart) {
            CString errmsg;
            errmsg.Format("Error: Invalid articulated part specified. (line %lu)", linenum);
            ScriptPostTextMessage((LPCTSTR)errmsg);
            return -2;
        }

        // Set the new angular rates.
        artpart->SetYawRate(yaw_rate);
        artpart->SetPitchRate(pitch_rate);
        artpart->SetRollRate(roll_rate);

        // Get the existing linear rates so we don't overwrite.
        x_rate = artpart->GetXRate();
        y_rate = artpart->GetYRate();
        z_rate = artpart->GetZRate();
    }

    // Queue a Rate Control packet.
    CIGI_VELOCITY_CONTROL rc;
    rc.packet_id = CIGI_VELOCITY_CONTROL_OPCODE;
    rc.packet_size = (uint8_t)sizeof(rc);
    rc.entity_id = (uint16_t)entity_id;
    rc.artpart_id = (uint8_t)part_id;
    rc.apply_artpart = (part_id > 0) ? 1 : 0;
    rc.x_rate = x_rate;
    rc.y_rate = y_rate;
    rc.z_rate = z_rate;
    rc.yaw_rate = yaw_rate;
    rc.pitch_rate = pitch_rate;
    rc.roll_rate = roll_rate;

    SendImmedCigiMessage(&rc);

    return 0;
}

long ScriptDoLinearRate(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    short part_id = -1;
    float yaw_rate = 0.0f;
    float pitch_rate = 0.0f;
    float roll_rate = 0.0f;
    float x_rate = 0.0f;
    float y_rate = 0.0f;
    float z_rate = 0.0f;

    sscanf(buffer, "%s %lu %hd %f %f %f",
           keyword, &entity_id, &part_id, &x_rate, &y_rate, &z_rate);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (part_id == -1) {
        // Set the new entity rates.
        RATES rates = entity->GetAndLockRates();
        rates.dx = x_rate;
        rates.dy = y_rate;
        rates.dz = z_rate;
        entity->SetLockedRates(rates);
        entity->Unlock();

        // Get the existing angular rates so we don't overwrite.
        yaw_rate = (float)rates.dyaw;
        pitch_rate = (float)rates.dpitch;
        roll_rate = (float)rates.droll;
    } else {
        CArtPart *artpart = entity->GetArtPart(part_id);
        if (!artpart) {
            CString errmsg;
            errmsg.Format("Error: Invalid articulated part specified. (line %lu)", linenum);
            ScriptPostTextMessage((LPCTSTR)errmsg);
            return -2;
        }

        // Set the new linear rates.
        artpart->SetXRate(x_rate);
        artpart->SetYRate(y_rate);
        artpart->SetZRate(z_rate);

        // Get the existing angular rates so we don't overwrite.
        yaw_rate = artpart->GetYawRate();
        pitch_rate = artpart->GetPitchRate();
        roll_rate = artpart->GetRollRate();
    }

    // Queue a Rate Control packet.
    CIGI_VELOCITY_CONTROL rc;
    rc.packet_id = CIGI_VELOCITY_CONTROL_OPCODE;
    rc.packet_size = sizeof(rc);
    rc.entity_id = (uint16_t)entity_id;
    rc.artpart_id = (uint8_t)part_id;
    rc.apply_artpart = (part_id > 0) ? 1 : 0;
    rc.x_rate = x_rate;
    rc.y_rate = y_rate;
    rc.z_rate = z_rate;
    rc.yaw_rate = yaw_rate;
    rc.pitch_rate = pitch_rate;
    rc.roll_rate = roll_rate;

    SendImmedCigiMessage(&rc);

    return 0;
}
