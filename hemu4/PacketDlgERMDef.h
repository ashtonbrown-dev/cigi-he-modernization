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

#if !defined(AFX_PACKETDLGERMDEF_H__293784EA_1B55_4A4C_81D3_A5481ACA8EF6__INCLUDED_)
#define AFX_PACKETDLGERMDEF_H__293784EA_1B55_4A4C_81D3_A5481ACA8EF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgERMDef.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgERMDef dialog

class CPacketDlgERMDef : public CDialog
{
    // Construction
public:
    CPacketDlgERMDef(CWnd *pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CPacketDlgERMDef)
    enum { IDD = IDD_PACKET_ERM_DEF };
    BOOL    m_Enable;
    double  m_Flattening;
    double  m_Radius;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgERMDef)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgERMDef)
    virtual void OnOK();
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGERMDEF_H__293784EA_1B55_4A4C_81D3_A5481ACA8EF6__INCLUDED_)
