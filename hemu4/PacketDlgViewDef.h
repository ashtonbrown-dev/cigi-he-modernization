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

#if !defined(AFX_PACKETDLGVIEWDEF_H__752C1662_FBED_4FC5_9416_012A604F6568__INCLUDED_)
#define AFX_PACKETDLGVIEWDEF_H__752C1662_FBED_4FC5_9416_012A604F6568__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgViewDef.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgViewDef dialog

class CPacketDlgViewDef : public CDialog
{
    // Construction
public:
    CPacketDlgViewDef(CWnd *pParent = NULL);   // standard constructor
    void PopulateViewIDCombo(void);
    void SelectViewIDComboItem(void);
    void PopulateGroupIDCombo(void);
    void SelectGroupIDComboItem(void);
    void RefreshView(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgViewDef)
    enum { IDD = IDD_PACKET_VIEW_DEF };
    CComboBoxEx m_ComboViewID;
    CComboBoxEx m_ComboGroupID;
    BOOL    m_EnableBottom;
    BOOL    m_EnableFar;
    BOOL    m_EnableLeft;
    BOOL    m_EnableNear;
    BOOL    m_EnableRight;
    BOOL    m_EnableTop;
    BOOL    m_Reorder;
    float   m_Bottom;
    float   m_Far;
    float   m_Left;
    float   m_Near;
    float   m_Right;
    float   m_Top;
    int     m_ViewType;
    int     m_MirrorMode;
    int     m_PixelRepMode;
    int     m_ProjectionType;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgViewDef)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_ViewID;
    int m_GroupID;

    void ApplyView(void);
    int GetViewIDFromCombo(void);
    int GetGroupIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgViewDef)
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeViewId();
    afx_msg void OnEndeditViewId(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangeGroupId();
    afx_msg void OnEndeditGroupId(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGVIEWDEF_H__752C1662_FBED_4FC5_9416_012A604F6568__INCLUDED_)
