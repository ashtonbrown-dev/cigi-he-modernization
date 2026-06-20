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
 *  FILENAME:   FilterIncomingDlg.cpp
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
#include "FilterIncomingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterIncomingDlg dialog

CFilterIncomingDlg::CFilterIncomingDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CFilterIncomingDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CFilterIncomingDlg)
    m_AerosolResp = TRUE;
    m_AnimationStopNotification = TRUE;
    m_CDSegmentNotification = TRUE;
    m_CDVolumeNotification = TRUE;
    m_EventNotification = TRUE;
    m_HatHotResp = TRUE;
    m_HatHotExtResp = TRUE;
    m_IGMessage = TRUE;
    m_LosResp = TRUE;
    m_LosExtResp = TRUE;
    m_MaritimeCondResp = TRUE;
    m_PositionResp = TRUE;
    m_SensorResp = TRUE;
    m_SensorExtResp = TRUE;
    m_StartOfFrame = TRUE;
    m_TerrestrialCondResp = TRUE;
    m_WeatherCondResp = TRUE;
    //}}AFX_DATA_INIT
}

void CFilterIncomingDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFilterIncomingDlg)
    DDX_Check(pDX, IDC_CHECK_AEROSOL, m_AerosolResp);
    DDX_Check(pDX, IDC_CHECK_ANIMATION, m_AnimationStopNotification);
    DDX_Check(pDX, IDC_CHECK_CD_SEGMENT, m_CDSegmentNotification);
    DDX_Check(pDX, IDC_CHECK_CD_VOLUME, m_CDVolumeNotification);
    DDX_Check(pDX, IDC_CHECK_EVENT, m_EventNotification);
    DDX_Check(pDX, IDC_CHECK_HATHOT, m_HatHotResp);
    DDX_Check(pDX, IDC_CHECK_HATHOT_EXT, m_HatHotExtResp);
    DDX_Check(pDX, IDC_CHECK_IG_MESSAGE, m_IGMessage);
    DDX_Check(pDX, IDC_CHECK_LOS, m_LosResp);
    DDX_Check(pDX, IDC_CHECK_LOS_EXT, m_LosExtResp);
    DDX_Check(pDX, IDC_CHECK_MARITIME, m_MaritimeCondResp);
    DDX_Check(pDX, IDC_CHECK_POSITION, m_PositionResp);
    DDX_Check(pDX, IDC_CHECK_SENSOR, m_SensorResp);
    DDX_Check(pDX, IDC_CHECK_SENSOR_EXT, m_SensorExtResp);
    DDX_Check(pDX, IDC_CHECK_SOF, m_StartOfFrame);
    DDX_Check(pDX, IDC_CHECK_TERRESTRIAL, m_TerrestrialCondResp);
    DDX_Check(pDX, IDC_CHECK_WEATHER, m_WeatherCondResp);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterIncomingDlg, CDialog)
    //{{AFX_MSG_MAP(CFilterIncomingDlg)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_NONE, OnButtonSelectNone)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterIncomingDlg message handlers

void CFilterIncomingDlg::OnButtonSelectAll()
{
    m_AerosolResp = TRUE;
    m_AnimationStopNotification = TRUE;
    m_CDSegmentNotification = TRUE;
    m_CDVolumeNotification = TRUE;
    m_EventNotification = TRUE;
    m_HatHotResp = TRUE;
    m_HatHotExtResp = TRUE;
    m_IGMessage = TRUE;
    m_LosResp = TRUE;
    m_LosExtResp = TRUE;
    m_MaritimeCondResp = TRUE;
    m_PositionResp = TRUE;
    m_SensorResp = TRUE;
    m_SensorExtResp = TRUE;
    m_StartOfFrame = TRUE;
    m_TerrestrialCondResp = TRUE;
    m_WeatherCondResp = TRUE;

    UpdateData(FALSE);
}

void CFilterIncomingDlg::OnButtonSelectNone()
{
    m_AerosolResp = FALSE;
    m_AnimationStopNotification = FALSE;
    m_CDSegmentNotification = FALSE;
    m_CDVolumeNotification = FALSE;
    m_EventNotification = FALSE;
    m_HatHotResp = FALSE;
    m_HatHotExtResp = FALSE;
    m_IGMessage = FALSE;
    m_LosResp = FALSE;
    m_LosExtResp = FALSE;
    m_MaritimeCondResp = FALSE;
    m_PositionResp = FALSE;
    m_SensorResp = FALSE;
    m_SensorExtResp = FALSE;
    m_StartOfFrame = FALSE;
    m_TerrestrialCondResp = FALSE;
    m_WeatherCondResp = FALSE;

    UpdateData(FALSE);
}
