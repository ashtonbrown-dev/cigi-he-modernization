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
#include "coordcnv.h"
#include "cigi4.h"
#include "CigiProtocolAdapter.h"
#include "cigi4types.h"
#include "SharedBufferQueue.h"
#include "SharedEntityObj.h"
#include "SharedViewObj.h"
#include "SharedViewGroupObj.h"
#include "LinkedList.h"
#include "simpleq.h"
#include "hemumsg.h"
#include "Fly.h"
#include "comm.h"
#include "WindowsCompatibility.h"

// Priority level codes
#define PRIORITY_HIGHEST        THREAD_PRIORITY_ABOVE_NORMAL
#define PRIORITY_HIGHER         THREAD_PRIORITY_NORMAL
#define PRIORITY_HIGH           THREAD_PRIORITY_NORMAL
#define PRIORITY_NORMAL         THREAD_PRIORITY_NORMAL
#define PRIORITY_LOW            THREAD_PRIORITY_BELOW_NORMAL


// Operate modes
#define MODE_NORMAL             0
#define MODE_PLAYBACK           1
#define MODE_TRANSPDELAYTEST    2


#ifdef WITH_HUD
typedef struct {
    double lat;
    double lon;
    float airspeed;
    float alt;
    float hdg;
    float pitch;
    float roll;
    float aoa;
    float Gs;
    long enable;
} HUD_DATA;

HUD_DATA hud_data;
SOCKADDR_IN haddr;
SOCKET hudsock;

#endif

// Function prototypes
void SendGuiMessage(HEMU_MESSAGE *message);
HEMU_MESSAGE *RcvGuiMessage(void);
int InitializeShutdown(void);
int InitializeSendRcv(void);
int InitializeSockets(MESSAGE_SET_ADDR *MsgSetAddr);
ULONG WINAPI WaitForWin32ProcToDie(void *nContext);
ULONG WINAPI SendRcvThread(void *nContext);
ULONG WINAPI ShutdownThread(void *nContext);
ULONG WINAPI RecordThread(void *nContext);
ULONG WINAPI PlaybackThread(void *nContext);
int StartRecord(char *filename);
void StopRecord(void);
int StartPlayback(char *filename);
void StopPlayback(void);
void BufferNextRecordMessage(unsigned char *buffer, int size);
void GetNextPlaybackMessage(unsigned char *buffer, int *size);
int Startup(void);
void CheckMessages(void);
void ChangeMapAndTransportAllEntities(double new_lat, double new_lon, double new_radius, double new_flattening);
void BuildEntityControlPackets(void);
void BuildViewPackets(void);
void UpdateEntities(double dt);
void ClearEntities(void);
void ClearViews(void);
void ClearViewGroups(void);
CSharedEntityObj *FindEntity(const int id, unsigned long *handle);
CSharedViewObj *FindView(const int id, unsigned long *handle);
CSharedViewGroupObj *FindViewGroup(const int id, unsigned long *handle);
int DeleteEntity(int id);
int DeleteView(int id);
int DeleteViewGroup(int id);
void AddHAT(MESSAGE_ADD_HAT *msg);
unsigned long FindHAT(const int id);
void AddHATPackets(void);
void AddLOS(MESSAGE_ADD_LOS *msg);
unsigned long FindLOS(const int id);
void AddLOSPackets(void);
void AddImmediateQueuedPackets(void);
void AddQueuedPackets(void);
void InitializeCigiHostCallbacks(CigiHostCallbacks *callbacks);

// For transport delay test:
void StartTranspDelayTest(MESSAGE_TDTEST_START *msg);
void StopTranspDelayTest(void);
CIGI_ENTITY_CONTROL *TranspDelayTestCreateECPacket(int posnum);

// CIGI Callback Functions
long DoSOF(const int sessionid, void *packet);
long DoHatHotResponse(const int sessionid, void *packet);
long DoHatHotExtResponse(const int sessionid, void *packet);
long DoLOSResponse(const int sessionid, void *packet);
long DoLOSExtResponse(const int sessionid, void *packet);
long DoSensorResponse(const int sessionid, void *packet);
long DoSensorExtResponse(const int sessionid, void *packet);
long DoPositionResponse(const int sessionid, void *packet);
long DoWeatherResponse(const int sessionid, void *packet);
long DoAerosolResponse(const int sessionid, void *packet);
long DoMaritimeResponse(const int sessionid, void *packet);
long DoTerrestrialResponse(const int sessionid, void *packet);
long DoCollisionSegNotification(const int sessionid, void *packet);
long DoCollisionVolNotification(const int sessionid, void *packet);
long DoAnimationStopNotification(const int sessionid, void *packet);
long DoEventNotification(const int sessionid, void *packet);
long DoIGResponse(const int sessionid, void *packet);

// Globals
char UTMZone[8] = {0};
int session;
int PreDbLoadRunState = 0;
int RunState = 0;
int OperateMode = MODE_NORMAL;
int verbose = 0;
int mp = 0;
int hud_enable = 1;
unsigned int LastReceivedFrameCtr = 0;
BOOL BigEndian = FALSE;
BOOL PauseRecPlaybackState = FALSE;
BOOL Recording = FALSE;
double DatabaseOriginLat = 0.0;
double DatabaseOriginLon = 0.0;
HANDLE hWin32ProcDiedMutex = NULL;
HANDLE hShutdownEvent = NULL;
HANDLE hRecordShutdownEvent = NULL;
HANDLE hPlaybackShutdownEvent = NULL;
HANDLE hWaitForWin32ProcToDieThread = NULL;
HANDLE hSendRcvThread = NULL;
HANDLE hShutdownThread = NULL;
HANDLE hRecordThread = NULL;
HANDLE hPlaybackThread = NULL;
HANDLE hRecordFile = NULL;
HANDLE hPlaybackFile = NULL;
//HANDLE hReceivedMessageEvent = NULL;
HANDLE hAsyncTimer = NULL;
SOCKADDR_IN saddr;
SOCKADDR_IN raddr;
SOCKET sndsock = (SOCKET) - 1;
SOCKET rcvsock = (SOCKET) - 1;
SharedBufferQueue RecPlaybackBufferQueue;
CLinkedList<CSharedEntityObj *> EntityList;
CLinkedList<CSharedViewObj *> ViewList;
CLinkedList<CSharedViewGroupObj *> ViewGroupList;
CLinkedList<HAT_REQUEST> HATRequestList;
CLinkedList<LOS_REQUEST> LOSRequestList;
CLinkedList<ENV_REQUEST> EnvRequestList;

// These are for the transport delay test:
HANDLE hCommPort = NULL;
bool SignalCommPort = false;
int TranspDelayLoopCounter = 0;
int TranspDelayCount1 = 0;
int TranspDelayCount2 = 0;
DOF TranspDelayDOF1 = {0};
DOF TranspDelayDOF2 = {0};

CIGI_IG_CONTROL igc = {0};
ICigiProtocolAdapter *ProtocolAdapter = NULL;

void _cdecl main(int  argc, char **argv, char **envp)
{
    igc.packet_id = CIGI_IG_CONTROL_OPCODE;
    igc.packet_size = sizeof(CIGI_IG_CONTROL);
    igc.cigi_version = 4;
    igc.db_number = 0;
    igc.ig_mode = IG_CONTROL_IG_MODE_OPERATE;
    igc.minor_version = 0;
	igc.extrap_interpol_enable = TRUE;
    igc.EntityTypeSubEn = TRUE;

    CigiProtocolAdapterSelection protocolSelection =
        CigiProtocolAdapterFactory::Select(CigiProtocolVersion::Current());
    ProtocolAdapter = protocolSelection.adapter;
    ProtocolAdapter->Configure(&igc);

    if ((argc > 2) && (((strcmp(argv[1], "-v") == 0)) || (strcmp(argv[2], "-v") == 0)))
        verbose = 1;
    else if ((argc > 1) && (strcmp(argv[1], "-v") == 0))
        verbose = 1;

    // On multiprocessor Windows systems, truncate sub-millisecond sleep
    // durations so the worker remains responsive.
    if ((argc > 2) && (((strcmp(argv[1], "-mp") == 0)) || (strcmp(argv[2], "-mp") == 0))) {
        mp = 1;
        LARGE_INTEGER useless;
        UseTruncatedSleepDurations(HEMU_CLOCK_FASTEST, &useless);
    } else if ((argc > 1) && (strcmp(argv[1], "-mp") == 0)) {
        mp = 1;
        LARGE_INTEGER useless;
        UseTruncatedSleepDurations(HEMU_CLOCK_FASTEST, &useless);
    }

    if (!Startup())
        ExitProcess(0);

    MESSAGE_DISPLAY_MSG_TEXT msg;
    strcpy(msg.text, "Windows driver loaded.");
    SendGuiMessage(&msg);

    WaitForSingleObject(hShutdownThread, INFINITE);

    if (verbose)
        printf("Shutdown complete.\n");

    ExitProcess(0);
}


inline void SendGuiMessage(HEMU_MESSAGE *message)
{
    g_DrvToGuiMsgQueue.Push((char *)message, message->size);
}

HEMU_MESSAGE *RcvGuiMessage(void)
{
    static char buffer[MESSAGE_MAX_SIZE];   // must be static to return address

    int count = g_GuiToDrvMsgQueue.Pop(buffer, MESSAGE_MAX_SIZE);
    if (count > 0)
        return (HEMU_MESSAGE *)buffer;
    else
        return NULL;
}

void InitializeCigiHostCallbacks(CigiHostCallbacks *callbacks)
{
    if (!callbacks)
        return;

    callbacks->startOfFrame = DoSOF;
    callbacks->hatHotResponse = DoHatHotResponse;
    callbacks->hatHotExtResponse = DoHatHotExtResponse;
    callbacks->losResponse = DoLOSResponse;
    callbacks->losExtResponse = DoLOSExtResponse;
    callbacks->sensorResponse = DoSensorResponse;
    callbacks->sensorExtResponse = DoSensorExtResponse;
    callbacks->positionResponse = DoPositionResponse;
    callbacks->weatherResponse = DoWeatherResponse;
    callbacks->aerosolResponse = DoAerosolResponse;
    callbacks->maritimeSurfaceResponse = DoMaritimeResponse;
    callbacks->terrestrialSurfaceResponse = DoTerrestrialResponse;
    callbacks->collisionSegmentNotification = DoCollisionSegNotification;
    callbacks->collisionVolumeNotification = DoCollisionVolNotification;
    callbacks->animationStopNotification = DoAnimationStopNotification;
    callbacks->eventNotification = DoEventNotification;
    callbacks->igMessage = DoIGResponse;
}

int Startup(void)
{
    char address[64] = "";
    int buffsize = 0;

    // Initialize the high-performance counter conversion used by the driver.
    LARGE_INTEGER useless;
    InitializeHighResolutionClock(HEMU_CLOCK_FASTEST, &useless);

    // Initialize the shutdown thread and events.
    if (!InitializeShutdown())
        return 0;

    // Initialize coord conversion code (default to WGS84).
    InitializeCCU(CC_WGS84_a, CC_WGS84_f);

    // Initialize the message queues.
    if (!InitializeIPC())
        return 0;

    // Initialize the record/playback buffer queue.  This will contain the
    // message size and then the contents of the message.
    RecPlaybackBufferQueue.Create("Rec-Playback_Buffer", 20, MAX_ETHERNET_PACKET_SIZE + sizeof(int));

    // Initialize winsock.  Do this here instead of in InitializeSockets() because
    // that function gets called whenever we get a MESSAGE_SET_ADDR message, and
    // we only need to do this once.
    WSADATA wsainfo;
    if (WSAStartup(MAKEWORD(1, 1), &wsainfo) != 0) {
        if (verbose)
            printf("Winsock not available.\n");
        return 0;
    }

    // Initialize the selected CIGI host protocol.  The CIGI 4 adapter keeps
    // the existing API/session/callback behavior; future adapters own their
    // own opcode registration here.
    CigiHostCallbacks cigiCallbacks = {0};
    InitializeCigiHostCallbacks(&cigiCallbacks);
    session = ProtocolAdapter->InitializeHostSession(&cigiCallbacks,
                                                     1,
                                                     8,
                                                     MAX_ETHERNET_PACKET_SIZE);

    // Initialize the Send/Receive thread and events.
    if (!InitializeSendRcv())
        return 0;

    // Open the comm port for the trigger.
    hCommPort = OpenCommPort(1);
    if (verbose)
        printf("Opened COM1 (%p)\n", static_cast<void *>(hCommPort));

    return 1;
}

int InitializeShutdown(void)
{
    DWORD SuspendCount;

    // Open the mutex created by the Win32 process.  If this mutex is ever unlocked,
    // we know that process has ended and we didn't receive a shutdown message.
    hWin32ProcDiedMutex = CreateMutex(NULL, FALSE, "HemuForceShutdownMutex");

    // Create the shutdown event.
    hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, "HemuRTStartShutdownEvent");
    if (!hShutdownEvent) {
        if (verbose)
            printf("Cannot create shutdown event.  Error = %d\n", GetLastError());
        return 0;
    }

    // Set the priority of main thread.
    if (SetThreadPriority(GetCurrentThread(), PRIORITY_HIGH) == FALSE) {
        if (verbose)
            printf("Cannot set main thread priority.  Error = %d\n", GetLastError());
        return 0;
    }

    // Create a high-priority thread to detect if the Win32 process has died.
    // Do this before we create the shutdown thread because the shutdown thread
    // waits on this handle.
    hWaitForWin32ProcToDieThread = CreateThread(0, 0, WaitForWin32ProcToDie, NULL, CREATE_SUSPENDED, 0);
    if (!hWaitForWin32ProcToDieThread) {
        if (verbose)
            printf("Cannot create thread.  Error = %d\n", GetLastError());
        return 0;
    }
    if (SetThreadPriority(hWaitForWin32ProcToDieThread, PRIORITY_HIGHEST) == FALSE) {
        if (verbose)
            printf("Cannot set thread priority.  Error = %d\n", GetLastError());
        return 0;
    }

    SuspendCount = ResumeThread(hWaitForWin32ProcToDieThread);
    if (SuspendCount == 0xFFFFFFFF) {
        if (verbose)
            printf("Cannot start thread.  Error = %d\n", GetLastError());
        return 0;
    }

    // Create a high-priority shutdown thread.  When the Win32 process triggers the
    // StartShutdownEvent, the thread is free to run.
    hShutdownThread = CreateThread(0, 0, ShutdownThread, NULL, CREATE_SUSPENDED, 0);
    if (!hShutdownThread) {
        if (verbose)
            printf("Cannot create thread.  Error = %d\n", GetLastError());
        return 0;
    }

    if (SetThreadPriority(hShutdownThread, PRIORITY_HIGHER) == FALSE) {
        if (verbose)
            printf("Cannot set shutdown thread priority.  Error = %d\n", GetLastError());
        return 0;
    }

    SuspendCount = ResumeThread(hShutdownThread);
    if (SuspendCount == 0xFFFFFFFF) {
        if (verbose)
            printf("Cannot start shutdown thread.  Error = %d\n", GetLastError());
        return 0;
    }

    // Abort the initialization procedure if the Win32 process is not loaded.  We
    // can determine this by testing the HemuForceShutdownMutex object.
    if (hWin32ProcDiedMutex == NULL)
        return 0;

    return 1;
}

