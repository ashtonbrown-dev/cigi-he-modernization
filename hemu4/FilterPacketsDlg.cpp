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
 *  FILENAME:   FilterPacketsDlg.cpp
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
#include "globals.h"
#include "FilterPacketsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterPacketsDlg dialog

CFilterPacketsDlg::CFilterPacketsDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CFilterPacketsDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CFilterPacketsDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CFilterPacketsDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFilterPacketsDlg)
    DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterPacketsDlg, CDialog)
    //{{AFX_MSG_MAP(CFilterPacketsDlg)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab)
    ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnSelchangingTab)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterPacketsDlg message handlers

void CFilterPacketsDlg::OnOK()
{
    // IG-to-Host packets.
    m_DlgIncoming.UpdateData(TRUE);
    g_FilterFlags.StartOfFrame = m_DlgIncoming.m_StartOfFrame;
    g_FilterFlags.HatResp = m_DlgIncoming.m_HatHotResp;
    g_FilterFlags.HatExtResp = m_DlgIncoming.m_HatHotExtResp;
    g_FilterFlags.LosResp = m_DlgIncoming.m_LosResp;
    g_FilterFlags.LosExtResp = m_DlgIncoming.m_LosExtResp;
    g_FilterFlags.SensorResp = m_DlgIncoming.m_SensorResp;
    g_FilterFlags.SensorExtResp = m_DlgIncoming.m_SensorExtResp;
    g_FilterFlags.PositionResp = m_DlgIncoming.m_PositionResp;
    g_FilterFlags.WeatherCondResp = m_DlgIncoming.m_WeatherCondResp;
    g_FilterFlags.AerosolResp = m_DlgIncoming.m_AerosolResp;
    g_FilterFlags.MaritimeCondResp = m_DlgIncoming.m_MaritimeCondResp;
    g_FilterFlags.TerrestrialCondResp = m_DlgIncoming.m_TerrestrialCondResp;
    g_FilterFlags.CDSegmentNotification = m_DlgIncoming.m_CDSegmentNotification;
    g_FilterFlags.CDVolumeNotification = m_DlgIncoming.m_CDVolumeNotification;
    g_FilterFlags.AnimationStopNotification = m_DlgIncoming.m_AnimationStopNotification;
    g_FilterFlags.EventNotification = m_DlgIncoming.m_EventNotification;
    g_FilterFlags.IGMessage = m_DlgIncoming.m_IGMessage;

    // Host-to-IG packets.
    m_DlgOutgoing.UpdateData(TRUE);
    g_FilterFlags.ArtPartControl = m_DlgOutgoing.m_ArtPartControl;
    g_FilterFlags.AtmosphereControl = m_DlgOutgoing.m_AtmosphereControl;
    g_FilterFlags.CCEntityControl = m_DlgOutgoing.m_CCEntityControl;
    g_FilterFlags.CDSegmentDef = m_DlgOutgoing.m_CDSegmentDef;
    g_FilterFlags.CDVolumeDef = m_DlgOutgoing.m_CDVolumeDef;
    g_FilterFlags.CelestialSphereControl = m_DlgOutgoing.m_CelestialSphereControl;
    g_FilterFlags.ComponentControl = m_DlgOutgoing.m_ComponentControl;
    g_FilterFlags.EntityControl = m_DlgOutgoing.m_EntityControl;
    g_FilterFlags.EnvRegionControl = m_DlgOutgoing.m_EnvRegionControl;
    g_FilterFlags.EnvConditionsReq = m_DlgOutgoing.m_EnvConditionsReq;
    g_FilterFlags.ErmDef = m_DlgOutgoing.m_ErmDef;
    g_FilterFlags.HatHotReq = m_DlgOutgoing.m_HatHotReq;
    g_FilterFlags.IGControl = m_DlgOutgoing.m_IGControl;
    g_FilterFlags.LOSSegmentReq = m_DlgOutgoing.m_LOSSegmentReq;
    g_FilterFlags.LOSVectorReq = m_DlgOutgoing.m_LOSVectorReq;
    g_FilterFlags.MaritimeCondControl = m_DlgOutgoing.m_MaritimeCondControl;
    g_FilterFlags.PositionReq = m_DlgOutgoing.m_PositionReq;
    g_FilterFlags.RateControl = m_DlgOutgoing.m_RateControl;
    g_FilterFlags.SensorControl = m_DlgOutgoing.m_SensorControl;
    g_FilterFlags.ShortArtPartControl = m_DlgOutgoing.m_ShortArtPartControl;
    g_FilterFlags.ShortComponentControl = m_DlgOutgoing.m_ShortComponentControl;
    g_FilterFlags.TerrestrialCondControl = m_DlgOutgoing.m_TerrestrialCondControl;
    g_FilterFlags.MotionTrackerControl = m_DlgOutgoing.m_MotionTrackerControl;
    g_FilterFlags.TrajectoryDef = m_DlgOutgoing.m_TrajectoryDef;
    g_FilterFlags.ViewControl = m_DlgOutgoing.m_ViewControl;
    g_FilterFlags.ViewDef = m_DlgOutgoing.m_ViewDef;
    g_FilterFlags.WaveControl = m_DlgOutgoing.m_WaveControl;
    g_FilterFlags.WeatherControl = m_DlgOutgoing.m_WeatherControl;
    g_FilterFlags.SymbolSurfaceDef = m_DlgOutgoing.m_SymbolSurfaceDef;
    g_FilterFlags.SymbolTextDef = m_DlgOutgoing.m_SymbolTextDef;
    g_FilterFlags.SymbolCircleDef = m_DlgOutgoing.m_SymbolCircleDef;
    g_FilterFlags.SymbolLineDef = m_DlgOutgoing.m_SymbolLineDef;
    g_FilterFlags.SymbolClone = m_DlgOutgoing.m_SymbolClone;
    g_FilterFlags.SymbolControl = m_DlgOutgoing.m_SymbolControl;
    g_FilterFlags.ShortSymbolControl = m_DlgOutgoing.m_ShortSymbolControl;

    CDialog::OnOK();
}

