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

// ArtPart.h: interface for the CArtPart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARTPART_H__DC3025D0_FD5F_4775_99D5_10A4F5300311__INCLUDED_)
#define AFX_ARTPART_H__DC3025D0_FD5F_4775_99D5_10A4F5300311__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "ConfigTempl.h"

class CArtPart : public CObject
{
    DECLARE_SERIAL(CArtPart)

public:
    CArtPart(TEMPL_ARTPART *artpart);
    virtual ~CArtPart();
    CArtPart &operator=(const CArtPart &rhs);
    BOOL HasChanged(void);
    void ResetChangedFlag(void);
    CString GetName(void);
    void SetName(CString name);
    int GetID(void);
    void SetID(const int id);
    int GetEntityID(void);
    void SetEntityID(const int id);
    BOOL GetEnableFlag(void);
    void SetEnableFlag(const BOOL enable);
    void SetXOffset(const float x, const BOOL enable = TRUE);
    float GetXOffset(void);
    void SetYOffset(const float y, const BOOL enable = TRUE);
    float GetYOffset(void);
    void SetZOffset(const float z, const BOOL enable = TRUE);
    float GetZOffset(void);
    void SetRoll(const float roll, const BOOL enable = TRUE);
    float GetRoll(void);
    void SetPitch(const float pitch, const BOOL enable = TRUE);
    float GetPitch(void);
    void SetYaw(const float yaw, const BOOL enable = TRUE);
    float GetYaw(void);
    void SetXEnable(const BOOL enable);
    BOOL GetXEnable(void);
    void SetYEnable(const BOOL enable);
    BOOL GetYEnable(void);
    void SetZEnable(const BOOL enable);
    BOOL GetZEnable(void);
    void SetRollEnable(const BOOL enable);
    BOOL GetRollEnable(void);
    void SetPitchEnable(const BOOL enable);
    BOOL GetPitchEnable(void);
    void SetYawEnable(const BOOL enable);
    BOOL GetYawEnable(void);
    void SetXRate(const float dx);
    float GetXRate(void);
    void SetYRate(const float dy);
    float GetYRate(void);
    void SetZRate(const float dz);
    float GetZRate(void);
    void SetRollRate(const float droll);
    float GetRollRate(void);
    void SetPitchRate(const float dpitch);
    float GetPitchRate(void);
    void SetYawRate(const float dyaw);
    float GetYawRate(void);
    BOOL CreateCigiPacket(void *buffer);

    virtual void Serialize(CArchive &ar);

protected:
    CArtPart();     // For serialization only

    CString m_Name;
    int m_ID;
    int m_EntityID;
    BOOL m_HasChanged;
    BOOL m_Enable;
    BOOL m_XValid;
    BOOL m_YValid;
    BOOL m_ZValid;
    BOOL m_RollValid;
    BOOL m_PitchValid;
    BOOL m_YawValid;
    float m_XOffset;
    float m_YOffset;
    float m_ZOffset;
    float m_Roll;
    float m_Pitch;
    float m_Yaw;
    float m_XRate;
    float m_YRate;
    float m_ZRate;
    float m_RollRate;
    float m_PitchRate;
    float m_YawRate;
};

inline BOOL CArtPart::HasChanged(void)
{
    return m_HasChanged;
}

inline void CArtPart::ResetChangedFlag(void)
{
    m_HasChanged = FALSE;
}

inline CString CArtPart::GetName(void)
{
    return m_Name;
}

inline void CArtPart::SetName(CString name)
{
    m_Name = name;
    m_HasChanged = TRUE;
}

inline int CArtPart::GetID(void)
{
    return m_ID;
}

inline void CArtPart::SetID(const int id)
{
    m_ID = id;
    m_HasChanged = TRUE;
}

inline int CArtPart::GetEntityID(void)
{
    return m_EntityID;
}

inline void CArtPart::SetEntityID(const int id)
{
    m_EntityID = id;
    m_HasChanged = TRUE;
}

inline BOOL CArtPart::GetEnableFlag(void)
{
    return m_Enable;
}

inline void CArtPart::SetEnableFlag(const BOOL enable)
{
    m_Enable = enable;
    m_HasChanged = TRUE;
}

inline float CArtPart::GetXOffset(void)
{
    return m_XOffset;
}

inline float CArtPart::GetYOffset(void)
{
    return m_YOffset;
}

inline float CArtPart::GetZOffset(void)
{
    return m_ZOffset;
}

inline float CArtPart::GetRoll(void)
{
    return m_Roll;
}

inline float CArtPart::GetPitch(void)
{
    return m_Pitch;
}

inline float CArtPart::GetYaw(void)
{
    return m_Yaw;
}

inline void CArtPart::SetXEnable(const BOOL enable)
{
    m_XValid = enable;
    m_HasChanged = TRUE;
}

inline BOOL CArtPart::GetXEnable(void)
{
    return m_XValid;
}

inline void CArtPart::SetYEnable(const BOOL enable)
{
    m_YValid = enable;
    m_HasChanged = TRUE;
}

inline BOOL CArtPart::GetYEnable(void)
{
    return m_YValid;
}

inline void CArtPart::SetZEnable(const BOOL enable)
{
    m_ZValid = enable;
    m_HasChanged = TRUE;
}

inline BOOL CArtPart::GetZEnable(void)
{
    return m_ZValid;
}

inline void CArtPart::SetRollEnable(const BOOL enable)
{
    m_RollValid = enable;
    m_HasChanged = TRUE;
}

inline BOOL CArtPart::GetRollEnable(void)
{
    return m_RollValid;
}

inline void CArtPart::SetPitchEnable(const BOOL enable)
{
    m_PitchValid = enable;
    m_HasChanged = TRUE;
}

inline BOOL CArtPart::GetPitchEnable(void)
{
    return m_PitchValid;
}

inline void CArtPart::SetYawEnable(const BOOL enable)
{
    m_YawValid = enable;
    m_HasChanged = TRUE;
}

inline BOOL CArtPart::GetYawEnable(void)
{
    return m_YawValid;
}

typedef CMap<int, int, CArtPart *, CArtPart *&> CArtPartMap;
template <> void AFXAPI SerializeElements<CArtPart *>(CArchive &ar, CArtPart **pElements, int nCount);

#endif // !defined(AFX_ARTPART_H__DC3025D0_FD5F_4775_99D5_10A4F5300311__INCLUDED_)
