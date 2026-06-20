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

// PacketDlgERMDef.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "PacketDlgERMDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgERMDef dialog

CPacketDlgERMDef::CPacketDlgERMDef(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgERMDef::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgERMDef)
    m_Enable = FALSE;
    m_Flattening = 0.0;
    m_Radius = 0.0;
    //}}AFX_DATA_INIT
}

void CPacketDlgERMDef::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgERMDef)
    DDX_Check(pDX, IDC_CHECK_ENABLE, m_Enable);
    DDX_Text(pDX, IDC_EDIT_FLATTENING, m_Flattening);
    DDX_Text(pDX, IDC_EDIT_RADIUS, m_Radius);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgERMDef, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgERMDef)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgERMDef message handlers

void CPacketDlgERMDef::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_ERM_DEF ed = {0};
    ed.packet_id = CIGI_ERM_DEF_OPCODE;
    ed.packet_size = sizeof(CIGI_ERM_DEF);
    ed.erm_enable = m_Enable;
    ed.equatorial_radius = m_Radius;
    ed.flattening = m_Flattening;

    SendImmedCigiMessage(&ed);
}

void CPacketDlgERMDef::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}