int InitializeSendRcv(void)
{
    DWORD SuspendCount;

    // Create an event object to tell the record thread we have received a CIGI message.
    //hReceivedMessageEvent = CreateEvent(NULL, FALSE, FALSE, "HemuRTReceivedMessageEvent");

    // Create a high-priority send/receive thread.  This must be done after
    // CIGI has been initialized, or else we would have to create an event
    // and wait indefinitely on multiple events (initialize and terminate).
    hSendRcvThread = CreateThread(0, 0, SendRcvThread, NULL, CREATE_SUSPENDED, 0);
    if (!hSendRcvThread) {
        if (verbose)
            printf("Cannot create thread.  Error = %d\n", GetLastError());
        return 0;
    }

    if (SetThreadPriority(hSendRcvThread, PRIORITY_HIGHEST) == FALSE) {
        if (verbose)
            printf("Cannot set send/receive thread priority.  Error = %d\n", GetLastError());
        return 0;
    }

    SuspendCount = ResumeThread(hSendRcvThread);
    if (SuspendCount == 0xFFFFFFFF) {
        if (verbose)
            printf("Cannot start send/receive thread.  Error = %d\n", GetLastError());
        return 0;
    }

    return 1;
}

int InitializeSockets(MESSAGE_SET_ADDR *MsgSetAddr)
{
    MESSAGE_DISPLAY_MSG_TEXT msg;

    // Clean up the sockets.
#ifdef WITH_HUD
    if ((int)hudsock >= 0)
        closesocket(hudsock);
#endif
    if ((int)sndsock >= 0)
        closesocket(sndsock);

    if ((int)rcvsock >= 0)
        closesocket(rcvsock);

    // Open the connection for sending.
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(MsgSetAddr->send_port);
    saddr.sin_addr.s_addr = inet_addr(MsgSetAddr->ip_addr);

    sndsock = socket(AF_INET, SOCK_DGRAM, 0);
    if ((short)sndsock == INVALID_SOCKET) {
        if (verbose)
            printf("Cannot create socket for sending.  Error = %d\n", WSAGetLastError());

        SetEvent(hShutdownEvent);
    }
    if (verbose)
        printf("Socket %d opened for sending to port %d at %s.\n",
                 sndsock, MsgSetAddr->send_port, MsgSetAddr->ip_addr);

    sprintf(msg.text, "Sending to port %d at %s.", MsgSetAddr->send_port, MsgSetAddr->ip_addr);
    SendGuiMessage(&msg);

#ifdef WITH_HUD
    // Open the connection for sending.
    haddr.sin_family = AF_INET;
    haddr.sin_port = htons(MsgSetAddr->hud_send_port);
    haddr.sin_addr.s_addr = inet_addr(MsgSetAddr->hud_ip_addr); //inet_addr("100.1.42.125");

    hudsock = socket(AF_INET, SOCK_DGRAM, 0);
    if ((short)hudsock == INVALID_SOCKET) {
        if (verbose)
            printf("Cannot create socket for sending HUD data.  Error = %d\n", WSAGetLastError());

        SetEvent(hShutdownEvent);
    }
    if (verbose)
        printf("Socket %d opened for sending HUD data to port %d at %s.\n",
                 hudsock, MsgSetAddr->hud_send_port, MsgSetAddr->hud_ip_addr);

    sprintf(msg.text, "Sending HUD data to port %d at %s.", MsgSetAddr->hud_send_port, MsgSetAddr->hud_ip_addr);
    SendGuiMessage(&msg);

#endif

    // Open the connection for receiving.
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(MsgSetAddr->rcv_port);
    raddr.sin_addr.s_addr = htonl(INADDR_ANY);

    rcvsock = socket(AF_INET, SOCK_DGRAM, 0);
    if ((short)rcvsock == INVALID_SOCKET) {
        if (verbose)
            printf("Cannot create socket for receiving.  Error = %d\n", WSAGetLastError());

        SetEvent(hShutdownEvent);
    }

    // Make the address reusable.
    int sockparam = TRUE;
    if (setsockopt(rcvsock, SOL_SOCKET, SO_REUSEADDR, (const char *)&sockparam, sizeof(int))
        == SOCKET_ERROR) {
        if (verbose)
            printf("Cannot make address reusable.\n");

        SetEvent(hShutdownEvent);
    }

    // Make the socket non-blocking.
    unsigned long arg = 1L;
    if (ioctlsocket(rcvsock, FIONBIO, &arg) == SOCKET_ERROR) {
        if (verbose)
            printf("Cannot make receive socket non-blocking.  Error = %d\n", WSAGetLastError());

        SetEvent(hShutdownEvent);
    }

    // Bind the socket to the local address.
    if (bind(rcvsock, (SOCKADDR *)&raddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        if (verbose)
            printf("Cannot perform bind on receive socket.  Error = %d\n", WSAGetLastError());

        SetEvent(hShutdownEvent);
    }
    if (verbose)
        printf("Socket %d bound to port %d at local address.\n",
                 rcvsock, MsgSetAddr->rcv_port);

    sprintf(msg.text, "Receiving on port %d at local address.", MsgSetAddr->rcv_port);
    SendGuiMessage(&msg);


    return 1;
}

ULONG WINAPI WaitForWin32ProcToDie(void *nContext)
{
    if (hWin32ProcDiedMutex) {
        HANDLE handles[2] = { hWin32ProcDiedMutex, hShutdownEvent };
        WaitForMultipleObjects(2, handles, FALSE, INFINITE);
    }

    igc.ig_mode = IG_CONTROL_IG_MODE_STANDBY;
    SetEvent(hShutdownEvent);

    return 0;
}

ULONG WINAPI SendRcvThread(void *nContext)
{
    static unsigned char buffer[MAX_ETHERNET_PACKET_SIZE];
    static unsigned char swapbuffer[MAX_ETHERNET_PACKET_SIZE];
    static unsigned char *sendbuffer = NULL;
    static unsigned int i = 0;
    static unsigned int j = 0;          // used by Win32 version
    static unsigned int loopcount = 0;
    static int buffsize = 0;
    static int readsize = 0;    // number of bytes read from the buffer last frame
    static int sendsize = 0;
    static int error = 0;
    static LARGE_INTEGER t1, t2, dt = {1};
    static LARGE_INTEGER t_rcv, t_send, t_rcv_prev = {1};
    static LARGE_INTEGER sleep_interval = {5000};   // 0.5ms
    static MESSAGE_FRAME_RATE MsgFrameRate;
    static MESSAGE_NO_CONNECT MsgNoConnect;
    static TIMEVAL TimeoutVal = {0, 100};   // 100 microsec

    // Since the driver is loaded, we want to send a message so the GUI will
    // update the status bar.  For the Win32 version of the driver, we had
    // to increase the value with which we are comparing i to 5000, which
    // made the status bar text change much later into the program.
    SendGuiMessage(&MsgNoConnect);

    // We MUST call CigiStartMessage() and CigiEndMessage() at least once
    // before we can call CigiGetOutgoingMsgBuffer().
    ProtocolAdapter->StartMessage(session);
    ProtocolAdapter->AddIGControlPacket(session, &igc);
    ProtocolAdapter->EndMessage(session);

    do {
        buffsize = recvfrom(rcvsock, (char *)buffer, MAX_ETHERNET_PACKET_SIZE, 0, NULL, 0);
        if (buffsize > 0) {
            // Get the current time.  This will be very close to the time we
            // actually received the data (within a few microsec).
            t_rcv_prev.QuadPart = t_rcv.QuadPart;
            GetHighResolutionClockTime(HEMU_CLOCK_FASTEST, &t_rcv);

            // Give CIGI a pointer to the incoming data so it can sync the frame counter.
            ProtocolAdapter->SetIncomingMessageBuffer(session, buffer, buffsize);

            ProtocolAdapter->SyncFrameCounter(session);

            if (OperateMode == MODE_PLAYBACK) {
                ProtocolAdapter->GetOutgoingMessageBuffer(session, &sendbuffer,
                                                          &sendsize);

                if (readsize) {
                    // If we are forcing big-endian output, swap to the
                    // temporary buffer.
                    if (BigEndian) {
                        memcpy((char *)swapbuffer, sendbuffer, sendsize);
                        ProtocolAdapter->SwapOutgoingMessageBuffer(
                            (char *)swapbuffer, sendsize);
                        sendto(sndsock, (char *)swapbuffer, readsize, 0, (SOCKADDR *)&saddr, sizeof(SOCKADDR));
                    } else {
                        sendto(sndsock, (char *)sendbuffer, readsize, 0, (SOCKADDR *)&saddr, sizeof(SOCKADDR));
                    }

                    GetHighResolutionClockTime(HEMU_CLOCK_FASTEST, &t_send);

                    LARGE_INTEGER deltas[2];
                    deltas[0].QuadPart = t_rcv.QuadPart - t_rcv_prev.QuadPart;
                    deltas[1].QuadPart = t_send.QuadPart - t_rcv.QuadPart;

                    MESSAGE_NOTIFY_RECPLAYBACK_FRAME notify_msg;
                    SendGuiMessage(&notify_msg);

                    g_SentCIGIMsgQueue.Push((char *)sendbuffer, readsize);
                    g_SentCIGIMsgQueue.Push((char *)deltas, 2 * sizeof(LARGE_INTEGER));
                }

                // Process the message buffer we just received.
                ProtocolAdapter->ProcessIncomingMessage(session);

                // Must begin and end the message, even though we will be overwriting the
                // buffer.
                ProtocolAdapter->StartMessage(session);
                ProtocolAdapter->EndMessage(session);

                // After we start the message, we need to get a pointer to the next buffer.
                ProtocolAdapter->GetOutgoingMessageBuffer(session, &sendbuffer,
                                                          &sendsize);

                // This could be dangerous because we are writing to an address that the
                // CIGI API is returning.  But since we are writing the file, we can
                // guarantee that none of the recorded messages will be larger than the
                // maximum outgoing buffer size.
                GetNextPlaybackMessage(sendbuffer, &readsize);
            } else if (OperateMode == MODE_NORMAL) {
                // If this is the first call, sendsize will remain zero.
                ProtocolAdapter->GetOutgoingMessageBuffer(session, &sendbuffer,
                                                          &sendsize);
                readsize = sendsize;    // in case we change modes before the next frame

                if (sendsize) {
                    // If we are forcing big-endian output, swap to the
                    // temporary buffer.
                    if (BigEndian) {
                        memcpy((char *)swapbuffer, sendbuffer, sendsize);
                        ProtocolAdapter->SwapOutgoingMessageBuffer(
                            (char *)swapbuffer, sendsize);
                        sendto(sndsock, (char *)swapbuffer, sendsize, 0, (SOCKADDR *)&saddr, sizeof(SOCKADDR));
                    } else {
                        sendto(sndsock, (char *)sendbuffer, sendsize, 0, (SOCKADDR *)&saddr, sizeof(SOCKADDR));
                    }

                    GetHighResolutionClockTime(HEMU_CLOCK_FASTEST, &t_send);

                    LARGE_INTEGER deltas[2];
                    deltas[0].QuadPart = t_rcv.QuadPart - t_rcv_prev.QuadPart;
                    deltas[1].QuadPart = t_send.QuadPart - t_rcv.QuadPart;

                    // First push the (unswapped) message.
                    g_SentCIGIMsgQueue.Push((char *)sendbuffer, sendsize);

                    // Then push the time deltas.
                    g_SentCIGIMsgQueue.Push((char *)deltas, 2 * sizeof(LARGE_INTEGER));
                }

#ifdef WITH_HUD
                unsigned long handle = NULL;
                CSharedEntityObj *ownship = FindEntity(0, &handle);
                if (ownship) {
                    POS_DATA ownship_pos = ownship->GetPosAtt();
                    RATE_DATA ownship_rates = ownship->GetRates();

                    hud_data.lat = ownship_pos.latitude;
                    hud_data.lon = ownship_pos.longitude;
                    hud_data.airspeed = (float)MPSToKnots(ownship_rates.airspeed);
                    hud_data.alt = (float)ownship_pos.altitude;
                    hud_data.hdg = (float)ownship_pos.heading;
                    hud_data.pitch = (float)ownship_pos.pitch;
                    hud_data.roll = (float)ownship_pos.roll;
                    hud_data.aoa = 0.0f;
                    hud_data.Gs = 1.0f;
                    hud_data.enable = hud_enable;

                    sendto(hudsock, (char *)&hud_data, sizeof(hud_data), 0, (SOCKADDR *)&haddr, sizeof(SOCKADDR));
                }
#endif

                if (Recording) {
                    BufferNextRecordMessage(sendbuffer, sendsize);

                    MESSAGE_NOTIFY_RECPLAYBACK_FRAME notify_msg;
                    SendGuiMessage(&notify_msg);
                }

                // Process the message buffer we just received.
                ProtocolAdapter->ProcessIncomingMessage(session);

                // Start the next outgoing message.
                ProtocolAdapter->StartMessage(session);
                ProtocolAdapter->AddIGControlPacket(session, &igc);

                // Build the entity control packets.
                BuildEntityControlPackets();

                // Build the view control and view definition packets.
                BuildViewPackets();

                // If we are operating, check the CIGI message queue and add
                // any packets created by the GUI.

                // We always want to add packets added to the immediate queue.
                // This function is not getting called when there is no IG.  We must
                // add it to the else condition or, better yet, find a new way to
                // communicate FRAME messages from the driver.
                AddImmediateQueuedPackets();

                // If we are paused, we want to hold the messages until we resume.
                if (RunState) {
                    AddQueuedPackets();

                    // Send any HAT or LOS Occult/Range requests.
                    AddHATPackets();
                    AddLOSPackets();
                }

                // When we have visited each node, end the message.
                ProtocolAdapter->EndMessage(session);
            } else if (OperateMode == MODE_TRANSPDELAYTEST) {
                // If this is the first call, sendsize will remain zero.
                ProtocolAdapter->GetOutgoingMessageBuffer(session, &sendbuffer,
                                                          &sendsize);
                if (sendsize) {
                    // If we are forcing big-endian output, swap to the
                    // temporary buffer.
                    if (BigEndian) {
                        memcpy((char *)swapbuffer, sendbuffer, sendsize);
                        ProtocolAdapter->SwapOutgoingMessageBuffer(
                            (char *)swapbuffer, sendsize);
                        sendto(sndsock, (char *)swapbuffer, sendsize, 0, (SOCKADDR *)&saddr, sizeof(SOCKADDR));
                    } else {
                        sendto(sndsock, (char *)sendbuffer, sendsize, 0, (SOCKADDR *)&saddr, sizeof(SOCKADDR));
                    }

                    GetHighResolutionClockTime(HEMU_CLOCK_FASTEST, &t_send);

                    // Send a signal on the COMM port.
                    if (SignalCommPort) {
                        WriteByteToCommPort(hCommPort, 0x00);
                        SignalCommPort = false;
                    }

                    LARGE_INTEGER deltas[2];
                    deltas[0].QuadPart = t_rcv.QuadPart - t_rcv_prev.QuadPart;
                    deltas[1].QuadPart = t_send.QuadPart - t_rcv.QuadPart;
                }

                // Process the message buffer we just received.
                ProtocolAdapter->ProcessIncomingMessage(session);

                // Start the next outgoing message.
                ProtocolAdapter->StartMessage(session);
                ProtocolAdapter->AddIGControlPacket(session, &igc);

                if (TranspDelayLoopCounter == 0) {
                    const CIGI_ENTITY_CONTROL *ec = TranspDelayTestCreateECPacket(0);
                    CigiAddFixedPacket<CIGI_ENTITY_CONTROL>(session, ec);

                    SignalCommPort = true;
                } else if (TranspDelayLoopCounter == TranspDelayCount1) {
                    // Create Entity Control packet for second position.
                    const CIGI_ENTITY_CONTROL *ec = TranspDelayTestCreateECPacket(1);
                    CigiAddFixedPacket<CIGI_ENTITY_CONTROL>(session, ec);
                } else if (TranspDelayLoopCounter >= TranspDelayCount1 + TranspDelayCount2 - 1) {
                    TranspDelayLoopCounter = -1;
                }

                TranspDelayLoopCounter++;

                // When we have visited each node, end the message.
                ProtocolAdapter->EndMessage(session);
            }

			// Send a status message every 30 frames.
            if (loopcount++ % 30 == 0) {
                if (dt.QuadPart == 0)
                  MsgFrameRate.framerate = 0;
                else
                    MsgFrameRate.framerate = (long)(0.5 + (10000000.0f / dt.QuadPart));

                SendGuiMessage(&MsgFrameRate);
			}

            i = 0;
        } else {
            SleepForFileTime(&sleep_interval);

            // For multiprocessor machines, the SleepForFileTime() call
            // simply makes this process reschedulable.  We need
            // to increase amount of time to wait for SOF messages.
            const unsigned int icount = (mp ? 10000 : 50);

            if (i++ < icount)
                continue;       // Check for SOF again
            else {
                i = 0;          // Reset

                // For multiprocessor machines, the SleepForFileTime() call
                // simply makes this process reschedulable.  We need
                // to increase amount of time between status messages.
                if (j++ == (unsigned int)(mp ? 1000 : 100)) {
                    j = 0;

                    // Send a status message.
                    SendGuiMessage(&MsgNoConnect);
                }
            }
        }

        GetHighResolutionClockTime(HEMU_CLOCK_FASTEST, &t1);
        if (RunState && (OperateMode == MODE_NORMAL)) {
            UpdateEntities(dt.QuadPart / 10000000.0f);
        }
        dt.QuadPart = t1.QuadPart - t2.QuadPart;
        GetHighResolutionClockTime(HEMU_CLOCK_FASTEST, &t2);

        // Check our message queue and perform the necessary actions.  This should be
        // performed AFTER the message is sent to the IG.  This is why the packet queue
        // is separate from the message queue.
        CheckMessages();

    } while (WaitForSingleObject(hShutdownEvent, 0) != WAIT_OBJECT_0);

    return 0;
}


ULONG WINAPI ShutdownThread(void *nContext)
{
    HANDLE handles[3] = { hShutdownEvent, hWin32ProcDiedMutex, hWaitForWin32ProcToDieThread };
    WaitForMultipleObjects(3, handles, FALSE, INFINITE);

    if (verbose)
        printf("Received shutdown event or Win32 process has died.\n");

    // Make sure the ShutdownEvent is signaled so the Send/Receive Thread
    // and Record/Playback threads will exit.
    SetEvent(hShutdownEvent);

    // Wait for the Send/Receive thread to terminate so that we don't
    // delete the entities, views, and view groups while the thread
    // is still using them.  Only wait for about 5 seconds -- anything
    // beyond that and there is an error.
    WaitForSingleObject(hSendRcvThread, INFINITE);

    ClearEntities();
    ClearViews();
    ClearViewGroups();

    // Sleep for .5 sec to give TimerHandler() time to finish.
    Sleep(500);

    // Clean up the sockets.
#ifdef WITH_HUD
    closesocket(hudsock);
#endif
    closesocket(sndsock);
    closesocket(rcvsock);
    WSACleanup();

    // Close the comm port.
    CloseCommPort(hCommPort);

    // Clean up CIGI.
    CigiCloseSession(session);
    CigiShutdown();

    StopRecord();
    StopPlayback();
    CloseHandle(hShutdownEvent);
    return(0);
}

ULONG WINAPI RecordThread(void *nContext)
{
    unsigned char buffer[MAX_ETHERNET_PACKET_SIZE + sizeof(int)];
    int *size = (int *)buffer;
    unsigned char *msg = &(buffer[sizeof(int)]);
    static unsigned long bytes_written;

    if (verbose && !hRecordFile) {
        printf("Record error.  Invalid file handle.\n");
        return(0);
    }

    if (verbose)
        printf("Record thread created.\n");

    // For some reason this event isn't being automatically reset after
    // it is released and re-created.  Reset it here.
    ResetEvent(hRecordShutdownEvent);

    HANDLE handles[2] = { hRecordShutdownEvent, hShutdownEvent };
    while ((WaitForMultipleObjects(2, handles, FALSE, 1) == WAIT_TIMEOUT)
           && RecPlaybackBufferQueue.GetSize()) {
        if (!PauseRecPlaybackState && RunState) {
            while (RecPlaybackBufferQueue.Pop((char *)buffer, sizeof(buffer)) > 0) {
                WriteFile(hRecordFile, (char *)buffer, *size + sizeof(int), &bytes_written, NULL);
            }
        } else
            Sleep(1);
    }

    if (verbose)
        printf("RecordThread terminated.\n");

    return(0);
}

void BufferNextRecordMessage(unsigned char *buffer, int size)
{
    static unsigned char tempbuff[MAX_ETHERNET_PACKET_SIZE + sizeof(int)];
    static int *buffsize = (int *)tempbuff;
    static unsigned char *msg = &(tempbuff[sizeof(int)]);

    if (PauseRecPlaybackState)
        return;

    // Add the message size to the buffer.
    *buffsize = size;

    // Copy the message to the buffer.
    memcpy(msg, buffer, size);

    // Queue the message.
    int queuesz = RecPlaybackBufferQueue.Push((char *)tempbuff, size + sizeof(int));
}

int StartRecord(char *filename)
{
    if (hRecordFile)
        StopRecord();

    PauseRecPlaybackState = FALSE;

    // Clear the queue first.
    unsigned char buffer[MAX_ETHERNET_PACKET_SIZE + sizeof(int)];
    while (RecPlaybackBufferQueue.Pop((char *)buffer, sizeof(buffer)) > 0);

    // Open the file.
    hRecordFile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS,
                             FILE_ATTRIBUTE_ARCHIVE | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (hRecordFile == INVALID_HANDLE_VALUE) {
        MESSAGE_FILE_ERROR msg;
        strcpy(msg.filename, filename);
        SendGuiMessage(&msg);

        MESSAGE_PLAYBACK_EOF eof;
        SendGuiMessage(&eof);

        printf("Cannot create record file %s.  Error code = %d\n", filename, GetLastError());
        hRecordFile = NULL;
        return 0;
    } else if (verbose)
        printf("File %s opened for recording.\n", filename);

    Recording = TRUE;

    // Create low-priority record thread.
    hRecordShutdownEvent = CreateEvent(NULL, TRUE, FALSE, "HemuRTRecordShutdownEvent");
    hRecordThread = CreateThread(0, 0, RecordThread, NULL, CREATE_SUSPENDED, 0);
    SetThreadPriority(hRecordThread, PRIORITY_LOW);
    ResumeThread(hRecordThread);

    return 1;
}

void StopRecord(void)
{
    Recording = FALSE;
    PauseRecPlaybackState = FALSE;

    if (hRecordFile == NULL)
        return;

    // Allow the record thread to stop.
    SetEvent(hRecordShutdownEvent);
    WaitForSingleObject(hRecordThread, INFINITE);

    CloseHandle(hRecordFile);
    CloseHandle(hRecordShutdownEvent);
    hRecordFile = NULL;
    hRecordShutdownEvent = NULL;
}

ULONG WINAPI PlaybackThread(void *nContext)
{
    unsigned char buffer[MAX_ETHERNET_PACKET_SIZE + sizeof(int)];
    int *size = (int *)buffer;
    unsigned char *msg = &(buffer[sizeof(int)]);
    BOOL readstatus = TRUE;
    unsigned long bytes_read1 = 1;      // != 0 for first time through inner while
    unsigned long bytes_read2 = 0;

    if (verbose && !hPlaybackFile) {
        printf("Read error.  Invalid file handle.\n");
        return(0);
    }

    if (verbose)
        printf("Read thread created.\n");

    // For some reason this event isn't being automatically reset after
    // it is released and re-created.  Reset it here.
    ResetEvent(hPlaybackShutdownEvent);

    HANDLE handles[2] = { hPlaybackShutdownEvent, hShutdownEvent };
    while ((WaitForMultipleObjects(2, handles, FALSE, 1) == WAIT_TIMEOUT)
           && (readstatus) && (bytes_read1)) {
        if (!PauseRecPlaybackState && RunState
            && (RecPlaybackBufferQueue.GetItemCount() < RecPlaybackBufferQueue.GetSize())) {
            // Read the buffer size and then the actual buffer.
            readstatus = ReadFile(hPlaybackFile, size, sizeof(int), &bytes_read1, NULL);

            if (readstatus && bytes_read1) {
                readstatus = ReadFile(hPlaybackFile, msg, *size, &bytes_read2, NULL);

                // Push it to the queue.
                RecPlaybackBufferQueue.Push((char *)buffer, bytes_read1 + bytes_read2);
            } else {
                if (verbose)
                    printf("End of playback file.\n");

                MESSAGE_PLAYBACK_EOF msg;
                SendGuiMessage(&msg);

                // Flag the event so the outter while loop will exit.
                SetEvent(hPlaybackShutdownEvent);

                *size = 0;
                break;
            }
        } else
            Sleep(1);
    }


    CloseHandle(hPlaybackFile);
    CloseHandle(hPlaybackShutdownEvent);
    hPlaybackFile = NULL;
    hPlaybackShutdownEvent = NULL;

    OperateMode = MODE_NORMAL;
    PauseRecPlaybackState = FALSE;

    if (verbose)
        printf("PlaybackThread terminated.\n");

    return(0);
}

void GetNextPlaybackMessage(unsigned char *buffer, int *size)
{
    static unsigned char tempbuff[MAX_ETHERNET_PACKET_SIZE + sizeof(int)];
    static int *buffsize = (int *)tempbuff;
    static unsigned char *msg = &(tempbuff[sizeof(int)]);
    DWORD bytesread = 0;

    if (PauseRecPlaybackState) {
        *size = 0;
        return;
    }

    // Get the next message.
    if (RecPlaybackBufferQueue.Pop((char *)tempbuff, MAX_ETHERNET_PACKET_SIZE + sizeof(int)) <= 0) {
        *size = 0;
        return;
    }

    // The size is the first four bytes.
    *size = *((int *)tempbuff);

    // Copy the message to the buffer.
    memcpy(buffer, msg, *size);
}

int StartPlayback(char *filename)
{
    if (hRecordFile) {
        if (verbose)
            printf("Recording. Attempting to stop for playback.\n");

        StopRecord();
    }

    if (hPlaybackFile) {
        if (verbose)
            printf("Previous playback has not stopped. Attempting to stop.\n");

        StopPlayback();
    }

    OperateMode = MODE_PLAYBACK;
    PauseRecPlaybackState = FALSE;

    // Clear the queue first.
    unsigned char buffer[MAX_ETHERNET_PACKET_SIZE + sizeof(int)];
    while (RecPlaybackBufferQueue.Pop((char *)buffer, sizeof(buffer)) > 0);

    // Open the file.
    hPlaybackFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (hPlaybackFile == INVALID_HANDLE_VALUE) {
        MESSAGE_FILE_ERROR msg;
        strcpy(msg.filename, filename);
        SendGuiMessage(&msg);

        MESSAGE_PLAYBACK_EOF eof;
        SendGuiMessage(&eof);

        printf("Cannot open playback file %s.  Error code = %d\n", filename, GetLastError());
        hPlaybackFile = NULL;
        return 0;
    } else if (verbose)
        printf("File %s opened for playback.\n", filename);

    // Create low-priority read thread. We can reuse the record shutdown event.
    hPlaybackShutdownEvent = CreateEvent(NULL, FALSE, FALSE, "HemuRTPlaybackShutdownEvent");
    hPlaybackThread = CreateThread(0, 0, PlaybackThread, NULL, CREATE_SUSPENDED, 0);
    SetThreadPriority(hPlaybackThread, PRIORITY_LOW);
    ResumeThread(hPlaybackThread);

    return 1;
}

void StopPlayback(void)
{
    PauseRecPlaybackState = FALSE;

    if (hPlaybackFile == NULL)
        return;

    // Allow the record thread to stop.
    SetEvent(hPlaybackShutdownEvent);
    WaitForSingleObject(hPlaybackThread, INFINITE);

    if (verbose)
        printf("Playback thread has been stopped successfully.\n");
}

void CheckMessages(void)
{
    HEMU_MESSAGE *msg = NULL;

    while ((msg = RcvGuiMessage()) != NULL) {
        switch (msg->msg) {
        case MSG_ADD_ENTITY: {
            if (verbose)
                printf("Received MESSAGE_ADD_ENTITY from GUI process.  ");

            CSharedEntityObj *entity = new CSharedEntityObj;
            if (entity) {
                entity->Create(((MESSAGE_ADD_ENTITY *)msg)->sharedname);

                // If the entity is locked, add the message back to the
                // end of the queue and retry later.
                if (verbose) {
                    ENTITY_CIGI_DATA entity_state = {0};

                    if (!entity->GetAndLockCigiData(&entity_state)) {
                        g_GuiToDrvMsgQueue.Push((char *)msg, msg->size);

                        if (verbose)
                            printf("     Entity %d is locked.  Queued for retry.\n", entity_state.id);

                        break;
                    } else {
                        entity->Unlock();
                    }
                    printf("     Entity ID = %d.\n", entity_state.id);
                }

                EntityList.InsertTail(entity);
            } else {
                printf("Out of memory!!!  Could not create entity.\n");
            }

            break;
        }

        case MSG_DEL_ENTITY: {
            if (verbose)
                printf("Received MESSAGE_DEL_ENTITY from GUI process (ID = %d).\n",
                         ((MESSAGE_DEL_ENTITY *)msg)->id);

            int id = ((MESSAGE_DEL_ENTITY *)msg)->id;
            unsigned long handle = NULL;
            CSharedEntityObj *shared_obj = FindEntity(id, &handle);
            if (!shared_obj)
                break;

            ENTITY_CIGI_DATA data = {0};

            // If the entity is locked, add the message back to the
            // end of the queue and retry later.
            if (!shared_obj->GetAndLockCigiData(&data)) {
                g_GuiToDrvMsgQueue.Push((char *)msg, msg->size);

                if (verbose)
                    printf("     Entity is locked. Queued for retry.\n");

                break;
            } else {
                shared_obj->Unlock();
            }

            // Send a message to the IG to destroy the entity.
            CIGI_ENTITY_CONTROL ec = { 0 };
            ec.packet_id = CIGI_ENTITY_CONTROL_OPCODE;
            ec.packet_size = sizeof(CIGI_ENTITY_CONTROL);
            ec.entity_id = id;
            ec.entity_state = ENTITY_STATE_DESTROYED;  // destroy
            g_SendImmedCIGIMsgQueue.Push((char *)&ec, sizeof(CIGI_ENTITY_CONTROL));

            int itemcount = DeleteEntity(id);

            // Don't deactivate any children -- they will be explicitly
            // deleted by the GUI process.

            break;
        }

        case MSG_CLEAR_ENTITIES:
            if (verbose)
                printf("Received MESSAGE_CLEAR_ENTITIES from GUI process.\n");

            ClearEntities();
            break;

        case MSG_ADD_VIEW: {
            if (verbose)
                printf("Received MESSAGE_ADD_VIEW from GUI process.  ");

            CSharedViewObj *view = NULL;
            view = new CSharedViewObj;
            if (!view)
                _asm int 3

                view->Create(((MESSAGE_ADD_VIEW *)msg)->sharedname);
            VIEW_DEF_DATA viewdef = {0};

            // If the view is locked, add the message back to the
            // end of the queue and retry later.
            if (!view->GetAndLockViewDef(&viewdef)) {
                g_GuiToDrvMsgQueue.Push((char *)msg, msg->size);

                if (verbose)
                    printf("     View %d is locked. Queued for retry.\n", viewdef.view_id);

                break;
            } else {
                view->Unlock();
            }

            if (verbose)
                printf("     View ID = %d.\n", viewdef.view_id);

            ViewList.InsertTail(view);

            break;
        }

        case MSG_DEL_VIEW: {
            if (verbose)
                printf("Received MESSAGE_DEL_VIEW from GUI process (ID = %d).\n",
                         ((MESSAGE_DEL_VIEW *)msg)->id);

            DeleteView(((MESSAGE_DEL_VIEW *)msg)->id);

            break;
        }

        case MSG_CLEAR_VIEWS:
            if (verbose)
                printf("Received MESSAGE_CLEAR_VIEWS from GUI process.\n");

            ClearViews();
            break;

        case MSG_ADD_VIEWGROUP: {
            if (verbose)
                printf("Received MESSAGE_ADD_VIEWGROUP from GUI process.  ");

            CSharedViewGroupObj *group = new CSharedViewGroupObj;
            group->Create(((MESSAGE_ADD_VIEWGROUP *)msg)->sharedname);
            VIEWGROUP_CIGI_DATA group_data = {0};

            // If the view is locked, add the message back to the
            // end of the queue and retry later.
            if (!group->GetAndLockCigi(&group_data)) {
                g_GuiToDrvMsgQueue.Push((char *)msg, msg->size);

                if (verbose)
                    printf("     View group %d is locked. Queued for retry.\n", group_data.group_id);

                break;
            } else {
                group->Unlock();
            }

            if (verbose)
                printf("     Group ID = %d.\n", group_data.group_id);

            ViewGroupList.InsertTail(group);

            break;
        }

        case MSG_DEL_VIEWGROUP: {
            if (verbose)
                printf("Received MESSAGE_DEL_VIEWGROUP from GUI process (ID = %d).\n",
                         ((MESSAGE_DEL_VIEWGROUP *)msg)->id);

            DeleteViewGroup(((MESSAGE_DEL_VIEWGROUP *)msg)->id);

            break;
        }

        case MSG_CLEAR_VIEWGROUPS:
            if (verbose)
                printf("Received MESSAGE_CLEAR_VIEWGROUPS from GUI process.\n");

            ClearViewGroups();
            break;

        case MSG_RUN_EXERCISE:
            if (verbose)
                printf("Received MESSAGE_RUN_EXERCISE from GUI process.\n");

            RunState = 1;
            PreDbLoadRunState = 1;
            igc.ig_mode = IG_CONTROL_IG_MODE_OPERATE;

            break;

        case MSG_PAUSE_EXERCISE:
            if (verbose)
                printf("Received MESSAGE_PAUSE_EXERCISE from GUI process.\n");

            RunState = 0;
            PreDbLoadRunState = 0;
            break;

        case MSG_RESET_HOST:
            if (verbose)
                printf("Received MESSAGE_RESET_HOST from GUI process.\n");

            RunState = 0;

            break;

        case MSG_RESET_IG:
            if (verbose)
                printf("Received MESSAGE_RESET_IG from GUI process.\n");

            igc.ig_mode = IG_CONTROL_IG_MODE_STANDBY;
            break;

        case MSG_SHUTDOWN:
            if (verbose)
                printf("Received MESSAGE_SHUTDOWN from GUI process.\n");

            igc.ig_mode = IG_CONTROL_IG_MODE_STANDBY;
            SetEvent(hShutdownEvent);
            break;

        case MSG_SET_ADDR:
            if (verbose)
                printf("Received MESSAGE_SET_ADDR from GUI process.\n");

            InitializeSockets((MESSAGE_SET_ADDR *)msg);
            break;

        case MSG_ADD_HAT:
            if (verbose)
                printf("Received MESSAGE_ADD_HAT from GUI process.\n");

            AddHAT((MESSAGE_ADD_HAT *)msg);
            break;

        case MSG_ADD_LOS:
            if (verbose)
                printf("Received MESSAGE_ADD_LOS from GUI process.\n");

            AddLOS((MESSAGE_ADD_LOS *)msg);
            break;

        case MSG_REMOVE_HAT:
            if (verbose)
                printf("Received MESSAGE_REMOVE_HAT from GUI process.\n");

            // TODO: RemoveHAT((MESSAGE_REMOVE_HAT *)msg);
            break;

        case MSG_REMOVE_LOS:
            if (verbose)
                printf("Received MESSAGE_REMOVE_LOS from GUI process.\n");

            // TODO: RemoveLOS((MESSAGE_REMOVE_LOS *)msg);
            break;

        case MSG_BEGIN_RECORD:
            if (verbose)
                printf("Received MESSAGE_BEGIN_RECORD from GUI process.\n");

            PauseRecPlaybackState = FALSE;

            StartRecord(((MESSAGE_BEGIN_RECORD *)msg)->filename);
            break;

        case MSG_END_RECORD:
            if (verbose)
                printf("Received MESSAGE_END_RECORD from GUI process.\n");

            StopRecord();
            break;

        case MSG_BEGIN_PLAYBACK:
            if (verbose)
                printf("Received MESSAGE_BEGIN_PLAYBACK from GUI process.\n");

            PauseRecPlaybackState = FALSE;

            StartPlayback(((MESSAGE_BEGIN_PLAYBACK *)msg)->filename);
            break;

        case MSG_END_PLAYBACK:
            if (verbose)
                printf("Received MESSAGE_END_PLAYBACK from GUI process.\n");

            StopPlayback();
            break;

        case MSG_PAUSE_RECPLAYBACK:
            if (verbose)
                printf("Received MESSAGE_PAUSE_RECPLAYBACK from GUI process.\n");

            PauseRecPlaybackState = TRUE;
            break;

        case MSG_RESUME_RECPLAYBACK:
            if (verbose)
                printf("Received MESSAGE_RESUME_RECPLAYBACK from GUI process.\n");

            PauseRecPlaybackState = FALSE;
            break;

        case MSG_SET_DATABASE:
            PreDbLoadRunState = RunState;
            RunState = 0;

            igc.db_number = ((MESSAGE_SET_DATABASE *)msg)->dbnum;

            ChangeMapAndTransportAllEntities(((MESSAGE_SET_DATABASE *)msg)->lat,
                                             ((MESSAGE_SET_DATABASE *)msg)->lon,
                                             ((MESSAGE_SET_DATABASE *)msg)->radius,
                                             ((MESSAGE_SET_DATABASE *)msg)->flattening);

            if (verbose)
                printf("Received MESSAGE_SET_DATABASE from GUI process.  "
                         "Number = %d\n", igc.db_number);

            break;

        case MSG_SET_TRACKER_STATE:
            if (verbose)
                printf("Received MESSAGE_SET_TRACKER_STATE from GUI process.\n");

            // TODO: igc.tracker_enable = ((MESSAGE_SET_TRACKER_STATE *)msg)->enable;

            break;

        case MSG_SET_TRACKER_BORESIGHT:
            if (verbose)
                printf("Received MESSAGE_SET_TRACKER_BORESIGHT from GUI process.\n");

            // TODO: igc.boresight = ((MESSAGE_SET_TRACKER_BORESIGHT *)msg)->boresight;

            break;

        case MSG_SET_IG_MODE:
            igc.ig_mode = ((MESSAGE_SET_IG_MODE *)msg)->mode;

            if (verbose)
                printf("Received MESSAGE_SET_IG_MODE from GUI process.  Mode = %d\n",
                         igc.ig_mode);

            break;

        case MSG_SET_IG_CONTROL:
            igc.extrap_interpol_enable = ((MESSAGE_SET_IG_CONTROL *)msg)->smoothing_enabled;
            igc.EntityTypeSubEn        = ((MESSAGE_SET_IG_CONTROL *)msg)->substitution_enabled;

            if (verbose)
                printf("Received MSG_SET_IG_CONTROL from GUI process.  Smoothing = %s, Substitution = %s\n",
                         igc.extrap_interpol_enable ? "Enabled" : "Disabled", igc.EntityTypeSubEn ? "Enabled" : "Disabled" );

            break;

        case MSG_SET_HUD_STATE:
            hud_enable = ((MESSAGE_SET_HUD_STATE *)msg)->state;

            if (verbose)
                printf("Received MESSAGE_SET_HUD_STATE from GUI process.  State = %d\n",
                         (hud_enable ? 1 : 0));

            break;

        case MSG_SYNC_UPDATE_ENTITIES_VIEW: {
            if (verbose)
                printf("Received MSG_SYNC_UPDATE_ENTITIES_VIEW from GUI process.\n");

            // Immediately send an update message to the GUI.
            MESSAGE_UPDATE_ENTITIES_VIEW resp_msg;
            SendGuiMessage(&resp_msg);

            break;
        }

        case MSG_SYNC_UPDATE_VIEWS_VIEW: {
            if (verbose)
                printf("Received MSG_SYNC_UPDATE_VIEWS_VIEW from GUI process.\n");

            // Immediately send an update message to the GUI.
            MESSAGE_UPDATE_VIEWS_VIEW resp_msg;
            SendGuiMessage(&resp_msg);

            break;
        }

        case MSG_SYNC_UPDATE_ENV_VIEW: {
            if (verbose)
                printf("Received MSG_SYNC_UPDATE_ENV_VIEW from GUI process.\n");

            // Immediately send an update message to the GUI.
            MESSAGE_UPDATE_ENV_VIEW resp_msg;
            SendGuiMessage(&resp_msg);

            break;
        }

        case MSG_SET_CIGI_OPTIONS: {
            if (verbose)
                printf("Received MSG_SET_CIGI_OPTIONS from GUI process.\n");

            BigEndian = ((MESSAGE_SET_CIGI_OPTIONS *)msg)->big_endian;
            CigiProtocolVersion requestedVersion;
            if (!CigiProtocolVersion::TryCreate(
                    ((MESSAGE_SET_CIGI_OPTIONS *)msg)->major_version,
                    ((MESSAGE_SET_CIGI_OPTIONS *)msg)->minor_version,
                    &requestedVersion))
                requestedVersion = CigiProtocolVersion::Current();

            CigiProtocolAdapterSelection selection =
                CigiProtocolAdapterFactory::Select(requestedVersion);
            ProtocolAdapter = selection.adapter;
            ProtocolAdapter->Configure(&igc);

            if (verbose && !selection.exactMatch)
                printf("CIGI %d.%d packet I/O is not implemented; retaining CIGI 4.0.\n",
                       requestedVersion.GetMajorVersion(),
                       requestedVersion.GetMinorVersion());
            break;
        }

        case MSG_TDTEST_START:
            StartTranspDelayTest((MESSAGE_TDTEST_START *)msg);
            break;

        case MSG_TDTEST_STOP:
            StopTranspDelayTest();
            break;

        default:
            break;
        }
    }
}

void ChangeMapAndTransportAllEntities(double new_lat, double new_lon, double new_radius, double new_flattening)
{
    // Create a queue in case we have to retry any entities.
    static CSimpleQ<CSharedEntityObj *> EntityRetryQueue;
    EntityRetryQueue.SetSize(4);

    // Visit each entity in the linked list and get the bearing and range.
    unsigned long handle = NULL;
    CSharedEntityObj **shared_obj = EntityList.GetHead(&handle);
    ENTITY entity;

    while (handle && shared_obj) {
        if ((*shared_obj)->GetAndLock(&entity)) {
            // Get the bearing and range relative to the origin.  Store
            // these in the entity's structure (the dvi and dvj variables
            // should be safe).
            CalcBearingAndRange(DatabaseOriginLat, DatabaseOriginLon, 0.0,
                                entity.cigi.dofs.latitude,
                                entity.cigi.dofs.longitude,
                                &(entity.unsaved.dvi), &(entity.unsaved.dvj));

            (*shared_obj)->SetLocked(entity);
            (*shared_obj)->Unlock();
        } else {
            EntityRetryQueue.Push(*shared_obj);
        }

        shared_obj = EntityList.GetNext(&handle);
    }

    // Now retry any entities in the queue.
    CSharedEntityObj *retry = NULL;
    while (EntityRetryQueue.Pop(&retry) >= 0) {
        if (retry->GetAndLock(&entity)) {
            // Get the bearing and range relative to the origin.  Store
            // these in the entity's structure (the dvi and dvj variables
            // should be safe).
            CalcBearingAndRange(DatabaseOriginLat, DatabaseOriginLon, 0.0,
                                entity.cigi.dofs.latitude,
                                entity.cigi.dofs.longitude,
                                &(entity.unsaved.dvi), &(entity.unsaved.dvj));
            retry->SetLocked(entity);
            retry->Unlock();
        } else {
            EntityRetryQueue.Push(retry);
        }
    }

    // Now move the origin.
    DatabaseOriginLat = new_lat;
    DatabaseOriginLon = new_lon;

    // Reinitialize the ERM structure using the new flattening and radius.
    InitializeCCU(new_radius, new_flattening);

    // We use TM for waypoint flight and missile target tracking.
    SetTMOrigin(new_lat, new_lon);

    // Revisit each entity and reset the position.
    handle = NULL;
    shared_obj = EntityList.GetHead(&handle);

    while (handle && shared_obj) {
        (*shared_obj)->GetAndLock(&entity);
        // Bearing and range are stored in dvi and dvj, respectively.
        GetRelativePos(new_lat, new_lon, 0.0, entity.unsaved.dvi, entity.unsaved.dvj,
                       &(entity.cigi.dofs.latitude),
                       &(entity.cigi.dofs.longitude));

        entity.unsaved.dvi = 0.0;
        entity.unsaved.dvj = 0.0;

        (*shared_obj)->SetLocked(entity);
        (*shared_obj)->Unlock();

        shared_obj = EntityList.GetNext(&handle);
    }
}

void BuildEntityControlPackets(void)
{
    void BuildEntityControlPacketsWorker(ENTITY * entity);

    // Create a queue in case we have to retry any entities.
    static CSimpleQ<CSharedEntityObj *> EntityRetryQueue;
    EntityRetryQueue.SetSize(4);

    // Visit each entity in the linked list.  Update the entity and create a
    // CIGI message.
    unsigned long handle = NULL;
    CSharedEntityObj **shared_obj = EntityList.GetHead(&handle);
    ENTITY entity;
    while (handle && shared_obj) {
        if ((*shared_obj)->GetAndLock(&entity)) {
            BuildEntityControlPacketsWorker(&entity);
            (*shared_obj)->SetLocked(entity);
            (*shared_obj)->ClearLockedChangeFlag();
            (*shared_obj)->Unlock();
        } else
            EntityRetryQueue.Push(*shared_obj);

        shared_obj = EntityList.GetNext(&handle);
    }

    CSharedEntityObj *retry = NULL;
    while (EntityRetryQueue.Pop(&retry) >= 0) {
        if (retry->GetAndLock(&entity)) {
            BuildEntityControlPacketsWorker(&entity);
            retry->SetLocked(entity);
            retry->ClearLockedChangeFlag();
            retry->Unlock();
        } else
            EntityRetryQueue.Push(retry);
    }
}

void BuildEntityControlPacketsWorker(ENTITY *entity)
{
CIGI_ENTITY_CONTROL ec    = { sizeof(CIGI_ENTITY_CONTROL),    CIGI_ENTITY_CONTROL_OPCODE,    0, 0, 0, 0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0 };
CIGI_VELOCITY_CONTROL rc  = { sizeof(CIGI_VELOCITY_CONTROL),  CIGI_VELOCITY_CONTROL_OPCODE,  0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0 };
CIGI_ANIMATION_CONTROL ea = { sizeof(CIGI_ANIMATION_CONTROL), CIGI_ANIMATION_CONTROL_OPCODE, 0, 0, 0, 0,   0,   0,   0, 0.0 };
CIGI_ENTITY_POSITION ep   = { sizeof(CIGI_ENTITY_POSITION),   CIGI_ENTITY_POSITION_OPCODE,   0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0  };

	if (entity->cigi.type > -1 ) {
		if (entity->unsaved.entityctrl_has_changed ) {
			// If an animation has been started, set the state to Continue so
			// we don't keep starting it over and over.  Do this first so we
			// can unlock the object as soon as possible.
			int anim_state = entity->cigi.anim_state;
			if ((entity->noncigi.entity_class == ENTITY_CLASS_ANIMATION) &&
				(anim_state == ENTITY_ANIM_STATE_PLAY)) {
				entity->cigi.anim_state = ENTITY_ANIM_STATE_CONTINUE;
			}

			// Reset the changed flag.
			entity->unsaved.entityctrl_has_changed = 0;
			ec.entity_state = entity->cigi.active;
			ec.collision_detect = entity->cigi.coll_detect;
			ec.inherit_alpha = entity->cigi.inherit_alpha;
			ec.smoothing_enable = entity->cigi.smoothing;
			ec.extended_entity_type = entity->cigi.extended_type;
			ec.alpha = entity->cigi.alpha;
			ec.entity_id = (unsigned short)entity->cigi.id;
			if( entity->cigi.extended_type == 0 ) 
				ec.entity_country_type = ( uint16_t ) entity->cigi.type;
			else {
				ec.entity_kind = entity->cigi.entity_kind;
				ec.entity_domain = entity->cigi.entity_domain;
				ec.entity_country_type = entity->cigi.entity_country;
				ec.entity_category = entity->cigi.entity_category;
				ec.entity_subcategory = entity->cigi.entity_subcategory;
				ec.entity_specific = entity->cigi.entity_specific;
				ec.entity_extra = entity->cigi.entity_extra;
			}
			CigiAddFixedPacket<CIGI_ENTITY_CONTROL>(session, &ec);
		} // end if (entity->unsaved.cigi_has_changed && (entity->cigi.type > -1))


		if (entity->unsaved.entityposition_has_changed ) {
			entity->unsaved.entityposition_has_changed = 0;
			ep.entity_id = (unsigned short)entity->cigi.id;
			ep.clamp_mode = entity->cigi.clamp_mode;

			if (entity->cigi.parent_id >= 0) {
				ep.attach_state = 1;
				ep.parent_id = (unsigned short)entity->cigi.parent_id;
				ep.roll = (float)entity->cigi.dofs.rel_roll;
				ep.pitch = (float)entity->cigi.dofs.rel_pitch;
				ep.yaw = (float)entity->cigi.dofs.rel_yaw;
				ep.lat_x = entity->cigi.dofs.offset_x;
				ep.lon_y = entity->cigi.dofs.offset_y;
				ep.alt_z = entity->cigi.dofs.offset_z;
			} else {
				ep.attach_state = 0;
				ep.parent_id = (unsigned short)0;
				ep.roll = (float)entity->cigi.dofs.roll;
				ep.pitch = (float)entity->cigi.dofs.pitch;
				ep.yaw = (float)entity->cigi.dofs.yaw;
				ep.lat_x = entity->cigi.dofs.latitude;
				ep.lon_y = entity->cigi.dofs.longitude;
				ep.alt_z = entity->cigi.dofs.altitude;
			}
			CigiAddFixedPacket<CIGI_ENTITY_POSITION>(session, &ep);
		} // end if (entity->unsaved.entityposition_has_changed )

		if (entity->unsaved.entityanimation_has_changed ) {
			ea.animation_state = entity->cigi.anim_state;
			ea.animation_frame_reset = 1; // [continue|reset]
			ea.animation_loop_mode = entity->cigi.anim_loop;
			ea.inherit_alpha = entity->cigi.inherit_alpha;
			ea.alpha = entity->cigi.alpha;
			ea.entity_id = (unsigned short) entity->cigi.id;
			ea.animation_id = (unsigned short) entity->cigi.type;
			ea.alpha = entity->cigi.alpha;
			ea.animation_speed = 1.0;
		} //end if (entity->unsaved.entityanimation_has_changed )
			// Send the rate packet if we need to.
		if (entity->noncigi.send_rates) {
			rc.entity_id = (unsigned short)entity->cigi.id;
			rc.artpart_id = 0;
			rc.apply_artpart = 0;
			rc.coord_system = (igc.minor_version < 2) ? 0 : entity->noncigi.rates_coordsys;
			rc.x_rate = (float)(entity->noncigi.speed * cos(DegToRad(entity->cigi.dofs.yaw)));
			rc.y_rate = (float)(entity->noncigi.speed * sin(DegToRad(entity->cigi.dofs.yaw)));
			rc.z_rate = (float)(entity->noncigi.speed * sin(DegToRad(entity->cigi.dofs.pitch)));
			rc.roll_rate = (float)entity->noncigi.rates.droll;      // deg/s
			rc.pitch_rate = (float)entity->noncigi.rates.dpitch;    // deg/s
			rc.yaw_rate = (float)entity->noncigi.rates.dyaw;    // deg/s

			CigiAddFixedPacket<CIGI_VELOCITY_CONTROL>(session, &rc);
		} // end if (entity->noncigi.send_rates)

	} // end if (entity->cigi.type > -1 )

    // If the entity is inactive, we should delete it here after we create
    // the cigi message so we don't have to do a special search elsewhere.
    if (entity->unsaved.destroy)
        DeleteEntity(entity->cigi.id);
}

void BuildViewPackets(void)
{
    void BuildViewPacketsWorker(VIEW * view);
    void BuildViewGroupPacketsWorker(VIEWGROUP * group);

    // Create a queue in case we have to retry any entities.
    static CSimpleQ<CSharedViewObj *> ViewRetryQueue;
    static CSimpleQ<CSharedViewGroupObj *> ViewGroupRetryQueue;
    ViewRetryQueue.SetSize(4);
    ViewGroupRetryQueue.SetSize(4);

    // Visit each view in the linked list.  Update the view and create a
    // CIGI message.
    unsigned long handle = NULL;
    CSharedViewObj **view_obj = ViewList.GetHead(&handle);
    VIEW view;
    while (handle && view_obj) {
        if ((*view_obj)->GetAndLock(&view)) {
            BuildViewPacketsWorker(&view);
            (*view_obj)->SetLocked(view);
            (*view_obj)->ClearLockedChangeFlags();
            (*view_obj)->Unlock();
        } else
            ViewRetryQueue.Push(*view_obj);

        view_obj = ViewList.GetNext(&handle);
    }

    // Visit each view group in the linked list.  Update the view and create a
    // CIGI message.
    handle = NULL;
    CSharedViewGroupObj **group_obj = ViewGroupList.GetHead(&handle);
    VIEWGROUP group;
    while (handle && group_obj) {
        if ((*group_obj)->GetAndLock(&group)) {
            BuildViewGroupPacketsWorker(&group);
            (*group_obj)->SetLocked(group);
            (*group_obj)->ClearLockedChangeFlag();
            (*group_obj)->Unlock();
        } else
            ViewGroupRetryQueue.Push(*group_obj);

        group_obj = ViewGroupList.GetNext(&handle);
    }

    // We should not have both a view and a view group locked at the
    // same time.
    CSharedViewObj *ViewRetry = NULL;
    while (ViewRetryQueue.Pop(&ViewRetry) >= 0) {
        if (ViewRetry->GetAndLock(&view)) {
            BuildViewPacketsWorker(&view);
            ViewRetry->SetLocked(view);
            ViewRetry->ClearLockedChangeFlags();
            ViewRetry->Unlock();
        } else
            ViewRetryQueue.Push(ViewRetry);
    }

    CSharedViewGroupObj *GroupRetry = NULL;
    while (ViewGroupRetryQueue.Pop(&GroupRetry) >= 0) {
        if (GroupRetry->GetAndLock(&group)) {
            BuildViewGroupPacketsWorker(&group);
            GroupRetry->SetLocked(group);
            GroupRetry->ClearLockedChangeFlag();
            GroupRetry->Unlock();
        } else
            ViewGroupRetryQueue.Push(GroupRetry);
    }
}

void BuildViewPacketsWorker(VIEW *view)
{
    // Don't make these static because we want them initialized every time.
    CIGI_VIEW_DEF vd = {    CIGI_VIEW_DEF_OPCODE,
                            sizeof(CIGI_VIEW_DEF),
                            0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0.0,
                            0.0, 0.0, 0.0, 0.0, 0.0
                       };

    CIGI_VIEW_CONTROL vc = {    CIGI_VIEW_CONTROL_OPCODE,
                                sizeof(CIGI_VIEW_CONTROL),
                                0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0.0, 0.0, 0.0, 0.0, 0.0, 0.0
                           };

    // Send a View Definition packet if necessary.
    if (view->noncigi.def_has_changed) {
        if (verbose)
            printf("View definition has changed\n");

        vd.view_id = view->viewdef.view_id;
        vd.group_id = view->viewdef.group_id;
        vd.view_type = view->viewdef.view_type;
        vd.projection_type = view->viewdef.projection;
        vd.pixel_replication_mode = view->viewdef.replication;
        vd.mirror_mode = view->viewdef.mirror_mode;
        vd.reorder = view->viewdef.reorder;
        vd.near_enable = view->viewdef.fov_near_enable;
        vd.fov_near = view->viewdef.fov_near;
        vd.far_enable = view->viewdef.fov_far_enable;
        vd.fov_far = view->viewdef.fov_far;
        vd.left_enable = view->viewdef.fov_left_enable;
        vd.fov_left = view->viewdef.fov_left;
        vd.right_enable = view->viewdef.fov_right_enable;
        vd.fov_right = view->viewdef.fov_right;
        vd.top_enable = view->viewdef.fov_top_enable;
        vd.fov_top = view->viewdef.fov_top;
        vd.bottom_enable = view->viewdef.fov_bottom_enable;
        vd.fov_bottom = view->viewdef.fov_bottom;

        CigiAddFixedPacket<CIGI_VIEW_DEF>(session, &vd);
    }

    // Send a View Control packet if necessary.
    if (view->noncigi.ctrl_has_changed) {
        if (verbose)
            printf("View control has changed\n");

        vc.view_id = view->viewdef.view_id; // ID is in viewdef
        vc.group_id = 0;                    // packet is applied to view
        vc.entity_id = view->viewctrl.entity_id;
        vc.xoffset_enable = view->viewctrl.offset_x_enable;
        vc.xoffset = view->viewctrl.offset_x;
        vc.yoffset_enable = view->viewctrl.offset_y_enable;
        vc.yoffset = view->viewctrl.offset_y;
        vc.zoffset_enable = view->viewctrl.offset_z_enable;
        vc.zoffset = view->viewctrl.offset_z;
        vc.roll_enable = view->viewctrl.roll_enable;
        vc.roll = view->viewctrl.roll;
        vc.pitch_enable = view->viewctrl.pitch_enable;
        vc.pitch = view->viewctrl.pitch;
        vc.yaw_enable = view->viewctrl.yaw_enable;
        vc.yaw = view->viewctrl.yaw;

        CigiAddFixedPacket<CIGI_VIEW_CONTROL>(session, &vc);

        view->viewctrl.offset_x_enable = 0;
        view->viewctrl.offset_y_enable = 0;
        view->viewctrl.offset_z_enable = 0;
        view->viewctrl.yaw_enable = 0;
        view->viewctrl.pitch_enable = 0;
        view->viewctrl.roll_enable = 0;
    }

    // Reset the dirty flags and the individual valid flags.
    view->noncigi.def_has_changed = 0;
    view->noncigi.ctrl_has_changed = 0;
    view->viewdef.fov_near_enable = 0;
    view->viewdef.fov_far_enable = 0;
    view->viewdef.fov_left_enable = 0;
    view->viewdef.fov_right_enable = 0;
    view->viewdef.fov_top_enable = 0;
    view->viewdef.fov_bottom_enable = 0;
    view->viewctrl.offset_x_enable = 0;
    view->viewctrl.offset_y_enable = 0;
    view->viewctrl.offset_z_enable = 0;
    view->viewctrl.yaw_enable = 0;
    view->viewctrl.pitch_enable = 0;
    view->viewctrl.roll_enable = 0;
}

void BuildViewGroupPacketsWorker(VIEWGROUP *group)
{
    // Don't make these static because we want them initialized every time.
    CIGI_VIEW_CONTROL vc = {    CIGI_VIEW_CONTROL_OPCODE,
                                sizeof(CIGI_VIEW_CONTROL),
                                0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0.0, 0.0, 0.0, 0.0, 0.0, 0.0
                           };



    if (group->noncigi.cigi_viewgroup_has_changed) {
        if (verbose)
            printf("View group has changed\n");

        vc.view_id = 0;                     // packet applied to group
        vc.group_id = group->cigi.group_id;
        vc.entity_id = group->cigi.entity_id;
        vc.xoffset_enable = group->cigi.offset_x_enable;
        vc.xoffset = group->cigi.offset_x;
        vc.yoffset_enable = group->cigi.offset_y_enable;
        vc.yoffset = group->cigi.offset_y;
        vc.zoffset_enable = group->cigi.offset_z_enable;
        vc.zoffset = group->cigi.offset_z;
        vc.roll_enable = group->cigi.roll_enable;
        vc.roll = group->cigi.roll;
        vc.pitch_enable = group->cigi.pitch_enable;
        vc.pitch = group->cigi.pitch;
        vc.yaw_enable = group->cigi.yaw_enable;
        vc.yaw = group->cigi.yaw;

        CigiAddFixedPacket<CIGI_VIEW_CONTROL>(session, &vc);

        // Reset the changed flag and the individual valid flags.
        group->noncigi.cigi_viewgroup_has_changed = 0;
        group->cigi.offset_x_enable = 0;
        group->cigi.offset_y_enable = 0;
        group->cigi.offset_z_enable = 0;
        group->cigi.yaw_enable = 0;
        group->cigi.pitch_enable = 0;
        group->cigi.roll_enable = 0;
    }
}

void UpdateEntities(double dt)
{
    // Create a queue in case we have to retry any entities.
    static CSimpleQ<CSharedEntityObj *> EntityRetryQueue;
    EntityRetryQueue.SetSize(4);

    // Visit each entity in the linked list and update the entity.
    unsigned long handle = NULL;
    CSharedEntityObj **shared_obj = EntityList.GetHead(&handle);
    ENTITY entity;
    while (handle && shared_obj && RunState) {
        if ((*shared_obj)->GetAndLock(&entity)) {
            // Update the object.
            if (UpdateEntityPosition(&entity, dt))
                (*shared_obj)->SetLockedEntityPosition(entity);

            (*shared_obj)->Unlock();
        } else
            EntityRetryQueue.Push(*shared_obj);

        shared_obj = EntityList.GetNext(&handle);
    }

    // Retry any entities that were locked.
    CSharedEntityObj *retry = NULL;
    while (RunState && (EntityRetryQueue.Pop(&retry) >= 0)) {
        if (retry->GetAndLock(&entity)) {
            // Update the object.
            if (UpdateEntityPosition(&entity, dt))
                retry->SetLockedEntityPosition(entity);

            retry->Unlock();
        } else
            EntityRetryQueue.Push(retry);
    }
}

void ClearEntities(void)
{
    // Visit each entity in the linked list and delete it.
    unsigned long handle = NULL;
    CSharedEntityObj **shared_obj = EntityList.GetHead(&handle);

    while (handle && shared_obj) {
        CSharedEntityObj *temp = *shared_obj;
        EntityList.Remove(&handle);
        delete temp;

        shared_obj = EntityList.GetNext(&handle);
    }
}

void ClearViews(void)
{
    // Visit each view in the linked list and delete it.
    unsigned long handle = NULL;
    CSharedViewObj **shared_obj = ViewList.GetHead(&handle);

    while (handle && shared_obj) {
        CSharedViewObj *temp = *shared_obj;
        ViewList.Remove(&handle);
        delete temp;

        shared_obj = ViewList.GetNext(&handle);
    }
}

void ClearViewGroups(void)
{
    // Visit each view group in the linked list and delete it.
    unsigned long handle = NULL;
    CSharedViewGroupObj **shared_obj = ViewGroupList.GetHead(&handle);

    while (handle && shared_obj) {
        CSharedViewGroupObj *temp = *shared_obj;
        ViewGroupList.Remove(&handle);
        delete temp;

        shared_obj = ViewGroupList.GetNext(&handle);
    }
}

CSharedEntityObj *FindEntity(const int id, unsigned long *handle)
{
    ENTITY_CIGI_DATA data = {0};
    CSharedEntityObj **shared_obj = EntityList.GetHead(handle);

    // Create a queue in case we have to retry any entities.
    static CSimpleQ<CSharedEntityObj *> EntityRetryQueue;
    EntityRetryQueue.SetSize(4);

    while (*handle && shared_obj) {
        if ((*shared_obj)->GetAndLockCigiData(&data)) {
            (*shared_obj)->Unlock();

            if (data.id == id) {
                return *shared_obj;
            }
        } else
            EntityRetryQueue.Push(*shared_obj);


        shared_obj = EntityList.GetNext(handle);
    }

    CSharedEntityObj *retry = NULL;
    while (EntityRetryQueue.Pop(&retry) >= 0) {
        if (retry->GetAndLockCigiData(&data)) {
            retry->Unlock();

            if (data.id == id) {
                return retry;
            }
        } else
            EntityRetryQueue.Push(retry);
    }

    return NULL;
}

CSharedViewObj *FindView(const int id, unsigned long *handle)
{
    VIEW_DEF_DATA data = {0};
    CSharedViewObj **shared_obj = ViewList.GetHead(handle);

    // Create a queue in case we have to retry any entities.
    static CSimpleQ<CSharedViewObj *> ViewRetryQueue;
    ViewRetryQueue.SetSize(4);

    *handle = NULL;

    while (*handle && shared_obj) {
        if ((*shared_obj)->GetAndLockViewDef(&data)) {
            (*shared_obj)->Unlock();

            if (data.view_id == (unsigned int)id) {
                return *shared_obj;
            }

        } else
            ViewRetryQueue.Push(*shared_obj);

        shared_obj = ViewList.GetNext(handle);
    }

    CSharedViewObj *retry = NULL;
    while (ViewRetryQueue.Pop(&retry) >= 0) {
        if (retry->GetAndLockViewDef(&data)) {
            retry->Unlock();

            if (data.view_id == (unsigned int)id) {
                return retry;
            }

        } else
            ViewRetryQueue.Push(retry);
    }

    return NULL;
}

CSharedViewGroupObj *FindViewGroup(const int id, unsigned long *handle)
{
    VIEWGROUP_CIGI_DATA data = {0};
    CSharedViewGroupObj **shared_obj = ViewGroupList.GetHead(handle);

    // Create a queue in case we have to retry any entities.
    static CSimpleQ<CSharedViewGroupObj *> ViewGroupRetryQueue;
    ViewGroupRetryQueue.SetSize(4);

    *handle = NULL;

    while (*handle && shared_obj) {
        if ((*shared_obj)->GetAndLockCigi(&data)) {
            (*shared_obj)->Unlock();

            if (data.group_id = (unsigned int)id) {
                return *shared_obj;
            }

        } else
            ViewGroupRetryQueue.Push(*shared_obj);

        shared_obj = ViewGroupList.GetNext(handle);
    }

    CSharedViewGroupObj *retry = NULL;
    while (ViewGroupRetryQueue.Pop(&retry) >= 0) {
        if (retry->GetAndLockCigi(&data)) {
            retry->Unlock();

            if (data.group_id == (unsigned int)id) {
                return retry;
            }

        } else
            ViewGroupRetryQueue.Push(retry);
    }

    return NULL;
}

int DeleteEntity(int id)
{
    unsigned long handle = NULL;
    CSharedEntityObj *entity = FindEntity(id, &handle);

    if (entity)
        delete entity;

    int numleft = EntityList.Remove(&handle);

    return numleft;
}

int DeleteView(int id)
{
    unsigned long handle = NULL;
    CSharedViewObj *view = FindView(id, &handle);

    if (view)
        delete view;

    int numleft = ViewList.Remove(&handle);

    return numleft;
}

int DeleteViewGroup(int id)
{
    unsigned long handle = NULL;
    CSharedViewGroupObj *group = FindViewGroup(id, &handle);

    if (group)
        delete group;

    int numleft = ViewGroupList.Remove(&handle);

    return numleft;
}

void AddHAT(MESSAGE_ADD_HAT *msg)
{
    HAT_REQUEST hat;
    unsigned long hdl;

    hat.identity.id = msg->id_base << 6;
    hat.identity.owner_id = msg->entity_id;
    hat.req_type = msg->request_type;
    hat.coord_system = msg->coord_system;
    hat.continuous = msg->continuous;
    hat.lat = msg->lat;
    hat.lon = msg->lon;
    hat.alt = msg->alt;
    hat.x_offset = msg->x_offset;
    hat.y_offset = msg->y_offset;
    hat.z_offset = msg->z_offset;

    // Try to find an existing list element with a matching ID base.
    if (hdl = FindHAT(msg->id_base)) {
        // If we found one, update it.
        *(HATRequestList.GetItem(&hdl)) = hat;
    } else {
        // If not, create a new element.
        HATRequestList.InsertTail(hat);
    }
}

unsigned long FindHAT(const int id)
{
    unsigned long handle = 0;
    HAT_REQUEST *hat = HATRequestList.GetHead(&handle);
    while (handle) {
        if (int(hat->identity.id >> 6) == id)
            return handle;

        hat = HATRequestList.GetNext(&handle);
    }

    return 0;
}

void AddHATPackets(void)
{
    void AddHATPacketsWorker(const ENTITY & entity, HAT_REQUEST * hat);
    void BuildHATPacket(const HAT_REQUEST * hat);

    // Create a queue in case we have to retry any entities.
    static CSimpleQ<HAT_REQUEST *> HATRetryQueue;
    HATRetryQueue.SetSize(32);

    unsigned long ent_hdl = 0;
    unsigned long handle = 0;
    CSharedEntityObj **shared_obj;
    ENTITY entity;
    HAT_REQUEST *hat;

    if (!RunState)
        return;

    hat = HATRequestList.GetHead(&handle);
    while (hat) {
        // If we are associating the HAT request to an entity, find the entity
        // and update the HAT's source lat, lon, and alt.  Otherwise, this is
        // an arbitrary point specified by lat, lon, and alt.
        if (hat->identity.owner_id >= 0) {  // Entity ID
            shared_obj = EntityList.GetHead(&ent_hdl);
            while (ent_hdl && shared_obj) {
                if ((*shared_obj)->GetAndLock(&entity)) {
                    (*shared_obj)->Unlock();
                    if (entity.cigi.id == hat->identity.owner_id) {
                        AddHATPacketsWorker(entity, hat);
                        break;
                    }
                    shared_obj = EntityList.GetNext(&ent_hdl);
                } else {
                    HATRetryQueue.Push(hat);
                }
            }

            // If the entity doesn't exist, we have deleted it.  Delete the
            // HAT request from the list.
            if (!(shared_obj)) {
                HATRequestList.Remove(&handle);
                continue;
            }
        }

        BuildHATPacket(hat);

        // Continuous requests.
        if (hat->continuous > 0) {
            // Increment the ID.
            hat->identity.id = ((hat->identity.id + 1) & 0x3F) | (hat->identity.id & 0xFFC0);
        } else {
            // One-shot
            HATRequestList.Remove(&handle);
        }

        // Get the next HAT request.
        hat = HATRequestList.GetNext(&handle);
    }

    // Now retry any entities in the queue.
    while (HATRetryQueue.Pop(&hat) >= 0) {
        // If we are associating the HAT request to an entity, find the entity
        // and update the HAT's source lat, lon, and alt.  Otherwise, this is
        // an arbitrary point specified by lat, lon, and alt.
        if (hat->identity.owner_id >= 0) {  // Entity ID
            shared_obj = EntityList.GetHead(&ent_hdl);
            while (ent_hdl && shared_obj) {
                if ((*shared_obj)->GetAndLock(&entity)) {
                    (*shared_obj)->Unlock();
                    if (entity.cigi.id == hat->identity.owner_id) {
                        AddHATPacketsWorker(entity, hat);
                        break;
                    }
                    shared_obj = EntityList.GetNext(&ent_hdl);
                } else {
                    HATRetryQueue.Push(hat);
                }
            }

            // If the entity doesn't exist, we have deleted it.  Delete the
            // HAT request from the list.
            if (!(shared_obj)) {
                HATRequestList.Remove(&handle);
                continue;
            }
        }

        BuildHATPacket(hat);

        // Continuous requests
        if (hat->continuous > 0) {
            // Increment the ID.
            hat->identity.id = ((hat->identity.id + 1) & 0x3F) | (hat->identity.id & 0xFFC0);
        } else {
            // One-shot
            HATRequestList.Remove(&handle);
        }
    }
}

void AddHATPacketsWorker(const ENTITY &entity, HAT_REQUEST *hat)
{
    // Add the offsets to the reference entity's coordinates.
    double x = hat->x_offset;
    double y = hat->y_offset;
    double dist;
    double angle;
    if (x == 0.0) {
        dist = fabs(y);
        if (y > 0)
            angle = 90.0;
        else if (y < 0)
            angle = -90.0;
        else
            angle = 0.0;
    } else {
        dist = sqrt(x * x + y * y);
        angle = RadToDeg(atan(DegToRad(y) / DegToRad(x)));

        if (x < 0)
            angle = 180.0 + angle;
    }

    waypoint(&g_ERM, entity.cigi.dofs.latitude,
             entity.cigi.dofs.longitude, dist,
             entity.cigi.dofs.yaw + angle,
             &(hat->lat), &(hat->lon));

    // The Z offset is easy.
    hat->alt = entity.cigi.dofs.altitude + hat->z_offset;
}

void BuildHATPacket(const HAT_REQUEST *hat)
{
    static CIGI_HAT_HOT_REQUEST packet = {  CIGI_HAT_HOT_REQUEST_OPCODE,
                                            sizeof(CIGI_HAT_HOT_REQUEST),
                                            0, 0, 0, 0, 0, 0.0, 0.0
                                         };

    // Build and send the packet.
    packet.hat_hot_id = hat->identity.id;
    packet.request_type = hat->req_type;
    packet.lat_x = hat->lat;
    packet.lon_y = hat->lon;
    packet.alt_z = hat->alt;
    CigiAddFixedPacket<CIGI_HAT_HOT_REQUEST>(session, &packet);
}

void AddLOS(MESSAGE_ADD_LOS *msg)
{
    LOS_REQUEST los;
    unsigned long hdl;

    los.identity.id = msg->id_base << 6;
    los.identity.owner_id = msg->entity_id;
    los.req_type = msg->req_type;
    los.ext_response = msg->ext_response;
    los.src_coord_system = msg->src_coord_system;
    los.dest_coord_system = msg->dest_coord_system;
    los.resp_coord_system = msg->resp_coord_system;
    los.continuous = msg->continuous;
    los.use_target_entity = msg->use_target_entity;
    los.alpha_thresh = msg->alpha_thresh;
    los.target_entity_id = msg->target_entity_id;
    los.material_mask = msg->material_mask;
    los.azimuth = msg->azimuth;
    los.elevation = msg->elevation;
    los.min_range = msg->min_range;
    los.max_range = msg->max_range;
    los.src_lat = msg->src_lat;
    los.src_lon = msg->src_lon;
    los.src_alt = msg->src_alt;
    los.src_x_offset = msg->src_x_offset;
    los.src_y_offset = msg->src_y_offset;
    los.src_z_offset = msg->src_z_offset;
    los.dest_lat_x = msg->dest_lat_x;
    los.dest_lon_y = msg->dest_lon_y;
    los.dest_alt_z = msg->dest_alt_z;

    // Try to find an existing list element with a matching ID base.
    if (hdl = FindLOS(msg->id_base)) {
        // If we found one, update it.
        *(LOSRequestList.GetItem(&hdl)) = los;
    } else {
        // If not, create a new element.
        LOSRequestList.InsertTail(los);
    }
}

unsigned long FindLOS(const int id)
{
    unsigned long handle = 0;
    LOS_REQUEST *los = LOSRequestList.GetHead(&handle);

    while (handle) {
        if (int(los->identity.id >> 6) == id)
            return handle;

        los = LOSRequestList.GetNext(&handle);
    }

    return 0;
}

void AddLOSPackets(void)
{
    void AddLOSPacketsWorker(const ENTITY & entity, LOS_REQUEST * los);
    void BuildLOSPacket(const LOS_REQUEST * los);

    // Create a queue in case we have to retry any entities.
    static CSimpleQ<LOS_REQUEST *> LOSRetryQueue;
    LOSRetryQueue.SetSize(32);

    unsigned long ent_hdl = 0;
    unsigned long handle = 0;
    CSharedEntityObj **shared_obj;
    ENTITY entity;
    LOS_REQUEST *los;

    if (!RunState)
        return;

    los = LOSRequestList.GetHead(&handle);
    while (los) {
        // If we are associating the LOS request to an entity, find the entity
        // and update the LOS's source lat, lon, and alt.  Otherwise, this is
        // an arbitrary point specified by lat, lon, and alt.
        if (los->identity.owner_id >= 0) {  // Entity ID
            shared_obj = EntityList.GetHead(&ent_hdl);
            while (ent_hdl && shared_obj) {
                if ((*shared_obj)->GetAndLock(&entity)) {
                    (*shared_obj)->Unlock();
                    if (entity.cigi.id == los->identity.owner_id) {
                        AddLOSPacketsWorker(entity, los);
                        break;
                    }
                    shared_obj = EntityList.GetNext(&ent_hdl);
                } else {
                    LOSRetryQueue.Push(los);
                }
            }

            // If the entity doesn't exist, we have deleted it.  Delete the
            // LOS request from the list.
            if (!(shared_obj)) {
                LOSRequestList.Remove(&handle);

                // LWD: send a message to the GUI to delete.

                continue;
            }
        }

        BuildLOSPacket(los);

        if (los->continuous > 0) {  // Continuous requests
            // Increment the ID.
            los->identity.id = ((los->identity.id + 1) & 0x3F) |
                               (los->identity.id & 0xFFC0);
        } else {                    // One-shot
            LOSRequestList.Remove(&handle);
        }

        // Get the next LOS request.
        los = LOSRequestList.GetNext(&handle);
    }

    // Now retry any entities in the queue.
    while (LOSRetryQueue.Pop(&los) >= 0) {
        // If we are associating the LOS request to an entity, find the entity
        // and update the LOS's source lat, lon, and alt.  Otherwise, this is
        // an arbitrary point specified by lat, lon, and alt.
        if (los->identity.owner_id >= 0) {  // Entity ID
            shared_obj = EntityList.GetHead(&ent_hdl);
            while (ent_hdl && shared_obj) {
                if ((*shared_obj)->GetAndLock(&entity)) {
                    (*shared_obj)->Unlock();
                    if (entity.cigi.id == los->identity.owner_id) {
                        AddLOSPacketsWorker(entity, los);
                        break;
                    }
                    shared_obj = EntityList.GetNext(&ent_hdl);
                } else {
                    LOSRetryQueue.Push(los);
                }
            }

            // If the entity doesn't exist, we have deleted it.  Delete the
            // LOS request from the list.
            if (!(shared_obj)) {
                LOSRequestList.Remove(&handle);

                // LWD: send a message to the GUI to delete.

                continue;
            }
        }

        BuildLOSPacket(los);

        if (los->continuous > 0) {  // Continuous requests
            // Increment the ID.
            los->identity.id = ((los->identity.id + 1) & 0x3F) |
                               (los->identity.id & 0xFFC0);
        } else {                    // One-shot
            LOSRequestList.Remove(&handle);
        }

        // Get the next LOS request.
        los = LOSRequestList.GetNext(&handle);
    }
}

void AddLOSPacketsWorker(const ENTITY &entity, LOS_REQUEST *los)
{
    // Add the offsets to the reference entity's coordinates.
    double x = los->src_x_offset;
    double y = los->src_y_offset;
    double dist;
    double angle;
    if (x == 0.0) {
        dist = fabs(y);
        if (y > 0)
            angle = 90.0;
        else if (y < 0)
            angle = -90.0;
        else
            angle = 0.0;
    } else {
        dist = sqrt(x * x + y * y);
        angle = RadToDeg(atan(DegToRad(y) / DegToRad(x)));

        if (x < 0)
            angle = 180.0 + angle;
    }

    waypoint(&g_ERM, entity.cigi.dofs.latitude,
             entity.cigi.dofs.longitude, dist,
             entity.cigi.dofs.yaw + angle,
             &(los->src_lat), &(los->src_lon));

    // The Z offset is easy.
    los->src_alt = entity.cigi.dofs.altitude + los->src_z_offset;

    // If we have a reference target entity, use it.
    if (los->use_target_entity && (los->req_type == LOS_TYPE_SEGMENT)) {
        unsigned long target_hdl = NULL;
        CSharedEntityObj *target = FindEntity(los->target_entity_id, &target_hdl);
        if (target) {
            ENTITY_CIGI_DATA target_data = {0};
            while (target->GetAndLockCigiData(&target_data) == FALSE)
                continue;
            target->Unlock();
            los->dest_lat_x = target_data.dofs.latitude;
            los->dest_lon_y = target_data.dofs.longitude;
            los->dest_alt_z = target_data.dofs.altitude;
            los->dest_coord_system = 1;
        }
    } else
        los->dest_coord_system = 0;
}

void BuildLOSPacket(const LOS_REQUEST *los)
{
    static CIGI_LOS_SEGMENT_REQUEST seg_packet = {  CIGI_LOS_SEGMENT_REQUEST_OPCODE,
                                                    sizeof(CIGI_LOS_SEGMENT_REQUEST),
                                                    // chas 0, 0, 0, 0, 0, 0, 0, 0, 0.0,
                                                    //chas 0.0, 0.0, 0.0, 0.0, 0, 0
                                                 };
    static CIGI_LOS_VECTOR_REQUEST vec_packet = {   CIGI_LOS_VECTOR_REQUEST_OPCODE,
                                                    sizeof(CIGI_LOS_VECTOR_REQUEST),
                                                    // chas 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0,
                                                    // chas 0.0, 0.0, 0.0, 0, 0
                                                };
    // Build and send the packet.
    if (los->req_type == LOS_TYPE_SEGMENT) {
        seg_packet.los_id = los->identity.id;
// chas        seg_packet.entity_id = los->identity.owner_id;
        seg_packet.source_lat_x = los->src_lat;
        seg_packet.source_lon_y = los->src_lon;
        seg_packet.source_alt_z = los->src_alt;
        seg_packet.request_type = los->ext_response;
        seg_packet.source_coord_system = los->src_coord_system;
        seg_packet.dest_coord_system = los->dest_coord_system;
        seg_packet.response_coord_system = los->resp_coord_system;
        seg_packet.alpha_threshold = los->alpha_thresh;
        seg_packet.source_lat_x = los->src_lat;
        seg_packet.source_lon_y = los->src_lon;
        seg_packet.source_alt_z = los->src_alt;
        seg_packet.dest_lat_x = los->dest_lat_x;
        seg_packet.dest_lon_y = los->dest_lon_y;
        seg_packet.dest_alt_z = los->dest_alt_z;
        seg_packet.material_mask = los->material_mask;
        CigiAddFixedPacket<CIGI_LOS_SEGMENT_REQUEST>(session, &seg_packet);
    } else {
        vec_packet.los_id = los->identity.id;
        vec_packet.entity_id = los->identity.owner_id;
        vec_packet.source_lat_x = los->src_lat;
        vec_packet.source_lon_y = los->src_lon;
        vec_packet.source_alt_z = los->src_alt;
        vec_packet.request_type = los->ext_response;
        vec_packet.source_coord_system = los->src_coord_system;
        vec_packet.resp_coord_system = los->resp_coord_system;
        vec_packet.alpha_threshold = los->alpha_thresh;
        vec_packet.azimuth = los->azimuth;
        vec_packet.elevation = los->elevation;
        vec_packet.min_range = los->min_range;
        vec_packet.max_range = los->max_range;
        vec_packet.source_lat_x = los->src_lat;
        vec_packet.source_lon_y = los->src_lon;
        vec_packet.source_alt_z = los->src_alt;
        vec_packet.material_mask = los->material_mask;
        CigiAddFixedPacket<CIGI_LOS_VECTOR_REQUEST>(session, &vec_packet);
    }
}

void AddPacketsFromQueue(SharedBufferQueue *queue)
{
    static char buffer[CIGI_MAX_PACKET_SIZE];
    static int size = 0;

    do {
        if (size = queue->Pop((char *)buffer, CIGI_MAX_PACKET_SIZE)) {
            if (ProtocolAdapter->IsFrameBoundaryPacket(
                    (const unsigned char *)buffer, size)) {
                // If we have sent a SOF, we know we have a FRAME keyword
                // in a script file.
                if (verbose)
                    printf("AddPacketsFromQueue():  Encountered a FRAME script command.\n");

                // Set the event so the driver knows when it's okay to continue
                // parsing the script file.
                SetEvent(g_FrameWaitEventHdl);

                return;
            }

            if (!ProtocolAdapter->AddLegacyQueuedPacket(
                    session, (const unsigned char *)buffer, size)) {
                if (verbose)
                    printf("Unknown Packet (ID = %d).\n",
                           ProtocolAdapter->GetPacketId(
                               (const unsigned char *)buffer, size));
            }
        }
    } while (size);
}

void AddImmediateQueuedPackets(void)
{
    AddPacketsFromQueue(&g_SendImmedCIGIMsgQueue);
}

void AddQueuedPackets(void)
{
    AddPacketsFromQueue(&g_SendCIGIMsgQueue);
}

void StartTranspDelayTest(MESSAGE_TDTEST_START *msg)
{
    unsigned long handle = NULL;
    if (FindEntity(0, &handle)) {
        OperateMode = MODE_TRANSPDELAYTEST;

        TranspDelayLoopCounter = 0;
        TranspDelayCount1 = msg->count1;
        TranspDelayCount2 = msg->count2;

        // Populate the first SIX_DOF structure.  We will send Entity
        // Control packets containing this information.
        double tm[3] = {0.0};
        double gdc[3] = { msg->lat1, msg->lon1, msg->alt1 };

        convertGDCtoTM(&g_ERM, gdc, tm);

        TranspDelayDOF1.offset_x = tm[1];
        TranspDelayDOF1.offset_y = tm[0];
        TranspDelayDOF1.offset_z = msg->alt1;
        TranspDelayDOF1.latitude = msg->lat1;
        TranspDelayDOF1.longitude = msg->lon1;
        TranspDelayDOF1.altitude = msg->alt1;
        TranspDelayDOF1.yaw = msg->heading1;
        TranspDelayDOF1.roll = msg->roll1;
        TranspDelayDOF1.pitch = msg->pitch1;

        // Populate the second SIX_DOF structure.
        tm[0] = tm[1] = tm[2] = 0.0;
        gdc[0] = msg->lat2;
        gdc[1] = msg->lon2;
        gdc[2] = msg->alt2;

        convertGDCtoTM(&g_ERM, gdc, tm);

        TranspDelayDOF2.offset_x = tm[1];
        TranspDelayDOF2.offset_y = tm[0];
        TranspDelayDOF1.offset_z = msg->alt1;
        TranspDelayDOF2.latitude = msg->lat2;
        TranspDelayDOF2.longitude = msg->lon2;
        TranspDelayDOF2.altitude = msg->alt2;
        TranspDelayDOF2.yaw = msg->heading2;
        TranspDelayDOF2.roll = msg->roll2;
        TranspDelayDOF2.pitch = msg->pitch2;
    }
}

void StopTranspDelayTest(void)
{
    OperateMode = MODE_NORMAL;
}

CIGI_ENTITY_CONTROL *TranspDelayTestCreateECPacket(int posnum)
{
    static unsigned long handle = 0;
    static CIGI_ENTITY_CONTROL ec = {   CIGI_ENTITY_CONTROL_SIZE,     // packet_size
										CIGI_ENTITY_CONTROL_OPCODE,   // packet_id
                                        
                                        //0,                            // entity_id
                                        //1,                            // state
                                        //0,                            // attach_state
                                        //0,                            // collision_detect
                                        //0,                            // inherit_alpha
                                        //0,                            // clamp_mode
                                        //0, 0, 0,                      // anim_dir, anim_loop_mode, anim_state
                                        //255,                          // alpha
                                        //0,                            // type
                                        //-1,                           // parent_id
                                        //0.0f, 0.0f, 0.0f,             // roll, pitch, yaw
                                        //0.0, 0.0, 0.0                 // lat/x, lon/y, alt/z
                                    };

    CSharedEntityObj *ownship = FindEntity(0, &handle);
    if (ownship) {
        DOF *dof = NULL;
        if (posnum == 0)
            dof = &TranspDelayDOF1;
        else
            dof = &TranspDelayDOF2;

        ENTITY_CIGI_DATA entityState = ENTITY_CIGI_DATA();
        while (ownship->GetAndLockCigiData(&entityState) == FALSE)
            continue;
        ownship->Unlock();

#if 0
        // Create Entity Control packet for first position.
        ec.type = (unsigned short)(entityState.type);
        ec.alpha = entityState.alpha;
        ec.inherit_alpha = entityState.inherit_alpha;
        ec.roll = (float)dof->roll;
        ec.pitch = (float)dof->pitch;
        ec.yaw = (float)dof->yaw;
        ec.lat_x = dof->latitude;
        ec.lon_y = dof->longitude;
        ec.alt_z = dof->altitude;
#endif
    }

    return &ec;
}

long DoSOF(const int sessionid, void *packet)
{
    static unsigned int expected_fc = 0;
    CIGI_START_OF_FRAME *msg = (CIGI_START_OF_FRAME *)packet;
    static CIGI_DUMMY_PACKET dummy = {  sizeof(CIGI_DUMMY_PACKET), CIGI_DUMMY_PACKET_OPCODE, 0, 0 };

    int MinorVer = msg->minor_version;

    // Send a message to the GUI when minor version changes.
    if (MinorVer != CigiGetMinorVersion()) {
        MESSAGE_FORCE_MINOR_VERSION gui_msg;
        gui_msg.minor_version = MinorVer;
        g_DrvToGuiMsgQueue.Push((char *)&gui_msg, gui_msg.size);
    }

    // Check to see if we have skipped a frame.  We will NEVER have a
    // packet with an opcode of zero, so we can create a dummy packet
    // with this opcode.
    switch (MinorVer) {
    case 0:
    default:
        expected_fc = LastReceivedFrameCtr + 1;
        break;
    }

    LastReceivedFrameCtr = msg->frame_ctr;
    if (LastReceivedFrameCtr != expected_fc) {
        dummy.data1 = LastReceivedFrameCtr - expected_fc;
        g_RcvCIGIMsgQueue.Push((char *)&dummy, sizeof(CIGI_DUMMY_PACKET));
    }

    // Check the database number and set it to zero (no action) if we
    // receive a negated number (loading) or the same number (loaded).
    if (igc.db_number > 0) {
        if ((msg->db_number == igc.db_number) ||
            (msg->db_number == -igc.db_number)) {
            igc.db_number = 0;
            RunState = PreDbLoadRunState;
        }
    }

    // Send a message back to the GUI.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_START_OF_FRAME));

    return 0;
}

long DoHatHotResponse(const int sessionid, void *packet)
{
    CIGI_HAT_HOT_RESPONSE *msg = (CIGI_HAT_HOT_RESPONSE *)packet;

    if (verbose) {
        printf("HAT Response:\n");
        printf("  ID = %d\n", msg->hat_hot_id);
        printf("  Response Type = %d\n", msg->response_type);
        printf("  Valid = %d\n", msg->valid);
        printf("  Height = %lf\n", msg->height);
        printf("\n");
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_HAT_HOT_RESPONSE));

    return 0;
}

