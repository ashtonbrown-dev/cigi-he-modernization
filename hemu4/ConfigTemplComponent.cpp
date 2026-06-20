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
 *  FILENAME:   ConfigTemplComponent.cpp
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
#include "ConfigTemplComponent.h"
#include "globals.h"

const unsigned char TEMPL_COMPONENT::DataTypeInt32  = 0;
const unsigned char TEMPL_COMPONENT::DataTypeUInt32 = 1;
const unsigned char TEMPL_COMPONENT::DataTypeFloat  = 2;
const unsigned char TEMPL_COMPONENT::DataTypeInt64  = 3;
const unsigned char TEMPL_COMPONENT::DataTypeUInt64 = 4;
const unsigned char TEMPL_COMPONENT::DataTypeDouble = 5;

///////////////////////////////////////////////////////////////////////////////
//
//  TEMPL_COMPONENT_STATE
//
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(TEMPL_COMPONENT_STATE, CObject, 0)

TEMPL_COMPONENT_STATE::TEMPL_COMPONENT_STATE()
{
    Value = -1;
}

TEMPL_COMPONENT_STATE::~TEMPL_COMPONENT_STATE()
{
}

TEMPL_COMPONENT_STATE &TEMPL_COMPONENT_STATE::operator=(const TEMPL_COMPONENT_STATE &rhs)
{
    Name = rhs.Name;
    Value = rhs.Value;

    return *this;
}

// Binary only.
void TEMPL_COMPONENT_STATE::Serialize(CArchive &ar)
{
    TRACE0("\n--    TEMPL_COMPONENT_STATE::Serialize()\n");

    int length = 0;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)Name);
        ar << length;
        ar.Write((LPCTSTR)Name, length);    // NULL terminator not copied

        // Store the integral types.
        ar << Value;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("			length = %d\n", length);
        TRACE1("			Name = \"%s\"\n", (LPCTSTR)Name);
        TRACE1("			Value = %d\n", Value);
    } else {
        // Read the name.
        ar >> length;
        ar.Read(Name.GetBuffer(length), length);
        Name.ReleaseBuffer(length);

        // Read the integral types.
        ar >> Value;

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("			length = %d\n", length);
        TRACE1("			Name = \"%s\"\n", (LPCTSTR)Name);
        TRACE1("			Value = %d\n", Value);
    }
}

// Text only.
BOOL TEMPL_COMPONENT_STATE::WriteText(CStdioFile &file)
{
    CString str;

    // Write the header and opening brace.
    file.WriteString("\n\t\tSTATE\n\t\t{\n");

    // Store the name and value.
    str.Format("\t\t\tNAME  = \"%s\";\n", Name);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\t\tVALUE = %d;\n", Value);
    file.WriteString((LPCTSTR)str);

    // Write the closing brace.
    file.WriteString("\t\t}\n");

    return TRUE;
}

// Text only.
BOOL TEMPL_COMPONENT_STATE::ReadText(CString &src, int *linenum)
{
    BOOL readstatus;
    CString str, temp, token, value;

    // We should be starting just inside the opening brace.

    // Get the component state block.
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
        } else if (token == "VALUE") {
            Value = atoi((LPCTSTR)value);
        } else {
            PopupReadErrorMsg(KEYWORD, (LPCTSTR)token, linenum);
        }

        readstatus = ReadLine(str, linenum, token, value);
    }

    return readstatus;
}

///////////////////////////////////////////////////////////////////////////////
//
//  TEMPL_COMPONENT
//
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(TEMPL_COMPONENT, CObject, 0)

TEMPL_COMPONENT::TEMPL_COMPONENT()
{
    ID = -1;
    InstID = -1;
    DefState = -1;
    ComponentClass = -1;
    DefData[0] = 0;
    DefData[1] = 0;
    DefData[2] = 0;
    DefData[3] = 0;
    DefData[4] = 0;
    DefData[5] = 0;
    DefDataType[0] = DataTypeInt32;
    DefDataType[1] = DataTypeInt32;
    DefDataType[2] = DataTypeInt32;
    DefDataType[3] = DataTypeInt32;
    DefDataType[4] = DataTypeInt32;
    DefDataType[5] = DataTypeInt32;
    DefDataSet[0] = 0;
    DefDataSet[1] = 0;
    DefDataSet[2] = 0;
    DefDataSet[3] = 0;
    DefDataSet[4] = 0;
    DefDataSet[5] = 0;
    reserved1 = 0;
    reserved2 = 0;
    reserved3 = 0;
    reserved4 = 0;
}

