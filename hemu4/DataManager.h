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

// DataManager.h: interface for the CDataManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "common.h"
#include "CigiEvent.h"
#include "ConfigTemplDatabase.h"
#include "ConfigTemplEntity.h"
#include "ConfigTemplGlobalEnv.h"
#include "ConfigTemplView.h"
#include "ConfigTemplViewGroup.h"
#include "DatabaseTemplMap.h"
#include "EntityTemplMap.h"
#include "EnvTemplMap.h"
#include "GlobalEnv.h"
#include "IDManager.h"
#include "SystemTemplMap.h"
#include "ViewTemplMap.h"
#include "ViewGroupTemplMap.h"

#define VIEWGROUP_ARRAY_SIZE    256
#define DATABASE_ARRAY_SIZE     128

// Forward declarations.
class CCigiView;
class CWaypoint;
class CEntity;
class CViewGroup;
class CTerrainDB;
class CHemuDoc;
class CHat;
class CLos;
class CComponent;
class CGlobalEnv;
class CWeather;
class CObjectTreeView;
class CObjectStateView;
class CEntityTreeView;
class CEnvTreeView;
class CViewTreeView;
class CMessageView;
class CPacketWatchView;
class CSituationDispCtrl;
class CEntityTemplMap;
class CViewTemplMap;
class CViewGroupTemplMap;
class CDatabaseTemplMap;
class TEMPL_ENTITY;
class TEMPL_VIEW;
class TEMPL_VIEWGROUP;
class TEMPL_DATABASE;
class TEMPL_GLOBAL_ENV;

typedef CMap<int, int, CEntity *, CEntity *&> CEntityMap;
typedef CMap<int, int, CWaypoint *, CWaypoint *&> CWaypointMap;
typedef CMap<int, int, DOF, DOF &> CDofMap;
typedef CMap<int, int, CComponent *, CComponent *&> CComponentArray;

template <> void AFXAPI SerializeElements<CWaypoint *>(CArchive &ar, CWaypoint **pElements, int nCount);

///////////////////////////////////////////////////////////////////////////////
//
// The dreaded DataManager, aka the Evil Blob.  (Refactor, refactor, refactor!)
//
//  This class manages all data objects.  The objects are stored directly in
//  the document or passed to the driver via messaging.
//
///////////////////////////////////////////////////////////////////////////////

class CDataManager : public CObject
{
    DECLARE_SERIAL(CDataManager)

public:
    CDataManager();
    virtual ~CDataManager();

