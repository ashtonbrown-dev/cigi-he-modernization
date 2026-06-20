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

// IDManager.h: interface for the CIDManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDMANAGER_H__1D96781C_B350_4EA4_B044_66D48EBA8967__INCLUDED_)
#define AFX_IDMANAGER_H__1D96781C_B350_4EA4_B044_66D48EBA8967__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIDManager : public CObject
{
    DECLARE_SERIAL(CIDManager)

public:
    CIDManager();
    CIDManager(const int initial_id);
    virtual ~CIDManager();
    virtual void Serialize(CArchive &ar);
    void SetInitialID(const int initial_id);
    int GetNextID(void);
    void MarkID(const int id);
    void UnmarkID(const int id);

protected:
    int m_InitialID;
    CByteArray m_IDArray;
};

inline void CIDManager::MarkID(const int id)
{
    m_IDArray.SetAtGrow(id, 1);
}

inline void CIDManager::UnmarkID(const int id)
{
    m_IDArray.SetAtGrow(id, 0);
}

#endif // !defined(AFX_IDMANAGER_H__1D96781C_B350_4EA4_B044_66D48EBA8967__INCLUDED_)