TEMPL_COMPONENT::~TEMPL_COMPONENT()
{
}

TEMPL_COMPONENT &TEMPL_COMPONENT::operator=(const TEMPL_COMPONENT &rhs)
{
    unsigned long handle = NULL;
    TEMPL_COMPONENT_STATE *compstate = NULL;

    Name = rhs.Name;
    ID = rhs.ID;
    InstID = rhs.InstID;
    DefState = rhs.DefState;
    ComponentClass = rhs.ComponentClass;
    reserved1 = rhs.reserved1;
    reserved2 = rhs.reserved2;
    reserved1 = rhs.reserved3;
    reserved2 = rhs.reserved4;

    for (int i = 0; i < 6; i++) {
        DefData[i] = rhs.DefData[i];
        DefDataType[i] = rhs.DefDataType[i];
        DefDataSet[i] = rhs.DefDataSet[i];
    }

    StateList.RemoveAll();
    compstate = rhs.StateList.GetHead(&handle);
    while (compstate) {
        StateList.InsertTail(*compstate);
        compstate = rhs.StateList.GetNext(&handle);
    }

    return *this;
}

// Binary only.
void TEMPL_COMPONENT::Serialize(CArchive &ar)
{
    //Sleep(1000);  // Use to keep from overrunning buffer in debugger.
    TRACE0("\n--   TEMPL_COMPONENT::Serialize()\n");

    int length = 0;
    int count = 0;
    int i = 0;
    unsigned long junk;
    unsigned long handle = NULL;
    TEMPL_COMPONENT_STATE *compstate = NULL;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)Name);
        ar << length;
        ar.Write((LPCTSTR)Name, length);    // NULL terminator not copied

        // Store the integral types.
        ar << ID;
        ar << InstID;
        ar << ComponentClass;
        ar << DefState;
        ar << DefData[0];
        ar << DefData[1];
        ar << DefData[2];
        ar << DefData[3];
        ar << DefData[4];
        ar << DefData[5];
        ar << DefDataType[0];
        ar << DefDataType[1];
        ar << DefDataType[2];
        ar << DefDataType[3];
        ar << DefDataType[4];
        ar << DefDataType[5];
        ar << DefDataSet[0];
        ar << DefDataSet[1];
        ar << DefDataSet[2];
        ar << DefDataSet[3];
        ar << DefDataSet[4];
        ar << DefDataSet[5];
        ar << reserved1;
        ar << reserved2;
        ar << reserved3;
        ar << reserved4;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        // Store the component states.
        count = StateList.GetItemCount();
        ar << count;

        TRACE1("		length = %d\n", length);
        TRACE1("		Name = \"%s\"\n", (LPCTSTR)Name);
        TRACE1("		ID = %d\n", ID);
        TRACE1("		Instance ID = %d\n", InstID);
        TRACE1("		ComponentClass = %d\n", ComponentClass);
        TRACE1("		DefState = %d\n", DefState);
        TRACE1("		DefData[0] = %x\n", DefData[0]);
        TRACE1("		DefData[1] = %x\n", DefData[1]);
        TRACE1("		DefData[2] = %x\n", DefData[2]);
        TRACE1("		DefData[3] = %x\n", DefData[3]);
        TRACE1("		DefData[4] = %x\n", DefData[4]);
        TRACE1("		DefData[5] = %x\n", DefData[5]);
        TRACE1("		DefDataType[0] = %d\n", DefDataType[0]);
        TRACE1("		DefDataType[1] = %d\n", DefDataType[1]);
        TRACE1("		DefDataType[2] = %d\n", DefDataType[2]);
        TRACE1("		DefDataType[3] = %d\n", DefDataType[3]);
        TRACE1("		DefDataType[4] = %d\n", DefDataType[4]);
        TRACE1("		DefDataType[5] = %d\n", DefDataType[5]);
        TRACE1("		DefDataSet[0] = %d\n", DefDataSet[0]);
        TRACE1("		DefDataSet[1] = %d\n", DefDataSet[1]);
        TRACE1("		DefDataSet[2] = %d\n", DefDataSet[2]);
        TRACE1("		DefDataSet[3] = %d\n", DefDataSet[3]);
        TRACE1("		DefDataSet[4] = %d\n", DefDataSet[4]);
        TRACE1("		DefDataSet[5] = %d\n", DefDataSet[5]);
        TRACE1("		reserved1 = %d\n", reserved1);
        TRACE1("		reserved2 = %d\n", reserved2);
        TRACE1("		reserved3 = %d\n", reserved3);
        TRACE1("		reserved4 = %d\n", reserved4);
        TRACE1("		state count = %d\n", count);

        if (count) {
            compstate = StateList.GetHead(&handle);
            while (compstate) {
                compstate->Serialize(ar);
                compstate = StateList.GetNext(&handle);
            };
        }
    } else {
        // Read the name.
        ar >> length;
        ar.Read(Name.GetBuffer(length), length);
        Name.ReleaseBuffer(length);

        // Read the integral types.
        ar >> ID;
        ar >> InstID;
        ar >> ComponentClass;
        ar >> DefState;
        ar >> DefData[0];
        ar >> DefData[1];
        ar >> DefData[2];
        ar >> DefData[3];
        ar >> DefData[4];
        ar >> DefData[5];
        ar >> DefDataType[0];
        ar >> DefDataType[1];
        ar >> DefDataType[2];
        ar >> DefDataType[3];
        ar >> DefDataType[4];
        ar >> DefDataType[5];
        ar >> DefDataSet[0];
        ar >> DefDataSet[1];
        ar >> DefDataSet[2];
        ar >> DefDataSet[3];
        ar >> DefDataSet[4];
        ar >> DefDataSet[5];
        ar >> reserved1;
        ar >> reserved2;
        ar >> reserved3;
        ar >> reserved4;

        // Read the padding.
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        // Read in the component states.
        ar >> count;

        TRACE1("		length = %d\n", length);
        TRACE1("		Name = \"%s\"\n", (LPCTSTR)Name);
        TRACE1("		ID = %d\n", ID);
        TRACE1("		Instance ID = %d\n", InstID);
        TRACE1("		ComponentClass = %d\n", ComponentClass);
        TRACE1("		DefState = %d\n", DefState);
        TRACE1("		DefData[0] = %x\n", DefData[0]);
        TRACE1("		DefData[1] = %x\n", DefData[1]);
        TRACE1("		DefData[2] = %x\n", DefData[2]);
        TRACE1("		DefData[3] = %x\n", DefData[3]);
        TRACE1("		DefData[4] = %x\n", DefData[4]);
        TRACE1("		DefData[5] = %x\n", DefData[5]);
        TRACE1("		DefDataType[0] = %d\n", DefDataType[0]);
        TRACE1("		DefDataType[1] = %d\n", DefDataType[1]);
        TRACE1("		DefDataType[2] = %d\n", DefDataType[2]);
        TRACE1("		DefDataType[3] = %d\n", DefDataType[3]);
        TRACE1("		DefDataType[4] = %d\n", DefDataType[4]);
        TRACE1("		DefDataType[5] = %d\n", DefDataType[5]);
        TRACE1("		DefDataSet[0] = %d\n", DefDataSet[0]);
        TRACE1("		DefDataSet[1] = %d\n", DefDataSet[1]);
        TRACE1("		DefDataSet[2] = %d\n", DefDataSet[2]);
        TRACE1("		DefDataSet[3] = %d\n", DefDataSet[3]);
        TRACE1("		DefDataSet[4] = %d\n", DefDataSet[4]);
        TRACE1("		DefDataSet[5] = %d\n", DefDataSet[5]);
        TRACE1("		reserved1 = %d\n", reserved1);
        TRACE1("		reserved2 = %d\n", reserved2);
        TRACE1("		reserved3 = %d\n", reserved3);
        TRACE1("		reserved4 = %d\n", reserved4);
        TRACE1("		state count = %d\n", count);

        for (i = 0; i < count; i++) {
            TEMPL_COMPONENT_STATE newstate;
            newstate.Serialize(ar);
            StateList.InsertTail(newstate);
        }
    }
}

