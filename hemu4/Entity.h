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

// Entity.h: interface for the CEntity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENTITY_H__8341BA00_61B6_46C0_83FB_35CCBAC7ADD3__INCLUDED_)
#define AFX_ENTITY_H__8341BA00_61B6_46C0_83FB_35CCBAC7ADD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ArtPart.h"
#include "AttachPoint.h"
#include "CollisionSeg.h"
#include "CollisionVol.h"
#include "common.h"
#include "Component.h"
#include "ConfigTemplEntity.h"
#include "Hat.h"
#include "IDManager.h"
#include "Los.h"
#include "SharedEntityObj.h"
#include "TSQueue.h"
#include "Waypoint.h"

class CWeather;

typedef CList<long, long> CLongList;

class CEntity : public CObject
{
    DECLARE_SERIAL(CEntity)

public:
//
// chas added to prevent drv from sending entityctrl packet. see entitypositionctrl packet
	void ClearLockedChangeFlag( void ){ m_SharedObject.ClearLockedChangeFlag(); };
//chas
//
    // Construction/Deconstruction
    CEntity(const int id, const int type, const int entity_class, const DOF &dof);
    CEntity(const int id, const TEMPL_ENTITY *templ, const DOF &dof);
    CEntity();      // Used only by serialization.
    ~CEntity();
    BOOL ApplyTemplate(const TEMPL_ENTITY *templ);
    const char *const GetSharedName(void);
    HANDLE GetSharedHandle(void);
    void SynchronizeToDriver(void);
    void SetNotifyDriver(BOOL enable);

    // Serialization
    virtual void Serialize(CArchive &ar);
    static void SerializeStaticVars(CArchive &ar);

    // GUI Integration
    void SetHtree(const HTREEITEM htree);
    HTREEITEM GetHtree(void);
    void SetHtreeEnv(const HTREEITEM htree);
    HTREEITEM GetHtreeEnv(void);
    void SetSituationHandle(const POSITION pos);
    POSITION GetSituationHandle(void);

    // Shared Data Access -- Locking
    BOOL Unlock(void);
    void SetSendRates(const BOOL send);
    BOOL GetSendRates(void);
    void SetRatesCoordSystem(const int cs);
    BOOL GetRatesCoordSystem(void);
    void SetLockedDofs(const DOF &dof, const BOOL set_initial = FALSE);
    DOF GetAndLockDofs(void);
    void SetLockedRates(const RATES &rates, const BOOL set_initial = FALSE);
    RATES GetAndLockRates(void);
    void SetLockedCigi(const ENTITY_CIGI_DATA &data, const BOOL set_initial = FALSE);
    ENTITY_CIGI_DATA GetAndLockCigi(void);
    void SetLockedNonCigi(const ENTITY_NONCIGI_DATA &data, const BOOL set_initial = FALSE);
    ENTITY_NONCIGI_DATA GetAndLockNonCigi(void);
    void SetLockedUnsaved(const ENTITY_TEMP_DATA &data);
    ENTITY_TEMP_DATA GetAndLockUnsaved(void);
    void SetLocked(const ENTITY &entity, const BOOL set_initial = FALSE);
    ENTITY GetAndLock(void);
    void SetLockedEntityPosition(const ENTITY &entity, const BOOL set_initial = FALSE);

