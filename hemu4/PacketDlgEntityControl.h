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

#if !defined(AFX_PACKETDLGENTITYCONTROL_H__C117D102_7261_45B3_8EEA_7FEE62B14CC0__INCLUDED_)
#define AFX_PACKETDLGENTITYCONTROL_H__C117D102_7261_45B3_8EEA_7FEE62B14CC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgEntityControl.h : header file
//

#include "resource.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgEntityControl dialog

class CPacketDlgEntityControl : public CDialog
{
    // Construction
public:
    CPacketDlgEntityControl(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);
    void SelectEntityIDComboItem(void);
    void RefreshView(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgEntityControl)
    enum { IDD = IDD_PACKET_ENTITY_CONTROL };
    CComboBoxEx m_ComboEntityID;
    BOOL    m_CollDetectEnable;
    BOOL    m_InheritAlpha;
	BOOL	m_SmoothingEnable;
	BOOL	m_ExtendedEntityType;
    int     m_EntityState;
    UINT    m_Alpha;
    UCHAR	m_EntityKind;
    UCHAR	m_EntityDomain;
    short	m_EntityCountry;
    UCHAR	m_EntityCategory;
    UCHAR	m_EntitySubcategory;
    UCHAR	m_EntitySpecific;
    UCHAR	m_EntityExtra;
	int		m_EntityType;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgEntityControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_EntityID;

    void ApplyEntity(void);
    int GetEntityIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgEntityControl)
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnSelchangeEntityID();
    afx_msg void OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedCheckExtendedEntityType();
	CButton m_extendedEntityType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGENTITYCONTROL_H__C117D102_7261_45B3_8EEA_7FEE62B14CC0__INCLUDED_)
