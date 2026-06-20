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
 *  FILENAME:   Component.cpp
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
#include "cigi4types.h"
#include "Component.h"
#include "globals.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

const unsigned char CComponent::DataTypeInt32   = 0;
const unsigned char CComponent::DataTypeUInt32  = 1;
const unsigned char CComponent::DataTypeFloat   = 2;
const unsigned char CComponent::DataTypeInt64   = 3;
const unsigned char CComponent::DataTypeUInt64  = 4;
const unsigned char CComponent::DataTypeDouble  = 5;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CComponent, CObject, 0)

CComponent::CComponent()
{
    m_HasChanged = FALSE;
    m_CompID = -1;
    m_CompClass = -1;
    m_InstID = -1;
    m_DbID = -1;
    m_State = -1;

    for (int i = 0; i < 6; i++) {
        m_Data[i] = 0;
        m_DataType[i] = -1;     // Invalid
        m_DataSet[i] = 0;
    }
}

CComponent::CComponent(const int comp_class, TEMPL_COMPONENT *comp)
{
    m_HasChanged = FALSE;
    m_CompClass = comp_class;
    m_CompID = comp->ID;
    m_InstID = comp->InstID;
    m_State = comp->DefState;
    m_Name = comp->Name;

    for (int i = 0; i < 6; i++) {
        m_Data[i] = comp->DefData[i];
        m_DataType[i] = comp->DefDataType[i];
        m_DataSet[i] = comp->DefDataSet[i];
    }

    // Populate the state list.
    unsigned long handle = 0;
    TEMPL_COMPONENT_STATE *state = comp->StateList.GetHead(&handle);
    while (state) {
        m_StateList.InsertTail(*state);
        state = comp->StateList.GetNext(&handle);
    }

    // LWD: Send a Component Control packet to the IG?
}

CComponent::~CComponent()
{
    m_StateList.RemoveAll();
}

CComponent &CComponent::operator=(const CComponent &rhs)
{
    m_HasChanged = TRUE;
    m_CompID = rhs.m_CompID;
    m_CompClass = rhs.m_CompClass;
    m_InstID = rhs.m_InstID;
    m_DbID = rhs.m_DbID;
    m_State = rhs.m_State;
    m_Name = rhs.m_Name;

    for (int i = 0; i < 6; i++) {
        m_Data[i] = rhs.m_Data[i];
        m_DataType[i] = rhs.m_DataType[i];
        m_DataSet[i] = rhs.m_DataSet[i];
    }

    // Copy the state list.
    unsigned long handle = 0;
    TEMPL_COMPONENT_STATE *state = rhs.m_StateList.GetHead(&handle);
    while (state) {
        m_StateList.InsertTail(*state);
        state = rhs.m_StateList.GetNext(&handle);
    }

    return *this;
}

void CComponent::Serialize(CArchive &ar)
{
    TRACE0("CComponent::Serialize()\n");

    int length = 0;
    unsigned long handle = 0;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)m_Name);
        ar << length;
        ar.Write((LPCTSTR)m_Name, length);  // NULL terminator not copied

        ar << m_CompID;
        ar << m_CompClass;
        ar << m_InstID;
        ar << m_DbID;
        ar << m_State;

        for (int i = 0; i < 6; i++) {
            ar << m_Data[i];
            ar << m_DataType[i];
            ar << m_DataSet[i];
        }

        // Store the state list.
        int count = m_StateList.GetItemCount();
        ar << count;
        CComponentState *state = m_StateList.GetHead(&handle);
        while (state) {
            state->Serialize(ar);
            state = m_StateList.GetNext(&handle);
        }

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("		length = %d\n", length);
        TRACE1("		Name = \"%s\"\n", (LPCTSTR)m_Name);
        TRACE1("		Component ID = %d\n", m_CompID);
        TRACE1("		Instance ID = %d\n", m_InstID);
        TRACE1("		Database ID = %d\n", m_DbID);
    } else {
        // Read the name.
        ar >> length;
        ar.Read(m_Name.GetBuffer(length), length);
        m_Name.ReleaseBuffer(length);

        ar >> m_CompID;
        ar >> m_CompClass;
        ar >> m_InstID;
        ar >> m_DbID;
        ar >> m_State;

        for (int i = 0; i < 6; i++) {
            ar >> m_Data[i];
            ar >> m_DataType[i];
            ar >> m_DataSet[i];
        }

        // Store the state list.
        int count = m_StateList.GetItemCount();
        ar >> count;
        for (int i = 0; i < count; i++) {
            CComponentState state;
            state.Serialize(ar);
            m_StateList.InsertTail(state);
        }

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        m_HasChanged = TRUE;

        TRACE1("		length = %d\n", length);
        TRACE1("		Name = \"%s\"\n", (LPCTSTR)m_Name);
        TRACE1("		Component ID = %d\n", m_CompID);
        TRACE1("		Instance ID = %d\n", m_InstID);
        TRACE1("		Database ID = %d\n", m_DbID);
    }
}

