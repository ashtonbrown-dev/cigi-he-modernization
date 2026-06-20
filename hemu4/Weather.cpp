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

// Weather.cpp: implementation of the CWeather class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "Weather.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CWeather, CObject, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeather::CWeather()
{
    m_HasChanged = FALSE;
    m_Htree = NULL;
    m_LayerID = -1;
    m_OwnerID = 0;
    m_Scope = -1;
    m_Enable = 0;
    m_TopScudEnable = 0;
    m_BottomScudEnable = 0;
    m_RandomWindEnable = 0;
    m_RandomLightningEnable = 0;
    m_CloudType = 0;
    m_Severity = 0;
    m_Humidity = 0;
    m_Temperature = 0.0;
    m_Visibility = 0.0;
    m_TopScudFreq = 0.0;
    m_BottomScudFreq = 0.0;
    m_Coverage = 0.0;
    m_BaseElevation = 0.0;
    m_Thickness = 0.0;
    m_TopTransitionBand = 0.0;
    m_BottomTransitionBand = 0.0;
    m_HorzWindspeed = 0.0;
    m_VertWindspeed = 0.0;
    m_WindDirection = 0.0;
    m_BaroPressure = 0.0;
    m_AerosolConcentration = 0.0;

    // Initialize the CMap.
    m_Components.RemoveAll();
    m_Components.InitHashTable(521);
}

CWeather::CWeather(const TEMPL_WEATHER *templ)
{
    m_HasChanged = FALSE;
    m_Htree = NULL;
    m_OwnerID = 0;
    m_Name = templ->Name;
    m_LayerID = templ->LayerID;
    m_Scope = templ->Scope;
    m_Enable = templ->Enable;
    m_TopScudEnable = templ->TopScudEnable;
    m_BottomScudEnable = templ->BottomScudEnable;
    m_RandomWindEnable = templ->RandomWindEnable;
    m_RandomLightningEnable = templ->RandomLightningEnable;
    m_CloudType = templ->CloudType;
    m_Severity = templ->Severity;
    m_Humidity = templ->Humidity;
    m_Temperature = templ->Temperature;
    m_Visibility = templ->Visibility;
    m_TopScudFreq = templ->TopScudFreq;
    m_BottomScudFreq = templ->BottomScudFreq;
    m_Coverage = templ->Coverage;
    m_BaseElevation = templ->BaseElevation;
    m_Thickness = templ->Thickness;
    m_TopTransitionBand = templ->TopTransitionBand;
    m_BottomTransitionBand = templ->BottomTransitionBand;
    m_HorzWindspeed = templ->HorzWindspeed;
    m_VertWindspeed = templ->VertWindspeed;
    m_WindDirection = templ->WindDirection;
    m_BaroPressure = templ->BaroPressure;
    m_AerosolConcentration = templ->AerosolConcentration;

    // Initialize the CMap.
    m_Components.RemoveAll();
    m_Components.InitHashTable(521);

    int comp_class = -1;
    switch (m_Scope) {
    case SCOPE_GLOBAL:
        comp_class = COMP_CLASS_G_WEATHER;
        break;

    case SCOPE_REGIONAL:
        comp_class = COMP_CLASS_R_WEATHER;
        break;

    case SCOPE_ENTITY:
        comp_class = COMP_CLASS_ENTITY;
        break;

    default:
        return;
    }

    // Build the component list.
    unsigned long handle = 0;
    TEMPL_COMPONENT *comp_templ = templ->ComponentList.GetHead(&handle);
    while (comp_templ) {
        CComponent *comp = new CComponent(comp_class, comp_templ);

        AddComponent(comp);

        comp_templ = templ->ComponentList.GetNext(&handle);
    }
}

CWeather::~CWeather()
{
}

CWeather &CWeather::operator=(const CWeather &rhs)
{
    m_HasChanged = TRUE;

    m_Name = rhs.m_Name;
    m_LayerID = rhs.m_LayerID;
    m_OwnerID = rhs.m_OwnerID;
    m_Scope = rhs.m_Scope;
    m_Enable = rhs.m_Enable;
    m_TopScudEnable = rhs.m_TopScudEnable;
    m_BottomScudEnable = rhs.m_BottomScudEnable;
    m_RandomWindEnable = rhs.m_RandomWindEnable;
    m_RandomLightningEnable = rhs.m_RandomLightningEnable;
    m_CloudType = rhs.m_CloudType;
    m_Severity = rhs.m_Severity;
    m_Humidity = rhs.m_Humidity;
    m_Temperature = rhs.m_Temperature;
    m_Visibility = rhs.m_Visibility;
    m_TopScudFreq = rhs.m_TopScudFreq;
    m_BottomScudFreq = rhs.m_BottomScudFreq;
    m_Coverage = rhs.m_Coverage;
    m_BaseElevation = rhs.m_BaseElevation;
    m_Thickness = rhs.m_Thickness;
    m_TopTransitionBand = rhs.m_TopTransitionBand;
    m_BottomTransitionBand = rhs.m_BottomTransitionBand;
    m_HorzWindspeed = rhs.m_HorzWindspeed;
    m_VertWindspeed = rhs.m_VertWindspeed;
    m_WindDirection = rhs.m_WindDirection;
    m_BaroPressure = rhs.m_BaroPressure;
    m_AerosolConcentration = rhs.m_AerosolConcentration;

    return *this;
}

