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

#include "StdAfx.h"
#include "cigi4types.h"
#include "hemumsg.h"
#include "globals.h"
#include "script.h"

volatile BOOL CanRemoveFnameFromStack = FALSE;
CMapStringToPtr g_KeywordMap;

char *ExecuteLine(const char *buffer, const unsigned int linenum);
void ucase(char *buffer);
unsigned int __stdcall ScriptThread(void *ptr);
long ScriptExecuteFile(const char *fname);

long ScriptInit(void)
{
    g_KeywordMap.RemoveAll();
    g_KeywordMap.InitHashTable(162);

    g_KeywordMap[CString("")] = ScriptDoNothing;
    g_KeywordMap[CString("SPAWN_APP")] = ScriptDoSpawnApp;
    g_KeywordMap[CString("#")] = ScriptDoNothing;
    g_KeywordMap[CString("EXECUTE")] = ScriptDoExecute;
    g_KeywordMap[CString("RUN")] = ScriptDoRun;
    g_KeywordMap[CString("FRAME_WAIT")] = ScriptDoFrameWait;
    g_KeywordMap[CString("FRAME")] = ScriptDoFrame;
    g_KeywordMap[CString("MESSAGE")] = ScriptDoMessage;
    g_KeywordMap[CString("PAUSE")] = ScriptDoPause;
    g_KeywordMap[CString("PAUSE_MESSAGE")] = ScriptDoPauseMessage;
    g_KeywordMap[CString("CLEAR_MESSAGES")] = ScriptDoClearMessages;
    g_KeywordMap[CString("WAIT")] = ScriptDoWait;

    g_KeywordMap[CString("ADD_ENTITY")] = ScriptDoAddEntity;
    g_KeywordMap[CString("ANGULAR_RATE")] = ScriptDoAngularRate;
    g_KeywordMap[CString("ARTPART_ENABLE")] = ScriptDoArtPartEnable;
    g_KeywordMap[CString("ARTPART_ENABLE_XYZ")] = ScriptDoArtPartEnableXYZ;
    g_KeywordMap[CString("ARTPART_ENABLE_YPR")] = ScriptDoArtPartEnableYPR;
    g_KeywordMap[CString("ARTPART_PITCH")] = ScriptDoArtPartPitch;
    g_KeywordMap[CString("ARTPART_ROLL")] = ScriptDoArtPartRoll;
    g_KeywordMap[CString("ARTPART_X")] = ScriptDoArtPartX;
    g_KeywordMap[CString("ARTPART_XYZ")] = ScriptDoArtPartXYZ;
    g_KeywordMap[CString("ARTPART_Y")] = ScriptDoArtPartY;
    g_KeywordMap[CString("ARTPART_YAW")] = ScriptDoArtPartYaw;
    g_KeywordMap[CString("ARTPART_YPR")] = ScriptDoArtPartYPR;
    g_KeywordMap[CString("ARTPART_Z")] = ScriptDoArtPartZ;
    g_KeywordMap[CString("ATMOSPHERE_COMPONENT_DATUM_DOUBLE")] = ScriptDoAtmosphereComponentDatumDouble;
    g_KeywordMap[CString("ATMOSPHERE_COMPONENT_DATUM_FLOAT")] = ScriptDoAtmosphereComponentDatumFloat;
    g_KeywordMap[CString("ATMOSPHERE_COMPONENT_DATUM_INT32")] = ScriptDoAtmosphereComponentDatumInt32;
    g_KeywordMap[CString("ATMOSPHERE_COMPONENT_DATUM_INT64")] = ScriptDoAtmosphereComponentDatumInt64;
    g_KeywordMap[CString("ATMOSPHERE_COMPONENT_DATUM_UINT32")] = ScriptDoAtmosphereComponentDatumUint32;
    g_KeywordMap[CString("ATMOSPHERE_COMPONENT_DATUM_UINT64")] = ScriptDoAtmosphereComponentDatumUint64;
    g_KeywordMap[CString("ATMOSPHERE_COMPONENT_END_UPDATE")] = ScriptDoAtmosphereComponentEndUpdate;
    g_KeywordMap[CString("ATMOSPHERE_COMPONENT_STATE")] = ScriptDoAtmosphereComponentState;
    g_KeywordMap[CString("ATM_MODEL_ENABLE")] = ScriptDoAtmModelEnable;
    g_KeywordMap[CString("ATTACH_VIEW")] = ScriptDoAttachView;
    g_KeywordMap[CString("ATTACH_VIEWGROUP")] = ScriptDoAttachViewgroup;
    g_KeywordMap[CString("CAPTURE_PLAY")] = ScriptDoCapturePlay;
    g_KeywordMap[CString("CD_SEGMENT")] = ScriptDoCDSegment;
    g_KeywordMap[CString("CD_SEGMENT_ENABLE")] = ScriptDoCDSegmentEnable;
    g_KeywordMap[CString("CD_VOLUME_CUBOID")] = ScriptDoCDVolumeCuboid;
    g_KeywordMap[CString("CD_VOLUME_ENABLE")] = ScriptDoCDVolumeEnable;
    g_KeywordMap[CString("CD_VOLUME_SPHERE")] = ScriptDoCDVolumeSphere;
    g_KeywordMap[CString("CELSPHERE_COMPONENT_DATUM_DOUBLE")] = ScriptDoCelSphereComponentDatumDouble;
    g_KeywordMap[CString("CELSPHERE_COMPONENT_DATUM_FLOAT")] = ScriptDoCelSphereComponentDatumFloat;
    g_KeywordMap[CString("CELSPHERE_COMPONENT_DATUM_INT32")] = ScriptDoCelSphereComponentDatumInt32;
    g_KeywordMap[CString("CELSPHERE_COMPONENT_DATUM_INT64")] = ScriptDoCelSphereComponentDatumInt64;
    g_KeywordMap[CString("CELSPHERE_COMPONENT_DATUM_UINT32")] = ScriptDoCelSphereComponentDatumUint32;
    g_KeywordMap[CString("CELSPHERE_COMPONENT_DATUM_UINT64")] = ScriptDoCelSphereComponentDatumUint64;
    g_KeywordMap[CString("CELSPHERE_COMPONENT_END_UPDATE")] = ScriptDoCelSphereComponentEndUpdate;
    g_KeywordMap[CString("CELSPHERE_COMPONENT_STATE")] = ScriptDoCelSphereComponentState;
    g_KeywordMap[CString("COMPONENT_DATUM_DOUBLE")] = ScriptDoComponentDatumDouble;
    g_KeywordMap[CString("COMPONENT_DATUM_FLOAT")] = ScriptDoComponentDatumFloat;
    g_KeywordMap[CString("COMPONENT_DATUM_INT32")] = ScriptDoComponentDatumInt32;
    g_KeywordMap[CString("COMPONENT_DATUM_INT64")] = ScriptDoComponentDatumInt64;
    g_KeywordMap[CString("COMPONENT_DATUM_UINT32")] = ScriptDoComponentDatumUint32;
    g_KeywordMap[CString("COMPONENT_DATUM_UINT64")] = ScriptDoComponentDatumUint64;
    g_KeywordMap[CString("COMPONENT_END_UPDATE")] = ScriptDoComponentEndUpdate;
    g_KeywordMap[CString("COMPONENT_STATE")] = ScriptDoComponentState;
    g_KeywordMap[CString("DATE")] = ScriptDoDate;
    g_KeywordMap[CString("ENTITY_ADD_WAYPOINT")] = ScriptDoEntityAddWaypoint;
    g_KeywordMap[CString("ENTITY_AIRSPEED")] = ScriptDoEntityAirspeed;
    g_KeywordMap[CString("ENTITY_ALPHA")] = ScriptDoEntityAlpha;
    g_KeywordMap[CString("ENTITY_ALTITUDE")] = ScriptDoEntityAltitude;
    g_KeywordMap[CString("ENTITY_ALTITUDE_RELATIVE")] = ScriptDoEntityAltitudeRelative;
    g_KeywordMap[CString("ENTITY_ANIM_DIRECTION")] = ScriptDoEntityAnimDirection;
    g_KeywordMap[CString("ENTITY_ANIM_LOOPMODE")] = ScriptDoEntityAnimLoopmode;
    g_KeywordMap[CString("ENTITY_ANIM_STATE")] = ScriptDoEntityAnimState;
    g_KeywordMap[CString("ENTITY_ATTACH")] = ScriptDoEntityAttach;
    g_KeywordMap[CString("ENTITY_CLAMP_MODE")] = ScriptDoEntityClampMode;
    g_KeywordMap[CString("ENTITY_CLEAR_WAYPOINTS")] = ScriptDoEntityClearWaypoints;
    g_KeywordMap[CString("ENTITY_COLLDET_ENABLE")] = ScriptDoEntityCdEnable;
    g_KeywordMap[CString("ENTITY_COMPONENT_DATUM_DOUBLE")] = ScriptDoEntityComponentDatumDouble;
    g_KeywordMap[CString("ENTITY_COMPONENT_DATUM_FLOAT")] = ScriptDoEntityComponentDatumFloat;
    g_KeywordMap[CString("ENTITY_COMPONENT_DATUM_INT32")] = ScriptDoEntityComponentDatumInt32;
    g_KeywordMap[CString("ENTITY_COMPONENT_DATUM_INT64")] = ScriptDoEntityComponentDatumInt64;
    g_KeywordMap[CString("ENTITY_COMPONENT_DATUM_UINT32")] = ScriptDoEntityComponentDatumUint32;
    g_KeywordMap[CString("ENTITY_COMPONENT_DATUM_UINT64")] = ScriptDoEntityComponentDatumUint64;
    g_KeywordMap[CString("ENTITY_COMPONENT_END_UPDATE")] = ScriptDoEntityComponentEndUpdate;
    g_KeywordMap[CString("ENTITY_COMPONENT_STATE")] = ScriptDoEntityComponentState;
    g_KeywordMap[CString("ENTITY_DETACH")] = ScriptDoEntityDetach;
    g_KeywordMap[CString("ENTITY_FLY")] = ScriptDoEntityFly;
    g_KeywordMap[CString("ENTITY_FLY_WAYPOINTS")] = ScriptDoEntityFlyWaypoints;
    g_KeywordMap[CString("ENTITY_INHERIT_ALPHA")] = ScriptDoEntityInheritAlpha;
    g_KeywordMap[CString("ENTITY_LATITUDE")] = ScriptDoEntityLatitude;
    g_KeywordMap[CString("ENTITY_LONGITUDE")] = ScriptDoEntityLongitude;
    g_KeywordMap[CString("ENTITY_PITCH")] = ScriptDoEntityPitch;
    g_KeywordMap[CString("ENTITY_PITCH_OFFSET")] = ScriptDoEntityPitchOffset;
    g_KeywordMap[CString("ENTITY_PITCH_RELATIVE")] = ScriptDoEntityPitchRelative;
    g_KeywordMap[CString("ENTITY_POS")] = ScriptDoEntityPos;
    g_KeywordMap[CString("ENTITY_POS_RELATIVE")] = ScriptDoEntityPosRelative;
    g_KeywordMap[CString("ENTITY_ROLL")] = ScriptDoEntityRoll;
    g_KeywordMap[CString("ENTITY_ROLL_OFFSET")] = ScriptDoEntityRollOffset;
    g_KeywordMap[CString("ENTITY_ROLL_RELATIVE")] = ScriptDoEntityRollRelative;
    g_KeywordMap[CString("ENTITY_SPEED")] = ScriptDoEntitySpeed;
    g_KeywordMap[CString("ENTITY_STATE")] = ScriptDoEntityState;
    g_KeywordMap[CString("ENTITY_XYZ_OFFSET")] = ScriptDoEntityXYZOffset;
    g_KeywordMap[CString("ENTITY_X_OFFSET")] = ScriptDoEntityXOffset;
    g_KeywordMap[CString("ENTITY_YAW")] = ScriptDoEntityYaw;
    g_KeywordMap[CString("ENTITY_YAW_OFFSET")] = ScriptDoEntityYawOffset;
    g_KeywordMap[CString("ENTITY_YAW_RELATIVE")] = ScriptDoEntityYawRelative;
    g_KeywordMap[CString("ENTITY_YPR")] = ScriptDoEntityYPR;
    g_KeywordMap[CString("ENTITY_YPR_OFFSET")] = ScriptDoEntityYPROffset;
    g_KeywordMap[CString("ENTITY_Y_OFFSET")] = ScriptDoEntityYOffset;
    g_KeywordMap[CString("ENTITY_Z_OFFSET")] = ScriptDoEntityZOffset;
    g_KeywordMap[CString("EPHEMERIS_MODE")] = ScriptDoEphemerisMode;
    g_KeywordMap[CString("EVENT_COMPONENT_DATUM_DOUBLE")] = ScriptDoEventComponentDatumDouble;
    g_KeywordMap[CString("EVENT_COMPONENT_DATUM_FLOAT")] = ScriptDoEventComponentDatumFloat;
    g_KeywordMap[CString("EVENT_COMPONENT_DATUM_INT32")] = ScriptDoEventComponentDatumInt32;
    g_KeywordMap[CString("EVENT_COMPONENT_DATUM_INT64")] = ScriptDoEventComponentDatumInt64;
    g_KeywordMap[CString("EVENT_COMPONENT_DATUM_UINT32")] = ScriptDoEventComponentDatumUint32;
    g_KeywordMap[CString("EVENT_COMPONENT_DATUM_UINT64")] = ScriptDoEventComponentDatumUint64;
    g_KeywordMap[CString("EVENT_COMPONENT_END_UPDATE")] = ScriptDoEventComponentEndUpdate;
    g_KeywordMap[CString("EVENT_COMPONENT_STATE")] = ScriptDoEventComponentState;
    g_KeywordMap[CString("GLAYER_AEROSOL_CONC")] = ScriptDoGLayerAerosolConc;
    g_KeywordMap[CString("GLAYER_AIR_TEMP")] = ScriptDoGLayerAirTemp;
    g_KeywordMap[CString("GLAYER_BARO_PRESSURE")] = ScriptDoGLayerBaroPressure;
    g_KeywordMap[CString("GLAYER_CLOUD_TYPE")] = ScriptDoGLayerCloudType;
    g_KeywordMap[CString("GLAYER_COMPONENT_DATUM_DOUBLE")] = ScriptDoGLayerComponentDatumDouble;
    g_KeywordMap[CString("GLAYER_COMPONENT_DATUM_FLOAT")] = ScriptDoGLayerComponentDatumFloat;
    g_KeywordMap[CString("GLAYER_COMPONENT_DATUM_INT32")] = ScriptDoGLayerComponentDatumInt32;
    g_KeywordMap[CString("GLAYER_COMPONENT_DATUM_INT64")] = ScriptDoGLayerComponentDatumInt64;
    g_KeywordMap[CString("GLAYER_COMPONENT_DATUM_UINT32")] = ScriptDoGLayerComponentDatumUint32;
    g_KeywordMap[CString("GLAYER_COMPONENT_DATUM_UINT64")] = ScriptDoGLayerComponentDatumUint64;
    g_KeywordMap[CString("GLAYER_COMPONENT_END_UPDATE")] = ScriptDoGLayerComponentEndUpdate;
    g_KeywordMap[CString("GLAYER_COMPONENT_STATE")] = ScriptDoGLayerComponentState;
    g_KeywordMap[CString("GLAYER_COVERAGE")] = ScriptDoGLayerCoverage;
    g_KeywordMap[CString("GLAYER_ELEVATION")] = ScriptDoGLayerElevation;
    g_KeywordMap[CString("GLAYER_ENABLE")] = ScriptDoGLayerEnable;
    g_KeywordMap[CString("GLAYER_HUMIDITY")] = ScriptDoGLayerHumidity;
    g_KeywordMap[CString("GLAYER_LIGHTNING_ENABLE")] = ScriptDoGLayerLightning;
    g_KeywordMap[CString("GLAYER_RANDOM_WIND_ENABLE")] = ScriptDoGLayerRandomWindEnable;
    g_KeywordMap[CString("GLAYER_TOP_SCUD_ENABLE")] = ScriptDoGLayerTopScudEnable;
    g_KeywordMap[CString("GLAYER_BOTTOM_SCUD_ENABLE")] = ScriptDoGLayerBottomScudEnable;
    g_KeywordMap[CString("GLAYER_TOP_SCUD_FREQ")] = ScriptDoGLayerTopScudFreq;
    g_KeywordMap[CString("GLAYER_BOTTOM_SCUD_FREQ")] = ScriptDoGLayerBottomScudFreq;
    g_KeywordMap[CString("GLAYER_SEVERITY")] = ScriptDoGLayerSeverity;
    g_KeywordMap[CString("GLAYER_THICKNESS")] = ScriptDoGLayerThickness;
    g_KeywordMap[CString("GLAYER_TOP_TRANSITION_BAND")] = ScriptDoGLayerTopTransitionBand;
    g_KeywordMap[CString("GLAYER_BOTTOM_TRANSITION_BAND")] = ScriptDoGLayerBottomTransitionBand;
    g_KeywordMap[CString("GLAYER_VISIBILITY")] = ScriptDoGLayerVisibility;
    g_KeywordMap[CString("GLAYER_WIND")] = ScriptDoGLayerWind;
    g_KeywordMap[CString("GLOBAL_AIR_TEMP")] = ScriptDoGlobalAirTemp;
    g_KeywordMap[CString("GLOBAL_BARO_PRESSURE")] = ScriptDoGlobalBaroPressure;
    g_KeywordMap[CString("GLOBAL_HUMIDITY")] = ScriptDoGlobalHumidity;
    g_KeywordMap[CString("GLOBAL_VISIBILITY")] = ScriptDoGlobalVisibility;
    g_KeywordMap[CString("GLOBAL_WIND")] = ScriptDoGlobalWind;
    g_KeywordMap[CString("GSEA_COMPONENT_DATUM_DOUBLE")] = ScriptDoGSeaComponentDatumDouble;
    g_KeywordMap[CString("GSEA_COMPONENT_DATUM_FLOAT")] = ScriptDoGSeaComponentDatumFloat;
    g_KeywordMap[CString("GSEA_COMPONENT_DATUM_INT32")] = ScriptDoGSeaComponentDatumInt32;
    g_KeywordMap[CString("GSEA_COMPONENT_DATUM_INT64")] = ScriptDoGSeaComponentDatumInt64;
    g_KeywordMap[CString("GSEA_COMPONENT_DATUM_UINT32")] = ScriptDoGSeaComponentDatumUint32;
    g_KeywordMap[CString("GSEA_COMPONENT_DATUM_UINT64")] = ScriptDoGSeaComponentDatumUint64;
    g_KeywordMap[CString("GSEA_COMPONENT_END_UPDATE")] = ScriptDoGSeaComponentEndUpdate;
    g_KeywordMap[CString("GSEA_COMPONENT_STATE")] = ScriptDoGSeaComponentState;
    g_KeywordMap[CString("GTERRAIN_COMPONENT_DATUM_DOUBLE")] = ScriptDoGTerrainComponentDatumDouble;
    g_KeywordMap[CString("GTERRAIN_COMPONENT_DATUM_FLOAT")] = ScriptDoGTerrainComponentDatumFloat;
    g_KeywordMap[CString("GTERRAIN_COMPONENT_DATUM_INT32")] = ScriptDoGTerrainComponentDatumInt32;
    g_KeywordMap[CString("GTERRAIN_COMPONENT_DATUM_INT64")] = ScriptDoGTerrainComponentDatumInt64;
    g_KeywordMap[CString("GTERRAIN_COMPONENT_DATUM_UINT32")] = ScriptDoGTerrainComponentDatumUint32;
    g_KeywordMap[CString("GTERRAIN_COMPONENT_DATUM_UINT64")] = ScriptDoGTerrainComponentDatumUint64;
    g_KeywordMap[CString("GTERRAIN_COMPONENT_END_UPDATE")] = ScriptDoGTerrainComponentEndUpdate;
    g_KeywordMap[CString("GTERRAIN_COMPONENT_STATE")] = ScriptDoGTerrainComponentState;
    g_KeywordMap[CString("HAT_HOT_REQ_BEGIN")] = ScriptDoHatHotReqBegin;
    g_KeywordMap[CString("HAT_HOT_REQ_END")] = ScriptDoHatHotReqEnd;
    g_KeywordMap[CString("HAT_HOT_REQ_SOURCE")] = ScriptDoHatHotReqSource;
    g_KeywordMap[CString("HAT_HOT_REQ_TYPE")] = ScriptDoHatHotReqType;
    g_KeywordMap[CString("HAT_HOT_REQ_UPDATE_PERIOD")] = ScriptDoHatHotReqUpdatePeriod;
    g_KeywordMap[CString("LINEAR_RATE")] = ScriptDoLinearRate;
    g_KeywordMap[CString("LOS_SEGMENT_REQ_BEGIN")] = ScriptDoLOSSegmentReqBegin;
    g_KeywordMap[CString("LOS_SEGMENT_REQ_DESTINATION")] = ScriptDoLOSSegmentReqDestination;
    g_KeywordMap[CString("LOS_SEGMENT_REQ_END")] = ScriptDoLOSSegmentReqEnd;
    g_KeywordMap[CString("LOS_SEGMENT_REQ_MATERIAL_MASK")] = ScriptDoLOSSegmentReqMaterialMask;
    g_KeywordMap[CString("LOS_SEGMENT_REQ_RESP_COORD_SYS")] = ScriptDoLOSSegmentReqRespCoordSys;
    g_KeywordMap[CString("LOS_SEGMENT_REQ_SOURCE")] = ScriptDoLOSSegmentReqSource;
    g_KeywordMap[CString("LOS_SEGMENT_REQ_TYPE")] = ScriptDoLOSSegmentReqType;
    g_KeywordMap[CString("LOS_SEGMENT_REQ_UPDATE_PERIOD")] = ScriptDoLOSSegmentReqUpdatePeriod;
    g_KeywordMap[CString("LOS_VECTOR_REQ_ALPHA_THRESH")] = ScriptDoLOSVectorReqAlphaThresh;
    g_KeywordMap[CString("LOS_VECTOR_REQ_AZ_EL")] = ScriptDoLOSVectorReqAzEl;
    g_KeywordMap[CString("LOS_VECTOR_REQ_BEGIN")] = ScriptDoLOSVectorReqBegin;
    g_KeywordMap[CString("LOS_VECTOR_REQ_END")] = ScriptDoLOSVectorReqEnd;
    g_KeywordMap[CString("LOS_VECTOR_REQ_MATERIAL_MASK")] = ScriptDoLOSVectorReqMaterialMask;
    g_KeywordMap[CString("LOS_VECTOR_REQ_MIN_MAX_RANGE")] = ScriptDoLOSVectorReqMinMaxRange;
    g_KeywordMap[CString("LOS_VECTOR_REQ_RESP_COORD_SYS")] = ScriptDoLOSVectorReqRespCoordSys;
    g_KeywordMap[CString("LOS_VECTOR_REQ_SOURCE")] = ScriptDoLOSVectorReqSource;
    g_KeywordMap[CString("LOS_VECTOR_REQ_TYPE")] = ScriptDoLOSVectorReqType;
    g_KeywordMap[CString("LOS_VECTOR_REQ_UPDATE_PERIOD")] = ScriptDoLOSVectorReqUpdatePeriod;
    g_KeywordMap[CString("MISSILE_FIRE")] = ScriptDoMissileFire;
    g_KeywordMap[CString("MISSILE_HIT_EFFECT")] = ScriptDoMissileHitEffect;
    g_KeywordMap[CString("MISSILE_PERSISTENT_LAUNCH_EFFECT")] = ScriptDoMissilePersistentLaunchEffect;
    g_KeywordMap[CString("MISSILE_TARGET")] = ScriptDoMissileTarget;
    g_KeywordMap[CString("MISSILE_TERMINATING_LAUNCH_EFFECT")] = ScriptDoMissileTerminatingLaunchEffect;
    g_KeywordMap[CString("MOON_ENABLE")] = ScriptDoMoonEnable;
    g_KeywordMap[CString("REMOVE_ENTITY")] = ScriptDoRemoveEntity;
    g_KeywordMap[CString("RLAYER_AEROSOL_CONC")] = ScriptDoRLayerAerosolConc;
    g_KeywordMap[CString("RLAYER_AIR_TEMP")] = ScriptDoRLayerAirTemp;
    g_KeywordMap[CString("RLAYER_BARO_PRESSURE")] = ScriptDoRLayerBaroPressure;
    g_KeywordMap[CString("RLAYER_CLOUD_TYPE")] = ScriptDoRLayerCloudType;
    g_KeywordMap[CString("RLAYER_COMPONENT_DATUM_DOUBLE")] = ScriptDoRLayerComponentDatumDouble;
    g_KeywordMap[CString("RLAYER_COMPONENT_DATUM_FLOAT")] = ScriptDoRLayerComponentDatumFloat;
    g_KeywordMap[CString("RLAYER_COMPONENT_DATUM_INT32")] = ScriptDoRLayerComponentDatumInt32;
    g_KeywordMap[CString("RLAYER_COMPONENT_DATUM_INT64")] = ScriptDoRLayerComponentDatumInt64;
    g_KeywordMap[CString("RLAYER_COMPONENT_DATUM_UINT32")] = ScriptDoRLayerComponentDatumUint32;
    g_KeywordMap[CString("RLAYER_COMPONENT_DATUM_UINT64")] = ScriptDoRLayerComponentDatumUint64;
    g_KeywordMap[CString("RLAYER_COMPONENT_END_UPDATE")] = ScriptDoRLayerComponentEndUpdate;
    g_KeywordMap[CString("RLAYER_COMPONENT_STATE")] = ScriptDoRLayerComponentState;
    g_KeywordMap[CString("RLAYER_COVERAGE")] = ScriptDoRLayerCoverage;
    g_KeywordMap[CString("RLAYER_ELEVATION")] = ScriptDoRLayerElevation;
    g_KeywordMap[CString("RLAYER_ENABLE")] = ScriptDoRLayerEnable;
    g_KeywordMap[CString("RLAYER_HUMIDITY")] = ScriptDoRLayerHumidity;
    g_KeywordMap[CString("RLAYER_LIGHTNING_ENABLE")] = ScriptDoRLayerLightning;
    g_KeywordMap[CString("RLAYER_RANDOM_WIND_ENABLE")] = ScriptDoRLayerRandomWindEnable;
    g_KeywordMap[CString("RLAYER_TOP_SCUD_ENABLE")] = ScriptDoRLayerTopScudEnable;
    g_KeywordMap[CString("RLAYER_BOTTOM_SCUD_ENABLE")] = ScriptDoRLayerBottomScudEnable;
    g_KeywordMap[CString("RLAYER_TOP_SCUD_FREQ")] = ScriptDoRLayerTopScudFreq;
    g_KeywordMap[CString("RLAYER_BOTTOM_SCUD_FREQ")] = ScriptDoRLayerBottomScudFreq;
    g_KeywordMap[CString("RLAYER_SEVERITY")] = ScriptDoRLayerSeverity;
    g_KeywordMap[CString("RLAYER_THICKNESS")] = ScriptDoRLayerThickness;
    g_KeywordMap[CString("RLAYER_TOP_TRANSITION_BAND")] = ScriptDoRLayerTopTransitionBand;
    g_KeywordMap[CString("RLAYER_BOTTOM_TRANSITION_BAND")] = ScriptDoRLayerBottomTransitionBand;
    g_KeywordMap[CString("RLAYER_VISIBILITY")] = ScriptDoRLayerVisibility;
    g_KeywordMap[CString("RLAYER_WIND")] = ScriptDoRLayerWind;
    g_KeywordMap[CString("RSEA_COMPONENT_DATUM_DOUBLE")] = ScriptDoRSeaComponentDatumDouble;
    g_KeywordMap[CString("RSEA_COMPONENT_DATUM_FLOAT")] = ScriptDoRSeaComponentDatumFloat;
    g_KeywordMap[CString("RSEA_COMPONENT_DATUM_INT32")] = ScriptDoRSeaComponentDatumInt32;
    g_KeywordMap[CString("RSEA_COMPONENT_DATUM_INT64")] = ScriptDoRSeaComponentDatumInt64;
    g_KeywordMap[CString("RSEA_COMPONENT_DATUM_UINT32")] = ScriptDoRSeaComponentDatumUint32;
    g_KeywordMap[CString("RSEA_COMPONENT_DATUM_UINT64")] = ScriptDoRSeaComponentDatumUint64;
    g_KeywordMap[CString("RSEA_COMPONENT_END_UPDATE")] = ScriptDoSystemComponentEndUpdate;
    g_KeywordMap[CString("RSEA_COMPONENT_STATE")] = ScriptDoRSeaComponentState;
    g_KeywordMap[CString("RTERRAIN_COMPONENT_DATUM_DOUBLE")] = ScriptDoRTerrainComponentDatumDouble;
    g_KeywordMap[CString("RTERRAIN_COMPONENT_DATUM_FLOAT")] = ScriptDoRTerrainComponentDatumFloat;
    g_KeywordMap[CString("RTERRAIN_COMPONENT_DATUM_INT32")] = ScriptDoRTerrainComponentDatumInt32;
    g_KeywordMap[CString("RTERRAIN_COMPONENT_DATUM_INT64")] = ScriptDoRTerrainComponentDatumInt64;
    g_KeywordMap[CString("RTERRAIN_COMPONENT_DATUM_UINT32")] = ScriptDoRTerrainComponentDatumUint32;
    g_KeywordMap[CString("RTERRAIN_COMPONENT_DATUM_UINT64")] = ScriptDoRTerrainComponentDatumUint64;
    g_KeywordMap[CString("RTERRAIN_COMPONENT_END_UPDATE")] = ScriptDoRTerrainComponentEndUpdate;
    g_KeywordMap[CString("RTERRAIN_COMPONENT_STATE")] = ScriptDoRSeaComponentState;
    g_KeywordMap[CString("SENSOR_COMPONENT")] = ScriptDoSensorComponent;
    g_KeywordMap[CString("SENSOR_COMPONENT_DATUM_DOUBLE")] = ScriptDoSensorComponentDatumDouble;
    g_KeywordMap[CString("SENSOR_COMPONENT_DATUM_FLOAT")] = ScriptDoSensorComponentDatumFloat;
    g_KeywordMap[CString("SENSOR_COMPONENT_DATUM_INT32")] = ScriptDoSensorComponentDatumInt32;
    g_KeywordMap[CString("SENSOR_COMPONENT_DATUM_INT64")] = ScriptDoSensorComponentDatumInt64;
    g_KeywordMap[CString("SENSOR_COMPONENT_DATUM_UINT32")] = ScriptDoSensorComponentDatumUint32;
    g_KeywordMap[CString("SENSOR_COMPONENT_DATUM_UINT64")] = ScriptDoSensorComponentDatumUint64;
    g_KeywordMap[CString("SENSOR_COMPONENT_END_UPDATE")] = ScriptDoSensorComponentEndUpdate;
    g_KeywordMap[CString("SENSOR_COMPONENT_STATE")] = ScriptDoSensorComponentState;
    g_KeywordMap[CString("SENSOR_SHORT_COMPONENT")] = ScriptDoSensorShortComponent;
    g_KeywordMap[CString("STARFIELD_ENABLE")] = ScriptDoStarfieldEnable;
    g_KeywordMap[CString("STARFIELD_INTENSITY")] = ScriptDoStarfieldIntensity;
    g_KeywordMap[CString("SUN_ENABLE")] = ScriptDoSunEnable;
    g_KeywordMap[CString("SYSTEM_COMPONENT_DATUM_DOUBLE")] = ScriptDoSystemComponentDatumDouble;
    g_KeywordMap[CString("SYSTEM_COMPONENT_DATUM_FLOAT")] = ScriptDoSystemComponentDatumFloat;
    g_KeywordMap[CString("SYSTEM_COMPONENT_DATUM_INT32")] = ScriptDoSystemComponentDatumInt32;
    g_KeywordMap[CString("SYSTEM_COMPONENT_DATUM_INT64")] = ScriptDoSystemComponentDatumInt64;
    g_KeywordMap[CString("SYSTEM_COMPONENT_DATUM_UINT32")] = ScriptDoSystemComponentDatumUint32;
    g_KeywordMap[CString("SYSTEM_COMPONENT_DATUM_UINT64")] = ScriptDoSystemComponentDatumUint64;
    g_KeywordMap[CString("SYSTEM_COMPONENT_END_UPDATE")] = ScriptDoSystemComponentEndUpdate;
    g_KeywordMap[CString("SYSTEM_COMPONENT_STATE")] = ScriptDoSystemComponentState;
    g_KeywordMap[CString("TIME")] = ScriptDoTime;
    g_KeywordMap[CString("TRANSPDELAY_ADD_POINT")] = ScriptDoTranspDelayAddPoint;
    g_KeywordMap[CString("TRANSPDELAY_CLEAR_POINTS")] = ScriptDoTranspDelayClearPoints;
    g_KeywordMap[CString("TRANSPDELAY_TEST")] = ScriptDoTranspDelayTest;
    g_KeywordMap[CString("VIEWGROUP_ATTACH")] = ScriptDoAttachViewgroup;
    g_KeywordMap[CString("VIEWGROUP_COMPONENT_DATUM_DOUBLE")] = ScriptDoViewGroupComponentDatumDouble;
    g_KeywordMap[CString("VIEWGROUP_COMPONENT_DATUM_FLOAT")] = ScriptDoViewGroupComponentDatumFloat;
    g_KeywordMap[CString("VIEWGROUP_COMPONENT_DATUM_INT32")] = ScriptDoViewGroupComponentDatumInt32;
    g_KeywordMap[CString("VIEWGROUP_COMPONENT_DATUM_INT64")] = ScriptDoViewGroupComponentDatumInt64;
    g_KeywordMap[CString("VIEWGROUP_COMPONENT_DATUM_UINT32")] = ScriptDoViewGroupComponentDatumUint32;
    g_KeywordMap[CString("VIEWGROUP_COMPONENT_DATUM_UINT64")] = ScriptDoViewGroupComponentDatumUint64;
    g_KeywordMap[CString("VIEWGROUP_COMPONENT_END_UPDATE")] = ScriptDoViewGroupComponentEndUpdate;
    g_KeywordMap[CString("VIEWGROUP_COMPONENT_STATE")] = ScriptDoViewGroupComponentState;
    g_KeywordMap[CString("VIEWGROUP_PITCH")] = ScriptDoViewgroupPitch;
    g_KeywordMap[CString("VIEWGROUP_ROLL")] = ScriptDoViewgroupRoll;
    g_KeywordMap[CString("VIEWGROUP_X")] = ScriptDoViewgroupX;
    g_KeywordMap[CString("VIEWGROUP_XYZ")] = ScriptDoViewgroupXYZ;
    g_KeywordMap[CString("VIEWGROUP_Y")] = ScriptDoViewgroupY;
    g_KeywordMap[CString("VIEWGROUP_YAW")] = ScriptDoViewgroupYaw;
    g_KeywordMap[CString("VIEWGROUP_YPR")] = ScriptDoViewgroupYPR;
    g_KeywordMap[CString("VIEWGROUP_Z")] = ScriptDoViewgroupZ;
    g_KeywordMap[CString("VIEW_ATTACH")] = ScriptDoAttachView;
    g_KeywordMap[CString("VIEW_BRING_TO_TOP")] = ScriptDoViewBringToTop;
    g_KeywordMap[CString("VIEW_COMPONENT_DATUM_DOUBLE")] = ScriptDoViewComponentDatumDouble;
    g_KeywordMap[CString("VIEW_COMPONENT_DATUM_FLOAT")] = ScriptDoViewComponentDatumFloat;
    g_KeywordMap[CString("VIEW_COMPONENT_DATUM_INT32")] = ScriptDoViewComponentDatumInt32;
    g_KeywordMap[CString("VIEW_COMPONENT_DATUM_INT64")] = ScriptDoViewComponentDatumInt64;
    g_KeywordMap[CString("VIEW_COMPONENT_DATUM_UINT32")] = ScriptDoViewComponentDatumUint32;
    g_KeywordMap[CString("VIEW_COMPONENT_DATUM_UINT64")] = ScriptDoViewComponentDatumUint64;
    g_KeywordMap[CString("VIEW_COMPONENT_END_UPDATE")] = ScriptDoViewComponentEndUpdate;
    g_KeywordMap[CString("VIEW_COMPONENT_STATE")] = ScriptDoViewComponentState;
    g_KeywordMap[CString("VIEW_FAR_PLANE")] = ScriptDoViewFarPlane;
    g_KeywordMap[CString("VIEW_FOV_BOTTOM")] = ScriptDoViewFOVBottom;
    g_KeywordMap[CString("VIEW_FOV_LEFT")] = ScriptDoViewFOVLeft;
    g_KeywordMap[CString("VIEW_FOV_RIGHT")] = ScriptDoViewFOVRight;
    g_KeywordMap[CString("VIEW_FOV_TOP")] = ScriptDoViewFOVTop;
    g_KeywordMap[CString("VIEW_GROUP")] = ScriptDoViewGroup;
    g_KeywordMap[CString("VIEW_MIRROR_MODE")] = ScriptDoViewMirrorMode;
    g_KeywordMap[CString("VIEW_NEAR_PLANE")] = ScriptDoViewNearPlane;
    g_KeywordMap[CString("VIEW_PITCH")] = ScriptDoViewPitch;
    g_KeywordMap[CString("VIEW_PIXEL_REP_MODE")] = ScriptDoViewPixelRepMode;
    g_KeywordMap[CString("VIEW_PROJECTION_TYPE")] = ScriptDoViewProjectionType;
    g_KeywordMap[CString("VIEW_ROLL")] = ScriptDoViewRoll;
    g_KeywordMap[CString("VIEW_TYPE")] = ScriptDoViewType;
    g_KeywordMap[CString("VIEW_X")] = ScriptDoViewX;
    g_KeywordMap[CString("VIEW_XYZ")] = ScriptDoViewXYZ;
    g_KeywordMap[CString("VIEW_Y")] = ScriptDoViewY;
    g_KeywordMap[CString("VIEW_YAW")] = ScriptDoViewYaw;
    g_KeywordMap[CString("VIEW_YPR")] = ScriptDoViewYPR;
    g_KeywordMap[CString("VIEW_Z")] = ScriptDoViewZ;
    g_KeywordMap[CString("WAYPOINT_ADD_ABSOLUTE")] = ScriptDoWaypointAddAbsolute;
    g_KeywordMap[CString("WAYPOINT_ADD_RELATIVE")] = ScriptDoWaypointAddRelative;
    g_KeywordMap[CString("WAYPOINT_CLEAR_ALL")] = ScriptDoWaypointClearAll;
    g_KeywordMap[CString("WEATHER_AEROSOL_CONC")] = ScriptDoWeatherAerosolConc;
    g_KeywordMap[CString("WEATHER_AIR_TEMP")] = ScriptDoWeatherAirTemp;
    g_KeywordMap[CString("WEATHER_BARO_PRESSURE")] = ScriptDoWeatherBaroPressure;
    g_KeywordMap[CString("WEATHER_CLOUD_TYPE")] = ScriptDoWeatherCloudType;
    g_KeywordMap[CString("WEATHER_COVERAGE")] = ScriptDoWeatherCoverage;
    g_KeywordMap[CString("WEATHER_ENABLE")] = ScriptDoWeatherEnable;
    g_KeywordMap[CString("WEATHER_HUMIDITY")] = ScriptDoWeatherHumidity;
    g_KeywordMap[CString("WEATHER_LIGHTNING_ENABLE")] = ScriptDoWeatherLightning;
    g_KeywordMap[CString("WEATHER_RANDOM_WIND_ENABLE")] = ScriptDoWeatherRandomWindEnable;
    g_KeywordMap[CString("WEATHER_TOP_SCUD_ENABLE")] = ScriptDoWeatherTopScudEnable;
    g_KeywordMap[CString("WEATHER_BOTTOM_SCUD_ENABLE")] = ScriptDoWeatherBottomScudEnable;
    g_KeywordMap[CString("WEATHER_TOP_SCUD_FREQ")] = ScriptDoWeatherTopScudFreq;
    g_KeywordMap[CString("WEATHER_BOTTOM_SCUD_FREQ")] = ScriptDoWeatherBottomScudFreq;
    g_KeywordMap[CString("WEATHER_SEVERITY")] = ScriptDoWeatherSeverity;
    g_KeywordMap[CString("WEATHER_TOP_TRANSITION_BAND")] = ScriptDoWeatherTopTransitionBand;
    g_KeywordMap[CString("WEATHER_BOTTOM_TRANSITION_BAND")] = ScriptDoWeatherBottomTransitionBand;
    g_KeywordMap[CString("WEATHER_VISIBILITY")] = ScriptDoWeatherVisibility;
    g_KeywordMap[CString("WEATHER_WIND")] = ScriptDoWeatherWind;

    // Symbols:
        g_KeywordMap[CString("SYMBOL_ADD_VERTEX")] = ScriptDoSymbolVertex;
        g_KeywordMap[CString("SYMBOL_CIRCLE_BEGIN")] = ScriptDoSymbolCircleDefBegin;
        g_KeywordMap[CString("SYMBOL_CIRCLE_CENTER_POSITION")] = ScriptDoSymbolCircleCenter;
        g_KeywordMap[CString("SYMBOL_CIRCLE_DRAWING_STYLE")] = ScriptDoSymbolCircleDefDrawingStyle;
        g_KeywordMap[CString("SYMBOL_CIRCLE_ELEMENT_BEGIN")] = ScriptDoSymbolAddCircleBegin;
        g_KeywordMap[CString("SYMBOL_CIRCLE_ELEMENT_END")] = ScriptDoSymbolAddCircleEnd;
        g_KeywordMap[CString("SYMBOL_CIRCLE_END")] = ScriptDoSymbolCircleDefEnd;
        g_KeywordMap[CString("SYMBOL_CIRCLE_END_ANGLE")] = ScriptDoSymbolCircleEndAngle;
        g_KeywordMap[CString("SYMBOL_CIRCLE_ID")] = ScriptDoSymbolCircleDefSymbolID;
        g_KeywordMap[CString("SYMBOL_CIRCLE_INNER_RADIUS")] = ScriptDoSymbolCircleInnerRadius;
        g_KeywordMap[CString("SYMBOL_CIRCLE_LINE_WIDTH")] = ScriptDoSymbolCircleDefLineWidth;
        g_KeywordMap[CString("SYMBOL_CIRCLE_RADIUS")] = ScriptDoSymbolCircleRadius;
        g_KeywordMap[CString("SYMBOL_CIRCLE_START_ANGLE")] = ScriptDoSymbolCircleStartAngle;
        g_KeywordMap[CString("SYMBOL_CIRCLE_STIPPLE_PATTERN")] = ScriptDoSymbolCircleDefStipple;
        g_KeywordMap[CString("SYMBOL_CIRCLE_STIPPLE_PATTERN_LENGTH")] = ScriptDoSymbolCircleDefStippleLength;
        g_KeywordMap[CString("SYMBOL_CLONE_BEGIN")] = ScriptDoSymbolCloneBegin;
        g_KeywordMap[CString("SYMBOL_CLONE_END")] = ScriptDoSymbolCloneEnd;
        g_KeywordMap[CString("SYMBOL_CLONE_ID")] = ScriptDoSymbolCloneSymbolID;
        g_KeywordMap[CString("SYMBOL_CLONE_SOURCE_ID")] = ScriptDoSymbolCloneSourceID;
        g_KeywordMap[CString("SYMBOL_CLONE_SOURCE_TYPE")] = ScriptDoSymbolCloneSourceType;
        g_KeywordMap[CString("SYMBOL_CONTROL_ATTACH_STATE")] = ScriptDoSymbolControlAttachState;
        g_KeywordMap[CString("SYMBOL_CONTROL_BEGIN")] = ScriptDoSymbolControlBegin;
        g_KeywordMap[CString("SYMBOL_CONTROL_END")] = ScriptDoSymbolControlEnd;
        g_KeywordMap[CString("SYMBOL_CONTROL_FLASH_DUTY_CYCLE")] = ScriptDoSymbolControlFlashDutyCycle;
        g_KeywordMap[CString("SYMBOL_CONTROL_FLASH_PERIOD")] = ScriptDoSymbolControlFlashPeriod;
        g_KeywordMap[CString("SYMBOL_CONTROL_ID")] = ScriptDoSymbolControlSymbolID;
        g_KeywordMap[CString("SYMBOL_CONTROL_INHERIT_COLOR")] = ScriptDoSymbolControlInheritColor;
        g_KeywordMap[CString("SYMBOL_CONTROL_LAYER")] = ScriptDoSymbolControlLayer;
        g_KeywordMap[CString("SYMBOL_CONTROL_PARENT_ID")] = ScriptDoSymbolControlParentID;
        g_KeywordMap[CString("SYMBOL_CONTROL_POSITION")] = ScriptDoSymbolControlPosition;
        g_KeywordMap[CString("SYMBOL_CONTROL_RESET_FLASH")] = ScriptDoSymbolControlResetFlash;
        g_KeywordMap[CString("SYMBOL_CONTROL_RGBA")] = ScriptDoSymbolControlRGBA;
        g_KeywordMap[CString("SYMBOL_CONTROL_ROTATION")] = ScriptDoSymbolControlRotation;
        g_KeywordMap[CString("SYMBOL_CONTROL_SCALE")] = ScriptDoSymbolControlScale;
        g_KeywordMap[CString("SYMBOL_CONTROL_SURFACE_ID")] = ScriptDoSymbolControlSurfaceID;
        g_KeywordMap[CString("SYMBOL_CONTROL_SYMBOL_STATE")] = ScriptDoSymbolControlSymbolState;
        g_KeywordMap[CString("SYMBOL_POLYGON_BEGIN")] = ScriptDoSymbolPolygonDefBegin;
        g_KeywordMap[CString("SYMBOL_POLYGON_END")] = ScriptDoSymbolPolygonDefEnd;
        g_KeywordMap[CString("SYMBOL_POLYGON_ID")] = ScriptDoSymbolPolygonDefSymbolID;
        g_KeywordMap[CString("SYMBOL_POLYGON_LINE_WIDTH")] = ScriptDoSymbolPolygonDefLineWidth;
        g_KeywordMap[CString("SYMBOL_POLYGON_PRIMITIVE")] = ScriptDoSymbolPolygonDefPrimitive;
        g_KeywordMap[CString("SYMBOL_POLYGON_STIPPLE_PATTERN")] = ScriptDoSymbolPolygonDefStipple;
        g_KeywordMap[CString("SYMBOL_POLYGON_STIPPLE_PATTERN_LENGTH")] = ScriptDoSymbolPolygonDefStippleLength;
        g_KeywordMap[CString("SYMBOL_SURFACE_ATTACH_TYPE")] = ScriptDoSurfaceAttachType;
        g_KeywordMap[CString("SYMBOL_SURFACE_END")] = ScriptDoSymbolSurfaceDefEnd;
        g_KeywordMap[CString("SYMBOL_SURFACE_ENTITY_ID")] = ScriptDoSurfaceEntityID;
        g_KeywordMap[CString("SYMBOL_SURFACE_EXTENTS")] = ScriptDoSurfaceExtents;
        g_KeywordMap[CString("SYMBOL_SURFACE_ID")] = ScriptDoSurfaceID;
        g_KeywordMap[CString("SYMBOL_SURFACE_IS_BILLBOARD")] = ScriptDoSurfaceIsBillboard;
        g_KeywordMap[CString("SYMBOL_SURFACE_IS_NOT_BILLBOARD")] = ScriptDoSurfaceIsNotBillboard;
        g_KeywordMap[CString("SYMBOL_SURFACE_OFFSET_ATTITUDE")] = ScriptDoSurfaceOffsetAttitude;
        g_KeywordMap[CString("SYMBOL_SURFACE_OFFSET_POSITION")] = ScriptDoSurfaceOffsetPosition;
        g_KeywordMap[CString("SYMBOL_SURFACE_PERSPECTIVE_GROWTH")] = ScriptDoSurfacePerspectiveGrowth;
        g_KeywordMap[CString("SYMBOL_SURFACE_SIZE")] = ScriptDoSurfaceSize;
        g_KeywordMap[CString("SYMBOL_SURFACE_STATE")] = ScriptDoSurfaceState;
        g_KeywordMap[CString("SYMBOL_SURFACE_UV_BOUNDARIES")] = ScriptDoSurfaceUVBoundaries;
        g_KeywordMap[CString("SYMBOL_SURFACE_VIEW_ID")] = ScriptDoSurfaceViewID;
        g_KeywordMap[CString("SYMBOL_TEXT_ALIGNMENT")] = ScriptDoSymbolTextDefAlignment;
        g_KeywordMap[CString("SYMBOL_TEXT_BEGIN")] = ScriptDoSymbolTextDefBegin;
        g_KeywordMap[CString("SYMBOL_TEXT_END")] = ScriptDoSymbolTextDefEnd;
        g_KeywordMap[CString("SYMBOL_TEXT_FONT_ID")] = ScriptDoSymbolTextDefFontID;
        g_KeywordMap[CString("SYMBOL_TEXT_FONT_SIZE")] = ScriptDoSymbolTextDefFontSize;
        g_KeywordMap[CString("SYMBOL_TEXT_ID")] = ScriptDoSymbolTextDefSymbolID;
        g_KeywordMap[CString("SYMBOL_TEXT_ORIENTATION")] = ScriptDoSymbolTextDefOrientation;
        g_KeywordMap[CString("SYMBOL_TEXT_STRING")] = ScriptDoSymbolTextDefText;
        g_KeywordMap[CString("SYMBOL_SURFACE_BEGIN")] = ScriptDoSymbolSurfaceDefBegin;

    return 0;
}

