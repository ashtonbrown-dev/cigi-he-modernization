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

#if !defined(AFX_ENTITYCOLLDETPROPPAGE_H__F9E81A05_958F_400F_A458_BE8022D1B372__INCLUDED_)
#define AFX_ENTITYCOLLDETPROPPAGE_H__F9E81A05_958F_400F_A458_BE8022D1B372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityCollDetPropPage.h : header file
//

#include "resource.h"

class CCollisionSeg;
class CCollisionVol;
struct CIGI_COLL_SEGMENT_NOTIFICATION;
struct CIGI_COLL_VOLUME_NOTIFICATION;

/////////////////////////////////////////////////////////////////////////////
// CEntityCollDetPropPage dialog

class CEntityCollDetPropPage : public CDialog
{
    // Construction
public:
    CEntityCollDetPropPage(CWnd *pParent = NULL);   // standard constructor
    virtual void OnOK();
    virtual void OnCancel();
    BOOL AddCollisionSegItem(CCollisionSeg *seg);
    BOOL RemoveCollisionSegItem(CCollisionSeg *seg);
    BOOL AddCollisionVolItem(CCollisionVol *vol);
    BOOL RemoveCollisionVolItem(CCollisionVol *vol);
    void AddCollisionDataItem(CIGI_COLL_SEGMENT_NOTIFICATION *packet);
    void AddCollisionDataItem(CIGI_COLL_VOLUME_NOTIFICATION *packet);
    void ClearCollisionData(void);
    CCollisionSeg *FindSegment(const int id);
    CCollisionVol *FindVolume(const int id);
    CCollisionSeg *GetSelSegment(void);
    CCollisionVol *GetSelVolume(void);
    void ClearSegsAndVols(void);
    void RefreshView(void);

    // Dialog Data
    //{{AFX_DATA(CEntityCollDetPropPage)
    enum { IDD = IDD_PROPPAGE_ENTITY_COLLDET };
    CStatic m_LabelZ1;
    CStatic m_LabelY1;
    CStatic m_LabelX1;
    CStatic m_LabelY2WidthUnits;
    CStatic m_LabelZ2DepthUnits;
    CStatic m_LabelZ2Depth;
    CStatic m_LabelYawUnits;
    CStatic m_LabelRollUnits;
    CStatic m_LabelPitchUnits;
    CStatic m_LabelY2Width;
    CStatic m_LabelX2HeightRadius;
    CStatic m_LabelStartOrType;
    CStatic m_LabelYaw;
    CStatic m_LabelRoll;
    CStatic m_LabelPitch;
    CStatic m_LabelMask;
    CStatic m_LabelEnd;
    CButton m_RadioSphere;
    CButton m_GroupBox;
    CEdit   m_EditZ2Depth;
    CEdit   m_EditY2Width;
    CEdit   m_EditX2HeightRadius;
    CEdit   m_EditZ1;
    CEdit   m_EditY1;
    CEdit   m_EditX1;
    CEdit   m_EditYaw;
    CEdit   m_EditRoll;
    CEdit   m_EditPitch;
    CEdit   m_EditMask;
    CComboBox   m_ComboID;
    CButton m_CheckEnableSegOrVol;
    CButton m_CheckEnableCD;
    CButton m_ButtonRemove;
    CString m_NotificationData;
    //}}AFX_DATA
    CButton m_RadioCuboid;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEntityCollDetPropPage)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    BOOL m_bHoldEnableCD;

    void EnableFields(BOOL enable = TRUE);
    void PopulateReqFields(CCollisionSeg *seg);
    void PopulateReqFields(CCollisionVol *vol);
    void PopulateRespFields(void);
    void SendCollisionSegDefToDriver(CCollisionSeg *seg);
    void SendCollisionVolDefToDriver(CCollisionVol *vol);

    // Generated message map functions
    //{{AFX_MSG(CEntityCollDetPropPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnRadioCuboid();
    afx_msg void OnRadioSphere();
    afx_msg void OnButtonAddSegment();
    afx_msg void OnButtonAddVolume();
    afx_msg void OnButtonRemoveColl();
    afx_msg void OnButtonApplyColl();
    afx_msg void OnSelchangeComboID();
    afx_msg void OnCheckCollEnable();
    afx_msg void OnCheckSegOrVolEnable();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline void CEntityCollDetPropPage::ClearCollisionData(void)
{
    m_NotificationData = "";
    UpdateData(FALSE);
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYCOLLDETPROPPAGE_H__F9E81A05_958F_400F_A458_BE8022D1B372__INCLUDED_)
