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
//  SHAREDOBJECT.H
//
//  Created:    7/18/01
//
//  Author:     Lance Durham
//
//  Description:    Template class that stores an object in Windows shared
//                  memory so it can be used by the GUI and driver processes.
//
//                  *   This class does not use any synchronization objects
//                      to control data access.
//
//                  *   This class does not use MFC.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef SHARED_OBJECT_H
#define SHARED_OBJECT_H

#include "StdAfx.h"
#include <crtdbg.h>
#include "WindowsCompatibility.h"
#include "cas.h"

template<class TYPE> class SharedObject
{
public:
    enum {MAX_NAME_LENGTH = 64};

    SharedObject();
    ~SharedObject();
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
//  Name:   SharedObject
//
//  Description:
//          Standard constructor.  Does NOT allocate memory for the object.
//          The program must call Create() to do this.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> SharedObject<TYPE>::SharedObject()
{
    m_locked = NULL;
    m_data = NULL;
    m_sharedhdl = NULL;

    memset(m_instancename, 0, MAX_NAME_LENGTH);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   ~SharedObject
//
//  Description:
//          Standard destructor.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> SharedObject<TYPE>::~SharedObject()
{
    // When all instances have released the handle, the shared memory is freed.
    if (m_locked)
        UnmapViewOfFile(m_locked);
    if (m_sharedhdl)
        CloseHandle(m_sharedhdl);
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
template <class TYPE> HANDLE SharedObject<TYPE>::Create(const char *name)
{
    // First, copy the name to shared memory.  We will derive the buffer
    // name from this.
    strncpy((char *)m_instancename, name, MAX_NAME_LENGTH);

    char *sharedname = (char *)malloc(strlen(name) + strlen(".SharedObject.sharedmem") + 1);
    strcpy((char *)sharedname, name);
    strcat((char *)sharedname, ".SharedObject.sharedmem");

    // Create a shared memory region for all of the members that
    // would normally be declared as static.  If this memory exists,
    // the address is simply stored in m_dirty.
    int size = sizeof(TYPE) + sizeof(m_locked);
    m_sharedhdl = CreateSharedMemory(PAGE_READWRITE,           // protection level
                                       0,                      // size high byte
                                       size,                   // size low byte
                                       sharedname,             // name of buffer
                                       (void **)(&m_locked));  // pointer to buffer

    if (!m_sharedhdl) {
        free(sharedname);
        return NULL;
    }

    const DWORD creationStatus = GetLastError();

    // Point m_data immediately past the lock flag.
    m_data = reinterpret_cast<TYPE *>(reinterpret_cast<BYTE *>(m_locked) + sizeof(long));

    if (creationStatus != ERROR_ALREADY_EXISTS)
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
template <class TYPE> const char   *const SharedObject<TYPE>::GetInstanceName(void)
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
template <class TYPE> long SharedObject<TYPE>::SetLocked(const TYPE &value)
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
template <class TYPE> long SharedObject<TYPE>::GetAndLock(TYPE *dest)
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
template <class TYPE> inline long SharedObject<TYPE>::Unlock(void)
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
template <class TYPE> inline long SharedObject<TYPE>::Lock(void)
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
template <class TYPE> inline long SharedObject<TYPE>::IsLocked(void)
{
    return *m_locked;
}

#endif // SHARED_OBJECT_H