long DoHatHotExtResponse(const int sessionid, void *packet)
{
    CIGI_HAT_HOT_EXT_RESPONSE *msg = (CIGI_HAT_HOT_EXT_RESPONSE *)packet;

    if (verbose) {
        printf("HAT Extended Response:\n");
        printf("  ID = %d\n", msg->hat_hot_id);
        printf("  Valid = %d\n", msg->valid);
        printf("  HAT = %lf\n", msg->hat);
        printf("  HOT = %lf\n", msg->hot);
        printf("  Normal Azimuth = %f\n", msg->normal_azimuth);
        printf("  Normal Elevation = %f\n", msg->normal_elevation);
        printf("  Material Code = %u\n", msg->material_code);
        printf("\n");
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_HAT_HOT_EXT_RESPONSE));

    return 0;
}

long DoLOSResponse(const int sessionid, void *packet)
{
    CIGI_LOS_RESPONSE *msg = (CIGI_LOS_RESPONSE *)packet;

    if (verbose) {
        printf("LOS Response:\n");
        printf("  ID = %d\n", msg->los_id);
        printf("  Valid = %d\n", msg->valid);
        printf("  Visible = %d\n", msg->visible);
        printf("  Entity ID = %d\n", msg->entity_id);
        printf("  Entity ID Valid = %d\n", msg->entity_id_valid);
        printf("  Range = %lf\n", msg->range);
        printf("  Response Count = %d\n", msg->response_count);
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_LOS_RESPONSE));

    return 0;
}

