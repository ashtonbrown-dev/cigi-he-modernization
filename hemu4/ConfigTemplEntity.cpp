/** <pre>
 *  Copyright 2004 The Boeing Company
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
 *
 *  FILENAME:   ConfigTemplEntity.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface Host Emulator (CIGI HE)
 *
 *  PROGRAM DESCRIPTION:
 *  The Host Emulator (HE) acts as a surrogate in the absence of a simulation host.
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME
 *  DESCRIPTION OF CHANGE
 *
 *  11/11/2004 Lance Durham
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  Version 3.1.1
 */

#include "ConfigTemplInternal.h"
#include "ConfigTemplEntity.h"
#include "ConfigTemplWeather.h"
#include "globals.h"

///////////////////////////////////////////////////////////////////////////////
//
//  TEMPL_ENTITY
//
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(TEMPL_ENTITY, CObject, 0)

TEMPL_ENTITY::TEMPL_ENTITY()
{
    Type = -1;
    reserved1 = 0;
    Alpha = 255;
    InheritAlpha = 0;
    ClampMode = 0;
    AnimDirection = 0;
    AnimLoop = 0;
    Class = ENTITY_CLASS_GENERIC;
    reserved2 = 0;
    reserved3 = 0;
    WeatherAttributes = NULL;
}


TEMPL_ENTITY::~TEMPL_ENTITY()
{
    if (WeatherAttributes)
        delete WeatherAttributes;
}

TEMPL_ENTITY &TEMPL_ENTITY::operator=(const TEMPL_ENTITY &rhs)
{
    unsigned long handle = NULL;
    TEMPL_COMPONENT *comp = NULL;
    TEMPL_ARTPART *artpart = NULL;
    TEMPL_ATTACHPOINT *attach = NULL;

    Name = rhs.Name;
    Type = rhs.Type;
    reserved1 = rhs.reserved1;
    Alpha = rhs.Alpha;
    InheritAlpha = rhs.InheritAlpha;
    ClampMode = rhs.ClampMode;
    AnimDirection = rhs.AnimDirection;
    AnimLoop = rhs.AnimLoop;
    Class = ENTITY_CLASS_GENERIC;
    reserved2 = rhs.reserved2;
    reserved3 = rhs.reserved3;
    *WeatherAttributes = *(rhs.WeatherAttributes);

    ComponentList.RemoveAll();
    comp = rhs.ComponentList.GetHead(&handle);
    while (comp) {
        ComponentList.InsertTail(*comp);
        comp = rhs.ComponentList.GetNext(&handle);
    }

    ArtPartList.RemoveAll();
    artpart = rhs.ArtPartList.GetHead(&handle);
    while (artpart) {
        ArtPartList.InsertTail(*artpart);
        artpart = rhs.ArtPartList.GetNext(&handle);
    }

    AttachPointList.RemoveAll();
    attach = rhs.AttachPointList.GetHead(&handle);
    while (attach) {
        AttachPointList.InsertTail(*attach);
        attach = rhs.AttachPointList.GetNext(&handle);
    }

    return *this;
}

