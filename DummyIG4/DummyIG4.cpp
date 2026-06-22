///////////////////////////////////////////////////////////////////////////////
//
//  FILENAME:   DummyIG4.cpp
//
//  DATE:       15 March, 2004
//
//  AUTHOR:     Lance Durham, The Boeing Company
//
//  PLATFORM:   PC running Windows 2000
//
//  COMPILER:   Microsoft Visual C++ 6.0.
//
//  COMMENTS:   This is a simple IG application illustrating the use of the
//              CIGI 4 API, Version 2.  This program uses the callback method
//              of processing messages.
//
///////////////////////////////////////////////////////////////////////////////

#include <cstdint>

// These three files are VC-specific:
#include "stdafx.h"
#include "winsock2.h"
#include "mmsystem.h"

#include "DummyIG4.h"
#include "FilterDialog.h"

#include "cigi4.h"
#include "cigi4types.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Structure to hold basic mission function info.
typedef struct {
    uint16_t id;
    uint16_t entity_id;
    uint8_t type;
    uint8_t reserved;
} MISSION_FUNC_INFO;

//***********************************
// Frame Rate
//***********************************
int framerate;

// CIGI session
int session;

// Berkeley Sockets stuff
SOCKET sndsock;
SOCKET rcvsock;
SOCKADDR_IN saddr;
SOCKADDR_IN raddr;

int ig_mode = 0;

// Set up some arrays so we can keep track of mission function requests.
// A real IG application would use a more robust queueing, list, or other
// system to manage the requests.
int HatCount = 0;
int LosSegmentCount = 0;
int LosVectorCount = 0;
int CDSegCount = 0;
int CDVolCount = 0;
int database = 0;
int db_load_count = -1;
MISSION_FUNC_INFO HatArray[256];
MISSION_FUNC_INFO LosSegArray[256];
MISSION_FUNC_INFO LosVecArray[256];
MISSION_FUNC_INFO CDSegArray[256];
MISSION_FUNC_INFO CDVolArray[256];

// Singleton Windows-specific window instance.  If modifying for another
// platform, you should create boolean flags to enable printing of each
// type of incoming packet.
CFilterDialog theFilterDlg;

// Singleton MFC application instance.
CWinApp theApp;

// CIGI Callback Functions
long DoInvalidPacket(const int sessionid, void *packet);
long DoIGControl(const int sessionid, void *packet);
long DoEntityControl(const int sessionid, void *packet);
long DoCCEntityControl(const int sessionid, void *packet);
long DoComponentControl(const int sessionid, void *packet);
long DoShortComponentControl(const int sessionid, void *packet);
long DoArtPartControl(const int sessionid, void *packet);
long DoShortArtPartControl(const int sessionid, void *packet);
long DoRateControl(const int sessionid, void *packet);
long DoCelestialSphereControl(const int sessionid, void *packet);
long DoAtmosphereControl(const int sessionid, void *packet);
long DoEnvRegionControl(const int sessionid, void *packet);
long DoWeatherControl(const int sessionid, void *packet);
long DoMaritimeCondControl(const int sessionid, void *packet);
long DoWaveControl(const int sessionid, void *packet);
long DoTerrestrialCondControl(const int sessionid, void *packet);
long DoViewControl(const int sessionid, void *packet);
long DoSensorControl(const int sessionid, void *packet);
long DoMotionTrackerControl(const int sessionid, void *packet);
long DoErmDefinition(const int sessionid, void *packet);
long DoTrajectoryDefinition(const int sessionid, void *packet);
long DoViewDefinition(const int sessionid, void *packet);
long DoCDSegmentDefinition(const int sessionid, void *packet);
long DoCDVolumeDefinition(const int sessionid, void *packet);
long DoHatHotRequest(const int sessionid, void *packet);
long DoLosSegmentRequest(const int sessionid, void *packet);
long DoLosVectorRequest(const int sessionid, void *packet);
long DoPositionRequest(const int sessionid, void *packet);
long DoEnvCondRequest(const int sessionid, void *packet);

// Windows Multimedia Callback Function.  Under Windows 2000, this is
// much more deterministic than WM_TIMER message handlers or waitable
// kernel timers (under NT 4.0, it doesn't matter).  The handler function
// runs in a separate thread (from a pre-allocated thread pool) each time
// the timer event fires.  The types of timers available will depend upon
// the platform you are using.
void CALLBACK MessageProc(UINT TimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2);

// Other Functions
void InitializeComm(const char *ipaddr, const int sndport, const int rcvport);
void InitializeCIGI(void);
void AddHatResponses(void);
void AddLosResponses(void);
void AddCDSegmentResponses(void);
void AddCDVolumeResponses(void);
void AddIGMessages(void);

int main(int argc, char *argv[])
{
    char *ip;
    int port_send;
    int port_rcv;

    // Get the IG's IP address and port numbers from the command line.
    if (argc != 5) {
        printf("Usage:  DummyIG4 <frame rate> <ip address> <send port> <receive port>\n\n");
        return 0;
    }

    framerate = atoi((const char *)argv[1]);
    printf("Running at %d Hz\n", framerate);

    ip = argv[2];
    port_send = atoi((const char *)argv[3]);
    port_rcv = atoi((const char *)argv[4]);

    // Initialize MFC (for CFilterDialog).  Windows-specific.
    AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);

    // Initialize communications.
    InitializeComm(ip, port_send, port_rcv);

    // Initialize CIGI.
    InitializeCIGI();

    // Set the Windows multimedia timer.  Non-Windows platforms will have
    // to use some other timer mechanism.
    int timer_id = timeSetEvent(1000 / framerate, 0, MessageProc, 0, TIME_PERIODIC);

    // Show a dialog containing check boxes for each CIGI packet type.
    // When the user checks/unchecks a box, a flag is set to enable/disable
    // printing of packets of that type.  Since the timer handler will run
    // in separate threads, we can call DoModal() here.  This will halt the
    // execution of this thread until the dialog window is dismissed.
    // Non-Win32 developers may want to create their own dialog class or
    // watch for a keystroke or some other event.
    theFilterDlg.DoModal();

    // Kill the Windows multimedia timer.
    timeKillEvent(timer_id);

    // Clean up the sockets.
    closesocket(sndsock);
    closesocket(rcvsock);

    // Clean up WinSock.  Again, this is not needed for non-Windows platforms.
    WSACleanup();

    // Close the CIGI session and clean up CIGI.
    CigiCloseSession(session);
    CigiShutdown();

    return 0;
}

