/** <pre>
 *  Copyright 2004 The Boeing Company
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
 *
 *  FILENAME:   GlobalEnv.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface Host Emulator (CIGI HE)
 *
 *  PROGRAM DESCRIPTION:
 *  The Host Emulator (HE) acts as a surrogate in the absence of a simulation host.
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME
 *  DESCRIPTION OF CHANGE
 *
 *  11/11/2004 Lance Durham
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  Version 3.1.1
 */

#include "stdafx.h"
#include "cigi4types.h"
#include "common.h"
#include "GlobalEnv.h"
#include "globals.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CGlobalEnv, CObject, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlobalEnv::CGlobalEnv()
{
    m_DateTime = CTime::GetCurrentTime();
    m_EphemerisEnable = 0;
    m_SunEnable = 0;
    m_MoonEnable = 0;
    m_StarfieldEnable = 0;
    m_StarfieldIntensity = 0.0;
    m_AtmosphericsEnable = 0;
    m_Humidity = 0;
    m_BaroPressure = 0.0;
    m_Temperature = 0.0;
    m_Visibility = 0.0;
    m_HorzWindspeed = 0.0;
    m_VertWindspeed = 0.0;
    m_WindDirection = 0.0;
    m_SeaSurfaceEnable = 0;
    m_WhitecapEnable = 0;
    m_SeaSurfaceHeight = 0.0;
    m_SurfaceTemperature = 0.0;
    m_SurfaceClarity = 0.0;

    m_CelestialComponents.RemoveAll();
    m_CelestialComponents.InitHashTable(521);
    m_AtmosphereComponents.RemoveAll();
    m_AtmosphereComponents.InitHashTable(521);
    m_SeaSurfaceComponents.RemoveAll();
    m_SeaSurfaceComponents.InitHashTable(521);
    m_TerrainComponents.RemoveAll();
    m_TerrainComponents.InitHashTable(521);
    m_TerrainConditions.RemoveAll();
    m_TerrainConditions.InitHashTable(521);
    m_Waves.RemoveAll();
    m_Waves.InitHashTable(263);
    m_WeatherLayers.RemoveAll();
    m_WeatherLayers.InitHashTable(263);
}

CGlobalEnv::~CGlobalEnv()
{
    ClearCelestialComponents();
    ClearAtmosphereComponents();
    ClearSeaSurfaceComponents();
    ClearTerrainComponents();
    ClearTerrainConditions();
    ClearWaves();
    ClearWeatherLayers();
}

CGlobalEnv &CGlobalEnv::operator=(const TEMPL_GLOBAL_ENV &templ)
{
    m_DateTime = CTime(templ.Year, templ.Month, templ.Day,
                       templ.Hour, templ.Minute, 0);
    m_EphemerisEnable = templ.EphemerisEnable;
    m_SunEnable = templ.SunEnable;;
    m_MoonEnable = templ.MoonEnable;
    m_StarfieldEnable = templ.StarfieldEnable;
    m_StarfieldIntensity = templ.StarfieldIntensity;
    m_AtmosphericsEnable = templ.AtmosphericsEnable;
    m_Humidity = templ.Humidity;
    m_BaroPressure = templ.BaroPressure;
    m_Temperature = templ.Temperature;
    m_Visibility = templ.Visibility;
    m_HorzWindspeed = templ.HorzWindspeed;
    m_VertWindspeed = templ.VertWindspeed;
    m_WindDirection = templ.WindDirection;
    m_SeaSurfaceEnable = templ.SeaSurfaceEnable;
    m_WhitecapEnable = templ.WhitecapEnable;
    m_SeaSurfaceHeight = templ.SeaSurfaceHeight;
    m_SurfaceTemperature = templ.SurfaceTemperature;
    m_SurfaceClarity = templ.SurfaceClarity;

    // Build the component lists.

    // Celestial Sphere
    ClearCelestialComponents();
    ClearAtmosphereComponents();
    ClearSeaSurfaceComponents();
    ClearTerrainComponents();
    unsigned long handle = 0;
    TEMPL_COMPONENT *comp_templ = templ.ComponentList.GetHead(&handle);
    while (comp_templ) {
        CComponent *comp = new CComponent(comp_templ->ComponentClass, comp_templ);

        AddComponent(comp);

        comp_templ = templ.ComponentList.GetNext(&handle);
    }

    // Terrain Conditions

    // Waves
    ClearWaves();

    // Weather Layers
    ClearWeatherLayers();
    handle = 0;
    TEMPL_WEATHER *weather_templ = templ.WeatherList.GetHead(&handle);
    while (weather_templ) {
        CWeather *weather = new CWeather(weather_templ);

        AddWeatherLayer(weather);

        weather_templ = templ.WeatherList.GetNext(&handle);
    }

    return *this;
}

