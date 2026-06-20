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

// Los.h: interface for the CLos class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOS_H__3F6EBB59_21F9_4D09_917D_6D627FA67221__INCLUDED_)
#define AFX_LOS_H__3F6EBB59_21F9_4D09_917D_6D627FA67221__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"
#include "MissionFuncObj.h"

class CLos : public CMissionFuncObj<LOS_REQUEST, LOS_RESPONSE>
{
public:
    CLos(const int id_base, const int owner_id);
    virtual ~CLos();
    BOOL GetContinuous(void);
    void SetContinuous(const BOOL cont = TRUE);
    unsigned int GetRequestType(void);
    void SetRequestType(const int type);
    BOOL GetRequestExtended(void);
    void SetRequestExtended(const BOOL req_ext);
    unsigned int GetSourceCoordSystem(void);
    void SetSourceCoordSystem(const unsigned int coordsys);
    unsigned int GetDestCoordSystem(void);
    void SetDestCoordSystem(const unsigned int coordsys);
    unsigned int GetDesiredRespCoordSystem(void);
    void SetDesiredRespCoordSystem(const unsigned int coordsys);
    unsigned char GetAlphaThreshold(void);
    void SetAlphaThreshold(const unsigned char alpha);
    int GetTargetEntityID(void);
    void SetTargetEntityID(const int id);
    unsigned long GetMaterialMask(void);
    void SetMaterialMask(const unsigned long mask);
    float GetRequestAzimuth(void);
    void SetRequestAzimuth(const float az);
    float GetRequestElevation(void);
    void SetRequestElevation(const float el);
    float GetRequestMinRange(void);
    void SetRequestMinRange(const float min);
    float GetRequestMaxRange(void);
    void SetRequestMaxRange(const float max);
    double GetSourceLat(void);
    void SetSourceLat(const double lat);
    double GetSourceLon(void);
    void SetSourceLon(const double lon);
    double GetSourceAlt(void);
    void SetSourceAlt(const double alt);
    double GetSourceXOffset(void);
    void SetSourceXOffset(const double x);
    double GetSourceYOffset(void);
    void SetSourceYOffset(const double y);
    double GetSourceZOffset(void);
    void SetSourceZOffset(const double z);
    double GetDestLatX(void);
    void SetDestLatX(const double lat_x);
    double GetDestLonY(void);
    void SetDestLonY(const double lon_y);
    double GetDestAltZ(void);
    void SetDestAltZ(const double alt_z);
    BOOL RespIsExtended(void);
    BOOL RespIsValid(void);
    BOOL RespEntityIsValid(void);
    BOOL RespRangeIsValid(void);
    BOOL RespIsVisible(void);
    unsigned int GetRespCoordSystem(void);
    int GetRespCount(void);
    int GetRespEntityID(void);
    unsigned char GetRespRed(void);
    unsigned char GetRespGreen(void);
    unsigned char GetRespBlue(void);
    unsigned char GetRespAlpha(void);
    double GetRespRange(void);
    double GetRespLatX(void);
    double GetRespLonY(void);
    double GetRespAltZ(void);
    float GetRespAzimuth(void);
    float GetRespElevation(void);
    unsigned long GetRespMaterialCode(void);

protected:
    CLos();
};

inline BOOL CLos::GetContinuous(void)
{
    return m_ReqData.continuous ? TRUE : FALSE;
}

inline void CLos::SetContinuous(const BOOL cont)
{
    m_ReqData.continuous = cont ? 1 : 0;
}

inline unsigned int CLos::GetRequestType(void)
{
    return m_ReqData.req_type;
}

inline void CLos::SetRequestType(const int type)
{
    m_ReqData.req_type = type;
}

inline BOOL CLos::GetRequestExtended(void)
{
    return m_ReqData.ext_response ? TRUE : FALSE;
}

inline void CLos::SetRequestExtended(const BOOL req_ext)
{
    m_ReqData.ext_response = req_ext ? 1 : 0;
}

inline unsigned int CLos::GetSourceCoordSystem(void)
{
    return m_ReqData.src_coord_system;
}

inline void CLos::SetSourceCoordSystem(const unsigned int coordsys)
{
    m_ReqData.src_coord_system = coordsys;
}

inline unsigned int CLos::GetDestCoordSystem(void)
{
    return m_ReqData.dest_coord_system;
}

inline void CLos::SetDestCoordSystem(const unsigned int coordsys)
{
    m_ReqData.dest_coord_system = coordsys;
}

inline unsigned int CLos::GetDesiredRespCoordSystem(void)
{
    return m_ReqData.resp_coord_system;
}

inline void CLos::SetDesiredRespCoordSystem(const unsigned int coordsys)
{
    m_ReqData.resp_coord_system = coordsys;
}

inline unsigned char CLos::GetAlphaThreshold(void)
{
    return m_ReqData.alpha_thresh;
}

inline void CLos::SetAlphaThreshold(const unsigned char alpha)
{
    m_ReqData.alpha_thresh = alpha;
}

inline int CLos::GetTargetEntityID(void)
{
    return m_ReqData.target_entity_id;
}

inline void CLos::SetTargetEntityID(const int id)
{
    m_ReqData.target_entity_id = id;
}

