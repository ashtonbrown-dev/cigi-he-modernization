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

// PacketDlgSensorControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "CigiView.h"
#include "globals.h"
#include "PacketDlgSensorControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgSensorControl dialog

CPacketDlgSensorControl::CPacketDlgSensorControl(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgSensorControl::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgSensorControl)
    m_AutoGain = FALSE;
    m_Dropout = FALSE;
    m_SensorOn = FALSE;
    m_TrackMode = 0;
    m_ACCoupling = 0.0f;
    m_Gain = 0.0f;
    m_Level = 0.0f;
    m_Noise = 0.0f;
    m_SensorID = 0;
    m_Polarity = 0;
    m_RespType = 0;
    m_TrackType = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgSensorControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgSensorControl)
    DDX_Control(pDX, IDC_COMBOBOXEX_VIEW_ID, m_ComboViewID);
    DDX_Check(pDX, IDC_CHECK_AUTO_GAIN, m_AutoGain);
    DDX_Check(pDX, IDC_CHECK_DROPOUT, m_Dropout);
    DDX_Check(pDX, IDC_CHECK_SENSOR_ON_OFF, m_SensorOn);
    DDX_CBIndex(pDX, IDC_COMBOBOXEX_TRACK_MODE, m_TrackMode);
    DDX_Text(pDX, IDC_EDIT_AC_COUPLING, m_ACCoupling);
    DDX_Text(pDX, IDC_EDIT_GAIN, m_Gain);
    DDV_MinMaxFloat(pDX, m_Gain, 0.f, 1.f);
    DDX_Text(pDX, IDC_EDIT_LEVEL, m_Level);
    DDV_MinMaxFloat(pDX, m_Level, 0.f, 1.f);
    DDX_Text(pDX, IDC_EDIT_NOISE, m_Noise);
    DDV_MinMaxFloat(pDX, m_Noise, 0.f, 1.f);
    DDX_Text(pDX, IDC_EDIT_SENSOR_ID, m_SensorID);
    DDV_MinMaxInt(pDX, m_SensorID, 0, 255);
    DDX_Radio(pDX, IDC_RADIO_POLARITY_WHITE, m_Polarity);
    DDX_Radio(pDX, IDC_RADIO_RESPONSE_NORMAL, m_RespType);
    DDX_Radio(pDX, IDC_RADIO_TRACK_WHITE, m_TrackType);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgSensorControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgSensorControl)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgSensorControl message handlers

void CPacketDlgSensorControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgSensorControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_SENSOR_CONTROL sc = {0};
    sc.packet_id = CIGI_SENSOR_CONTROL_OPCODE;
    sc.packet_size = sizeof(CIGI_SENSOR_CONTROL);
    sc.view_id = GetViewIDFromCombo();
    sc.sensor_id = m_SensorID;
    sc.sensor_on_off = m_SensorOn;
    sc.polarity = m_Polarity;
    sc.line_dropout = m_Dropout;
    sc.auto_gain = m_AutoGain;
    sc.track_white_black = m_TrackType;
    sc.track_mode = m_TrackMode;
    sc.response_type = m_RespType;
    sc.gain = m_Gain;
    sc.level = m_Level;
    sc.ac_coupling = m_ACCoupling;
    sc.noise = m_Noise;

    SendImmedCigiMessage(&sc);
}

BOOL CPacketDlgSensorControl::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_ComboViewID.SetImageList(&g_ImgListIcons);
    PopulateViewIDCombo();
    m_ComboViewID.SetCurSel(0);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

int CPacketDlgSensorControl::GetViewIDFromCombo(void)
{
    int itemdata = m_ComboViewID.GetItemData(m_ComboViewID.GetCurSel());
    if (itemdata != -1)
        return MAKE_ID(itemdata);
    else {
        CString text;
        m_ComboViewID.GetWindowText(text);
        return atoi((LPCTSTR)text);
    }
}

void CPacketDlgSensorControl::PopulateViewIDCombo(void)
{
    char text[256] = {0};
    int cbo_idx = -1;
    int view_id = -1;
    CCigiView *view = NULL;
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_VIEW, IMG_VIEW, 0, 0, -1
                             };

    m_ComboViewID.ResetContent();

    CViewMap &map = g_DataManager.GetViewMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, view_id, view);

        strcpy(text, view->GetName());
        cboitem.pszText = text;
        cboitem.cchTextMax = strlen(text + 1);
        cboitem.lParam = MAKE_TREE_DATA(view->GetViewID(), 0, 0);

        // Add a new item to the list.
        cboitem.iItem = FindComboInsertPoint(m_ComboViewID, view->GetViewID());
        cbo_idx = m_ComboViewID.InsertItem(&cboitem);
    }
}

int CPacketDlgSensorControl::FindComboInsertPoint(CComboBoxEx &combo, const int id)
{
    int i = 0;
    COMBOBOXEXITEM cboitem = { CBEIF_LPARAM, 0, NULL, 1, 0, 0, 0, 0, -1 };

    while (combo.GetItem(&cboitem)) {
        if ((short)MAKE_ID(cboitem.lParam) > id)
            return i;

        cboitem.iItem = ++i;
    }

    return i;
}