void CGlobalEnv::Serialize(CArchive &ar)
{
    TRACE0("\nCGlobalEnv::Serialize()\n");

    unsigned long junk;
    int length = 0;

    if (ar.IsStoring()) {
        ar << m_DateTime;
        ar << m_EphemerisEnable;
        ar << m_SunEnable;
        ar << m_MoonEnable;
        ar << m_StarfieldEnable;
        ar << m_StarfieldIntensity;
        ar << m_AtmosphericsEnable;
        ar << m_Humidity;
        ar << m_BaroPressure;
        ar << m_Temperature;
        ar << m_Visibility;
        ar << m_HorzWindspeed;
        ar << m_VertWindspeed;
        ar << m_WindDirection;
        ar << m_SeaSurfaceEnable;
        ar << m_WhitecapEnable;
        ar << m_SeaSurfaceHeight;
        ar << m_SurfaceTemperature;
        ar << m_SurfaceClarity;
        m_CelestialComponents.Serialize(ar);
        m_AtmosphereComponents.Serialize(ar);
        m_WeatherLayers.Serialize(ar);
        m_SeaSurfaceComponents.Serialize(ar);
        m_Waves.Serialize(ar);
        m_TerrainComponents.Serialize(ar);
        m_TerrainConditions.Serialize(ar);

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("	length = %d\n", length);
    } else {
        ar >> m_DateTime;
        ar >> m_EphemerisEnable;
        ar >> m_SunEnable;
        ar >> m_MoonEnable;
        ar >> m_StarfieldEnable;
        ar >> m_StarfieldIntensity;
        ar >> m_AtmosphericsEnable;
        ar >> m_Humidity;
        ar >> m_BaroPressure;
        ar >> m_Temperature;
        ar >> m_Visibility;
        ar >> m_HorzWindspeed;
        ar >> m_VertWindspeed;
        ar >> m_WindDirection;
        ar >> m_SeaSurfaceEnable;
        ar >> m_WhitecapEnable;
        ar >> m_SeaSurfaceHeight;
        ar >> m_SurfaceTemperature;
        ar >> m_SurfaceClarity;

        // Because we keep the same global environment class instance around,
        // we need to clear all the data out before we call
        // Serialize() for each collection class.
        ClearCelestialComponents();
        ClearAtmosphereComponents();
        ClearSeaSurfaceComponents();
        ClearTerrainComponents();
        ClearTerrainConditions();
        ClearWaves();
        ClearWeatherLayers();

        m_CelestialComponents.Serialize(ar);
        m_AtmosphereComponents.Serialize(ar);
        m_WeatherLayers.Serialize(ar);
        m_SeaSurfaceComponents.Serialize(ar);
        m_Waves.Serialize(ar);
        m_TerrainComponents.Serialize(ar);
        m_TerrainConditions.Serialize(ar);

        // Store some padding in case we have to add something later.
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("	length = %d\n", length);
    }
}

