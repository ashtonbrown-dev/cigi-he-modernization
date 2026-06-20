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

#ifndef _COMMON_H_
#define _COMMON_H_

#include "RTXSharedBufferQ.h"
#include "coordcnv.h"
#include "math.h"
#include "stdio.h"

#ifdef NO_RTX
#define MAX_ETHERNET_PACKET_SIZE    16384       // Self-imposed limit of 16KB
#else
#define MAX_ETHERNET_PACKET_SIZE    1472        // Limited by RTX TCP-IP
#endif

#ifndef PI
#define PI                  3.1415926535897932
#endif

#define TYPE_UNDEFINED              0
#define TYPE_INT32                  1
#define TYPE_UINT32                 2
#define TYPE_FLOAT                  3
#define TYPE_INT64                  4
#define TYPE_UINT64                 5
#define TYPE_DOUBLE                 6
#define TYPE_HEX                    7   // Store as UINT32

// Component classes
#define COMP_CLASS_ENTITY           0
#define COMP_CLASS_VIEW             1
#define COMP_CLASS_VIEWGROUP        2
#define COMP_CLASS_SENSOR           3
#define COMP_CLASS_R_SEA            4
#define COMP_CLASS_R_TERRAIN        5
#define COMP_CLASS_R_WEATHER        6
#define COMP_CLASS_G_SEA            7
#define COMP_CLASS_G_TERRAIN        8
#define COMP_CLASS_G_WEATHER        9
#define COMP_CLASS_ATMOSPHERE       10
#define COMP_CLASS_CELSPHERE        11
#define COMP_CLASS_EVENT            12
#define COMP_CLASS_SYSTEM           13

#define OBJECT_TYPE_ENTITY          0
#define OBJECT_TYPE_VIEW            1
#define OBJECT_TYPE_VIEWGROUP       2
#define OBJECT_TYPE_WEATHER         3
#define OBJECT_TYPE_SKY             4
#define OBJECT_TYPE_DATABASE        5
#define NUM_OBJECT_TYPES            6

#define WEATHER_CLASS_LAYER         0
#define WEATHER_CLASS_ENTITY        1

#define ENTITY_CLAMP_NONE           0
#define ENTITY_CLAMP_NONCONFORMAL   1
#define ENTITY_CLAMP_CONFORMAL      2

#define ENTITY_CLASS_GENERIC        0
#define ENTITY_CLASS_FIXEDWING      1
#define ENTITY_CLASS_ANIMATION      2
#define ENTITY_CLASS_CLOUD          3
#define ENTITY_CLASS_MISSILE        4
#define ENTITY_CLASS_TANK           5
#define ENTITY_CLASS_CAR            6
#define ENTITY_CLASS_BOAT           7
#define ENTITY_CLASS_ROTORCRAFT     8
#define ENTITY_CLASS_BOMB           9
#define ENTITY_CLASS_SAM            10
#define ENTITY_CLASS_PATTERN        11
#define ENTITY_CLASS_CULTURE        12
#define ENTITY_CLASS_SEA_STATE      13
#define ENTITY_CLASS_TERRAIN_COND   14

#define ENTITY_FLYMODE_FLY          0
#define ENTITY_FLYMODE_MAGIC_CARPET 1
#define ENTITY_FLYMODE_ROLL_PITCH   2
#define ENTITY_FLYMODE_YAW_Z        3

#define ENTITY_ANIM_STATE_STOP      0
#define ENTITY_ANIM_STATE_PAUSE     1
#define ENTITY_ANIM_STATE_PLAY      2
#define ENTITY_ANIM_STATE_CONTINUE  3

#define LOS_TYPE_SEGMENT            0
#define LOS_TYPE_VECTOR             1

#define LOS_RESP_TYPE_BASIC         0
#define LOS_RESP_TYPE_EXTENDED      1

#define LOS_COORD_SYSTEM_WORLD      0
#define LOS_COORD_SYSTEM_ENTITY     1

#define HAT_TYPE_HAT                0
#define HAT_TYPE_HOT                1
#define HAT_TYPE_COMBINED           2

#define HAT_COORD_SYSTEM_WORLD      0
#define HAT_COORD_SYSTEM_ENTITY     1

