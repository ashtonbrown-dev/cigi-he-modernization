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
 *  FILENAME:   Hemu4.cpp
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
#include "cigi4.h"
#include "cigi4types.h"
#include "globals.h"
#include "Hemu4.h"
#include "HemuDoc.h"
#include "hemumsg.h"
#include "HemuView.h"
#include "MainFrm.h"
#include "ObjectTreeView.h"
#include "PacketHandlers.h"
#include "Script.h"
#include "SetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHemuApp

BEGIN_MESSAGE_MAP(CHemuApp, CWinApp)
    //{{AFX_MSG_MAP(CHemuApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    ON_COMMAND(ID_HELP_CONTENTS, OnHelpContents)
    ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
    //}}AFX_MSG_MAP
    // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
    // Global help commands
    //ON_COMMAND(ID_HELP_FINDER, CWinApp::OnHelpFinder)
    //ON_COMMAND(ID_HELP, CWinApp::OnHelp)
    //ON_COMMAND(ID_CONTEXT_HELP, CWinApp::OnContextHelp)
    //ON_COMMAND(ID_DEFAULT_HELP, CWinApp::OnHelpFinder)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHemuApp construction

CHemuApp::CHemuApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHemuApp object

CHemuApp theApp;

#ifdef _DEBUG
static void LaunchDummyIGForDebugging()
{
    if (!IsDebuggerPresent())
        return;

    char commandLine[] = "DummyIG4.exe 60 127.1.1.1 8015 8014";
    STARTUPINFO startupInfo = {sizeof(startupInfo)};
    PROCESS_INFORMATION processInfo = {0};

    if (CreateProcess(NULL, commandLine, NULL, NULL, FALSE,
                      CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &processInfo)) {
        CloseHandle(processInfo.hThread);
        CloseHandle(processInfo.hProcess);
    } else {
        MessageBox(NULL, "DummyIG4.exe could not be started.",
                   "Debug companion", MB_OK | MB_ICONWARNING);
    }
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CHemuApp initialization

BOOL CHemuApp::InitInstance()
{
    AfxEnableControlContainer();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

    // The registry key under which our settings are stored.
    SetRegistryKey(_T("Boeing TSS"));

    LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)

    // Initialize the Image Lists.
    CBitmap bmpTypes, bmpIcons;
    bmpTypes.LoadBitmap(IDB_ENTITY_TYPES);
    g_ImgListTypes.Create(16, 16, ILC_COLOR16 | ILC_MASK, 16, 16);
    g_ImgListTypes.Add(&bmpTypes, RGB(255, 230, 255));
    g_ImgListTypes.SetBkColor(CLR_NONE);    // make transparent
    bmpIcons.LoadBitmap(IDB_ICONS);
    g_ImgListIcons.Create(16, 16, ILC_COLOR16 | ILC_MASK, 16, 16);
    g_ImgListIcons.Add(&bmpIcons, RGB(255, 230, 255));
    g_ImgListIcons.SetBkColor(CLR_NONE);    // make transparent

    // Use the shared shutdown mutex to prevent duplicate host/driver pairs.
    HANDLE hTemp = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "HemuForceShutdownMutex");
    if (hTemp) {
        MessageBox(NULL, "An instance of the Host Emulator or driver is already "
                   "running.\nExit the application or use the Task Manager "
                   "to terminate the\nHemuDrv.exe process.", "Error", MB_ICONSTOP);

        CloseHandle(hTemp);

        return FALSE;
    }

    // Initialize our IPC so we can post messages from CMainFrame::OnCreate().
    InitializeIPC();

    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views.

    CSingleDocTemplate *pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CHemuDoc),
        RUNTIME_CLASS(CMainFrame),       // main SDI frame window
        RUNTIME_CLASS(CHemuView));
    AddDocTemplate(pDocTemplate);

    // Enable DDE Execute open
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Dispatch commands specified on the command line
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    // The one and only window has been initialized, so show and update it.
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    // Enable drag/drop open
    m_pMainWnd->DragAcceptFiles();

    ALLOC_CONSOLE(1024, 1024);

    // Initialize DirectInput for joystick use.
    if (DirectInput8Create(m_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&g_pIDirectInput, NULL) == DI_OK) {
        // Enumerate only the attached joysticks.
        g_pIDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
    } else {
        g_pIDirectInput = NULL;
        g_pJoystick = NULL;
    }

    // Set up CCU.
    InitializeCCU(TERRAIN_DEFAULT_RADIUS, TERRAIN_DEFAULT_FLATTENING);

    // Initialize CIGI.
    InitializeCIGI();

    // Set up the messaging queues and events.
    //InitializeIPC();  -- moved higher in the function so it's called before CMainFrame::OnCreate().
    GetMainFrame().StartTimer(4);   // 4Hz
    // After we have created CMainFrame, set up the trees.
    InitializeTrees();

    // After we have initialized the IPC message queues and tree controls, we
    // can apply the default values from configuration.
    g_DataManager.ApplyViewConfiguration();
    g_DataManager.ApplyTerrainConfiguration();
    g_DataManager.ApplyEnvConfiguration();

    // Simulate selection of the Entity tab.
    GetMainFrame().GetObjectTreeView().SelectTab(0);

    // Retrieve the network configuration and send a message to the driver.
    if (SetupCommFromRegistry() == FALSE)
        return FALSE;

