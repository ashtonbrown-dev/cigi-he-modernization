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
//  RTXSHAREDBUFFERQ.H
//
//  Created:    7/18/01
//
//  Author:     Lance Durham
//
//  Description:    Class that implements a buffer queue that can be used
//                  by either Windows or RTX processes.  The buffers will
//                  reside in non-paged kernel memory and can be shared by
//                  multiple processes.  This class does not use MFC.
//
//                  This class is similar to the RTXSharedBufferQueue template
//                  class.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _RTXSHAREDBUFFERQ_H_
#define _RTXSHAREDBUFFERQ_H_

#include "RTXCompatibility.h"

#define     TIMEOUT_MILLISEC    1

class RTXSharedBufferQ
{
public:
    RTXSharedBufferQ();
    ~RTXSharedBufferQ();
    long int Create(const char *name, const long buffcount, const long buffsize);
    long int GetSize(void);
    long int GetItemCount(void);
    long int Push(const char *buffer, const long length);
    long int Pop(char *buffer, const long length);

private:
    long int *m_bufflengths;        // Shared
    long int *m_size;               // Shared
    long int *m_bottom;             // Shared
    long int *m_top;                // Shared
    long int *m_itemcount;          // Shared
    char *m_instancename;           // Not shared
    char **m_data;                  // Not shared
    HANDLE m_mutex;                 // Not shared
    HANDLE m_sharedhdl;             // Not shared

protected:
};


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetSize
//
//  Description:
//          Returns the size of the internal array.
//
///////////////////////////////////////////////////////////////////////////////
inline long int RTXSharedBufferQ::GetSize(void)
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
inline long int RTXSharedBufferQ::GetItemCount(void)
{
    return *m_itemcount;
}


#endif // _RTXSHAREDBUFFERQ_H_