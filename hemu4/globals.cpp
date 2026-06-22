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
 *  FILENAME:   Globals.cpp
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

#include "stdafx.h"
#include "cigi4types.h"
#include "CigiView.h"
#include "Entity.h"
#include "EntityStateView.h"
#include "EntityTreeView.h"
#include "globals.h"
#include "Hemu4.h"
#include "hemumsg.h"
#include "MessageView.h"
#include "regutils.h"
#include "SituationDispCtrl.h"
#include "ViewGroup.h"

// Global scope (declared in globals.h)

COLORREF EntityClassColor[13] = {   RGB(255, 255, 255),     // GENERIC
                                    RGB(191, 191, 255),     // FIXEDWING
                                    RGB(255, 115, 235),     // ANIMATION
                                    RGB(127, 127, 127),     // CLOUD
                                    RGB(255, 154, 25),      // MISSILE
                                    RGB(127, 191, 127),     // TANK
                                    RGB(127, 191, 127),     // CAR
                                    RGB(127, 191, 255),     // BOAT
                                    RGB(191, 191, 255),     // ROTORCRAFT
                                    RGB(255, 154, 25),      // BOMB
                                    RGB(127, 191, 127),     // SAM
                                    RGB(127, 127, 127),     // PATTERN
                                    RGB(255, 255, 255)
                                };  // CULTURE

extern int g_HostSession = -1;
extern int g_IGSession = -1;
CigiProtocolVersion g_CigiProtocolVersion = CigiProtocolVersion::Current();

// Because we are dequeueing a bunch of Host-to-IG packets and then
// IG-to-Host messages, we need to keep track of the Frame Counter
// we used to send the IG-to-Host message.
volatile unsigned int   g_WatchFrameCounter = (unsigned long) - 1;

//volatile unsigned int g_TestScriptState = SCRIPT_STATE_STOP;
volatile unsigned int   g_TestScriptTranspDelayRunning;
volatile int            g_NestedScriptCount = 0;
volatile int            g_FrameRate = 0;
volatile int            g_AvgFrameRate = 0;
volatile unsigned int   g_FrameCounter = 0;
volatile BOOL           g_bFreezeFlag = TRUE;
volatile BOOL           g_bResetHostFlag = FALSE;
volatile BOOL           g_bResetIGFlag = FALSE;
volatile BOOL           g_bDebugIGFlag = FALSE;
volatile BOOL           g_bWatchPackets = FALSE;
volatile BOOL           g_bRecPlaybackLoop = FALSE;
volatile int            g_RecPlaybackState = 0;
volatile int            g_ScriptState = 0;
volatile BOOL           g_ScriptParsePause = FALSE;
unsigned int            g_ScriptThreadAddrHdl = 0;

// Flag used to force driver to Big-Endian byte order.
volatile BOOL           g_bBigEndian;

HTREEITEM               g_htreeSky = NULL;
HTREEITEM               g_htreeTerrain = NULL;
HTREEITEM               g_htreeRegions = NULL;
CImageList              g_ImgListTypes;
CImageList              g_ImgListIcons;
CDataManager            g_DataManager;
IDirectInput8          *g_pIDirectInput = NULL;
IDirectInputDevice8    *g_pJoystick = NULL;
struct _FILTER_FLAGS    g_FilterFlags = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1
};

// File scope
DWORD s_IPAddr = 0;
unsigned short s_RemotePort = 0;
unsigned short s_LocalPort = 0;
int s_DefaultDB = 1;

// Externals
extern CHemuApp theApp;

// Static class members
unsigned int CSharedEntityObj::m_InstanceCounter = 0;
unsigned int CSharedViewObj::m_InstanceCounter = 0;
unsigned int CSharedViewGroupObj::m_InstanceCounter = 0;

// Dialog Data Exchange routines
AFX_STATIC BOOL CustomParseDouble(LPCTSTR buffer, double &value)
{
    BOOL negate = FALSE;
    double temp = 0.0;

    ASSERT(buffer != NULL);

    // Trim any leading whitespaces.
    while ((*buffer == ' ') || (*buffer == '\t'))
        buffer++;

    // Check for a negative number.
    if (*buffer == '-') {
        negate = TRUE;
        buffer++;
    } else if (*buffer == '\0') { // Check for blank field.
        // Don't change value and don't return an error.

        return TRUE;
    }

    char first = buffer[0];
    temp = _tcstod(buffer, (LPTSTR *)&buffer);      // (start, *end)
    if ((temp == 0.0) && (first != '0') && (first != '.') && (!negate))
        return FALSE;

    if (negate)
        value = -temp;
    else
        value = temp;

    return TRUE;
}