#define COLLISION_TYPE_SEGMENT      0
#define COLLISION_TYPE_VOLUME       1

#define COLLISION_VOLUME_SPHERE     0
#define COLLISION_VOLUME_CUBOID     1

#define PROJECTION_TYPE_PERSPECTIVE 0
#define PROJECTION_TYPE_PARALLEL    1

#define PIXEL_REPLICATION_MODE_NONE 0
#define PIXEL_REPLICATION_MODE_1X2  1
#define PIXEL_REPLICATION_MODE_2X1  2
#define PIXEL_REPLICATION_MODE_2X2  3

#define MIRROR_MODE_NONE            0
#define MIRROR_MODE_HORZ            1
#define MIRROR_MODE_VERT            2
#define MIRROR_MODE_BOTH            3

#define WAYPOINT_TYPE_ABSOLUTE      0
#define WAYPOINT_TYPE_RELATIVE      1       // Used only by GUI
#define WAYPOINT_TYPE_FORMATION     2

extern const double TERRAIN_DEFAULT_FLATTENING;
extern const double TERRAIN_DEFAULT_RADIUS;

///////////////////////////////////////////////////////////////////////////////
//
//  Datatypes
//
///////////////////////////////////////////////////////////////////////////////

typedef struct {
    double  x;
    double  y;
    double  z;
} POINT3D;

typedef struct {
    double  i;
    double  j;
    double  k;
} VECTOR3D;

typedef struct {
    double  y;
    double  p;
    double  r;
} ROTATION3D;

typedef struct {
    // Absolute coordinates.
    double  latitude;
    double  longitude;
    double  altitude;
    double  yaw;
    double  roll;
    double  pitch;

    // Relative (to parent) position and attitude:
    double  offset_x;
    double  offset_y;
    double  offset_z;
    double  rel_yaw;
    double  rel_pitch;
    double  rel_roll;
} DOF;

typedef struct {
    double  dx;
    double  dy;
    double  dz;
    double  droll;
    double  dpitch;
    double  dyaw;
} RATES;


// This is used for serialization to maintain file compatibility.
typedef struct {
    long            id;
    long            type;
    long            parent_id;              // -1 means no parent
    long            target_id;              // target for missiles and bombs (-1 = none)
    unsigned long   entity_class    : 4;    // 0 - 13
    unsigned long   active          : 1;    // visible
    unsigned long   alpha           : 8;
    unsigned long   anim_state      : 2;    // 0 - 4
    unsigned long   anim_dir        : 1;    // 0 = forward; 1 = backward
    unsigned long   anim_loop       : 1;    // 0 = one-shot; 1 = loop
    unsigned long   inherit_alpha   : 1;
    unsigned long   clamp_mode      : 2;
    unsigned long   coll_detect     : 1;    // 1 = enabled; 0 = disabled
    unsigned long   flymode         : 2;
    unsigned long   send_rates      : 1;
    unsigned long   use_attach_point: 1;    // attached to attach_point
    unsigned long   attach_point    : 5;    // attach point of parent
    unsigned long   spare1          : 2;
    unsigned long   spare2;
    unsigned long   spare3;
    unsigned long   spare4;
    double          speed;

    // Absolute position and attitude:
    DOF dofs;

    // Rate information:
    RATES rates;
} ENTITY_SAVE_STATE_CORE;

typedef struct {
    // Extended stuff
    unsigned long   waypoint_id;            // for debugging
    unsigned long   track_target    : 1;    // missile is tracking target
    unsigned long   fly_waypoints   : 1;    // entity is flying toward waypoint
    unsigned long   waypoint_valid  : 1;    // waypoint is valid -- okay to fly to it
    unsigned long   waypoint_reached: 1;
    unsigned long   waypoint_type   : 2;
    unsigned long   roll_disable    : 1;
    unsigned long   destroy         : 1;    // notify driver to delete object
    unsigned long   collective      : 7;
    unsigned long   spare5          : 17;
    unsigned short  ramp_up_ctr;            // counter to begin tracking of target
    unsigned short  yaw_ctr;                // counter to begin yaw to waypoint
    unsigned short  pitch_ctr;              // counter to begin pitch to waypoint
    unsigned short  roll_ctr;               // counter to begin roll to waypoint
    long            ref_entity;
    long            reserved;
    double          accel;
    double          speed_init;
    double          speed_final;
    double          turn_rate;
    double          waypoint_x;             // UTM or offset from ref_entity
    double          waypoint_y;
    double          waypoint_z;
    double          ddy;
    double          ddp;
    double          ddr;
    double          vi;
    double          vj;
    double          dvi;                    // i-acceleration for helicopter
    double          dvj;                    // j-acceleration for helicopter

} ENTITY_SAVE_STATE_EXT;