void CWeather::Serialize(CArchive &ar)
{
    TRACE0("\nCWeather::Serialize()\n");

    int length = 0;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)m_Name);
        ar << length;
        ar.Write((LPCTSTR)m_Name, length);  // NULL terminator not copied

        ar << m_LayerID;
        ar << m_OwnerID;
        ar << m_Scope;
        ar << m_Enable;
        ar << m_TopScudEnable;
        ar << m_BottomScudEnable;
        ar << m_RandomWindEnable;
        ar << m_RandomLightningEnable;
        ar << m_CloudType;
        ar << m_Severity;
        ar << m_Humidity;
        ar << m_Temperature;
        ar << m_Visibility;
        ar << m_TopScudFreq;
        ar << m_BottomScudFreq;
        ar << m_Coverage;
        ar << m_BaseElevation;
        ar << m_Thickness;
        ar << m_TopTransitionBand;
        ar << m_BottomTransitionBand;
        ar << m_HorzWindspeed;
        ar << m_VertWindspeed;
        ar << m_WindDirection;
        ar << m_BaroPressure;
        ar << m_AerosolConcentration;

        // Store the components.
        m_Components.Serialize(ar);

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("	length = %d\n", length);
        TRACE1("	Name = \"%s\"\n", (LPCTSTR)m_Name);
    } else {
        // Read the name.
        ar >> length;
        ar.Read(m_Name.GetBuffer(length), length);
        m_Name.ReleaseBuffer(length);

        ar >> m_LayerID;
        ar >> m_OwnerID;
        ar >> m_Scope;
        ar >> m_Enable;
        ar >> m_TopScudEnable;
        ar >> m_BottomScudEnable;
        ar >> m_RandomWindEnable;
        ar >> m_RandomLightningEnable;
        ar >> m_CloudType;
        ar >> m_Severity;
        ar >> m_Humidity;
        ar >> m_Temperature;
        ar >> m_Visibility;
        ar >> m_TopScudFreq;
        ar >> m_BottomScudFreq;
        ar >> m_Coverage;
        ar >> m_BaseElevation;
        ar >> m_Thickness;
        ar >> m_TopTransitionBand;
        ar >> m_BottomTransitionBand;
        ar >> m_HorzWindspeed;
        ar >> m_VertWindspeed;
        ar >> m_WindDirection;
        ar >> m_BaroPressure;
        ar >> m_AerosolConcentration;

        // Read the components.
        m_Components.Serialize(ar);

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        m_HasChanged = TRUE;

        TRACE1("	length = %d\n", length);
        TRACE1("	Name = \"%s\"\n", (LPCTSTR)m_Name);
    }
}


CComponent *CWeather::GetComponent(const int id)
{
    CComponent *search = NULL;

    m_Components.Lookup(id, search);

    return search;
}

BOOL CWeather::AddComponent(CComponent *comp)
{
    BOOL retval = FALSE;
    int id = comp->GetID();
    CComponent *search = NULL;

    // If the component already exists, delete it.
    if (m_Components.Lookup(id, search)) {
        //LWD: we need to preserve the states and revert to default if they don't match

        delete search;
        retval = TRUE;      // return TRUE if component exists
    }

    m_Components.SetAt(id, comp);

    m_HasChanged = TRUE;

    return retval;
}

BOOL CWeather::DeleteComponent(const int id)
{
    CComponent *search = NULL;

    if (m_Components.Lookup(id, search)) {
        if (search)
            delete search;

        m_Components.RemoveKey(id);

        m_HasChanged = TRUE;

        return TRUE;
    }

    return FALSE;
}

void CWeather::ClearComponents(void)
{
    int id = 0;
    CComponent *comp = NULL;
    POSITION pos = m_Components.GetStartPosition();
    while (pos) {
        m_Components.GetNextAssoc(pos, id, comp);
        delete comp;
    }

    m_Components.RemoveAll();

    m_HasChanged = TRUE;
}

void CWeather::CreateCigiPacket(void *buffer)
{
    CIGI_WEATHER_CONTROL *packet = (CIGI_WEATHER_CONTROL *)buffer;

    packet->packet_id = CIGI_WEATHER_CONTROL_OPCODE;
    packet->packet_size = sizeof(CIGI_WEATHER_CONTROL);
    packet->entity_region_id = GetOwnerID();
    packet->layer_id = GetLayerID();
    packet->humidity = GetHumidity();
    packet->weather_enable = GetWeatherEnable() ? 1 : 0;
    packet->top_scud_enable = GetTopScudEnable() ? 1 : 0;
    packet->bottom_scud_enable = GetBottomScudEnable() ? 1 : 0;
    packet->random_winds = GetRandomWindEnable() ? 1 : 0;
    packet->random_lightning = GetRandomLightningEnable() ? 1 : 0;
    packet->cloud_type = GetCloudType();
    packet->scope = GetScope();
    packet->severity = GetSeverity();
    packet->air_temp = GetTemperature();
    packet->visibility = GetVisibility();
    packet->top_scud_freq = GetTopScudFreq();
    packet->bottom_scud_freq = GetBottomScudFreq();
    packet->coverage = GetCoverage();
    packet->base_elevation = GetBaseElevation();
    packet->thickness = GetThickness();
    packet->top_transition_band = GetTopTransitionBand();
    packet->bottom_transition_band = GetBottomTransitionBand();
    packet->windspeed_horz = GetHorzWindspeed();
    packet->windspeed_vert = GetVertWindspeed();
    packet->wind_direction = GetWindDirection();
    packet->baro_pressure = GetBaroPressure();
    packet->aerosol_concentration = GetAerosolConcentration();
}

template <> void AFXAPI SerializeElements<CWeather *>(CArchive &ar, CWeather **pElements, int nCount)
{
    if (ar.IsStoring()) {
        for (int i = 0; i < nCount; i++) {
            if (pElements[i])
                pElements[i]->Serialize(ar);
        }
    } else {
        for (int i = 0; i < nCount; i++) {
            TEMPL_WEATHER dummy;
            pElements[i] = new CWeather(&dummy);
            pElements[i]->Serialize(ar);
        }
    }
}
