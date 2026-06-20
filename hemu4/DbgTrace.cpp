// DebugTrace.cpp: implementation of the CDebugTrace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hemu4.h"
#include "DbgTrace.h"

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
    m_IndentCount += IndentSize;
}

CDebugTrace::CDebugTrace(LPCTSTR func_name)
{
    CString indent, str;
    int i;
    for (i = 0; i < IndentSize - 1; i++)
        indent += " ";

    indent += char(0xB3);

    for (i = 0; i < m_IndentCount; i++)
        str += indent;

    str += char(0xDA);
    str += func_name;
    str += "\n";

    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), (LPCTSTR)str,
                 str.GetLength(), NULL, NULL);

    m_IndentCount += IndentSize;
}

CDebugTrace::~CDebugTrace()
{
    m_IndentCount -= IndentSize;

    CString indent, str;
    int i;
    for (i = 0; i < IndentSize - 1; i++)
        indent += " ";

    indent += char(0xB3);

    for (i = 0; i < m_IndentCount; i++)
        str += indent;

    str += char(0xC0);
    str += "Returning";
    str += "\n";

    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), (LPCTSTR)str,
                 str.GetLength(), NULL, NULL);
}

#endif
