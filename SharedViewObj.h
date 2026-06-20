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

#ifndef _SHAREDVIEWOBJ_H_
#define _SHAREDVIEWOBJ_H_

#include "common.h"
#include "stdlib.h"
#include "RTXSharedObj.h"

class CSharedViewObj : public RTXSharedObj<VIEW>
{
public:
    HANDLE Create(const char *name);
    HANDLE Create(void);
    virtual long SetLocked(const VIEW &value);
    long SetLockedViewDef(const VIEW_DEF_DATA &value);
    long GetAndLockViewDef(VIEW_DEF_DATA *dest);
    long SetLockedViewCtrl(const VIEW_CONTROL_DATA &value);
    long GetAndLockViewCtrl(VIEW_CONTROL_DATA *dest);
    long SetLockedNonCigi(const VIEW_NONCIGI_DATA &value);
    long GetAndLockNonCigi(VIEW_NONCIGI_DATA *dest);
    long ClearLockedChangeFlags(void);

protected:
    static unsigned int m_InstanceCounter;  // Created in Hemu3\globals.cpp and HemuRtDrv\HemuRtDrv.cpp
};

inline HANDLE CSharedViewObj::Create(const char *name)
{
    return RTXSharedObj<VIEW>::Create(name);
}

inline HANDLE CSharedViewObj::Create(void)
{
    char name[256];
    sprintf(name, "CSharedViewObj%d", m_InstanceCounter++);
    return RTXSharedObj<VIEW>::Create(name);
}

inline long CSharedViewObj::SetLocked(const VIEW &value)
{
    if (*m_locked) {
        *m_data = value;
        m_data->noncigi.def_has_changed = 1;
        m_data->noncigi.ctrl_has_changed = 1;
        return TRUE;
    } else
        return FALSE;
}

inline long CSharedViewObj::SetLockedViewDef(const VIEW_DEF_DATA &value)
{
    if (*m_locked) {
        m_data->noncigi.def_has_changed = 1;
        m_data->viewdef = value;
        return TRUE;
    } else
        return FALSE;
}

inline long CSharedViewObj::GetAndLockViewDef(VIEW_DEF_DATA *dest)
{
    if (Lock()) {
        *dest = m_data->viewdef;
        return TRUE;
    } else {
        return FALSE;
    }
}

inline long CSharedViewObj::SetLockedViewCtrl(const VIEW_CONTROL_DATA &value)
{
    if (*m_locked) {
        m_data->noncigi.ctrl_has_changed = 1;
        m_data->viewctrl = value;
        return TRUE;
    } else
        return FALSE;
}

inline long CSharedViewObj::GetAndLockViewCtrl(VIEW_CONTROL_DATA *dest)
{
    if (Lock()) {
        *dest = m_data->viewctrl;
        return TRUE;
    } else {
        return FALSE;
    }
}

inline long CSharedViewObj::SetLockedNonCigi(const VIEW_NONCIGI_DATA &value)
{
    if (*m_locked) {
        m_data->noncigi = value;
        return TRUE;
    } else
        return FALSE;
}

inline long CSharedViewObj::GetAndLockNonCigi(VIEW_NONCIGI_DATA *dest)
{
    if (Lock()) {
        *dest = m_data->noncigi;
        return TRUE;
    } else {
        return FALSE;
    }
}

inline long CSharedViewObj::ClearLockedChangeFlags(void)
{
    if (*m_locked) {
        m_data->noncigi.def_has_changed = 0;
        m_data->noncigi.ctrl_has_changed = 0;
        return TRUE;
    } else
        return FALSE;
}

#endif // _SHAREDVIEWOBJ_H_