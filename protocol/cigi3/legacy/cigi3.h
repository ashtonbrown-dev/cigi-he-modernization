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

#ifndef _CIGI_API_INCLUDED_
#define _CIGI_API_INCLUDED_

#include <cstdint>
#include <cstdlib>

/** CIGI API status codes */
#define CIGI_SUCCESS                              0
#define CIGI_ERROR_NONE                           0
#define CIGI_ERROR_MISCELLANEOUS                 -1
#define CIGI_ERROR_UNEXPECTED_NULL               -2
#define CIGI_ERROR_BUFFER_OVERRUN                -3
#define CIGI_ERROR_CALLED_OUT_OF_SEQUENCE        -4
#define CIGI_ERROR_INVALID_PACKET                -5
#define CIGI_ERROR_INVALID_OPCODE                -6
#define CIGI_ERROR_ALLOC_FAILED                  -7
#define CIGI_ERROR_BUFFER_TOO_SMALL              -8
#define CIGI_ERROR_MAX_SESSIONS                  -9
#define CIGI_ERROR_INVALID_SESSION              -10
#define CIGI_ERROR_INVALID_SESSION_TYPE         -11
#define CIGI_ERROR_MISSING_IG_CONTROL_PACKET    -12
#define CIGI_ERROR_MISSING_SOF_PACKET           -13
#define CIGI_ERROR_NOT_IMPLEMENTED              -14
#define CIGI_ERROR_PACKET_TOO_LARGE             -15
#define CIGI_ERROR_INVALID_PACKET_SIZE          -16

/** The maximum number of sessions to allocate memory for. */
#define CIGI_MAX_SESSIONS                       128

/** Invalid session flag. */
#define CIGI_INVALID_SESSION                     -1

/** Communication mode. */
#define CIGI_HOST_SESSION                         1
#define CIGI_IG_SESSION                           2

/** Macros for copying bytes of data from one address to another. */
#define CigiCopy1( _dest_, _src_)                           \
    { *((uint8_t *)_dest_) = *((uint8_t *)_src_); }

#define CigiCopy2( _dest_, _src_)                           \
    { *((uint16_t *)_dest_) = *((uint16_t *)_src_); }

#define CigiCopy4( _dest_, _src_)                           \
    { *((uint32_t *)_dest_) = *((uint32_t *)_src_); }

#define CigiCopy8( _dest_, _src_)                           \
    { *((uint64_t *)_dest_) = *((uint64_t *)_src_); }

/** Forward declarations. */
union CIGI_IG_CONTROL;
struct CIGI_IG_CONTROL_30;
struct CIGI_IG_CONTROL_32;
struct CIGI_ENTITY_CONTROL;
struct CIGI_ENTITY_CONTROL_CC;
struct CIGI_COMPONENT_CONTROL_32;
struct CIGI_COMPONENT_CONTROL_33;
union CIGI_COMPONENT_CONTROL;
struct CIGI_COMPONENT_CONTROL_S_32;
struct CIGI_COMPONENT_CONTROL_S_33;
union CIGI_COMPONENT_CONTROL_S;
struct CIGI_ARTPART_CONTROL;
struct CIGI_ARTPART_CONTROL_S;
struct CIGI_RATE_CONTROL;
struct CIGI_CELESTIAL_SPHERE_CONTROL;
struct CIGI_ATMOSPHERE_CONTROL;
struct CIGI_ENV_REGION_CONTROL;
struct CIGI_WEATHER_CONTROL;
struct CIGI_MARITIME_CONDITIONS_CONTROL;
struct CIGI_WAVE_CONTROL;
struct CIGI_TERRESTRIAL_CONDITIONS_CONTROL;
struct CIGI_VIEW_CONTROL;
struct CIGI_SENSOR_CONTROL;
struct CIGI_MOTION_TRACKER_CONTROL;
struct CIGI_ERM_DEF;
struct CIGI_TRAJECTORY_DEF;
struct CIGI_VIEW_DEF;
struct CIGI_COLL_SEGMENT_DEF;
struct CIGI_COLL_VOLUME_DEF;
struct CIGI_HAT_HOT_REQUEST;
struct CIGI_LOS_SEGMENT_REQUEST;
struct CIGI_LOS_VECTOR_REQUEST;
struct CIGI_POSITION_REQUEST;
struct CIGI_ENV_CONDITIONS_REQUEST;
union CIGI_START_OF_FRAME;
struct CIGI_START_OF_FRAME_30;
struct CIGI_START_OF_FRAME_32;
struct CIGI_HAT_HOT_RESPONSE;
struct CIGI_HAT_HOT_EXT_RESPONSE;
struct CIGI_LOS_RESPONSE;
struct CIGI_LOS_EXT_RESPONSE;
struct CIGI_SENSOR_RESPONSE;
struct CIGI_SENSOR_EXT_RESPONSE;
struct CIGI_POSITION_RESPONSE;
struct CIGI_WEATHER_RESPONSE;
struct CIGI_AEROSOL_RESPONSE;
struct CIGI_MARITIME_SURFACE_RESPONSE;
struct CIGI_TERRESTRIAL_SURFACE_RESPONSE;
struct CIGI_COLL_SEGMENT_NOTIFICATION;
struct CIGI_COLL_VOLUME_NOTIFICATION;
struct CIGI_ANIMATION_STOP_NOTIFICATION;
struct CIGI_EVENT_NOTIFICATION;
struct CIGI_IG_MESSAGE;
struct CIGI_SYMBOL_SURFACE_DEF;
struct CIGI_SYMBOL_CONTROL;
struct CIGI_SYMBOL_CONTROL_S;
struct CIGI_SYMBOL_TEXT_DEF;
struct CIGI_SYMBOL_CIRCLE_DEF;
struct CIGI_SYMBOL_LINE_DEF;
struct CIGI_SYMBOL_CLONE;
struct CIGI_ENTITY_CONTROL;
struct CIGI_ENTITY_CONTROL_CC;
struct CIGI_USER_DATA_PACKET;

