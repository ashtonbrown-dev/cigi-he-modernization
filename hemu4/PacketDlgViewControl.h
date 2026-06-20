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

#if !defined(AFX_PACKETDLGVIEWCONTROL_H__CD714C6D_CA58_4E65_AF98_7CBE728DCCA4__INCLUDED_)
#define AFX_PACKETDLGVIEWCONTROL_H__CD714C6D_CA58_4E65_AF98_7CBE728DCCA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgViewControl.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgViewControl dialog

class CPacketDlgViewControl : public CDialog
{
    // Construction
public:
    CPacketDlgViewControl(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);
    void SelectEntityIDComboItem(void);
    void PopulateViewIDCombo(void);
    void SelectViewIDComboItem(void);
    void PopulateGroupIDCombo(void);
    void SelectGroupIDComboItem(void);
    void RefreshView(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgViewControl)
    enum { IDD = IDD_PACKET_VIEW_CONTROL };
    CComboBoxEx m_ComboGroupID;
    CComboBoxEx m_ComboViewID;
    CComboBoxEx m_ComboEntityID;
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
    //{{AFX_VIRTUAL(CPacketDlgViewControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_EntityID;
    int m_ViewID;
    int m_GroupID;

    void ApplyView(void);
    int GetEntityIDFromCombo(void);
    int GetViewIDFromCombo(void);
    int GetGroupIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgViewControl)
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnSelchangeEntityId();
    afx_msg void OnEndeditEntityId(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangeGroupId();
    afx_msg void OnEndeditGroupId(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangeViewId();
    afx_msg void OnEndeditViewId(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGVIEWCONTROL_H__CD714C6D_CA58_4E65_AF98_7CBE728DCCA4__INCLUDED_)
