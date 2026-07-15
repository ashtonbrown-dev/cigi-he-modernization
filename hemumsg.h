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

#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include "HemuDrv/StdAfx.h"

#define MESSAGE_MAX_SIZE            sizeof(MESSAGE_BEGIN_RECORD)

#define MSG_GENERIC                     0x00

// Win32-to-Driver Messages
#define MSG_ADD_ENTITY                     MSG_GENERIC + 1
#define MSG_DEL_ENTITY                     MSG_GENERIC + 2
#define MSG_CLEAR_ENTITIES                 MSG_GENERIC + 5
#define MSG_ADD_VIEW                          MSG_GENERIC + 6
#define MSG_DEL_VIEW                          MSG_GENERIC + 7
#define MSG_CLEAR_VIEWS                    MSG_GENERIC + 9
#define MSG_ADD_VIEWGROUP                  MSG_GENERIC + 10
#define MSG_DEL_VIEWGROUP                  MSG_GENERIC + 11
#define MSG_CLEAR_VIEWGROUPS               MSG_GENERIC + 13
#define MSG_RUN_EXERCISE                   MSG_GENERIC + 14
#define MSG_PAUSE_EXERCISE                 MSG_GENERIC + 15
#define MSG_RESET_HOST                     MSG_GENERIC + 16
#define MSG_RESET_IG                          MSG_GENERIC + 17
#define MSG_SHUTDOWN                          MSG_GENERIC + 18
#define MSG_SET_ADDR                          MSG_GENERIC + 19
#define MSG_ADD_HAT                        MSG_GENERIC + 20
#define MSG_REMOVE_HAT                     MSG_GENERIC + 21
#define MSG_ADD_LOS                        MSG_GENERIC + 22
#define MSG_REMOVE_LOS                     MSG_GENERIC + 23
#define MSG_BEGIN_RECORD                   MSG_GENERIC + 24
#define MSG_END_RECORD                     MSG_GENERIC + 25
#define MSG_BEGIN_PLAYBACK                 MSG_GENERIC + 26
#define MSG_END_PLAYBACK                   MSG_GENERIC + 27
#define MSG_PAUSE_RECPLAYBACK              MSG_GENERIC + 28
#define MSG_RESUME_RECPLAYBACK          MSG_GENERIC + 29
#define MSG_SET_DATABASE                   MSG_GENERIC + 30
#define MSG_SET_TRACKER_STATE              MSG_GENERIC + 31
#define MSG_SET_TRACKER_BORESIGHT       MSG_GENERIC + 32
#define MSG_SET_IG_MODE                    MSG_GENERIC + 33
#define MSG_SET_HUD_STATE                  MSG_GENERIC + 34
#define MSG_SYNC_UPDATE_ENV_VIEW           MSG_GENERIC + 35
#define MSG_SYNC_UPDATE_ENTITIES_VIEW   MSG_GENERIC + 36
#define MSG_SYNC_UPDATE_VIEWS_VIEW      MSG_GENERIC + 37
#define MSG_SET_CIGI_OPTIONS               MSG_GENERIC + 38
#define MSG_TDTEST_START                  MSG_GENERIC + 39
#define MSG_TDTEST_STOP                   MSG_GENERIC + 40
#define MSG_SET_IG_CONTROL                MSG_GENERIC + 41
#define MSG_PUBLISH_SELECTED_ENTITY       MSG_GENERIC + 42

// Driver-to-Win32 Messages
#define MSG_PLAYBACK_EOF                   MSG_GENERIC + 100
#define MSG_FILE_ERROR                     MSG_GENERIC + 101
#define MSG_FRAME_RATE                     MSG_GENERIC + 102
#define MSG_NO_CONNECT                     MSG_GENERIC + 103
#define MSG_MISSILE_HIT                    MSG_GENERIC + 104
#define MSG_NOTIFY_ANIM_STOP               MSG_GENERIC + 105
#define MSG_NOTIFY_RECPLAYBACK_FRAME    MSG_GENERIC + 106
#define MSG_WAYPOINT_REACHED               MSG_GENERIC + 107
#define MSG_UPDATE_ENV_VIEW             MSG_GENERIC + 108
#define MSG_UPDATE_ENTITIES_VIEW           MSG_GENERIC + 109
#define MSG_UPDATE_VIEWS_VIEW              MSG_GENERIC + 110
#define MSG_UPDATE_ENV_TAB_PAGES           MSG_GENERIC + 111
#define MSG_UPDATE_ENTITY_TAB_PAGES     MSG_GENERIC + 112
#define MSG_UPDATE_VIEW_TAB_PAGES       MSG_GENERIC + 113
#define MSG_FORCE_MINOR_VERSION         MSG_GENERIC + 114

