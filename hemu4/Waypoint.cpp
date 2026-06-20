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

// Waypoint.cpp: implementation of the CWaypoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "common.h"
#include "hemu4.h"
#include "Waypoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CWaypoint, CObject, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaypoint::CWaypoint()
{
    memset(&m_Data, 0, sizeof(m_Data));
}

CWaypoint::CWaypoint(const CWaypoint &waypoint)
{
    memcpy(&m_Data, &(waypoint.m_Data), sizeof(m_Data));
}

CWaypoint::~CWaypoint()
{
}

void CWaypoint::Serialize(CArchive &ar)
{
    TRACE0("\nCWaypoint::Serialize()\n");

    int length = 0;

    if (ar.IsStoring()) {
        ar << m_Data.id;
        ar << m_Data.type;
        ar << m_Data.ref_entity;
        ar << m_Data.time;
        ar << m_Data.airspeed;
        ar << m_Data.accel;
        ar << m_Data.turn_rate;
        ar << m_Data.azimuth;
        ar << m_Data.range;
        ar << m_Data.elevation;
        ar << m_Data.lat;
        ar << m_Data.lon;
        ar << m_Data.alt;
        ar << m_Data.x;
        ar << m_Data.y;
        ar << m_Data.z;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
    } else {
        ar >> m_Data.id;
        ar >> m_Data.type;
        ar >> m_Data.ref_entity;
        ar >> m_Data.time;
        ar >> m_Data.airspeed;
        ar >> m_Data.accel;
        ar >> m_Data.turn_rate;
        ar >> m_Data.azimuth;
        ar >> m_Data.range;
        ar >> m_Data.elevation;
        ar >> m_Data.lat;
        ar >> m_Data.lon;
        ar >> m_Data.alt;
        ar >> m_Data.x;
        ar >> m_Data.y;
        ar >> m_Data.z;

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
    }
}

CWaypoint &CWaypoint::operator=(const CWaypoint &rhs)
{
    memcpy(&m_Data, &(rhs.m_Data), sizeof(m_Data));
    return *this;
}

void CWaypoint::MakeAbsolute(const double ref_lat, const double ref_lon, const double ref_alt)
{
    // If the waypoint is already absolute, make sure the X, Y, and Z values
    // are valid.
    if (GetType() == WAYPOINT_TYPE_ABSOLUTE) {
        CalcXYZ();
        return;
    }

    SetType(WAYPOINT_TYPE_ABSOLUTE);

    double el = DegToRad(GetElevation());
    double az = DegToRad(GetAzimuth());

    // Reference X, Y, and Z
    double x1 = 0.0;
    double y1 = 0.0;
    double z1 = ref_alt;
    GDCtoTM(ref_lat, ref_lon, &y1, &x1);

    // Waypoint X, Y, and Z
    double a = GetRange() * cos(el);
    double x2 = x1 + (a * sin(az));
    double y2 = y1 + (a * cos(az));
    double z2 = z1 + (GetRange() * sin(el));
    SetX(x2);
    SetY(y2);
    SetZ(z2);

    // Now set the new position.
    double lat = 0.0;
    double lon = 0.0;
    TMtoGDC(y2, x2, &lat, &lon);
    SetLatitude(lat);
    SetLongitude(lon);
    SetAltitude(z2);
}

BOOL CWaypoint::CalcXYZ(void)
{
    double x = 0.0;
    double y = 0.0;
    GDCtoTM(GetLatitude(), GetLongitude(), &y, &x);

    SetX(x);
    SetY(y);
    SetZ(GetAltitude());

    return TRUE;
}