long DoLOSExtResponse(const int sessionid, void *packet)
{
    CIGI_LOS_EXT_RESPONSE *msg = (CIGI_LOS_EXT_RESPONSE *)packet;

    if (verbose) {
        printf("LOS Extended Response:\n");
        printf("  ID = %d\n", msg->los_id);
        printf("  Valid = %d\n", msg->valid);
        printf("  Visible = %d\n", msg->visible);
        printf("  Entity ID = %d\n", msg->entity_id);
        printf("  Entity ID Valid = %d\n", msg->entity_id_valid);
        printf("  Latitude/X = %lf\n", msg->lat_x);
        printf("  Longitude/Y = %lf\n", msg->lon_y);
        printf("  Altitude/Z = %lf\n", msg->alt_z);
        printf("  Range = %lf\n", msg->range);
        printf("  Range Valid = %d\n", msg->range_valid);
        printf("  Normal Azimuth = %f\n", msg->normal_azimuth);
        printf("  Normal Elevation = %f\n", msg->normal_elevation);
        printf("  Material Code = %u\n", msg->material_code);
        printf("  RGBA = %d %d %d %d\n", msg->red, msg->green, msg->blue, msg->alpha);
        printf("  Response Count = %d\n", msg->response_count);
        printf("\n");
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_LOS_EXT_RESPONSE));

    return 0;
}

