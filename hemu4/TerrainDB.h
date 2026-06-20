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

// TerrainDB.h: interface for the CTerrainDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TERRAINDB_H__FA7AE863_2654_4591_968E_CE0A2BC03A64__INCLUDED_)
#define AFX_TERRAINDB_H__FA7AE863_2654_4591_968E_CE0A2BC03A64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "Component.h"
#include "ConfigTempl.h"

class CTerrainDB : public CObject
{
    DECLARE_SERIAL(CTerrainDB)

public:
    CTerrainDB(const TEMPL_DATABASE *templ);
    ~CTerrainDB();
    CComponentMap &GetComponentMap(void);
    CComponent *GetComponent(int compId, int instId);
    BOOL AddComponent(CComponent *comp);
    BOOL DeleteComponent(int compId, int instId);
    void ClearComponents(void);
    BOOL ApplyTemplate(const TEMPL_DATABASE *templ);
    void SetName(LPCTSTR name);
    void SetName(CString &name);
    CString GetName(void);
    int GetID(void);
    void SetOrigin(const double lat, const double lon, const double alt);
    void SetOriginLat(const double lat);
    double GetOriginLat(void);
    void SetOriginLon(const double lon);
    double GetOriginLon(void);
    void SetOriginAlt(const double alt);
    double GetOriginAlt(void);
    void SetDefault(const double lat, const double lon, const double alt);
    void SetDefaultLat(const double lat);
    double GetDefaultLat(void);
    void SetDefaultLon(const double lon);
    double GetDefaultLon(void);
    void SetDefaultAlt(const double alt);
    double GetDefaultAlt(void);
    void SetErmName(LPCTSTR name);
    void SetErmName(CString &name);
    CString GetErmName(void);
    void SetErmRadius(const double radius);
    double GetErmRadius(void);
    void SetErmFlattening(const double flattening);
    double GetErmFlattening(void);

    virtual void Serialize(CArchive &ar);

public:
    CTerrainDB();

    CString m_Name;
    CString m_ErmName;
    int m_DatabaseID;
    double m_OriginLat;
    double m_OriginLon;
    double m_OriginAlt;
    double m_DefaultLat;
    double m_DefaultLon;
    double m_DefaultAlt;
    double m_ErmRadius;
    double m_ErmFlattening;
    CComponentMap m_Components;
};

inline CComponentMap &CTerrainDB::GetComponentMap(void)
{
    return m_Components;
}

inline void CTerrainDB::SetName(LPCTSTR name)
{
    m_Name = name;
}

inline void CTerrainDB::SetName(CString &name)
{
    m_Name = name;
}

inline CString CTerrainDB::GetName(void)
{
    return m_Name;
}

inline void CTerrainDB::SetErmName(LPCTSTR name)
{
    m_ErmName = name;
}

inline void CTerrainDB::SetErmName(CString &name)
{
    m_ErmName = name;
}

inline CString CTerrainDB::GetErmName(void)
{
    return m_ErmName;
}

inline int CTerrainDB::GetID(void)
{
    return m_DatabaseID;
}

inline void CTerrainDB::SetOrigin(const double lat, const double lon, const double alt)
{
    m_OriginLat = lat;
    m_OriginLon = lon;
    m_OriginAlt = alt;
}

inline void CTerrainDB::SetOriginLat(const double lat)
{
    m_OriginLat = lat;
}

inline double CTerrainDB::GetOriginLat(void)
{
    return m_OriginLat;
}

inline void CTerrainDB::SetOriginLon(const double lon)
{
    m_OriginLon = lon;
}

inline double CTerrainDB::GetOriginLon(void)
{
    return m_OriginLon;
}

inline void CTerrainDB::SetOriginAlt(const double alt)
{
    m_OriginAlt = alt;
}

inline double CTerrainDB::GetOriginAlt(void)
{
    return m_OriginAlt;
}

inline void CTerrainDB::SetDefault(const double lat, const double lon, const double alt)
{
    m_DefaultLat = lat;
    m_DefaultLon = lon;
    m_DefaultAlt = alt;
}

inline void CTerrainDB::SetDefaultLat(const double lat)
{
    m_DefaultLat = lat;
}

inline double CTerrainDB::GetDefaultLat(void)
{
    return m_DefaultLat;
}

inline void CTerrainDB::SetDefaultLon(const double lon)
{
    m_DefaultLon = lon;
}

inline double CTerrainDB::GetDefaultLon(void)
{
    return m_DefaultLon;
}

inline void CTerrainDB::SetDefaultAlt(const double alt)
{
    m_DefaultAlt = alt;
}

inline double CTerrainDB::GetDefaultAlt(void)
{
    return m_DefaultAlt;
}

inline void CTerrainDB::SetErmRadius(const double radius)
{
    m_ErmRadius = radius;
}

inline double CTerrainDB::GetErmRadius(void)
{
    return m_ErmRadius;
}

inline void CTerrainDB::SetErmFlattening(const double flattening)
{
    m_ErmFlattening = flattening;
}

inline double CTerrainDB::GetErmFlattening(void)
{
    return m_ErmFlattening;
}

#endif // !defined(AFX_TERRAINDB_H__FA7AE863_2654_4591_968E_CE0A2BC03A64__INCLUDED_)