void InitializeComm(const char *ipaddr, const int sndport, const int rcvport)
{
    // Initialize winsock.  If you are using Berkeley sockets on a
    // non-Win32 platform, you can leave out these first two lines.
    WSADATA wsainfo;
    WSAStartup(MAKEWORD(1, 1), &wsainfo);

    // Open the connection for sending.
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(sndport);    // Use whatever port your host listens on
    saddr.sin_addr.s_addr = inet_addr(ipaddr);  // IP address of host

    sndsock = socket(AF_INET, SOCK_DGRAM, 0);

    // Open the connection for receiving.
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(rcvport);    // Use whatever port your host sends to
    raddr.sin_addr.s_addr = htonl(INADDR_ANY);

    rcvsock = socket(AF_INET, SOCK_DGRAM, 0);

    // Make the address reusable.
    int sockparam = TRUE;
    setsockopt(rcvsock, SOL_SOCKET, SO_REUSEADDR, (const char *)&sockparam, sizeof(int));

    // Make the socket non-blocking.
    unsigned long arg = 1L;
    ioctlsocket(rcvsock, FIONBIO, &arg);

    // Bind the socket to the local address.
    bind(rcvsock, (SOCKADDR *)&raddr, sizeof(SOCKADDR));
}

void InitializeCIGI(void)
{
    // The first call should be to CigiInit().  The 1 indicates that we will
    // only be creating one session.
    CigiInit(1, 4);

    // Next, create a session.  It will be an IG-to-Host session with four
    // buffers of 1472 bytes each (if the TCP stack doesn't support
    // fragmentation, the maximum UDP packet size is 1472 bytes).
    session = CigiCreateSession(CIGI_IG_SESSION, 4, 1472);

    // Now create a callback function for each packet type we might receive.
    // The first argument is the packet opcode; the secong argument is the
    // function that will handle that packet type.
    CigiSetCallback(CIGI_IG_CONTROL_OPCODE, DoIGControl);
    CigiSetCallback(CIGI_ENTITY_POSITION_OPCODE, DoEntityControl);
    CigiSetCallback(CIGI_ENTITY_POSITION_CC_OPCODE, DoCCEntityControl);
    CigiSetCallback(CIGI_COMPONENT_CONTROL_OPCODE, DoComponentControl);
    CigiSetCallback(CIGI_COMPONENT_CONTROL_S_OPCODE, DoShortComponentControl);
    CigiSetCallback(CIGI_ARTPART_CONTROL_OPCODE, DoArtPartControl);
    CigiSetCallback(CIGI_ARTPART_CONTROL_S_OPCODE, DoShortArtPartControl);
    CigiSetCallback(CIGI_VELOCITY_CONTROL_OPCODE, DoRateControl);
    CigiSetCallback(CIGI_CELESTIAL_SPHERE_CONTROL_OPCODE, DoCelestialSphereControl);
    CigiSetCallback(CIGI_ATMOSPHERE_CONTROL_OPCODE, DoAtmosphereControl);
    CigiSetCallback(CIGI_ENV_REGION_CONTROL_OPCODE, DoEnvRegionControl);
    CigiSetCallback(CIGI_WEATHER_CONTROL_OPCODE, DoWeatherControl);
    CigiSetCallback(CIGI_MARITIME_CONDITIONS_CONTROL_OPCODE, DoMaritimeCondControl);
    CigiSetCallback(CIGI_WAVE_CONTROL_OPCODE, DoWaveControl);
    CigiSetCallback(CIGI_TERRESTRIAL_CONDITIONS_CONTROL_OPCODE, DoTerrestrialCondControl);
    CigiSetCallback(CIGI_VIEW_CONTROL_OPCODE, DoViewControl);
    CigiSetCallback(CIGI_SENSOR_CONTROL_OPCODE, DoSensorControl);
    CigiSetCallback(CIGI_MOTION_TRACKER_CONTROL_OPCODE, DoMotionTrackerControl);
    CigiSetCallback(CIGI_ERM_DEF_OPCODE, DoErmDefinition);
    CigiSetCallback(CIGI_ACCELERATION_CONTROL_DEF_OPCODE, DoTrajectoryDefinition);
    CigiSetCallback(CIGI_VIEW_DEF_OPCODE, DoViewDefinition);
    CigiSetCallback(CIGI_COLL_SEGMENT_DEF_OPCODE, DoCDSegmentDefinition);
    CigiSetCallback(CIGI_COLL_VOLUME_DEF_OPCODE, DoCDVolumeDefinition);
    CigiSetCallback(CIGI_HAT_HOT_REQUEST_OPCODE, DoHatHotRequest);
    CigiSetCallback(CIGI_LOS_SEGMENT_REQUEST_OPCODE, DoLosSegmentRequest);
    CigiSetCallback(CIGI_LOS_VECTOR_REQUEST_OPCODE, DoLosVectorRequest);
    CigiSetCallback(CIGI_POSITION_REQUEST_OPCODE, DoPositionRequest);
    CigiSetCallback(CIGI_ENV_CONDITIONS_REQUEST_OPCODE, DoEnvCondRequest);

    // Set the default callback.
    CigiSetDefaultCallback(DoInvalidPacket);
}

long DoInvalidPacket(const int sessionid, void *packet)
{
    char *id = (char *)packet;
    char *size = (char *)packet + 1;

    printf("Invalid packet (ID = %d, size = %d)\n\n", *id, *size);

    return 0;
}

long DoIGControl(const int sessionid, void *packet)
{
    ig_mode = ((CIGI_IG_CONTROL *)packet)->ig_mode;

    if (((CIGI_IG_CONTROL *)packet)->db_number &&
        (((CIGI_IG_CONTROL *)packet)->db_number != database)) {
        database = ((CIGI_IG_CONTROL *)packet)->db_number;
        db_load_count = 30;
    }

    // CFilterDialog::m_PrintIGControl is just a boolean flag that indicates
    // whether we should print this type of packet.  Ditto for all of the
    // other callbacks.
    if (theFilterDlg.m_PrintIGControl) {
        CIGI_IG_CONTROL *igc = (CIGI_IG_CONTROL *)packet;

        printf("IG Control:\n");
        printf("   Packet ID = %d\n", igc->packet_id);
        printf("   Packet Size = %d\n", igc->packet_size);
        printf("   CIGI Version = %d.%d\n", igc->cigi_version, igc->minor_version);
        printf("   Database Number = %d\n", igc->db_number);
        printf("   IG Mode = %u\n", igc->ig_mode);
        printf("   Timestamp Valid = %u\n", igc->timestamp_valid);
        printf("   Host Frame Number = %lu\n", igc->host_frame_num);
        printf("   Last IG Frame Number = %lu\n", igc->last_ig_frame_num);
        printf("   Timestamp = %lu\n", igc->timestamp);
        printf("\n");
    }

    return 0;
}

long DoEntityControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintEntityControl) {
        CIGI_ENTITY_POSITION *ec = (CIGI_ENTITY_POSITION *)packet;

        printf("Entity Position:\n");
        printf("   Packet ID = %d\n", ec->packet_id);
        printf("   Packet Size = %d\n", ec->packet_size);
        printf("   Entity ID = %d\n", ec->entity_id);
        printf("   Clamp Mode = %u\n", ec->clamp_mode);
        printf("   Roll = %f\n", ec->roll);
        printf("   Pitch = %f\n", ec->pitch);
        printf("   Yaw = %f\n", ec->yaw);
        printf("   Attach Flag = %u\n", ec->attach_state);
        printf("   Parent ID = %d\n", ec->parent_id);

        if (ec->attach_state) {
            printf("   X Offset = %lf\n", ec->lat_x);
            printf("   Y Offset = %lf\n", ec->lon_y);
            printf("   Z Offset = %lf\n", ec->alt_z);
        } else {
            printf("   Latitude = %lf\n", ec->lat_x);
            printf("   Longitude = %lf\n", ec->lon_y);
            printf("   Altitude = %lf\n", ec->alt_z);
        }

        printf("\n");
    }

    return 0;
}

long DoCCEntityControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintCCEntityControl) {
        CIGI_ENTITY_POSITION_CC *ec = (CIGI_ENTITY_POSITION_CC *)packet;

        printf("Conformal Claped Entity Control:\n");
        printf("   Packet ID = %d\n", ec->packet_id);
        printf("   Packet Size = %d\n", ec->packet_size);
        printf("   Entity ID = %d\n", ec->entity_id);
        printf("   Yaw = %f\n", ec->yaw);
        printf("   Latitude = %lf\n", ec->latitude);
        printf("   Longitude = %lf\n", ec->longitude);
        printf("\n");

    }

    return 0;
}

long DoComponentControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintComponentControl) {
        CIGI_COMPONENT_CONTROL *cc = (CIGI_COMPONENT_CONTROL *)packet;

        printf("Component Control:\n");
        printf("   Packet ID = %d\n", cc->packet_id);
        printf("   Packet Size = %d\n", cc->packet_size);
        printf("   Component ID = %u\n", cc->component_id);
        printf("   Instance ID = %u\n", cc->instance_id);
        printf("   Component Class = %u\n", cc->component_class);
        printf("   Component State = %u\n", cc->component_state);
        printf("   Data 1 = %X\n", cc->data1);
        printf("   Data 2 = %X\n", cc->data2);
        printf("   Data 3 = %X\n", cc->data3);
        printf("   Data 4 = %X\n", cc->data4);
        printf("   Data 5 = %X\n", cc->data5);
        printf("   Data 6 = %X\n", cc->data6);
        printf("\n");
    }

    return 0;
}

long DoShortComponentControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintShortComponentControl) {
        CIGI_COMPONENT_CONTROL_S *cc = (CIGI_COMPONENT_CONTROL_S *)packet;

        printf("Short Component Control:\n");
        printf("   Packet ID = %d\n", cc->packet_id);
        printf("   Packet Size = %d\n", cc->packet_size);
        printf("   Component ID = %u\n", cc->component_id);
        printf("   Instance ID = %u\n", cc->instance_id);
        printf("   Component Class = %u\n", cc->component_class);
        printf("   Component State = %u\n", cc->component_state);
        printf("   Data 1 = %X\n", cc->data1);
        printf("   Data 2 = %X\n", cc->data2);
        printf("\n");
    }

    return 0;
}

long DoArtPartControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintArtPartControl) {
        CIGI_ARTPART_CONTROL *ap = (CIGI_ARTPART_CONTROL *)packet;

        printf("Articulated Part Control:\n");
        printf("   Packet ID = %d\n", ap->packet_id);
        printf("   Packet Size = %d\n", ap->packet_size);
        printf("   Entity ID = %d\n", ap->entity_id);
        printf("   Articulated Part ID = %u\n", ap->artpart_id);
        printf("   Articulated Part Enable = %u\n", ap->artpart_enable);
        printf("   X Offset Enable = %u\n", ap->xoffset_enable);
        printf("   Y Offset Enable = %u\n", ap->yoffset_enable);
        printf("   Z Offset Enable = %u\n", ap->zoffset_enable);
        printf("   Roll Enable = %u\n", ap->roll_enable);
        printf("   Pitch Enable = %u\n", ap->pitch_enable);
        printf("   Yaw Enable = %u\n", ap->yaw_enable);
        printf("   X Offset = %f\n", ap->xoffset);
        printf("   Y Offset = %f\n", ap->yoffset);
        printf("   Z Offset = %f\n", ap->zoffset);
        printf("   Roll = %f\n", ap->roll);
        printf("   Pitch = %f\n", ap->pitch);
        printf("   Yaw = %f\n", ap->yaw);
        printf("\n");
    }

    return 0;
}

long DoShortArtPartControl(const int sessionid, void *packet)
{
    static char DOF[7][9] = {   "None",
                                "X Offset",
                                "Y Offset",
                                "Z Offset",
                                "Yaw",
                                "Pitch",
                                "Roll"
                            };

    if (theFilterDlg.m_PrintShortArtPartControl) {
        CIGI_ARTPART_CONTROL_S *apc = (CIGI_ARTPART_CONTROL_S *)packet;

        printf("Short Articulated Part Control:\n");
        printf("   Packet ID = %d\n", apc->packet_id);
        printf("   Packet Size = %d\n", apc->packet_size);
        printf("   Entity ID = %d\n", apc->entity_id);
        printf("   Articulated Part 1:\n");
        printf("      ID = %u\n", apc->artpart1_id);
        printf("      Enable = %u\n", apc->artpart1_enable);
        printf("      DOF = %s\n", DOF[apc->dof1_select]);
        printf("      Data = %f\n", apc->dof1);
        printf("   Articulated Part 2:\n");
        printf("      ID = %u\n", apc->artpart2_id);
        printf("      Enable = %u\n", apc->artpart2_enable);
        printf("      DOF = %s\n", DOF[apc->dof2_select]);
        printf("      Data = %f\n", apc->dof2);
        printf("\n");
    }

    return 0;
}

long DoRateControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintRateControl) {
        CIGI_VELOCITY_CONTROL *rc = (CIGI_VELOCITY_CONTROL *)packet;

        printf("Velocity Control:\n");
        printf("   Packet ID = %u\n", rc->packet_id);
        printf("   Packet Size = %u\n", rc->packet_size);
        printf("   Entity ID = %u\n", rc->entity_id);
        printf("   Articulated Part ID = %u\n", rc->artpart_id);
        printf("   Apply to Articulated Part = %u\n", rc->apply_artpart);
        printf("   Coordinate System = %u\n", rc->coord_system);
        printf("   X Linear Rate = %f\n", rc->x_rate);
        printf("   Y Linear Rate = %f\n", rc->y_rate);
        printf("   Z Linear Rate = %f\n", rc->z_rate);
        printf("   Roll Angular Rate = %f\n", rc->roll_rate);
        printf("   Pitch Angular Rate = %f\n", rc->pitch_rate);
        printf("   Yaw Angular Rate = %f\n", rc->yaw_rate);
        printf("\n");
    }

    return 0;
}

long DoCelestialSphereControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintCelestialSphereControl) {
        CIGI_CELESTIAL_SPHERE_CONTROL *csc = (CIGI_CELESTIAL_SPHERE_CONTROL *)packet;

        printf("Celestial Sphere Control:\n");
        printf("   Packet ID = %d\n", csc->packet_id);
        printf("   Packet Size = %u\n", csc->packet_size);
        printf("   Hour = %u\n", csc->hour);
        printf("   Minute = %u\n", csc->minute);
        printf("   Ephemeris Model Enable = %u\n", csc->ephemeris_enable);
        printf("   Sun Enable = %u\n", csc->sun_enable);
        printf("   Moon Enable = %u\n", csc->moon_enable);
        printf("   Star Field Enable = %u\n", csc->starfield_enable);
        printf("   Date/Time Valid = %u\n", csc->datetime_valid);
        printf("   Date = %u\n", csc->date);
        printf("   Star Field Intensity = %f\n", csc->starfield_intensity);
        printf("\n");
    }

    return 0;
}

long DoAtmosphereControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintAtmosphereControl) {
        CIGI_ATMOSPHERE_CONTROL *ac = (CIGI_ATMOSPHERE_CONTROL *)packet;

        printf("Atmosphere Control:\n");
        printf("   Packet ID = %u\n", ac->packet_id);
        printf("   Packet Size = %u\n", ac->packet_size);
        printf("   Atmospheric Model Enable = %u\n", ac->atm_model_enable);
        printf("   Global Humidity = %u\n", ac->global_humidity);
        printf("   Global Air Temperature = %f\n", ac->global_air_temp);
        printf("   Global Visibility Range = %f\n", ac->global_visibility);
        printf("   Global Horizontal Wind Speed = %f\n", ac->global_windspeed_horz);
        printf("   Global Vertical Wind Speed = %f\n", ac->global_windspeed_vert);
        printf("   Global Wind Direction = %f\n", ac->global_wind_direction);
        printf("   Global Barometric Pressure = %f\n", ac->global_baro_pressure);
        printf("\n");
    }

    return 0;
}

long DoEnvRegionControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintEnvRegionControl) {
        CIGI_ENV_REGION_CONTROL *rc = (CIGI_ENV_REGION_CONTROL *)packet;

        printf("Environmental Region Control:\n");
        printf("   Packet ID = %u\n", rc->packet_id);
        printf("   Packet Size = %u\n", rc->packet_size);
        printf("   Region ID = %u\n", rc->region_id);
        printf("   Region State = %u\n", rc->region_state);
        printf("   Merge Weather Properties = %u\n", rc->merge_weather);
        printf("   Merge Aerosol Concentrations = %u\n", rc->merge_aerosols);
        printf("   Merge Maritime Surface Conditions = %u\n", rc->merge_maritime);
        printf("   Merge Terrestrial Surface Conditions = %u\n", rc->merge_terrestrial);
        printf("   Latitude = %lf\n", rc->latitude);
        printf("   Longitude = %lf\n", rc->longitude);
        printf("   Size X = %f\n", rc->size_x);
        printf("   Size Y = %f\n", rc->size_y);
        printf("   Corner Radius = %f\n", rc->corner_radius);
        printf("   Rotation = %f\n", rc->rotation);
        printf("   Transition Parameter = %f\n", rc->transition_perimeter);
        printf("\n");
    }

    return 0;
}

long DoWeatherControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintWeatherControl) {
        CIGI_WEATHER_CONTROL *wc = (CIGI_WEATHER_CONTROL *)packet;

        printf("Weather Control:\n");
        printf("   Packet ID = %d\n", wc->packet_id);
        printf("   Packet Size = %d\n", wc->packet_size);
        printf("   Entity/Region ID = %d\n", wc->entity_region_id);
        printf("   Layer ID = %d\n", wc->layer_id);
        printf("   Weather Enable = %d\n", wc->weather_enable);
        printf("   Bottom Scud Enable = %d\n", wc->bottom_scud_enable);
        printf("   Top Scud Enable = %d\n", wc->top_scud_enable);
        printf("   Random Winds Enable = %d\n", wc->random_winds);
        printf("   Random Lightning Enable = %d\n", wc->random_lightning);
        printf("   Cloud Type = %d\n", wc->cloud_type);
        printf("   Scope = %d\n", wc->scope);
        printf("   Severity = %d\n", wc->severity);
        printf("   Humidity = %d\n", wc->humidity);
        printf("   Air Temperature = %f\n", wc->air_temp);
        printf("   Visibility Range = %f\n", wc->visibility);
        printf("   Bottom Scud Frequency = %f\n", wc->bottom_scud_freq);
        printf("   Top Scud Frequency = %f\n", wc->top_scud_freq);
        printf("   Coverage = %f\n", wc->coverage);
        printf("   Base Elevation = %f\n", wc->base_elevation);
        printf("   Thickness = %f\n", wc->thickness);
        printf("   Bottom Transition Band = %f\n", wc->bottom_transition_band);
        printf("   Top Transition Band = %f\n", wc->top_transition_band);
        printf("   Horizontal Wind Speed = %f\n", wc->windspeed_horz);
        printf("   Vertical Wind Speed = %f\n", wc->windspeed_vert);
        printf("   Wind Direction = %f\n", wc->wind_direction);
        printf("   Barometric Pressure = %f\n", wc->baro_pressure);
        printf("   Aerosol Concentration = %f\n", wc->aerosol_concentration);
        printf("\n");
    }

    return 0;
}

long DoMaritimeCondControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintMaritimeCondControl) {
        CIGI_MARITIME_CONDITIONS_CONTROL *mc = (CIGI_MARITIME_CONDITIONS_CONTROL *)packet;

        printf("Maritime Surface Conditions Control:\n");
        printf("   Packet ID = %u\n", mc->packet_id);
        printf("   Packet Size = %u\n", mc->packet_size);
        printf("   Entity ID/Region ID = %u\n", mc->entity_region_id);
        printf("   Surface Conditions Enable = %u\n", mc->conditions_enable);
        printf("   Whitecap Enable = %u\n", mc->whitecap_enable);
        printf("   Scope = %u\n", mc->scope);
        printf("   Sea Surface Height = %f\n", mc->surface_height);
        printf("   Surface Water Temperature = %f\n", mc->surface_temp);
        printf("   Surface Clarity = %f\n", mc->surface_clarity);
        printf("\n");
    }

    return 0;
}

long DoWaveControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintWaveControl) {
        CIGI_WAVE_CONTROL *wc = (CIGI_WAVE_CONTROL *)packet;

        printf("Wave Control:\n");
        printf("   Packet ID = %u\n", wc->packet_id);
        printf("   Packet Size = %u\n", wc->packet_size);
        printf("   Wave Enable = %u\n", wc->wave_enable);
        printf("   Scope = %u\n", wc->scope);
        printf("   Breaker Type = %u\n", wc->breaker_type);
        printf("   Entity ID/Region ID = %u\n", wc->entity_region_id);
        printf("   Wave ID = %u\n", wc->wave_id);
        printf("   Wave Height = %f\n", wc->wave_height);
        printf("   Wavelength = %f\n", wc->wavelength);
        printf("   Period = %f\n", wc->period);
        printf("   Direction = %f\n", wc->direction);
        printf("   Phase Offset = %f\n", wc->phase_offset);
        printf("   Leading = %f\n", wc->leading);
        printf("\n");
    }

    return 0;
}

long DoTerrestrialCondControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintTerrestrialCondControl) {
        CIGI_TERRESTRIAL_CONDITIONS_CONTROL *tc = (CIGI_TERRESTRIAL_CONDITIONS_CONTROL *)packet;

        printf("Terrestrial Surface Conditions Control:\n");
        printf("   Packet ID = %u\n", tc->packet_id);
        printf("   Packet Size = %u\n", tc->packet_size);
        printf("   Entity ID/Region ID = %u\n", tc->entity_region_id);
        printf("   Surface Condition ID = %u\n", tc->condition_id);
        printf("   Surface Condition Enable = %u\n", tc->condition_enable);
        printf("   Scope = %u\n", tc->scope);
        printf("   Severity = %u\n", tc->severity);
        printf("   Coverage = %u\n", tc->coverage);
        printf("\n");
    }

    return 0;
}

long DoViewControl(const int sessionid, void *packet)
{
    static char enable_text[2][9] = {   "Disabled", "Enabled"   };

    if (theFilterDlg.m_PrintViewControl) {
        CIGI_VIEW_CONTROL *vc = (CIGI_VIEW_CONTROL *)packet;

        printf("View Control:\n");
        printf("   Packet ID = %u\n", vc->packet_id);
        printf("   Packet Size = %u\n", vc->packet_size);
        printf("   View ID = %d\n", vc->view_id);
        printf("   Group ID = %d\n", vc->group_id);
        printf("   Entity ID = %d\n", vc->entity_id);
        printf("   X Offset = %f (%s)\n", vc->xoffset, enable_text[vc->xoffset_enable]);
        printf("   Y Offset = %f (%s)\n", vc->yoffset, enable_text[vc->yoffset_enable]);
        printf("   Z Offset = %f (%s)\n", vc->zoffset, enable_text[vc->zoffset_enable]);
        printf("   Roll = %f (%s)\n", vc->roll, enable_text[vc->roll_enable]);
        printf("   Pitch = %f (%s)\n", vc->pitch, enable_text[vc->pitch_enable]);
        printf("   Yaw = %f (%s)\n", vc->yaw, enable_text[vc->yaw_enable]);
        printf("\n");
    }

    return 0;
}

long DoSensorControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintSensorControl) {
        CIGI_SENSOR_CONTROL *sc = (CIGI_SENSOR_CONTROL *)packet;

        printf("Sensor Control:\n");
        printf("   Packet ID = %u\n", sc->packet_id);
        printf("   Packet Size = %u\n", sc->packet_size);
        printf("   View ID = %u\n", sc->view_id);
        printf("   Sensor ID = %u\n", sc->sensor_id);
        printf("   Track Mode = %u\n", sc->track_mode);
        printf("   Sensor On/Off = %u\n", sc->sensor_on_off);
        printf("   Polarity = %u\n", sc->polarity);
        printf("   Line-by-Line Dropout Enable = %u\n", sc->line_dropout);
        printf("   Automatic Gain = %u\n", sc->auto_gain);
        printf("   Track White/Black = %u\n", sc->track_white_black);
        printf("   Response Type = %u\n", sc->response_type);
        printf("   Gain = %f\n", sc->gain);
        printf("   Level = %f\n", sc->level);
        printf("   AC Coupling = %f\n", sc->ac_coupling);
        printf("   Noise = %f\n", sc->noise);
        printf("\n");
    }

    return 0;
}

long DoMotionTrackerControl(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintMotionTrackerControl) {
        CIGI_MOTION_TRACKER_CONTROL *mtc = (CIGI_MOTION_TRACKER_CONTROL *)packet;

        printf("Motion Tracker Control:\n");
        printf("   Packet ID = %u\n", mtc->packet_id);
        printf("   Packet Size = %u\n", mtc->packet_size);
        printf("   View/View Group ID = %u\n", mtc->view_or_group_id);
        printf("   Tracker ID = %u\n", mtc->tracker_id);
        printf("   Tracker Enable = %u\n", mtc->tracker_enable);
        printf("   Boresight ID = %u\n", mtc->boresight_enable);
        printf("   X Enable = %u\n", mtc->x_enable);
        printf("   Y Enable = %u\n", mtc->y_enable);
        printf("   Z Enable = %u\n", mtc->z_enable);
        printf("   Roll Enable = %u\n", mtc->roll_enable);
        printf("   Pitch Enable = %u\n", mtc->pitch_enable);
        printf("   Yaw Enable = %u\n", mtc->yaw_enable);
        printf("   View/View Group Select = %u\n", mtc->view_or_group_select);
        printf("\n");
    }

    return 0;
}

long DoErmDefinition(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintErmDef) {
        CIGI_ERM_DEF *erm = (CIGI_ERM_DEF *)packet;

        printf("Earth Ref Model Definition:\n");
        printf("   Packet ID = %u\n", erm->packet_id);
        printf("   Packet Size = %u\n", erm->packet_size);
        printf("   Custom ERM Enable = %u\n", erm->erm_enable);
        printf("   Equatorial Radius = %lf\n", erm->equatorial_radius);
        printf("   Flattening = %lf\n", erm->flattening);
        printf("\n");
    }

    return 0;
}

long DoTrajectoryDefinition(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintTrajectoryDef) {
        CIGI_ACCELERATION_CONTROL_DEF *td = (CIGI_ACCELERATION_CONTROL_DEF *)packet;

        printf("Acceleration Control Definition:\n");
        printf("   Packet ID = %u\n", td->packet_id);
        printf("   Packet Size = %u\n", td->packet_size);
        printf("   Entity ID = %u\n", td->entity_id);
        printf("   Articulated Part ID = %u\n", td->artpart_id);
        printf("   Apply to Articulated Part = %u\n", td->artpart_apply);
        printf("   Coordinate System = %u\n", td->coord_system);
        printf("   Acceleration X = %f\n", td->accel_x);
        printf("   Acceleration Y = %f\n", td->accel_y);
        printf("   Acceleration Z = %f\n", td->accel_z);
        printf("   Roll Acceleration = %f\n", td->accel_roll);
        printf("   Pitch Acceleration = %f\n", td->accel_pitch);
        printf("   Yaw Acceleration = %f\n", td->accel_yaw);
        printf("\n");
    }

    return 0;
}

long DoViewDefinition(const int sessionid, void *packet)
{
    static char enable_text[2][9] = {   "Disabled", "Enabled"   };

    if (theFilterDlg.m_PrintViewDef) {
        CIGI_VIEW_DEF *vd = (CIGI_VIEW_DEF *)packet;

        printf("View Definition:\n");
        printf("   Packet ID = %u\n", vd->packet_id);
        printf("   Packet Size = %u\n", vd->packet_size);
        printf("   View ID = %d\n", vd->view_id);
        printf("   Group ID = %d\n", vd->group_id);
        printf("   View Type = %d\n", vd->view_type);
        printf("   Projection Type = %d\n", vd->projection_type);
        printf("   Replication Mode = %d\n", vd->pixel_replication_mode);
        printf("   Mirror Mode = %d\n", vd->mirror_mode);
        printf("   Reorder = %d\n", vd->reorder);
        printf("   Near = %f (%s)\n", vd->fov_near, enable_text[vd->near_enable]);
        printf("   Far = %f (%s)\n", vd->fov_far, enable_text[vd->far_enable]);
        printf("   Left = %f (%s)\n", vd->fov_left, enable_text[vd->left_enable]);
        printf("   Right = %f (%s)\n", vd->fov_right, enable_text[vd->right_enable]);
        printf("   Top = %f (%s)\n", vd->fov_top, enable_text[vd->top_enable]);
        printf("   Bottom = %f (%s)\n", vd->fov_bottom, enable_text[vd->bottom_enable]);
        printf("\n");
    }

    return 0;
}

