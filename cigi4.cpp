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
 *  FILENAME:   Cigi4.cpp
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
 *  MES Corp.
 *  Version 4.0.0
 */

#include "cigi4.h"
#include "cigi4types.h"

#include <memory.h>
#include <string.h>
#include <malloc.h>

/* -------- preprocessor and macro definitions ------------------------------ */

/** The number of total callbacks.
  */
#define CIGI_NUM_CALLBACKS  0x10000 // chas - 

/** The index to use in the _CigiCallbackProc[] array for the default callback
  * function.
  */
#define CIGI_DEFAULT_CALLBACK_INDEX  0x0f00 // chas - no longer can be 0. Zero is now IG Control

/** Macro for determining if a session is valid.
  */
#define CIGI_IS_VALID_SESSION(_SESSID_)                     \
    ( ( ( ( _SESSID_) < 0)                                  \
        || ( ( _SESSID_) >= _CigiMaxSessions))              \
      ? 0                                                   \
      : ( _CigiSessionPtrs[ ( _SESSID_)] != NULL))

/* -------- module-level variables ------------------------------------------ */
/** The minimum version of CIGI to work with. */
/*:Associate with "API_Internals" */
static int _CigiMinVersion = 0;

/** The maximum number of sessions to create. */
/*:Associate with "API_Internals" */
static int _CigiMaxSessions = 0;

/** The index of the next session to create. */
/*:Associate with "API_Internals" */
static int _CigiSessionIdx = -1;

/** An array to hold the session pointers. */
/*:Associate with "API_Internals" */
static CIGI_SESSION **_CigiSessionPtrs = NULL;

/** Function pointers to the callback procedures. */
/*:Associate with "API_Internals" */
static CIGICBPROC _CigiCallbackProc[CIGI_NUM_CALLBACKS];

/** Minor version number; can be decreased from remote device. */
/*:Associate with "API_Internals" */
static int _CigiMinorVersion = 0;

/* -------- local function prototypes --------------------------------------- */
/*:Associate with "API_Internals" */
static int GetSessionPtr(const int sessionid, CIGI_SESSION **ppcs);

static int CigiCompareMemoryMasked(const char *ptr1, const char *ptr2, const char *mask, const int length);

static int CigiSwapIncomingMsgBuffer(char *buffer, const int size);
static int CigiSwapPacketIGCtrl(const CIGI_IG_CONTROL *packet, char *dest);
static int CigiSwapPacketEntityPosition(const CIGI_ENTITY_POSITION *packet, char *dest);
static int CigiSwapPacketCCEntityPosition(const CIGI_ENTITY_POSITION_CC *packet, char *dest);
static int CigiSwapPacketComponentCtrl(const CIGI_COMPONENT_CONTROL *packet, char *dest);
static int CigiSwapPacketShortComponentCtrl(const CIGI_COMPONENT_CONTROL_S *packet, char *dest);
static int CigiSwapPacketArtPartCtrl(const CIGI_ARTPART_CONTROL *packet, char *dest);
static int CigiSwapPacketShortArtPartCtrl(const CIGI_ARTPART_CONTROL_S *packet, char *dest);
static int CigiSwapPacketVelocityCtrl(const CIGI_VELOCITY_CONTROL *packet, char *dest);
static int CigiSwapPacketCelestialSphereCtrl(const CIGI_CELESTIAL_SPHERE_CONTROL *packet, char *dest);
static int CigiSwapPacketAtmosphereCtrl(const CIGI_ATMOSPHERE_CONTROL *packet, char *dest);
static int CigiSwapPacketEnvRegionCtrl(const CIGI_ENV_REGION_CONTROL *packet, char *dest);
static int CigiSwapPacketWeatherCtrl(const CIGI_WEATHER_CONTROL *packet, char *dest);
static int CigiSwapPacketMaritimeCondCtrl(const CIGI_MARITIME_CONDITIONS_CONTROL *packet, char *dest);
static int CigiSwapPacketWaveCtrl(const CIGI_WAVE_CONTROL *packet, char *dest);
static int CigiSwapPacketTerrestrialCondCtrl(const CIGI_TERRESTRIAL_CONDITIONS_CONTROL *packet, char *dest);
static int CigiSwapPacketViewCtrl(const CIGI_VIEW_CONTROL *packet, char *dest);
static int CigiSwapPacketSensorCtrl(const CIGI_SENSOR_CONTROL *packet, char *dest);
static int CigiSwapPacketMotionTrackerCtrl(const CIGI_MOTION_TRACKER_CONTROL *packet, char *dest);
static int CigiSwapPacketErmDef(const CIGI_ERM_DEF *packet, char *dest);
static int CigiSwapPacketAccelerationDef(const CIGI_ACCELERATION_CONTROL_DEF *packet, char *dest);
static int CigiSwapPacketViewDef(const CIGI_VIEW_DEF *packet, char *dest);
static int CigiSwapPacketCollSegmentDef(const CIGI_COLL_SEGMENT_DEF *packet, char *dest);
static int CigiSwapPacketCollVolumeDef(const CIGI_COLL_VOLUME_DEF *packet, char *dest);
static int CigiSwapPacketHatHotReq(const CIGI_HAT_HOT_REQUEST *packet, char *dest);
static int CigiSwapPacketLosSegmentReq(const CIGI_LOS_SEGMENT_REQUEST *packet, char *dest);
static int CigiSwapPacketLosVectorReq(const CIGI_LOS_VECTOR_REQUEST *packet, char *dest);
static int CigiSwapPacketPositionReq(const CIGI_POSITION_REQUEST *packet, char *dest);
static int CigiSwapPacketEnvCondReq(const CIGI_ENV_CONDITIONS_REQUEST *packet, char *dest);
static int CigiSwapPacketSymbolSurfaceDef(const CIGI_SYMBOL_SURFACE_DEF *packet, char *dest);
static int CigiSwapPacketSymbolCtrl(const CIGI_SYMBOL_CONTROL *packet, char *dest);
static int CigiSwapPacketShortSymbolCtrl(const CIGI_SYMBOL_CONTROL_S *packet, char *dest);
static int CigiSwapPacketSymbolTextDef(const CIGI_SYMBOL_TEXT_DEF *packet, char *dest);
static int CigiSwapPacketSymbolCircleDef(const CIGI_SYMBOL_CIRCLE_DEF *packet, char *dest);
static int CigiSwapPacketSymbolPolygonDef(const CIGI_SYMBOL_POLYGON_DEF *packet, char *dest);
static int CigiSwapPacketSymbolTexturedCircleDef(const CIGI_SYMBOL_TEXTURED_CIRCLE_DEF *packet, char *dest);
static int CigiSwapPacketSymbolTexturedPolygonDef(const CIGI_SYMBOL_TEXTURED_POLYGON_DEF *packet, char *dest);
static int CigiSwapPacketSymbolClone(const CIGI_SYMBOL_CLONE *packet, char *dest);
static int CigiSwapPacketEntityCtrl(const CIGI_ENTITY_CONTROL *packet, char *dest);
static int CigiSwapPacketsof(const CIGI_START_OF_FRAME *packet, char *dest);
static int CigiSwapPacketHatHotResp(const CIGI_HAT_HOT_RESPONSE *packet, char *dest);
static int CigiSwapPacketHatHotExtResp(const CIGI_HAT_HOT_EXT_RESPONSE *packet, char *dest);
static int CigiSwapPacketLosResp(const CIGI_LOS_RESPONSE *packet, char *dest);
static int CigiSwapPacketLosExtResp(const CIGI_LOS_EXT_RESPONSE *packet, char *dest);
static int CigiSwapPacketSensorResp(const CIGI_SENSOR_RESPONSE *packet, char *dest);
static int CigiSwapPacketSensorExtResp(const CIGI_SENSOR_EXT_RESPONSE *packet, char *dest);
static int CigiSwapPacketPositionResp(const CIGI_POSITION_RESPONSE *packet, char *dest);
static int CigiSwapPacketWeatherResp(const CIGI_WEATHER_RESPONSE *packet, char *dest);
static int CigiSwapPacketAerosolResp(const CIGI_AEROSOL_RESPONSE *packet, char *dest);
static int CigiSwapPacketMaritimeCondResp(const CIGI_MARITIME_SURFACE_RESPONSE *packet, char *dest);
static int CigiSwapPacketTerrestrialCondResp(const CIGI_TERRESTRIAL_SURFACE_RESPONSE *packet, char *dest);
static int CigiSwapPacketCollSegmentNotification(const CIGI_COLL_SEGMENT_NOTIFICATION *packet, char *dest);
static int CigiSwapPacketCollVolumeNotification(const CIGI_COLL_VOLUME_NOTIFICATION *packet, char *dest);
static int CigiSwapPacketAnimationStopNotification(const CIGI_ANIMATION_STOP_NOTIFICATION *packet, char *dest);
static int CigiSwapPacketEventNotification(const CIGI_EVENT_NOTIFICATION *packet, char *dest);
static int CigiSwapPacketIGMessage(const CIGI_IG_MESSAGE *packet, char *dest);
static int CigiSwapPacketUserData(const CIGI_USER_DATA_PACKET *packet, char *dest);

/* -------- function definitions -------------------------------------------- */

int CigiGetMinorVersion(void)
{
    return _CigiMinorVersion;
}

void CigiSetMinorVersion(const int version)
{
    _CigiMinorVersion = version;
}

/* -----------------------------------------------------------------------------
 *  CigiSwap2()                                                               */
/** ----------------------------------------------------------------------------
 *  Performs byte swapping on 2-byte values.
 *
 *  @param dest  [out] The starting address of the copy destination.
 *  @param src   [in] The starting address of the copy source.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "API_Internals" */
void CigiSwap2(void *dest, const void *src)
{
    static unsigned short d, s;

    s = *((unsigned short *)src);

    d = s << 8;
    d |= s >> 8;

    *((unsigned short *)dest) = d;
}

/* -----------------------------------------------------------------------------
 *  CigiSwap4()                                                               */
/** ----------------------------------------------------------------------------
 *  Performs byte swapping on 4-byte values.
 *
 *  @param dest  [out] The starting address of the copy destination.
 *  @param src   [in] The starting address of the copy source.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "API_Internals" */
void CigiSwap4(void *dest, const void *src)
{
    static SWAP_UNION d, s;

    s.i = *((unsigned long *)src);

    d.b[0] = s.b[3];
    d.b[1] = s.b[2];
    d.b[2] = s.b[1];
    d.b[3] = s.b[0];

    *((unsigned long *)dest) = d.i;
}

/* -----------------------------------------------------------------------------
 *  CigiSwap8()                                                               */
/** ----------------------------------------------------------------------------
 *  Performs byte swapping on 8-byte values.
 *
 *  @param dest  [out] The starting address of the copy destination.
 *  @param src   [in] The starting address of the copy source.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "API_Internals" */
void CigiSwap8(void *dest, const void *src)
{
    static SWAP_UNION d1, s1;
    static SWAP_UNION d2, s2;

    /* Do each word separately.  Each pair of variables should be enclosed within
     * braces to minimize the number of variables declared as "register."
     */

    s1.i = *((unsigned long *)src);
    s2.i = *((unsigned long *)src + 1);

    d1.b[0] = s1.b[3];
    d1.b[1] = s1.b[2];
    d1.b[2] = s1.b[1];
    d1.b[3] = s1.b[0];

    d2.b[0] = s2.b[3];
    d2.b[1] = s2.b[2];
    d2.b[2] = s2.b[1];
    d2.b[3] = s2.b[0];

    *((unsigned long *)dest) = d2.i;
    *((unsigned long *)dest + 1) = d1.i;
}

/* -----------------------------------------------------------------------------
 *  CigiCompareMemoryMasked()                                                 */
/** ----------------------------------------------------------------------------
 *  Performs a word-wise (32 bit) comparison of two buffers using a mask.
 *
 *  @param ptr1    [in] The starting address of the first buffer.
 *  @param ptr2    [in] The starting address of the second buffer.
 *  @param mask    [in] The starting address of the mask.
 *  @param length  [in] The length of each buffer (and mask).
 *  @return        0 if all unmasked bits are identical; otherwise non-zero.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "API_Internals" */
static int CigiCompareMemoryMasked(const char *ptr1, const char *ptr2, const char *mask, const int length)
{
    int comp_len = length / sizeof(long);    /* We can assume packet is on a 32-bit word boundary */

    for (int i = 0; i < comp_len; i++) {
        if ((((long *)mask)[i] & ((long *)ptr1)[i]) != (((long *)mask)[i] & ((long *)ptr2)[i]))
            return 1;
    }

    return 0;
}

/* -----------------------------------------------------------------------------
 *  CigiSearchForPacket()                                                     */
/** ----------------------------------------------------------------------------
 *  Searches the outgoing buffer(s) for a packet whose unmasked bits match
 *  those of the given packet.
 *
 *  @param sessionid  [in] The ID of the session number.
 *  @param mask       [in] The address of the mask.
 *  @param packet     [in] The address of the packet for which to search.
 *  @param dest       [out] The address of the found packet, or NULL if a
 *                    match is not made.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "API_Internals" */