long DoSensorResponse(const int sessionid, void *packet)
{
    CIGI_SENSOR_RESPONSE *msg = (CIGI_SENSOR_RESPONSE *)packet;

    if (verbose) {
        printf("Sensor Response:\n");
        printf("  View ID = %d\n", msg->view_id);
        printf("  Sensor ID = %d\n", msg->sensor_id);
        printf("  Sensor Status = %d\n", msg->sensor_status);
        printf("  Gate Size X = %d\n", msg->gate_x_size);
        printf("  Gate Size Y = %d\n", msg->gate_y_size);
        printf("  Gate Position X = %f\n", msg->gate_x_position);
        printf("  Gate Position Y = %f\n", msg->gate_y_position);
        printf("  Frame Counter = %lu\n", msg->frame_counter);
        printf("\n");
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_SENSOR_RESPONSE));

    return 0;
}

long DoSensorExtResponse(const int sessionid, void *packet)
{
    CIGI_SENSOR_EXT_RESPONSE *msg = (CIGI_SENSOR_EXT_RESPONSE *)packet;

    if (verbose) {
        printf("Sensor Extended Response:\n");
        printf("  View ID = %d\n", msg->view_id);
        printf("  Sensor ID = %d\n", msg->sensor_id);
        printf("  Sensor Status = %d\n", msg->sensor_status);
        printf("  Entity ID = %d\n", msg->entity_id);
        printf("  Entity ID Valid = %d\n", msg->entity_id_valid);
        printf("  Gate Size X = %d\n", msg->gate_x_size);
        printf("  Gate Size Y = %d\n", msg->gate_y_size);
        printf("  Gate Position X = %f\n", msg->gate_x_position);
        printf("  Gate Position Y = %f\n", msg->gate_y_position);
        printf("  Latitude = %lf\n", msg->lat);
        printf("  Longitude = %lf\n", msg->lon);
        printf("  Altitude = %lf\n", msg->alt);
        printf("  Frame Counter = %lu\n", msg->frame_counter);
        printf("\n");
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_SENSOR_EXT_RESPONSE));

    return 0;
}

