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

#if !defined(AFX_FILTERPACKETSDLG_H__223CF9E6_448B_43F5_98F9_47363944D1D4__INCLUDED_)
#define AFX_FILTERPACKETSDLG_H__223CF9E6_448B_43F5_98F9_47363944D1D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterPacketsDlg.h : header file
//

#include "FilterIncomingDlg.h"
#include "FilterOutgoingDlg.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CFilterPacketsDlg dialog

class CFilterPacketsDlg : public CDialog
{
    // Construction
public:
    CFilterPacketsDlg(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();

    // Dialog Data
    //{{AFX_DATA(CFilterPacketsDlg)
    enum { IDD = IDD_DIALOG_PACKET_FILTER };
    CTabCtrl    m_TabCtrl;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFilterPacketsDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CDialog *m_TabDlg[2];
    DLGTEMPLATE *m_DlgTempl[2];
    CFilterIncomingDlg m_DlgIncoming;
    CFilterOutgoingDlg m_DlgOutgoing;

    void InitTabs(void);

    // Generated message map functions
    //{{AFX_MSG(CFilterPacketsDlg)
    afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERPACKETSDLG_H__223CF9E6_448B_43F5_98F9_47363944D1D4__INCLUDED_)
