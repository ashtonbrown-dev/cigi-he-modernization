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
 *  FILENAME:   ConfigTemplWeather.cpp
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
#include "ConfigTemplWeather.h"
#include "globals.h"

///////////////////////////////////////////////////////////////////////////////
//
//  TEMPL_WEATHER
//
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(TEMPL_WEATHER, CObject, 0)

TEMPL_WEATHER::TEMPL_WEATHER()
{
    LayerID = -1;
    Scope = 0;
    Enable = 0;
    TopScudEnable = 0;
    BottomScudEnable = 0;
    RandomWindEnable = 0;
    RandomLightningEnable = 0;
    CloudType = 0;
    Severity = 0;
    Humidity = 0;
    Temperature = 0.0;
    Visibility = 0.0;
    TopScudFreq = 0.0;
    BottomScudFreq = 0.0;
    Coverage = 0.0;
    BaseElevation = 0.0;
    Thickness = 0.0;
    TopTransitionBand = 0.0;
    BottomTransitionBand = 0.0;
    HorzWindspeed = 0.0;
    VertWindspeed = 0.0;
    WindDirection = 0.0;
    BaroPressure = 0.0;
    AerosolConcentration = 0.0;
}


TEMPL_WEATHER::~TEMPL_WEATHER()
{
}

TEMPL_WEATHER &TEMPL_WEATHER::operator=(const TEMPL_WEATHER &rhs)
{
    Name = rhs.Name;
    LayerID = rhs.LayerID;
    Scope = rhs.Scope;
    Enable = rhs.Enable;
    TopScudEnable = rhs.TopScudEnable;
    BottomScudEnable = rhs.BottomScudEnable;
    RandomWindEnable = rhs.RandomWindEnable;
    RandomLightningEnable = rhs.RandomLightningEnable;
    CloudType = rhs.CloudType;
    Severity = rhs.Severity;
    Humidity = rhs.Humidity;
    Temperature = rhs.Temperature;
    Visibility = rhs.Visibility;
    TopScudFreq = rhs.TopScudFreq;
    BottomScudFreq = rhs.BottomScudFreq;
    Coverage = rhs.Coverage;
    BaseElevation = rhs.BaseElevation;
    Thickness = rhs.Thickness;
    TopTransitionBand = rhs.TopTransitionBand;
    BottomTransitionBand = rhs.BottomTransitionBand;
    HorzWindspeed = rhs.HorzWindspeed;
    VertWindspeed = rhs.VertWindspeed;
    WindDirection = rhs.WindDirection;
    BaroPressure = rhs.BaroPressure;
    AerosolConcentration = rhs.AerosolConcentration;

    ComponentList.RemoveAll();
    unsigned long handle = NULL;
    TEMPL_COMPONENT *comp = rhs.ComponentList.GetHead(&handle);
    while (comp) {
        ComponentList.InsertTail(*comp);
        comp = rhs.ComponentList.GetNext(&handle);
    }

    return *this;
}

