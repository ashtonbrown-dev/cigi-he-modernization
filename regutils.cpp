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

// These lines are required for Wincrypt.h to compile.  They must be in
// this order, and before Afxwin.h is included.
//#ifndef WINVER
//#define _WIN32_WINNT    0x0400
//#define WINVER          0x0400
//#endif

#include <afx.h>
#include <afxwin.h>
#include <afxtempl.h>
#include <direct.h>     // for directory routines
#include <wincrypt.h>   // for cryptographic routines  (link with Advapi32.lib)

#define REG_BASE_KEY    "Boeing TSS"

CString g_CryptoPwd = "dljf098#!$%@odifu[da0t3Q$5fda'ivuda0=g$#Qp5i=re 09";


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   SetupCryptoClient()
//
//  Description:    Sets up the cryptographic client.  This should be called at
//                  the beginning of the program if that program uses any
//                  cryptography routines.
//
///////////////////////////////////////////////////////////////////////////////
BOOL SetupCryptoClient(void)
{
    // Ensure that the default cryptographic client is set up.
    HCRYPTPROV hProv;
    HCRYPTKEY hKey;

    // Attempt to acquire a handle to the default key container.
    if (!CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, 0)) {
        // Some sort of error occured, create default key
        // container.
        if (!CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET)) {
            // Error creating key container!
            return FALSE;
        }
    }

    // Attempt to get handle to signature key.
    if (!CryptGetUserKey(hProv, AT_SIGNATURE, &hKey)) {
        if (GetLastError() == NTE_NO_KEY) {
            // Create signature key pair.
            if (!CryptGenKey(hProv, AT_SIGNATURE, 0, &hKey)) {
                // Error during CryptGenKey!
                CryptReleaseContext(hProv, 0);

                return FALSE;
            } else {
                CryptDestroyKey(hKey);
            }
        } else {
            // Error during CryptGetUserKey!
            CryptReleaseContext(hProv, 0);

            return FALSE;
        }
    }

    // Attempt to get handle to exchange key.
    if (!CryptGetUserKey(hProv, AT_KEYEXCHANGE, &hKey)) {
        if (GetLastError() == NTE_NO_KEY) {
            // Create key exchange key pair.
            if (!CryptGenKey(hProv, AT_KEYEXCHANGE, 0, &hKey)) {
                // Error during CryptGenKey!
                CryptReleaseContext(hProv, 0);

                return FALSE;
            } else {
                CryptDestroyKey(hKey);
            }
        } else {
            // Error during CryptGetUserKey!
            CryptReleaseContext(hProv, 0);

            return FALSE;
        }
    }

    CryptReleaseContext(hProv, 0);

    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   SetCryptoPwd()
