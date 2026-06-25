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

// View.h: interface for the CCigiView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEW_H__80DCCCEC_896F_4E6A_B67C_35314DDDE091__INCLUDED_)
#define AFX_VIEW_H__80DCCCEC_896F_4E6A_B67C_35314DDDE091__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DebugTrace.h"
#include "stdafx.h"
#include "SharedViewObj.h"
#include "Component.h"
#include "ConfigTempl.h"

// Name this class CCigiView to differentiate it from MFC's CView class.

class CCigiView : public CObject
{
    DECLARE_SERIAL(CCigiView)

public:
    CCigiView(const int id);
    CCigiView(const TEMPL_VIEW *templ);
    ~CCigiView();
    CComponentMap &GetComponentMap(void);
    CComponent *GetComponent(const int id);
    BOOL AddComponent(CComponent *comp);
    BOOL DeleteComponent(const int id);
    void ClearComponents(void);
    BOOL ApplyTemplate(const TEMPL_VIEW *templ);
    BOOL GetDataFromTemplate(VIEW *view, const TEMPL_VIEW *templ);
    const char *const GetSharedName(void);
    HANDLE GetSharedHandle(void);
    void SynchronizeToDriver(void);
    void SetNotifyDriver(BOOL enable);
    static CCigiView *CreateForSerialization(void);
    void SetHtree(const HTREEITEM htree);
    HTREEITEM GetHtree(void);
    void SetName(LPCTSTR name);
    void SetName(CString &name);
    CString GetName(void);
    int GetViewID(void);
    void SetGroupID(const int group, const BOOL set_initial = FALSE);
    int GetGroupID(void);
    void SetEntityID(const int entity, const BOOL set_initial = FALSE);
    int GetEntityID(void);
    void SetViewType(const int type, const BOOL set_initial = FALSE);
    int GetViewType(void);
    void SetProjectionType(const int type, const BOOL set_initial = FALSE);
    int GetProjectionType(void);
    void SetReplicationMode(const int mode, const BOOL set_initial = FALSE);
    int GetReplicationMode(void);
    void SetReorderFlag(const BOOL reorder, const BOOL set_initial = FALSE);
    BOOL GetReorderFlag(void);
    void SetMirrorMode(const int mode, const BOOL set_initial = FALSE);
    int GetMirrorMode(void);
    void SetFovTop(const float top, const BOOL set_initial = FALSE);
    float GetFovTop(void);
    void SetFovBottom(const float bottom, const BOOL set_initial = FALSE);
    float GetFovBottom(void);
    void SetFovLeft(const float left, const BOOL set_initial = FALSE);
    float GetFovLeft(void);
    void SetFovRight(const float right, const BOOL set_initial = FALSE);
    float GetFovRight(void);
    void SetFovNear(const float near_dist, const BOOL set_initial = FALSE);
    float GetFovNear(void);
    void SetFovFar(const float far_dist, const BOOL set_initial = FALSE);
    float GetFovFar(void);
    void SetXOffset(const float x, const BOOL set_initial = FALSE);
    float GetXOffset(void);
    void SetYOffset(const float y, const BOOL set_initial = FALSE);
    float GetYOffset(void);
    void SetZOffset(const float z, const BOOL set_initial = FALSE);
    float GetZOffset(void);
    void SetYaw(const float yaw, const BOOL set_initial = FALSE);
    float GetYaw(void);
    void SetPitch(const float pitch, const BOOL set_initial = FALSE);
    float GetPitch(void);
    void SetRoll(const float roll, const BOOL set_initial = FALSE);
    float GetRoll(void);
    void SetXRate(const float dx);
    float GetXRate(void);
    void SetYRate(const float dy);
    float GetYRate(void);
    void SetZRate(const float dz);
    float GetZRate(void);
    void SetYawRate(const float dyaw);
    float GetYawRate(void);
    void SetPitchRate(const float dpitch);
    float GetPitchRate(void);
    void SetRollRate(const float droll);
    float GetRollRate(void);
    BOOL Unlock(void);
    void SetLocked(const VIEW &view, const BOOL set_initial = FALSE);
    VIEW GetAndLock(void);
    void SetLockedViewDef(const VIEW_DEF_DATA &viewdef, const BOOL set_initial = FALSE);
    VIEW_DEF_DATA GetAndLockViewDef(void);
    void SetLockedViewCtrl(const VIEW_CONTROL_DATA &viewctrl, const BOOL set_initial = FALSE);
    VIEW_CONTROL_DATA GetAndLockViewCtrl(void);
    void SetLockedNonCigi(const VIEW_NONCIGI_DATA &noncigi, const BOOL set_initial = FALSE);
    VIEW_NONCIGI_DATA GetAndLockNonCigi(void);