// Win32-to-Win32 Messages (use the same queue as Driver-to-Win32)
#define MSG_SCRIPT_EOF                     MSG_GENERIC + 200
#define MSG_SCRIPT_PROGRESS             MSG_GENERIC + 201
#define MSG_DISPLAY_MSG_TEXT               MSG_GENERIC + 202
#define MSG_SCRIPT_LOAD_SCENARIO           MSG_GENERIC + 203
#define MSG_SCRIPT_SAVE_SCENARIO           MSG_GENERIC + 204
#define MSG_SCRIPT_RELOAD_SCENARIO      MSG_GENERIC + 205
#define MSG_SCRIPT_CAPTURE_PLAY         MSG_GENERIC + 206
#define MSG_CLEAR_MSG_TEXT                 MSG_GENERIC + 207
#define MSG_UPDATE_DATABASE             MSG_GENERIC + 208
#define MSG_SPAWN_APP                      MSG_GENERIC + 209
#define MSG_SCRIPT_BEGIN_WAIT_TIMER     MSG_GENERIC + 210
#define MSG_SCRIPT_INC_WAIT_TIMER       MSG_GENERIC + 211
#define MSG_SCRIPT_END_WAIT_TIMER       MSG_GENERIC + 212
#define MSG_SCRIPT_ADD_ENTITY_TO_GUI    MSG_GENERIC + 213
#define MSG_SCRIPT_DEL_ENTITY_FROM_GUI  MSG_GENERIC + 214
#define MSG_SCRIPT_SET_PARENT_IN_GUI    MSG_GENERIC + 215
#define MSG_SCRIPT_SET_VIEWGROUP_IN_GUI MSG_GENERIC + 216
#define MSG_SCRIPT_SET_PAUSE               MSG_GENERIC + 217
#define MSG_SCRIPT_FIRE_MISSILE        MSG_GENERIC + 218
#define MSG_SCRIPT_UPDATE_MISSILE_TARGET_COMBO  MSG_GENERIC + 219

class HEMU_MESSAGE
{
public:
    const unsigned long msg;
    const unsigned long size;

    HEMU_MESSAGE();
    HEMU_MESSAGE &operator=(const HEMU_MESSAGE &src);

protected:
    HEMU_MESSAGE(unsigned long type, unsigned long sz);
};

class MESSAGE_ADD_ENTITY : public HEMU_MESSAGE
{
public:
    char sharedname[64];

    MESSAGE_ADD_ENTITY();
};

class MESSAGE_DEL_ENTITY : public HEMU_MESSAGE
{
public:
    unsigned short id;

    MESSAGE_DEL_ENTITY();
};

class MESSAGE_ADD_VIEW : public HEMU_MESSAGE
{
public:
    char sharedname[64];

    MESSAGE_ADD_VIEW();
};

class MESSAGE_CLEAR_ENTITIES : public HEMU_MESSAGE
{
public:
    MESSAGE_CLEAR_ENTITIES();
};

class MESSAGE_PUBLISH_SELECTED_ENTITY : public HEMU_MESSAGE
{
public:
    unsigned short view_id;
    unsigned short entity_id;

    MESSAGE_PUBLISH_SELECTED_ENTITY();
};

class MESSAGE_DEL_VIEW : public HEMU_MESSAGE
{
public:
    unsigned short id;

    MESSAGE_DEL_VIEW();
};

class MESSAGE_CLEAR_VIEWS : public HEMU_MESSAGE
{
public:
    MESSAGE_CLEAR_VIEWS();
};

class MESSAGE_ADD_VIEWGROUP : public HEMU_MESSAGE
{
public:
    char sharedname[64];

    MESSAGE_ADD_VIEWGROUP();
};

class MESSAGE_DEL_VIEWGROUP : public HEMU_MESSAGE
{
public:
    unsigned short id;

    MESSAGE_DEL_VIEWGROUP();
};

class MESSAGE_CLEAR_VIEWGROUPS : public HEMU_MESSAGE
{
public:
    MESSAGE_CLEAR_VIEWGROUPS();
};

class MESSAGE_PAUSE_EXERCISE : public HEMU_MESSAGE
{
public:
    MESSAGE_PAUSE_EXERCISE();
};