void AFXAPI DDX_TextPrecision(CDataExchange *pDX, int nIDC, float &value, int precision)
{
    double d = (double)value;
    DDX_TextPrecision(pDX, nIDC, d, precision);
    value = (float)d;
}

void AFXAPI DDX_TextPrecision(CDataExchange *pDX, int nIDC, double &value, int precision)
{
    char buff[32] = {'\0'};
    HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);

    if (pDX->m_bSaveAndValidate) {
        // Convert from control text to a double.
        ::GetWindowText(hWndCtrl, buff, sizeof(buff));

        double d;
        if (CustomParseDouble(buff, d)) {
            *((double *)&value) = d;
        } else {
            // We don't ever want to pop up a dialog because this puts us in
            // an infinite loop. Instead, put a message in the message window.
            PrintMessageText("Invalid entry. Please enter a valid number.");
        }
    } else {
        // Convert from double to text.
        CString format;
        format.Format("%%.%dlf", precision);

        CString text;
        text.Format((LPCTSTR)format, value);

        ::SetWindowText(hWndCtrl, (LPCTSTR)text);
    }
}

// Callback for enumerating joystick devices.
BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
    HRESULT hr;

    // Create a joystick interface for the first joystick device.
    hr = g_pIDirectInput->CreateDevice(lpddi->guidInstance, &g_pJoystick, NULL);

    if (FAILED(hr))
        return DIENUM_CONTINUE;

    g_pJoystick->SetDataFormat(&c_dfDIJoystick2);

    DIPROPDWORD prop;
    prop.diph.dwSize = sizeof(DIPROPDWORD);
    prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    prop.diph.dwObj = 0;
    prop.diph.dwHow = DIPH_DEVICE;

    // Set the buffer size.
    prop.dwData = 0;    // 0 = Not buffered -- was 25;
    g_pJoystick->SetProperty(DIPROP_BUFFERSIZE, &prop.diph);

    // Specify auto-centering axes.
    prop.dwData = DIPROPAUTOCENTER_ON;
    g_pJoystick->SetProperty(DIPROP_AUTOCENTER, &prop.diph);

    // Specify absolute axis mode.
    prop.dwData = DIPROPAXISMODE_ABS;
    g_pJoystick->SetProperty(DIPROP_AXISMODE, &prop.diph);

    // Set the dead zone.  This value was determined by trial and error using the
    // Microsoft SideWinder.  Later, we might want to make this user-configurable.
    prop.dwData = 1000;
    g_pJoystick->SetProperty(DIPROP_DEADZONE, &prop.diph);

    return DIENUM_STOP;
}

DLGTEMPLATE *WINAPI LockDlgRes(LPCSTR lpszResName)
{
    HRSRC hrsrc = FindResource(NULL, lpszResName, RT_DIALOG);
    HGLOBAL hglb = LoadResource(NULL, hrsrc);
    return (DLGTEMPLATE *) LockResource(hglb);
}

void CalcNewAvgFrameRate(void)
{
    static int history[7] = {0};    // allocate extra so we don't overflow during shift
    int total = 0;
    int count = 0;
    int i;

    if (g_FrameRate == 0) {
        g_AvgFrameRate = 0;
        return;
    }

    // Count (from the beginning) the number of non-zero items.
    for (i = 0; i < 6; i++) {
        if (history[i] != 0)
            count++;
        else
            break;  // Break as soon as we have a non-valid rate.
    }

    // Shift everything up.
    for (i = count - 1; i >= 0; i--) {
        history[i + 1] = history[i];
        total += history[i];
    }

    history[0] = g_FrameRate;
    total += g_FrameRate;
    count++;    // Add one for the new frame rate.

    g_AvgFrameRate = (int)(total / (float)count);
}

HEMU_MESSAGE *GetDriverMsg(void)
{
    static char buffer[MESSAGE_MAX_SIZE];
    int size = g_GuiToDrvMsgQueue.Pop(buffer, MESSAGE_MAX_SIZE);
    if (size > 0) {
        return (HEMU_MESSAGE *)buffer;
    } else
        return NULL;
}

