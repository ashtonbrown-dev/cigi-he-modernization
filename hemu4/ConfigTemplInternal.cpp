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
 *  FILENAME:   ConfigTemplInternal.cpp
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
#include "globals.h"

const char DataTypeStr[6][7] = {{"INT32"},
    {"UINT32"},
    {"FLOAT"},
    {"INT64"},
    {"UINT64"},
    {"DOUBLE"}
};

const char CompClassStr[14][17] = { {"ENTITY"},
    {"VIEW"},
    {"VIEWGROUP"},
    {"SENSOR"},
    {"REGIONAL_SEA"},
    {"REGIONAL_TERRAIN"},
    {"REGIONAL_WEATHER"},
    {"GLOBAL_SEA"},
    {"GLOBAL_TERRAIN"},
    {"GLOBAL_WEATHER"},
    {"ATMOSPHERE"},
    {"CELESTIAL_SPHERE"},
    {"EVENT"},
    {"SYSTEM"}
};

const char EntityClassStr[13][12] = {   {"GENERIC"},
    {"FIXEDWING"},
    {"ANIMATION"},
    {"WEATHER"},
    {"MISSILE"},
    {"GROUNDMIL"},
    {"GROUNDCIV"},
    {"WATERCRAFT"},
    {"ROTORCRAFT"},
    {"BOMB"},
    {"SAM"},
    {"TESTPATTERN"},
    {"CULTURE"}
};

const char EntityClampStr[3][13] = {{"NONE"},
    {"NONCONFORMAL"},
    {"CONFORMAL"}
};

const char CollisionVolTypeStr[2][7] = {{"SPHERE"},
    {"CUBOID"}
};

const char ProjectionTypeStr[2][12] = { {"PERSPECTIVE"},
    {"PARALLEL"}
};

const char ReplicationStr[4][5] = { {"NONE"},
    {"1X2"},
    {"2X1"},
    {"2X2"}
};

const char MirrorModeStr[4][11] = { {"NONE"},
    {"HORIZONTAL"},
    {"VERTICAL"},
    {"BOTH"}
};

const int UNKNOWN = -1;
const int SYMBOL = 0;
const int KEYWORD = 1;
const int DATATYPE = 2;

char s_Filename[256] = {'\0'};

///////////////////////////////////////////////////////////////////////////////
//
//  Common Functions
//
///////////////////////////////////////////////////////////////////////////////

int StrToEntityClass(LPCTSTR str)
{
    CString temp = str;
    temp.MakeUpper();

    for (int i = 0; i < 13; i++) {
        if (temp == EntityClassStr[i])
            return i;
    }

    return 0;
}

int StrToDatatype(LPCTSTR str)
{
    CString temp = str;
    temp.MakeUpper();

    for (int i = 0; i < 6; i++) {
        if (temp == DataTypeStr[i])
            return i;
    }

    return 0;
}

int StrToCompClass(LPCTSTR str)
{
    CString temp = str;
    temp.MakeUpper();

    for (int i = 0; i < 14; i++) {
        if (temp == CompClassStr[i])
            return i;
    }

    return 0;
}

int StrToVolType(LPCTSTR str)
{
    CString temp = str;
    temp.MakeUpper();

    for (int i = 0; i < 2; i++) {
        if (temp == CollisionVolTypeStr[i])
            return i;
    }

    return 0;
}

BOOL StrToBool(LPCTSTR str)
{
    CString temp = str;
    temp.MakeUpper();

    return ((temp == "TRUE") || (temp == "ON") || (temp == "YES") || (temp == "1"));
}

void MakeDate(LPCTSTR str, unsigned char *month, unsigned char *day, int *year)
{
    int m = 0;
    int d = 0;

    sscanf(str, "%d/%d/%d", &m, &d, year);
    *month = m;
    *day = d;
}

void MakeTime(LPCTSTR str, unsigned char *hour, unsigned char *minute)
{
    int h = 0;
    int m = 0;

    sscanf(str, "%d:%d", &h, &m);
    *hour = h;
    *minute = m;
}

void PopupReadErrorMsg(const int type, const char *param, const int *linenum)
{
    CString msg;

    switch (type) {
    case SYMBOL:
        msg.Format("%s: Expected \"%s\" at line %d.",
                   s_Filename, param, *linenum);
        break;

    case KEYWORD:
        msg.Format("%s: Unexpected keyword \"%s\" at line %d.",
                   s_Filename, param, *linenum);
        break;

    case DATATYPE:
        msg.Format("%s: Cannot store type \"%s\" at line %d.",
                   s_Filename, param, *linenum);
        break;

    default:
        msg.Format("%s: Unexpected error at line %d.", s_Filename, *linenum);
        break;
    }

    PrintMessageText((LPCTSTR)msg);
}

