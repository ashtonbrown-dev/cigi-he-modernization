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

#if !defined(AFX_PACKETDLGCDVOLUMEDEF_H__3A9D428B_53FF_4A2F_BF5A_AEF34747A3BC__INCLUDED_)
#define AFX_PACKETDLGCDVOLUMEDEF_H__3A9D428B_53FF_4A2F_BF5A_AEF34747A3BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgCdVolumeDef.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgCdVolumeDef dialog

class CPacketDlgCdVolumeDef : public CDialog
{
    // Construction
public:
    CPacketDlgCdVolumeDef(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);
    void RefreshView(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgCdVolumeDef)
    enum { IDD = IDD_PACKET_CD_VOLUME_DEF };
    CStatic m_LabelHeightOrRadius;
    CEdit   m_EditDepth;
    CEdit   m_EditWidth;
    CComboBoxEx m_ComboEntityID;
    BOOL    m_VolumeEnable;
    int     m_VolumeID;
    float   m_Depth;
    float   m_HeightOrRadius;
    float   m_Width;
    float   m_Pitch;
    float   m_Roll;
    float   m_Yaw;
    float   m_X;
    float   m_Y;
    float   m_Z;
    int     m_VolumeType;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgCdVolumeDef)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_EntityID;

    void ApplyVolume(void);
    int GetEntityIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgCdVolumeDef)
    afx_msg void OnRadioTypeSphere();
    afx_msg void OnRadioTypeCuboid();
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

#endif // !defined(AFX_PACKETDLGCDVOLUMEDEF_H__3A9D428B_53FF_4A2F_BF5A_AEF34747A3BC__INCLUDED_)
