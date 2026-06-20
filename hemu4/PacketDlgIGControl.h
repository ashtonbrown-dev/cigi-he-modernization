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

#if !defined(AFX_PACKETDLGIGCONTROL_H__2DBB26E1_A95E_4C06_9853_6A1A6BD9540E__INCLUDED_)
#define AFX_PACKETDLGIGCONTROL_H__2DBB26E1_A95E_4C06_9853_6A1A6BD9540E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgIGControl.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgIGControl dialog

class CPacketDlgIGControl : public CDialog
{
    // Construction
public:
    CPacketDlgIGControl(CWnd *pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CPacketDlgIGControl)
    enum { IDD = IDD_PACKET_IG_CONTROL };
    CComboBox   m_ComboDatabase;
    CString m_Version;
    CString m_Byteswap;
    int     m_IGMode;
    UINT    m_Timestamp;
    BOOL    m_TimestampValid;
    BOOL    m_SmoothingEnable;
    BOOL    m_EntityTypeSubEnable;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgIGControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int FindComboInsertPoint(const int item_id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgIGControl)
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGIGCONTROL_H__2DBB26E1_A95E_4C06_9853_6A1A6BD9540E__INCLUDED_)
