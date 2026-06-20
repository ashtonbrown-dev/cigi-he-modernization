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
//  TSQUEUE.H
//
//  Created:    7/18/01
//
//  Author:     Lance Durham
//
//  Description:    Thread-safe template class that implements a simple queue.
//                  This class uses MFC's CCriticalSection class and is
//                  derived from the CObject class.
//
///////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>
#include <afxmt.h>

#ifndef _TSQUEUE_H_
#define _TSQUEUE_H_


template<class TYPE> class CTSQueue : public CObject
{
public:
    CTSQueue();
    ~CTSQueue();
    void SetSize(const long int size);
    long int GetSize(void);
    void SetGrowBy(const long int size);
    long int GetGrowBy(void);
    long int GetItemCount(void);
    long int Push(const TYPE value);
    long int Pop(TYPE *const value);

    virtual void Serialize(CArchive &archive);

private:
    TYPE *m_data;
    long int m_size;
    long int m_bottom;
    long int m_top;
    long int m_itemcount;
    long int m_growby;

    // MFC-specific thread synchronization object
    CCriticalSection m_CriticalSection;

protected:
};


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   CTSQueue
//
//  Description:
//          Standard constructor.  Does NOT allocate memory for the array.
//          The program must call SetSize() to do this.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> CTSQueue<TYPE>::CTSQueue()
{
    m_data = NULL;
    m_size = 0;
    m_bottom = 0;
    m_top = 0;
    m_itemcount = 0;
    m_growby = 16;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   ~CTSQueue
//
//  Description:
//          Standard destructor.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> CTSQueue<TYPE>::~CTSQueue()
{
    // Create an accessor object to access m_CriticalSection.  This
    // will unlock the resource automatically when we go out of scope.
    CSingleLock lock(&m_CriticalSection);
    lock.Lock();

    if (m_data)
        delete [] m_data;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   SetSize
//
//  Description:
//          Sets the size of the internal array.  The program MUST call
//          SetSize() after instantiating the object.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> void CTSQueue<TYPE>::SetSize(const long int size)
{
    // We don't want to ever shrink the array.
    if (size <= m_size)
        return;

    // Create an accessor object to access m_CriticalSection.  This
    // will unlock the resource automatically when we go out of scope.
    CSingleLock lock(&m_CriticalSection);
    lock.Lock();

    int num_items = m_itemcount;        // have to store m_itemcount since
    // Pop() decrements it

    TYPE temp;
    TYPE *newarray = new TYPE[size];

    if (newarray) {
        // copy the existing array
        for (long int i = 0; i < num_items; i++) {
            Pop(&temp);
            newarray[i] = temp;
        }

        // delete old array
        if (m_data)
            delete [] m_data;

        m_data = newarray;
        m_size = size;
        m_itemcount = num_items;    // restore old m_itemcount
        m_top = num_items;
        m_bottom = 0;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetSize
//
//  Description:
//          Returns the size of the internal array.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> long int CTSQueue<TYPE>::GetSize(void)
{
    // Create an accessor object to access m_CriticalSection.  This
    // will unlock the resource automatically when we go out of scope.
    CSingleLock lock(&m_CriticalSection);
    lock.Lock();

    return m_size;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   SetGrowBy
//
//  Description:
//          Sets the size by which the internal array will grow when the
//          queue completely fills up.  If this size is zero, the array
//          will not be resized.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> void CTSQueue<TYPE>::SetGrowBy(const long int size)
{
    CSingleLock lock(&m_CriticalSection);
    lock.Lock();

    m_growby = size;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetGrowBy
//
//  Description:
//          Returns the size by which the internal array will grow if the
//          queue fills up.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> long int CTSQueue<TYPE>::GetGrowBy(void)
{
    // Create an accessor object to access m_CriticalSection.  This
    // will unlock the resource automatically when we go out of scope.
    CSingleLock lock(&m_CriticalSection);
    lock.Lock();

    return m_growby;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetItemCount
//
//  Description:
//          Returns the number of elements in the queue.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> long int CTSQueue<TYPE>::GetItemCount(void)
{
    // Create an accessor object to access m_CriticalSection.  This
    // will unlock the resource automatically when we go out of scope.
    CSingleLock lock(&m_CriticalSection);
    lock.Lock();

    return m_itemcount;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Push
//
//  Description:
//          Enqueues a new item to the bottom of the queue.  Returns the
//          number of items now on the queue.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> long int CTSQueue<TYPE>::Push(const TYPE value)
{
    // Create an accessor object to access m_CriticalSection.  This
    // will unlock the resource automatically when we go out of scope.
    CSingleLock lock(&m_CriticalSection);
    lock.Lock();

    // If the queue is full, increase the size of the array.
    if ((m_itemcount == m_size) && (m_growby))
        SetSize(GetSize() + m_growby);

    if (m_size == 0)
        return 0;

    m_data[m_top] = value;

    m_itemcount++;
    m_top++;

    // Wrap around to the bottom if we go past the top
    if (m_top == m_size)
        m_top = 0;

    return m_itemcount;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Pop
//
//  Description:
//          Dequeues the next item from the top of the queue.  Returns the
//          number of items now on the queue, -1 if the queue is empty, or
//          -2 if no memory has been allocated for the internal array.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> long int CTSQueue<TYPE>::Pop(TYPE *const value)
{
    if (m_itemcount == 0)
        return -1;

    if (m_data == NULL)
        return -2;

    // Create an accessor object to access m_CriticalSection.  This
    // will unlock the resource automatically when we go out of scope.
    CSingleLock lock(&m_CriticalSection);
    lock.Lock();

    *value = m_data[m_bottom];

    m_itemcount--;
    m_bottom++;

    // Wrap around the the bottom if necessary
    if (m_bottom == m_size)
        m_bottom = 0;


    return m_itemcount;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Serialize
//
//  Description:
//          Overrides CObject::Serialize().  For the sake of simplicity
//          the entire buffer is stored, including the data beyond the
//          bounds set by m_bottom and m_top.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> void CTSQueue<TYPE>::Serialize(CArchive &ar)
{
    // Call base class Serialize() to make sure all inherited
    // members are saved/loaded.
    CObject::Serialize(ar);

    if (ar.IsStoring()) {
        // Store the members that are simple types.
        ar << m_size << m_bottom << m_top << m_itemcount << m_growby;

        // Now store the data.  Since TYPE may not be a simple type,
        // we must use CArchive::Write().
        ar.Write(m_data, m_size * sizeof(TYPE));
    } else {
        // Restore the members that are simple types.
        ar >> m_size >> m_bottom >> m_top >> m_itemcount >> m_growby;

        // Allocate memory for the data.
        m_data = new TYPE[m_size];

        // Now retrieve the data.
        ar.Read(m_data, m_size * sizeof(TYPE));
    }
}

#endif // _TSQUEUE_H_