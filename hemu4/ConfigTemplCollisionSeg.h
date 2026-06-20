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

#ifndef _CONFIGTEMPLCOLLISIONSEG_H_
#define _CONFIGTEMPLCOLLISIONSEG_H_

#include "StdAfx.h"

class TEMPL_COLLISION_SEG : public CObject
{
public:
    DECLARE_SERIAL(TEMPL_COLLISION_SEG)

    TEMPL_COLLISION_SEG();
    ~TEMPL_COLLISION_SEG();
    TEMPL_COLLISION_SEG &operator=(const TEMPL_COLLISION_SEG &rhs);
    BOOL ReadText(CString &src, int *linenum);
    BOOL WriteText(CStdioFile &file);
    void Serialize(CArchive &ar);

    CString Name;
    int ID;
    int Enabled;
    unsigned long MaterialMask;
    int reserved;
    double X1;
    double Y1;
    double Z1;
    double X2;
    double Y2;
    double Z2;
};

#endif