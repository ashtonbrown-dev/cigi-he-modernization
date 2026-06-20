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

// See ScriptInit() in Script.cpp for the list of script command keyword mappings.

#include "afxtempl.h"

#define SCRIPT_ERROR_FILE_NOT_FOUND     -1

typedef long(* SCRIPT_CALLBACK)(const char *buffer, const unsigned int linenum);
typedef CMap<CString, CString, SCRIPT_CALLBACK, SCRIPT_CALLBACK> CKeywordHandlerMap;

// Forward-reference the message base class.
class HEMU_MESSAGE;

long ScriptInit(void);
unsigned int ExecuteScript(const char *fname);
void ScriptPostTextMessage(const char *text);

extern long ScriptDoNothing(const char *buffer, const unsigned int linenum);
extern long ScriptDoSpawnApp(const char *buffer, const unsigned int linenum);
extern long ScriptDoExecute(const char *buffer, const unsigned int linenum);
extern long ScriptDoRun(const char *buffer, const unsigned int linenum);
extern long ScriptDoFrameWait(const char *buffer, const unsigned int linenum);
extern long ScriptDoFrame(const char *buffer, const unsigned int linenum);
extern long ScriptDoMessage(const char *buffer, const unsigned int linenum);
extern long ScriptDoPause(const char *buffer, const unsigned int linenum);
extern long ScriptDoPauseMessage(const char *buffer, const unsigned int linenum);
extern long ScriptDoClearMessages(const char *buffer, const unsigned int linenum);
extern long ScriptDoWait(const char *buffer, const unsigned int linenum);
extern long ScriptDoAddEntity(const char *buffer, const unsigned int linenum);
extern long ScriptDoRemoveEntity(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityAddWaypoint(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityAirspeed(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityAlpha(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityAltitude(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityAltitudeRelative(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityAnimDirection(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityAnimLoopmode(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityAnimState(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityAttach(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityClampMode(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityClearWaypoints(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityCdEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityDetach(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityFly(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityFlyWaypoints(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityInheritAlpha(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityLatitude(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityLongitude(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityPitch(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityPitchOffset(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityPitchRelative(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityPos(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityPosRelative(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityRoll(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityRollOffset(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityRollRelative(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntitySpeed(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityState(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityXOffset(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityXYZOffset(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityYOffset(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityYaw(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityYawOffset(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityYawRelative(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityYPR(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityYPROffset(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityZOffset(const char *buffer, const unsigned int linenum);
extern long ScriptDoArtPartEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoArtPartEnableXYZ(const char *buffer, const unsigned int linenum);
extern long ScriptDoArtPartEnableYPR(const char *buffer, const unsigned int linenum);
extern long ScriptDoArtPartPitch(const char *buffer, const unsigned int linenum);
extern long ScriptDoArtPartRoll(const char *buffer, const unsigned int linenum);
extern long ScriptDoArtPartX(const char *buffer, const unsigned int linenum);
extern long ScriptDoArtPartXYZ(const char *buffer, const unsigned int linenum);
extern long ScriptDoArtPartY(const char *buffer, const unsigned int linenum);
extern long ScriptDoArtPartYaw(const char *buffer, const unsigned int linenum);
extern long ScriptDoArtPartYPR(const char *buffer, const unsigned int linenum);
extern long ScriptDoArtPartZ(const char *buffer, const unsigned int linenum);
extern long ScriptDoDate(const char *buffer, const unsigned int linenum);
extern long ScriptDoTime(const char *buffer, const unsigned int linenum);
extern long ScriptDoEphemerisMode(const char *buffer, const unsigned int linenum);
extern long ScriptDoSunEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoMoonEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoStarfieldEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoStarfieldIntensity(const char *buffer, const unsigned int linenum);
extern long ScriptDoAtmModelEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoGlobalHumidity(const char *buffer, const unsigned int linenum);
extern long ScriptDoGlobalAirTemp(const char *buffer, const unsigned int linenum);
extern long ScriptDoGlobalVisibility(const char *buffer, const unsigned int linenum);
extern long ScriptDoGlobalWind(const char *buffer, const unsigned int linenum);
extern long ScriptDoGlobalBaroPressure(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherHumidity(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherTopScudEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherBottomScudEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherRandomWindEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherLightning(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherCloudType(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherSeverity(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherAirTemp(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherVisibility(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherTopScudFreq(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherBottomScudFreq(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherCoverage(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherTopTransitionBand(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherBottomTransitionBand(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherWind(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherBaroPressure(const char *buffer, const unsigned int linenum);
extern long ScriptDoWeatherAerosolConc(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerHumidity(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerTopScudEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerBottomScudEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerRandomWindEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerLightning(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerCloudType(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerSeverity(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerAirTemp(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerVisibility(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerTopScudFreq(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerBottomScudFreq(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerCoverage(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerElevation(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerThickness(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerTopTransitionBand(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerBottomTransitionBand(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerWind(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerBaroPressure(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerAerosolConc(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerHumidity(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerTopScudEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerBottomScudEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerRandomWindEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerLightning(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerCloudType(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerSeverity(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerAirTemp(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerVisibility(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerTopScudFreq(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerBottomScudFreq(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerCoverage(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerElevation(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerThickness(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerTopTransitionBand(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerBottomTransitionBand(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerWind(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerBaroPressure(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerAerosolConc(const char *buffer, const unsigned int linenum);
extern long ScriptDoAttachView(const char *buffer, const unsigned int linenum);
extern long ScriptDoAttachViewgroup(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewPitch(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewRoll(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewType(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewX(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewXYZ(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewY(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewYaw(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewYPR(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewZ(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewGroup(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewMirrorMode(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewPixelRepMode(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewProjectionType(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewBringToTop(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewNearPlane(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewFarPlane(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewFOVLeft(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewFOVRight(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewFOVTop(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewFOVBottom(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewgroupPitch(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewgroupRoll(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewgroupX(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewgroupXYZ(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewgroupY(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewgroupYaw(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewgroupYPR(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewgroupZ(const char *buffer, const unsigned int linenum);
extern long ScriptDoCDSegment(const char *buffer, const unsigned int linenum);
extern long ScriptDoCDSegmentEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoCDVolumeSphere(const char *buffer, const unsigned int linenum);
extern long ScriptDoCDVolumeCuboid(const char *buffer, const unsigned int linenum);
extern long ScriptDoCDVolumeEnable(const char *buffer, const unsigned int linenum);
extern long ScriptDoWaypointAddRelative(const char *buffer, const unsigned int linenum);
extern long ScriptDoWaypointAddAbsolute(const char *buffer, const unsigned int linenum);
extern long ScriptDoWaypointClearAll(const char *buffer, const unsigned int linenum);
extern long ScriptDoMissileFire(const char *buffer, const unsigned int linenum);
extern long ScriptDoMissileHitEffect(const char *buffer, const unsigned int linenum);
extern long ScriptDoMissilePersistentLaunchEffect(const char *buffer, const unsigned int linenum);
extern long ScriptDoMissileTerminatingLaunchEffect(const char *buffer, const unsigned int linenum);
extern long ScriptDoMissileTarget(const char *buffer, const unsigned int linenum);
extern long ScriptDoCapturePlay(const char *buffer, const unsigned int linenum);
extern long ScriptDoAngularRate(const char *buffer, const unsigned int linenum);
extern long ScriptDoLinearRate(const char *buffer, const unsigned int linenum);

// Defined in ScriptHandlersComponents.cpp
extern long ScriptDoComponentState(const char *buffer, const unsigned int linenum);       // undocumented
extern long ScriptDoComponentDatumInt32(const char *buffer, const unsigned int linenum);  // undocumented
extern long ScriptDoComponentDatumUint32(const char *buffer, const unsigned int linenum); // undocumented
extern long ScriptDoComponentDatumInt64(const char *buffer, const unsigned int linenum);  // undocumented
extern long ScriptDoComponentDatumUint64(const char *buffer, const unsigned int linenum); // undocumented
extern long ScriptDoComponentDatumFloat(const char *buffer, const unsigned int linenum);  // undocumented
extern long ScriptDoComponentDatumDouble(const char *buffer, const unsigned int linenum); // undocumented
extern long ScriptDoComponentEndUpdate(const char *buffer, const unsigned int linenum);   // undocumented
extern long ScriptDoEntityComponentState(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewComponentState(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewGroupComponentState(const char *buffer, const unsigned int linenum);
extern long ScriptDoSensorComponentState(const char *buffer, const unsigned int linenum);
extern long ScriptDoRSeaComponentState(const char *buffer, const unsigned int linenum);
extern long ScriptDoRTerrainComponentState(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerComponentState(const char *buffer, const unsigned int linenum);
extern long ScriptDoGSeaComponentState(const char *buffer, const unsigned int linenum);
extern long ScriptDoGTerrainComponentState(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerComponentState(const char *buffer, const unsigned int linenum);
extern long ScriptDoAtmosphereComponentState(const char *buffer, const unsigned int linenum);
extern long ScriptDoCelSphereComponentState(const char *buffer, const unsigned int linenum);
extern long ScriptDoEventComponentState(const char *buffer, const unsigned int linenum);
extern long ScriptDoSystemComponentState(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityComponentDatumInt32(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewComponentDatumInt32(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewGroupComponentDatumInt32(const char *buffer, const unsigned int linenum);
extern long ScriptDoSensorComponentDatumInt32(const char *buffer, const unsigned int linenum);
extern long ScriptDoRSeaComponentDatumInt32(const char *buffer, const unsigned int linenum);
extern long ScriptDoRTerrainComponentDatumInt32(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerComponentDatumInt32(const char *buffer, const unsigned int linenum);
extern long ScriptDoGSeaComponentDatumInt32(const char *buffer, const unsigned int linenum);
extern long ScriptDoGTerrainComponentDatumInt32(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerComponentDatumInt32(const char *buffer, const unsigned int linenum);
extern long ScriptDoAtmosphereComponentDatumInt32(const char *buffer, const unsigned int linenum);
extern long ScriptDoCelSphereComponentDatumInt32(const char *buffer, const unsigned int linenum);
extern long ScriptDoEventComponentDatumInt32(const char *buffer, const unsigned int linenum);
extern long ScriptDoSystemComponentDatumInt32(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityComponentDatumUint32(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewComponentDatumUint32(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewGroupComponentDatumUint32(const char *buffer, const unsigned int linenum);
extern long ScriptDoSensorComponentDatumUint32(const char *buffer, const unsigned int linenum);
extern long ScriptDoRSeaComponentDatumUint32(const char *buffer, const unsigned int linenum);
extern long ScriptDoRTerrainComponentDatumUint32(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerComponentDatumUint32(const char *buffer, const unsigned int linenum);
extern long ScriptDoGSeaComponentDatumUint32(const char *buffer, const unsigned int linenum);
extern long ScriptDoGTerrainComponentDatumUint32(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerComponentDatumUint32(const char *buffer, const unsigned int linenum);
extern long ScriptDoAtmosphereComponentDatumUint32(const char *buffer, const unsigned int linenum);
extern long ScriptDoCelSphereComponentDatumUint32(const char *buffer, const unsigned int linenum);
extern long ScriptDoEventComponentDatumUint32(const char *buffer, const unsigned int linenum);
extern long ScriptDoSystemComponentDatumUint32(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityComponentDatumInt64(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewComponentDatumInt64(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewGroupComponentDatumInt64(const char *buffer, const unsigned int linenum);
extern long ScriptDoSensorComponentDatumInt64(const char *buffer, const unsigned int linenum);
extern long ScriptDoRSeaComponentDatumInt64(const char *buffer, const unsigned int linenum);
extern long ScriptDoRTerrainComponentDatumInt64(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerComponentDatumInt64(const char *buffer, const unsigned int linenum);
extern long ScriptDoGSeaComponentDatumInt64(const char *buffer, const unsigned int linenum);
extern long ScriptDoGTerrainComponentDatumInt64(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerComponentDatumInt64(const char *buffer, const unsigned int linenum);
extern long ScriptDoAtmosphereComponentDatumInt64(const char *buffer, const unsigned int linenum);
extern long ScriptDoCelSphereComponentDatumInt64(const char *buffer, const unsigned int linenum);
extern long ScriptDoEventComponentDatumInt64(const char *buffer, const unsigned int linenum);
extern long ScriptDoSystemComponentDatumInt64(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityComponentDatumUint64(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewComponentDatumUint64(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewGroupComponentDatumUint64(const char *buffer, const unsigned int linenum);
extern long ScriptDoSensorComponentDatumUint64(const char *buffer, const unsigned int linenum);
extern long ScriptDoRSeaComponentDatumUint64(const char *buffer, const unsigned int linenum);
extern long ScriptDoRTerrainComponentDatumUint64(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerComponentDatumUint64(const char *buffer, const unsigned int linenum);
extern long ScriptDoGSeaComponentDatumUint64(const char *buffer, const unsigned int linenum);
extern long ScriptDoGTerrainComponentDatumUint64(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerComponentDatumUint64(const char *buffer, const unsigned int linenum);
extern long ScriptDoAtmosphereComponentDatumUint64(const char *buffer, const unsigned int linenum);
extern long ScriptDoCelSphereComponentDatumUint64(const char *buffer, const unsigned int linenum);
extern long ScriptDoEventComponentDatumUint64(const char *buffer, const unsigned int linenum);
extern long ScriptDoSystemComponentDatumUint64(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityComponentDatumFloat(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewComponentDatumFloat(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewGroupComponentDatumFloat(const char *buffer, const unsigned int linenum);
extern long ScriptDoSensorComponentDatumFloat(const char *buffer, const unsigned int linenum);
extern long ScriptDoRSeaComponentDatumFloat(const char *buffer, const unsigned int linenum);
extern long ScriptDoRTerrainComponentDatumFloat(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerComponentDatumFloat(const char *buffer, const unsigned int linenum);
extern long ScriptDoGSeaComponentDatumFloat(const char *buffer, const unsigned int linenum);
extern long ScriptDoGTerrainComponentDatumFloat(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerComponentDatumFloat(const char *buffer, const unsigned int linenum);
extern long ScriptDoAtmosphereComponentDatumFloat(const char *buffer, const unsigned int linenum);
extern long ScriptDoCelSphereComponentDatumFloat(const char *buffer, const unsigned int linenum);
extern long ScriptDoEventComponentDatumFloat(const char *buffer, const unsigned int linenum);
extern long ScriptDoSystemComponentDatumFloat(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityComponentDatumDouble(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewComponentDatumDouble(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewGroupComponentDatumDouble(const char *buffer, const unsigned int linenum);
extern long ScriptDoSensorComponentDatumDouble(const char *buffer, const unsigned int linenum);
extern long ScriptDoRSeaComponentDatumDouble(const char *buffer, const unsigned int linenum);
extern long ScriptDoRTerrainComponentDatumDouble(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerComponentDatumDouble(const char *buffer, const unsigned int linenum);
extern long ScriptDoGSeaComponentDatumDouble(const char *buffer, const unsigned int linenum);
extern long ScriptDoGTerrainComponentDatumDouble(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerComponentDatumDouble(const char *buffer, const unsigned int linenum);
extern long ScriptDoAtmosphereComponentDatumDouble(const char *buffer, const unsigned int linenum);
extern long ScriptDoCelSphereComponentDatumDouble(const char *buffer, const unsigned int linenum);
extern long ScriptDoEventComponentDatumDouble(const char *buffer, const unsigned int linenum);
extern long ScriptDoSystemComponentDatumDouble(const char *buffer, const unsigned int linenum);
extern long ScriptDoEntityComponentEndUpdate(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewComponentEndUpdate(const char *buffer, const unsigned int linenum);
extern long ScriptDoViewGroupComponentEndUpdate(const char *buffer, const unsigned int linenum);
extern long ScriptDoSensorComponentEndUpdate(const char *buffer, const unsigned int linenum);
extern long ScriptDoRSeaComponentEndUpdate(const char *buffer, const unsigned int linenum);
extern long ScriptDoRTerrainComponentEndUpdate(const char *buffer, const unsigned int linenum);
extern long ScriptDoRLayerComponentEndUpdate(const char *buffer, const unsigned int linenum);
extern long ScriptDoGSeaComponentEndUpdate(const char *buffer, const unsigned int linenum);
extern long ScriptDoGTerrainComponentEndUpdate(const char *buffer, const unsigned int linenum);
extern long ScriptDoGLayerComponentEndUpdate(const char *buffer, const unsigned int linenum);
extern long ScriptDoAtmosphereComponentEndUpdate(const char *buffer, const unsigned int linenum);
extern long ScriptDoCelSphereComponentEndUpdate(const char *buffer, const unsigned int linenum);
extern long ScriptDoEventComponentEndUpdate(const char *buffer, const unsigned int linenum);
extern long ScriptDoSystemComponentEndUpdate(const char *buffer, const unsigned int linenum);

// Mission function-related commands.
extern long ScriptDoLOSSegmentReqBegin(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSSegmentReqEnd(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSSegmentReqType(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSSegmentReqSource(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSSegmentReqDestination(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSSegmentReqRespCoordSys(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSSegmentReqMaterialMask(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSSegmentReqUpdatePeriod(const char *buffer, const unsigned int linenum);

extern long ScriptDoLOSVectorReqBegin(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSVectorReqEnd(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSVectorReqType(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSVectorReqSource(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSVectorReqAzEl(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSVectorReqMinMaxRange(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSVectorReqRespCoordSys(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSVectorReqMaterialMask(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSVectorReqAlphaThresh(const char *buffer, const unsigned int linenum);
extern long ScriptDoLOSVectorReqUpdatePeriod(const char *buffer, const unsigned int linenum);

extern long ScriptDoHatHotReqBegin(const char *buffer, const unsigned int linenum);
extern long ScriptDoHatHotReqEnd(const char *buffer, const unsigned int linenum);
extern long ScriptDoHatHotReqType(const char *buffer, const unsigned int linenum);
extern long ScriptDoHatHotReqSource(const char *buffer, const unsigned int linenum);
extern long ScriptDoHatHotReqUpdatePeriod(const char *buffer, const unsigned int linenum);

// Symbol-related commands.
extern long ScriptDoSymbolSurfaceDefBegin(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolSurfaceDefEnd(const char *buffer, const unsigned int linenum);
extern long ScriptDoSurfaceID(const char *buffer, const unsigned int linenum);
extern long ScriptDoSurfaceState(const char *buffer, const unsigned int linenum);
extern long ScriptDoSurfaceAttachType(const char *buffer, const unsigned int linenum);
extern long ScriptDoSurfaceIsNotBillboard(const char *buffer, const unsigned int linenum);
extern long ScriptDoSurfaceIsBillboard(const char *buffer, const unsigned int linenum);
extern long ScriptDoSurfacePerspectiveGrowth(const char *buffer, const unsigned int linenum);
extern long ScriptDoSurfaceEntityID(const char *buffer, const unsigned int linenum);
extern long ScriptDoSurfaceViewID(const char *buffer, const unsigned int linenum);
extern long ScriptDoSurfaceOffsetPosition(const char *buffer, const unsigned int linenum);
extern long ScriptDoSurfaceOffsetAttitude(const char *buffer, const unsigned int linenum);
extern long ScriptDoSurfaceSize(const char *buffer, const unsigned int linenum);
extern long ScriptDoSurfaceExtents(const char *buffer, const unsigned int linenum);
extern long ScriptDoSurfaceUVBoundaries(const char *buffer, const unsigned int linenum);

extern long ScriptDoSymbolTextDefBegin(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolTextDefEnd(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolTextDefSymbolID(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolTextDefAlignment(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolTextDefOrientation(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolTextDefFontID(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolTextDefFontSize(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolTextDefText(const char *buffer, const unsigned int linenum);

extern long ScriptDoSymbolCircleDefBegin(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCircleDefEnd(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCircleDefSymbolID(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCircleDefDrawingStyle(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCircleDefStipple(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCircleDefLineWidth(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCircleDefStippleLength(const char *buffer, const unsigned int linenum);

extern long ScriptDoSymbolAddCircleBegin(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolAddCircleEnd(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCircleCenter(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCircleRadius(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCircleInnerRadius(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCircleStartAngle(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCircleEndAngle(const char *buffer, const unsigned int linenum);

extern long ScriptDoSymbolPolygonDefBegin(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolPolygonDefEnd(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolPolygonDefSymbolID(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolPolygonDefPrimitive(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolPolygonDefStipple(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolPolygonDefLineWidth(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolPolygonDefStippleLength(const char *buffer, const unsigned int linenum);

extern long ScriptDoSymbolVertex(const char *buffer, const unsigned int linenum);

extern long ScriptDoSymbolCloneBegin(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCloneEnd(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCloneSymbolID(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCloneSourceType(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolCloneSourceID(const char *buffer, const unsigned int linenum);

extern long ScriptDoSymbolControlBegin(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlEnd(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlSymbolID(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlSymbolState(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlAttachState(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlResetFlash(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlInheritColor(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlParentID(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlSurfaceID(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlLayer(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlFlashDutyCycle(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlFlashPeriod(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlPosition(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlRotation(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlRGBA(const char *buffer, const unsigned int linenum);
extern long ScriptDoSymbolControlScale(const char *buffer, const unsigned int linenum);

// Undocumented
extern long ScriptDoSensorComponent(const char *buffer, const unsigned int linenum);
extern long ScriptDoSensorShortComponent(const char *buffer, const unsigned int linenum);
extern long ScriptDoCapturePlay(const char *buffer, const unsigned int linenum);
extern long ScriptDoTranspDelayAddPoint(const char *buffer, const unsigned int linenum);
extern long ScriptDoTranspDelayClearPoints(const char *buffer, const unsigned int linenum);
extern long ScriptDoTranspDelayTest(const char *buffer, const unsigned int linenum);