HEMU_MESSAGE *GetGUIMsg(void)
{
    static char buffer[MESSAGE_MAX_SIZE];
    int size = g_DrvToGuiMsgQueue.Pop(buffer, MESSAGE_MAX_SIZE);
    if (size > 0) {
        return (HEMU_MESSAGE *)buffer;
    } else
        return NULL;
}

void IPAddrToStr(const DWORD addr, char *str)
{
    unsigned char *bytes = (unsigned char *)&addr;
    sprintf(str, "%d.%d.%d.%d", bytes[3], bytes[2], bytes[1], bytes[0]);
}

DWORD GetIPAddr(void)
{
    return s_IPAddr;
}

void SetIPAddr(const DWORD addr)
{
    s_IPAddr = addr;
}

BOOL StoreIPAddr(const DWORD addr)
{
    unsigned char *bytes = (unsigned char *)&addr;
    CString str;
    str.Format("%d.%d.%d.%d", bytes[3], bytes[2], bytes[1], bytes[0]);

    if (SetRegEntryStr("CIGI Host Emulator 4", "Remote IP Addr", str) == ERROR_SUCCESS)
        return TRUE;
    else
        return FALSE;
}

BOOL RetrieveIPAddr(DWORD *addr)
{
    CString str;
    if (GetRegEntryStr("CIGI Host Emulator 4", "Remote IP Addr", &str) == ERROR_SUCCESS) {
        unsigned char *bytes = (unsigned char *)addr;
        unsigned int b0, b1, b2, b3;

        sscanf((LPCTSTR)str, "%u.%u.%u.%u", &b3, &b2, &b1, &b0);
        bytes[0] = (unsigned char)b0;
        bytes[1] = (unsigned char)b1;
        bytes[2] = (unsigned char)b2;
        bytes[3] = (unsigned char)b3;
        return TRUE;
    } else
        return FALSE;
}

unsigned short GetRemotePort(void)
{
    return s_RemotePort;
}

void SetRemotePort(const unsigned short port)
{
    s_RemotePort = port;
}

BOOL StoreRemotePort(const unsigned short port)
{
    CDebugTrace trace("::StoreRemotePort(const unsigned short)");

    if (SetRegEntryInt32("CIGI Host Emulator 4", "Remote Port", (__int32)port) == ERROR_SUCCESS)
        return TRUE;
    else
        return FALSE;
}

BOOL RetrieveRemotePort(unsigned short *port)
{
    CDebugTrace trace("::RetrieveRemotePort(unsigned short *)");

    __int32 p;
    if (GetRegEntryInt32("CIGI Host Emulator 4", "Remote Port", &p) == ERROR_SUCCESS) {
        *port = (unsigned short)p;
        return TRUE;
    } else
        return FALSE;
}

unsigned short GetLocalPort(void)
{
    return s_LocalPort;
}

void SetLocalPort(const unsigned short port)
{
    s_LocalPort = port;
}

BOOL StoreLocalPort(const unsigned short port)
{
    CDebugTrace trace("::StoreLocalPort(const unsigned short)");

    if (SetRegEntryInt32("CIGI Host Emulator 4", "Local Port", (__int32)port) == ERROR_SUCCESS)
        return TRUE;
    else
        return FALSE;
}

BOOL RetrieveLocalPort(unsigned short *port)
{
    CDebugTrace trace("::RetrieveLocalPort(unsigned short *)");

    __int32 p;
    if (GetRegEntryInt32("CIGI Host Emulator 4", "Local Port", &p) == ERROR_SUCCESS) {
        *port = (unsigned short)p;
        return TRUE;
    } else
        return FALSE;
}

int GetDefaultDB(void)
{
    return s_DefaultDB;
}

void SetDefaultDB(const int dbnum)
{
    s_DefaultDB = dbnum;
}

BOOL StoreDefaultDB(const int dbnum)
{
    CDebugTrace trace("::StoreDefaultDB(const int)");

    if (SetRegEntryInt32("CIGI Host Emulator 4", "Default Database", (__int32)dbnum) == ERROR_SUCCESS)
        return TRUE;
    else
        return FALSE;
}

BOOL RetrieveDefaultDB(int *dbnum)
{
    CDebugTrace trace("::RetrieveDefaultDB(int *)");

    __int32 db;
    if (GetRegEntryInt32("CIGI Host Emulator 4", "Default Database", &db) == ERROR_SUCCESS) {
        *dbnum = (int)db;
        return TRUE;
    } else
        return FALSE;
}

