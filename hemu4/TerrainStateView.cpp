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

// TerrainStateView.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "hemu4.h"
#include "TerrainStateView.h"
#include "TerrainDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTerrainStateView dialog

CTerrainStateView::CTerrainStateView(CWnd *pParent /*=NULL*/)
    : CBaseStateView(CTerrainStateView::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTerrainStateView)
    m_strAltitude = _T("");
    m_strERM = _T("");
    m_strLatitude = _T("");
    m_strLongitude = _T("");
    m_strRadius = _T("");
    m_strFlattening = _T("");
    //}}AFX_DATA_INIT

    m_PrevDatabaseID = -1;
}

void CTerrainStateView::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTerrainStateView)
    DDX_Control(pDX, IDC_TAB1, m_Tab);
    DDX_Control(pDX, IDC_COMBO_DATABASE, m_ComboDatabase);
    DDX_Text(pDX, IDC_LABEL_ALTITUDE, m_strAltitude);
    DDX_Text(pDX, IDC_LABEL_ERM, m_strERM);
    DDX_Text(pDX, IDC_LABEL_LATITUDE, m_strLatitude);
    DDX_Text(pDX, IDC_LABEL_LONGITUDE, m_strLongitude);
    DDX_Text(pDX, IDC_LABEL_RADIUS, m_strRadius);
    DDX_Text(pDX, IDC_LABEL_FLATTENING, m_strFlattening);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTerrainStateView, CDialog)
    //{{AFX_MSG_MAP(CTerrainStateView)
    ON_CBN_SELCHANGE(IDC_COMBO_DATABASE, OnSelchangeDatabase)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab)
    ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnSelchangingTab)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerrainStateView message handlers

void CTerrainStateView::OnOK()
{
}

void CTerrainStateView::OnCancel()
{
}

void CTerrainStateView::RefreshView(void)
{
    CTerrainDB *db = g_DataManager.GetSelectedDatabase();
    if (!db)
        return;

    const int dbId = db->GetID();

    m_strLatitude.Format("%lf deg", db->GetOriginLat());
    m_strLongitude.Format("%lf deg", db->GetOriginLon());
    m_strAltitude.Format("%lf m", db->GetOriginAlt());
    m_strRadius.Format("%lf m", db->GetErmRadius());
    m_strFlattening.Format("%lf", db->GetErmFlattening());
    m_strERM = db->GetErmName();

    UpdateData(FALSE);

    // Select the current database from the combo.
    const int comboId = FindComboItem(dbId);
    m_ComboDatabase.SetCurSel(comboId);

    // Update the Components tab.
    if (m_PrevDatabaseID != dbId) {
        PopulateCompPropPage(db);
        m_PrevDatabaseID = dbId;
    }
}

void CTerrainStateView::PopulateCompPropPage(CTerrainDB *db)
{
    m_DlgComponents.ClearComponents();

    int id = 0;
    CComponent *compptr = NULL;
    POSITION pos = NULL;

    // First, get the "universal" terrain components.
    CComponentMap &univcompmap = g_DataManager.GetGlobalEnv()->GetTerrainComponentMap();
    pos = univcompmap.GetStartPosition();
    while (pos) {
        univcompmap.GetNextAssoc(pos, id, compptr);
        m_DlgComponents.AddComponent(compptr);
    }

    // Then get the database-specific terrain components.
    CComponentMap &dbcompmap = db->GetComponentMap();
    pos = dbcompmap.GetStartPosition();
    while (pos) {
        dbcompmap.GetNextAssoc(pos, id, compptr);
        m_DlgComponents.AddComponent(compptr);
    }
}

void CTerrainStateView::ClearDatabaseList(void)
{
    m_ComboDatabase.ResetContent();
}

