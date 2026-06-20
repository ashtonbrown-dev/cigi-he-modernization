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
 *  FILENAME:   ConfigTemplDatabase.cpp
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
#include "ConfigTemplDatabase.h"
#include "globals.h"

///////////////////////////////////////////////////////////////////////////////
//
//  TEMPL_DATABASE
//
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(TEMPL_DATABASE, CObject, 0)

TEMPL_DATABASE::TEMPL_DATABASE()
{
    ErmName = "WGS84";
    DatabaseID = 0;
    OriginLat = 0;
    OriginLon = 0;
    OriginAlt = 0;
    DefaultLat = 0;
    DefaultLon = 0;
    DefaultAlt = 0;
    ErmRadius = TERRAIN_DEFAULT_RADIUS;
    ErmFlattening = TERRAIN_DEFAULT_FLATTENING;
}

TEMPL_DATABASE::~TEMPL_DATABASE()
{
}

TEMPL_DATABASE &TEMPL_DATABASE::operator=(const TEMPL_DATABASE &rhs)
{
    unsigned long handle = NULL;
    TEMPL_COMPONENT *comp = NULL;

    Name = rhs.Name;
    ErmName = rhs.ErmName;
    DatabaseID = rhs.DatabaseID;
    OriginLat = rhs.OriginLat;
    OriginLon = rhs.OriginLon;
    OriginAlt = rhs.OriginAlt;
    DefaultLat = rhs.DefaultLat;
    DefaultLon = rhs.DefaultLon;
    DefaultAlt = rhs.DefaultAlt;
    ErmRadius = rhs.ErmRadius;
    ErmFlattening = rhs.ErmFlattening;

    ComponentList.RemoveAll();
    comp = rhs.ComponentList.GetHead(&handle);
    while (comp) {
        ComponentList.InsertTail(*comp);
        comp = rhs.ComponentList.GetNext(&handle);
    }

    return *this;
}

// Binary only.
void TEMPL_DATABASE::Serialize(CArchive &ar)
{
    TRACE0("\n--  TEMPL_DATABASE::Serialize()\n");

    unsigned long handle = NULL;
    int length = 0;
    int count = 0;

    // Call base class Serialize() to make sure all inherited
    // members are saved/loaded.
    CObject::Serialize(ar);

    if (ar.IsStoring()) {
        int i = 0;
        TEMPL_COMPONENT *comp = NULL;

        // Store the name.
        length = strlen((LPCTSTR)Name);
        ar << length;
        ar.Write((LPCTSTR)Name, length);    // NULL terminator not copied

        TRACE1("	length = %d\n", length);
        TRACE1("	name = %s\n", Name);

        // Store the ERM's name.
        length = strlen((LPCTSTR)ErmName);
        ar << length;
        ar.Write((LPCTSTR)ErmName, length); // NULL terminator not copied

        TRACE1("	length = %d\n", length);
        TRACE1("	ERM Name = %s\n", ErmName);

        // Store the integral types.
        ar << DatabaseID;
        ar << OriginLat;
        ar << OriginLon;
        ar << OriginAlt;
        ar << DefaultLat;
        ar << DefaultLon;
        ar << DefaultAlt;
        ar << ErmRadius;
        ar << ErmFlattening;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("	DatabaseID = %d\n", DatabaseID);
        TRACE1("	OriginLat = %f\n", OriginLat);
        TRACE1("	OriginLon = %f\n", OriginLon);
        TRACE1("	OriginAlt = %f\n", OriginAlt);
        TRACE1("	DefaultLat = %f\n", DefaultLat);
        TRACE1("	DefaultLon = %f\n", DefaultLon);
        TRACE1("	DefaultAlt = %f\n", DefaultAlt);
        TRACE1("	ErmRadius = %f\n", ErmRadius);
        TRACE1("	ErmFlattening = 1 / %f\n", 1 / ErmFlattening);

        // Store the components.
        count = ComponentList.GetItemCount();
        ar << count;

        TRACE1("	component count = %d\n", count);

        if (count) {
            comp = ComponentList.GetHead(&handle);
            while (comp) {
                comp->Serialize(ar);
                comp = ComponentList.GetNext(&handle);
            };
        }
    } else {
        int i = 0;

        // Read the name.
        ar >> length;
        ar.Read(Name.GetBuffer(length), length);
        Name.ReleaseBuffer(length);

        TRACE1("	length = %d\n", length);
        TRACE1("	Name = %s\n", Name);

        // Read the ERM's name.
        ar >> length;
        ar.Read(ErmName.GetBuffer(length), length);
        ErmName.ReleaseBuffer(length);

        TRACE1("	length = %d\n", length);
        TRACE1("	ERM Name = %s\n", ErmName);

        // Read the integral types.
        ar >> DatabaseID;
        ar >> OriginLat;
        ar >> OriginLon;
        ar >> OriginAlt;
        ar >> DefaultLat;
        ar >> DefaultLon;
        ar >> DefaultAlt;
        ar >> ErmRadius;
        ar >> ErmFlattening;

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("	DatabaseID = %d\n", DatabaseID);
        TRACE1("	OriginLat = %f\n", OriginLat);
        TRACE1("	OriginLon = %f\n", OriginLon);
        TRACE1("	OriginAlt = %f\n", OriginAlt);
        TRACE1("	DefaultLat = %f\n", DefaultLat);
        TRACE1("	DefaultLon = %f\n", DefaultLon);
        TRACE1("	DefaultAlt = %f\n", DefaultAlt);
        TRACE1("	ErmRadius = %f\n", ErmRadius);
        TRACE1("	ErmFlattening = 1 / %f\n", 1 / ErmFlattening);

        // Read in the components.
        ar >> count;

        TRACE1("	component count = %d\n", count);

        for (i = 0; i < count; i++) {
            TEMPL_COMPONENT comp;
            comp.Serialize(ar);
            ComponentList.InsertTail(comp);
        }
    }
}

