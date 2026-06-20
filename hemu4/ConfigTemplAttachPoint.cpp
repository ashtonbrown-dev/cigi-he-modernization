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
 *  FILENAME:   ConfigTemplAttachPoint.cpp
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
#include "ConfigTemplAttachPoint.h"

///////////////////////////////////////////////////////////////////////////////
//
//  TEMPL_ATTACHPOINT
//
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(TEMPL_ATTACHPOINT, CObject, 0)

TEMPL_ATTACHPOINT::TEMPL_ATTACHPOINT()
{
    //TRACE0("--   TEMPL_ATTACHPOINT::TEMPL_ATTACHPOINT()\n");

    ID = -1;
    reserved = 0;
    XOffset = 0.0;
    YOffset = 0.0;
    ZOffset = 0.0;
    Roll = 0.0;
    Pitch = 0.0;
    Yaw = 0.0;
}

TEMPL_ATTACHPOINT::~TEMPL_ATTACHPOINT()
{
}

TEMPL_ATTACHPOINT &TEMPL_ATTACHPOINT::operator=(const TEMPL_ATTACHPOINT &rhs)
{
    Name = rhs.Name;
    ID = rhs.ID;
    reserved = rhs.reserved;
    XOffset = rhs.XOffset;
    YOffset = rhs.YOffset;
    ZOffset = rhs.ZOffset;
    Roll = rhs.Roll;
    Pitch = rhs.Pitch;
    Yaw = rhs.Yaw;

    return *this;
}

// Binary only.
void TEMPL_ATTACHPOINT::Serialize(CArchive &ar)
{
    TRACE0("\n--    TEMPL_ATTACHPOINT::Serialize()\n");

    int length = 0;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)Name);
        ar << length;
        ar.Write((LPCTSTR)Name, length);    // NULL terminator not copied

        // Store the integral types.
        ar << ID;
        ar << XOffset;
        ar << YOffset;
        ar << ZOffset;
        ar << Roll;
        ar << Pitch;
        ar << Yaw;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("			length = %d\n", length);
        TRACE1("			Name = \"%s\"\n", (LPCTSTR)Name);
        TRACE1("			ID = %d\n", ID);
        TRACE1("			XOffset = %f\n", XOffset);
        TRACE1("			YOffset = %f\n", YOffset);
        TRACE1("			ZOffset = %f\n", ZOffset);
        TRACE1("			Roll = %f\n", Roll);
        TRACE1("			Pitch = %f\n", Pitch);
        TRACE1("			Yaw = %f\n", Yaw);
    } else {
        // Read the name.
        ar >> length;
        ar.Read(Name.GetBuffer(length), length);
        Name.ReleaseBuffer(length);

        // Read the integral types.
        ar >> ID;
        ar >> XOffset;
        ar >> YOffset;
        ar >> ZOffset;
        ar >> Roll;
        ar >> Pitch;
        ar >> Yaw;

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("			length = %d\n", length);
        TRACE1("			Name = \"%s\"\n", (LPCTSTR)Name);
        TRACE1("			ID = %d\n", ID);
        TRACE1("			XOffset = %f\n", XOffset);
        TRACE1("			YOffset = %f\n", YOffset);
        TRACE1("			ZOffset = %f\n", ZOffset);
        TRACE1("			Roll = %f\n", Roll);
        TRACE1("			Pitch = %f\n", Pitch);
        TRACE1("			Yaw = %f\n", Yaw);
    }
}

// Text only.
BOOL TEMPL_ATTACHPOINT::WriteText(CStdioFile &file)
{
    CString str;

    // Write the opening brace.
    file.WriteString("\n\tATTACH_POINT\n\t{\n");

    // Store the name and value.
    str.Format("\t\tNAME            = \"%s\";\n", Name);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tID              = %d;\n", ID);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tOFFSET          = %f, %f, %f;\n", XOffset, YOffset, ZOffset);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tROTATION        = %f, %f, %f;\n", Yaw, Pitch, Roll);
    file.WriteString((LPCTSTR)str);

    // Write the closing brace.
    file.WriteString("\t}\n");

    return TRUE;
}

// Text only.
BOOL TEMPL_ATTACHPOINT::ReadText(CString &src, int *linenum)
{
    BOOL readstatus;
    CString str, temp, token, value, param1, param2, param3;

    // We should be starting just inside the opening brace.

    // Get the attachpoint block.
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
        } else if (token == "OFFSET") {
            // Separate the parameters.
            ReadParameter(value, param1, temp);
            ReadParameter(temp, param2, param3);

            XOffset = atof((LPCTSTR)param1);
            YOffset = atof((LPCTSTR)param2);
            ZOffset = atof((LPCTSTR)param3);
        } else if (token == "X_OFFSET") {
            XOffset = atof((LPCTSTR)value);
        } else if (token == "Y_OFFSET") {
            YOffset = atof((LPCTSTR)value);
        } else if (token == "Z_OFFSET") {
            ZOffset = atof((LPCTSTR)value);
        } else if (token == "ROTATION") {
            // Separate the parameters.
            ReadParameter(value, param1, temp);
            ReadParameter(temp, param2, param3);

            Yaw = atof((LPCTSTR)param1);
            Pitch = atof((LPCTSTR)param2);
            Roll = atof((LPCTSTR)param3);
        } else if (token == "PITCH") {
            Pitch = atof((LPCTSTR)value);
        } else if (token == "ROLL") {
            Roll = atof((LPCTSTR)value);
        } else if (token == "YAW") {
            Yaw = atof((LPCTSTR)value);
        } else {
            PopupReadErrorMsg(KEYWORD, (LPCTSTR)token, linenum);
        }

        readstatus = ReadLine(str, linenum, token, value);
    }

    return readstatus;
}
