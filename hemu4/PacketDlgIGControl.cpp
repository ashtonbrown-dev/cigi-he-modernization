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

// PacketDlgIGControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "hemu4.h"
#include "hemumsg.h"
#include "TerrainDB.h"
#include "PacketDlgIGControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgIGControl dialog

CPacketDlgIGControl::CPacketDlgIGControl(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgIGControl::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgIGControl)
// chas   m_Version = _T("3");
// chas   m_Byteswap = _T("8000h");
    m_Version = _T("4");
    m_Byteswap = _T(" N/A ");
    m_IGMode = 1;
    m_Timestamp = 0;
	m_SmoothingEnable = TRUE;
    m_EntityTypeSubEnable = TRUE;
    m_TimestampValid = FALSE;
    //}}AFX_DATA_INIT
}

void CPacketDlgIGControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgIGControl)
    DDX_Control(pDX, IDC_COMBO_DATABASE, m_ComboDatabase);
    DDX_Text(pDX, IDC_EDIT_VERSION, m_Version);
    DDX_Text(pDX, IDC_EDIT_BYTESWAP, m_Byteswap);
    DDX_Radio(pDX, IDC_RADIO_MODE_RESET, m_IGMode);
    DDX_Text(pDX, IDC_EDIT_TIMESTAMP, m_Timestamp);
    DDX_Check(pDX, IDC_CHECK_TIMESTAMP_VALID, m_TimestampValid);
    DDX_Check(pDX, IDC_CHECK_SMOOTHING_ENABLE, m_SmoothingEnable);
    DDX_Check(pDX, IDC_CHECK_ENTITY_SUBSTITUTION_ENABLE, m_EntityTypeSubEnable);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgIGControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgIGControl)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgIGControl message handlers

BOOL CPacketDlgIGControl::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Populate the database combo box.
    m_ComboDatabase.AddString("No load requested");

    int cbo_idx;
    CTerrainDB *db = NULL;
    for (int i = 1; i < 128; i++) { // 1 - 127 are valid IDs
        db = g_DataManager.GetDatabase(i);
        if (db) {
            // Add a new item to the list.
            cbo_idx = FindComboInsertPoint(i);
            cbo_idx = m_ComboDatabase.InsertString(cbo_idx, (LPCTSTR)db->GetName());
            m_ComboDatabase.SetItemData(cbo_idx, db->GetID());
        }
    }

    // Don't request new database load by default.
    m_ComboDatabase.SetCurSel(0);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

int CPacketDlgIGControl::FindComboInsertPoint(const int item_id)
{
    int i = 0;
    int count = m_ComboDatabase.GetCount();

    for (int i = 0; i < count; i++)
        if (m_ComboDatabase.GetItemData(i) > (unsigned int)item_id)
            return i;

    return i;
}

void CPacketDlgIGControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgIGControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_IG_CONTROL igc = {0};
    igc.packet_id = CIGI_IG_CONTROL_OPCODE;
    igc.cigi_version = 4;
    igc.db_number = (char)(m_ComboDatabase.GetItemData(m_ComboDatabase.GetCurSel()));
    igc.ig_mode = m_IGMode;
    igc.timestamp_valid = m_TimestampValid;
	igc.EntityTypeSubEn = m_EntityTypeSubEnable;
	igc.extrap_interpol_enable = m_SmoothingEnable;
    igc.host_frame_num = 0;            // will be overridden in driver
    igc.timestamp = m_Timestamp;
    igc.last_ig_frame_num = 0;   // will be overridden in driver
    igc.minor_version = GetCigiMinorVersion();

    if (GetCigiMinorVersion() == 0 ) {
        igc.packet_size = sizeof(CIGI_IG_CONTROL);
    } 
    SendImmedCigiMessage(&igc);

	MESSAGE_SET_IG_CONTROL msg;
    msg.smoothing_enabled = m_SmoothingEnable;
	msg.substitution_enabled = m_EntityTypeSubEnable;
    PostDriverMsg(msg);

    // Set the IG Mode to match what we just sent.
    switch (m_IGMode) {
    case 0:
        g_bFreezeFlag = TRUE;
        g_bResetIGFlag = TRUE;
        g_bDebugIGFlag = FALSE;
        break;

    case 1:
        g_bFreezeFlag = FALSE;
        g_bResetIGFlag = FALSE;
        g_bDebugIGFlag = FALSE;
        break;

    case 2:
        g_bFreezeFlag = FALSE;
        g_bResetIGFlag = FALSE;
        g_bDebugIGFlag = TRUE;
        break;

    default:
        break;
    }

    // Set the database to match what we just sent.  This will force the
    // Terrain properties view to be updated, too.
    if (igc.db_number != 0) {
        g_DataManager.SetSelectedDatabase(igc.db_number);
        theApp.GetMainFrame().UpdateDatabaseCombo();
    }
}