class MESSAGE_RUN_EXERCISE : public HEMU_MESSAGE
{
public:
    MESSAGE_RUN_EXERCISE();
};

class MESSAGE_RESET_HOST : public HEMU_MESSAGE
{
public:
    MESSAGE_RESET_HOST();
};

class MESSAGE_RESET_IG : public HEMU_MESSAGE
{
public:
    MESSAGE_RESET_IG();
};

class MESSAGE_SHUTDOWN : public HEMU_MESSAGE
{
public:
    MESSAGE_SHUTDOWN();
};

class MESSAGE_SET_ADDR : public HEMU_MESSAGE
{
public:
    char    ip_addr[16];
    char    hud_ip_addr[16];
    int     send_port;
    int     rcv_port;
    int     hud_send_port;

    MESSAGE_SET_ADDR();
};

class MESSAGE_ADD_HAT : public HEMU_MESSAGE
{
public:
    int     id_base;    // Base used to generate successive HAT IDs.
    int     entity_id;
    char    request_type;
    char    coord_system;
    char    continuous;
    char    reserved1;
    int     reserved2;
    double  lat;
    double  lon;
    double  alt;
    double  x_offset;
    double  y_offset;
    double  z_offset;

    MESSAGE_ADD_HAT();
};

class MESSAGE_ADD_LOS : public HEMU_MESSAGE
{
public:
    int id_base;    // Base used to generate successive LOS IDs.
    int entity_id;
    unsigned int req_type           : 1;    // 0 = Segment, 1 = vector
    unsigned int ext_response       : 1;    // 0 = Basic, 1 = Extended
    unsigned int src_coord_system   : 1;
    unsigned int dest_coord_system  : 1;
    unsigned int resp_coord_system  : 1;
    unsigned int continuous         : 1;
    unsigned int use_target_entity  : 1;
    unsigned int                    : 1;
    unsigned int alpha_thresh       : 8;
    unsigned int target_entity_id   : 16;
    unsigned long material_mask;
    float azimuth;
    float elevation;
    float min_range;
    float max_range;
    double src_lat;
    double src_lon;
    double src_alt;
    double src_x_offset;
    double src_y_offset;
    double src_z_offset;
    double dest_lat_x;
    double dest_lon_y;
    double dest_alt_z;

    MESSAGE_ADD_LOS();
};

class MESSAGE_BEGIN_RECORD : public HEMU_MESSAGE
{
public:
    char filename[256];

    MESSAGE_BEGIN_RECORD();
};

class MESSAGE_END_RECORD : public HEMU_MESSAGE
{
public:
    MESSAGE_END_RECORD();
};

class MESSAGE_BEGIN_PLAYBACK : public HEMU_MESSAGE
{
public:
    char filename[256];

    MESSAGE_BEGIN_PLAYBACK();
};

class MESSAGE_END_PLAYBACK : public HEMU_MESSAGE
{
public:
    MESSAGE_END_PLAYBACK();
};

class MESSAGE_PAUSE_RECPLAYBACK : public HEMU_MESSAGE
{
public:
    MESSAGE_PAUSE_RECPLAYBACK();
};

class MESSAGE_RESUME_RECPLAYBACK : public HEMU_MESSAGE
{
public:
    MESSAGE_RESUME_RECPLAYBACK();
};

class MESSAGE_SET_DATABASE : public HEMU_MESSAGE
{
public:
    int dbnum;
    double lat;
    double lon;
    double radius;
    double flattening;

    MESSAGE_SET_DATABASE();
};

class MESSAGE_SET_TRACKER_STATE : public HEMU_MESSAGE
{
public:
    int enable;

    MESSAGE_SET_TRACKER_STATE();
};

class MESSAGE_SET_TRACKER_BORESIGHT : public HEMU_MESSAGE
{
public:
    int boresight;

    MESSAGE_SET_TRACKER_BORESIGHT();
};

class MESSAGE_SET_IG_MODE : public HEMU_MESSAGE
{
public:
    int mode;

    MESSAGE_SET_IG_MODE();
};

class MESSAGE_SET_IG_CONTROL : public HEMU_MESSAGE
{
public:
    int smoothing_enabled;
    int substitution_enabled;

    MESSAGE_SET_IG_CONTROL();
};

class MESSAGE_SET_HUD_STATE : public HEMU_MESSAGE
{
public:
    int state;

    MESSAGE_SET_HUD_STATE();
};

