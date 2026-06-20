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
 *  FILENAME:   ConfigTemplViewGroup.cpp
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
#include "ConfigTemplViewGroup.h"

///////////////////////////////////////////////////////////////////////////////
//
//  TEMPL_VIEWGROUP
//
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(TEMPL_VIEWGROUP, CObject, 0)

TEMPL_VIEWGROUP::TEMPL_VIEWGROUP()
{
    GroupID = -1;
    XOffset = 0.0;
    YOffset = 0.0;
    ZOffset = 0.0;
    Yaw = 0.0;
    Pitch = 0.0;
    Roll = 0.0;
}

TEMPL_VIEWGROUP::~TEMPL_VIEWGROUP()
{
}

TEMPL_VIEWGROUP &TEMPL_VIEWGROUP::operator=(const TEMPL_VIEWGROUP &rhs)
{
    unsigned long handle = NULL;
    TEMPL_COMPONENT *comp = NULL;

    Name = rhs.Name;
    GroupID = rhs.GroupID;
    XOffset = rhs.XOffset;
    YOffset = rhs.YOffset;
    ZOffset = rhs.ZOffset;
    Yaw = rhs.Yaw;
    Pitch = rhs.Pitch;
    Roll = rhs.Roll;

    ComponentList.RemoveAll();
    comp = rhs.ComponentList.GetHead(&handle);
    while (comp) {
        ComponentList.InsertTail(*comp);
        comp = rhs.ComponentList.GetNext(&handle);
    }

    return *this;
}

// Binary only.
void TEMPL_VIEWGROUP::Serialize(CArchive &ar)
{
    TRACE0("\n--  TEMPL_VIEWGROUP::Serialize()\n");

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
        ar << GroupID;
        ar << XOffset;
        ar << YOffset;
        ar << ZOffset;
        ar << Yaw;
        ar << Pitch;
        ar << Roll;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("	length = %d\n", length);
        TRACE1("	name = %s\n", Name);
        TRACE1("	GroupID = %d\n", GroupID);
        TRACE1("	XOffset = %f\n", XOffset);
        TRACE1("	YOffset = %f\n", YOffset);
        TRACE1("	ZOffset = %f\n", ZOffset);
        TRACE1("	Yaw = %f\n", Yaw);
        TRACE1("	Pitch = %f\n", Pitch);
        TRACE1("	Roll = %f\n", Roll);

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
        // Read the name.
        ar >> length;
        ar.Read(Name.GetBuffer(length), length);
        Name.ReleaseBuffer(length);

        // Read the integral types.
        ar >> GroupID;
        ar >> XOffset;
        ar >> YOffset;
        ar >> ZOffset;
        ar >> Yaw;
        ar >> Pitch;
        ar >> Roll;

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("	length = %d\n", length);
        TRACE1("	name = %s\n", Name);
        TRACE1("	GroupID = %d\n", GroupID);
        TRACE1("	XOffset = %f\n", XOffset);
        TRACE1("	YOffset = %f\n", YOffset);
        TRACE1("	ZOffset = %f\n", ZOffset);
        TRACE1("	Yaw = %f\n", Yaw);
        TRACE1("	Pitch = %f\n", Pitch);
        TRACE1("	Roll = %f\n", Roll);

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
BOOL TEMPL_VIEWGROUP::WriteText(CStdioFile &file)
{
    unsigned long handle = 0;
    CString str;
    TEMPL_COMPONENT *comp = NULL;

    // Write the header and opening brace.
    file.WriteString("\nVIEWGROUP\n{\n");

    // Store the name and values.
    str.Format("\tNAME           = \"%s\";\n", Name);
    file.WriteString((LPCTSTR)str);

    str.Format("\tID             = %d;\n", GroupID);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tOFFSET         = %f, %f, %f;\n", XOffset, YOffset, ZOffset);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tROTATION       = %f, %f, %f;\n", Yaw, Pitch, Roll);
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
BOOL TEMPL_VIEWGROUP::ReadText(CString &src, int *linenum)
{
    BOOL readstatus;
    CString str, temp, token, value, param1, param2, param3;

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
            GroupID = atoi((LPCTSTR)value);
        } else if (token == "OFFSET") {
            // Separate the parameters.
            ReadParameter(value, param1, temp);
            ReadParameter(temp, param2, param3);

            XOffset = (float)atof((LPCTSTR)param1);
            YOffset = (float)atof((LPCTSTR)param2);
            ZOffset = (float)atof((LPCTSTR)param3);
        } else if (token == "X_OFFSET") {
            XOffset = (float)atof(value);
        } else if (token == "Y_OFFSET") {
            YOffset = (float)atof(value);
        } else if (token == "Z_OFFSET") {
            ZOffset = (float)atof(value);
        } else if (token == "ROTATION") {
            // Separate the parameters.
            ReadParameter(value, param1, temp);
            ReadParameter(temp, param2, param3);

            Yaw = (float)atof((LPCTSTR)param1);
            Pitch = (float)atof((LPCTSTR)param2);
            Roll = (float)atof((LPCTSTR)param3);
        } else if (token == "YAW") {
            Yaw = (float)atof(value);
        } else if (token == "PITCH") {
            Pitch = (float)atof(value);
        } else if (token == "ROLL") {
            Roll = (float)atof(value);
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

TEMPL_COMPONENT *TEMPL_VIEWGROUP::FindComponent(const int id)
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

TEMPL_COMPONENT_STATE *TEMPL_VIEWGROUP::FindComponentState(const int component_id, const int comp_value)
{
    TEMPL_COMPONENT *comp = FindComponent(component_id);
    if (comp)
        return comp->FindState(comp_value);

    return NULL;
}
