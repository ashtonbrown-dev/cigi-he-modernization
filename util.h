#ifndef _UTIL_H_
#define _UTIL_H_

//#include <afxwin.h>
#include <windows.h>

//#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
//#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
//#define LOWORD(l)           ((WORD)(l))
//#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
//#define LOBYTE(w)           ((BYTE)(w))
//#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))




// Convert degrees to radians.
__forceinline double DegToRad(const double deg)
{
    return (deg * 0.0174532925199432957692369076848f);
}

// Convert radians to degrees.
__forceinline double RadToDeg(const double rad)
{
    return (rad * 57.295779513082320876798154814105f);
}

// Swap the 16-bit WORDs in a 32-bit DWORD.
__forceinline DWORD WORDSwap(const DWORD value)
{

    //DWORD dw = 0; return( dw);
    return (MAKELONG(HIWORD(value), LOWORD(value)));
}

// Swap the 32-bit DWORDs in a 64-bit value.
__forceinline double DWORDSwap(const double value)
{
    double ret = value;
    DWORD *w1 = (DWORD *)(&ret);
    DWORD *w2 = (DWORD *)(&ret) + 1;
    DWORD temp;

    temp = *w1;
    *w1 = *w2;
    *w2 = temp;

    return ret;
}

#endif  // _UTIL_H_