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

// PacketWatchView.cpp : implementation file
//

#include "stdafx.h"
#include "Hemu4.h"
#include "PacketWatchView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketWatchView dialog

CPacketWatchView::CPacketWatchView(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketWatchView::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPacketWatchView)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CPacketWatchView::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPacketWatchView)
    DDX_Control(pDX, IDC_LIST1, m_List);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPacketWatchView, CDialog)
    //{{AFX_MSG_MAP(CPacketWatchView)
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketWatchView message handlers

void CPacketWatchView::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if (m_List.GetSafeHwnd()) {
        // Resize the list control.
        m_List.MoveWindow(0, 0, cx, cy);

        // Resize the columns automatically.
        CRect rect;
        m_List.GetClientRect(&rect);
        int Col0Width = m_List.GetColumnWidth(COLUMN_DELTA_T_SOF);
        int Col1Width = m_List.GetColumnWidth(COLUMN_RESPONSE_TIME);
        int width = rect.Width() - Col0Width - Col1Width;
        int half = width / 2;
        m_List.SetColumnWidth(COLUMN_IG_TO_HOST, half);
        m_List.SetColumnWidth(COLUMN_HOST_TO_IG, width - half);
    }
}

BOOL CPacketWatchView::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_List.SetItemCount(1024);
    m_List.InsertColumn(COLUMN_DELTA_T_SOF, "dT", LVCFMT_LEFT, 100, -1);
    m_List.InsertColumn(COLUMN_RESPONSE_TIME, "Resp Time", LVCFMT_LEFT, 100, 1);
    m_List.InsertColumn(COLUMN_IG_TO_HOST, "IG-to-Host", LVCFMT_LEFT, 100, 2);
    m_List.InsertColumn(COLUMN_HOST_TO_IG, "Host-to-IG", LVCFMT_LEFT, 100, 3);
    m_List.SetColumnLineColor(::GetSysColor(COLOR_BTNSHADOW));
    m_List.SetFullRowSelect(TRUE);
    m_List.SetColumnWidth(COLUMN_DELTA_T_SOF, 50);
    m_List.SetColumnWidth(COLUMN_RESPONSE_TIME, 68);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}
