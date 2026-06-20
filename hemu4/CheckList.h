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

// CheckList.h: interface for the CCheckList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKLIST_H__5C6F54E2_9557_448C_8621_59BA7C40BA7B__INCLUDED_)
#define AFX_CHECKLIST_H__5C6F54E2_9557_448C_8621_59BA7C40BA7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RadioList.h"

class CCheckList : public CRadioList
{
public:
    CCheckList();
    virtual ~CCheckList();
    virtual void Init(void);
    int AddItem(LPCTSTR text);
    int GetItemCheck(const int item);
    void SetItemCheck(const int item, const int state);

protected:
    //{{AFX_MSG(CCheckList)
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline int CCheckList::GetItemCheck(const int item)
{
    if (item >= m_BtnCount)
        return -1;

    return m_BtnArray[item]->GetCheck();
}

inline void CCheckList::SetItemCheck(const int item, const int state)
{

    if (item >= m_BtnCount)
        return;

    m_BtnArray[item]->SetCheck(state);
}

#endif // !defined(AFX_CHECKLIST_H__5C6F54E2_9557_448C_8621_59BA7C40BA7B__INCLUDED_)