BOOL GetBigEndian(void)
{
    return g_bBigEndian;
}

void SetBigEndian(const BOOL big_endian)
{
    g_bBigEndian = big_endian;
}

BOOL StoreBigEndian(const int byteorder)
{
    CDebugTrace trace("::StoreBigEndian(const int)");

    if (SetRegEntryInt32("CIGI Host Emulator 4", "Big Endian", byteorder ? 1 : 0) == ERROR_SUCCESS)
        return TRUE;
    else
        return FALSE;
}

BOOL RetrieveBigEndian(int *byteorder)
{
    CDebugTrace trace("::RetrieveBigEndian(int *)");

    __int32 order;
    if (GetRegEntryInt32("CIGI Host Emulator 4", "Big Endian", &order) == ERROR_SUCCESS) {
        *byteorder = (int)order;
        return TRUE;
    } else
        return FALSE;
}

int GetCigiMinorVersion(void)
{
    // Legacy packet-building code asks for only a minor version. Keep those
    // CIGI 4 packet paths on the safe active wire version when the selected
    // CIGI 3 adapter is still a stub.
    if (!g_CigiProtocolVersion.IsPacketIoImplemented())
        return CigiProtocolVersion::Current().GetMinorVersion();

    return g_CigiProtocolVersion.GetMinorVersion();
}

void SetCigiMinorVersion(const int version)
{
    CigiProtocolVersion protocolVersion;
    if (CigiProtocolVersion::TryCreate(
            g_CigiProtocolVersion.GetMajorVersion(), version, &protocolVersion))
        g_CigiProtocolVersion = protocolVersion;
}

BOOL StoreCigiMinorVersion(const int version)
{
    CDebugTrace trace("::StoreCigiMinorVersion(const int)");

    if (SetRegEntryInt32("CIGI Host Emulator 4", "CIGI Minor Version", (__int32)version) == ERROR_SUCCESS)
        return TRUE;
    else
        return FALSE;
}

BOOL RetrieveCigiMinorVersion(int *version)
{
    CDebugTrace trace("::RetrieveCigiMinorVersion(int *)");

    __int32 ver;
    if (GetRegEntryInt32("CIGI Host Emulator 4", "CIGI Minor Version", &ver) == ERROR_SUCCESS) {
        *version = (int)ver;
        return TRUE;
    } else
        return FALSE;
}

CigiProtocolVersion GetCigiProtocolVersion(void)
{
    return g_CigiProtocolVersion;
}

void SetCigiProtocolVersion(const CigiProtocolVersion &version)
{
    g_CigiProtocolVersion = version;
}

BOOL StoreCigiProtocolVersion(const CigiProtocolVersion &version)
{
    CDebugTrace trace("::StoreCigiProtocolVersion(const CigiProtocolVersion &)");

    if (SetRegEntryInt32("CIGI Host Emulator 4", "CIGI Major Version",
                        (__int32)version.GetMajorVersion()) != ERROR_SUCCESS)
        return FALSE;

    if (SetRegEntryInt32("CIGI Host Emulator 4", "CIGI Protocol Minor Version",
                        (__int32)version.GetMinorVersion()) != ERROR_SUCCESS)
        return FALSE;

    // Preserve the old key as the active CIGI 4 wire minor. An older build
    // therefore retains known-good behavior after a CIGI 3 selection.
    const int legacyMinorVersion = version.IsPacketIoImplemented()
        ? version.GetMinorVersion()
        : CigiProtocolVersion::Current().GetMinorVersion();
    return StoreCigiMinorVersion(legacyMinorVersion);
}

BOOL RetrieveCigiProtocolVersion(CigiProtocolVersion *version)
{
    CDebugTrace trace("::RetrieveCigiProtocolVersion(CigiProtocolVersion *)");

    if (!version)
        return FALSE;

    __int32 majorVersion;
    __int32 protocolMinorVersion;
    if (GetRegEntryInt32("CIGI Host Emulator 4", "CIGI Major Version",
                        &majorVersion) != ERROR_SUCCESS ||
        GetRegEntryInt32("CIGI Host Emulator 4", "CIGI Protocol Minor Version",
                        &protocolMinorVersion) != ERROR_SUCCESS) {
        // Legacy settings only stored a minor version and always used the
        // existing CIGI 4 implementation.
        int legacyMinorVersion;
        if (!RetrieveCigiMinorVersion(&legacyMinorVersion))
            return FALSE;
        majorVersion = 4;
        protocolMinorVersion = legacyMinorVersion;
    }

    return CigiProtocolVersion::TryCreate((int)majorVersion,
                                          (int)protocolMinorVersion,
                                          version) ? TRUE : FALSE;
}

