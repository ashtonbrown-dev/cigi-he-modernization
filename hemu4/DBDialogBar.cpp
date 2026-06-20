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
 *  FILENAME:   DBDialogBar.cpp
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
#include "DBDialogBar.h"
#include "hemu4.h"
#include "globals.h"
#include "TerrainDB.h"
#include "TerrainStateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBDialogBar dialog

CDBDialogBar::CDBDialogBar(CWnd *pParent /*=NULL*/)
    : CDialog(CDBDialogBar::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDBDialogBar)
    //}}AFX_DATA_INIT
}

void CDBDialogBar::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDBDialogBar)
    DDX_Control(pDX, IDC_COMBO_DATABASE, m_ComboDatabase);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDBDialogBar, CDialog)
    //{{AFX_MSG_MAP(CDBDialogBar)
    ON_CBN_SELCHANGE(IDC_COMBO_DATABASE, OnSelchangeDatabase)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBDialogBar message handlers

void CDBDialogBar::PopulateDatabaseCombo(void)
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

void CDBDialogBar::UpdateDatabaseCombo(void)
{
    CTerrainDB *db = g_DataManager.GetSelectedDatabase();

    if (db)
        m_ComboDatabase.SetCurSel(FindComboItem(db->GetID()));
}

int CDBDialogBar::FindComboItem(const int id)
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

int CDBDialogBar::FindComboInsertPoint(const int item_id)
{
    int i = 0;
    int count = m_ComboDatabase.GetCount();

    for (i = 0; i < count; i++)
        if (m_ComboDatabase.GetItemData(i) > (unsigned int)item_id)
            return i;

    return i;
}

void CDBDialogBar::OnSelchangeDatabase()
{
    int item_idx = m_ComboDatabase.GetCurSel();
    int db_idx = m_ComboDatabase.GetItemData(item_idx);

    // First select the database in the Data Manager.  This will
    // send a message to the driver, which will move all of the
    // entities relative to the new origin.
    g_DataManager.SetSelectedDatabase(db_idx);

    // Finally, refresh the Terrain View windows.
    theApp.GetMainFrame().GetTerrainStateView().RefreshView();
}