void CGlobalEnv::SendCelestialSpherePacket(void)
{
    // Post a message to the driver.
    CIGI_CELESTIAL_SPHERE_CONTROL celsphere;
    celsphere.packet_id = CIGI_CELESTIAL_SPHERE_CONTROL_OPCODE;
    celsphere.packet_size = sizeof(CIGI_CELESTIAL_SPHERE_CONTROL);
    celsphere.date = m_DateTime.GetYear() + (m_DateTime.GetDay() * 10000) +
                     (m_DateTime.GetMonth() * 1000000);
    celsphere.hour = m_DateTime.GetHour();
    celsphere.minute = m_DateTime.GetMinute();
    celsphere.datetime_valid = 1;
    celsphere.ephemeris_enable = m_EphemerisEnable ? 1 : 0;
    celsphere.sun_enable = m_SunEnable ? 1 : 0;
    celsphere.moon_enable = m_MoonEnable ? 1 : 0;
    celsphere.starfield_enable = m_StarfieldEnable ? 1 : 0;
    celsphere.starfield_intensity = m_StarfieldIntensity;

    g_SendImmedCIGIMsgQueue.Push((char *)&celsphere, sizeof(celsphere));
}

void CGlobalEnv::SendAtmospherePacket(void)
{
    // Post a message to the driver.
    CIGI_ATMOSPHERE_CONTROL atmosphere;
    atmosphere.packet_id = CIGI_ATMOSPHERE_CONTROL_OPCODE;
    atmosphere.packet_size = sizeof(CIGI_ATMOSPHERE_CONTROL);
    atmosphere.atm_model_enable = m_AtmosphericsEnable ? 1 : 0;
    atmosphere.global_air_temp = m_Temperature;
    atmosphere.global_baro_pressure = m_BaroPressure;
    atmosphere.global_humidity = m_Humidity;
    atmosphere.global_visibility = m_Visibility;
    atmosphere.global_wind_direction = m_WindDirection;
    atmosphere.global_windspeed_horz = m_HorzWindspeed;
    atmosphere.global_windspeed_vert = m_VertWindspeed;

    g_SendImmedCIGIMsgQueue.Push((char *)&atmosphere, sizeof(atmosphere));
}

void CGlobalEnv::SendMaritimeSurfacePacket(void)
{
    // Post a message to the driver.
    CIGI_MARITIME_CONDITIONS_CONTROL ctrl;
    ctrl.packet_id = CIGI_MARITIME_CONDITIONS_CONTROL_OPCODE;
    ctrl.packet_size = sizeof(CIGI_MARITIME_CONDITIONS_CONTROL);
    ctrl.conditions_enable = m_SeaSurfaceEnable ? 1 : 0;
    ctrl.scope = 0; // global
    ctrl.entity_region_id = 0;
    ctrl.surface_clarity = m_SurfaceClarity;
    ctrl.surface_height = m_SeaSurfaceHeight;
    ctrl.surface_temp = m_SurfaceTemperature;
    ctrl.whitecap_enable = m_WhitecapEnable;

    g_SendImmedCIGIMsgQueue.Push((char *)&ctrl, sizeof(ctrl));
}

void CGlobalEnv::SendTerrainSurfacePacket(void)
{
    // LWD: No global terrain surface conditions.
    // Post a message to the driver.
    //CIGI_TERRESTRIAL_CONDITIONS_CONTROL ctrl;
    //ctrl.packet_id = CIGI_MARITIME_CONDITIONS_CONTROL_OPCODE;
    //ctrl.packet_size = CIGI_MARITIME_CONDITIONS_CONTROL_SIZE;
    //ctrl.condition_enable = this-> ? 1 : 0;
    //ctrl.scope = 0;   // global
    //ctrl.entity_region_id = 0;
    //ctrl.surface_clarity = m_SurfaceClarity;
    //ctrl.surface_height = m_SurfaceHeight;
    //ctrl.surface_temp = m_SurfaceTemperature;
    //ctrl.whitecap_enable = m_WhitecapEnable;

    //g_SendImmedCIGIMsgQueue.Push((char *)&ctrl, sizeof(ctrl));
}

