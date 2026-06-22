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

// SetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "SetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
CString GetCigiVersionDisplayName(const CigiProtocolVersion &version)
{
    if (version.IsCigi3() && version.GetMinorVersion() <= 1)
        return _T("3.0/3.1");

    CString displayName;
    displayName.Format(_T("%d.%d"), version.GetMajorVersion(),
                       version.GetMinorVersion());
    return displayName;
}
}

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog

CSetupDlg::CSetupDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CSetupDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSetupDlg)
    m_bAsyncMode = FALSE;
    m_nFrameRate = 60;
    m_nIGPort = 0;
    m_nLocalPort = 0;
    m_BigEndian = FALSE;
    m_DefaultDB = 1;
    //}}AFX_DATA_INIT
}

void CSetupDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSetupDlg)
    DDX_Control(pDX, IDC_LABEL_HZ, m_LabelHz);
    DDX_Control(pDX, IDC_LABEL_FRAME_RATE, m_LabelFrameRate);
    DDX_Control(pDX, IDC_EDIT_FRAME_RATE, m_EditFrameRate);
    DDX_Control(pDX, IDC_CHECK_ASYNCHRONOUS, m_CheckAsyncMode);
    DDX_Control(pDX, IDC_COMBO_VERSION, m_ComboVersion);
    DDX_Control(pDX, IDC_IPADDRESS_IG, m_IPAddrCtrl);
    DDX_Check(pDX, IDC_CHECK_ASYNCHRONOUS, m_bAsyncMode);
    DDX_Text(pDX, IDC_EDIT_FRAME_RATE, m_nFrameRate);
    DDV_MinMaxUInt(pDX, m_nFrameRate, 1, 100);
    DDX_Text(pDX, IDC_EDIT_IG_RCV_PORT, m_nIGPort);
    DDX_Text(pDX, IDC_EDIT_LOCAL_RCV_PORT, m_nLocalPort);
    DDX_Check(pDX, IDC_CHECK_BIG_ENDIAN, m_BigEndian);
    DDX_Text(pDX, IDC_EDIT_DEFAULT_DB, m_DefaultDB);
    DDV_MinMaxInt(pDX, m_DefaultDB, 1, 127);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
    //{{AFX_MSG_MAP(CSetupDlg)
    ON_CBN_SELCHANGE(IDC_COMBO_VERSION, OnCigiVersionChanged)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg message handlers

void CSetupDlg::OnOK()
{
    m_IPAddrCtrl.GetAddress(m_Address);

    const int selection = m_ComboVersion.GetCurSel();
    if (selection != CB_ERR) {
        const CigiProtocolVersionId id =
            (CigiProtocolVersionId)m_ComboVersion.GetItemData(selection);
        m_CigiProtocolVersion = CigiProtocolVersion::FromId(id);
    }

    if (!m_CigiProtocolVersion.IsPacketIoImplemented()) {
        AfxMessageBox(
            _T("This version selection will be saved. Packet I/O will safely remain CIGI 4.0 until the CIGI 3 protocol adapter is implemented."),
            MB_OK | MB_ICONINFORMATION);
    }

    CDialog::OnOK();
}

void CSetupDlg::OnCancel()
{
    CDialog::OnCancel();
}

BOOL CSetupDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    PopulateCigiVersions();
    SetCigiProtocolVersion(::GetCigiProtocolVersion());

    SetIPAddr(::GetIPAddr());
    SetIGPort(::GetRemotePort());
    SetLocalPort(::GetLocalPort());

    SetBigEndian(::GetBigEndian());

    SetDefaultDB(::GetDefaultDB());

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CSetupDlg::PopulateCigiVersions(void)
{
    m_ComboVersion.ResetContent();

    for (int i = 0; i < CigiProtocolVersionCatalog::GetVersionCount(); ++i) {
        const CigiProtocolVersion version =
            CigiProtocolVersionCatalog::GetVersion(i);
        const int item = m_ComboVersion.AddString(GetCigiVersionDisplayName(version));
        m_ComboVersion.SetItemData(item, version.GetId());
    }
}

void CSetupDlg::SetCigiProtocolVersion(const CigiProtocolVersion &version)
{
    m_CigiProtocolVersion = version;
    int selectedItem = CB_ERR;

    for (int i = 0; i < m_ComboVersion.GetCount(); ++i) {
        const CigiProtocolVersionId id =
            (CigiProtocolVersionId)m_ComboVersion.GetItemData(i);
        CigiProtocolVersion itemVersion = CigiProtocolVersion::FromId(id);
        if (version.IsSameSelection(itemVersion)) {
            selectedItem = i;
            break;
        }
    }

    // A persisted future CIGI 4 minor remains selectable even before it is
    // promoted into the standard catalog.
    if (selectedItem == CB_ERR && version.IsCigi4()) {
        selectedItem = m_ComboVersion.AddString(GetCigiVersionDisplayName(version));
        m_ComboVersion.SetItemData(selectedItem, version.GetId());
    }

    m_ComboVersion.SetCurSel(selectedItem);
}

CigiProtocolVersion CSetupDlg::GetCigiProtocolVersion(void) const
{
    return m_CigiProtocolVersion;
}

void CSetupDlg::OnCigiVersionChanged()
{
    const int selection = m_ComboVersion.GetCurSel();
    if (selection != CB_ERR) {
        const CigiProtocolVersionId id =
            (CigiProtocolVersionId)m_ComboVersion.GetItemData(selection);
        m_CigiProtocolVersion = CigiProtocolVersion::FromId(id);
    }

}
