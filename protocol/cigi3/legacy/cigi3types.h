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

#ifndef _CIGI3TYPES_H_
#define _CIGI3TYPES_H_

#include "stdint.h"

#define CIGI_MAX_PACKET_SIZE       256  /* The largest allowable packet size */
#define CIGI_OPCODE_NONE             0  /* Undefined opcode */
#define CIGI_VERSION                 3  /* CIGI version number */

#define CIGI_DISABLED                0  /* Used to indicate a disabled state */
#define CIGI_ENABLED                 1  /* Used to indicate an enabled state */

#define CIGI_INACTIVE                0  /* Used to indicate an inactive state */
#define CIGI_ACTIVE                  1  /* Used to indicate an active state */

#define CIGI_INVALID                 0  /* Used to indicate an invalid state */
#define CIGI_VALID                   1  /* Used to indicate a valid state */

// C/C++ naming
#ifdef __cplusplus
extern "C" {
#endif

    struct CIGI_DUMMY_PACKET {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t data1;
        uint32_t data2;
    };

#define CIGI_DUMMY_PACKET_OPCODE                0
#define CIGI_DUMMY_PACKET_SIZE                  8


    // CIGI IG Control
    struct CIGI_IG_CONTROL_30 {
        uint8_t packet_id;                  /** Packet ID = 1           */
        uint8_t packet_size;                /** Packet size = 16 bytes  */
        uint8_t cigi_version;               /** CIGI version ID         */
        int8_t db_number;                   /** Database number         */
        unsigned int ig_mode         : 2;   /** IG mode control         */
        unsigned int timestamp_valid : 1;   /** Timestamp valid flag    */
        unsigned int                 : 5;   /** Reserved                */
        unsigned int                 : 8;   /** Reserved                */
        unsigned int byteswap        : 16;  /** Byteswap magic number   */
        uint32_t frame_ctr;                 /** Frame counter           */
        uint32_t timestamp;                 /** Timing value/tag        */
    };

    struct CIGI_IG_CONTROL_32 {
        uint8_t packet_id;                          /** Packet ID = 1                      */
        uint8_t packet_size;                        /** Packet size = 24 bytes             */
        uint8_t cigi_version;                       /** CIGI major version                 */
        int8_t db_number;                           /** Database number                    */
        unsigned int ig_mode                : 2;    /** IG mode control                    */
        unsigned int timestamp_valid        : 1;    /** Timestamp valid flag               */
        unsigned int extrap_interpol_enable : 1;    /** Extrapolation/interpolation enable */
        unsigned int minor_version          : 4;    /** CIGI minor version                 */
        unsigned int                        : 8;    /** Reserved                           */
        unsigned int byteswap               : 16;   /** Byteswap magic number              */
        uint32_t host_frame_num;                    /** Host frame number                  */
        uint32_t timestamp;                         /** Timing value/tag                   */
        uint32_t last_ig_frame_num;                 /** Last IG frame number               */
        unsigned int                        : 32;   /** Reserved                           */
    };

    union CIGI_IG_CONTROL {
        CIGI_IG_CONTROL_32 igc32;
        CIGI_IG_CONTROL_30 igc30;
    };


#define CIGI_IG_CONTROL_OPCODE                  1
#define CIGI_IG_CONTROL_SIZE                    16

    // IG mode
#define IG_CONTROL_IG_MODE_STANDBY              0
#define IG_CONTROL_IG_MODE_OPERATE              1
#define IG_CONTROL_IG_MODE_DEBUG                2

    // Byteswap magic numbers
#define IG_CONTROL_SWAP                         0x0080
#define IG_CONTROL_NOSWAP                       0x8000

    // CIGI Entity Control
    struct CIGI_ENTITY_CONTROL {
        uint8_t packet_id;                  /** Packet ID = 2               */
        uint8_t packet_size;                /** Packet size = 48 bytes      */
        uint16_t entity_id;                 /** Entity ID                   */
        unsigned int state            : 2;  /** Reserved                    */
        unsigned int attach_state     : 1;  /** Effect animation state      */
        unsigned int collision_detect : 1;  /** Collision detection request */
        unsigned int inherit_alpha    : 1;  /** Attach/detach switch        */
        unsigned int clamp_mode       : 2;  /** Ground/ocean clamp          */
        unsigned int                  : 1;  /** Reserved                    */
        unsigned int anim_direction   : 1;  /** Animation direction         */
        unsigned int anim_loop_mode   : 1;  /** Animation loop mode         */
        unsigned int anim_state       : 2;  /** Animation state             */
        unsigned int                  : 4;  /** Reserved                    */
        unsigned int alpha            : 8;  /** Alpha                       */
        unsigned int                  : 8;  /** Reserved                    */
        uint16_t type;                      /** Entity type                 */
        uint16_t parent_id;                 /** Parent ID                   */
        float roll;                         /** Entity roll                 */
        float pitch;                        /** Entity pitch                */
        float yaw;                          /** Entity yaw                  */
        double lat_x;                       /** Entity latitude  / X offset */
        double lon_y;                       /** Entity longitude / Y offset */
        double alt_z;                       /** Entity altitude  / Z offset */
    };

#define CIGI_ENTITY_CONTROL_OPCODE              2
#define CIGI_ENTITY_CONTROL_SIZE                48

    /** Entity state */
#define ENTITY_STATE_INACTIVE                   0
#define ENTITY_STATE_ACTIVE                     1
#define ENTITY_STATE_DESTROYED                  2

    /** Entity attached flag */
#define ENTITY_ATTACHED                         1
#define ENTITY_DETACHED                         0

    /** Conformal clamped entity control */
    struct CIGI_ENTITY_CONTROL_CC {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t entity_id;
        float yaw;
        double latitude;
        double longitude;
    };

#define CIGI_ENTITY_CONTROL_CC_OPCODE           3
#define CIGI_ENTITY_CONTROL_CC_SIZE             24

    struct CIGI_COMPONENT_CONTROL_32 {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t component_id;
        unsigned int instance_id        : 16;
        unsigned int component_class    : 4;
        unsigned int                    : 4;
        unsigned int component_state    : 8;
        uint32_t data1;
        uint32_t data2;
        uint32_t data3;
        uint32_t data4;
        uint32_t data5;
        uint32_t data6;
    };

    struct CIGI_COMPONENT_CONTROL_33 {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t component_id;
        unsigned int instance_id        : 16;
        unsigned int component_class    : 6;
        unsigned int                    : 2;
        unsigned int component_state    : 8;
        uint32_t data1;
        uint32_t data2;
        uint32_t data3;
        uint32_t data4;
        uint32_t data5;
        uint32_t data6;
    };

    union CIGI_COMPONENT_CONTROL {
        CIGI_COMPONENT_CONTROL_32 v32;
        CIGI_COMPONENT_CONTROL_33 v33;
    };

#define CIGI_COMPONENT_CONTROL_OPCODE           4
#define CIGI_COMPONENT_CONTROL_SIZE             32

    struct CIGI_COMPONENT_CONTROL_S_32 {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t component_id;
        unsigned int instance_id        : 16;
        unsigned int component_class    : 4;
        unsigned int                    : 4;
        unsigned int component_state    : 8;
        uint32_t data1;
        uint32_t data2;
    };

    struct CIGI_COMPONENT_CONTROL_S_33 {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t component_id;
        unsigned int instance_id        : 16;
        unsigned int component_class    : 6;
        unsigned int                    : 2;
        unsigned int component_state    : 8;
        uint32_t data1;
        uint32_t data2;
    };

    union CIGI_COMPONENT_CONTROL_S {
        CIGI_COMPONENT_CONTROL_S_32 v32;
        CIGI_COMPONENT_CONTROL_S_33 v33;
    };

#define CIGI_COMPONENT_CONTROL_S_OPCODE         5
#define CIGI_COMPONENT_CONTROL_S_SIZE           16

    struct CIGI_ARTPART_CONTROL {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t entity_id;
        unsigned int artpart_id     : 8;
        unsigned int artpart_enable : 1;
        unsigned int xoffset_enable : 1;
        unsigned int yoffset_enable : 1;
        unsigned int zoffset_enable : 1;
        unsigned int roll_enable    : 1;
        unsigned int pitch_enable   : 1;
        unsigned int yaw_enable     : 1;
        unsigned int                : 1;
        unsigned int                : 16;
        float xoffset;
        float yoffset;
        float zoffset;
        float roll;
        float pitch;
        float yaw;
    };

#define CIGI_ARTPART_CONTROL_OPCODE             6
#define CIGI_ARTPART_CONTROL_SIZE               32

    struct CIGI_ARTPART_CONTROL_S {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t entity_id;
        unsigned int artpart1_id        : 8;
        unsigned int artpart2_id        : 8;
        unsigned int dof1_select        : 3;
        unsigned int dof2_select        : 3;
        unsigned int artpart1_enable    : 1;
        unsigned int artpart2_enable    : 1;
        unsigned int                    : 8;
        float dof1;
        float dof2;
    };

#define CIGI_ARTPART_CONTROL_S_OPCODE           7
#define CIGI_ARTPART_CONTROL_S_SIZE             16

    struct CIGI_RATE_CONTROL {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t entity_id;
        unsigned int artpart_id     : 8;
        unsigned int apply_artpart  : 1;
        unsigned int coord_system   : 1;
        unsigned int                : 6;
        unsigned int                : 16;
        float x_rate;
        float y_rate;
        float z_rate;
        float roll_rate;
        float pitch_rate;
        float yaw_rate;
    };

#define CIGI_RATE_CONTROL_OPCODE                8
#define CIGI_RATE_CONTROL_SIZE                  32

    struct CIGI_CELESTIAL_SPHERE_CONTROL {
        uint8_t packet_id;
        uint8_t packet_size;
        uint8_t hour;
        uint8_t minute;
        unsigned int ephemeris_enable   : 1;
        unsigned int sun_enable         : 1;
        unsigned int moon_enable        : 1;
        unsigned int starfield_enable   : 1;
        unsigned int datetime_valid     : 1;
        unsigned int                    : 3;
        unsigned int                    : 24;
        uint32_t date;
        float starfield_intensity;
    };

#define CIGI_CELESTIAL_SPHERE_CONTROL_OPCODE    9
#define CIGI_CELESTIAL_SPHERE_CONTROL_SIZE      16

    struct CIGI_ATMOSPHERE_CONTROL {
        unsigned int packet_id          : 8;
        unsigned int packet_size        : 8;
        unsigned int atm_model_enable   : 1;
        unsigned int                    : 7;
        unsigned int global_humidity    : 8;
        float global_air_temp;
        float global_visibility;
        float global_windspeed_horz;
        float global_windspeed_vert;
        float global_wind_direction;
        float global_baro_pressure;
        unsigned int                    : 32;
    };

#define CIGI_ATMOSPHERE_CONTROL_OPCODE          10
#define CIGI_ATMOSPHERE_CONTROL_SIZE            32

    struct CIGI_ENV_REGION_CONTROL {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t region_id;
        unsigned int region_state       : 2;
        unsigned int merge_weather      : 1;
        unsigned int merge_aerosols     : 1;
        unsigned int merge_maritime     : 1;
        unsigned int merge_terrestrial  : 1;
        unsigned int                    : 2;
        unsigned int                    : 24;
        double latitude;
        double longitude;
        float size_x;
        float size_y;
        float corner_radius;
        float rotation;
        float transition_perimeter;
        unsigned int                    : 32;
    };

#define CIGI_ENV_REGION_CONTROL_OPCODE          11
#define CIGI_ENV_REGION_CONTROL_SIZE            48

    struct CIGI_WEATHER_CONTROL {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t entity_region_id;
        unsigned int layer_id           : 8;
        unsigned int humidity           : 8;
        unsigned int weather_enable     : 1;
        unsigned int scud_enable        : 1;
        unsigned int random_winds       : 1;
        unsigned int random_lightning   : 1;
        unsigned int cloud_type         : 4;
        unsigned int scope              : 2;
        unsigned int severity           : 3;
        unsigned int                    : 3;
        float air_temp;
        float visibility;
        float scud_freq;
        float coverage;
        float base_elevation;
        float thickness;
        float transition_band;
        float windspeed_horz;
        float windspeed_vert;
        float wind_direction;
        float baro_pressure;
        float aerosol_concentration;
    };

#define CIGI_WEATHER_CONTROL_OPCODE             12
#define CIGI_WEATHER_CONTROL_SIZE               56

    struct CIGI_MARITIME_CONDITIONS_CONTROL {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t entity_region_id;
        unsigned int conditions_enable  : 1;
        unsigned int whitecap_enable    : 1;
        unsigned int scope              : 2;
        unsigned int                    : 4;
        unsigned int                    : 24;
        float surface_height;
        float surface_temp;
        float surface_clarity;
        unsigned int                    : 32;
    };

#define CIGI_MARITIME_CONDITIONS_CONTROL_OPCODE 13
#define CIGI_MARITIME_CONDITIONS_CONTROL_SIZE   24

    struct CIGI_WAVE_CONTROL {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t entity_region_id;
        unsigned int wave_id            : 8;
        unsigned int wave_enable        : 1;
        unsigned int scope              : 2;
        unsigned int breaker_type       : 2;
        unsigned int                    : 3;
        unsigned int                    : 16;
        float wave_height;
        float wavelength;
        float period;
        float direction;
        float phase_offset;
        float leading;
    };

#define CIGI_WAVE_CONTROL_OPCODE                14
#define CIGI_WAVE_CONTROL_SIZE                  32

    struct CIGI_TERRESTRIAL_CONDITIONS_CONTROL {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t entity_region_id;
        unsigned int condition_id       : 16;
        unsigned int condition_enable   : 1;
        unsigned int scope              : 2;
        unsigned int severity           : 5;
        unsigned int coverage           : 8;
    };

#define CIGI_TERRESTRIAL_CONDITIONS_CONTROL_OPCODE  15
#define CIGI_TERRESTRIAL_CONDITIONS_CONTROL_SIZE    8

    struct CIGI_VIEW_CONTROL {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t view_id;
        unsigned int group_id       : 8;
        unsigned int xoffset_enable : 1;
        unsigned int yoffset_enable : 1;
        unsigned int zoffset_enable : 1;
        unsigned int roll_enable    : 1;
        unsigned int pitch_enable   : 1;
        unsigned int yaw_enable     : 1;
        unsigned int                : 2;
        unsigned int entity_id      : 16;
        float xoffset;
        float yoffset;
        float zoffset;
        float roll;
        float pitch;
        float yaw;
    };

#define CIGI_VIEW_CONTROL_OPCODE                16
#define CIGI_VIEW_CONTROL_SIZE                  32

    struct CIGI_SENSOR_CONTROL {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t view_id;
        unsigned int sensor_id          : 8;
        unsigned int sensor_on_off      : 1;
        unsigned int polarity           : 1;
        unsigned int line_dropout       : 1;
        unsigned int auto_gain          : 1;
        unsigned int track_white_black  : 1;
        unsigned int track_mode         : 3;
        unsigned int response_type      : 1;
        unsigned int                    : 7;
        unsigned int                    : 8;
        float gain;
        float level;
        float ac_coupling;
        float noise;
    };

#define CIGI_SENSOR_CONTROL_OPCODE              17
#define CIGI_SENSOR_CONTROL_SIZE                24

    struct CIGI_MOTION_TRACKER_CONTROL {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t view_or_group_id;
        unsigned int tracker_id             : 8;
        unsigned int tracker_enable         : 1;
        unsigned int boresight_enable       : 1;
        unsigned int x_enable               : 1;
        unsigned int y_enable               : 1;
        unsigned int z_enable               : 1;
        unsigned int roll_enable            : 1;
        unsigned int pitch_enable           : 1;
        unsigned int yaw_enable             : 1;
        unsigned int view_or_group_select   : 1;
        unsigned int                        : 7;
        unsigned int                        : 8;
    };

#define CIGI_MOTION_TRACKER_CONTROL_OPCODE      18
#define CIGI_MOTION_TRACKER_CONTROL_SIZE        8

    struct CIGI_ERM_DEF {
        unsigned int packet_id      : 8;
        unsigned int packet_size    : 8;
        unsigned int erm_enable     : 1;
        unsigned int                : 7;
        unsigned int                : 8;
        unsigned int                : 32;
        double equatorial_radius;
        double flattening;
    };

#define CIGI_ERM_DEF_OPCODE                 19
#define CIGI_ERM_DEF_SIZE                   24

    struct CIGI_TRAJECTORY_DEF {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t entity_id;
        float accel_x;
        float accel_y;
        float accel_z;
        float retardation_rate;
        float terminal_velocity;
    };

#define CIGI_TRAJECTORY_DEF_OPCODE          20
#define CIGI_TRAJECTORY_DEF_SIZE            24

    struct CIGI_VIEW_DEF {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t view_id;
        unsigned int group_id               : 8;
        unsigned int near_enable            : 1;
        unsigned int far_enable             : 1;
        unsigned int left_enable            : 1;
        unsigned int right_enable           : 1;
        unsigned int top_enable             : 1;
        unsigned int bottom_enable          : 1;
        unsigned int mirror_mode            : 2;
        unsigned int pixel_replication_mode : 3;
        unsigned int projection_type        : 1;
        unsigned int reorder                : 1;
        unsigned int view_type              : 3;
        unsigned int                        : 8;
        float fov_near;
        float fov_far;
        float fov_left;
        float fov_right;
        float fov_top;
        float fov_bottom;
    };

#define CIGI_VIEW_DEF_OPCODE                21
#define CIGI_VIEW_DEF_SIZE                  32

    struct CIGI_COLL_SEGMENT_DEF {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t entity_id;
        unsigned int segment_id     : 8;
        unsigned int segment_enable : 1;
        unsigned int                : 7;
        unsigned int                : 16;
        float x1;
        float y1;
        float z1;
        float x2;
        float y2;
        float z2;
        uint32_t material_mask;
        unsigned int                : 32;
    };

#define CIGI_COLL_SEGMENT_DEF_OPCODE    22
#define CIGI_COLL_SEGMENT_DEF_SIZE      40

    struct CIGI_COLL_VOLUME_DEF {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t entity_id;
        unsigned int volume_id      : 8;
        unsigned int volume_enable  : 1;
        unsigned int volume_type    : 1;
        unsigned int                : 6;
        unsigned int                : 16;
        float x;
        float y;
        float z;
        float height_radius;
        float width;
        float depth;
        float roll;
        float pitch;
        float yaw;
        unsigned int                : 32;
    };

#define CIGI_COLL_VOLUME_DEF_OPCODE     23
#define CIGI_COLL_VOLUME_DEF_SIZE       48

    struct CIGI_HAT_HOT_REQUEST {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t hat_hot_id;
        unsigned int request_type   : 2;
        unsigned int coord_system   : 1;
        unsigned int                : 5;
        unsigned int update_period  : 8;
        unsigned int entity_id      : 16;
        double lat_x;
        double lon_y;
        double alt_z;
    };

#define CIGI_HAT_HOT_REQUEST_OPCODE             24
#define CIGI_HAT_HOT_REQUEST_SIZE               32

    struct CIGI_LOS_SEGMENT_REQUEST {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t los_id;
        unsigned int request_type           : 1;
        unsigned int source_coord_system    : 1;
        unsigned int dest_coord_system      : 1;
        unsigned int response_coord_system  : 1;
        unsigned int dest_entity_id_valid   : 1;
        unsigned int                        : 3;
        unsigned int alpha_threshold        : 8;
        unsigned int entity_id              : 16;
        double source_lat_x;
        double source_lon_y;
        double source_alt_z;
        double dest_lat_x;
        double dest_lon_y;
        double dest_alt_z;
        uint32_t material_mask;
        unsigned int update_period          : 8;
        unsigned int                        : 8;
        unsigned int dest_entity_id         : 16;
    };

#define CIGI_LOS_SEGMENT_REQUEST_OPCODE     25
#define CIGI_LOS_SEGMENT_REQUEST_SIZE       64

    struct CIGI_LOS_VECTOR_REQUEST {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t los_id;
        unsigned int request_type           : 1;
        unsigned int source_coord_system    : 1;
        unsigned int resp_coord_system      : 1;
        unsigned int                        : 5;
        unsigned int alpha_threshold        : 8;
        unsigned int entity_id              : 16;
        float azimuth;
        float elevation;
        float min_range;
        float max_range;
        double source_lat_x;
        double source_lon_y;
        double source_alt_z;
        uint32_t material_mask;
        unsigned int update_period          : 8;
        unsigned int                        : 24;
    };

#define CIGI_LOS_VECTOR_REQUEST_OPCODE      26
#define CIGI_LOS_VECTOR_REQUEST_SIZE        56

    struct CIGI_POSITION_REQUEST {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t object_id;
        unsigned int artpart_id     : 8;
        unsigned int update_mode    : 1;
        unsigned int object_class   : 3;
        unsigned int coord_system   : 2;
        unsigned int                : 2;
        unsigned int                : 16;
    };

#define CIGI_POSITION_REQUEST_OPCODE        27
#define CIGI_POSITION_REQUEST_SIZE          8

    struct CIGI_ENV_CONDITIONS_REQUEST {
        unsigned int packet_id      : 8;
        unsigned int packet_size    : 8;
        unsigned int request_type   : 4;
        unsigned int                : 4;
        unsigned int request_id     : 8;
        unsigned int                : 32;
        double latitude;
        double longitude;
        double altitude;
    };

#define CIGI_ENV_CONDITIONS_REQUEST_OPCODE  28
#define CIGI_ENV_CONDITIONS_REQUEST_SIZE    32

    struct CIGI_SYMBOL_SURFACE_DEF {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t surface_id;
        unsigned int surface_state          : 1;
        unsigned int attach_type            : 1;
        unsigned int billboard              : 1;
        unsigned int persp_growth_enable    : 1;
        unsigned int                        : 4;
        unsigned int                        : 8;
        unsigned int entity_or_view_id      : 16;
        float x_left;
        float y_right;
        float z_top;
        float yaw_bottom;
        float pitch;
        float roll;
        float width;
        float height;
        float min_u;
        float max_u;
        float min_v;
        float max_v;
    };

#define CIGI_SYMBOL_SURFACE_DEF_OPCODE 29
#define CIGI_SYMBOL_SURFACE_DEF_SIZE   56

#define CIGI_SYMBOL_TEXT_MAX_TEXT_SIZE 236
    struct CIGI_SYMBOL_TEXT_DEF {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t symbol_id;
        unsigned int alignment      : 4;
        unsigned int orientation    : 2;
        unsigned int                : 2;
        unsigned int font_id        : 8;
        unsigned int                : 16;
        float font_size;
        char text[CIGI_SYMBOL_TEXT_MAX_TEXT_SIZE];
    };

#define CIGI_SYMBOL_TEXT_DEF_OPCODE     30
#define CIGI_SYMBOL_TEXT_DEF_SIZE       256
#define CIGI_SYMBOL_TEXT_DEF_BASE_SIZE  4

    struct CIGI_CIRCLE_DEF_ELEMENT {
        float line_width;
        float stipple_pattern_length;
        float center_u;
        float center_v;
        float radius;
        float inner_radius;
        float start_angle;
        float end_angle;
    };

#define CIGI_SYMBOL_CIRCLE_DEF_MAX_ELEMENT_CNT  10
    struct CIGI_SYMBOL_CIRCLE_DEF {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t symbol_id;
        unsigned int drawing_style   : 1;
        unsigned int                 : 7;
        unsigned int                 : 8;
        unsigned int stipple_pattern : 16;
        float line_width;
        float stipple_pattern_length;
        CIGI_CIRCLE_DEF_ELEMENT circle[CIGI_SYMBOL_CIRCLE_DEF_MAX_ELEMENT_CNT];
    };

#define CIGI_SYMBOL_CIRCLE_DEF_OPCODE    31
#define CIGI_SYMBOL_CIRCLE_DEF_SIZE      256
#define CIGI_SYMBOL_CIRCLE_DEF_BASE_SIZE 16
#define CIGI_SYMBOL_CIRCLE_ELEMENT_SIZE  24

    struct CIGI_LINE_DEF_ELEMENT {
        float vertex_u;
        float vertex_v;
    };

#define CIGI_SYMBOL_LINE_DEF_MAX_VERTEX_CNT  30
    struct CIGI_SYMBOL_LINE_DEF {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t symbol_id;
        unsigned int primitive_type  : 4;
        unsigned int                 : 4;
        unsigned int                 : 8;
        unsigned int stipple_pattern : 16;
        float line_width;
        float stipple_pattern_length;
        CIGI_LINE_DEF_ELEMENT line[CIGI_SYMBOL_LINE_DEF_MAX_VERTEX_CNT];
    };

#define CIGI_SYMBOL_LINE_DEF_OPCODE    32
#define CIGI_SYMBOL_LINE_DEF_SIZE      256
#define CIGI_SYMBOL_LINE_DEF_BASE_SIZE 16
#define CIGI_SYMBOL_VERTEX_SIZE        8

    struct CIGI_SYMBOL_CLONE {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t symbol_id;
        unsigned int source_type    : 1;
        unsigned int                : 7;
        unsigned int                : 8;
        unsigned int source_id      : 16;
    };

#define CIGI_SYMBOL_CLONE_OPCODE   33
#define CIGI_SYMBOL_CLONE_SIZE     8

    struct CIGI_SYMBOL_CONTROL {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t symbol_id;
        unsigned int symbol_state   : 2;
        unsigned int attach_state   : 1;
        unsigned int reset_flash    : 1;
        unsigned int inherit_color  : 1;
        unsigned int                : 3;
        unsigned int                : 8;
        unsigned int parent_id      : 16;
        uint16_t surface_id;
        uint8_t layer;
        uint8_t flash_duty_cycle;
        float flash_period;
        float pos_u;
        float pos_v;
        float rotation;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;
        float scale_u;
        float scale_v;
    };

#define CIGI_SYMBOL_CONTROL_OPCODE   34
#define CIGI_SYMBOL_CONTROL_SIZE     40

    struct CIGI_SYMBOL_CONTROL_S {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t symbol_id;
        unsigned int symbol_state   : 2;
        unsigned int attach_state   : 1;
        unsigned int reset_flash    : 1;
        unsigned int inherit_color  : 1;
        unsigned int                : 3;
        unsigned int                : 8;
        unsigned int attribute1     : 8;
        unsigned int attribute2     : 8;
        uint32_t attribute1_value;
        uint32_t attribute2_value;
    };

#define CIGI_SYMBOL_CONTROL_S_OPCODE   35
#define CIGI_SYMBOL_CONTROL_S_SIZE     16

    // CIGI Start of Frame (CIGI v3.0 version)
    struct CIGI_START_OF_FRAME_30 {
        uint8_t packet_id;                   /** Packet ID = 101         */
        uint8_t packet_size;                 /** Packet size = 16 bytes  */
        uint8_t cigi_major_version;          /** CIGI version ID         */
        int8_t db_number;                    /** Database number         */
        unsigned int ig_status       : 8;    /** IG status code          */
        unsigned int ig_mode         : 2;    /** IG mode                 */
        unsigned int timestamp_valid : 1;    /** Timestamp valid flag    */
        unsigned int earth_ref_model : 1;    /** Earth Reference Model   */
        unsigned int                 : 4;    /** Reserved                */
        unsigned int byteswap        : 16;   /** Byteswap magic number   */
        uint32_t frame_ctr;                  /** Frame counter           */
        uint32_t timestamp;                  /** Timing value/tag        */
    };

    // CIGI Start of Frame (CIGI v3.2 version)
    struct CIGI_START_OF_FRAME_32 {
        uint8_t packet_id;                   /** Packet ID = 101         */
        uint8_t packet_size;                 /** Packet size = 24 bytes  */
        uint8_t cigi_major_version;          /** CIGI version ID         */
        int8_t db_number;                    /** Database number         */
        unsigned int ig_status       : 8;    /** IG status code          */
        unsigned int ig_mode         : 2;    /** IG mode                 */
        unsigned int timestamp_valid : 1;    /** Timestamp valid flag    */
        unsigned int earth_ref_model : 1;    /** Earth Reference Model   */
        unsigned int minor_version   : 4;    /** CIGI minor version      */
        unsigned int byteswap        : 16;   /** Byteswap magic number   */
        uint32_t frame_ctr;                  /** Frame counter           */
        uint32_t timestamp;                  /** Timing value/tag        */
        uint32_t last_frame_ctr;
        unsigned int                 : 32;
    };

    union CIGI_START_OF_FRAME {
        CIGI_START_OF_FRAME_32 sof32;
        CIGI_START_OF_FRAME_30 sof30;
    };

#define CIGI_START_OF_FRAME_OPCODE          101
#define CIGI_START_OF_FRAME_SIZE            16

    struct CIGI_HAT_HOT_RESPONSE {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t hat_hot_id;
        unsigned int valid          : 1;
        unsigned int response_type  : 1;
        unsigned int                : 2;
        unsigned int frame_num_lsn  : 4;
        unsigned int                : 24;
        double height;
    };

#define CIGI_HAT_HOT_RESPONSE_OPCODE            102
#define CIGI_HAT_HOT_RESPONSE_SIZE              16

    struct CIGI_HAT_HOT_EXT_RESPONSE {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t hat_hot_id;
        unsigned int valid          : 1;
        unsigned int                : 3;
        unsigned int frame_num_lsn  : 4;
        unsigned int                : 24;
        double hat;
        double hot;
        uint32_t material_code;
        float normal_azimuth;
        float normal_elevation;
        unsigned int                : 32;
    };

#define CIGI_HAT_HOT_EXT_RESPONSE_OPCODE        103
#define CIGI_HAT_HOT_EXT_RESPONSE_SIZE          40

    struct CIGI_LOS_RESPONSE {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t los_id;
        unsigned int valid              : 1;
        unsigned int entity_id_valid    : 1;
        unsigned int visible            : 1;
        unsigned int                    : 1;
        unsigned int frame_num_lsn      : 4;
        unsigned int response_count     : 8;
        unsigned int entity_id          : 16;
        double range;
    };

#define CIGI_LOS_RESPONSE_OPCODE            104
#define CIGI_LOS_RESPONSE_SIZE              16

    struct CIGI_LOS_EXT_RESPONSE {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t los_id;
        unsigned int valid              : 1;
        unsigned int entity_id_valid    : 1;
        unsigned int range_valid        : 1;
        unsigned int visible            : 1;
        unsigned int frame_num_lsn      : 4;
        unsigned int response_count     : 8;
        unsigned int entity_id          : 16;
        double range;
        double lat_x;
        double lon_y;
        double alt_z;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;
        uint32_t material_code;
        float normal_azimuth;
        float normal_elevation;
    };

#define CIGI_LOS_EXT_RESPONSE_OPCODE        105
#define CIGI_LOS_EXT_RESPONSE_SIZE          56

    struct CIGI_SENSOR_RESPONSE {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t view_id;
        unsigned int sensor_id      : 8;
        unsigned int sensor_status  : 2;
        unsigned int                : 6;
        unsigned int                : 16;
        uint16_t gate_x_size;
        uint16_t gate_y_size;
        float gate_x_position;
        float gate_y_position;
        uint32_t frame_counter;
    };

#define CIGI_SENSOR_RESPONSE_OPCODE         106
#define CIGI_SENSOR_RESPONSE_SIZE           24

    struct CIGI_SENSOR_EXT_RESPONSE {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t view_id;
        unsigned int sensor_id          : 8;
        unsigned int sensor_status      : 2;
        unsigned int entity_id_valid    : 1;
        unsigned int                    : 5;
        unsigned int entity_id          : 16;
        uint16_t gate_x_size;
        uint16_t gate_y_size;
        float gate_x_position;
        float gate_y_position;
        uint32_t frame_counter;
        double lat;
        double lon;
        double alt;
    };

#define CIGI_SENSOR_EXT_RESPONSE_OPCODE         107
#define CIGI_SENSOR_EXT_RESPONSE_SIZE           48

    struct CIGI_POSITION_RESPONSE {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t object_id;
        unsigned int artpart_id         : 8;
        unsigned int object_class       : 3;
        unsigned int coord_system       : 2;
        unsigned int                    : 3;
        unsigned int                    : 16;
        double lat_x;
        double lon_y;
        double alt_z;
        float roll;
        float pitch;
        float yaw;
        unsigned int                    : 32;
    };

#define CIGI_POSITION_RESPONSE_OPCODE           108
#define CIGI_POSITION_RESPONSE_SIZE             48

    struct CIGI_WEATHER_RESPONSE {
        uint8_t packet_id;
        uint8_t packet_size;
        uint8_t request_id;
        uint8_t humidity;
        float air_temp;
        float visibility;
        float windspeed_horz;
        float windspeed_vert;
        float wind_direction;
        float baro_pressure;
        unsigned int          : 32;
    };

#define CIGI_WEATHER_RESPONSE_OPCODE        109
#define CIGI_WEATHER_RESPONSE_SIZE          32

    struct CIGI_AEROSOL_RESPONSE {
        uint8_t packet_id;
        uint8_t packet_size;
        uint8_t request_id;
        uint8_t layer_id;
        float concentration;
    };

#define CIGI_AEROSOL_RESPONSE_OPCODE        110
#define CIGI_AEROSOL_RESPONSE_SIZE          8

    struct CIGI_MARITIME_SURFACE_RESPONSE {
        unsigned int packet_id      : 8;
        unsigned int packet_size    : 8;
        unsigned int request_id     : 8;
        unsigned int                : 8;
        float surface_height;
        float surface_temp;
        float surface_clarity;
    };

#define CIGI_MARITIME_SURFACE_RESPONSE_OPCODE   111
#define CIGI_MARITIME_SURFACE_RESPONSE_SIZE     16

    struct CIGI_TERRESTRIAL_SURFACE_RESPONSE {
        unsigned int packet_id      : 8;
        unsigned int packet_size    : 8;
        unsigned int request_id     : 8;
        unsigned int                : 8;
        uint32_t condition_id;
    };

#define CIGI_TERRESTRIAL_SURFACE_RESPONSE_OPCODE    112
#define CIGI_TERRESTRIAL_SURFACE_RESPONSE_SIZE      8

    struct CIGI_COLL_SEGMENT_NOTIFICATION {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t entity_id;
        unsigned int segment_id             : 8;
        unsigned int collision_type         : 1;
        unsigned int                        : 7;
        unsigned int contacted_entity_id    : 16;
        uint32_t material_code;
        float intersection_distance;
    };

#define CIGI_COLL_SEGMENT_NOTIFICATION_OPCODE   113
#define CIGI_COLL_SEGMENT_NOTIFICATION_SIZE     16

    struct CIGI_COLL_VOLUME_NOTIFICATION {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t entity_id;
        unsigned int volume_id              : 8;
        unsigned int collision_type         : 1;
        unsigned int                        : 7;
        unsigned int contacted_entity_id    : 16;
        unsigned int contacted_volume_id    : 8;
        unsigned int                        : 24;
        unsigned int                        : 32;
    };

#define CIGI_COLL_VOLUME_NOTIFICATION_OPCODE    114
#define CIGI_COLL_VOLUME_NOTIFICATION_SIZE      16

    struct CIGI_ANIMATION_STOP_NOTIFICATION {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t entity_id;
        unsigned int                : 32;
    };

#define CIGI_ANIMATION_STOP_NOTIFICATION_OPCODE     115
#define CIGI_ANIMATION_STOP_NOTIFICATION_SIZE       8

    struct CIGI_EVENT_NOTIFICATION {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t event_id;
        uint32_t data1;
        uint32_t data2;
        uint32_t data3;
    };

#define CIGI_EVENT_NOTIFICATION_OPCODE              116
#define CIGI_EVENT_NOTIFICATION_SIZE                16

    struct CIGI_IG_MESSAGE {
        uint8_t packet_id;
        uint8_t packet_size;
        uint16_t message_id;
        char text[100];
    };

#define CIGI_IG_MESSAGE_OPCODE                      117
#define CIGI_IG_MESSAGE_SIZE                        104
#define CIGI_IG_MESSAGE_BASE_SIZE                   4

    struct CIGI_USER_DATA_PACKET {
        uint8_t packet_id;
        uint8_t packet_size;
        uint8_t *data_ptr;
    };

#define CIGI_USER_DATA_PACKET_OPCODE_MIN    201
#define CIGI_USER_DATA_PACKET_OPCODE_MAX    255
#define CIGI_USER_DATA_PACKET_BASE_SIZE     2

#ifdef __cplusplus
}
#endif

#endif // _CIGI3TYPES_H_