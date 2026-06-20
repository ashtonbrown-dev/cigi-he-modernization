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

#if !defined(AFX_PACKETDLGLOSSEGMENTREQ_H__4A8DC408_B8F7_44B4_894B_A25413F6BA49__INCLUDED_)
#define AFX_PACKETDLGLOSSEGMENTREQ_H__4A8DC408_B8F7_44B4_894B_A25413F6BA49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgLosSegmentReq.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgLosSegmentReq dialog

class CPacketDlgLosSegmentReq : public CDialog
{
    // Construction
public:
    CPacketDlgLosSegmentReq(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgLosSegmentReq)
    enum { IDD = IDD_PACKET_LOS_SEGMENT_REQ };
    CButton m_CheckDestEntityIDValid;
    CEdit   m_EditDestEntityID;
    CEdit   m_EditUpdatePeriod;
    CStatic m_LabelSrcLonYUnits;
    CStatic m_LabelSrcLatXUnits;
    CStatic m_LabelDestLonYUnits;
    CStatic m_LabelDestLatXUnits;
    CStatic m_LabelSrcAltZ;
    CStatic m_LabelSrcLonY;
    CStatic m_LabelSrcLatX;
    CStatic m_LabelDestLonY;
    CStatic m_LabelDestLatX;
    CStatic m_LabelDestAltZ;
    CComboBoxEx m_ComboEntityID;
    CComboBoxEx m_ComboDestinationEntityID;
    int     m_AlphaThreshold;
    UINT    m_MaterialMask;
    int     m_ReqType;
    int     m_RespCoordSystem;
    int     m_SrcCoordSystem;
    int     m_DestCoordSystem;
    double  m_SrcLatX;
    double  m_SrcLonY;
    double  m_SrcAltZ;
    double  m_DestLatX;
    double  m_DestLonY;
    double  m_DestAltZ;
    int     m_LosID;
    UINT    m_UpdatePeriod;
    UINT    m_DestEntityID;
    BOOL    m_DestEntityIDValid;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgLosSegmentReq)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_EntityID;

    void ApplyLos(void);
    int GetEntityIDFromCombo(void);
	int GetDestinationEntityIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgLosSegmentReq)
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeEntityId();
    afx_msg void OnEndeditEntityId(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSelchangeDestinationEntityId();
    afx_msg void OnEndeditDestinationEntityId(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnRadioDestEntity();
    afx_msg void OnRadioDestGeodetic();
    afx_msg void OnRadioRespEntity();
    afx_msg void OnRadioRespGeodetic();
    afx_msg void OnRadioSrcEntity();
    afx_msg void OnRadioSrcGeodetic();
    afx_msg void OnRadioTypeBasic();
    afx_msg void OnRadioTypeExt();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGLOSSEGMENTREQ_H__4A8DC408_B8F7_44B4_894B_A25413F6BA49__INCLUDED_)
