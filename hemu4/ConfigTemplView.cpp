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
 *  FILENAME:   ConfigTemplView.cpp
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
#include "ConfigTemplView.h"
#include "globals.h"

///////////////////////////////////////////////////////////////////////////////
//
//  TEMPL_VIEW
//
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(TEMPL_VIEW, CObject, 0)

TEMPL_VIEW::TEMPL_VIEW()
{
    ViewID = -1;
    GroupID = 0;
    ViewType = 0;
    ProjectionType = 0;
    ReplicationMode = 0;
    MirrorMode = 0;
    XOffset = 0.0;
    YOffset = 0.0;
    ZOffset = 0.0;
    Yaw = 0.0;
    Pitch = 0.0;
    Roll = 0.0;
    Top = 0.0;
    Bottom = 0.0;
    Left = 0.0;
    Right = 0.0;
    Near = 0.0;
    Far = 0.0;
}

TEMPL_VIEW::~TEMPL_VIEW()
{
}

TEMPL_VIEW &TEMPL_VIEW::operator=(const TEMPL_VIEW &rhs)
{
    unsigned long handle = NULL;
    TEMPL_COMPONENT *comp = NULL;

    Name = rhs.Name;
    ViewID = rhs.ViewID;
    GroupID = rhs.GroupID;
    ViewType = rhs.ViewType;
    ProjectionType = rhs.ProjectionType;
    ReplicationMode = rhs.ReplicationMode;
    MirrorMode = rhs.MirrorMode;
    XOffset = rhs.XOffset;
    YOffset = rhs.YOffset;
    ZOffset = rhs.ZOffset;
    Yaw = rhs.Yaw;
    Pitch = rhs.Pitch;
    Roll = rhs.Roll;
    Top = rhs.Top;
    Bottom = rhs.Bottom;
    Left = rhs.Left;
    Right = rhs.Right;
    Near = rhs.Near;
    Far = rhs.Far;

    ComponentList.RemoveAll();
    comp = rhs.ComponentList.GetHead(&handle);
    while (comp) {
        ComponentList.InsertTail(*comp);
        comp = rhs.ComponentList.GetNext(&handle);
    }

    return *this;
}

