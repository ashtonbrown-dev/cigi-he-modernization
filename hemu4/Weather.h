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

// Weather.h: interface for the CWeather class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEATHER_H__2A4FCF4A_4B2D_4E38_AF13_70C23E75F26C__INCLUDED_)
#define AFX_WEATHER_H__2A4FCF4A_4B2D_4E38_AF13_70C23E75F26C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "Component.h"
#include "ConfigTempl.h"

class CWeather : public CObject
{
    DECLARE_SERIAL(CWeather)

public:
    CWeather();
    CWeather(const TEMPL_WEATHER *templ);
    virtual ~CWeather();
    CWeather &operator=(const CWeather &rhs);
    BOOL HasChanged(void);
    void ResetChangedFlag(void);
    CComponent *GetComponent(const int id);
    BOOL AddComponent(CComponent *comp);
    BOOL DeleteComponent(const int id);
    void ClearComponents(void);
    CComponentMap &GetComponentMap(void);
    HTREEITEM GetHtree(void);
    void SetHtree(const HTREEITEM htree);
    CString GetName(void);
    void SetName(CString name);
    void SetName(LPCTSTR name);
    int GetLayerID(void);
    void SetScope(const unsigned int scope);
    unsigned char GetScope(void);
    void SetOwnerID(const int id);
    int GetOwnerID(void);
    void SetWeatherEnable(const BOOL enable);
    BOOL GetWeatherEnable(void);
    void SetTopScudEnable(const BOOL enable);
    BOOL GetTopScudEnable(void);
    void SetBottomScudEnable(const BOOL enable);
    BOOL GetBottomScudEnable(void);
    void SetRandomWindEnable(const BOOL enable);
    BOOL GetRandomWindEnable(void);
    void SetRandomLightningEnable(const BOOL enable);
    BOOL GetRandomLightningEnable(void);
    void SetCloudType(const unsigned char type);
    unsigned char GetCloudType(void);
    void SetSeverity(const unsigned char severity);
    unsigned char GetSeverity(void);
    void SetHumidity(const unsigned char humidity);
    unsigned char GetHumidity(void);
    void SetTemperature(const float temperature);
    float GetTemperature(void);
    void SetVisibility(const float visibility);
    float GetVisibility(void);
    void SetTopScudFreq(const float freq);
    float GetTopScudFreq(void);
    void SetBottomScudFreq(const float freq);
    float GetBottomScudFreq(void);
    void SetCoverage(const float coverage);
    float GetCoverage(void);
    void SetBaseElevation(const float elev);
    float GetBaseElevation(void);
    void SetThickness(const float thickness);
    float GetThickness(void);
    void SetTopTransitionBand(const float height);
    float GetTopTransitionBand(void);
    void SetBottomTransitionBand(const float height);
    float GetBottomTransitionBand(void);
    void SetHorzWindspeed(const float speed);
    float GetHorzWindspeed(void);
    void SetVertWindspeed(const float speed);
    float GetVertWindspeed(void);
    void SetWindDirection(const float direction);
    float GetWindDirection(void);
    void SetBaroPressure(const float pressure);
    float GetBaroPressure(void);
    void SetAerosolConcentration(const float concentration);
    float GetAerosolConcentration(void);
    void CreateCigiPacket(void *buffer);

    virtual void Serialize(CArchive &ar);

protected:
    CString m_Name;
    int m_LayerID;
    int m_OwnerID;
    BOOL m_HasChanged;
    HTREEITEM m_Htree;
    unsigned char m_Scope;
    unsigned char m_Enable;
    unsigned char m_TopScudEnable;
    unsigned char m_BottomScudEnable;
    unsigned char m_RandomWindEnable;
    unsigned char m_RandomLightningEnable;
    unsigned char m_CloudType;
    unsigned char m_Severity;
    unsigned char m_Humidity;
    float m_Temperature;
    float m_Visibility;
    float m_TopScudFreq;
    float m_BottomScudFreq;
    float m_Coverage;
    float m_BaseElevation;
    float m_Thickness;
    float m_TopTransitionBand;
    float m_BottomTransitionBand;
    float m_HorzWindspeed;
    float m_VertWindspeed;
    float m_WindDirection;
    float m_BaroPressure;
    float m_AerosolConcentration;