// Binary only.
void TEMPL_WEATHER::Serialize(CArchive &ar)
{
    TRACE0("\n--  TEMPL_WEATHER::Serialize()\n");

    unsigned long handle = NULL;
    int length = 0;
    int count = 0;

    // Call base class Serialize() to make sure all inherited
    // members are saved/loaded.
    CObject::Serialize(ar);

    if (ar.IsStoring()) {
        TEMPL_COMPONENT *comp = NULL;

        // Store the name.
        length = strlen((LPCTSTR)Name);
        ar << length;
        ar.Write((LPCTSTR)Name, length);    // NULL terminator not copied

        // Store the integral types.
        ar << LayerID;
        ar << Scope;
        ar << Enable;
        ar << TopScudEnable;
        ar << BottomScudEnable;
        ar << RandomWindEnable;
        ar << RandomLightningEnable;
        ar << CloudType;
        ar << Severity;
        ar << Humidity;
        ar << Temperature;
        ar << Visibility;
        ar << TopScudFreq;
        ar << BottomScudFreq;
        ar << Coverage;
        ar << BaseElevation;
        ar << Thickness;
        ar << TopTransitionBand;
        ar << BottomTransitionBand;
        ar << HorzWindspeed;
        ar << VertWindspeed;
        ar << WindDirection;
        ar << BaroPressure;
        ar << AerosolConcentration;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("	Layer ID = %d\n", LayerID);
        TRACE1("	Scope = %d\n", Scope);
        TRACE1("	Weather Enable = %d\n", Enable);
        TRACE1("	Top Scud Enable = %d\n", TopScudEnable);
        TRACE1("	Bottom Scud Enable = %d\n", BottomScudEnable);
        TRACE1("	Random Wind Enable = %d\n", RandomWindEnable);
        TRACE1("	Random Lightning Enable = %d\n", RandomLightningEnable);
        TRACE1("	Cloud Type = %d\n", CloudType);
        TRACE1("	Severity = %d\n", Severity);
        TRACE1("	Humidity = %d\n", Humidity);
        TRACE1("	Temperature = %d\n", Temperature);
        TRACE1("	Visibility = %f\n", Visibility);
        TRACE1("	Top Scud Freqency = %f\n", TopScudFreq);
        TRACE1("	bottom Scud Freqency = %f\n", BottomScudFreq);
        TRACE1("	Coverage = %f\n", Coverage);
        TRACE1("	Base Elevation = %f\n", BaseElevation);
        TRACE1("	Thickness = %f\n", Thickness);
        TRACE1("	TopTransition Band = %f\n", TopTransitionBand);
        TRACE1("	BottomTransition Band = %f\n", BottomTransitionBand);
        TRACE1("	Horizontal Windspeed = %f\n", HorzWindspeed);
        TRACE1("	Vertical Windspeed = %f\n", VertWindspeed);
        TRACE1("	Wind Direction = %f\n", WindDirection);
        TRACE1("	Barometric Pressure = %f\n", BaroPressure);
        TRACE1("	Aerosol Concentration = %f\n", AerosolConcentration);

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
    } else {
        // Read the name.
        ar >> length;
        ar.Read(Name.GetBuffer(length), length);
        Name.ReleaseBuffer(length);

        // Read the integral types.
        ar >> LayerID;
        ar >> Scope;
        ar >> Enable;
        ar >> TopScudEnable;
        ar >> BottomScudEnable;
        ar >> RandomWindEnable;
        ar >> RandomLightningEnable;
        ar >> CloudType;
        ar >> Severity;
        ar >> Humidity;
        ar >> Temperature;
        ar >> Visibility;
        ar >> TopScudFreq;
        ar >> BottomScudFreq;
        ar >> Coverage;
        ar >> BaseElevation;
        ar >> Thickness;
        ar >> TopTransitionBand;
        ar >> BottomTransitionBand;
        ar >> HorzWindspeed;
        ar >> VertWindspeed;
        ar >> WindDirection;
        ar >> BaroPressure;
        ar >> AerosolConcentration;

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("	Layer ID = %d\n", LayerID);
        TRACE1("	Scope = %d\n", Scope);
        TRACE1("	Weather Enable = %d\n", Enable);
        TRACE1("	Top Scud Enable = %d\n", TopScudEnable);
        TRACE1("	Bottom Scud Enable = %d\n", BottomScudEnable);
        TRACE1("	Random Wind Enable = %d\n", RandomWindEnable);
        TRACE1("	Random Lightning Enable = %d\n", RandomLightningEnable);
        TRACE1("	Cloud Type = %d\n", CloudType);
        TRACE1("	Severity = %d\n", Severity);
        TRACE1("	Humidity = %d\n", Humidity);
        TRACE1("	Temperature = %d\n", Temperature);
        TRACE1("	Visibility = %f\n", Visibility);
        TRACE1("	Top Scud Freqency = %f\n", TopScudFreq);
        TRACE1("	Bottom Scud Freqency = %f\n", BottomScudFreq);
        TRACE1("	Coverage = %f\n", Coverage);
        TRACE1("	Base Elevation = %f\n", BaseElevation);
        TRACE1("	Thickness = %f\n", Thickness);
        TRACE1("	TopTransition Band = %f\n", TopTransitionBand);
        TRACE1("	BottomTransition Band = %f\n", BottomTransitionBand);
        TRACE1("	Horizontal Windspeed = %f\n", HorzWindspeed);
        TRACE1("	Vertical Windspeed = %f\n", VertWindspeed);
        TRACE1("	Wind Direction = %f\n", WindDirection);
        TRACE1("	Barometric Pressure = %f\n", BaroPressure);
        TRACE1("	Aerosol Concentration = %f\n", AerosolConcentration);

        // Read in the components.
        ar >> count;

        TRACE1("	component count = %d\n", count);

        for (int i = 0; i < count; i++) {
            TEMPL_COMPONENT comp;
            comp.Serialize(ar);
            ComponentList.InsertTail(comp);
        }
    }
}