    BOOL FOVTopHasChanged(void);
    BOOL FOVBottomHasChanged(void);
    BOOL FOVLeftHasChanged(void);
    BOOL FOVRightHasChanged(void);
    BOOL FOVNearHasChanged(void);
    BOOL FOVFarHasChanged(void);
    BOOL XOffsetHasChanged(void);
    BOOL YOffsetHasChanged(void);
    BOOL ZOffsetHasChanged(void);
    BOOL YawHasChanged(void);
    BOOL PitchHasChanged(void);
    BOOL RollHasChanged(void);

    void SetInitialGroupID(const int group);
    int GetInitialGroupID(void);
    void SetInitialEntityID(const int entity);
    int GetInitialEntityID(void);
    void SetInitialViewType(const int type);
    int GetInitialViewType(void);
    void SetInitialProjectionType(const int type);
    int GetInitialProjectionType(void);
    void SetInitialReplicationMode(const int mode);
    int GetInitialReplicationMode(void);
    void SetInitialReorderFlag(const BOOL reorder);
    BOOL GetInitialReorderFlag(void);
    void SetInitialMirrorMode(const int mode);
    int GetInitialMirrorMode(void);
    void SetInitialFovTop(const float top);
    float GetInitialFovTop(void);
    void SetInitialFovBottom(const float bottom);
    float GetInitialFovBottom(void);
    void SetInitialFovLeft(const float left);
    float GetInitialFovLeft(void);
    void SetInitialFovRight(const float right);
    float GetInitialFovRight(void);
    void SetInitialFovNear(const float near_dist);
    float GetInitialFovNear(void);
    void SetInitialFovFar(const float far_dist);
    float GetInitialFovFar(void);
    void SetInitialXOffset(const float x);
    float GetInitialXOffset(void);
    void SetInitialYOffset(const float y);
    float GetInitialYOffset(void);
    void SetInitialZOffset(const float z);
    float GetInitialZOffset(void);
    void SetInitialYaw(const float yaw);
    float GetInitialYaw(void);
    void SetInitialPitch(const float pitch);
    float GetInitialPitch(void);
    void SetInitialRoll(const float roll);
    float GetInitialRoll(void);
    void SetInitial(const VIEW view);
    VIEW GetInitial(void);

    virtual void Serialize(CArchive &ar);

protected:
    CCigiView();    // Used only during serialization.

    BOOL                        m_bNotifyDriver;
    HANDLE                      m_SharedHandle;  // Non-owning alias; m_SharedObject owns the handle.
    HTREEITEM                   m_Htree;
    VIEW                        m_InitialState;
    CString                     m_Name;
    CSharedViewObj              m_SharedObject;
    CComponentMap               m_Components;

private:
    void GetView(VIEW *view, BOOL keepLock = false);
    void GetViewControlData(VIEW_CONTROL_DATA *viewControlData, BOOL keepLock = false);
    void GetViewDefData(VIEW_DEF_DATA *viewDefData, BOOL keepLock = false);
    void GetViewNonCigiData(VIEW_NONCIGI_DATA *viewNonCigiData, BOOL keepLock = false);
};

inline BOOL CCigiView::Unlock(void)
{
    // The only way this will fail is if it is not locked. We
    // should check for this in DEBUG mode.

    int retval = m_SharedObject.Unlock();
    ASSERT(retval);
    return (BOOL)retval;
}

inline const char *const CCigiView::GetSharedName(void)
{
    CDebugTrace trace("CCigiView::GetSharedName()");

    return m_SharedObject.GetInstanceName();
}

inline HANDLE CCigiView::GetSharedHandle(void)
{
    CDebugTrace trace("CCigiView::GetSharedHandle)");

    return m_SharedHandle;
}

inline CComponentMap &CCigiView::GetComponentMap(void)
{
    return m_Components;
}

inline void CCigiView::SetHtree(const HTREEITEM htree)
{
    CDebugTrace trace("CCigiView::SetHtree(HTREEITEM)");

    m_Htree = htree;
}

inline HTREEITEM CCigiView::GetHtree(void)
{
    CDebugTrace trace("CCigiView::GetHtree()");

    return m_Htree;
}

inline void CCigiView::SetName(LPCTSTR name)
{
    m_Name = name;
}

inline void CCigiView::SetName(CString &name)
{
    m_Name = name;
}

inline CString CCigiView::GetName(void)
{
    return m_Name;
}


inline void CCigiView::SetInitialGroupID(const int group)
{
    m_InitialState.viewdef.group_id = group;
}

inline int CCigiView::GetInitialGroupID(void)
{
    return m_InitialState.viewdef.group_id;
}

inline void CCigiView::SetInitialEntityID(const int entity)
{
    m_InitialState.viewctrl.entity_id = entity;
}