long DoCDSegmentDefinition(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintCDSegmentDef) {
        CIGI_COLL_SEGMENT_DEF *cds = (CIGI_COLL_SEGMENT_DEF *)packet;

        printf("Collision Detection Segment Definition:\n");
        printf("   Packet ID = %u\n", cds->packet_id);
        printf("   Packet Size = %u\n", cds->packet_size);
        printf("   Entity ID = %u\n", cds->entity_id);
        printf("   Segment ID = %u\n", cds->segment_id);
        printf("   Segment Enable = %u\n", cds->segment_enable);
        printf("   X1 = %f\n", cds->x1);
        printf("   Y1 = %f\n", cds->y1);
        printf("   Z1 = %f\n", cds->z1);
        printf("   X2 = %f\n", cds->x2);
        printf("   Y2 = %f\n", cds->y2);
        printf("   Z2 = %f\n", cds->z2);
        printf("   Material Mask = %u\n", cds->material_mask);
        printf("\n");
    }

    // Add the request to the appropriate array.
    MISSION_FUNC_INFO cd_info;
    cd_info.id = ((CIGI_COLL_SEGMENT_DEF *)packet)->segment_id;
    cd_info.entity_id = ((CIGI_COLL_SEGMENT_DEF *)packet)->entity_id;
    CDSegArray[CDSegCount++] = cd_info;

    return 0;
}

long DoCDVolumeDefinition(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintCDVolumeDef) {
        CIGI_COLL_VOLUME_DEF *cdv = (CIGI_COLL_VOLUME_DEF *)packet;

        printf("Collision Detection Volume Definition:\n");
        printf("   Packet ID = %u\n", cdv->packet_id);
        printf("   Packet Size = %u\n", cdv->packet_size);
        printf("   Entity ID = %u\n", cdv->entity_id);
        printf("   Volume ID = %u\n", cdv->volume_id);
        printf("   Volume Enable = %u\n", cdv->volume_enable);
        printf("   Volume Type = %u\n", cdv->volume_type);
        printf("   X = %f\n", cdv->x);
        printf("   Y = %f\n", cdv->y);
        printf("   Z = %f\n", cdv->z);
        printf("   Height/Radius = %f\n", cdv->height_radius);
        printf("   Width = %f\n", cdv->width);
        printf("   Depth = %f\n", cdv->depth);
        printf("   Roll = %f\n", cdv->roll);
        printf("   Pitch = %f\n", cdv->pitch);
        printf("   Yaw = %f\n", cdv->yaw);
        printf("\n");
    }

    // Add the request to the appropriate array.
    MISSION_FUNC_INFO cd_info;
    cd_info.id = ((CIGI_COLL_VOLUME_DEF *)packet)->volume_id;
    cd_info.entity_id = ((CIGI_COLL_VOLUME_DEF *)packet)->entity_id;
    CDVolArray[CDVolCount++] = cd_info;

    return 0;
}

long DoHatHotRequest(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintHatReq) {
        CIGI_HAT_HOT_REQUEST *hat = (CIGI_HAT_HOT_REQUEST *)packet;

        printf("HAT/HOT Request:\n");
        printf("   Packet ID = %d\n", hat->packet_id);
        printf("   Packet Size = %d\n", hat->packet_size);
        printf("   HAT ID = %d\n", hat->hat_hot_id);
        printf("   Request Type = %d\n", hat->request_type);
        printf("   Coord System = %d\n", hat->coord_system);
        printf("   Entity ID = %d\n", hat->entity_id);
        printf("   Latitude/X = %lf\n", hat->lat_x);
        printf("   Longitude/Y = %lf\n", hat->lon_y);
        printf("   Altitude/Z = %lf\n", hat->alt_z);
        printf("\n");
    }

    // Add the request to the appropriate array.
    MISSION_FUNC_INFO hat_info;
    hat_info.id = ((CIGI_HAT_HOT_REQUEST *)packet)->hat_hot_id;
    hat_info.entity_id = 0;
    hat_info.type = ((CIGI_HAT_HOT_REQUEST *)packet)->request_type;
    HatArray[HatCount++] = hat_info;

    return 0;
}

long DoLosSegmentRequest(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintLosSegmentReq) {
        CIGI_LOS_SEGMENT_REQUEST *los = (CIGI_LOS_SEGMENT_REQUEST *)packet;

        printf("LOS Segment Request:\n");
        printf("   Packet ID = %d\n", los->packet_id);
        printf("   Packet Size = %d\n", los->packet_size);
        printf("   LOS ID = %d\n", los->los_id);
        printf("   Request Type = %d\n", los->request_type);
        printf("   Source Coord System = %d\n", los->source_coord_system);
        printf("   Dest Coord System = %d\n", los->dest_coord_system);
        printf("   Resp Coord System = %d\n", los->response_coord_system);
        printf("   Alpha Threshold = %d\n", los->alpha_threshold);
        printf("   Source Entity ID = %d\n", los->source_entity_id);
        printf("   Destination Entity ID = %d\n", los->dest_entity_id);
        printf("   Source Latitude/X = %lf\n", los->source_lat_x);
        printf("   Source Longitude/Y = %lf\n", los->source_lon_y);
        printf("   Source Altitude/Z = %lf\n", los->source_alt_z);
        printf("   Dest Latitude/X = %lf\n", los->dest_lat_x);
        printf("   Dest Longitude/Z = %lf\n", los->dest_lon_y);
        printf("   Dest Altitude/Z = %lf\n", los->dest_alt_z);
        printf("   Material Mask = %X\n", los->material_mask);
        printf("\n");
    }

    // Add the request to the appropriate array.
    MISSION_FUNC_INFO los_info;
    los_info.id = ((CIGI_LOS_SEGMENT_REQUEST *)packet)->los_id;
    los_info.entity_id = 0;
    los_info.type = ((CIGI_LOS_SEGMENT_REQUEST *)packet)->request_type;
    LosSegArray[LosSegmentCount++] = los_info;

    return 0;
}

