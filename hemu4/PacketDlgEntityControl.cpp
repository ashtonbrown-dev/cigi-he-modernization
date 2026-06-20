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

// PacketDlgEntityControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "hemu4.h"
#include "PacketDlgEntityControl.h"
#include "EntityStateView.h"
#include "EntityTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgEntityControl dialog

CPacketDlgEntityControl::CPacketDlgEntityControl(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgEntityControl::IDD, pParent)
{
    m_EntityID = -1;

    //{{AFX_DATA_INIT(CPacketDlgEntityControl)
    m_CollDetectEnable = FALSE;
    m_SmoothingEnable = FALSE;
    m_InheritAlpha = FALSE;
	m_ExtendedEntityType = FALSE;
    m_EntityState = 1;
    m_Alpha = 0;
	m_EntityKind = 0;
	m_EntityDomain = 0;
	m_EntityCountry = 0;
	m_EntityCategory = 0;
	m_EntitySubcategory = 0;
	m_EntitySpecific = 0;
	m_EntityExtra = 0;
    //}}AFX_DATA_INIT
}

void CPacketDlgEntityControl::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPacketDlgEntityControl)
	DDX_Control(pDX, IDC_COMBOBOXEX_ENTITY_ID, m_ComboEntityID);
	DDX_Check(pDX, IDC_CHECK_COLLISION_DETECT, m_CollDetectEnable);
	DDX_Check(pDX, IDC_CHECK_INHERIT_ALPHA, m_InheritAlpha);
	DDX_Check(pDX, IDC_CHECK_INHERIT_SMOOTHING_ENABLE2, m_SmoothingEnable);
	DDX_Check(pDX, IDC_CHECK_EXTENDED_ENTITY_TYPE2, m_ExtendedEntityType);

	int inactive  = 1;
	int destroyed = 1;
	DDX_Radio( pDX, IDC_RADIO_STATE_INACTIVE, inactive );
	DDX_Radio( pDX, IDC_RADIO_STATE_DESTROYED, destroyed );
	// inactive/standby - 0, active - 1, destroyed - 2
	m_EntityState = inactive == 0 ? 0 : destroyed == 0 ? 2 : 1;

	DDX_Text(pDX, IDC_EDIT_ALPHA, m_Alpha);
	DDV_MinMaxUInt(pDX, m_Alpha, 0, 255);
	DDX_Text(pDX, IDC_EDIT_ENTITY_KIND, m_EntityKind);
	DDV_MinMaxUInt(pDX, m_EntityKind, 0, 255);
	DDX_Text(pDX, IDC_EDIT_ENTITY_DOMAIN, m_EntityDomain);
	DDV_MinMaxUInt(pDX, m_EntityDomain, 0, 255);
	DDX_Text(pDX, IDC_EDIT_ENTITY_COUNTRY, m_EntityCountry);
	DDV_MinMaxUInt(pDX, m_EntityCountry, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_ENTITY_CATEGORY, m_EntityCategory);
	DDV_MinMaxUInt(pDX, m_EntityCategory, 0, 255);
	DDX_Text(pDX, IDC_EDIT_ENTITY_SUBCATEGORY, m_EntitySubcategory);
	DDV_MinMaxUInt(pDX, m_EntitySubcategory, 0, 255);
	DDX_Text(pDX, IDC_EDIT_ENTITY_SPECIFIC, m_EntitySpecific);
	DDV_MinMaxUInt(pDX, m_EntitySpecific, 0, 255);
	DDX_Text(pDX, IDC_EDIT_ENTITY_EXTRA, m_EntityExtra);
	DDV_MinMaxUInt(pDX, m_EntityExtra, 0, 255);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_EXTENDED_ENTITY_TYPE2, m_extendedEntityType);
}

BEGIN_MESSAGE_MAP(CPacketDlgEntityControl, CDialog)
    //{{AFX_MSG_MAP(CPacketDlgEntityControl)
    ON_CBN_SELCHANGE(IDC_COMBOBOXEX_ENTITY_ID, OnSelchangeEntityID)
    ON_NOTIFY(CBEN_ENDEDIT, IDC_COMBOBOXEX_ENTITY_ID, OnEndeditEntityID)
    //}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_EXTENDED_ENTITY_TYPE, OnClickedCheckExtendedEntityType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgEntityControl message handlers

