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
 *  FILENAME:   CollisionSeg.cpp
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
#include "hemu4.h"
#include "CollisionSeg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCollisionSeg::CCollisionSeg()
{
    SetEnable(FALSE);
}

CCollisionSeg::CCollisionSeg(const int id_base, const int owner_id)
    : CMissionFuncObj<COLLISION_SEG, MF_DUMMY_RESP>(id_base, owner_id)
{
    m_Name.Format("Segment %d", id_base);
    SetEnable(FALSE);
}

CCollisionSeg::CCollisionSeg(TEMPL_COLLISION_SEG *templ)
    : CMissionFuncObj<COLLISION_SEG, MF_DUMMY_RESP>(templ->ID, -1)
{
    m_Name = templ->Name;
    SetEnable(templ->Enabled ? TRUE : FALSE);
    SetMaterialMask(templ->MaterialMask);
    SetStartX((float)templ->X1);
    SetStartY((float)templ->Y1);
    SetStartZ((float)templ->Z1);
    SetEndX((float)templ->X2);
    SetEndY((float)templ->Y2);
    SetEndZ((float)templ->Z2);
}

CCollisionSeg::~CCollisionSeg()
{
}

void CCollisionSeg::ClearNotifications(void)
{
    COLLISION_SEG_NOTIFICATION *resp = GetFirstNotification();
    while (resp)
        delete resp;

    m_NotificationList.RemoveAll();
}

template <> void AFXAPI SerializeElements<CCollisionSeg *>(CArchive &ar, CCollisionSeg **pElements, int nCount)
{
    if (ar.IsStoring()) {
        for (int i = 0; i < nCount; i++) {
            if (pElements[i])
                pElements[i]->Serialize(ar);
        }
    } else {
        for (int i = 0; i < nCount; i++) {
            pElements[i] = new CCollisionSeg(0, 0);
            pElements[i]->Serialize(ar);
        }
    }
}