    CComponentMap m_Components;
};

inline BOOL CWeather::HasChanged(void)
{
    return m_HasChanged;
}

inline void CWeather::ResetChangedFlag(void)
{
    m_HasChanged = FALSE;
}

inline CComponentMap &CWeather::GetComponentMap(void)
{
    return m_Components;
}

inline HTREEITEM CWeather::GetHtree(void)
{
    return m_Htree;
}

inline void CWeather::SetHtree(const HTREEITEM htree)
{
    m_Htree = htree;
    m_HasChanged = TRUE;
}

inline CString CWeather::GetName(void)
{
    return m_Name;
}

inline void CWeather::SetName(CString name)
{
    m_Name = name;
    m_HasChanged = TRUE;
}

inline void CWeather::SetName(LPCTSTR name)
{
    m_Name = name;
    m_HasChanged = TRUE;
}

inline int CWeather::GetLayerID(void)
{
    return m_LayerID;
}

inline void CWeather::SetScope(const unsigned int scope)
{
    if (scope <= 2) {
        m_Scope = scope;
        m_HasChanged = TRUE;
    }
}

inline unsigned char CWeather::GetScope(void)
{
    return m_Scope;
}

inline void CWeather::SetOwnerID(const int id)
{
    m_OwnerID = id;
    m_HasChanged = TRUE;
}

inline int CWeather::GetOwnerID(void)
{
    return m_OwnerID;
}

inline void CWeather::SetWeatherEnable(const BOOL enable)
{
    m_Enable = enable ? 1 : 0;
    m_HasChanged = TRUE;
}

inline BOOL CWeather::GetWeatherEnable(void)
{
    return m_Enable ? TRUE : FALSE;
}

inline void CWeather::SetTopScudEnable(const BOOL enable)
{
    m_TopScudEnable = enable ? 1 : 0;
    m_HasChanged = TRUE;
}

inline BOOL CWeather::GetTopScudEnable(void)
{
    return m_TopScudEnable ? TRUE : FALSE;
}

inline void CWeather::SetBottomScudEnable(const BOOL enable)
{
    m_BottomScudEnable = enable ? 1 : 0;
    m_HasChanged = TRUE;
}

inline BOOL CWeather::GetBottomScudEnable(void)
{
    return m_BottomScudEnable ? TRUE : FALSE;
}

inline void CWeather::SetRandomWindEnable(const BOOL enable)
{
    m_RandomWindEnable = enable ? 1 : 0;
    m_HasChanged = TRUE;
}

inline BOOL CWeather::GetRandomWindEnable(void)
{
    return m_RandomWindEnable ? TRUE : FALSE;
}

inline void CWeather::SetRandomLightningEnable(const BOOL enable)
{
    m_RandomLightningEnable = enable ? 1 : 0;
    m_HasChanged = TRUE;
}

inline BOOL CWeather::GetRandomLightningEnable(void)
{
    return m_RandomLightningEnable ? TRUE : FALSE;
}

inline void CWeather::SetCloudType(const unsigned char type)
{
    if (type <= 15) {
        m_CloudType = type;
        m_HasChanged = TRUE;
    }
}

inline unsigned char CWeather::GetCloudType(void)
{
    return m_CloudType;
}

inline void CWeather::SetSeverity(const unsigned char severity)
{
    if (severity <= 5) {
        m_Severity = severity;
        m_HasChanged = TRUE;
    }
}

inline unsigned char CWeather::GetSeverity(void)
{
    return m_Severity;
}

