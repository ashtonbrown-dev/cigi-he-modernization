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

// GlobalEnv.h: interface for the CGlobalEnv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALENV_H__4A12DBE8_C2BB_4FE8_9AD3_A8383030DEA8__INCLUDED_)
#define AFX_GLOBALENV_H__4A12DBE8_C2BB_4FE8_9AD3_A8383030DEA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "Component.h"
#include "TerrainCondition.h"
#include "Wave.h"
#include "Weather.h"

class TEMPL_GLOBAL_ENV;

class CGlobalEnv : public CObject
{
    DECLARE_SERIAL(CGlobalEnv)

public:
    CGlobalEnv();
    virtual ~CGlobalEnv();
    CGlobalEnv &operator=(const TEMPL_GLOBAL_ENV &templ);
    CComponent *GetCelestialComponent(unsigned int componentID, unsigned int instanceID);
    CComponent *GetAtmosphereComponent(unsigned int componentID, unsigned int instanceID);
    CComponent *GetSeaSurfaceComponent(unsigned int componentID, unsigned int instanceID);
    CComponent *GetTerrainComponent(unsigned int componentID, unsigned int instanceID);
    CTerrainCondition *GetTerrainCondition(const int id);
    CTerrainCondition *GetTerrainCondition(const HTREEITEM htree);
    CWave *GetWave(const int id);
    CWave *GetWave(const HTREEITEM htree);
    CWeather *GetWeatherLayer(const int id);
    CWeather *GetWeatherLayer(const HTREEITEM htree);
    CComponentMap &GetCelestialComponentMap(void);
    CComponentMap &GetAtmosphereComponentMap(void);
    CComponentMap &GetSeaSurfaceComponentMap(void);
    CComponentMap &GetTerrainComponentMap(void);
    CTerrainConditionMap &GetTerrainConditionMap(void);
    CWaveMap &GetWaveMap(void);
    CWeatherMap &GetWeatherLayerMap(void);
    BOOL AddComponent(CComponent *comp);
    BOOL AddCelestialComponent(CComponent *comp);
    BOOL AddAtmosphereComponent(CComponent *comp);
    BOOL AddSeaSurfaceComponent(CComponent *comp);
    BOOL AddTerrainComponent(CComponent *comp);
    BOOL AddTerrainCondition(CTerrainCondition *cond);
    BOOL AddWave(CWave *wave);
    BOOL AddWeatherLayer(CWeather *weather);
    BOOL DeleteCelestialComponent(const int id);
    BOOL DeleteAtmosphereComponent(const int id);
    BOOL DeleteSeaSurfaceComponent(const int id);
    BOOL DeleteTerrainComponent(const int id);
    BOOL DeleteTerrainCondition(const int id);
    BOOL DeleteWave(const int id);
    BOOL DeleteWeatherLayer(const int id);
    void ClearCelestialComponents(void);
    void ClearAtmosphereComponents(void);
    void ClearSeaSurfaceComponents(void);
    void ClearTerrainComponents(void);
    void ClearTerrainConditions(void);
    void ClearWaves(void);
    void ClearWeatherLayers(void);
    void SetDate(CTime &date);
    void SetDate(const int year, const int month, const int day);
    void SetTime(CTime &time);
    void SetTime(const int hour, const int minute);
    void SetDateTime(CTime &time);
    void SetDateTime(const int year, const int month, const int day,
                     const int hour, const int minute);
    CTime GetDateTime(void);
    void SetEphemerisEnable(const BOOL enable);
    BOOL GetEphemerisEnable(void);
    void SetSunEnable(const BOOL enable);
    BOOL GetSunEnable(void);
    void SetMoonEnable(const BOOL enable);
    BOOL GetMoonEnable(void);
    void SetStarfieldEnable(const BOOL enable);
    BOOL GetStarfieldEnable(void);
    void SetStarfieldIntensity(const float intensity);
    float GetStarfieldIntensity(void);
    void SetAtmosphericsEnable(const BOOL enable);
    BOOL GetAtmosphericsEnable(void);
    void SetHumidity(const int humidity);
    int GetHumidity(void);
    void SetBaroPressure(const float pressure);
    float GetBaroPressure(void);
    void SetTemperature(const float temperature);
    float GetTemperature(void);
    void SetVisibility(const float vis);
    float GetVisibility(void);
    void SetHorzWindspeed(const float speed);
    float GetHorzWindspeed(void);
    void SetVertWindspeed(const float speed);
    float GetVertWindspeed(void);
    void SetWindDirection(const float direction);
    float GetWindDirection(void);
    void SetSeaSurfaceEnable(const BOOL enable);
    BOOL GetSeaSurfaceEnable(void);
    void SetWhitecapEnable(const BOOL enable);
    BOOL GetWhitecapEnable(void);
    void SetSeaSurfaceHeight(const float height);
    float GetSeaSurfaceHeight(void);
    void SetSurfaceTemperature(const float temperature);
    float GetSurfaceTemperature(void);
    void SetSurfaceClarity(const float clarity);
    float GetSurfaceClarity(void);

