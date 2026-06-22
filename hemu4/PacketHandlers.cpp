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
#include "common.h"
#include "cigi4.h"
#include "cigi4Types.h"
#include "Entity.h"
#include "EntityStateView.h"
#include "globals.h"
#include "Hemu4.h"
#include "MessageView.h"
#include "PacketHandlers.h"
#include "PacketWatchView.h"
#include "PacketViewListCtrl.h"

///////////////////////////////////////////////////////////////////////////////
//
//  Host-to-IG
//
///////////////////////////////////////////////////////////////////////////////

void ProcessOutgoingCigiPacketQueue(void)
{
    static char buffer[MAX_ETHERNET_PACKET_SIZE] = {0};
    static unsigned int heartbeat_counter = 0;
    static unsigned int fr = 0;
    static unsigned int avg_fr = 0;
    static unsigned int max_fr = 0;
    static unsigned int min_fr = (unsigned int) - 1;
    static int size = 0;

    while ((size = g_SentCIGIMsgQueue.Pop(buffer, sizeof(buffer))) > 0) {
        // Pop the CIGI message and process it.
        CigiSetIncomingMsgBuffer(g_IGSession, (unsigned char *)buffer, size);
        CigiProcessIncomingMsgBuffer(g_IGSession);

        // Pop and display the time delta between SOF and the host response time.
        size = g_SentCIGIMsgQueue.Pop(buffer, MAX_ETHERNET_PACKET_SIZE);
        if (size == 2 * sizeof(LARGE_INTEGER)) {
            LARGE_INTEGER *t = (LARGE_INTEGER *)buffer;
            CString t_str;

            // Update the history window.
            fr = (int)(t->QuadPart / 1000.0f);

            if (fr > max_fr)
                max_fr = fr;
            if (fr < min_fr)
                min_fr = fr;

            avg_fr += fr;

            if (heartbeat_counter++ % 5 == 0) {
                avg_fr /= 5;
                theApp.GetMainFrame().GetHistoryDlg().SetLineValue(0, max_fr);
                theApp.GetMainFrame().GetHistoryDlg().SetLineValue(1, min_fr);
                theApp.GetMainFrame().GetHistoryDlg().SetLineValue(2, avg_fr);
                theApp.GetMainFrame().GetHistoryDlg().Scroll();

                avg_fr = 0;
                max_fr = 0;
                min_fr = (unsigned int) - 1;
            }

            // Update the packet capture list.  The timing statistics only
            // make sense if we are capturing SOF and IG Control packets.
            if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()
                && (g_FilterFlags.IGControl || g_FilterFlags.StartOfFrame)) {
                t_str.Format("%.3lf", t->QuadPart / 10000.0f);  //("%I64d", t->QuadPart);
                theApp.GetMainFrame().GetWatchView().AddItem((LPCTSTR)t_str, COLUMN_DELTA_T_SOF, g_WatchFrameCounter);

                t++;
                t_str.Format("%.3lf", t->QuadPart / 10000.0f);  //("%I64d", t->QuadPart);
                theApp.GetMainFrame().GetWatchView().AddItem((LPCTSTR)t_str, COLUMN_RESPONSE_TIME, g_WatchFrameCounter);
            } else if (!theApp.GetMainFrame().IsWatchingThisFrame()) {
                // Do this here instead of in ProcessCigiIGControl() because we want to
                // be able to skip all of the packets plus the stats.
                theApp.GetMainFrame().WatchThisFrame();
            }
        }
    }
}


long DoCigiIGControl(const int sessionid, void *data)
{
    CIGI_IG_CONTROL *packet = (CIGI_IG_CONTROL *)data;

    // For CIGI 3.0/3.1, we will be grouping by Host-to-IG frame counter.
    // For later versions, we will be grouping by the last IG-to-Host frame
    // counter.
    if (GetCigiMinorVersion() == 0 ) {
        g_WatchFrameCounter = packet->last_ig_frame_num;
    }

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.IGControl) {
            CString str;

            if (GetCigiMinorVersion() == 0) {
                str.Format("IG Control: Host Frame Ctr=%lu (IG Frame Ctr=%lu)  "
                           "IG Mode=%d  Ver=%d.%d  DB=%d  Timestamp=%u",
                           packet->host_frame_num, packet->last_ig_frame_num,
                           packet->ig_mode, packet->cigi_version,
                           packet->minor_version, packet->db_number,
                           packet->timestamp);
            }

            if (packet->timestamp_valid)
                str += " (valid)";
            else
                str += " (invalid)";

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}


long DoCigiEntityPosition(const int sessionid, void *data)
{
    CIGI_ENTITY_POSITION *packet = (CIGI_ENTITY_POSITION *)data;
#if 0 // chas
    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.CCEntityControl) {
            CString str;
            str.Format("Clamped Entity Control: Entity ID=%lu  "
                       "Lat/X=%lf  Lon/Y=%lf  Yaw=%lf",
                       packet->entity_id, packet->latitude, packet->longitude,
                       packet->yaw);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }
#endif
    return 0;
}

long DoCigiCCEntityPosition(const int sessionid, void *data)
{
    CIGI_ENTITY_POSITION_CC *packet = (CIGI_ENTITY_POSITION_CC *)data;
#if 0 // chas
    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.CCEntityControl) {
            CString str;
            str.Format("Clamped Entity Control: Entity ID=%lu  "
                       "Lat/X=%lf  Lon/Y=%lf  Yaw=%lf",
                       packet->entity_id, packet->latitude, packet->longitude,
                       packet->yaw);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }
#endif
    return 0;
}

