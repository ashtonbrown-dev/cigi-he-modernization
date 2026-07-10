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
const int MaxSetupProfileCount = 64;

CString GetCigiVersionDisplayName(const CigiProtocolVersion &version)
{
    if (version.IsCigi3() && version.GetMinorVersion() <= 1)
        return _T("3.0/3.1");

    CString displayName;
    displayName.Format(_T("%d.%d"), version.GetMajorVersion(),
                       version.GetMinorVersion());
    return displayName;
}

CString GetExecutableDirectory()
{
    TCHAR modulePath[MAX_PATH] = {0};
    GetModuleFileName(NULL, modulePath, MAX_PATH);

    CString executablePath(modulePath);
    const int slash = executablePath.ReverseFind(_T('\\'));
    if (slash < 0)
        return _T(".");

    return executablePath.Left(slash);
}

CString GetSetupProfileConfigDirectory()
{
    return GetExecutableDirectory() + _T("\\config");
}

CString GetSetupProfileDefaultFilePath()
{
    return GetSetupProfileConfigDirectory() + _T("\\setup_profiles.ini");
}

CString GetSetupProfileUserFilePath()
{
    return GetSetupProfileConfigDirectory() + _T("\\setup_profiles.user.ini");
}

BOOL SetupProfileFileExists(const CString &filePath)
{
    const DWORD attributes = GetFileAttributes(filePath);
    return attributes != INVALID_FILE_ATTRIBUTES
           && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

CString GetSetupProfileReadFilePath()
{
    const CString userFilePath = GetSetupProfileUserFilePath();
    if (SetupProfileFileExists(userFilePath))
        return userFilePath;

    return GetSetupProfileDefaultFilePath();
}

BOOL EnsureSetupProfileConfigDirectory()
{
    const CString configDirectory = GetSetupProfileConfigDirectory();
    const DWORD attributes = GetFileAttributes(configDirectory);

    if (attributes != INVALID_FILE_ATTRIBUTES)
        return (attributes & FILE_ATTRIBUTE_DIRECTORY) ? TRUE : FALSE;

    return CreateDirectory(configDirectory, NULL) ? TRUE : FALSE;
}

CString GetSetupProfileSectionName(const int index)
{
    CString sectionName;
    sectionName.Format(_T("SetupProfile%d"), index);
    return sectionName;
}

CString ReadSetupProfileString(const CString &sectionName, LPCTSTR keyName,
                               LPCTSTR defaultValue, const CString &filePath)
{
    TCHAR value[256] = {0};
    GetPrivateProfileString(sectionName, keyName, defaultValue, value,
                            sizeof(value) / sizeof(value[0]), filePath);
    return CString(value);
}

int ReadSetupProfileInt(const CString &sectionName, LPCTSTR keyName,
                        const int defaultValue, const CString &filePath)
{
    return GetPrivateProfileInt(sectionName, keyName, defaultValue, filePath);
}

BOOL WriteSetupProfileString(const CString &sectionName, LPCTSTR keyName,
                             const CString &value, const CString &filePath)
{
    return WritePrivateProfileString(sectionName, keyName, value, filePath)
           ? TRUE : FALSE;
}

BOOL WriteSetupProfileInt(const CString &sectionName, LPCTSTR keyName,
                          const int value, const CString &filePath)
{
    CString text;
    text.Format(_T("%d"), value);
    return WriteSetupProfileString(sectionName, keyName, text, filePath);
}

CString GetIPAddrString(const DWORD ipAddr)
{
    char text[32] = {0};
    IPAddrToStr(ipAddr, text);
    return CString(text);
}

BOOL TryParseIPAddr(const CString &text, DWORD *ipAddr)
{
    if (!ipAddr)
        return FALSE;

    unsigned int b0 = 0;
    unsigned int b1 = 0;
    unsigned int b2 = 0;
    unsigned int b3 = 0;

    if (_stscanf((LPCTSTR)text, _T("%u.%u.%u.%u"), &b3, &b2, &b1, &b0) != 4)
        return FALSE;

    if (b0 > 255 || b1 > 255 || b2 > 255 || b3 > 255)
        return FALSE;

    unsigned char *bytes = (unsigned char *)ipAddr;
    bytes[0] = (unsigned char)b0;
    bytes[1] = (unsigned char)b1;
    bytes[2] = (unsigned char)b2;
    bytes[3] = (unsigned char)b3;

    return TRUE;
}

SETUP_PROFILE GetCurrentSetupProfile()
{
    SETUP_PROFILE profile;
    profile.name = _T("Default");
    profile.ipAddr = ::GetIPAddr();
    profile.remotePort = ::GetRemotePort();
    profile.localPort = ::GetLocalPort();
    profile.bigEndian = ::GetBigEndian();
    profile.defaultDB = ::GetDefaultDB();
    profile.cigiProtocolVersion = ::GetCigiProtocolVersion();
    return profile;
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
    m_SelectedSetupProfile = CB_ERR;
}

void CSetupDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSetupDlg)
    DDX_Control(pDX, IDC_COMBO_SETUP_PROFILE, m_ComboSetupProfile);
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
    ON_CBN_SELCHANGE(IDC_COMBO_SETUP_PROFILE, OnSetupProfileChanged)
    ON_BN_CLICKED(IDC_BUTTON_SAVE_SETUP_PROFILE, OnSaveSetupProfile)
    ON_BN_CLICKED(IDC_BUTTON_DELETE_SETUP_PROFILE, OnDeleteSetupProfile)
    ON_CBN_SELCHANGE(IDC_COMBO_VERSION, OnCigiVersionChanged)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg message handlers