// Binary only.
void TEMPL_ENTITY::Serialize(CArchive &ar)
{
    TRACE0("\n--  TEMPL_ENTITY::Serialize()\n");

    unsigned long junk;
    unsigned long handle = NULL;
    int length = 0;
    int count = 0;

    // Call base class Serialize() to make sure all inherited
    // members are saved/loaded.
    CObject::Serialize(ar);

    if (ar.IsStoring()) {
        int i = 0;
        TEMPL_COMPONENT *comp = NULL;
        TEMPL_ARTPART *artpart = NULL;
        TEMPL_ATTACHPOINT *attach = NULL;

        // Store the name.
        length = strlen((LPCTSTR)Name);
        ar << length;
        ar.Write((LPCTSTR)Name, length);    // NULL terminator not copied

        // Store the integral types.
        ar << Type;
        ar << Class;
        ar << reserved1;
        ar << Alpha;
        ar << InheritAlpha;
        ar << ClampMode;
        ar << AnimDirection;
        ar << AnimLoop;
        ar << reserved2;
        ar << reserved3;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("	length = %d\n", length);
        TRACE1("	name = %s\n", Name);
        TRACE1("	Type = %d\n", Type);
        TRACE1("	Class = %d\n", Class);
        TRACE1("	reserved1 = %d\n", reserved1);
        TRACE1("	Alpha = %d\n", Alpha);
        TRACE1("	InheritAlpha = %d\n", InheritAlpha);
        TRACE1("	ClampMode = %d\n", ClampMode);
        TRACE1("	AnimDirection = %d\n", AnimDirection);
        TRACE1("	AnimLoop = %d\n", AnimLoop);
        TRACE1("	reserved2 = %d\n", reserved2);
        TRACE1("	reserved3 = %d\n", reserved3);

        // Store the components.
        count = ComponentList.GetItemCount();
        ar << count;

        TRACE1("	component count = %d\n", count);

        if (count) {
            comp = ComponentList.GetHead(&handle);
            while (comp) {
                comp->Serialize(ar);
                comp = ComponentList.GetNext(&handle);
            }
        }

        // Store the articulated parts.
        count = ArtPartList.GetItemCount();
        ar << count;

        TRACE1("	artpart count = %d\n", count);

        if (count) {
            artpart = ArtPartList.GetHead(&handle);
            while (artpart) {
                artpart->Serialize(ar);
                artpart = ArtPartList.GetNext(&handle);
            }
        }

        // Store the attach points
        count = AttachPointList.GetItemCount();
        ar << count;

        TRACE1("	attach count = %d\n", count);

        if (count) {
            attach = AttachPointList.GetHead(&handle);
            while (attach) {
                attach->Serialize(ar);
                attach = AttachPointList.GetNext(&handle);
            }
        }
    } else {
        int i = 0;

        // Read the name.
        ar >> length;
        ar.Read(Name.GetBuffer(length), length);
        Name.ReleaseBuffer(length);

        // Read the integral types.
        ar >> Type;
        ar >> Class;
        ar >> reserved1;
        ar >> Alpha;
        ar >> InheritAlpha;
        ar >> ClampMode;
        ar >> AnimDirection;
        ar >> AnimLoop;
        ar >> reserved2;
        ar >> reserved3;

        // Read the padding.
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        TRACE1("	length = %d\n", length);
        TRACE1("	name = %s\n", Name);
        TRACE1("	Type = %d\n", Type);
        TRACE1("	Class = %d\n", Class);
        TRACE1("	reserved1 = %d\n", reserved1);
        TRACE1("	Alpha = %d\n", Alpha);
        TRACE1("	InheritAlpha = %d\n", InheritAlpha);
        TRACE1("	ClampMode = %d\n", ClampMode);
        TRACE1("	AnimDirection = %d\n", AnimDirection);
        TRACE1("	AnimLoop = %d\n", AnimLoop);
        TRACE1("	reserved2 = %d\n", reserved2);
        TRACE1("	reserved3 = %d\n", reserved3);

        // Read in the components.
        ar >> count;

        TRACE1("	component count = %d\n", count);

        for (i = 0; i < count; i++) {
            TEMPL_COMPONENT comp;
            comp.Serialize(ar);
            ComponentList.InsertTail(comp);
        }

        // Read in the articulated parts.
        ar >> count;

        TRACE1("	artpart count = %d\n", count);

        for (i = 0; i < count; i++) {
            TEMPL_ARTPART artpart;
            artpart.Serialize(ar);
            ArtPartList.InsertTail(artpart);
        }

        // Read in the attach points
        ar >> count;

        TRACE1("	attach count = %d\n", count);

        for (i = 0; i < count; i++) {
            TEMPL_ATTACHPOINT attach;
            attach.Serialize(ar);
            AttachPointList.InsertTail(attach);
        }
    }
}