long DoPositionResponse(const int sessionid, void *packet)
{
    CIGI_POSITION_RESPONSE *msg = (CIGI_POSITION_RESPONSE *)packet;

    if (verbose) {
        printf("Position Response:\n");
        printf("  Object Class = %d\n", msg->object_class);
        printf("  Object ID = %d\n", msg->object_id);
        printf("  Articulated Part ID = %d\n", msg->artpart_id);
        printf("  Coord System = %d\n", msg->coord_system);
        printf("  Latitude/X = %lf\n", msg->lat_x);
        printf("  Longitude/Y = %lf\n", msg->lon_y);
        printf("  Altitude/Z = %lf\n", msg->alt_z);
        printf("  Roll = %f\n", msg->roll);
        printf("  Pitch = %f\n", msg->pitch);
        printf("  Yaw = %f\n", msg->yaw);
        printf("\n");
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_POSITION_RESPONSE));

    return 0;
}

long DoWeatherResponse(const int sessionid, void *packet)
{
    CIGI_WEATHER_RESPONSE *msg = (CIGI_WEATHER_RESPONSE *)packet;

    if (verbose) {
        printf("Weather Response:\n");
        printf("  Request ID = %d\n", msg->request_id);
        printf("  Humidity = %d\n", msg->humidity);
        printf("  Air Temperature = %f\n", msg->air_temp);
        printf("  Visibility = %f\n", msg->visibility);
        printf("  Wind Speed (horizontal) = %f\n", msg->windspeed_horz);
        printf("  Wind Speed (vertical) = %f\n", msg->windspeed_vert);
        printf("  Wind Direction = %f\n", msg->wind_direction);
        printf("  Barometric Pressure = %f\n", msg->baro_pressure);
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_WEATHER_RESPONSE));

    return 0;
}