int CigiSearchForPacket(const int sessionid, const char *mask, const char *packet, char **dest)
{
    static int retOpcode = CIGI_SUCCESS;
    static int buffidx = 0;
    static int packetlength = 0;
    static unsigned char *ptr = NULL;
    static CIGI_SESSION *pcs = NULL;

    retOpcode = GetSessionPtr(sessionid, &pcs);

    if (retOpcode != CIGI_SUCCESS)
        return retOpcode;

    buffidx = pcs->StartBufferIdx;
    packetlength = ((CIGI_PACKET_HEADER *)packet)->packet_size;

    /* iterate through the buffers until we find a matching packet. */
    for (int i = 0; i < pcs->OverflowCount + 1; i++) {
        /* start AFTER the IG Control packet */
        ptr = pcs->BasePtrs[buffidx]; /* *** Ask Jim if OPCODE_NONE will ever be placed
                                        at the beginning of a buffer.
                                        + sizeof(CIGI_IG_CONTROL); */

        while ((ptr < pcs->BufferPtrs[buffidx]) &&
               (((CIGI_PACKET_HEADER *)ptr)->packet_id != CIGI_OPCODE_NONE)) {
            /* If we have a match, set the destination and return. */
            if ((*((long *)mask) & *((long *)packet)) == (*((long *)mask) & *((long *)ptr))) {  /* test first word first */
                if (CigiCompareMemoryMasked(packet, (char *)ptr, mask, packetlength) == 0) {
                    *dest = (char *)ptr;
                    return (CIGI_SUCCESS);
                }
            }

            ptr += ((CIGI_PACKET_HEADER *)ptr)->packet_size;
        }

        buffidx++;
        if (buffidx >= pcs->NumBuffers)
            buffidx = 0;
    }

    /* We have not yet added a matching packet, so set the destination
     * to NULL and return CIGI_SUCCESS.
     */
    *dest = NULL;

    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  PrepareSessionAndGetPtr()                                                 */
/** ----------------------------------------------------------------------------
 *  Given a session ID, retreives the session from the global session list and
 *  positions the session buffer pointer to the next buffer if there is not
 *  enough room available in the current one to add the next packet.
 *
 *  @param sessionid   [in] The ID for the session to retreive.
 *  @param packetsize  [in] The number of bytes needed to add the next packet.
 *  @param ppcs        [out] Address of a CIGI_SESSION pointer to assign the
 *                     session to.
 *  @return            Zero (0) if successful; otherwise, a negative non-zero
 *                     integer error code.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "API_Internals" */
int PrepareSessionAndGetPtr(const int sessionid, const int packetsize, CIGI_SESSION **ppcs)
{
    /* verify that the session pointer array has been created during
     * initialization and check for a valid session id
     */
    if (_CigiSessionPtrs == NULL)
        return CIGI_ERROR_UNEXPECTED_NULL;

    if (!CIGI_IS_VALID_SESSION(sessionid))
        return CIGI_ERROR_INVALID_SESSION;

    /* assign the session pointer to the outgoing parameter */
    *ppcs = _CigiSessionPtrs[sessionid];

    /* check to make sure that adding a packet will not cause the data
     * to overrun the buffer, and move to the next buffer if needed
     */

    /* if the packet will overrun the buffer... */
    if (((*ppcs)->BufferPtrs[(*ppcs)->BufferIdx] + packetsize)
        > (*ppcs)->ThresholdPtrs[(*ppcs)->BufferIdx]) {
        /* check to see if all of the overflow buffers are already
         * used, in which case there is no room to add the packet.
         *
         * this happens when incrementing the overflow count would
         * equal the number of buffers available.
         */
        if (((*ppcs)->OverflowCount + 1) >= (*ppcs)->NumBuffers)
            return CIGI_ERROR_BUFFER_OVERRUN;

        /* set the current buffer index to the next available buffer */

        if (++((*ppcs)->BufferIdx) >= (*ppcs)->NumBuffers)
            (*ppcs)->BufferIdx = 0;

        /* increment the overflow count session variable */
        ++((*ppcs)->OverflowCount);

        /* set the buffer position to the beginning after the IG
         * control packet (for host to IG mode) or the SOF packet
         * (for IG to host mode)
         */
        switch (_CigiMinorVersion) {
        case 0:
            (*ppcs)->BufferPtrs[(*ppcs)->BufferIdx] =
                (*ppcs)->BasePtrs[(*ppcs)->BufferIdx] +
                (((*ppcs)->SessionType == CIGI_HOST_SESSION) ?
                 sizeof(CIGI_IG_CONTROL) :
                 sizeof(CIGI_START_OF_FRAME)
                );
            break;

        }
    }
    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  GetSessionPtr()                                                           */
/** ----------------------------------------------------------------------------
 *  Given a session ID, retreives the session from the global session list.
 *
 *  @param sessionid   [in] The ID for the session to retreive.
 *  @param ppcs        [out] Address of a CIGI_SESSION pointer to assign the
 *                     session to.
 *  @return            Zero (0) if successful; otherwise, a negative non-zero
 *                     integer error code.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "API_Internals" */
static int GetSessionPtr(const int sessionid, CIGI_SESSION **ppcs)
{
    /* verify that the session pointer array has been created during
     * initialization and check for a valid session id
     */
    if (_CigiSessionPtrs == NULL)
        return CIGI_ERROR_UNEXPECTED_NULL;

    if (!CIGI_IS_VALID_SESSION(sessionid))
        return CIGI_ERROR_INVALID_SESSION;

    /* assign the session pointer to the outgoing parameter */
    *ppcs = _CigiSessionPtrs[sessionid];

    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  CigiInit()                                                                */
/** ----------------------------------------------------------------------------
 *  Initializes the CIGI API, seting the maximum number of sessions to be
 *  created and the minimum version of the CIGI that can be used. Internal
 *  memory is allocated to manage the sessions.
 *
 *  @param maxsessions   [in] The maximum number of sessions that will be
 *                       allowed.
 *  @param minver        [in] The minimum CIGI version that this device will
 *                       support.
 *  @return              Zero (0) if successful; otherwise, a negative non-zero
 *                       integer error code.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "Startup_Shutdown_Functions" */
int CigiInit(const int maxsessions, const int minver)
{
    _CigiMaxSessions = maxsessions;
    _CigiMinVersion = minver;

    /* check the max session parameter */
    if (_CigiMaxSessions > CIGI_MAX_SESSIONS)
        return CIGI_ERROR_MAX_SESSIONS;

    /* create the array to hold session pointers */
    _CigiSessionPtrs = (CIGI_SESSION **)calloc(_CigiMaxSessions, sizeof(CIGI_SESSION *));
    if (!_CigiSessionPtrs)
        return CIGI_ERROR_ALLOC_FAILED;

    /* initialize all callback function pointers to empy (NULL) */
    memset(_CigiCallbackProc, 0, CIGI_NUM_CALLBACKS * sizeof(CIGICBPROC));

    /* initialization was successful */
    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  CigiShutdown()                                                              */
/** ----------------------------------------------------------------------------
 *  Frees send and receive buffers and performs other cleanup.
 *
 *  @return Zero (0) if successful; otherwise, a negative non-zero integer.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "Startup_Shutdown_Functions" */
int CigiShutdown(void)
{
    /* make sure that the session pointer array has been allocated */
    if (_CigiSessionPtrs == NULL)
        return CIGI_ERROR_UNEXPECTED_NULL;

    /* delete all of the sessions */
    for (int i = 0; i < _CigiMaxSessions; ++i)
        CigiCloseSession(i);

    /* free the session array pointer and set it back to NULL */
    free(_CigiSessionPtrs);

    _CigiSessionPtrs = NULL;
    _CigiMaxSessions = 0;

    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  CigiCreateSession()                                                       */
/** ----------------------------------------------------------------------------
 *  Creates a new session, allocates the memory needed for the session, and
 *  initializes (internal) session variables.
 *
 *  @note Sessions are used for creating a distinct set of session variables
 *        from within the CIGI API, and are each identified by a unique ID.
 *
 *        Sessions allow more than one host or IG instance to run at once
 *        within the same process.
 *
 *        It is recommended that sessions are created during startup, because
 *        memory for the buffers and buffer positions are allocated.
 *
 *  @param type         [in] The type of session to create, set to
 *                      either CIGI_HOST_SESSION or CIGI_IG_SESSION.
 *  @param numbuffers   [in] The number of buffers to create for the
 *                      session.
 *  @param buffersize   [in] The size of each buffer for the session.
 *  @return             A session ID (positive, numeric) if
 *                      successful, -1 on failure.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "Startup_Shutdown_Functions" */
int CigiCreateSession(const int type, const int numbuffers, const int buffersize)
{
    static CIGI_SESSION *pSession = NULL;

    /* check to make sure the maximum session count is not exceeded */
    if (++_CigiSessionIdx >= _CigiMaxSessions)
        return CIGI_INVALID_SESSION;

    /* verify the requested buffer size */
    if (buffersize < CIGI_MAX_PACKET_SIZE)
        return CIGI_ERROR_BUFFER_TOO_SMALL;

    /* check the session type */
    if ((type != CIGI_HOST_SESSION) && (type != CIGI_IG_SESSION))
        return CIGI_ERROR_INVALID_SESSION_TYPE;

    /* create the session at the next index*/
    _CigiSessionPtrs[_CigiSessionIdx] = (CIGI_SESSION *)malloc(sizeof(CIGI_SESSION));
    if (! _CigiSessionPtrs[_CigiSessionIdx])
        return CIGI_INVALID_SESSION;   /* unable to allocate session */

    /* initialize the session variables */
    pSession = _CigiSessionPtrs[_CigiSessionIdx];

    pSession->SessionId = _CigiSessionIdx;
    pSession->SessionType = type;

    pSession->ReceivedBuffer = NULL;
    pSession->ReceivedBufferSize = 0;

    pSession->NumBuffers = numbuffers;
    pSession->BufferSize = buffersize;

    /* StartBufferIdx : will start off at 0 when incremented */
    pSession->StartBufferIdx = -1;
    pSession->BufferIdx = 0;
    pSession->OverflowCount = 0;  /* no overflow initially */

    pSession->ExplicitAddFirstPacket = 0;
    pSession->IncomingBufferUnpacked = 0;

    pSession->FrameCounter = 0;

    /* allocate memory for and initialize all the buffers at once */
    pSession->MemoryBlockPtr = (unsigned char *)calloc(numbuffers, buffersize);
    if (pSession->MemoryBlockPtr == NULL)
        return CIGI_ERROR_ALLOC_FAILED;

    /* set up pointers to the start, working base, and ends of each of
     * the outgoing buffers. set up the array to flag full buffers.
     */
    pSession->BufferPtrs = (unsigned char **)calloc(numbuffers, sizeof(CIGI_SESSION *));
    if (pSession->BufferPtrs == NULL)
        return CIGI_ERROR_ALLOC_FAILED;

    pSession->BasePtrs = (unsigned char **)calloc(numbuffers, sizeof(CIGI_SESSION *));
    if (pSession->BasePtrs == NULL)
        return CIGI_ERROR_ALLOC_FAILED;

    pSession->ThresholdPtrs = (unsigned char **)calloc(numbuffers, sizeof(CIGI_SESSION *));
    if (pSession->ThresholdPtrs == NULL)
        return CIGI_ERROR_ALLOC_FAILED;

    for (int i = 0; i < numbuffers; ++i) {
        /* pointers to the start of the buffers */
        pSession->BasePtrs[i] = (unsigned char *)(pSession->MemoryBlockPtr + (i * buffersize));

        /* pointers to the positions within each buffer, initially the same
         * as the base pointer plus the ig contorl packet
         */
        switch (_CigiMinorVersion) {
        case 0:
            pSession->BufferPtrs[i] = pSession->BasePtrs[i]
                                      + ((type == CIGI_HOST_SESSION) ? sizeof(CIGI_IG_CONTROL) : sizeof(CIGI_START_OF_FRAME));
            break;
        }

        /* set the buffer thresholds for the buffers to check for overflow */
        pSession->ThresholdPtrs[i] = (unsigned char *)(pSession->BufferPtrs[i] + buffersize - 1);
    }

    /* return the valid session id */
    return _CigiSessionIdx;
}

/* -----------------------------------------------------------------------------
 *  CigiCloseSession()                                                        */
/** ----------------------------------------------------------------------------
 *  Frees the memory and removes a session.
 *
 *  @param sessionid  [in] A valid session ID returned by CigiCreateSession().
 * -----------------------------------------------------------------------------
 */
/*:Associate with "Startup_Shutdown_Functions" */
void CigiCloseSession(const int sessionid)
{
    static CIGI_SESSION *pcs = NULL;

    /* make sure the session id parameter is valid */
    if (!CIGI_IS_VALID_SESSION(sessionid))
        return;

    pcs = _CigiSessionPtrs[sessionid];

    /* free allocated memory */
    if (pcs->MemoryBlockPtr)
        free(pcs->MemoryBlockPtr);

    if (pcs->BufferPtrs)
        free(pcs->BufferPtrs);

    if (pcs->BasePtrs)
        free(pcs->BasePtrs);

    if (pcs->ThresholdPtrs)
        free(pcs->ThresholdPtrs);

    /* set the session back to the empty state (NULL) */
    if (_CigiSessionPtrs[sessionid] != NULL)
        free(_CigiSessionPtrs[sessionid]);

    _CigiSessionPtrs[sessionid] = NULL;
}

/* -----------------------------------------------------------------------------
 *  CigiSetCallback()                                                         */
/** ----------------------------------------------------------------------------
 *  Sets the callback function to be called when the specified packet type is
 *  received.
 *
 *  @param opcode  [in] The packet ID corresponding to the type of packet to be
 *                 handled. The opcodes are defined in cigi_icd.h.
 *  @param proc    [in] A pointer to the callback function that will handle
 *                 packets containing the opcode.
 *  @return        Zero (0) if successful; otherwise, a negative non-zero
 *                 integer.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "Startup_Shutdown_Functions" */
int CigiSetCallback(const int opcode, const CIGICBPROC proc)
{
    /* check for a valid opcode ( index) */
    if ((opcode < 0) || (opcode >= CIGI_NUM_CALLBACKS))
        return CIGI_ERROR_INVALID_OPCODE;

    /* if a NULL function pointer is given, unsubscribe to the
     * callback function for that opcode, otherwise set the function
     * pointer for that opcode
     */
    _CigiCallbackProc[opcode] = (CIGICBPROC)proc;

    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  CigiSetDefaultCallback()
 * -------------------------------------------------------------------------- */
/** Sets or unsets the default callback function. If set, this function will
  * be called for each time a packet encountered which does not have a callback
  * function set.
  *
  * @param proc [in] A pointer to the callback function that will be used for
  *             the default, or a value of NULL to indicate that no default
  *             callback function should be used.
  */
/*:Associate with "Startup_Shutdown_Functions" */
void CigiSetDefaultCallback(const CIGICBPROC proc)
{
    /* set the pointer to the callback function to use as the default */
    _CigiCallbackProc[CIGI_DEFAULT_CALLBACK_INDEX] = (CIGICBPROC)proc;
}

/* -----------------------------------------------------------------------------
 *  CigiGetFirstPacket()                                                      */
/** ----------------------------------------------------------------------------
 *  Used to get the first packet in the incoming received buffer.
 *
 *  @note Used as the first call when iterating through an incoming message.
 *
 *  @param sessionid  [in] The ID of the session for which to add the packet,
 *                    as returned by the CigiCreateSession() method.
 *  @param packet     [in, out] Address of a pointer of the packet. This
 *                    parameter is also used as a placeholder for subsequent
 *                    calls to CigiGetNextPacket() and should not be modified
 *                    in any way.
 *  @return           The opcode of the packet, CIGI_OPCODE_NONE (0) if there
 *                    is not a valid packet, or a negative number error code
 *                    on failure.
 * -----------------------------------------------------------------------------
 */
int CigiGetFirstPacket(const int sessionid, unsigned char **packet)
{
    static CIGI_SESSION *pcs = NULL;
    static int swapbuffer = 0;

    /* check the session id */
    if (!CIGI_IS_VALID_SESSION(sessionid))
        return CIGI_ERROR_INVALID_SESSION;

    pcs = _CigiSessionPtrs[sessionid];

    /* set the packet to the start of the receive buffer */
#ifdef CIGI_FORCE_DWORD_ALIGN

    /* save the position of the packet for the next call */
    pcs->IncomingPacketPos = pcs->ReceivedBuffer;

    /* first check to make sure that the packet will fit in the incoming
     * temporary packet buffer
     */
    if (((CIGI_PACKET_HEADER *)pcs->ReceivedBuffer)->packet_size <= CIGI_MAX_PACKET_SIZE) {
        /* copy the packet to a temporary buffer that is known to be
         * properly aligned on a double word boundary, and set packet to
         * point to this address.
         */
        memcpy(pcs->IncomingPacketBuffer, pcs->IncomingPacketPos,
               CIGI_MAX_PACKET_SIZE);

        /* have packet to point to the temporary packet buffer */
        *packet = pcs->IncomingPacketBuffer;
    } else {
        /* have packet point to the position in the incoming message buffer */
        *packet = pcs->ReceivedBuffer;
    }

#else

    /* have packet point to the position in the incoming message buffer */
    *packet = pcs->ReceivedBuffer;

#endif

    swapbuffer = 0;

    // If the first packet is not an IG Control or Start of Frame packet,
    // return an error.  We should also check the "magic number" to
    // determine whether we need to byte-swap.
    if (((CIGI_PACKET_HEADER *)(*packet))->packet_id == CIGI_IG_CONTROL_OPCODE) {
        if (((CIGI_IG_CONTROL *)*packet)->packet_size & 0x00ff )
            swapbuffer = 1;
    } else if (((CIGI_PACKET_HEADER *)(*packet))->packet_id == CIGI_START_OF_FRAME_OPCODE) {
        if (((CIGI_START_OF_FRAME *)*packet)->packet_size == 0x00ff)
            swapbuffer = 1;
    } else {
        if (pcs->SessionType == CIGI_HOST_SESSION)
            return CIGI_ERROR_MISSING_IG_CONTROL_PACKET;
        else
            return CIGI_ERROR_MISSING_SOF_PACKET;
    }

    // Check the received CIGI Minor Version.
    if (((CIGI_START_OF_FRAME *)*packet)->packet_size == sizeof(CIGI_START_OF_FRAME)) {
        // This takes care of CIGI 4.0.
        _CigiMinorVersion = 0;
	}

    // Byte-swap if we need to AND if we haven't already.
    if (!pcs->IncomingBufferUnpacked) {
        if (swapbuffer) {
            int status = CigiSwapIncomingMsgBuffer((char *)pcs->ReceivedBuffer,
                                                   pcs->ReceivedBufferSize);
            if (status != CIGI_SUCCESS)
                return status;
        }

        /* set the flag to indicate that it has been unpacked */
        pcs->IncomingBufferUnpacked = 1;
    }

    /* return the opcode for the next packet */
    return ((CIGI_PACKET_HEADER *)*packet)->packet_id;
}

/* -----------------------------------------------------------------------------
 *  CigiGetNextPacket()                                                      */
/** ----------------------------------------------------------------------------
 *  Method used for iterating through the packets received in the incoming
 *  buffer.
 *
 *  @note The CigiGetFirstPacket() method should be called before calling
 *        CigiGetNextPacket(). It will be used to position the packet argument
 *        placeholder at the beginning of the message.
 *
 *  @param sessionid  [in] The ID of the session for which to add the packet,
 *                    as returned by the CigiCreateSession() method.
 *  @param packet     [in, out] Address of a pointer of the packet. This
 *                    parameter is also used as a placeholder for subsequent
 *                    calls to CigiGetNextPacket() and should not be modified
 *                    in any way.
 *  @return           The opcode of the packet, CIGI_OPCODE_NONE (0) if there
 *                    is not a valid packet, or a negative number error code
 *                    on failure.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "Incoming_Message_Functions" */
int CigiGetNextPacket(const int sessionid, unsigned char **packet)
{
    static CIGI_SESSION *pcs = NULL;

    /* check the session id */
    if (!CIGI_IS_VALID_SESSION(sessionid))
        return CIGI_ERROR_INVALID_SESSION;

    pcs = _CigiSessionPtrs[sessionid];

    /* check the first packet */
    if (*packet == NULL)
        return CIGI_OPCODE_NONE;

    /* get the next packet */
    /* increment the value of the packet to point to the next one */
    *packet += (((CIGI_PACKET_HEADER *)(*packet))->packet_size);

    /* check for the last message */
    if (*packet >= (pcs->ReceivedBuffer + pcs->ReceivedBufferSize)) {
        *packet = NULL;
        return CIGI_OPCODE_NONE;
    }

    /* return the opcode for the next packet */
    return ((CIGI_PACKET_HEADER *)(*packet))->packet_id;
}

/* -----------------------------------------------------------------------------
 *  CigiProcessIncomingMsgBuffer()                                            */
/** ----------------------------------------------------------------------------
 *  Parses the incoming message buffer, calling the appropriate
 *  application-defined callback for each CIGI packet.
 *
 *  @note This function should only be called when the callback method is used
 *        for handling incoming messages.
 *
 *  @param sessionid  [in] The ID of the session for which to add the packet,
 *                    as returned by the CigiCreateSession() method.
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 * -----------------------------------------------------------------------------
 */
int CigiProcessIncomingMsgBuffer(const int sessionid)
{
    /* create the variables as static so they are not created on the stack
     * each time
     */
    static int offset = 0;
    static CIGICBPROC pfn = NULL;
    static CIGI_PACKET_HEADER *packetHeader = NULL;

    static CIGI_SESSION *pcs = NULL;

    /* check the session id */
    if (!CIGI_IS_VALID_SESSION(sessionid))
        return CIGI_ERROR_INVALID_SESSION;

    pcs = _CigiSessionPtrs[sessionid];

    // Make sure the first packet is an IG Control or a Start of Frame.
    // If it is, then byte-swap if necessary.
    if (pcs->ReceivedBufferSize > 8) {  // minimum packet size
        // Since the Byte Swap parameters of the IG Control and Start of Frame
        // packets are in the same place within the packet, we can just
        // typecast to a CIGI_IG_CONTROL.
        if (((CIGI_IG_CONTROL *)(pcs->ReceivedBuffer))->packet_size & 0xff00 )
            CigiSwapIncomingMsgBuffer( ( char  * ) pcs->ReceivedBuffer, pcs->ReceivedBufferSize );
    }

    /* start at the beginning of the message buffer */
    offset = 0;

    /* loop through the entire message, one packet at a time */
    while (offset < pcs->ReceivedBufferSize) {
        /* get the packet header AND save the start address of the current
         * packet
         */
        packetHeader = (CIGI_PACKET_HEADER *)(pcs->ReceivedBuffer + offset);

        /* invoke the callback function, if one is assigned for this opcode */
// chas        if (packetHeader->packet_id > 0) { /* otherwise an invalid opcode */
            pfn = _CigiCallbackProc[(packetHeader->packet_id)];

            /* if no callback function is set for the packet type, use the callback
             * function pointer instead
             */
            if (pfn == NULL)
                pfn = _CigiCallbackProc[CIGI_DEFAULT_CALLBACK_INDEX];

            /* invoke the callback function, if set */
            if (pfn != NULL)
                pfn(sessionid, (void *)packetHeader);
// chas        }

        /* move to the next packet, but verify that the packet_size field is ok */
        if (packetHeader->packet_size < 1)
            return CIGI_ERROR_UNEXPECTED_NULL;

        offset += packetHeader->packet_size;
    }

    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  CigiSwapIncomingMsgBuffer()                                               */
/** ----------------------------------------------------------------------------
 *  Byte-swaps the incoming message buffer.
 *
 *  @param buffer     The starting address of the buffer.
 *  @param size       The total size of the buffer.
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "API_Internals" */
static int CigiSwapIncomingMsgBuffer(char *buffer, const int size)
{
    static int nOpcode = CIGI_OPCODE_NONE;
    static char *packet = NULL;
    static char *stop = NULL;

    packet = buffer;
    stop = buffer + size;

    while (packet < stop) {
        nOpcode = ((CIGI_PACKET_HEADER *)packet)->packet_id;

        switch (nOpcode) {
        case CIGI_IG_CONTROL_OPCODE:
            if (((CIGI_IG_CONTROL *)packet)->packet_size == sizeof(CIGI_IG_CONTROL)) {
                CigiSwapPacketIGCtrl((CIGI_IG_CONTROL *)packet, packet);
                packet += ((CIGI_IG_CONTROL *)packet)->packet_size;
            }
            break;

        case CIGI_ENTITY_POSITION_OPCODE:
            CigiSwapPacketEntityPosition((CIGI_ENTITY_POSITION *)packet, packet);
            packet += sizeof(CIGI_ENTITY_POSITION);
            break;

        case CIGI_ENTITY_CONTROL_OPCODE:
            CigiSwapPacketEntityCtrl((CIGI_ENTITY_CONTROL *)packet, packet);
            packet += sizeof(CIGI_ENTITY_CONTROL);
            break;

        case CIGI_ENTITY_POSITION_CC_OPCODE:
            CigiSwapPacketCCEntityPosition((CIGI_ENTITY_POSITION_CC *)packet, packet);
            packet += sizeof(CIGI_ENTITY_POSITION_CC);
            break;

        case CIGI_COMPONENT_CONTROL_OPCODE:
            CigiSwapPacketComponentCtrl((CIGI_COMPONENT_CONTROL *)packet, packet);
            packet += sizeof(CIGI_COMPONENT_CONTROL);
            break;

        case CIGI_COMPONENT_CONTROL_S_OPCODE:
            CigiSwapPacketShortComponentCtrl((CIGI_COMPONENT_CONTROL_S *)packet, packet);
            packet += sizeof(CIGI_COMPONENT_CONTROL_S);
            break;

        case CIGI_ARTPART_CONTROL_OPCODE:
            CigiSwapPacketArtPartCtrl((CIGI_ARTPART_CONTROL *)packet, packet);
            packet += sizeof(CIGI_ARTPART_CONTROL);
            break;

        case CIGI_ARTPART_CONTROL_S_OPCODE:
            CigiSwapPacketShortArtPartCtrl((CIGI_ARTPART_CONTROL_S *)packet, packet);
            packet += sizeof(CIGI_ARTPART_CONTROL_S);
            break;

        case CIGI_VELOCITY_CONTROL_OPCODE:
            CigiSwapPacketVelocityCtrl((CIGI_VELOCITY_CONTROL *)packet, packet);
            packet += sizeof(CIGI_VELOCITY_CONTROL);
            break;

        case CIGI_CELESTIAL_SPHERE_CONTROL_OPCODE:
            CigiSwapPacketCelestialSphereCtrl((CIGI_CELESTIAL_SPHERE_CONTROL *)packet, packet);
            packet += sizeof(CIGI_CELESTIAL_SPHERE_CONTROL);
            break;

        case CIGI_ATMOSPHERE_CONTROL_OPCODE:
            CigiSwapPacketAtmosphereCtrl((CIGI_ATMOSPHERE_CONTROL *)packet, packet);
            packet += sizeof(CIGI_ATMOSPHERE_CONTROL);
            break;

        case CIGI_ENV_REGION_CONTROL_OPCODE:
            CigiSwapPacketEnvRegionCtrl((CIGI_ENV_REGION_CONTROL *)packet, packet);
            packet += sizeof(CIGI_ENV_REGION_CONTROL);
            break;

        case CIGI_WEATHER_CONTROL_OPCODE:
            CigiSwapPacketWeatherCtrl((CIGI_WEATHER_CONTROL *)packet, packet);
            packet += sizeof(CIGI_WEATHER_CONTROL);
            break;

        case CIGI_MARITIME_CONDITIONS_CONTROL_OPCODE:
            CigiSwapPacketMaritimeCondCtrl((CIGI_MARITIME_CONDITIONS_CONTROL *)packet, packet);
            packet += sizeof(CIGI_MARITIME_CONDITIONS_CONTROL);
            break;

        case CIGI_WAVE_CONTROL_OPCODE:
            CigiSwapPacketWaveCtrl((CIGI_WAVE_CONTROL *)packet, packet);
            packet += sizeof(CIGI_WAVE_CONTROL);
            break;

        case CIGI_TERRESTRIAL_CONDITIONS_CONTROL_OPCODE:
            CigiSwapPacketTerrestrialCondCtrl((CIGI_TERRESTRIAL_CONDITIONS_CONTROL *)packet, packet);
            packet += sizeof(CIGI_TERRESTRIAL_CONDITIONS_CONTROL);
            break;

        case CIGI_VIEW_CONTROL_OPCODE:
            CigiSwapPacketViewCtrl((CIGI_VIEW_CONTROL *)packet, packet);
            packet += sizeof(CIGI_VIEW_CONTROL);
            break;

        case CIGI_SENSOR_CONTROL_OPCODE:
            CigiSwapPacketSensorCtrl((CIGI_SENSOR_CONTROL *)packet, packet);
            packet += sizeof(CIGI_SENSOR_CONTROL);
            break;

        case CIGI_MOTION_TRACKER_CONTROL_OPCODE:
            CigiSwapPacketMotionTrackerCtrl((CIGI_MOTION_TRACKER_CONTROL *)packet, packet);
            packet += sizeof(CIGI_MOTION_TRACKER_CONTROL);
            break;

        case CIGI_ERM_DEF_OPCODE:
            CigiSwapPacketErmDef((CIGI_ERM_DEF *)packet, packet);
            packet += sizeof(CIGI_ERM_DEF);
            break;

        case CIGI_ACCELERATION_CONTROL_DEF_OPCODE:
            CigiSwapPacketAccelerationDef((CIGI_ACCELERATION_CONTROL_DEF *)packet, packet);
            packet += sizeof(CIGI_ACCELERATION_CONTROL_DEF);
            break;

        case CIGI_VIEW_DEF_OPCODE:
            CigiSwapPacketViewDef((CIGI_VIEW_DEF *)packet, packet);
            packet += sizeof(CIGI_VIEW_DEF);
            break;

        case CIGI_COLL_SEGMENT_DEF_OPCODE:
            CigiSwapPacketCollSegmentDef((CIGI_COLL_SEGMENT_DEF *)packet, packet);
            packet += sizeof(CIGI_COLL_SEGMENT_DEF);
            break;

        case CIGI_COLL_VOLUME_DEF_OPCODE:
            CigiSwapPacketCollVolumeDef((CIGI_COLL_VOLUME_DEF *)packet, packet);
            packet += sizeof(CIGI_COLL_VOLUME_DEF);
            break;

        case CIGI_HAT_HOT_REQUEST_OPCODE:
            CigiSwapPacketHatHotReq((CIGI_HAT_HOT_REQUEST *)packet, packet);
            packet += sizeof(CIGI_HAT_HOT_REQUEST);
            break;

        case CIGI_LOS_SEGMENT_REQUEST_OPCODE:
            CigiSwapPacketLosSegmentReq((CIGI_LOS_SEGMENT_REQUEST *)packet, packet);
            packet += sizeof(CIGI_LOS_SEGMENT_REQUEST);
            break;

        case CIGI_LOS_VECTOR_REQUEST_OPCODE:
            CigiSwapPacketLosVectorReq((CIGI_LOS_VECTOR_REQUEST *)packet, packet);
            packet += sizeof(CIGI_LOS_VECTOR_REQUEST);
            break;

        case CIGI_POSITION_REQUEST_OPCODE:
            CigiSwapPacketPositionReq((CIGI_POSITION_REQUEST *)packet, packet);
            packet += sizeof(CIGI_POSITION_REQUEST);
            break;

        case CIGI_ENV_CONDITIONS_REQUEST_OPCODE:
            CigiSwapPacketEnvCondReq((CIGI_ENV_CONDITIONS_REQUEST *)packet, packet);
            packet += sizeof(CIGI_ENV_CONDITIONS_REQUEST);
            break;

        case CIGI_SYMBOL_SURFACE_DEF_OPCODE:
            CigiSwapPacketSymbolSurfaceDef((CIGI_SYMBOL_SURFACE_DEF *)packet, packet);
            packet += sizeof(CIGI_SYMBOL_SURFACE_DEF);
            break;

        case CIGI_SYMBOL_CONTROL_OPCODE:
            CigiSwapPacketSymbolCtrl((CIGI_SYMBOL_CONTROL *)packet, packet);
            packet += sizeof(CIGI_SYMBOL_CONTROL);
            break;

        case CIGI_SYMBOL_CONTROL_S_OPCODE:
            CigiSwapPacketShortSymbolCtrl((CIGI_SYMBOL_CONTROL_S *)packet, packet);
            packet += sizeof(CIGI_SYMBOL_CONTROL_S);
            break;

        case CIGI_SYMBOL_TEXT_DEF_OPCODE:
            CigiSwapPacketSymbolTextDef((CIGI_SYMBOL_TEXT_DEF *)packet, packet);
            packet += ((CIGI_SYMBOL_TEXT_DEF *)packet)->packet_size;
            break;

        case CIGI_SYMBOL_CIRCLE_DEF_OPCODE:
            CigiSwapPacketSymbolCircleDef((CIGI_SYMBOL_CIRCLE_DEF *)packet, packet);
            packet += ((CIGI_SYMBOL_CIRCLE_DEF *)packet)->packet_size;
            break;

        case CIGI_SYMBOL_POLYGON_DEF_OPCODE:
            CigiSwapPacketSymbolPolygonDef((CIGI_SYMBOL_POLYGON_DEF *)packet, packet);
            packet += ((CIGI_SYMBOL_POLYGON_DEF *)packet)->packet_size;
            break;

        case CIGI_SYMBOL_TEXTURED_CIRCLE_DEF_OPCODE:
            CigiSwapPacketSymbolTexturedCircleDef((CIGI_SYMBOL_TEXTURED_CIRCLE_DEF *)packet, packet);
            packet += ((CIGI_SYMBOL_TEXTURED_CIRCLE_DEF *)packet)->packet_size;
            break;

        case CIGI_SYMBOL_TEXTURED_POLYGON_DEF_OPCODE:
            CigiSwapPacketSymbolTexturedPolygonDef((CIGI_SYMBOL_TEXTURED_POLYGON_DEF *)packet, packet);
            packet += ((CIGI_SYMBOL_TEXTURED_POLYGON_DEF *)packet)->packet_size;
            break;

        case CIGI_SYMBOL_CLONE_OPCODE:
            CigiSwapPacketSymbolClone((CIGI_SYMBOL_CLONE *)packet, packet);
            packet += sizeof(CIGI_SYMBOL_CLONE);
            break;

        case CIGI_START_OF_FRAME_OPCODE:
            if (((CIGI_START_OF_FRAME *)packet)->packet_size == sizeof(CIGI_START_OF_FRAME)) {
                CigiSwapPacketsof((CIGI_START_OF_FRAME *)packet, packet);
                packet += ((CIGI_START_OF_FRAME *)packet)->packet_size;
            }
            break;

        case CIGI_HAT_HOT_RESPONSE_OPCODE:
            CigiSwapPacketHatHotResp((CIGI_HAT_HOT_RESPONSE *)packet, packet);
            packet += sizeof(CIGI_HAT_HOT_RESPONSE);
            break;

        case CIGI_HAT_HOT_EXT_RESPONSE_OPCODE:
            CigiSwapPacketHatHotExtResp((CIGI_HAT_HOT_EXT_RESPONSE *)packet, packet);
            packet += sizeof(CIGI_HAT_HOT_EXT_RESPONSE);
            break;

        case CIGI_LOS_RESPONSE_OPCODE:
            CigiSwapPacketLosResp((CIGI_LOS_RESPONSE *)packet, packet);
            packet += sizeof(CIGI_LOS_RESPONSE);
            break;

        case CIGI_LOS_EXT_RESPONSE_OPCODE:
            CigiSwapPacketLosExtResp((CIGI_LOS_EXT_RESPONSE *)packet, packet);
            packet += sizeof(CIGI_LOS_EXT_RESPONSE);
            break;

        case CIGI_SENSOR_RESPONSE_OPCODE:
            CigiSwapPacketSensorResp((CIGI_SENSOR_RESPONSE *)packet, packet);
            packet += sizeof(CIGI_SENSOR_RESPONSE);
            break;

        case CIGI_SENSOR_EXT_RESPONSE_OPCODE:
            CigiSwapPacketSensorExtResp((CIGI_SENSOR_EXT_RESPONSE *)packet, packet);
            packet += sizeof(CIGI_SENSOR_EXT_RESPONSE);
            break;

        case CIGI_POSITION_RESPONSE_OPCODE:
            CigiSwapPacketPositionResp((CIGI_POSITION_RESPONSE *)packet, packet);
            packet += sizeof(CIGI_POSITION_RESPONSE);
            break;

        case CIGI_WEATHER_RESPONSE_OPCODE:
            CigiSwapPacketWeatherResp((CIGI_WEATHER_RESPONSE *)packet, packet);
            packet += sizeof(CIGI_WEATHER_RESPONSE);
            break;

        case CIGI_AEROSOL_RESPONSE_OPCODE:
            CigiSwapPacketAerosolResp((CIGI_AEROSOL_RESPONSE *)packet, packet);
            packet += sizeof(CIGI_AEROSOL_RESPONSE);
            break;

        case CIGI_MARITIME_SURFACE_RESPONSE_OPCODE:
            CigiSwapPacketMaritimeCondResp((CIGI_MARITIME_SURFACE_RESPONSE *)packet, packet);
            packet += sizeof(CIGI_MARITIME_SURFACE_RESPONSE);
            break;

        case CIGI_TERRESTRIAL_SURFACE_RESPONSE_OPCODE:
            CigiSwapPacketTerrestrialCondResp((CIGI_TERRESTRIAL_SURFACE_RESPONSE *)packet, packet);
            packet += sizeof(CIGI_TERRESTRIAL_SURFACE_RESPONSE);
            break;

        case CIGI_COLL_SEGMENT_NOTIFICATION_OPCODE:
            CigiSwapPacketCollSegmentNotification((CIGI_COLL_SEGMENT_NOTIFICATION *)packet, packet);
            packet += sizeof(CIGI_COLL_SEGMENT_NOTIFICATION);
            break;

        case CIGI_COLL_VOLUME_NOTIFICATION_OPCODE:
            CigiSwapPacketCollVolumeNotification((CIGI_COLL_VOLUME_NOTIFICATION *)packet, packet);
            packet += sizeof(CIGI_COLL_VOLUME_NOTIFICATION);
            break;

        case CIGI_ANIMATION_STOP_NOTIFICATION_OPCODE:
            CigiSwapPacketAnimationStopNotification((CIGI_ANIMATION_STOP_NOTIFICATION *)packet, packet);
            packet += sizeof(CIGI_ANIMATION_STOP_NOTIFICATION);
            break;

        case CIGI_EVENT_NOTIFICATION_OPCODE:
            CigiSwapPacketEventNotification((CIGI_EVENT_NOTIFICATION *)packet, packet);
            packet += sizeof(CIGI_EVENT_NOTIFICATION);
            break;

        case CIGI_IG_MESSAGE_OPCODE:
            CigiSwapPacketIGMessage((CIGI_IG_MESSAGE *)packet, packet);
            packet += sizeof(CIGI_IG_MESSAGE);
            break;

        default:
            /* check to see if the packet is a user defined packet */
            if ((nOpcode >= CIGI_USER_DATA_PACKET_OPCODE_MIN)
                && (nOpcode <= CIGI_USER_DATA_PACKET_OPCODE_MAX)) {
                CigiSwapPacketUserData((CIGI_USER_DATA_PACKET *)packet, packet);
                packet += ((CIGI_PACKET_HEADER *)&packet)->packet_size;
                break;
            } else {
                /* otherwise assume that we have swapped all the packets */
                return CIGI_SUCCESS;
            }
        }
    }

    return CIGI_SUCCESS;
}

int CigiSwapOutgoingMsgBuffer(char *buffer, const int size)
{
    return CigiSwapIncomingMsgBuffer(buffer, size);
}

/* -----------------------------------------------------------------------------
 *  CigiStartMessage()                                                        */
/** ----------------------------------------------------------------------------
 *  Prepares the send buffer for building a new CIGI message.
 *
 *  @param sessionid  [in] The ID of the session for which to add the packet,
 *                    as returned by the CigiCreateSession() method.
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "Outgoing_Message_Functions" */
int CigiStartMessage(const int sessionid)
{
    static CIGI_SESSION *pcs = NULL;

    /* check the session id */
    if (!CIGI_IS_VALID_SESSION(sessionid))
        return CIGI_ERROR_INVALID_SESSION;

    pcs = _CigiSessionPtrs[sessionid];

    /* point to the next buffer, back to the first if we are at the last one */
    ++pcs->StartBufferIdx;
    if (pcs->StartBufferIdx >= pcs->NumBuffers)
        pcs->StartBufferIdx = 0;

    /* if overflow has occurred, the pointer will be set to the next buffer to
     * send, but new packets will continue to be added to the current buffer
     *
     * one of the overflow buffers becomes the current, so decrement the
     * overflow count
     */
    if (pcs->OverflowCount > 0)
        --pcs->OverflowCount;
    else {
        /* otherwise, if we haven't overflowed, start over again with the next
         * buffer and move the current buffer index also
         */
        pcs->BufferIdx = pcs->StartBufferIdx;

        /* set the position within the buffer to the beginning, after the
         * start of frame or ig control packet
         */
        switch (_CigiMinorVersion) {
        case 0:
        default:
            pcs->BufferPtrs[pcs->BufferIdx] = pcs->BasePtrs[pcs->BufferIdx] + ((pcs->SessionType == CIGI_HOST_SESSION) ? sizeof(CIGI_IG_CONTROL) : sizeof(CIGI_START_OF_FRAME));
            break;
        }
    }

    /* reset the flag(s) */
    pcs->ExplicitAddFirstPacket = 0;

    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  CigiEndMessage()                                                          */
/** ----------------------------------------------------------------------------
 *  Prepares the send buffer for building a new CIGI message. Fails if the
 *  first packet has not been explicitly added (the IG control packet for
 *  host to IG, and the SOF packet for IG to host).
 *
 *  @note For IG to host sessions, the frame counter is automatically
 *        incremented. For host to IG sessions, the frame counter of the IG
 *        control packet is synchronized to match the last SOF frame counter.
 *
 *  @param sessionid  [in] The ID of the session for which to add the packet,
 *                    as returned by the CigiCreateSession() method.
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "Outgoing_Message_Functions" */
int CigiEndMessage(const int sessionid)
{
    static CIGI_SESSION *pcs = NULL;

    /* check the session id */
    if (!CIGI_IS_VALID_SESSION(sessionid))
        return CIGI_ERROR_INVALID_SESSION;

    pcs = _CigiSessionPtrs[sessionid];

    /* check to see if the first packet (SOF or IG control) has been
     * added, and if not return an error.
     */
    if (!pcs->ExplicitAddFirstPacket) {
        if (pcs->SessionType == CIGI_IG_SESSION)
            return CIGI_ERROR_MISSING_SOF_PACKET;

        else if (pcs->SessionType == CIGI_HOST_SESSION)
            return CIGI_ERROR_MISSING_IG_CONTROL_PACKET;

        else
            return CIGI_ERROR_INVALID_SESSION_TYPE;
    }

    /* finally, add a CIGI_OPCODE_NONE value at the end to terminate the
     * message.
     */
    *(pcs->BufferPtrs[pcs->BufferIdx]) = (unsigned char)CIGI_OPCODE_NONE;

    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  CigiSetIncomingMsgBuffer()                                                */
/** ----------------------------------------------------------------------------
 *  Specifies the location and length of the last CIGI message received from
 *  the IG. Copies the frame counter from the Start Of Frame to the outgoing
 *  message's IG Control packet.
 *
 *  @note The function should be called before CigiGetOutgoingMemoryBlockPtr(),
 *        or the application must be responsible for setting the frame counter
 *        in the outgoing IG Control packet.
 *
 *        The function must be called before CigiProcessIncomingMemoryBlockPtr()
 *        or any of the CigiGetPacket functions.
 *
 *  @param sessionid  [in] The ID of the session for which to add the packet,
 *                    as returned by the CigiCreateSession() method.
 *  @param buffer     [out] Address of the buffer pointer.
 *  @param size       [out] Address of an int that will receive the buffer size.
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "Incoming_Message_Functions" */
int CigiSetIncomingMsgBuffer(const int sessionid, const unsigned char *buffer, const int size)
{
    static CIGI_SESSION *pcs = NULL;

    /* check the session id */
    if (!CIGI_IS_VALID_SESSION(sessionid))
        return CIGI_ERROR_INVALID_SESSION;

    pcs = _CigiSessionPtrs[sessionid];

    /* set the the received buffer and size variables to the argument
     * values provided
     */
    pcs->ReceivedBuffer = (unsigned char *)buffer;
    pcs->ReceivedBufferSize = size;

    /* set the flag to indicate that the buffer needs to be unpacked */
    pcs->IncomingBufferUnpacked = 0;

    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  CigiGetIncomingMsgBuffer()                                                */
/** ----------------------------------------------------------------------------
 *  Returns the address of the inocming message buffer, containing the most
 *  recent (unparsed) CIGI message sent from the IG.
 *
 *  @note The incoming message buffer address is the same as the the one set
 *        in a previous call to CigiSetIncomingMemoryBlockPtr(). If
 *        CigiSetIncomingMemoryBlockPtr() has not been called, the value is
 *        NULL.
 *
 *  @param sessionid  [in] The ID of the session for which to add the packet,
 *                    as returned by the CigiCreateSession() method.
 *  @param buffer     [out] Varable to place the address of the message buffer.
 *  @param size       [out] Length of the buffer in bytes.
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "Incoming_Message_Functions" */
int CigiGetIncomingMsgBuffer(const int sessionid, unsigned char **buffer, int *size)
{
    static CIGI_SESSION *pcs = NULL;

    /* check the session id */
    if (!CIGI_IS_VALID_SESSION(sessionid))
        return CIGI_ERROR_INVALID_SESSION;

    pcs = _CigiSessionPtrs[sessionid];

    /* set the outgoing parameter values */
    *buffer = pcs->ReceivedBuffer;
    *size = pcs->ReceivedBufferSize;

    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  CigiSyncFrameCounter()                                                    */
/** ----------------------------------------------------------------------------
 *  Synchronizes the frame counter for the outgoing message. This can be called
 *  after receiving the outgoing message buffer pointer, but should be called
 *  after setting the incoming message buffer through CigiSetIncomingMsgBuffer()
 *  and before the message is actually sent.
 *
 *  @note For CIGI_IG_SESSION sessions, the frame counter in the start of frame
 *        message is incremented. For CIGI_HOST_SESSION sessions, the frame
 *        frame counter is set to match the frame counter received in start of
 *        frame pacekt of the last message received.
 *
 *  @param sessionid  [in] The ID of the session for which to add the packet,
 *                    as returned by the CigiCreateSession() method.
 *
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 * -----------------------------------------------------------------------------
 */
int CigiSyncFrameCounter(const int sessionid)
{
    static CIGI_SESSION *pcs = NULL;
    static CIGI_IG_CONTROL *igControl = NULL;
    static CIGI_START_OF_FRAME *startOfFrame = NULL;

    /* check the session id and retreive the session */
    if (!CIGI_IS_VALID_SESSION(sessionid))
        return CIGI_ERROR_INVALID_SESSION;

    pcs = _CigiSessionPtrs[sessionid];

    /* modify the first packet depending on the session type,
     * to adjust the frame counter field
     */
    if (pcs->SessionType == CIGI_HOST_SESSION) {
        /* check to make sure that there is an incoming message buffer to
         * set the frame counter from
         */
        if (pcs->ReceivedBuffer == NULL)
            return CIGI_ERROR_UNEXPECTED_NULL;

        /* copy the current frame counter from received buffer */
        igControl = (CIGI_IG_CONTROL *)pcs->BasePtrs[ pcs->StartBufferIdx];


        /* For CIGI 3.0/3.1 the Host just copies the frame counter received
         * from the IG into frame_ctr.  For CIGI 3.2 and higher, the value is
         * copied into last_frame_ctr.
         */
 //chas //if (_CigiMinorVersion < 2) {
        //    /* if the buffer has already been unpacked (and swapped), make sure to
        //     * swap the values when setting the frame counter...
        //     */
        //    if (pcs->IncomingBufferUnpacked) {
        //        CigiSwap4(
        //            &igControl->host_frame_num,
        //            &((CIGI_START_OF_FRAME *)pcs->ReceivedBuffer)->frame_ctr
        //        );
        //    } else { /* otherwise just copy the value */
        //        igControl->host_frame_num = ((CIGI_START_OF_FRAME *)
        //                                     pcs->ReceivedBuffer)->frame_ctr;
        //    }
        //} else {
            ++pcs->FrameCounter;

            /* if the buffer has already been unpacked (and swapped), make sure to
             * swap the values when setting the frame counter...
             */
            if (pcs->IncomingBufferUnpacked) {
                CigiSwap4(
                    &igControl->last_ig_frame_num,
                    &((CIGI_START_OF_FRAME *)pcs->ReceivedBuffer)->frame_ctr
                );

                /* ...and send the Host's own frame counter. */
                CigiSwap4(&igControl->host_frame_num, &pcs->FrameCounter);
            } else { /* otherwise just copy the value */
                igControl->last_ig_frame_num = ((CIGI_START_OF_FRAME *)
                                                pcs->ReceivedBuffer)->frame_ctr;

                /* Send the Host's own frame counter. */
                igControl->host_frame_num = pcs->FrameCounter;
            }
// chas //}
    } else if (pcs->SessionType == CIGI_IG_SESSION) {
        startOfFrame = ((CIGI_START_OF_FRAME *)(pcs->BasePtrs[ pcs->StartBufferIdx]));

        ++pcs->FrameCounter;

        /* increment the session frame counter and update the SOF packet
         * frame counter for the next outgoing message
         */
// chas //if (_CigiMinorVersion < 2) {
        //    /* if the buffer has already been unpacked (and swapped), make sure to
        //     * swap the values when setting the frame counter...
        //     */
        //    if (pcs->IncomingBufferUnpacked) {
        //        CigiSwap4(&startOfFrame->frame_ctr, &pcs->FrameCounter);
        //    } else { /* otherwise just copy the values */
        //        startOfFrame->frame_ctr = pcs->FrameCounter;
        //    }
        //} else {
            /* if the buffer has already been unpacked (and swapped), make sure to
             * swap the values when setting the frame counter...
             */
            if (pcs->IncomingBufferUnpacked) {
                CigiSwap4(
                    &startOfFrame->last_frame_ctr,
                    &((CIGI_START_OF_FRAME *)pcs->ReceivedBuffer)->frame_ctr
                );

                /* ...and send the IG's own frame counter. */
                CigiSwap4(&igControl->host_frame_num, &pcs->FrameCounter);
            } else { /* otherwise just copy the values */
                igControl->last_ig_frame_num = ((CIGI_START_OF_FRAME *)
                                                pcs->ReceivedBuffer)->frame_ctr;

                /* Send the IG's own frame counter. */
                igControl->host_frame_num = pcs->FrameCounter;
            }
// chas //}
    } else
        return CIGI_ERROR_INVALID_SESSION_TYPE;

    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  CigiGetOutgoingMsgBuffer()                                                */
/** ----------------------------------------------------------------------------
 *  Retreives a pointer to the send buffer and the size of the buffer (in
 *  bytes).
 *
 *  @param sessionid  [in] The ID of the session for which to add the packet,
 *                    as returned by the CigiCreateSession() method.
 *  @param buffer     [out] Address of the buffer pointer.
 *  @param size       [out] Address of an int that will receive the buffer size.
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 * -----------------------------------------------------------------------------
 */
/*:Associate with "Outgoing_Message_Functions" */
int CigiGetOutgoingMsgBuffer(const int sessionid, unsigned char **buffer, int *size)
{
    static CIGI_SESSION *pcs = NULL;
    int idx = -1;

    /* check the session id */
    if (!CIGI_IS_VALID_SESSION(sessionid))
        return CIGI_ERROR_INVALID_SESSION;

    pcs = _CigiSessionPtrs[sessionid];

    idx = pcs->StartBufferIdx;

    /* set the outgoing buffer and size parameters */
    *buffer = pcs->BasePtrs[idx];
    *size = (int)(pcs->BufferPtrs[idx] - pcs->BasePtrs[idx]);

    return CIGI_SUCCESS;
}

/**
 *  Adds a fixed-size CIGI packet to the current CIGI message. If the
 *  packet will not fit into the current message buffer, it will be placed in
 *  another buffer to be sent at a later time. Packets of variable size will
 *  need their own specialized function.
 *
 *  IG Control and Start of Frame packets should be added with the
 *  explicitAddFirstPacket parameter set to true, so that they will be placed
 *  as the first packet in the outgoing message.
 *
 *  If the attemptReplace parameter is true, the function searches the message
 *  buffers for a packet that matches the given replaceMask and replaces that
 *  packet instead of adding a new one. If a matching packet cannot be found,
 *  the packet is added to the outgoing message buffer.
 *
 *  @note Packets are buffered in the order in which they are added. The size
 *        of the buffer and the number of back buffers are set in the
 *        CigiCreateSession() method.
 *
 *        The session used must be of type CIGI_HOST_SESSION.
 *
 *  @param sessionid  [in] The ID of the session for which to add the packet,
 *                    as returned by the CigiCreateSession() method.
 *
 *  @param packet     [in] Address of the packet to be added.
 *
 *  @param packetSize [in] The size of the packet.
 *
 *  @param explicitAddFirstPacket [in] Whether or not the packet should be
 *                                placed first in the outgoing queue. (For
 *                                IG Control and Start of Frame packets.)
 *
 *  @param attemptReplace [in] Whether or not to first attempt at replacing an
 *                             already existing packet matching a given bit mask
 *                             before adding a new one.
 *
 *  @param replaceMask [in] The bit mask to check against when attempting to
 *                          replace a packet.
 *
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 */
int CigiAddFixedPacket(int sessionid,
                       const char *packet,
                       uint16_t packetSize,
                       bool explicitAddFirstPacket,
                       bool attemptReplace,
                       const char *replaceMask)
{
    int ret = CIGI_SUCCESS;
    CIGI_SESSION *pcs = NULL;

    // Make sure a session pointer can be retreived and that the session can
    // handle the new packet.
    ret = PrepareSessionAndGetPtr(sessionid, packetSize, &pcs);

    if (ret == CIGI_SUCCESS) {
        // Check for the correct session type to add the packet.
        if (pcs->SessionType == CIGI_HOST_SESSION) {
            unsigned char *packetBuffer = NULL;

            if (attemptReplace && !explicitAddFirstPacket)
                CigiSearchForPacket(sessionid, replaceMask, (char *)packet, (char **)&packetBuffer);

            if (packetBuffer == NULL) {
                unsigned char **buffers = explicitAddFirstPacket ? pcs->BasePtrs : pcs->BufferPtrs;
                const int       index   = explicitAddFirstPacket ? pcs->StartBufferIdx : pcs->BufferIdx;

                packetBuffer = buffers[index];
            }

            // No need to perform byte-swapping, just do a byte-wise copy.
            memcpy(packetBuffer, packet, packetSize);

            // Set flag to true if this is an IG Control or Start of Frame.
            pcs->ExplicitAddFirstPacket = explicitAddFirstPacket;

            // Increment the current buffer pointer if this NOT an IG Control
            // or Start of Frame.
            if (!explicitAddFirstPacket)
                pcs->BufferPtrs[pcs->BufferIdx] += packetSize;
        } else {
            ret = CIGI_ERROR_INVALID_SESSION_TYPE;
        }
    }

    return ret;
}

#if 0
/**
 *  Adds a variable-size CIGI packet to the current CIGI message. If the
 *  packet will not fit into the current message buffer, it will be placed in
 *  another buffer to be sent at a later time. Packets of variable size will
 *  need their own specialized function.
 *
 *  @note Packets are buffered in the order in which they are added. The size
 *        of the buffer and the number of back buffers are set in the
 *        CigiCreateSession() method.
 *
 *        The session used must be of type CIGI_HOST_SESSION.
 *
 *  @param sessionid  [in] The ID of the session for which to add the packet,
 *                    as returned by the CigiCreateSession() method.
 *
 *  @param packet     [in] Address of the packet to be added.
 *
 *  @param packetSize [in] The size of the packet.
 *
 *
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 */
int CigiAddVariablePacket(int sessionid,
                       const char *packet,
                       uint16_t packetSize)
{
    int ret = CIGI_SUCCESS;
    CIGI_SESSION *pcs = NULL;

    // Make sure a session pointer can be retreived and that the session can
    // handle the new packet.
    ret = PrepareSessionAndGetPtr(sessionid, packetSize, &pcs);

    if (ret == CIGI_SUCCESS) {
        // Check for the correct session type to add the packet.
        if (pcs->SessionType == CIGI_HOST_SESSION) {
            unsigned char *packetBuffer = NULL;
            unsigned char **buffers = pcs->BufferPtrs;
            const int       index   = pcs->BufferIdx;
                packetBuffer = buffers[index];

            // No need to perform byte-swapping, just do a byte-wise copy.
            memcpy(packetBuffer, packet, packetSize);

            // Set flag to true if this is an IG Control or Start of Frame.
            pcs->ExplicitAddFirstPacket = false;

            // Increment the current buffer pointer if this NOT an IG Control
            // or Start of Frame.
            pcs->BufferPtrs[pcs->BufferIdx] += packetSize;
        } else {
            ret = CIGI_ERROR_INVALID_SESSION_TYPE;
        }
    }

    return ret;
}
#endif
/* -----------------------------------------------------------------------------
 *  CigiAddPacketIGCtrl()                                                     */
/** ----------------------------------------------------------------------------
 *  Adds a CIGI_IG_CONTROL_30 or CIGI_IG_CONTROL_32 packet to the current CIGI
 *  message. If the packet
 *  will not fit into the current message buffer, it will be placed in another
 *  buffer to be sent at a later time.
 *
 *  @note Packets are buffered in the order in which they are added. The size
 *        of the buffer and the number of back buffers are set in the
 *        CigiCreateSession() method.
 *
 *        The packet added by the CigiAddPacketIGCtrl() method is always
 *        placed as the first packet in the outgoing message. If the
 *        CigiAddPacketIGCtrl() method is called multiple times, the most
 *        recently added packet will be
 *        placed in the outgoing messgae.
 *
 *        If CigiAddPacketIGCtrl() is not called explicitly, the frame
 *        counter value is copied from the Start of Frame packet in the
 *        incoming message buffer if such a packet exists. This is done during
 *        the CigiEndMessage() method.
 *
 *        The session used must be of type CIGI_HOST_SESSION.
 *
 *  @param sessionid  [in] The ID of the session for which to add the packet,
 *                    as returned by the CigiCreateSession() method.
 *
 *  @param packet     [in] Address of the packet to be added.
 *
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 *
 *  @see              CIGI_IG_CONTROL
 * -----------------------------------------------------------------------------
 */
int CigiAddPacketIGCtrl(const int sessionid, const CIGI_IG_CONTROL *packet)
{
    int ret = CIGI_SUCCESS;

    switch (_CigiMinorVersion) {
    case 0:
    default:
        ret = CigiAddFixedPacket<CIGI_IG_CONTROL>(sessionid, packet, true);
        break;
    }

    return ret;
}

#if 0
/* -----------------------------------------------------------------------------
 *  CigiSwapPacketIGCtrl30()                                                    */
/** ----------------------------------------------------------------------------
 *  Byte-swaps the fields in a CIGI_IG_CONTROL_30 packet.
 *
 *  @param packet     [in] Address of the packet to be swapped.
 *
 *  @param dest       [in] Starting address of where the result is written to.
 *                    The value of this parameter may be the same as the packet
 *                    being swapped.
 *
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 *
 *  @see              CIGI_IG_CONTROL_30
 * -----------------------------------------------------------------------------
 */
/*:Associate with "API_Internals" */
static int CigiSwapPacketIGCtrl30(const CIGI_IG_CONTROL_30 *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet ID : 1 byte */
    /* packet size : 1 byte */
    /* CIGI version : 1 byte */
    /* database number : 1 byte */
    CigiCopy4(dest, src);

    /* IG mode change request : 2 bits */
    /* timestamp valid        : 1 bit */
    /* spare (not swapped)    : 13 bits */
    /* byteswap               : 16 bits */
    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    /* host to IG frame counter : 4 bytes */
    CigiSwap4(dest + 8, src + 8);

    /* timestamp : 4 bytes */
    CigiSwap4(dest + 12, src + 12);

    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  CigiSwapPacketIGCtrl32()                                                    */
/** ----------------------------------------------------------------------------
 *  Byte-swaps the fields in a CIGI_IG_CONTROL_32 packet.
 *
 *  @param packet     [in] Address of the packet to be swapped.
 *
 *  @param dest       [in] Starting address of where the result is written to.
 *                    The value of this parameter may be the same as the packet
 *                    being swapped.
 *
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 *
 *  @see              CIGI_IG_CONTROL_32
 * -----------------------------------------------------------------------------
 */
/*:Associate with "API_Internals" */
static int CigiSwapPacketIGCtrl32(const CIGI_IG_CONTROL_32 *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet ID : 1 byte */
    /* packet size : 1 byte */
    /* CIGI version : 1 byte */
    /* database number : 1 byte */
    CigiCopy4(dest, src);

    /* IG mode change request : 2 bits */
    /* timestamp valid        : 1 bit */
    /* spare (not swapped)    : 13 bits */
    /* byteswap               : 16 bits */
    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    /* host to IG frame counter : 4 bytes */
    CigiSwap4(dest + 8, src + 8);

    /* timestamp : 4 bytes */
    CigiSwap4(dest + 12, src + 12);

    /* Last IG Frame Counter : 4 bytes */
    CigiSwap4(dest + 16, src + 16);

    /* Minor version : 1 byte */
    /* Reserved1 : 1 byte */
    /* Reserved2 : 2 bytes */
    CigiCopy4(dest + 20, src + 20);

    return CIGI_SUCCESS;
}
#endif

static int CigiSwapPacketIGCtrl(const CIGI_IG_CONTROL *packet, char *dest)
{
    return CIGI_SUCCESS;
}

/********************************************************************************************/

int CigiAddPacketReplaceEntityPosition(const int sessionid, const CIGI_ENTITY_POSITION *packet)
{
    static const CIGI_ENTITY_POSITION mask = { sizeof(CIGI_ENTITY_POSITION), CIGI_ENTITY_POSITION_OPCODE,
                                              (uint8_t) - 1,
                                              0, 0, 0, 0, 0, 0, 0, 0, 0
                                            };

    return CigiAddReplaceFixedPacket<CIGI_ENTITY_POSITION>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceCCEntityPosition(const int sessionid, const CIGI_ENTITY_POSITION_CC *packet)
{
    static const CIGI_ENTITY_POSITION_CC mask = {   sizeof(CIGI_ENTITY_POSITION_CC), CIGI_ENTITY_POSITION_CC_OPCODE,
                                                    (uint8_t) - 1,
                                                    0, 0.0, 0
                                               };

    return CigiAddReplaceFixedPacket<CIGI_ENTITY_POSITION_CC>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceComponentCtrl(const int sessionid, const CIGI_COMPONENT_CONTROL *packet)
{
    static const CIGI_COMPONENT_CONTROL mask = {  sizeof(CIGI_COMPONENT_CONTROL), CIGI_COMPONENT_CONTROL_OPCODE,
                                                       
                                                       -1, -1, -1,
                                                       0, 0, 0, 0, 0, 0, 0
                                                  };

    return CigiAddReplaceFixedPacket<CIGI_COMPONENT_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceShortComponentCtrl(const int sessionid, const CIGI_COMPONENT_CONTROL_S *packet)
{
    static const CIGI_COMPONENT_CONTROL_S mask = { sizeof(CIGI_COMPONENT_CONTROL_S), CIGI_COMPONENT_CONTROL_S_OPCODE,
                                                       
                                                       -1, -1, -1,
                                                       0, 0, 0,
                                                    };

    return CigiAddReplaceFixedPacket<CIGI_COMPONENT_CONTROL_S>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceArtPartCtrl(const int sessionid, const CIGI_ARTPART_CONTROL *packet)
{
    static const CIGI_ARTPART_CONTROL mask = { sizeof(CIGI_ARTPART_CONTROL), CIGI_ARTPART_CONTROL_OPCODE,
                                                
                                                -1, -1,
                                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                                             };

    return CigiAddReplaceFixedPacket<CIGI_ARTPART_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceShortArtPartCtrl(const int sessionid, const CIGI_ARTPART_CONTROL_S *packet)
{
    static const CIGI_ARTPART_CONTROL_S mask = {  sizeof(CIGI_ARTPART_CONTROL_S),  CIGI_ARTPART_CONTROL_S_OPCODE,
                                                    
                                                    -1, -1, -1,
                                                    0, 0, 0, 0, 0.0, 0.0
                                               };

    return CigiAddReplaceFixedPacket<CIGI_ARTPART_CONTROL_S>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceVelocityCtrl(const int sessionid, const CIGI_VELOCITY_CONTROL *packet)
{
    static const CIGI_VELOCITY_CONTROL mask = { sizeof(CIGI_VELOCITY_CONTROL), CIGI_VELOCITY_CONTROL_OPCODE,
                                            
                                            -1, -1,
                                            0, 0, 0.0, 0.0, 0.0, 0.0, 0.0
                                          };

    return CigiAddReplaceFixedPacket<CIGI_VELOCITY_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceCelestialSphereCtrl(const int sessionid, const CIGI_CELESTIAL_SPHERE_CONTROL *packet)
{
    static const CIGI_CELESTIAL_SPHERE_CONTROL mask = { sizeof(CIGI_CELESTIAL_SPHERE_CONTROL), CIGI_CELESTIAL_SPHERE_CONTROL_OPCODE,
                                                        
//                                                        0, 0, 0, 0, 0, 0, 0, 0, 0.0
                                                      };

    return CigiAddReplaceFixedPacket<CIGI_CELESTIAL_SPHERE_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceAtmosphereCtrl(const int sessionid, const CIGI_ATMOSPHERE_CONTROL *packet)
{
    static const CIGI_ATMOSPHERE_CONTROL mask = {  sizeof(CIGI_ATMOSPHERE_CONTROL), CIGI_ATMOSPHERE_CONTROL_OPCODE,
                                                    
                                                    0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
                                                };

    return CigiAddReplaceFixedPacket<CIGI_ATMOSPHERE_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceEnvRegionCtrl(const int sessionid, const CIGI_ENV_REGION_CONTROL *packet)
{
    static const CIGI_ENV_REGION_CONTROL mask = {  sizeof(CIGI_ENV_REGION_CONTROL), CIGI_ENV_REGION_CONTROL_OPCODE,
                                                    0, 0, 0, 0, 0, 0,
                                                    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
                                                };

    return CigiAddReplaceFixedPacket<CIGI_ENV_REGION_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceWeatherCtrl(const int sessionid, const CIGI_WEATHER_CONTROL *packet)
{
    static const CIGI_WEATHER_CONTROL mask = { sizeof(CIGI_WEATHER_CONTROL), CIGI_WEATHER_CONTROL_OPCODE,
                                                
                                                (uint8_t) - 1,
                                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                                             };

    return CigiAddReplaceFixedPacket<CIGI_WEATHER_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceMaritimeCondCtrl(const int sessionid, const CIGI_MARITIME_CONDITIONS_CONTROL *packet)
{
    static const CIGI_MARITIME_CONDITIONS_CONTROL mask = { sizeof(CIGI_MARITIME_CONDITIONS_CONTROL), CIGI_MARITIME_CONDITIONS_CONTROL_OPCODE,
                                                            
                                                            (uint8_t) - 1, 0, 0, 0, 0.0, 0.0, 0.0
                                                         };

    return CigiAddReplaceFixedPacket<CIGI_MARITIME_CONDITIONS_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceWaveCtrl(const int sessionid, const CIGI_WAVE_CONTROL *packet)
{
    static const CIGI_WAVE_CONTROL mask = { sizeof(CIGI_WAVE_CONTROL), CIGI_WAVE_CONTROL_OPCODE,
                                            
                                            -1, -1,
                                            0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
                                          };

    return CigiAddReplaceFixedPacket<CIGI_WAVE_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceTerrestrialCondCtrl(const int sessionid, const CIGI_TERRESTRIAL_CONDITIONS_CONTROL *packet)
{
    static const CIGI_TERRESTRIAL_CONDITIONS_CONTROL mask = { sizeof(CIGI_TERRESTRIAL_CONDITIONS_CONTROL), CIGI_TERRESTRIAL_CONDITIONS_CONTROL_OPCODE,
                                                                
                                                                -1, -1,
                                                                0, 0, 0
                                                            };

    return CigiAddReplaceFixedPacket<CIGI_TERRESTRIAL_CONDITIONS_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceViewCtrl(const int sessionid, const CIGI_VIEW_CONTROL *packet)
{
    static const CIGI_VIEW_CONTROL mask = { sizeof(CIGI_VIEW_CONTROL), CIGI_VIEW_CONTROL_OPCODE,
                                            
                                            -1, -1,
                                            0, 0, 0, 0, 0, 0,
                                            -1,
                                            0.0, 0.0, 0.0, 0.0, 0.0, 0.0
                                          };

    return CigiAddReplaceFixedPacket<CIGI_VIEW_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceSensorCtrl(const int sessionid, const CIGI_SENSOR_CONTROL *packet)
{
    static const CIGI_SENSOR_CONTROL mask = { sizeof(CIGI_SENSOR_CONTROL),  CIGI_SENSOR_CONTROL_OPCODE,
                                                
                                                -1, -1,
                                                0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0
                                            };

    return CigiAddReplaceFixedPacket<CIGI_SENSOR_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceMotionTrackerCtrl(const int sessionid, const CIGI_MOTION_TRACKER_CONTROL *packet)
{
    static const CIGI_MOTION_TRACKER_CONTROL mask = {  sizeof(CIGI_MOTION_TRACKER_CONTROL), CIGI_MOTION_TRACKER_CONTROL_OPCODE,
                                                        
                                                        -1, -1,
                                                        0, 0, 0, 0, 0, 0, 0, 0, 0
                                                    };

    return CigiAddReplaceFixedPacket<CIGI_MOTION_TRACKER_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceErmDef(const int sessionid, const CIGI_ERM_DEF *packet)
{
    static const CIGI_ERM_DEF mask = { sizeof(CIGI_ERM_DEF), CIGI_ERM_DEF_OPCODE,
                                        
                                        0, 0, 0.0
                                     };

    return CigiAddReplaceFixedPacket<CIGI_ERM_DEF>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceAccelerationDef(const int sessionid, const CIGI_ACCELERATION_CONTROL_DEF *packet)
{
    static const CIGI_ACCELERATION_CONTROL_DEF mask = {  sizeof(CIGI_ACCELERATION_CONTROL_DEF_SIZE), CIGI_ACCELERATION_CONTROL_DEF_OPCODE,
                                                
                                                -1,
//                                                0.0, 0.0, 0.0, 0.0, 0.0
                                            };

    return CigiAddReplaceFixedPacket<CIGI_ACCELERATION_CONTROL_DEF>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceViewDef(const int sessionid, const CIGI_VIEW_DEF *packet)
{
    static const CIGI_VIEW_DEF mask = { sizeof(CIGI_VIEW_DEF), CIGI_VIEW_DEF_OPCODE,
                                        
                                        -1,
                                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                                      };

    return CigiAddReplaceFixedPacket<CIGI_VIEW_DEF>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceCollDetSegmentDef(const int sessionid, const CIGI_COLL_SEGMENT_DEF *packet)
{
    static const CIGI_COLL_SEGMENT_DEF mask = { sizeof(CIGI_COLL_SEGMENT_DEF), CIGI_COLL_SEGMENT_DEF_OPCODE,
                                                
                                                -1, -1,
                                                0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0
                                              };

    return CigiAddReplaceFixedPacket<CIGI_COLL_SEGMENT_DEF>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceCollDetVolumeDef(const int sessionid, const CIGI_COLL_VOLUME_DEF *packet)
{
    static const CIGI_COLL_VOLUME_DEF mask = { sizeof(CIGI_COLL_VOLUME_DEF), CIGI_COLL_VOLUME_DEF_OPCODE,
                                                
                                                -1, -1,
                                                0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
                                             };

    return CigiAddReplaceFixedPacket<CIGI_COLL_VOLUME_DEF>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceHatHotReq(const int sessionid, const CIGI_HAT_HOT_REQUEST *packet)
{
    static const CIGI_HAT_HOT_REQUEST mask = { sizeof(CIGI_HAT_HOT_REQUEST), CIGI_HAT_HOT_REQUEST_OPCODE,
                                                
                                                -1, 0
                                             };

    return CigiAddReplaceFixedPacket<CIGI_HAT_HOT_REQUEST>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceLosSegmentReq(const int sessionid, const CIGI_LOS_SEGMENT_REQUEST *packet)
{
    static const CIGI_LOS_SEGMENT_REQUEST mask = { sizeof(CIGI_LOS_SEGMENT_REQUEST), CIGI_LOS_SEGMENT_REQUEST_OPCODE,
                                                    
                                                    -1, 0
                                                 };

    return CigiAddReplaceFixedPacket<CIGI_LOS_SEGMENT_REQUEST>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceLosVectorReq(const int sessionid, const CIGI_LOS_VECTOR_REQUEST *packet)
{
    static const CIGI_LOS_VECTOR_REQUEST mask = {  sizeof(CIGI_LOS_VECTOR_REQUEST), CIGI_LOS_VECTOR_REQUEST_OPCODE,
                                                    
                                                    -1,
                                                    0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0
                                                };

    return CigiAddReplaceFixedPacket<CIGI_LOS_VECTOR_REQUEST>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplacePositionReq(const int sessionid, const CIGI_POSITION_REQUEST *packet)
{
    static const CIGI_POSITION_REQUEST mask = { sizeof(CIGI_POSITION_REQUEST), CIGI_POSITION_REQUEST_OPCODE,
                                                
                                                -1, -1, 0, -1, 0
                                              };

    return CigiAddReplaceFixedPacket<CIGI_POSITION_REQUEST>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceEnvConditionsReq(const int sessionid, const CIGI_ENV_CONDITIONS_REQUEST *packet)
{
    static const CIGI_ENV_CONDITIONS_REQUEST mask = {  sizeof(CIGI_ENV_CONDITIONS_REQUEST), CIGI_ENV_CONDITIONS_REQUEST_OPCODE,
                                                        
                                                        0, 0, 0.0, 0.0
                                                    };

    return CigiAddReplaceFixedPacket<CIGI_ENV_CONDITIONS_REQUEST>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceSymbolSurfaceDef(const int sessionid, const CIGI_SYMBOL_SURFACE_DEF *packet)
{
    static const CIGI_SYMBOL_SURFACE_DEF mask = {
        sizeof(CIGI_SYMBOL_SURFACE_DEF),
        CIGI_SYMBOL_SURFACE_DEF_OPCODE,
        (uint16_t) - 1,
        0, 0, 0, 0, 0,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };

    return CigiAddReplaceFixedPacket<CIGI_SYMBOL_SURFACE_DEF>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceSymbolCtrl(const int sessionid, const CIGI_SYMBOL_CONTROL *packet)
{
    static const CIGI_SYMBOL_CONTROL mask = {
        sizeof(CIGI_SYMBOL_CONTROL),
        CIGI_SYMBOL_CONTROL_OPCODE,
        -1,
        0, 0, 0, 0, 0, 0, 0, 0,
        0.0f, 0.0f, 0.0f, 0.0f,
        0, 0, 0, 0,
        0.0f, 0.0f
    };

    return CigiAddReplaceFixedPacket<CIGI_SYMBOL_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceShortSymbolCtrl(const int sessionid, const CIGI_SYMBOL_CONTROL_S *packet)
{
    // In-place packet update is not supported for Short Symbol Ctrl because
    // the user can use multiple short symbol ctrl packets with different
    // data types.
    // Just add the packet to the end.
    return CigiAddFixedPacket<CIGI_SYMBOL_CONTROL_S>(sessionid, packet);
}

/********************************************************************************************/

int CigiAddPacketReplaceSymbolTextDef(const int sessionid, const CIGI_SYMBOL_TEXT_DEF *packet)
{
    // In-place packet update is not supported for variable-length packets.  Just
    // add the packet to the end.
    return CigiAddFixedPacket(sessionid, (const char *)packet, packet->packet_size );
}

/********************************************************************************************/

int CigiAddPacketReplaceSymbolCircleDef(const int sessionid, const CIGI_SYMBOL_CIRCLE_DEF *packet)
{
    // In-place packet update is not supported for variable-length packets.  Just
    // add the packet to the end.
    return CigiAddFixedPacket(sessionid, (const char *)packet, packet->packet_size );
}

/********************************************************************************************/

int CigiAddPacketReplaceSymbolPolygonDef(const int sessionid, const CIGI_SYMBOL_POLYGON_DEF *packet)
{
    // In-place packet update is not supported for variable-length packets.  Just
    // add the packet to the end.
    return CigiAddFixedPacket(sessionid, (const char *)packet, packet->packet_size );
}

/********************************************************************************************/

int CigiAddPacketReplaceSymbolClone(const int sessionid, const CIGI_SYMBOL_CLONE *packet)
{
    static const CIGI_SYMBOL_CLONE mask = {
        sizeof(CIGI_SYMBOL_CLONE),
        CIGI_SYMBOL_CLONE_OPCODE,
        (uint16_t) - 1,
        0, 0
    };

    return CigiAddReplaceFixedPacket<CIGI_SYMBOL_CLONE>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceEntityCtrl(const int sessionid, const CIGI_ENTITY_CONTROL *packet)
{
    static const CIGI_ENTITY_CONTROL mask = { sizeof(CIGI_ENTITY_CONTROL), CIGI_ENTITY_CONTROL_OPCODE,
                                              (uint8_t) - 1,
                                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            };

    return CigiAddReplaceFixedPacket<CIGI_ENTITY_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceAnimationCtrl(const int sessionid, const CIGI_ANIMATION_CONTROL *packet)
{
    static const CIGI_ANIMATION_CONTROL mask = { sizeof(CIGI_ANIMATION_CONTROL), CIGI_ANIMATION_CONTROL_OPCODE,
                                              0, 0, 0, 0, 0, 0,0, 0.0
                                            };

    return CigiAddReplaceFixedPacket<CIGI_ANIMATION_CONTROL>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceSymbolTexturedCircleDef(const int sessionid, const CIGI_SYMBOL_TEXTURED_CIRCLE_DEF *packet)
{
    // In-place packet update is not supported for variable-length packets.  Just
    // add the packet to the end.
    return CigiAddFixedPacket(sessionid, (const char *)packet, packet->packet_size );
}

/********************************************************************************************/

int CigiAddPacketReplaceSymbolTexturedPolygonDef(const int sessionid, const CIGI_SYMBOL_TEXTURED_POLYGON_DEF *packet)
{
    // In-place packet update is not supported for variable-length packets.  Just
    // add the packet to the end.
    return CigiAddFixedPacket(sessionid, (const char *)packet, packet->packet_size );
}

/********************************************************************************************/

int CigiAddPacketReplaceHatResp(const int sessionid, const CIGI_HAT_HOT_RESPONSE *packet)
{
    static const CIGI_HAT_HOT_RESPONSE mask = { sizeof(CIGI_HAT_HOT_RESPONSE), CIGI_HAT_HOT_RESPONSE_OPCODE,
                                                
                                                (uint8_t) - 1, 0
                                              };

    return CigiAddReplaceFixedPacket<CIGI_HAT_HOT_RESPONSE>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceHatExtResp(const int sessionid, const CIGI_HAT_HOT_EXT_RESPONSE *packet)
{
    static const CIGI_HAT_HOT_EXT_RESPONSE mask = { sizeof(CIGI_HAT_HOT_EXT_RESPONSE), CIGI_HAT_HOT_EXT_RESPONSE_OPCODE,
                                                    
                                                    (uint8_t) - 1, 0
                                                  };

    return CigiAddReplaceFixedPacket<CIGI_HAT_HOT_EXT_RESPONSE>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceLosResp(const int sessionid, const CIGI_LOS_RESPONSE *packet)
{
    static const CIGI_LOS_RESPONSE mask = { sizeof(CIGI_LOS_RESPONSE), CIGI_LOS_RESPONSE_OPCODE,
                                            
                                            (uint16_t) - 1, 0
                                          };

    return CigiAddReplaceFixedPacket<CIGI_LOS_RESPONSE>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceLosResp(const int sessionid, const CIGI_LOS_EXT_RESPONSE *packet)
{
    static const CIGI_LOS_EXT_RESPONSE mask = { sizeof(CIGI_LOS_EXT_RESPONSE), CIGI_LOS_EXT_RESPONSE_OPCODE,
                                                
                                                (uint16_t) - 1, (uint16_t) - 1, (uint8_t) - 1,
                                                0, 0, 0, 0, 0, 0, 0
                                              };

    return CigiAddReplaceFixedPacket<CIGI_LOS_EXT_RESPONSE>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceSensorResp(const int sessionid, const CIGI_SENSOR_RESPONSE *packet)
{
    static const CIGI_SENSOR_RESPONSE mask = {  sizeof(CIGI_SENSOR_RESPONSE), CIGI_SENSOR_RESPONSE_OPCODE,
                                                
                                                -1, -1,
                                                0, 0, 0, 0, 0, 0
                                             };

    return CigiAddReplaceFixedPacket<CIGI_SENSOR_RESPONSE>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceSensorExtResp(const int sessionid, const CIGI_SENSOR_EXT_RESPONSE *packet)
{
    static const CIGI_SENSOR_EXT_RESPONSE mask = { sizeof(CIGI_SENSOR_EXT_RESPONSE), CIGI_SENSOR_EXT_RESPONSE_OPCODE,
                                                    
                                                    -1, -1, -1,
                                                    0, 0, 0, 0, 0, 0, 0
                                                 };

    return CigiAddReplaceFixedPacket<CIGI_SENSOR_EXT_RESPONSE>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplacePositionResp(const int sessionid, const CIGI_POSITION_RESPONSE *packet)
{
    static const CIGI_POSITION_RESPONSE mask = {   sizeof(CIGI_POSITION_RESPONSE), CIGI_POSITION_RESPONSE_OPCODE,
                                                    
                                                    -1, -1, -1,
                                                    0, 0, 0, 0, 0, 0, 0
                                               };

    return CigiAddReplaceFixedPacket<CIGI_POSITION_RESPONSE>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceWeatherResp(const int sessionid, const CIGI_WEATHER_RESPONSE *packet)
{
    static const CIGI_WEATHER_RESPONSE mask = { sizeof(CIGI_WEATHER_RESPONSE), CIGI_WEATHER_RESPONSE_OPCODE,
                                                
                                                (uint8_t) - 1,
                                                0, 0.0, 0.0, 0.0, 00, 00, 0.0
                                              };

    return CigiAddReplaceFixedPacket<CIGI_WEATHER_RESPONSE>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceAerosolResp(const int sessionid, const CIGI_AEROSOL_RESPONSE *packet)
{
    static const CIGI_AEROSOL_RESPONSE mask = { sizeof(CIGI_AEROSOL_RESPONSE), CIGI_AEROSOL_RESPONSE_OPCODE,
                                                
                                                (uint8_t) - 1, (uint8_t) - 1, 0.0
                                              };

    return CigiAddReplaceFixedPacket<CIGI_AEROSOL_RESPONSE>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceMaritimeCondResp(const int sessionid, const CIGI_MARITIME_SURFACE_RESPONSE *packet)
{
    static const CIGI_MARITIME_SURFACE_RESPONSE mask = {sizeof(CIGI_MARITIME_SURFACE_RESPONSE), CIGI_MARITIME_SURFACE_RESPONSE_OPCODE,
                                                        
                                                        (uint8_t) - 1,
                                                        0, 0.0, 0.0
                                                       };

    return CigiAddReplaceFixedPacket<CIGI_MARITIME_SURFACE_RESPONSE>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceTerrestrialCondResp(const int sessionid, const CIGI_TERRESTRIAL_SURFACE_RESPONSE *packet)
{
    static const CIGI_TERRESTRIAL_SURFACE_RESPONSE mask = { sizeof(CIGI_TERRESTRIAL_SURFACE_RESPONSE), CIGI_TERRESTRIAL_SURFACE_RESPONSE_OPCODE,
                                                            
                                                            (uint8_t) - 1,
                                                            0
                                                          };

    return CigiAddReplaceFixedPacket<CIGI_TERRESTRIAL_SURFACE_RESPONSE>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceCollDetSegmentNotification(const int sessionid, const CIGI_COLL_SEGMENT_NOTIFICATION *packet)
{
    static const CIGI_COLL_SEGMENT_NOTIFICATION mask = { sizeof(CIGI_COLL_SEGMENT_NOTIFICATION), CIGI_COLL_SEGMENT_NOTIFICATION_OPCODE,
                                                        
                                                        -1, -1,
                                                        0, 0, 0, 0.0
                                                       };

    return CigiAddReplaceFixedPacket<CIGI_COLL_SEGMENT_NOTIFICATION>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceCollDetVolumeNotification(const int sessionid, const CIGI_COLL_VOLUME_NOTIFICATION *packet)
{
    static const CIGI_COLL_VOLUME_NOTIFICATION mask = { sizeof(CIGI_COLL_VOLUME_NOTIFICATION), CIGI_COLL_VOLUME_NOTIFICATION_OPCODE,
                                                        
                                                        -1, -1,
                                                        0, 0, 0
                                                      };

    return CigiAddReplaceFixedPacket<CIGI_COLL_VOLUME_NOTIFICATION>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceAnimationStopNotification(const int sessionid, const CIGI_ANIMATION_STOP_NOTIFICATION *packet)
{
    static const CIGI_ANIMATION_STOP_NOTIFICATION mask = { sizeof(CIGI_ANIMATION_STOP_NOTIFICATION), CIGI_ANIMATION_STOP_NOTIFICATION_OPCODE,
                                                            
                                                            (uint16_t) - 1
                                                         };

    return CigiAddReplaceFixedPacket<CIGI_ANIMATION_STOP_NOTIFICATION>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketReplaceEventNotification(const int sessionid, const CIGI_EVENT_NOTIFICATION *packet)
{
    static const CIGI_EVENT_NOTIFICATION mask = {  sizeof(CIGI_EVENT_NOTIFICATION), CIGI_EVENT_NOTIFICATION_OPCODE,
                                                    
                                                    (uint16_t) - 1,
                                                    0, 0, 0
                                                };

    return CigiAddReplaceFixedPacket<CIGI_EVENT_NOTIFICATION>(sessionid, packet, &mask);
}

/********************************************************************************************/

int CigiAddPacketIGMessage(const int sessionid, const CIGI_IG_MESSAGE *packet)
{
    static int retOpcode = CIGI_SUCCESS;
    static int packetLen = 0;
    static CIGI_SESSION *pcs = NULL;
    static unsigned char *bufferPos = NULL;

    /* determine the size of the packet and the message string including the
     * terminating NULL, ensuring that the message length ends on a 32-bit
     * word boundary
     */
    packetLen = CIGI_IG_MESSAGE_BASE_SIZE
                + (int)strlen(packet->text)
                + 1;

    if ((packetLen % 4) != 0)
        packetLen += (4 - (packetLen % 4));

    /* make sure the total packet size does not exceed the maximum allowable size */
    if (packetLen > CIGI_MAX_PACKET_SIZE)
        return CIGI_ERROR_PACKET_TOO_LARGE;

    /* make sure a session pointer can be retreived and the session can
     * handle the new packet
     */
    retOpcode = PrepareSessionAndGetPtr(sessionid, packetLen, &pcs);

    if (retOpcode != CIGI_SUCCESS)
        return retOpcode;

    /* check for the correct session type to add the packet */
    if (pcs->SessionType != CIGI_IG_SESSION)
        return CIGI_ERROR_INVALID_SESSION_TYPE;

    /* copy the data from the packet to the buffer */
    bufferPos = pcs->BufferPtrs[ pcs->BufferIdx];
    memcpy(bufferPos, packet, packetLen);

    /* ensure that the opcode and packet size fields are correct */
    ((CIGI_PACKET_HEADER *)bufferPos)->packet_id = CIGI_IG_MESSAGE_OPCODE;
    ((CIGI_PACKET_HEADER *)bufferPos)->packet_size = packetLen;

    /* increment the current buffer pointer */
    pcs->BufferPtrs[ pcs->BufferIdx] += packetLen;

    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  CigiSwapPacketEntityPosition()                                                */
/** ----------------------------------------------------------------------------
 *  Byte-swaps the fields in a CIGI_ENTITY_CONTROL packet.
 *
 *  @param packet     [in] Address of the packet to be swapped.
 *
 *  @param dest       [in] Starting address of where the result is written to.
 *                    The value of this parameter may be the same as the packet
 *                    being swapped.
 *
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 *
 *  @see              CIGI_ENTITY_POSITION
 * -----------------------------------------------------------------------------
 */
/*:Associate with "API_Internals" */
int CigiSwapPacketEntityPosition(const CIGI_ENTITY_POSITION *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet size : 2 bytes */
    /* packet id : 2 bytes */
    CigiCopy2(dest, src);
    CigiCopy2(dest+2, src+2);

    /* entity id : 2 bytes */
    CigiSwap2(dest + 2, src + 2);

    /* state                       : 2 bits */
    /* attach state                : 1 bit */
    /* collision detection request : 1 bit */
    /* inherit alpha flag          : 1 bits */
    /* clamp mode                  : 2 bits */
    /* spare                       : 1 bit */
    /* animation direction         : 1 bit */
    /* animation loop mode         : 1 bit */
    /* spare                       : 4 bits */
    /* animation state             : 2 bytes */
    /* alpha                       : 8 bytes */
    /* spare                       : 8 bytes */
    CigiCopy4(dest + 4, src + 4);

    /* entity type : 2 bytes */
    CigiSwap2(dest + 8, src + 8);

    /* parent entity id : 2 bytes */
    CigiSwap2(dest + 10, src + 10);

    /* entity roll : 4 bytes */
    CigiSwap4(dest + 12, src + 12);

    /* entity pitch : 4 bytes */
    CigiSwap4(dest + 16, src + 16);

    /* entity yaw : 4 bytes */
    CigiSwap4(dest + 20, src + 20);

    /* entity latitude / x offset : 8 bytes */
    CigiSwap8(dest + 24, src + 24);

    /* entity longitude / y offset : 8 bytes */
    CigiSwap8(dest + 32, src + 32);

    /* entity altitude / z offset : 8 bytes */
    CigiSwap8(dest + 40, src + 40);

    return CIGI_SUCCESS;
}

int CigiSwapPacketCCEntityPosition(const CIGI_ENTITY_POSITION_CC *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);

    /* entity id : 2 bytes */
    CigiSwap2(dest + 2, src + 2);

    /* yaw : four bytes */
    CigiSwap4(dest + 4, src + 4);

    /* latitude : eight bytes */
    CigiSwap8(dest + 8, src + 8);

    /* longitude : eight bytes */
    CigiSwap8(dest + 16, src + 16);

    return CIGI_SUCCESS;
}

int CigiSwapPacketComponentCtrl(const CIGI_COMPONENT_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);

    CigiSwap2(dest + 2, src + 2);

    CigiSwap2(dest + 4, src + 4);
    CigiCopy2(dest + 6, src + 6);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    CigiSwap4(dest + 24, src + 24);

    CigiSwap4(dest + 28, src + 28);

    return CIGI_SUCCESS;
}

int CigiSwapPacketShortComponentCtrl(const CIGI_COMPONENT_CONTROL_S *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiSwap2(dest + 4, src + 4);
    CigiCopy2(dest + 6, src + 6);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    return CIGI_SUCCESS;
}

int CigiSwapPacketArtPartCtrl(const CIGI_ARTPART_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);

    /* entity id : 2 bytes */
    CigiSwap2(dest + 2, src + 2);

    /* flags : 4 bytes */
    CigiCopy4(dest + 4, src + 4);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    CigiSwap4(dest + 24, src + 24);

    CigiSwap4(dest + 28, src + 28);

    return CIGI_SUCCESS;
}

int CigiSwapPacketShortArtPartCtrl(const CIGI_ARTPART_CONTROL_S *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    return CIGI_SUCCESS;
}

int CigiSwapPacketVelocityCtrl(const CIGI_VELOCITY_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    CigiSwap4(dest + 24, src + 24);

    CigiSwap4(dest + 28, src + 28);

    return CIGI_SUCCESS;
}

int CigiSwapPacketCelestialSphereCtrl(const CIGI_CELESTIAL_SPHERE_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy8(dest, src);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    return CIGI_SUCCESS;
}

int CigiSwapPacketAtmosphereCtrl(const CIGI_ATMOSPHERE_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy4(dest, src);

    CigiSwap4(dest + 4, src + 4);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    CigiSwap4(dest + 24, src + 24);

    CigiCopy4(dest + 28, src + 28);

    return CIGI_SUCCESS;
}

int CigiSwapPacketEnvRegionCtrl(const CIGI_ENV_REGION_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    CigiSwap8(dest + 8, src + 8);

    CigiSwap8(dest + 16, src + 16);

    CigiSwap4(dest + 24, src + 24);

    CigiSwap4(dest + 28, src + 28);

    CigiSwap4(dest + 32, src + 32);

    CigiSwap4(dest + 36, src + 36);

    CigiSwap4(dest + 40, src + 40);

    CigiCopy4(dest + 44, src + 44);

    return CIGI_SUCCESS;
}

int CigiSwapPacketWeatherCtrl(const CIGI_WEATHER_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);

    /* entity/region id : 2 bytes */
    CigiSwap2(dest + 2, src + 2);

    /* flags : 4 bytes */
    CigiCopy4(dest + 4, src + 4);

    /* air temp : 4 bytes */
    CigiSwap4(dest + 8, src + 8);

    /* visibility : 4 bytes */
    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    CigiSwap4(dest + 24, src + 24);

    CigiSwap4(dest + 28, src + 28);

    CigiSwap4(dest + 32, src + 32);

    CigiSwap4(dest + 36, src + 36);

    CigiSwap4(dest + 40, src + 40);

    CigiSwap4(dest + 44, src + 44);

    CigiSwap4(dest + 48, src + 48);

    CigiSwap4(dest + 52, src + 52);

    return CIGI_SUCCESS;
}

int CigiSwapPacketMaritimeCondCtrl(const CIGI_MARITIME_CONDITIONS_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    return CIGI_SUCCESS;
}

int CigiSwapPacketWaveCtrl(const CIGI_WAVE_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    CigiSwap4(dest + 24, src + 24);

    CigiSwap4(dest + 28, src + 28);

    return CIGI_SUCCESS;
}

int CigiSwapPacketTerrestrialCondCtrl(const CIGI_TERRESTRIAL_CONDITIONS_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiSwap2(dest + 4, src + 4);
    CigiCopy2(dest + 6, src + 6);

    return CIGI_SUCCESS;
}

int CigiSwapPacketViewCtrl(const CIGI_VIEW_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    CigiSwap4(dest + 24, src + 24);

    CigiSwap4(dest + 28, src + 28);

    return CIGI_SUCCESS;
}

int CigiSwapPacketSensorCtrl(const CIGI_SENSOR_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    return CIGI_SUCCESS;
}

int CigiSwapPacketMotionTrackerCtrl(const CIGI_MOTION_TRACKER_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    return CIGI_SUCCESS;
}

int CigiSwapPacketErmDef(const CIGI_ERM_DEF *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy8(dest, src);

    CigiSwap8(dest + 8, src + 8);

    CigiSwap8(dest + 16, src + 16);

    return CIGI_SUCCESS;
}

int CigiSwapPacketAccelerationDef(const CIGI_ACCELERATION_CONTROL_DEF *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiSwap4(dest + 4, src + 4);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    return CIGI_SUCCESS;
}

int CigiSwapPacketViewDef(const CIGI_VIEW_DEF *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    CigiSwap4(dest + 24, src + 24);

    CigiSwap4(dest + 28, src + 28);


    return CIGI_SUCCESS;
}

int CigiSwapPacketCollSegmentDef(const CIGI_COLL_SEGMENT_DEF *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    CigiSwap4(dest + 24, src + 24);

    CigiSwap4(dest + 28, src + 28);

    CigiSwap4(dest + 32, src + 32);

    CigiCopy4(dest + 36, src + 36);

    return CIGI_SUCCESS;
}

int CigiSwapPacketCollVolumeDef(const CIGI_COLL_VOLUME_DEF *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    CigiSwap4(dest + 24, src + 24);

    CigiSwap4(dest + 28, src + 28);

    CigiSwap4(dest + 32, src + 32);

    CigiSwap4(dest + 36, src + 36);

    CigiSwap4(dest + 40, src + 40);

    CigiCopy4(dest + 44, src + 44);

    return CIGI_SUCCESS;
}

int CigiSwapPacketHatHotReq(const CIGI_HAT_HOT_REQUEST *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    CigiSwap8(dest + 8, src + 8);

    CigiSwap8(dest + 16, src + 16);

    CigiSwap8(dest + 24, src + 24);

    return CIGI_SUCCESS;
}

int CigiSwapPacketLosSegmentReq(const CIGI_LOS_SEGMENT_REQUEST *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    CigiSwap8(dest + 8, src + 8);

    CigiSwap8(dest + 16, src + 16);

    CigiSwap8(dest + 24, src + 24);

    CigiSwap8(dest + 32, src + 32);

    CigiSwap8(dest + 40, src + 40);

    CigiSwap8(dest + 48, src + 48);

    CigiSwap4(dest + 56, src + 56);

    CigiCopy4(dest + 60, src + 60);

    return CIGI_SUCCESS;
}

int CigiSwapPacketLosVectorReq(const CIGI_LOS_VECTOR_REQUEST *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    CigiSwap8(dest + 24, src + 24);

    CigiSwap8(dest + 32, src + 32);

    CigiSwap8(dest + 40, src + 40);

    CigiSwap4(dest + 48, src + 48);

    CigiCopy4(dest + 52, src + 52);

    return CIGI_SUCCESS;
}

int CigiSwapPacketPositionReq(const CIGI_POSITION_REQUEST *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    return CIGI_SUCCESS;
}

int CigiSwapPacketEnvCondReq(const CIGI_ENV_CONDITIONS_REQUEST *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy8(dest, src);

    CigiSwap8(dest + 8, src + 8);

    CigiSwap8(dest + 16, src + 16);

    CigiSwap8(dest + 24, src + 24);

    return CIGI_SUCCESS;
}

int CigiSwapPacketSymbolSurfaceDef(const CIGI_SYMBOL_SURFACE_DEF *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);

    // surface_id
    CigiSwap2(dest + 2, src + 2);

    // bit fields and reserved
    CigiCopy2(dest + 4, src + 4);

    // entity/view id
    CigiSwap2(dest + 6, src + 6);

    // the rest:
    CigiSwap4(dest + 8, src + 8);
    CigiSwap4(dest + 12, src + 12);
    CigiSwap4(dest + 16, src + 16);
    CigiSwap4(dest + 20, src + 20);
    CigiSwap4(dest + 24, src + 24);
    CigiSwap4(dest + 28, src + 28);
    CigiSwap4(dest + 32, src + 32);
    CigiSwap4(dest + 36, src + 36);
    CigiSwap4(dest + 40, src + 40);
    CigiSwap4(dest + 44, src + 44);
    CigiSwap4(dest + 48, src + 48);
    CigiSwap4(dest + 52, src + 52);

    return CIGI_SUCCESS;
}

int CigiSwapPacketSymbolCtrl(const CIGI_SYMBOL_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);

    // symbol_id
    CigiSwap2(dest + 2, src + 2);

    // bit fields and reserved
    CigiCopy2(dest + 4, src + 4);

    // parent id
    CigiSwap2(dest + 6, src + 6);

    // the rest:
    CigiSwap2(dest + 8, src + 8);
    CigiCopy2(dest + 10, src + 10);
    CigiSwap4(dest + 12, src + 12);
    CigiSwap4(dest + 16, src + 16);
    CigiSwap4(dest + 20, src + 20);
    CigiSwap4(dest + 24, src + 24);
    CigiCopy4(dest + 28, src + 28);  // RGBA
    CigiSwap4(dest + 32, src + 32);
    CigiSwap4(dest + 36, src + 36);

    return CIGI_SUCCESS;
}

int CigiSwapPacketShortSymbolCtrl(const CIGI_SYMBOL_CONTROL_S *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);

    // symbol_id
    CigiSwap2(dest + 2, src + 2);

    // bit fields, reserved, datum types
    CigiCopy4(dest + 4, src + 4);

    // the rest:
    CigiSwap4(dest + 8, src + 8);
    CigiSwap4(dest + 12, src + 12);

    return CIGI_SUCCESS;
}

int CigiSwapPacketSymbolTextDef(const CIGI_SYMBOL_TEXT_DEF *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);

    // symbol_id
    CigiSwap2(dest + 2, src + 2);

    // bit fields, font id, and reserved
    CigiCopy4(dest + 4, src + 4);

    // font size
    CigiSwap4(dest + 8, src + 8);

    // The rest are chars, so we're done.

    return CIGI_SUCCESS;
}

int CigiSwapPacketSymbolCircleDef(const CIGI_SYMBOL_CIRCLE_DEF *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);

    // symbol_id
    CigiSwap2(dest + 2, src + 2);

    // bit fields, reserved, and stipple pattern
    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    // line width, stipple pattern length
    CigiSwap4(dest + 8, src + 8);
    CigiSwap4(dest + 12, src + 12);

    // Swap each circle definition.  "Unroll" the loop for performance.
    src += 16;
    dest += 16;
    int circleCount = (packet->packet_size - 16) / 24;
    for (int i = 0; i < circleCount; ++i) {
        CigiSwap4(dest, src);
        CigiSwap4(dest + 4, src + 4);
        CigiSwap4(dest + 8, src + 8);
        CigiSwap4(dest + 12, src + 12);
        CigiSwap4(dest + 16, src + 16);
        CigiSwap4(dest + 20, src + 20);

        src += 24;
        dest += 24;
    }

    return CIGI_SUCCESS;
}

int CigiSwapPacketSymbolTexturedCircleDef(const CIGI_SYMBOL_TEXTURED_CIRCLE_DEF *packet, char *dest)
{
#if 0
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);

    // symbol_id
    CigiSwap2(dest + 2, src + 2);

    // bit fields, reserved, and stipple pattern
    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    // line width, stipple pattern length
    CigiSwap4(dest + 8, src + 8);
    CigiSwap4(dest + 12, src + 12);

    // Swap each circle definition.  "Unroll" the loop for performance.
    src += 16;
    dest += 16;
    int circleCount = (packet->packet_size - 16) / 24;
    for (int i = 0; i < circleCount; ++i) {
        CigiSwap4(dest, src);
        CigiSwap4(dest + 4, src + 4);
        CigiSwap4(dest + 8, src + 8);
        CigiSwap4(dest + 12, src + 12);
        CigiSwap4(dest + 16, src + 16);
        CigiSwap4(dest + 20, src + 20);

        src += 24;
        dest += 24;
    }
#endif
    return CIGI_SUCCESS;
}

int CigiSwapPacketSymbolPolygonDef(const CIGI_SYMBOL_POLYGON_DEF *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);

    // symbol_id
    CigiSwap2(dest + 2, src + 2);

    // bit fields, reserved, and stipple pattern
    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    // line width, stipple pattern length
    CigiSwap4(dest + 8, src + 8);
    CigiSwap4(dest + 12, src + 12);

    // Swap each vertex.  "Unroll" the loop for performance.
    src += 16;
    dest += 16;
    int lineCount = (packet->packet_size - 16) / 8;
    for (int i = 0; i < lineCount; ++i) {
        CigiSwap4(dest, src);
        CigiSwap4(dest + 4, src + 4);

        src += 8;
        dest += 8;
    }

    return CIGI_SUCCESS;
}

int CigiSwapPacketSymbolTexturedPolygonDef(const CIGI_SYMBOL_TEXTURED_POLYGON_DEF *packet, char *dest)
{
#if 0
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);

    // symbol_id
    CigiSwap2(dest + 2, src + 2);

    // bit fields, reserved, and stipple pattern
    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    // line width, stipple pattern length
    CigiSwap4(dest + 8, src + 8);
    CigiSwap4(dest + 12, src + 12);

    // Swap each vertex.  "Unroll" the loop for performance.
    src += 16;
    dest += 16;
    int lineCount = (packet->packet_size - 16) / 8;
    for (int i = 0; i < lineCount; ++i) {
        CigiSwap4(dest, src);
        CigiSwap4(dest + 4, src + 4);

        src += 8;
        dest += 8;
    }
#endif
    return CIGI_SUCCESS;
}


int CigiSwapPacketSymbolClone(const CIGI_SYMBOL_CLONE *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);

    // symbol_id
    CigiSwap2(dest + 2, src + 2);

    // bit field and reserved
    CigiCopy2(dest + 4, src + 4);

    // source_id
    CigiSwap2(dest + 6, src + 6);

    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  CigiSwapPacketEntityCtrl()                                                */
/** ----------------------------------------------------------------------------
 *  Byte-swaps the fields in a CIGI_ENTITY_CONTROL packet.
 *
 *  @param packet     [in] Address of the packet to be swapped.
 *
 *  @param dest       [in] Starting address of where the result is written to.
 *                    The value of this parameter may be the same as the packet
 *                    being swapped.
 *
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 *
 *  @see              CIGI_ENTITY_CONTROL
 * -----------------------------------------------------------------------------
 */
/*:Associate with "API_Internals" */
int CigiSwapPacketEntityCtrl(const CIGI_ENTITY_CONTROL *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);

    /* entity id : 2 bytes */
    CigiSwap2(dest + 2, src + 2);

    /* state                       : 2 bits */
    /* attach state                : 1 bit */
    /* collision detection request : 1 bit */
    /* inherit alpha flag          : 1 bits */
    /* clamp mode                  : 2 bits */
    /* spare                       : 1 bit */
    /* animation direction         : 1 bit */
    /* animation loop mode         : 1 bit */
    /* spare                       : 4 bits */
    /* animation state             : 2 bytes */
    /* alpha                       : 8 bytes */
    /* spare                       : 8 bytes */
    CigiCopy4(dest + 4, src + 4);

    /* entity type : 2 bytes */
    CigiSwap2(dest + 8, src + 8);

    /* parent entity id : 2 bytes */
    CigiSwap2(dest + 10, src + 10);

    /* entity roll : 4 bytes */
    CigiSwap4(dest + 12, src + 12);

    /* entity pitch : 4 bytes */
    CigiSwap4(dest + 16, src + 16);

    /* entity yaw : 4 bytes */
    CigiSwap4(dest + 20, src + 20);

    /* entity latitude / x offset : 8 bytes */
    CigiSwap8(dest + 24, src + 24);

    /* entity longitude / y offset : 8 bytes */
    CigiSwap8(dest + 32, src + 32);

    /* entity altitude / z offset : 8 bytes */
    CigiSwap8(dest + 40, src + 40);

    return CIGI_SUCCESS;
}

int CigiSwapPacketHatHotResp(const CIGI_HAT_HOT_RESPONSE *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    CigiSwap8(dest + 8, src + 8);

    return CIGI_SUCCESS;
}

int CigiSwapPacketHatHotExtResp(const CIGI_HAT_HOT_EXT_RESPONSE *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    CigiSwap8(dest + 8, src + 8);

    CigiSwap8(dest + 16, src + 16);

    CigiSwap4(dest + 24, src + 24);

    CigiSwap4(dest + 28, src + 28);

    CigiSwap4(dest + 32, src + 32);

    CigiCopy4(dest + 36, src + 36);

    return CIGI_SUCCESS;
}

int CigiSwapPacketLosResp(const CIGI_LOS_RESPONSE *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    CigiSwap8(dest + 8, src + 8);

    return CIGI_SUCCESS;
}

int CigiSwapPacketLosExtResp(const CIGI_LOS_EXT_RESPONSE *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    CigiSwap8(dest + 8, src + 8);

    CigiSwap8(dest + 16, src + 16);

    CigiSwap8(dest + 24, src + 24);

    CigiSwap8(dest + 32, src + 32);

    CigiCopy4(dest + 40, src + 40);

    CigiSwap4(dest + 44, src + 44);

    CigiSwap4(dest + 48, src + 48);

    CigiSwap4(dest + 52, src + 52);

    return CIGI_SUCCESS;
}

int CigiSwapPacketSensorResp(const CIGI_SENSOR_RESPONSE *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    CigiSwap2(dest + 8, src + 8);
    CigiSwap2(dest + 10, src + 10);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    return CIGI_SUCCESS;
}

int CigiSwapPacketSensorExtResp(const CIGI_SENSOR_EXT_RESPONSE *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    CigiSwap2(dest + 8, src + 8);
    CigiSwap2(dest + 10, src + 10);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    CigiSwap8(dest + 24, src + 24);

    CigiSwap8(dest + 32, src + 32);

    CigiSwap8(dest + 40, src + 40);

    return CIGI_SUCCESS;
}

int CigiSwapPacketPositionResp(const CIGI_POSITION_RESPONSE *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    CigiSwap8(dest + 8, src + 8);

    CigiSwap8(dest + 16, src + 16);

    CigiSwap8(dest + 24, src + 24);

    CigiSwap4(dest + 32, src + 32);

    CigiSwap4(dest + 36, src + 36);

    CigiSwap4(dest + 40, src + 40);

    CigiCopy4(dest + 44, src + 44);

    return CIGI_SUCCESS;
}

int CigiSwapPacketWeatherResp(const CIGI_WEATHER_RESPONSE *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy4(dest, src);

    CigiSwap4(dest + 4, src + 4);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    CigiSwap4(dest + 16, src + 16);

    CigiSwap4(dest + 20, src + 20);

    CigiSwap4(dest + 24, src + 24);

    CigiCopy4(dest + 28, src + 28);

    return CIGI_SUCCESS;
}

int CigiSwapPacketAerosolResp(const CIGI_AEROSOL_RESPONSE *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy4(dest, src);

    CigiSwap4(dest + 4, src + 4);

    return CIGI_SUCCESS;
}

int CigiSwapPacketMaritimeCondResp(const CIGI_MARITIME_SURFACE_RESPONSE *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy4(dest, src);

    CigiSwap4(dest + 4, src + 4);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    return CIGI_SUCCESS;
}

int CigiSwapPacketTerrestrialCondResp(const CIGI_TERRESTRIAL_SURFACE_RESPONSE *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy4(dest, src);

    CigiSwap4(dest + 4, src + 4);

    return CIGI_SUCCESS;
}

int CigiSwapPacketCollSegmentNotification(const CIGI_COLL_SEGMENT_NOTIFICATION *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    return CIGI_SUCCESS;
}

int CigiSwapPacketCollVolumeNotification(const CIGI_COLL_VOLUME_NOTIFICATION *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy2(dest + 4, src + 4);
    CigiSwap2(dest + 6, src + 6);

    CigiCopy8(dest + 8, src + 8);

    return CIGI_SUCCESS;
}

int CigiSwapPacketAnimationStopNotification(const CIGI_ANIMATION_STOP_NOTIFICATION *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiCopy4(dest + 4, src + 4);

    return CIGI_SUCCESS;
}

int CigiSwapPacketEventNotification(const CIGI_EVENT_NOTIFICATION *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    CigiSwap4(dest + 4, src + 4);

    CigiSwap4(dest + 8, src + 8);

    CigiSwap4(dest + 12, src + 12);

    return CIGI_SUCCESS;
}

int CigiSwapPacketIGMessage(const CIGI_IG_MESSAGE *packet, char *dest)
{
    static char *src = NULL;
    int msglen = packet->packet_size - 4;

    src = (char *)packet;

    /* packet id : 1 byte */
    /* packet size : 1 byte */
    CigiCopy2(dest, src);
    CigiSwap2(dest + 2, src + 2);

    // Copy the remaining data if the source and destination pointers
    // are not the same.
    if ((void *)packet != (void *)dest)
        memcpy(dest, packet, msglen);

    return CIGI_SUCCESS;
}

int CigiSwapPacketUserData(const CIGI_USER_DATA_PACKET *packet, char *dest)
{
    static char *src = NULL;

    src = (char *)packet;

    return CIGI_ERROR_NOT_IMPLEMENTED;
}

/* -----------------------------------------------------------------------------
 *  CigiAddPacketSof()                                                        */
/** ----------------------------------------------------------------------------
 *  Adds a CIGI_START_OF_FRAME_30 or CIGI_START_OF_FRAME_32 packet to the
 *  current CIGI message. If the
 *  packet will not fit into the current message buffer, it will be placed in
 *  another buffer to be sent at a later time.
 *
 *  @note Packets are buffered in the order in which they are added. The size
 *        of the buffer and the number of back buffers are set in the
 *        CigiCreateSession() method.
 *
 *        The packet added by the CigiAddPacketSof() method sets the frame
 *        counter value received by the host. If the start of frame packet is
 *        not explicitly called using this method, the frame counter sent will
 *        automatically be incremented from its value during the previous
 *        message.
 *
 *        To disable the increment of the frame counter value, a SOF packet
 *        must be explicitly added through a call to CigiAddPacketSof() each
 *        frame.
 *
 *        The session used must be of type CIGI_HOST_SESSION.
 *
 *  @param sessionid  [in] The ID of the session for which to add the packet,
 *                    as returned by the CigiCreateSession() method.
 *
 *  @param packet     [in] Address of the packet to be added.
 *
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 *
 *  @see              CIGI_START_OF_FRAME
 * -----------------------------------------------------------------------------
 */
/*:Associate with "IG_Session_Functions" */
int CigiAddPacketSof(const int sessionid, const CIGI_START_OF_FRAME *packet)
{
    int ret = CIGI_SUCCESS;

    switch (_CigiMinorVersion) {
    case 0:
    default:
        ret = CigiAddFixedPacket<CIGI_START_OF_FRAME>(sessionid, packet, true);
        break;
    }

    return ret;
}

#if 0
/* -----------------------------------------------------------------------------
 *  CigiSwapPacketsof()                                                       */
/** ----------------------------------------------------------------------------
 *  Byte-swaps the fields in a CIGI_START_OF_FRAME_30 packet.
 *
 *  @param packet     [in] Address of the packet to be swapped.
 *
 *  @param dest       [in] Starting address of where the result is written to.
 *                    The value of this parameter may be the same as the packet
 *                    being swapped.
 *
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 *
 *  @see              CIGI_START_OF_FRAME
 * -----------------------------------------------------------------------------
 */
/*:Associate with "API_Internals" */
static int CigiSwapPacketsof30(const CIGI_START_OF_FRAME_30 *packet, char *dest)
{
    uint8_t *src = (uint8_t *)packet;

    // packet id      : 1 byte
    // packet size    : 1 byte
    // cigi version # : 1 byte
    // database #     : 1 byte
    CigiCopy4(dest, src);

    // ig status code        : 1 byte
    // current ig mode       :
    // timestamp valid       :
    // earth reference model :
    // reserved              : 1 byte
    CigiCopy2(dest + 4, src + 4);

    // byteswap : 2 bytes
    CigiSwap2(dest + 6, src + 6);

    // frame counter : 4 bytes
    CigiSwap4(dest + 8, src + 8);

    // timestamp : 4 bytes
    CigiSwap4(dest + 12, src + 12);

    return CIGI_SUCCESS;
}

/* -----------------------------------------------------------------------------
 *  CigiSwapPacketSof32()                                                       */
/** ----------------------------------------------------------------------------
 *  Byte-swaps the fields in a CIGI_START_OF_FRAME_32 packet.
 *
 *  @param packet     [in] Address of the packet to be swapped.
 *
 *  @param dest       [in] Starting address of where the result is written to.
 *                    The value of this parameter may be the same as the packet
 *                    being swapped.
 *
 *  @return           Zero (0) if successful, a negative error code value
 *                    otherwise.
 *
 *  @see              CIGI_START_OF_FRAME
 * -----------------------------------------------------------------------------
 */
/*:Associate with "API_Internals" */
static int CigiSwapPacketSof32(const CIGI_START_OF_FRAME_32 *packet, char *dest)
{
    uint8_t *src = (uint8_t *)packet;

    // packet id      : 1 byte
    // packet size    : 1 byte
    // cigi version # : 1 byte
    // database #     : 1 byte
    CigiCopy4(dest, src);

    // ig status code        : 1 byte
    // current ig mode       :
    // timestamp valid       :
    // earth reference model :
    // reserved              : 1 byte
    CigiCopy2(dest + 4, src + 4);

    // byteswap : 2 bytes
    CigiSwap2(dest + 6, src + 6);

    // IG frame # : 4 bytes
    CigiSwap4(dest + 8, src + 8);

    // timestamp : 4 bytes
    CigiSwap4(dest + 12, src + 12);

    // last host frame # : 4 bytes
    CigiSwap4(dest + 16, src + 16);

    // reserved : 4 bytes
    CigiSwap4(dest + 20, src + 20);

    return CIGI_SUCCESS;
}
#endif

static int CigiSwapPacketsof(const CIGI_START_OF_FRAME *packet, char *dest)
{
    return CIGI_SUCCESS;
}