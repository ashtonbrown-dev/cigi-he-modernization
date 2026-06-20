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
 *  FILENAME:   DebugTrace.cpp
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

#include "stdafx.h"
#include "DebugTrace.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#ifdef _USE_DEBUGTRACE

int CDebugTrace::m_IndentCount = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDebugTrace::CDebugTrace()
{
    m_IndentCount++;
}

CDebugTrace::CDebugTrace(LPCTSTR func_name)
{
    CString indent, str;

    indent += char(0xB3);

    for (int i = 0; i < IndentSize - 1; i++)
        indent += " ";

    for (int i = 0; i < m_IndentCount; i++)
        str += indent;

    str += char(0xDA);
    str += func_name;
    str += "\n";

    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), (LPCTSTR)str,
                 str.GetLength(), NULL, NULL);

    m_IndentCount++;
}

CDebugTrace::~CDebugTrace()
{
    CString indent, str;

    m_IndentCount--;

    indent += char(0xB3);

    for (int i = 0; i < IndentSize - 1; i++)
        indent += " ";

    for (int i = 0; i < m_IndentCount; i++)
        str += indent;

    str += char(0xC0);
    str += "Returning";
    str += "\n";

    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), (LPCTSTR)str,
                 str.GetLength(), NULL, NULL);
}

#endif