void FireMissile(CEntity *missile)
{
    CDebugTrace trace("::FireMissile(CEntity *)");

    if (!missile)
        return;

    ENTITY missile_state = missile->GetAndLock();

    CEntity *parent = g_DataManager.GetEntity(missile_state.cigi.parent_id);
    if (!parent) {
        missile->Unlock();
        return;
    }

    missile_state.cigi.parent_id = -1;

    DOF parent_pos = parent->GetDofs();
    double parent_x = 0.0;
    double parent_y = 0.0;
    GDCtoTM(parent_pos.latitude, parent_pos.longitude, &parent_y, &parent_x);

    // Convert the weapon's relative position to UTM.
    double missile_x = parent_x + missile_state.cigi.dofs.offset_x;
    double missile_y = parent_y + missile_state.cigi.dofs.offset_y;
    TMtoGDC(missile_y, missile_x,
            &missile_state.cigi.dofs.latitude, &missile_state.cigi.dofs.longitude);
    missile_state.cigi.dofs.altitude = parent_pos.altitude
                                       + missile_state.cigi.dofs.offset_z;

    // Convert the weapon's relative orientation.
    missile_state.cigi.dofs.yaw = parent_pos.yaw + missile_state.cigi.dofs.rel_yaw;
    missile_state.cigi.dofs.pitch = parent_pos.pitch + missile_state.cigi.dofs.rel_pitch;
    missile_state.cigi.dofs.roll = parent_pos.roll + missile_state.cigi.dofs.rel_roll;

    missile_state.unsaved.track_target = 1;
    missile_state.unsaved.ramp_up_ctr = 0;

    // Set the missile's initial and final velocity.  The final velocity
    // is specified by the airspeed field in the GUI.
    missile_state.unsaved.speed_final = missile_state.noncigi.speed;
    missile_state.noncigi.speed = parent->GetSpeed();

    missile->SetLocked(missile_state);
    missile->Unlock();

    // Play the launch animations.
    PlayPersistentLaunchAnims(missile);
    PlayTerminatingLaunchAnims(missile);

    // Repopulate the reference entity list.  Do this after we have unlocked
    // because this calls GetParent(), which calls GetAndLock().
    theApp.GetMainFrame().GetEntityStateView().PopulateRefEntityList();

    // Update the tree view.
    theApp.GetMainFrame().GetEntityTreeView().MoveEntityToParent(missile, -1);

    // Show the missile in the situtational display control.
    theApp.GetMainFrame().GetSituationCtrl().SetEntityVisible(
        missile->GetSituationHandle(), TRUE);
}

int PlayPersistentLaunchAnims(CEntity *missile)
{
    CDebugTrace trace("::PlayPersistentLaunchAnims(CEntity *)");

    if (!missile)
        return -1;

    int count = 0;
    long id = -1;
    CEntity *anim = NULL;
    CLongList &list = missile->GetPersistentLaunchAnimList();

    // Get the first child of the entity.
    POSITION pos = list.GetHeadPosition();

    // Play each child effect.
    while (pos) {
        id = list.GetNext(pos);
        anim = g_DataManager.GetEntity(id);

        if (anim->GetClass() == ENTITY_CLASS_ANIMATION) {
            anim->SetActiveState(1);
            anim->SetAnimState(ENTITY_ANIM_STATE_PLAY);
        }
    }

    return count;
}

int PlayTerminatingLaunchAnims(CEntity *missile)
{
    CDebugTrace trace("::PlayTerminatingLaunchAnims(CEntity *)");

    if (!missile)
        return -1;

    int count = 0;
    long id = -1;
    CEntity *anim = NULL;
    CLongList &list = missile->GetLaunchAnimList();

    // Get the first child of the entity.
    POSITION pos = list.GetHeadPosition();

    // Play each child effect.
    while (pos) {
        id = list.GetNext(pos);
        anim = g_DataManager.GetEntity(id);

        if (anim->GetClass() == ENTITY_CLASS_ANIMATION) {
            anim->SetActiveState(1);
            anim->SetAnimState(ENTITY_ANIM_STATE_PLAY);
        }
    }

    return count;
}

