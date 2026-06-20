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

#if !defined(AFX_PACKETDLGARTPARTCONTROL_H__6BE09CE0_6137_4BA6_A89F_7A13E0A7242C__INCLUDED_)
#define AFX_PACKETDLGARTPARTCONTROL_H__6BE09CE0_6137_4BA6_A89F_7A13E0A7242C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgArtpartControl.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgArtpartControl dialog

class CPacketDlgArtpartControl : public CDialog
{
    // Construction
public:
    CPacketDlgArtpartControl(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);
    void SelectEntityIDComboItem(void);
    void PopulatePartIDCombo(void);
    void RefreshView(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgArtpartControl)
    enum { IDD = IDD_PACKET_ARTPART_CONTROL };
    CComboBoxEx m_ComboPartID;
    CComboBoxEx m_ComboEntityID;
    BOOL    m_EnableArtPart;
    BOOL    m_EnablePitch;
    BOOL    m_EnableRoll;
    BOOL    m_EnableYaw;
    BOOL    m_EnableX;
    BOOL    m_EnableY;
    BOOL    m_EnableZ;
    float   m_Pitch;
    float   m_Roll;
    float   m_Yaw;
    float   m_X;
    float   m_Y;
    float   m_Z;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgArtpartControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_EntityID;
    int m_PartID;

    void ApplyArtPart(void);
    int GetEntityIDFromCombo(void);
    int GetPartIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgArtpartControl)
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

#endif // !defined(AFX_PACKETDLGARTPARTCONTROL_H__6BE09CE0_6137_4BA6_A89F_7A13E0A7242C__INCLUDED_)