    virtual void Serialize(CArchive &ar);

protected:
    void SendCelestialSpherePacket(void);
    void SendAtmospherePacket(void);
    void SendMaritimeSurfacePacket(void);
    void SendTerrainSurfacePacket(void);

    // Celestial Sphere Properties
    CTime m_DateTime;
    char m_EphemerisEnable;
    char m_SunEnable;
    char m_MoonEnable;
    char m_StarfieldEnable;
    float m_StarfieldIntensity;
    CComponentMap m_CelestialComponents;

    // Atmospheric Properties
    char m_AtmosphericsEnable;
    uint8_t m_Humidity;
    float m_BaroPressure;
    float m_Temperature;
    float m_Visibility;
    float m_HorzWindspeed;
    float m_VertWindspeed;
    float m_WindDirection;
    CComponentMap m_AtmosphereComponents;
    CWeatherMap m_WeatherLayers;

    // Maritime Surface Conditions
    char m_SeaSurfaceEnable;
    char m_WhitecapEnable;
    float m_SeaSurfaceHeight;
    float m_SurfaceTemperature;
    float m_SurfaceClarity;
    CComponentMap m_SeaSurfaceComponents;
    CWaveMap m_Waves;

    // Terrain Surface Conditions
    CComponentMap m_TerrainComponents;  // For all databases
    CTerrainConditionMap m_TerrainConditions;
};

inline CComponentMap &CGlobalEnv::GetCelestialComponentMap(void)
{
    return m_CelestialComponents;
}

inline CComponentMap &CGlobalEnv::GetAtmosphereComponentMap(void)
{
    return m_AtmosphereComponents;
}

inline CComponentMap &CGlobalEnv::GetSeaSurfaceComponentMap(void)
{
    return m_SeaSurfaceComponents;
}

inline CComponentMap &CGlobalEnv::GetTerrainComponentMap(void)
{
    return m_TerrainComponents;
}

inline CTerrainConditionMap &CGlobalEnv::GetTerrainConditionMap(void)
{
    return m_TerrainConditions;
}

inline CWaveMap &CGlobalEnv::GetWaveMap(void)
{
    return m_Waves;
}

inline CWeatherMap &CGlobalEnv::GetWeatherLayerMap(void)
{
    return m_WeatherLayers;
}

inline void CGlobalEnv::SetDate(CTime &date)
{
    m_DateTime = CTime(date.GetYear(), date.GetMonth(),
                       date.GetDay(), m_DateTime.GetHour(),
                       m_DateTime.GetMinute(), 0);
    SendCelestialSpherePacket();
}

inline void CGlobalEnv::SetDate(const int year, const int month, const int day)
{
    m_DateTime = CTime(year, month, day, m_DateTime.GetHour(),
                       m_DateTime.GetMinute(), 0);
    SendCelestialSpherePacket();
}

inline void CGlobalEnv::SetTime(CTime &time)
{
    m_DateTime = CTime(m_DateTime.GetYear(), m_DateTime.GetMonth(),
                       m_DateTime.GetDay(), time.GetHour(),
                       time.GetMinute(), 0);
    SendCelestialSpherePacket();
}

inline void CGlobalEnv::SetTime(const int hour, const int minute)
{
    m_DateTime = CTime(m_DateTime.GetYear(), m_DateTime.GetMonth(),
                       m_DateTime.GetDay(), hour, minute, 0);
    SendCelestialSpherePacket();
}

inline void CGlobalEnv::SetDateTime(CTime &time)
{
    m_DateTime = time;
    SendCelestialSpherePacket();
}

inline void CGlobalEnv::SetDateTime(const int year, const int month, const int day,
                                    const int hour, const int minute)
{
    m_DateTime = CTime(year, month, day, hour, minute, 0);
    SendCelestialSpherePacket();
}

inline CTime CGlobalEnv::GetDateTime(void)
{
    return m_DateTime;
}

inline void CGlobalEnv::SetEphemerisEnable(const BOOL enable)
{
    m_EphemerisEnable = enable ? 1 : 0;
    SendCelestialSpherePacket();
}

inline BOOL CGlobalEnv::GetEphemerisEnable(void)
{
    return m_EphemerisEnable ? TRUE : FALSE;
}

inline void CGlobalEnv::SetSunEnable(const BOOL enable)
{
    m_SunEnable = enable ? 1 : 0;
    SendCelestialSpherePacket();
}

inline BOOL CGlobalEnv::GetSunEnable(void)
{
    return m_SunEnable ? TRUE : FALSE;
}

