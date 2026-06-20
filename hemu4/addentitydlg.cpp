/** <pre>
 *  Copyright 2004 The Boeing Company
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
 *
 *  FILENAME:   EddEntityDlg.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface Host Emulator (CIGI HE)
 *
 *  PROGRAM DESCRIPTION:
 *  The Host Emulator (HE) acts as a surrogate in the absence of a simulation host.
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME
 *  DESCRIPTION OF CHANGE
 *
 *  11/11/2004 Lance Durham
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  Version 3.1.1
 */

#include "stdafx.h"
#include "AddEntityDlg.h"
#include "DataManager.h"
#include "globals.h"
#include "hemu4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddEntityDlg dialog

CAddEntityDlg::CAddEntityDlg(CWnd *pParent /*=NULL*/)
    : CDialog(CAddEntityDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CAddEntityDlg)
    m_ID = 0;
    //}}AFX_DATA_INIT

    m_Type = 0;
}

void CAddEntityDlg::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAddEntityDlg)
    DDX_Control(pDX, IDOK, m_ButtonOK);
    DDX_Control(pDX, IDC_LIST_TYPES, m_TypesList);
    DDX_Text(pDX, IDC_EDIT_ID, m_ID);
    DDV_MinMaxUInt(pDX, m_ID, 0, 65535);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddEntityDlg, CDialog)
    //{{AFX_MSG_MAP(CAddEntityDlg)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TYPES, OnItemchangedListTypes)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddEntityDlg message handlers

BOOL CAddEntityDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add the next available ID to the edit.
    m_ID = ::GetNextEntityID();
    ::UnmarkEntityID(m_ID); // don't mark as used until we click OK
    UpdateData(FALSE);

    // Configure and populate the type list.
    m_TypesList.SetImageList(&g_ImgListTypes, LVSIL_SMALL);
    PopulateTypeList();

    // Return TRUE unless you set the focus to a control.
    // EXCEPTION: OCX Property Pages should return FALSE.
    return TRUE;
}

void CAddEntityDlg::PopulateTypeList(void)
{
    TEMPL_ENTITY *templ = NULL;
    int type = 0;
    int index = -1;
    CString str;
    CEntityTemplMap &map = g_DataManager.GetEntityTemplMap();
    POSITION pos = map.GetStartPosition();

    // If we don't have a template for type 0, create an item.
    if (g_DataManager.GetEntityTempl(0) == NULL) {
        m_TypesList.InsertItem(LVIF_IMAGE | LVIF_TEXT | LVIF_PARAM,
                               0, "0: None", 0, 0, IMG_NOTYPE, 0);
    }

    while (pos) {
        map.GetNextAssoc(pos, type, templ);
        str.Format("%d: %s", type, templ->Name);

        if (templ) {
            index = FindListInsertPoint(type);
            index = m_TypesList.InsertItem(LVIF_IMAGE | LVIF_TEXT | LVIF_PARAM,
                                           index, (LPCTSTR)str, 0, 0, templ->Class, type);
        }
    }
}

int CAddEntityDlg::FindListInsertPoint(const int type)
{
    const int count = m_TypesList.GetItemCount();

    for (int i = 0; i < count; i++) {
        int val = m_TypesList.GetItemData(i);
        if (val > type)
            return i;
    }

    return count;
}

void CAddEntityDlg::OnOK()
{
    POSITION pos = m_TypesList.GetFirstSelectedItemPosition();

    if (pos) {
        UpdateData(TRUE);
        ::MarkEntityID(m_ID);

        int idx = m_TypesList.GetNextSelectedItem(pos);
        m_Type = m_TypesList.GetItemData(idx);

        CDialog::OnOK();
    }
}

void CAddEntityDlg::OnCancel()
{
    CDialog::OnCancel();
}

void CAddEntityDlg::OnItemchangedListTypes(NMHDR *pNMHDR, LRESULT *pResult)
{
    NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;

    m_ButtonOK.EnableWindow(TRUE);

    *pResult = 0;
}
