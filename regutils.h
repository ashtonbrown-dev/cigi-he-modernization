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

#ifndef _REGUTILS_H_
#define _REGUTILS_H_

#include <afx.h>
#include <afxtempl.h>

////////////////////////////////////////////////////////////////
// Encryption/Decryption Routines
////////////////////////////////////////////////////////////////
BOOL SetupCryptoClient(void);
void SetCryptoPwd(const char *pwd);
const char *GetCryptoPwd(void);
const char *EncryptString(char *dest, const char *src, const int length);
const char *DecryptString(char *dest, const char *src, const int length);

////////////////////////////////////////////////////////////////
// Environment Variable Routines
////////////////////////////////////////////////////////////////
DWORD GetSystemEnvInt32(const char *varname, __int32 *val);
DWORD GetSystemEnvStr(const char *varname, CString *val);
DWORD GetSystemEnvStrExp(const char *varname, CString *val);
DWORD SetSystemEnvInt32(const char *varname, const __int32 val);
DWORD SetSystemEnvStr(const char *varname, const char *val);

inline DWORD SetSystemEnvStr(const char *varname, CString val)
{
    return SetSystemEnvStr(varname, (LPCTSTR)val);
}

////////////////////////////////////////////////////////////////
// Registry Routines
////////////////////////////////////////////////////////////////
DWORD GetRegEntryInt32(const char *keyname, const char *varname, __int32 *val);
DWORD GetRegEntryBin(const char *keyname, const char *varname, BYTE *val, __int32 size);
DWORD GetRegEntryStr(const char *keyname, const char *varname, CString *val);
DWORD GetRegEntryStr(const char *keyname, const char *varname, const char *val, __int32 *size);
DWORD GetRegEntryCrypto(const char *keyname, const char *varname, CString *val);
DWORD SetRegEntryInt32(const char *keyname, const char *varname, const __int32 val);
DWORD SetRegEntryBin(const char *keyname, const char *varname, BYTE *val, const __int32 size);
DWORD SetRegEntryStr(const char *keyname, const char *varname, const char *val);
DWORD SetRegEntryCrypto(const char *keyname, const char *varname, const char *val);

inline DWORD SetRegEntryStr(const char *keyname, const char *varname, CString val)
{
    return SetRegEntryStr(keyname, varname, (LPCTSTR)val);
}

inline DWORD SetRegEntryCrypt(const char *keyname, const char *varname, CString val)
{
    return SetRegEntryCrypto(keyname, varname, (LPCTSTR)val);
}

#endif