BOOL CGlobalEnv::AddComponent(CComponent *comp)
{
    if (!comp)
        return FALSE;

    switch (comp->GetClass()) {
    case COMP_CLASS_G_SEA:
        return AddSeaSurfaceComponent(comp);
        break;

    case COMP_CLASS_G_TERRAIN:
        return AddTerrainComponent(comp);
        break;

    case COMP_CLASS_ATMOSPHERE:
        return AddAtmosphereComponent(comp);
        break;

    case COMP_CLASS_CELSPHERE:
        return AddCelestialComponent(comp);
        break;

    default:
        break;
    }

    return FALSE;
}

BOOL CGlobalEnv::AddCelestialComponent(CComponent *comp)
{
    BOOL retval = FALSE;
    const unsigned int id = MakeKey(comp->GetID(), comp->GetInstanceID());
    CComponent *search = NULL;

    // If the component already exists, delete it.
    if (m_CelestialComponents.Lookup(id, search)) {
        delete search;
        retval = TRUE;      // return TRUE if component exists
    }

    m_CelestialComponents.SetAt(id, comp);

    return retval;
}

BOOL CGlobalEnv::AddAtmosphereComponent(CComponent *comp)
{
    BOOL retval = FALSE;
    const unsigned int id = MakeKey(comp->GetID(), comp->GetInstanceID());
    CComponent *search = NULL;

    // If the component already exists, delete it.
    if (m_AtmosphereComponents.Lookup(id, search)) {
        delete search;
        retval = TRUE;      // return TRUE if component exists
    }

    m_AtmosphereComponents.SetAt(id, comp);

    return retval;
}

BOOL CGlobalEnv::AddSeaSurfaceComponent(CComponent *comp)
{
    BOOL retval = FALSE;
    const unsigned int id = MakeKey(comp->GetID(), comp->GetInstanceID());
    CComponent *search = NULL;

    // If the component already exists, delete it.
    if (m_SeaSurfaceComponents.Lookup(id, search)) {
        delete search;
        retval = TRUE;      // return TRUE if component exists
    }

    m_SeaSurfaceComponents.SetAt(id, comp);

    return retval;
}

BOOL CGlobalEnv::AddTerrainComponent(CComponent *comp)
{
    BOOL retval = FALSE;
    const unsigned int id = MakeKey(comp->GetID(), comp->GetInstanceID());
    CComponent *search = NULL;

    // If the component already exists, delete it.
    if (m_TerrainComponents.Lookup(id, search)) {
        delete search;
        retval = TRUE;      // return TRUE if component exists
    }

    m_TerrainComponents.SetAt(id, comp);

    return retval;
}

BOOL CGlobalEnv::AddTerrainCondition(CTerrainCondition *cond)
{
    BOOL retval = FALSE;
    int id = cond->GetID();
    CTerrainCondition *search = NULL;

    // If the component already exists, delete it.
    if (m_TerrainConditions.Lookup(id, search)) {
        delete search;
        retval = TRUE;      // return TRUE if component exists
    }

    m_TerrainConditions.SetAt(id, cond);

    return retval;
}

BOOL CGlobalEnv::AddWave(CWave *wave)
{
    BOOL retval = FALSE;
    int id = wave->GetID();
    CWave *search = NULL;

    // If the component already exists, delete it.
    if (m_Waves.Lookup(id, search)) {
        delete search;
        retval = TRUE;      // return TRUE if component exists
    }

    m_Waves.SetAt(id, wave);

    return retval;
}

BOOL CGlobalEnv::AddWeatherLayer(CWeather *weather)
{
    BOOL retval = FALSE;
    int id = weather->GetLayerID();
    CWeather *search = NULL;

    // If the component already exists, delete it.
    if (m_WeatherLayers.Lookup(id, search)) {
        delete search;
        retval = TRUE;      // return TRUE if layer exists
    }

    m_WeatherLayers.SetAt(id, weather);

    return retval;
}

