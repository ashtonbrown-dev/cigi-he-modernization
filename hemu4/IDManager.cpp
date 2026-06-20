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
 *  FILENAME:   IDManager.cpp
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
#include "hemu4.h"
#include "IDManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CIDManager, CObject, 0)

CIDManager::CIDManager()
{
    m_InitialID = 0;
    m_IDArray.SetSize(128, 64);
}

CIDManager::CIDManager(const int initial_id)
{
    m_InitialID = initial_id;
    m_IDArray.SetSize(128, 64);
}

CIDManager::~CIDManager()
{
}

void CIDManager::Serialize(CArchive &ar)
{
    TRACE0("\nCIDManager::Serialize()");

    if (ar.IsStoring()) {
        ar << m_InitialID;
        m_IDArray.Serialize(ar);
    } else {
        ar >> m_InitialID;
        m_IDArray.Serialize(ar);
    }
}

int CIDManager::GetNextID(void)
{
    int i = 0;
    int val = 0;
    int arraysize = m_IDArray.GetSize();

    while (i < arraysize) {
        val = m_IDArray[i];

        if (!val) {
            m_IDArray[i] = 1;
            return i;
        }

        i++;
    }

    // We have overrun our array, so grow it and use the new index as the ID.
    m_IDArray.SetAtGrow(arraysize, 1);
    return arraysize;
}