//
//  Description:    Sets the password that will be used to generate the
//                  cryptographic key.  Must be called before any encryption
//                  or decryption can be performed.
//
///////////////////////////////////////////////////////////////////////////////
void SetCryptoPwd(const char *pwd)
{
    g_CryptoPwd = CString(pwd);
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetCryptoPwd()
//
//  Description:    Gets the password that will be used to generate the
//                  cryptographic key.
//
///////////////////////////////////////////////////////////////////////////////
const char *GetCryptoPwd(void)
{
    return (LPCTSTR)g_CryptoPwd;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   EncryptString()
//
//  Description:    Encrypts scr and copies the encrypted string into a char
//                  buffer specified by dest.  dest must be allocated and must
//                  be at least the same length as src.  src and dest may point
//                  to the same buffer.  length is the size of both strings,
///                 including the terminating NULL.
//
//                  Returns the address of the encrypted string.
//
///////////////////////////////////////////////////////////////////////////////
const char *EncryptString(char *dest, const char *src, const int length)
{
    HCRYPTPROV hProv = NULL;
    HCRYPTKEY hKey = NULL;
    HCRYPTKEY hXchgKey = NULL;
    HCRYPTHASH hHash = NULL;
    DWORD dwLength;

    // Used to generate the key
    CString LocalPassword(GetCryptoPwd());

    // Get handle to user default provider.
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
        // Create hash object.
        if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
            // Hash password string.
            dwLength = LocalPassword.GetLength() * sizeof(TCHAR);

            if (CryptHashData(hHash, (BYTE *)LocalPassword.GetBuffer(dwLength), dwLength, 0)) {
                // Create block cipher session key based on hash of the
                // password.
                if (CryptDeriveKey(hProv, CALG_RC4, hHash, CRYPT_EXPORTABLE, &hKey)) {
                    // Make a copy of the value because we need to send a pointer to
                    // CrypteEncrypt()
                    dwLength = length;

                    // Allocate memory.
                    BYTE *pbBuffer = (BYTE *)calloc(length, 1);     // + 1

                    if (pbBuffer != NULL) {
                        // Copy the buffer.
                        memcpy(pbBuffer, src, dwLength);

                        // Encrypt data.
                        if (CryptEncrypt(hKey, 0, TRUE, 0, pbBuffer, &dwLength, dwLength)) {
                            // Now copy the buffer, excluding the NULL.
                            memcpy(dest, pbBuffer, dwLength - 1);
                        }

                        // Destroy the contents of the buffer.
                        memset(pbBuffer, 0, dwLength);

                        // Free memory.
                        free(pbBuffer);
                    }

                    CryptDestroyKey(hKey);  // Release provider handle.
                }
            }

            CryptDestroyHash(hHash); // Destroy session key.
        }

        CryptReleaseContext(hProv, 0);
    }

    return dest;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   DecryptString()
//
//  Description:    Decrypts scr and copies the unencrypted string into a char
//                  buffer specified by dest.  dest must be allocated and must
//                  be at least the same length as src.  src and dest may point
//                  to the same buffer.  length is the size of both buffers,
///                 including the terminating NULL.
//
//                  Returns the address of the unencrypted string.
//
///////////////////////////////////////////////////////////////////////////////
const char *DecryptString(char *dest, const char *src, const int length)
{
    HCRYPTPROV hProv = NULL;
    HCRYPTKEY hKey = NULL;
    HCRYPTKEY hXchgKey = NULL;
    HCRYPTHASH hHash = NULL;
    DWORD dwLength;
    CString retval;

    // Used to encrypt the real password
    CString LocalPassword(GetCryptoPwd());

    // Get handle to user default provider.
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
        // Create hash object.
        if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
            // Hash password string.
            dwLength = LocalPassword.GetLength() * sizeof(TCHAR);

            if (CryptHashData(hHash, (BYTE *)LocalPassword.GetBuffer(dwLength), dwLength, 0)) {
                // Create block cipher session key based on hash of the
                // password.
                if (CryptDeriveKey(hProv, CALG_RC4, hHash, CRYPT_EXPORTABLE, &hKey)) {
                    // Make a copy of the value because we need to send a pointer to
                    // CrypteDecrypt()
                    dwLength = length;

                    // Allocate memory.
                    BYTE *pbBuffer = (BYTE *)calloc(length, 1);

                    if (pbBuffer != NULL) {
                        // Copy the buffer.
                        memcpy(pbBuffer, src, dwLength);

                        // Decrypt data.
                        if (CryptDecrypt(hKey, 0, TRUE, 0, pbBuffer, &dwLength)) {
                            // Now copy the buffer, excluding the NULL.
                            memcpy(dest, pbBuffer, dwLength - 1);
                        }

                        // Destroy the contents of the buffer.
                        memset(pbBuffer, 0, dwLength);

                        // Free memory.
                        free(pbBuffer);
                    }

                    CryptDestroyKey(hKey);  // Release provider handle.
                }
            }

            CryptDestroyHash(hHash); // Destroy session key.
        }

        CryptReleaseContext(hProv, 0);
    }

    return dest;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetSystemEnvInt32()