class MESSAGE_REMOVE_HAT : public HEMU_MESSAGE
{
public:
    int id_base;    // Base used to generate successive HAT IDs.

    MESSAGE_REMOVE_HAT();
};

class MESSAGE_REMOVE_LOS : public HEMU_MESSAGE
{
public:
    int id_base;    // Base used to generate successive LOS IDs.

    MESSAGE_REMOVE_LOS();
};

class MESSAGE_SET_FLY_MODE : public HEMU_MESSAGE
{
public:
    int entity_id;
    int mode;

    MESSAGE_SET_FLY_MODE();
};

class MESSAGE_PLAYBACK_EOF : public HEMU_MESSAGE
{
public:
    MESSAGE_PLAYBACK_EOF();
};

class MESSAGE_FILE_ERROR : public HEMU_MESSAGE
{
public:
    char filename[256];

    MESSAGE_FILE_ERROR();
};

class MESSAGE_FRAME_RATE : public HEMU_MESSAGE
{
public:
    long framerate;

    MESSAGE_FRAME_RATE();
};

class MESSAGE_NO_CONNECT : public HEMU_MESSAGE
{
public:
    MESSAGE_NO_CONNECT();
};

class MESSAGE_MISSILE_HIT : public HEMU_MESSAGE
{
public:
    unsigned long missile_id;

    MESSAGE_MISSILE_HIT();
};

class MESSAGE_NOTIFY_ANIM_STOP : public HEMU_MESSAGE
{
public:
    unsigned long entity_id;

    MESSAGE_NOTIFY_ANIM_STOP();
};

class MESSAGE_NOTIFY_RECPLAYBACK_FRAME : public HEMU_MESSAGE
{
public:
    MESSAGE_NOTIFY_RECPLAYBACK_FRAME();
};

class MESSAGE_WAYPOINT_REACHED : public HEMU_MESSAGE
{
public:
    unsigned long entity_id;
    unsigned long waypoint_id;

    MESSAGE_WAYPOINT_REACHED();
};

class MESSAGE_SCRIPT_EOF : public HEMU_MESSAGE
{
public:
    MESSAGE_SCRIPT_EOF();
};

class MESSAGE_SCRIPT_PROGRESS : public HEMU_MESSAGE
{
public:
    int linenum;

    MESSAGE_SCRIPT_PROGRESS();
};

class MESSAGE_DISPLAY_MSG_TEXT : public HEMU_MESSAGE
{
public:
    char text[256];

    MESSAGE_DISPLAY_MSG_TEXT();
};

class MESSAGE_UPDATE_ENV_VIEW : public HEMU_MESSAGE
{
public:
    MESSAGE_UPDATE_ENV_VIEW();
};

class MESSAGE_UPDATE_ENTITIES_VIEW : public HEMU_MESSAGE
{
public:
    MESSAGE_UPDATE_ENTITIES_VIEW();
};

class MESSAGE_UPDATE_VIEWS_VIEW : public HEMU_MESSAGE
{
public:
    MESSAGE_UPDATE_VIEWS_VIEW();
};

class MESSAGE_UPDATE_ENV_TAB_PAGES : public HEMU_MESSAGE
{
public:
    MESSAGE_UPDATE_ENV_TAB_PAGES();
};

class MESSAGE_UPDATE_ENTITY_TAB_PAGES : public HEMU_MESSAGE
{
public:
    MESSAGE_UPDATE_ENTITY_TAB_PAGES();
};

class MESSAGE_UPDATE_VIEW_TAB_PAGES : public HEMU_MESSAGE
{
public:
    MESSAGE_UPDATE_VIEW_TAB_PAGES();
};

class MESSAGE_FORCE_MINOR_VERSION : public HEMU_MESSAGE
{
public:
    int minor_version;

    MESSAGE_FORCE_MINOR_VERSION();
};

class MESSAGE_SYNC_UPDATE_ENV_VIEW : public HEMU_MESSAGE
{
public:
    MESSAGE_SYNC_UPDATE_ENV_VIEW();
};

class MESSAGE_SYNC_UPDATE_ENTITIES_VIEW : public HEMU_MESSAGE
{
public:
    MESSAGE_SYNC_UPDATE_ENTITIES_VIEW();
};

class MESSAGE_SYNC_UPDATE_VIEWS_VIEW : public HEMU_MESSAGE
{
public:
    MESSAGE_SYNC_UPDATE_VIEWS_VIEW();
};