#ifdef _DEBUG
    LaunchDummyIGForDebugging();
#endif
    LoadDriver();

    // Get the default database.
    SetupDBFromRegistry();

    // Set the CIGI version and byte order.
    SetupCigiOptionsFromRegistry();

    return TRUE;
}

void CHemuApp::InitializeTrees(void)
{
    GetMainFrame().GetEntityTree().SetImageList(&g_ImgListTypes, TVSIL_NORMAL);
    GetMainFrame().GetEnvTree().SetImageList(&g_ImgListIcons, TVSIL_NORMAL);
    GetMainFrame().GetViewTree().SetImageList(&g_ImgListIcons, TVSIL_NORMAL);

    g_htreeSky = GetMainFrame().GetEnvTree().InsertItem("Sky and Global Atmosphere",
                                                        IMG_SKY, IMG_SKY);
    GetMainFrame().GetEnvTree().SetItemData(g_htreeSky,
                                            MAKE_TREE_DATA(0, OBJECT_TYPE_SKY, 0));
    g_htreeTerrain = GetMainFrame().GetEnvTree().InsertItem("Terrain",
                                                            IMG_GLOBE, IMG_GLOBE);
    GetMainFrame().GetEnvTree().SetItemData(g_htreeTerrain,
                                            MAKE_TREE_DATA(0, OBJECT_TYPE_DATABASE, 0));
    //g_htreeRegions = GetMainFrame().GetEnvTree().InsertItem("Environmental Regions",
    //                                              IMG_REGION, IMG_REGION);
    //GetMainFrame().GetEnvTree().SetItemData(g_htreeRegions, OBJECT_TYPE_REGION);
}

BOOL CHemuApp::SetupDBFromRegistry(void)
{
    int dbnum = 0;

    if (!RetrieveDefaultDB(&dbnum))
        SetDefaultDB(1);
    else {
        SetDefaultDB(dbnum);
        g_DataManager.SetSelectedDatabase(dbnum);
    }

    // Check the default database.
    if (!g_DataManager.GetSelectedDatabase()) {
        dbnum = g_DataManager.FindFirstDatabaseNum();
        if (dbnum > 0) {
            SetDefaultDB(dbnum);
            g_DataManager.SetSelectedDatabase(dbnum);
        } else {
            AfxMessageBox("No database configured in TERRAIN.DEF.", MB_ICONEXCLAMATION);
            return FALSE;
        }
    }

    // Select the correct database item in the toolbar.
    GetMainFrame().UpdateDatabaseCombo();

    return TRUE;
}

BOOL CHemuApp::SetupCommFromRegistry(void)
{
    DWORD ipaddr = 0;
    unsigned short lport = 0;
    unsigned short rport = 0;

    if (!RetrieveIPAddr(&ipaddr)) {
        if (ShowSetupDlg() == FALSE)
            return FALSE;
    } else
        SetIPAddr(ipaddr);

    if (!RetrieveLocalPort(&lport)) {
        if (ShowSetupDlg() == FALSE)
            return FALSE;
    } else
        SetLocalPort(lport);

    if (!RetrieveRemotePort(&rport)) {
        if (ShowSetupDlg() == FALSE)
            return FALSE;
    } else
        SetRemotePort(rport);

    // Send a message to the driver.
    MESSAGE_SET_ADDR MsgSetAddr;
    IPAddrToStr(::GetIPAddr(), MsgSetAddr.ip_addr);
    MsgSetAddr.send_port = ::GetRemotePort();
    MsgSetAddr.rcv_port = ::GetLocalPort();
    PostDriverMsg(MsgSetAddr);

    return TRUE;
}

