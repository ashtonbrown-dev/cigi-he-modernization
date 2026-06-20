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

// Wave.cpp: implementation of the CWave class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hemu4.h"
#include "Wave.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CWave, CObject, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWave::CWave(const int id)
{
    m_ID = id;
    m_Htree = NULL;
}

CWave::CWave()
{
    m_ID = -1;
    m_Htree = NULL;
}

CWave::~CWave()
{
}

void CWave::Serialize(CArchive &ar)
{
    TRACE0("\nCCigiView::Serialize()\n");

    int length = 0;
    double morejunk;

    if (ar.IsStoring()) {
        ar << m_ID;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << double(0);
        ar << double(0);
        ar << double(0);
        ar << double(0);
        ar << double(0);
        ar << double(0);
        ar << double(0);
        ar << double(0);

        TRACE1("	ID = %d\n", m_ID);
    } else {
        ar >> m_ID;

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> morejunk;
        ar >> morejunk;
        ar >> morejunk;
        ar >> morejunk;
        ar >> morejunk;
        ar >> morejunk;
        ar >> morejunk;
        ar >> morejunk;

        TRACE1("	ID = %d\n", m_ID);
    }
}


template <> void AFXAPI SerializeElements<CWave *>(CArchive &ar, CWave **pElements, int nCount)
{
    if (ar.IsStoring()) {
        for (int i = 0; i < nCount; i++) {
            if (pElements[i])
                pElements[i]->Serialize(ar);
        }
    } else {
        for (int i = 0; i < nCount; i++) {
            pElements[i] = new CWave(-1);

            pElements[i]->Serialize(ar);
        }
    }
}