// Binary only.
void TEMPL_VIEW::Serialize(CArchive &ar)
{
    TRACE0("\n--  TEMPL_VIEW::Serialize()\n");

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
        ar << ViewID;
        ar << GroupID;
        ar << ViewType;
        ar << ProjectionType;
        ar << ReplicationMode;
        ar << MirrorMode;
        ar << XOffset;
        ar << YOffset;
        ar << ZOffset;
        ar << Yaw;
        ar << Pitch;
        ar << Roll;
        ar << Top;
        ar << Bottom;
        ar << Left;
        ar << Right;
        ar << Near;
        ar << Far;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("	length = %d\n", length);
        TRACE1("	name = %s\n", Name);
        TRACE1("	ViewID = %d\n", ViewID);
        TRACE1("	GroupID = %d\n", GroupID);
        TRACE1("	ViewType = %d\n", ViewType);
        TRACE1("	ProjectionType = %d\n", ProjectionType);
        TRACE1("	ReplicationMode = %d\n", ReplicationMode);
        TRACE1("	MirrorMode = %d\n", MirrorMode);
        TRACE1("	XOffset = %f\n", XOffset);
        TRACE1("	YOffset = %f\n", YOffset);
        TRACE1("	ZOffset = %f\n", ZOffset);
        TRACE1("	Yaw = %f\n", Yaw);
        TRACE1("	Pitch = %f\n", Pitch);
        TRACE1("	Roll = %f\n", Roll);
        TRACE1("	Top = %f\n", Top);
        TRACE1("	Bottom = %f\n", Bottom);
        TRACE1("	Left = %f\n", Left);
        TRACE1("	Right = %f\n", Right);
        TRACE1("	Near = %f\n", Near);
        TRACE1("	Far = %f\n", Far);

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
        ar >> ViewID;
        ar >> GroupID;
        ar >> ViewType;
        ar >> ProjectionType;
        ar >> ReplicationMode;
        ar >> MirrorMode;
        ar >> XOffset;
        ar >> YOffset;
        ar >> ZOffset;
        ar >> Yaw;
        ar >> Pitch;
        ar >> Roll;
        ar >> Top;
        ar >> Bottom;
        ar >> Left;
        ar >> Right;
        ar >> Near;
        ar >> Far;

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("	length = %d\n", length);
        TRACE1("	name = %s\n", Name);
        TRACE1("	ViewID = %d\n", ViewID);
        TRACE1("	GroupID = %d\n", GroupID);
        TRACE1("	ViewType = %d\n", ViewType);
        TRACE1("	ProjectionType = %d\n", ProjectionType);
        TRACE1("	ReplicationMode = %d\n", ReplicationMode);
        TRACE1("	MirrorMode = %d\n", MirrorMode);
        TRACE1("	XOffset = %f\n", XOffset);
        TRACE1("	YOffset = %f\n", YOffset);
        TRACE1("	ZOffset = %f\n", ZOffset);
        TRACE1("	Yaw = %f\n", Yaw);
        TRACE1("	Pitch = %f\n", Pitch);
        TRACE1("	Roll = %f\n", Roll);
        TRACE1("	Top = %f\n", Top);
        TRACE1("	Bottom = %f\n", Bottom);
        TRACE1("	Left = %f\n", Left);
        TRACE1("	Right = %f\n", Right);
        TRACE1("	Near = %f\n", Near);
        TRACE1("	Far = %f\n", Far);

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
BOOL TEMPL_VIEW::WriteText(CStdioFile &file)
{
    unsigned long handle = 0;
    CString str;
    TEMPL_COMPONENT *comp = NULL;

    // Write the header and opening brace.
    file.WriteString("\nVIEW\n{\n");

    // Store the name and values.
    str.Format("\tNAME             = \"%s\";\n", Name);
    file.WriteString((LPCTSTR)str);

    str.Format("\tID               = %d;\n", ViewID);
    file.WriteString((LPCTSTR)str);

    str.Format("\tGROUP            = %d;\n", GroupID);
    file.WriteString((LPCTSTR)str);

    str.Format("\tTYPE             = %d;\n", ViewType);
    file.WriteString((LPCTSTR)str);

    str.Format("\tPROJECTION_TYPE  = %s;\n", ProjectionTypeStr[ProjectionType]);
    file.WriteString((LPCTSTR)str);

    str.Format("\tREPLICATION_MODE = %s;\n", ReplicationStr[ReplicationMode]);
    file.WriteString((LPCTSTR)str);

    str.Format("\tMIRROR_MODE      = %s;\n", MirrorModeStr[MirrorMode]);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tOFFSET         = %f, %f, %f;\n", XOffset, YOffset, ZOffset);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tROTATION       = %f, %f, %f;\n", Yaw, Pitch, Roll);
    file.WriteString((LPCTSTR)str);

    str.Format("\tTOP              = %f;\n", Top);
    file.WriteString((LPCTSTR)str);

    str.Format("\tBOTTOM           = %f;\n", Bottom);
    file.WriteString((LPCTSTR)str);

    str.Format("\tLEFT             = %f;\n", Left);
    file.WriteString((LPCTSTR)str);

    str.Format("\tRIGHT            = %f;\n", Right);
    file.WriteString((LPCTSTR)str);

    str.Format("\tNEAR             = %f;\n", Near);
    file.WriteString((LPCTSTR)str);

    str.Format("\tFAR              = %f;\n", Far);
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
BOOL TEMPL_VIEW::ReadText(CString &src, int *linenum)
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
            ViewID = atoi((LPCTSTR)value);
        } else if (token == "GROUP") {
            GroupID = atoi((LPCTSTR)value);
        } else if (token == "TYPE") {
            ViewType = atoi((LPCTSTR)value);
        } else if (token == "PROJECTION_TYPE") {
            if (value == "PERSPECTIVE")
                ProjectionType = PROJECTION_TYPE_PERSPECTIVE;
            else if (value == "PARALLEL")
                ProjectionType = PROJECTION_TYPE_PARALLEL;
            else
                PopupReadErrorMsg(KEYWORD, (LPCTSTR)value, linenum);
        } else if (token == "REPLICATION_MODE") {
            if (value == "NONE")
                ReplicationMode = PIXEL_REPLICATION_MODE_NONE;
            else if (value == "1X2")
                ReplicationMode = PIXEL_REPLICATION_MODE_1X2;
            else if (value == "2X1")
                ReplicationMode = PIXEL_REPLICATION_MODE_2X1;
            else if (value == "2X2")
                ReplicationMode = PIXEL_REPLICATION_MODE_2X2;
            else
                ReplicationMode = atoi(value);
        } else if (token == "MIRROR_MODE") {
            if (value == "NONE")
                MirrorMode = MIRROR_MODE_NONE;
            else if (value == "HORIZONTAL")
                MirrorMode = MIRROR_MODE_HORZ;
            else if (value == "VERTICAL")
                MirrorMode = MIRROR_MODE_VERT;
            else if (value == "BOTH")
                MirrorMode = MIRROR_MODE_BOTH;
            else
                PopupReadErrorMsg(KEYWORD, (LPCTSTR)value, linenum);
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
        } else if (token == "TOP") {
            Top = (float)atof(value);
        } else if (token == "BOTTOM") {
            Bottom = (float)atof(value);
        } else if (token == "LEFT") {
            Left = (float)atof(value);
        } else if (token == "RIGHT") {
            Right = (float)atof(value);
        } else if (token == "NEAR") {
            Near = (float)atof(value);
        } else if (token == "FAR") {
            Far = (float)atof(value);
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

TEMPL_COMPONENT *TEMPL_VIEW::FindComponent(const int id)
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

TEMPL_COMPONENT_STATE *TEMPL_VIEW::FindComponentState(const int component_id, const int comp_value)
{
    TEMPL_COMPONENT *comp = FindComponent(component_id);
    if (comp)
        return comp->FindState(comp_value);

    return NULL;
}