void CSetupDlg::OnOK()
{
    if (!UpdateData(TRUE))
        return;

    m_IPAddrCtrl.GetAddress(m_Address);

    const int selection = m_ComboVersion.GetCurSel();
    if (selection != CB_ERR) {
        const CigiProtocolVersionId id =
            (CigiProtocolVersionId)m_ComboVersion.GetItemData(selection);
        m_CigiProtocolVersion = CigiProtocolVersion::FromId(id);
    }

    if (!m_CigiProtocolVersion.IsPacketIoImplemented()) {
        AfxMessageBox(
            _T("This version selection will be saved. CIGI 3 packet I/O is scaffolded only and will not send CIGI 3 packets until CIGI 3 packet mapping is implemented."),
            MB_OK | MB_ICONINFORMATION);
    }

    SaveSelectedSetupProfileName();

    EndDialog(IDOK);
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

    LoadSetupProfiles();
    PopulateSetupProfileCombo();

    if (m_SelectedSetupProfile >= 0
        && m_SelectedSetupProfile < m_SetupProfiles.GetSize()) {
        ApplySetupProfile(m_SetupProfiles[m_SelectedSetupProfile]);
    }

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

void CSetupDlg::LoadSetupProfiles(void)
{
    m_SetupProfiles.RemoveAll();
    m_SelectedSetupProfile = CB_ERR;

    const CString filePath = GetSetupProfileReadFilePath();
    const int count = ReadSetupProfileInt(_T("SetupProfiles"), _T("Count"), 0,
                                          filePath);
    const int profileCount = min(count, MaxSetupProfileCount);

    for (int i = 0; i < profileCount; ++i) {
        const CString sectionName = GetSetupProfileSectionName(i);

        CString profileName = ReadSetupProfileString(
            sectionName, _T("Name"), _T(""), filePath);
        profileName.TrimLeft();
        profileName.TrimRight();

        if (profileName.IsEmpty())
            continue;

        SETUP_PROFILE profile = GetCurrentSetupProfile();
        profile.name = profileName;

        CString ipAddrText = ReadSetupProfileString(
            sectionName, _T("RemoteIP"), GetIPAddrString(profile.ipAddr),
            filePath);
        DWORD ipAddr = profile.ipAddr;
        if (TryParseIPAddr(ipAddrText, &ipAddr))
            profile.ipAddr = ipAddr;

        profile.remotePort = (unsigned short)ReadSetupProfileInt(
            sectionName, _T("RemotePort"), profile.remotePort, filePath);
        profile.localPort = (unsigned short)ReadSetupProfileInt(
            sectionName, _T("LocalPort"), profile.localPort, filePath);
        profile.bigEndian = ReadSetupProfileInt(
            sectionName, _T("BigEndian"), profile.bigEndian ? 1 : 0,
            filePath) ? TRUE : FALSE;
        profile.defaultDB = ReadSetupProfileInt(
            sectionName, _T("DefaultDB"), profile.defaultDB, filePath);

        const int majorVersion = ReadSetupProfileInt(
            sectionName, _T("CigiMajor"), profile.cigiProtocolVersion.GetMajorVersion(),
            filePath);
        const int minorVersion = ReadSetupProfileInt(
            sectionName, _T("CigiMinor"), profile.cigiProtocolVersion.GetMinorVersion(),
            filePath);
        CigiProtocolVersion protocolVersion;
        if (CigiProtocolVersion::TryCreate(majorVersion, minorVersion,
                                           &protocolVersion)) {
            profile.cigiProtocolVersion = protocolVersion;
        }

        m_SetupProfiles.Add(profile);
    }

    if (m_SetupProfiles.GetSize() == 0) {
        SETUP_PROFILE profile = GetCurrentSetupProfile();
        m_SetupProfiles.Add(profile);
    }

    CString selectedProfileName = ReadSetupProfileString(
        _T("SetupProfiles"), _T("Current"), _T(""), filePath);
    selectedProfileName.TrimLeft();
    selectedProfileName.TrimRight();

    m_SelectedSetupProfile = FindSetupProfileByName(selectedProfileName);
    if (m_SelectedSetupProfile == CB_ERR)
        m_SelectedSetupProfile = 0;
}

void CSetupDlg::PopulateSetupProfileCombo(void)
{
    m_ComboSetupProfile.ResetContent();

    for (int i = 0; i < m_SetupProfiles.GetSize(); ++i) {
        const int item = m_ComboSetupProfile.AddString(m_SetupProfiles[i].name);
        m_ComboSetupProfile.SetItemData(item, i);
    }

    if (m_SelectedSetupProfile >= 0
        && m_SelectedSetupProfile < m_SetupProfiles.GetSize()) {
        for (int item = 0; item < m_ComboSetupProfile.GetCount(); ++item) {
            if ((int)m_ComboSetupProfile.GetItemData(item)
                == m_SelectedSetupProfile) {
                m_ComboSetupProfile.SetCurSel(item);
                return;
            }
        }
    }

    m_ComboSetupProfile.SetWindowText(_T(""));
}

void CSetupDlg::ApplySetupProfile(const SETUP_PROFILE &profile)
{
    SetIPAddr(profile.ipAddr);
    SetIGPort(profile.remotePort);
    SetLocalPort(profile.localPort);
    SetBigEndian(profile.bigEndian);
    SetDefaultDB(profile.defaultDB);
    SetCigiProtocolVersion(profile.cigiProtocolVersion);
}

BOOL CSetupDlg::CaptureSetupProfile(SETUP_PROFILE *profile)
{
    if (!profile)
        return FALSE;

    if (!UpdateData(TRUE))
        return FALSE;

    m_IPAddrCtrl.GetAddress(m_Address);

    profile->ipAddr = m_Address;
    profile->remotePort = (unsigned short)m_nIGPort;
    profile->localPort = (unsigned short)m_nLocalPort;
    profile->bigEndian = m_BigEndian;
    profile->defaultDB = m_DefaultDB;

    const int selection = m_ComboVersion.GetCurSel();
    if (selection != CB_ERR) {
        const CigiProtocolVersionId id =
            (CigiProtocolVersionId)m_ComboVersion.GetItemData(selection);
        profile->cigiProtocolVersion = CigiProtocolVersion::FromId(id);
    } else {
        profile->cigiProtocolVersion = m_CigiProtocolVersion;
    }

    return TRUE;
}

BOOL CSetupDlg::SaveSetupProfiles(const int selectedProfile)
{
    if (!EnsureSetupProfileConfigDirectory())
        return FALSE;

    const CString filePath = GetSetupProfileUserFilePath();
    const int oldCount = ReadSetupProfileInt(_T("SetupProfiles"), _T("Count"),
                                             0, filePath);
    const int deleteCount = max(oldCount, (int)m_SetupProfiles.GetSize());
    BOOL ok = TRUE;

    ok = WritePrivateProfileString(_T("SetupProfiles"), NULL, NULL, filePath)
         ? ok : FALSE;

    for (int i = 0; i < deleteCount; ++i) {
        ok = WritePrivateProfileString(GetSetupProfileSectionName(i), NULL,
                                       NULL, filePath) ? ok : FALSE;
    }

    ok = WriteSetupProfileInt(_T("SetupProfiles"), _T("Count"),
                              (int)m_SetupProfiles.GetSize(), filePath)
         ? ok : FALSE;

    if (selectedProfile >= 0 && selectedProfile < m_SetupProfiles.GetSize()) {
        ok = WriteSetupProfileString(_T("SetupProfiles"), _T("Current"),
                                     m_SetupProfiles[selectedProfile].name,
                                     filePath) ? ok : FALSE;
    } else {
        ok = WriteSetupProfileString(_T("SetupProfiles"), _T("Current"),
                                     _T(""), filePath) ? ok : FALSE;
    }

    for (int i = 0; i < m_SetupProfiles.GetSize(); ++i) {
        const SETUP_PROFILE &profile = m_SetupProfiles[i];
        const CString sectionName = GetSetupProfileSectionName(i);

        ok = WriteSetupProfileString(sectionName, _T("Name"), profile.name,
                                     filePath) ? ok : FALSE;
        ok = WriteSetupProfileString(sectionName, _T("RemoteIP"),
                                     GetIPAddrString(profile.ipAddr),
                                     filePath) ? ok : FALSE;
        ok = WriteSetupProfileInt(sectionName, _T("RemotePort"),
                                  profile.remotePort, filePath) ? ok : FALSE;
        ok = WriteSetupProfileInt(sectionName, _T("LocalPort"),
                                  profile.localPort, filePath) ? ok : FALSE;
        ok = WriteSetupProfileInt(sectionName, _T("BigEndian"),
                                  profile.bigEndian ? 1 : 0, filePath)
             ? ok : FALSE;
        ok = WriteSetupProfileInt(sectionName, _T("DefaultDB"),
                                  profile.defaultDB, filePath) ? ok : FALSE;
        ok = WriteSetupProfileInt(
            sectionName, _T("CigiMajor"),
            profile.cigiProtocolVersion.GetMajorVersion(), filePath)
             ? ok : FALSE;
        ok = WriteSetupProfileInt(
            sectionName, _T("CigiMinor"),
            profile.cigiProtocolVersion.GetMinorVersion(), filePath)
             ? ok : FALSE;
    }

    return ok;
}

BOOL CSetupDlg::SaveSelectedSetupProfileName(void)
{
    CString profileName;
    m_ComboSetupProfile.GetWindowText(profileName);
    profileName.TrimLeft();
    profileName.TrimRight();

    const int profileIndex = FindSetupProfileByName(profileName);
    if (profileIndex == CB_ERR)
        return SaveSetupProfiles(m_SelectedSetupProfile);

    m_SelectedSetupProfile = profileIndex;
    return SaveSetupProfiles(profileIndex);
}

int CSetupDlg::FindSetupProfileByName(const CString &name) const
{
    if (name.IsEmpty())
        return CB_ERR;

    for (int i = 0; i < m_SetupProfiles.GetSize(); ++i) {
        if (m_SetupProfiles[i].name.CompareNoCase(name) == 0)
            return i;
    }

    return CB_ERR;
}

void CSetupDlg::OnSetupProfileChanged()
{
    const int selection = m_ComboSetupProfile.GetCurSel();
    if (selection == CB_ERR)
        return;

    const int profileIndex = (int)m_ComboSetupProfile.GetItemData(selection);
    if (profileIndex < 0 || profileIndex >= m_SetupProfiles.GetSize())
        return;

    m_SelectedSetupProfile = profileIndex;
    ApplySetupProfile(m_SetupProfiles[profileIndex]);
}

void CSetupDlg::OnSaveSetupProfile()
{
    CString profileName;
    m_ComboSetupProfile.GetWindowText(profileName);
    profileName.TrimLeft();
    profileName.TrimRight();

    if (profileName.IsEmpty()) {
        AfxMessageBox(_T("Enter a setup profile name before saving."),
                      MB_OK | MB_ICONEXCLAMATION);
        return;
    }

    SETUP_PROFILE profile;
    if (!CaptureSetupProfile(&profile))
        return;

    profile.name = profileName;

    int profileIndex = FindSetupProfileByName(profileName);
    if (profileIndex == CB_ERR) {
        profileIndex = (int)m_SetupProfiles.Add(profile);
    } else {
        m_SetupProfiles[profileIndex] = profile;
    }

    m_SelectedSetupProfile = profileIndex;

    if (!SaveSetupProfiles(profileIndex)) {
        AfxMessageBox(_T("The setup profile could not be saved."),
                      MB_OK | MB_ICONEXCLAMATION);
        return;
    }

    PopulateSetupProfileCombo();
}

void CSetupDlg::OnDeleteSetupProfile()
{
    CString profileName;
    m_ComboSetupProfile.GetWindowText(profileName);
    profileName.TrimLeft();
    profileName.TrimRight();

    const int profileIndex = FindSetupProfileByName(profileName);
    if (profileIndex == CB_ERR) {
        AfxMessageBox(_T("Select an existing setup profile before deleting."),
                      MB_OK | MB_ICONEXCLAMATION);
        return;
    }

    if (m_SetupProfiles.GetSize() <= 1) {
        AfxMessageBox(_T("At least one setup profile must remain."),
                      MB_OK | MB_ICONEXCLAMATION);
        return;
    }

    CString prompt;
    prompt.Format(_T("Delete setup profile \"%s\"?"), profileName);
    if (AfxMessageBox(prompt, MB_YESNO | MB_ICONQUESTION) != IDYES)
        return;

    m_SetupProfiles.RemoveAt(profileIndex);
    m_SelectedSetupProfile = min(profileIndex,
                                 (int)m_SetupProfiles.GetSize() - 1);

    if (!SaveSetupProfiles(m_SelectedSetupProfile)) {
        AfxMessageBox(_T("The setup profile could not be deleted."),
                      MB_OK | MB_ICONEXCLAMATION);
        return;
    }

    PopulateSetupProfileCombo();

    if (m_SelectedSetupProfile >= 0
        && m_SelectedSetupProfile < m_SetupProfiles.GetSize()) {
        ApplySetupProfile(m_SetupProfiles[m_SelectedSetupProfile]);
    }
}
