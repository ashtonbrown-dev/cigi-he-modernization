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

#ifndef _SHAREDENTITYOBJ_H_
#define _SHAREDENTITYOBJ_H_

#include "common.h"
#include "stdlib.h"
#include "RTXSharedObj.h"

class CSharedEntityObj : public RTXSharedObj<ENTITY>
{
public:
    HANDLE Create(const char *name);
    HANDLE Create(void);
    virtual long SetLocked(const ENTITY &value);
    long SetLockedEntityPosition(const ENTITY &value);
    long SetLockedDofs(const DOF &value);
    long GetAndLockDofs(DOF *dest);
    long SetLockedRates(const RATES &value);
    long GetAndLockRates(RATES *dest);
    long SetLockedCigiData(const ENTITY_CIGI_DATA &value);
    long GetAndLockCigiData(ENTITY_CIGI_DATA *dest);
    long SetLockedNonCigiData(const ENTITY_NONCIGI_DATA &value);
    long GetAndLockNonCigiData(ENTITY_NONCIGI_DATA *dest);
    long SetLockedUnsavedData(const ENTITY_TEMP_DATA &value);
    long GetAndLockUnsavedData(ENTITY_TEMP_DATA *dest);
    long ClearLockedChangeFlag(void);

protected:
    static unsigned int m_InstanceCounter;  // Created in Hemu3\globals.cpp and HemuRtDrv\HemuRtDrv.cpp
};

inline HANDLE CSharedEntityObj::Create(const char *name)
{
    return RTXSharedObj<ENTITY>::Create(name);
}

inline HANDLE CSharedEntityObj::Create(void)
{
    char name[256];
    sprintf(name, "CSharedEntityObj%d", m_InstanceCounter++);
    return RTXSharedObj<ENTITY>::Create(name);
}

inline long CSharedEntityObj::SetLocked(const ENTITY &value)
{
    if (*m_locked) {
        *m_data = value;
        m_data->unsaved.entityctrl_has_changed = 1;
        return TRUE;
    } else
        return FALSE;
}

inline long CSharedEntityObj::SetLockedEntityPosition(const ENTITY &value)
{
    if (*m_locked) {
        *m_data = value;
        m_data->unsaved.entityposition_has_changed = 1;
        return TRUE;
    } else
        return FALSE;
}

inline long CSharedEntityObj::SetLockedDofs(const DOF &value)
{
    if (*m_locked) {
        m_data->unsaved.entityposition_has_changed = 1;
        m_data->cigi.dofs = value;
        return TRUE;
    } else
        return FALSE;
}

inline long CSharedEntityObj::GetAndLockDofs(DOF *dest)
{
    if (Lock()) {
        *dest = m_data->cigi.dofs;
        return TRUE;
    } else {
        return FALSE;
    }
}

inline long CSharedEntityObj::SetLockedRates(const RATES &value)
{
    if (*m_locked) {
        m_data->noncigi.rates = value;
        return TRUE;
    } else
        return FALSE;
}

inline long CSharedEntityObj::GetAndLockRates(RATES *dest)
{
    if (Lock()) {
        *dest = m_data->noncigi.rates;
        return TRUE;
    } else {
        return FALSE;
    }
}

inline long CSharedEntityObj::SetLockedCigiData(const ENTITY_CIGI_DATA &value)
{
    if (*m_locked) {
        m_data->unsaved.entityctrl_has_changed = 1;
        m_data->cigi = value;
        return TRUE;
    } else
        return FALSE;
}

inline long CSharedEntityObj::GetAndLockCigiData(ENTITY_CIGI_DATA *dest)
{
    if (Lock()) {
        *dest = m_data->cigi;
        return TRUE;
    } else {
        return FALSE;
    }
}

inline long CSharedEntityObj::SetLockedNonCigiData(const ENTITY_NONCIGI_DATA &value)
{
    if (*m_locked) {
        m_data->noncigi = value;
        return TRUE;
    } else
        return FALSE;
}

inline long CSharedEntityObj::GetAndLockNonCigiData(ENTITY_NONCIGI_DATA *dest)
{
    if (Lock()) {
        *dest = m_data->noncigi;
        return TRUE;
    } else {
        return FALSE;
    }
}

inline long CSharedEntityObj::SetLockedUnsavedData(const ENTITY_TEMP_DATA &value)
{
    if (*m_locked) {
        m_data->unsaved = value;
        return TRUE;
    } else
        return FALSE;
}

inline long CSharedEntityObj::GetAndLockUnsavedData(ENTITY_TEMP_DATA *dest)
{
    if (Lock()) {
        *dest = m_data->unsaved;
        return TRUE;
    } else {
        return FALSE;
    }
}

inline long CSharedEntityObj::ClearLockedChangeFlag(void)
{
    if (*m_locked) {
        m_data->unsaved.entityctrl_has_changed = 0;
        return TRUE;
    } else
        return FALSE;
}

#endif