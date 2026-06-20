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

#include "windows.h"
#include "RTXCompatibility.h"


template<class TYPE> class RTXSharedObj
{
public:
    RTXSharedObj();
    ~RTXSharedObj();
    HANDLE Create(const char *name);
    const char *const GetInstanceName(void);
    void Set(const TYPE &value);
    TYPE Get(void);
    BOOL HasChanged(void);
    void ClearChangedFlag(void);

protected:
    BOOL *m_changed;
    TYPE *m_data;
    HANDLE m_mutexhdl;

    // Class to automatically wait on mutex and to
    // release it when the function returns.  This
    // class should be instantiated on the stack.
    class RTXSharedObjMutexLock
    {
    public:
        RTXSharedObjMutexLock() {
            m_hdl = NULL;
        };
        RTXSharedObjMutexLock(const HANDLE hdl) {
            m_hdl = hdl;
            RtWaitForSingleObject(hdl, INFINITE);

        };

        ~RTXSharedObjMutexLock() {
            RtReleaseMutex(m_hdl);
        };

    private:
        HANDLE m_hdl;
    };

private:
    const char *m_instancename;
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
    m_changed = NULL;
    m_data = NULL;
    m_mutexhdl = NULL;
    m_instancename = NULL;
    m_sharedhdl = NULL;
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
    // Wait for the mutex to be released.
    RtWaitForSingleObject(m_mutexhdl, INFINITE);

    // When all instances have released the handle, the shared memory is freed.
    RtCloseHandle(m_sharedhdl);

    // Release the mutex and close the handle.
    RtReleaseMutex(m_mutexhdl);
    RtCloseHandle(m_mutexhdl);
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
    m_instancename = (char *)malloc(strlen(name));
    strcpy((char *)m_instancename, name);

    char *sharedname = (char *)malloc(strlen(name) + strlen(".RTXSharedObj.sharedmem") + 1);
    strcpy((char *)sharedname, name);
    strcat((char *)sharedname, ".RTXSharedObj.sharedmem");

    // Create a shared memory region for all of the members that
    // would normally be declared as static.  If this memory exists,
    // the address is simply stored in m_dirty.
    int size = sizeof(TYPE) + sizeof(m_changed);
    m_sharedhdl = RtCreateSharedMemory(PAGE_READWRITE,           // protection level
                                       0,                      // size high byte
                                       size,                   // size low byte
                                       sharedname,             // name of buffer
                                       (void **)(&m_changed)); // pointer to buffer

    // Create a mutex.
    char *mutexname = (char *)malloc(strlen(name) + strlen(".RTXSharedObj.mutex") + 1);
    strcpy((char *)mutexname, name);
    strcat((char *)mutexname, ".RTXSharedObj.mutex");

    m_mutexhdl = RtCreateMutex(NULL, FALSE, mutexname);

    // Point m_data immediately past m_dirty.
    m_data = (TYPE *)((unsigned long)m_changed + sizeof(BOOL));

    // Default changed bit to FALSE.
    *m_changed = FALSE;

    // Delete the temporary shared names.
    free(sharedname);
    free(mutexname);

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
//          Sets the value of the shared object.  Returns a positive integer
//          if successful, or -1 if the object is blocked by another process.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> void RTXSharedObj<TYPE>::Set(const TYPE &value)
{
    RTXSharedObjMutexLock(m_mutexhdl);

    *m_data = value;
    *m_changed = TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Get
//
//  Description:
//          Gets the value of the shared object.  Returns a positive integer
//          if successful, or -1 if the object is blocked by another process.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> TYPE RTXSharedObj<TYPE>::Get(void)
{
    RTXSharedObjMutexLock(m_mutexhdl);

    return *m_data;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   HasChanged
//
//  Description:
//          Returns TRUE if the data has changed since the flag was reset.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> BOOL RTXSharedObj<TYPE>::HasChanged(void)
{
    RTXSharedObjMutexLock(m_mutexhdl);

    return *m_changed;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   ClearChangedFlag
//
//  Description:
//          Clears the changed flag.  The flag will be set when Set() is
//          called.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> void RTXSharedObj<TYPE>::ClearChangedFlag(void)
{
    RTXSharedObjMutexLock(m_mutexhdl);

    *m_changed = FALSE;
}

#endif // _RTXSHAREDOBJ_H_