void ScriptPostTextMessage(const char *text)
{
    // Display the text in the message window.  Since we are running in our
    // own thread, we have problems with window handles not being valid.
    // We can get around that by posting a message directly to the main thread.
    MESSAGE_DISPLAY_MSG_TEXT msg;
    strcpy(msg.text, text);
    PostGUIMsg(msg);
}

unsigned int ExecuteScript(const char *fname)
{
    ScriptInit();

    if (g_ScriptThreadAddrHdl)
        return 0;

    // Freeze the host emulator. This should be done by CTestView::OnDebugRun(),
    // but just to be safe...
    ::SetFreezeFlag(TRUE);

    // Send a message to the RT driver.
    MESSAGE_PAUSE_EXERCISE msg;
    PostDriverMsg(msg);

    g_ScriptState = SCRIPT_STATE_PLAY;
    g_ScriptParsePause = FALSE;

    // If we don't combine the send message queues, we might need to
    // empty g_SendCIGIMsgQueue here.

    // The filename is on the stack, so we don't want to destroy it
    // until the other thread has a chance to make a copy of it.
    CanRemoveFnameFromStack = FALSE;

    if (_beginthreadex(NULL, 0, ScriptThread, (void *)fname, 0, &g_ScriptThreadAddrHdl) == -1) {
        MessageBox(NULL, "Unable to start script thread.", "Error", MB_OK | MB_ICONEXCLAMATION);
        return NULL;
    }

    while (g_ScriptThreadAddrHdl && !CanRemoveFnameFromStack)
        Sleep(1);

    // Empty the send message queue so we don't resend everything that
    // happened during the script playback.
    char buffer[CIGI_MAX_PACKET_SIZE];
    while (g_SendCIGIMsgQueue.Pop(buffer, CIGI_MAX_PACKET_SIZE) > 0);

    return g_ScriptThreadAddrHdl;
}

