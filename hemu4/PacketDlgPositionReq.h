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

#if !defined(AFX_PACKETDLGPOSITIONREQ_H__AF8485E2_E609_49D4_9A4D_749C5E6ED723__INCLUDED_)
#define AFX_PACKETDLGPOSITIONREQ_H__AF8485E2_E609_49D4_9A4D_749C5E6ED723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgPositionReq.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgPositionReq dialog

class CPacketDlgPositionReq : public CDialog
{
    // Construction
public:
    CPacketDlgPositionReq(CWnd *pParent = NULL);   // standard constructor
    void PopulateObjectIDCombo(void);
    void PopulatePartIDCombo(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgPositionReq)
    enum { IDD = IDD_PACKET_POSITION_REQ };
    CComboBoxEx m_ComboObjectID;
    CComboBoxEx m_ComboPartID;
    CString m_LabelObjectID;
    int     m_ObjectClass;
    int     m_CoordSystem;
    int     m_ReqMode;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgPositionReq)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_ObjectID;
    int m_PartID;

    void ApplyHatHot(void);
    void PopulateEntityIDCombo(void);
    void PopulateViewIDCombo(void);
    void PopulateViewGroupIDCombo(void);
    void PopulateTrackerIDCombo(void);
    int GetObjectIDFromCombo(void);
    int GetPartIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgPositionReq)
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnRadioClassArtpart();
    afx_msg void OnRadioClassEntity();
    afx_msg void OnRadioClassTracker();
    afx_msg void OnRadioClassView();
    afx_msg void OnRadioClassViewgroup();
    afx_msg void OnSelchangeObjectId();
    afx_msg void OnEndeditObjectId(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangePartId();
    afx_msg void OnEndeditPartId(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGPOSITIONREQ_H__AF8485E2_E609_49D4_9A4D_749C5E6ED723__INCLUDED_)