int PlayDetonationAnims(CEntity *missile)
{
    CDebugTrace trace("::PlayDetonationAnims(CEntity *)");

    if (!missile)
        return -1;

    int count = 0;
    long id = -1;
    CEntity *anim = NULL;
    CLongList &list = missile->GetDetonationAnimList();

    // Get the first child of the entity.
    POSITION pos = list.GetHeadPosition();

    // Play each child effect.
    while (pos) {
        id = list.GetNext(pos);
        anim = g_DataManager.GetEntity(id);

        if (anim->GetClass() == ENTITY_CLASS_ANIMATION) {
            anim->SetActiveState(1);
            anim->SetAnimState(ENTITY_ANIM_STATE_PLAY);
        }
    }

    return count;
}

int StopPersistentLaunchAnims(CEntity *missile)
{
    CDebugTrace trace("::StopPersistentLaunchAnims(CEntity *)");

    if (!missile)
        return -1;

    int count = 0;
    long id = -1;
    CEntity *anim = NULL;
    CLongList &list = missile->GetPersistentLaunchAnimList();

    // Get the first child of the entity.
    POSITION pos = list.GetHeadPosition();

    // Play each child effect.
    while (pos) {
        id = list.GetNext(pos);
        anim = g_DataManager.GetEntity(id);

        if (anim->GetClass() == ENTITY_CLASS_ANIMATION) {
            // The only difference from terminating anims is that
            // we don't hide the entity here.
            anim->SetAnimState(ENTITY_ANIM_STATE_STOP);
        }
    }

    return count;
}

int StopTerminatingLaunchAnims(CEntity *missile)
{
    CDebugTrace trace("::StopTerminatingLaunchAnims(CEntity *)");

    if (!missile)
        return -1;

    int count = 0;
    long id = -1;
    CEntity *anim = NULL;
    CLongList &list = missile->GetLaunchAnimList();

    // Get the first child of the entity.
    POSITION pos = list.GetHeadPosition();

    // Play each child effect.
    while (pos) {
        id = list.GetNext(pos);
        anim = g_DataManager.GetEntity(id);

        if (anim->GetClass() == ENTITY_CLASS_ANIMATION) {
            anim->SetAnimState(ENTITY_ANIM_STATE_STOP);
            anim->SetActiveState(0);
        }
    }

    return count;
}

int StopDetonationAnims(CEntity *missile)
{
    CDebugTrace trace("::StopDetonationAnims(CEntity *)");

    if (!missile)
        return -1;

    int count = 0;
    long id = -1;
    CEntity *anim = NULL;
    CLongList &list = missile->GetDetonationAnimList();

    // Get the first child of the entity.
    POSITION pos = list.GetHeadPosition();

    // Play each child effect.
    while (pos) {
        id = list.GetNext(pos);
        anim = g_DataManager.GetEntity(id);

        if (anim->GetClass() == ENTITY_CLASS_ANIMATION) {
            anim->SetAnimState(ENTITY_ANIM_STATE_STOP);
            anim->SetActiveState(0);
        }
    }

    return count;
}

void PrintMessageText(LPCTSTR text)
{
    MESSAGE_DISPLAY_MSG_TEXT msg;
    strcpy(msg.text, text);
    PostGUIMsg(msg);
}

void ClearMessageText(void)
{
    if (theApp.GetMainFrame().GetSafeHwnd()) {
        CMessageView &msgview = theApp.GetMainFrame().GetMessageView();
        if (msgview.GetSafeHwnd())
            msgview.ClearMessageText();
    }
}

