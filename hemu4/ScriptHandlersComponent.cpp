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

#include "cigi4types.h"
#include "Entity.h"
#include "script.h"
#include "globals.h"
#include "hemu4.h"
#include "hemumsg.h"
#include "MainFrm.h"
#include "ViewGroup.h"
#include "TerrainDB.h"
#include "CigiView.h"

extern CHemuApp theApp;

long ScriptDoComponentState(const char *buffer, const unsigned int linenum)
{
    char new_str[256] = "";
    char keyword[64] = "";
    unsigned long  component_class = -1;
    unsigned long instance_id = -1;
    unsigned long component_id = -1;
    unsigned long  state = -1;

    sscanf(buffer,
           "%s %lu %lu %lu %lu",
           keyword, &component_class, &instance_id, &component_id, &state);

    // Create a similar string but without the component class.
    sprintf(new_str,
            "%s %lu %lu %lu",
            keyword, instance_id, component_id, state);

    switch (component_class) {
    case 0:     // Entity
        return ScriptDoEntityComponentState(new_str, linenum);

    case 1:     // View
        return ScriptDoViewComponentState(new_str, linenum);

    case 2:     // View Group
        return ScriptDoViewGroupComponentState(new_str, linenum);

    case 3:     // Sensor
        return ScriptDoSensorComponentState(new_str, linenum);

    case 4:     // Regional Sea Surface
        return ScriptDoRSeaComponentState(new_str, linenum);

    case 5:     // Regional Terrain Surface
        return ScriptDoRTerrainComponentState(new_str, linenum);

    case 6:     // Regional Weather Layer
        return ScriptDoRLayerComponentState(new_str, linenum);

    case 7:     // Global Sea Surface
        return ScriptDoGSeaComponentState(new_str, linenum);

    case 8:     // Global Terrain Surface
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str, "%s %lu %lu %lu", keyword, component_id, instance_id, state);

        return ScriptDoGTerrainComponentState(new_str, linenum);

    case 9:     // Global Weather Layer
        return ScriptDoGLayerComponentState(new_str, linenum);

    case 10:    // Atmosphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str, "%s %lu %lu %lu", keyword, component_id, instance_id, state);

        return ScriptDoAtmosphereComponentState(new_str, linenum);

    case 11:    // Celestial Sphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str, "%s %lu %lu %lu", keyword, component_id, instance_id, state);

        return ScriptDoCelSphereComponentState(new_str, linenum);

    case 12:    // Event
        return ScriptDoEventComponentState(new_str, linenum);

    case 13:    // System
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str, "%s %lu %lu %lu", keyword, component_id, instance_id, state);

        return ScriptDoSystemComponentState(new_str, linenum);

    default: {
        CString errmsg;
        errmsg.Format("Error: Invalid component class. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    }

    return 0;
}

long ScriptDoComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    char new_str[256] = "";
    char keyword[64] = "";
    unsigned long component_class = -1;
    unsigned long instance_id = -1;
    unsigned long component_id = -1;
    unsigned long datum_id = -1;
    int32_t datum_value = -1;

    sscanf(buffer,
           "%s %lu %lu %lu %lu %ld",
           keyword, &component_class, &instance_id, &component_id, &datum_id, &datum_value);

    // Create a similar string but without the component class.
    sprintf(new_str,
            "%s %lu %lu %lu %ld",
            keyword, instance_id, component_id, datum_id, datum_value);

    switch (component_class) {
    case 0:     // Entity
        return ScriptDoEntityComponentDatumInt32(new_str, linenum);

    case 1:     // View
        return ScriptDoViewComponentDatumInt32(new_str, linenum);

    case 2:     // View Group
        return ScriptDoViewGroupComponentDatumInt32(new_str, linenum);

    case 3:     // Sensor
        return ScriptDoSensorComponentDatumInt32(new_str, linenum);

    case 4:     // Regional Sea Surface
        return ScriptDoRSeaComponentDatumInt32(new_str, linenum);

    case 5:     // Regional Terrain Surface
        return ScriptDoRTerrainComponentDatumInt32(new_str, linenum);

    case 6:     // Regional Weather Layer
        return ScriptDoRLayerComponentDatumInt32(new_str, linenum);

    case 7:     // Global Sea Surface
        return ScriptDoGSeaComponentDatumInt32(new_str, linenum);

    case 8:     // Global Terrain Surface
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %ld",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoGTerrainComponentDatumInt32(new_str, linenum);

    case 9:     // Global Weather Layer
        return ScriptDoGLayerComponentDatumInt32(new_str, linenum);

    case 10:    // Atmosphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %ld",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoAtmosphereComponentDatumInt32(new_str, linenum);

    case 11:    // Celestial Sphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %ld",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoCelSphereComponentDatumInt32(new_str, linenum);

    case 12:    // Event
        return ScriptDoEventComponentDatumInt32(new_str, linenum);

    case 13:    // System
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %ld",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoSystemComponentDatumInt32(new_str, linenum);

    default: {
        CString errmsg;
        errmsg.Format("Error: Invalid component class. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    }

    return 0;
}

long ScriptDoComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    char new_str[256] = "";
    char keyword[64] = "";
    unsigned long component_class = -1;
    unsigned long instance_id = -1;
    unsigned long component_id = -1;
    unsigned long datum_id = -1;
    uint32_t datum_value = -1;

    sscanf(buffer,
           "%s %lu %lu %lu %lu %lu",
           keyword, &component_class, &instance_id, &component_id, &datum_id, &datum_value);

    // Create a similar string but without the component class.
    sprintf(new_str,
            "%s %lu %lu %lu %lu",
            keyword, instance_id, component_id, datum_id, datum_value);

    switch (component_class) {
    case 0:     // Entity
        return ScriptDoEntityComponentDatumUint32(new_str, linenum);

    case 1:     // View
        return ScriptDoViewComponentDatumUint32(new_str, linenum);

    case 2:     // View Group
        return ScriptDoViewGroupComponentDatumUint32(new_str, linenum);

    case 3:     // Sensor
        return ScriptDoSensorComponentDatumUint32(new_str, linenum);

    case 4:     // Regional Sea Surface
        return ScriptDoRSeaComponentDatumUint32(new_str, linenum);

    case 5:     // Regional Terrain Surface
        return ScriptDoRTerrainComponentDatumUint32(new_str, linenum);

    case 6:     // Regional Weather Layer
        return ScriptDoRLayerComponentDatumUint32(new_str, linenum);

    case 7:     // Global Sea Surface
        return ScriptDoGSeaComponentDatumUint32(new_str, linenum);

    case 8:     // Global Terrain Surface
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %lu",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoGTerrainComponentDatumUint32(new_str, linenum);

    case 9:     // Global Weather Layer
        return ScriptDoGLayerComponentDatumUint32(new_str, linenum);

    case 10:    // Atmosphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %lu",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoAtmosphereComponentDatumUint32(new_str, linenum);

    case 11:    // Celestial Sphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %lu",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoCelSphereComponentDatumUint32(new_str, linenum);

    case 12:    // Event
        return ScriptDoEventComponentDatumUint32(new_str, linenum);

    case 13:    // System
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %lu",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoSystemComponentDatumUint32(new_str, linenum);

    default: {
        CString errmsg;
        errmsg.Format("Error: Invalid component class. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    }

    return 0;
}

long ScriptDoComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    char new_str[256] = "";
    char keyword[64] = "";
    unsigned long component_class = -1;
    unsigned long instance_id = -1;
    unsigned long component_id = -1;
    unsigned long datum_id = -1;
    int64_t datum_value = -1;

    sscanf(buffer,
           "%s %lu %lu %lu %lu %lld",
           keyword, &component_class, &instance_id, &component_id, &datum_id, &datum_value);

    // Create a similar string but without the component class.
    sprintf(new_str,
            "%s %lu %lu %lu %lld",
            keyword, instance_id, component_id, datum_id, datum_value);

    switch (component_class) {
    case 0:     // Entity
        return ScriptDoEntityComponentDatumInt64(new_str, linenum);

    case 1:     // View
        return ScriptDoViewComponentDatumInt64(new_str, linenum);

    case 2:     // View Group
        return ScriptDoViewGroupComponentDatumInt64(new_str, linenum);

    case 3:     // Sensor
        return ScriptDoSensorComponentDatumInt64(new_str, linenum);

    case 4:     // Regional Sea Surface
        return ScriptDoRSeaComponentDatumInt64(new_str, linenum);

    case 5:     // Regional Terrain Surface
        return ScriptDoRTerrainComponentDatumInt64(new_str, linenum);

    case 6:     // Regional Weather Layer
        return ScriptDoRLayerComponentDatumInt64(new_str, linenum);

    case 7:     // Global Sea Surface
        return ScriptDoGSeaComponentDatumInt64(new_str, linenum);

    case 8:     // Global Terrain Surface
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %lld",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoGTerrainComponentDatumInt64(new_str, linenum);

    case 9:     // Global Weather Layer
        return ScriptDoGLayerComponentDatumInt64(new_str, linenum);

    case 10:    // Atmosphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %lld",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoAtmosphereComponentDatumInt64(new_str, linenum);

    case 11:    // Celestial Sphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %lld",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoCelSphereComponentDatumInt64(new_str, linenum);

    case 12:    // Event
        return ScriptDoEventComponentDatumInt64(new_str, linenum);

    case 13:    // System
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %lld",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoSystemComponentDatumInt64(new_str, linenum);

    default: {
        CString errmsg;
        errmsg.Format("Error: Invalid component class. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    }

    return 0;
}

long ScriptDoComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    char new_str[256] = "";
    char keyword[64] = "";
    unsigned long component_class = -1;
    unsigned long instance_id = -1;
    unsigned long component_id = -1;
    unsigned long datum_id = -1;
    uint64_t datum_value = -1;

    sscanf(buffer,
           "%s %lu %lu %lu %lu %llu",
           keyword, &component_class, &instance_id, &component_id, &datum_id, &datum_value);

    // Create a similar string but without the component class.
    sprintf(new_str,
            "%s %lu %lu %lu %llu",
            keyword, instance_id, component_id, datum_id, datum_value);

    switch (component_class) {
    case 0:     // Entity
        return ScriptDoEntityComponentDatumUint64(new_str, linenum);

    case 1:     // View
        return ScriptDoViewComponentDatumUint64(new_str, linenum);

    case 2:     // View Group
        return ScriptDoViewGroupComponentDatumUint64(new_str, linenum);

    case 3:     // Sensor
        return ScriptDoSensorComponentDatumUint64(new_str, linenum);

    case 4:     // Regional Sea Surface
        return ScriptDoRSeaComponentDatumUint64(new_str, linenum);

    case 5:     // Regional Terrain Surface
        return ScriptDoRTerrainComponentDatumUint64(new_str, linenum);

    case 6:     // Regional Weather Layer
        return ScriptDoRLayerComponentDatumUint64(new_str, linenum);

    case 7:     // Global Sea Surface
        return ScriptDoGSeaComponentDatumUint64(new_str, linenum);

    case 8:     // Global Terrain Surface
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %llu",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoGTerrainComponentDatumUint64(new_str, linenum);

    case 9:     // Global Weather Layer
        return ScriptDoGLayerComponentDatumUint64(new_str, linenum);

    case 10:    // Atmosphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %llu",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoAtmosphereComponentDatumUint64(new_str, linenum);

    case 11:    // Celestial Sphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %llu",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoCelSphereComponentDatumUint64(new_str, linenum);

    case 12:    // Event
        return ScriptDoEventComponentDatumUint64(new_str, linenum);

    case 13:    // System
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %llu",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoSystemComponentDatumUint64(new_str, linenum);

    default: {
        CString errmsg;
        errmsg.Format("Error: Invalid component class. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    }

    return 0;
}

long ScriptDoComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    char new_str[256] = "";
    char keyword[64] = "";
    unsigned long component_class = -1;
    unsigned long instance_id = -1;
    unsigned long component_id = -1;
    unsigned long datum_id = -1;
    float datum_value = -1;

    sscanf(buffer,
           "%s %lu %lu %lu %lu %f",
           keyword, &component_class, &instance_id, &component_id, &datum_id, &datum_value);

    // Create a similar string but without the component class.
    sprintf(new_str,
            "%s %lu %lu %lu %f",
            keyword, instance_id, component_id, datum_id, datum_value);

    switch (component_class) {
    case 0:     // Entity
        return ScriptDoEntityComponentDatumFloat(new_str, linenum);

    case 1:     // View
        return ScriptDoViewComponentDatumFloat(new_str, linenum);

    case 2:     // View Group
        return ScriptDoViewGroupComponentDatumFloat(new_str, linenum);

    case 3:     // Sensor
        return ScriptDoSensorComponentDatumFloat(new_str, linenum);

    case 4:     // Regional Sea Surface
        return ScriptDoRSeaComponentDatumFloat(new_str, linenum);

    case 5:     // Regional Terrain Surface
        return ScriptDoRTerrainComponentDatumFloat(new_str, linenum);

    case 6:     // Regional Weather Layer
        return ScriptDoRLayerComponentDatumFloat(new_str, linenum);

    case 7:     // Global Sea Surface
        return ScriptDoGSeaComponentDatumFloat(new_str, linenum);

    case 8:     // Global Terrain Surface
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %f",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoGTerrainComponentDatumFloat(new_str, linenum);

    case 9:     // Global Weather Layer
        return ScriptDoGLayerComponentDatumFloat(new_str, linenum);

    case 10:    // Atmosphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %f",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoAtmosphereComponentDatumFloat(new_str, linenum);

    case 11:    // Celestial Sphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %f",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoCelSphereComponentDatumFloat(new_str, linenum);

    case 12:    // Event
        return ScriptDoEventComponentDatumFloat(new_str, linenum);

    case 13:    // System
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %f",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoSystemComponentDatumFloat(new_str, linenum);

    default: {
        CString errmsg;
        errmsg.Format("Error: Invalid component class. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    }

    return 0;
}

long ScriptDoComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    char new_str[256] = "";
    char keyword[64] = "";
    unsigned long component_class = -1;
    unsigned long instance_id = -1;
    unsigned long component_id = -1;
    unsigned long datum_id = -1;
    double datum_value = -1;

    sscanf(buffer,
           "%s %lu %lu %lu %lu %lf",
           keyword, &component_class, &instance_id, &component_id, &datum_id, &datum_value);

    // Create a similar string but without the component class.
    sprintf(new_str,
            "%s %lu %lu %lu %lf",
            keyword, instance_id, component_id, datum_id, datum_value);

    switch (component_class) {
    case 0:     // Entity
        return ScriptDoEntityComponentDatumDouble(new_str, linenum);

    case 1:     // View
        return ScriptDoViewComponentDatumDouble(new_str, linenum);

    case 2:     // View Group
        return ScriptDoViewGroupComponentDatumDouble(new_str, linenum);

    case 3:     // Sensor
        return ScriptDoSensorComponentDatumDouble(new_str, linenum);

    case 4:     // Regional Sea Surface
        return ScriptDoRSeaComponentDatumDouble(new_str, linenum);

    case 5:     // Regional Terrain Surface
        return ScriptDoRTerrainComponentDatumDouble(new_str, linenum);

    case 6:     // Regional Weather Layer
        return ScriptDoRLayerComponentDatumDouble(new_str, linenum);

    case 7:     // Global Sea Surface
        return ScriptDoGSeaComponentDatumDouble(new_str, linenum);

    case 8:     // Global Terrain Surface
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %lf",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoGTerrainComponentDatumDouble(new_str, linenum);

    case 9:     // Global Weather Layer
        return ScriptDoGLayerComponentDatumDouble(new_str, linenum);

    case 10:    // Atmosphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %lf",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoAtmosphereComponentDatumDouble(new_str, linenum);

    case 11:    // Celestial Sphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %lf",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoCelSphereComponentDatumDouble(new_str, linenum);

    case 12:    // Event
        return ScriptDoEventComponentDatumDouble(new_str, linenum);

    case 13:    // System
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu %lu %lf",
                keyword, component_id, instance_id, datum_id, datum_value);

        return ScriptDoSystemComponentDatumDouble(new_str, linenum);

    default: {
        CString errmsg;
        errmsg.Format("Error: Invalid component class. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    }

    return 0;
}

long ScriptDoComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    char new_str[256] = "";
    char keyword[64] = "";
    unsigned long component_class = -1;
    unsigned long instance_id = -1;
    unsigned long component_id = -1;

    sscanf(buffer,
           "%s %lu %lu %lu",
           keyword, &component_class, &instance_id, &component_id);

    // Create a similar string but without the component class.
    sprintf(new_str,
            "%s %lu %lu",
            keyword, instance_id, component_id);

    switch (component_class) {
    case 0:     // Entity
        return ScriptDoEntityComponentEndUpdate(new_str, linenum);

    case 1:     // View
        return ScriptDoViewComponentEndUpdate(new_str, linenum);

    case 2:     // View Group
        return ScriptDoViewGroupComponentEndUpdate(new_str, linenum);

    case 3:     // Sensor
        return ScriptDoSensorComponentEndUpdate(new_str, linenum);

    case 4:     // Regional Sea Surface
        return ScriptDoRSeaComponentEndUpdate(new_str, linenum);

    case 5:     // Regional Terrain Surface
        return ScriptDoRTerrainComponentEndUpdate(new_str, linenum);

    case 6:     // Regional Weather Layer
        return ScriptDoRLayerComponentEndUpdate(new_str, linenum);

    case 7:     // Global Sea Surface
        return ScriptDoGSeaComponentEndUpdate(new_str, linenum);

    case 8:     // Global Terrain Surface
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu",
                keyword, component_id, instance_id);

        return ScriptDoGTerrainComponentEndUpdate(new_str, linenum);

    case 9:     // Global Weather Layer
        return ScriptDoGLayerComponentEndUpdate(new_str, linenum);

    case 10:    // Atmosphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu",
                keyword, component_id, instance_id);

        return ScriptDoAtmosphereComponentEndUpdate(new_str, linenum);

    case 11:    // Celestial Sphere
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu",
                keyword, component_id, instance_id);

        return ScriptDoCelSphereComponentEndUpdate(new_str, linenum);

    case 12:    // Event
        return ScriptDoEventComponentEndUpdate(new_str, linenum);

    case 13:    // System
        // Create a new string with the component ID and instance ID swapped.
        sprintf(new_str,
                "%s %lu %lu",
                keyword, component_id, instance_id);

        return ScriptDoSystemComponentEndUpdate(new_str, linenum);

    default: {
        CString errmsg;
        errmsg.Format("Error: Invalid component class. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }
    }

    return 0;
}

long ScriptDoEntityComponentState(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long component_id = -1;
    unsigned long state = -1;

    sscanf(buffer, "%s %lu %lu %lu", keyword, &entity_id, &component_id, &state);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = entity->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    comp->SetState(state);

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewComponentState(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long component_id = -1;
    unsigned long state = -1;

    sscanf(buffer, "%s %lu %lu %lu", keyword, &view_id, &component_id, &state);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = view->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    comp->SetState(state);

    MESSAGE_UPDATE_VIEW_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewGroupComponentState(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long viewgroup_id = -1;
    unsigned long component_id = -1;
    unsigned long state = -1;

    sscanf(buffer, "%s %lu %lu %lu", keyword, &viewgroup_id, &component_id, &state);

    CViewGroup *viewgroup = g_DataManager.GetViewGroup((unsigned char)viewgroup_id);
    if (!viewgroup) {
        CString errmsg;
        errmsg.Format("Error: Invalid viewgroup specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = viewgroup->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    comp->SetState(state);

    MESSAGE_UPDATE_VIEW_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoSensorComponentState(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Sensors not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRSeaComponentState(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRTerrainComponentState(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional terrain conditions not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRLayerComponentState(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional weather layers not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGSeaComponentState(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Global sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGTerrainComponentState(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long state = -1;

    sscanf(buffer, "%s %lu %lu %lu", keyword, &component_id, &instance_id, &state);

    CComponent *comp = g_DataManager.GetTerrainComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    comp->SetState(state);

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoGLayerComponentState(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long component_id = -1;
    unsigned long state = -1;

    sscanf(buffer, "%s %lu %lu %lu", keyword, &layer_id, &component_id, &state);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = weather->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    comp->SetState(state);

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoAtmosphereComponentState(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long state = -1;

    sscanf(buffer, "%s %lu %lu %lu", keyword, &component_id, &instance_id, &state);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetAtmosphereComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    comp->SetState(state);

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoCelSphereComponentState(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long state = -1;

    sscanf(buffer, "%s %lu %lu %lu", keyword, &component_id, &instance_id, &state);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetCelestialComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    comp->SetState(state);

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEventComponentState(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Event components not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoSystemComponentState(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    uint16_t component_id = -1;
    uint16_t instance_id = -1;
    unsigned int state = -1;

    sscanf(buffer, "%s %hu %hu %lu", keyword, &component_id, &instance_id, &state);

    CComponent *comp = g_DataManager.GetSystemComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    comp->SetState(state);

    MESSAGE_UPDATE_ENV_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    long data = 0;

    sscanf(buffer, "%s %lu %lu %lu %ld", keyword, &entity_id, &component_id, &wordnum, &data);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = entity->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    long data = 0;

    sscanf(buffer, "%s %lu %lu %lu %ld", keyword, &view_id, &component_id, &wordnum, &data);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = view->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_VIEW_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewGroupComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long viewgroup_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    long data = 0;

    sscanf(buffer, "%s %lu %lu %lu %ld", keyword, &viewgroup_id, &component_id, &wordnum, &data);

    CViewGroup *viewgroup = g_DataManager.GetViewGroup((unsigned char)viewgroup_id);
    if (!viewgroup) {
        CString errmsg;
        errmsg.Format("Error: Invalid viewgroup specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = viewgroup->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_VIEW_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoSensorComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Sensors not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRSeaComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRTerrainComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional terrain conditions not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRLayerComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional weather layers not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGSeaComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Global sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGTerrainComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    long data = 0;

    sscanf(buffer, "%s %lu %lu %lu %ld", keyword, &component_id, &instance_id, &wordnum, &data);

    CComponent *comp = g_DataManager.GetTerrainComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoGLayerComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long weather_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    long data = 0;

    sscanf(buffer, "%s %lu %lu %lu %ld", keyword, &weather_id, &component_id, &wordnum, &data);

    CWeather *weather = g_DataManager.GetWeatherLayer(weather_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = weather->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoAtmosphereComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    long data = 0;

    sscanf(buffer, "%s %lu %lu %lu %ld", keyword, &component_id, &instance_id, &wordnum, &data);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetAtmosphereComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoCelSphereComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    long data = 0;

    sscanf(buffer, "%s %lu %lu %lu %ld", keyword, &component_id, &instance_id, &wordnum, &data);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetCelestialComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEventComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Event components not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoSystemComponentDatumInt32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    uint16_t component_id = -1;
    uint16_t instance_id = -1;
    unsigned long wordnum = -1;
    int32_t data = 0;

    sscanf(buffer, "%s %hu %hu %ld %ld", keyword, &component_id, &instance_id, &wordnum, &data);

    CComponent *comp = g_DataManager.GetSystemComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    unsigned long data = 0;

    sscanf(buffer, "%s %lu %lu %lu %lu", keyword, &entity_id, &component_id, &wordnum, &data);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = entity->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    unsigned long data = 0;

    sscanf(buffer, "%s %lu %lu %lu %lu", keyword, &view_id, &component_id, &wordnum, &data);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = view->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_VIEW_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewGroupComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long viewgroup_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    unsigned long data = 0;

    sscanf(buffer, "%s %lu %lu %lu %lu", keyword, &viewgroup_id, &component_id, &wordnum, &data);

    CViewGroup *viewgroup = g_DataManager.GetViewGroup((unsigned char)viewgroup_id);
    if (!viewgroup) {
        CString errmsg;
        errmsg.Format("Error: Invalid viewgroup specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = viewgroup->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_VIEW_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoSensorComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Sensors not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRSeaComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRTerrainComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional terrain conditions not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRLayerComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional weather layers not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGSeaComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Global sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGTerrainComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    unsigned long data = 0;

    sscanf(buffer, "%s %lu %lu %lu %lu", keyword, &component_id, &instance_id, &wordnum, &data);

    CComponent *comp = g_DataManager.GetTerrainComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoGLayerComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long weather_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    unsigned long data = 0;

    sscanf(buffer, "%s %lu %lu %lu %lu", keyword, &weather_id, &component_id, &wordnum, &data);

    CWeather *weather = g_DataManager.GetWeatherLayer(weather_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = weather->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoAtmosphereComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    unsigned long data = 0;

    sscanf(buffer, "%s %lu %lu %lu %lu", keyword, &component_id, &instance_id, &wordnum, &data);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetAtmosphereComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoCelSphereComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    unsigned long data = 0;

    sscanf(buffer, "%s %lu %lu %lu %lu", keyword, &component_id, &instance_id, &wordnum, &data);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetCelestialComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEventComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Event components not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoSystemComponentDatumUint32(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    uint16_t component_id = -1;
    uint16_t instance_id = -1;
    unsigned long wordnum = -1;
    uint32_t data = 0;

    sscanf(buffer, "%s %hu %hu %lu %lu", keyword, &component_id, &instance_id, &wordnum, &data);

    CComponent *comp = g_DataManager.GetSystemComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt32, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %lu %lu %lu %I64d", keyword, &entity_id, &component_id, &wordnum, &data.QuadPart);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = entity->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %lu %lu %lu %I64d", keyword, &view_id, &component_id, &wordnum, &data.QuadPart);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = view->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_VIEW_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewGroupComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long viewgroup_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %lu %lu %lu %I64d", keyword, &viewgroup_id, &component_id, &wordnum, &data.QuadPart);

    CViewGroup *viewgroup = g_DataManager.GetViewGroup((unsigned char)viewgroup_id);
    if (!viewgroup) {
        CString errmsg;
        errmsg.Format("Error: Invalid viewgroup specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = viewgroup->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_VIEW_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoSensorComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Sensors not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRSeaComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRTerrainComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional terrain conditions not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRLayerComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional weather layers not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGSeaComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Global sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGTerrainComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %lu %lu %lu %I64d", keyword, &component_id, &instance_id, &wordnum, &data.QuadPart);

    CComponent *comp = g_DataManager.GetTerrainComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoGLayerComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long weather_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %lu %lu %lu %I64d", keyword, &weather_id, &component_id, &wordnum, &data.QuadPart);

    CWeather *weather = g_DataManager.GetWeatherLayer(weather_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = weather->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoAtmosphereComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %lu %lu %lu %I64d", keyword, &component_id, &instance_id, &wordnum, &data.QuadPart);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetAtmosphereComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoCelSphereComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %lu %lu %lu %I64d", keyword, &component_id, &instance_id, &wordnum, &data.QuadPart);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetCelestialComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEventComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Event components not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoSystemComponentDatumInt64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    uint16_t component_id = -1;
    uint16_t instance_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %hu %hu %lu %I64d", keyword, &component_id, &instance_id, &wordnum, &data.QuadPart);

    CComponent *comp = g_DataManager.GetSystemComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %lu %lu %lu %I64u", keyword, &entity_id, &component_id, &wordnum, &data.QuadPart);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = entity->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %lu %lu %lu %I64u", keyword, &view_id, &component_id, &wordnum, &data.QuadPart);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = view->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_VIEW_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewGroupComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long viewgroup_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %lu %lu %lu %I64u", keyword, &viewgroup_id, &component_id, &wordnum, &data.QuadPart);

    CViewGroup *viewgroup = g_DataManager.GetViewGroup((unsigned char)viewgroup_id);
    if (!viewgroup) {
        CString errmsg;
        errmsg.Format("Error: Invalid viewgroup specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = viewgroup->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_VIEW_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoSensorComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Sensors not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRSeaComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRTerrainComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional terrain conditions not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRLayerComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional weather layers not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGSeaComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Global sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGTerrainComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %lu %lu %lu %I64u", keyword, &component_id, &instance_id, &wordnum, &data.QuadPart);

    CComponent *comp = g_DataManager.GetTerrainComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoGLayerComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long weather_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %lu %lu %lu %I64u", keyword, &weather_id, &component_id, &wordnum, &data.QuadPart);

    CWeather *weather = g_DataManager.GetWeatherLayer(weather_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = weather->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoAtmosphereComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %lu %lu %lu %I64u", keyword, &component_id, &instance_id, &wordnum, &data.QuadPart);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetAtmosphereComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoCelSphereComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %lu %lu %lu %I64u", keyword, &component_id, &instance_id, &wordnum, &data.QuadPart);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetCelestialComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEventComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Event components not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoSystemComponentDatumUint64(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    uint16_t component_id = -1;
    uint16_t instance_id = -1;
    unsigned long wordnum = -1;
    LARGE_INTEGER data = {0};

    sscanf(buffer, "%s %hu %hu %lu %I64u", keyword, &component_id, &instance_id, &wordnum, &data.QuadPart);

    CComponent *comp = g_DataManager.GetSystemComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeUInt64, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    float data = 0;

    sscanf(buffer, "%s %lu %lu %lu %f", keyword, &entity_id, &component_id, &wordnum, &data);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = entity->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeFloat, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    float data = 0;

    sscanf(buffer, "%s %lu %lu %lu %f", keyword, &view_id, &component_id, &wordnum, &data);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = view->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeFloat, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_VIEW_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewGroupComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long viewgroup_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    float data = 0;

    sscanf(buffer, "%s %lu %lu %lu %f", keyword, &viewgroup_id, &component_id, &wordnum, &data);

    CViewGroup *viewgroup = g_DataManager.GetViewGroup((unsigned char)viewgroup_id);
    if (!viewgroup) {
        CString errmsg;
        errmsg.Format("Error: Invalid viewgroup specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = viewgroup->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeFloat, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_VIEW_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoSensorComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Sensors not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRSeaComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRTerrainComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional terrain conditions not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRLayerComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional weather layers not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGSeaComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Global sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGTerrainComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    float data = 0;

    sscanf(buffer, "%s %lu %lu %lu %f", keyword, &component_id, &instance_id, &wordnum, &data);

    CComponent *comp = g_DataManager.GetTerrainComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeFloat, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoGLayerComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long weather_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    float data = 0;

    sscanf(buffer, "%s %lu %lu %lu %f", keyword, &weather_id, &component_id, &wordnum, &data);

    CWeather *weather = g_DataManager.GetWeatherLayer(weather_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = weather->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeFloat, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoAtmosphereComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    float data = 0;

    sscanf(buffer, "%s %lu %lu %lu %f", keyword, &component_id, &instance_id, &wordnum, &data);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetAtmosphereComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeFloat, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoCelSphereComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    float data = 0;

    sscanf(buffer, "%s %lu %lu %lu %f", keyword, &component_id, &instance_id, &wordnum, &data);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetCelestialComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeFloat, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEventComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Event components not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoSystemComponentDatumFloat(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    uint16_t component_id = -1;
    uint16_t instance_id = -1;
    unsigned long wordnum = -1;
    float data = 0;

    sscanf(buffer, "%s %hu %hu %lu %f", keyword, &component_id, &instance_id, &wordnum, &data);

    CComponent *comp = g_DataManager.GetSystemComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeFloat, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    double data = 0;

    sscanf(buffer, "%s %lu %lu %lu %lf", keyword, &entity_id, &component_id, &wordnum, &data);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = entity->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeDouble, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENTITY_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    double data = 0;

    sscanf(buffer, "%s %lu %lu %lu %lf", keyword, &view_id, &component_id, &wordnum, &data);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = view->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeDouble, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_VIEW_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewGroupComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long viewgroup_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    double data = 0;

    sscanf(buffer, "%s %lu %lu %lu %lf", keyword, &viewgroup_id, &component_id, &wordnum, &data);

    CViewGroup *viewgroup = g_DataManager.GetViewGroup((unsigned char)viewgroup_id);
    if (!viewgroup) {
        CString errmsg;
        errmsg.Format("Error: Invalid viewgroup specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = viewgroup->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeDouble, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_VIEW_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoSensorComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Sensors not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRSeaComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRTerrainComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional terrain conditions not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRLayerComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional weather layers not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGSeaComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Global sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGTerrainComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    double data = 0;

    sscanf(buffer, "%s %lu %lu %lu %lf", keyword, &component_id, &instance_id, &wordnum, &data);

    CComponent *comp = g_DataManager.GetTerrainComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeDouble, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoGLayerComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long weather_id = -1;
    unsigned long component_id = -1;
    unsigned long wordnum = -1;
    double data = 0;

    sscanf(buffer, "%s %lu %lu %lu %lf", keyword, &weather_id, &component_id, &wordnum, &data);

    CWeather *weather = g_DataManager.GetWeatherLayer(weather_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = weather->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeDouble, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoAtmosphereComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    double data = 0;

    sscanf(buffer, "%s %lu %lu %lu %lf", keyword, &component_id, &instance_id, &wordnum, &data);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetAtmosphereComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeDouble, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoCelSphereComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned long wordnum = -1;
    double data = 0;

    sscanf(buffer, "%s %lu %lu %lu %lf", keyword, &component_id, &instance_id, &wordnum, &data);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetCelestialComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeDouble, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEventComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Event components not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoSystemComponentDatumDouble(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    uint16_t component_id = -1;
    uint16_t instance_id = -1;
    unsigned long wordnum = -1;
    double data = 0;

    sscanf(buffer, "%s %hu %hu %lu %lf", keyword, &component_id, &instance_id, &wordnum, &data);

    CComponent *comp = g_DataManager.GetSystemComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (!comp->SetData(wordnum, CComponent::DataTypeDouble, &data)) {
        CString errmsg;
        errmsg.Format("Error: Cannot set component datum %d. (line %lu)", wordnum, linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -3;
    }

    MESSAGE_UPDATE_ENV_TAB_PAGES msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoEntityComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long component_id = -1;
    unsigned char packetbuff[CIGI_COMPONENT_CONTROL_SIZE] = {0};

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &component_id);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = entity->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Create either a Component or Short Component packet.
    comp->CreateCigiPacket(packetbuff);

    // SendImmedCigiMessage() will figure out the packet size.
    SendImmedCigiMessage(packetbuff);

    return 0;
}

long ScriptDoViewComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long component_id = -1;
    unsigned char packetbuff[CIGI_COMPONENT_CONTROL_SIZE] = {0};

    sscanf(buffer, "%s %lu %lu", keyword, &view_id, &component_id);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = view->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Create either a Component or Short Component packet.
    comp->CreateCigiPacket(packetbuff);

    // SendImmedCigiMessage() will figure out the packet size.
    SendImmedCigiMessage(packetbuff);

    return 0;
}

long ScriptDoViewGroupComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long viewgroup_id = -1;
    unsigned long component_id = -1;
    unsigned char packetbuff[CIGI_COMPONENT_CONTROL_SIZE] = {0};

    sscanf(buffer, "%s %lu %lu", keyword, &viewgroup_id, &component_id);

    CViewGroup *viewgroup = g_DataManager.GetViewGroup((unsigned char)viewgroup_id);
    if (!viewgroup) {
        CString errmsg;
        errmsg.Format("Error: Invalid viewgroup specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = viewgroup->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Create either a Component or Short Component packet.
    comp->CreateCigiPacket(packetbuff);

    // SendImmedCigiMessage() will figure out the packet size.
    SendImmedCigiMessage(packetbuff);

    return 0;
}

long ScriptDoSensorComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Sensors not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRSeaComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRTerrainComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional terrain conditions not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoRLayerComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Regional weather layers not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGSeaComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Global sea states not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoGTerrainComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned char packetbuff[CIGI_COMPONENT_CONTROL_SIZE] = {0};

    sscanf(buffer, "%s %lu %lu", keyword, &component_id, &instance_id);

    CComponent *comp = g_DataManager.GetTerrainComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Create either a Component or Short Component packet.
    comp->CreateCigiPacket(packetbuff);

    // SendImmedCigiMessage() will figure out the packet size.
    SendImmedCigiMessage(packetbuff);

    return 0;
}

long ScriptDoGLayerComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long component_id = -1;
    unsigned char packetbuff[CIGI_COMPONENT_CONTROL_SIZE] = {0};

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &component_id);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = weather->GetComponent(component_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Create either a Component or Short Component packet.
    comp->CreateCigiPacket(packetbuff);

    // SendImmedCigiMessage() will figure out the packet size.
    SendImmedCigiMessage(packetbuff);

    return 0;
}

long ScriptDoAtmosphereComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned char packetbuff[CIGI_COMPONENT_CONTROL_SIZE] = {0};

    sscanf(buffer, "%s %lu %lu", keyword, &component_id, &instance_id);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetAtmosphereComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Create either a Component or Short Component packet.
    comp->CreateCigiPacket(packetbuff);

    // SendImmedCigiMessage() will figure out the packet size.
    SendImmedCigiMessage(packetbuff);

    return 0;
}

long ScriptDoCelSphereComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = -1;
    unsigned long instance_id = -1;
    unsigned char packetbuff[CIGI_COMPONENT_CONTROL_SIZE] = {0};

    sscanf(buffer, "%s %lu %lu", keyword, &component_id, &instance_id);

    CGlobalEnv *env = g_DataManager.GetGlobalEnv();
    if (!env) {
        CString errmsg;
        errmsg.Format("Error: Cannot access global environment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CComponent *comp = env->GetCelestialComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Create either a Component or Short Component packet.
    comp->CreateCigiPacket(packetbuff);

    // SendImmedCigiMessage() will figure out the packet size.
    SendImmedCigiMessage(packetbuff);

    return 0;
}

long ScriptDoEventComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    CString errmsg;
    errmsg.Format("Warning: Event components not supported. (line %lu)", linenum);
    ScriptPostTextMessage((LPCTSTR)errmsg);

    return 0;   // return success
}

long ScriptDoSystemComponentEndUpdate(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    uint16_t component_id = -1;
    uint16_t instance_id = -1;
    unsigned char packetbuff[CIGI_COMPONENT_CONTROL_SIZE] = {0};

    sscanf(buffer, "%s %hu %hu", keyword, &component_id, &instance_id);

    CComponent *comp = g_DataManager.GetSystemComponent(component_id, instance_id);
    if (!comp) {
        CString errmsg;
        errmsg.Format("Error: Invalid component specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Create either a Component or Short Component packet.
    comp->CreateCigiPacket(packetbuff);

    // SendImmedCigiMessage() will figure out the packet size.
    SendImmedCigiMessage(packetbuff);

    return 0;
}

// Undocumented
///////////////////////////////////////////////////////////////////////////////

long ScriptDoSensorComponent(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = 0;
    unsigned long instance_id = 0;
    unsigned long state = 0;
    CIGI_COMPONENT_CONTROL ctrl = { CIGI_COMPONENT_CONTROL_OPCODE,
                                    CIGI_COMPONENT_CONTROL_SIZE,
                                    0, 0, 3, 0, 0, 0, 0, 0, 0, 0
                                  };

    sscanf(buffer, "%s %lu %lu %lu %x %x %x %x %x %x", keyword, &component_id,
           &instance_id, &state, &ctrl.data1, &ctrl.data2,
           &ctrl.data3, &ctrl.data4, &ctrl.data5, &ctrl.data6);

    ctrl.component_id = (unsigned short)component_id;
    ctrl.instance_id = (unsigned short)instance_id;
    ctrl.component_state = (unsigned char)state;

    SendImmedCigiMessage(&ctrl);

    return 0;
}

long ScriptDoSensorShortComponent(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long component_id = 0;
    unsigned long instance_id = 0;
    unsigned long state = 0;
    CIGI_COMPONENT_CONTROL_S ctrl = {   CIGI_COMPONENT_CONTROL_S_OPCODE,
                                        CIGI_COMPONENT_CONTROL_S_SIZE,
                                        0, 0, 3, 0, 0, 0
                                    };

    sscanf(buffer, "%s %lu %lu %lu %x %x", keyword, &component_id, &instance_id,
           &state, &ctrl.data1, &ctrl.data2);

    ctrl.component_id = (unsigned short)component_id;
    ctrl.instance_id = (unsigned short)instance_id;
    ctrl.component_state = (unsigned char)state;

    SendImmedCigiMessage(&ctrl);

    return 0;
}