/** Internal structure for holding variables associated with a session. */
struct _CIGI_SESSION {
    int SessionId;                        /** Unique session id */
    int SessionType;                      /** Session type: IG to host or host to IG */
    unsigned char *ReceivedBuffer;        /** Pointer to the incoming buffer */
    int ReceivedBufferSize;               /** Size in bytes of the incoming buffer */
    int NumBuffers;                       /** The total number of outgoing buffers */
    int BufferSize;                       /** The size of each outgoing buffer */
    int StartBufferIdx;                   /** Index of the start buffer for the current message */
    int BufferIdx;                        /** Index of the current buffer */
    int OverflowCount;                    /** The number of buffers the current buffer is ahead of the start */
    unsigned char IncomingBufferUnpacked; /** Flag for if the incoming buffer has been unpacked */
    unsigned char ExplicitAddFirstPacket; /** Flag for if the SOF or IG control packet was explicitly added */
    unsigned short reserved1;
    unsigned int FrameCounter;            /** Frame counter for SOF packets */
    unsigned int reserved2;
    unsigned char *MemoryBlockPtr;        /** Pointer to the memory allocated for the buffers */
    unsigned char **BufferPtrs;           /** Pointers to the current position within each buffer */
    unsigned char **BasePtrs;             /** Pointers to the start of each buffer */
    unsigned char **ThresholdPtrs;        /** Pointers to the ends of each buffer */
};
typedef struct _CIGI_SESSION CIGI_SESSION;

/** Structure for the header information found in the first 3 bytes of each packet. */
struct _CIGI_PACKET_HEADER {
    unsigned char packet_id;        /** Packet ID number (type). */
    unsigned char packet_size;      /** The size of the packet, in bytes. */
    unsigned char version_number;   /** Valid for SOF packet only. */
};
typedef struct _CIGI_PACKET_HEADER CIGI_PACKET_HEADER;

/** Union type used in internal byte swapping routines. */
union _SWAP_UNION {
    unsigned long i;
    unsigned char b[4];
};
typedef union _SWAP_UNION SWAP_UNION;

/** type for the callback procedure function pointers */
typedef long(* CIGICBPROC)(const int sessionid, void *packet);

/* ****** function prototypes ****** */
int CigiGetMinorVersion(void);
void CigiSetMinorVersion(const int version);
int PrepareSessionAndGetPtr(const int sessionid, const int packetsize, CIGI_SESSION **ppcs);

/* -------- startup / shutdown ------------------------------------ */
/*:Associate with "Startup_Shutdown_Functions" */
int CigiInit(const int maxsessions, const int minver);
int CigiShutdown(void);

int CigiCreateSession(
    const int type,
    const int numbuffers,
    const int buffersize
);
void CigiCloseSession(const int sessionid);

int CigiSetCallback(const int opcode, const CIGICBPROC proc);
void CigiSetDefaultCallback(const CIGICBPROC proc);

/* -------- byte swapping functions ------------------------------- */
/*:Associate with "Utility_Functions" */
void CigiSwap2(void *dest, const void *src);
void CigiSwap4(void *dest, const void *src);
void CigiSwap8(void *dest, const void *src);

/* -------- incoming messages ------------------------------------- */
/*:Associate with "Incoming_Message_Functions" */
int CigiProcessIncomingMsgBuffer(const int sessionid);
int CigiGetFirstPacket(const int sessionid, unsigned char **packet);
int CigiGetNextPacket(const int sessionid, unsigned char **packet);

