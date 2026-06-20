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

// PacketDlgEnvRegionControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "PacketDlgEnvRegionControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgEnvRegionControl dialog

CPacketDlgEnvRegionControl::CPacketDlgEnvRegionControl(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgEnvRegionControl::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgEnvRegionControl)
    m_Latitude = 0.0;
    m_Longitude = 0.0;
    m_TransitionPerimeter = 0.0f;
    m_CornerRadius = 0.0f;
    m_RegionID = 0;
    m_Rotation = 0.0f;
    m_SizeX = 0.0f;
    m_SizeY = 0.0f;
    m_MergeAerosol = 0;
    m_MergeMaritime = 0;
    m_MergeTerrestrial = 0;
    m_MergeWeather = 0;
    m_RegionState = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgEnvRegionControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgEnvRegionControl)
    DDX_Text(pDX, IDC_EDIT_LATITUDE, m_Latitude);
    DDX_Text(pDX, IDC_EDIT_LONGITUDE, m_Longitude);
    DDX_Text(pDX, IDC_EDIT_PERIMETER, m_TransitionPerimeter);
    DDX_Text(pDX, IDC_EDIT_RADIUS, m_CornerRadius);
    DDX_Text(pDX, IDC_EDIT_REGION_ID, m_RegionID);
    DDX_Text(pDX, IDC_EDIT_ROTATION, m_Rotation);
    DDX_Text(pDX, IDC_EDIT_SIZE_X, m_SizeX);
    DDX_Text(pDX, IDC_EDIT_SIZE_Y, m_SizeY);
    DDX_Radio(pDX, IDC_RADIO_AEROSOL_USELAST, m_MergeAerosol);
    DDX_Radio(pDX, IDC_RADIO_MARITIME_USELAST, m_MergeMaritime);
    DDX_Radio(pDX, IDC_RADIO_TERRESTRIAL_USELAST, m_MergeTerrestrial);
    DDX_Radio(pDX, IDC_RADIO_WEATHER_USELAST, m_MergeWeather);
    DDX_Radio(pDX, IDC_RADIO_REGION_STATE_INACTIVE, m_RegionState);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgEnvRegionControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgEnvRegionControl)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgEnvRegionControl message handlers

void CPacketDlgEnvRegionControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgEnvRegionControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_ENV_REGION_CONTROL rc = {0};
    rc.packet_id = CIGI_ENV_REGION_CONTROL_OPCODE;
    rc.packet_size = sizeof(CIGI_ENV_REGION_CONTROL);
    rc.region_id = m_RegionID;
    rc.region_state = m_RegionState;
    rc.latitude = m_Latitude;
    rc.longitude = m_Longitude;
    rc.transition_perimeter = m_TransitionPerimeter;
    rc.corner_radius = m_CornerRadius;
    rc.rotation = m_Rotation;
    rc.size_x = m_SizeX;
    rc.size_y = m_SizeY;
    rc.merge_aerosols = m_MergeAerosol;
    rc.merge_maritime = m_MergeMaritime;
    rc.merge_terrestrial = m_MergeTerrestrial;
    rc.merge_weather = m_MergeWeather;

    SendImmedCigiMessage(&rc);
}
