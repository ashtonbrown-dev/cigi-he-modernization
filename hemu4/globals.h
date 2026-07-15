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

///////////////////////////////////////////////////////////////////////////////
//
//  Includes
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _HEMU3_GLOBALS_
#define _HEMU3_GLOBALS_

#ifdef _DEBUG
#define _USE_DEBUGTRACE
#endif

#include "DataManager.h"
#include "CigiProtocolVersion.h"

#define DIRECTINPUT_VERSION     0x0800
#include <basetsd.h>
#include <dinput.h>

class CEntity;
class HEMU_MESSAGE;

///////////////////////////////////////////////////////////////////////////////
//
//  Symbols and Macros
//
///////////////////////////////////////////////////////////////////////////////

#define IMG_GENERIC             ENTITY_CLASS_GENERIC
#define IMG_FIXEDWING           ENTITY_CLASS_FIXEDWING
#define IMG_ANIMATION           ENTITY_CLASS_ANIMATION
#define IMG_CLOUD               ENTITY_CLASS_CLOUD
#define IMG_MISSILE             ENTITY_CLASS_MISSILE
#define IMG_TANK                ENTITY_CLASS_TANK
#define IMG_CAR                 ENTITY_CLASS_CAR
#define IMG_BOAT                ENTITY_CLASS_BOAT
#define IMG_ROTORCRAFT          ENTITY_CLASS_ROTORCRAFT
#define IMG_BOMB                ENTITY_CLASS_BOMB
#define IMG_SAM                 ENTITY_CLASS_SAM
#define IMG_PATTERN             ENTITY_CLASS_PATTERN
#define IMG_CULTURE             ENTITY_CLASS_CULTURE
#define IMG_NOTYPE              31

#define IMG_VIEW                0
#define IMG_VIEWGROUP           1
#define IMG_SKY                 2
#define IMG_GLOBE               3
#define IMG_REGION              4
#define IMG_SEASTATE            5
#define IMG_SURFACECOND         6
#define IMG_WEATHERLAYER        7
#define IMG_QUESTIONMARK        8
#define IMG_CHECKMARK           9
#define IMG_EX                  10
#define IMG_WEATHER_ENTITY      11

#define SCOPE_GLOBAL    0
#define SCOPE_REGIONAL  1
#define SCOPE_ENTITY    2

#define RECPLAYBACK_STATE_STOP      0
#define RECPLAYBACK_STATE_PAUSE     1
#define RECPLAYBACK_STATE_PLAY      2
#define RECPLAYBACK_STATE_RECORD    4

#define SCRIPT_STATE_STOP           0
#define SCRIPT_STATE_PAUSE          1   // 001 binary
#define SCRIPT_STATE_PLAY           2   // 010 binary
//#define SCRIPT_STATE_AUTO_PAUSE       5   // 101 binary

///////////////////////////////////////////////////////////////////////////////
//
//  Global Variables
//
///////////////////////////////////////////////////////////////////////////////

// Colors for the situational display control
extern COLORREF EntityClassColor[13];

// Pointers to our DirectInput interfaces.
extern IDirectInput8            *g_pIDirectInput;
extern IDirectInputDevice8      *g_pJoystick;

// Handles to tree items.
extern HTREEITEM g_htreeSky;
extern HTREEITEM g_htreeTerrain;
extern HTREEITEM g_htreeRegions;

// These image lists will be shared by many CListCtrl and CTreeCtrl objects.
extern CImageList g_ImgListTypes;
extern CImageList g_ImgListIcons;

// Global CDataManager object;
extern CDataManager g_DataManager;

// Frame rate -- used in time calculations (e.g., missile firing)
extern volatile int g_FrameRate;
extern volatile int g_AvgFrameRate;

// Frame counter received from IG message.
extern volatile unsigned int g_FrameCounter;

// Because we are dequeueing a bunch of Host-to-IG packets and then
// IG-to-Host messages, we need to keep track of the Frame Counter
// we used to send the IG-to-Host message.
extern volatile unsigned int g_WatchFrameCounter;

// CIGI session IDs.
extern int g_HostSession;
extern int g_IGSession;

// Selected CIGI protocol version. Packet I/O support is provided by the
// protocol adapter selected in the driver.
extern CigiProtocolVersion g_CigiProtocolVersion;