int SpawnApp(LPCTSTR command)
{
    CDebugTrace trace("::SpawnApp(LPCTSTR)");

    CString path;
    CString args;
    CString cmdstr(command);
    cmdstr.TrimLeft();
    cmdstr.TrimRight();
    int retval = 0;

    // Replace any tabs with spaces.
    cmdstr.Replace('\t', ' ');

    if (((LPCTSTR)cmdstr)[0] == '\"') {
        // We have to take the path and filename out of the quotes
        // and convert them to the short filename format.
        cmdstr.TrimLeft();
        cmdstr.TrimLeft('\"');

        int idx = cmdstr.Find('\"', 1);
        if (idx > 0) {
            char pathbuff[256] = {'\0'};

            path = cmdstr.Left(idx);
            if (GetShortPathName((LPCTSTR)path, pathbuff, 256) > 0)
                path = pathbuff;

            args = cmdstr.Right(cmdstr.GetLength() - idx - 1);
            args.TrimLeft();
        } else {
            idx = cmdstr.Find(' ');
            if (idx > 0) {
                path = cmdstr.Left(idx);
                args = cmdstr.Right(cmdstr.GetLength() - idx - 1);
                args.TrimLeft();
            } else {
                path = cmdstr;
                args = "";
            }
        }
    } else {
        char pathbuff[256] = {'\0'};
        int idx = cmdstr.Find(' ');
        if (idx > 0) {
            path = cmdstr.Left(idx);
            if (GetShortPathName((LPCTSTR)path, pathbuff, 256) > 0)
                path = pathbuff;

            args = cmdstr.Right(cmdstr.GetLength() - idx - 1);
            args.TrimLeft();
        } else
            path = cmdstr;
    }

    if (args == "")
        retval = _spawnlp(_P_NOWAIT, (LPCTSTR)path, (LPCTSTR)path, NULL);
    else
        retval = _spawnlp(_P_NOWAIT, (LPCTSTR)path, (LPCTSTR)path, (LPCTSTR)args, NULL);

    return retval;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Inline Functions
//
///////////////////////////////////////////////////////////////////////////////

// Macros and inlines for tracing during debugging.
void ALLOC_CONSOLE(short cols = 80, short rows = 1024)
{
#ifdef _USE_DEBUGTRACE
    COORD size = {cols, rows};
    AllocConsole();
    SetConsoleTitle("Hemu4 Debug Output");
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), size);
#else
    return;
#endif
}

void FREE_CONSOLE(void)
{
#ifdef _USE_DEBUGTRACE
    FreeConsole();
#else
    return;
#endif
}

void DEBUG_PRINT(LPCTSTR buffer)
{
#ifdef _USE_DEBUGTRACE
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), buffer, strlen(buffer),
                 NULL, NULL);
#else
    return;
#endif
}

// Set/Get the values of the simulation state flags.
void SetFreezeFlag(BOOL freeze)
{
    g_bFreezeFlag = freeze;
}

BOOL GetFreezeFlag(void)
{
    return g_bFreezeFlag;
}

void SetResetHostFlag(BOOL reset)
{
    g_bResetHostFlag = reset;
}

BOOL GetResetHostFlag(void)
{
    return g_bResetHostFlag;
}

void SetResetIGFlag(BOOL freeze)
{
    g_bResetIGFlag = freeze;
}

BOOL GetResetIGFlag(void)
{
    return g_bResetIGFlag;
}

void SetDebugIGFlag(BOOL freeze)
{
    g_bDebugIGFlag = freeze;
}

BOOL GetDebugIGFlag(void)
{
    return g_bDebugIGFlag;
}

void SetPacketWatchFlag(BOOL capturing)
{
    g_bWatchPackets = capturing;
}

BOOL GetPacketWatchFlag(void)
{
    return g_bWatchPackets;
}

// This function combines an entity type and an entity class into a single DWORD.
// This is useful for stuffing both of these values into the ItemData for a
// control.
DWORD MAKE_TREE_DATA(const unsigned int id,
                     const unsigned int type,
                     const unsigned int subtype)
{
    return MAKELONG((WORD)id, MAKE_WORD(type, subtype));
}

// This function returns the entity type from a DWORD.
unsigned int MAKE_ID(const DWORD value)
{
    return LOWORD(value);
}

// This function returns the entity class from a DWORD.
unsigned int MAKE_TYPE(const DWORD value)
{
    return LOBYTE(HIWORD(value));
}

// This function returns the entity subclass from a DWORD.
unsigned int MAKE_SUBTYPE(const DWORD value)
{
    return HIBYTE(HIWORD(value));
}

