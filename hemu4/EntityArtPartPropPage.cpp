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
 *  FILENAME:   EntityArtPartPropPage.cpp
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
#include "ArtPart.h"
#include "cigi4types.h"
#include "EntityArtPartPropPage.h"
#include "globals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityArtPartPropPage dialog

CEntityArtPartPropPage::CEntityArtPartPropPage(CWnd *pParent /*=NULL*/)
    : CDialog(CEntityArtPartPropPage::IDD, pParent)
{
    //{{AFX_DATA_INIT(CEntityArtPartPropPage)
    //}}AFX_DATA_INIT
}

void CEntityArtPartPropPage::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEntityArtPartPropPage)
    DDX_Control(pDX, IDC_SPIN_ARTPART_Z, m_SpinZ);
    DDX_Control(pDX, IDC_SPIN_ARTPART_Y, m_SpinY);
    DDX_Control(pDX, IDC_SPIN_ARTPART_X, m_SpinX);
    DDX_Control(pDX, IDC_SPIN_ARTPART_YAW, m_SpinYaw);
    DDX_Control(pDX, IDC_SPIN_ARTPART_ROLL, m_SpinRoll);
    DDX_Control(pDX, IDC_SPIN_ARTPART_PITCH, m_SpinPitch);
    DDX_Control(pDX, IDC_LIST_ENTITY_ARTPARTS, m_ListBox);
    DDX_Control(pDX, IDC_EDIT_ARTPART_Z, m_EditZ);
    DDX_Control(pDX, IDC_EDIT_ARTPART_Y, m_EditY);
    DDX_Control(pDX, IDC_EDIT_ARTPART_X, m_EditX);
    DDX_Control(pDX, IDC_EDIT_ARTPART_YAW, m_EditYaw);
    DDX_Control(pDX, IDC_EDIT_ARTPART_ROLL, m_EditRoll);
    DDX_Control(pDX, IDC_EDIT_ARTPART_PITCH, m_EditPitch);
    DDX_Control(pDX, IDC_CHECK_ENTITY_ARTPART_AUTOAPPLY, m_CheckAutoApply);
    DDX_Control(pDX, IDC_CHECK_ARTPART_Z, m_CheckZ);
    DDX_Control(pDX, IDC_CHECK_ARTPART_Y, m_CheckY);
    DDX_Control(pDX, IDC_CHECK_ARTPART_X, m_CheckX);
    DDX_Control(pDX, IDC_CHECK_ARTPART_YAW, m_CheckYaw);
    DDX_Control(pDX, IDC_CHECK_ARTPART_ROLL, m_CheckRoll);
    DDX_Control(pDX, IDC_CHECK_ARTPART_PITCH, m_CheckPitch);
    DDX_Control(pDX, IDC_CHECK_ARTPART_ENABLE, m_CheckEnable);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEntityArtPartPropPage, CDialog)
    //{{AFX_MSG_MAP(CEntityArtPartPropPage)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ARTPART_PITCH, OnDeltaposPitch)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ARTPART_ROLL, OnDeltaposRoll)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ARTPART_YAW, OnDeltaposYaw)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ARTPART_X, OnDeltaposX)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ARTPART_Y, OnDeltaposY)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ARTPART_Z, OnDeltaposZ)
    ON_BN_CLICKED(IDC_BUTTON_APPLY_ARTPART, OnApply)
    ON_BN_CLICKED(IDC_CHECK_ARTPART_ENABLE, OnCheckArtpartEnable)
    ON_BN_CLICKED(IDC_CHECK_ARTPART_PITCH, OnCheckPitchEnable)
    ON_BN_CLICKED(IDC_CHECK_ARTPART_ROLL, OnCheckRollEnable)
    ON_BN_CLICKED(IDC_CHECK_ARTPART_YAW, OnCheckYawEnable)
    ON_BN_CLICKED(IDC_CHECK_ARTPART_X, OnCheckXEnable)
    ON_BN_CLICKED(IDC_CHECK_ARTPART_Y, OnCheckYEnable)
    ON_BN_CLICKED(IDC_CHECK_ARTPART_Z, OnCheckZEnable)
    ON_LBN_SELCHANGE(IDC_LIST_ENTITY_ARTPARTS, OnSelchangeArtpart)
    //}}AFX_MSG_MAP

    ON_MESSAGE(WM_USEREDIT, OnUserEditControl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityArtPartPropPage message handlers

void CEntityArtPartPropPage::OnOK()
{
}

void CEntityArtPartPropPage::OnCancel()
{
}

void CEntityArtPartPropPage::OnApply()
{
    CArtPart *artpart = GetSelArtPart();
    if (!artpart)
        return;

    char tempstr[64] = {'\0'};
    float tempf;

    artpart->SetEnableFlag(m_CheckEnable.GetCheck() ? TRUE : FALSE);

    m_EditX.GetWindowText(tempstr, 64);
    sscanf(tempstr, "%f", &tempf);
    artpart->SetXOffset(tempf, m_CheckX.GetCheck() ? TRUE : FALSE);

    m_EditY.GetWindowText(tempstr, 64);
    sscanf(tempstr, "%f", &tempf);
    artpart->SetYOffset(tempf, m_CheckY.GetCheck() ? TRUE : FALSE);

    m_EditZ.GetWindowText(tempstr, 64);
    sscanf(tempstr, "%f", &tempf);
    artpart->SetZOffset(tempf, m_CheckZ.GetCheck() ? TRUE : FALSE);

    m_EditRoll.GetWindowText(tempstr, 64);
    sscanf(tempstr, "%f", &tempf);
    artpart->SetRoll(tempf, m_CheckRoll.GetCheck() ? TRUE : FALSE);

    m_EditPitch.GetWindowText(tempstr, 64);
    sscanf(tempstr, "%f", &tempf);
    artpart->SetPitch(tempf, m_CheckPitch.GetCheck() ? TRUE : FALSE);

    m_EditYaw.GetWindowText(tempstr, 64);
    sscanf(tempstr, "%f", &tempf);
    artpart->SetYaw(tempf, m_CheckYaw.GetCheck() ? TRUE : FALSE);

    // Send the packet.
    SendPacketToDriver(artpart);
}

void CEntityArtPartPropPage::SendPacketToDriver(CArtPart *artpart)
{
    int size = 0;
    unsigned char buffer[sizeof(CIGI_ARTPART_CONTROL)] = {0};

    // Create either an Articulated Part or Short Articulated Part packet.
    artpart->CreateCigiPacket(buffer);

    // SendImmedCigiMessage() will figure out the packet size.
    SendImmedCigiMessage(buffer);
}


LRESULT CEntityArtPartPropPage::OnUserEditControl(WPARAM wParam, LPARAM lParam)
{
    if (m_CheckAutoApply.GetCheck())
        OnApply();

    return 0;
}

BOOL CEntityArtPartPropPage::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_CheckAutoApply.SetCheck(TRUE);

    UDACCEL accel = {0, 1};
    m_SpinZ.SetAccel(1, &accel);
    m_SpinY.SetAccel(1, &accel);
    m_SpinX.SetAccel(1, &accel);
    m_SpinRoll.SetAccel(1, &accel);
    m_SpinPitch.SetAccel(1, &accel);
    m_SpinYaw.SetAccel(1, &accel);

    m_SpinX.SetRange(-10000, 10000);
    m_SpinX.SetPos(0);
    m_SpinY.SetRange(-10000, 10000);
    m_SpinY.SetPos(0);
    m_SpinZ.SetRange(-10000, 10000);
    m_SpinZ.SetPos(0);
    m_SpinYaw.SetRange(-3600, 3600);
    m_SpinYaw.SetPos(0);
    m_SpinPitch.SetRange(-900, 900);
    m_SpinPitch.SetPos(0);
    m_SpinRoll.SetRange(-1800, 1800);
    m_SpinRoll.SetPos(0);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CEntityArtPartPropPage::OnDeltaposPitch(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    NM_UPDOWN *pNMUpDown = (NM_UPDOWN *)pNMHDR;
    short pos = LOWORD(m_SpinPitch.GetPos()) + pNMUpDown->iDelta;
    int lower = 0, upper = 0;
    m_SpinPitch.GetRange(lower, upper);
    if ((pos > upper) || (pos < lower))
        return;

    float pitch = (pos / 10.0f);
    CString temp;
    temp.Format("%.1f", pitch);
    m_EditPitch.SetWindowText((LPCTSTR)temp);

    m_CheckPitch.SetCheck(1);

    if (m_CheckAutoApply.GetCheck()) {
        CArtPart *artpart = GetSelArtPart();
        if (!artpart)
            return;

        artpart->SetPitch(pitch);

        SendPacketToDriver(artpart);
    }
}

void CEntityArtPartPropPage::OnDeltaposRoll(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    NM_UPDOWN *pNMUpDown = (NM_UPDOWN *)pNMHDR;
    short pos = LOWORD(m_SpinRoll.GetPos()) + pNMUpDown->iDelta;
    int lower = 0, upper = 0;
    m_SpinRoll.GetRange(lower, upper);
    if ((pos > upper) || (pos < lower))
        return;

    float roll = (pos / 10.0f);
    CString temp;
    temp.Format("%.1f", roll);
    m_EditRoll.SetWindowText((LPCTSTR)temp);

    m_CheckRoll.SetCheck(1);

    if (m_CheckAutoApply.GetCheck()) {
        CArtPart *artpart = GetSelArtPart();
        if (!artpart)
            return;

        artpart->SetRoll(roll);

        SendPacketToDriver(artpart);
    }
}

void CEntityArtPartPropPage::OnDeltaposYaw(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    NM_UPDOWN *pNMUpDown = (NM_UPDOWN *)pNMHDR;
    short pos = LOWORD(m_SpinYaw.GetPos()) + pNMUpDown->iDelta;
    int lower = 0, upper = 0;
    m_SpinYaw.GetRange(lower, upper);
    if ((pos > upper) || (pos < lower))
        return;

    float yaw = (pos / 10.0f);
    CString temp;
    temp.Format("%.1f", yaw);
    m_EditYaw.SetWindowText((LPCTSTR)temp);

    m_CheckYaw.SetCheck(1);

    if (m_CheckAutoApply.GetCheck()) {
        CArtPart *artpart = GetSelArtPart();
        if (!artpart)
            return;

        artpart->SetYaw(yaw);

        SendPacketToDriver(artpart);
    }
}

void CEntityArtPartPropPage::OnDeltaposX(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    NM_UPDOWN *pNMUpDown = (NM_UPDOWN *)pNMHDR;
    short pos = LOWORD(m_SpinX.GetPos()) + pNMUpDown->iDelta;
    int lower = 0, upper = 0;
    m_SpinX.GetRange(lower, upper);
    if ((pos > upper) || (pos < lower))
        return;

    float x = (pos / 10.0f);
    CString temp;
    temp.Format("%.1f", x);
    m_EditX.SetWindowText((LPCTSTR)temp);

    m_CheckX.SetCheck(1);

    if (m_CheckAutoApply.GetCheck()) {
        CArtPart *artpart = GetSelArtPart();
        if (!artpart)
            return;

        artpart->SetXOffset(x);

        SendPacketToDriver(artpart);
    }
}

void CEntityArtPartPropPage::OnDeltaposY(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    NM_UPDOWN *pNMUpDown = (NM_UPDOWN *)pNMHDR;
    short pos = LOWORD(m_SpinY.GetPos()) + pNMUpDown->iDelta;
    int lower = 0, upper = 0;
    m_SpinY.GetRange(lower, upper);
    if ((pos > upper) || (pos < lower))
        return;

    float y = (pos / 10.0f);
    CString temp;
    temp.Format("%.1f", y);
    m_EditY.SetWindowText((LPCTSTR)temp);

    m_CheckY.SetCheck(1);

    if (m_CheckAutoApply.GetCheck()) {
        CArtPart *artpart = GetSelArtPart();
        if (!artpart)
            return;

        artpart->SetYOffset(y);

        SendPacketToDriver(artpart);
    }
}

void CEntityArtPartPropPage::OnDeltaposZ(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;
    NM_UPDOWN *pNMUpDown = (NM_UPDOWN *)pNMHDR;
    short pos = LOWORD(m_SpinZ.GetPos()) + pNMUpDown->iDelta;
    int lower = 0, upper = 0;
    m_SpinZ.GetRange(lower, upper);
    if ((pos > upper) || (pos < lower))
        return;

    float z = (pos / 10.0f);
    CString temp;
    temp.Format("%.1f", z);
    m_EditZ.SetWindowText((LPCTSTR)temp);

    m_CheckZ.SetCheck(1);

    if (m_CheckAutoApply.GetCheck()) {
        CArtPart *artpart = GetSelArtPart();
        if (!artpart)
            return;

        artpart->SetZOffset(z);

        SendPacketToDriver(artpart);
    }
}

void CEntityArtPartPropPage::RefreshView(void)
{
    CArtPart *artpart = GetSelArtPart();

    if (!artpart) {
        // If for some reason we have an invalid item in the list, just
        // get rid of it.  This should never happen, though.
        m_ListBox.DeleteString(m_ListBox.GetCurSel());
        EnableFields(FALSE);
        return;
    }

    EnableFields(TRUE);
    PopulateFields(artpart);
}

void CEntityArtPartPropPage::EnableFields(BOOL enable)
{
    if (enable) {
        m_EditX.EnableWindow(TRUE);
        m_EditY.EnableWindow(TRUE);
        m_EditZ.EnableWindow(TRUE);
        m_EditRoll.EnableWindow(TRUE);
        m_EditPitch.EnableWindow(TRUE);
        m_EditYaw.EnableWindow(TRUE);
        m_SpinX.EnableWindow(TRUE);
        m_SpinY.EnableWindow(TRUE);
        m_SpinZ.EnableWindow(TRUE);
        m_SpinRoll.EnableWindow(TRUE);
        m_SpinPitch.EnableWindow(TRUE);
        m_SpinYaw.EnableWindow(TRUE);
        m_CheckEnable.EnableWindow(TRUE);
        m_CheckX.EnableWindow(TRUE);
        m_CheckY.EnableWindow(TRUE);
        m_CheckZ.EnableWindow(TRUE);
        m_CheckRoll.EnableWindow(TRUE);
        m_CheckPitch.EnableWindow(TRUE);
        m_CheckYaw.EnableWindow(TRUE);
    } else {
        m_EditX.SetWindowText("");
        m_EditY.SetWindowText("");
        m_EditZ.SetWindowText("");
        m_EditRoll.SetWindowText("");
        m_EditPitch.SetWindowText("");
        m_EditYaw.SetWindowText("");
        m_CheckEnable.SetCheck(FALSE);
        m_CheckX.SetCheck(FALSE);
        m_CheckY.SetCheck(FALSE);
        m_CheckZ.SetCheck(FALSE);
        m_CheckRoll.SetCheck(FALSE);
        m_CheckPitch.SetCheck(FALSE);
        m_CheckYaw.SetCheck(FALSE);

        m_EditX.EnableWindow(FALSE);
        m_EditY.EnableWindow(FALSE);
        m_EditZ.EnableWindow(FALSE);
        m_EditRoll.EnableWindow(FALSE);
        m_EditPitch.EnableWindow(FALSE);
        m_EditYaw.EnableWindow(FALSE);
        m_SpinX.EnableWindow(FALSE);
        m_SpinY.EnableWindow(FALSE);
        m_SpinZ.EnableWindow(FALSE);
        m_SpinRoll.EnableWindow(FALSE);
        m_SpinPitch.EnableWindow(FALSE);
        m_SpinYaw.EnableWindow(FALSE);
        m_CheckEnable.EnableWindow(FALSE);
        m_CheckX.EnableWindow(FALSE);
        m_CheckY.EnableWindow(FALSE);
        m_CheckZ.EnableWindow(FALSE);
        m_CheckRoll.EnableWindow(FALSE);
        m_CheckPitch.EnableWindow(FALSE);
        m_CheckYaw.EnableWindow(FALSE);
    }
}

void CEntityArtPartPropPage::PopulateFields(CArtPart *artpart)
{
    CString str;

    m_CheckEnable.SetCheck(artpart->GetEnableFlag());

    m_CheckX.SetCheck(artpart->GetXEnable());
    m_CheckY.SetCheck(artpart->GetYEnable());
    m_CheckZ.SetCheck(artpart->GetZEnable());
    m_CheckRoll.SetCheck(artpart->GetRollEnable());
    m_CheckPitch.SetCheck(artpart->GetPitchEnable());
    m_CheckYaw.SetCheck(artpart->GetYawEnable());

    m_SpinX.SetPos(int(artpart->GetXOffset() * 10.0001));
    m_SpinY.SetPos(int(artpart->GetYOffset() * 10.0001));
    m_SpinZ.SetPos(int(artpart->GetZOffset() * 10.0001));
    m_SpinRoll.SetPos(int(artpart->GetRoll() * 10.0001));
    m_SpinPitch.SetPos(int(artpart->GetPitch() * 10.0001));
    m_SpinYaw.SetPos(int(artpart->GetYaw() * 10.0001));

    str.Format("%.1f", artpart->GetXOffset());
    m_EditX.SetWindowText((LPCTSTR)str);
    str.Format("%.1f", artpart->GetYOffset());
    m_EditY.SetWindowText((LPCTSTR)str);
    str.Format("%.1f", artpart->GetZOffset());
    m_EditZ.SetWindowText((LPCTSTR)str);
    str.Format("%.1f", artpart->GetRoll());
    m_EditRoll.SetWindowText((LPCTSTR)str);
    str.Format("%.1f", artpart->GetPitch());
    m_EditPitch.SetWindowText((LPCTSTR)str);
    str.Format("%.1f", artpart->GetYaw());
    m_EditYaw.SetWindowText((LPCTSTR)str);
}

BOOL CEntityArtPartPropPage::AddArtPart(CArtPart *artpart)
{
    if (!artpart)
        return FALSE;

    int index = m_ListBox.AddString((LPCTSTR)(artpart->GetName()));
    m_ListBox.SetItemData(index, artpart->GetID());

    m_ArtPartList.InsertTail(artpart);

    return TRUE;
}

BOOL CEntityArtPartPropPage::RemoveArtPart(CArtPart *artpart)
{
    if (!artpart)
        return FALSE;

    // First remove the item from the list box.
    int id = artpart->GetID();
    int item_id = -1;
    int idx = 0;
    do {
        item_id = m_ListBox.GetItemData(idx);
        idx++;
    } while (item_id != id);
    m_ListBox.DeleteString(idx);

    // Now remove the item from the linked list.
    unsigned long handle = 0;
    CArtPart **ap = m_ArtPartList.GetHead(&handle);
    while (*ap) {
        if ((*ap)->GetID() == id) {
            m_ArtPartList.Remove(&handle);
            return TRUE;
        }

        ap = m_ArtPartList.GetNext(&handle);
    }

    return FALSE;
}

CArtPart *CEntityArtPartPropPage::FindArtPart(const int id)
{
    unsigned long handle = 0;
    CArtPart **artpart = m_ArtPartList.GetHead(&handle);
    while (*artpart) {
        if ((*artpart)->GetID() == id)
            return *artpart;

        artpart = m_ArtPartList.GetNext(&handle);
    }

    return NULL;
}

CArtPart *CEntityArtPartPropPage::GetSelArtPart(void)
{
    int idx = m_ListBox.GetCurSel();
    if (idx == -1)
        return NULL;

    int part_id = m_ListBox.GetItemData(idx);

    return FindArtPart(part_id);
}

void CEntityArtPartPropPage::ClearArtParts(void)
{
    m_ArtPartList.RemoveAll();
    m_ListBox.ResetContent();
    RefreshView();
}

void CEntityArtPartPropPage::OnCheckArtpartEnable()
{
    CArtPart *artpart = GetSelArtPart();
    if (!artpart)
        return;

    if (m_CheckAutoApply.GetCheck()) {
        artpart->SetEnableFlag(m_CheckEnable.GetCheck() ? TRUE : FALSE);
        SendPacketToDriver(artpart);
    }
}

void CEntityArtPartPropPage::OnCheckPitchEnable()
{
    CArtPart *artpart = GetSelArtPart();
    if (!artpart)
        return;

    if (m_CheckAutoApply.GetCheck())
        artpart->SetPitchEnable(m_CheckPitch.GetCheck() ? TRUE : FALSE);
}

void CEntityArtPartPropPage::OnCheckRollEnable()
{
    CArtPart *artpart = GetSelArtPart();
    if (!artpart)
        return;

    if (m_CheckAutoApply.GetCheck())
        artpart->SetRollEnable(m_CheckRoll.GetCheck() ? TRUE : FALSE);
}

void CEntityArtPartPropPage::OnCheckYawEnable()
{
    CArtPart *artpart = GetSelArtPart();
    if (!artpart)
        return;

    if (m_CheckAutoApply.GetCheck())
        artpart->SetYawEnable(m_CheckYaw.GetCheck() ? TRUE : FALSE);
}

void CEntityArtPartPropPage::OnCheckXEnable()
{
    CArtPart *artpart = GetSelArtPart();
    if (!artpart)
        return;

    if (m_CheckAutoApply.GetCheck())
        artpart->SetXEnable(m_CheckX.GetCheck() ? TRUE : FALSE);
}

void CEntityArtPartPropPage::OnCheckYEnable()
{
    CArtPart *artpart = GetSelArtPart();
    if (!artpart)
        return;

    if (m_CheckAutoApply.GetCheck())
        artpart->SetYEnable(m_CheckY.GetCheck() ? TRUE : FALSE);
}

void CEntityArtPartPropPage::OnCheckZEnable()
{
    CArtPart *artpart = GetSelArtPart();
    if (!artpart)
        return;

    if (m_CheckAutoApply.GetCheck())
        artpart->SetZEnable(m_CheckZ.GetCheck() ? TRUE : FALSE);
}

void CEntityArtPartPropPage::OnSelchangeArtpart()
{
    CArtPart *artpart = GetSelArtPart();

    if (artpart) {
        EnableFields(TRUE);
        PopulateFields(artpart);
    } else {
        EnableFields(FALSE);
    }
}