// Text only.
BOOL TEMPL_WEATHER::WriteText(CStdioFile &file)
{
    CString str;

    // Write the opening brace.
    switch (Scope) {
    case SCOPE_ENTITY:
        file.WriteString("\n\tWEATHER_ATTRIBUTES\n\t{\n");
        break;

    case SCOPE_REGIONAL:
    case SCOPE_GLOBAL:
    default:
        file.WriteString("\n\tWEATHER_LAYER\n\t{\n");
        break;
    }

    // Store the name and value.
    str.Format("\t\tNAME                    = \"%s\";\n", Name);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tID                      = %d;\n", LayerID);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tENABLE                  = %s;\n", Enable ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tTOP_SCUD_ENABLE             = %s;\n", TopScudEnable ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tBOTTOM_SCUD_ENABLE             = %s;\n", BottomScudEnable ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tRANDOM_WIND_ENABLE      = %s;\n", RandomWindEnable ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tRANDOM_LIGHTNING_ENABLE = %s;\n", RandomLightningEnable ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tCLOUD_TYPE              = %d;\t\\\\ See CIGI 3 ICD for value assignments\n", CloudType);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tSEVERITY                = %d;\n", Severity);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tHUMIDITY                = %d;\n", Humidity);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tTEMPERATURE             = %f;\n", Temperature);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tVISIBILITY              = %f;\n", Visibility);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tTOP_SCUD_FREQUENCY          = %f;\n", TopScudFreq);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tBOTTOM_SCUD_FREQUENCY          = %f;\n", BottomScudFreq);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tCOVERAGE                = %f;\n", Coverage);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tBASE_ELEVATION          = %f;\n", BaseElevation);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tTHICKNESS               = %f;\n", Thickness);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tTOP_TRANSITION_BAND         = %f;\n", TopTransitionBand);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tBOTTOM_TRANSITION_BAND         = %f;\n", BottomTransitionBand);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tHORZONTAL_WINDSPEED     = %f;\n", HorzWindspeed);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tVERTICAL_WINDSPEED      = %f;\n", VertWindspeed);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tWIND_DIRECTION          = %f;\n", WindDirection);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tBAROMETRIC_PRESSURE     = %f;\n", BaroPressure);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tAEROSOL_CONCENTRATION   = %f;\n", AerosolConcentration);
    file.WriteString((LPCTSTR)str);

    // Write each component.
    unsigned long handle = NULL;
    TEMPL_COMPONENT *comp = ComponentList.GetHead(&handle);
    while (comp) {
        comp->WriteText(file);
        comp = ComponentList.GetNext(&handle);
    }

    // Write the closing brace.
    file.WriteString("\t}\n");

    return TRUE;
}