// Text only.
BOOL TEMPL_COMPONENT::WriteText(CStdioFile &file)
{
    unsigned long handle = 0;
    CString str;
    TEMPL_COMPONENT_STATE *compstate = NULL;

    // Write the header and opening brace.
    file.WriteString("\n\tCOMPONENT\n\t{\n");

    // Store the data.
    str.Format("\t\tNAME        = \"%s\";\n", Name);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tID          = %d;\n", ID);
    file.WriteString((LPCTSTR)str);

    str.Format("\t\tINSTANCE_ID = %d;\n", InstID);
    file.WriteString((LPCTSTR)str);

    if ((ComponentClass == COMP_CLASS_G_SEA) ||
        (ComponentClass == COMP_CLASS_G_TERRAIN) ||
        (ComponentClass == COMP_CLASS_ATMOSPHERE) ||
        (ComponentClass == COMP_CLASS_CELSPHERE)) {
        str.Format("\t\tCOMPONENT_CLASS = %s\n", CompClassStr[ComponentClass]);
        file.WriteString((LPCTSTR)str);
    }

    if (DefState > -1) {
        str.Format("\t\tDEF_STATE   = %d;\n", DefState);
        file.WriteString((LPCTSTR)str);
    }

    for (int i = 0; i < 6; i++) {
        if ((DefDataSet[i] != 1) && (DefDataType[i] > 0)) {
            str.Format("\t\tDEF_DATA%d   = %s, %d;\n", i + 1,
                       DataTypeStr[DefDataType[i]], DefData[i]);
            file.WriteString((LPCTSTR)str);
        }
    }

    compstate = StateList.GetHead(&handle);
    while (compstate) {
        compstate->WriteText(file);
        compstate = StateList.GetNext(&handle);
    };

    // Write the closing brace.
    file.WriteString("\t}\n");

    return TRUE;
}

