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

// Event.h: interface for the CCigiEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENT_H__0365D89B_6916_449F_896B_83619B1E4DBF__INCLUDED_)
#define AFX_EVENT_H__0365D89B_6916_449F_896B_83619B1E4DBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Component.h"

class CCigiEvent : public CObject
{
public:
    DECLARE_SERIAL(CCigiEvent)

    static const unsigned char DataTypeInt32;   // = 0
    static const unsigned char DataTypeUInt32;  // = 1
    static const unsigned char DataTypeFloat;   // = 2
    static const unsigned char DataTypeInt64;   // = 3
    static const unsigned char DataTypeUInt64;  // = 4
    static const unsigned char DataTypeDouble;  // = 5

    CCigiEvent();
    virtual ~CCigiEvent();
    CCigiEvent &operator=(const CCigiEvent &rhs);
    CString GetName(void);
    void SetName(LPCTSTR name);
    void SetName(CString &name);
    int GetID(void);
    void SetID(const int id);
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
    void ClearComponents(void);

    virtual void Serialize(CArchive &ar);

protected:
    CString m_Name;
    int m_ID;
    unsigned long m_Data[3];
    unsigned char m_DataType[3];
    unsigned char m_DataSet[3];
    CComponentMap m_Components;
};

inline CString CCigiEvent::GetName(void)
{
    return m_Name;
}

inline void CCigiEvent::SetName(LPCTSTR name)
{
    m_Name = name;
}

inline void CCigiEvent::SetName(CString &name)
{
    m_Name = name;
}

inline int CCigiEvent::GetID(void)
{
    return m_ID;
}

inline void CCigiEvent::SetID(const int id)
{
    m_ID = id;
}

inline BOOL CCigiEvent::SetData(const int wordnum, const int value)
{
    return SetData(wordnum, DataTypeInt32, (void *)&value);
}

inline BOOL CCigiEvent::SetData(const int wordnum, const unsigned int value)
{
    return SetData(wordnum, DataTypeUInt32, (void *)&value);
}

inline BOOL CCigiEvent::SetData(const int wordnum, const long value)
{
    return SetData(wordnum, DataTypeInt32, (void *)&value);
}

inline BOOL CCigiEvent::SetData(const int wordnum, const unsigned long value)
{
    return SetData(wordnum, DataTypeUInt32, (void *)&value);
}

inline BOOL CCigiEvent::SetData(const int wordnum, const __int64 value)
{
    return SetData(wordnum, DataTypeInt64, (void *)&value);
}

inline BOOL CCigiEvent::SetData(const int wordnum, const unsigned __int64 value)
{
    return SetData(wordnum, DataTypeUInt64, (void *)&value);
}

inline BOOL CCigiEvent::SetData(const int wordnum, const float value)
{
    return SetData(wordnum, DataTypeFloat, (void *)&value);
}

inline BOOL CCigiEvent::SetData(const int wordnum, const double value)
{
    return SetData(wordnum, DataTypeDouble, (void *)&value);
}

typedef CMap<int, int, CCigiEvent *, CCigiEvent *&> CCigiEventMap;
template <> void AFXAPI SerializeElements<CCigiEvent *>(CArchive &ar, CCigiEvent **pElements, int nCount);

#endif // !defined(AFX_EVENT_H__0365D89B_6916_449F_896B_83619B1E4DBF__INCLUDED_)
