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
 *  FILENAME:   HemuDoc.cpp
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
#include "Entity.h"
#include "Hemu4.h"
#include "HemuDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHemuDoc

IMPLEMENT_DYNCREATE(CHemuDoc, CDocument)

BEGIN_MESSAGE_MAP(CHemuDoc, CDocument)
    //{{AFX_MSG_MAP(CHemuDoc)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHemuDoc construction/destruction

CHemuDoc::CHemuDoc()
{
    //ASSERT(::g_pDocument == NULL);
    //::g_pDocument = this;
}

CHemuDoc::~CHemuDoc()
{
    //::g_pDocument = NULL;
}

BOOL CHemuDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    g_DataManager.Reinitialize();
    theApp.EnsureDefaultOwnship();

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CHemuDoc serialization

void CHemuDoc::Serialize(CArchive &ar)
{
    if (ar.IsStoring()) {
        // Store all the necessary global variables.
        CEntity::SerializeStaticVars(ar);
        g_DataManager.Serialize(ar);
    } else {
        // Restore all the necessary global variables.
        CEntity::SerializeStaticVars(ar);
        g_DataManager.Serialize(ar);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CHemuDoc diagnostics

#ifdef _DEBUG
void CHemuDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CHemuDoc::Dump(CDumpContext &dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHemuDoc commands
