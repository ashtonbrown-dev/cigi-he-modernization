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
 *  FILENAME:   ConfigTemplGlobalEnv.cpp
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

#include "ConfigTemplInternal.h"
#include "ConfigTemplGlobalEnv.h"
#include "globals.h"

///////////////////////////////////////////////////////////////////////////////
//
//  TEMPL_GLOBAL_ENV
//
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(TEMPL_GLOBAL_ENV, CObject, 0)

TEMPL_GLOBAL_ENV::TEMPL_GLOBAL_ENV()
{
    Hour = 0;
    Minute = 0;
    Month = 1;
    Day = 1;
    Year = 2004;
    EphemerisEnable = 0;
    SunEnable = 0;
    MoonEnable = 0;
    StarfieldEnable = 0;
    StarfieldIntensity = 0.0;
    AtmosphericsEnable = 0;
    Humidity = 0;
    BaroPressure = 0.0;
    Temperature = 0.0;
    Visibility = 0.0;
    HorzWindspeed = 0.0;
    VertWindspeed = 0.0;
    WindDirection = 0.0;
    AtmosphericsEnable = 0;
    Humidity = 0;
    BaroPressure = 0.0;
    Temperature = 0.0;
    Visibility = 0.0;
    HorzWindspeed = 0.0;
    VertWindspeed = 0.0;
    WindDirection = 0.0;
    SeaSurfaceEnable = 0;
    WhitecapEnable = 0;
    SeaSurfaceHeight = 0.0;
    SurfaceTemperature = 0.0;
    SurfaceClarity = 0.0;
    reserved1 = 0;
    reserved2 = 0;
}

TEMPL_GLOBAL_ENV::~TEMPL_GLOBAL_ENV()
{
}

TEMPL_GLOBAL_ENV &TEMPL_GLOBAL_ENV::operator=(const TEMPL_GLOBAL_ENV &rhs)
{
    unsigned long handle = NULL;
    TEMPL_COMPONENT *comp = NULL;
    TEMPL_WEATHER *weather = NULL;

    Hour = rhs.Hour;
    Minute = rhs.Minute;
    Month = rhs.Month;
    Day = rhs.Day;
    Year = rhs.Year;
    EphemerisEnable = rhs.EphemerisEnable;
    SunEnable = rhs.SunEnable;
    MoonEnable = rhs.MoonEnable;
    StarfieldEnable = rhs.StarfieldEnable;
    StarfieldIntensity = rhs.StarfieldIntensity;
    AtmosphericsEnable = rhs.AtmosphericsEnable;
    Humidity = rhs.Humidity;
    BaroPressure = rhs.BaroPressure;
    Temperature = rhs.Temperature;
    Visibility = rhs.Visibility;
    HorzWindspeed = rhs.HorzWindspeed;
    VertWindspeed = rhs.VertWindspeed;
    WindDirection = rhs.WindDirection;
    SeaSurfaceEnable = rhs.SeaSurfaceEnable;
    WhitecapEnable = rhs.WhitecapEnable;
    SeaSurfaceHeight = rhs.SeaSurfaceHeight;
    SurfaceTemperature = rhs.SurfaceTemperature;
    SurfaceClarity = rhs.SurfaceClarity;

    ComponentList.RemoveAll();
    comp = rhs.ComponentList.GetHead(&handle);
    while (comp) {
        ComponentList.InsertTail(*comp);
        comp = rhs.ComponentList.GetNext(&handle);
    }

    WeatherList.RemoveAll();
    weather = rhs.WeatherList.GetHead(&handle);
    while (weather) {
        WeatherList.InsertTail(*weather);
        weather = rhs.WeatherList.GetNext(&handle);
    }

    return *this;
}

