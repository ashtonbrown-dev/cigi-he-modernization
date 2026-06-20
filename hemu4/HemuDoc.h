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

// HemuDoc.h : interface of the CHemuDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEMUDOC_H__8D5C335C_D421_4D30_94C1_844D0E9346C2__INCLUDED_)
#define AFX_HEMUDOC_H__8D5C335C_D421_4D30_94C1_844D0E9346C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataManager.h"
extern CDataManager g_DataManager;

class CEntityTemplMap;

class CHemuDoc : public CDocument
{
    friend class CDataManager;

protected: // create from serialization only
    CHemuDoc();
    DECLARE_DYNCREATE(CHemuDoc)
    CEntityTemplMap &GetEntityTemplMap(void);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CHemuDoc)
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive &ar);
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CHemuDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext &dc) const;
#endif

protected:
    //{{AFX_MSG(CHemuDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline CEntityTemplMap &CHemuDoc::GetEntityTemplMap(void)
{
    return g_DataManager.GetEntityTemplMap();
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEMUDOC_H__8D5C335C_D421_4D30_94C1_844D0E9346C2__INCLUDED_)
