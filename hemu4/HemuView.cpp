/** <pre>
 *  Copyright 2004 The Boeing Company
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
 *
 *  FILENAME:   HemuView.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface Host Emulator (CIGI HE)
 *
 *  PROGRAM DESCRIPTION:
 *  The Host Emulator (HE) acts as a surrogate in the absence of a simulation host.
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME
 *  DESCRIPTION OF CHANGE
 *
 *  11/11/2004 Lance Durham
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  Version 3.1.1
 */

#include "stdafx.h"
#include "Hemu4.h"

#include "HemuDoc.h"
#include "HemuView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHemuView

IMPLEMENT_DYNCREATE(CHemuView, CView)

BEGIN_MESSAGE_MAP(CHemuView, CView)
    //{{AFX_MSG_MAP(CHemuView)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHemuView construction/destruction

CHemuView::CHemuView()
{
}

CHemuView::~CHemuView()
{
}

BOOL CHemuView::PreCreateWindow(CREATESTRUCT &cs)
{
    return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CHemuView drawing

void CHemuView::OnDraw(CDC *pDC)
{
    CHemuDoc *pDoc = GetDocument();
    ASSERT_VALID(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CHemuView diagnostics

#ifdef _DEBUG
void CHemuView::AssertValid() const
{
    CView::AssertValid();
}

void CHemuView::Dump(CDumpContext &dc) const
{
    CView::Dump(dc);
}

CHemuDoc *CHemuView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHemuDoc)));
    return (CHemuDoc *)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHemuView message handlers
