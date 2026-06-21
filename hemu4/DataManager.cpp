/** <pre>
 *  Copyright 2004 The Boeing Company
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
 *
 *  FILENAME:   DataManager.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface Host Emulator (CIGI HE)
 *
 *  PROGRAM DESCRIPTION:
 *  The Host Emulator (HE) acts as a surrogate in the absence of a simulation host.
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME
 *  DESCRIPTION OF CHANGE
 *
 *  11/11/2004 Lance Durham
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  Version 3.1.1
 */

#include "stdafx.h"
#include "hemu4.h"
#include "ViewGroup.h"
#include "TerrainDB.h"
#include "HemuDoc.h"
#include "DataManager.h"
#include "Entity.h"
#include "SituationDispCtrl.h"
#include "ObjectTreeView.h"
#include "CigiView.h"
#include "ViewStateView.h"
#include "TerrainStateView.h"
#include "ObjectStateView.h"
#include "EntityStateView.h"
#include "hemumsg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// Used for reading configuration templates.  Defined in ConfigTempl.cpp.
CString ReadBlock(CStdioFile &file, int *linenum);
CString ReadBlock(CString &src, int *linenum);
BOOL ReadLine(CString IN OUT &str, int *linenum, CString OUT &token, CString OUT &params);

static const DWORD_PTR ENTITY_MAP_SERIAL_MAGIC = 0x48454e31; // "HEN1"
static const DWORD_PTR ENTITY_MAP_SERIAL_VERSION = 1;

