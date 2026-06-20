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
#include "Waypoint.h"

extern void ScriptPostTextMessage(const char *text);

long ScriptDoWaypointAddRelative(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long waypoint_id = -1;
    double az = 0.0;
    double el = 0.0;
    double range = 0.0;

    sscanf(buffer, "%s %lu %lf %lf %lf", keyword, &waypoint_id, &az, &el, &range);

    CWaypoint *wp = new CWaypoint;
    if (!wp) {
        CString errmsg;
        errmsg.Format("Error: Not enough memory to create waypoint. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    wp->SetID(waypoint_id);
    wp->SetType(WAYPOINT_TYPE_RELATIVE);
    wp->SetAzimuth(az);
    wp->SetElevation(el);
    wp->SetRange(range);

    g_DataManager.AddWaypoint(wp);

    return 0;
}

long ScriptDoWaypointAddAbsolute(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long waypoint_id = -1;
    double lat = 0.0;
    double lon = 0.0;
    double alt = 0.0;

    sscanf(buffer, "%s %lu %lf %lf %lf", keyword, &waypoint_id, &lat, &lon, &alt);

    CWaypoint *wp = new CWaypoint;
    if (!wp) {
        CString errmsg;
        errmsg.Format("Error: Not enough memory to create waypoint. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    wp->SetID(waypoint_id);
    wp->SetType(WAYPOINT_TYPE_ABSOLUTE);
    wp->SetLatitude(lat);
    wp->SetLongitude(lon);
    wp->SetAltitude(alt);

    g_DataManager.AddWaypoint(wp);

    return 0;
}

long ScriptDoWaypointClearAll(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);

    g_DataManager.ClearWaypoints();

    return 0;
}