// Flags used to indicate state of simulation.
extern volatile BOOL g_bFreezeFlag;
extern volatile BOOL g_bResetHostFlag;
extern volatile BOOL g_bResetIGFlag;
extern volatile BOOL g_bDebugIGFlag;

// Flags used to indicate watch, script, and rec/playback states.
extern volatile BOOL g_bWatchPackets;
extern volatile BOOL g_ScriptParsePause;
extern volatile int g_ScriptState;
extern volatile int g_NestedScriptCount;
extern volatile int g_RecPlaybackState;
extern volatile BOOL g_bRecPlaybackLoop;

// Handle to the script thread.
extern unsigned int g_ScriptThreadAddrHdl;

extern volatile unsigned int    g_TestScriptTranspDelayRunning;

// Filter flags for received CIGI packets.
extern struct _FILTER_FLAGS {
    // IG-to-Host
    unsigned int StartOfFrame               : 1;
    unsigned int HatResp                    : 1;
    unsigned int HatExtResp                 : 1;
    unsigned int LosResp                    : 1;
    unsigned int LosExtResp                 : 1;
    unsigned int SensorResp                 : 1;
    unsigned int SensorExtResp              : 1;
    unsigned int PositionResp               : 1;
    unsigned int WeatherCondResp            : 1;
    unsigned int AerosolResp                : 1;
    unsigned int MaritimeCondResp           : 1;
    unsigned int TerrestrialCondResp        : 1;
    unsigned int CDSegmentNotification      : 1;
    unsigned int CDVolumeNotification       : 1;
    unsigned int AnimationStopNotification  : 1;
    unsigned int EventNotification          : 1;
    unsigned int IGMessage                  : 1;

    // Host-to-IG
    unsigned int ArtPartControl             : 1;
    unsigned int AtmosphereControl          : 1;
    unsigned int CCEntityControl            : 1;
    unsigned int CDSegmentDef               : 1;
    unsigned int CDVolumeDef                : 1;
    unsigned int CelestialSphereControl     : 1;
    unsigned int ComponentControl           : 1;
    unsigned int EntityControl              : 1;
    unsigned int EnvRegionControl           : 1;
    unsigned int EnvConditionsReq           : 1;
    unsigned int ErmDef                     : 1;
    unsigned int HatHotReq                  : 1;
    unsigned int IGControl                  : 1;
    unsigned int LOSSegmentReq              : 1;
    unsigned int LOSVectorReq               : 1;
    unsigned int MaritimeCondControl        : 1;
    unsigned int PositionReq                : 1;
    unsigned int RateControl                : 1;
    unsigned int SensorControl              : 1;
    unsigned int ShortArtPartControl        : 1;
    unsigned int ShortComponentControl      : 1;
    unsigned int TerrestrialCondControl     : 1;
    unsigned int MotionTrackerControl       : 1;
    unsigned int TrajectoryDef              : 1;
    unsigned int ViewControl                : 1;
    unsigned int ViewDef                    : 1;
    unsigned int WaveControl                : 1;
    unsigned int WeatherControl             : 1;
    unsigned int SymbolSurfaceDef           : 1;
    unsigned int SymbolTextDef              : 1;
    unsigned int SymbolCircleDef            : 1;
    unsigned int SymbolLineDef              : 1;
    unsigned int SymbolClone                : 1;
    unsigned int SymbolControl              : 1;
    unsigned int ShortSymbolControl         : 1;
} g_FilterFlags;

