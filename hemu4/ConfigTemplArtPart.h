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

#ifndef _CONFIGTEMPLARTPART_H_
#define _CONFIGTEMPLARTPART_H_

#include "StdAfx.h"

class TEMPL_ARTPART : public CObject
{
public:
    DECLARE_SERIAL(TEMPL_ARTPART)

    TEMPL_ARTPART();
    ~TEMPL_ARTPART();
    TEMPL_ARTPART &operator=(const TEMPL_ARTPART &rhs);
    BOOL ReadText(CString &src, int *linenum);
    BOOL WriteText(CStdioFile &file);
    void Serialize(CArchive &ar);

    CString Name;
    int ID;
    int reserved;
    double XOffset;
    double YOffset;
    double ZOffset;
    double Roll;
    double Pitch;
    double Yaw;
    unsigned char Enable;
    unsigned char XOffsetEnable;
    unsigned char YOffsetEnable;
    unsigned char ZOffsetEnable;
    unsigned char RollEnable;
    unsigned char PitchEnable;
    unsigned char YawEnable;
    unsigned char reserved2;
};

#endif