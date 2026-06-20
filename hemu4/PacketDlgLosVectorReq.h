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

#if !defined(AFX_PACKETDLGLOSVECTORREQ_H__131C8855_4577_4E72_8EEE_2493D9403363__INCLUDED_)
#define AFX_PACKETDLGLOSVECTORREQ_H__131C8855_4577_4E72_8EEE_2493D9403363__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgLosVectorReq.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgLosVectorReq dialog

class CPacketDlgLosVectorReq : public CDialog
{
    // Construction
public:
    CPacketDlgLosVectorReq(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgLosVectorReq)
    enum { IDD = IDD_PACKET_LOS_VECTOR_REQ };
    CEdit   m_EditUpdatePeriod;
    CStatic m_LabelSrcLonYUnits;
    CStatic m_LabelSrcLatXUnits;
    CStatic m_LabelSrcLonY;
    CStatic m_LabelSrcLatX;
    CStatic m_LabelSrcAltZ;
    CComboBoxEx m_ComboEntityID;
    int     m_AlphaThreshold;
    float   m_Azimuth;
    float   m_Elevation;
    int     m_LosID;
    UINT    m_MaterialMask;
    float   m_MaxRange;
    float   m_MinRange;
    double  m_SrcAltZ;
    double  m_SrcLatX;
    double  m_SrcLonY;
    int     m_RespCoordSystem;
    int     m_SrcCoordSystem;
    int     m_ReqType;
    UINT    m_UpdatePeriod;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgLosVectorReq)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_EntityID;

    void ApplyLos(void);
    int GetEntityIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgLosVectorReq)
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeEntityId();
    afx_msg void OnEndeditEntityId(NMHDR *pNMHDR, LRESULT *pResult);
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

#endif // !defined(AFX_PACKETDLGLOSVECTORREQ_H__131C8855_4577_4E72_8EEE_2493D9403363__INCLUDED_)
