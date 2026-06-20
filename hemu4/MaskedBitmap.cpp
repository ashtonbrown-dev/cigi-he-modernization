// MaskedBitmap.cpp: implementation of the CMaskedBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hemu4.h"
#include "MaskedBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMaskedBitmap::CMaskedBitmap()
{

}

CMaskedBitmap::~CMaskedBitmap()
{

}

void CMaskedBitmap::Draw(CDC *pDC, int x, int y)
{
    BITMAP bmp;
    GetObject(sizeof(BITMAP), &bmp);

    CPoint size(bmp.bmWidth, bmp.bmHeight);
    pDC->DPtoLP(&size);

    CPoint org(0, 0);
    pDC->DPtoLP(&org);

    CDC dcMem;
    dcMem.CreateCompatibleDC(pDC);
    CBitmap *pOldBitmap = dcMem.SelectObject(this);
    dcMem.SetMapMode(pDC->GetMapMode());

    pDC->BitBlt(x, y, size.x, size.y, &dcMem, org.x, org.y, SRCCOPY);

    dcMem.SelectObject(pOldBitmap);
}

void CMaskedBitmap::DrawTransparent(CDC *pDC, int x, int y, COLORREF crMaskColor)
{
    BITMAP bmp;
    GetObject(sizeof(BITMAP), &bmp);

    CPoint size(bmp.bmWidth, bmp.bmHeight);
    pDC->DPtoLP(&size);

    CPoint org(0, 0);
    pDC->DPtoLP(&org);

    // Create a memory DC and select the bitmap into it.
    CDC dcImage;
    dcImage.CreateCompatibleDC(pDC);
    CBitmap *pOldBmpImage = dcImage.SelectObject(this);
    dcImage.SetMapMode(pDC->GetMapMode());

    // Create a second memory DC and in it create an AND mask.
    CDC dcAnd;
    dcAnd.CreateCompatibleDC(pDC);
    dcAnd.SetMapMode(pDC->GetMapMode());

    CBitmap bmpAnd;
    bmpAnd.CreateBitmap(bmp.bmWidth, bmp.bmHeight, 1, 1, NULL);
    CBitmap *pOldBmpAnd = dcAnd.SelectObject(&bmpAnd);

    dcImage.SetBkColor(crMaskColor);
    dcAnd.BitBlt(org.x, org.y, size.x, size.y, &dcImage, org.x, org.y, SRCCOPY);

    // Create a third memor y DC and in it create an XOR mask.
    CDC dcXor;
    dcXor.CreateCompatibleDC(pDC);
    dcXor.SetMapMode(pDC->GetMapMode());

    CBitmap bmpXor;
    bmpXor.CreateCompatibleBitmap(&dcImage, bmp.bmWidth, bmp.bmHeight);
    CBitmap *pOldBmpXor = dcXor.SelectObject(&bmpXor);

    dcXor.BitBlt(org.x, org.y, size.x, size.y, &dcImage, org.x, org.y, SRCCOPY);
    dcXor.BitBlt(org.x, org.y, size.x, size.y, &dcAnd, org.x, org.y, 0x220326);

    // Copy the pixels in the destination rectangle to a temporary memory DC.
    CDC dcTemp;
    dcTemp.CreateCompatibleDC(pDC);
    dcTemp.SetMapMode(pDC->GetMapMode());

    CBitmap bmpTemp;
    bmpTemp.CreateCompatibleBitmap(&dcImage, bmp.bmWidth, bmp.bmHeight);
    CBitmap *pOldBmpTemp = dcTemp.SelectObject(&bmpTemp);

    dcTemp.BitBlt(org.x, org.y, size.x, size.y, pDC, x, y, SRCCOPY);

    // Generate the final image by applying the AND and XOR masks to the
    // image in the temporary memory DC.
    dcTemp.BitBlt(org.x, org.y, size.x, size.y, &dcAnd, org.x, org.y, SRCAND);
    dcXor.BitBlt(org.x, org.y, size.x, size.y, &dcXor, org.x, org.y, SRCINVERT);

    // Blit the resulting image to the screen.
    pDC->BitBlt(x, y, size.x, size.y, &dcTemp, org.x, org.y, SRCCOPY);

    // Restore the default bitmaps.
    dcTemp.SelectObject(pOldBmpTemp);
    dcXor.SelectObject(pOldBmpXor);
    dcAnd.SelectObject(pOldBmpAnd);
    dcImage.SelectObject(pOldBmpImage);
}
