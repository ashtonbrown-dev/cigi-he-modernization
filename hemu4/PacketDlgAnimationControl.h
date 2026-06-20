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

#if !defined(AFX_PACKETDLGANIMATIONCONTROL_H__C117D102_7261_45B3_8EEA_7FEE62B14CC0__INCLUDED_)
#define AFX_PACKETDLGANIMATIONCONTROL_H__C117D102_7261_45B3_8EEA_7FEE62B14CC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgEntityControl.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgAnimationControl dialog

class CPacketDlgAnimationControl : public CDialog
{
    // Construction
public:
    CPacketDlgAnimationControl(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);
    void SelectEntityIDComboItem(void);
    void RefreshView(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgAnimationControl)
    enum { IDD = IDD_PACKET_ANIMATION_CONTROL };
    CComboBoxEx m_ComboEntityID;
    CComboBoxEx m_ComboAnimationID;
    BOOL    m_AnimLoop;
    BOOL    m_InheritAlpha;
    int     m_AnimDirection;
    int     m_AnimState;
	int		m_AnimFrameReset;
    UINT    m_Alpha;
    float   m_Speed;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgAnimationControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_EntityID;
    int m_AnimationID;

    int GetEntityIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgAnimationControl)
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnSelchangeEntityID();
    afx_msg void OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangeAnimationID();
    afx_msg void OnEndeditAnimationID(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGANIMATIONCONTROL_H__C117D102_7261_45B3_8EEA_7FEE62B14CC0__INCLUDED_)