BOOL CHemuApp::SetupCigiOptionsFromRegistry(void)
{
    int byteorder = 0;
    int minor_version = 3;

    if (!RetrieveBigEndian(&byteorder))
        ::SetBigEndian(FALSE);
    else
        ::SetBigEndian(byteorder ? TRUE : FALSE);

    if (!RetrieveCigiMinorVersion(&minor_version))
        ::SetCigiMinorVersion(3);
    else
        ::SetCigiMinorVersion(minor_version);

    // Send a message to the driver.
    MESSAGE_SET_CIGI_OPTIONS MsgSetCigiOptions;
    MsgSetCigiOptions.big_endian = ::GetBigEndian();
    MsgSetCigiOptions.minor_version = ::GetCigiMinorVersion();
    PostDriverMsg(MsgSetCigiOptions);

    return TRUE;
}

BOOL CHemuApp::ShowSetupDlg(void)
{
    CSetupDlg dlg;
    const bool okPressed = (dlg.DoModal() == IDOK);

    if (okPressed) {
        ::SetCigiMinorVersion(dlg.GetCigiMinorVersion());
        ::StoreCigiMinorVersion(dlg.GetCigiMinorVersion());
        GetMainFrame().EnforceCigiVersion();

        ::SetIPAddr(dlg.GetIPAddr());
        ::StoreIPAddr(dlg.GetIPAddr());

        ::SetRemotePort(dlg.GetIGPort());
        ::StoreRemotePort(dlg.GetIGPort());

        ::SetLocalPort(dlg.GetLocalPort());
        ::StoreLocalPort(dlg.GetLocalPort());

        ::SetDefaultDB(dlg.GetDefaultDB());
        ::StoreDefaultDB(dlg.GetDefaultDB());

        ::SetBigEndian(dlg.GetBigEndian());
        ::StoreBigEndian(dlg.GetBigEndian());

        // Send a message to the driver.
        MESSAGE_SET_ADDR MsgSetAddr;
        IPAddrToStr(::GetIPAddr(), MsgSetAddr.ip_addr);
        MsgSetAddr.send_port = ::GetRemotePort();
        MsgSetAddr.rcv_port = ::GetLocalPort();
        PostDriverMsg(MsgSetAddr);

        MESSAGE_SET_CIGI_OPTIONS MsgSetCigiOptions;
        MsgSetCigiOptions.big_endian = ::GetBigEndian();
        MsgSetCigiOptions.minor_version = ::GetCigiMinorVersion();
        PostDriverMsg(MsgSetCigiOptions);

        /*
        g_DataManager.SetSelectedDatabase(dlg.GetDefaultDB());
        /*
        CTerrainDB *db = g_DataManager.GetDatabase(dlg.GetDefaultDB());
        MESSAGE_SET_DATABASE MsgSetDatabase;
        MsgSetDatabase.dbnum = dlg.GetDefaultDB();
        MsgSetDatabase.lat = db->GetOriginLat();
        MsgSetDatabase.lon = db->GetOriginLon();
        MsgSetDatabase.radius = db->GetErmRadius();
        MsgSetDatabase.flattening = db->GetErmFlattening();
        PostDriverMsg(MsgSetByteOrder);
        */
    }

    return okPressed;
}