    int GetNextEntityID(void);
    void MarkEntityID(const int id);
    void UnmarkEntityID(const int id);
    CEntity *CreateEntity(const int id, TEMPL_ENTITY *templ);
    CEntity *CreateEntity(const int id, const int type);
    BOOL DeleteEntity(const int id);
    BOOL DeleteEntity(CEntity *entity);
    void DeleteChildEntities(const int parent);
    void DeleteAllEntities(void);
    BOOL SetEntity(CEntity *entity);
    BOOL SetEntityDOF(const DOF dof);
    CEntity *GetEntity(const int id);
    CEntity *GetEntity(const HTREEITEM htree);
    CEntity *GetSelectedEntity(void);
    CCigiView *GetView(const int id);
    CCigiView *GetView(const HTREEITEM htree);
    CCigiView *GetSelectedView(void);
    CViewGroup *GetViewGroup(const unsigned char id);
    CViewGroup *GetViewGroup(const HTREEITEM htree);
    CViewGroup *GetSelectedViewGroup(void);
    CComponent *GetSystemComponent(uint16_t component_id, uint16_t instance_id);
    CTerrainDB *GetDatabase(const unsigned char id);
    CTerrainDB *GetSelectedDatabase(void);
    unsigned char GetSelectedDatabaseNum(void);
    void SetSelectedDatabase(const unsigned char id);
    int FindFirstDatabaseNum(void);
    CGlobalEnv *GetGlobalEnv(void);
    CWeather *GetWeatherLayer(const int id);
    CWeather *GetWeatherLayer(const HTREEITEM htree);
    CWeather *GetSelectedWeatherLayer(void);
    void AddHatLookup(const int id, CHat *hat);
    BOOL RemoveHatLookup(const int id);
    void RemoveHatLookupItems(void);
    CHat *LookupHat(const int id);
    void AddLosLookup(const int id, CLos *los);
    BOOL RemoveLosLookup(const int id);
    void RemoveLosLookupItems(void);
    CLos *LookupLos(const int id);
    CHemuDoc *GetDocument();
    BOOL SetDocument(const CHemuDoc *doc);
    CObjectTreeView &GetObjectTreeView(void);
    CObjectStateView &GetObjectStateView(void);
    CEntityTreeView &GetEntityTreeView(void);
    CEnvTreeView &GetEnvTreeView(void);
    CViewTreeView &GetViewTreeView(void);
    CMessageView &GetMessageView(void);
    CPacketWatchView &GetWatchView(void);
    CSituationDispCtrl &GetSituationCtrl(void);
    CTreeCtrl &GetEntityTree(void);
    CTreeCtrl &GetEnvTree(void);
    CTreeCtrl &GetViewTree(void);
    CEntityMap &GetEntityMap(void);
    CViewMap &GetViewMap(void);
    CViewGroup **GetViewGroupArray(void);
    CEntityTemplMap &GetEntityTemplMap(void);
    CViewTemplMap &GetViewTemplMap(void);
    CViewGroupTemplMap &GetViewGroupTemplMap(void);
    CDatabaseTemplMap &GetDatabaseTemplMap(void);
    void SetEntityTempl(TEMPL_ENTITY *templ);
    void SetViewTempl(TEMPL_VIEW *templ);
    void SetViewGroupTempl(TEMPL_VIEWGROUP *templ);
    void SetDatabaseTempl(TEMPL_DATABASE *templ);
    void SetGlobalEnvTempl(TEMPL_GLOBAL_ENV *templ);
    TEMPL_ENTITY *GetEntityTempl(const int type);
    TEMPL_VIEW *GetViewTempl(const int id);
    TEMPL_VIEWGROUP *GetViewGroupTempl(const int id);
    TEMPL_DATABASE *GetDatabaseTempl(const int id);
    TEMPL_GLOBAL_ENV *GetGlobalEnvTempl(void);
    BOOL RemoveEntityTempl(const int type);
    BOOL RemoveViewTempl(const int id);
    BOOL RemoveViewGroupTempl(const int id);
    BOOL RemoveDatabaseTempl(const int id);
    void RemoveAllEntityTemplates();
    void RemoveAllViewTemplates();
    void RemoveAllViewGroupTemplates();
    void RemoveAllDatabaseTemplates();
    void LoadEntityTemplates(CStdioFile &file);
    void LoadViewTemplates(CStdioFile &file);
    void LoadViewGroupTemplates(CStdioFile &file);
    void LoadDatabaseTemplates(CStdioFile &file);
    void LoadEnvTemplates(CStdioFile &file);
    void LoadSystemTemplates(CStdioFile &file);
    void WriteEntityTemplates(CStdioFile &file);
    void WriteViewTemplates(CStdioFile &file);
    void WriteViewGroupTemplates(CStdioFile &file);
    void WriteDatabaseTemplates(CStdioFile &file);
    void WriteEnvTemplates(CStdioFile &file);
    void ApplyViewConfiguration(void);
    void ApplyTerrainConfiguration(void);
    void ApplyEnvConfiguration(void);
    void DisableWaypointFlight(void);
    void AddWaypoint(CWaypoint *waypoint);
    CWaypoint *GetWaypoint(const int id);
    void ClearWaypoints(void);
    void ResetScenario(void);
    void Reinitialize(void);
    void ClearSystemComponents(void);
    void ClearEvents(void);
    void SetTranspDelayTestPoint(int id, DOF *dof);
    BOOL GetTranspDelayTestPoint(int id, DOF *dof);
    void ClearTranspDelayTestPoints(void);
    CComponent *GetTerrainComponent(unsigned int component_id, unsigned int instance_id);

    virtual void Serialize(CArchive &ar);

private:
    int m_CurrentDatabaseID;
    CEntity *m_SearchEntity;    // Used in GetEntity().
    CEntityMap m_EntityMap;
    CViewMap m_ViewMap;
    CEntityTemplMap m_EntityTemplMap;
    CViewTemplMap m_ViewTemplMap;
    CViewGroupTemplMap m_ViewGroupTemplMap;
    CDatabaseTemplMap m_DatabaseTemplMap;
    CEnvTemplMap m_EnvTemplMap;
    CSystemTemplMap m_SystemComponents;
    TEMPL_GLOBAL_ENV m_GlobalEnvTempl;
    CIDManager m_EntityIDManager;
    CMap<int, int, CHat *, CHat *&> m_HatLookupMap;
    CMap<int, int, CLos *, CLos *&> m_LosLookupMap;
    CViewGroup *m_ViewGroupArray[VIEWGROUP_ARRAY_SIZE];
    CTerrainDB *m_DatabaseArray[DATABASE_ARRAY_SIZE];
    CGlobalEnv m_GlobalEnv;
    CWaypointMap m_WaypointMap;
    CCigiEventMap m_Events;
    CDofMap m_TranspDelayTestPoints;

    void CreateViewsFromTemplates(void);
    void CreateViewGroupsFromTemplates(void);
    void CreateDatabasesFromTemplates(void);
    void DeleteAllViews(void);
    void DeleteAllViewGroups(void);
    void DeleteAllDatabases(void);
    void CreateGlobalWeatherLayerTreeItems(HTREEITEM hparent, CWeatherMap &map);
    void ClearEnvSkyNodes(void);
    void RebuildEntityTree(void);
    void RebuildEnvTree(void);
    void RebuildViewTree(void);
    void ReorgEntityTree(void);
    void ReorgViewTree(void);
    void ResetDriverRuntimeForScenarioLoad(void);
    void DisableDriverDeletesForScenarioLoad(void);
    void SynchronizeLoadedScenarioToDriver(void);
    void ResetEntities(void);
};

#endif  // _DATAMANAGER_H_
