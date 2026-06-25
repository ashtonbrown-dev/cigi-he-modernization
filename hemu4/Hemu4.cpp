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
#include "CigiProtocolAdapter.h"
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

static const char *kDriverShutdownEventName = "HemuRTStartShutdownEvent";
static const DWORD kDriverShutdownWaitMs = 3000;
static const DWORD kDriverTerminateWaitMs = 1000;
static const int kDriverShutdownMessageAttempts = 50;

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
    : m_ForceShutdownMutexHandle(NULL),
      m_DriverProcessHandle(NULL),
      m_IpcInitialized(FALSE)
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
    m_IpcInitialized = (InitializeIPC() != 0);

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
    CigiProtocolVersion protocolVersion;

    if (!RetrieveBigEndian(&byteorder))
        ::SetBigEndian(FALSE);
    else
        ::SetBigEndian(byteorder ? TRUE : FALSE);

    if (!RetrieveCigiProtocolVersion(&protocolVersion))
        ::SetCigiProtocolVersion(CigiProtocolVersion::Current());
    else
        ::SetCigiProtocolVersion(protocolVersion);

    // Send a message to the driver.
    MESSAGE_SET_CIGI_OPTIONS MsgSetCigiOptions;
    MsgSetCigiOptions.big_endian = ::GetBigEndian();
    MsgSetCigiOptions.major_version =
        ::GetCigiProtocolVersion().GetMajorVersion();
    MsgSetCigiOptions.minor_version =
        ::GetCigiProtocolVersion().GetMinorVersion();
    PostDriverMsg(MsgSetCigiOptions);

    return TRUE;
}