void CHemuApp::InitializeCIGI(void)
{

    CigiInit(2, 4);     // Two sessions, version 4

    g_HostSession = CigiCreateSession(CIGI_HOST_SESSION, 8, MAX_ETHERNET_PACKET_SIZE);
    CigiSetCallback(CIGI_IG_CONTROL_OPCODE, DoCigiIGControl);
    CigiSetCallback(CIGI_ENTITY_POSITION_OPCODE, DoCigiEntityPosition);
    CigiSetCallback(CIGI_ENTITY_POSITION_CC_OPCODE, DoCigiCCEntityPosition);
    CigiSetCallback(CIGI_COMPONENT_CONTROL_OPCODE, DoCigiComponentControl);
    CigiSetCallback(CIGI_COMPONENT_CONTROL_S_OPCODE, DoCigiShortComponentControl);
    CigiSetCallback(CIGI_ARTPART_CONTROL_OPCODE, DoCigiArtPartControl);
    CigiSetCallback(CIGI_ARTPART_CONTROL_S_OPCODE, DoCigiShortArtPartControl);
    CigiSetCallback(CIGI_VELOCITY_CONTROL_OPCODE, DoCigiRateControl);
    CigiSetCallback(CIGI_CELESTIAL_SPHERE_CONTROL_OPCODE, DoCigiCelestialSphereControl);
    CigiSetCallback(CIGI_ATMOSPHERE_CONTROL_OPCODE, DoCigiAtmosphereControl);
    CigiSetCallback(CIGI_ENV_REGION_CONTROL_OPCODE, DoCigiEnvRegionControl);
    CigiSetCallback(CIGI_WEATHER_CONTROL_OPCODE, DoCigiWeatherControlOpcode);
    CigiSetCallback(CIGI_MARITIME_CONDITIONS_CONTROL_OPCODE, DoCigiMaritimeSurfaceControl);
    CigiSetCallback(CIGI_WAVE_CONTROL_OPCODE, DoCigiWaveControl);
    CigiSetCallback(CIGI_TERRESTRIAL_CONDITIONS_CONTROL_OPCODE, DoCigiTerrestrialSurfaceControl);
    CigiSetCallback(CIGI_VIEW_CONTROL_OPCODE, DoCigiViewControl);
    CigiSetCallback(CIGI_SENSOR_CONTROL_OPCODE, DoCigiSensorControl);
    CigiSetCallback(CIGI_MOTION_TRACKER_CONTROL_OPCODE, DoCigiMotionTrackerControl);
    CigiSetCallback(CIGI_ERM_DEF_OPCODE, DoCigiErmDef);
    CigiSetCallback(CIGI_ACCELERATION_CONTROL_DEF_OPCODE, DoCigiTrajectoryDef);
    CigiSetCallback(CIGI_VIEW_DEF_OPCODE, DoCigiViewDef);
    CigiSetCallback(CIGI_COLL_SEGMENT_DEF_OPCODE, DoCigiCollisionSegDef);
    CigiSetCallback(CIGI_COLL_VOLUME_DEF_OPCODE, DoCigiCollisionVolDef);
    CigiSetCallback(CIGI_HAT_HOT_REQUEST_OPCODE, DoCigiHatRequest);
    CigiSetCallback(CIGI_LOS_SEGMENT_REQUEST_OPCODE, DoCigiLosSegmentRequest);
    CigiSetCallback(CIGI_LOS_VECTOR_REQUEST_OPCODE, DoCigiLosVectorRequest);
    CigiSetCallback(CIGI_POSITION_REQUEST_OPCODE, DoCigiPositionRequest);
    CigiSetCallback(CIGI_ENV_CONDITIONS_REQUEST_OPCODE, DoCigiEnvConditionsRequest);
    CigiSetCallback(CIGI_SYMBOL_SURFACE_DEF_OPCODE, DoCigiSymbolSurfaceDef);
    CigiSetCallback(CIGI_SYMBOL_TEXT_DEF_OPCODE, DoCigiSymbolTextDef);
    CigiSetCallback(CIGI_SYMBOL_CIRCLE_DEF_OPCODE, DoCigiSymbolCircleDef);
    CigiSetCallback(CIGI_SYMBOL_POLYGON_DEF_OPCODE, DoCigiSymbolLineDef);
    CigiSetCallback(CIGI_SYMBOL_CLONE_OPCODE, DoCigiSymbolClone);
    CigiSetCallback(CIGI_SYMBOL_CONTROL_OPCODE, DoCigiSymbolControl);
    CigiSetCallback(CIGI_SYMBOL_CONTROL_S_OPCODE, DoCigiShortSymbolControl);
    CigiSetCallback(CIGI_ENTITY_CONTROL_OPCODE, DoCigiEntityControl);
    CigiSetCallback(CIGI_ANIMATION_CONTROL_OPCODE, DoCigiAnimationControl);

    g_IGSession = CigiCreateSession(CIGI_IG_SESSION, 8, MAX_ETHERNET_PACKET_SIZE);
    CigiSetCallback(100, DoSkippedFrame);                   // Skipped frame handler --> chas - needs opcode for v4.0
    CigiSetCallback(CIGI_START_OF_FRAME_OPCODE, DoCigiStartOfFrame);
    CigiSetCallback(CIGI_HAT_HOT_RESPONSE_OPCODE, DoCigiHatResponse);
    CigiSetCallback(CIGI_HAT_HOT_EXT_RESPONSE_OPCODE, DoCigiHatExtResponse);
    CigiSetCallback(CIGI_LOS_RESPONSE_OPCODE, DoCigiLosResponse);
    CigiSetCallback(CIGI_LOS_EXT_RESPONSE_OPCODE, DoCigiLosExtResponse);
    CigiSetCallback(CIGI_SENSOR_RESPONSE_OPCODE, DoCigiSensorResponse);
    CigiSetCallback(CIGI_SENSOR_EXT_RESPONSE_OPCODE, DoCigiSensorExtResponse);
    CigiSetCallback(CIGI_POSITION_RESPONSE_OPCODE, DoCigiPositionResponse);
    CigiSetCallback(CIGI_WEATHER_RESPONSE_OPCODE, DoCigiWeatherResponse);
    CigiSetCallback(CIGI_AEROSOL_RESPONSE_OPCODE, DoCigiAerosolResponse);
    CigiSetCallback(CIGI_MARITIME_SURFACE_RESPONSE_OPCODE, DoCigiMaritimeSurfaceResponse);
    CigiSetCallback(CIGI_TERRESTRIAL_SURFACE_RESPONSE_OPCODE, DoCigiTerrestrialSurfaceResponse);
    CigiSetCallback(CIGI_COLL_SEGMENT_NOTIFICATION_OPCODE, DoCigiCollSegmentNotification);
    CigiSetCallback(CIGI_COLL_VOLUME_NOTIFICATION_OPCODE, DoCigiCollVolumeNotification);
    CigiSetCallback(CIGI_ANIMATION_STOP_NOTIFICATION_OPCODE, DoCigiAnimStopNotification);
    CigiSetCallback(CIGI_EVENT_NOTIFICATION_OPCODE, DoCigiEventNotification);
    CigiSetCallback(CIGI_IG_MESSAGE_OPCODE, DoCigiIGMessage);
}

