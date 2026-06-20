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

void ProcessIncomingCigiPacketQueue(void);
void ProcessOutgoingCigiPacketQueue(void);

long DoCigiIGControl(const int sessionid, void *data);
long DoCigiEntityPosition(const int sessionid, void *data);
long DoCigiCCEntityPosition(const int sessionid, void *data);
long DoCigiComponentControl(const int sessionid, void *data);
long DoCigiShortComponentControl(const int sessionid, void *data);
long DoCigiArtPartControl(const int sessionid, void *data);
long DoCigiShortArtPartControl(const int sessionid, void *data);
long DoCigiRateControl(const int sessionid, void *data);
long DoCigiCelestialSphereControl(const int sessionid, void *data);
long DoCigiAtmosphereControl(const int sessionid, void *data);
long DoCigiEnvRegionControl(const int sessionid, void *data);
long DoCigiWeatherControlOpcode(const int sessionid, void *data);
long DoCigiMaritimeSurfaceControl(const int sessionid, void *data);
long DoCigiWaveControl(const int sessionid, void *data);
long DoCigiTerrestrialSurfaceControl(const int sessionid, void *data);
long DoCigiViewControl(const int sessionid, void *data);
long DoCigiSensorControl(const int sessionid, void *data);
long DoCigiMotionTrackerControl(const int sessionid, void *data);
long DoCigiErmDef(const int sessionid, void *data);
long DoCigiTrajectoryDef(const int sessionid, void *data);
long DoCigiViewDef(const int sessionid, void *data);
long DoCigiCollisionSegDef(const int sessionid, void *data);
long DoCigiCollisionVolDef(const int sessionid, void *data);
long DoCigiHatRequest(const int sessionid, void *data);
long DoCigiLosSegmentRequest(const int sessionid, void *data);
long DoCigiLosVectorRequest(const int sessionid, void *data);
long DoCigiPositionRequest(const int sessionid, void *data);
long DoCigiEnvConditionsRequest(const int sessionid, void *data);
long DoCigiSymbolSurfaceDef(const int sessionid, void *data);
long DoCigiSymbolTextDef(const int sessionid, void *data);
long DoCigiSymbolCircleDef(const int sessionid, void *data);
long DoCigiSymbolLineDef(const int sessionid, void *data);
long DoCigiSymbolClone(const int sessionid, void *data);
long DoCigiSymbolControl(const int sessionid, void *data);
long DoCigiShortSymbolControl(const int sessionid, void *data);
long DoCigiEntityControl(const int sessionid, void *data);
long DoCigiAnimationControl(const int sessionid, void *data);

long DoSkippedFrame(const int sessionid, void *data);
long DoCigiStartOfFrame(const int sessionid, void *data);
long DoCigiHatResponse(const int sessionid, void *data);
long DoCigiHatExtResponse(const int sessionid, void *data);
long DoCigiLosResponse(const int sessionid, void *data);
long DoCigiLosExtResponse(const int sessionid, void *data);
long DoCigiSensorResponse(const int sessionid, void *data);
long DoCigiSensorExtResponse(const int sessionid, void *data);
long DoCigiPositionResponse(const int sessionid, void *data);
long DoCigiWeatherResponse(const int sessionid, void *data);
long DoCigiAerosolResponse(const int sessionid, void *data);
long DoCigiMaritimeSurfaceResponse(const int sessionid, void *data);
long DoCigiTerrestrialSurfaceResponse(const int sessionid, void *data);
long DoCigiCollSegmentNotification(const int sessionid, void *data);
long DoCigiCollVolumeNotification(const int sessionid, void *data);
long DoCigiAnimStopNotification(const int sessionid, void *data);
long DoCigiEventNotification(const int sessionid, void *data);
long DoCigiIGMessage(const int sessionid, void *data);