long DoCigiComponentControl(const int sessionid, void *data)
{
    CIGI_COMPONENT_CONTROL *packet = (CIGI_COMPONENT_CONTROL *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.ComponentControl) {
            CString str;

            const unsigned int componentClass = packet->component_class;

            str.Format("Component Control: Comp ID=%lu  "
                       "Instance ID=%lu  Comp Class=%lu  State=%lu  "
                       "Data1=%X  Data2=%X  Data3=%X  Data4=%X  Data5=%X  Data6=%X",
                       packet->component_id, packet->instance_id,
                       componentClass, packet->component_state,
                       packet->data1, packet->data2, packet->data3, packet->data4,
                       packet->data5, packet->data6);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiShortComponentControl(const int sessionid, void *data)
{
    CIGI_COMPONENT_CONTROL_S *packet = (CIGI_COMPONENT_CONTROL_S *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.ShortComponentControl) {
            CString str;

            const unsigned int componentClass = packet->component_class;

            str.Format("Short Component Control: Comp ID=%lu  "
                       "Instance ID=%lu  Comp Class=%lu  State=%lu  "
                       "Data1=%X  Data2=%X",
                       packet->component_id, packet->instance_id,
                       componentClass, packet->component_state,
                       packet->data1, packet->data2);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiArtPartControl(const int sessionid, void *data)
{
    static char enable_text[2][9] = {   "Disabled", "Enabled"   };

    CIGI_ARTPART_CONTROL *packet = (CIGI_ARTPART_CONTROL *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.ArtPartControl) {
            CString str;
            str.Format("Articulated Part Control: Entity ID=%lu  Part ID=%lu  "
                       "Enable=%d  X=%f (%s)  Y=%f (%s)  Z=%f (%s)  Yaw=%f (%s)  "
                       "Pitch=%f (%s)  Roll=%f (%s)",
                       packet->entity_id, packet->artpart_id,
                       packet->artpart_enable,
                       packet->xoffset, enable_text[packet->xoffset_enable],
                       packet->yoffset, enable_text[packet->yoffset_enable],
                       packet->zoffset, enable_text[packet->zoffset_enable],
                       packet->yaw, enable_text[packet->yaw_enable],
                       packet->pitch, enable_text[packet->pitch_enable],
                       packet->roll, enable_text[packet->roll_enable]);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiShortArtPartControl(const int sessionid, void *data)
{
    static char enable_text[2][9] = {   "Disabled", "Enabled"   };
    static char dof_text[7][12] = { "Unused Data",
                                    "X Offset",
                                    "Y Offset",
                                    "Z Offset",
                                    "Yaw",
                                    "Pitch",
                                    "Roll"
                                  };

    CIGI_ARTPART_CONTROL_S *packet = (CIGI_ARTPART_CONTROL_S *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.ShortArtPartControl) {
            CString str;
            str.Format("Short Articulated Part Control: Entity ID=%lu  Part 1:  ID=%lu  "
                       "Enable=%d   %s=%f  Part 2:  ID=%lu  Enable=%d   %s=%f",
                       packet->entity_id, packet->artpart1_id,
                       packet->artpart1_enable, dof_text[packet->dof1_select],
                       packet->dof1, packet->artpart2_id,
                       packet->artpart2_enable, dof_text[packet->dof2_select],
                       packet->dof2);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiRateControl(const int sessionid, void *data)
{
    CIGI_VELOCITY_CONTROL *packet = (CIGI_VELOCITY_CONTROL *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.RateControl) {
            CString str;

                str.Format("Rate Control: Entity ID=%lu  Part ID=%lu  "
                           "Linear Rate [X, Y, Z]=[%f, %f, %f]  "
                           "Angular Rate [Y, P, R]=[%f, %f, %f]  "
                           "Coordinate System = %d",
                           packet->entity_id, packet->artpart_id,
                           packet->x_rate, packet->y_rate, packet->z_rate,
                           packet->yaw_rate, packet->pitch_rate, packet->roll_rate,
                           packet->coord_system);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiCelestialSphereControl(const int sessionid, void *data)
{
    CIGI_CELESTIAL_SPHERE_CONTROL *packet = (CIGI_CELESTIAL_SPHERE_CONTROL *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.CelestialSphereControl) {
            CString str;
            str.Format("Celestial Sphere Control: Time=%d:%d  "
                       "Date=%.2d/%.2d/%.4d  Date/Time Valid=%d  "
                       "Ephemeris Enable=%d  Sun Enable=%d  Moon Enable=%d  "
                       "Starfield Enable=%d  Intensity=%f",
                       packet->hour, packet->minute,
                       packet->date / 1000000, (packet->date % 1000000) / 10000,
                       packet->date % 10000, packet->datetime_valid,
                       packet->ephemeris_enable,
                       packet->sun_enable,
                       packet->moon_enable,
                       packet->starfield_enable,
                       packet->starfield_intensity);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiAtmosphereControl(const int sessionid, void *data)
{
    static char enable_text[2][9] = {   "disabled", "enabled"   };

    CIGI_ATMOSPHERE_CONTROL *packet = (CIGI_ATMOSPHERE_CONTROL *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.AtmosphereControl) {
            CString str;
            str.Format("Atmosphere Control: Temp=%f  Humidity=%d  "
                       "Visibility=%f  Horz Windspeed=%f  Vert Windspeed=%f  "
                       "Wind Direction=%f  Baro Pressure=%f  "
                       "Atmospheric Model Enable=%d",
                       packet->global_air_temp, packet->global_humidity,
                       packet->global_visibility, packet->global_windspeed_horz,
                       packet->global_windspeed_vert, packet->global_wind_direction,
                       packet->global_baro_pressure, packet->atm_model_enable);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiEnvRegionControl(const int sessionid, void *data)
{
    CIGI_ENV_REGION_CONTROL *packet = (CIGI_ENV_REGION_CONTROL *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.EnvRegionControl) {
            CString str;
            str.Format("Env Region Control: ID=%d  State=%d  Latitude=%lf  "
                       "Longitude=%lf  Size [X, Y]=[%f, %f]  Rotation=%f  "
                       "Corner Radius=%f  Transition Perimeter=%f  "
                       "Merge Weather=%d  Merge Aerosols=%d  Merge Maritime=%d  "
                       "Merge Terrestrial=%d", packet->region_id,
                       packet->region_state, packet->latitude,
                       packet->longitude, packet->size_x, packet->size_y,
                       packet->rotation, packet->corner_radius,
                       packet->transition_perimeter, packet->merge_weather,
                       packet->merge_aerosols, packet->merge_maritime,
                       packet->merge_terrestrial);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiWeatherControlOpcode(const int sessionid, void *data)
{
    static char scope_text[3][7] = {    "Entity", "Region", "Entity"    };

    CIGI_WEATHER_CONTROL *packet = (CIGI_WEATHER_CONTROL *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.AtmosphereControl) {
            // We have to do this in two lines.
            CString str;
            str.Format("Weather Control: %s ID=%d  Layer ID=%d  "
                       "Enable=%d  Scope=%d  Severity=%d  Coverage=%f  "
                       "Cloud Type=%d  Air Temp=%.1f  Vis=%.1f  Humidity=%d  ",
                       scope_text[packet->scope],
                       packet->entity_region_id, packet->layer_id,
                       packet->weather_enable, packet->scope,
                       packet->severity, packet->coverage, packet->cloud_type,
                       packet->air_temp, packet->visibility, packet->humidity);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);

            str.Format("                 Base Elev=%.1f  "
                       "Thickness=%.1f  Random Lightning=%d  "
                       "Random Wind=%d  Horz Windspeed=%.1f  Vert Windspeed=%.1f  "
                       "Wind Dir=%.1f  Baro Pressure=%.1f  Aerosol=%.1f",
                       packet->base_elevation, packet->thickness,
// chas                        packet->transition_band,
                       packet->random_lightning,  packet->random_winds,
                       packet->windspeed_horz, packet->windspeed_vert,
                       packet->wind_direction, packet->baro_pressure,
                       packet->aerosol_concentration);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiMaritimeSurfaceControl(const int sessionid, void *data)
{
    static char scope_text[3][7] = {    "Entity", "Region", "Entity"    };

    CIGI_MARITIME_CONDITIONS_CONTROL *packet = (CIGI_MARITIME_CONDITIONS_CONTROL *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.MaritimeCondControl) {
            CString str;
            str.Format("Maritime Surface Cond Control: %s ID=%d  "
                       "Enable=%d  Scope=%d  Whitecap Enable=%d  "
                       "Surface Height=%f  Temp=%f  Clarity=%f",
                       scope_text[packet->scope],
                       packet->entity_region_id,
                       packet->conditions_enable, packet->scope,
                       packet->whitecap_enable, packet->surface_height,
                       packet->surface_temp, packet->surface_clarity);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiWaveControl(const int sessionid, void *data)
{
    static char scope_text[3][7] = {    "Entity", "Region", "Entity"    };

    CIGI_WAVE_CONTROL *packet = (CIGI_WAVE_CONTROL *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.WaveControl) {
            CString str;
            str.Format("Wave Control: %s ID=%d  Wave ID=%d  Enable=%d  Scope=%d  "
                       "Breaker Type=%d  Wave Height=%f  Wavelength=%f  "
                       "Period=%f  Direction=%f  Phase Offset=%f  Leading=%f",
                       scope_text[packet->scope], packet->entity_region_id,
                       packet->wave_id, packet->wave_enable, packet->scope,
                       packet->breaker_type, packet->wave_height,
                       packet->wavelength, packet->period, packet->direction,
                       packet->phase_offset, packet->leading);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiTerrestrialSurfaceControl(const int sessionid, void *data)
{
    static char scope_text[3][7] = {    "Entity", "Region", "Entity"    };

    CIGI_TERRESTRIAL_CONDITIONS_CONTROL *packet = (CIGI_TERRESTRIAL_CONDITIONS_CONTROL *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.TerrestrialCondControl) {
            CString str;
            str.Format("Maritime Surface Cond Control: %s ID=%d  "
                       "Condition ID=%d  Enable=%d  Scope=%d  Severity=%d  Coverage=%d",
                       scope_text[packet->scope],
                       packet->entity_region_id, packet->condition_id,
                       packet->condition_enable, packet->scope,
                       packet->severity, packet->coverage);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiViewControl(const int sessionid, void *data)
{
    static char enable_text[2][9] = {   "disabled", "enabled"   };

    CIGI_VIEW_CONTROL *packet = (CIGI_VIEW_CONTROL *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.ViewControl) {
            CString str;
            str.Format("View Control: View ID=%d  Group ID=%d  Entity ID=%d  "
                       "X Offset=%f (%s)  Y Offset=%f (%s)  "
                       "Z Offset=%f (%s)  Yaw=%f (%s)  "
                       "Pitch=%f (%s)  Roll=%f (%s)",
                       packet->view_id, packet->group_id, packet->entity_id,
                       packet->xoffset, enable_text[packet->xoffset_enable],
                       packet->yoffset, enable_text[packet->yoffset_enable],
                       packet->zoffset, enable_text[packet->zoffset_enable],
                       packet->yaw, enable_text[packet->yaw_enable],
                       packet->pitch, enable_text[packet->pitch_enable],
                       packet->roll, enable_text[packet->roll_enable]);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiSensorControl(const int sessionid, void *data)
{
    static char enable_text[2][4] = {   "Off", "On" };
    static char track_text[2][6] = {    "White", "Black"    };

    CIGI_SENSOR_CONTROL *packet = (CIGI_SENSOR_CONTROL *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.SensorControl) {
            CString str;
            str.Format("Sensor Control: View ID=%d  Sensor ID=%d  Resp Type=%d  "
                       "Sensor %s  Polarity=%d  Line-By-Line Dropout=%d  "
                       "Auto Gain=%d  Track %s  Track Mode=%d  "
                       "Gain=%f  Level=%f  AC Coupling=%f  Noise=%f",
                       packet->view_id, packet->sensor_id, packet->response_type,
                       enable_text[packet->sensor_on_off], packet->polarity,
                       packet->line_dropout, packet->auto_gain,
                       track_text[packet->track_white_black], packet->track_mode,
                       packet->gain, packet->level, packet->ac_coupling,
                       packet->noise);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiMotionTrackerControl(const int sessionid, void *data)
{
    static char type_text[2][7] = { "", "Group " };

    CIGI_MOTION_TRACKER_CONTROL *packet = (CIGI_MOTION_TRACKER_CONTROL *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.MotionTrackerControl) {
            CString str;
            str.Format("Motion Tracker Control: Tracker ID=%d  View %s ID=%d  "
                       "Tracker Enable=%d  Boresight Enable=%d  "
                       "X Enable=%d  Y Enable=%d  Z Enable=%d  "
                       "Yaw Enable=%d  Pitch Enable=%d  Roll Enable=%d  ",
                       packet->tracker_id, type_text[packet->view_or_group_select],
                       packet->view_or_group_id, packet->tracker_enable,
                       packet->boresight_enable, packet->x_enable,
                       packet->y_enable, packet->z_enable,
                       packet->yaw_enable, packet->pitch_enable,
                       packet->roll_enable);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiErmDef(const int sessionid, void *data)
{
    CIGI_ERM_DEF *packet = (CIGI_ERM_DEF *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.ErmDef) {
            CString str;
            str.Format("ERM Definition: Custom ERM Enable=%d  "
                       "Radius=%lf  Flattening=%lf",
                       packet->erm_enable, packet->equatorial_radius, packet->flattening);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiTrajectoryDef(const int sessionid, void *data)
{
    CIGI_ACCELERATION_CONTROL_DEF *packet = (CIGI_ACCELERATION_CONTROL_DEF *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.TrajectoryDef) {
            CString str;
            str.Format("Trajectory Definition: Entity ID=%d  "
                       "Acceleration [X, Y, Z]=[%f, %f, %f]",
                       packet->entity_id, packet->accel_x, packet->accel_y,
                       packet->accel_z);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiViewDef(const int sessionid, void *data)
{
    static char enable_text[2][9] = {   "disabled", "enabled"   };

    CIGI_VIEW_DEF *packet = (CIGI_VIEW_DEF *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.ViewDef) {
            CString str;
            str.Format("View Definition: View ID=%d  Group ID=%d  "
                       "Type=%d  Projection=%d  Replicate Mode=%d  "
                       "Mirror Mode=%d  Reorder=%d  "
                       "Left=%f (%s)  Right=%f (%s)  Top=%f (%s)  "
                       "Bottom=%f (%s)  Near=%f (%s)  Far=%f (%s)",
                       packet->view_id, packet->group_id, packet->view_type,
                       packet->projection_type, packet->pixel_replication_mode,
                       packet->mirror_mode, packet->reorder,
                       packet->fov_left, enable_text[packet->left_enable],
                       packet->fov_right, enable_text[packet->right_enable],
                       packet->fov_top, enable_text[packet->top_enable],
                       packet->fov_bottom, enable_text[packet->bottom_enable],
                       packet->fov_near, enable_text[packet->near_enable],
                       packet->fov_far, enable_text[packet->far_enable]);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiCollisionSegDef(const int sessionid, void *data)
{
    CIGI_COLL_SEGMENT_DEF *packet = (CIGI_COLL_SEGMENT_DEF *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.CDSegmentDef) {
            CString str;
            str.Format("Collision Seg Definition: Entity ID=%d  Segment ID=%d  "
                       "Enable=%d  Start (X, Y, Z)=(%f, %f, %f)  "
                       "End (X, Y, Z)=(%f, %f, %f)  Material Mask=%X",
                       packet->entity_id, packet->segment_id,
                       packet->segment_enable, packet->x1, packet->y1, packet->z1,
                       packet->x2, packet->y2, packet->z2, packet->material_mask);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiCollisionVolDef(const int sessionid, void *data)
{
    static char type_text[2][7] = { "Sphere", "Cuboid" };
    static char height_text[2][7] = { "Radius", "Height" };

    CIGI_COLL_VOLUME_DEF *packet = (CIGI_COLL_VOLUME_DEF *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.CDVolumeDef) {
            CString str;
            str.Format("Collision Vol Definition: Entity ID=%d  Volume ID=%d  "
                       "Enable=%d  Type=%s  Center (X, Y, Z)=(%f, %f, %f)  "
                       "Orientation [Y, P, R]=[%f, %f, %f]  %s=%f  "
                       "Width=%f  Depth=%f",
                       packet->entity_id, packet->volume_id,
                       packet->volume_enable, type_text[packet->volume_type],
                       packet->x, packet->y, packet->z,
                       packet->roll, packet->pitch, packet->yaw,
                       height_text[packet->volume_type],
                       packet->height_radius, packet->width, packet->depth);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiHatRequest(const int sessionid, void *data)
{
    static char type_text[3][4] = { "HAT", "HOT", "Ext" };
    static char coord_text[2][9] = { "Geodetic", "Entity" };
    static char latx_text[2][9] = { "Latitude", "X" };
    static char lony_text[2][10] = { "Longitude", "Y" };
    static char altz_text[2][9] = { "Altitude", "Z" };

    CIGI_HAT_HOT_REQUEST *packet = (CIGI_HAT_HOT_REQUEST *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.HatHotReq) {
            CString str;

                str.Format("HAT/HOT Request: Type=%s  Entity ID=%d  %s ID=%d  "
                           "Coord System=%s  %s=%lf  %s=%lf  %s=%lf  Update Period=%d",
                           type_text[packet->request_type], packet->entity_id,
                           type_text[packet->request_type], packet->hat_hot_id,
                           coord_text[packet->coord_system],
                           latx_text[packet->coord_system], packet->lat_x,
                           lony_text[packet->coord_system], packet->lon_y,
                           altz_text[packet->coord_system], packet->alt_z,
                           packet->update_period);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiLosSegmentRequest(const int sessionid, void *data)
{
    static char type_text[2][6] = { "Basic", "Ext" };
    static char coord_text[2][9] = { "Geodetic", "Entity" };
    static char latx_text[2][4] = { "Lat", "X" };
    static char lony_text[2][4] = { "Lon", "Y" };
    static char altz_text[2][4] = { "Alt", "Z" };
    static char valid_text[2][8] = { "invalid", "valid" };

    CIGI_LOS_SEGMENT_REQUEST *packet = (CIGI_LOS_SEGMENT_REQUEST *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.LOSSegmentReq) {
            CString str;
                str.Format("LOS Segment Request: Type=%s   LOS ID=%d  "
                           "Source (%s, %s, %s)=(%lf, %lf, %lf)  "
                           "Dest Entity ID=%d (%s)  "
                           "Dest (%s, %s, %s)=(%lf, %lf, %lf)  "
                           "Resp Coord System=%s  Alpha Threshold=%d  Material Mask=%X  "
                           "Update Period=%d",
                           type_text[packet->request_type], // chas packet->entity_id,
                           packet->los_id,
                           latx_text[packet->source_coord_system],
                           lony_text[packet->source_coord_system],
                           altz_text[packet->source_coord_system],
                           packet->source_lat_x, packet->source_lon_y, packet->source_alt_z,
                           packet->dest_entity_id,
                           valid_text[packet->dest_entity_id_valid],
                           latx_text[packet->dest_coord_system],
                           lony_text[packet->dest_coord_system],
                           altz_text[packet->dest_coord_system],
                           packet->dest_lat_x, packet->dest_lon_y, packet->dest_alt_z,
                           coord_text[packet->response_coord_system],
                           packet->alpha_threshold, packet->material_mask,
                           packet->update_period);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiLosVectorRequest(const int sessionid, void *data)
{
    static char type_text[2][6] = { "Basic", "Ext" };
    static char coord_text[2][9] = { "Geodetic", "Entity" };
    static char latx_text[2][4] = { "Lat", "X" };
    static char lony_text[2][4] = { "Lon", "Y" };
    static char altz_text[2][4] = { "Alt", "Z" };

    CIGI_LOS_VECTOR_REQUEST *packet = (CIGI_LOS_VECTOR_REQUEST *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.LOSVectorReq) {
            CString str;

                str.Format("LOS Vector Request: Type=%s  Entity ID=%d  LOS ID=%d  "
                           "Source (%s, %s, %s)=(%lf, %lf, %lf)  "
                           "Azimuth=%f  Elevation=%f  Min Range=%f  Max Range=%f  "
                           "Resp Coord System=%s  Alpha Threshold=%d  Material Mask=%X  ",
                           type_text[packet->request_type], packet->entity_id,
                           packet->los_id,
                           latx_text[packet->source_coord_system],
                           lony_text[packet->source_coord_system],
                           altz_text[packet->source_coord_system],
                           packet->source_lat_x, packet->source_lon_y, packet->source_alt_z,
                           packet->azimuth, packet->elevation, packet->min_range,
                           packet->max_range,
                           coord_text[packet->resp_coord_system],
                           packet->alpha_threshold, packet->material_mask);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiPositionRequest(const int sessionid, void *data)
{
    static char coord_text[3][9] = {    "Geodetic",
                                        "Parent",
                                        "Submodel"
                                   };
    static char id_text[5][11] = {  "Entity",
                                    "Entity",
                                    "View",
                                    "View Group",
                                    "Tracker"
                                 };
    static char class_text[5][11] = {   "Entity",
                                        "Art Part",
                                        "View",
                                        "View Group",
                                        "Tracker"
                                    };
    static char mode_text[2][11] = {    "One-Shot",
                                        "Continuous"
                                   };

    CIGI_POSITION_REQUEST *packet = (CIGI_POSITION_REQUEST *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.PositionReq) {
            CString str;
            str.Format("Position Request: Object Class=%s  %s ID=%d  "
                       "Part ID=%d  Coord System=%s  Update Mode=%s",
                       class_text[packet->object_class],
                       id_text[packet->object_class], packet->object_id,
                       packet->artpart_id, coord_text[packet->coord_system],
                       mode_text[packet->update_mode]);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiEnvConditionsRequest(const int sessionid, void *data)
{
    CIGI_ENV_CONDITIONS_REQUEST *packet = (CIGI_ENV_CONDITIONS_REQUEST *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.EnvConditionsReq) {
            CString str;
            str.Format("Env Conditions Request: Request Type=%d  Request ID=%d  "
                       "Latitude=%lf  Longitude=%lf  Altitude=%lf",
                       packet->request_type, packet->request_id,
                       packet->latitude, packet->longitude, packet->altitude);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiSymbolSurfaceDef(const int sessionid, void *data)
{
    CIGI_SYMBOL_SURFACE_DEF *packet = (CIGI_SYMBOL_SURFACE_DEF *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.SymbolSurfaceDef) {
            CString str;
            str.Format("Symbol Surface Definition: Surface ID=%lu  "
                       "Surface State=%lu  Attach Type=%lu  Billboard=%lu  "
                       "Persp Growth=%lu  Entity/View ID=%lu  X/Left=%f  "
                       "Y/Right=%f  Z/Top=%f  Yaw/Bottom=%f  Pitch=%f  "
                       "Roll=%f",
                       packet->surface_id, packet->surface_state,
                       packet->attach_type, packet->billboard,
                       packet->persp_growth_enable,
                       packet->entity_or_view_id,
                       packet->x_left, packet->y_right,
                       packet->z_top, packet->yaw_bottom,
                       packet->pitch, packet->roll);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);

            str.Format("               Width=%f  Height=%f  Min U=%f  Max U=%f  "
                       "Min V=%f  Max V=%f",
                       packet->width, packet->height,
                       packet->min_u, packet->max_u,
                       packet->min_v, packet->max_v);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiSymbolTextDef(const int sessionid, void *data)
{
    CIGI_SYMBOL_TEXT_DEF *packet = (CIGI_SYMBOL_TEXT_DEF *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.SymbolTextDef) {
            CString str;
            str.Format("Symbol Text Definition: Symbol ID=%lu  "
                       "Alignment=%lu  Orientation=%lu  Font ID=%lu  Font Size=%f",
                       packet->symbol_id, packet->alignment, packet->orientation,
                       packet->font_id, packet->font_size);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);

            str.Format("               \"%s\"", packet->text);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiSymbolCircleDef(const int sessionid, void *data)
{
    CIGI_SYMBOL_CIRCLE_DEF *packet = (CIGI_SYMBOL_CIRCLE_DEF *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.SymbolCircleDef) {
            CString str;
            str.Format("Symbol Circle Definition: Symbol ID=%lu  "
                       "Style=%lu  Line Width=%f  Stipple Pattern Length=%f  "
                       "Stipple Pattern=%X",
                       packet->symbol_id, packet->drawing_style, packet->line_width,
                       packet->stipple_pattern_length, packet->stipple_pattern);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);

            int numCircles = (packet->packet_size - 16) / 24;
            for (int i = 0; i < numCircles; ++i) {
                str.Format("               Circle %d:  Center=(%f, %f)  Radius=%f  "
                           "Inner Radius=%f  Start Angle=%f  End Angle=%f",
                           i, packet->circle[i].center_u, packet->circle[i].center_v,
                           packet->circle[i].radius, packet->circle[i].inner_radius,
                           packet->circle[i].start_angle, packet->circle[i].end_angle);

                theApp.GetMainFrame().GetWatchView().
                AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
            }
        }
    }

    return 0;
}

long DoCigiSymbolLineDef(const int sessionid, void *data)
{
    CIGI_SYMBOL_POLYGON_DEF *packet = (CIGI_SYMBOL_POLYGON_DEF *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.SymbolLineDef) {
            CString str;
            str.Format("Symbol Line Definition: Symbol ID=%lu  "
                       "Primitive=%lu  Line Width=%f  Stipple Pattern Length=%f  "
                       "Stipple Pattern=%X",
                       packet->symbol_id, packet->primitive_type, packet->line_width,
                       packet->stipple_pattern_length, packet->stipple_pattern);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);

            int numLines = (packet->packet_size - 16) / 8;
            for (int i = 0; i < numLines; ++i) {
                str.Format("               Vertex %d:  Center=(%f, %f)",
                           i, packet->line[i].vertex_u, packet->line[i].vertex_v);

                theApp.GetMainFrame().GetWatchView().
                AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
            }
        }
    }

    return 0;
}

long DoCigiSymbolClone(const int sessionid, void *data)
{
    CIGI_SYMBOL_CLONE *packet = (CIGI_SYMBOL_CLONE *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.SymbolClone) {
            CString str;
            str.Format("Symbol Clone: Symbol ID=%lu  Source Type=%lu  Source ID=%lu",
                       packet->symbol_id, packet->source_type, packet->source_id);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiSymbolControl(const int sessionid, void *data)
{
    CIGI_SYMBOL_CONTROL *packet = (CIGI_SYMBOL_CONTROL *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.SymbolControl) {
            CString str;
            str.Format("Symbol Control: Symbol ID=%lu  "
                       "Symbol State=%lu  Attach State=%lu  Reset Flash=%lu  "
                       "Inherit Color=%lu  Parent ID=%lu  Surface ID=%u  "
                       "Layer=%u  Flash Duty Cycle=%u%%  Flash Period=%f  ",
                       packet->symbol_id, packet->symbol_state,
                       packet->attach_state, packet->reset_flash,
                       packet->inherit_color, packet->parent_id,
                       packet->surface_id, packet->layer,
                       packet->flash_duty_cycle, packet->flash_period);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);

            str.Format("               Position=(%f, %f)  Rotation=%f  "
                       "Scale=(%f, %f)  Red=%u  Green=%u  Blue=%u  Alpha=%u",
                       packet->pos_u, packet->pos_v, packet->rotation,
                       packet->scale_u, packet->scale_v, packet->red,
                       packet->green, packet->blue, packet->alpha);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiShortSymbolControl(const int sessionid, void *data)
{
    const char label[][24] = { "(Not used)",
                               "Surface ID",
                               "Parent Symbol ID",
                               "Layer",
                               "Flash Duty Cycle",
                               "Flash Period",
                               "Position U",
                               "Position V",
                               "Rotation",
                               "Color",
                               "Scale U",
                               "Scale V"
                             };

    CIGI_SYMBOL_CONTROL_S *packet = (CIGI_SYMBOL_CONTROL_S *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.ShortSymbolControl) {
            CString str;
            str.Format("Short Symbol Control: Symbol ID=%lu  "
                       "Symbol State=%lu  Attach State=%lu  Reset Flash=%lu  "
                       "Inherit Color=%lu  %s=",
                       packet->symbol_id, packet->symbol_state,
                       packet->attach_state, packet->reset_flash,
                       packet->inherit_color, label[packet->attribute1]);

            CString temp;
            if ((packet->attribute1) == 9 || (packet->attribute1 <= 4))
                temp.Format("%lu  %s=", packet->attribute1_value, label[packet->attribute2]);
            else
                temp.Format("%f  %s=", *((float *)&packet->attribute1_value), label[packet->attribute2]);

            str += temp;

            if ((packet->attribute2) == 9 || (packet->attribute2 <= 4))
                temp.Format("%lu", packet->attribute2_value);
            else
                temp.Format("%f", *((float *)&packet->attribute2_value));

            str += temp;

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }

    return 0;
}

long DoCigiEntityControl(const int sessionid, void *data)
{
    CIGI_ENTITY_CONTROL *packet = (CIGI_ENTITY_CONTROL *)data;

#if 0 // chas
    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.EntityControl) {
            CString str;
            str.Format("Entity Control: Entity ID=%lu  Type=%d  State=%d  "
                       "Attached=%d  Parent=%d  Alpha=%d  Inherit=%d",
                       packet->entity_id, packet->type, packet->state,
                       packet->attach_state, packet->parent_id,
                       packet->alpha, packet->inherit_alpha);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);

            str.Format("               Lat/X=%lf  Lon/Y=%lf  Alt/Z=%lf  "
                       "Yaw=%lf  Pitch=%lf  Roll=%lf",
                       packet->lat_x, packet->lon_y, packet->alt_z,
                       packet->yaw, packet->pitch, packet->roll);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);

            str.Format("               CD Enable=%d  Clamp=%d  "
                       "Anim Dir=%d  Anim Loop=%d  Anim State=%d",
                       packet->collision_detect, packet->clamp_mode,
                       packet->anim_direction, packet->anim_loop_mode, packet->anim_state);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }
#endif
    return 0;
}

long DoCigiAnimationControl(const int sessionid, void *data)
{
    CIGI_ANIMATION_CONTROL *packet = (CIGI_ANIMATION_CONTROL *)data;

#if 0 // chas
    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.EntityControl) {
            CString str;
            str.Format("Entity Control: Entity ID=%lu  Type=%d  State=%d  "
                       "Attached=%d  Parent=%d  Alpha=%d  Inherit=%d",
                       packet->entity_id, packet->type, packet->state,
                       packet->attach_state, packet->parent_id,
                       packet->alpha, packet->inherit_alpha);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);

            str.Format("               Lat/X=%lf  Lon/Y=%lf  Alt/Z=%lf  "
                       "Yaw=%lf  Pitch=%lf  Roll=%lf",
                       packet->lat_x, packet->lon_y, packet->alt_z,
                       packet->yaw, packet->pitch, packet->roll);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);

            str.Format("               CD Enable=%d  Clamp=%d  "
                       "Anim Dir=%d  Anim Loop=%d  Anim State=%d",
                       packet->collision_detect, packet->clamp_mode,
                       packet->anim_direction, packet->anim_loop_mode, packet->anim_state);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_HOST_TO_IG, g_WatchFrameCounter);
        }
    }
#endif
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//  IG-to-Host
//
///////////////////////////////////////////////////////////////////////////////

void ProcessIncomingCigiPacketQueue(void)
{
    static char buffer[CIGI_MAX_PACKET_SIZE] = {0};
    static CIGI_DUMMY_PACKET *packet = NULL;
    int rcvsize;

    while ((rcvsize = g_RcvCIGIMsgQueue.Pop(buffer, sizeof(buffer))) > 0) {
        // Pop the CIGI message and process it.
        CigiSetIncomingMsgBuffer(g_HostSession, (unsigned char *)buffer, rcvsize);
        CigiProcessIncomingMsgBuffer(g_HostSession);
    }
}

long DoSkippedFrame(const int sessionid, void *data)
{
    CIGI_DUMMY_PACKET *packet = (CIGI_DUMMY_PACKET *)data;
    int frames = packet->data2;

    // If the number of skipped frames is large, just assume
    // we have reconnected.
    if (frames > 1000)
        return 0;

    // Otherwise, invalidate the frame counter so we will alternate
    // row colors.
    g_FrameCounter = (unsigned long) - 1;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.StartOfFrame) {
            CString str;
            str.Format("Missed %d Start of Frame packet(s)", frames);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter, TRUE);
        }
    }

    return 0;
}

long DoCigiStartOfFrame(const int sessionid, void *data)
{
    static int PrevIGMode = -1;
    CIGI_START_OF_FRAME *packet = (CIGI_START_OF_FRAME *)data;

    // Keep up with the frame counter so we can store it in the list's lParam
    // with each packet item.
    g_FrameCounter = packet->frame_ctr;

    // Store the minor version.
    //g_CigiMinorVersion = packet->sof32.minor_version;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.StartOfFrame) {
            CString str;

            if (GetCigiMinorVersion() == 0 ) {
                 str.Format("Start of Frame: Ver=%d.%d  DB=%d  IG Mode=%d   ERM=%d  IG Frame Ctr=%lu  Timestamp=%.3fms  (Last Host Frame Ctr=%lu)  OverframingCondition=%d  PagingCondition=%d  ExcessVarLenDataCondition=%d",
packet->cigi_major_version, packet->minor_version,
packet->db_number,
packet->ig_mode,
packet->earth_ref_model,
packet->frame_ctr, 
packet->timestamp / 1000.0 , 
packet->last_frame_ctr,
packet->OverframingCondition,
packet->PagingCondition,
packet->ExcessVarLenDataCondition );



            }

            if (packet->timestamp_valid)
                str += " TimeStamp (valid)";
            else
                str += " TimeStamp (not valid)";

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    // Only look at IG Mode if it has changed.
    if ((int)(packet->ig_mode) != PrevIGMode) {
        PrevIGMode = packet->ig_mode;

        switch (PrevIGMode) {
        case 0:
            SetResetIGFlag(TRUE);
            SetDebugIGFlag(FALSE);
            theApp.GetMainFrame().SetIGModeStatusText("IG Mode: Standby/Reset");
            break;

        case 1:
            SetResetIGFlag(FALSE);
            SetDebugIGFlag(FALSE);
            theApp.GetMainFrame().SetIGModeStatusText("IG Mode: Operate");
            break;

        case 2:
            SetDebugIGFlag(TRUE);
            theApp.GetMainFrame().SetIGModeStatusText("IG Mode: Debug");
            break;

        case 3:
            SetDebugIGFlag(TRUE);
            theApp.GetMainFrame().SetIGModeStatusText("IG Mode: Offline");
            break;

        default:
            theApp.GetMainFrame().SetIGModeStatusText("IG Mode: Unknown");
            break;
        }
    }

    return 0;
}

long DoCigiHatResponse(const int sessionid, void *data)
{
    static char type_text[][4] = {  "HAT",
                                    "HOT"
                                 };

    CIGI_HAT_HOT_RESPONSE *packet = (CIGI_HAT_HOT_RESPONSE *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.HatResp) {
            CString str;

                str.Format("HAT/HOT Response: HAT/HOT ID=%u  Type=%s  Height=%lf  "
                           "Valid=%d  Host Frame Number LSN=%d",
                           packet->hat_hot_id, type_text[packet->response_type],
                           packet->height, packet->valid, packet->frame_num_lsn);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    CHat *hat = g_DataManager.LookupHat(packet->hat_hot_id >> 6);
    if (!hat)
        return 0;

    HAT_RESPONSE hat_resp = hat->GetRespData();
    hat_resp.resp_type = packet->response_type;     // 0 = HAT, 1 = HOT
    hat_resp.valid = packet->valid;
    hat_resp.material_code = 0;
    hat_resp.azimuth = 0.0;
    hat_resp.elevation = 0.0;

    if (packet->response_type == 0) {
        hat_resp.hat = packet->height;
        hat_resp.hot = 0.0;
    } else {
        hat_resp.hat = 0.0;
        hat_resp.hot = packet->height;
    }

    hat->SetRespData(hat_resp);

    return 0;
}

long DoCigiHatExtResponse(const int sessionid, void *data)
{
    CIGI_HAT_HOT_EXT_RESPONSE *packet = (CIGI_HAT_HOT_EXT_RESPONSE *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.HatExtResp) {
            CString str;

                str.Format("HAT/HOT Ext Response: HAT/HOT ID=%u  HAT=%lf  HOT=%lf  "
                           "Valid=%d  Normal Az=%f  Normal El=%lf  Material Code=%X  "
                           "Host Frame Number LSN=%d",
                           packet->hat_hot_id, packet->hat, packet->hot, packet->valid,
                           packet->normal_azimuth, packet->normal_elevation, packet->material_code,
                           packet->frame_num_lsn);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    CHat *hat = g_DataManager.LookupHat(packet->hat_hot_id >> 6);
    if (!hat)
        return 0;

    HAT_RESPONSE hat_resp = hat->GetRespData();
    hat_resp.resp_type = 2;                     // Extended HAT/HOT Response
    hat_resp.valid = packet->valid;
    hat_resp.material_code = packet->material_code;
    hat_resp.azimuth = packet->normal_azimuth;
    hat_resp.elevation = packet->normal_elevation;
    hat_resp.hat = packet->hat;
    hat_resp.hot = packet->hot;

    hat->SetRespData(hat_resp);

    return 0;
}

long DoCigiLosResponse(const int sessionid, void *data)
{
    static char valid_text[][12] = {    "(not valid)",
                                        "(valid)"
                                   };

    CIGI_LOS_RESPONSE *packet = (CIGI_LOS_RESPONSE *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.LosResp) {
            CString str;

                str.Format("LOS Response: LOS ID=%u  Range=%lf  Visible=%d  Valid=%d  "
                           "Entity=%d %s  Response Count=%d  Host Frame Number LSN=%d",
                           packet->los_id, packet->range, packet->visible, packet->valid,
                           packet->entity_id, valid_text[packet->entity_id_valid],
                           packet->response_count, packet->frame_num_lsn);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    CLos *los = g_DataManager.LookupLos(packet->los_id >> 6);
    if (!los)
        return 0;

    LOS_RESPONSE los_resp = los->GetRespData();
    los_resp.resp_type = 0;             // Basic
    los_resp.valid = packet->valid;
    los_resp.range_valid = packet->valid;           // Same as Valid for this packet
    los_resp.entity_id_valid = packet->entity_id_valid;
    los_resp.visible = packet->visible;
    los_resp.response_count = packet->response_count;
    los_resp.entity_id = packet->entity_id;
    los_resp.range = packet->range;
    los_resp.coord_system = 0;
    los_resp.material_code = 0;
    los_resp.red = 0;
    los_resp.green = 0;
    los_resp.blue = 0;
    los_resp.alpha = 0;
    los_resp.azimuth = 0.0;
    los_resp.elevation = 0.0;
    los_resp.lat_x = 0.0;
    los_resp.lon_y = 0.0;
    los_resp.alt_z = 0.0;

    los->SetRespData(los_resp);

    return 0;
}

long DoCigiLosExtResponse(const int sessionid, void *data)
{
    static char valid_text[][12] = {    "(not valid)",
                                        "(valid)"
                                   };

    CIGI_LOS_EXT_RESPONSE *packet = (CIGI_LOS_EXT_RESPONSE *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.LosExtResp) {
            CString str;
            CString str2;

            // First line:
            str.Format("LOS Ext Response: LOS ID=%u  Visible=%d  Valid=%d  Range=%lf ",
                       "%s  Entity=%d %s  Lat/X=%lf  Lon/Y=%lf  Alt/Z=%lf",
                       packet->los_id, packet->visible, packet->valid,
                       packet->range, valid_text[packet->range_valid], packet->entity_id,
                       valid_text[packet->entity_id_valid],
                       packet->lat_x, packet->lon_y, packet->alt_z);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);

            // Second line:
            str.Format("                 Normal Az=%f  Normal El=%lf  Material Code=%X  "
                       "Red=%d  Green=%d  Blue=%d  Alpha=%d  Resp Count=%d",
                       packet->normal_azimuth, packet->normal_elevation, packet->material_code,
                       packet->red, packet->green, packet->blue, packet->alpha,
                       packet->response_count);

                str2.Format("  Host Frame Number LSN=%d", packet->frame_num_lsn);

                str += str2;

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    CLos *los = g_DataManager.LookupLos(packet->los_id >> 6);
    if (!los)
        return 0;

    LOS_RESPONSE los_resp = los->GetRespData();
    los_resp.resp_type = 1;         // Extended;
    los_resp.valid = packet->valid;
    los_resp.range_valid = packet->valid;
    los_resp.entity_id_valid = packet->entity_id_valid;
    los_resp.visible = packet->visible;
    los_resp.response_count = packet->response_count;
    los_resp.entity_id = packet->entity_id;
    los_resp.range = packet->range;
    los_resp.material_code = packet->material_code;
    los_resp.red = packet->red;
    los_resp.green = packet->green;
    los_resp.blue = packet->blue;
    los_resp.alpha = packet->alpha;
    los_resp.azimuth = packet->normal_azimuth;
    los_resp.elevation = packet->normal_elevation;
    los_resp.lat_x = packet->lat_x;
    los_resp.lon_y = packet->lon_y;
    los_resp.alt_z = packet->alt_z;

    // LWD:  Update this to CIGI 3.2 fields:
    los_resp.coord_system = 0;

    los->SetRespData(los_resp);

    return 0;
}

long DoCigiSensorResponse(const int sessionid, void *data)
{
    CIGI_SENSOR_RESPONSE *packet = (CIGI_SENSOR_RESPONSE *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.SensorResp) {
            CString str;
            str.Format("Sensor Response: View ID=%u  Sensor ID=%u  Status=%u  "
                       "Gate Size X=%d  Gate Size Y=%d  Gate Pos X=%d  "
                       "Gate Pos Y=%d=%X  Host Frame Number=%lu",
                       packet->view_id, packet->sensor_id, packet->sensor_status,
                       packet->gate_x_size, packet->gate_y_size, packet->gate_x_position,
                       packet->gate_y_position, packet->frame_counter);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    return 0;
}

long DoCigiSensorExtResponse(const int sessionid, void *data)
{
    static char valid_text[][12] = {    "(not valid)",
                                        "(valid)"
                                   };

    CIGI_SENSOR_EXT_RESPONSE *packet = (CIGI_SENSOR_EXT_RESPONSE *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.SensorExtResp) {
            CString str;
            str.Format("Sensor Ext Response: View ID=%u  Sensor ID=%u  Status=%u  "
                       "Entity ID=%d %s  Latitude=%lf  Longitude=%lf  Altitude=%lf  "
                       "Gate Size X=%d  Gate Size Y=%d  Gate Pos X=%d  "
                       "Gate Pos Y=%d=%X  Host Frame Number=%lu",
                       packet->view_id, packet->sensor_id,
                       packet->sensor_status, packet->entity_id,
                       valid_text[packet->entity_id_valid],
                       packet->lat, packet->lon, packet->alt,
                       packet->gate_x_size, packet->gate_y_size, packet->gate_x_position,
                       packet->gate_y_position, packet->frame_counter);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    return 0;
}

long DoCigiPositionResponse(const int sessionid, void *data)
{
    static char obj_class[][17] = { "Entity",
                                    "Articulated Part",
                                    "View",
                                    "View Group",
                                    "Motion Tracker"
                                  };

    static char coord_system_text[][14] = { "Geodetic",
                                            "Parent Entity",
                                            "Submodel"
                                          };

    CIGI_POSITION_RESPONSE *packet = (CIGI_POSITION_RESPONSE *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.SensorExtResp) {
            CString str;
            CString str2;

            str.Format("Position Response: Object Class=%d  %s ID=%u  Art Part ID=%u  "
                       "Coord System=%u (%s)  ", packet->object_class,
                       obj_class[packet->object_class], packet->object_id,
                       packet->artpart_id, packet->coord_system,
                       coord_system_text[packet->coord_system]);

            if (packet->coord_system == 0) {
                str2.Format("Latitude=%lf  Longitude=%lf  Altitude=%lf  ",
                            packet->lat_x, packet->lon_y, packet->alt_z);
            } else {
                str2.Format("X Offset=%lf  Y Offset=%lf  Z Offset=%lf  ",
                            packet->lat_x, packet->lon_y, packet->alt_z);
            }
            str += str2;

            str2.Format("Roll=%f  Pitch=%f  Yaw=%f", packet->roll, packet->pitch,
                        packet->yaw);
            str += str2;

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    return 0;
}

long DoCigiWeatherResponse(const int sessionid, void *data)
{
    CIGI_WEATHER_RESPONSE *packet = (CIGI_WEATHER_RESPONSE *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.WeatherCondResp) {
            CString str;
            str.Format("Weather Response: Request ID=%d  Humidity=%d  "
                       "Air Temp=%f  Visibility=%f  Windspeed (horz)=%f  "
                       "Windspeed (vert)=%f  Wind Direction=%f  "
                       "Barometric Pressure=%f", packet->request_id,
                       packet->humidity, packet->air_temp, packet->visibility,
                       packet->windspeed_horz, packet->windspeed_vert,
                       packet->wind_direction, packet->baro_pressure);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    return 0;
}

long DoCigiAerosolResponse(const int sessionid, void *data)
{
    CIGI_AEROSOL_RESPONSE *packet = (CIGI_AEROSOL_RESPONSE *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.AerosolResp) {
            CString str;
            str.Format("Aerosol Conc Response: Request ID=%d  Layer ID=%d  "
                       "Concentration=%f", packet->request_id,
                       packet->layer_id, packet->concentration);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    return 0;
}

long DoCigiMaritimeSurfaceResponse(const int sessionid, void *data)
{
    CIGI_MARITIME_SURFACE_RESPONSE *packet = (CIGI_MARITIME_SURFACE_RESPONSE *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.MaritimeCondResp) {
            CString str;
            str.Format("Maritime Surface Response: Request ID=%d  "
                       "Sea Surface Height=%f  Surface Temp=%f  Clarity=%f",
                       packet->request_id, packet->surface_height,
                       packet->surface_temp, packet->surface_clarity);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    return 0;
}

long DoCigiTerrestrialSurfaceResponse(const int sessionid, void *data)
{
    CIGI_TERRESTRIAL_SURFACE_RESPONSE *packet = (CIGI_TERRESTRIAL_SURFACE_RESPONSE *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.TerrestrialCondResp) {
            CString str;
            str.Format("Terrestrial Surface Response: Request ID=%d  "
                       "Condition ID=%d", packet->request_id, packet->condition_id);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    return 0;
}

long DoCigiCollSegmentNotification(const int sessionid, void *data)
{
    static char coll_type_text[][11] = {    "Non-entity",
                                            "Entity"
                                       };

    CIGI_COLL_SEGMENT_NOTIFICATION *packet = (CIGI_COLL_SEGMENT_NOTIFICATION *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.CDSegmentNotification) {
            CString str;
            str.Format("Collision Seg Notification: Entity ID=%d  "
                       "Segment ID=%d  Collision Type=%d (%s)  "
                       "Contacted Entity=%d  Material Code=%X  Intersection Dist=%f",
                       packet->entity_id, packet->segment_id, packet->collision_type,
                       coll_type_text[packet->collision_type],
                       packet->contacted_entity_id, packet->material_code,
                       packet->intersection_distance);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    // If the entity is selected, set the text in the Collision Detection tab page.
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return 0;

    int sel_entity = entity->GetID();
    if (packet->entity_id == sel_entity) {
        theApp.GetMainFrame().GetEntityStateView().
        GetCollDetPropPage().AddCollisionDataItem(packet);
    }

    return 0;
}

long DoCigiCollVolumeNotification(const int sessionid, void *data)
{
    static char coll_type_text[][11] = {    "Non-entity",
                                            "Entity"
                                       };

    CIGI_COLL_VOLUME_NOTIFICATION *packet = (CIGI_COLL_VOLUME_NOTIFICATION *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.CDVolumeNotification) {
            CString str;
            str.Format("Collision Vol Notification: Entity ID=%d  "
                       "Volume ID=%d  Collision Type=%d (%s)  "
                       "Contacted Entity=%d  Contacted Volume ID=%d",
                       packet->entity_id, packet->volume_id, packet->collision_type,
                       coll_type_text[packet->collision_type],
                       packet->contacted_entity_id, packet->contacted_volume_id);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    // If the entity is selected, set the text in the Collision Detection tab page.
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return 0;

    int sel_entity = entity->GetID();
    if (packet->entity_id == sel_entity) {
        theApp.GetMainFrame().GetEntityStateView().
        GetCollDetPropPage().AddCollisionDataItem(packet);
    }

    return 0;
}

long DoCigiAnimStopNotification(const int sessionid, void *data)
{
    CIGI_ANIMATION_STOP_NOTIFICATION *packet = (CIGI_ANIMATION_STOP_NOTIFICATION *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.AnimationStopNotification) {
            CString str;
            str.Format("Animation Stop Notification: Entity ID=%d",
                       packet->entity_id);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    // Set the entity's animation state.
    CEntity *entity = g_DataManager.GetEntity(packet->entity_id);
    if (entity)
        entity->SetAnimState(ENTITY_ANIM_STATE_STOP);
    else {
        CString errmsg;
        errmsg.Format("Received Animation Stop Notification message for non-existent entity %d",
                      packet->entity_id);
        PrintMessageText((LPCTSTR)errmsg);
    }

    return 0;
}

long DoCigiEventNotification(const int sessionid, void *data)
{
    CIGI_EVENT_NOTIFICATION *packet = (CIGI_EVENT_NOTIFICATION *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.EventNotification) {
            CString str;
            str.Format("Event Notification: Event ID=%d  Data1=%X  Data2=%X  "
                       "Data3=%X", packet->event_id, packet->data1, packet->data2,
                       packet->data3);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    return 0;
}
long DoCigiIGMessage(const int sessionid, void *data)
{
    CIGI_IG_MESSAGE *packet = (CIGI_IG_MESSAGE *)data;

    // Add the message to the list in the CIGI Debugging window.
    if (GetPacketWatchFlag() && theApp.GetMainFrame().IsWatchingThisFrame()) {
        // Add the message to the list box only if the filter is set.
        if (g_FilterFlags.IGMessage) {
            CString str;
            str.Format("IG Message: Message ID=%d  \"%s\"", packet->message_id,
                       packet->text);

            theApp.GetMainFrame().GetWatchView().
            AddItem((LPCTSTR)str, COLUMN_IG_TO_HOST, g_FrameCounter);
        }
    }

    theApp.GetMainFrame().GetMessageView().PrintMessage(packet->text);

    return 0;
}