CComponent *CGlobalEnv::GetCelestialComponent(unsigned int componentID, unsigned int instanceID)
{
    CComponent *search = NULL;

    const unsigned int key = MakeKey(componentID, instanceID);
    m_CelestialComponents.Lookup(key, search);

    return search;
}

CComponent *CGlobalEnv::GetAtmosphereComponent(unsigned int componentID, unsigned int instanceID)
{
    CComponent *search = NULL;

    const unsigned int key = MakeKey(componentID, instanceID);
    m_AtmosphereComponents.Lookup(key, search);

    return search;
}

CComponent *CGlobalEnv::GetSeaSurfaceComponent(unsigned int componentID, unsigned int instanceID)
{
    CComponent *search = NULL;

    const unsigned int key = MakeKey(componentID, instanceID);
    m_SeaSurfaceComponents.Lookup(key, search);

    return search;
}

CComponent *CGlobalEnv::GetTerrainComponent(unsigned int componentID, unsigned int instanceID)
{
    CComponent *search = NULL;

    const unsigned int key = MakeKey(componentID, instanceID);
    m_TerrainComponents.Lookup(key, search);

    return search;
}

CTerrainCondition *CGlobalEnv::GetTerrainCondition(const int id)
{
    CTerrainCondition *search = NULL;

    m_TerrainConditions.Lookup(id, search);

    return search;
}

CTerrainCondition *CGlobalEnv::GetTerrainCondition(const HTREEITEM htree)
{
    int id = 0;
    CTerrainCondition *cond = NULL;
    POSITION pos = m_TerrainConditions.GetStartPosition();
    while (pos) {
        m_TerrainConditions.GetNextAssoc(pos, id, cond);

        if (cond->GetHtree() == htree)
            return cond;
    }

    return NULL;
}

CWave *CGlobalEnv::GetWave(const int id)
{
    CWave *search = NULL;

    m_Waves.Lookup(id, search);

    return search;
}

CWave *CGlobalEnv::GetWave(const HTREEITEM htree)
{
    int id = 0;
    CWave *wave = NULL;
    POSITION pos = m_Waves.GetStartPosition();
    while (pos) {
        m_Waves.GetNextAssoc(pos, id, wave);

        if (wave->GetHtree() == htree)
            return wave;
    }

    return NULL;
}

CWeather *CGlobalEnv::GetWeatherLayer(const int id)
{
    CWeather *search = NULL;

    m_WeatherLayers.Lookup(id, search);

    return search;
}

CWeather *CGlobalEnv::GetWeatherLayer(const HTREEITEM htree)
{
    int id = 0;
    CWeather *weather = NULL;
    POSITION pos = m_WeatherLayers.GetStartPosition();
    while (pos) {
        m_WeatherLayers.GetNextAssoc(pos, id, weather);

        if (weather->GetHtree() == htree)
            return weather;
    }

    return NULL;
}

BOOL CGlobalEnv::DeleteCelestialComponent(const int id)
{
    CComponent *search = NULL;

    if (m_CelestialComponents.Lookup(id, search)) {
        if (search)
            delete search;

        m_CelestialComponents.RemoveKey(id);

        return TRUE;
    }

    return FALSE;
}

BOOL CGlobalEnv::DeleteAtmosphereComponent(const int id)
{
    CComponent *search = NULL;

    if (m_AtmosphereComponents.Lookup(id, search)) {
        if (search)
            delete search;

        m_AtmosphereComponents.RemoveKey(id);

        return TRUE;
    }

    return FALSE;
}

BOOL CGlobalEnv::DeleteSeaSurfaceComponent(const int id)
{
    CComponent *search = NULL;

    if (m_SeaSurfaceComponents.Lookup(id, search)) {
        if (search)
            delete search;

        m_SeaSurfaceComponents.RemoveKey(id);

        return TRUE;
    }

    return FALSE;
}

