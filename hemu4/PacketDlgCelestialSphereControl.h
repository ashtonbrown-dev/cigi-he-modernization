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

#if !defined(AFX_PACKETDLGCELESTIALSPHERECONTROL_H__CE1B3344_3D2F_439A_B53C_FD03296FD460__INCLUDED_)
#define AFX_PACKETDLGCELESTIALSPHERECONTROL_H__CE1B3344_3D2F_439A_B53C_FD03296FD460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgCelestialSphereControl.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgCelestialSphereControl dialog

class CPacketDlgCelestialSphereControl : public CDialog
{
    // Construction
public:
    CPacketDlgCelestialSphereControl(CWnd *pParent = NULL);   // standard constructor
    void RefreshView(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgCelestialSphereControl)
    enum { IDD = IDD_PACKET_CELESTIAL_SPHERE_CONTROL };
    BOOL    m_MoonEnable;
    BOOL    m_SunEnable;
    BOOL    m_StarFieldEnable;
    BOOL    m_EphemerisEnable;
    int     m_Hour;
    int     m_Minute;
	float	m_Seconds;
    int     m_Month;
    int     m_Day;
    int     m_Year;
    float   m_StarFieldIntensity;
    BOOL    m_DateTimeValid;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgCelestialSphereControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    void ApplyCelestialSphere(void);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgCelestialSphereControl)
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGCELESTIALSPHERECONTROL_H__CE1B3344_3D2F_439A_B53C_FD03296FD460__INCLUDED_)