// Text only.
BOOL TEMPL_COMPONENT::ReadText(CString &src, int *linenum)
{
    BOOL readstatus;
    CString str, temp, token, value, param1, param2;

    // We should be starting just inside the opening brace.

    // Get the component block.
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
        } else if (token == "INSTANCE_ID") {
            InstID = atoi((LPCTSTR)value);
        } else if (token == "COMPONENT_CLASS") {
            ComponentClass = StrToCompClass((LPCTSTR)value);
        } else if ((token == "DEFAULT_STATE") || (token == "DEF_STATE")) {
            DefState = atoi((LPCTSTR)value);
        } else if ((token.Left(12) == "DEFAULT_DATA") || (token.Left(8) == "DEF_DATA")) {
            unsigned int index = ((LPCTSTR)token)[8] - 48;  // Convert char to integer
            index--;                                        // For zero-indexed array

            if (index > 5) {
                PopupReadErrorMsg(KEYWORD, (LPCTSTR)token, linenum);
            }

            // Separate the two parameters.
            ReadParameter(value, param1, param2);

            DefDataType[index] = StrToDatatype((LPCTSTR)param2);
            switch (DefDataType[index]) {
            case DataTypeInt32:
                sscanf((LPCTSTR)param1, "%ld", (long *) & (DefData[index]));
                DefDataSet[index] = 1;
                break;

            case DataTypeUInt32:
                sscanf((LPCTSTR)param1, "%lu", (unsigned long *) & (DefData[index]));
                DefDataSet[index] = 1;
                break;

            case DataTypeFloat:
                sscanf((LPCTSTR)param1, "%f", (float *) & (DefData[index]));
                DefDataSet[index] = 1;
                break;

            case DataTypeInt64:
                if (index == 5)
                    PopupReadErrorMsg(DATATYPE, (LPCTSTR)param1, linenum);
                else {
                    sscanf((LPCTSTR)param1, "%I64d", (__int64 *) & (DefData[index]));
                    DefDataSet[index] = 1;
                    DefDataSet[index + 1] = 0;
                }

                break;

            case DataTypeUInt64:
                if (index == 5)
                    PopupReadErrorMsg(DATATYPE, (LPCTSTR)param1, linenum);
                else {
                    sscanf((LPCTSTR)param1, "%I64u", (unsigned __int64 *) & (DefData[index]));
                    DefDataSet[index] = 1;
                    DefDataSet[index + 1] = 0;
                }

                break;

            case DataTypeDouble:
                if (index == 5)
                    PopupReadErrorMsg(DATATYPE, (LPCTSTR)param1, linenum);
                else {
                    sscanf((LPCTSTR)param1, "%lf", (double *) & (DefData[index]));
                    DefDataSet[index] = 1;
                    DefDataSet[index + 1] = 0;
                }

                break;

            default:
                PopupReadErrorMsg(KEYWORD, (LPCTSTR)param1, linenum);
                break;
            }
        } else if (token == "STATE") {
            TEMPL_COMPONENT_STATE state;
            state.ReadText(str, linenum);

            StateList.InsertTail(state);
        } else {
            PopupReadErrorMsg(KEYWORD, (LPCTSTR)token, linenum);
        }

        readstatus = ReadLine(str, linenum, token, value);
    }

    return readstatus;
}

TEMPL_COMPONENT_STATE *TEMPL_COMPONENT::FindState(const int value)
{
    unsigned long handle = 0;
    TEMPL_COMPONENT_STATE *state = StateList.GetHead(&handle);

    while (state) {
        if (state->Value == value)
            return state;

        state = StateList.GetNext(&handle);
    }

    return NULL;
}
