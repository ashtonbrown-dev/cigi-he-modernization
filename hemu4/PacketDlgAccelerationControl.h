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

#if !defined(AFX_PACKETDLGACCELERATIONCONTROL_H__B4721F02_8ACC_4598_92BC_8C8390C58B5F__INCLUDED_)
#define AFX_PACKETDLGACCELERATIONCONTROL_H__B4721F02_8ACC_4598_92BC_8C8390C58B5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgAccelerationControl.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgAccelerationControl dialog

class CPacketDlgAccelerationControl : public CDialog
{
    // Construction
public:
    CPacketDlgAccelerationControl(CWnd *pParent = NULL);   // standard constructor
    void PopulateEntityIDCombo(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgAccelerationControl)
    enum { IDD = IDD_PACKET_ACCELERATION_CONTROL };
    CComboBoxEx m_ComboPartID;
    CComboBoxEx m_ComboEntityID;
    BOOL    m_ApplyToArtPart;
    float   m_AccelX;
    float   m_AccelY;
    float   m_AccelZ;
    float   m_AccelYaw;
    float   m_AccelPitch;
    float   m_AccelRoll;
    int     m_CoordSystem;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgAccelerationControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    int GetEntityIDFromCombo(void);
    int GetPartIDFromCombo(void);
    int FindComboItem(CComboBoxEx &combo, const int id);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgAccelerationControl)
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGACCELERATIONCONTROL_H__B4721F02_8ACC_4598_92BC_8C8390C58B5F__INCLUDED_)
