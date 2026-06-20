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

// PacketDlgEnvConditionsReq.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "PacketDlgEnvConditionsReq.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgEnvConditionsReq dialog

CPacketDlgEnvConditionsReq::CPacketDlgEnvConditionsReq(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgEnvConditionsReq::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgEnvConditionsReq)
    m_Altitude = 0.0;
    m_Latitude = 0.0;
    m_Longitude = 0.0;
    m_RequestID = 0;
    m_RequestType = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgEnvConditionsReq::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgEnvConditionsReq)
    DDX_Text(pDX, IDC_EDIT_ALTITUDE, m_Altitude);
    DDX_Text(pDX, IDC_EDIT_LATITUDE, m_Latitude);
    DDX_Text(pDX, IDC_EDIT_LONGITUDE, m_Longitude);
    DDX_Text(pDX, IDC_EDIT_REQUEST_ID, m_RequestID);
    DDX_Radio(pDX, IDC_RADIO_MARITIME, m_RequestType);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgEnvConditionsReq, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgEnvConditionsReq)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgEnvConditionsReq message handlers

void CPacketDlgEnvConditionsReq::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgEnvConditionsReq::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_ENV_CONDITIONS_REQUEST ec = {0};
    ec.packet_id = CIGI_ENV_CONDITIONS_REQUEST_OPCODE;
    ec.packet_size = sizeof(CIGI_ENV_CONDITIONS_REQUEST);
    ec.request_type = m_RequestType;
    ec.request_id = m_RequestID;
    ec.latitude = m_Latitude;
    ec.longitude = m_Longitude;
    ec.altitude = m_Altitude;

    SendImmedCigiMessage(&ec);
}
