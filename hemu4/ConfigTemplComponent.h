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

#ifndef _CONFIGTEMPLCOMPONENT_H_
#define _CONFIGTEMPLCOMPONENT_H_

#include "StdAfx.h"
#include "linkedlist.h"

class TEMPL_COMPONENT_STATE : public CObject
{
public:
    DECLARE_SERIAL(TEMPL_COMPONENT_STATE)

    TEMPL_COMPONENT_STATE();
    ~TEMPL_COMPONENT_STATE();
    TEMPL_COMPONENT_STATE &operator=(const TEMPL_COMPONENT_STATE &rhs);
    BOOL ReadText(CString &src, int *linenum);
    BOOL WriteText(CStdioFile &file);
    void Serialize(CArchive &ar);

    CString Name;
    int Value;
};

class TEMPL_COMPONENT : public CObject
{
public:
    DECLARE_SERIAL(TEMPL_COMPONENT)

    static const unsigned char DataTypeInt32;   // = 0
    static const unsigned char DataTypeUInt32;  // = 1
    static const unsigned char DataTypeFloat;   // = 2
    static const unsigned char DataTypeInt64;   // = 3
    static const unsigned char DataTypeUInt64;  // = 4
    static const unsigned char DataTypeDouble;  // = 5

    TEMPL_COMPONENT();
    ~TEMPL_COMPONENT();
    TEMPL_COMPONENT &operator=(const TEMPL_COMPONENT &rhs);
    BOOL ReadText(CString &src, int *linenum);
    BOOL WriteText(CStdioFile &file);
    void Serialize(CArchive &ar);
    TEMPL_COMPONENT_STATE *FindState(const int value);

    CString Name;
    int ID;
    int InstID;
    int DefState;
    int ComponentClass;
    unsigned long DefData[6];
    unsigned char DefDataType[6];
    unsigned char DefDataSet[6];
    unsigned char reserved1;
    unsigned char reserved2;
    unsigned char reserved3;
    unsigned char reserved4;
    CLinkedList<TEMPL_COMPONENT_STATE> StateList;
};

#endif