// Convert a string to a number
BOOL StrToFloat(float *f, LPCTSTR str)
{
    CString temp(str);
    if (temp.FindOneOf("1234567890") == -1 || temp.FindOneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`~[]{}_=+\\|'/?>,<") != -1) {
        return FALSE;
    }

    *f = (float)atof(str);
    return TRUE;
}

BOOL StrToDouble(double *f, LPCTSTR str)
{
    CString temp(str);
    if (temp.FindOneOf("1234567890") == -1 || temp.FindOneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`~[]{}_=+\\|'/?>,<") != -1) {
        return FALSE;
    }

    *f = atof(str);
    return TRUE;
}

BOOL StrToInt(int *i, LPCTSTR str)
{
    CString temp(str);
    if (temp.FindOneOf("1234567890") == -1 || temp.FindOneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`~[]{}_=+\\|'/?>,<") != -1) {
        return FALSE;
    }

    *i = atoi(str);
    return TRUE;
}

BOOL StrToULong(unsigned long *ul, LPCTSTR str)
{
    CString temp(str);
    if (temp.FindOneOf("1234567890") == -1 || temp.FindOneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`~[]{}-_=+\\|'/?>,<") != -1) {
        return FALSE;
    }

    *ul = (unsigned long)_atoi64(str);
    return TRUE;
}

BOOL StrToInt64(int *i, LPCTSTR str)
{
    CString temp(str);
    if (temp.FindOneOf("1234567890") == -1 || temp.FindOneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`~[]{}_=+\\|'/?>,<") != -1) {
        return FALSE;
    }

    *i = (int)(_atoi64)(str);
    return TRUE;
}

BOOL StrToUInt64(unsigned long *ui, LPCTSTR str)
{
    CString temp(str);
    if (temp.FindOneOf("1234567890") == -1 || temp.FindOneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`~[]{}-_=+\\|'/?>,<") != -1) {
        return FALSE;
    }

    *ui = (unsigned long)_atoi64(str);
    return TRUE;
}

BOOL StrToUChar(unsigned char *u, LPCTSTR str)
{
    CString temp(str);
    if (temp.FindOneOf("1234567890") == -1 || temp.FindOneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`~[]{}-_=+\\|'/?>,<") != -1) {
        return FALSE;
    }

    *u = (unsigned char)atoi(str);
    return TRUE;
}

// Return a pointer to the current document
CHemuDoc *GetDocument(void)
{
    return g_DataManager.GetDocument();
}

void PostDriverMsg(HEMU_MESSAGE &msg)
{
    CDebugTrace trace("::PostDriverMsg(HEMU_MESSAGE &)");

    while (g_GuiToDrvMsgQueue.Push((char *)&msg, msg.size) == -1)
        continue;
}

void PostGUIMsg(HEMU_MESSAGE &msg)
{
    CDebugTrace trace("::PostGUIMsg(HEMU_MESSAGE &)");

    while (g_DrvToGuiMsgQueue.Push((char *)&msg, msg.size) == -1)
        continue;
}

void QueueCigiMessage(const void *msg)
{
    CDebugTrace trace("::QueueCigiMessage(const void *)");

    while (g_SendCIGIMsgQueue.Push((char *)msg, ((CIGI_DUMMY_PACKET *)msg)->packet_size) == -1)
        continue;
}

void SendImmedCigiMessage(const void *msg)
{
    CDebugTrace trace("::SendImmedCigiMessage(const void *)");

    while (g_SendImmedCIGIMsgQueue.Push((char *)msg, ((CIGI_DUMMY_PACKET *)msg)->packet_size) == -1)
        continue;
}

TEMPL_ENTITY *GetEntityTempl(const int type)
{
    return g_DataManager.GetEntityTempl(type);
}

int GetNextEntityID(void)
{
    return g_DataManager.GetNextEntityID();
}

void MarkEntityID(const int id)
{
    g_DataManager.MarkEntityID(id);
}

void UnmarkEntityID(const int id)
{
    g_DataManager.UnmarkEntityID(id);
}

// Creates a 32 bit word by appending two 16 bit words.
uint32_t MakeKey(uint16_t componentID, uint16_t instanceID)
{
    uint32_t key = 0;       // 0000 0000 0000 0000 0000 0000 0000 0000

    key = componentID;      // 0000 0000 0000 0000 CCCC CCCC CCCC CCCC (C = componentID bits)
    key = key << 16;        // CCCC CCCC CCCC CCCC 0000 0000 0000 0000
    key = key + instanceID; // CCCC CCCC CCCC CCCC IIII IIII IIII IIII (I = instanceID bits)

    return key;
}