int CigiSetIncomingMsgBuffer(
    const int sessionid,
    const unsigned char *buffer,
    const int size
);

int CigiGetIncomingMsgBuffer(
    const int sessionid,
    unsigned char **buffer,
    int *size
);

/* -------- outgoing messages ------------------------------------- */
/*:Associate with "Outgoing_Message_Functions" */
int CigiStartMessage(const int sessionid);
int CigiEndMessage(const int sessionid);

int CigiGetOutgoingMsgBuffer(
    const int sessionid,
    unsigned char **buffer,
    int *size
);

int CigiSyncFrameCounter(const int sessionid);

// This is for debugging only:
int CigiSwapOutgoingMsgBuffer(char *buffer, const int size);

int CigiSearchForPacket(const int sessionid, const char *mask, const char *packet, char **dest);

/* -------- messaging : host to IG -------------------------------- */
/*:Associate with "Host_Session_Functions" */
int CigiAddFixedPacket(int sessionid,
                       const char *packet,
                       const uint8_t packetSize,
                       bool explicitAddFirstPacket = false,
                       bool attemptReplace = false,
                       const char *replaceMask = NULL);
int CigiAddPacketIGCtrl(const int sessionid, const CIGI_IG_CONTROL *packet);
int CigiAddPacketSof(const int sessionid, const CIGI_START_OF_FRAME *packet);
int CigiAddPacketIGMessage(const int sessionid, const CIGI_IG_MESSAGE *packet);

// See documentation for non-templated CigiAddFixedPacket in cigi3.cpp.
template <class PacketT>
int CigiAddFixedPacket(int sessionid,
                       const PacketT *packet,
                       bool explicitAddFirstPacket = false,
                       bool attemptReplace = false,
                       const char *replaceMask = NULL)
{
    const uint8_t packetSize = (uint8_t)sizeof(PacketT);

    return CigiAddFixedPacket(sessionid,
                              (const char *)packet,
                              packetSize,
                              explicitAddFirstPacket,
                              attemptReplace,
                              replaceMask);
}

// See documentation for non-templated CigiAddFixedPacket in cigi3.cpp.
template <class PacketT>
int CigiAddReplaceFixedPacket(int sessionid,
                              const PacketT *packet,
                              const PacketT *replaceMask)
{
    const bool explicitAddFirstPacket = false;
    const bool attemptReplace = true;

    return CigiAddFixedPacket<PacketT>(sessionid,
                                       packet,
                                       explicitAddFirstPacket,
                                       attemptReplace,
                                       (const char *)replaceMask);
}