    // Shared Data Access -- Unlocking
    DOF GetDofs(void);                      // unlocking version
    void SetActiveState(const int state, const BOOL set_initial = FALSE);
    int GetActiveState(void);
    void SetParent(const int parent, const BOOL set_initial = FALSE);
    int GetParent(void);
    void SetTarget(const int target, const BOOL set_initial = FALSE);
    int GetTarget(void);
    void SetAlpha(const unsigned char alpha, const BOOL set_initial = FALSE);
    unsigned char GetAlpha(void);
    void SetAnimState(const int state, const BOOL set_initial = FALSE);
    int GetAnimState(void);
    void SetAnimDir(const int direction, const BOOL set_initial = FALSE);
    int GetAnimDir(void);
    void SetAnimLoop(const int loopmode, const BOOL set_initial = FALSE);
    int GetAnimLoop(void);
    void SetInheritAlpha(const BOOL inherit, const BOOL set_initial = FALSE);
    BOOL GetInheritAlpha(void);
    void SetClampMode(const int mode, const BOOL set_initial = FALSE);
    int GetClampMode(void);
    void SetCollDetectEnable(const BOOL enable, const BOOL set_initial = FALSE);
    BOOL GetCollDetectEnable(void);
    void SetFlyMode(const int mode, const BOOL set_initial = FALSE);
    int GetFlyMode(void);
    void SetAttachPoint(const int attachpt, const BOOL set_initial = FALSE);
    int GetAttachPoint(void);
    void SetSpeed(const double speed, const BOOL set_initial = FALSE);
    double GetSpeed(void);
    void SetCollective(const long collective, const BOOL set_initial = FALSE);
    long GetCollective(void);
    void SetFlyWaypoints(const BOOL enable);
    BOOL GetFlyWaypoints(void);
    void SetTrackTarget(const BOOL enable);
    BOOL GetTrackTarget(void);
    void SetRollDisable(const BOOL disable);
    BOOL GetRollDisable(void);

    // Non-Shared Data Access
    long GetID(void);
    long GetType(void);
    long GetClass(void);
    void SetName(LPCTSTR name);
    void SetName(CString &name);
    CString GetName(void);
    long GetRefEntityID(void);
    void SetRefEntityID(const long id);
    CComponentMap &GetComponentMap(void);
    CComponent *GetComponent(const int id);
    BOOL AddComponent(CComponent *comp);
    BOOL DeleteComponent(const int id);
    CArtPartMap &GetArtPartMap(void);
    CArtPart *GetArtPart(const int id);
    BOOL AddArtPart(CArtPart *artpart);
    BOOL DeleteArtPart(const int id);
    CAttachPointMap &GetAttachPointMap(void);
    CAttachPoint *GetAttachPoint(const int id);
    BOOL AddAttachPoint(CAttachPoint *attach);
    BOOL DeleteAttachPoint(const int id);
    CHatMap &GetHatMap(void);
    CHat *GetHat(const int id);
    BOOL AddHat(CHat *hat);
    BOOL DeleteHat(const int id);
    CLosMap &GetLosMap(void);
    CLos *GetLos(const int id);
    BOOL AddLos(CLos *los);
    BOOL DeleteLos(const int id);
    CCollisionSegMap &GetCollisionSegMap(void);
    CCollisionSeg *GetCollisionSeg(const int id);
    BOOL AddCollisionSeg(CCollisionSeg *seg);
    BOOL DeleteCollisionSeg(const int id);
    CCollisionVolMap &GetCollisionVolMap(void);
    CCollisionVol *GetCollisionVol(const int id);
    BOOL AddCollisionVol(CCollisionVol *vol);
    BOOL DeleteCollisionVol(const int id);
    void ClearArtParts(void);
    void ClearComponents(void);
    void ClearHat(void);
    void ClearLos(void);
    void ClearCollisionSegs(void);
    void ClearCollisionVols(void);
    int GetNextHatID(void);
    int GetNextLosID(void);
    int GetNextCollisionSegID(void);
    int GetNextCollisionVolID(void);
    CWeather *GetWeatherAttributes(void);
    CLongList &GetLaunchAnimList(void);
    CLongList &GetPersistentLaunchAnimList(void);
    CLongList &GetDetonationAnimList(void);
    void AddWaypoint(CWaypoint *waypoint);
    int AcquireNextWaypoint(const BOOL enable = TRUE);
    void ClearWaypoints(void);
    void FlyWaypoints(const BOOL enable = TRUE);

