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
//  LINKEDLIST.H
//
//  Created:    7/18/01
//
//  Author:     Lance Durham
//
//  Description:    Template class that implements a singly-linked list.
//                  This class does not use MFC.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

template<class TYPE> class CLinkedList
{
public:
    CLinkedList();
    ~CLinkedList();
    unsigned long GetItemCount(void);
    TYPE *Find(const TYPE &value, const TYPE *startfrom = 0);
    TYPE *InsertHead(const TYPE &newitem);
    TYPE *InsertTail(const TYPE &newitem);
    TYPE *InsertAfter(unsigned long *handle, const TYPE &newitem);
    TYPE *InsertBefore(unsigned long *handle, const TYPE &newitem);
    unsigned long Remove(unsigned long *handle);
    void RemoveAll(void);
    TYPE *GetHead(unsigned long *handle) const;
    TYPE *GetTail(unsigned long *handle) const;
    TYPE *GetItem(unsigned long *handle) const;
    TYPE *GetNext(unsigned long *handle) const;
    TYPE *GetPrev(unsigned long *handle) const;
    void MoveHead(unsigned long *handle);
    void MoveTail(unsigned long *handle);
    void MoveAfter(unsigned long *handle, unsigned long *target);
    void MoveBefore(unsigned long *handle, unsigned long *target);
    TYPE *PtrFromHandle(unsigned long *handle);

protected:
    struct _LISTNODE {
        TYPE        m_data;
        _LISTNODE   *m_next;
    };

    unsigned long m_itemcount;
    _LISTNODE *m_list;

private:
};


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   CLinkedList
//
//  Description:
//          Standard constructor.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> CLinkedList<TYPE>::CLinkedList()
{
    m_itemcount = 0;
    m_list = 0;         // NULL
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   ~CLinkedList
//
//  Description:
//          Standard destructor.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> CLinkedList<TYPE>::~CLinkedList()
{
    RemoveAll();
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetItemCount
//
//  Description:
//          Returns the number of items in the linked list.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> inline unsigned long CLinkedList<TYPE>::GetItemCount(void)
{
    return m_itemcount;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Find
//
//  Description:
//          Searches the list starting with startfrom for the next occurance
//          of the specified value.  If startfrom is NULL, the search starts
//          from the head of the list.  Returns the address of the next
//          occurance, or NULL if the value is not found.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> TYPE *CLinkedList<TYPE>::Find(const TYPE &value, const TYPE *startfrom)
{
    unsigned long i;
    _LISTNODE *node = (startfrom ? (_LISTNODE *)startfrom : m_list);
    _LISTNODE *next;

    for (i = 0; i < m_itemcount; i++) {
        next = node->m_next;

        if (node->m_data == value)
            return (TYPE *)node;

        node = next;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   InsertHead
//
//  Description:
//          Allocates a new node at the head (start) of the list.  Returns
//          the address of the new node.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> TYPE *CLinkedList<TYPE>::InsertHead(const TYPE &newitem)
{
    // Create the new node.
    _LISTNODE *node = new _LISTNODE;
    node->m_next = m_list;

    // Copy the item.
    node->m_data = newitem;

    // Insert the node.
    m_list = node;
    m_itemcount++;

    return (TYPE *)node;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   InsertTail
//
//  Description:
//          Allocates a new node at the tail (end) of the list.  Returns the
//          address of the new node.  Less efficient than InsertHead() or
//          InsertAfter().
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> inline TYPE *CLinkedList<TYPE>::InsertTail(const TYPE &newitem)
{
    unsigned long tail;
    GetTail(&tail);
    return (tail ? InsertAfter(&tail, newitem) : InsertHead(newitem));
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   InsertAfter
//
//  Description:
//          Allocates a new node after the specified node.  Returns the address
//          of the new node.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> TYPE *CLinkedList<TYPE>::InsertAfter(unsigned long *handle, const TYPE &newitem)
{
    // Create the new node.
    _LISTNODE *node = new _LISTNODE;
    node->m_next = ((_LISTNODE *)*handle)->m_next;

    // Copy the item.
    node->m_data = newitem;

    // Insert the node.
    ((_LISTNODE *)*handle)->m_next = node;
    m_itemcount++;

    return (TYPE *)node;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   InsertBefore
//
//  Description:
//          Allocates a new node before the specified node.  Returns the
//          address of the new node.  Less efficient than InsertHead() or
//          InsertAfter()
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> TYPE *CLinkedList<TYPE>::InsertBefore(unsigned long *handle, const TYPE &newitem)
{
    // Find the previous node.  If there is none, we are inserting at the head
    // of the linked list.
    GetPrev(handle);
    _LISTNODE *prev = (_LISTNODE *)*handle;
    if (!prev)
        return InsertHead(newitem);

    // Create the new node.
    _LISTNODE *node = new _LISTNODE;
    node->m_next = prev->m_next;

    // Copy the item.
    node->m_data = newitem;

    // Insert the node.
    prev->m_next = node;
    m_itemcount++;

    return (TYPE *)node;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   Remove
//
//  Description:
//          Removes the specified node.  Sets handle to the next item in the
//          linked list.  Returns the number of items remaining.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> unsigned long CLinkedList<TYPE>::Remove(unsigned long *handle)
{
    if (!(*handle && m_itemcount))
        return 0;

    // Unlink the node.  If this is the head of the list, set m_list to the
    // new first node.
    _LISTNODE *current = (_LISTNODE *)*handle;
    _LISTNODE *prev = current;
    GetPrev((unsigned long *)&prev);
    if (prev)
        prev->m_next = current->m_next;
    else
        m_list = current->m_next;

    *handle = (unsigned long)current->m_next;

    // Delete the node.
    delete current;

    return --m_itemcount;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   RemoveAll
//
//  Description:
//          Removes all nodes from the list.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> void CLinkedList<TYPE>::RemoveAll(void)
{
    unsigned long i;
    _LISTNODE *node = m_list, *next;

    for (i = 0; i < m_itemcount; i++) {
        next = node->m_next;
        delete node;
        node = next;
    }

    m_list = 0;         // NULL
    m_itemcount = 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetHead
//
//  Description:
//          Returns a pointer to the item at the head of the list.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> TYPE *CLinkedList<TYPE>::GetHead(unsigned long *handle) const
{
    if (m_itemcount && m_list) {
        *handle = (unsigned long)m_list;
        return &(m_list->m_data);
    }

    *handle = 0;    // NULL
    return 0;       // NULL
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetTail
//
//  Description:
//          Returns a pointer to the item at the end of the list.  Less
//          efficient than GetHead().
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> TYPE *CLinkedList<TYPE>::GetTail(unsigned long *handle) const
{
    if (m_itemcount && m_list) {
        // Iterate through the list until we reach the last item.
        unsigned long i;
        _LISTNODE *item = m_list;
        for (i = 1; i < m_itemcount; i++) {
            if (item)
                item = item->m_next;
        }

        *handle = (unsigned long)item;
        return &(item->m_data);
    }

    *handle = 0;        // NULL
    return 0;           // NULL
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetItem
//
//  Description:
//          Returns a pointer to the specified item in the list.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> inline TYPE *CLinkedList<TYPE>::GetItem(unsigned long *handle) const
{
    if (!*handle)
        return 0;       // NULL;

    return &(((_LISTNODE *)*handle)->m_data);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetNext
//
//  Description:
//          Returns a pointer to the next item in the list. Increments handle
//          to point to the returned item.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> TYPE *CLinkedList<TYPE>::GetNext(unsigned long *handle) const
{
    if (!*handle)
        return 0;       // NULL;

    *handle = (unsigned long)(((_LISTNODE *)*handle)->m_next);

    // If there is only one item in the list, return NULL.
    if (!*handle)
        return 0;

    return &(((_LISTNODE *)*handle)->m_data);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetPrev
//
//  Description:
//          Returns a pointer to the previous item in the list. Decrements
//          handle to point to the returned item.  Less efficient than
//          GetNext().
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> TYPE *CLinkedList<TYPE>::GetPrev(unsigned long *handle) const
{
    if (*handle && m_itemcount && m_list) {
        // If this is the head of the list, there is no previous item.
        if (m_list == (_LISTNODE *)*handle) {
            *handle = 0;    // NULL
            return 0;       // NULL
        }

        // Iterate through the list until we reach the current item.
        unsigned long i;
        _LISTNODE *item = m_list;
        for (i = 1; i < m_itemcount; i++) {
            if (!item)
                return 0;   // NULL

            // If the next item is the current item, this is the previous one.
            if (item->m_next == (_LISTNODE *)*handle) {
                *handle = (unsigned long)item;
                return &(item->m_data);
            }

            item = item->m_next;
        }
    }

    *handle = 0;    // NULL
    return 0;       // NULL
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   MoveHead
//
//  Description:
//          Moves the specified node to the head of the list.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> void CLinkedList<TYPE>::MoveHead(unsigned long *handle)
{
    if ((m_itemcount < 2) || (!*handle))
        return;

    _LISTNODE *current = (_LISTNODE *)*handle;
    _LISTNODE *prev = current;
    GetPrev((unsigned long *)&prev);

    if (!prev)
        return;

    prev->m_next = current->m_next;
    current->m_next = m_list;
    m_list = current;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   MoveTail
//
//  Description:
//          Moves the specified node to the end of the list.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> void CLinkedList<TYPE>::MoveTail(unsigned long *handle)
{
    if ((m_itemcount < 2) || (!*handle))
        return;

    _LISTNODE *tail;
    _LISTNODE *current = (_LISTNODE *)*handle;
    _LISTNODE *prev = current;
    GetPrev((unsigned long *)&prev);
    GetTail((unsigned long *)&tail);

    tail->m_next = current;
    prev->m_next = current->m_next;
    current->m_next = 0;            // NULL
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   MoveAfter
//
//  Description:
//          Moves the specified node after the target.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> void CLinkedList<TYPE>::MoveAfter(unsigned long *handle, unsigned long *target)
{
    if ((m_itemcount < 2) || (!*handle) || (!*target) || (handle == target))
        return;

    _LISTNODE *newprev = (_LISTNODE *)*target;
    _LISTNODE *current = (_LISTNODE *)*handle;
    _LISTNODE *prev = current;
    GetPrev((unsigned long *)&prev);

    if (!prev)
        return;

    prev->m_next = current->m_next;
    current->m_next = newprev->m_next;
    newprev->m_next = current;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   MoveBefore
//
//  Description:
//          Moves the specified node before the target.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> void CLinkedList<TYPE>::MoveBefore(unsigned long *handle, unsigned long *target)
{
    if ((m_itemcount < 2) || (!*handle) || (handle == target))
        return;

    _LISTNODE *newprev = (_LISTNODE *)*target;
    _LISTNODE *current = (_LISTNODE *)*handle;
    _LISTNODE *prev = current;
    GetPrev((unsigned long *)&prev);
    GetPrev((unsigned long *)&newprev);

    if ((!prev) || (!newprev))
        return;

    prev->m_next = current->m_next;
    current->m_next = newprev->m_next;
    newprev->m_next = current;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   PtrFromHandle
//
//  Description:
//          Returns the address of the item associated with the given handle.
//
///////////////////////////////////////////////////////////////////////////////
template <class TYPE> inline TYPE *CLinkedList<TYPE>::PtrFromHandle(unsigned long *handle)
{
    return &(((_LISTNODE *)handle)->m_data);
}


#endif // _LINKEDLIST_H_