// Text only.
BOOL TEMPL_DATABASE::WriteText(CStdioFile &file)
{
    unsigned long handle = 0;
    CString str;
    TEMPL_COMPONENT *comp = NULL;

    // Write the header and opening brace.
    file.WriteString("\nDATABASE\n{\n");

    // Store the name and values.
    str.Format("\tNAME               = \"%s\";\n", Name);
    file.WriteString((LPCTSTR)str);

    str.Format("\tID                 = %d;\n", DatabaseID);
    file.WriteString((LPCTSTR)str);

    if ((ErmFlattening == TERRAIN_DEFAULT_FLATTENING) &&
        (ErmRadius == TERRAIN_DEFAULT_RADIUS)) {
        str.Format("\tERM_NAME       = \"%s\";\n", ErmName);
        file.WriteString((LPCTSTR)str);

        str.Format("\tERM_RADIUS     = %f;\n", ErmRadius);
        file.WriteString((LPCTSTR)str);

        str.Format("\tERM_FLATTENING = %f;\n", ErmFlattening);
        file.WriteString((LPCTSTR)str);
    }

    str.Format("\tORIGIN_LATITUDE    = %f;\n", OriginLat);
    file.WriteString((LPCTSTR)str);

    str.Format("\tORIGIN_LONGITUDE   = %f;\n", OriginLon);
    file.WriteString((LPCTSTR)str);

    str.Format("\tORIGIN_ALTITUDE    = %f;\n", OriginAlt);
    file.WriteString((LPCTSTR)str);

    str.Format("\tDEFAULT_LATITUDE   = %f;\n", DefaultLat);
    file.WriteString((LPCTSTR)str);

    str.Format("\tDEFAULT_LONGITUDE  = %f;\n", DefaultLon);
    file.WriteString((LPCTSTR)str);

    str.Format("\tDEFAULT_ALTITUDE   = %f;\n", DefaultAlt);
    file.WriteString((LPCTSTR)str);

    // Write each component.
    comp = ComponentList.GetHead(&handle);
    while (comp) {
        comp->WriteText(file);
        comp = ComponentList.GetNext(&handle);
    }

    // Write the closing brace.
    file.WriteString("}\n\n");

    return TRUE;
}

// Text only.
BOOL TEMPL_DATABASE::ReadText(CString &src, int *linenum)
{
    BOOL readstatus;
    CString str, temp, token, value;

    // We should be starting just inside the opening brace.

    // Get the entity block.
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
            DatabaseID = atoi((LPCTSTR)value);
        } else if (token == "ERM_NAME") {
            // Trim the quotes.
            value.TrimLeft("\"");
            value.TrimRight("\"");
            value.TrimRight();

            ErmName = value;
        } else if (token == "ERM_RADIUS") {
            ErmRadius = (float)atof(value);
        } else if (token == "ERM_FLATTENING") {
            ErmFlattening = (float)atof(value);
        } else if (token == "ORIGIN_LATITUDE") {
            OriginLat = (float)atof(value);
        } else if (token == "ORIGIN_LONGITUDE") {
            OriginLon = (float)atof(value);
        } else if (token == "ORIGIN_ALTITUDE") {
            OriginAlt = (float)atof(value);
        } else if (token == "DEFAULT_LATITUDE") {
            DefaultLat = (float)atof(value);
        } else if (token == "DEFAULT_LONGITUDE") {
            DefaultLon = (float)atof(value);
        } else if (token == "DEFAULT_ALTITUDE") {
            DefaultAlt = (float)atof(value);
        } else if (token == "COMPONENT") {
            TEMPL_COMPONENT component;
            component.ReadText(str, linenum);

            component.ComponentClass = StrToCompClass("GLOBAL_TERRAIN");

            ComponentList.InsertTail(component);
        } else {
            PopupReadErrorMsg(KEYWORD, (LPCTSTR)token, linenum);
        }

        readstatus = ReadLine(str, linenum, token, value);
    }

    return readstatus;
}

TEMPL_COMPONENT *TEMPL_DATABASE::FindComponent(const int id)
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

TEMPL_COMPONENT_STATE *TEMPL_DATABASE::FindComponentState(const int component_id, const int comp_value)
{
    TEMPL_COMPONENT *comp = FindComponent(component_id);
    if (comp)
        return comp->FindState(comp_value);

    return NULL;
}