int CigiAddPacketReplaceEntityCtrl(const int sessionid, const CIGI_ENTITY_CONTROL *packet);
int CigiAddPacketReplaceCCEntityCtrl(const int sessionid, const CIGI_ENTITY_CONTROL_CC *packet);
int CigiAddPacketReplaceComponentCtrl32(const int sessionid, const CIGI_COMPONENT_CONTROL_32 *packet);
int CigiAddPacketReplaceComponentCtrl33(const int sessionid, const CIGI_COMPONENT_CONTROL_33 *packet);
int CigiAddPacketReplaceShortComponentCtrl32(const int sessionid, const CIGI_COMPONENT_CONTROL_S_32 *packet);
int CigiAddPacketReplaceShortComponentCtrl33(const int sessionid, const CIGI_COMPONENT_CONTROL_S_33 *packet);
int CigiAddPacketReplaceArtPartCtrl(const int sessionid, const CIGI_ARTPART_CONTROL *packet);
int CigiAddPacketReplaceShortArtPartCtrl(const int sessionid, const CIGI_ARTPART_CONTROL_S *packet);
int CigiAddPacketReplaceRateCtrl(const int sessionid, const CIGI_RATE_CONTROL *packet);
int CigiAddPacketReplaceCelestialSphereCtrl(const int sessionid, const CIGI_CELESTIAL_SPHERE_CONTROL *packet);
int CigiAddPacketReplaceAtmosphereCtrl(const int sessionid, const CIGI_ATMOSPHERE_CONTROL *packet);
int CigiAddPacketReplaceEnvRegionCtrl(const int sessionid, const CIGI_ENV_REGION_CONTROL *packet);
int CigiAddPacketReplaceWeatherCtrl(const int sessionid, const CIGI_WEATHER_CONTROL *packet);
int CigiAddPacketReplaceMaritimeCondCtrl(const int sessionid, const CIGI_MARITIME_CONDITIONS_CONTROL *packet);
int CigiAddPacketReplaceWaveCtrl(const int sessionid, const CIGI_WAVE_CONTROL *packet);
int CigiAddPacketReplaceTerrestrialCondCtrl(const int sessionid, const CIGI_TERRESTRIAL_CONDITIONS_CONTROL *packet);
int CigiAddPacketReplaceViewCtrl(const int sessionid, const CIGI_VIEW_CONTROL *packet);
int CigiAddPacketReplaceSensorCtrl(const int sessionid, const CIGI_SENSOR_CONTROL *packet);
int CigiAddPacketReplaceMotionTrackerCtrl(const int sessionid, const CIGI_MOTION_TRACKER_CONTROL *packet);
int CigiAddPacketReplaceErmDef(const int sessionid, const CIGI_ERM_DEF *packet);
int CigiAddPacketReplaceTrajectoryDef(const int sessionid, const CIGI_TRAJECTORY_DEF *packet);
int CigiAddPacketReplaceViewDef(const int sessionid, const CIGI_VIEW_DEF *packet);
int CigiAddPacketReplaceCollDetSegmentDef(const int sessionid, const CIGI_COLL_SEGMENT_DEF *packet);
int CigiAddPacketReplaceCollDetVolumeDef(const int sessionid, const CIGI_COLL_VOLUME_DEF *packet);
int CigiAddPacketReplaceHatHotReq(const int sessionid, const CIGI_HAT_HOT_REQUEST *packet);
int CigiAddPacketReplaceLosSegmentReq(const int sessionid, const CIGI_LOS_SEGMENT_REQUEST *packet);
int CigiAddPacketReplaceLosVectorReq(const int sessionid, const CIGI_LOS_VECTOR_REQUEST *packet);
int CigiAddPacketReplacePositionReq(const int sessionid, const CIGI_POSITION_REQUEST *packet);
int CigiAddPacketReplaceEnvConditionsReq(const int sessionid, const CIGI_ENV_CONDITIONS_REQUEST *packet);
int CigiAddPacketReplaceHatResp(const int sessionid, const CIGI_HAT_HOT_RESPONSE *packet);
int CigiAddPacketReplaceHatExtResp(const int sessionid, const CIGI_HAT_HOT_EXT_RESPONSE *packet);
int CigiAddPacketReplaceLosResp(const int sessionid, const CIGI_LOS_RESPONSE *packet);
int CigiAddPacketReplaceLosExtResp(const int sessionid, const CIGI_LOS_EXT_RESPONSE *packet);
int CigiAddPacketReplaceSensorResp(const int sessionid, const CIGI_SENSOR_RESPONSE *packet);
int CigiAddPacketReplaceSensorExtResp(const int sessionid, const CIGI_SENSOR_EXT_RESPONSE *packet);
int CigiAddPacketReplacePositionResp(const int sessionid, const CIGI_POSITION_RESPONSE *packet);
int CigiAddPacketReplaceWeatherResp(const int sessionid, const CIGI_WEATHER_RESPONSE *packet);
int CigiAddPacketReplaceAerosolResp(const int sessionid, const CIGI_AEROSOL_RESPONSE *packet);
int CigiAddPacketReplaceMaritimeCondResp(const int sessionid, const CIGI_MARITIME_SURFACE_RESPONSE *packet);
int CigiAddPacketReplaceTerrestrialCondResp(const int sessionid, const CIGI_TERRESTRIAL_SURFACE_RESPONSE *packet);
int CigiAddPacketReplaceCollDetSegmentNotification(const int sessionid, const CIGI_COLL_SEGMENT_NOTIFICATION *packet);
int CigiAddPacketReplaceCollDetVolumeNotification(const int sessionid, const CIGI_COLL_VOLUME_NOTIFICATION *packet);
int CigiAddPacketReplaceAnimationStopNotification(const int sessionid, const CIGI_ANIMATION_STOP_NOTIFICATION *packet);
int CigiAddPacketReplaceEventNotification(const int sessionid, const CIGI_EVENT_NOTIFICATION *packet);
int CigiAddPacketReplaceSymbolSurfaceDef(const int sessionid, const CIGI_SYMBOL_SURFACE_DEF *packet);
int CigiAddPacketReplaceSymbolCtrl(const int sessionid, const CIGI_SYMBOL_CONTROL *packet);
int CigiAddPacketReplaceShortSymbolCtrl(const int sessionid, const CIGI_SYMBOL_CONTROL_S *packet);
int CigiAddPacketReplaceSymbolTextDef(const int sessionid, const CIGI_SYMBOL_TEXT_DEF *packet);
int CigiAddPacketReplaceSymbolCircleDef(const int sessionid, const CIGI_SYMBOL_CIRCLE_DEF *packet);
int CigiAddPacketReplaceSymbolLineDef(const int sessionid, const CIGI_SYMBOL_LINE_DEF *packet);
int CigiAddPacketReplaceSymbolClone(const int sessionid, const CIGI_SYMBOL_CLONE *packet);

#endif  /* _CIGI_API_INCLUDED_ */