///////////////////////////////////////////////////////////////////////////////
//
//  Function Prototypes
//
///////////////////////////////////////////////////////////////////////////////
void AFXAPI DDX_TextPrecision(CDataExchange *pDX, int nIDC, float &value, int precision);
void AFXAPI DDX_TextPrecision(CDataExchange *pDX, int nIDC, double &value, int precision);
BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
DLGTEMPLATE *WINAPI LockDlgRes(LPCSTR lpszResName);
void CalcNewAvgFrameRate(void);
int SpawnApp(LPCTSTR command);
void IPAddrToStr(const DWORD addr, char *str);
DWORD GetIPAddr(void);
void SetIPAddr(const DWORD addr);
BOOL StoreIPAddr(const DWORD addr);
BOOL RetrieveIPAddr(DWORD *addr);
unsigned short GetRemotePort(void);
void SetRemotePort(const unsigned short port);
BOOL StoreRemotePort(const unsigned short port);
BOOL RetrieveRemotePort(unsigned short *port);
unsigned short GetLocalPort(void);
void SetLocalPort(const unsigned short port);
BOOL StoreLocalPort(const unsigned short port);
BOOL RetrieveLocalPort(unsigned short *port);
int GetDefaultDB(void);
void SetDefaultDB(const int dbnum);
BOOL StoreDefaultDB(const int dbnum);
BOOL RetrieveDefaultDB(int *dbnum);
BOOL GetBigEndian(void);
void SetBigEndian(const BOOL big_endian);
BOOL StoreBigEndian(const int byteorder);
BOOL RetrieveBigEndian(int *byteorder);
int GetCigiMinorVersion(void);
void SetCigiMinorVersion(const int version);
BOOL StoreCigiMinorVersion(const int version);
BOOL RetrieveCigiMinorVersion(int *version);
CigiProtocolVersion GetCigiProtocolVersion(void);
void SetCigiProtocolVersion(const CigiProtocolVersion &version);
BOOL StoreCigiProtocolVersion(const CigiProtocolVersion &version);
BOOL RetrieveCigiProtocolVersion(CigiProtocolVersion *version);
BOOL GetPublishSelectedEntityViewControl(void);
void SetPublishSelectedEntityViewControl(BOOL enabled);
unsigned short GetSelectedEntityViewId(void);
void SetSelectedEntityViewId(unsigned short viewId);
void NotifySelectedEntityChanged(int entityId);
void FireMissile(CEntity *missile);
int PlayTerminatingLaunchAnims(CEntity *missile);
int PlayPersistentLaunchAnims(CEntity *missile);
int PlayDetonationAnims(CEntity *missile);
int StopTerminatingLaunchAnims(CEntity *missile);
int StopPersistentLaunchAnims(CEntity *missile);
int StopDetonationAnims(CEntity *missile);
int HideTerminatingLaunchAnims(CEntity *missile);
int HidePersistentLaunchAnims(CEntity *missile);
int HideDetonationAnims(CEntity *missile);
void SetBigEndian(const BOOL big_endian);
HEMU_MESSAGE *GetDriverMsg(void);
HEMU_MESSAGE *GetGUIMsg(void);
void PrintMessageText(LPCTSTR text);
void ClearMessageText(void);

///////////////////////////////////////////////////////////////////////////////
//
//  Inline Functions
//
///////////////////////////////////////////////////////////////////////////////

// Macros and inlines for tracing during debugging.
void ALLOC_CONSOLE(short cols, short rows);
void FREE_CONSOLE(void);
void DEBUG_PRINT(LPCTSTR buffer);

// Set/Get the values of the simulation state flags.
void SetFreezeFlag(BOOL freeze);
BOOL GetFreezeFlag(void);
void SetResetHostFlag(BOOL reset);
BOOL GetResetHostFlag(void);
void SetResetIGFlag(BOOL freeze);
BOOL GetResetIGFlag(void);
void SetDebugIGFlag(BOOL freeze);
BOOL GetDebugIGFlag(void);
void SetPacketWatchFlag(BOOL capturing);
BOOL GetPacketWatchFlag(void);
DWORD MAKE_TREE_DATA(const unsigned int id,
                     const unsigned int type,
                     const unsigned int subtype);
unsigned int MAKE_ID(const DWORD value);
unsigned int MAKE_TYPE(const DWORD value);
unsigned int MAKE_SUBTYPE(const DWORD value);
BOOL StrToFloat(float *f, LPCTSTR str);
BOOL StrToDouble(double *f, LPCTSTR str);
BOOL StrToInt(int *i, LPCTSTR str);
BOOL StrToULong(unsigned long *ul, LPCTSTR str);
BOOL StrToInt64(int *i, LPCTSTR str);
BOOL StrToUInt64(unsigned long *ui, LPCTSTR str);
BOOL StrToUChar(unsigned char *u, LPCTSTR str);
CHemuDoc *GetDocument(void);
void PostDriverMsg(HEMU_MESSAGE &msg);
void PostGUIMsg(HEMU_MESSAGE &msg);
void QueueCigiMessage(const void *msg);
void SendImmedCigiMessage(const void *msg);
TEMPL_ENTITY *GetEntityTempl(const int type);
int GetNextEntityID(void);
void MarkEntityID(const int id);
void UnmarkEntityID(const int id);

uint32_t MakeKey(uint16_t componentID, uint16_t instanceID);

#endif  // _HEMU3_GLOBALS_
