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

// AttachPoint.h: interface for the CAttachPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATTACHPOINT_H__714603AF_8B8E_43DD_BF14_B1AD3D190DB1__INCLUDED_)
#define AFX_ATTACHPOINT_H__714603AF_8B8E_43DD_BF14_B1AD3D190DB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "ConfigTempl.h"

class CAttachPoint : public CObject
{
    DECLARE_SERIAL(CAttachPoint)

public:
    CAttachPoint();
    CAttachPoint(TEMPL_ATTACHPOINT *attach);
    virtual ~CAttachPoint();
    CAttachPoint &operator=(const CAttachPoint &rhs);
    CString GetName(void);
    void SetName(CString name);
    int GetID(void);
    void SetID(const int id);
    double GetXOffset(void);
    void SetXOffset(const double x);
    double GetYOffset(void);
    void SetYOffset(const double y);
    double GetZOffset(void);
    void SetZOffset(const double z);
    double GetRoll(void);
    void SetRoll(const double roll);
    double GetPitch(void);
    void SetPitch(const double pitch);
    double GetYawt(void);
    void SetYaw(const double yaw);

    virtual void Serialize(CArchive &ar);

protected:
    CString m_Name;
    int m_ID;
    double m_XOffset;
    double m_YOffset;
    double m_ZOffset;
    double m_Roll;
    double m_Pitch;
    double m_Yaw;
};

inline int CAttachPoint::GetID(void)
{
    return m_ID;
}

inline void CAttachPoint::SetID(const int id)
{
    m_ID = id;
}

typedef CMap<int, int, CAttachPoint *, CAttachPoint *&> CAttachPointMap;
template <> void AFXAPI SerializeElements<CAttachPoint *>(CArchive &ar, CAttachPoint **pElements, int nCount);

#endif // !defined(AFX_ATTACHPOINT_H__714603AF_8B8E_43DD_BF14_B1AD3D190DB1__INCLUDED_)
