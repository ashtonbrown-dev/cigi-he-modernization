// FilterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DummyIG4.h"
#include "FilterDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterDialog dialog

CFilterDialog::CFilterDialog(CWnd *pParent /*=NULL*/)
    : CDialog(CFilterDialog::IDD, pParent)
{
    //{{AFX_DATA_INIT(CFilterDialog)
    m_PrintArtPartControl = FALSE;
    m_PrintAtmosphereControl = FALSE;
    m_PrintCCEntityControl = FALSE;
    m_PrintCelestialSphereControl = FALSE;
    m_PrintCDVolumeDef = FALSE;
    m_PrintCDSegmentDef = FALSE;
    m_PrintComponentControl = FALSE;
    m_PrintEntityControl = TRUE;
    m_PrintEnvConditionsReq = FALSE;
    m_PrintEnvRegionControl = FALSE;
    m_PrintErmDef = FALSE;
    m_PrintHatReq = FALSE;
    m_PrintIGControl = TRUE;
    m_PrintLosSegmentReq = FALSE;
    m_PrintLosVectorReq = FALSE;
    m_PrintMaritimeCondControl = FALSE;
    m_PrintMotionTrackerControl = FALSE;
    m_PrintPositionReq = FALSE;
    m_PrintRateControl = FALSE;
    m_PrintSensorControl = FALSE;
    m_PrintShortArtPartControl = FALSE;
    m_PrintShortComponentControl = FALSE;
    m_PrintTerrestrialCondControl = FALSE;
    m_PrintTrajectoryDef = FALSE;
    m_PrintViewControl = FALSE;
    m_PrintViewDef = FALSE;
    m_PrintWaveControl = FALSE;
    m_PrintWeatherControl = FALSE;
    m_PrintFrameStart = FALSE;
    //}}AFX_DATA_INIT
}

void CFilterDialog::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFilterDialog)
    DDX_Check(pDX, IDC_CHECK_ART_PART_CONTROL, m_PrintArtPartControl);
    DDX_Check(pDX, IDC_CHECK_ATMOSPHERE_CONTROL, m_PrintAtmosphereControl);
    DDX_Check(pDX, IDC_CHECK_CC_ENTITY_CONTROL, m_PrintCCEntityControl);
    DDX_Check(pDX, IDC_CHECK_CELESTIAL_SPHERE_CONTROL, m_PrintCelestialSphereControl);
    DDX_Check(pDX, IDC_CHECK_COLL_VOL_DEF, m_PrintCDVolumeDef);
    DDX_Check(pDX, IDC_CHECK_COLL_SEG_DEF, m_PrintCDSegmentDef);
    DDX_Check(pDX, IDC_CHECK_COMPONENT_CONTROL, m_PrintComponentControl);
    DDX_Check(pDX, IDC_CHECK_ENTITY_CONTROL, m_PrintEntityControl);
    DDX_Check(pDX, IDC_CHECK_ENV_CONDITIONS_REQ, m_PrintEnvConditionsReq);
    DDX_Check(pDX, IDC_CHECK_ENV_REGION_CONTROL, m_PrintEnvRegionControl);
    DDX_Check(pDX, IDC_CHECK_ERM_DEFINITION, m_PrintErmDef);
    DDX_Check(pDX, IDC_CHECK_HAT_REQUEST, m_PrintHatReq);
    DDX_Check(pDX, IDC_CHECK_IG_CONTROL, m_PrintIGControl);
    DDX_Check(pDX, IDC_CHECK_LOS_SEGMENT_REQUEST, m_PrintLosSegmentReq);
    DDX_Check(pDX, IDC_CHECK_LOS_VECTOR_REQUEST, m_PrintLosVectorReq);
    DDX_Check(pDX, IDC_CHECK_MARITIME_COND_CONTROL, m_PrintMaritimeCondControl);
    DDX_Check(pDX, IDC_CHECK_MOTION_TRACKER_CONTROL, m_PrintMotionTrackerControl);
    DDX_Check(pDX, IDC_CHECK_POSITION_REQUEST, m_PrintPositionReq);
    DDX_Check(pDX, IDC_CHECK_RATE_CONTROL, m_PrintRateControl);
    DDX_Check(pDX, IDC_CHECK_SENSOR_CONTROL, m_PrintSensorControl);
    DDX_Check(pDX, IDC_CHECK_SHORT_ART_PART_CONTROL, m_PrintShortArtPartControl);
    DDX_Check(pDX, IDC_CHECK_SHORT_COMPONENT_CONTROL, m_PrintShortComponentControl);
    DDX_Check(pDX, IDC_CHECK_TERRESTRIAL_COND_CONTROL, m_PrintTerrestrialCondControl);
    DDX_Check(pDX, IDC_CHECK_TRAJ_DEF, m_PrintTrajectoryDef);
    DDX_Check(pDX, IDC_CHECK_VIEW_CONTROL, m_PrintViewControl);
    DDX_Check(pDX, IDC_CHECK_VIEW_DEF, m_PrintViewDef);
    DDX_Check(pDX, IDC_CHECK_WAVE_CONTROL, m_PrintWaveControl);
    DDX_Check(pDX, IDC_CHECK_WEATHER_CONTROL, m_PrintWeatherControl);
    DDX_Check(pDX, IDC_CHECK_FRAME_START, m_PrintFrameStart);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterDialog, CDialog)
    //{{AFX_MSG_MAP(CFilterDialog)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK_ENTITY_CONTROL, IDC_CHECK_ENV_CONDITIONS_REQ, OnCheck)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterDialog message handlers

void CFilterDialog::OnButtonClear()
{
    fflush(NULL);
    system("cls");  // Windows' command to clear a console screen.
}

void CFilterDialog::OnCheck(UINT ctrlId)
{
    UpdateData(TRUE);
}
