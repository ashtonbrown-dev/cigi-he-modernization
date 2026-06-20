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

#if !defined(AFX_PACKETDLGCDSEGMENTDEF_H__2C711EFA_786E_4033_8319_02C7DE7FBE5E__INCLUDED_)
#define AFX_PACKETDLGCDSEGMENTDEF_H__2C711EFA_786E_4033_8319_02C7DE7FBE5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgCdSegmentDef.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgCdSegmentDef dialog

class CPacketDlgCdSegmentDef : public CDialog
{
    // Construction
public:
    CPacketDlgCdSegmentDef(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);
    void RefreshView(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgCdSegmentDef)
    enum { IDD = IDD_PACKET_CD_SEGMENT_DEF };
    CComboBoxEx m_ComboEntityID;
    BOOL    m_SegmentEnable;
    float   m_X1;
    float   m_X2;
    float   m_Y1;
    float   m_Y2;
    float   m_Z1;
    float   m_Z2;
    int     m_SegmentID;
    UINT    m_MaterialMask;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgCdSegmentDef)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_EntityID;

    void ApplySegment(void);
    int GetEntityIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgCdSegmentDef)
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeEntityId();
    afx_msg void OnEndeditEntityId(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGCDSEGMENTDEF_H__2C711EFA_786E_4033_8319_02C7DE7FBE5E__INCLUDED_)