typedef struct {
    ENTITY_SAVE_STATE_CORE core;
    ENTITY_SAVE_STATE_EXT ext;
} ENTITY_SAVE_STATE;


typedef struct {
    long            id;
    long            type;
    long            parent_id;              // -1 means no parent
    unsigned long   alpha           : 8;
    unsigned long   anim_state      : 2;    // 0 - 4
    unsigned long   anim_dir        : 1;    // 0 = forward; 1 = backward
    unsigned long   anim_loop       : 1;    // 0 = one-shot; 1 = loop
    unsigned long   inherit_alpha   : 1;
    unsigned long   clamp_mode      : 2;
    unsigned long   coll_detect     : 1;    // 1 = enabled; 0 = disabled
    unsigned long   smoothing       : 1;    // 1 = enabled; 0 = disabled
    unsigned long   extended_type   : 1;    // 1 = enabled; 0 = disabled
    unsigned long   active          : 2;    // 0 = Inactive/Standby; 1 = visible; 2 = Destroyed
    unsigned long   spare           : 12;
	unsigned char	entity_kind;
	unsigned char	entity_domain;
	unsigned short	entity_country;
	unsigned char	entity_category;
	unsigned char	entity_subcategory;
	unsigned char	entity_specific;
	unsigned char	entity_extra;
	
    // Absolute position and attitude:
    DOF dofs;   // Make this a struct so we can read/write in
    // a single operation.

} ENTITY_CIGI_DATA;

typedef struct {
    unsigned long   entity_class    : 4;    // 0 - 13
    unsigned long   collective      : 7;
    unsigned long   flymode         : 2;
    unsigned long   use_attach_point: 1;    // attached to attach_point
    unsigned long   attach_point    : 5;    // attach point of parent
    unsigned long   send_rates      : 1;
    unsigned long   rates_coordsys  : 1;    // 0= world/parent; 1 = locl
    unsigned long   spare           : 11;
    long            target_id;              // target for missiles and bombs (-1 = none)
    double          speed;

    // Rate information:
    RATES rates;        // Make this a struct so we can read/write in
    // a single operation.
} ENTITY_NONCIGI_DATA;

typedef struct {
    unsigned long   waypoint_id;            // for debugging
    unsigned long   track_target    : 1;    // missile is tracking target
    unsigned long   fly_waypoints   : 1;    // entity is flying toward waypoint
    unsigned long   waypoint_valid  : 1;    // waypoint is valid -- okay to fly to it
    unsigned long   waypoint_reached: 1;
    unsigned long   waypoint_type   : 2;
    unsigned long   roll_disable    : 1;
    unsigned long   destroy         : 1;    // notify driver to delete object
    unsigned long   spare1          : 21;
    unsigned long   entityanimation_has_changed: 1;
    unsigned long   entityposition_has_changed: 1;
    unsigned long   entityctrl_has_changed: 1;
    unsigned short  ramp_up_ctr;            // counter to begin tracking of target
    unsigned short  yaw_ctr;                // counter to begin yaw to waypoint
    unsigned short  pitch_ctr;              // counter to begin pitch to waypoint
    unsigned short  roll_ctr;               // counter to begin roll to waypoint
    long            ref_entity;
    long            reserved;
    double          accel;
    double          speed_init;
    double          speed_final;
    double          turn_rate;
    double          waypoint_x;             // UTM or offset from ref_entity
    double          waypoint_y;
    double          waypoint_z;
    double          ddy;
    double          ddp;
    double          ddr;
    double          vi;
    double          vj;
    double          dvi;                    // i-acceleration for helicopter
    double          dvj;                    // j-acceleration for helicopter
} ENTITY_TEMP_DATA;


