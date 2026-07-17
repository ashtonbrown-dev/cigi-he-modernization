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

#include "SharedBufferQueue.h"
#include "stdio.h"

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   SharedBufferQueue
//
//  Description:
//          Standard constructor.  Does NOT allocate memory for the array.
//          The program must call Create() to do this.
//
///////////////////////////////////////////////////////////////////////////////
SharedBufferQueue::SharedBufferQueue()
{
    m_bufflengths = NULL;
    m_size = NULL;
    m_bottom = NULL;
    m_top = NULL;
    m_itemcount = NULL;
    m_instancename = NULL;
    m_data = NULL;
    m_mutex = 0;
    m_sharedhdl = 0;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   ~SharedBufferQueue
//
//  Description:
//          Standard destructor.
//
///////////////////////////////////////////////////////////////////////////////
SharedBufferQueue::~SharedBufferQueue()
{
    free(m_instancename);
    free(m_data);

    if (m_bufflengths)
        UnmapViewOfFile(m_bufflengths);
    if (m_sharedhdl)
        CloseHandle(m_sharedhdl);
    if (m_mutex)
        CloseHandle(m_mutex);
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
long int SharedBufferQueue::Create(const char *name, const long buffcount, const long buffsize)
{
    // First, copy the name to shared memory.  We will get the buffer, mutex,
    // and other names from this.
    m_instancename = (char *)malloc(strlen(name) + 1);
    strcpy((char *)m_instancename, name);

    char *sharedname = (char *)malloc(strlen(name) + strlen(".SharedBufferQueue.sharedmem") + 1);
    strcpy((char *)sharedname, name);
    strcat((char *)sharedname, ".SharedBufferQueue.sharedmem");

    char *mutexname = (char *)malloc(strlen(name) + strlen(".SharedBufferQueue.mutex") + 1);
    strcpy((char *)mutexname, name);
    strcat((char *)mutexname, ".SharedBufferQueue.mutex");


    // Create a shared memory region for all of the members that
    // would normally be declared as static.  If this memory exists,
    // we will get a handle to it.
    long sz = (buffcount * sizeof(long))                // for array of buffer lengths
              + (buffcount * buffsize * sizeof(char))     // for actual buffers
              + (4 * sizeof(long));                       // for other variables
    m_sharedhdl = CreateSharedMemory(PAGE_READWRITE,           // protection level
                                       0,                      // size high byte
                                       sz,                     // size low byte
                                       sharedname,             // name of buffer
                                       (void **)(&m_bufflengths));     // pointer to start of struct
    if (m_sharedhdl == NULL)
        return 0;

    DWORD status = GetLastError();

    // Set the pointers to the shared memory at the proper offsets.
    m_size = (long *)(m_bufflengths + buffcount);
    m_bottom = (long *)((char *)m_size + sizeof(long));
    m_top = (long *)((char *)m_bottom + sizeof(long));
    m_itemcount = (long *)((char *)m_top + sizeof(long));

    // Create our instance-specific pointer array.
    m_data = (char **)malloc(buffcount * sizeof(char *));

    // Finally, set the pointers to each of the buffers.  These will be at
    // the end of all of the other variables, so we can keep all of the other
    // variables together to try to decrease our cache misses.
    long i;
    char *base = (char *)((char *)m_itemcount + sizeof(long));
    for (i = 0; i < buffcount; i++) {
        m_data[i] = base + (buffsize * i);

        // Set the length only if the memory block did not previously exist.
        if (status != ERROR_ALREADY_EXISTS)
            m_bufflengths[i] = 0;
    }

    // If the shared memory block did not previously exist, we should
    // initialize the rest of the variables.
    if (status != ERROR_ALREADY_EXISTS) {
        *m_size = buffcount;
        *m_bottom = 0;
        *m_top = 0;
        *m_itemcount = 0;
    }

    // Create the mutex object (or get a handle to the existing one).
    m_mutex = CreateMutex(NULL, FALSE, mutexname);

    // Delete the temporary strings we allocated.
    free(mutexname);
    free(sharedname);

    return 1;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Push
//
//  Description:
//          Enqueues a new item to the bottom of the queue.  Returns the number
//          of items on the queue if successful, or one of the following values
//          if failed:
//
//              -1  The queue has been blocked by another process.
//              -2  Create() has not been called, or the queue failed to
//                  initialize.
//              -3  The queue is full.
//
///////////////////////////////////////////////////////////////////////////////
long int SharedBufferQueue::Push(const char *buffer, const long length)
{
    if (*m_size == 0)
        return -2;

    int waitval = WaitForSingleObject(m_mutex, TIMEOUT_MILLISEC);
    if (waitval != WAIT_OBJECT_0)
        return -1;

    if (*m_itemcount >= *m_size) {
        ReleaseMutex(m_mutex);
        return -3;
    }

    memcpy(m_data[*m_top], buffer, length);
    m_bufflengths[*m_top] = length;

    (*m_itemcount)++;
    (*m_top)++;

    // Wrap around to the bottom if we go past the top
    if (*m_top == *m_size)
        *m_top = 0;

    ReleaseMutex(m_mutex);

    return *m_itemcount;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   PushPair
//
//  Description:
//          Atomically enqueues two related buffers. This prevents a CIGI
//          message and its timing record from becoming separated when the
//          queue reaches capacity.
//
///////////////////////////////////////////////////////////////////////////////
long int SharedBufferQueue::PushPair(const char *firstBuffer,
                                     const long firstLength,
                                     const char *secondBuffer,
                                     const long secondLength)
{
    if (*m_size == 0)
        return -2;

    if (WaitForSingleObject(m_mutex, TIMEOUT_MILLISEC) != WAIT_OBJECT_0)
        return -1;

    if (*m_size < 2 || *m_itemcount > *m_size - 2) {
        ReleaseMutex(m_mutex);
        return -3;
    }

    memcpy(m_data[*m_top], firstBuffer, firstLength);
    m_bufflengths[*m_top] = firstLength;
    (*m_top)++;
    if (*m_top == *m_size)
        *m_top = 0;

    memcpy(m_data[*m_top], secondBuffer, secondLength);
    m_bufflengths[*m_top] = secondLength;
    (*m_top)++;
    if (*m_top == *m_size)
        *m_top = 0;

    *m_itemcount += 2;

    ReleaseMutex(m_mutex);
    return *m_itemcount;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Pop
//
//  Description:
//          Dequeues the next item from the top of the queue.  Returns the
//          length of the buffer if successful, or one of the following values
//          if failed:
//
//              0   No items on the queue.
//              -1  The queue has been blocked by another process.
//              -2  Create() has not been called, or the queue failed to
//                  initialize.
//
///////////////////////////////////////////////////////////////////////////////
long int SharedBufferQueue::Pop(char *buffer, const long length)
{
    static int copysize;
    static int actualsize;

    if (*m_itemcount == 0)
        return 0;

    if (!m_bufflengths)
        return -2;

    if (WaitForSingleObject(m_mutex, TIMEOUT_MILLISEC) != WAIT_OBJECT_0)
        return -1;

    // Set the buffer size to the shorter of the actual buffer size or
    // the length of the destination buffer.
    actualsize = m_bufflengths[*m_bottom];
    copysize = ((actualsize < length) ? actualsize : length);

    memcpy(buffer, m_data[*m_bottom], copysize);

    // Set the size of the queue element.
    m_bufflengths[*m_bottom] = 0;

    (*m_itemcount)--;
    (*m_bottom)++;

    // Wrap around the the bottom if necessary
    if (*m_bottom == *m_size)
        *m_bottom = 0;

    ReleaseMutex(m_mutex);

    return copysize;
}