inline void CGlobalEnv::SetMoonEnable(const BOOL enable)
{
    m_MoonEnable = enable ? 1 : 0;
    SendCelestialSpherePacket();
}

inline BOOL CGlobalEnv::GetMoonEnable(void)
{
    return m_MoonEnable ? TRUE : FALSE;
}

inline void CGlobalEnv::SetStarfieldEnable(const BOOL enable)
{
    m_StarfieldEnable = enable ? 1 : 0;
    SendCelestialSpherePacket();
}

inline BOOL CGlobalEnv::GetStarfieldEnable(void)
{
    return m_StarfieldEnable ? TRUE : FALSE;
}

inline void CGlobalEnv::SetStarfieldIntensity(const float intensity)
{
    if ((intensity >= 0.0) && (intensity <= 100.0)) {
        m_StarfieldIntensity = intensity;
        SendCelestialSpherePacket();
    }
}

inline float CGlobalEnv::GetStarfieldIntensity(void)
{
    return m_StarfieldIntensity;
}

inline void CGlobalEnv::SetAtmosphericsEnable(const BOOL enable)
{
    m_AtmosphericsEnable = enable ? 1 : 0;
    SendAtmospherePacket();
}

inline BOOL CGlobalEnv::GetAtmosphericsEnable(void)
{
    return m_AtmosphericsEnable ? TRUE : FALSE;
}

inline void CGlobalEnv::SetHumidity(const int humidity)
{
    if ((humidity >= 0) && (humidity <= 100)) {
        m_Humidity = humidity;
        SendAtmospherePacket();
    }
}

inline int CGlobalEnv::GetHumidity(void)
{
    return m_Humidity;
}

inline void CGlobalEnv::SetBaroPressure(const float pressure)
{
    m_BaroPressure = pressure;
    SendAtmospherePacket();
}

inline float CGlobalEnv::GetBaroPressure(void)
{
    return m_BaroPressure;
}

inline void CGlobalEnv::SetTemperature(const float temperature)
{
    m_Temperature = temperature;
    SendAtmospherePacket();
}

inline float CGlobalEnv::GetTemperature(void)
{
    return m_Temperature;
}

inline void CGlobalEnv::SetVisibility(const float vis)
{
    m_Visibility = vis;
    SendAtmospherePacket();
}

inline float CGlobalEnv::GetVisibility(void)
{
    return m_Visibility;
}

inline void CGlobalEnv::SetHorzWindspeed(const float speed)
{
    m_HorzWindspeed = speed;
    SendAtmospherePacket();
}

inline float CGlobalEnv::GetHorzWindspeed(void)
{
    return m_HorzWindspeed;
}

inline void CGlobalEnv::SetVertWindspeed(const float speed)
{
    m_VertWindspeed = speed;
    SendAtmospherePacket();
}

inline float CGlobalEnv::GetVertWindspeed(void)
{
    return m_VertWindspeed;
}

inline void CGlobalEnv::SetWindDirection(const float direction)
{
    m_WindDirection = direction;
    SendAtmospherePacket();
}

inline float CGlobalEnv::GetWindDirection(void)
{
    return m_WindDirection;
}

inline void CGlobalEnv::SetSeaSurfaceEnable(const BOOL enable)
{
    m_SeaSurfaceEnable = enable ? 1 : 0;
    SendMaritimeSurfacePacket();
}

inline BOOL CGlobalEnv::GetSeaSurfaceEnable(void)
{
    return m_SeaSurfaceEnable ? TRUE : FALSE;
}

inline void CGlobalEnv::SetWhitecapEnable(const BOOL enable)
{
    m_WhitecapEnable = enable ? 1 : 0;
    SendMaritimeSurfacePacket();
}

inline BOOL CGlobalEnv::GetWhitecapEnable(void)
{
    return m_WhitecapEnable ? TRUE : FALSE;
}

inline void CGlobalEnv::SetSeaSurfaceHeight(const float height)
{
    m_SeaSurfaceHeight = height;
    SendMaritimeSurfacePacket();
}

inline float CGlobalEnv::GetSeaSurfaceHeight(void)
{
    return m_SeaSurfaceHeight;
}

inline void CGlobalEnv::SetSurfaceTemperature(const float temperature)
{
    m_SurfaceTemperature = temperature;
    SendMaritimeSurfacePacket();
}

inline float CGlobalEnv::GetSurfaceTemperature(void)
{
    return m_SurfaceTemperature;
}

inline void CGlobalEnv::SetSurfaceClarity(const float clarity)
{
    m_SurfaceClarity = clarity;
    SendMaritimeSurfacePacket();
}

inline float CGlobalEnv::GetSurfaceClarity(void)
{
    return m_SurfaceClarity;
}

#endif // !defined(AFX_GLOBALENV_H__4A12DBE8_C2BB_4FE8_9AD3_A8383030DEA8__INCLUDED_)