typedef struct {
    ENTITY_CIGI_DATA        cigi;       // saved, included in cigi packet
    ENTITY_NONCIGI_DATA     noncigi;    // saved, not in cigi packet
    ENTITY_TEMP_DATA        unsaved;    // not saved
} ENTITY;


typedef struct {
    unsigned int view_id            : 16;
    unsigned int group_id           : 8;
    unsigned int view_type          : 3;
    unsigned int projection         : 1;
    unsigned int replication        : 3;
    unsigned int reorder            : 1;
    unsigned int entity_id          : 16;
    unsigned int mirror_mode        : 2;
    unsigned int fov_top_enable     : 1;
    unsigned int fov_bottom_enable  : 1;
    unsigned int fov_left_enable    : 1;
    unsigned int fov_right_enable   : 1;
    unsigned int fov_near_enable    : 1;
    unsigned int fov_far_enable     : 1;
    unsigned int offset_x_enable    : 1;
    unsigned int offset_y_enable    : 1;
    unsigned int offset_z_enable    : 1;
    unsigned int yaw_enable         : 1;
    unsigned int pitch_enable       : 1;
    unsigned int roll_enable        : 1;
    unsigned int def_has_changed    : 1;
    unsigned int camera_has_changed : 1;
    unsigned int                    : 2;
    float offset_x;
    float offset_y;
    float offset_z;
    float yaw;
    float pitch;
    float roll;
    float dx;
    float dy;
    float dz;
    float dyaw;
    float dpitch;
    float droll;
    float fov_top;
    float fov_bottom;
    float fov_left;
    float fov_right;
    float fov_near;
    float fov_far;
} VIEW_SAVE_STATE;


typedef struct {
    unsigned int view_id            : 16;
    unsigned int group_id           : 8;
    unsigned int view_type          : 3;
    unsigned int projection         : 1;
    unsigned int replication        : 3;
    unsigned int reorder            : 1;

    unsigned int mirror_mode        : 2;
    unsigned int fov_top_enable     : 1;
    unsigned int fov_bottom_enable  : 1;
    unsigned int fov_left_enable    : 1;
    unsigned int fov_right_enable   : 1;
    unsigned int fov_near_enable    : 1;
    unsigned int fov_far_enable     : 1;
    unsigned int                    : 24;

    float fov_top;
    float fov_bottom;
    float fov_left;
    float fov_right;
    float fov_near;
    float fov_far;
} VIEW_DEF_DATA;


typedef struct {
    unsigned int entity_id          : 16;
    unsigned int offset_x_enable    : 1;
    unsigned int offset_y_enable    : 1;
    unsigned int offset_z_enable    : 1;
    unsigned int yaw_enable         : 1;
    unsigned int pitch_enable       : 1;
    unsigned int roll_enable        : 1;
    unsigned int                    : 10;

    unsigned int reserved;

    float offset_x;
    float offset_y;
    float offset_z;
    float yaw;
    float pitch;
    float roll;
} VIEW_CONTROL_DATA;


typedef struct {
    unsigned int def_has_changed    : 1;
    unsigned int ctrl_has_changed   : 1;
    unsigned int                    : 30;

    unsigned int reserved;

    float dx;
    float dy;
    float dz;
    float dyaw;
    float dpitch;
    float droll;
} VIEW_NONCIGI_DATA;

typedef struct {
    VIEW_DEF_DATA       viewdef;
    VIEW_CONTROL_DATA   viewctrl;
    VIEW_NONCIGI_DATA   noncigi;
} VIEW;

typedef struct {
    unsigned int group_id           : 8;
    unsigned int offset_x_enable    : 1;
    unsigned int offset_y_enable    : 1;
    unsigned int offset_z_enable    : 1;
    unsigned int yaw_enable         : 1;
    unsigned int pitch_enable       : 1;
    unsigned int roll_enable        : 1;
    unsigned int                    : 2;
    unsigned int entity_id          : 16;
    unsigned int reserved;
    float offset_x;
    float offset_y;
    float offset_z;
    float yaw;
    float pitch;
    float roll;
    float dx;
    float dy;
    float dz;
    float dyaw;
    float dpitch;
    float droll;
} VIEWGROUP_SAVE_STATE;

