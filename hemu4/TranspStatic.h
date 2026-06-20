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

#if !defined(AFX_TRANSPSTATIC_H__33450580_1039_11D6_8023_00065B871B36__INCLUDED_)
#define AFX_TRANSPSTATIC_H__33450580_1039_11D6_8023_00065B871B36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TranspStatic.h : header file
//

#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CTranspStatic window

class CTranspStatic : public CStatic
{
public:
    CTranspStatic();
    virtual ~CTranspStatic();
    HBITMAP LoadBitmap(UINT nIDResource);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTranspStatic)
    //}}AFX_VIRTUAL

protected:
    CBitmap m_Bitmap;

    //{{AFX_MSG(CTranspStatic)
    afx_msg void OnPaint();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSPSTATIC_H__33450580_1039_11D6_8023_00065B871B36__INCLUDED_)
