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

#if !defined(AFX_PACKETDLGSHORTCOMPONENTCONTROL_H__1EC1694E_20D6_4E78_854F_D017371B3330__INCLUDED_)
#define AFX_PACKETDLGSHORTCOMPONENTCONTROL_H__1EC1694E_20D6_4E78_854F_D017371B3330__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgShortComponentControl.h : header file
//

#include "ComboBoxVariant.h"
#include "resource.h"
#include "OldDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgShortComponentControl dialog

class CPacketDlgShortComponentControl : public OldDialog
{
    // Construction
public:
    CPacketDlgShortComponentControl(CWnd *pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CPacketDlgShortComponentControl)
    enum { IDD = IDD_PACKET_SHORT_COMPONENT_CONTROL };
    CComboBoxVariant    m_cboData2;
    CComboBoxVariant    m_cboData1;
    int     m_CompClass;
    int     m_CompID;
    int     m_CompState;
    int     m_InstID;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgShortComponentControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    void ApplyComponent(void);
    afx_msg LRESULT OnUserSelDatatype(WPARAM wParam, LPARAM lParam);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgShortComponentControl)
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGSHORTCOMPONENTCONTROL_H__1EC1694E_20D6_4E78_854F_D017371B3330__INCLUDED_)
