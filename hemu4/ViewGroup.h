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

// ViewGroup.h: interface for the CViewGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWGROUP_H__AA61D430_15B2_4FE5_A503_9D05CF554B4A__INCLUDED_)
#define AFX_VIEWGROUP_H__AA61D430_15B2_4FE5_A503_9D05CF554B4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DebugTrace.h"
#include "SharedViewGroupObj.h"
#include "Component.h"

class TEMPL_VIEWGROUP;

#define NUM_VIEWGROUPTAB_PAGES  1

class CViewGroup : public CObject
{
    DECLARE_SERIAL(CViewGroup)

public:
    CViewGroup();   // Used only during serialization.
    CViewGroup(const int id);
    CViewGroup(const TEMPL_VIEWGROUP *templ);
    ~CViewGroup();
    void SelectTab(const int index);
    void PopulateCompPropPage(CViewGroup *group);
    CComponentMap &GetComponentMap(void);
    CComponent *GetComponent(const int id);
    BOOL AddComponent(CComponent *comp);
    BOOL DeleteComponent(const int id);
    void ClearComponents(void);
    BOOL ApplyTemplate(const TEMPL_VIEWGROUP *templ);
    BOOL GetDataFromTemplate(VIEWGROUP *viewgroup, const TEMPL_VIEWGROUP *templ);
    const char *const GetSharedName(void);
    HANDLE GetSharedHandle(void);
    void SetHtree(const HTREEITEM htree);
    HTREEITEM GetHtree(void);
    void SetName(LPCTSTR name);
    void SetName(CString &name);
    CString GetName(void);
    int GetGroupID(void);
    void SetEntityID(const int entity, BOOL set_initial = FALSE);
    int GetEntityID(void);
    void SetXOffset(float x, BOOL set_initial = FALSE);
    float GetXOffset(void);
    void SetYOffset(float y, BOOL set_initial = FALSE);
    float GetYOffset(void);
    void SetZOffset(float z, BOOL set_initial = FALSE);
    float GetZOffset(void);
    void SetYaw(float yaw, BOOL set_initial = FALSE);
    float GetYaw(void);
    void SetPitch(float pitch, BOOL set_initial = FALSE);
    float GetPitch(void);
    void SetRoll(float roll, BOOL set_initial = FALSE);
    float GetRoll(void);
    void SetXRate(float dx);
    float GetXRate(void);
    void SetYRate(float dy);
    float GetYRate(void);
    void SetZRate(float dz);
    float GetZRate(void);
    void SetYawRate(float dyaw);
    float GetYawRate(void);
    void SetPitchRate(float dpitch);
    float GetPitchRate(void);
    void SetRollRate(float droll);
    float GetRollRate(void);
    BOOL Unlock(void);
    void SetLocked(const VIEWGROUP &group, BOOL set_initial = FALSE);
    VIEWGROUP GetAndLock(void);
    void SetLockedCigi(const VIEWGROUP_CIGI_DATA &group, BOOL set_initial = FALSE);
    VIEWGROUP_CIGI_DATA GetAndLockCigi(void);
    void SetLockedNonCigi(const VIEWGROUP_NONCIGI_DATA &group, BOOL set_initial = FALSE);
    VIEWGROUP_NONCIGI_DATA GetAndLockNonCigi(void);
    void SetInitialEntityID(const int entity, BOOL set_initial = FALSE);
    int GetInitialEntityID(void);
    void SetInitialXOffset(float x, BOOL set_initial = FALSE);
    float GetInitialXOffset(void);
    void SetInitialYOffset(float y, BOOL set_initial = FALSE);
    float GetInitialYOffset(void);
    void SetInitialZOffset(float z, BOOL set_initial = FALSE);
    float GetInitialZOffset(void);
    void SetInitialYaw(float yaw, BOOL set_initial = FALSE);
    float GetInitialYaw(void);
    void SetInitialPitch(float pitch, BOOL set_initial = FALSE);
    float GetInitialPitch(void);
    void SetInitialRoll(float roll, BOOL set_initial = FALSE);
    float GetInitialRoll(void);
    void SetInitial(const VIEWGROUP group, BOOL set_initial = FALSE);
    VIEWGROUP GetInitial(void);

    BOOL XOffsetHasChanged(void);
    BOOL YOffsetHasChanged(void);
    BOOL ZOffsetHasChanged(void);
    BOOL YawHasChanged(void);
    BOOL PitchHasChanged(void);
    BOOL RollHasChanged(void);