typedef struct {
    unsigned int group_id           : 8;
    unsigned int offset_x_enable    : 1;
    unsigned int offset_y_enable    : 1;
    unsigned int offset_z_enable    : 1;
    unsigned int yaw_enable         : 1;
    unsigned int pitch_enable       : 1;
    unsigned int roll_enable        : 1;
    unsigned int                    : 2;
    unsigned int entity_id          : 16;
    unsigned int reserved;
    float offset_x;
    float offset_y;
    float offset_z;
    float yaw;
    float pitch;
    float roll;
} VIEWGROUP_CIGI_DATA;

typedef struct {
    unsigned int cigi_viewgroup_has_changed   : 1;
    unsigned int                    : 31;
    float dx;
    float dy;
    float dz;
    float dyaw;
    float dpitch;
    float droll;
} VIEWGROUP_NONCIGI_DATA;

typedef struct {
    VIEWGROUP_CIGI_DATA     cigi;
    VIEWGROUP_NONCIGI_DATA  noncigi;
} VIEWGROUP;


typedef struct {
    unsigned int id;
    unsigned short owner_id;        // usually entity id
    unsigned short artpart_id;      // only for position req/resp
} MF_IDENTITY;


typedef struct {
    MF_IDENTITY identity;
} MF_DUMMY_RESP;


typedef struct {
    MF_IDENTITY identity;
    unsigned int req_type           : 2;    // 0 = HAT, 1 = HOT, 2 = both
    unsigned int coord_system       : 1;    // 0 = World, 1 = Entity
    unsigned int continuous         : 1;
    unsigned int                    : 28;
    unsigned long reserved;
    double lat;
    double lon;
    double alt;
    double x_offset;
    double y_offset;
    double z_offset;
} HAT_REQUEST;


typedef struct {
    MF_IDENTITY identity;
    unsigned int resp_type          : 2;    // 0 = HAT, 1 = HOT, 2 = both
    unsigned int valid              : 1;
    unsigned int                    : 29;
    unsigned long material_code;
    double hat;
    double hot;
    float azimuth;
    float elevation;
} HAT_RESPONSE;


typedef struct {
    MF_IDENTITY identity;
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
} LOS_REQUEST;


typedef struct {
    MF_IDENTITY identity;
    unsigned int resp_type          : 1;    // 0 = Basic, 1 = Extended
    unsigned int valid              : 1;
    unsigned int entity_id_valid    : 1;
    unsigned int range_valid        : 1;
    unsigned int visible            : 1;
    unsigned int coord_system       : 1;
    unsigned int                    : 2;
    unsigned int response_count     : 8;
    unsigned int                    : 16;
    int entity_id;
    unsigned long material_code;
    unsigned int red                : 8;
    unsigned int green              : 8;
    unsigned int blue               : 8;
    unsigned int alpha              : 8;
    float azimuth;
    float elevation;
    double lat_x;
    double lon_y;
    double alt_z;
    double range;
} LOS_RESPONSE;


typedef struct {
    MF_IDENTITY identity;
    unsigned int enable     : 1;
    unsigned int            : 31;
    unsigned long material_mask;
    float x1;
    float y1;
    float z1;
    float x2;
    float y2;
    float z2;
} COLLISION_SEG;


typedef struct {
    MF_IDENTITY identity;
    unsigned int type       : 1;
    unsigned int            : 31;
    unsigned short contacted_entity_id;
    unsigned short reserved;
    unsigned long material_code;
    float intersect_dist;
} COLLISION_SEG_NOTIFICATION;


typedef struct {
    MF_IDENTITY identity;
    unsigned int enable     : 1;
    unsigned int type       : 1;
    unsigned int            : 30;
    float x;
    float y;
    float z;
    float height_radius;
    float width;
    float depth;
    float roll;
    float pitch;
    float yaw;
} COLLISION_VOL;


typedef struct {
    MF_IDENTITY identity;
    unsigned int type       : 1;
    unsigned int            : 31;
    unsigned short contacted_entity_id;
    unsigned short contacted_volume_id;
} COLLISION_VOL_NOTIFICATION;


typedef struct {
    MF_IDENTITY identity;
} ENV_REQUEST;


typedef struct {
    MF_IDENTITY identity;
} WEATHER_RESPONSE;


typedef struct {
    MF_IDENTITY identity;
} AEROSOL_RESPONSE;


