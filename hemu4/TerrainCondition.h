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

// TerrainCondition.h: interface for the CTerrainCondition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TERRAINCONDITION_H__18BDD3F8_1823_40C5_B619_B01D8CF03C0F__INCLUDED_)
#define AFX_TERRAINCONDITION_H__18BDD3F8_1823_40C5_B619_B01D8CF03C0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "ConfigTempl.h"

class CTerrainCondition : public CObject
{
    DECLARE_SERIAL(CTerrainCondition)

public:
    CTerrainCondition(const int ID);
    virtual ~CTerrainCondition();
    int GetID();
    void SetHtree(const HTREEITEM htree);
    HTREEITEM GetHtree(void);

    virtual void Serialize(CArchive &ar);

protected:
    CTerrainCondition();    // for serialization

    CString m_Name;
    int m_ID;
    int m_Scope;
    int m_Severity;
    int m_Coverage;
    BOOL m_Enable;
    HTREEITEM m_Htree;
};

inline int CTerrainCondition::GetID(void)
{
    return m_ID;
}

inline void CTerrainCondition::SetHtree(const HTREEITEM htree)
{
    m_Htree = htree;
}

inline HTREEITEM CTerrainCondition::GetHtree(void)
{
    return m_Htree;
}

typedef CMap<int, int, CTerrainCondition *, CTerrainCondition *&> CTerrainConditionMap;

#endif // !defined(AFX_TERRAINCONDITION_H__18BDD3F8_1823_40C5_B619_B01D8CF03C0F__INCLUDED_)
