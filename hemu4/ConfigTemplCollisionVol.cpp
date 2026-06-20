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
 *  FILENAME:   ConfigTemplCollisionVol.cpp
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
#include "ConfigTemplCollisionVol.h"

///////////////////////////////////////////////////////////////////////////////
//
//  TEMPL_COLLISION_VOL
//
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(TEMPL_COLLISION_VOL, CObject, 0)

TEMPL_COLLISION_VOL::TEMPL_COLLISION_VOL()
{
    ID = -1;
    Enabled = 0;
    Type = 0;
    reserved = 0;
    X = 0.0;
    Y = 0.0;
    Z = 0.0;
    Roll = 0.0;
    Pitch = 0.0;
    Yaw = 0.0;
    Height = 0.0;
    Width = 0.0;
    Depth = 0.0;
    Radius = 0.0;
}


TEMPL_COLLISION_VOL::~TEMPL_COLLISION_VOL()
{
}

TEMPL_COLLISION_VOL &TEMPL_COLLISION_VOL::operator=(const TEMPL_COLLISION_VOL &rhs)
{
    Name = rhs.Name;
    ID = rhs.ID;
    Enabled = rhs.Enabled;
    Type = rhs.Type;
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;
    Roll = rhs.Roll;
    Pitch = rhs.Pitch;
    Yaw = rhs.Yaw;
    Height = rhs.Height;
    Width = rhs.Width;
    Depth = rhs.Depth;
    Radius = rhs.Radius;

    return *this;
}

// Binary only.
void TEMPL_COLLISION_VOL::Serialize(CArchive &ar)
{
    TRACE0("\n--    TEMPL_COLLISION_VOL::Serialize()\n");

    int length = 0;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)Name);
        ar << length;
        ar.Write((LPCTSTR)Name, length);    // NULL terminator not copied

        // Store the integral types.
        ar << ID;
        ar << Type;
        ar << Enabled;
        ar << X;
        ar << Y;
        ar << Z;
        ar << Roll;
        ar << Pitch;
        ar << Yaw;
        ar << Height;
        ar << Width;
        ar << Depth;
        ar << Radius;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("			length = %d\n", length);
        TRACE1("			Name = \"%s\"\n", (LPCTSTR)Name);
        TRACE1("			ID = %d\n", ID);
        TRACE1("			Enabled = %d\n", Enabled);
        TRACE1("			X = %f\n", X);
        TRACE1("			Y = %f\n", Y);
        TRACE1("			Z = %f\n", Z);
        TRACE1("			Roll = %f\n", Roll);
        TRACE1("			Pitch = %f\n", Pitch);
        TRACE1("			Yaw = %f\n", Yaw);
        TRACE1("			Height = %f\n", Height);
        TRACE1("			Width = %f\n", Width);
        TRACE1("			Depth = %f\n", Depth);
        TRACE1("			Radius = %f\n", Radius);
    } else {
        // Read the name.
        ar >> length;
        ar.Read(Name.GetBuffer(length), length);
        Name.ReleaseBuffer(length);

        // Read the integral types.
        ar >> ID;
        ar >> Type;
        ar >> Enabled;
        ar >> X;
        ar >> Y;
        ar >> Z;
        ar >> Roll;
        ar >> Pitch;
        ar >> Yaw;
        ar >> Height;
        ar >> Width;
        ar >> Depth;
        ar >> Radius;

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("			length = %d\n", length);
        TRACE1("			Name = \"%s\"\n", (LPCTSTR)Name);
        TRACE1("			ID = %d\n", ID);
        TRACE1("			Enabled = %d\n", Enabled);
        TRACE1("			X = %f\n", X);
        TRACE1("			Y = %f\n", Y);
        TRACE1("			Z = %f\n", Z);
        TRACE1("			Roll = %f\n", Roll);
        TRACE1("			Pitch = %f\n", Pitch);
        TRACE1("			Yaw = %f\n", Yaw);
        TRACE1("			Height = %f\n", Height);
        TRACE1("			Width = %f\n", Width);
        TRACE1("			Depth = %f\n", Depth);
        TRACE1("			Radius = %f\n", Radius);
    }
}

// Text only.
BOOL TEMPL_COLLISION_VOL::WriteText(CStdioFile &file)
{
    CString str;

    // Write the opening brace.
    file.WriteString("\n\tARTPART\n\t{\n");

    // Store the name and value.
    str.Format("\t\tNAME            = \"%s\";\n", Name);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tID              = %d;\n", ID);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tENABLE          = %s;\n", Enabled ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tTYPE            = %d;\n", Type == 0 ? "SPHERE" : "CUBOID");
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tPOSITION        = %f, %f, %f;\n", X, Y, Z);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tROTATION        = %f, %f, %f;\n", Yaw, Pitch, Roll);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tHEIGHT          = %f;\n", Height);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tWIDTH           = %f;\n", Width);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tDEPTH           = %f;\n", Depth);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tRADIUS          = %f;\n", Radius);
    file.WriteString((LPCTSTR)str);

    // Write the closing brace.
    file.WriteString("\t}\n");

    return TRUE;
}

// Text only.
BOOL TEMPL_COLLISION_VOL::ReadText(CString &src, int *linenum)
{
    BOOL readstatus;
    CString str, temp, token, value, param1, param2, param3;

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
            ID = atoi((LPCTSTR)value);
        } else if (token == "ENABLE") {
            Enabled = StrToBool((LPCTSTR)value);
        } else if (token == "TYPE") {
            Type = StrToVolType((LPCTSTR)value);
        } else if (token == "POSITION") {
            // Separate the parameters.
            ReadParameter(value, param1, temp);
            ReadParameter(temp, param2, param3);

            X = atof((LPCTSTR)param1);
            Y = atof((LPCTSTR)param2);
            Z = atof((LPCTSTR)param3);
        } else if (token == "X") {
            X = atof((LPCTSTR)value);
        } else if (token == "Y") {
            Y = atof((LPCTSTR)value);
        } else if (token == "Z") {
            Z = atof((LPCTSTR)value);
        } else if (token == "ROTATION") {
            // Separate the parameters.
            ReadParameter(value, param1, temp);
            ReadParameter(temp, param2, param3);

            Yaw = atof((LPCTSTR)param1);
            Pitch = atof((LPCTSTR)param2);
            Roll = atof((LPCTSTR)param3);
        } else if (token == "ROLL") {
            Roll = atof((LPCTSTR)value);
        } else if (token == "PITCH") {
            Pitch = atof((LPCTSTR)value);
        } else if (token == "YAW") {
            Yaw = atof((LPCTSTR)value);
        } else if (token == "HEIGHT") {
            Height = atof((LPCTSTR)value);
        } else if (token == "WIDTH") {
            Width = atof((LPCTSTR)value);
        } else if (token == "DEPTH") {
            Depth = atof((LPCTSTR)value);
        } else if (token == "RADIUS") {
            Radius = atof((LPCTSTR)value);
        } else {
            PopupReadErrorMsg(KEYWORD, (LPCTSTR)token, linenum);
        }

        readstatus = ReadLine(str, linenum, token, value);
    }

    return readstatus;
}