    // This will only be used after serializing (reading) the
    // object if we have created a temporary instance.
    // Serialize() resets the temporary's flag to TRUE, so
    // we need a way to reset the flag so we don't send a
    // MESSAGE_DEL_VIEWGROUP message in the temporary's
    // destructor.
    void SetNotifyDriver(BOOL enable);

    virtual void Serialize(CArchive &ar);

protected:
    BOOL                        m_bNotifyDriver;
    HANDLE                      m_SharedHandle;  // Non-owning alias; m_SharedObject owns the handle.
    HTREEITEM                   m_Htree;
    VIEWGROUP                   m_InitialState;
    CString                     m_Name;
    CSharedViewGroupObj         m_SharedObject;
    CComponentMap               m_Components;
};

inline BOOL CViewGroup::Unlock(void)
{
    // The only way this will fail is if it is not locked. We
    // should check for this in DEBUG mode.

    int retval = m_SharedObject.Unlock();
    ASSERT(retval);
    return (BOOL)retval;
}

inline void CViewGroup::SetNotifyDriver(BOOL enable)
{
    m_bNotifyDriver = enable;
}

inline const char *const CViewGroup::GetSharedName(void)
{
    CDebugTrace trace("CViewGroup::GetSharedName()");

    return m_SharedObject.GetInstanceName();
}

inline HANDLE CViewGroup::GetSharedHandle(void)
{
    CDebugTrace trace("CViewGroup::GetSharedHandle()");

    return m_SharedHandle;
}

inline  CComponentMap &CViewGroup::GetComponentMap(void)
{
    return m_Components;
}

inline void CViewGroup::SetHtree(const HTREEITEM htree)
{
    CDebugTrace trace("CViewGroup::SetHtree(HTREEITEM)");

    m_Htree = htree;
}

inline HTREEITEM CViewGroup::GetHtree(void)
{
    CDebugTrace trace("CViewGroup::GetHtree()");

    return m_Htree;
}

inline void CViewGroup::SetName(LPCTSTR name)
{
    m_Name = name;
}

inline void CViewGroup::SetName(CString &name)
{
    m_Name = name;
}

inline CString CViewGroup::GetName(void)
{
    return m_Name;
}

inline void CViewGroup::SetInitialEntityID(const int entity, BOOL set_initial)
{
    m_InitialState.cigi.entity_id = entity;
}

inline int CViewGroup::GetInitialEntityID(void)
{
    return m_InitialState.cigi.entity_id;
}

inline void CViewGroup::SetInitialXOffset(float x, BOOL set_initial)
{
    m_InitialState.cigi.offset_x = x;
}

inline float CViewGroup::GetInitialXOffset(void)
{
    return m_InitialState.cigi.offset_x;
}

inline void CViewGroup::SetInitialYOffset(float y, BOOL set_initial)
{
    m_InitialState.cigi.offset_y = y;
}

inline float CViewGroup::GetInitialYOffset(void)
{
    return m_InitialState.cigi.offset_y;
}

inline void CViewGroup::SetInitialZOffset(float z, BOOL set_initial)
{
    m_InitialState.cigi.offset_z = z;
}

inline float CViewGroup::GetInitialZOffset(void)
{
    return m_InitialState.cigi.offset_z;
}

inline void CViewGroup::SetInitialYaw(float yaw, BOOL set_initial)
{
    m_InitialState.cigi.yaw = yaw;
}

inline float CViewGroup::GetInitialYaw(void)
{
    return m_InitialState.cigi.yaw;
}

inline void CViewGroup::SetInitialPitch(float pitch, BOOL set_initial)
{
    m_InitialState.cigi.pitch = pitch;
}

inline float CViewGroup::GetInitialPitch(void)
{
    return m_InitialState.cigi.pitch;
}

inline void CViewGroup::SetInitialRoll(float roll, BOOL set_initial)
{
    m_InitialState.cigi.roll = roll;
}

inline float CViewGroup::GetInitialRoll(void)
{
    return m_InitialState.cigi.roll;
}

inline void CViewGroup::SetInitial(const VIEWGROUP group, BOOL set_initial)
{
    m_InitialState = group;
}

inline VIEWGROUP CViewGroup::GetInitial(void)
{
    return m_InitialState;
}

#endif // !defined(AFX_VIEWGROUP_H__AA61D430_15B2_4FE5_A503_9D05CF554B4A__INCLUDED_)