// Text only.
BOOL TEMPL_WEATHER::ReadText(CString &src, int *linenum)
{
    BOOL readstatus;
    CString str, temp, token, value;

    // We should be starting just inside the opening brace.

    // Get the artpart block.
    str = ReadBlock(src, linenum);

    // Read and process each line.
    readstatus = ReadLine(str, linenum, token, value);
    while ((readstatus == TRUE) && (token != "}")) {
        if (token == "NAME") {
            // Trim the quotes.
            value.TrimLeft("\"");
            value.TrimRight("\"");
            value.TrimRight();

            Name = value;
        } else if (token == "ID") {
            LayerID = atoi((LPCTSTR)value);
        } else if (token == "ENABLE") {
            Enable = StrToBool((LPCTSTR)value);
        } else if (token == "TOP_SCUD_ENABLE") {
            TopScudEnable = StrToBool((LPCTSTR)value);
        } else if (token == "BOTTOM_SCUD_ENABLE") {
            BottomScudEnable = StrToBool((LPCTSTR)value);
        } else if (token == "RANDOM_WIND_ENABLE") {
            RandomWindEnable = StrToBool((LPCTSTR)value);
        } else if (token == "RANDOM_LIGHTNING_ENABLE") {
            RandomLightningEnable = StrToBool((LPCTSTR)value);
        } else if (token == "CLOUD_TYPE") {
            CloudType = atoi((LPCTSTR)value);
        } else if (token == "SEVERITY") {
            Severity = atoi((LPCTSTR)value);
        } else if (token == "TOP_SCUD_FREQUENCY") {
            TopScudFreq = (float)atof((LPCTSTR)value);
        } else if (token == "BOTTOM_SCUD_FREQUENCY") {
            BottomScudFreq = (float)atof((LPCTSTR)value);
        } else if (token == "COVERAGE") {
            Coverage = (float)atof((LPCTSTR)value);
        } else if (token == "BASE_ELEVATION") {
            BaseElevation = (float)atof((LPCTSTR)value);
        } else if (token == "THICKNESS") {
            Thickness = (float)atof((LPCTSTR)value);
        } else if (token == "TOP_TRANSITION_BAND") {
            TopTransitionBand = (float)atof((LPCTSTR)value);
        } else if (token == "BOTTOM_TRANSITION_BAND") {
            BottomTransitionBand = (float)atof((LPCTSTR)value);
        } else if (token == "BAROMETRIC_PRESSURE") {
            BaroPressure = (float)atof((LPCTSTR)value);
        } else if (token == "HUMIDITY") {
            Humidity = atoi((LPCTSTR)value);
        } else if (token == "TEMPERATURE") {
            Temperature = (float)atof((LPCTSTR)value);
        } else if (token == "VISIBILITY") {
            Visibility = (float)atof((LPCTSTR)value);
        } else if (token == "HORIZONTAL_WINDSPEED") {
            HorzWindspeed = (float)atof((LPCTSTR)value);
        } else if (token == "VERTICAL_WINDSPEED") {
            VertWindspeed = (float)atof((LPCTSTR)value);
        } else if (token == "WIND_DIRECTION") {
            WindDirection = (float)atof((LPCTSTR)value);
        } else if (token == "AEROSOL_CONCENTRATION") {
            AerosolConcentration = (float)atof((LPCTSTR)value);
        } else if (token == "COMPONENT") {
            TEMPL_COMPONENT component;
            component.ReadText(str, linenum);

            ComponentList.InsertTail(component);
        } else {
            PopupReadErrorMsg(KEYWORD, (LPCTSTR)token, linenum);
        }

        readstatus = ReadLine(str, linenum, token, value);
    }

    return readstatus;
}

///////////////////////////////////////////////////////////////////////////////
//
//  CWeatherTemplMap
//
///////////////////////////////////////////////////////////////////////////////

// Called by CWeatherTemplMap::Serialize when reading/writing binary files.
void AFXAPI SerializeElements(CArchive &ar, TEMPL_WEATHER **pElements, int nCount)
{
    if (ar.IsStoring()) {
        for (int i = 0; i < nCount; i++) {
            if (pElements[i])
                pElements[i]->Serialize(ar);
        }
    } else {
        for (int i = 0; i < nCount; i++) {
            pElements[i] = new TEMPL_WEATHER;
            pElements[i]->Serialize(ar);
        }
    }
}