class MESSAGE_SET_CIGI_OPTIONS : public HEMU_MESSAGE
{
public:
    BOOL big_endian;
    int major_version;
    int minor_version;

    MESSAGE_SET_CIGI_OPTIONS();
};

class MESSAGE_SCRIPT_LOAD_SCENARIO : public HEMU_MESSAGE
{
public:
    char filename[256];

    MESSAGE_SCRIPT_LOAD_SCENARIO();
};

class MESSAGE_SCRIPT_SAVE_SCENARIO : public HEMU_MESSAGE
{
public:
    char filename[256];

    MESSAGE_SCRIPT_SAVE_SCENARIO();
};

class MESSAGE_SCRIPT_RELOAD_SCENARIO : public HEMU_MESSAGE
{
public:
    MESSAGE_SCRIPT_RELOAD_SCENARIO();
};

class MESSAGE_SCRIPT_CAPTURE_PLAY : public HEMU_MESSAGE
{
public:
    char filename[256];

    MESSAGE_SCRIPT_CAPTURE_PLAY();
};

class MESSAGE_CLEAR_MSG_TEXT : public HEMU_MESSAGE
{
public:
    MESSAGE_CLEAR_MSG_TEXT();
};

class MESSAGE_UPDATE_DATABASE : public HEMU_MESSAGE
{
public:
    int db_num;

    MESSAGE_UPDATE_DATABASE();
};

class MESSAGE_SPAWN_APP : public HEMU_MESSAGE
{
public:
    char command[256];

    MESSAGE_SPAWN_APP();
};

class MESSAGE_SCRIPT_BEGIN_WAIT_TIMER : public HEMU_MESSAGE
{
public:
    int secs;

    MESSAGE_SCRIPT_BEGIN_WAIT_TIMER();
};

class MESSAGE_SCRIPT_INC_WAIT_TIMER : public HEMU_MESSAGE
{
public:
    int secs;

    MESSAGE_SCRIPT_INC_WAIT_TIMER();
};

class MESSAGE_SCRIPT_END_WAIT_TIMER : public HEMU_MESSAGE
{
public:
    MESSAGE_SCRIPT_END_WAIT_TIMER();
};

class MESSAGE_SCRIPT_ADD_ENTITY_TO_GUI : public HEMU_MESSAGE
{
public:
    int entity_id;

    MESSAGE_SCRIPT_ADD_ENTITY_TO_GUI();
};

class MESSAGE_SCRIPT_DEL_ENTITY_FROM_GUI : public HEMU_MESSAGE
{
public:
    int entity_id;
    unsigned long htree;
    unsigned long htree_env;
    unsigned long situation_hdl;

    MESSAGE_SCRIPT_DEL_ENTITY_FROM_GUI();
};

class MESSAGE_SCRIPT_SET_PARENT_IN_GUI : public HEMU_MESSAGE
{
public:
    int entity_id;
    int parent_id;

    MESSAGE_SCRIPT_SET_PARENT_IN_GUI();
};

class MESSAGE_SCRIPT_SET_VIEWGROUP_IN_GUI : public HEMU_MESSAGE
{
public:
    int view_id;
    int group_id;

    MESSAGE_SCRIPT_SET_VIEWGROUP_IN_GUI();
};

class MESSAGE_SCRIPT_SET_PAUSE : public HEMU_MESSAGE
{
public:
    MESSAGE_SCRIPT_SET_PAUSE();
};

class CEntity;
class MESSAGE_SCRIPT_FIRE_MISSILE : public HEMU_MESSAGE
{
public:
    CEntity *pMissile;
    MESSAGE_SCRIPT_FIRE_MISSILE();
};

class MESSAGE_SCRIPT_UPDATE_MISSILE_TARGET_COMBO : public HEMU_MESSAGE
{
public:
    MESSAGE_SCRIPT_UPDATE_MISSILE_TARGET_COMBO();
};

class MESSAGE_TDTEST_START : public HEMU_MESSAGE
{
public:
    int count1;
    int count2;
    double lat1;
    double lon1;
    double alt1;
    double heading1;
    double pitch1;
    double roll1;
    double lat2;
    double lon2;
    double alt2;
    double heading2;
    double pitch2;
    double roll2;

    MESSAGE_TDTEST_START();
};

class MESSAGE_TDTEST_STOP : public HEMU_MESSAGE
{
public:
    MESSAGE_TDTEST_STOP();
};

#endif  // _MESSAGES_H_
