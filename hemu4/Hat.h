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

// Hat.h: interface for the CHat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HAT_H__3C0B268B_0BB7_415F_A125_E015B2315064__INCLUDED_)
#define AFX_HAT_H__3C0B268B_0BB7_415F_A125_E015B2315064__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"
#include "MissionFuncObj.h"

class CHat : public CMissionFuncObj<HAT_REQUEST, HAT_RESPONSE>
{
public:
    CHat(const int id_base, const int owner_id);
    virtual ~CHat();
    unsigned int GetRequestType(void);
    void SetRequestType(const unsigned int type);
    BOOL GetContinuous(void);
    void SetContinuous(const BOOL cont = TRUE);
    double GetLat(void);
    void SetLat(const double alt);
    double GetLon(void);
    void SetLon(const double lon);
    double GetAlt(void);
    void SetAlt(const double alt);
    double GetXOffset(void);
    void SetXOffset(const double x);
    double GetYOffset(void);
    void SetYOffset(const double y);
    double GetZOffset(void);
    void SetZOffset(const double z);
    BOOL RespIsHat(void);
    BOOL RespIsHot(void);
    BOOL RespIsValid(void);
    double GetRespHat(void);
    double GetRespHot(void);
    float GetRespAzimuth(void);
    float GetRespElevation(void);
    unsigned long GetRespMaterialCode(void);

protected:
    CHat();
};

inline void CHat::SetRequestType(const unsigned int type)
{
    m_ReqData.req_type = type;
}

inline unsigned int CHat::GetRequestType(void)
{
    return m_ReqData.req_type;
}

inline BOOL CHat::GetContinuous(void)
{
    return m_ReqData.continuous ? TRUE : FALSE;
}

inline void CHat::SetContinuous(const BOOL cont)
{
    m_ReqData.continuous = cont ? 1 : 0;
}

inline double CHat::GetLat(void)
{
    return m_ReqData.lat;
}

inline void CHat::SetLat(const double lat)
{
    m_ReqData.lat = lat;
}

inline double CHat::GetLon(void)
{
    return m_ReqData.lon;
}

inline void CHat::SetLon(const double lon)
{
    m_ReqData.lon = lon;
}

inline double CHat::GetAlt(void)
{
    return m_ReqData.alt;
}

inline void CHat::SetAlt(const double alt)
{
    m_ReqData.alt = alt;
}

inline double CHat::GetXOffset(void)
{
    return m_ReqData.x_offset;
}

inline void CHat::SetXOffset(const double x)
{
    m_ReqData.x_offset = x;
}

inline double CHat::GetYOffset(void)
{
    return m_ReqData.y_offset;
}

inline void CHat::SetYOffset(const double y)
{
    m_ReqData.y_offset = y;
}

inline double CHat::GetZOffset(void)
{
    return m_ReqData.z_offset;
}

inline void CHat::SetZOffset(const double z)
{
    m_ReqData.z_offset = z;
}

inline BOOL CHat::RespIsHat(void)
{
    return ((m_RespData.resp_type == 0) || (m_RespData.resp_type == 2)) ? TRUE : FALSE;
}

inline BOOL CHat::RespIsHot(void)
{
    return ((m_RespData.resp_type == 1) || (m_RespData.resp_type == 2)) ? TRUE : FALSE;
}

inline BOOL CHat::RespIsValid(void)
{
    return m_RespData.valid ? TRUE : FALSE;
}

inline double CHat::GetRespHat(void)
{
    return m_RespData.hat;
}

inline double CHat::GetRespHot(void)
{
    return m_RespData.hot;
}

inline float CHat::GetRespAzimuth(void)
{
    return m_RespData.azimuth;
}

inline float CHat::GetRespElevation(void)
{
    return m_RespData.elevation;
}

inline unsigned long CHat::GetRespMaterialCode(void)
{
    return m_RespData.material_code;
}

typedef CMap<int, int, CHat *, CHat *&> CHatMap;
template <> void AFXAPI SerializeElements<CHat *>(CArchive &ar, CHat **pElements, int nCount);

#endif // !defined(AFX_HAT_H__3C0B268B_0BB7_415F_A125_E015B2315064__INCLUDED_)
