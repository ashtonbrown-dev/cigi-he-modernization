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
 *  FILENAME:   ArtPart.cpp
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

#include "stdafx.h"
#include "cigi4types.h"
#include "hemu4.h"
#include "ArtPart.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CArtPart, CObject, 0)

CArtPart::CArtPart()
{
    m_HasChanged = FALSE;
    m_ID = -1;
    m_EntityID = -1;
    m_Enable = TRUE;
    m_XValid = FALSE;
    m_YValid = FALSE;
    m_ZValid = FALSE;
    m_RollValid = FALSE;
    m_PitchValid = FALSE;
    m_YawValid = FALSE;
    m_XOffset = 0.0;
    m_YOffset = 0.0;
    m_ZOffset = 0.0;
    m_Roll = 0.0;
    m_Pitch = 0.0;
    m_Yaw = 0.0;
    m_XRate = 0.0;
    m_YRate = 0.0;
    m_ZRate = 0.0;
    m_RollRate = 0.0;
    m_PitchRate = 0.0;
    m_YawRate = 0.0;
}

CArtPart::CArtPart(TEMPL_ARTPART *artpart)
{
    m_HasChanged = FALSE;
    m_Name = artpart->Name;
    m_ID = artpart->ID;
    m_EntityID = -1;        // Set by CEntity::AddArtPart()
    m_Enable = (BOOL)(artpart->Enable ? TRUE : FALSE);
    m_XValid = (BOOL)(artpart->XOffsetEnable ? TRUE : FALSE);
    m_YValid = (BOOL)(artpart->YOffsetEnable ? TRUE : FALSE);
    m_ZValid = (BOOL)(artpart->ZOffsetEnable ? TRUE : FALSE);
    m_RollValid = (BOOL)(artpart->RollEnable ? TRUE : FALSE);
    m_PitchValid = (BOOL)(artpart->PitchEnable ? TRUE : FALSE);
    m_YawValid = (BOOL)(artpart->YawEnable ? TRUE : FALSE);
    m_XOffset = (float)artpart->XOffset;
    m_YOffset = (float)artpart->YOffset;
    m_ZOffset = (float)artpart->ZOffset;
    m_Roll = (float)artpart->Roll;
    m_Pitch = (float)artpart->Pitch;
    m_Yaw = (float)artpart->Yaw;
    m_XRate = 0.0;
    m_YRate = 0.0;
    m_ZRate = 0.0;
    m_RollRate = 0.0;
    m_PitchRate = 0.0;
    m_YawRate = 0.0;
}

CArtPart::~CArtPart()
{
}

CArtPart &CArtPart::operator=(const CArtPart &rhs)
{
    m_Name = rhs.m_Name;
    m_ID = rhs.m_ID;
    m_EntityID = rhs.m_EntityID;
    m_Enable = rhs.m_Enable;
    m_XValid = rhs.m_XValid;
    m_YValid = rhs.m_YValid;
    m_ZValid = rhs.m_ZValid;
    m_RollValid = rhs.m_RollValid;
    m_PitchValid = rhs.m_PitchValid;
    m_YawValid = rhs.m_YawValid;
    m_XOffset = rhs.m_XOffset;
    m_YOffset = rhs.m_YOffset;
    m_ZOffset = rhs.m_ZOffset;
    m_Roll = rhs.m_Roll;
    m_Pitch = rhs.m_Pitch;
    m_Yaw = rhs.m_Yaw;
    m_XRate = rhs.m_XRate;
    m_YRate = rhs.m_YRate;
    m_ZRate = rhs.m_ZRate;
    m_RollRate = rhs.m_RollRate;
    m_PitchRate = rhs.m_PitchRate;
    m_YawRate = rhs.m_YawRate;

    m_HasChanged = TRUE;

    return *this;
}

