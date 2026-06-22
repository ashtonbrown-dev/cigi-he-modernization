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

#ifndef _SHAREDVIEWGROUPOBJ_H_
#define _SHAREDVIEWGROUPOBJ_H_

#include "common.h"
#include "stdlib.h"
#include "SharedObject.h"

class CSharedViewGroupObj : public SharedObject<VIEWGROUP>
{
public:
    HANDLE Create(const char *name);
    HANDLE Create(void);
    virtual long SetLocked(const VIEWGROUP &value);
    long SetLockedCigi(const VIEWGROUP_CIGI_DATA &value);
    long GetAndLockCigi(VIEWGROUP_CIGI_DATA *dest);
    long SetLockedNonCigi(const VIEWGROUP_NONCIGI_DATA &value);
    long GetAndLockNonCigi(VIEWGROUP_NONCIGI_DATA *dest);
    long ClearLockedChangeFlag(void);

protected:
    static unsigned int m_InstanceCounter;  // Created in Hemu3\globals.cpp and HemuDrv\HemuDrv.cpp
};

inline HANDLE CSharedViewGroupObj::Create(const char *name)
{
    return SharedObject<VIEWGROUP>::Create(name);
}

inline HANDLE CSharedViewGroupObj::Create(void)
{
    char name[256];
    sprintf(name, "CSharedViewGroupObj%d", m_InstanceCounter++);
    return SharedObject<VIEWGROUP>::Create(name);
}

inline long CSharedViewGroupObj::SetLocked(const VIEWGROUP &value)
{
    if (*m_locked) {
        *m_data = value;
        m_data->noncigi.cigi_viewgroup_has_changed = 1;
        return TRUE;
    } else
        return FALSE;
}

inline long CSharedViewGroupObj::SetLockedCigi(const VIEWGROUP_CIGI_DATA &value)
{
    if (*m_locked) {
        m_data->noncigi.cigi_viewgroup_has_changed = 1;
        m_data->cigi = value;
        return TRUE;
    } else
        return FALSE;
}

inline long CSharedViewGroupObj::GetAndLockCigi(VIEWGROUP_CIGI_DATA *dest)
{
    if (Lock()) {
        *dest = m_data->cigi;
        return TRUE;
    } else {
        return FALSE;
    }
}

inline long CSharedViewGroupObj::SetLockedNonCigi(const VIEWGROUP_NONCIGI_DATA &value)
{
    if (*m_locked) {
        m_data->noncigi = value;
        return TRUE;
    } else
        return FALSE;
}

inline long CSharedViewGroupObj::GetAndLockNonCigi(VIEWGROUP_NONCIGI_DATA *dest)
{
    if (Lock()) {
        *dest = m_data->noncigi;
        return TRUE;
    } else {
        return FALSE;
    }
}

inline long CSharedViewGroupObj::ClearLockedChangeFlag(void)
{
    if (*m_locked) {
        m_data->noncigi.cigi_viewgroup_has_changed = 0;
        return TRUE;
    } else
        return FALSE;
}

#endif // _SHAREDVIEWGROUPOBJ_H_
