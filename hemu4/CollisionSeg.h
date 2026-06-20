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

// CollisionSeg.h: interface for the CCollisionSeg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLLISIONSEG_H__7267EBB6_BE2D_4D08_8065_5E0B32F5E5FE__INCLUDED_)
#define AFX_COLLISIONSEG_H__7267EBB6_BE2D_4D08_8065_5E0B32F5E5FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"
#include "ConfigTempl.h"
#include "MissionFuncObj.h"

class CCollisionSeg : public CMissionFuncObj<COLLISION_SEG, MF_DUMMY_RESP>
{
public:
    CCollisionSeg(const int id_base, const int owner_id);
    CCollisionSeg(TEMPL_COLLISION_SEG *templ);
    virtual ~CCollisionSeg();
    COLLISION_SEG_NOTIFICATION *GetFirstNotification(void);
    COLLISION_SEG_NOTIFICATION *GetNextNotification(COLLISION_SEG_NOTIFICATION *current);
    void ClearNotifications(void);
    void SetEntityID(const int id);
    void SetEnable(const BOOL enable);
    BOOL GetEnable(void);
    void SetMaterialMask(const unsigned long mask);
    unsigned long GetMaterialMask(void);
    void SetStartX(const float x);
    float GetStartX(void);
    void SetStartY(const float y);
    float GetStartY(void);
    void SetStartZ(const float z);
    float GetStartZ(void);
    void SetEndX(const float x);
    float GetEndX(void);
    void SetEndY(const float y);
    float GetEndY(void);
    void SetEndZ(const float z);
    float GetEndZ(void);

protected:
    CLinkedList<COLLISION_SEG_NOTIFICATION> m_NotificationList;

    CCollisionSeg();
};

inline COLLISION_SEG_NOTIFICATION *CCollisionSeg::GetFirstNotification(void)
{
    unsigned long hdl = 0;
    return m_NotificationList.GetHead(&hdl);
}

inline COLLISION_SEG_NOTIFICATION *CCollisionSeg::GetNextNotification(COLLISION_SEG_NOTIFICATION *current)
{
    return m_NotificationList.GetNext((unsigned long *)current);
}

inline void CCollisionSeg::SetEntityID(const int id)
{
    m_ReqData.identity.owner_id = id;
    m_RespData.identity.owner_id = id;
}

inline void CCollisionSeg::SetEnable(const BOOL enable)
{
    m_ReqData.enable = enable ? 1 : 0;
}

inline BOOL CCollisionSeg::GetEnable(void)
{
    return m_ReqData.enable ? TRUE : FALSE;
}

inline void CCollisionSeg::SetMaterialMask(const unsigned long mask)
{
    m_ReqData.material_mask = mask;
}

inline unsigned long CCollisionSeg::GetMaterialMask(void)
{
    return m_ReqData.material_mask;
}

inline void CCollisionSeg::SetStartX(const float x)
{
    m_ReqData.x1 = x;
}

inline float CCollisionSeg::GetStartX(void)
{
    return m_ReqData.x1;
}

inline void CCollisionSeg::SetStartY(const float y)
{
    m_ReqData.y1 = y;
}

inline float CCollisionSeg::GetStartY(void)
{
    return m_ReqData.y1;
}

inline void CCollisionSeg::SetStartZ(const float z)
{
    m_ReqData.z1 = z;
}

inline float CCollisionSeg::GetStartZ(void)
{
    return m_ReqData.z1;
}

inline void CCollisionSeg::SetEndX(const float x)
{
    m_ReqData.x2 = x;
}

inline float CCollisionSeg::GetEndX(void)
{
    return m_ReqData.x2;
}

inline void CCollisionSeg::SetEndY(const float y)
{
    m_ReqData.y2 = y;
}

inline float CCollisionSeg::GetEndY(void)
{
    return m_ReqData.y2;
}

inline void CCollisionSeg::SetEndZ(const float z)
{
    m_ReqData.z2 = z;
}

inline float CCollisionSeg::GetEndZ(void)
{
    return m_ReqData.z2;
}

typedef CMap<int, int, CCollisionSeg *, CCollisionSeg *&>   CCollisionSegMap;
template <> void AFXAPI SerializeElements<CCollisionSeg *>(CArchive &ar, CCollisionSeg **pElements, int nCount);

#endif // !defined(AFX_COLLISIONSEG_H__7267EBB6_BE2D_4D08_8065_5E0B32F5E5FE__INCLUDED_)