BOOL CPacketDlgEntityControl::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the image lists for the combo boxes.
    m_ComboEntityID.SetImageList(&g_ImgListTypes);
	if( GetDlgItem( IDC_EDIT_ENTITY_KIND ) ) GetDlgItem( IDC_EDIT_ENTITY_KIND )->ShowWindow( SW_HIDE );
	if( GetDlgItem( IDC_EDIT_ENTITY_CATEGORY ) ) GetDlgItem( IDC_EDIT_ENTITY_CATEGORY )->ShowWindow( SW_HIDE );
	if( GetDlgItem( IDC_EDIT_ENTITY_SPECIFIC ) ) GetDlgItem( IDC_EDIT_ENTITY_SPECIFIC )->ShowWindow( SW_HIDE );
	if( GetDlgItem( IDC_EDIT_ENTITY_DOMAIN ) )	GetDlgItem( IDC_EDIT_ENTITY_DOMAIN )->ShowWindow( SW_HIDE );
	if( GetDlgItem( IDC_EDIT_ENTITY_SUBCATEGORY ) ) GetDlgItem( IDC_EDIT_ENTITY_SUBCATEGORY )->ShowWindow( SW_HIDE );
	if( GetDlgItem( IDC_EDIT_ENTITY_EXTRA ) ) GetDlgItem( IDC_EDIT_ENTITY_EXTRA )->ShowWindow( SW_HIDE );
	if( GetDlgItem( IDC_LABEL_KIND ) ) GetDlgItem( IDC_LABEL_KIND )->ShowWindow( SW_HIDE );
	if( GetDlgItem( IDC_LABEL_CATEGORY ) ) GetDlgItem( IDC_LABEL_CATEGORY )->ShowWindow( SW_HIDE );
	if( GetDlgItem( IDC_LABEL_SPECIFIC ) ) GetDlgItem( IDC_LABEL_SPECIFIC )->ShowWindow( SW_HIDE );
	if( GetDlgItem( IDC_LABEL_DOMAIN ) ) GetDlgItem( IDC_LABEL_DOMAIN )->ShowWindow( SW_HIDE );
	if( GetDlgItem( IDC_LABEL_SUBCATEGORY ) ) GetDlgItem( IDC_LABEL_SUBCATEGORY )->ShowWindow( SW_HIDE );
	if( GetDlgItem( IDC_LABEL_EXTRA ) ) GetDlgItem( IDC_LABEL_EXTRA )->ShowWindow( SW_HIDE );
	if( GetDlgItem( IDC_LABEL_COUNTRY ) ) GetDlgItem( IDC_LABEL_COUNTRY )->SetWindowTextA( "Short Entity Type" );

    PopulateEntityIDCombo();
    SelectEntityIDComboItem();
    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgEntityControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgEntityControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    //CIGI_ENTITY_CONTROL ec = {0};
    //ec.packet_id = CIGI_ENTITY_CONTROL_OPCODE;
    //ec.packet_size = sizeof(CIGI_ENTITY_CONTROL);
    //ec.entity_id = m_EntityID;
    //ec.collision_detect = m_CollDetectEnable;
    //ec.inherit_alpha = m_InheritAlpha;
    //ec.alpha = m_Alpha;
    //ec.smoothing_enable = m_SmoothingEnable;
    //ec.entity_extra = m_ExtendedEntityType;

    //ec.entity_kind = m_EntityKind;
    //ec.entity_domain = m_EntityDomain;
    //ec.entity_country_type = m_EntityCountry;
    //ec.entity_category = m_EntityCategory;
    //ec.entity_subcategory = m_EntitySubcategory;
    //ec.entity_specific = m_EntitySpecific;
    //ec.entity_extra = m_EntityExtra;
    //ec.entity_state = m_EntityState;
    //ec.extended_entity_type = m_ExtendedEntityType;

    //SendImmedCigiMessage(&ec);

    ApplyEntity();
}

