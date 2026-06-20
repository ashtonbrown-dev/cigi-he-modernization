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
 *  FILENAME:   Common.cpp
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

#include "cigi4types.h"
#include "common.h"
#include "hemumsg.h"

RTXSharedBufferQ        g_DrvToGuiMsgQueue;
RTXSharedBufferQ        g_GuiToDrvMsgQueue;
RTXSharedBufferQ        g_SentCIGIMsgQueue;
RTXSharedBufferQ        g_SendImmedCIGIMsgQueue;
RTXSharedBufferQ        g_SendCIGIMsgQueue;
RTXSharedBufferQ        g_RcvCIGIMsgQueue;
HANDLE                  g_CreatedEntityEventHdl = NULL;
HANDLE                  g_ShutdownEventHdl = NULL;
HANDLE                  g_FrameWaitEventHdl = NULL;
EarthRefModelStruct     g_ERM;

const double TERRAIN_DEFAULT_FLATTENING = 1 / 298.257223563;
const double TERRAIN_DEFAULT_RADIUS = 6378137.0;

int InitializeIPC(void)
{
    int retval = 0;

    // Create the events. g_ShutdownEventHdl is created in CHemuApp::LoadDriver().
    g_CreatedEntityEventHdl = RtCreateEvent(NULL, FALSE, FALSE, "CreatedEntityEvent");
    g_FrameWaitEventHdl = RtCreateEvent(NULL, FALSE, FALSE, "FrameWaitEvent");

    // Set up the message queues.
    retval = g_GuiToDrvMsgQueue.Create("Win32-RTX_MsgQueue", 256, MESSAGE_MAX_SIZE);
    retval &= g_DrvToGuiMsgQueue.Create("RTX-Win32_MsgQueue", 512, MESSAGE_MAX_SIZE);   // make large for scripts

    // Initialize the CIGI message queues.
    retval &= g_SentCIGIMsgQueue.Create("Sent_CIGIMsgQueue", 512, MAX_ETHERNET_PACKET_SIZE);
    retval &= g_SendImmedCIGIMsgQueue.Create("SendImmediate_CIGIMsgQueue", 512, CIGI_MAX_PACKET_SIZE);
    retval &= g_SendCIGIMsgQueue.Create("Send_CIGIMsgQueue", 512, CIGI_MAX_PACKET_SIZE);
    retval &= g_RcvCIGIMsgQueue.Create("Rcv_CIGIMsgQueue", 128, MAX_ETHERNET_PACKET_SIZE);

    return retval;
}

void InitializeCCU(const double radius, const double flattening)
{
    // CCU_WGS84_a, CCU_WGS84_f
    initEarthRefModel(&g_ERM, radius, flattening);
}

BOOL GDCtoTM(double lat, double lon, double *y, double *x)
{
    // Don't use fabs() in case we have an overflow condition.
    if ((lat > 90.0) || (lat < -90.0) ||
        (lon > 180.0) || (lon < -180.0)) {
        return FALSE;
    }

    double gdc[3] = { lat, lon, 0.0 };
    double tm[3] = { 0.0 };             // East-North-Up

    convertGDCtoTM(&g_ERM, gdc, tm);
    *x = tm[0];
    *y = tm[1];

    return TRUE;
}

BOOL TMtoGDC(double y, double x, double *lat, double *lon)
{
    double tm[3] = { x, y, 0.0 };       // East-North-Up
    double gdc[3] = { 0.0 };

    convertTMtoGDC(&g_ERM, tm, gdc);

    // Don't use fabs() in case we have an overflow condition.
    if ((gdc[0] > 90.0) || (gdc[0] < -90.0) ||
        (gdc[1] > 180.0) || (gdc[1] < -180.0)) {
        return FALSE;
    }

    *lat = gdc[0];
    *lon = gdc[1];

    return TRUE;
}

void SetTMOrigin(double lat, double lon)
{
    double FalseNorthing;
    double FalseEasting;

    if (lat > 0.0)
        FalseNorthing = 0.0;
    else
        FalseNorthing = 10000000;

    if (lon < 0.0)
        FalseEasting = 500000;
    else
        FalseEasting = 0.0;

    //Calculate the centrial meridian for the specified zone
    double CentralMeridian = CalcCentralMeridian(lon);

    //Initialize ccu to use the new central meridian
    initTM(&g_ERM, CentralMeridian, FalseEasting, FalseNorthing,
           CC_UTM_CENTRAL_SCALE);
}

double CalcGeoDistance(const double lat1, const double lon1,
                       const double lat2, const double lon2)
{
    double az = 0.0;

    return distAzimuth(&g_ERM, lat1, lon1, lat2, lon2, &az);
}

double CalcGeoRelBearing(const double lat1, const double lon1, const double yaw1,
                         const double lat2, const double lon2)
{
    double az = 0.0;

    distAzimuth(&g_ERM, lat1, lon1, lat2, lon2, &az);
    az -= yaw1;

    // Find coterminal angle >= 0 and < 360.
    if (az < 0.0)
        az += 360.0;
    else if (az >= 360.0)
        az -= 360.0;

    return az;
}

BOOL CalcBearingAndRange(const double ref_lat, const double ref_lon,
                         const double ref_yaw, const double lat,
                         const double lon, double *bearing, double *range)
{
    if ((lat > 90.0) || (lat < -90.0) ||
        (lon > 180.0) || (lon < -180.0) ||
        (ref_lat > 90.0) || (ref_lat < -90.0) ||
        (ref_lon > 180.0) || (ref_lon < -180.0)) {
        return FALSE;
    }

    double az = 0.0;

    *range = distAzimuth(&g_ERM, ref_lat, ref_lon, lat, lon, &az);
    az -= ref_yaw;

    // Find coterminal angle >= 0 and < 360.
    if (az < 0.0)
        az += 360.0;
    else if (az >= 360.0)
        az -= 360.0;

    *bearing = az;

    return TRUE;
}

BOOL GetRelativePos(const double ref_lat, const double ref_lon,
                    const double ref_yaw, const double bearing,
                    const double range, double *lat, double *lon)
{
    double latitude = 0.0;
    double longitude = 0.0;

    waypoint(&g_ERM, ref_lat, ref_lon, range,
             (ref_yaw + bearing), &latitude, &longitude);

    if ((latitude > 90.0) || (latitude < -90.0) ||
        (longitude > 180.0) || (longitude < -180.0)) {
        return FALSE;
    }

    *lat = latitude;
    *lon = longitude;

    return TRUE;
}