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
 *  FILENAME:   ConfigTemplArtPart.cpp
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
#include "ConfigTemplArtPart.h"

///////////////////////////////////////////////////////////////////////////////
//
//  TEMPL_ARTPART
//
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(TEMPL_ARTPART, CObject, 0)

TEMPL_ARTPART::TEMPL_ARTPART()
{
    //TRACE0("--   TEMPL_ARTPART::TEMPL_ARTPART()\n");

    ID = -1;
    reserved = 0;
    XOffset = 0.0;
    YOffset = 0.0;
    ZOffset = 0.0;
    Roll = 0.0;
    Pitch = 0.0;
    Yaw = 0.0;
    Enable = 0;
    XOffsetEnable = 0;
    YOffsetEnable = 0;
    ZOffsetEnable = 0;
    RollEnable = 0;
    PitchEnable = 0;
    YawEnable = 0;
    reserved2 = 0;
}

TEMPL_ARTPART::~TEMPL_ARTPART()
{
}

TEMPL_ARTPART &TEMPL_ARTPART::operator=(const TEMPL_ARTPART &rhs)
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
    Enable = rhs.Enable;
    XOffsetEnable = rhs.XOffsetEnable;
    YOffsetEnable = rhs.YOffsetEnable;
    ZOffsetEnable = rhs.ZOffsetEnable;
    RollEnable = rhs.RollEnable;
    PitchEnable = rhs.PitchEnable;
    YawEnable = rhs.YawEnable;
    reserved2 = rhs.reserved2;

    return *this;
}

// Binary only.
void TEMPL_ARTPART::Serialize(CArchive &ar)
{
    TRACE0("\n--    TEMPL_ARTPART::Serialize()\n");

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
        ar << Enable;
        ar << XOffsetEnable;
        ar << YOffsetEnable;
        ar << ZOffsetEnable;
        ar << RollEnable;
        ar << PitchEnable;
        ar << YawEnable;

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
        TRACE1("			Enable = %d\n", Enable);
        TRACE1("			XOffsetEnable = %d\n", XOffsetEnable);
        TRACE1("			YOffsetEnable = %d\n", YOffsetEnable);
        TRACE1("			ZOffsetEnable = %d\n", ZOffsetEnable);
        TRACE1("			RollEnable = %d\n", RollEnable);
        TRACE1("			PitchEnable = %d\n", PitchEnable);
        TRACE1("			YawEnable = %d\n", YawEnable);
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
        ar >> Enable;
        ar >> XOffsetEnable;
        ar >> YOffsetEnable;
        ar >> ZOffsetEnable;
        ar >> RollEnable;
        ar >> PitchEnable;
        ar >> YawEnable;

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
        TRACE1("			Enable = %d\n", Enable);
        TRACE1("			XOffsetEnable = %d\n", XOffsetEnable);
        TRACE1("			YOffsetEnable = %d\n", YOffsetEnable);
        TRACE1("			ZOffsetEnable = %d\n", ZOffsetEnable);
        TRACE1("			RollEnable = %d\n", RollEnable);
        TRACE1("			PitchEnable = %d\n", PitchEnable);
        TRACE1("			YawEnable = %d\n", YawEnable);
    }
}

// Text only.
BOOL TEMPL_ARTPART::WriteText(CStdioFile &file)
{
    CString str;

    // Write the opening brace.
    file.WriteString("\n\tARTPART\n\t{\n");

    // Store the name and value.
    str.Format("\t\tNAME            = \"%s\";\n", Name.GetString());
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tID              = %d;\n", ID);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tENABLE          = %s;\n", Enable ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tOFFSET          = %f, %f, %f;\n", XOffset, YOffset, ZOffset);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tOFFSET_ENABLE   = %s, %s, %s;\n", XOffsetEnable ? "TRUE" : "FALSE",
               YOffsetEnable ? "TRUE" : "FALSE", YOffsetEnable ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tROTATION        = %f, %f, %f;\n", Yaw, Pitch, Roll);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tROTATION_ENABLE = %s, %s, %s;\n", YawEnable ? "TRUE" : "FALSE",
               PitchEnable ? "TRUE" : "FALSE", RollEnable ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    // Write the closing brace.
    file.WriteString("\t}\n");

    return TRUE;
}

// Text only.
BOOL TEMPL_ARTPART::ReadText(CString &src, int *linenum)
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
            Enable = StrToBool((LPCTSTR)value);
        } else if (token == "OFFSET") {
            // Separate the parameters.
            ReadParameter(value, param1, temp);
            ReadParameter(temp, param2, param3);

            XOffset = atof((LPCTSTR)param1);
            YOffset = atof((LPCTSTR)param2);
            ZOffset = atof((LPCTSTR)param3);
        } else if (token == "OFFSET_ENABLE") {
            // Separate the parameters.
            ReadParameter(value, param1, temp);
            ReadParameter(temp, param2, param3);

            XOffsetEnable = StrToBool((LPCTSTR)param1);
            YOffsetEnable = StrToBool((LPCTSTR)param2);
            ZOffsetEnable = StrToBool((LPCTSTR)param3);
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
        } else if (token == "ROTATION_ENABLE") {
            // Separate the parameters.
            ReadParameter(value, param1, temp);
            ReadParameter(temp, param2, param3);

            YawEnable = StrToBool((LPCTSTR)param1);
            PitchEnable = StrToBool((LPCTSTR)param2);
            RollEnable = StrToBool((LPCTSTR)param3);
        } else if (token == "PITCH") {
            Pitch = atof((LPCTSTR)value);
        } else if (token == "ROLL") {
            Roll = atof((LPCTSTR)value);
        } else if (token == "YAW") {
            Yaw = atof((LPCTSTR)value);
        } else if (token == "X_OFFSET_ENABLE") {
            XOffsetEnable = StrToBool((LPCTSTR)value);
        } else if (token == "Y_OFFSET_ENABLE") {
            YOffsetEnable = StrToBool((LPCTSTR)value);
        } else if (token == "Z_OFFSET_ENABLE") {
            ZOffsetEnable = StrToBool((LPCTSTR)value);
        } else if (token == "ROLL_ENABLE") {
            RollEnable = StrToBool((LPCTSTR)value);
        } else if (token == "PITCH_ENABLE") {
            PitchEnable = StrToBool((LPCTSTR)value);
        } else if (token == "YAW_ENABLE") {
            YawEnable = StrToBool((LPCTSTR)value);
        } else {
            PopupReadErrorMsg(KEYWORD, (LPCTSTR)token, linenum);
        }

        readstatus = ReadLine(str, linenum, token, value);
    }

    return readstatus;
}
