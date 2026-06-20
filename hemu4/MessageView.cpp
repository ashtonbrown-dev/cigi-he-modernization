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

// MessageView.cpp : implementation file
//

#include "stdafx.h"
#include "Hemu4.h"
#include "MessageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageView dialog


CMessageView::CMessageView(CWnd *pParent /*=NULL*/)
    : CDialog(CMessageView::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMessageView)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CMessageView::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMessageView)
    DDX_Control(pDX, IDC_EDIT1, m_Edit);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMessageView, CDialog)
    //{{AFX_MSG_MAP(CMessageView)
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageView message handlers

void CMessageView::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // Resize the edit control.
    if (m_Edit.GetSafeHwnd())
        m_Edit.MoveWindow(0, 0, cx, cy);
}

BOOL CMessageView::OnInitDialog()
{
    CDialog::OnInitDialog();

    LOGFONT lf = {  100,        // height in 10ths of a pt
                    0,          // width -- 0 means find closest
                    0,          // escapement in 10ths of a degree
                    0,          // orientation in 10ths of a degree
                    FW_NORMAL,  // weight
                    FALSE,      // italic
                    FALSE,      // underline
                    FALSE,      // strikeout
                    ANSI_CHARSET,
                    OUT_DEFAULT_PRECIS,     // default output precision
                    CLIP_DEFAULT_PRECIS,    // default clipping precision
                    DEFAULT_QUALITY,
                    DEFAULT_PITCH | FF_MODERN,
                    ""
                 };
    m_MessageFont.CreatePointFontIndirect(&lf, NULL);
    m_Edit.SetFont(&m_MessageFont, FALSE);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CMessageView::PrintMessage(LPCTSTR msg)
{
    CString str;
    m_Edit.GetWindowText(str);

    if (str != "")
        str += "\r\n";

    str += msg;

    // Turn off drawing so the window doesn't try to redraw twice.
    m_Edit.SetRedraw(FALSE);

    // Set the text and scroll to the bottom.
    m_Edit.SetWindowText((LPCTSTR)str);
    m_Edit.LineScroll(m_Edit.GetLineCount());

    // Re-enable drawing.
    m_Edit.SetRedraw(TRUE);
}