long DoLosVectorRequest(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintLosVectorReq) {
        CIGI_LOS_VECTOR_REQUEST *los = (CIGI_LOS_VECTOR_REQUEST *)packet;

        printf("LOS Vector Request:\n");
        printf("   Packet ID = %d\n", los->packet_id);
        printf("   Packet Size = %d\n", los->packet_size);
        printf("   LOS ID = %d\n", los->los_id);
        printf("   Request Type = %d\n", los->request_type);
        printf("   Source Coord System = %d\n", los->source_coord_system);
        printf("   Resp Coord System = %d\n", los->resp_coord_system);
        printf("   Alpha Threshold = %d\n", los->alpha_threshold);
        printf("   Entity ID = %d\n", los->entity_id);
        printf("   Source Latitude/X = %lf\n", los->source_lat_x);
        printf("   Source Longitude/Y = %lf\n", los->source_lon_y);
        printf("   Source Altitude/Z = %lf\n", los->source_alt_z);
        printf("   Azimuth = %f\n", los->azimuth);
        printf("   Elevation = %f\n", los->elevation);
        printf("   Min Range = %f\n", los->min_range);
        printf("   Max Range = %f\n", los->max_range);
        printf("   Material Mask = %X\n", los->material_mask);
        printf("\n");
    }

    // Add the request to the appropriate array.
    MISSION_FUNC_INFO los_info;
    los_info.id = ((CIGI_LOS_VECTOR_REQUEST *)packet)->los_id;
    los_info.entity_id = 0;
    los_info.type = ((CIGI_LOS_VECTOR_REQUEST *)packet)->request_type;
    LosVecArray[LosVectorCount++] = los_info;

    return 0;
}

long DoPositionRequest(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintPositionReq) {
        CIGI_POSITION_REQUEST *pr = (CIGI_POSITION_REQUEST *)packet;

        printf("Position Request:\n");
        printf("   Packet ID = %d\n", pr->packet_id);
        printf("   Packet Size = %d\n", pr->packet_size);
        printf("   Object ID = %d\n", pr->object_id);
        printf("   Articulated Part ID = %d\n", pr->artpart_id);
        printf("   Update Mode = %d\n", pr->update_mode);
        printf("   Object Class = %d\n", pr->object_class);
        printf("   Coordinate System = %d\n", pr->coord_system);
        printf("\n");
    }

    return 0;
}

long DoEnvCondRequest(const int sessionid, void *packet)
{
    if (theFilterDlg.m_PrintEnvConditionsReq) {
        CIGI_ENV_CONDITIONS_REQUEST *ec = (CIGI_ENV_CONDITIONS_REQUEST *)packet;

        printf("Env Conditions Request:\n");
        printf("   Packet ID = %d\n", ec->packet_id);
        printf("   Packet Size = %d\n", ec->packet_size);
        printf("   Request Type = %d\n", ec->request_type);
        printf("   Request ID = %d\n", ec->request_id);
        printf("   Latitude = %f\n", ec->latitude);
        printf("   Longitude = %f\n", ec->longitude);
        printf("   Altitude = %f\n", ec->altitude);
        printf("\n");
    }

    return 0;
}

static CIGI_START_OF_FRAME sof = {};

// This is the Windows Multimedia Timer handler.  This function runs in a thread
// that is separate from main().
void CALLBACK MessageProc(UINT TimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
    static unsigned char *sendbuff = NULL;
    static unsigned char rcvbuff[1472];
    static int sendsize = 0;
    static int rcvsize = 0;

    sof.packet_size = CIGI_START_OF_FRAME_SIZE;
    sof.packet_id = CIGI_START_OF_FRAME_OPCODE;
    sof.cigi_major_version = 4;
    sof.minor_version = 0;
    sof.ig_mode = ig_mode;

    if (--db_load_count > 0)
        sof.db_number = -database;    // loading
    else if (db_load_count == 0)
        sof.db_number = database;     // finished loading
    else
        sof.db_number = 0;            // no request

    // Start the message. This MUST come before any CigiAddPacket*() functions.
    CigiStartMessage(session);

    // Now we must add a Start of Frame.
    CigiAddPacketSof(session, &sof);

    // And then we can add any other packets for this frame.
    AddHatResponses();
    AddLosResponses();
    AddCDSegmentResponses();
    AddCDVolumeResponses();

    // End the message.
    CigiEndMessage(session);

    // After we call CigiEndMessage(), we can get a pointer to that
    // message buffer.  We want to send the contents of that buffer
    // to the host.
    CigiGetOutgoingMsgBuffer(session, &sendbuff, &sendsize);
    sendto(sndsock, (const char *)sendbuff, sendsize, 0, (SOCKADDR *)&saddr,
           sizeof(SOCKADDR));

    // The host should immediately respond.
    rcvsize = recvfrom(rcvsock, (char *)rcvbuff, 1472, 0, NULL, 0);
    if (rcvsize >= 0) {
        if (theFilterDlg.m_PrintFrameStart)
            printf("----------------------------------------\n");

        // Make the API aware of the data we just received.
        CigiSetIncomingMsgBuffer(session, rcvbuff, rcvsize);

        // Process that data.
        CigiProcessIncomingMsgBuffer(session);

        // Increment the frame counter.
        sof.frame_ctr++;
    }

    // You could also print outgoing messages here.  One way to do this would
    // be to do the following:
    //
    //      1. [in InitializeCIGI()]  Create a second CIGI session (of type
    //          CIGI_HOST_SESSION) and create a callback function for each of
    //          the remaining packet types.
    //
    //      2. [in this function]  Set the incoming message buffer of Session 2
    //          to the outgoing message buffer of Session 1.  Then, call
    //          CigiProcessIncomingMsgBuffer() for Session 2.
    //
    //              unsigned char buff[1472] = {0};
    //              int sz = 0;
    //              CigiGetOutgoingMsgBuffer(session, buff, &sz);
    //              CigiSetIncomingMsgBuffer(session2, buff, sz);
    //              CigiProcessIncomingMsgBuffer(session2);
    //
}

void AddHatResponses(void)
{
    static unsigned int value = 0; // Some bogus data
    CIGI_HAT_HOT_RESPONSE hat = {};
    CIGI_HAT_HOT_EXT_RESPONSE hatx = {};
    hat.packet_size = CIGI_HAT_HOT_RESPONSE_SIZE;
    hat.packet_id = CIGI_HAT_HOT_RESPONSE_OPCODE;
    hatx.packet_size = CIGI_HAT_HOT_EXT_RESPONSE_SIZE;
    hatx.packet_id = CIGI_HAT_HOT_EXT_RESPONSE_OPCODE;

    for (int i = 0; i < HatCount; i++) {
        int type = HatArray[i].type;

        if (type == 2) {
            // Extended response.
            hatx.hat_hot_id = HatArray[i].id;
            hatx.valid = (value / 64) % 2;
            hatx.frame_num_lsn = sof.frame_ctr & 0x0000000f;
            hatx.hat = 2000.0 + (2.0 * value);
            hatx.hot = 3000.0 + (3.0 * value);
            hatx.material_code = value;
            hatx.normal_azimuth = value * 4.0f;
            hatx.normal_elevation = value * 5.0f;

            // Send the packet.  Note that we have already called
            // CigiStartMessage() for this frame.
            CigiAddFixedPacket<CIGI_HAT_HOT_EXT_RESPONSE>(session, &hatx);
        } else {
            // Populate a packet.
            hat.hat_hot_id = HatArray[i].id;
            hat.response_type = type;       // 0 or 1
            hat.valid = (value / 64) % 2;
            hat.frame_num_lsn = sof.frame_ctr & 0x0000000f;
            hat.height = 1000.0 + (float)value;

            // Send the packet.  Note that we have already called
            // CigiStartMessage() for this frame.
            CigiAddFixedPacket<CIGI_HAT_HOT_RESPONSE>(session, &hat);
        }

        // Change the data from frame to frame. (Static variable.)
        value++;
    }

    // "Clear" the array.
    HatCount = 0;
}

