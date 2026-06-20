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

#ifndef _CONFIGTEMPLENTITY_H_
#define _CONFIGTEMPLENTITY_H_

#include "StdAfx.h"
#include "ConfigTemplComponent.h"
#include "ConfigTemplArtPart.h"
#include "ConfigTemplAttachPoint.h"
#include "ConfigTemplCollisionSeg.h"
#include "ConfigTemplCollisionVol.h"
#include "linkedlist.h"

class TEMPL_WEATHER;

class TEMPL_ENTITY : public CObject
{
public:
    DECLARE_SERIAL(TEMPL_ENTITY)

    TEMPL_ENTITY();
    ~TEMPL_ENTITY();
    TEMPL_ENTITY &operator=(const TEMPL_ENTITY &rhs);
    BOOL ReadText(CString &src, int *linenum);
    BOOL WriteText(CStdioFile &file);
    void Serialize(CArchive &ar);
    TEMPL_COMPONENT *FindComponent(const int id);
    TEMPL_COMPONENT_STATE *FindComponentState(const int component_id, const int value);
    TEMPL_ARTPART *FindArtPart(const int id);
    TEMPL_ATTACHPOINT *FindAttachPoint(const int id);
    TEMPL_COLLISION_SEG *FindCollisionSeg(const int id);
    TEMPL_COLLISION_VOL *FindCollisionVol(const int id);
    TEMPL_WEATHER *GetWeatherAttributes(void);

    CString Name;
    int Type;
    int reserved1;
    unsigned char Alpha;
    unsigned char InheritAlpha;
    unsigned char ClampMode;
    unsigned char AnimDirection;
    unsigned char AnimLoop;
    unsigned char Class;
    unsigned char reserved2;
    unsigned char reserved3;
    TEMPL_WEATHER *WeatherAttributes;
    CLinkedList<TEMPL_COMPONENT> ComponentList;
    CLinkedList<TEMPL_ARTPART> ArtPartList;
    CLinkedList<TEMPL_ATTACHPOINT> AttachPointList;
    CLinkedList<TEMPL_COLLISION_SEG> CollisionSegList;
    CLinkedList<TEMPL_COLLISION_VOL> CollisionVolList;
};

inline TEMPL_WEATHER *TEMPL_ENTITY::GetWeatherAttributes(void)
{
    return WeatherAttributes;
}

#endif