// Text only.
BOOL TEMPL_ENTITY::WriteText(CStdioFile &file)
{
    unsigned long handle = 0;
    CString str;
    TEMPL_COMPONENT *comp = NULL;
    TEMPL_ARTPART *artpart = NULL;
    TEMPL_ATTACHPOINT *attach = NULL;

    // Write the header and opening brace.
    file.WriteString("\nENTITY\n{\n");

    // Store the name and value.
    str.Format("\tNAME           = \"%s\";\n", Name);
    file.WriteString((LPCTSTR)str);

    str.Format("\tTYPE           = %d;\n", Type);
    file.WriteString((LPCTSTR)str);

    str.Format("\tCLASS          = %s;\n", EntityClassStr[Class]);
    file.WriteString((LPCTSTR)str);

    str.Format("\tALPHA          = %d;\n", Alpha);
    file.WriteString((LPCTSTR)str);

    str.Format("\tINHERIT_ALPHA  = %s;\n", InheritAlpha ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    str.Format("\tCLAMP_MODE     = %s;\n", EntityClampStr[ClampMode]);
    file.WriteString((LPCTSTR)str);

    str.Format("\tANIM_DIRECTION = %s;\n", AnimDirection ? "BACKWARD" : "FORWARD");
    file.WriteString((LPCTSTR)str);

    str.Format("\tANIM_LOOP      = %s;\n", AnimLoop ? "TRUE" : "FALSE");
    file.WriteString((LPCTSTR)str);

    // Write each component.
    comp = ComponentList.GetHead(&handle);
    while (comp) {
        comp->WriteText(file);
        comp = ComponentList.GetNext(&handle);
    }

    // Store the articulated parts.
    artpart = ArtPartList.GetHead(&handle);
    while (artpart) {
        artpart->WriteText(file);
        artpart = ArtPartList.GetNext(&handle);
    }

    // Store the attach points
    attach = AttachPointList.GetHead(&handle);
    while (attach) {
        attach->WriteText(file);
        attach = AttachPointList.GetNext(&handle);
    }

    // Write the closing brace.
    file.WriteString("}\n\n");

    return TRUE;
}

// Text only.
BOOL TEMPL_ENTITY::ReadText(CString &src, int *linenum)
{
    BOOL readstatus;
    CString str, temp, token, value;

    // We should be starting just inside the opening brace.

    // Get the entity block.
    str = ReadBlock(src, linenum);

    // Read and process each line.
    readstatus = ReadLine(str, linenum, token, value);
    while ((readstatus == TRUE) && (token != "}")) {
        if (token == "NAME") {
            // Trim the quotes.
            value.TrimLeft("\"");
            value.TrimRight("\"");
            value.TrimRight();

            Name = value;
        } else if (token == "TYPE") {
            Type = atoi((LPCTSTR)value);
        } else if (token == "CLASS") {
            Class = StrToEntityClass((LPCTSTR)value);
        } else if (token == "ALPHA") {
            Alpha = atoi((LPCTSTR)value);
        } else if (token == "INHERIT_ALPHA") {
            InheritAlpha = StrToBool((LPCTSTR)value);
        } else if (token == "CLAMP_MODE") {
            if (value == "NONE")
                ClampMode = ENTITY_CLAMP_NONE;
            else if (value == "CONFORMAL")
                ClampMode = ENTITY_CLAMP_CONFORMAL;
            else if (value == "NONCONFORMAL")
                ClampMode = ENTITY_CLAMP_NONCONFORMAL;
            else
                PopupReadErrorMsg(KEYWORD, (LPCTSTR)value, linenum);
        } else if (token == "ANIM_DIRECTION") {
            if (value == "FORWARD")
                AnimDirection = 0;
            else if (value == "BACKWARD")
                AnimDirection = 1;
            else
                PopupReadErrorMsg(KEYWORD, (LPCTSTR)value, linenum);
        } else if (token == "ANIM_LOOP") {
            AnimLoop = StrToBool((LPCTSTR)value);
        } else if (token == "COMPONENT") {
            TEMPL_COMPONENT component;
            component.ReadText(str, linenum);

            ComponentList.InsertTail(component);
        } else if (token == "ARTPART") {
            TEMPL_ARTPART artpart;
            artpart.ReadText(str, linenum);

            ArtPartList.InsertTail(artpart);
        } else if (token == "ATTACH_POINT") {
            TEMPL_ATTACHPOINT attach;
            attach.ReadText(str, linenum);

            AttachPointList.InsertTail(attach);
        } else if (token == "COLLISION_SEGMENT") {
            TEMPL_COLLISION_SEG seg;
            seg.ReadText(str, linenum);

            CollisionSegList.InsertTail(seg);
        } else if (token == "COLLISION_VOLUME") {
            TEMPL_COLLISION_VOL vol;
            vol.ReadText(str, linenum);

            CollisionVolList.InsertTail(vol);
        } else if (token == "WEATHER_ATTRIBUTES") {
            WeatherAttributes = new TEMPL_WEATHER;
            if (WeatherAttributes) {
                WeatherAttributes->ReadText(str, linenum);
                WeatherAttributes->Scope = SCOPE_ENTITY;
            }
        } else {
            PopupReadErrorMsg(KEYWORD, (LPCTSTR)token, linenum);
        }

        readstatus = ReadLine(str, linenum, token, value);
    }

    return readstatus;
}

TEMPL_COMPONENT *TEMPL_ENTITY::FindComponent(const int id)
{
    unsigned long handle = 0;
    TEMPL_COMPONENT *comp = ComponentList.GetHead(&handle);

    while (comp) {
        if (comp->ID == id)
            return comp;

        comp = ComponentList.GetNext(&handle);
    }

    return NULL;
}

TEMPL_COMPONENT_STATE *TEMPL_ENTITY::FindComponentState(const int component_id, const int comp_value)
{
    TEMPL_COMPONENT *comp = FindComponent(component_id);
    if (comp)
        return comp->FindState(comp_value);

    return NULL;
}

TEMPL_ARTPART *TEMPL_ENTITY::FindArtPart(const int id)
{
    unsigned long handle = 0;
    TEMPL_ARTPART *artpart = ArtPartList.GetHead(&handle);

    while (artpart) {
        if (artpart->ID == id)
            return artpart;

        artpart = ArtPartList.GetNext(&handle);
    }

    return NULL;
}

TEMPL_ATTACHPOINT *TEMPL_ENTITY::FindAttachPoint(const int id)
{
    unsigned long handle = 0;
    TEMPL_ATTACHPOINT *attach = AttachPointList.GetHead(&handle);

    while (attach) {
        if (attach->ID == id)
            return attach;

        attach = AttachPointList.GetNext(&handle);
    }

    return NULL;
}

TEMPL_COLLISION_SEG *TEMPL_ENTITY::FindCollisionSeg(const int id)
{
    unsigned long handle = 0;
    TEMPL_COLLISION_SEG *seg = CollisionSegList.GetHead(&handle);

    while (seg) {
        if (seg->ID == id)
            return seg;

        seg = CollisionSegList.GetNext(&handle);
    }

    return NULL;
}

TEMPL_COLLISION_VOL *TEMPL_ENTITY::FindCollisionVol(const int id)
{
    unsigned long handle = 0;
    TEMPL_COLLISION_VOL *vol = CollisionVolList.GetHead(&handle);

    while (vol) {
        if (vol->ID == id)
            return vol;

        vol = CollisionVolList.GetNext(&handle);
    }

    return NULL;
}