void CFilterPacketsDlg::OnCancel()
{
    CDialog::OnCancel();
}

BOOL CFilterPacketsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    InitTabs();

    // IG-to-Host packets.
    m_DlgIncoming.m_StartOfFrame = g_FilterFlags.StartOfFrame;
    m_DlgIncoming.m_HatHotResp = g_FilterFlags.HatResp;
    m_DlgIncoming.m_HatHotExtResp = g_FilterFlags.HatExtResp;
    m_DlgIncoming.m_LosResp = g_FilterFlags.LosResp;
    m_DlgIncoming.m_LosExtResp = g_FilterFlags.LosExtResp;
    m_DlgIncoming.m_SensorResp = g_FilterFlags.SensorResp;
    m_DlgIncoming.m_SensorExtResp = g_FilterFlags.SensorExtResp;
    m_DlgIncoming.m_PositionResp = g_FilterFlags.PositionResp;
    m_DlgIncoming.m_WeatherCondResp = g_FilterFlags.WeatherCondResp;
    m_DlgIncoming.m_AerosolResp = g_FilterFlags.AerosolResp;
    m_DlgIncoming.m_MaritimeCondResp = g_FilterFlags.MaritimeCondResp;
    m_DlgIncoming.m_TerrestrialCondResp = g_FilterFlags.TerrestrialCondResp;
    m_DlgIncoming.m_CDSegmentNotification = g_FilterFlags.CDSegmentNotification;
    m_DlgIncoming.m_CDVolumeNotification = g_FilterFlags.CDVolumeNotification;
    m_DlgIncoming.m_AnimationStopNotification = g_FilterFlags.AnimationStopNotification;
    m_DlgIncoming.m_EventNotification = g_FilterFlags.EventNotification;
    m_DlgIncoming.m_IGMessage = g_FilterFlags.IGMessage;
    m_DlgIncoming.UpdateData(FALSE);

    // Host-to-IG packets.
    m_DlgOutgoing.m_ArtPartControl = g_FilterFlags.ArtPartControl;
    m_DlgOutgoing.m_AtmosphereControl = g_FilterFlags.AtmosphereControl;
    m_DlgOutgoing.m_CCEntityControl = g_FilterFlags.CCEntityControl;
    m_DlgOutgoing.m_CDSegmentDef = g_FilterFlags.CDSegmentDef;
    m_DlgOutgoing.m_CDVolumeDef = g_FilterFlags.CDVolumeDef;
    m_DlgOutgoing.m_CelestialSphereControl = g_FilterFlags.CelestialSphereControl;
    m_DlgOutgoing.m_ComponentControl = g_FilterFlags.ComponentControl;
    m_DlgOutgoing.m_EntityControl = g_FilterFlags.EntityControl;
    m_DlgOutgoing.m_EnvRegionControl = g_FilterFlags.EnvRegionControl;
    m_DlgOutgoing.m_EnvConditionsReq = g_FilterFlags.EnvConditionsReq;
    m_DlgOutgoing.m_ErmDef = g_FilterFlags.ErmDef;
    m_DlgOutgoing.m_HatHotReq = g_FilterFlags.HatHotReq;
    m_DlgOutgoing.m_IGControl = g_FilterFlags.IGControl;
    m_DlgOutgoing.m_LOSSegmentReq = g_FilterFlags.LOSSegmentReq;
    m_DlgOutgoing.m_LOSVectorReq = g_FilterFlags.LOSVectorReq;
    m_DlgOutgoing.m_MaritimeCondControl = g_FilterFlags.MaritimeCondControl;
    m_DlgOutgoing.m_PositionReq = g_FilterFlags.PositionReq;
    m_DlgOutgoing.m_RateControl = g_FilterFlags.RateControl;
    m_DlgOutgoing.m_SensorControl = g_FilterFlags.SensorControl;
    m_DlgOutgoing.m_ShortArtPartControl = g_FilterFlags.ShortArtPartControl;
    m_DlgOutgoing.m_ShortComponentControl = g_FilterFlags.ShortComponentControl;
    m_DlgOutgoing.m_TerrestrialCondControl = g_FilterFlags.TerrestrialCondControl;
    m_DlgOutgoing.m_MotionTrackerControl = g_FilterFlags.MotionTrackerControl;
    m_DlgOutgoing.m_TrajectoryDef = g_FilterFlags.TrajectoryDef;
    m_DlgOutgoing.m_ViewControl = g_FilterFlags.ViewControl;
    m_DlgOutgoing.m_ViewDef = g_FilterFlags.ViewDef;
    m_DlgOutgoing.m_WaveControl = g_FilterFlags.WaveControl;
    m_DlgOutgoing.m_WeatherControl = g_FilterFlags.WeatherControl;
    m_DlgOutgoing.m_SymbolSurfaceDef = g_FilterFlags.SymbolSurfaceDef;
    m_DlgOutgoing.m_SymbolTextDef = g_FilterFlags.SymbolTextDef;
    m_DlgOutgoing.m_SymbolCircleDef = g_FilterFlags.SymbolCircleDef;
    m_DlgOutgoing.m_SymbolLineDef = g_FilterFlags.SymbolLineDef;
    m_DlgOutgoing.m_SymbolClone = g_FilterFlags.SymbolClone;
    m_DlgOutgoing.m_SymbolControl = g_FilterFlags.SymbolControl;
    m_DlgOutgoing.m_ShortSymbolControl = g_FilterFlags.ShortSymbolControl;
    m_DlgOutgoing.UpdateData(FALSE);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CFilterPacketsDlg::InitTabs(void)
{
    // Add a tab for each of the child dialog boxes.
    m_TabCtrl.InsertItem(0, "IG-to-Host");
    m_TabCtrl.InsertItem(1, "Host-to-IG");

    // Lock the resources for the child dialog boxes.
    m_DlgTempl[0] = LockDlgRes(MAKEINTRESOURCE(IDD_FILTER_PAGE_INCOMING));
    m_DlgTempl[1] = LockDlgRes(MAKEINTRESOURCE(IDD_FILTER_PAGE_OUTGOING));

    // Create an array of pointers to the child dialog boxes.
    m_TabDlg[0] = (CDialog *)&m_DlgIncoming;
    m_TabDlg[1] = (CDialog *)&m_DlgOutgoing;

    // Create child dialog boxes.
    for (int i = 0; i < 2; i++)
        m_TabDlg[i]->CreateIndirect(m_DlgTempl[i], &m_TabCtrl);

    for (int i = 0; i < 2; i++)
        m_TabDlg[i]->SetWindowPos(NULL, 5, 25, 0, 0, SWP_NOSIZE);

    // Simulate selection of the first item.
    LRESULT result;
    OnSelchangeTab(NULL, &result);
}

void CFilterPacketsDlg::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Show the appropriate window.
    int index = m_TabCtrl.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_SHOW);
    }

    *pResult = 0;
}

void CFilterPacketsDlg::OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Hide the current window.
    int index = m_TabCtrl.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_HIDE);
    }

    *pResult = 0;
}