static BOOL SerializeEntityMap(CArchive &ar, CEntityMap &map)
{
    if (ar.IsStoring()) {
        DWORD_PTR count = 0;
        POSITION countPos = map.GetStartPosition();

        while (countPos) {
            int id = 0;
            CEntity *entity = NULL;
            map.GetNextAssoc(countPos, id, entity);

            if (entity)
                count++;
        }

        ar.WriteCount(ENTITY_MAP_SERIAL_MAGIC);
        ar.WriteCount(ENTITY_MAP_SERIAL_VERSION);
        ar.WriteCount(count);

        POSITION pos = map.GetStartPosition();
        while (pos) {
            int id = 0;
            CEntity *entity = NULL;
            map.GetNextAssoc(pos, id, entity);

            if (entity) {
                ar << id;
                entity->Serialize(ar);
            }
        }

        return FALSE;
    }

    DWORD_PTR first = ar.ReadCount();

    if (first != ENTITY_MAP_SERIAL_MAGIC) {
        // Older Win32 builds stored raw pointer values instead of entities.
        for (DWORD_PTR i = 0; i < first; i++) {
            int id = 0;
            DWORD ignoredPointer = 0;

            ar.EnsureRead(&id, sizeof(id));
            ar.EnsureRead(&ignoredPointer, sizeof(ignoredPointer));
        }

        return first > 0;
    }

    DWORD_PTR version = ar.ReadCount();
    if (version != ENTITY_MAP_SERIAL_VERSION)
        AfxThrowArchiveException(CArchiveException::badSchema);

    DWORD_PTR count = ar.ReadCount();

    for (DWORD_PTR i = 0; i < count; i++) {
        int id = 0;
        CEntity *entity = new CEntity;

        try {
            ar >> id;
            entity->Serialize(ar);
            map.SetAt(id, entity);
        } catch (...) {
            delete entity;
            throw;
        }
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CDataManager, CObject, 0)

CDataManager::CDataManager() :
    m_DatabaseTemplMap(&m_GlobalEnvTempl),
    m_EnvTemplMap(&m_GlobalEnvTempl)
{
    CDebugTrace trace("CDataManager::CDataManager()");

    m_CurrentDatabaseID = 1;    // Default to Database 1
    m_SearchEntity = NULL;

    // Set up the CMaps.  Microsoft recommends setting the hash table
    // size to a prime number that is 20% larger than the size of the
    // data set.  Assume a maximum of around 512 entries for each.
    m_EntityMap.RemoveAll();
    m_EntityMap.InitHashTable(617);
    m_ViewMap.RemoveAll();
    m_ViewMap.InitHashTable(617);
    m_EntityTemplMap.RemoveAll();
    m_EntityTemplMap.InitHashTable(617);
    m_HatLookupMap.RemoveAll();
    m_HatLookupMap.InitHashTable(521);
    m_LosLookupMap.RemoveAll();
    m_LosLookupMap.InitHashTable(521);
    m_WaypointMap.RemoveAll();
    m_WaypointMap.InitHashTable(521);
    m_SystemComponents.RemoveAll();
    m_SystemComponents.InitHashTable(521);
    m_Events.RemoveAll();
    m_Events.InitHashTable(521);

    // Initialize the arrays.
    for (int i = 0; i < VIEWGROUP_ARRAY_SIZE; i++)
        m_ViewGroupArray[i] = NULL;

    for (int i = 0; i < DATABASE_ARRAY_SIZE; i++)
        m_DatabaseArray[i] = NULL;
}

CDataManager::~CDataManager()
{
    CDebugTrace trace("CDataManager::~CDataManager()");

    DeleteAllEntities();
    DeleteAllViews();
    DeleteAllViewGroups();
    DeleteAllDatabases();
}

void CDataManager::Serialize(CArchive &ar)
{
    CDebugTrace trace("CDataManager::Serialize(CArchive &)");

    m_SearchEntity = NULL;

    if (ar.IsStoring()) {
        ar << m_CurrentDatabaseID;
        SerializeEntityMap(ar, m_EntityMap);
        m_ViewMap.Serialize(ar);
        m_EntityTemplMap.Serialize(ar);
        m_ViewTemplMap.Serialize(ar);
        m_ViewGroupTemplMap.Serialize(ar);
        m_DatabaseTemplMap.Serialize(ar);
        m_GlobalEnvTempl.Serialize(ar);
        m_EntityIDManager.Serialize(ar);
        m_HatLookupMap.Serialize(ar);
        m_LosLookupMap.Serialize(ar);
        m_GlobalEnv.Serialize(ar);
        m_WaypointMap.Serialize(ar);
        m_SystemComponents.Serialize(ar);
        m_Events.Serialize(ar);

        // Find the number of view groups.
        int viewGroupsCount = 0;
        for (int i = 0; i < VIEWGROUP_ARRAY_SIZE; i++) {
            CViewGroup *group = m_ViewGroupArray[i];
            if (group)
                viewGroupsCount++;
        }

        ar << viewGroupsCount;

        // Now store the view groups.
        for (int i = 0; i < VIEWGROUP_ARRAY_SIZE; i++) {
            CViewGroup *group = m_ViewGroupArray[i];
            if (group)
                group->Serialize(ar);
        }

        // Find the number of databases.
        int dbCount = 0;
        for (int i = 0; i < DATABASE_ARRAY_SIZE; i++) {
            CTerrainDB *db = m_DatabaseArray[i];
            if (db)
                dbCount++;
        }

        ar << dbCount;

        // Now store the databases.
        for (int i = 0; i < DATABASE_ARRAY_SIZE; i++) {
            CTerrainDB *db = m_DatabaseArray[i];
            if (db)
                db->Serialize(ar);
        }

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
    } else {
        // Clear the situational display.
        theApp.GetMainFrame().GetSituationCtrl().RemoveAllEntities();

        // Clear the trees and reload the configuration.
        theApp.GetMainFrame().GetEntityTree().DeleteAllItems();
        theApp.GetMainFrame().GetViewTree().DeleteAllItems();
        theApp.GetMainFrame().GetEnvTree().DeleteAllItems();
        theApp.InitializeTrees();

        // Delete the entities, views, view groups, databases, mission function
        // objects, and configuration templates.
        DeleteAllEntities();
        DeleteAllViews();
        DeleteAllViewGroups();
        DeleteAllDatabases();
        RemoveHatLookupItems();
        RemoveLosLookupItems();
        ClearEvents();
        ClearWaypoints();

        ar >> m_CurrentDatabaseID;
        BOOL legacyEntityMapSkipped = SerializeEntityMap(ar, m_EntityMap);
        m_ViewMap.Serialize(ar);
        m_EntityTemplMap.Serialize(ar);
        m_ViewTemplMap.Serialize(ar);
        m_ViewGroupTemplMap.Serialize(ar);
        m_DatabaseTemplMap.Serialize(ar);
        m_GlobalEnvTempl.Serialize(ar);
        m_EntityIDManager.Serialize(ar);
        m_HatLookupMap.Serialize(ar);
        m_LosLookupMap.Serialize(ar);
        m_GlobalEnv.Serialize(ar);
        m_WaypointMap.Serialize(ar);
        m_SystemComponents.Serialize(ar);
        m_Events.Serialize(ar);

        // Clear the view groups and databases.
        for (int i = 0; i < VIEWGROUP_ARRAY_SIZE; i++) {
            if (m_ViewGroupArray[i])
                delete m_ViewGroupArray[i];

            m_ViewGroupArray[i] = NULL;

            if (i < DATABASE_ARRAY_SIZE) {
                if (m_DatabaseArray[i])
                    delete m_DatabaseArray[i];

                m_DatabaseArray[i] = NULL;
            }
        }

        // Read the view groups.
        int viewGroupsCount = 0;

        ar >> viewGroupsCount;

        for (int i = 0; i < viewGroupsCount; i++) {
            // Create a temporary object.  This means that the
            // CViewGroup constructor needs to be public.
            CViewGroup temp;
            temp.Serialize(ar);
            temp.SetNotifyDriver(FALSE);    // Don't send delete message when
            // temp goes out of scope

            // Now create the real object and copy temp to it.
            int group_id = temp.GetGroupID();
            CViewGroup *group = new CViewGroup(group_id);
            group->GetAndLock();
            group->SetLocked(temp.GetAndLock(), TRUE);
            group->Unlock();
            temp.Unlock();

            if (group_id == -1)
                delete group;
            else
                m_ViewGroupArray[group_id] = group;
        }

        // Now read the databases.
        int dbCount = 0;

        ar >> dbCount;

        for (int i = 0; i < dbCount; i++) {
            TEMPL_DATABASE dummy;
            dummy.DatabaseID = -1;
            CTerrainDB *db = new CTerrainDB(&dummy);
            db->Serialize(ar);

            if (db->GetID() == -1)
                delete db;
            else
                m_DatabaseArray[db->GetID()] = db;
        }

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        if (legacyEntityMapSkipped && (m_EntityMap.GetCount() == 0)) {
            TEMPL_ENTITY *templ = GetEntityTempl(0);
            TEMPL_ENTITY defaultType;

            if (!templ) {
                defaultType.Type = 0;
                defaultType.Name = "None";
                defaultType.Class = ENTITY_CLASS_FIXEDWING;
                templ = &defaultType;
            }

            CreateEntity(0, templ);
        }

        // Populate the trees.
        RebuildEntityTree();
        RebuildEnvTree();
        RebuildViewTree();

        // Simulate selection of the Entity tab.
        theApp.GetMainFrame().GetObjectTreeView().SelectTab(0);
    }
}

CHemuDoc *CDataManager::GetDocument()
{
    CDebugTrace trace("CDataManager::GetDocument()");

    CHemuDoc *document = NULL;
    CHemuApp *app = (CHemuApp *)AfxGetApp();

    if (app) {
        POSITION PosTempl = app->GetFirstDocTemplatePosition();
        CSingleDocTemplate *templ = (CSingleDocTemplate *)app->GetNextDocTemplate(PosTempl);

        if (templ) {
            POSITION PosDoc = templ->GetFirstDocPosition();
            document = (CHemuDoc *)templ->GetNextDoc(PosDoc);
        }
    }

    return document;
}

CEntity *CDataManager::CreateEntity(const int id, TEMPL_ENTITY *templ)
{
    CDebugTrace trace("CDataManager::CreateEntity(const int, TEMPL_ENTITY *)");

    DOF dof = { 0 };
    double speed = 0.0;

    if (id == 0) {  // Place the ownship at the default position specified in the def file.
        CTerrainDB *db = GetSelectedDatabase();
        if (db) {
            dof.latitude = ((fabs(db->GetDefaultLat()) < 0.0000001) ?
                            db->GetOriginLat() : db->GetDefaultLat());
            dof.longitude = ((fabs(db->GetDefaultLon()) < 0.0000001) ?
                             db->GetOriginLon() : db->GetDefaultLon());
            dof.altitude = ((fabs(db->GetDefaultAlt()) < 0.0000001) ?
                            db->GetOriginAlt() : db->GetDefaultAlt());
        }
    } else {        // Place the new entity near the Ownship.
        double bearing, range;
        CEntity *ownship = GetEntity(0);

        // If there is no ownship, just place the entity at the origin.
        if (!ownship) {
            CTerrainDB *db = GetSelectedDatabase();
            if (db) {
                dof.latitude = ((fabs(db->GetDefaultLat()) < 0.0000001) ?
                                db->GetOriginLat() : db->GetDefaultLat());
                dof.longitude = ((fabs(db->GetDefaultLon()) < 0.0000001) ?
                                 db->GetOriginLon() : db->GetDefaultLon());
                dof.altitude = ((fabs(db->GetDefaultAlt()) < 0.0000001) ?
                                db->GetOriginAlt() : db->GetDefaultAlt());
            }
        } else { // Otherwise, place new entities in a circle around the Ownship.
            DOF ownship_dof = ownship->GetDofs();
            dof.altitude = ownship_dof.altitude;
            dof.yaw = ownship_dof.yaw;

            speed = ownship->GetSpeed();

            // Place the models at 45 degree intervals.
            if (id % 2)     // Odd-numbered entity -- left side
                bearing = (id - 1) * -22.5f;
            else            // Even-numbered entity -- right side
                bearing = id * 22.5f;

            range = 30.0 * (((id - 1) / 8) + 1);

            GetRelativePos(ownship_dof.latitude, ownship_dof.longitude,
                           ownship_dof.yaw, bearing, range,
                           &dof.latitude, &dof.longitude);
        }
    }

    CEntity *entity = new CEntity(id, templ, dof);

    if (entity) {
        // Add the entity to the entity map.
        m_EntityMap[id] = entity;

        // Mark the ID as used.
        MarkEntityID(id);

        entity->SetSpeed(speed);

        // Set the initial conditions, including position and orientation.
        entity->SetInitialDofs(dof);
    }

    return entity;
}

CEntity *CDataManager::CreateEntity(const int id, const int type)
{
    CDebugTrace trace("CDataManager::CreateEntity(const int, const int)");

    TEMPL_ENTITY *templ = GetEntityTempl(type);

    return templ ? CreateEntity(id, templ) : NULL;
}

BOOL CDataManager::DeleteEntity(CEntity *entity)
{
    CDebugTrace trace("CDataManager::DeleteEntity(CEntity *)");

    if (entity) {
        int id = entity->GetID();

        // Delete all the children.
        DeleteChildEntities(id);

        // Remove the entry from the map.
        m_EntityMap.RemoveKey(id);

        // Mark the ID as not used.
        UnmarkEntityID(id);

        if (entity == m_SearchEntity)
            m_SearchEntity = NULL;

        // Delete the object.  The destructor will send a MESSAGE_DEL_ENTITY
        // message to the driver.
        delete entity;
    }

    return (entity != NULL);
}

void CDataManager::DeleteChildEntities(const int parent)
{
    CDebugTrace trace("CDataManager::DeleteChildEntities(const int)");

    POSITION pos = m_EntityMap.GetStartPosition();

    while (pos) {
        int id = 0;
        CEntity *entity = NULL;

        m_EntityMap.GetNextAssoc(pos, id, entity);

        if (entity && (entity->GetParent() == parent))
            DeleteEntity(entity);
    }
}

void CDataManager::DeleteAllEntities(void)
{
    CDebugTrace trace("CDataManager::DeleteAllEntities()");

    m_SearchEntity = NULL;

    CArray<CEntity *, CEntity *> entities;
    CArray<int, int> ids;

    POSITION pos = m_EntityMap.GetStartPosition();

    while (pos) {
        int id = 0;
        CEntity *entity = NULL;

        m_EntityMap.GetNextAssoc(pos, id, entity);

        if (entity) {
            ids.Add(id);
            entities.Add(entity);
        }
    }

    m_EntityMap.RemoveAll();

    for (INT_PTR i = 0; i < ids.GetSize(); i++)
        UnmarkEntityID(ids[i]);

    for (INT_PTR j = 0; j < entities.GetSize(); j++) {
        // The destructor sends the driver a delete message when appropriate.
        delete entities[j];
    }
}

TEMPL_ENTITY *CDataManager::GetEntityTempl(const int type)
{
    CDebugTrace trace("CDataManager::GetEntityTempl(const int)");

    TEMPL_ENTITY *templ = NULL;

    return m_EntityTemplMap.Lookup(type, templ) ? templ : NULL;
}

void CDataManager::RemoveAllEntityTemplates()
{
    m_EntityTemplMap.RemoveTemplates();
}

void CDataManager::RemoveAllViewTemplates()
{
    m_ViewTemplMap.RemoveTemplates();
}

void CDataManager::RemoveAllViewGroupTemplates()
{
    m_ViewGroupTemplMap.RemoveTemplates();
}

void CDataManager::RemoveAllDatabaseTemplates()
{
    m_DatabaseTemplMap.RemoveTemplates();
}

BOOL CDataManager::SetEntity(CEntity *entity)
{
    CDebugTrace trace("CDataManager::SetEntity(CEntity *)");

    CEntity *mapentry = NULL;

    m_EntityMap.Lookup(entity->GetID(), mapentry);

    if (!mapentry)
        return FALSE;

    ENTITY newstate = entity->GetAndLock();
    entity->Unlock();

    // Overwrite the existing entity.
    mapentry->GetAndLock();
    mapentry->SetLocked(newstate);
    mapentry->Unlock();

    return TRUE;
}

CEntity *CDataManager::GetEntity(const HTREEITEM htree)
{
    // Test the last entity we found.  This will make subsequent
    // calls much more efficient.
    if (m_SearchEntity && (m_SearchEntity->GetHtree() == htree))
        return m_SearchEntity;

    // If we have selected a new entity since the last call, perform
    // the search.
    POSITION pos = m_EntityMap.GetStartPosition();

    while (pos) {
        int id = 0;

        m_EntityMap.GetNextAssoc(pos, id, m_SearchEntity);

        if (m_SearchEntity->GetHtree() == htree)
            return m_SearchEntity;
    }

    return NULL;
}

CCigiView *CDataManager::GetView(const HTREEITEM htree)
{
    CDebugTrace trace("CDataManager::GetView(const HTREEITEM)");

    POSITION pos = m_ViewMap.GetStartPosition();

    while (pos) {
        int id = 0;
        CCigiView *view = NULL;

        m_ViewMap.GetNextAssoc(pos, id, view);

        if (view && (view->GetHtree() == htree))
            return view;
    }

    return NULL;
}

CViewGroup *CDataManager::GetViewGroup(const HTREEITEM htree)
{
    CDebugTrace trace("CDataManager::GetViewGroup(const HTREEITEM)");

    for (int i = 0; i < VIEWGROUP_ARRAY_SIZE; i++) {
        if (m_ViewGroupArray[i]) {
            if (m_ViewGroupArray[i]->GetHtree() == htree)
                return m_ViewGroupArray[i];
        }
    }

    return NULL;
}

void CDataManager::SetSelectedDatabase(const unsigned char id)
{
    CDebugTrace trace("CDataManager::SetSelectedDatabase(const unsigned char)");

    if ((id == 0) || (id > 127) || (m_DatabaseArray[id] == NULL))
        return;

    m_CurrentDatabaseID = id;

    // We use TM for the situational view.
    SetTMOrigin(m_DatabaseArray[id]->GetOriginLat(),
                m_DatabaseArray[id]->GetOriginLon());

    // Send a message to the driver.
    MESSAGE_SET_DATABASE msg;
    msg.dbnum = m_DatabaseArray[id]->GetID();
    msg.lat = m_DatabaseArray[id]->GetOriginLat();
    msg.lon = m_DatabaseArray[id]->GetOriginLon();
    msg.radius = m_DatabaseArray[id]->GetErmRadius();
    msg.flattening = m_DatabaseArray[id]->GetErmFlattening();
    PostDriverMsg(msg);
}

int CDataManager::FindFirstDatabaseNum(void)
{
    for (int i = 0; i < DATABASE_ARRAY_SIZE; i++) {
        if (m_DatabaseArray[i])
            return i;
    }

    return 0;
}

TEMPL_GLOBAL_ENV *CDataManager::GetGlobalEnvTempl(void)
{
    CDebugTrace trace("CDataManager::GetGlobalEnvTempl()");

    return &m_GlobalEnvTempl;
}

void CDataManager::CreateViewsFromTemplates(void)
{
    CDebugTrace trace("CDataManager::CreateViewsFromTemplates()");

    m_ViewTemplMap.CreateViews(this,
                               &theApp.GetMainFrame(),
                               m_ViewGroupArray,
                               &m_ViewMap);
}

void CDataManager::CreateViewGroupsFromTemplates(void)
{
    CDebugTrace trace("CDataManager::CreateViewGroupsFromTemplates()");

    m_ViewGroupTemplMap.CreateViewGroups(&theApp.GetMainFrame(),
                                         m_ViewGroupArray);

    theApp.GetMainFrame().GetViewStateView().PopulateViewGroupList();
}

void CDataManager::CreateDatabasesFromTemplates(void)
{
    CDebugTrace trace("CDataManager::CreateDatabasesFromTemplates()");

    m_DatabaseTemplMap.CreateDatabases(m_DatabaseArray);

    theApp.GetMainFrame().GetTerrainStateView().PopulateDatabaseList();
    theApp.GetMainFrame().PopulateDatabaseCombo();
}

void CDataManager::DeleteAllViews(void)
{
    CDebugTrace trace("CDataManager::DeleteAllViews()");

    POSITION pos = m_ViewMap.GetStartPosition();

    while (pos) {
        int id = 0;
        CCigiView *view = NULL;

        m_ViewMap.GetNextAssoc(pos, id, view);

        if (view) {
            // Remove the view from the map.
            m_ViewMap.RemoveKey(id);

            // Delete the object.  The destructor will send a MESSAGE_DEL_VIEW
            // message to the driver.
            delete view;
        }
    }
}

void CDataManager::DeleteAllViewGroups(void)
{
    CDebugTrace trace("CDataManager::DeleteAllViewGroups()");

    for (int i = 0; i < VIEWGROUP_ARRAY_SIZE; i++) {
        if (m_ViewGroupArray[i]) {
            delete m_ViewGroupArray[i];
            m_ViewGroupArray[i] = NULL;
        }
    }
}

void CDataManager::DeleteAllDatabases(void)
{
    CDebugTrace trace("CDataManager::DeleteAllDatabases()");

    for (int i = 0; i < DATABASE_ARRAY_SIZE; i++) {
        delete m_DatabaseArray[i];
        m_DatabaseArray[i] = NULL;
    }
}

CGlobalEnv *CDataManager::GetGlobalEnv(void)
{
    CDebugTrace trace("CDataManager::GetGlobalEnv()");

    return &m_GlobalEnv;
}

void CDataManager::ApplyViewConfiguration(void)
{
    CDebugTrace trace("CDataManager::ApplyViewConfiguration()");

    // Create the view groups first.
    DeleteAllViewGroups();
    CreateViewGroupsFromTemplates();

    // Then create the views.  If a view is assigned to a non-existant
    // view group, then a new group will be created with default values.
    DeleteAllViews();
    CreateViewsFromTemplates();

    // Select the first item in the tree.
    theApp.GetMainFrame().GetViewTree().SelectItem(
        theApp.GetMainFrame().GetViewTree().GetFirstVisibleItem());
}

void CDataManager::ApplyTerrainConfiguration(void)
{
    CDebugTrace trace("CDataManager::ApplyTerrainConfiguration()");

    DeleteAllDatabases();
    CreateDatabasesFromTemplates();

    // Set the current database.
    SetSelectedDatabase(::GetDefaultDB());

    // Then select the item in the main toolbar.
    theApp.GetMainFrame().UpdateDatabaseCombo();
}

void CDataManager::ApplyEnvConfiguration(void)
{
    CDebugTrace trace("CDataManager::ApplyEnvConfiguration()");

    // Remove all non-entity items from the Sky and Atmosphere tree node.
    ClearEnvSkyNodes();

    // Copy the configuration and default values from the template.
    // This will delete any existing components, waves, surface
    // conditions, and weather layers.
    m_GlobalEnv = m_GlobalEnvTempl;

    // Create a tree item for each global weather layer.
    CreateGlobalWeatherLayerTreeItems(g_htreeSky, m_GlobalEnv.GetWeatherLayerMap());

    // Since our environment property pages aren't updated by the timer,
    // force a refresh here.
    theApp.GetMainFrame().GetObjectStateView().RefreshAllViews();
}

void CDataManager::ClearEnvSkyNodes(void)
{
    CDebugTrace trace("CDataManager::ClearEnvSkyNodes()");

    HTREEITEM hitem = theApp.GetMainFrame().GetEnvTree().GetChildItem(g_htreeSky);

    while (hitem) {
        HTREEITEM temp = hitem;
        hitem = theApp.GetMainFrame().GetEnvTree().GetNextSiblingItem(hitem);

        if (MAKE_TYPE(theApp.GetMainFrame().GetEnvTree().GetItemData(temp))
            != OBJECT_TYPE_ENTITY) {
            theApp.GetMainFrame().GetEnvTree().DeleteItem(temp);
        }
    }
}

void CDataManager::RebuildEntityTree(void)
{
    CDebugTrace trace("CDataManager::RebuildEntityTree()");

    CTreeCtrl &tree = GetEntityTree();

    // First, clear the tree and situational view.
    tree.DeleteAllItems();
    CSituationDispCtrl &sitctrl = theApp.GetMainFrame().GetSituationCtrl();
    sitctrl.RemoveAllEntities();

    // Then go through the entities list and add a new node for each item.
    POSITION pos = m_EntityMap.GetStartPosition();

    while (pos) {
        int id = 0;
        CEntity *entity = NULL;

        m_EntityMap.GetNextAssoc(pos, id, entity);

        if (entity) {
            // Create an item in the tree.
            HTREEITEM htree = theApp.GetMainFrame().
                              AddItemToEntityTree(entity->GetName(), entity->GetClass(),
                                                  MAKE_TREE_DATA(entity->GetID(),
                                                                 OBJECT_TYPE_ENTITY,
                                                                 entity->GetClass()),
                                                  TVI_ROOT,
                                                  ((entity->GetID()) == 0 ? TRUE : FALSE));
            entity->SetHtree(htree);


            // Create an item in the situational display control.
            double x, y;
            DOF dof = entity->GetAndLockDofs();
            entity->Unlock();
            GDCtoTM(dof.latitude, dof.longitude, &y, &x);
            POSITION shdl = sitctrl.AddEntity(x, y, dof.yaw,
                                              EntityClassColor[entity->GetClass()],
                                              (LPCTSTR)(entity->GetName()));

            // Update the new situational display item to make it visible.
            sitctrl.SetEntity(shdl, y, x, dof.yaw, TRUE);

            entity->SetSituationHandle(shdl);
        }
    }

    // Finally, reorganize the tree.
    ReorgEntityTree();

    // And select the first item if the file actually contains entities.
    HTREEITEM first = tree.GetFirstVisibleItem();
    if (first)
        tree.SelectItem(first);
    theApp.GetMainFrame().GetEntityStateView().RefreshView();
}

void CDataManager::RebuildEnvTree(void)
{
    CDebugTrace trace("CDataManager::RebuildEnvTree()");

    CTreeCtrl &tree = GetEnvTree();

    // First, clear the tree.
    ClearEnvSkyNodes();

    // Then add a new node for each global weather layer.
    POSITION pos = m_GlobalEnv.GetWeatherLayerMap().GetStartPosition();

    while (pos) {
        int id = 0;
        CWeather *layer = NULL;

        m_GlobalEnv.GetWeatherLayerMap().GetNextAssoc(pos, id, layer);

        if (layer) {
            CString name;
            name.Format("Global Weather Layer %d: \"", layer->GetLayerID());
            name += layer->GetName() += "\"";

            HTREEITEM htree = theApp.GetMainFrame().AddItemToEnvTree((LPCTSTR)name,
                                                                     IMG_WEATHERLAYER,
                                                                     MAKE_TREE_DATA(layer->GetLayerID(),
                                                                                    OBJECT_TYPE_WEATHER,
                                                                                    0),
                                                                     g_htreeSky);

            layer->SetHtree(htree);
        }
    }

    // Go through the entities list and add a new node for each weather entity.
    pos = m_EntityMap.GetStartPosition();

    while (pos) {
        int id = 0;
        CEntity *entity = NULL;

        m_EntityMap.GetNextAssoc(pos, id, entity);

        if (entity && (entity->GetClass() == ENTITY_CLASS_CLOUD)) {
            CString temp_name = "Entity " + entity->GetName();
            HTREEITEM htree = theApp.GetMainFrame().
                              AddItemToEnvTree((LPCTSTR)temp_name, IMG_WEATHER_ENTITY,
                                               MAKE_TREE_DATA(entity->GetID(),
                                                              OBJECT_TYPE_ENTITY,
                                                              entity->GetClass()),
                                               g_htreeSky);
            entity->SetHtreeEnv(htree);
        }
    }

    // And select the first item.
    tree.SelectItem(tree.GetFirstVisibleItem());
    theApp.GetMainFrame().GetActiveStateView()->RefreshView();
}

void CDataManager::RebuildViewTree(void)
{
    CDebugTrace trace("CDataManager::RebuildViewTree()");

    CTreeCtrl &tree = GetViewTree();

    // First, clear the tree.
    tree.DeleteAllItems();

    // Then go through the view list and add a new node for each item.
    POSITION pos = m_ViewMap.GetStartPosition();

    while (pos) {
        int id = 0;
        CCigiView *view = NULL;

        m_ViewMap.GetNextAssoc(pos, id, view);

        if (view) {
            HTREEITEM htree = theApp.GetMainFrame().
                              AddItemToViewTree(view->GetName(), IMG_VIEW,
                                                MAKE_TREE_DATA(view->GetViewID(),
                                                               OBJECT_TYPE_VIEW, 0));
            view->SetHtree(htree);
        }
    }

    // And go through the view group array and do the same thing.
    for (int i = 0; i < VIEWGROUP_ARRAY_SIZE; i++) {
        CViewGroup *group = m_ViewGroupArray[i];

        if (group) {
            HTREEITEM htree = theApp.GetMainFrame().
                              AddItemToViewTree(group->GetName(),
                                                IMG_VIEWGROUP,
                                                MAKE_TREE_DATA(group->GetGroupID(),
                                                               OBJECT_TYPE_VIEWGROUP, 0));
            group->SetHtree(htree);
        }
    }

    // Finally, reorganize the tree.
    ReorgViewTree();

    // And select the first item.
    tree.SelectItem(tree.GetFirstVisibleItem());
    theApp.GetMainFrame().GetViewStateView().RefreshView();
}

void CDataManager::ReorgEntityTree(void)
{
    CDebugTrace trace("CDataManager::ReorgEntityTree()");

    POSITION pos = m_EntityMap.GetStartPosition();

    // Go through the entities list and set the parent for each node.
    while (pos) {
        int id = 0;
        CEntity *entity = NULL;

        m_EntityMap.GetNextAssoc(pos, id, entity);

        if (entity) {
            int parent_id = entity->GetParent();
            if (parent_id > -1) {
                CEntity *parent = GetEntity(parent_id);
                if (parent) {
                    HTREEITEM htree = theApp.GetMainFrame().GetEntityTreeView().
                                      MoveTreeItem(entity->GetHtree(), parent->GetHtree());

                    entity->SetHtree(htree);

                    // If the entity is a child, hide it from the situation view.
                    POSITION situationHandle = entity->GetSituationHandle();
                    if (situationHandle)
                        theApp.GetMainFrame().GetSituationCtrl().SetEntityVisible(situationHandle, FALSE);
                }
            } else {
                // If the entity is not a child, show it from the situation view.
                POSITION situationHandle = entity->GetSituationHandle();
                if (situationHandle)
                    theApp.GetMainFrame().GetSituationCtrl().SetEntityVisible(situationHandle, TRUE);
            }
        }
    }

    theApp.GetMainFrame().GetEntityTreeView().RedrawWindow();
}

void CDataManager::ReorgViewTree(void)
{
    CDebugTrace trace("CDataManager::ReorgViewTree()");

    POSITION pos = m_ViewMap.GetStartPosition();

    // Go through the view array and set the group for each one.
    while (pos) {
        int id = -1;
        CCigiView *view = NULL;

        m_ViewMap.GetNextAssoc(pos, id, view);

        if (view) {
            CViewGroup *group = m_ViewGroupArray[view->GetGroupID()];
            if (group) {
                HTREEITEM htree = theApp.GetMainFrame().GetViewTreeView().
                                  MoveTreeItem(view->GetHtree(), group->GetHtree());

                view->SetHtree(htree);
            }
        }
    }

    theApp.GetMainFrame().GetViewTreeView().RedrawWindow();
}

void CDataManager::CreateGlobalWeatherLayerTreeItems(HTREEITEM hparent, CWeatherMap &map)
{
    CDebugTrace trace("CDataManager::CreateGlobalWeatherLayerTreeItems(HTREEITEM, CWeatherMap &)");

    POSITION pos = map.GetStartPosition();

    while (pos) {
        int id = 0;
        CWeather *weather = NULL;

        map.GetNextAssoc(pos, id, weather);

        if (weather) {
            CString name;
            HTREEITEM hitem = NULL;

            name.Format("Global Weather Layer %d: \"", weather->GetLayerID());
            name += weather->GetName() += "\"";

            hitem = theApp.GetMainFrame().AddItemToEnvTree((LPCTSTR)name,
                                                           IMG_WEATHERLAYER,
                                                           MAKE_TREE_DATA(weather->GetLayerID(), OBJECT_TYPE_WEATHER, 0),
                                                           hparent);
            weather->SetHtree(hitem);
        }
    }
}

void CDataManager::LoadEntityTemplates(CStdioFile &file)
{
    CDebugTrace trace("CDataManager::LoadEntityTemplates(CStdioFile &)");
    m_EntityTemplMap.LoadTemplates(file);
}

void CDataManager::LoadViewTemplates(CStdioFile &file)
{
    CDebugTrace trace("CDataManager::LoadViewTemplates(CStdioFile &)");
    m_ViewTemplMap.LoadTemplates(file);
}

void CDataManager::LoadViewGroupTemplates(CStdioFile &file)
{
    CDebugTrace trace("CDataManager::LoadViewGroupTemplates(CStdioFile &)");
    m_ViewGroupTemplMap.LoadTemplates(file);
}

void CDataManager::LoadDatabaseTemplates(CStdioFile &file)
{
    CDebugTrace trace("CDataManager::LoadDatabaseTemplates(CStdioFile &)");
    m_DatabaseTemplMap.LoadTemplates(file);
}

void CDataManager::LoadEnvTemplates(CStdioFile &file)
{
    CDebugTrace trace("CDataManager::LoadEnvTemplates(CStdioFile &)");
    m_EnvTemplMap.LoadTemplates(file);
}

void CDataManager::LoadSystemTemplates(CStdioFile &file)
{
    CDebugTrace trace("CDataManager::LoadSystemTemplates(CStdioFile &)");
    m_SystemComponents.LoadTemplates(file);
}

void CDataManager::WriteEntityTemplates(CStdioFile &file)
{
    CDebugTrace trace("CDataManager::WriteEntityTemplates(CStdioFile &)");
    m_EntityTemplMap.WriteTemplates(file);
}

void CDataManager::WriteViewTemplates(CStdioFile &file)
{
    CDebugTrace trace("CDataManager::WriteViewTemplates(CStdioFile &)");
    m_ViewTemplMap.WriteTemplates(file);
}

void CDataManager::WriteViewGroupTemplates(CStdioFile &file)
{
    CDebugTrace trace("CDataManager::WriteViewGroupTemplates(CStdioFile &)");
    m_ViewGroupTemplMap.WriteTemplates(file);
}

void CDataManager::WriteDatabaseTemplates(CStdioFile &file)
{
    CDebugTrace trace("CDataManager::WriteDatabaseTemplates(CStdioFile &)");
    m_DatabaseTemplMap.WriteTemplates(file);
}

void CDataManager::WriteEnvTemplates(CStdioFile &file)
{
    CDebugTrace trace("CDataManager::WriteEnvTemplates(CStdioFile &)");
    m_GlobalEnvTempl.WriteText(file);
}

void CDataManager::DisableWaypointFlight(void)
{
    CDebugTrace trace("CDataManager::DisableWaypointFlight()");

    int entity_id = NULL;
    CEntity *entity = NULL;
    POSITION pos = m_EntityMap.GetStartPosition();
    while (pos) {
        m_EntityMap.GetNextAssoc(pos, entity_id, entity);

        entity->SetFlyWaypoints(FALSE);
    }
}

CWaypoint *CDataManager::GetWaypoint(const int id)
{
    CDebugTrace trace("CDataManager::GetWaypoint(const int)");

    CWaypoint *waypoint = NULL;
    if (m_WaypointMap.Lookup(id, waypoint))
        return waypoint;
    else
        return NULL;
}

void CDataManager::ClearWaypoints(void)
{
    CDebugTrace trace("CDataManager::ClearWaypoints()");

    int id = 0;
    CWaypoint *wp = NULL;

    POSITION pos = m_WaypointMap.GetStartPosition();
    while (pos) {
        m_WaypointMap.GetNextAssoc(pos, id, wp);
        delete wp;
    }

    m_WaypointMap.RemoveAll();
}

void CDataManager::ResetScenario(void)
{
    CDebugTrace trace("CDataManager::ResetScenario()");

    ResetEntities();

    // I'm not sure if we want to do anything else. If the user
    // wants to reset the views, view groups, or environment,
    // he/she can reload the scenario file from disk.
}

void CDataManager::ResetEntities(void)
{
    CDebugTrace trace("CDataManager::ResetEntities()");

    POSITION pos = m_EntityMap.GetStartPosition();
    int id = 0;
    CEntity *entity = NULL;

    while (pos) {
        m_EntityMap.GetNextAssoc(pos, id, entity);
        if (entity) {
            ENTITY state = entity->GetAndLock();
            state.cigi = entity->GetInitialCigi();
            state.noncigi = entity->GetInitialNonCigi();
			state.unsaved.entityposition_has_changed = 1;
            entity->SetLocked(state);
            entity->Unlock();
        }
    }

    // Sleep briefly so the parent will get set correctly from the
    // initial state.
    Sleep(250);

    // Finally, reorganize the tree.
    ReorgEntityTree();

    // And re-select the previously selected item.
    CEntity *sel_entity = GetSelectedEntity();
    if (sel_entity)
        theApp.GetMainFrame().GetEntityTreeView().SelectEntityByID(sel_entity->GetID());
    theApp.GetMainFrame().GetEntityStateView().RefreshView();
}

void CDataManager::Reinitialize(void)
{
    CDebugTrace trace("CDataManager::Reinitialize()");

    m_SearchEntity = NULL;

    // Delete the entity, view, and mission function objects.
    DeleteAllEntities();
    RemoveHatLookupItems();
    RemoveLosLookupItems();
    DeleteAllViews();
    DeleteAllViewGroups();
    DeleteAllDatabases();

    // If we are just launching the application, the trees will not be
    // initialized.  In this case, we will do the initialization later
    // in CHemuApp::InitInstance().
    if (theApp.GetMainFrame().GetSafeHwnd()) {
        // Clear the situational display.
        theApp.GetMainFrame().GetSituationCtrl().RemoveAllEntities();

        // Clear the trees and reload the configuration.
        theApp.GetMainFrame().GetEntityTree().DeleteAllItems();
        theApp.GetMainFrame().GetViewTree().DeleteAllItems();
        theApp.GetMainFrame().GetEnvTree().DeleteAllItems();
        theApp.InitializeTrees();

        ApplyViewConfiguration();
        ApplyTerrainConfiguration();
        ApplyEnvConfiguration();
    }
}

void CDataManager::ClearSystemComponents(void)
{
    CDebugTrace trace("CDataManager::ClearSystemComponents()");

    POSITION pos = m_SystemComponents.GetStartPosition();
    int id = 0;
    CComponent *comp = NULL;

    while (pos) {
        m_SystemComponents.GetNextAssoc(pos, id, comp);
        delete comp;
    }

    m_SystemComponents.RemoveAll();
}

void CDataManager::ClearEvents(void)
{
    CDebugTrace trace("CDataManager::ClearEvents()");

    POSITION pos = m_Events.GetStartPosition();
    int id = 0;
    CCigiEvent *event = NULL;

    while (pos) {
        m_Events.GetNextAssoc(pos, id, event);
        delete event;
    }

    m_Events.RemoveAll();
}

template <> void AFXAPI SerializeElements<CWaypoint *>(CArchive &ar, CWaypoint **pElements, int nCount)
{
    CDebugTrace trace("SerializeElements<CWaypoint *>(CArchive &, CWaypoint **, int)");

    if (ar.IsStoring()) {
        for (int i = 0; i < nCount; i++)
            if (pElements[i])
                pElements[i]->Serialize(ar);
    } else {
        for (int i = 0; i < nCount; i++) {
            pElements[i] = new CWaypoint;
            pElements[i]->Serialize(ar);
        }
    }
}

BOOL CDataManager::DeleteEntity(const int id)
{
    CDebugTrace trace("CDataManager::DeleteEntity(const int)");

    return DeleteEntity(GetEntity(id));
}

CTreeCtrl &CDataManager::GetEntityTree(void)
{
    return GetEntityTreeView().GetTreeCtrl();
}

CTreeCtrl &CDataManager::GetEnvTree(void)
{
    return GetEnvTreeView().GetTreeCtrl();
}

CTreeCtrl &CDataManager::GetViewTree(void)
{
    return GetViewTreeView().GetTreeCtrl();
}

CEntityTreeView &CDataManager::GetEntityTreeView(void)
{
    return ((CMainFrame *)AfxGetMainWnd())->GetEntityTreeView();
}

CEnvTreeView &CDataManager::GetEnvTreeView(void)
{
    return ((CMainFrame *)AfxGetMainWnd())->GetEnvTreeView();
}

CViewTreeView &CDataManager::GetViewTreeView(void)
{
    return ((CMainFrame *)AfxGetMainWnd())->GetViewTreeView();
}

CMessageView &CDataManager::GetMessageView(void)
{
    return ((CMainFrame *)AfxGetMainWnd())->GetMessageView();
}

CPacketWatchView &CDataManager::GetWatchView(void)
{
    return ((CMainFrame *)AfxGetMainWnd())->GetWatchView();
}

int CDataManager::GetNextEntityID(void)
{
    CDebugTrace trace("CDataManager::GetNextEntityID()");

    return m_EntityIDManager.GetNextID();
}

void CDataManager::MarkEntityID(const int id)
{
    CDebugTrace trace("CDataManager::MarkEntityID(const int)");

    m_EntityIDManager.MarkID(id);
}

void CDataManager::UnmarkEntityID(const int id)
{
    CDebugTrace trace("CDataManager::UnmarkEntityID(const int)");

    m_EntityIDManager.UnmarkID(id);
}

CEntity *CDataManager::GetEntity(const int id)
{
    if (m_EntityMap.Lookup(id, m_SearchEntity) == FALSE)
        m_SearchEntity = NULL;   // if case id is invalid

    return m_SearchEntity;
}

CCigiView *CDataManager::GetView(const int id)
{
    CDebugTrace trace("CDataManager::GetView(const int)");

    CCigiView *view = NULL;
    m_ViewMap.Lookup(id, view);

    return view;
}

CViewGroup *CDataManager::GetViewGroup(const unsigned char id)
{
    CDebugTrace trace("CDataManager::GetViewGroup(const unsigned int)");

    return (id < VIEWGROUP_ARRAY_SIZE) ? m_ViewGroupArray[id] : NULL;
}

CTerrainDB *CDataManager::GetDatabase(const unsigned char id)
{
    CDebugTrace trace("CDataManager::GetDatabase(const unsigned int)");

    return (id < DATABASE_ARRAY_SIZE) ? m_DatabaseArray[id] : NULL;
}

unsigned char CDataManager::GetSelectedDatabaseNum(void)
{
    CDebugTrace trace("CDataManager::GetSelectedDatabaseNum()");

    return m_CurrentDatabaseID;
}

void CDataManager::AddHatLookup(const int id, CHat *hat)
{
    CDebugTrace trace("CDataManager::AddHatLookup(const int, CHat *)");

    m_HatLookupMap.SetAt(id, hat);
}

BOOL CDataManager::RemoveHatLookup(const int id)
{
    CDebugTrace trace("CDataManager::RemoveHatLookup(const int)");

    return m_HatLookupMap.RemoveKey(id) ? TRUE : FALSE;
}

void CDataManager::RemoveHatLookupItems(void)
{
    CDebugTrace trace("CDataManager::RemoveHatLookupItems()");

    m_HatLookupMap.RemoveAll();
}

CHat *CDataManager::LookupHat(const int id)
{
    CDebugTrace trace("CDataManager::LookupHat(const int)");

    CHat *hat = NULL;
    m_HatLookupMap.Lookup(id, hat);

    return hat;
}

void CDataManager::AddLosLookup(const int id, CLos *los)
{
    CDebugTrace trace("CDataManager::AddLosLookup(const int, CLos *)");

    m_LosLookupMap.SetAt(id, los);
}

BOOL CDataManager::RemoveLosLookup(const int id)
{
    CDebugTrace trace("CDataManager::RemoveLosLookup(const int)");

    return m_LosLookupMap.RemoveKey(id) ? TRUE : FALSE;
}

void CDataManager::RemoveLosLookupItems(void)
{
    CDebugTrace trace("CDataManager::RemoveLosLookupItems)");

    m_LosLookupMap.RemoveAll();
}

CLos *CDataManager::LookupLos(const int id)
{
    CDebugTrace trace("CDataManager::LookupLos(const int)");

    CLos *los = NULL;
    m_LosLookupMap.Lookup(id, los);

    return los;
}

CEntity *CDataManager::GetSelectedEntity(void)
{
    HTREEITEM htree = GetEntityTree().GetSelectedItem();

    return GetEntity(htree);
}

CCigiView *CDataManager::GetSelectedView(void)
{
    CDebugTrace trace("CDataManager::GetSelectedView()");

    HTREEITEM htree = GetViewTree().GetSelectedItem();

    return GetView(htree);  // Will return NULL if not a view.
}

CViewGroup *CDataManager::GetSelectedViewGroup(void)
{
    CDebugTrace trace("CDataManager::GetSelectedViewGroup()");

    HTREEITEM htree = GetViewTree().GetSelectedItem();

    return GetViewGroup(htree); // Will return NULL if not a view group.
}

CComponent *CDataManager::GetSystemComponent(uint16_t component_id, uint16_t instance_id)
{
    CDebugTrace trace("CDataManager::GetSystemComponent()");

    const uint32_t key = MakeKey(component_id, instance_id);
    CComponent *pComp = NULL;

    return m_SystemComponents.Lookup(key, pComp) ? pComp : NULL;
}

CTerrainDB *CDataManager::GetSelectedDatabase(void)
{
    CDebugTrace trace("CDataManager::GetSelectedDatabase()");

    return GetDatabase(m_CurrentDatabaseID);
}

CWeather *CDataManager::GetWeatherLayer(const int id)
{
    CDebugTrace trace("CDataManager::GetWeatherLayer(const int)");

    return m_GlobalEnv.GetWeatherLayer(id);
}

CWeather *CDataManager::GetWeatherLayer(const HTREEITEM htree)
{
    CDebugTrace trace("CDataManager::GetWeatherLayer(const HTREEITEM)");

    return m_GlobalEnv.GetWeatherLayer(htree);
}

CWeather *CDataManager::GetSelectedWeatherLayer(void)
{
    CDebugTrace trace("CDataManager::GetSelectedWeatherLayer()");

    HTREEITEM htree = GetEnvTree().GetSelectedItem();

    return m_GlobalEnv.GetWeatherLayer(htree);
}

CEntityMap &CDataManager::GetEntityMap(void)
{
    return m_EntityMap;
}

CViewMap &CDataManager::GetViewMap(void)
{
    return m_ViewMap;
}

CViewGroup **CDataManager::GetViewGroupArray(void)
{
    return &m_ViewGroupArray[0];
}

CEntityTemplMap &CDataManager::GetEntityTemplMap(void)
{
    return m_EntityTemplMap;
}

CViewTemplMap &CDataManager::GetViewTemplMap(void)
{
    return m_ViewTemplMap;
}

CViewGroupTemplMap &CDataManager::GetViewGroupTemplMap(void)
{
    return m_ViewGroupTemplMap;
}

CDatabaseTemplMap &CDataManager::GetDatabaseTemplMap(void)
{
    return m_DatabaseTemplMap;
}

void CDataManager::SetEntityTempl(TEMPL_ENTITY *templ)
{
    CDebugTrace trace("CDataManager::SetEntityTempl(TEMPL_ENTITY *)");

    m_EntityTemplMap.SetAt(templ->Type, templ);
}

void CDataManager::SetViewTempl(TEMPL_VIEW *templ)
{
    CDebugTrace trace("CDataManager::SetViewTempl(TEMPL_VIEW *)");

    m_ViewTemplMap.SetAt(templ->ViewID, templ);
}

void CDataManager::SetViewGroupTempl(TEMPL_VIEWGROUP *templ)
{
    CDebugTrace trace("CDataManager::SetViewGroupTempl(TEMPL_VIEWGROUP *)");

    m_ViewGroupTemplMap.SetAt(templ->GroupID, templ);
}

void CDataManager::SetDatabaseTempl(TEMPL_DATABASE *templ)
{
    CDebugTrace trace("CDataManager::SetDatabaseTempl(TEMPL_DATABASE *)");

    m_DatabaseTemplMap.SetAt(templ->DatabaseID, templ);
}

void CDataManager::SetGlobalEnvTempl(TEMPL_GLOBAL_ENV *templ)
{
    CDebugTrace trace("CDataManager::SetGlobalEnvTempl(TEMPL_GLOBAL_ENV *)");

    m_GlobalEnvTempl = *templ;
}

BOOL CDataManager::RemoveEntityTempl(const int type)
{
    CDebugTrace trace("CDataManager::RemoveEntityTempl(const int)");

    return m_EntityTemplMap.RemoveKey(type);
}

BOOL CDataManager::RemoveViewTempl(const int id)
{
    CDebugTrace trace("CDataManager::RemoveViewTempl(const int)");

    return m_ViewTemplMap.RemoveKey(id);
}

BOOL CDataManager::RemoveViewGroupTempl(const int id)
{
    CDebugTrace trace("CDataManager::RemoveViewGroupTempl(const int)");

    return m_ViewGroupTemplMap.RemoveKey(id);
}

BOOL CDataManager::RemoveDatabaseTempl(const int id)
{
    CDebugTrace trace("CDataManager::RemoveDatabaseTempl(const int)");

    return m_DatabaseTemplMap.RemoveKey(id);
}

void CDataManager::AddWaypoint(CWaypoint *waypoint)
{
    CDebugTrace trace("CDataManager::AddWaypoint(CWaypoint *)");

    if (waypoint)
        m_WaypointMap[waypoint->GetID()] = waypoint;
}

void CDataManager::SetTranspDelayTestPoint(int id, DOF *dof)
{
    if (dof)
        m_TranspDelayTestPoints.SetAt(id, *dof);
}

BOOL CDataManager::GetTranspDelayTestPoint(int id, DOF *dof)
{
    return m_TranspDelayTestPoints.Lookup(id, *dof);
}

void CDataManager::ClearTranspDelayTestPoints(void)
{
    m_TranspDelayTestPoints.RemoveAll();
}

CComponent *CDataManager::GetTerrainComponent(unsigned int component_id, unsigned int instance_id)
{
    CComponent *comp = NULL;
    CTerrainDB *db = GetSelectedDatabase();

    comp = (db ? db->GetComponent(component_id, instance_id) : NULL);

    if (!comp) {
        CGlobalEnv *pEnv = GetGlobalEnv();
        comp = (pEnv ? pEnv->GetTerrainComponent(component_id, instance_id) : NULL);
    }

    return comp;
}
