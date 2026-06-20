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

#if !defined(AFX_FilterOutgoingDlg_H__FEEF96FB_DE23_430F_AC7E_87237906ACBD__INCLUDED_)
#define AFX_FilterOutgoingDlg_H__FEEF96FB_DE23_430F_AC7E_87237906ACBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterOutgoingDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CFilterOutgoingDlg dialog

class CFilterOutgoingDlg : public CDialog
{
    // Construction
public:
    CFilterOutgoingDlg(CWnd *pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CFilterOutgoingDlg)
    enum { IDD = IDD_FILTER_PAGE_OUTGOING };
    BOOL    m_ArtPartControl;
    BOOL    m_AtmosphereControl;
    BOOL    m_CCEntityControl;
    BOOL    m_CDSegmentDef;
    BOOL    m_CDVolumeDef;
    BOOL    m_CelestialSphereControl;
    BOOL    m_ComponentControl;
    BOOL    m_EntityControl;
    BOOL    m_EnvRegionControl;
    BOOL    m_EnvConditionsReq;
    BOOL    m_ErmDef;
    BOOL    m_HatHotReq;
    BOOL    m_IGControl;
    BOOL    m_LOSSegmentReq;
    BOOL    m_LOSVectorReq;
    BOOL    m_MaritimeCondControl;
    BOOL    m_PositionReq;
    BOOL    m_RateControl;
    BOOL    m_SensorControl;
    BOOL    m_ShortArtPartControl;
    BOOL    m_ShortComponentControl;
    BOOL    m_TerrestrialCondControl;
    BOOL    m_MotionTrackerControl;
    BOOL    m_TrajectoryDef;
    BOOL    m_ViewControl;
    BOOL    m_ViewDef;
    BOOL    m_WaveControl;
    BOOL    m_WeatherControl;
    BOOL m_SymbolSurfaceDef;
    BOOL m_SymbolTextDef;
    BOOL m_SymbolCircleDef;
    BOOL m_SymbolLineDef;
    BOOL  m_SymbolClone;
    BOOL m_SymbolControl;
    BOOL m_ShortSymbolControl;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFilterOutgoingDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CFilterOutgoingDlg)
    afx_msg void OnButtonSelectAll();
    afx_msg void OnButtonSelectNone();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FilterOutgoingDlg_H__FEEF96FB_DE23_430F_AC7E_87237906ACBD__INCLUDED_)
