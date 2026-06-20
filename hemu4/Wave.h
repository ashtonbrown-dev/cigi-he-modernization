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

// Wave.h: interface for the CWave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVE_H__936D4E4D_AB26_4A7E_8B5B_CF1946BE87DB__INCLUDED_)
#define AFX_WAVE_H__936D4E4D_AB26_4A7E_8B5B_CF1946BE87DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "ConfigTempl.h"

class CWave : public CObject
{
    DECLARE_SERIAL(CWave)

public:
    CWave(const int id);
    virtual ~CWave();
    int GetID(void);
    void SetHtree(const HTREEITEM htree);
    HTREEITEM GetHtree(void);

    virtual void Serialize(CArchive &ar);

protected:
    CWave();

    int m_ID;
    HTREEITEM m_Htree;
};

inline int CWave::GetID(void)
{
    return m_ID;
}

inline void CWave::SetHtree(const HTREEITEM htree)
{
    m_Htree = htree;
}

inline HTREEITEM CWave::GetHtree(void)
{
    return m_Htree;
}

typedef CMap<int, int, CWave *, CWave *&> CWaveMap;
template <> void AFXAPI SerializeElements<CWave *>(CArchive &ar, CWave **pElements, int nCount);

#endif // !defined(AFX_WAVE_H__936D4E4D_AB26_4A7E_8B5B_CF1946BE87DB__INCLUDED_)