// Binary only.
void TEMPL_GLOBAL_ENV::Serialize(CArchive &ar)
{
    TRACE0("\n--  TEMPL_GLOBAL_ENV::Serialize()\n");

    unsigned long handle = NULL;
    int length = 0;
    int count = 0;

    // Call base class Serialize() to make sure all inherited
    // members are saved/loaded.
    CObject::Serialize(ar);

    if (ar.IsStoring()) {
        TEMPL_COMPONENT *comp = NULL;
        TEMPL_WEATHER *weather = NULL;

        // Store the integral types.
        ar << Hour;
        ar << Minute;
        ar << Month;
        ar << Day;
        ar << Year;
        ar << EphemerisEnable;
        ar << SunEnable;
        ar << MoonEnable;
        ar << StarfieldEnable;
        ar << StarfieldIntensity;
        ar << AtmosphericsEnable;
        ar << Humidity;
        ar << BaroPressure;
        ar << Temperature;
        ar << Visibility;
        ar << HorzWindspeed;
        ar << VertWindspeed;
        ar << WindDirection;
        ar << SeaSurfaceEnable;
        ar << WhitecapEnable;
        ar << SeaSurfaceHeight;
        ar << SurfaceTemperature;
        ar << SurfaceClarity;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("	Hour = %d\n", Hour);
        TRACE1("	Minute = %d\n", Minute);
        TRACE1("	Month = %d\n", Month);
        TRACE1("	Day = %d\n", Day);
        TRACE1("	Year = %d\n", Year);
        TRACE1("	EphemerisEnable = %d\n", EphemerisEnable);
        TRACE1("	SunEnable = %d\n", SunEnable);
        TRACE1("	MoonEnable = %d\n", MoonEnable);
        TRACE1("	StarfieldEnable = %d\n", StarfieldEnable);
        TRACE1("	StarfieldIntensity = %f\n", StarfieldIntensity);
        TRACE1("	AtmosphericsEnable = %d\n", AtmosphericsEnable);
        TRACE1("	Humidity = %d\n", Humidity);
        TRACE1("	BaroPressure = %f\n", BaroPressure);
        TRACE1("	Temperature = %f\n", Temperature);
        TRACE1("	Visibility = %f\n", Visibility);
        TRACE1("	HorzWindspeed = %f\n", HorzWindspeed);
        TRACE1("	VertWindspeed = %fn", VertWindspeed);
        TRACE1("	WindDirection = %f\n", WindDirection);
        TRACE1("	SeaSurfaceEnable = %f\n", SeaSurfaceEnable);
        TRACE1("	WhitecapEnable; = %f\n", WhitecapEnable);
        TRACE1("	SeaSurfaceHeight = %f\n", SeaSurfaceHeight);
        TRACE1("	SurfaceTemperature = %f\n", SurfaceTemperature);
        TRACE1("	SurfaceClarity = %f\n", SurfaceClarity);

        // Store the components.
        count = ComponentList.GetItemCount();
        ar << count;

        TRACE1("	component count = %d\n", count);

        if (count) {
            comp = ComponentList.GetHead(&handle);
            while (comp) {
                comp->Serialize(ar);
                comp = ComponentList.GetNext(&handle);
            }
        }

        // Store the weather layers.
        count = WeatherList.GetItemCount();
        ar << count;

        TRACE1("	weather count = %d\n", count);

        if (count) {
            weather = WeatherList.GetHead(&handle);
            while (weather) {
                weather->Serialize(ar);
                weather = WeatherList.GetNext(&handle);
            }
        }
    } else {
        ar >> Hour;
        ar >> Minute;
        ar >> Month;
        ar >> Day;
        ar >> Year;
        ar >> EphemerisEnable;
        ar >> SunEnable;
        ar >> MoonEnable;
        ar >> StarfieldEnable;
        ar >> StarfieldIntensity;
        ar >> AtmosphericsEnable;
        ar >> Humidity;
        ar >> BaroPressure;
        ar >> Temperature;
        ar >> Visibility;
        ar >> HorzWindspeed;
        ar >> VertWindspeed;
        ar >> WindDirection;
        ar >> SeaSurfaceEnable;
        ar >> WhitecapEnable;
        ar >> SeaSurfaceHeight;
        ar >> SurfaceTemperature;
        ar >> SurfaceClarity;

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("	Hour = %d\n", Hour);
        TRACE1("	Minute = %d\n", Minute);
        TRACE1("	Month = %d\n", Month);
        TRACE1("	Day = %d\n", Day);
        TRACE1("	Year = %d\n", Year);
        TRACE1("	EphemerisEnable = %d\n", EphemerisEnable);
        TRACE1("	SunEnable = %d\n", SunEnable);
        TRACE1("	MoonEnable = %d\n", MoonEnable);
        TRACE1("	StarfieldEnable = %d\n", StarfieldEnable);
        TRACE1("	StarfieldIntensity = %f\n", StarfieldIntensity);
        TRACE1("	AtmosphericsEnable = %d\n", AtmosphericsEnable);
        TRACE1("	Humidity = %d\n", Humidity);
        TRACE1("	BaroPressure = %f\n", BaroPressure);
        TRACE1("	Temperature = %f\n", Temperature);
        TRACE1("	Visibility = %f\n", Visibility);
        TRACE1("	HorzWindspeed = %f\n", HorzWindspeed);
        TRACE1("	VertWindspeed = %fn", VertWindspeed);
        TRACE1("	WindDirection = %f\n", WindDirection);
        TRACE1("	SeaSurfaceEnable = %f\n", SeaSurfaceEnable);
        TRACE1("	WhitecapEnable; = %f\n", WhitecapEnable);
        TRACE1("	SeaSurfaceHeight = %f\n", SeaSurfaceHeight);
        TRACE1("	SurfaceTemperature = %f\n", SurfaceTemperature);
        TRACE1("	SurfaceClarity = %f\n", SurfaceClarity);

        // Read in the components.
        ar >> count;

        TRACE1("	component count = %d\n", count);

        for (int i = 0; i < count; i++) {
            TEMPL_COMPONENT comp;
            comp.Serialize(ar);
            ComponentList.InsertTail(comp);
        }

        // Read in the weather.
        ar >> count;

        TRACE1("	weather count = %d\n", count);

        for (int i = 0; i < count; i++) {
            TEMPL_WEATHER weather;
            weather.Serialize(ar);
            WeatherList.InsertTail(weather);
        }
    }
}

