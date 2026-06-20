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

#ifndef _CONFIGTEMPLINTERNAL_H_
#define _CONFIGTEMPLINTERNAL_H_

#include "stdafx.h"

#define ENTITY_CLASS_GENERIC        0
#define ENTITY_CLASS_FIXEDWING      1
#define ENTITY_CLASS_ANIMATION      2
#define ENTITY_CLASS_CLOUD          3
#define ENTITY_CLASS_MISSILE        4
#define ENTITY_CLASS_TANK           5
#define ENTITY_CLASS_CAR            6
#define ENTITY_CLASS_BOAT           7
#define ENTITY_CLASS_ROTORCRAFT     8
#define ENTITY_CLASS_BOMB           9
#define ENTITY_CLASS_SAM            10
#define ENTITY_CLASS_PATTERN        11
#define ENTITY_CLASS_CULTURE        12

extern const char DataTypeStr[6][7];

extern const char CompClassStr[14][17];

extern const char EntityClassStr[13][12];

extern const char EntityClampStr[3][13];

extern const char CollisionVolTypeStr[2][7];

extern const char ProjectionTypeStr[2][12];

extern const char ReplicationStr[4][5];

extern const char MirrorModeStr[4][11];

extern char s_Filename[256];

extern const int UNKNOWN;
extern const int SYMBOL;
extern const int KEYWORD;
extern const int DATATYPE;

///////////////////////////////////////////////////////////////////////////////
//
//  Common Functions
//
///////////////////////////////////////////////////////////////////////////////

void SetFilenameForConfigErrorMessages(LPCTSTR filename);

int StrToEntityClass(LPCTSTR str);

int StrToDatatype(LPCTSTR str);

int StrToCompClass(LPCTSTR str);

int StrToVolType(LPCTSTR str);

BOOL StrToBool(LPCTSTR str);

void MakeDate(LPCTSTR str, unsigned char *month, unsigned char *day, int *year);

void MakeTime(LPCTSTR str, unsigned char *hour, unsigned char *minute);

void PopupReadErrorMsg(const int type, const char *param, const int *linenum);

//  ReadBlock() --  Reads a block ("TOKEN{...}") from the given file.  Strips
//                  out comments.  The file position is incremented.
CString ReadBlock(CStdioFile &file, int *linenum);

//  ReadBlock() --  Reads a block ("TOKEN{...}") from the given string.  The
//                  block is removed from the source string.
CString ReadBlock(CString &src, int *linenum);

//  ReadLine() --   Reads a line ("TOKEN" or "TOKEN=VALUE;") from the given
//                  string.  The line is removed from the source string.
BOOL ReadLine(CString IN OUT &str, int *linenum, CString OUT &token, CString OUT &params);

//  ReadParameter() --  Reads a parameter from the given string.  The
//                      first parameter and remainder are returned.
BOOL ReadParameter(CString IN &str, CString OUT &param1, CString OUT &remainder);

#endif
