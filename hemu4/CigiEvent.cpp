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
 *  FILENAME:   CigiEvent.cpp
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
#include "hemu4.h"
#include "CigiEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

const unsigned char CCigiEvent::DataTypeInt32   = 0;
const unsigned char CCigiEvent::DataTypeUInt32  = 1;
const unsigned char CCigiEvent::DataTypeFloat   = 2;
const unsigned char CCigiEvent::DataTypeInt64   = 3;
const unsigned char CCigiEvent::DataTypeUInt64  = 4;
const unsigned char CCigiEvent::DataTypeDouble  = 5;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CCigiEvent, CObject, 0)

CCigiEvent::CCigiEvent()
{
    m_ID = -1;

    for (int i = 0; i < 3; i++) {
        m_Data[i] = 0;
        m_DataType[i] = -1;     // Invalid
        m_DataSet[i] = 0;
    }

    m_Components.RemoveAll();
    m_Components.InitHashTable(521);
}

CCigiEvent::~CCigiEvent()
{
    ClearComponents();
}

CCigiEvent &CCigiEvent::operator=(const CCigiEvent &rhs)
{
    m_ID = rhs.m_ID;
    m_Name = rhs.m_Name;

    for (int i = 0; i < 3; i++) {
        m_Data[i] = rhs.m_Data[i];
        m_DataType[i] = rhs.m_DataType[i];
        m_DataSet[i] = rhs.m_DataSet[i];
    }

    ClearComponents();

    POSITION pos = m_Components.GetStartPosition();
    while (pos) {
        int id = -1;
        CComponent *comp = NULL;
        rhs.m_Components.GetNextAssoc(pos, id, comp);
        m_Components.SetAt(id, comp);
    }

    return *this;
}

void CCigiEvent::Serialize(CArchive &ar)
{
    TRACE0("CComponent::Serialize()\n");

    int length = 0;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)m_Name);
        ar << length;
        ar.Write((LPCTSTR)m_Name, length);  // NULL terminator not copied

        ar << m_ID;

        for (int i = 0; i < 6; i++) {
            ar << m_Data[i];
            ar << m_DataType[i];
            ar << m_DataSet[i];
        }

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("		length = %d\n", length);
        TRACE1("		Name = \"%s\"\n", (LPCTSTR)m_Name);
        TRACE1("		ID = %d\n", m_ID);
    } else {
        // Read the name.
        ar >> length;
        ar.Read(m_Name.GetBuffer(length), length);
        m_Name.ReleaseBuffer(length);

        ar >> m_ID;

        for (int i = 0; i < 6; i++) {
            ar >> m_Data[i];
            ar >> m_DataType[i];
            ar >> m_DataSet[i];
        }

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("		length = %d\n", length);
        TRACE1("		Name = \"%s\"\n", (LPCTSTR)m_Name);
        TRACE1("		ID = %d\n", m_ID);
    }
}

void CCigiEvent::ClearComponents(void)
{
    CDebugTrace trace("CCigiEvent::ClearComponents()");

    int id = 0;
    CComponent *comp = NULL;
    POSITION pos = m_Components.GetStartPosition();

    while (pos) {
        m_Components.GetNextAssoc(pos, id, comp);
        delete comp;
    }

    m_Components.RemoveAll();
}

void *CCigiEvent::GetData(const int wordnum, int *datatype)
{
    // wordnum is a 1-based index, so convert to 0-based.
    unsigned int idx = (unsigned int)wordnum - 1;

    // If we have an invalid word number, do nothing.
    if (idx > 5) {
        *datatype = -1;
        return NULL;
    }

    if (m_DataSet[idx]) {
        *datatype = m_DataType[idx];
        return &(m_Data[idx]);
    } else {
        *datatype = -1;
        return NULL;
    }
}

BOOL CCigiEvent::SetData(const int wordnum, const int datatype, void const *value)
{
    // wordnum is a 1-based index, so convert to 0-based.
    unsigned int idx = (unsigned int)wordnum - 1;

    // If we have an invalid word number, do nothing.
    if (idx > 2)
        return FALSE;

    // If we have an invalid datatype or value is NULL, mark as not set.
    if (((unsigned int)datatype > DataTypeDouble) || (value == NULL)) {
        m_DataSet[idx] = 0;
        m_Data[idx] = 0;

        return TRUE;
    }

    // See if we are using a 64-bit datatype.
    BOOL Is64 = ((m_DataType[idx] == DataTypeInt64) ||
                 (m_DataType[idx] == DataTypeUInt64) ||
                 (m_DataType[idx] == DataTypeDouble));

    // If we are using the last word but are trying to store
    // a 64-bit value, do nothing.
    if ((idx == 2) && Is64) {
        return FALSE;
    }

    switch (datatype) {
    case DataTypeInt32:
        *((__int32 *)&m_Data[idx]) = *((__int32 *)value);
        break;

    case DataTypeUInt32:
        *((unsigned __int32 *)&m_Data[idx]) = *((unsigned __int32 *)value);
        break;

    case DataTypeFloat:
        *((float *)&m_Data[idx]) = *((float *)value);
        break;

    case DataTypeInt64:
        m_DataSet[idx + 1] = 0;
        *((__int64 *)&m_Data[idx]) = *((__int64 *)value);
        break;

    case DataTypeUInt64:
        m_DataSet[idx + 1] = 0;
        *((unsigned __int64 *)&m_Data[idx]) = *((unsigned __int64 *)value);
        break;

    case DataTypeDouble:
        m_DataSet[idx + 1] = 0;
        *((double *)&m_Data[idx]) = *((double *)value);
        break;

    default:
        return FALSE;
    }

    m_DataType[idx] = datatype;
    m_DataSet[idx] = 1;

    return TRUE;
}

template <> void AFXAPI SerializeElements<CCigiEvent *>(CArchive &ar, CCigiEvent **pElements, int nCount)
{
    if (ar.IsStoring()) {
        for (int i = 0; i < nCount; i++) {
            if (pElements[i])
                pElements[i]->Serialize(ar);
        }
    } else {
        for (int i = 0; i < nCount; i++) {
            pElements[i] = new CCigiEvent;
            pElements[i]->Serialize(ar);
        }
    }
}
