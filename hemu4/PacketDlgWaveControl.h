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

#if !defined(AFX_PACKETDLGWAVECONTROL_H__799F8A35_13AB_47E9_B335_7313BE576FC3__INCLUDED_)
#define AFX_PACKETDLGWAVECONTROL_H__799F8A35_13AB_47E9_B335_7313BE576FC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgWaveControl.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgWaveControl dialog

class CPacketDlgWaveControl : public CDialog
{
    // Construction
public:
    CPacketDlgWaveControl(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgWaveControl)
    enum { IDD = IDD_PACKET_WAVE_CONTROL };
    CStatic m_LabelRegionEntityID;
    CComboBoxEx m_ComboRegionEntityID;
    BOOL    m_EnableWave;
    float   m_Direction;
    float   m_Height;
    float   m_Leading;
    float   m_Period;
    float   m_PhaseOffset;
    float   m_Wavelength;
    int     m_Scope;
    int     m_BreakerType;
    int     m_WaveID;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgWaveControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_EntityID;

    void ApplyWave(void);
    int GetEntityIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgWaveControl)
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

#endif // !defined(AFX_PACKETDLGWAVECONTROL_H__799F8A35_13AB_47E9_B335_7313BE576FC3__INCLUDED_)