void CArtPart::Serialize(CArchive &ar)
{
    TRACE0("CArtPart::Serialize()\n");

    int length = 0;

    if (ar.IsStoring()) {
        // Store the name.
        length = strlen((LPCTSTR)m_Name);
        ar << length;
        ar.Write((LPCTSTR)m_Name, length);  // NULL terminator not copied

        // Store the integral types.
        ar << m_ID;
        ar << m_EntityID;
        ar << m_Enable;
        ar << m_XValid;
        ar << m_YValid;
        ar << m_ZValid;
        ar << m_RollValid;
        ar << m_PitchValid;
        ar << m_YawValid;
        ar << m_XOffset;
        ar << m_YOffset;
        ar << m_ZOffset;
        ar << m_Roll;
        ar << m_Pitch;
        ar << m_Yaw;

        // Store some padding in case we have to add something later.
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);
        ar << unsigned long(0);

        TRACE1("		length = %d\n", length);
        TRACE1("		m_Name = \"%s\"\n", (LPCTSTR)m_Name);
        TRACE1("		m_ID = %d\n", m_ID);
        TRACE1("		m_EntityID = %d\n", m_EntityID);
        TRACE1("		m_Enable = %d\n", m_Enable);
    } else {
        // Read the name.
        ar >> length;
        ar.Read(m_Name.GetBuffer(length), length);
        m_Name.ReleaseBuffer(length);

        // Store the integral types.
        ar >> m_ID;
        ar >> m_EntityID;
        ar >> m_Enable;
        ar >> m_XValid;
        ar >> m_YValid;
        ar >> m_ZValid;
        ar >> m_RollValid;
        ar >> m_PitchValid;
        ar >> m_YawValid;
        ar >> m_XOffset;
        ar >> m_YOffset;
        ar >> m_ZOffset;
        ar >> m_Roll;
        ar >> m_Pitch;
        ar >> m_Yaw;

        // Read the padding.
        unsigned long junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;
        ar >> junk;

        m_HasChanged = TRUE;

        TRACE1("		length = %d\n", length);
        TRACE1("		m_Name = \"%s\"\n", (LPCTSTR)m_Name);
        TRACE1("		m_ID = %d\n", m_ID);
        TRACE1("		m_EntityID = %d\n", m_EntityID);
        TRACE1("		m_Enable = %d\n", m_Enable);
    }
}

void CArtPart::SetXOffset(const float x, const BOOL enable)
{
    m_XOffset = x;
    m_XValid = enable;

    m_HasChanged = TRUE;
}

void CArtPart::SetYOffset(const float y, const BOOL enable)
{
    m_YOffset = y;
    m_YValid = enable;

    m_HasChanged = TRUE;
}

void CArtPart::SetZOffset(const float z, const BOOL enable)
{
    m_ZOffset = z;
    m_ZValid = enable;

    m_HasChanged = TRUE;
}

void CArtPart::SetRoll(const float roll, const BOOL enable)
{
    m_Roll = roll;
    m_RollValid = enable;

    m_HasChanged = TRUE;
}

void CArtPart::SetPitch(const float pitch, const BOOL enable)
{
    m_Pitch = pitch;
    m_PitchValid = enable;

    m_HasChanged = TRUE;
}

void CArtPart::SetYaw(const float yaw, const BOOL enable)
{
    m_Yaw = yaw;
    m_YawValid = enable;

    m_HasChanged = TRUE;
}

void CArtPart::SetXRate(const float dx)
{
    m_XRate = dx;
}

float CArtPart::GetXRate(void)
{
    return m_XRate;
}

void CArtPart::SetYRate(const float dy)
{
    m_YRate = dy;
}

float CArtPart::GetYRate(void)
{
    return m_YRate;
}

void CArtPart::SetZRate(const float dz)
{
    m_ZRate = dz;
}

float CArtPart::GetZRate(void)
{
    return m_ZRate;
}

void CArtPart::SetRollRate(const float droll)
{
    m_RollRate = droll;
}

float CArtPart::GetRollRate(void)
{
    return m_RollRate;
}

void CArtPart::SetPitchRate(const float dpitch)
{
    m_PitchRate = dpitch;
}

float CArtPart::GetPitchRate(void)
{
    return m_PitchRate;
}

void CArtPart::SetYawRate(const float dyaw)
{
    m_YawRate = dyaw;
}

float CArtPart::GetYawRate(void)
{
    return m_YawRate;
}

