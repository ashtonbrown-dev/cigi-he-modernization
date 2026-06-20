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

bool in_los_segment = false;
bool in_los_vector = false;
CIGI_LOS_SEGMENT_REQUEST los_segment =  {   CIGI_LOS_SEGMENT_REQUEST_SIZE, CIGI_LOS_SEGMENT_REQUEST_OPCODE,
                                            //0, 0, 0, 0, 0, 0, 0, 0,
                                            //0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                            //0, 0, 0
                                        };
CIGI_LOS_VECTOR_REQUEST los_vector =    {   CIGI_LOS_VECTOR_REQUEST_SIZE, CIGI_LOS_VECTOR_REQUEST_OPCODE,
                                            //0, 0, 0, 0, 0, 0,
                                            //0.0f, 0.0f, 0.0f, 0.0f,
                                            //0.0, 0.0, 0.0,
                                            //0
                                        };

long ScriptDoLOSSegmentReqBegin(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned short los_id = 0;

    sscanf(buffer, "%s %hu", keyword, &los_id);
    if (in_los_segment) {
        CString errmsg;
        errmsg.Format("Error: Must call LOS_SEGMENT_REQ_END before starting a new LOS segment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Clear out any old data and set defaults.
    memset(&los_segment, 0, sizeof(los_segment));
    los_segment.packet_id = CIGI_LOS_SEGMENT_REQUEST_OPCODE;
    los_segment.packet_size = CIGI_LOS_SEGMENT_REQUEST_SIZE;
    los_segment.los_id = los_id;
    los_segment.material_mask = 0xFFFFFFFF;

    in_los_segment = true;

    return 0;
}

long ScriptDoLOSSegmentReqEnd(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (!in_los_segment) {
        CString errmsg;
        errmsg.Format("Error: LOS_SEGMENT_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Send the LOS Segment Request packet.
    SendImmedCigiMessage(&los_segment);

    in_los_segment = false;

    return 0;
}

long ScriptDoLOSSegmentReqType(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char type[32] = "";

    sscanf(buffer, "%s %s", keyword, &type);
    if (!in_los_segment) {
        CString errmsg;
        errmsg.Format("Error: LOS_SEGMENT_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    if (strcmp(type, "BASIC") == 0) {
        los_segment.request_type = 0;
    } else if (strcmp(type, "EXTENDED") == 0) {
        los_segment.request_type = 1;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid LOS Segment request type. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoLOSSegmentReqSource(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char type[32] = "";
    int entity_id = 0;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    sscanf(buffer, "%s %s %lf %lf %lf %d", keyword, &type, &x, &y, &z, &entity_id);
    if (!in_los_segment) {
        CString errmsg;
        errmsg.Format("Error: LOS_SEGMENT_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    if (strcmp(type, "GEODETIC") == 0) {
        los_segment.source_coord_system = 0;
    } else if (strcmp(type, "ENTITY") == 0) {
        los_segment.source_coord_system = 1;
// chas         los_segment.entity_id = entity_id;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid coordinate system specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    los_segment.source_lat_x = x;
    los_segment.source_lon_y = y;
    los_segment.source_alt_z = z;

    return 0;
}

long ScriptDoLOSSegmentReqDestination(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char type[32] = "";
    int entity_id = 0;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    sscanf(buffer, "%s %s %lf %lf %lf %d", keyword, &type, &x, &y, &z, &entity_id);
    if (!in_los_segment) {
        CString errmsg;
        errmsg.Format("Error: LOS_SEGMENT_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    if (strcmp(type, "GEODETIC") == 0) {
        los_segment.dest_coord_system = 0;
    } else if (strcmp(type, "ENTITY") == 0) {
        los_segment.dest_coord_system = 1;
        los_segment.dest_entity_id_valid = 1;
        los_segment.dest_entity_id = entity_id;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid coordinate system specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    los_segment.dest_lat_x = x;
    los_segment.dest_lon_y = y;
    los_segment.dest_alt_z = z;

    return 0;
}

long ScriptDoLOSSegmentReqRespCoordSys(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char type[32] = "";

    sscanf(buffer, "%s %s", keyword, &type);
    if (!in_los_segment) {
        CString errmsg;
        errmsg.Format("Error: LOS_SEGMENT_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    if (strcmp(type, "GEODETIC") == 0) {
        los_segment.response_coord_system = 0;
    } else if (strcmp(type, "ENTITY") == 0) {
        los_segment.response_coord_system = 1;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid coordinate system specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoLOSSegmentReqMaterialMask(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int mask = 0;

    sscanf(buffer, "%s %x", keyword, &mask);
    if (!in_los_segment) {
        CString errmsg;
        errmsg.Format("Error: LOS_SEGMENT_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    los_segment.material_mask = mask;

    return 0;
}

long ScriptDoLOSSegmentReqUpdatePeriod(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned int period = 0;

    sscanf(buffer, "%s %u", keyword, &period);
    if (!in_los_segment) {
        CString errmsg;
        errmsg.Format("Error: LOS_SEGMENT_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    los_segment.update_period = period;

    return 0;
}

long ScriptDoLOSVectorReqBegin(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned short los_id = 0;

    sscanf(buffer, "%s %hu", keyword, &los_id);
    if (in_los_vector) {
        CString errmsg;
        errmsg.Format("Error: Must call LOS_VECTOR_REQ_END before starting a new LOS vector. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Clear out any old data.
    memset(&los_vector, 0, sizeof(los_vector));
    los_vector.packet_id = CIGI_LOS_VECTOR_REQUEST_OPCODE;
    los_vector.packet_size = CIGI_LOS_VECTOR_REQUEST_SIZE;
    los_vector.los_id = los_id;
    los_vector.material_mask = 0xFFFFFFFF;

    in_los_vector = true;

    return 0;
}

long ScriptDoLOSVectorReqEnd(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (!in_los_vector) {
        CString errmsg;
        errmsg.Format("Error: LOS_VECTOR_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Send the LOS Vector Request packet.
    SendImmedCigiMessage(&los_vector);

    in_los_vector = false;

    return 0;
}

long ScriptDoLOSVectorReqType(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char type[32] = "";

    sscanf(buffer, "%s %s", keyword, &type);
    if (!in_los_vector) {
        CString errmsg;
        errmsg.Format("Error: LOS_VECTOR_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    if (strcmp(type, "BASIC") == 0) {
        los_vector.request_type = 0;
    } else if (strcmp(type, "EXTENDED") == 0) {
        los_vector.request_type = 1;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid LOS Vector request type. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoLOSVectorReqSource(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char type[32] = "";
    int entity_id = 0;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    sscanf(buffer, "%s %s %lf %lf %lf %d", keyword, &type, &x, &y, &z, &entity_id);
    if (!in_los_vector) {
        CString errmsg;
        errmsg.Format("Error: LOS_VECTOR_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    if (strcmp(type, "GEODETIC") == 0) {
        los_vector.source_coord_system = 0;
    } else if (strcmp(type, "ENTITY") == 0) {
        los_vector.source_coord_system = 1;
        los_vector.entity_id = entity_id;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid coordinate system specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    los_vector.source_lat_x = x;
    los_vector.source_lon_y = y;
    los_vector.source_alt_z = z;

    return 0;
}

long ScriptDoLOSVectorReqAzEl(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float az = 0.0;
    float el = 0.0;

    sscanf(buffer, "%s %f %f", keyword, &az, &el);
    if (!in_los_vector) {
        CString errmsg;
        errmsg.Format("Error: LOS_VECTOR_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    los_vector.azimuth = az;
    los_vector.elevation = el;

    return 0;
}

long ScriptDoLOSVectorReqMinMaxRange(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float min = 0.0;
    float max = 0.0;

    sscanf(buffer, "%s %f %f", keyword, &min, &max);
    if (!in_los_vector) {
        CString errmsg;
        errmsg.Format("Error: LOS_VECTOR_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    los_vector.min_range = min;
    los_vector.max_range = max;

    return 0;
}

long ScriptDoLOSVectorReqRespCoordSys(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char type[32] = "";

    sscanf(buffer, "%s %s", keyword, &type);
    if (!in_los_vector) {
        CString errmsg;
        errmsg.Format("Error: LOS_VECTOR_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    if (strcmp(type, "GEODETIC") == 0) {
        los_vector.resp_coord_system = 0;
    } else if (strcmp(type, "ENTITY") == 0) {
        los_vector.resp_coord_system = 1;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid coordinate system specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoLOSVectorReqMaterialMask(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long mask = 0;

    sscanf(buffer, "%s %lu", keyword, &mask);
    if (!in_los_vector) {
        CString errmsg;
        errmsg.Format("Error: LOS_VECTOR_REQ_BEGIN was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    los_vector.material_mask = mask;

    return 0;
}

long ScriptDoLOSVectorReqAlphaThresh(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned short alpha = 0;

    sscanf(buffer, "%s %hu", keyword, &alpha);
    if (!in_los_vector) {
        CString errmsg;
        errmsg.Format("Error: LOS_VECTOR_REQ_BEGIN was not called. (line %lh)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    los_vector.alpha_threshold = (uint8_t)alpha;

    return 0;
}

long ScriptDoLOSVectorReqUpdatePeriod(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    short period = 0;

    sscanf(buffer, "%s %hd", keyword, &period);
    if (!in_los_vector) {
        CString errmsg;
        errmsg.Format("Error: LOS_VECTOR_REQ_BEGIN was not called. (line %ld)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

// chas    los_vector.update_period = period;

    return 0;
}
