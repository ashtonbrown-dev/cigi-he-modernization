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
 *  FILENAME:   ConfigTemplCollisionSeg.cpp
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
#include "ConfigTemplCollisionSeg.h"

///////////////////////////////////////////////////////////////////////////////
//
//  TEMPL_COLLISION_SEG
//
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(TEMPL_COLLISION_SEG, CObject, 0)

TEMPL_COLLISION_SEG::TEMPL_COLLISION_SEG()
{
    ID = -1;
    Enabled = 0;
    MaterialMask = 0;
    reserved = 0;
    X1 = 0.0;
    Y1 = 0.0;
    Z1 = 0.0;
    X2 = 0.0;
    Y2 = 0.0;
    Z2 = 0.0;
}


TEMPL_COLLISION_SEG::~TEMPL_COLLISION_SEG()
{
}

TEMPL_COLLISION_SEG &TEMPL_COLLISION_SEG::operator=(const TEMPL_COLLISION_SEG &rhs)
{
    Name = rhs.Name;
    ID = rhs.ID;
    Enabled = rhs.Enabled;
    MaterialMask = rhs.MaterialMask;
    X1 = rhs.X1;
    Y1 = rhs.Y1;
    Z1 = rhs.Z1;
    X2 = rhs.X2;
    Y2 = rhs.Y2;
    Z2 = rhs.Z2;

    return *this;
}

// Binary only.
void TEMPL_COLLISION_SEG::Serialize(CArchive &ar)
{
    TRACE0("\n--    TEMPL_COLLISION_SEG::Serialize()\n");

    int length = 0;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)Name);
        ar << length;
        ar.Write((LPCTSTR)Name, length);    // NULL terminator not copied

        // Store the integral types.
        ar << ID;
        ar << Enabled;
        ar << MaterialMask;
        ar << X1;
        ar << Y1;
        ar << Z1;
        ar << X2;
        ar << Y2;
        ar << Z2;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("			length = %d\n", length);
        TRACE1("			Name = \"%s\"\n", (LPCTSTR)Name);
        TRACE1("			ID = %d\n", ID);
        TRACE1("			Enabled = %d\n", Enabled);
        TRACE1("			X1 = %f\n", X1);
        TRACE1("			Y1 = %f\n", Y1);
        TRACE1("			Z1 = %f\n", Z1);
        TRACE1("			X2 = %f\n", X2);
        TRACE1("			Y2 = %f\n", Y2);
        TRACE1("			Z2 = %f\n", Z2);
        TRACE1("			Material Mask = %X\n", MaterialMask);
    } else {
        // Read the name.
        ar >> length;
        ar.Read(Name.GetBuffer(length), length);
        Name.ReleaseBuffer(length);

        // Read the integral types.
        ar >> ID;
        ar >> Enabled;
        ar >> MaterialMask;
        ar >> X1;
        ar >> Y1;
        ar >> Z1;
        ar >> X2;
        ar >> Y2;
        ar >> Z2;

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
        TRACE1("			X1 = %f\n", X1);
        TRACE1("			Y1 = %f\n", Y1);
        TRACE1("			Z1 = %f\n", Z1);
        TRACE1("			X2 = %f\n", X2);
        TRACE1("			Y2 = %f\n", Y2);
        TRACE1("			Z2 = %f\n", Z2);
        TRACE1("			Material Mask = %X\n", MaterialMask);
    }
}

// Text only.
BOOL TEMPL_COLLISION_SEG::WriteText(CStdioFile &file)
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

    str.Format("\t\tSTART           = %f, %f, %f;\n", X1, Y1, Z1);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tEND             = %f, %f, %f;\n", X2, Y2, Z2);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tMATERIAL_MASK   = %X;\n", MaterialMask);
    file.WriteString((LPCTSTR)str);

    // Write the closing brace.
    file.WriteString("\t}\n");

    return TRUE;
}

// Text only.
BOOL TEMPL_COLLISION_SEG::ReadText(CString &src, int *linenum)
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
        } else if (token == "START") {
            // Separate the parameters.
            ReadParameter(value, param1, temp);
            ReadParameter(temp, param2, param3);

            X1 = atof((LPCTSTR)param1);
            Y1 = atof((LPCTSTR)param2);
            Z1 = atof((LPCTSTR)param3);
        } else if (token == "END") {
            // Separate the parameters.
            ReadParameter(value, param1, temp);
            ReadParameter(temp, param2, param3);

            X2 = atof((LPCTSTR)param1);
            Y2 = atof((LPCTSTR)param2);
            Z2 = atof((LPCTSTR)param3);
        } else if (token == "X1") {
            X1 = atof((LPCTSTR)value);
        } else if (token == "Y1") {
            Y1 = atof((LPCTSTR)value);
        } else if (token == "Z1") {
            Z1 = atof((LPCTSTR)value);
        } else if (token == "X2") {
            X2 = atof((LPCTSTR)value);
        } else if (token == "Y2") {
            Y2 = atof((LPCTSTR)value);
        } else if (token == "Z2") {
            Z2 = atof((LPCTSTR)value);
        } else if (token == "MATERIAL_MASK") {
            sscanf((LPCTSTR)value, "%x", &MaterialMask);
        } else {
            PopupReadErrorMsg(KEYWORD, (LPCTSTR)token, linenum);
        }

        readstatus = ReadLine(str, linenum, token, value);
    }

    return readstatus;
}