// Text only.
BOOL TEMPL_GLOBAL_ENV::WriteText(CStdioFile &file)
{
    unsigned long handle = 0;
    CString str;
    TEMPL_COMPONENT *comp = NULL;
    TEMPL_WEATHER *weather = NULL;

    // Write the header and opening brace.
    file.WriteString("\nGLOBAL_ENVIRONMENT{\n");

    // Store the values.
    str.Format("\tDATE                    = %02d/%02d/%04d;\n", Month, Day, Year);
    file.WriteString((LPCTSTR)str);

    str.Format("\tTIME                    = %02d:02d;\n", Hour, Minute);
    file.WriteString((LPCTSTR)str);

    str.Format("\tEPHEMERIS_ENABLE        = %s;\n", EphemerisEnable ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    str.Format("\tATMOSPHERE_MODEL_ENABLE = %s;\n", AtmosphericsEnable ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    str.Format("\tSUN_ENABLE              = %s;\n", SunEnable ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    str.Format("\tMOON_ENABLE             = %s;\n", MoonEnable ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    str.Format("\tSTARFIELD_ENABLE        = %s;\n", StarfieldEnable ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    str.Format("\tSTARFIELD_INTENSITY     = %f;\n", StarfieldIntensity);
    file.WriteString((LPCTSTR)str);

    str.Format("\tBAROMETRIC_PRESSURE     = %f;\n", BaroPressure);
    file.WriteString((LPCTSTR)str);

    str.Format("\tHUMIDITY                = %d;\n", Humidity);
    file.WriteString((LPCTSTR)str);

    str.Format("\tTEMPERATURE             = %f;\n", Temperature);
    file.WriteString((LPCTSTR)str);

    str.Format("\tVISIBILITY              = %f;\n", Visibility);
    file.WriteString((LPCTSTR)str);

    str.Format("\tHORIZONTAL_WINDSPEED    = %f;\n", HorzWindspeed);
    file.WriteString((LPCTSTR)str);

    str.Format("\tVERTICAL_WINDSPEED      = %f;\n", VertWindspeed);
    file.WriteString((LPCTSTR)str);

    str.Format("\tWIND_DIRECTION          = %f;\n", WindDirection);
    file.WriteString((LPCTSTR)str);

    //LWD:  Do Maritime and Terrain Surface Conditions

    // Write each component.
    comp = ComponentList.GetHead(&handle);
    while (comp) {
        comp->WriteText(file);
        comp = ComponentList.GetNext(&handle);
    }

    // Write each weather layer.
    weather = WeatherList.GetHead(&handle);
    while (weather) {
        weather->WriteText(file);
        weather = WeatherList.GetNext(&handle);
    }

    // Write the closing brace.
    file.WriteString("}\n\n");

    return TRUE;
}

// Text only.
BOOL TEMPL_GLOBAL_ENV::ReadText(CString &src, int *linenum)
{
    BOOL readstatus;
    CString str, temp, token, value;

    // We should be starting just inside the opening brace.

    // Get the entity block.
    str = ReadBlock(src, linenum);

    // Read and process each line.
    readstatus = ReadLine(str, linenum, token, value);
    while ((readstatus == TRUE) && (token != "}")) {
        if (token == "DATE") {
            MakeDate(value, &Month, &Day, &Year);
        } else if (token == "TIME") {
            MakeTime(value, &Hour, &Minute);
        } else if (token == "EPHEMERIS_ENABLE") {
            EphemerisEnable = StrToBool(value);
        } else if (token == "ATMOSPHERE_MODEL_ENABLE") {
            AtmosphericsEnable = StrToBool(value);
        } else if (token == "SUN_ENABLE") {
            SunEnable = StrToBool(value);
        } else if (token == "MOON_ENABLE") {
            MoonEnable = StrToBool(value);
        } else if (token == "STARFIELD_ENABLE") {
            StarfieldEnable = StrToBool(value);
        } else if (token == "STARFIELD_INTENSITY") {
            StarfieldIntensity = (float)atof(value);
        } else if (token == "BAROMETRIC_PRESSURE") {
            BaroPressure = (float)atof(value);
        } else if (token == "HUMIDITY") {
            Humidity = atoi(value);
        } else if (token == "TEMPERATURE") {
            Temperature = (float)atof(value);
        } else if (token == "VISIBILITY") {
            Visibility = (float)atof(value);
        } else if (token == "HORIZONTAL_WINDSPEED") {
            HorzWindspeed = (float)atof(value);
        } else if (token == "VERTICAL_WINDSPEED") {
            VertWindspeed = (float)atof(value);
        } else if (token == "WIND_DIRECTION") {
            WindDirection = (float)atof(value);
        }
        //LWD:  Do Maritime and Terrain Surface Conditions
        else if (token == "COMPONENT") {
            TEMPL_COMPONENT component;
            component.ReadText(str, linenum);

            ComponentList.InsertTail(component);
        } else if (token == "WEATHER_LAYER") {
            TEMPL_WEATHER weather;
            weather.ReadText(str, linenum);
            weather.Scope = SCOPE_GLOBAL;

            WeatherList.InsertTail(weather);
        } else {
            PopupReadErrorMsg(KEYWORD, (LPCTSTR)token, linenum);
        }

        readstatus = ReadLine(str, linenum, token, value);
    }

    return readstatus;
}

TEMPL_COMPONENT *TEMPL_GLOBAL_ENV::FindComponent(const int id)
{
    unsigned long handle = 0;
    TEMPL_COMPONENT *comp = ComponentList.GetHead(&handle);

    while (comp) {
        if (comp->ID == id)
            return comp;

        comp = ComponentList.GetNext(&handle);
    }

    return NULL;
}

TEMPL_COMPONENT_STATE *TEMPL_GLOBAL_ENV::FindComponentState(const int component_id, const int comp_value)
{
    TEMPL_COMPONENT *comp = FindComponent(component_id);
    if (comp)
        return comp->FindState(comp_value);

    return NULL;
}

TEMPL_WEATHER *TEMPL_GLOBAL_ENV::FindWeatherLayer(const int id)
{
    unsigned long handle = 0;
    TEMPL_WEATHER *weather = WeatherList.GetHead(&handle);

    while (weather) {
        if (weather->LayerID == id)
            return weather;

        weather = WeatherList.GetNext(&handle);
    }

    return NULL;
}