BOOL CArtPart::CreateCigiPacket(void *buffer)
{
    int setcount = 0;
    CIGI_ARTPART_CONTROL *l = (CIGI_ARTPART_CONTROL *)buffer;
    CIGI_ARTPART_CONTROL_S *s = (CIGI_ARTPART_CONTROL_S *)buffer;

    if (m_XValid)
        setcount++;
    if (m_YValid)
        setcount++;
    if (m_ZValid)
        setcount++;
    if (m_RollValid)
        setcount++;
    if (m_PitchValid)
        setcount++;
    if (m_YawValid)
        setcount++;

    if (setcount > 2) { // Articulated Part Control
        l->packet_id = CIGI_ARTPART_CONTROL_OPCODE;
        l->packet_size = sizeof(CIGI_ARTPART_CONTROL);
        l->entity_id = m_EntityID;
        l->artpart_id = m_ID;
        l->artpart_enable = m_Enable;
        l->xoffset_enable = m_XValid;
        l->yoffset_enable = m_YValid;
        l->zoffset_enable = m_ZValid;
        l->roll_enable = m_RollValid;
        l->pitch_enable = m_PitchValid;
        l->yaw_enable = m_YawValid;
        l->xoffset = m_XOffset;
        l->yoffset = m_YOffset;
        l->zoffset = m_ZOffset;
        l->roll = m_Roll;
        l->pitch = m_Pitch;
        l->yaw = m_Yaw;

        return FALSE;   // not short version
    } else {            // Short Articulated Part Control
        s->packet_id = CIGI_ARTPART_CONTROL_S_OPCODE;
        s->packet_size = sizeof(CIGI_ARTPART_CONTROL_S);
        s->entity_id = m_EntityID;
        s->artpart1_id = m_ID;
        s->artpart2_id = m_ID;
        s->artpart1_enable = m_Enable;
        s->artpart2_enable = m_Enable;
        s->dof1_select = 0;     // default to Not Used
        s->dof2_select = 0;     // ditto
        s->dof1 = 0.0;
        s->dof2 = 0.0;

        // Now setcount is used to tell us which DOF we are setting.
        setcount = 0;
        if (m_XValid) {
            s->dof1_select = 1;
            s->dof1 = m_XOffset;
            setcount++;
        }
        if (m_YValid) {
            if (setcount == 0) {
                s->dof1_select = 2;
                s->dof1 = m_YOffset;
                setcount++;
            } else {
                s->dof2_select = 2;
                s->dof2 = m_YOffset;
                return TRUE;
            }
        }
        if (m_ZValid) {
            if (setcount == 0) {
                s->dof1_select = 3;
                s->dof1 = m_ZOffset;
                setcount++;
            } else {
                s->dof2_select = 3;
                s->dof2 = m_ZOffset;
                return TRUE;
            }
        }
        if (m_YawValid) {
            if (setcount == 0) {
                s->dof1_select = 4;
                s->dof1 = m_Yaw;
                setcount++;
            } else {
                s->dof2_select = 4;
                s->dof2 = m_Yaw;
                return TRUE;
            }
        }
        if (m_PitchValid) {
            if (setcount == 0) {
                s->dof1_select = 5;
                s->dof1 = m_Pitch;
                setcount++;
            } else {
                s->dof2_select = 5;
                s->dof2 = m_Pitch;
                return TRUE;
            }
        }
        if (m_RollValid) {
            if (setcount == 0) {
                s->dof1_select = 6;
                s->dof1 = m_Roll;
                setcount++;
            } else {
                s->dof2_select = 6;
                s->dof2 = m_Roll;
                return TRUE;
            }
        }

        return TRUE;    // short version
    }
}

template <> void AFXAPI SerializeElements<CArtPart *>(CArchive &ar, CArtPart **pElements, int nCount)
{
    if (ar.IsStoring()) {
        for (int i = 0; i < nCount; i++) {
            if (pElements[i])
                pElements[i]->Serialize(ar);
        }
    } else {
        for (int i = 0; i < nCount; i++) {
            TEMPL_ARTPART dummy;
            pElements[i] = new CArtPart(&dummy);
            pElements[i]->Serialize(ar);
        }
    }
}