BOOL CHemuApp::ShowSetupDlg(void)
{
    CSetupDlg dlg;
    const bool okPressed = (dlg.DoModal() == IDOK);

    if (okPressed) {
        ::SetCigiProtocolVersion(dlg.GetCigiProtocolVersion());
        ::StoreCigiProtocolVersion(dlg.GetCigiProtocolVersion());
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
        MsgSetCigiOptions.major_version =
            ::GetCigiProtocolVersion().GetMajorVersion();
        MsgSetCigiOptions.minor_version =
            ::GetCigiProtocolVersion().GetMinorVersion();
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

static void InitializeCigiParserCallbacks(CigiParserCallbacks *callbacks)
{
    if (!callbacks)
        return;

    callbacks->igControl = DoCigiIGControl;
    callbacks->entityPosition = DoCigiEntityPosition;
    callbacks->ccEntityPosition = DoCigiCCEntityPosition;
    callbacks->componentControl = DoCigiComponentControl;
    callbacks->shortComponentControl = DoCigiShortComponentControl;
    callbacks->artPartControl = DoCigiArtPartControl;
    callbacks->shortArtPartControl = DoCigiShortArtPartControl;
    callbacks->velocityControl = DoCigiRateControl;
    callbacks->celestialSphereControl = DoCigiCelestialSphereControl;
    callbacks->atmosphereControl = DoCigiAtmosphereControl;
    callbacks->envRegionControl = DoCigiEnvRegionControl;
    callbacks->weatherControl = DoCigiWeatherControlOpcode;
    callbacks->maritimeSurfaceControl = DoCigiMaritimeSurfaceControl;
    callbacks->waveControl = DoCigiWaveControl;
    callbacks->terrestrialSurfaceControl = DoCigiTerrestrialSurfaceControl;
    callbacks->viewControl = DoCigiViewControl;
    callbacks->sensorControl = DoCigiSensorControl;
    callbacks->motionTrackerControl = DoCigiMotionTrackerControl;
    callbacks->ermDef = DoCigiErmDef;
    callbacks->accelerationControlDef = DoCigiTrajectoryDef;
    callbacks->viewDef = DoCigiViewDef;
    callbacks->collisionSegmentDef = DoCigiCollisionSegDef;
    callbacks->collisionVolumeDef = DoCigiCollisionVolDef;
    callbacks->hatHotRequest = DoCigiHatRequest;
    callbacks->losSegmentRequest = DoCigiLosSegmentRequest;
    callbacks->losVectorRequest = DoCigiLosVectorRequest;
    callbacks->positionRequest = DoCigiPositionRequest;
    callbacks->envConditionsRequest = DoCigiEnvConditionsRequest;
    callbacks->symbolSurfaceDef = DoCigiSymbolSurfaceDef;
    callbacks->symbolTextDef = DoCigiSymbolTextDef;
    callbacks->symbolCircleDef = DoCigiSymbolCircleDef;
    callbacks->symbolPolygonDef = DoCigiSymbolLineDef;
    callbacks->symbolClone = DoCigiSymbolClone;
    callbacks->symbolControl = DoCigiSymbolControl;
    callbacks->shortSymbolControl = DoCigiShortSymbolControl;
    callbacks->entityControl = DoCigiEntityControl;
    callbacks->animationControl = DoCigiAnimationControl;

    callbacks->skippedFrame = DoSkippedFrame;
    callbacks->igResponses.startOfFrame = DoCigiStartOfFrame;
    callbacks->igResponses.hatHotResponse = DoCigiHatResponse;
    callbacks->igResponses.hatHotExtResponse = DoCigiHatExtResponse;
    callbacks->igResponses.losResponse = DoCigiLosResponse;
    callbacks->igResponses.losExtResponse = DoCigiLosExtResponse;
    callbacks->igResponses.sensorResponse = DoCigiSensorResponse;
    callbacks->igResponses.sensorExtResponse = DoCigiSensorExtResponse;
    callbacks->igResponses.positionResponse = DoCigiPositionResponse;
    callbacks->igResponses.weatherResponse = DoCigiWeatherResponse;
    callbacks->igResponses.aerosolResponse = DoCigiAerosolResponse;
    callbacks->igResponses.maritimeSurfaceResponse =
        DoCigiMaritimeSurfaceResponse;
    callbacks->igResponses.terrestrialSurfaceResponse =
        DoCigiTerrestrialSurfaceResponse;
    callbacks->igResponses.collisionSegmentNotification =
        DoCigiCollSegmentNotification;
    callbacks->igResponses.collisionVolumeNotification =
        DoCigiCollVolumeNotification;
    callbacks->igResponses.animationStopNotification =
        DoCigiAnimStopNotification;
    callbacks->igResponses.eventNotification = DoCigiEventNotification;
    callbacks->igResponses.igMessage = DoCigiIGMessage;
}

void CHemuApp::InitializeCIGI(void)
{
    CigiParserCallbacks callbacks = {0};
    InitializeCigiParserCallbacks(&callbacks);

    CigiProtocolAdapterSelection protocolSelection =
        CigiProtocolAdapterFactory::Select(::GetCigiProtocolVersion());
    CigiParserSessions sessions =
        protocolSelection.adapter->InitializeParserSessions(
            &callbacks,
            2,
            8,
            MAX_ETHERNET_PACKET_SIZE);

    g_HostSession = sessions.hostSession;
    g_IGSession = sessions.igSession;
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
    PROCESS_INFORMATION procinfo = {0};
    STARTUPINFO si = {sizeof(si)};
#ifdef _DEBUG
    if (!CreateProcess(NULL, (LPTSTR)(LPCTSTR)DriverPath, 0, 0, 0,
                         CREATE_NEW_CONSOLE | HIGH_PRIORITY_CLASS, 0, 0, &si, &procinfo)) {
        MessageBox(NULL, "The driver cannot be loaded.", "Error", MB_ICONSTOP);
        return 0;
    }
#else
    if (!CreateProcess(NULL, (LPTSTR)(LPCTSTR)DriverPath, 0, 0, 0,
                         DETACHED_PROCESS | HIGH_PRIORITY_CLASS, 0, 0, &si, &procinfo)) {
        MessageBox(NULL, "The driver cannot be loaded.", "Error", MB_ICONSTOP);
        return 0;
    }
#endif  // _DEBUG

    if (procinfo.hThread)
        CloseHandle(procinfo.hThread);
    m_DriverProcessHandle = procinfo.hProcess;

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

void CHemuApp::ShutdownDriver(void)
{
    HANDLE shutdownEventHandle = NULL;
    BOOL driverWasLaunched = (m_DriverProcessHandle != NULL);

    if (driverWasLaunched && m_IpcInitialized) {
        MESSAGE_SHUTDOWN shutdownMsg;
        for (int attempt = 0; attempt < kDriverShutdownMessageAttempts; ++attempt) {
            long result = g_GuiToDrvMsgQueue.Push((char *)&shutdownMsg, shutdownMsg.size);
            if (result >= 0)
                break;
            if (result == -2)
                break;
            Sleep(1);
        }
    }

    if (driverWasLaunched) {
        shutdownEventHandle = OpenEvent(EVENT_MODIFY_STATE, FALSE,
                                        kDriverShutdownEventName);
        if (!shutdownEventHandle)
            shutdownEventHandle = CreateEvent(NULL, TRUE, TRUE,
                                              kDriverShutdownEventName);
        if (shutdownEventHandle)
            SetEvent(shutdownEventHandle);
    }

    if (m_ForceShutdownMutexHandle) {
        ReleaseMutex(m_ForceShutdownMutexHandle);
        CloseHandle(m_ForceShutdownMutexHandle);
        m_ForceShutdownMutexHandle = NULL;
    }

    if (m_DriverProcessHandle) {
        DWORD waitResult = WaitForSingleObject(m_DriverProcessHandle,
                                               kDriverShutdownWaitMs);
        if (waitResult == WAIT_TIMEOUT) {
            // Last resort: a stale HemuDrv.exe blocks the next HEMU launch.
            // Clean shutdown was already requested through IPC, the named
            // event, and the GUI-death mutex before reaching this point.
            if (TerminateProcess(m_DriverProcessHandle, 0))
                WaitForSingleObject(m_DriverProcessHandle,
                                    kDriverTerminateWaitMs);
        }

        CloseHandle(m_DriverProcessHandle);
        m_DriverProcessHandle = NULL;
    }

    if (shutdownEventHandle)
        CloseHandle(shutdownEventHandle);
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
    ShutdownDriver();

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
