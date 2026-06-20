#include "afxwin.h"         // windows.h
#include "perftimer.h"


namespace
{
__int64 g_CounterFreqHz, g_CounterFreqMHz;
double g_CounterPeriodSec, g_CounterPeriodMicsec;
}


__int64 GetCounterFreqSec(void)
{
    return g_CounterFreqHz;
}

__int64 GetCounterFreqMicsec(void)
{
    return g_CounterFreqMHz;
}

double GetCounterPeriodSec(void)
{
    return g_CounterPeriodSec;
}

double GetCounterPeriodMicsec(void)
{
    return g_CounterPeriodMicsec;
}

void InitPerfTimer(void)
{
    LARGE_INTEGER freq;

    // Get the number of tics per second on this machine.
    QueryPerformanceFrequency(&freq);

    // This is the frequency in terms of seconds.  We can easily find
    // it in terms of microseconds.
    g_CounterFreqHz = freq.QuadPart;
    g_CounterFreqMHz = unsigned __int64(freq.QuadPart / 1000000);

    // From these, we can find the period of each tic in seconds and
    // microseconds.
    g_CounterPeriodSec = (double)(1) / g_CounterFreqHz;
    g_CounterPeriodMicsec = (double)(1) / g_CounterFreqMHz;
}


void WaitForMicsec(const unsigned long duration)
{
    LARGE_INTEGER time1, time2;
    __int64 timeout = g_CounterFreqMHz * duration;

    QueryPerformanceCounter(&time1);

    // Sleep until almost time.
    Sleep((duration / 1000) - 100);

    do {
        QueryPerformanceCounter(&time2);
    } while (time2.QuadPart - time1.QuadPart < timeout);
}

void WaitUntil(const __int64 tic)
{
    LARGE_INTEGER time;
    __int64 t;

    // Sleep until almost time.
    QueryPerformanceCounter(&time);
    t = ((tic - time.QuadPart) * g_CounterPeriodMicsec) + 100;
    if (t < 0)
        return;
    Sleep(t / 1000);

    do {
        QueryPerformanceCounter(&time);
    } while (time.QuadPart < tic);
}

__int64 GetTickAtInterval(const unsigned long microsec)
{
    LARGE_INTEGER time;

    QueryPerformanceCounter(&time);

    return (time.QuadPart + (g_CounterFreqMHz * (__int64)microsec));
}

__int64 GetTicksDuringInterval(const unsigned long microsec)
{
    return (g_CounterFreqMHz * (__int64)microsec);
}

double GetActualFreq(void)
{
    static __int64 lastval = 0;
    double retval;
    LARGE_INTEGER time;

    QueryPerformanceCounter(&time);
    retval = (int)(lastval != 0) / (double)((unsigned long)time.QuadPart - (unsigned long)lastval);
    lastval = time.QuadPart;

    return retval;
}