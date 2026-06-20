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

// PacketDlgMotionTrackerControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "PacketDlgMotionTrackerControl.h"
#include "ViewGroup.h"
#include "CigiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgMotionTrackerControl dialog

CPacketDlgMotionTrackerControl::CPacketDlgMotionTrackerControl(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgMotionTrackerControl::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketDlgMotionTrackerControl)
    m_BoresightEnable = FALSE;
    m_PitchEnable = FALSE;
    m_RollEnable = FALSE;
    m_TrackerEnable = FALSE;
    m_XEnable = FALSE;
    m_YEnable = FALSE;
    m_YawEnable = FALSE;
    m_ZEnable = FALSE;
    m_TrackerID = 0;
    m_AttachTo = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgMotionTrackerControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketDlgMotionTrackerControl)
    DDX_Control(pDX, IDC_LABEL_VIEW_OR_GROUP, m_LabelViewOrViewGroup);
    DDX_Control(pDX, IDC_COMBOBOXEX_VIEW_ID, m_ComboViewID);
    DDX_Check(pDX, IDC_CHECK_BORESIGHT_ENABLE, m_BoresightEnable);
    DDX_Check(pDX, IDC_CHECK_PITCH_ENABLE, m_PitchEnable);
    DDX_Check(pDX, IDC_CHECK_ROLL_ENABLE, m_RollEnable);
    DDX_Check(pDX, IDC_CHECK_TRACKER_ENABLE, m_TrackerEnable);
    DDX_Check(pDX, IDC_CHECK_X_ENABLE, m_XEnable);
    DDX_Check(pDX, IDC_CHECK_Y_ENABLE, m_YEnable);
    DDX_Check(pDX, IDC_CHECK_YAW_ENABLE, m_YawEnable);
    DDX_Check(pDX, IDC_CHECK_Z_ENABLE, m_ZEnable);
    DDX_Text(pDX, IDC_EDIT_TRACKER_ID, m_TrackerID);
    DDV_MinMaxInt(pDX, m_TrackerID, 0, 255);
    DDX_Radio(pDX, IDC_RADIO_ATTACH_TO_VIEW, m_AttachTo);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketDlgMotionTrackerControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgMotionTrackerControl)
    ON_BN_CLICKED(IDC_RADIO_ATTACH_TO_VIEW, OnRadioView)
    ON_BN_CLICKED(IDC_RADIO_ATTACH_TO_GROUP, OnRadioGroup)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgMotionTrackerControl message handlers

void CPacketDlgMotionTrackerControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgMotionTrackerControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_MOTION_TRACKER_CONTROL mtc = {0};
    mtc.packet_id = CIGI_MOTION_TRACKER_CONTROL_OPCODE;
    mtc.packet_size = sizeof(CIGI_MOTION_TRACKER_CONTROL);
    mtc.tracker_id = m_TrackerID;
    mtc.view_or_group_id = GetViewIDFromCombo();
    mtc.view_or_group_select = m_AttachTo;
    mtc.tracker_enable = m_TrackerEnable;
    mtc.boresight_enable = m_BoresightEnable;
    mtc.x_enable = m_XEnable;
    mtc.y_enable = m_YEnable;
    mtc.z_enable = m_ZEnable;
    mtc.yaw_enable = m_YawEnable;
    mtc.pitch_enable = m_PitchEnable;
    mtc.roll_enable = m_RollEnable;

    SendImmedCigiMessage(&mtc);
}

BOOL CPacketDlgMotionTrackerControl::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_ComboViewID.SetImageList(&g_ImgListIcons);
    PopulateViewIDCombo();
    m_ComboViewID.SetCurSel(0);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

int CPacketDlgMotionTrackerControl::GetViewIDFromCombo(void)
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

void CPacketDlgMotionTrackerControl::PopulateViewIDCombo(void)
{
    char text[256] = {0};
    int cbo_idx = -1;
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_VIEW, IMG_VIEW, 0, 0, -1
                             };

    m_ComboViewID.ResetContent();

    UpdateData(TRUE);
    if (m_AttachTo == 0) {
        int view_id = -1;
        CCigiView *view = NULL;

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
    } else {
        int i;
        int group_id = -1;
        CViewGroup *group = NULL;
        CViewGroup **array = g_DataManager.GetViewGroupArray();

        for (i = 1; i < VIEWGROUP_ARRAY_SIZE; i++) {
            group = array[i];
            if (group) {
                group_id = group->GetGroupID();

                strcpy(text, group->GetName());
                cboitem.pszText = text;
                cboitem.cchTextMax = strlen(text + 1);
                cboitem.iImage = IMG_VIEWGROUP;
                cboitem.iSelectedImage = cboitem.iImage;
                cboitem.lParam = MAKE_TREE_DATA(group_id, 0, 0);

                // Add a new item to the list.
                cboitem.iItem = FindComboInsertPoint(m_ComboViewID, group_id);
                cbo_idx = m_ComboViewID.InsertItem(&cboitem);
            }
        }
    }
}

int CPacketDlgMotionTrackerControl::FindComboInsertPoint(CComboBoxEx &combo, const int id)
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

void CPacketDlgMotionTrackerControl::OnRadioView()
{
    m_LabelViewOrViewGroup.SetWindowText("View ID:");
    PopulateViewIDCombo();
    m_ComboViewID.SetCurSel(0);
}

void CPacketDlgMotionTrackerControl::OnRadioGroup()
{
    m_LabelViewOrViewGroup.SetWindowText("Group ID:");
    PopulateViewIDCombo();
    m_ComboViewID.SetCurSel(0);
}
