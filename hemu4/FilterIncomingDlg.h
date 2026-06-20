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

#if !defined(AFX_FilterIncomingDlg_H__FD943253_F8F6_4AB8_B3CF_9A25959A2C93__INCLUDED_)
#define AFX_FilterIncomingDlg_H__FD943253_F8F6_4AB8_B3CF_9A25959A2C93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterIncomingDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CFilterIncomingDlg dialog

class CFilterIncomingDlg : public CDialog
{
    // Construction
public:
    CFilterIncomingDlg(CWnd *pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CFilterIncomingDlg)
    enum { IDD = IDD_FILTER_PAGE_INCOMING };
    BOOL    m_AerosolResp;
    BOOL    m_AnimationStopNotification;
    BOOL    m_CDSegmentNotification;
    BOOL    m_CDVolumeNotification;
    BOOL    m_EventNotification;
    BOOL    m_HatHotResp;
    BOOL    m_HatHotExtResp;
    BOOL    m_IGMessage;
    BOOL    m_LosResp;
    BOOL    m_LosExtResp;
    BOOL    m_MaritimeCondResp;
    BOOL    m_PositionResp;
    BOOL    m_SensorResp;
    BOOL    m_SensorExtResp;
    BOOL    m_StartOfFrame;
    BOOL    m_TerrestrialCondResp;
    BOOL    m_WeatherCondResp;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFilterIncomingDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CFilterIncomingDlg)
    afx_msg void OnButtonSelectAll();
    afx_msg void OnButtonSelectNone();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FilterIncomingDlg_H__FD943253_F8F6_4AB8_B3CF_9A25959A2C93__INCLUDED_)
