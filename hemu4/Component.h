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

// Component.h: interface for the CComponent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPONENT_H__76569F6F_0BBD_46D8_B2EE_5D50FB0A8E0F__INCLUDED_)
#define AFX_COMPONENT_H__76569F6F_0BBD_46D8_B2EE_5D50FB0A8E0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConfigTemplComponent.h"

#include <cstdint>

typedef TEMPL_COMPONENT_STATE CComponentState;

class CComponent : public CObject
{
public:
    DECLARE_SERIAL(CComponent)

    static const unsigned char DataTypeInt32;   // = 0
    static const unsigned char DataTypeUInt32;  // = 1
    static const unsigned char DataTypeFloat;   // = 2
    static const unsigned char DataTypeInt64;   // = 3
    static const unsigned char DataTypeUInt64;  // = 4
    static const unsigned char DataTypeDouble;  // = 5

    CComponent();       // Used only for serialization
    CComponent(const int comp_class, TEMPL_COMPONENT *comp);
    virtual ~CComponent();
    CComponent &operator=(const CComponent &rhs);
    BOOL HasChanged(void);
    void ResetChangedFlag(void);
    CString GetName(void);
    void SetName(LPCTSTR name);
    void SetName(CString &name);
    int GetID(void);
    void SetID(const int id);
    int GetInstanceID(void);
    void SetInstanceID(const int id);
    int GetClass(void);
    void SetClass(const int comp_class);
    int GetDatabaseID(void);
    void SetDatabaseID(const int id);
    int GetState(void);
    BOOL SetState(const int stateval);
    void *GetData(const int wordnum, int *datatype);    // 1-based index
    BOOL SetData(const int wordnum, const int datatype, void const *value);
    BOOL SetData(const int wordnum, const int value);
    BOOL SetData(const int wordnum, const unsigned int value);
    BOOL SetData(const int wordnum, const long value);
    BOOL SetData(const int wordnum, const unsigned long value);
    BOOL SetData(const int wordnum, const __int64 value);
    BOOL SetData(const int wordnum, const unsigned __int64 value);
    BOOL SetData(const int wordnum, const float value);
    BOOL SetData(const int wordnum, const double value);
    CComponentState *FindState(const int stateval);
    CComponentState *GetFirstState(unsigned long *handle);
    CComponentState *GetLastState(unsigned long *handle);
    CComponentState *GetNextState(unsigned long *handle);
    CComponentState *GetPrevState(unsigned long *handle);
    BOOL CreateCigiPacket(void *buffer);

    virtual void Serialize(CArchive &ar);

protected:
    CString m_Name;
    uint16_t m_CompID;
    uint16_t m_InstID;
    int m_CompClass;
    int m_DbID;         // Used only for terrain components.
    int m_State;
    BOOL m_HasChanged;
    unsigned long m_Data[6];
    unsigned char m_DataType[6];
    unsigned char m_DataSet[6];
    CLinkedList<CComponentState> m_StateList;
};

inline BOOL CComponent::HasChanged(void)
{
    return m_HasChanged;
}

inline void CComponent::ResetChangedFlag(void)
{
    m_HasChanged = FALSE;
}

inline CString CComponent::GetName(void)
{
    return m_Name;
}

inline void CComponent::SetName(LPCTSTR name)
{
    m_Name = name;
    m_HasChanged = TRUE;
}

inline void CComponent::SetName(CString &name)
{
    m_Name = name;
    m_HasChanged = TRUE;
}

inline int CComponent::GetID(void)
{
    return m_CompID;
}

inline void CComponent::SetID(const int id)
{
    m_CompID = id;
    m_HasChanged = TRUE;
}

inline int CComponent::GetInstanceID(void)
{
    return m_InstID;
}

inline void CComponent::SetInstanceID(const int id)
{
    m_InstID = id;
    m_HasChanged = TRUE;
}

inline int CComponent::GetClass(void)
{
    return m_CompClass;
}

inline void CComponent::SetClass(const int comp_class)
{
    m_CompClass = comp_class;
    m_HasChanged = TRUE;
}

inline int CComponent::GetDatabaseID(void)
{
    return m_DbID;
}

inline void CComponent::SetDatabaseID(const int id)
{
    m_DbID = id;
    m_HasChanged = TRUE;
}

inline int CComponent::GetState(void)
{
    return m_State;
}

inline BOOL CComponent::SetData(const int wordnum, const int value)
{
    return SetData(wordnum, DataTypeInt32, (void *)&value);
}

inline BOOL CComponent::SetData(const int wordnum, const unsigned int value)
{
    return SetData(wordnum, DataTypeUInt32, (void *)&value);
}

inline BOOL CComponent::SetData(const int wordnum, const long value)
{
    return SetData(wordnum, DataTypeInt32, (void *)&value);
}

inline BOOL CComponent::SetData(const int wordnum, const unsigned long value)
{
    return SetData(wordnum, DataTypeUInt32, (void *)&value);
}

inline BOOL CComponent::SetData(const int wordnum, const __int64 value)
{
    return SetData(wordnum, DataTypeInt64, (void *)&value);
}

inline BOOL CComponent::SetData(const int wordnum, const unsigned __int64 value)
{
    return SetData(wordnum, DataTypeUInt64, (void *)&value);
}

inline BOOL CComponent::SetData(const int wordnum, const float value)
{
    return SetData(wordnum, DataTypeFloat, (void *)&value);
}

inline BOOL CComponent::SetData(const int wordnum, const double value)
{
    return SetData(wordnum, DataTypeDouble, (void *)&value);
}

inline CComponentState *CComponent::GetFirstState(unsigned long *handle)
{
    return m_StateList.GetHead(handle);
}

inline CComponentState *CComponent::GetLastState(unsigned long *handle)
{
    return m_StateList.GetTail(handle);
}

inline CComponentState *CComponent::GetNextState(unsigned long *handle)
{
    return m_StateList.GetNext(handle);
}

inline CComponentState *CComponent::GetPrevState(unsigned long *handle)
{
    return m_StateList.GetPrev(handle);
}

typedef CMap<int, int, CComponent *, CComponent *&> CComponentMap;
template <> void AFXAPI SerializeElements<CComponent *>(CArchive &ar, CComponent **pElements, int nCount);

#endif // !defined(AFX_COMPONENT_H__76569F6F_0BBD_46D8_B2EE_5D50FB0A8E0F__INCLUDED_)