//  ReadBlock() --  Reads a block ("TOKEN{...}") from the given file.  Strips
//                  out comments.  The file position is incremented.
CString ReadBlock(CStdioFile &file, int *linenum)
{
    CString temp, str;
    int readstatus;
    int bracecount = 0;
    int idx;
    BOOL breakloop = FALSE;
    BOOL comment = FALSE;

    // Read until we find the closing brace.
    do {
        readstatus = file.ReadString(temp);

        // If we're inside a "/*...*/" comment, find our way out.
        while (comment == TRUE) {
            idx = temp.Find("*/");
            if (idx > -1) {
                temp = temp.Right(temp.GetLength() - idx - 2);  // - 2 for '/'
                comment = FALSE;
            } else {
                readstatus = file.ReadString(temp);
            }
        }

        // If we have a "//..." comment, remove it.
        idx = temp.Find("//");
        if (idx > -1)
            temp = temp.Left(idx);

        // If we have a "/*...*/" comment, remove it and set the flag.
        idx = temp.Find("/*");
        if (idx > -1) {
            CString s = temp.Left(idx);
            comment = TRUE;

            // Take care of the special case when it's all on the same line.
            temp = temp.Right(temp.GetLength() - idx - 2);
            idx = temp.Find("*/");
            if (idx > -1) {
                temp = temp.Right(temp.GetLength() - idx - 2);
                comment = FALSE;
            } else
                temp = "";

            temp = s + temp;
        }

        temp.TrimLeft();
        temp.TrimRight();

        str += temp;

        // If we have a blank line, loop.
        if (temp != "") {
            // Otherwise, count the braces.
            if (temp.Left(1) == "{") {
                bracecount++;
                breakloop = FALSE;
            } else if (temp.Left(1) == "}") {
                bracecount--;

                if (bracecount == 0)
                    breakloop = TRUE;
            }
        }

        (*linenum)++;
    } while ((readstatus == TRUE) && (breakloop == FALSE));

    return str;
}

//  ReadBlock() --  Reads a block ("TOKEN{...}") from the given string.  The
//                  block is removed from the source string.
CString ReadBlock(CString &src, int *linenum)
{
    CString temp;
    int index = 0;
    int total = 0;
    int bracecount = 1;     // Assume we're inside the first opening brace.
    int line = *linenum;

    temp = src;

    // Read until we find the closing brace.
    do {
        index = temp.FindOneOf("{}\n");
        if (index < 0) {
            PopupReadErrorMsg(SYMBOL, "}", &line);
        }

        total += index + 1;     // Account for brace we are skipping.

        if (temp[index] == '{')
            bracecount++;
        else if (temp[index] == '}')
            bracecount--;
        else if (temp[index] == '\n')
            line++;
        else
            PopupReadErrorMsg(UNKNOWN, NULL, &line);

        temp = temp.Right(temp.GetLength() - index - 1);
    } while ((temp != "") && (bracecount > 0));

    // Increment src and return the block.
    temp = src.Left(total);
    src = src.Right(src.GetLength() - total);

    return temp;
}

//  ReadLine() --   Reads a line ("TOKEN" or "TOKEN=VALUE;") from the given
//                  string.  The line is removed from the source string.
BOOL ReadLine(CString IN OUT &str, int *linenum, CString OUT &token, CString OUT &params)
{
    CString temp, findtemp;
    int findidx = 0;
    int index = 0;
    BOOL IsBlockHdr = FALSE;
    BOOL HasQuotes = FALSE;

    // Clear the strings.
    token = "";
    params = "";

    // Strip off any whitespace characters on the left.
    str.TrimLeft();

    // Make sure we ignore lone semicolons correctly.
    if (str.Left(1) == ";") {
        str = str.Right(str.GetLength() - 1);
    }

    // See if we have a closing brace.
    if (str.Left(1) == "}") {
        token = "}";
        str = str.Right(str.GetLength() - 1);   // Don't use Trim() or it will delete all closing braces.

        return TRUE;
    }

    // Read until we find the semicolon or, in the case of a new block, an opening brace.
    index = str.FindOneOf(";{");
    if (index <= 0)
        return FALSE;

    temp = str.Left(index + 1);
    temp.TrimLeft();
    temp.TrimRight();

    // Remove the semicolon if there is one.
    temp.TrimRight(";");
    temp.TrimRight();

    // Increment the start of str.
    str = str.Right(str.GetLength() - index - 1);

    // See if we have quotes.
    index = temp.Find('\"');
    {
        if (index > 0) {
            HasQuotes = TRUE;
            findtemp = temp.Right(temp.GetLength() - index);
        }
    }

    // Remove any whitespaces.
    temp.Remove('\n');
    temp.Remove('\r');
    temp.Remove('\t');
    temp.Remove(' ');

    // If we have quotes, reconstruct the string.
    if (HasQuotes) {
        index = temp.Find('=');
        if (index > -1)
            temp = temp.Left(index) + "=" + findtemp;
    }

    // If we have a block header token, strip off the opening brace.
    if (temp.Right(1) == "{") {
        temp.TrimRight("{");
        token = temp;
    } else { // Otherwise, string is now in the format TOKEN=VALUE.
        // Find the equals.
        index = temp.Find('=');
        if (index <= 0)
            return FALSE;

        token = temp.Left(index);

        params = temp.Right(temp.GetLength() - index - 1);
    }

    token.MakeUpper();

    return TRUE;
}

//  ReadParameter() --  Reads a parameter from the given string.  The
//                      first parameter and remainder are returned.
BOOL ReadParameter(CString IN &str, CString OUT &param1, CString OUT &remainder)
{
    // This function assumes that there are no whitespaces or trailing semicolons.

    CString temp;
    int index = 0;

    // Clear the strings.
    param1 = "";
    remainder = "";

    // Find the comma.
    index = str.Find(',');
    if (index <= 0)
        return FALSE;

    param1 = str.Left(index);
    param1.TrimLeft();
    param1.TrimRight();

    remainder = str.Right(str.GetLength() - index - 1);
    remainder.TrimLeft();
    remainder.TrimRight();
    remainder.MakeUpper();

    return TRUE;
}