//
//  Description:    Gets the specified system environment variable value as
//                  a 32-bit integer.
//
///////////////////////////////////////////////////////////////////////////////
DWORD GetSystemEnvInt32(const char *varname, __int32 *val)
{
    unsigned char tempbuf[1024];
    unsigned long len = 1024;
    DWORD retval;
    HKEY hkeySystem;
    HKEY hkeyControlSet;
    HKEY hkeyControl;
    HKEY hkeySessionMgr;
    HKEY hkeyEnvironment;

    // Get a handle to the system environment variables stored in the registry.
    RegOpenKeyEx(HKEY_CURRENT_USER, "SYSTEM", 0, KEY_READ, &hkeySystem);
    RegOpenKeyEx(hkeySystem, "CurrentControlSet", 0, KEY_READ, &hkeyControlSet);
    RegOpenKeyEx(hkeyControlSet, "Control", 0, KEY_READ, &hkeyControl);
    RegOpenKeyEx(hkeyControl, "Session Manager", 0, KEY_READ, &hkeySessionMgr);
    RegOpenKeyEx(hkeySessionMgr, "Environment", 0, KEY_READ, &hkeyEnvironment);

    // Now get the value.
    retval = RegQueryValueEx(hkeyEnvironment, varname, NULL, NULL, tempbuf, &len);

    // Close the handles to the registry keys.
    if (hkeyEnvironment)
        RegCloseKey(hkeyEnvironment);
    if (hkeySessionMgr)
        RegCloseKey(hkeySessionMgr);
    if (hkeyControl)
        RegCloseKey(hkeyControl);
    if (hkeyControlSet)
        RegCloseKey(hkeyControlSet);
    if (hkeySystem)
        RegCloseKey(hkeySystem);

    sscanf((LPCTSTR)tempbuf, "%ld", val);

    return retval;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetSystemEnvStr()
//
//  Description:    Gets the specified system environment variable value as
//                  a CString.
//
///////////////////////////////////////////////////////////////////////////////
DWORD GetSystemEnvStr(const char *varname, CString *val)
{
    unsigned char tempbuf[1024] = "";
    unsigned long len = 1024;
    DWORD retval;
    HKEY hkeySystem;
    HKEY hkeyControlSet;
    HKEY hkeyControl;
    HKEY hkeySessionMgr;
    HKEY hkeyEnvironment;

    // Get a handle to the system environment variables stored in the registry.
    RegOpenKeyEx(HKEY_CURRENT_USER, "SYSTEM", 0, KEY_READ, &hkeySystem);
    RegOpenKeyEx(hkeySystem, "CurrentControlSet", 0, KEY_READ, &hkeyControlSet);
    RegOpenKeyEx(hkeyControlSet, "Control", 0, KEY_READ, &hkeyControl);
    RegOpenKeyEx(hkeyControl, "Session Manager", 0, KEY_READ, &hkeySessionMgr);
    RegOpenKeyEx(hkeySessionMgr, "Environment", 0, KEY_READ, &hkeyEnvironment);

    // Now get the value.
    retval = RegQueryValueEx(hkeyEnvironment, varname, NULL, NULL, tempbuf, &len);

    // Close the handles to the registry keys.
    if (hkeyEnvironment)
        RegCloseKey(hkeyEnvironment);
    if (hkeySessionMgr)
        RegCloseKey(hkeySessionMgr);
    if (hkeyControl)
        RegCloseKey(hkeyControl);
    if (hkeyControlSet)
        RegCloseKey(hkeyControlSet);
    if (hkeySystem)
        RegCloseKey(hkeySystem);

    *val = CString(tempbuf);

    return retval;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetSystemEnvStrExp()
//
//  Description:    Gets the specified system environment variable value as
//                  a CString and expands any %VARIABLE% tokens.
//
///////////////////////////////////////////////////////////////////////////////
DWORD GetSystemEnvStrExp(const char *varname, CString *val)
{
    unsigned char tempbuf[1024] = "";
    unsigned char expbuf[1024] = "";
    unsigned long len = 1024;
    DWORD retval;
    HKEY hkeySystem;
    HKEY hkeyControlSet;
    HKEY hkeyControl;
    HKEY hkeySessionMgr;
    HKEY hkeyEnvironment;

    // Get a handle to the system environment variables stored in the registry.
    RegOpenKeyEx(HKEY_CURRENT_USER, "SYSTEM", 0, KEY_READ, &hkeySystem);
    RegOpenKeyEx(hkeySystem, "CurrentControlSet", 0, KEY_READ, &hkeyControlSet);
    RegOpenKeyEx(hkeyControlSet, "Control", 0, KEY_READ, &hkeyControl);
    RegOpenKeyEx(hkeyControl, "Session Manager", 0, KEY_READ, &hkeySessionMgr);
    RegOpenKeyEx(hkeySessionMgr, "Environment", 0, KEY_READ, &hkeyEnvironment);

    // Now get the value.
    retval = RegQueryValueEx(hkeyEnvironment, varname, NULL, NULL, tempbuf, &len);

    // Close the handles to the registry keys.
    if (hkeyEnvironment)
        RegCloseKey(hkeyEnvironment);
    if (hkeySessionMgr)
        RegCloseKey(hkeySessionMgr);
    if (hkeyControl)
        RegCloseKey(hkeyControl);
    if (hkeyControlSet)
        RegCloseKey(hkeyControlSet);
    if (hkeySystem)
        RegCloseKey(hkeySystem);

    // Expand environment variable tokens.
    ExpandEnvironmentStrings((LPCTSTR)tempbuf, (LPTSTR)expbuf, 1024);

    *val = CString(expbuf);

    return retval;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   SetSystemEnvInt32()
//
//  Description:    Converts the specified value to a string and sets the
//                  specified system environment variable value to that value.
//
///////////////////////////////////////////////////////////////////////////////
DWORD SetSystemEnvInt32(const char *varname, const __int32 val)
{
    HKEY hkeySystem;
    HKEY hkeyControlSet;
    HKEY hkeyControl;
    HKEY hkeySessionMgr;
    HKEY hkeyEnvironment;
    DWORD retval;
    CString tempstr;

    // Get a handle to the system environment variables stored in the registry.
    RegOpenKeyEx(HKEY_CURRENT_USER, "SYSTEM", 0, KEY_READ, &hkeySystem);
    RegOpenKeyEx(hkeySystem, "CurrentControlSet", 0, KEY_READ, &hkeyControlSet);
    RegOpenKeyEx(hkeyControlSet, "Control", 0, KEY_READ, &hkeyControl);
    RegOpenKeyEx(hkeyControl, "Session Manager", 0, KEY_READ, &hkeySessionMgr);
    RegOpenKeyEx(hkeySessionMgr, "Environment", 0, KEY_ALL_ACCESS, &hkeyEnvironment);

    // Now set the value.
    tempstr.Format("%ld", val);
    retval = RegSetValueEx(hkeyEnvironment, varname, NULL, REG_DWORD,
                           (const unsigned char *)(LPCTSTR)tempstr, tempstr.GetLength() + 1);

    // Close the handles to the registry keys.
    if (hkeyEnvironment)
        RegCloseKey(hkeyEnvironment);
    if (hkeySessionMgr)
        RegCloseKey(hkeySessionMgr);
    if (hkeyControl)
        RegCloseKey(hkeyControl);
    if (hkeyControlSet)
        RegCloseKey(hkeyControlSet);
    if (hkeySystem)
        RegCloseKey(hkeySystem);

    return retval;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   SetSystemEnvStr()
//
//  Description:    Sets the specified system environment variable to the
//                  specified value.
//
///////////////////////////////////////////////////////////////////////////////
DWORD SetSystemEnvStr(const char *varname, const char *val)
{
    CString tempstr;
    HKEY hkeySystem;
    HKEY hkeyControlSet;
    HKEY hkeyControl;
    HKEY hkeySessionMgr;
    HKEY hkeyEnvironment;
    DWORD retval;

    // Get a handle to the system environment variables stored in the registry.
    RegOpenKeyEx(HKEY_CURRENT_USER, "SYSTEM", 0, KEY_READ, &hkeySystem);
    RegOpenKeyEx(hkeySystem, "CurrentControlSet", 0, KEY_READ, &hkeyControlSet);
    RegOpenKeyEx(hkeyControlSet, "Control", 0, KEY_READ, &hkeyControl);
    RegOpenKeyEx(hkeyControl, "Session Manager", 0, KEY_READ, &hkeySessionMgr);
    RegOpenKeyEx(hkeySessionMgr, "Environment", 0, KEY_ALL_ACCESS, &hkeyEnvironment);

    // Now set the value.
    retval = RegSetValueEx(hkeyEnvironment, varname, NULL, REG_SZ,
                           (const unsigned char *)val, strlen(val) + 1);

    // Close the handles to the registry keys.
    if (hkeyEnvironment)
        RegCloseKey(hkeyEnvironment);
    if (hkeySessionMgr)
        RegCloseKey(hkeySessionMgr);
    if (hkeyControl)
        RegCloseKey(hkeyControl);
    if (hkeyControlSet)
        RegCloseKey(hkeyControlSet);
    if (hkeySystem)
        RegCloseKey(hkeySystem);

    return retval;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetRegEntryInt32()
//
//  Description:    Gets the specified registry value as a 32-bit integer.
//                  The value should be stored as a String value.  The
//                  registry value will be located in the key
//                  HKEY_CURRENT_USER/SOFTWARE/<REG_BASE_KEY>/"keyname",
//                  where "keyname" will either contain the value "Common"
//                  or the name of the application.
//
///////////////////////////////////////////////////////////////////////////////
DWORD GetRegEntryInt32(const char *keyname, const char *varname, __int32 *val)
{
    char regstring[1024];
    HKEY regkey_software;
    HKEY regkey_approot;
    HKEY regkey_key;
    DWORD keyval_length;    // = 1024;
    DWORD retval;

    RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE", 0, KEY_ALL_ACCESS, &regkey_software);
    RegCreateKeyEx(regkey_software, REG_BASE_KEY, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_approot, 0);

    // Create or open the application's subkey.
    RegCreateKeyEx(regkey_approot, keyname, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_key, 0);

    // Retrieve the value.  Do this twice because the first time doesn't always
    // work.
    retval = RegQueryValueEx(regkey_key, varname, NULL, NULL,
                             (unsigned char *)regstring, &keyval_length);
    retval = RegQueryValueEx(regkey_key, varname, NULL, NULL,
                             (unsigned char *)regstring, &keyval_length);
    if (retval == ERROR_SUCCESS) {
        sscanf(regstring, "%ld", val);
    }

    // Close the keys.
    RegCloseKey(regkey_software);
    RegCloseKey(regkey_approot);
    RegCloseKey(regkey_key);

    return retval;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetRegEntryBin()
//
//  Description:    Gets the specified registry value as a BYTE array.  The
//                  registry value will be located in the key
//                  HKEY_CURRENT_USER/SOFTWARE/<REG_BASE_KEY>/"keyname",
//                  where "keyname" will either contain the value "Common"
//                  or the name of the application.
//
///////////////////////////////////////////////////////////////////////////////
DWORD GetRegEntryBin(const char *keyname, const char *varname, BYTE *val, __int32 *length)
{
    HKEY regkey_software;
    HKEY regkey_approot;
    HKEY regkey_key;
    DWORD retval;

    RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE", 0, KEY_ALL_ACCESS, &regkey_software);
    RegCreateKeyEx(regkey_software, REG_BASE_KEY, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_approot, 0);

    // Create or open the application's subkey.
    RegCreateKeyEx(regkey_approot, keyname, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_key, 0);

    // Retrieve the value.  Do this twice because the first time doesn't always
    // work.
    retval = RegQueryValueEx(regkey_key, varname, NULL, NULL, val, (unsigned long *)length);
    retval = RegQueryValueEx(regkey_key, varname, NULL, NULL, val, (unsigned long *)length);

    // Close the keys.
    RegCloseKey(regkey_software);
    RegCloseKey(regkey_approot);
    RegCloseKey(regkey_key);

    return retval;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetRegEntryStr()
//
//  Description:    Gets the specified registry value as a CString.
//                  The value should be stored as a String value.  The
//                  registry value will be located in the key
//                  HKEY_CURRENT_USER/SOFTWARE/<REG_BASE_KEY>/"keyname",
//                  where "keyname" will either contain the value "Common"
//                  or the name of the application.
//
///////////////////////////////////////////////////////////////////////////////
DWORD GetRegEntryStr(const char *keyname, const char *varname, CString *val)
{
    char regstring[1024];
    HKEY regkey_software;
    HKEY regkey_approot;
    HKEY regkey_key;
    DWORD keyval_length = 1024;
    DWORD retval;

    RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE", 0, KEY_ALL_ACCESS, &regkey_software);
    RegCreateKeyEx(regkey_software, REG_BASE_KEY, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_approot, 0);

    // Create or open the application's subkey.
    RegCreateKeyEx(regkey_approot, keyname, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_key, 0);

    // Retrieve the value.
    retval = RegQueryValueEx(regkey_key, varname, NULL, NULL,
                             (unsigned char *)regstring, &keyval_length);
    if (retval == ERROR_SUCCESS) {
        *val = CString(regstring);
    }

    // Close the keys.
    RegCloseKey(regkey_software);
    RegCloseKey(regkey_approot);
    RegCloseKey(regkey_key);

    return retval;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   SetRegEntryInt32()
//
//  Description:    Converts the specified 32-bit integer to a string and
//                  Sets the specified registry value to that string.  The
//                  registry value will be located in the key
//                  HKEY_CURRENT_USER/SOFTWARE/<REG_BASE_KEY>/"keyname",
//                  where "keyname" will either contain the value "Common"
//                  or the name of the application.
//
///////////////////////////////////////////////////////////////////////////////
DWORD SetRegEntryInt32(const char *keyname, const char *varname, const __int32 val)
{
    HKEY regkey_software;
    HKEY regkey_approot;
    HKEY regkey_key;
    DWORD retval;
    CString tempstr;

    RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE", 0, KEY_ALL_ACCESS, &regkey_software);
    RegCreateKeyEx(regkey_software, REG_BASE_KEY, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_approot, 0);

    // Create or open the subkey.
    RegCreateKeyEx(regkey_approot, keyname, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_key, 0);

    // Set the value.
    tempstr.Format("%ld", val);
    retval = RegSetValueEx(regkey_key, varname, NULL, REG_SZ,
                           (const unsigned char *)(LPCTSTR)tempstr, tempstr.GetLength() + 1);

    // Close the keys.
    RegCloseKey(regkey_software);
    RegCloseKey(regkey_approot);
    RegCloseKey(regkey_key);

    return retval;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   SetRegEntryBin()
//
//  Description:    Sets the specified registry value to the specified BYTE
//                  array.  The registry value will be located in the key
//                  HKEY_CURRENT_USER/SOFTWARE/<REG_BASE_KEY>/"keyname",
//                  where "keyname" will either contain the value "Common"
//                  or the name of the application.
//
///////////////////////////////////////////////////////////////////////////////
DWORD SetRegEntryBin(const char *keyname, const char *varname, const BYTE *val, const __int32 length)
{
    HKEY regkey_software;
    HKEY regkey_approot;
    HKEY regkey_key;
    DWORD retval;
    CString tempstr;

    RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE", 0, KEY_ALL_ACCESS, &regkey_software);
    RegCreateKeyEx(regkey_software, REG_BASE_KEY, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_approot, 0);

    // Create or open the application's subkey.
    RegCreateKeyEx(regkey_approot, keyname, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_key, 0);

    // Set the value.
    retval = RegSetValueEx(regkey_key, varname, NULL, REG_DWORD, val, length);

    // Close the keys.
    RegCloseKey(regkey_software);
    RegCloseKey(regkey_approot);
    RegCloseKey(regkey_key);

    return retval;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   SetRegEntryStr()
//
//  Description:    Sets the specified registry value to the specified string.
//                  The value should be stored as a String value.  The
//                  registry value will be located in the key
//                  HKEY_CURRENT_USER/SOFTWARE/<REG_BASE_KEY>/"keyname",
//                  where "keyname" will either contain the value "Common"
//                  or the name of the application.
//
///////////////////////////////////////////////////////////////////////////////
DWORD SetRegEntryStr(const char *keyname, const char *varname, const char *val)
{
    HKEY regkey_software;
    HKEY regkey_approot;
    HKEY regkey_key;
    DWORD retval;

    RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE", 0, KEY_ALL_ACCESS, &regkey_software);
    RegCreateKeyEx(regkey_software, REG_BASE_KEY, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_approot, 0);

    // Create or open the Resource Manager subkey.
    RegCreateKeyEx(regkey_approot, keyname, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_key, 0);

    // Set the value.
    retval = RegSetValueEx(regkey_key, varname, NULL, REG_SZ,
                           (const unsigned char *)val, strlen(val) + 1);

    // Close the keys.
    RegCloseKey(regkey_software);
    RegCloseKey(regkey_approot);
    RegCloseKey(regkey_key);

    return retval;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetRegEntryStr()
//
//  Description:    Gets the specified registry value as a CString.
//                  The value should be stored as a String value.  The
//                  registry value will be located in the key
//                  HKEY_CURRENT_USER/SOFTWARE/Boeing TSS/"keyname",
//                  where "keyname" will either contain the value "Common"
//                  or the name of the application.
//
///////////////////////////////////////////////////////////////////////////////
DWORD GetRegEntryStr(const char *keyname, const char *varname, const char *val, __int32 *size)
{
    //char regstring[1024];
    HKEY regkey_software;
    HKEY regkey_root;
    HKEY regkey_key;
    //DWORD keyval_length = 1024;
    DWORD retval;

    RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE", 0, KEY_ALL_ACCESS, &regkey_software);
    RegCreateKeyEx(regkey_software, REG_BASE_KEY, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_root, 0);

    // Create or open the Resource Manager subkey.
    RegCreateKeyEx(regkey_root, keyname, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_key, 0);

    // Retrieve the value.
    retval = RegQueryValueEx(regkey_key, varname, NULL, NULL,
                             (unsigned char *)val, (unsigned long *)size);

    // Close the keys.
    RegCloseKey(regkey_software);
    RegCloseKey(regkey_root);
    RegCloseKey(regkey_key);

    return retval;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   SetRegEntryCrypto()
//
//  Description:    Encrypts the specified string using g_CryptKey as the key.
//                  Sets the specified registry value to the encrypted string.
//                  The value should be stored as a String value.  The
//                  registry value will be located in the key
//                  HKEY_CURRENT_USER/SOFTWARE/<REG_BASE_KEY>/"keyname",
//                  where "keyname" will either contain the value "Common"
//                  or the name of the application.
//
///////////////////////////////////////////////////////////////////////////////
DWORD SetRegEntryCrypto(const char *keyname, const char *varname, const char *val)
{
    HKEY regkey_software;
    HKEY regkey_approot;
    HKEY regkey_key;
    DWORD retval;

    RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE", 0, KEY_ALL_ACCESS, &regkey_software);
    RegCreateKeyEx(regkey_software, REG_BASE_KEY, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_approot, 0);

    // Create or open the application's subkey.
    RegCreateKeyEx(regkey_approot, keyname, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_key, 0);

    // Encrypt the string.
    int length = strlen(val) + 1;
    BYTE *byte_array = new BYTE[length];
    memset(byte_array, 0, length);
    EncryptString((char *)byte_array, val, length);

    // Set the value.  Exclude the NULL.
    retval = RegSetValueEx(regkey_key, varname, NULL, REG_BINARY, byte_array, length - 1);

    // Close the keys.
    RegCloseKey(regkey_software);
    RegCloseKey(regkey_approot);
    RegCloseKey(regkey_key);

    delete [] byte_array;

    return retval;
}


///////////////////////////////////////////////////////////////////////////////
//
//  Name:   GetRegEntryCrypto()
//
//  Description:    Gets the specified registry value and decrypts that string
//                  using g_CryptKey as the key.  The value should be stored
//                  as a String value.  The registry value will be located in
//                  the key HKEY_CURRENT_USER/SOFTWARE/<REG_BASE_KEY>/"keyname",
//                  where "keyname" will either contain the value "Common"
//                  or the name of the application.
//
///////////////////////////////////////////////////////////////////////////////
DWORD GetRegEntryCrypto(const char *keyname, const char *varname, CString *val)
{
    BYTE byte_array[1024];
    HKEY regkey_software;
    HKEY regkey_approot;
    HKEY regkey_key;
    DWORD keyval_length = 1024;
    DWORD retval;

    RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE", 0, KEY_ALL_ACCESS, &regkey_software);
    RegCreateKeyEx(regkey_software, REG_BASE_KEY, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_approot, 0);

    // Create or open the application's subkey.
    RegCreateKeyEx(regkey_approot, keyname, 0, "", REG_OPTION_NON_VOLATILE,
                   KEY_ALL_ACCESS, NULL, &regkey_key, 0);

    memset(byte_array, 0, 1024);

    // Retrieve and decrypt the value.
    retval = RegQueryValueEx(regkey_key, varname, NULL, NULL, byte_array, &keyval_length);
    if (retval == ERROR_SUCCESS) {
        // safe to decrypt into the same buffer
        DecryptString((char *)byte_array, (const char *)byte_array, keyval_length + 1);
        *val = CString((LPCTSTR)byte_array);
    }

    // Close the keys.
    RegCloseKey(regkey_software);
    RegCloseKey(regkey_approot);
    RegCloseKey(regkey_key);

    return retval;
}
