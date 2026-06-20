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

#if !defined(AFX_PACKETDLGTERRESTRIALCONDCONTROL_H__D74E8224_31EC_4776_9822_80A57BB2394C__INCLUDED_)
#define AFX_PACKETDLGTERRESTRIALCONDCONTROL_H__D74E8224_31EC_4776_9822_80A57BB2394C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgTerrestrialCondControl.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgTerrestrialCondControl dialog

class CPacketDlgTerrestrialCondControl : public CDialog
{
    // Construction
public:
    CPacketDlgTerrestrialCondControl(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);
    void SelectEntityIDComboItem(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgTerrestrialCondControl)
    enum { IDD = IDD_PACKET_TERRESTRIAL_CONTROL };
    CStatic m_LabelRegionEntityID;
    CComboBoxEx m_ComboRegionEntityID;
    CSpinButtonCtrl m_SpinSeverity;
    BOOL    m_EnableConditions;
    int     m_ConditionID;
    int     m_Coverage;
    int     m_Severity;
    int     m_Scope;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgTerrestrialCondControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_EntityID;

    void ApplyTerrestrialCond(void);
    int GetEntityIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgTerrestrialCondControl)
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnSelchangeEntityID();
    afx_msg void OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnRadioEntity();
    afx_msg void OnRadioGlobal();
    afx_msg void OnRadioRegional();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGTERRESTRIALCONDCONTROL_H__D74E8224_31EC_4776_9822_80A57BB2394C__INCLUDED_)
