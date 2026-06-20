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

#if !defined(AFX_PACKETDLGVELOCITYCONTROL_H__630AD0C8_76A4_4B47_9A3F_F9F91065D523__INCLUDED_)
#define AFX_PACKETDLGVELOCITYCONTROL_H__630AD0C8_76A4_4B47_9A3F_F9F91065D523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgRateControl.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgVelocityControl dialog

class CPacketDlgVelocityControl : public CDialog
{
    // Construction
public:
    CPacketDlgVelocityControl(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);
    void SelectEntityIDComboItem(void);
    void PopulatePartIDCombo(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgVelocityControl)
    enum { IDD = IDD_PACKET_VELOCITY_CONTROL };
    CComboBoxEx m_ComboPartID;
    CComboBoxEx m_ComboEntityID;
    BOOL    m_ApplyToArtPart;
    float   m_Pitch;
    float   m_Roll;
    float   m_Yaw;
    float   m_X;
    float   m_Y;
    float   m_Z;
    int     m_CoordSystem;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgVelocityControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_EntityID;
    int m_PartID;

    int GetEntityIDFromCombo(void);
    int GetPartIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgVelocityControl)
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnSelchangeEntityID();
    afx_msg void OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangePartID();
    afx_msg void OnEndeditPartID(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGVELOCITYCONTROL_H__630AD0C8_76A4_4B47_9A3F_F9F91065D523__INCLUDED_)