void CPacketDlgEntityControl::ApplyEntity(void)
{
	if( m_ExtendedEntityType == false ) 
		m_EntityType = m_EntityCountry;
    // Find the entity and update its data.
    CEntity *entity = g_DataManager.GetEntity(m_EntityID);
    if (!entity) {
        TEMPL_ENTITY *templ = g_DataManager.GetEntityTempl(m_EntityType);
        if (templ)
            entity = theApp.GetMainFrame().AddNewEntity(m_EntityID, templ);

		if ( ! entity )
			return;
    }

    ENTITY state = entity->GetAndLock();
    state.cigi.active = m_EntityState;
	state.cigi.coll_detect = m_CollDetectEnable;
	state.cigi.inherit_alpha = m_InheritAlpha;
	state.cigi.smoothing = m_SmoothingEnable;
	state.cigi.extended_type = m_ExtendedEntityType;
	state.cigi.id = m_EntityID;
    state.cigi.alpha = m_Alpha;
	state.cigi.entity_kind = m_EntityKind;
	state.cigi.entity_domain = m_EntityDomain;
	state.cigi.entity_country = m_EntityCountry;
	state.cigi.entity_category = m_EntityCategory;
	state.cigi.entity_subcategory = m_EntitySubcategory;
	state.cigi.entity_specific = m_EntitySpecific;
	state.cigi.entity_extra = m_EntityExtra;
	state.cigi.type = m_EntityType;

    // Set the shared object and unlock.
    entity->SetLocked(state);
    entity->Unlock();

    // chasCEntity *parent = g_DataManager.GetEntity(m_ParentID);
    // chasif ((state.cigi.parent_id != (short)m_ParentID) && (parent || (m_ParentID == -1))) {
        // Update the tree view.
        HTREEITEM htreeParent;
        CEntityTreeView &treeview = theApp.GetMainFrame().GetEntityTreeView();

        // chasif (parent)
        // chas    htreeParent = parent->GetHtree();
        // chaselse
            htreeParent = NULL;

        HTREEITEM htreeItem = treeview.MoveTreeItem(entity->GetHtree(), htreeParent);
        entity->SetHtree(htreeItem);
        treeview.GetTreeCtrl().Expand(htreeParent, TVE_EXPAND);
        treeview.GetTreeCtrl().SelectItem(htreeItem);

        // Repopulate the parent and reference entity lists.
        treeview.RepopulateEntityStateParentList();
        theApp.GetMainFrame().GetEntityStateView().PopulateRefEntityList();
    // chas}

    // Update the entity view.
    theApp.GetMainFrame().GetEntityStateView().RefreshView();
}

void CPacketDlgEntityControl::RefreshView(void)
{
    CEntity *entity = g_DataManager.GetEntity(m_EntityID);
    if (!entity)
        return;

    ENTITY state = entity->GetAndLock();
    entity->Unlock();

    m_CollDetectEnable = state.cigi.coll_detect;
    m_InheritAlpha = state.cigi.inherit_alpha;
    m_EntityState = state.cigi.active;
    m_Alpha = state.cigi.alpha;
	m_EntityType = entity->GetType();
	m_ExtendedEntityType = state.cigi.extended_type;
	if( state.cigi.extended_type ) {
		m_EntityKind = state.cigi.entity_kind;
		m_EntityDomain = state.cigi.entity_domain;
		m_EntityCountry = state.cigi.entity_country;
		m_EntityCategory = state.cigi.entity_category;
		m_EntitySubcategory = state.cigi.entity_subcategory;
		m_EntitySpecific = state.cigi.entity_specific;
		m_EntityExtra = state.cigi.entity_extra;
	}
	else {
		m_EntityCountry = ( short ) m_EntityType;
	}

    UpdateData(FALSE);
}

void CPacketDlgEntityControl::OnSelchangeEntityID()
{
    // Get the ID of the selected entity.
    int idx = m_ComboEntityID.GetCurSel();
    if (idx > -1)
        m_EntityID = MAKE_ID(m_ComboEntityID.GetItemData(idx));

    RefreshView();
}

void CPacketDlgEntityControl::OnEndeditEntityID(NMHDR *pNMHDR, LRESULT *pResult)
{
    int new_id = atoi(((NMCBEENDEDIT *)pNMHDR)->szText);
    if (new_id == m_EntityID)
        return;

    m_EntityID = new_id;
    int idx = FindComboItem(m_ComboEntityID, m_EntityID);
    if (idx > -1) {
        // Post a CB_SELCURSEL message to the message queue.  Otherwise, MFC will
        // select no item after it has finished its stuff.
        ::PostMessage(m_ComboEntityID.GetSafeHwnd(), CB_SETCURSEL, idx, 0);

        // Post a CBN_SELCHANGE notification so OnSelchangeEntityID() will be called.
        // The function must be called after the CB_SELCURSEL message is processed
        // so the new item will be selected.
        ::PostMessage(GetSafeHwnd(), WM_COMMAND,
                      MAKEWPARAM(IDC_COMBOBOXEX_ENTITY_ID, CBN_SELCHANGE),
                      (LPARAM)m_ComboEntityID.GetSafeHwnd());
    }
    *pResult = 0;
}