inline unsigned long CLos::GetMaterialMask(void)
{
    return m_ReqData.material_mask;
}

inline void CLos::SetMaterialMask(const unsigned long mask)
{
    m_ReqData.material_mask = mask;
}

inline float CLos::GetRequestAzimuth(void)
{
    return m_ReqData.azimuth;
}

inline void CLos::SetRequestAzimuth(const float az)
{
    m_ReqData.azimuth = az;
}

inline float CLos::GetRequestElevation(void)
{
    return m_ReqData.elevation;
}

inline void CLos::SetRequestElevation(const float el)
{
    m_ReqData.elevation = el;
}

inline float CLos::GetRequestMinRange(void)
{
    return m_ReqData.min_range;
}

inline void CLos::SetRequestMinRange(const float min)
{
    m_ReqData.min_range = min;
}

inline float CLos::GetRequestMaxRange(void)
{
    return m_ReqData.max_range;
}

inline void CLos::SetRequestMaxRange(const float max)
{
    m_ReqData.max_range = max;
}

inline double CLos::GetSourceLat(void)
{
    return m_ReqData.src_lat;
}

inline void CLos::SetSourceLat(const double lat)
{
    m_ReqData.src_lat = lat;
}

inline double CLos::GetSourceLon(void)
{
    return m_ReqData.src_lon;
}

inline void CLos::SetSourceLon(const double lon)
{
    m_ReqData.src_lon = lon;
}

inline double CLos::GetSourceAlt(void)
{
    return m_ReqData.src_alt;
}

inline void CLos::SetSourceAlt(const double alt)
{
    m_ReqData.src_alt = alt;
}

inline double CLos::GetSourceXOffset(void)
{
    return m_ReqData.src_x_offset;
}

inline void CLos::SetSourceXOffset(const double x)
{
    m_ReqData.src_x_offset = x;
}

inline double CLos::GetSourceYOffset(void)
{
    return m_ReqData.src_y_offset;
}

inline void CLos::SetSourceYOffset(const double y)
{
    m_ReqData.src_y_offset = y;
}

inline double CLos::GetSourceZOffset(void)
{
    return m_ReqData.src_z_offset;
}

inline void CLos::SetSourceZOffset(const double z)
{
    m_ReqData.src_z_offset = z;
}

inline double CLos::GetDestLatX(void)
{
    return m_ReqData.dest_lat_x;
}

inline void CLos::SetDestLatX(const double lat_x)
{
    m_ReqData.dest_lat_x = lat_x;
}

inline double CLos::GetDestLonY(void)
{
    return m_ReqData.dest_lon_y;
}

inline void CLos::SetDestLonY(const double lon_y)
{
    m_ReqData.dest_lon_y = lon_y;
}

inline double CLos::GetDestAltZ(void)
{
    return m_ReqData.dest_alt_z;
}

inline void CLos::SetDestAltZ(const double alt_z)
{
    m_ReqData.dest_alt_z = alt_z;
}

inline BOOL CLos::RespIsExtended(void)
{
    return (m_RespData.resp_type == LOS_RESP_TYPE_EXTENDED) ? TRUE : FALSE;
}

inline BOOL CLos::RespIsValid(void)
{
    return m_RespData.valid ? TRUE : FALSE;
}

inline BOOL CLos::RespEntityIsValid(void)
{
    return m_RespData.entity_id_valid ? TRUE : FALSE;
}

inline BOOL CLos::RespRangeIsValid(void)
{
    return m_RespData.range_valid ? TRUE : FALSE;
}

inline BOOL CLos::RespIsVisible(void)
{
    return m_RespData.visible ? TRUE : FALSE;
}

inline int CLos::GetRespCount(void)
{
    return m_RespData.response_count;
}

inline unsigned int CLos::GetRespCoordSystem(void)
{
    return m_RespData.coord_system;
}

inline int CLos::GetRespEntityID(void)
{
    return m_RespData.entity_id;
}

inline unsigned char CLos::GetRespRed(void)
{
    return m_RespData.red;
}

inline unsigned char CLos::GetRespGreen(void)
{
    return m_RespData.green;
}

inline unsigned char CLos::GetRespBlue(void)
{
    return m_RespData.blue;
}

inline unsigned char CLos::GetRespAlpha(void)
{
    return m_RespData.alpha;
}

inline double CLos::GetRespRange(void)
{
    return m_RespData.range;
}

inline double CLos::GetRespLatX(void)
{
    return m_RespData.lat_x;
}

inline double CLos::GetRespLonY(void)
{
    return m_RespData.lon_y;
}

inline double CLos::GetRespAltZ(void)
{
    return m_RespData.alt_z;
}

inline float CLos::GetRespAzimuth(void)
{
    return m_RespData.azimuth;
}

inline float CLos::GetRespElevation(void)
{
    return m_RespData.elevation;
}

inline unsigned long CLos::GetRespMaterialCode(void)
{
    return m_RespData.material_code;
}

typedef CMap<int, int, CLos *, CLos *&> CLosMap;
template <> void AFXAPI SerializeElements<CLos *>(CArchive &ar, CLos **pElements, int nCount);

#endif // !defined(AFX_LOS_H__3F6EBB59_21F9_4D09_917D_6D627FA67221__INCLUDED_)
