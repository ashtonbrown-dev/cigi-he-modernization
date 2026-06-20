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

#if !defined(AFX_ENTITYMISSILEPROPPAGE_H__E7C520FA_DBDD_4534_917E_7A571DFDC867__INCLUDED_)
#define AFX_ENTITYMISSILEPROPPAGE_H__E7C520FA_DBDD_4534_917E_7A571DFDC867__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityMissilePropPage.h : header file
//

#include "CheckList.h"
#include "resource.h"

class CEntity;

/////////////////////////////////////////////////////////////////////////////
// CEntityMissilePropPage dialog

class CEntityMissilePropPage : public CDialog
{
    // Construction
public:
    CEntityMissilePropPage(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    void RefreshView(CEntity *entity);

    // Dialog Data
    //{{AFX_DATA(CEntityMissilePropPage)
    enum { IDD = IDD_PROPPAGE_ENTITY_MISSILE };
    CButton m_ButtonFire;
    CButton m_ButtonApply;
    CCheckList  m_ListDetonation;
    CCheckList  m_ListPersistent;
    CCheckList  m_ListTerminating;
    CComboBoxEx m_ComboTarget;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEntityMissilePropPage)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    void PopulateTargetCombo(CEntity *entity);
    void PopulateAnimLists(CEntity *entity);
    void AddItemsToLinkedList(CCheckList &ctrl, CList<long, long> &list);
    void UpdateAnimList(CList<long, long> &list, CCheckList &ctrl);
    void ClearCheckList(CCheckList &list);
    int GetCheckListItem(CCheckList &list, const int id);
    int FindEntityComboItem(CComboBoxEx &combo, const int entity_id);
    int FindEntityComboInsertPoint(CComboBoxEx &combo, const int entity_id);

    // Generated message map functions
    //{{AFX_MSG(CEntityMissilePropPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnButtonApply();
    afx_msg void OnSelchangeTarget();
    afx_msg void OnButtonFire();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYMISSILEPROPPAGE_H__E7C520FA_DBDD_4534_917E_7A571DFDC867__INCLUDED_)