void CPacketDlgEntityControl::PopulateEntityIDCombo(void)
{
    char text[256] = {0};
    int cbo_idx = -1;
    int entity_id = -1;
    CEntity *entity = NULL;
    COMBOBOXEXITEM cboitem = {  CBEIF_IMAGE | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT,
                                0, NULL, 1, IMG_GENERIC, IMG_GENERIC, 0, 0, -1
                             };

    m_ComboEntityID.ResetContent();

    CEntityMap &map = g_DataManager.GetEntityMap();
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, entity_id, entity);

        strcpy(text, entity->GetName());
        cboitem.pszText = text;
        cboitem.cchTextMax = strlen(text + 1);
        cboitem.iImage = entity->GetClass();
        cboitem.iSelectedImage = cboitem.iImage;
        cboitem.lParam = MAKE_TREE_DATA(entity->GetID(), OBJECT_TYPE_ENTITY,
                                        entity->GetClass());

        // Add a new item to the list.
        cboitem.iItem = FindComboInsertPoint(m_ComboEntityID, entity->GetID());
        cbo_idx = m_ComboEntityID.InsertItem(&cboitem);
        if (entity->GetID() == entity_id) {
            // Select the item in the combo box.
            m_ComboEntityID.SetCurSel(cbo_idx);
        }
    }
}

void CPacketDlgEntityControl::SelectEntityIDComboItem(void)
{
    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity) {
        m_ComboEntityID.SetCurSel(-1);

        return;
    }

    m_EntityID = entity->GetID();
    m_ComboEntityID.SetCurSel(FindComboItem(m_ComboEntityID, m_EntityID));
    OnSelchangeEntityID();
}



int CPacketDlgEntityControl::GetEntityIDFromCombo(void)
{
    int itemdata = m_ComboEntityID.GetItemData(m_ComboEntityID.GetCurSel());
    if (itemdata != -1)
        return MAKE_ID(itemdata);
    else {
        CString text;
        m_ComboEntityID.GetWindowText(text);
        return atoi((LPCTSTR)text);
    }
}

int CPacketDlgEntityControl::FindComboItem(CComboBoxEx &combo, const int id)
{
    int i = 0;
    COMBOBOXEXITEM cboitem = { CBEIF_LPARAM, 0, NULL, 1, 0, 0, 0, 0, -1 };

    // Test the selected item first.
    i = combo.GetCurSel();
    if (i > -1) {
        if ((short)MAKE_ID(combo.GetItemData(i)) == (short)id)
            return i;
    }

    i = 0;

    while (combo.GetItem(&cboitem)) {
        if ((short)MAKE_ID(cboitem.lParam) == (short)id)
            return i;

        cboitem.iItem = ++i;
    }

    return -1;
}

int CPacketDlgEntityControl::FindComboInsertPoint(CComboBoxEx &combo, const int id)
{
    int i = 0;
    COMBOBOXEXITEM cboitem = { CBEIF_LPARAM, 0, NULL, 1, 0, 0, 0, 0, -1 };

    while (combo.GetItem(&cboitem)) {
        if ((short)MAKE_ID(cboitem.lParam) > id)
            return i;

        cboitem.iItem = ++i;
    }

    return i;
}