BOOL CComponent::SetState(const int stateval)
{
    // Check to make sure the state is valid.
    TEMPL_COMPONENT_STATE *state = FindState(stateval);

    if (state) {
        m_State = stateval;
        m_HasChanged = TRUE;
    }

    return (state != NULL);
}

CComponentState *CComponent::FindState(const int stateval)
{
    // Check to make sure the state is
    unsigned long handle = 0;
    TEMPL_COMPONENT_STATE *state = m_StateList.GetHead(&handle);

    while (state) {
        if (state->Value == stateval)
            return state;
        state = m_StateList.GetNext(&handle);
    }

    return NULL;
}

void *CComponent::GetData(const int wordnum, int *datatype)
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

BOOL CComponent::SetData(const int wordnum, const int datatype, void const *value)
{
    // wordnum is a 1-based index, so convert to 0-based.
    unsigned int idx = (unsigned int)wordnum - 1;

    // If we have an invalid word number, do nothing.
    if (idx > 5)
        return FALSE;

    // If we have an invalid datatype or value is NULL, mark as not set.
    if (((unsigned int)datatype > DataTypeDouble) || (value == NULL)) {
        m_DataSet[idx] = 0;
        m_Data[idx] = 0;

        m_HasChanged = TRUE;

        return TRUE;
    }

    // See if we are using a 64-bit datatype.
    BOOL Is64 = ((datatype == DataTypeInt64) ||
                 (datatype == DataTypeUInt64) ||
                 (datatype == DataTypeDouble));

    // If we are using the last word but are trying to store
    // a 64-bit value, do nothing.
    if ((idx == 5) && Is64) {
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
    case DataTypeUInt64: {
        unsigned long temp[2] = {0};
        *((unsigned __int64 *)temp) = *((unsigned __int64 *)value);

        m_DataSet[idx + 1] = 0;

        m_Data[idx]     = temp[1];
        m_Data[idx + 1] = temp[0];
        break;
    }

    case DataTypeDouble:
        m_DataSet[idx + 1] = 0;
        *((double *)&m_Data[idx]) = *((double *)value);
        break;

    default:
        return FALSE;
    }

    m_DataType[idx] = datatype;
    m_DataSet[idx] = 1;

    m_HasChanged = TRUE;

    return TRUE;
}

BOOL CComponent::CreateCigiPacket(void *buffer)
{
    BOOL needs_long = FALSE;
    CIGI_COMPONENT_CONTROL *l = (CIGI_COMPONENT_CONTROL *)buffer;
    unsigned long data[6] = {0};

    // Run through the use data items to see if we need a long or short packet
    // and to prepare the user data fields.
    for (int i = 0; i < 6; i++) {
        if (m_DataSet[i]) {
            if (i > 1)
                needs_long = TRUE;

            if (m_DataType[i] < DataTypeInt64) { // 32-bit
                data[i] = m_Data[i];
            } else if (i < 5) {                         // 64-bit
                if (i > 0)
                    needs_long = TRUE;

                // Pack the data according to the rules in the CIGI 3 ICD.
                LARGE_INTEGER i64;
                i64.QuadPart = *((__int64 *)&m_Data[i]);
                data[i] = (unsigned long)(i64.QuadPart & 0xFFFFFFFF);   // truncate
                data[i + 1] = (unsigned long)(i64.QuadPart >> 32);      // shift

                i++;    // skip the next slot since we just filled it
            }
        }
    }

    l->packet_id = CIGI_COMPONENT_CONTROL_OPCODE;
    l->packet_size = sizeof(CIGI_COMPONENT_CONTROL);
    l->component_id = m_CompID;
    l->instance_id = m_InstID;
    l->component_state = m_State;
    l->data1 = data[0];
    l->data2 = data[1];
    l->data3 = data[2];
    l->data4 = data[3];
    l->data5 = data[4];
    l->data6 = data[5];
    l->component_class = m_CompClass;

    return FALSE;
}

template <> void AFXAPI SerializeElements<CComponent *>(CArchive &ar, CComponent **pElements, int nCount)
{
    if (ar.IsStoring()) {
        for (int i = 0; i < nCount; i++) {
            if (pElements[i])
                pElements[i]->Serialize(ar);
        }
    } else {
        for (int i = 0; i < nCount; i++) {
            TEMPL_COMPONENT dummy;
            pElements[i] = new CComponent(-1, &dummy);

            pElements[i]->Serialize(ar);
        }
    }
}