inline void CWeather::SetHumidity(const unsigned char humidity)
{
    m_Humidity = humidity;
    m_HasChanged = TRUE;
}

inline unsigned char CWeather::GetHumidity(void)
{
    return m_Humidity;
}

inline void CWeather::SetTemperature(const float temperature)
{
    m_Temperature = temperature;
    m_HasChanged = TRUE;
}

inline float CWeather::GetTemperature(void)
{
    return m_Temperature;
}

inline void CWeather::SetVisibility(const float visibility)
{
    m_Visibility = visibility;
    m_HasChanged = TRUE;
}

inline float CWeather::GetVisibility(void)
{
    return m_Visibility;
}

inline void CWeather::SetTopScudFreq(const float freq)
{
    m_TopScudFreq = freq;
    m_HasChanged = TRUE;
}

inline float CWeather::GetTopScudFreq(void)
{
    return m_TopScudFreq;
}

inline void CWeather::SetBottomScudFreq(const float freq)
{
    m_BottomScudFreq = freq;
    m_HasChanged = TRUE;
}

inline float CWeather::GetBottomScudFreq(void)
{
    return m_BottomScudFreq;
}

inline void CWeather::SetCoverage(const float coverage)
{
    m_Coverage = coverage;
    m_HasChanged = TRUE;
}

inline float CWeather::GetCoverage(void)
{
    return m_Coverage;
}

inline void CWeather::SetBaseElevation(const float elev)
{
    m_BaseElevation = elev;
    m_HasChanged = TRUE;
}

inline float CWeather::GetBaseElevation(void)
{
    return m_BaseElevation;
}

inline void CWeather::SetThickness(const float thickness)
{
    m_Thickness = thickness;
    m_HasChanged = TRUE;
}

inline float CWeather::GetThickness(void)
{
    return m_Thickness;
}

inline void CWeather::SetTopTransitionBand(const float height)
{
    m_TopTransitionBand = height;
    m_HasChanged = TRUE;
}

inline float CWeather::GetTopTransitionBand(void)
{
    return m_TopTransitionBand;
}

inline void CWeather::SetBottomTransitionBand(const float height)
{
    m_BottomTransitionBand = height;
    m_HasChanged = TRUE;
}

inline float CWeather::GetBottomTransitionBand(void)
{
    return m_BottomTransitionBand;
}

inline void CWeather::SetHorzWindspeed(const float speed)
{
    m_HorzWindspeed = speed;
    m_HasChanged = TRUE;
}

inline float CWeather::GetHorzWindspeed(void)
{
    return m_HorzWindspeed;
}

inline void CWeather::SetVertWindspeed(const float speed)
{
    m_VertWindspeed = speed;
    m_HasChanged = TRUE;
}

inline float CWeather::GetVertWindspeed(void)
{
    return m_VertWindspeed;
}

inline void CWeather::SetWindDirection(const float direction)
{
    m_WindDirection = direction;
    m_HasChanged = TRUE;
}

inline float CWeather::GetWindDirection(void)
{
    return m_WindDirection;
}

inline void CWeather::SetBaroPressure(const float pressure)
{
    m_BaroPressure = pressure;
    m_HasChanged = TRUE;
}

inline float CWeather::GetBaroPressure(void)
{
    return m_BaroPressure;
}

inline void CWeather::SetAerosolConcentration(const float concentration)
{
    m_AerosolConcentration = concentration;
    m_HasChanged = TRUE;
}

inline float CWeather::GetAerosolConcentration(void)
{
    return m_AerosolConcentration;
}

typedef CMap<int, int, CWeather *, CWeather *&> CWeatherMap;
template <> void AFXAPI SerializeElements<CWeather *>(CArchive &ar, CWeather **pElements, int nCount);

#endif // !defined(AFX_WEATHER_H__2A4FCF4A_4B2D_4E38_AF13_70C23E75F26C__INCLUDED_)
