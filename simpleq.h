///////////////////////////////////////////////////////////////////////////////
//
//  SIMPLEQ.H
//
//  Created:    7/18/01
//
//  Author:     Lance Durham
//
//  Description:    Template class that implements a simple queue.  This class
//                  is NOT thread-safe.  It does not use MFC.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _SIMPLEQ_H_
#define _SIMPLEQ_H_


template<class TYPE> class CSimpleQ
{
public:
    CSimpleQ();
    ~CSimpleQ();
    void SetSize(const long int size);
    long int GetSize(void);
    void SetGrowBy(const long int size);
    long int GetGrowBy(void);
    long int GetItemCount(void);
    long int Push(const TYPE value);
    long int Pop(TYPE *const value);

private:
    TYPE *m_data;
    long int m_size;
    long int m_bottom;
    long int m_top;
    long int m_itemcount;
    long int m_growby;

protected:
};


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   CSimpleQ
//
//  Description:
//          Standard constructor.  Does NOT allocate memory for the array.
//          The program must call SetSize() to do this.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> CSimpleQ<TYPE>::CSimpleQ()
{
    m_data = NULL;
    m_size = 0;
    m_bottom = 0;
    m_top = 0;
    m_itemcount = 0;
    m_growby = 0;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   ~CSimpleQ
//
//  Description:
//          Standard destructor.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> CSimpleQ<TYPE>::~CSimpleQ()
{
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
template <class TYPE> void CSimpleQ<TYPE>::SetSize(const long int size)
{
    // We don't want to ever shrink the array.
    if (size <= m_size)
        return;

    int num_items = m_itemcount;        // have to store m_itemcount since
    // Pop() decrements it

    TYPE *newarray = new TYPE[size];

    if (newarray) {
        // copy the existing array
        for (long int i = 0; i < num_items; i++) {
            TYPE temp;
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
template <class TYPE> long int CSimpleQ<TYPE>::GetSize(void)
{
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
template <class TYPE> void CSimpleQ<TYPE>::SetGrowBy(const long int size)
{
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
template <class TYPE> long int CSimpleQ<TYPE>::GetGrowBy(void)
{
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
template <class TYPE> long int CSimpleQ<TYPE>::GetItemCount(void)
{
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
template <class TYPE> long int CSimpleQ<TYPE>::Push(const TYPE value)
{
    if (m_size == 0)
        return 0;

    // If the queue is full, increase the size of the array.
    if ((m_itemcount != 0) && (m_itemcount == m_size) && (m_growby))
        SetSize(GetSize() + m_growby);

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
template <class TYPE> long int CSimpleQ<TYPE>::Pop(TYPE *const value)
{
    if (m_itemcount == 0)
        return -1;

    if (m_data == NULL)
        return -2;

    *value = m_data[m_bottom];

    m_itemcount--;
    m_bottom++;

    // Wrap around the the bottom if necessary
    if (m_bottom == m_size)
        m_bottom = 0;


    return m_itemcount;
}

#endif // _SIMPLEQ_H_