void CPacketDlgEntityControl::OnClickedCheckExtendedEntityType()
{
static short temp;
CString str;
	if( m_extendedEntityType.GetCheck() == 0 ) {
		if( GetDlgItem( IDC_EDIT_ENTITY_KIND ) ) GetDlgItem( IDC_EDIT_ENTITY_KIND )->ShowWindow( SW_HIDE );
		if( GetDlgItem( IDC_EDIT_ENTITY_CATEGORY ) ) GetDlgItem( IDC_EDIT_ENTITY_CATEGORY )->ShowWindow( SW_HIDE );
		if( GetDlgItem( IDC_EDIT_ENTITY_SPECIFIC ) ) GetDlgItem( IDC_EDIT_ENTITY_SPECIFIC )->ShowWindow( SW_HIDE );
		if( GetDlgItem( IDC_EDIT_ENTITY_DOMAIN ) )	GetDlgItem( IDC_EDIT_ENTITY_DOMAIN )->ShowWindow( SW_HIDE );
		if( GetDlgItem( IDC_EDIT_ENTITY_SUBCATEGORY ) ) GetDlgItem( IDC_EDIT_ENTITY_SUBCATEGORY )->ShowWindow( SW_HIDE );
		if( GetDlgItem( IDC_EDIT_ENTITY_EXTRA ) ) GetDlgItem( IDC_EDIT_ENTITY_EXTRA )->ShowWindow( SW_HIDE );
		if( GetDlgItem( IDC_LABEL_KIND ) ) GetDlgItem( IDC_LABEL_KIND )->ShowWindow( SW_HIDE );
		if( GetDlgItem( IDC_LABEL_CATEGORY ) ) GetDlgItem( IDC_LABEL_CATEGORY )->ShowWindow( SW_HIDE );
		if( GetDlgItem( IDC_LABEL_SPECIFIC ) ) GetDlgItem( IDC_LABEL_SPECIFIC )->ShowWindow( SW_HIDE );
		if( GetDlgItem( IDC_LABEL_DOMAIN ) ) GetDlgItem( IDC_LABEL_DOMAIN )->ShowWindow( SW_HIDE );
		if( GetDlgItem( IDC_LABEL_SUBCATEGORY ) ) GetDlgItem( IDC_LABEL_SUBCATEGORY )->ShowWindow( SW_HIDE );
		if( GetDlgItem( IDC_LABEL_EXTRA ) ) GetDlgItem( IDC_LABEL_EXTRA )->ShowWindow( SW_HIDE );
		if( GetDlgItem( IDC_LABEL_COUNTRY ) ) GetDlgItem( IDC_LABEL_COUNTRY )->SetWindowTextA( "Short Entity Type" );
		str.Format( "%d", m_EntityType );
		if( GetDlgItem( IDC_EDIT_ENTITY_COUNTRY ) ) GetDlgItem( IDC_EDIT_ENTITY_COUNTRY )->SetWindowTextA( str );
		temp = m_EntityCountry;
	}
	else {
		if( GetDlgItem( IDC_EDIT_ENTITY_KIND ) ) GetDlgItem( IDC_EDIT_ENTITY_KIND )->ShowWindow( SW_SHOW );
		if( GetDlgItem( IDC_EDIT_ENTITY_CATEGORY ) ) GetDlgItem( IDC_EDIT_ENTITY_CATEGORY )->ShowWindow( SW_SHOW );
		if( GetDlgItem( IDC_EDIT_ENTITY_SPECIFIC ) ) GetDlgItem( IDC_EDIT_ENTITY_SPECIFIC )->ShowWindow( SW_SHOW );
		if( GetDlgItem( IDC_EDIT_ENTITY_DOMAIN ) )	GetDlgItem( IDC_EDIT_ENTITY_DOMAIN )->ShowWindow( SW_SHOW );
		if( GetDlgItem( IDC_EDIT_ENTITY_SUBCATEGORY ) ) GetDlgItem( IDC_EDIT_ENTITY_SUBCATEGORY )->ShowWindow( SW_SHOW );
		if( GetDlgItem( IDC_EDIT_ENTITY_EXTRA ) ) GetDlgItem( IDC_EDIT_ENTITY_EXTRA )->ShowWindow( SW_SHOW );
		if( GetDlgItem( IDC_LABEL_KIND ) ) GetDlgItem( IDC_LABEL_KIND )->ShowWindow( SW_SHOW );
		if( GetDlgItem( IDC_LABEL_CATEGORY ) ) GetDlgItem( IDC_LABEL_CATEGORY )->ShowWindow( SW_SHOW );
		if( GetDlgItem( IDC_LABEL_SPECIFIC ) ) GetDlgItem( IDC_LABEL_SPECIFIC )->ShowWindow( SW_SHOW );
		if( GetDlgItem( IDC_LABEL_DOMAIN ) ) GetDlgItem( IDC_LABEL_DOMAIN )->ShowWindow( SW_SHOW );
		if( GetDlgItem( IDC_LABEL_SUBCATEGORY ) ) GetDlgItem( IDC_LABEL_SUBCATEGORY )->ShowWindow( SW_SHOW );
		if( GetDlgItem( IDC_LABEL_EXTRA ) ) GetDlgItem( IDC_LABEL_EXTRA )->ShowWindow( SW_SHOW );
		if( GetDlgItem( IDC_LABEL_COUNTRY ) ) GetDlgItem( IDC_LABEL_COUNTRY )->SetWindowTextA( "Country" );
		str.Format( "%d", temp );
		if( GetDlgItem( IDC_EDIT_ENTITY_COUNTRY ) ) GetDlgItem( IDC_EDIT_ENTITY_COUNTRY )->SetWindowTextA( str );
		m_EntityCountry = temp;
	}
}