int CHemuApp::LoadDriver(void)
{
    // Create a mutex that the Windows driver can use to detect if this process has died.
    m_ForceShutdownMutexHandle = CreateMutex(NULL, TRUE, "HemuForceShutdownMutex");

    // Create an event so the scripting thread knows to shut down.
    g_ShutdownEventHdl = CreateEvent(NULL, TRUE, FALSE, NULL);

    // Load and initialize the Windows driver.
    CString DriverPath;
    {
        char directory[256];
        GetCurrentDirectory(256, directory);
        DriverPath.Format("\"%s\\HemuDrv.exe\"", directory);

        // Adjust the driver's short sleep behavior on multiprocessor systems.
        if (CheckForMultiProcessor())
            DriverPath += " -mp";

#ifdef _DEBUG
        DriverPath += " -v";
#endif
    }   // unload char array from the stack
    PROCESS_INFORMATION procinfo;
    STARTUPINFO si = {sizeof(si)};
#ifdef _DEBUG
    if (!CreateProcess(NULL, (LPTSTR)(LPCTSTR)DriverPath, 0, 0, 0,
                         CREATE_NEW_CONSOLE | HIGH_PRIORITY_CLASS, 0, 0, &si, &procinfo))
        MessageBox(NULL, "The driver cannot be loaded.", "Error", MB_ICONSTOP);
#else
    if (!CreateProcess(NULL, (LPTSTR)(LPCTSTR)DriverPath, 0, 0, 0,
                         DETACHED_PROCESS | HIGH_PRIORITY_CLASS, 0, 0, &si, &procinfo))
        MessageBox(NULL, "The driver cannot be loaded.", "Error", MB_ICONSTOP);
#endif  // _DEBUG

    /*
    // Send a message to the Windows driver to initialize the sockets.
    MESSAGE_SET_ADDR MsgSetAddr;
    IPAddrToStr(::GetIPAddr(), MsgSetAddr.ip_addr);
    MsgSetAddr.send_port = ::GetRemotePort();
    MsgSetAddr.rcv_port = ::GetLocalPort();

    // Open the hudcfg.def file and read the IP address.
    FILE *fhud = fopen("hudcfg.def", "r");
    if (fhud)
    {
        fscanf(fhud, "%s %d", &(MsgSetAddr.hud_ip_addr), &(MsgSetAddr.hud_send_port));
        fclose(fhud);
    }

    PostDriverMsg(MsgSetAddr);
    */

    return 1;
}

BOOL CHemuApp::CheckForMultiProcessor(void)
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return (sysinfo.dwNumberOfProcessors > 1);
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CHemuApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CHemuApp message handlers

int CHemuApp::ExitInstance()
{
    FREE_CONSOLE();

    CigiShutdown();

    return CWinApp::ExitInstance();
}

void CHemuApp::OnHelpContents()
{
    ::HtmlHelp(AfxGetMainWnd()->GetSafeHwnd(), "hemu4.chm", HH_DISPLAY_TOC, 0);
}

void CHemuApp::OnHelpIndex()
{
    ::HtmlHelp(AfxGetMainWnd()->GetSafeHwnd(), "hemu4.chm", HH_DISPLAY_INDEX, 0);
}
