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
 *  FILENAME:   FilterOutgoingDlg.cpp
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
#include "hemu4.h"
#include "FilterOutgoingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterOutgoingDlg dialog

CFilterOutgoingDlg::CFilterOutgoingDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CFilterOutgoingDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CFilterOutgoingDlg)
    m_ArtPartControl = TRUE;
    m_AtmosphereControl = TRUE;
    m_CCEntityControl = TRUE;
    m_CDSegmentDef = TRUE;
    m_CDVolumeDef = TRUE;
    m_CelestialSphereControl = TRUE;
    m_ComponentControl = TRUE;
    m_EntityControl = TRUE;
    m_EnvRegionControl = TRUE;
    m_EnvConditionsReq = TRUE;
    m_ErmDef = TRUE;
    m_HatHotReq = TRUE;
    m_IGControl = TRUE;
    m_LOSSegmentReq = TRUE;
    m_LOSVectorReq = TRUE;
    m_MaritimeCondControl = TRUE;
    m_PositionReq = TRUE;
    m_RateControl = TRUE;
    m_SensorControl = TRUE;
    m_ShortArtPartControl = TRUE;
    m_ShortComponentControl = TRUE;
    m_TerrestrialCondControl = TRUE;
    m_MotionTrackerControl = TRUE;
    m_TrajectoryDef = TRUE;
    m_ViewControl = TRUE;
    m_ViewDef = TRUE;
    m_WaveControl = TRUE;
    m_WeatherControl = TRUE;
    m_SymbolSurfaceDef = TRUE;
    m_SymbolTextDef = TRUE;
    m_SymbolCircleDef = TRUE;
    m_SymbolLineDef = TRUE;
    m_SymbolClone = TRUE;
    m_SymbolControl = TRUE;
    m_ShortSymbolControl = TRUE;
    //}}AFX_DATA_INIT
}

void CFilterOutgoingDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFilterOutgoingDlg)
    DDX_Check(pDX, IDC_CHECK_ARTPART_CONTROL, m_ArtPartControl);
    DDX_Check(pDX, IDC_CHECK_ATMOSPHERE_CONTROL, m_AtmosphereControl);
    DDX_Check(pDX, IDC_CHECK_CC_ENTITY_CONTROL, m_CCEntityControl);
    DDX_Check(pDX, IDC_CHECK_CD_SEGMENT_DEF, m_CDSegmentDef);
    DDX_Check(pDX, IDC_CHECK_CD_VOLUME_DEF, m_CDVolumeDef);
    DDX_Check(pDX, IDC_CHECK_CELESTIAL_CONTROL, m_CelestialSphereControl);
    DDX_Check(pDX, IDC_CHECK_COMPONENT_CONTROL, m_ComponentControl);
    DDX_Check(pDX, IDC_CHECK_ENTITY_CONTROL, m_EntityControl);
    DDX_Check(pDX, IDC_CHECK_ENV_REGION_CONTROL, m_EnvRegionControl);
    DDX_Check(pDX, IDC_CHECK_ENV_CONDITIONS_REQUEST, m_EnvConditionsReq);
    DDX_Check(pDX, IDC_CHECK_ERM_DEF, m_ErmDef);
    DDX_Check(pDX, IDC_CHECK_HATHOT_REQUEST, m_HatHotReq);
    DDX_Check(pDX, IDC_CHECK_IG_CONTROL, m_IGControl);
    DDX_Check(pDX, IDC_CHECK_LOS_SEGMENT_REQUEST, m_LOSSegmentReq);
    DDX_Check(pDX, IDC_CHECK_LOS_VECTOR_REQUEST, m_LOSVectorReq);
    DDX_Check(pDX, IDC_CHECK_MARITIME_CONTROL, m_MaritimeCondControl);
    DDX_Check(pDX, IDC_CHECK_POSITION_REQUEST, m_PositionReq);
    DDX_Check(pDX, IDC_CHECK_RATE_CONTROL, m_RateControl);
    DDX_Check(pDX, IDC_CHECK_SENSOR_CONTROL, m_SensorControl);
    DDX_Check(pDX, IDC_CHECK_SHORT_ARTPART_CONTROL, m_ShortArtPartControl);
    DDX_Check(pDX, IDC_CHECK_SHORT_COMPONENT_CONTROL, m_ShortComponentControl);
    DDX_Check(pDX, IDC_CHECK_TERRESTRIAL_CONTROL, m_TerrestrialCondControl);
    DDX_Check(pDX, IDC_CHECK_TRACKER, m_MotionTrackerControl);
    DDX_Check(pDX, IDC_CHECK_TRAJECTORY_DEF, m_TrajectoryDef);
    DDX_Check(pDX, IDC_CHECK_VIEW_CONTROL, m_ViewControl);
    DDX_Check(pDX, IDC_CHECK_VIEW_DEF, m_ViewDef);
    DDX_Check(pDX, IDC_CHECK_WAVE_CONTROL, m_WaveControl);
    DDX_Check(pDX, IDC_CHECK_WEATHER_CONTROL, m_WeatherControl);
    DDX_Check(pDX, IDC_CHECK_SYMBOL_SURFACE_DEF, m_SymbolSurfaceDef);
    DDX_Check(pDX, IDC_CHECK_SYMBOL_TEXT_DEF, m_SymbolTextDef);
    DDX_Check(pDX, IDC_CHECK_SYMBOL_CIRCLE_DEF, m_SymbolCircleDef);
    DDX_Check(pDX, IDC_CHECK_SYMBOL_LINE_DEF, m_SymbolLineDef);
    DDX_Check(pDX, IDC_CHECK_SYMBOL_CLONE, m_SymbolClone);
    DDX_Check(pDX, IDC_CHECK_SYMBOL_CONTROL, m_SymbolControl);
    DDX_Check(pDX, IDC_CHECK_SHORT_SYMBOL_CONTROL, m_ShortSymbolControl);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterOutgoingDlg, CDialog)
    //{{AFX_MSG_MAP(CFilterOutgoingDlg)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_NONE, OnButtonSelectNone)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterOutgoingDlg message handlers

