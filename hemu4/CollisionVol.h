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

// CollisionVol.h: interface for the CCollisionVol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLLISIONVOL_H__8BF171D3_29DD_488A_B29D_708A43747AA0__INCLUDED_)
#define AFX_COLLISIONVOL_H__8BF171D3_29DD_488A_B29D_708A43747AA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"
#include "ConfigTempl.h"
#include "MissionFuncObj.h"

class CCollisionVol : public CMissionFuncObj<COLLISION_VOL, MF_DUMMY_RESP>
{
public:
    CCollisionVol(const int id_base, const int owner_id);
    CCollisionVol(TEMPL_COLLISION_VOL *templ);
    virtual ~CCollisionVol();
    int AddNotification(COLLISION_VOL_NOTIFICATION &resp);
    COLLISION_VOL_NOTIFICATION *GetFirstNotification(void);
    COLLISION_VOL_NOTIFICATION *GetNextNotification(COLLISION_VOL_NOTIFICATION *current);
    void ClearNotifications(void);
    void SetEntityID(const int id);
    void SetEnable(const BOOL enable);
    BOOL GetEnable(void);
    void SetType(const int type);
    int GetType(void);
    void SetX(const float x);
    float GetX(void);
    void SetY(const float y);
    float GetY(void);
    void SetZ(const float z);
    float GetZ(void);
    void SetRoll(const float roll);
    float GetRoll(void);
    void SetPitch(const float pitch);
    float GetPitch(void);
    void SetYaw(const float yaw);
    float GetYaw(void);
    void SetHeightOrRadius(const float hr);
    float GetHeightOrRadius(void);
    void SetWidth(const float w);
    float GetWidth(void);
    void SetDepth(const float d);
    float GetDepth(void);

private:
    CLinkedList<COLLISION_VOL_NOTIFICATION> m_NotificationList;

    CCollisionVol();
};

inline COLLISION_VOL_NOTIFICATION *CCollisionVol::GetFirstNotification(void)
{
    unsigned long hdl = 0;
    return m_NotificationList.GetHead(&hdl);
}

inline COLLISION_VOL_NOTIFICATION *CCollisionVol::GetNextNotification(COLLISION_VOL_NOTIFICATION *current)
{
    return m_NotificationList.GetNext((unsigned long *)current);
}

inline void CCollisionVol::SetEntityID(const int id)
{
    m_ReqData.identity.owner_id = id;
    m_RespData.identity.owner_id = id;
}

inline void CCollisionVol::SetEnable(const BOOL enable)
{
    m_ReqData.enable = enable ? 1 : 0;
}

inline BOOL CCollisionVol::GetEnable(void)
{
    return m_ReqData.enable ? TRUE : FALSE;
}

inline void CCollisionVol::SetType(const int type)
{
    m_ReqData.type = type;
}

inline int CCollisionVol::GetType(void)
{
    return m_ReqData.type;
}

inline void CCollisionVol::SetX(const float x)
{
    m_ReqData.x = x;
}

inline float CCollisionVol::GetX(void)
{
    return m_ReqData.x;
}

inline void CCollisionVol::SetY(const float y)
{
    m_ReqData.y = y;
}

inline float CCollisionVol::GetY(void)
{
    return m_ReqData.y;
}

inline void CCollisionVol::SetZ(const float z)
{
    m_ReqData.z = z;
}

inline float CCollisionVol::GetZ(void)
{
    return m_ReqData.z;
}

inline void CCollisionVol::SetRoll(const float roll)
{
    m_ReqData.roll = roll;
}

inline float CCollisionVol::GetRoll(void)
{
    return m_ReqData.roll;
}

inline void CCollisionVol::SetPitch(const float pitch)
{
    m_ReqData.pitch = pitch;
}

inline float CCollisionVol::GetPitch(void)
{
    return m_ReqData.pitch;
}

inline void CCollisionVol::SetYaw(const float yaw)
{
    m_ReqData.yaw = yaw;
}

inline float CCollisionVol::GetYaw(void)
{
    return m_ReqData.yaw;
}

inline void CCollisionVol::SetHeightOrRadius(const float hr)
{
    m_ReqData.height_radius = hr;
}

inline float CCollisionVol::GetHeightOrRadius(void)
{
    return m_ReqData.height_radius;
}

inline void CCollisionVol::SetWidth(const float w)
{
    m_ReqData.width = w;
}

inline float CCollisionVol::GetWidth(void)
{
    return m_ReqData.width;
}

inline void CCollisionVol::SetDepth(const float d)
{
    m_ReqData.depth = d;
}

inline float CCollisionVol::GetDepth(void)
{
    return m_ReqData.depth;
}

typedef CMap<int, int, CCollisionVol *, CCollisionVol *&>   CCollisionVolMap;
template <> void AFXAPI SerializeElements<CCollisionVol *>(CArchive &ar, CCollisionVol **pElements, int nCount);

#endif // !defined(AFX_COLLISIONVOL_H__8BF171D3_29DD_488A_B29D_708A43747AA0__INCLUDED_)
