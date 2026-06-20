/** <pre>
 *  Copyright 2004 The Boeing Company
 *  Author: Lance Durham
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
 * </pre>
 */

// DebugTrace.h: interface for the CDebugTrace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DTTRACE_H__C8AD1286_E95F_458C_B029_2637D4B62862__INCLUDED_)
#define AFX_DTTRACE_H__C8AD1286_E95F_458C_B029_2637D4B62862__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"

class CDebugTrace
{
public:
    enum { IndentSize = 2 };

    CDebugTrace(LPCTSTR func_name);
    ~CDebugTrace();

private:
    CDebugTrace();

#ifdef _USE_DEBUGTRACE
    static int m_IndentCount;
#endif
};

// The release version of this class does absolutely nothing.

#ifndef _USE_DEBUGTRACE

inline CDebugTrace::CDebugTrace()
{
}

inline CDebugTrace::CDebugTrace(LPCTSTR func_name)
{
}

inline CDebugTrace::~CDebugTrace()
{
}

#endif  // _USE_DEBUGTRACE

#endif  // !defined(AFX_DTTRACE_H__C8AD1286_E95F_458C_B029_2637D4B62862__INCLUDED_)
