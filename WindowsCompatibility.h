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

#ifndef WINDOWS_COMPATIBILITY_H
#define WINDOWS_COMPATIBILITY_H

#include <windows.h>

enum HemuClock { HEMU_CLOCK_FASTEST };

extern int g_sleepRoundValue;
extern __int64 g_highPerformanceClockScale;

inline BOOL InitializeHighResolutionClock(HemuClock, PLARGE_INTEGER value)
{
    if (!QueryPerformanceFrequency(value))
        return FALSE;

    g_highPerformanceClockScale = value->QuadPart / 10000000;
    return TRUE;
}

inline BOOL UseTruncatedSleepDurations(HemuClock, PLARGE_INTEGER)
{
    g_sleepRoundValue = 0;
    return TRUE;
}

inline BOOL GetHighResolutionClockTime(HemuClock, PLARGE_INTEGER value)
{
    if (g_highPerformanceClockScale) {
        QueryPerformanceCounter(value);
        value->QuadPart /= g_highPerformanceClockScale;
        return TRUE;
    }

    FILETIME fileTime;
    GetSystemTimeAsFileTime(&fileTime);
    value->LowPart = fileTime.dwLowDateTime;
    value->HighPart = fileTime.dwHighDateTime;
    return TRUE;
}

inline HANDLE CreateSharedMemory(DWORD protection,
                                 DWORD maximumSizeHigh,
                                 DWORD maximumSizeLow,
                                 LPCTSTR name,
                                 VOID **location)
{
    HANDLE handle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, protection,
                                      maximumSizeHigh, maximumSizeLow, name);
    if (!handle) {
        *location = NULL;
        return NULL;
    }

    *location = MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (!*location) {
        CloseHandle(handle);
        return NULL;
    }

    return handle;
}

inline void SleepForFileTime(PLARGE_INTEGER duration)
{
    Sleep(ULONG((duration->QuadPart + g_sleepRoundValue) / 10000));
}

#endif  // WINDOWS_COMPATIBILITY_H