inline int CCigiView::GetInitialEntityID(void)
{
    return m_InitialState.viewctrl.entity_id;
}

inline void CCigiView::SetInitialViewType(const int type)
{
    m_InitialState.viewdef.view_type = type;
}

inline int CCigiView::GetInitialViewType(void)
{
    return m_InitialState.viewdef.view_type;
}

inline void CCigiView::SetInitialProjectionType(const int type)
{
    m_InitialState.viewdef.projection = type;
}

inline int CCigiView::GetInitialProjectionType(void)
{
    return m_InitialState.viewdef.projection;
}

inline void CCigiView::SetInitialReplicationMode(const int mode)
{
    m_InitialState.viewdef.replication = mode;
}

inline int CCigiView::GetInitialReplicationMode(void)
{
    return m_InitialState.viewdef.replication;
}

inline void CCigiView::SetInitialReorderFlag(const BOOL reorder)
{
    m_InitialState.viewdef.reorder = reorder;
}

inline BOOL CCigiView::GetInitialReorderFlag(void)
{
    return m_InitialState.viewdef.reorder;
}

inline void CCigiView::SetInitialMirrorMode(const int mode)
{
    m_InitialState.viewdef.mirror_mode = mode;
}

inline int CCigiView::GetInitialMirrorMode(void)
{
    return m_InitialState.viewdef.mirror_mode;
}

inline void CCigiView::SetInitialFovTop(const float top)
{
    m_InitialState.viewdef.fov_top = top;
}

inline float CCigiView::GetInitialFovTop(void)
{
    return m_InitialState.viewdef.fov_top;
}

inline void CCigiView::SetInitialFovBottom(const float bottom)
{
    m_InitialState.viewdef.fov_bottom = bottom;
}

inline float CCigiView::GetInitialFovBottom(void)
{
    return m_InitialState.viewdef.fov_bottom;
}

inline void CCigiView::SetInitialFovLeft(const float left)
{
    m_InitialState.viewdef.fov_left = left;
}

inline float CCigiView::GetInitialFovLeft(void)
{
    return m_InitialState.viewdef.fov_left;
}

inline void CCigiView::SetInitialFovRight(const float right)
{
    m_InitialState.viewdef.fov_right = right;
}

inline float CCigiView::GetInitialFovRight(void)
{
    return m_InitialState.viewdef.fov_right;
}

inline void CCigiView::SetInitialFovNear(const float near_dist)
{
    m_InitialState.viewdef.fov_near = near_dist;
}

inline float CCigiView::GetInitialFovNear(void)
{
    return m_InitialState.viewdef.fov_near;
}

inline void CCigiView::SetInitialFovFar(const float far_dist)
{
    m_InitialState.viewdef.fov_far = far_dist;
}

inline float CCigiView::GetInitialFovFar(void)
{
    return m_InitialState.viewdef.fov_far;
}

inline void CCigiView::SetInitialXOffset(const float x)
{
    m_InitialState.viewctrl.offset_x;
}

inline float CCigiView::GetInitialXOffset(void)
{
    return m_InitialState.viewctrl.offset_x;
}

inline void CCigiView::SetInitialYOffset(const float y)
{
    m_InitialState.viewctrl.offset_y = y;
}

inline float CCigiView::GetInitialYOffset(void)
{
    return m_InitialState.viewctrl.offset_y;
}

inline void CCigiView::SetInitialZOffset(const float z)
{
    m_InitialState.viewctrl.offset_z = z;
}

inline float CCigiView::GetInitialZOffset(void)
{
    return m_InitialState.viewctrl.offset_z;
}

inline void CCigiView::SetInitialYaw(const float yaw)
{
    m_InitialState.viewctrl.yaw = yaw;
}

inline float CCigiView::GetInitialYaw(void)
{
    return m_InitialState.viewctrl.yaw;
}

inline void CCigiView::SetInitialPitch(const float pitch)
{
    m_InitialState.viewctrl.pitch = pitch;
}

inline float CCigiView::GetInitialPitch(void)
{
    return m_InitialState.viewctrl.pitch;
}

inline void CCigiView::SetInitialRoll(const float roll)
{
    m_InitialState.viewctrl.roll = roll;
}

inline float CCigiView::GetInitialRoll(void)
{
    return m_InitialState.viewctrl.roll;
}

inline void CCigiView::SetInitial(const VIEW view)
{
    m_InitialState = view;
}

inline VIEW CCigiView::GetInitial(void)
{
    return m_InitialState;
}

typedef CMap<int, int, CCigiView *, CCigiView *&> CViewMap;
template <> void AFXAPI SerializeElements<CCigiView *>(CArchive &ar, CCigiView **pElements, int nCount);

#endif // !defined(AFX_VIEW_H__80DCCCEC_896F_4E6A_B67C_35314DDDE091__INCLUDED_)
