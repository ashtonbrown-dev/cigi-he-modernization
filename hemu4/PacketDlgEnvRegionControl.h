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

#if !defined(AFX_PACKETDLGENVREGIONCONTROL_H__99A0617B_6FAC_4092_AA8E_D5FCDDD685E4__INCLUDED_)
#define AFX_PACKETDLGENVREGIONCONTROL_H__99A0617B_6FAC_4092_AA8E_D5FCDDD685E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgEnvRegionControl.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgEnvRegionControl dialog

class CPacketDlgEnvRegionControl : public CDialog
{
    // Construction
public:
    CPacketDlgEnvRegionControl(CWnd *pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CPacketDlgEnvRegionControl)
    enum { IDD = IDD_PACKET_ENV_REGION_CONTROL };
    double  m_Latitude;
    double  m_Longitude;
    float   m_TransitionPerimeter;
    float   m_CornerRadius;
    int     m_RegionID;
    float   m_Rotation;
    float   m_SizeX;
    float   m_SizeY;
    int     m_MergeAerosol;
    int     m_MergeMaritime;
    int     m_MergeTerrestrial;
    int     m_MergeWeather;
    int     m_RegionState;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgEnvRegionControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgEnvRegionControl)
    virtual void OnCancel();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGENVREGIONCONTROL_H__99A0617B_6FAC_4092_AA8E_D5FCDDD685E4__INCLUDED_)
