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

#if !defined(AFX_PACKETDLGHATHOTREQ_H__008E5308_4F6C_43E6_B981_9051163D4305__INCLUDED_)
#define AFX_PACKETDLGHATHOTREQ_H__008E5308_4F6C_43E6_B981_9051163D4305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgHatHotReq.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgHatHotReq dialog

class CPacketDlgHatHotReq : public CDialog
{
    // Construction
public:
    CPacketDlgHatHotReq(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgHatHotReq)
    enum { IDD = IDD_PACKET_HAT_HOT_REQ };
    CEdit   m_EditUpdatePeriod;
    CStatic m_LabelLonYUnits;
    CStatic m_LabelLatXUnits;
    CStatic m_LabelLonY;
    CStatic m_LabelLatX;
    CStatic m_LabelAltZ;
    CComboBoxEx m_ComboEntityID;
    double  m_AltZ;
    double  m_LatX;
    double  m_LonY;
    int     m_HatHotID;
    int     m_ReqType;
    int     m_CoordSystem;
    UINT    m_UpdatePeriod;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgHatHotReq)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_EntityID;

    void ApplyHatHot(void);
    int GetEntityIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgHatHotReq)
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnRadioCoordSysEntity();
    afx_msg void OnRadioCoordSysGeodetic();
    afx_msg void OnRadioTypeHat();
    afx_msg void OnRadioTypeHot();
    afx_msg void OnRadioTypeExt();
    afx_msg void OnSelchangeEntityId();
    afx_msg void OnEndeditEntityId(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGHATHOTREQ_H__008E5308_4F6C_43E6_B981_9051163D4305__INCLUDED_)
