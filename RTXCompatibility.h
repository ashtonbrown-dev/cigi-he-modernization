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

#ifndef NO_RTX
#include "rtapi.h"
#else

#include "HemuRtDrv/StdAfx.h"

#ifndef _RTXCOMPATIBILITY_H_
#define _RTXCOMPATIBILITY_H_

#define RT_PRIORITY_MIN     THREAD_PRIORITY_IDLE
#define RT_PRIORITY_MAX     THREAD_PRIORITY_ABOVE_NORMAL
//#define RT_PRIORITY_MAX       THREAD_PRIORITY_TIME_CRITICAL

// Define RTAPI function calling convention unless an override is in effect.
#if (defined(_MSC_VER)&&(_MSC_VER >= 800)) || defined(_STDCALL_SUPPORTED)
#define RTAPI __stdcall
#else
#define RTAPI
#endif

// Define the declaration for application provided functions.
#define RTFCNDCL RTAPI

#define RtPrintf    printf

enum CLOCK { CLOCK_1, CLOCK_2, CLOCK_FASTEST, CLOCK_SYSTEM };

extern int sleep_round_val;
extern __int64 hp_clock_freq;

// Note: This function actually obtains the FREQUENCY of the high-
// performance counter.  We are using the RtGetClockTimerPeriod()
// function simply as a convenience.
inline BOOL RtGetClockTimerPeriod(CLOCK Clock, PLARGE_INTEGER pTime)
{
    if (QueryPerformanceFrequency(pTime) == FALSE)
        return FALSE;

    hp_clock_freq = pTime->QuadPart / 10000000;

    return TRUE;
}

// Note: This function actually sets RtSleepFt() to truncate instead of
// round.  We are using the RtGetClockResolution() function simply
// as a convenience.
inline BOOL RtGetClockResolution(CLOCK Clock, PLARGE_INTEGER pTime)
{
    sleep_round_val = 0;

    return TRUE;
}

inline BOOL RtGetClockTime(CLOCK Clock, PLARGE_INTEGER pTime)
{
    if (hp_clock_freq) {
        QueryPerformanceCounter(pTime);
        pTime->QuadPart /= hp_clock_freq;
        return TRUE;
    } else {
        static BOOL hasrun = FALSE;
        if (!hasrun) {
			// chas
            //MessageBox(NULL, "High-performance counter functions are not supported on this system. "
            //           "Using kernel-mode system time functions. This will degrade performance.",
            //           "WARNING", MB_OK | MB_ICONWARNING | MB_SYSTEMMODAL);
			// chas
            hasrun = TRUE;
        }

        /*
        This code is simpler, since GetSystemTimeAsFileTime() and
        RtGetClockTime() both return 100ns units.  But it is slow
        because GetSystemTimeAsFileTime() jumps to kernel mode and
        back every time.  But it is our only option if the
        high-performance counters aren't supported.
        */
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        *pTime = *((LARGE_INTEGER *)&ft);

        return TRUE;
    }
}

inline HANDLE RtCreateSharedMemory(DWORD flProtect,
                                   DWORD MaximumSizeHigh,
                                   DWORD MaximumSizeLow,
                                   LPCTSTR lpName,
                                   VOID **location)
{
    HANDLE handle = CreateFileMapping((HANDLE) - 1, NULL, flProtect,
                                      MaximumSizeHigh, MaximumSizeLow,
                                      lpName);
    *location = MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    return handle;
}

inline HANDLE RtCreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes,
                            BOOL bInitialOwner,
                            LPCTSTR lpName)
{
    return CreateMutex(lpMutexAttributes, bInitialOwner, lpName);
}

inline HANDLE RtOpenMutex(DWORD DesiredAccess, BOOL bInheritHandle, LPCTSTR lpName)
{
    return OpenMutex(DesiredAccess, bInheritHandle, lpName);
}

inline BOOL RtReleaseMutex(HANDLE hMutex)
{
    return ReleaseMutex(hMutex);
}

inline HANDLE RtCreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes,
                            BOOL ManualReset,
                            BOOL pInitialState,
                            LPCTSTR lpName)
{
    return CreateEvent(lpEventAttributes, ManualReset, pInitialState, lpName);
}

inline BOOL RtSetEvent(HANDLE hEvent)
{
    return SetEvent(hEvent);
}

inline BOOL RtResetEvent(HANDLE hEvent)
{
    return ResetEvent(hEvent);
}

inline BOOL RtPulseEvent(HANDLE hEvent)
{
    return PulseEvent(hEvent);
}

inline BOOL RtCreateProcess(LPCTSTR lpApplicationName,
                            LPTSTR lpCommandLine,
                            LPSECURITY_ATTRIBUTES lpProcessAttributes,
                            LPSECURITY_ATTRIBUTES lpThreadAttributes,
                            BOOL bInheritHandles,
                            DWORD dwCreationFlags,
                            LPVOID lpEnvironment,
                            LPCTSTR lpCurrentDirectory,
                            LPSTARTUPINFO lpStartupInfo,
                            LPPROCESS_INFORMATION lpProcessInformation)
{
    return CreateProcess(lpApplicationName,
                         lpCommandLine,
                         lpProcessAttributes,
                         lpThreadAttributes,
                         bInheritHandles,
                         dwCreationFlags,
                         lpEnvironment,
                         lpCurrentDirectory,
                         lpStartupInfo,
                         lpProcessInformation);

}

inline BOOL RtCloseHandle(HANDLE hObject)
{
    BOOL retval;
    DWORD info;

    // See if the handle is valid and can be closed.
    retval = GetHandleInformation(hObject, &info);

    if (retval && (info & HANDLE_FLAG_PROTECT_FROM_CLOSE))
        return CloseHandle(hObject);
    else
        return FALSE;
}

inline DWORD RtWaitForSingleObject(HANDLE Handle, DWORD Milliseconds)
{
    return WaitForSingleObject(Handle, Milliseconds);
}

inline DWORD RtWaitForMultipleObjects(DWORD count, const HANDLE *handles, BOOL fWaitAll, DWORD Milliseconds)
{
    return WaitForMultipleObjects(count, handles, fWaitAll, Milliseconds);
}

inline void RtSleep(ULONG milliSeconds)
{
    Sleep(milliSeconds);
}

inline void RtSleepFt(PLARGE_INTEGER pDuration)
{
    Sleep(ULONG((pDuration->QuadPart + sleep_round_val) / 10000));  // round OR truncate
}


inline HANDLE RtCreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes,
                             DWORD StackSize,
                             LPTHREAD_START_ROUTINE lpStartAddress,
                             LPVOID lpParameter,
                             DWORD dwCreationFlags,
                             LPDWORD lpThreadId)
{
    return CreateThread(lpThreadAttributes, StackSize, lpStartAddress,
                        lpParameter, dwCreationFlags, lpThreadId);
}

inline DWORD RtResumeThread(HANDLE hThread)
{
    return ResumeThread(hThread);
}

inline BOOL RtSetThreadPriority(HANDLE hThread, int nPriority)
{
    return SetThreadPriority(hThread, nPriority);
}

#endif  // _RTXCOMPATIBILITY_H_
#endif  // NO_RTX
