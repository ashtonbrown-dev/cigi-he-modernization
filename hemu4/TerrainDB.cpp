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

// TerrainDB.cpp: implementation of the CTerrainDB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "common.h"
#include "globals.h"
#include "TerrainDB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CTerrainDB, CObject, 0)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTerrainDB::CTerrainDB()
{
    m_ErmName = "WGS84";
    m_ErmRadius = TERRAIN_DEFAULT_RADIUS;
    m_ErmFlattening = TERRAIN_DEFAULT_FLATTENING;
}

CTerrainDB::CTerrainDB(const TEMPL_DATABASE *templ)
{
    // Initialize the CMap.
    m_Components.RemoveAll();
    m_Components.InitHashTable(521);

    // Assign the ID here because ApplyTemplate() checks.
    m_DatabaseID = templ->DatabaseID;

    ApplyTemplate(templ);
}

CTerrainDB::~CTerrainDB()
{
    ClearComponents();
}

CComponent *CTerrainDB::GetComponent(int compId, int instId)
{
    CComponent *search = NULL;

    const int key = MakeKey(compId, instId);
    m_Components.Lookup(key, search);

    return search;
}

void CTerrainDB::Serialize(CArchive &ar)
{
    TRACE0("\nCTerrainDB::Serialize()\n");

    int length = 0;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)m_Name);
        ar << length;
        ar.Write((LPCTSTR)m_Name, length);  // NULL terminator not copied

        // Store the ERM's name.
        length = strlen((LPCTSTR)m_ErmName);
        ar << length;
        ar.Write((LPCTSTR)m_ErmName, length);   // NULL terminator not copied

        ar << m_DatabaseID;
        ar << m_OriginLat;
        ar << m_OriginLon;
        ar << m_OriginAlt;
        ar << m_DefaultLat;
        ar << m_DefaultLon;
        ar << m_DefaultAlt;
        ar << m_ErmRadius;
        ar << m_ErmFlattening;

        // Store the components.
        m_Components.Serialize(ar);

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("	length = %d\n", length);
        TRACE1("	Name = \"%s\"\n", (LPCTSTR)m_Name);
    } else {
        // Read the name.
        ar >> length;
        ar.Read(m_Name.GetBuffer(length), length);
        m_Name.ReleaseBuffer(length);

        // Read the REM's name.
        ar >> length;
        ar.Read(m_ErmName.GetBuffer(length), length);
        m_ErmName.ReleaseBuffer(length);

        ar >> m_DatabaseID;
        ar >> m_OriginLat;
        ar >> m_OriginLon;
        ar >> m_OriginAlt;
        ar >> m_DefaultLat;
        ar >> m_DefaultLon;
        ar >> m_DefaultAlt;
        ar >> m_ErmRadius;
        ar >> m_ErmFlattening;

        // Read the components.
        m_Components.Serialize(ar);

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("	length = %d\n", length);
        TRACE1("	Name = \"%s\"\n", (LPCTSTR)m_Name);
    }
}

BOOL CTerrainDB::AddComponent(CComponent *comp)
{
    BOOL retval = FALSE;
    const int key = MakeKey(comp->GetID(), comp->GetInstanceID());
    CComponent *search = NULL;

    // If the component already exists, delete it.
    if (m_Components.Lookup(key, search)) {
        //LWD: we need to preserve the states and revert to default if they don't match

        delete search;
        retval = TRUE;      // return TRUE if component exists
    }

    m_Components.SetAt(key, comp);

    return retval;
}

BOOL CTerrainDB::DeleteComponent(int compId, int instId)
{
    CComponent *search = NULL;

    const int key = MakeKey(compId, instId);
    if (m_Components.Lookup(key, search)) {
        if (search)
            delete search;

        m_Components.RemoveKey(key);

        return TRUE;
    }

    return FALSE;
}

void CTerrainDB::ClearComponents(void)
{
    int key = 0;
    CComponent *comp = NULL;
    POSITION pos = m_Components.GetStartPosition();
    while (pos) {
        m_Components.GetNextAssoc(pos, key, comp);
        delete comp;
    }

    m_Components.RemoveAll();
}

BOOL CTerrainDB::ApplyTemplate(const TEMPL_DATABASE *templ)
{
    if (templ->DatabaseID != m_DatabaseID)
        return FALSE;

    // Set the name.
    if (templ->Name == "")
        m_Name.Format("%d", m_DatabaseID);
    else
        m_Name.Format("%d: %s", m_DatabaseID, templ->Name);

    // Set all the initial properties from the template.
    m_ErmName = templ->ErmName;
    m_OriginLat = templ->OriginLat;
    m_OriginLon = templ->OriginLon;
    m_OriginAlt = templ->OriginAlt;
    m_DefaultLat = templ->DefaultLat;
    m_DefaultLon = templ->DefaultLon;
    m_DefaultAlt = templ->DefaultAlt;
    m_ErmRadius = templ->ErmRadius;
    m_ErmFlattening = templ->ErmFlattening;

    // Build the component list.
    unsigned long handle = 0;
    TEMPL_COMPONENT *comp_templ = templ->ComponentList.GetHead(&handle);
    while (comp_templ) {
        CComponent *comp = new CComponent(COMP_CLASS_G_TERRAIN, comp_templ);

        // The user specifies the instance ID in the .def file, so
        // don't do this: comp->SetInstanceID(GetID());

        // Set the database ID of the component.
        comp->SetDatabaseID(m_DatabaseID);

        AddComponent(comp);

        comp_templ = templ->ComponentList.GetNext(&handle);
    }

    return TRUE;
}
