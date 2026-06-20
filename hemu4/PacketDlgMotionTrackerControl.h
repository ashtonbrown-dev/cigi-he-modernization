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

#if !defined(AFX_PACKETDLGMOTIONTRACKERCONTROL_H__AC6A28F6_1480_464D_90DE_A7D9BADC8BFA__INCLUDED_)
#define AFX_PACKETDLGMOTIONTRACKERCONTROL_H__AC6A28F6_1480_464D_90DE_A7D9BADC8BFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgMotionTrackerControl.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgMotionTrackerControl dialog

class CPacketDlgMotionTrackerControl : public CDialog
{
    // Construction
public:
    CPacketDlgMotionTrackerControl(CWnd *pParent = NULL);   // standard constructor
    void PopulateViewIDCombo(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgMotionTrackerControl)
    enum { IDD = IDD_PACKET_MOTION_TRACKER_CONTROL };
    CStatic m_LabelViewOrViewGroup;
    CComboBoxEx m_ComboViewID;
    BOOL    m_BoresightEnable;
    BOOL    m_PitchEnable;
    BOOL    m_RollEnable;
    BOOL    m_TrackerEnable;
    BOOL    m_XEnable;
    BOOL    m_YEnable;
    BOOL    m_YawEnable;
    BOOL    m_ZEnable;
    int     m_TrackerID;
    int     m_AttachTo;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgMotionTrackerControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int GetViewIDFromCombo(void);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgMotionTrackerControl)
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnRadioView();
    afx_msg void OnRadioGroup();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGMOTIONTRACKERCONTROL_H__AC6A28F6_1480_464D_90DE_A7D9BADC8BFA__INCLUDED_)
