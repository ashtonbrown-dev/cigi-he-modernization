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

// Waypoint.h: interface for the CWaypoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAYPOINT_H__38588B38_FF6A_4485_86B6_9A774A691C2B__INCLUDED_)
#define AFX_WAYPOINT_H__38588B38_FF6A_4485_86B6_9A774A691C2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"

class CWaypoint : public CObject
{
    DECLARE_SERIAL(CWaypoint)

public:
    CWaypoint();
    CWaypoint(const CWaypoint &waypoint);
    virtual ~CWaypoint();
    virtual void Serialize(CArchive &ar);
    CWaypoint &operator=(const CWaypoint &rhs);
    void MakeAbsolute(const double ref_lat, const double ref_lon, const double ref_alt);
    int GetID(void);
    void SetID(const int id);
    int GetType(void);
    void SetType(const int type);
    int GetRefEntity(void);
    void SetRefEntity(const int entity_id);
    int GetTime(void);
    void SetTime(const int time);
    int GetAirspeed(void);
    void SetAirspeed(const int airspeed);
    double GetAccel(void);
    void SetAccel(const double accel);
    double GetTurnRate(void);
    void SetTurnRate(const double rate);
    double GetAzimuth(void);
    void SetAzimuth(const double azimuth);
    double GetRange(void);
    void SetRange(const double range);
    double GetElevation(void);
    void SetElevation(const double elevation);
    double GetLatitude(void);
    void SetLatitude(const double latitude);
    double GetLongitude(void);
    void SetLongitude(const double longitude);
    double GetAltitude(void);
    void SetAltitude(const double altitude);
    double GetX(void);
    void SetX(const double x);
    double GetY(void);
    void SetY(const double y);
    double GetZ(void);
    void SetZ(const double z);

protected:
    WAYPOINT m_Data;

    BOOL CalcXYZ(void);
};

inline int CWaypoint::GetID(void)
{
    return m_Data.id;
}

inline void CWaypoint::SetID(const int id)
{
    m_Data.id = id;
}

inline int CWaypoint::GetType(void)
{
    return m_Data.type;
}

inline void CWaypoint::SetType(const int type)
{
    m_Data.type = type;
}

inline int CWaypoint::GetRefEntity(void)
{
    return m_Data.ref_entity;
}

inline void CWaypoint::SetRefEntity(const int entity_id)
{
    m_Data.ref_entity = entity_id;
}

inline int CWaypoint::GetTime(void)
{
    return m_Data.time;
}

inline void CWaypoint::SetTime(const int time)
{
    m_Data.time = time;
}

inline int CWaypoint::GetAirspeed(void)
{
    return m_Data.airspeed;
}

inline void CWaypoint::SetAirspeed(const int airspeed)
{
    m_Data.airspeed = airspeed;
}

inline double CWaypoint::GetAccel(void)
{
    return m_Data.accel;
}

inline void CWaypoint::SetAccel(const double accel)
{
    m_Data.accel = accel;
}

inline double CWaypoint::GetTurnRate(void)
{
    return m_Data.turn_rate;
}

inline void CWaypoint::SetTurnRate(const double rate)
{
    m_Data.turn_rate = rate;
}

inline double CWaypoint::GetAzimuth(void)
{
    return m_Data.azimuth;
}

inline void CWaypoint::SetAzimuth(const double azimuth)
{
    m_Data.azimuth = azimuth;
}

inline double CWaypoint::GetRange(void)
{
    return m_Data.range;
}

inline void CWaypoint::SetRange(const double range)
{
    m_Data.range = range;
}

inline double CWaypoint::GetElevation(void)
{
    return m_Data.elevation;
}

inline void CWaypoint::SetElevation(const double elevation)
{
    m_Data.elevation = elevation;
}

inline double CWaypoint::GetLatitude(void)
{
    return m_Data.lat;
}

inline void CWaypoint::SetLatitude(const double latitude)
{
    m_Data.lat = latitude;
}

inline double CWaypoint::GetLongitude(void)
{
    return m_Data.lon;
}

inline void CWaypoint::SetLongitude(const double longitude)
{
    m_Data.lon = longitude;
}

inline double CWaypoint::GetAltitude(void)
{
    return m_Data.alt;
}

inline void CWaypoint::SetAltitude(const double altitude)
{
    m_Data.alt = altitude;
}

inline double CWaypoint::GetX(void)
{
    return m_Data.x;
}

inline void CWaypoint::SetX(const double x)
{
    m_Data.x = x;
}

inline double CWaypoint::GetY(void)
{
    return m_Data.y;
}

inline void CWaypoint::SetY(const double y)
{
    m_Data.y = y;
}

inline double CWaypoint::GetZ(void)
{
    return m_Data.z;
}

inline void CWaypoint::SetZ(const double z)
{
    m_Data.z = z;
}

#endif // !defined(AFX_WAYPOINT_H__38588B38_FF6A_4485_86B6_9A774A691C2B__INCLUDED_)
