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
 *  FILENAME:   AttachPoint.cpp
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
#include "AttachPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CAttachPoint, CObject, 0)

CAttachPoint::CAttachPoint()
{
    m_ID = -1;
    m_XOffset = 0.0;
    m_YOffset = 0.0;
    m_ZOffset = 0.0;
    m_Roll = 0.0;
    m_Pitch = 0.0;
    m_Yaw = 0.0;
}

CAttachPoint::CAttachPoint(TEMPL_ATTACHPOINT *attach)
{
    m_Name = attach->Name;
    m_ID = attach->ID;
    m_XOffset = attach->XOffset;
    m_YOffset = attach->YOffset;
    m_ZOffset = attach->ZOffset;
    m_Roll = attach->Roll;
    m_Pitch = attach->Pitch;
    m_Yaw = attach->Yaw;
}

CAttachPoint::~CAttachPoint()
{
}

CAttachPoint &CAttachPoint::operator=(const CAttachPoint &rhs)
{
    m_Name = rhs.m_Name;
    m_ID = rhs.m_ID;
    m_XOffset = rhs.m_XOffset;
    m_YOffset = rhs.m_YOffset;
    m_ZOffset = rhs.m_ZOffset;
    m_Roll = rhs.m_Roll;
    m_Pitch = rhs.m_Pitch;
    m_Yaw = rhs.m_Yaw;

    return *this;
}

void CAttachPoint::Serialize(CArchive &ar)
{
    TRACE0("CAttachPoint::Serialize()\n");

    int length = 0;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)m_Name);
        ar << length;
        ar.Write((LPCTSTR)m_Name, length);  // NULL terminator not copied

        // Store the integral types.
        ar << m_ID;
        ar << m_XOffset;
        ar << m_YOffset;
        ar << m_ZOffset;
        ar << m_Roll;
        ar << m_Pitch;
        ar << m_Yaw;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("	length = %d\n", length);
        TRACE1("	m_Name = \"%s\"\n", (LPCTSTR)m_Name);
        TRACE1("	m_ID = %d\n", m_ID);
        TRACE1("	m_XOffset = %f\n", m_XOffset);
        TRACE1("	m_YOffset = %f\n", m_YOffset);
        TRACE1("	m_ZOffset = %f\n", m_ZOffset);
        TRACE1("	m_Roll = %f\n", m_Roll);
        TRACE1("	m_Pitch = %f\n", m_Pitch);
        TRACE1("	m_Yaw = %f\n", m_Yaw);
    } else {
        // Read the name.
        ar >> length;
        ar.Read(m_Name.GetBuffer(length), length);
        m_Name.ReleaseBuffer(length);

        // Store the integral types.
        ar >> m_ID;
        ar >> m_XOffset;
        ar >> m_YOffset;
        ar >> m_ZOffset;
        ar >> m_Roll;
        ar >> m_Pitch;
        ar >> m_Yaw;

        // Store some padding in case we have to add something later.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("	length = %d\n", length);
        TRACE1("	m_Name = \"%s\"\n", (LPCTSTR)m_Name);
        TRACE1("	m_ID = %d\n", m_ID);
        TRACE1("	m_XOffset = %f\n", m_XOffset);
        TRACE1("	m_YOffset = %f\n", m_YOffset);
        TRACE1("	m_ZOffset = %f\n", m_ZOffset);
        TRACE1("	m_Roll = %f\n", m_Roll);
        TRACE1("	m_Pitch = %f\n", m_Pitch);
        TRACE1("	m_Yaw = %f\n", m_Yaw);
    }
}


template <> void AFXAPI SerializeElements<CAttachPoint *>(CArchive &ar, CAttachPoint **pElements, int nCount)
{
    if (ar.IsStoring()) {
        for (int i = 0; i < nCount; i++) {
            if (pElements[i])
                pElements[i]->Serialize(ar);
        }
    } else {
        for (int i = 0; i < nCount; i++) {
            TEMPL_ATTACHPOINT dummy;
            pElements[i] = new CAttachPoint(&dummy);
            pElements[i]->Serialize(ar);
        }
    }
}
