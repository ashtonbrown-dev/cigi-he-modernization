/** <pre>
 *  Copyright 2005 The Boeing Company
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

#include "cas.h"

///////////////////////////////////////////////////////////////////////////////
//
//  Name:   CAS
//
//  Description:
//          Atomic compare and set. If *target == expected then set *target
//          to newval and return TRUE. Otherwise, return FALSE.
//
///////////////////////////////////////////////////////////////////////////////
long CAS(long *target, long expected, long newval)
{
    int retval = 0;

    _asm {
        mov ECX, target         ; ECX = target
        mov EAX, expected       ; EAX = expected
        mov EBX, newval         ; EBX = newval

        lock cmpxchg [ECX], EBX ; Atomic compare and swap
                                ; if (EAX == *target)
                                ;     *target = EBX, ZF = 1
                                ; else
                                ;     EAX = *target, ZF = 0

        jne LABEL_NOT_EQ        ; if succeeded, then
        mov retval, 1h          ;     retval = TRUE

    LABEL_NOT_EQ:
    }

    return retval;
}