BOOL CGlobalEnv::DeleteTerrainComponent(const int id)
{
    CComponent *search = NULL;

    if (m_TerrainComponents.Lookup(id, search)) {
        if (search)
            delete search;

        m_TerrainComponents.RemoveKey(id);

        return TRUE;
    }

    return FALSE;
}

BOOL CGlobalEnv::DeleteTerrainCondition(const int id)
{
    CTerrainCondition *search = NULL;

    if (m_TerrainConditions.Lookup(id, search)) {
        if (search)
            delete search;

        m_TerrainConditions.RemoveKey(id);

        return TRUE;
    }

    return FALSE;
}

BOOL CGlobalEnv::DeleteWave(const int id)
{
    CWave *search = NULL;

    if (m_Waves.Lookup(id, search)) {
        if (search)
            delete search;

        m_Waves.RemoveKey(id);

        return TRUE;
    }

    return FALSE;
}

BOOL CGlobalEnv::DeleteWeatherLayer(const int id)
{
    CWeather *search = NULL;

    if (m_WeatherLayers.Lookup(id, search)) {
        if (search)
            delete search;

        m_WeatherLayers.RemoveKey(id);

        return TRUE;
    }

    return FALSE;
}

void CGlobalEnv::ClearCelestialComponents(void)
{
    int id = 0;
    CComponent *comp = NULL;
    POSITION pos = m_CelestialComponents.GetStartPosition();
    while (pos) {
        m_CelestialComponents.GetNextAssoc(pos, id, comp);
        delete comp;
    }

    m_CelestialComponents.RemoveAll();
}

void CGlobalEnv::ClearAtmosphereComponents(void)
{
    int id = 0;
    CComponent *comp = NULL;
    POSITION pos = m_AtmosphereComponents.GetStartPosition();
    while (pos) {
        m_AtmosphereComponents.GetNextAssoc(pos, id, comp);
        delete comp;
    }

    m_AtmosphereComponents.RemoveAll();
}

void CGlobalEnv::ClearSeaSurfaceComponents(void)
{
    int id = 0;
    CComponent *comp = NULL;
    POSITION pos = m_SeaSurfaceComponents.GetStartPosition();
    while (pos) {
        m_SeaSurfaceComponents.GetNextAssoc(pos, id, comp);
        delete comp;
    }

    m_SeaSurfaceComponents.RemoveAll();
}

void CGlobalEnv::ClearTerrainComponents(void)
{
    int id = 0;
    CComponent *comp = NULL;
    POSITION pos = m_TerrainComponents.GetStartPosition();
    while (pos) {
        m_TerrainComponents.GetNextAssoc(pos, id, comp);
        delete comp;
    }

    m_TerrainComponents.RemoveAll();
}

void CGlobalEnv::ClearTerrainConditions(void)
{
    int id = 0;
    CTerrainCondition *comd = NULL;
    POSITION pos = m_TerrainConditions.GetStartPosition();
    while (pos) {
        m_TerrainConditions.GetNextAssoc(pos, id, comd);
        delete comd;
    }

    m_TerrainConditions.RemoveAll();
}

void CGlobalEnv::ClearWaves(void)
{
    int id = 0;
    CWave *wave = NULL;
    POSITION pos = m_Waves.GetStartPosition();
    while (pos) {
        m_Waves.GetNextAssoc(pos, id, wave);
        delete wave;
    }

    m_Waves.RemoveAll();
}

void CGlobalEnv::ClearWeatherLayers(void)
{
    int id = 0;
    CWeather *weather = NULL;
    POSITION pos = m_WeatherLayers.GetStartPosition();
    while (pos) {
        m_WeatherLayers.GetNextAssoc(pos, id, weather);
        delete weather;
    }

    m_WeatherLayers.RemoveAll();
}