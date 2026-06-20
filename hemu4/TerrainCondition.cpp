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

// TerrainCondition.cpp: implementation of the CTerrainCondition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hemu4.h"
#include "TerrainCondition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CTerrainCondition, CObject, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTerrainCondition::CTerrainCondition()
{
    m_ID = -1;
    m_Htree = NULL;
}

CTerrainCondition::CTerrainCondition(const int id)
{
    m_ID = id;
    m_Htree = NULL;
}

CTerrainCondition::~CTerrainCondition()
{
}

void CTerrainCondition::Serialize(CArchive &ar)
{
    TRACE0("\nCCigiView::Serialize()\n");

    int length = 0;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)m_Name);
        ar << length;
        ar.Write((LPCTSTR)m_Name, length);  // NULL terminator not copied

        // Store the shared data.
        ar << m_ID;
        ar << m_Scope;
        ar << m_Severity;
        ar << m_Coverage;
        ar << m_Enable;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("	length = %d\n", length);
        TRACE1("	Name = \"%s\"\n", (LPCTSTR)m_Name);
    } else {
        // Read the name.
        ar >> length;
        ar.Read(m_Name.GetBuffer(length), length);
        m_Name.ReleaseBuffer(length);

        ar << m_ID;
        ar << m_Scope;
        ar << m_Severity;
        ar << m_Coverage;
        ar << m_Enable;

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("	length = %d\n", length);
        TRACE1("	Name = \"%s\"\n", (LPCTSTR)m_Name);
    }
}
