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
#include "globals.h"

extern void ScriptPostTextMessage(const char *text);

// HAT / HOT Request

bool in_hat_hot = false;

CIGI_HAT_HOT_REQUEST HatHotReq =    {   CIGI_HAT_HOT_REQUEST_OPCODE,
                                        CIGI_HAT_HOT_REQUEST_SIZE,
                                        0, 0, 0, 0, 0,
                                        0.0, 0.0, 0.0
                                    };

long ScriptDoHatHotReqBegin(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned int hat_hot_id = 0;

    sscanf(buffer, "%s %lu", keyword, &hat_hot_id);

    if (in_hat_hot) {
        CString errmsg;
        errmsg.Format("Error: Must call HAT_HOT_REQ_END before starting a new HAT/HOT segment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Clear out any old data.
    memset(&HatHotReq, 0, sizeof(HatHotReq));
    HatHotReq.packet_id = CIGI_HAT_HOT_REQUEST_OPCODE;
    HatHotReq.packet_size = CIGI_HAT_HOT_REQUEST_SIZE;
    HatHotReq.hat_hot_id = hat_hot_id;

    in_hat_hot = true;

    return 0;
}

long ScriptDoHatHotReqEnd(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);

    if (!in_hat_hot) {
        CString errmsg;
        errmsg.Format("Error: HAT_HOT_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Send the HAT/HOT Request packet.
    SendImmedCigiMessage(&HatHotReq);

    in_hat_hot = false;

    return 0;
}

long ScriptDoHatHotReqType(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char type[32] = "";

    sscanf(buffer, "%s %s", keyword, &type);

    if (!in_hat_hot) {
        CString errmsg;
        errmsg.Format("Error: HAT_HOT_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (strcmp(type, "HAT") == 0) {
        HatHotReq.request_type = 0;
    } else if (strcmp(type, "HOT") == 0) {
        HatHotReq.request_type = 1;
    } else if (strcmp(type, "EXTENDED") == 0) {
        HatHotReq.request_type = 2;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid HAT/HOT request type. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoHatHotReqSource(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char type[32] = "";
    int entity_id = 0;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    sscanf(buffer, "%s %s %lf %lf %lf %d", keyword, &type, &x, &y, &z, &entity_id);

    if (!in_hat_hot) {
        CString errmsg;
        errmsg.Format("Error: HAT_HOT_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (strcmp(type, "GEODETIC") == 0) {
        HatHotReq.coord_system = 0;
    } else if (strcmp(type, "ENTITY") == 0) {
        HatHotReq.coord_system = 1;
        HatHotReq.entity_id = entity_id;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid HAT/HOT Coordinate System. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    HatHotReq.lat_x = x;
    HatHotReq.lon_y = y;
    HatHotReq.alt_z = z;

    return 0;
}

long ScriptDoHatHotReqUpdatePeriod(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned int period = 0;

    sscanf(buffer, "%s %u", keyword, &period);

    if (!in_hat_hot) {
        CString errmsg;
        errmsg.Format("Error: HAT_HOT_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    HatHotReq.update_period = period;

    return 0;
}