long DoAerosolResponse(const int sessionid, void *packet)
{
    CIGI_AEROSOL_RESPONSE *msg = (CIGI_AEROSOL_RESPONSE *)packet;

    if (verbose) {
        printf("Aerosol Response:\n");
        printf("  Request ID = %d\n", msg->request_id);
        printf("  Layer ID = %d\n", msg->layer_id);
        printf("  Concentration = %f\n", msg->concentration);
        printf("\n");
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_AEROSOL_RESPONSE));

    return 0;
}

long DoMaritimeResponse(const int sessionid, void *packet)
{
    CIGI_MARITIME_SURFACE_RESPONSE *msg = (CIGI_MARITIME_SURFACE_RESPONSE *)packet;

    if (verbose) {
        printf("Maritime Surface Conditions Response:\n");
        printf("  Request ID = %d\n", msg->request_id);
        printf("  Surface Height = %f\n", msg->surface_height);
        printf("  Surface Temperature = %f\n", msg->surface_temp);
        printf("  Surface Clarity = %f\n", msg->surface_clarity);
        printf("\n");
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_MARITIME_SURFACE_RESPONSE));

    return 0;
}

long DoTerrestrialResponse(const int sessionid, void *packet)
{
    CIGI_TERRESTRIAL_SURFACE_RESPONSE *msg = (CIGI_TERRESTRIAL_SURFACE_RESPONSE *)packet;

    if (verbose) {
        printf("Terrestrial Surface Conditions Response:\n");
        printf("  Request ID = %d\n", msg->request_id);
        printf("  Condition ID = %d\n", msg->condition_id);
        printf("\n");
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_TERRESTRIAL_SURFACE_RESPONSE));

    return 0;
}

