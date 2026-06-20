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

// MissionFuncObj.h: interface for the CMissionFuncObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MISSIONFUNCOBJ_H__F406D741_9AA1_4604_8790_5CF673BCB015__INCLUDED_)
#define AFX_MISSIONFUNCOBJ_H__F406D741_9AA1_4604_8790_5CF673BCB015__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<class REQ_TYPE, class RESP_TYPE> class CMissionFuncObj : public CObject
{
public:
    CMissionFuncObj(const int id_base, const int owner_id);
    virtual ~CMissionFuncObj();
    void Serialize(CArchive &ar);
    void SetReqData(const REQ_TYPE &req_data);
    REQ_TYPE GetReqData(void);
    void SetRespData(const RESP_TYPE &resp_data);
    RESP_TYPE GetRespData(void);
    int GetID(void);
    int GetOwnerID(void);
    CString GetName(void);

protected:
    CMissionFuncObj();

    CString         m_Name;
    REQ_TYPE        m_ReqData;
    RESP_TYPE       m_RespData;
};

template <class REQ_TYPE, class RESP_TYPE> CMissionFuncObj<REQ_TYPE, RESP_TYPE>
::CMissionFuncObj()
{
}

template <class REQ_TYPE, class RESP_TYPE>
CMissionFuncObj<REQ_TYPE, RESP_TYPE>::CMissionFuncObj(const int id_base, const int owner_id)
{
    memset(&m_ReqData, 0, sizeof(REQ_TYPE));
    memset(&m_RespData, 0, sizeof(RESP_TYPE));
    m_ReqData.identity.id = id_base;
    m_ReqData.identity.owner_id = owner_id;
    m_RespData.identity.id = id_base;
    m_RespData.identity.owner_id = owner_id;
}

template <class REQ_TYPE, class RESP_TYPE>
CMissionFuncObj<REQ_TYPE, RESP_TYPE>::~CMissionFuncObj()
{
}

template <class REQ_TYPE, class RESP_TYPE>
void CMissionFuncObj<REQ_TYPE, RESP_TYPE>::Serialize(CArchive &ar)
{
    int length = 0;

    ASSERT_VALID(this);

    CObject::Serialize(ar);
    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)m_Name);
        ar << length;
        ar.Write((LPCTSTR)m_Name, length);  // NULL terminator not copied

        // Store the request data.
        ar.Write(&m_ReqData, sizeof(m_ReqData));
    } else {
        // Read the name.
        ar >> length;
        ar.Read(m_Name.GetBuffer(length), length);
        m_Name.ReleaseBuffer(length);

        // Read the request data.
        ar.Read(&m_ReqData, sizeof(m_ReqData));
    }
}

template <class REQ_TYPE, class RESP_TYPE>
void CMissionFuncObj<REQ_TYPE, RESP_TYPE>::SetReqData(const REQ_TYPE &req_data)
{
    m_ReqData = req_data;
}

template <class REQ_TYPE, class RESP_TYPE>
REQ_TYPE CMissionFuncObj<REQ_TYPE, RESP_TYPE>::GetReqData(void)
{
    return m_ReqData;
}

template <class REQ_TYPE, class RESP_TYPE>
void CMissionFuncObj<REQ_TYPE, RESP_TYPE>::SetRespData(const RESP_TYPE &resp_data)
{
    m_RespData = resp_data;
}

template <class REQ_TYPE, class RESP_TYPE>
RESP_TYPE CMissionFuncObj<REQ_TYPE, RESP_TYPE>::GetRespData(void)
{
    return m_RespData;
}

template <class REQ_TYPE, class RESP_TYPE>
int CMissionFuncObj<REQ_TYPE, RESP_TYPE>::GetID(void)
{
    return (int)(m_ReqData.identity.id);
}

template <class REQ_TYPE, class RESP_TYPE>
int CMissionFuncObj<REQ_TYPE, RESP_TYPE>::GetOwnerID(void)
{
    return (int)(m_ReqData.identity.owner_id);
}

template <class REQ_TYPE, class RESP_TYPE>
CString CMissionFuncObj<REQ_TYPE, RESP_TYPE>::GetName(void)
{
    return m_Name;
}

#endif // !defined(AFX_MISSIONFUNCOBJ_H__F406D741_9AA1_4604_8790_5CF673BCB015__INCLUDED_)