void CFilterOutgoingDlg::OnButtonSelectAll()
{
    m_ArtPartControl = TRUE;
    m_AtmosphereControl = TRUE;
    m_CCEntityControl = TRUE;
    m_CDSegmentDef = TRUE;
    m_CDVolumeDef = TRUE;
    m_CelestialSphereControl = TRUE;
    m_ComponentControl = TRUE;
    m_EntityControl = TRUE;
    m_EnvRegionControl = TRUE;
    m_EnvConditionsReq = TRUE;
    m_ErmDef = TRUE;
    m_HatHotReq = TRUE;
    m_IGControl = TRUE;
    m_LOSSegmentReq = TRUE;
    m_LOSVectorReq = TRUE;
    m_MaritimeCondControl = TRUE;
    m_PositionReq = TRUE;
    m_RateControl = TRUE;
    m_SensorControl = TRUE;
    m_ShortArtPartControl = TRUE;
    m_ShortComponentControl = TRUE;
    m_TerrestrialCondControl = TRUE;
    m_MotionTrackerControl = TRUE;
    m_TrajectoryDef = TRUE;
    m_ViewControl = TRUE;
    m_ViewDef = TRUE;
    m_WaveControl = TRUE;
    m_WeatherControl = TRUE;
    m_SymbolSurfaceDef = TRUE;
    m_SymbolTextDef = TRUE;
    m_SymbolCircleDef = TRUE;
    m_SymbolLineDef = TRUE;
    m_SymbolClone = TRUE;
    m_SymbolControl = TRUE;
    m_ShortSymbolControl = TRUE;

    UpdateData(FALSE);
}

void CFilterOutgoingDlg::OnButtonSelectNone()
{
    m_ArtPartControl = FALSE;
    m_AtmosphereControl = FALSE;
    m_CCEntityControl = FALSE;
    m_CDSegmentDef = FALSE;
    m_CDVolumeDef = FALSE;
    m_CelestialSphereControl = FALSE;
    m_ComponentControl = FALSE;
    m_EntityControl = FALSE;
    m_EnvRegionControl = FALSE;
    m_EnvConditionsReq = FALSE;
    m_ErmDef = FALSE;
    m_HatHotReq = FALSE;
    m_IGControl = FALSE;
    m_LOSSegmentReq = FALSE;
    m_LOSVectorReq = FALSE;
    m_MaritimeCondControl = FALSE;
    m_PositionReq = FALSE;
    m_RateControl = FALSE;
    m_SensorControl = FALSE;
    m_ShortArtPartControl = FALSE;
    m_ShortComponentControl = FALSE;
    m_TerrestrialCondControl = FALSE;
    m_MotionTrackerControl = FALSE;
    m_TrajectoryDef = FALSE;
    m_ViewControl = FALSE;
    m_ViewDef = FALSE;
    m_WaveControl = FALSE;
    m_WeatherControl = FALSE;
    m_SymbolSurfaceDef = FALSE;
    m_SymbolTextDef = FALSE;
    m_SymbolCircleDef = FALSE;
    m_SymbolLineDef = FALSE;
    m_SymbolClone = FALSE;
    m_SymbolControl = FALSE;
    m_ShortSymbolControl = FALSE;

    UpdateData(FALSE);
}
