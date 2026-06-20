/** <pre>
 *  Copyright 2001 The Boeing Company
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

///////////////////////////////////////////////////////////////////////////////
//
//  RTXSHAREDOBJ.H
//
//  Created:    7/18/01
//
//  Author:     Lance Durham
//
//  Description:    Template class that encapsulates an object accessible
//                  by either Windows or RTX processes.  The object will
//                  reside in non-paged kernel memory and can be shared by
//                  multiple processes.
//
//                  *   This class does not use any synchronization objects
//                      to control data access.
//
//                  *   This class does not use MFC.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _RTXSHAREDOBJ_H_
#define _RTXSHAREDOBJ_H_

#include "StdAfx.h"
#include <crtdbg.h>
#include "RTXCompatibility.h"
#include "cas.h"

template<class TYPE> class RTXSharedObj
{
public:
    enum {MAX_NAME_LENGTH = 64};

    RTXSharedObj();
    ~RTXSharedObj();
    HANDLE Create(const char *name);
    const char *const GetInstanceName(void);
    virtual long SetLocked(const TYPE &value);
    long GetAndLock(TYPE *dest);
    long Unlock(void);
    long IsLocked(void);

protected:
    long *volatile m_locked;
    TYPE *volatile m_data;

    long Lock(void);

private:
    char m_instancename[MAX_NAME_LENGTH];
    HANDLE m_sharedhdl;
};


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   RTXSharedObj
//
//  Description:
//          Standard constructor.  Does NOT allocate memory for the object.
//          The program must call Create() to do this.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> RTXSharedObj<TYPE>::RTXSharedObj()
{
    m_locked = NULL;
    m_data = NULL;
    m_sharedhdl = NULL;

    memset(m_instancename, 0, MAX_NAME_LENGTH);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   ~RTXSharedObj
//
//  Description:
//          Standard destructor.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> RTXSharedObj<TYPE>::~RTXSharedObj()
{
    // When all instances have released the handle, the shared memory is freed.
    RtCloseHandle(m_sharedhdl);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Create
//
//  Description:
//          Allocates a named shared buffer of the specified size if one does
//          not exist; otherwise, creates a handle to the existing buffer.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> HANDLE RTXSharedObj<TYPE>::Create(const char *name)
{
    // First, copy the name to shared memory.  We will derive the buffer
    // name from this.
    strncpy((char *)m_instancename, name, MAX_NAME_LENGTH);

    char *sharedname = (char *)malloc(strlen(name) + strlen(".RTXSharedObj.sharedmem") + 1);
    strcpy((char *)sharedname, name);
    strcat((char *)sharedname, ".RTXSharedObj.sharedmem");

    // Create a shared memory region for all of the members that
    // would normally be declared as static.  If this memory exists,
    // the address is simply stored in m_dirty.
    int size = sizeof(TYPE) + sizeof(m_locked);
    m_sharedhdl = RtCreateSharedMemory(PAGE_READWRITE,           // protection level
                                       0,                      // size high byte
                                       size,                   // size low byte
                                       sharedname,             // name of buffer
                                       (void **)(&m_locked));  // pointer to buffer

    // Point m_data immediately past m_dirty.
    m_data = (TYPE *)((unsigned long)m_locked + sizeof(BOOL));

    // Default to unlocked.
    *m_locked = FALSE;

    // Delete the temporary shared name.
    free(sharedname);

    return m_sharedhdl;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetInstanceName
//
//  Description:
//          Returns the shared instance name.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> const char   *const RTXSharedObj<TYPE>::GetInstanceName(void)
{
    return m_instancename;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Set
//
//  Description:
//          Sets the value of the shared object.  Does not unlock the object.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> long RTXSharedObj<TYPE>::SetLocked(const TYPE &value)
{
    if (*m_locked) {
        *m_data = value;
        return TRUE;
    } else
        return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetAndLock
//
//  Description:
//          Gets the value of the shared object and locks the object.  Returns
//          FALSE if the object has already been locked.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> long RTXSharedObj<TYPE>::GetAndLock(TYPE *dest)
{
    if (Lock()) {
        *dest = *m_data;
        return TRUE;
    } else {
        return FALSE;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Unlock
//
//  Description:
//          Clears the lock flag in an atomic operation. Returns TRUE if the
//          unlock was successful. Returns FALSE if not locked.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> inline long RTXSharedObj<TYPE>::Unlock(void)
{
    return CAS(m_locked, 1, 0);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Lock
//
//  Description:
//          Sets the lock flag in an atomic operation. Returns TRUE if the lock
//          was successful. Returns FALSE if already locked.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> inline long RTXSharedObj<TYPE>::Lock(void)
{
    return CAS(m_locked, 0, 1);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   IsLocked
//
//  Description:
//          Returns TRUE is the object is locked; otherwise, returns FALSE.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> inline long RTXSharedObj<TYPE>::IsLocked(void)
{
    return *m_locked;
}

#endif // _RTXSHAREDOBJ_H_