    // Initial State Data
    void SetInitialActiveState(const int state);
    int GetInitialActiveState(void);
    void SetInitialParent(const int parent);
    int GetInitialParent(void);
    void SetInitialTarget(const int target);
    int GetInitialTarget(void);
    void SetInitialAlpha(const unsigned char alpha);
    unsigned char GetInitialAlpha(void);
    void SetInitialAnimState(const int state);
    int GetInitialAnimState(void);
    void SetInitialAnimDir(const int direction);
    int GetInitialAnimDir(void);
    void SetInitialAnimLoop(const int loopmode);
    int GetInitialAnimLoop(void);
    void SetInitialInheritAlpha(const BOOL inherit);
    BOOL GetInitialInheritAlpha(void);
    void SetInitialClampMode(const int mode);
    int GetInitialClampMode(void);
    void SetInitialCollDetectEnable(const BOOL enable);
    BOOL GetInitialCollDetectEnable(void);
    void SetInitialFlyMode(const int mode);
    int GetInitialFlyMode(void);
    void SetInitialAttachPoint(const int attachpt);
    int GetInitialAttachPoint(void);
    void SetInitialSpeed(const double speed);
    double GetInitialSpeed(void);
    void SetInitialCollective(const long collective);
    long GetInitialCollective(void);
    void SetInitialSendRates(const BOOL send);
    BOOL GetInitialSendRates(void);
    void SetInitialRatesCoordSystem(const int cs);
    int GetInitialRatesCoordSystem(void);
    void SetInitialDofs(const DOF &dofs);
    DOF GetInitialDofs(void);
    void SetInitialRates(const RATES &rates);
    RATES GetInitialRates(void);
    void SetInitialCigi(const ENTITY_CIGI_DATA &data);
    ENTITY_CIGI_DATA GetInitialCigi(void);
    void SetInitialNonCigi(const ENTITY_NONCIGI_DATA &data);
    ENTITY_NONCIGI_DATA GetInitialNonCigi(void);

    // Utility Functions
    BOOL CalcRefBearingAndRange(double *bearing, double *range);
    BOOL SetRelativePosition(const double bearing, const double range, const BOOL set_initial = FALSE);
    void SendCigiPacket(void);

    // Relational
    BOOL IsDescendantOf(const int ancestor_id);
    BOOL IsDescendantOf(CEntity *ancestor);

protected:
    void ClearAttachPoints(void);

    static CIDManager       m_HatIDManager;         // shared
    static CIDManager       m_LosIDManager;         // shared
    long                    m_EntityID;
    long                    m_EntityType;
    long                    m_EntityClass;
    long                    m_RefEntityID;
    BOOL                    m_bNotifyDriver;
    HANDLE                  m_SharedHandle;  // Non-owning alias; m_SharedObject owns the handle.
    HTREEITEM               m_HtreeEntity;
    HTREEITEM               m_HtreeEnv;
    POSITION                m_SituationHandle;
    CString                 m_Name;
    CSharedEntityObj        m_SharedObject;
    ENTITY_CIGI_DATA        m_InitialCigiData;
    ENTITY_NONCIGI_DATA     m_InitialNonCigiData;
    CAttachPointMap         m_AttachPoints;
    CArtPartMap             m_ArtParts;
    CComponentMap           m_Components;
    CHatMap                 m_Hat;
    CLosMap                 m_Los;
    CCollisionSegMap        m_CollSegs;
    CCollisionVolMap        m_CollVols;
    CIDManager              m_CollSegIDManager;
    CIDManager              m_CollVolIDManager;
    CWeather                *m_WeatherAttributes;
    CLongList               m_LaunchAnimIDs;
    CLongList               m_PersistentLaunchAnimIDs;
    CLongList               m_DetonationAnimIDs;
    CTSQueue<CWaypoint>     m_Waypoints;

private:
    void CommonEntityInit(int entityId = -1,
                          int entityType = -1,
                          int entityClass = -1,
                          BOOL notifyDriver = FALSE,
                          BOOL applyTemplate = FALSE,
                          const DOF *dof = NULL,
                          const TEMPL_ENTITY *templ = NULL);
    void GetEntity(ENTITY *entity, BOOL keepLock = false);
};

typedef CMap<int, int, CEntity *, CEntity *&> CEntityMap;

#endif // !defined(AFX_ENTITY_H__8341BA00_61B6_46C0_83FB_35CCBAC7ADD3__INCLUDED_)
