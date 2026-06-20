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

#if !defined(AFX_PACKETDLGSHORTARTPARTCONTROL_H__2ADF1BB0_A142_483C_9C0B_4405A4249280__INCLUDED_)
#define AFX_PACKETDLGSHORTARTPARTCONTROL_H__2ADF1BB0_A142_483C_9C0B_4405A4249280__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgShortArtpartControl.h : header file
//

#include "resource.h"
#include "OldDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgShortArtpartControl dialog

class CPacketDlgShortArtpartControl : public OldDialog
{
    // Construction
public:
    CPacketDlgShortArtpartControl(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);
    void SelectEntityIDComboItem(void);
    void PopulatePartIDCombos(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgShortArtpartControl)
    enum { IDD = IDD_PACKET_SHORT_ARTPART_CONTROL };
    CComboBoxEx m_ComboPartID2;
    CComboBoxEx m_ComboPartID1;
    CComboBoxEx m_ComboEntityID;
    BOOL    m_Enable1;
    BOOL    m_Enable2;
    int     m_Dof1;
    int     m_Dof2;
    float   m_Value1;
    float   m_Value2;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgShortArtpartControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_EntityID;
    int m_PartID1;
    int m_PartID2;

    void ApplyArtPart1(void);
    void ApplyArtPart2(void);
    int GetEntityIDFromCombo(void);
    int GetPartIDFromCombo1(void);
    int GetPartIDFromCombo2(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgShortArtpartControl)
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangeEntityID();
    afx_msg void OnSelchangePartID1();
    afx_msg void OnEndeditPartID1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangePartID2();
    afx_msg void OnEndeditPartID2(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGSHORTARTPARTCONTROL_H__2ADF1BB0_A142_483C_9C0B_4405A4249280__INCLUDED_)