typedef struct {
    MF_IDENTITY identity;
} TERRESTRIAL_RESPONSE;


typedef struct {
    MF_IDENTITY identity;
} MARITIME_RESPONSE;


typedef struct {
    unsigned short id;
    unsigned short type;
    int ref_entity;
    int time;               // time to intercept, or formation time
    int airspeed;
    double accel;
    double turn_rate;
    double azimuth;
    double range;
    double elevation;
    double lat;
    double lon;
    double alt;
    double x;               // Formation offset or UTM
    double y;               // Formation offset or UTM
    double z;               // Formation offset or UTM
} WAYPOINT;


///////////////////////////////////////////////////////////////////////////////
//
//  Global Variables
//
///////////////////////////////////////////////////////////////////////////////

// Handle to shutdown event so we can terminate the thread
// if the program closes.
extern HANDLE g_ShutdownEventHdl;

// Event for FRAME and FRAME_WAIT script commands.
extern HANDLE g_FrameWaitEventHdl;

// Message Queues for RTX-Win32 and Win32-RTX IPC.
extern RTXSharedBufferQ     g_GuiToDrvMsgQueue;
extern RTXSharedBufferQ     g_DrvToGuiMsgQueue;
extern RTXSharedBufferQ     g_SentCIGIMsgQueue;
extern RTXSharedBufferQ     g_SendImmedCIGIMsgQueue;
extern RTXSharedBufferQ     g_SendCIGIMsgQueue;
extern RTXSharedBufferQ     g_RcvCIGIMsgQueue;

// CCU state info
extern EarthRefModelStruct g_ERM;


///////////////////////////////////////////////////////////////////////////////
//
//  Function Prototypes
//
///////////////////////////////////////////////////////////////////////////////

int InitializeIPC(void);
void InitializeCCU(const double radius, const double flattening);
BOOL GDCtoTM(double lat, double lon, double *y, double *x);
BOOL TMtoGDC(double y, double x, double *lat, double *lon);
void SetTMOrigin(double lat, double lon);
double CalcGeoDistance(const double lat1, const double lon1,
                       const double lat2, const double lon2);
double CalcGeoRelBearing(const double lat1, const double lon1, const double yaw1,
                         const double lat2, const double lon2);
BOOL CalcBearingAndRange(const double ref_lat, const double ref_lon,
                         const double ref_yaw, const double lat,
                         const double lon, double *bearing, double *range);
BOOL GetRelativePos(const double ref_lat, const double ref_lon,
                    const double ref_yaw, const double bearing,
                    const double range, double *lat, double *lon);


///////////////////////////////////////////////////////////////////////////////
//
//  Inline Functions
//
///////////////////////////////////////////////////////////////////////////////

// Structure is identical to CIGI_PACKET_HEADER.
typedef struct {
    unsigned char packet_id;        /** Packet ID number (type). */
    unsigned char packet_size;      /** The size of the packet, in bytes. */
    unsigned char version_number;   /** Valid for SOF packet only. */
} PACKET_HEADER;



inline WORD MAKE_WORD(const BYTE lo_byte, const BYTE hi_byte)
{
    return (hi_byte << 8) + lo_byte;
}

// Converts Knots to m/sec and vice-versa.
inline double KnotsToMPS(double knots)
{
    return knots * 0.5144444f;
}

inline double MPSToKnots(double mps)
{
    return mps * 1.943844f;
}

// Converts Degrees to Radians.
inline double DegToRad(double degs)
{
    return degs * PI / 180.0f;
}

inline double RadToDeg(double rads)
{
    return rads * 180.0f / PI;
}

// Rounding routines
inline double Round2(double num)
{
    return (double)(int)((num * 100) + 0.5) / 100;
}

inline double Round5(double num)
{
    return (double)(int)((num * 100000) + 0.5) / 100000;
}

// Inline for calculating the central meridian of a UTM zone given
// a longitude.
inline double CalcCentralMeridian(const double lon)
{
    return (6 * (floor(lon / 6.0f) + 31) - 183);
}

// Find the UTM zone.
inline void CalcUTMZone(char *buffer, double lat, double lon)
{
    sprintf(buffer, "%d%c", (long)((lon / 6.0f) + 31), (char)((lat / 8.0f) + 'N'));
}

#endif