long DoCollisionSegNotification(const int sessionid, void *packet)
{
    CIGI_COLL_SEGMENT_NOTIFICATION *msg = (CIGI_COLL_SEGMENT_NOTIFICATION *)packet;

    if (verbose) {
        printf("Collision Detection Segment Notification:\n");
        printf("  Entity ID = %d\n", msg->entity_id);
        printf("  Segment ID = %d\n", msg->segment_id);
        printf("  Collision Type = %d\n", msg->collision_type);
        printf("  Contacted Entity ID = %d\n", msg->contacted_entity_id);
        printf("  Material Code = %lu\n", msg->material_code);
        printf("  Intersection Distance = %f\n", msg->intersection_distance);
        printf("\n");
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_COLL_SEGMENT_NOTIFICATION));

    return 0;
}

long DoCollisionVolNotification(const int sessionid, void *packet)
{
    CIGI_COLL_VOLUME_NOTIFICATION *msg = (CIGI_COLL_VOLUME_NOTIFICATION *)packet;

    if (verbose) {
        printf("Collision Detection Volume Notification:\n");
        printf("  Entity ID = %d\n", msg->entity_id);
        printf("  Volume ID = %d\n", msg->volume_id);
        printf("  Collision Type = %d\n", msg->collision_type);
        printf("  Contacted Entity ID = %d\n", msg->contacted_entity_id);
        printf("  Contacted Volume ID = %d\n", msg->contacted_volume_id);
        printf("\n");
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_COLL_VOLUME_NOTIFICATION));

    return 0;
}

long DoAnimationStopNotification(const int sessionid, void *packet)
{
    CIGI_ANIMATION_STOP_NOTIFICATION *msg = (CIGI_ANIMATION_STOP_NOTIFICATION *)packet;

    if (verbose) {
        printf("Animation Stop Notification:\n");
        printf("  Entity ID = %d\n", msg->entity_id);
        printf("\n");
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_ANIMATION_STOP_NOTIFICATION));

    return 0;
}

long DoEventNotification(const int sessionid, void *packet)
{
    CIGI_EVENT_NOTIFICATION *msg = (CIGI_EVENT_NOTIFICATION *)packet;

    if (verbose) {
        printf("Event Notification:\n");
        printf("  Event ID = %d\n", msg->event_id);
        printf("  Data 1 = %X\n", msg->data1);
        printf("  Data 2 = %X\n", msg->data2);
        printf("  Data 3 = %X\n", msg->data3);
        printf("\n");
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_EVENT_NOTIFICATION));

    return 0;
}

long DoIGResponse(const int sessionid, void *packet)
{
    CIGI_IG_MESSAGE *msg = (CIGI_IG_MESSAGE *)packet;

    if (verbose) {
        printf("IG Message:\n");
        printf("  Message ID = %d\n", msg->message_id);
        printf("  \"%s\"\n", msg->text);
        printf("\n");
    }

    // Send a message back to the GUI.  The GUI will perform any
    // mission function processing.
    g_RcvCIGIMsgQueue.Push((char *)msg, sizeof(CIGI_IG_MESSAGE));

    return 0;
}