void AddLosResponses(void)
{
    static unsigned int value = 0; // Some bogus data.
    CIGI_LOS_RESPONSE los = {};
    CIGI_LOS_EXT_RESPONSE losx = {};
    los.packet_size = CIGI_LOS_RESPONSE_SIZE;
    los.packet_id = CIGI_LOS_RESPONSE_OPCODE;
    losx.packet_size = CIGI_LOS_EXT_RESPONSE_SIZE;
    losx.packet_id = CIGI_LOS_EXT_RESPONSE_OPCODE;

    for (int i = 0; i < LosSegmentCount; i++) {
        int type = LosSegArray[i].type;

        if (type == 1) {
            // Extended LOS Response
            losx.los_id = LosSegArray[i].id;
            losx.valid = (value / 64) % 2;
            losx.entity_id_valid = (value / 256) % 2;
            losx.range_valid = (value / 32) % 2;
            losx.visible = (value / 64) % 3;
            losx.frame_num_lsn = sof.frame_ctr & 0x0000000f;
            losx.response_count = 0;
            losx.entity_id = value / 640;
            losx.range = 100.0f + value;
            losx.lat_x = 1000.0 + value;
            losx.lon_y = 2000.0 + value;
            losx.alt_z = 3000.0 + value;
            losx.material_code = value;
            losx.normal_azimuth = value * 4.0f;
            losx.normal_elevation = value * 5.0f;
            losx.red = 63;
            losx.green = 127;
            losx.blue = 191;
            losx.alpha = 255;

            // Send the packet.  Note that we have already called
            // CigiStartMessage() for this frame.
            CigiAddFixedPacket<CIGI_LOS_EXT_RESPONSE>(session, &losx);
        } else {
            // Populate a packet.
            los.los_id = LosSegArray[i].id;
            los.valid = (value / 64) % 2;
            los.entity_id_valid = (value / 256) % 2;
            los.visible = (value / 64) % 3;
            los.frame_num_lsn = sof.frame_ctr & 0x0000000f;
            los.range = 100.0f + value;
            los.entity_id = value / 640;
            los.response_count = 1;

            // Send the packet.  Note that we have already called
            // CigiStartMessage() for this frame.
            CigiAddFixedPacket<CIGI_LOS_RESPONSE>(session, &los);
        }

        // Change the data from frame to frame. (Static variable.)
        value++;
    }

    for (int i = 0; i < LosVectorCount; i++) {
        int type = LosVecArray[i].type;

        if (type == 1) {
            // Extended LOS Response
            losx.los_id = LosVecArray[i].id;
            losx.valid = (value / 64) % 2;
            losx.entity_id_valid = (value / 256) % 2;
            losx.range_valid = (value / 32) % 2;
            losx.visible = (value / 64) % 3;
            losx.frame_num_lsn = sof.frame_ctr & 0x0000000f;
            losx.response_count = 0;
            losx.entity_id = value / 640;
            losx.range = 100.0f + value;
            losx.lat_x = 1000.0 + value;
            losx.lon_y = 2000.0 + value;
            losx.alt_z = 3000.0 + value;
            losx.material_code = value;
            losx.normal_azimuth = value * 4.0f;
            losx.normal_elevation = value * 5.0f;
            losx.red = 63;
            losx.green = 127;
            losx.blue = 191;
            losx.alpha = 255;

            // Send the packet.  Note that we have already called
            // CigiStartMessage() for this frame.
            CigiAddFixedPacket<CIGI_LOS_EXT_RESPONSE>(session, &losx);
        } else {
            // Populate a packet.
            los.los_id = LosVecArray[i].id;
            los.valid = (value / 64) % 2;
            los.entity_id_valid = (value / 256) % 2;
            los.visible = (value / 64) % 3;
            los.frame_num_lsn = sof.frame_ctr & 0x0000000f;
            los.range = 100.0f + value;
            los.entity_id = value / 64;
            los.response_count = 1;

            // Send the packet.  Note that we have already called
            // CigiStartMessage() for this frame.
            CigiAddFixedPacket<CIGI_LOS_RESPONSE>(session, &los);
        }

        // Change the data from frame to frame. (Static variable.)
        value++;
    }

    // "Clear" the array.
    LosSegmentCount = 0;
    LosVectorCount = 0;
}

void AddCDSegmentResponses(void)
{
    static unsigned int value = 0;  // Some bogus data.
    CIGI_COLL_SEGMENT_NOTIFICATION cd = {};
    cd.packet_size = CIGI_COLL_SEGMENT_NOTIFICATION_SIZE;
    cd.packet_id = CIGI_COLL_SEGMENT_NOTIFICATION_OPCODE;

    for (int i = 0; i < CDSegCount; i++) {
        // Populate a packet.
        cd.segment_id = (uint8_t)CDSegArray[i].id;
        cd.entity_id = CDSegArray[i].entity_id;
        cd.collision_type = !CDSegArray[i].entity_id;
        cd.material_code = value;

        // Send the packet.  Note that we have already called
        // CigiStartMessage() for this frame.
        CigiAddFixedPacket<CIGI_COLL_SEGMENT_NOTIFICATION>(session, &cd);

        // Change the data from frame to frame. (Static variable.)
        value++;
    }

    // "Clear" the array.
    CDSegCount = 0;
}

void AddCDVolumeResponses(void)
{
    static unsigned int value = 0;  // Some bogus data.
    CIGI_COLL_VOLUME_NOTIFICATION cd = {};
    cd.packet_size = CIGI_COLL_VOLUME_NOTIFICATION_SIZE;
    cd.packet_id = CIGI_COLL_VOLUME_NOTIFICATION_OPCODE;

    for (int i = 0; i < CDVolCount; i++) {
        // Populate a packet.
        cd.volume_id = (uint8_t)CDVolArray[i].id;
        cd.entity_id = CDVolArray[i].entity_id;
        cd.collision_type = !CDVolArray[i].entity_id;
        cd.contacted_entity_id = (value / 64) % 2;
        cd.contacted_volume_id = (value / 64) % 3;

        // Send the packet.  Note that we have already called
        // CigiStartMessage() for this frame.
        CigiAddFixedPacket<CIGI_COLL_VOLUME_NOTIFICATION>(session, &cd);

        // Change the data from frame to frame. (Static variable.)
        value++;
    }

    // "Clear" the array.
    CDVolCount = 0;
}

void AddIGMessages(void)
{
    static int value = 0;
    int length = 0;
    CString str;
    CIGI_IG_MESSAGE msg;

    str.Format("Value equals %d.", value++);
    length = str.GetLength() + 1;
    if (length % 4 > 0)
        length = (((length / 4) + 1) * 4);

    msg.packet_id = CIGI_IG_MESSAGE_OPCODE;
    msg.packet_size = 4 + length;
    strcpy(msg.text, (LPCTSTR)str);

    CigiAddPacketIGMessage(session, &msg);
}
