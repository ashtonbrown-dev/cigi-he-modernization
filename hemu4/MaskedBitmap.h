// MaskedBitmap.h: interface for the CMaskedBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASKEDBITMAP_H__762B9ABA_0DB7_4D9A_9183_C692493409B9__INCLUDED_)
#define AFX_MASKEDBITMAP_H__762B9ABA_0DB7_4D9A_9183_C692493409B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>

class CMaskedBitmap : public CBitmap
{
public:
    CMaskedBitmap();
    virtual ~CMaskedBitmap();
    virtual void Draw(CDC *pDC, int x, int y);
    virtual void DrawTransparent(CDC *pDC, int x, int y, COLORREF crMaskColor);

};

#endif // !defined(AFX_MASKEDBITMAP_H__762B9ABA_0DB7_4D9A_9183_C692493409B9__INCLUDED_)
