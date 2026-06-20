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

#ifndef _CONFIGTEMPLVIEW_H_
#define _CONFIGTEMPLVIEW_H_

#include "stdafx.h"
#include "ConfigTemplComponent.h"
#include "linkedlist.h"

class TEMPL_VIEW : public CObject
{
public:
    DECLARE_SERIAL(TEMPL_VIEW)

    TEMPL_VIEW();
    ~TEMPL_VIEW();
    TEMPL_VIEW &operator=(const TEMPL_VIEW &rhs);
    BOOL ReadText(CString &src, int *linenum);
    BOOL WriteText(CStdioFile &file);
    void Serialize(CArchive &ar);
    TEMPL_COMPONENT *FindComponent(const int id);
    TEMPL_COMPONENT_STATE *FindComponentState(const int component_id, const int value);

    CString Name;
    int ViewID;
    int GroupID;
    unsigned char ViewType;
    unsigned char ProjectionType;
    unsigned char ReplicationMode;
    unsigned char MirrorMode;
    float XOffset;
    float YOffset;
    float ZOffset;
    float Yaw;
    float Pitch;
    float Roll;
    float Top;
    float Bottom;
    float Left;
    float Right;
    float Near;
    float Far;
    CLinkedList<TEMPL_COMPONENT> ComponentList;
};

#endif
