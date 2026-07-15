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

#if !defined(AFX_SETUPDLG_H__EFF60960_AC6F_423C_AC2E_A71003B2E128__INCLUDED_)
#define AFX_SETUPDLG_H__EFF60960_AC6F_423C_AC2E_A71003B2E128__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupDlg.h : header file
//

#include <afxtempl.h>

#include "resource.h"
#include "CigiProtocolVersion.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog

struct SETUP_PROFILE
{
    CString name;
    DWORD ipAddr;
    unsigned short remotePort;
    unsigned short localPort;
    BOOL bigEndian;
    int defaultDB;
    CigiProtocolVersion cigiProtocolVersion;
    BOOL publishSelectedEntityViewControl;
    unsigned short selectedEntityViewId;
};

class CSetupDlg : public CDialog
{
    // Construction
public:
    CSetupDlg(CWnd *pParent = NULL);   // standard constructor
    void SetIPAddr(const DWORD addr);
    void SetIPAddr(const unsigned char b0, const unsigned char b1,
                   const unsigned char b2, const unsigned char b3);
    DWORD GetIPAddr();
    void SetIGPort(const unsigned short port);
    unsigned short GetIGPort();
    void SetLocalPort(const unsigned short port);
    unsigned short GetLocalPort();
    void SetBigEndian(const BOOL big_endian);
    BOOL GetBigEndian(void);
    void SetDefaultDB(const int dbnum);
    int GetDefaultDB(void);
    void SetCigiProtocolVersion(const CigiProtocolVersion &version);
    CigiProtocolVersion GetCigiProtocolVersion(void) const;
    void SetPublishSelectedEntityViewControl(BOOL enabled);
    BOOL GetPublishSelectedEntityViewControl(void);
    void SetSelectedEntityViewId(unsigned short viewId);
    unsigned short GetSelectedEntityViewId(void);

    // Dialog Data
    //{{AFX_DATA(CSetupDlg)
    enum { IDD = IDD_DIALOG_SETUP };
    CComboBox m_ComboSetupProfile;
    CStatic m_LabelHz;
    CStatic m_LabelFrameRate;
    CEdit   m_EditFrameRate;
    CButton m_CheckAsyncMode;
    CComboBox m_ComboVersion;
    CIPAddressCtrl  m_IPAddrCtrl;
    BOOL    m_bAsyncMode;
    UINT    m_nFrameRate;
    UINT    m_nIGPort;
    UINT    m_nLocalPort;
    BOOL    m_BigEndian;
    int     m_DefaultDB;
    BOOL    m_PublishSelectedEntityViewControl;
    UINT    m_SelectedEntityViewId;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSetupDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    DWORD m_Address;
    CigiProtocolVersion m_CigiProtocolVersion;
    CArray<SETUP_PROFILE, SETUP_PROFILE&> m_SetupProfiles;
    int m_SelectedSetupProfile;
    void PopulateCigiVersions(void);
    void LoadSetupProfiles(void);
    void PopulateSetupProfileCombo(void);
    void ApplySetupProfile(const SETUP_PROFILE &profile);
    BOOL CaptureSetupProfile(SETUP_PROFILE *profile);
    BOOL SaveSetupProfiles(const int selectedProfile);
    BOOL SaveSelectedSetupProfileName(void);
    int FindSetupProfileByName(const CString &name) const;

    // Generated message map functions
    //{{AFX_MSG(CSetupDlg)
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL OnInitDialog();
    afx_msg void OnCigiVersionChanged();
    afx_msg void OnSetupProfileChanged();
    afx_msg void OnSaveSetupProfile();
    afx_msg void OnDeleteSetupProfile();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline void CSetupDlg::SetIPAddr(const DWORD addr)
{
    m_Address = addr;
    m_IPAddrCtrl.SetAddress(addr);
}

inline void CSetupDlg::SetIPAddr(const unsigned char b0, const unsigned char b1,
                                 const unsigned char b2, const unsigned char b3)
{
    m_Address = ((DWORD)b0 << 24) & ((DWORD)b1 << 16) & ((DWORD)b2 << 8) & (DWORD)b3;
    m_IPAddrCtrl.SetAddress(b0, b1, b2, b3);
}

inline DWORD CSetupDlg::GetIPAddr()
{
    return m_Address;
}

inline void CSetupDlg::SetIGPort(const unsigned short port)
{
    m_nIGPort = port;

    if (GetSafeHwnd())
        UpdateData(FALSE);
}

inline unsigned short CSetupDlg::GetIGPort()
{
    if (GetSafeHwnd())
        UpdateData(TRUE);

    return m_nIGPort;
}

inline void CSetupDlg::SetLocalPort(const unsigned short port)
{
    m_nLocalPort = port;

    if (GetSafeHwnd())
        UpdateData(FALSE);
}

inline unsigned short CSetupDlg::GetLocalPort()
{
    if (GetSafeHwnd())
        UpdateData(TRUE);

    return m_nLocalPort;
}

inline void CSetupDlg::SetBigEndian(const BOOL big_endian)
{
    m_BigEndian = big_endian;

    if (GetSafeHwnd())
        UpdateData(FALSE);
}

inline BOOL CSetupDlg::GetBigEndian(void)
{
    if (GetSafeHwnd())
        UpdateData(TRUE);

    return m_BigEndian;
}

inline void CSetupDlg::SetDefaultDB(const int dbnum)
{
    m_DefaultDB = dbnum;

    if (GetSafeHwnd())
        UpdateData(FALSE);
}

inline int CSetupDlg::GetDefaultDB(void)
{
    if (GetSafeHwnd())
        UpdateData(TRUE);

    return m_DefaultDB;
}

inline void CSetupDlg::SetPublishSelectedEntityViewControl(BOOL enabled)
{
    m_PublishSelectedEntityViewControl = enabled;
    if (GetSafeHwnd())
        UpdateData(FALSE);
}

inline BOOL CSetupDlg::GetPublishSelectedEntityViewControl(void)
{
    if (GetSafeHwnd())
        UpdateData(TRUE);
    return m_PublishSelectedEntityViewControl;
}

inline void CSetupDlg::SetSelectedEntityViewId(unsigned short viewId)
{
    m_SelectedEntityViewId = viewId;
    if (GetSafeHwnd())
        UpdateData(FALSE);
}

inline unsigned short CSetupDlg::GetSelectedEntityViewId(void)
{
    if (GetSafeHwnd())
        UpdateData(TRUE);
    return (unsigned short)m_SelectedEntityViewId;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPDLG_H__EFF60960_AC6F_423C_AC2E_A71003B2E128__INCLUDED_)
