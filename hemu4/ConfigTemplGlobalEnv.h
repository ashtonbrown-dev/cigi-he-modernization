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

#ifndef _CONFIGTEMPLGLOBAL_ENV_H_
#define _CONFIGTEMPLGLOBAL_ENV_H_

#include <cstdint>

#include "StdAfx.h"
#include "linkedlist.h"
#include "ConfigTemplComponent.h"
#include "ConfigTemplWeather.h"

class TEMPL_GLOBAL_ENV : public CObject
{
public:
    DECLARE_SERIAL(TEMPL_GLOBAL_ENV)

    TEMPL_GLOBAL_ENV();
    ~TEMPL_GLOBAL_ENV();
    TEMPL_GLOBAL_ENV &operator=(const TEMPL_GLOBAL_ENV &rhs);
    BOOL ReadText(CString &src, int *linenum);
    BOOL WriteText(CStdioFile &file);
    void Serialize(CArchive &ar);
    TEMPL_COMPONENT *FindComponent(const int id);
    TEMPL_COMPONENT_STATE *FindComponentState(const int component_id, const int value);
    TEMPL_WEATHER *FindWeatherLayer(const int id);

    // Celestial Sphere Properties
    unsigned char Hour;
    unsigned char Minute;
    unsigned char Month;
    unsigned char Day;
    int Year;
    char EphemerisEnable;
    char SunEnable;
    char MoonEnable;
    char StarfieldEnable;
    float StarfieldIntensity;

    // Atmospheric Properties
    char AtmosphericsEnable;
    char reserved1;
    uint8_t Humidity;
    float BaroPressure;
    float Temperature;
    float Visibility;
    float HorzWindspeed;
    float VertWindspeed;
    float WindDirection;

    // Maritime Surface Conditions
    char SeaSurfaceEnable;
    char WhitecapEnable;
    short reserved2;
    float SeaSurfaceHeight;
    float SurfaceTemperature;
    float SurfaceClarity;

    CLinkedList<TEMPL_COMPONENT> ComponentList;
    CLinkedList<TEMPL_WEATHER> WeatherList;
};

#endif