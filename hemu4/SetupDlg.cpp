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
    m_CigiVersionIdx = -1;
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
    DDX_Control(pDX, IDC_IPADDRESS_IG, m_IPAddrCtrl);
    DDX_Check(pDX, IDC_CHECK_ASYNCHRONOUS, m_bAsyncMode);
    DDX_Text(pDX, IDC_EDIT_FRAME_RATE, m_nFrameRate);
    DDV_MinMaxUInt(pDX, m_nFrameRate, 1, 100);
    DDX_Text(pDX, IDC_EDIT_IG_RCV_PORT, m_nIGPort);
    DDX_Text(pDX, IDC_EDIT_LOCAL_RCV_PORT, m_nLocalPort);
    DDX_Check(pDX, IDC_CHECK_BIG_ENDIAN, m_BigEndian);
    DDX_Text(pDX, IDC_EDIT_DEFAULT_DB, m_DefaultDB);
    DDV_MinMaxInt(pDX, m_DefaultDB, 1, 127);
    DDX_CBIndex(pDX, IDC_COMBO_VERSION, m_CigiVersionIdx);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
    //{{AFX_MSG_MAP(CSetupDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg message handlers

void CSetupDlg::OnOK()
{
    m_IPAddrCtrl.GetAddress(m_Address);

    CDialog::OnOK();
}

void CSetupDlg::OnCancel()
{
    CDialog::OnCancel();
}

BOOL CSetupDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetCigiMinorVersion(::GetCigiMinorVersion());

    SetIPAddr(::GetIPAddr());
    SetIGPort(::GetRemotePort());
    SetLocalPort(::GetLocalPort());

    SetBigEndian(::GetBigEndian());

    SetDefaultDB(::GetDefaultDB());

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CSetupDlg::SetCigiMinorVersion(const int version)
{
    switch (version) {
    case 0:
    case 1:
        m_CigiVersionIdx = 0;
        break;

    case 2:
        m_CigiVersionIdx = 1;
        break;

    case 3:
    default:
        m_CigiVersionIdx = 2;
        break;
    }

    if (GetSafeHwnd())
        UpdateData(FALSE);
}

int CSetupDlg::GetCigiMinorVersion(void)
{
#if 0 // chas
    int retval = 2;

    if (GetSafeHwnd())
        UpdateData(TRUE);

    switch (m_CigiVersionIdx) {
    case 0:
        retval = 0;
        break;

    case 1:
        retval = 2;
        break;

    case 2:
        retval = 3;
        break;

    default:
        retval = 0;
        break;
    }
#else
    int retval = 0;

    if (GetSafeHwnd())
        UpdateData(TRUE);
#endif

    return retval;
}