void CTerrainStateView::PopulateDatabaseList(void)
{
    CTerrainDB *db = g_DataManager.GetSelectedDatabase();
    int current = -1;
    int cbo_idx = -1;
    int i = -1;

    if (db)
        current = db->GetID();

    m_ComboDatabase.ResetContent();

    for (i = 1; i < 128; i++) { // 1 - 127 are valid IDs
        db = g_DataManager.GetDatabase(i);
        if (db) {
            // Add a new item to the list.
            cbo_idx = FindComboInsertPoint(i);
            cbo_idx = m_ComboDatabase.InsertString(cbo_idx, (LPCTSTR)db->GetName());
            m_ComboDatabase.SetItemData(cbo_idx, db->GetID());

            if (db->GetID() == current) {
                // Select the item in the combo box.
                m_ComboDatabase.SetCurSel(cbo_idx);
            }
        }
    }

    // If we have no Database #1, select the first one in the list.
    if (m_ComboDatabase.GetCurSel() == -1)
        m_ComboDatabase.SetCurSel(0);
}

int CTerrainStateView::FindComboItem(const int id)
{
    int i = 0;
    int count = m_ComboDatabase.GetCount();

    // Test the selected item first.
    i = m_ComboDatabase.GetCurSel();
    if (i > -1) {
        if (m_ComboDatabase.GetItemData(i) == (unsigned int)id)
            return i;
    }

    i = 0;

    for (i = 0; i < count; i++) {
        if (m_ComboDatabase.GetItemData(i) == (unsigned int)id)
            return i;
    }

    return -1;
}

int CTerrainStateView::FindComboInsertPoint(const int item_id)
{
    int i = 0;
    const int count = m_ComboDatabase.GetCount();

    for (i = 0; i < count; i++)
        if (m_ComboDatabase.GetItemData(i) > (unsigned int)item_id)
            break;

    return i;
}

void CTerrainStateView::OnSelchangeDatabase()
{
    int item_idx = m_ComboDatabase.GetCurSel();
    int db_idx = m_ComboDatabase.GetItemData(item_idx);

    // First select the database in the Data Manager.  This
    // will cause the driver to relocated all of the entities.
    g_DataManager.SetSelectedDatabase(db_idx);

    // Then select the item in the toolbar.
    theApp.GetMainFrame().UpdateDatabaseCombo();

    // Finally, refresh the window.
    RefreshView();

    m_PrevDatabaseID = g_DataManager.GetSelectedDatabaseNum();
}

BOOL CTerrainStateView::OnInitDialog()
{
    CDialog::OnInitDialog();

    InitTabs();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CTerrainStateView::InitTabs(void)
{
    // Add a tab for each of the child dialog boxes.
    m_Tab.InsertItem(0, "Components");

    // Lock the resources for the child dialog boxes.
    m_DlgTempl[0] = LockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE_ENTITY_COMPONENTS));

    // Create an array of pointers to the child dialog boxes.
    m_TabDlg[0] = (CDialog *)&m_DlgComponents;

    // Create child dialog boxes.
    int tabcount = m_Tab.GetItemCount();
    ASSERT(tabcount == NUM_TERRAINTAB_PAGES);

    for (int i = 0; i < tabcount; i++) {
        m_TabDlg[i]->CreateIndirect(m_DlgTempl[i], &m_Tab);
        m_TabDlg[i]->SetWindowPos(NULL, 5, 25, 0, 0, SWP_NOSIZE);
    }

    // Simulate selection of the first item.
    SelectTab(0);
}

void CTerrainStateView::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Show the appropriate window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_SHOW);
    }

    *pResult = 0;
}

void CTerrainStateView::OnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // Hide the current window.
    int index = m_Tab.GetCurSel();

    if (m_TabDlg[index]->m_hWnd) {
        m_TabDlg[index]->ShowWindow(SW_HIDE);
    }

    *pResult = 0;
}

void CTerrainStateView::SelectTab(const int index)
{
    // Simulate selection of the item.
    LRESULT result;
    OnSelchangingTab(NULL, &result);

    m_Tab.SetCurSel(index);

    OnSelchangeTab(NULL, &result);
}