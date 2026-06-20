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

#if !defined(AFX_PACKETDLGATMOSPHERECONTROL_H__AD4259BD_94D8_427A_B2B7_DB80F5A184D4__INCLUDED_)
#define AFX_PACKETDLGATMOSPHERECONTROL_H__AD4259BD_94D8_427A_B2B7_DB80F5A184D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgAtmosphereControl.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgAtmosphereControl dialog

class CPacketDlgAtmosphereControl : public CDialog
{
    // Construction
public:
    CPacketDlgAtmosphereControl(CWnd *pParent = NULL);   // standard constructor
    void RefreshView(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgAtmosphereControl)
    enum { IDD = IDD_PACKET_ATMOSPHERE_CONTROL };
    BOOL    m_AtmosphericsEnable;
    float   m_AirTemperature;
    float   m_BaroPressure;
    int     m_Humidity;
    float   m_VisibilityRange;
    float   m_WindDirection;
    float   m_WindSpeedHorz;
    float   m_WindSpeedVert;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgAtmosphereControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    void ApplyAtmosphere(void);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgAtmosphereControl)
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGATMOSPHERECONTROL_H__AD4259BD_94D8_427A_B2B7_DB80F5A184D4__INCLUDED_)