unsigned int __stdcall ScriptThread(void *ptr)
{
    char fname[256];

    // Since the filename (ptr) is on the stack of another thread, we need to
    // make a copy of it.
    strcpy(fname, (char *)ptr);
    CanRemoveFnameFromStack = TRUE;

    ScriptExecuteFile(fname);

    // Send a message to ourselves by placing a message in the
    // IG-to-Host message queue.  We will close the handle in
    // the message handler.
    MESSAGE_SCRIPT_EOF msg;
    PostGUIMsg(msg);

    // Turn off waypoint flight per entity.
    g_DataManager.DisableWaypointFlight();

    return 1;
}

long ScriptExecuteFile(const char *fname)
{
    FILE *fp = NULL;
    unsigned int linecount = 1;
    char buffer[256];
    CString msg;

    // Open the file.
    fp = fopen(fname, "r");
    if (!fp) {
        msg.Format("Cannot open %s.", fname);
        ScriptPostTextMessage((LPCSTR)msg);

        return SCRIPT_ERROR_FILE_NOT_FOUND;
    }

    g_NestedScriptCount++;

    msg.Format("Executing %s.", fname);
    ScriptPostTextMessage((LPCSTR)msg);

    while (!feof(fp) && g_ScriptState) { // and check event
        if ((g_ScriptState != SCRIPT_STATE_PAUSE) && (g_ScriptParsePause == FALSE)) {
            // If the shutdown event is signaled, terminate the thread.
            if (WaitForSingleObject(g_ShutdownEventHdl, 0) != WAIT_TIMEOUT) {
                _endthread();
            }

            memset(buffer, 0, 256);
            fgets(buffer, 256, fp);
            ucase(buffer);

            // If not a blank line or comment
            if ((buffer[0] != '\0') && (buffer[0] != '\n') && (buffer[0] != '#'))
                ExecuteLine(buffer, linecount);

            linecount++;

            // Every 10 lines, we want to update the progress meter.
            if ((g_NestedScriptCount == 1) && (linecount % 10 == 0)) {
                MESSAGE_SCRIPT_PROGRESS msg_progress;
                msg_progress.linenum = linecount;
                PostGUIMsg(msg_progress);
            }
        } else
            Sleep(1);
    }

    fclose(fp);

    g_NestedScriptCount--;

    // Post update messages and notify the user we are finished.
    MESSAGE_SYNC_UPDATE_ENTITIES_VIEW ent_msg;
    PostDriverMsg(ent_msg);
    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);
    MESSAGE_SYNC_UPDATE_VIEWS_VIEW views_msg;
    PostDriverMsg(views_msg);

    // If the last line was PAUSE...
    while (g_ScriptState == SCRIPT_STATE_PAUSE)
        Sleep(1);

    // Display in the GUI that the script is finished.
    msg.Format("Finished executing %s.", fname);
    ScriptPostTextMessage((LPCTSTR)msg);

    // Update the script progress bar.
    MESSAGE_SCRIPT_PROGRESS msg_progress;
    msg_progress.linenum = linecount;
    PostGUIMsg(msg_progress);

    return 0;
}

char *ExecuteLine(const char *buffer, const unsigned int linenum)
{
    SCRIPT_CALLBACK func = NULL;
    void *fptr = NULL;
    char kw[64] = {0};
    CString keyword;
    CString errmsg;

    sscanf(buffer, "%s", &kw);
    keyword = kw;
    keyword.TrimLeft();

    g_KeywordMap.Lookup(keyword, fptr);
    if (fptr) {
        func = (SCRIPT_CALLBACK)fptr;
        func(buffer, linenum);
    } else {
        // If it isn't a comment, print a warning.
        if (((LPCTSTR)keyword)[0] != '#') {
            errmsg.Format("Error: Invalid keyword (line %lu). Ensure CIGI version is configured correctly in File -> Setup.", linenum);
            ScriptPostTextMessage((LPCTSTR)errmsg);
        }
    }

    return 0;
}

void ucase(char *buffer)
{
    BOOL inside_quotes = FALSE;

    while (*buffer) {
        if (*buffer == '\"')
            inside_quotes = !inside_quotes;

        if (!inside_quotes)
            *buffer = toupper(*buffer);

        buffer++;
    }
}