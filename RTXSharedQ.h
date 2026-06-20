///////////////////////////////////////////////////////////////////////////////
//
//  RTXSHAREDQ.H
//
//  Created:    7/18/01
//
//  Author:     Lance Durham
//
//  Description:    Template class that implements a queue that can be used
//                  by either Windows or RTX processes.  The buffer will
//                  reside in non-paged kernel memory and can be shared by
//                  multiple processes.  This class does not use MFC.
//
//                  This class is similar to CTSQueue, except for the
//                  following changes:
//
//                      *   The MFC CCriticalSection object has been replaced
//                          with an RTX mutex.
//                      *   All new and delete operators have been replaced
//                          with calls to the appropriate RTX allocation and
//                          deallocation functions.
//                      *   The Create() member function has been added to
//                          allocate or connect to a named shared memory
//                          buffer.
//                      *   Because memory allocation is not deterministic,
//                          SetSize(), SetGrowBy(), and GetGrowBy() have been
//                          dropped.
//                      *   Push() and Pop() no longer return the number of
//                          elements in the queue.  Both functions now return
//                          non-zero if they return successfully, or zero if
//                          they fail.
//                      *   All data members are now stored in shared memory.
//                          All member variables have been replaced by pointers
//                          to the shared memory region.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _RTXSHAREDQ_H_
#define _RTXSHAREDQ_H_

#include "windows.h"
#include "RTXCompatibility.h"

#define     TIMEOUT_MILLISEC    1

template<class TYPE> class RTXSharedQ
{
public:
    RTXSharedQ();
    ~RTXSharedQ();
    long int Create(const char *name, const long size);
    long int GetSize(void);
    long int GetItemCount(void);
    long int Push(const TYPE value);
    long int Pop(TYPE *value);

private:
    TYPE *m_data;
    long int *m_size;
    long int *m_bottom;
    long int *m_top;
    long int *m_itemcount;
    const char *m_instancename;
    HANDLE m_mutex;
    HANDLE m_sharedhdl;

protected:
};


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   RTXSharedQ
//
//  Description:
//          Standard constructor.  Does NOT allocate memory for the array.
//          The program must call Create() to do this.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> RTXSharedQ<TYPE>::RTXSharedQ()
{
    m_data = NULL;
    m_size = NULL;
    m_bottom = NULL;
    m_top = NULL;
    m_itemcount = NULL;
    m_instancename = NULL;
    m_mutex = 0;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   ~RTXSharedQ
//
//  Description:
//          Standard destructor.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> RTXSharedQ<TYPE>::~RTXSharedQ()
{
    // Free the instance-specific memory.
#ifndef _DEBUG
    free(m_instancename);
#endif

    // All we have to do is close the handles.  When all objects have
    // released the handles, the mutex is deleted and the shared memory
    // is freed.
    RtCloseHandle(m_sharedhdl);
    RtReleaseMutex(m_mutex);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Connect
//
//  Description:
//          Allocates a named shared buffer of the specified size if one does
//          not exist; otherwise, creates a handle to the existing buffer.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> long int RTXSharedQ<TYPE>::Create(const char *name, const long size)
{
    // First, copy the name to shared memory.  We will get the buffer, mutex,
    // and other names from this.
    m_instancename = (char *)malloc(strlen(name) + 1);
    strcpy((char *)m_instancename, name);

    char *sharedname = (char *)malloc(strlen(name) + strlen(".RTXSharedQ.sharedmem") + 1);
    strcpy((char *)sharedname, name);
    strcat((char *)sharedname, ".RTXSharedQ.sharedmem");

    char *mutexname = (char *)malloc(strlen(name) + strlen(".RTXSharedQ.mutex") + 1);
    strcpy((char *)mutexname, name);
    strcat((char *)mutexname, ".RTXSharedQ.mutex");


    // Create a shared memory region for all of the members that
    // would normally be declared as static.  If this memory exists,
    //
    long sz = (size * sizeof(TYPE)) + (5 * sizeof(long));
    m_sharedhdl = RtCreateSharedMemory(PAGE_READWRITE,           // protection level
                                       0,                      // size high byte
                                       sz,                     // size low byte
                                       sharedname,             // name of buffer
                                       (void **)(&m_data));    // pointer to buffer
    if (m_sharedhdl == NULL)
        return 0;

    DWORD status = GetLastError();

    // Set the pointers to the shared memory at the proper offsets.
    m_size = (long *)((char *)(m_data + size));
    m_bottom = (long *)((char *)m_size + sizeof(long));
    m_top = (long *)((char *)m_bottom + sizeof(long));
    m_itemcount = (long *)((char *)m_top + sizeof(long));

    // If the shared memory block did not previously exist, we should
    // initialize the variables.
    if (status != ERROR_ALREADY_EXISTS) {
        *m_size = size;
        *m_bottom = 0;
        *m_top = 0;
        *m_itemcount = 0;
    }

    // Create the mutex object (or get a handle to the existing one).
    m_mutex = RtCreateMutex(NULL, FALSE, mutexname);

    // Delete the temporary strings we allocated.
    free(mutexname);
    free(sharedname);

    return 1;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetSize
//
//  Description:
//          Returns the size of the internal array.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> long int RTXSharedQ<TYPE>::GetSize(void)
{
    return *m_size;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetItemCount
//
//  Description:
//          Returns the number of elements in the queue.  This value might
//          be changed at any time by another process, so the value returned
//          should be assumed to be inaccurate.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> long int RTXSharedQ<TYPE>::GetItemCount(void)
{
    return *m_itemcount;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Push
//
//  Description:
//          Enqueues a new item to the bottom of the queue.  Returns a positive
//          integer if successful, or one of the following values if failed:
//
//              -1  The queue has been blocked by another process.
//              -2  Create() has not been called, or the queue failed to
//                  initialize.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> long int RTXSharedQ<TYPE>::Push(const TYPE value)
{
    if (*m_size == 0)
        return -2;

    if (RtWaitForSingleObject(m_mutex, TIMEOUT_MILLISEC) == WAIT_TIMEOUT)
        return -1;

    m_data[*m_top] = value;

    (*m_itemcount)++;
    (*m_top)++;

    // Wrap around to the bottom if we go past the top
    if (*m_top == *m_size)
        *m_top = 0;

    RtReleaseMutex(m_mutex);

    return 1;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Pop
//
//  Description:
//          Dequeues the next item from the top of the queue.  Returns a
//          positive integer if successful, or one of the following values if
//          failed:
//
//              0   No items on the queue.
//              -1  The queue has been blocked by another process.
//              -2  Create() has not been called, or the queue failed to
//                  initialize.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> long int RTXSharedQ<TYPE>::Pop(TYPE *value)
{
    if (*m_itemcount == 0)
        return 0;

    if (!m_data)
        return -2;

    if (RtWaitForSingleObject(m_mutex, TIMEOUT_MILLISEC) == WAIT_TIMEOUT)
        return -1;

    *value = m_data[*m_bottom];

    (*m_itemcount)--;
    (*m_bottom)++;

    // Wrap around the the bottom if necessary
    if (*m_bottom == *m_size)
        *m_bottom = 0;

    RtReleaseMutex(m_mutex);

    return 1;
}

#endif // _RTXSHAREDQ_H_