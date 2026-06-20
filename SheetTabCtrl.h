#if !defined(AFX_SHEETTABCTRL_H__6E61A73E_BBC0_4252_A608_27CBF593137A__INCLUDED_)
#define AFX_SHEETTABCTRL_H__6E61A73E_BBC0_4252_A608_27CBF593137A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SheetTabCtrl.h : header file
//

#include "stdafx.h"
#include "BaseTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSheetTabCtrl window

class CSheetTabCtrl : public CBaseTabCtrl
{
    CSheetTabCtrl();
    virtual ~CSheetTabCtrl();

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSheetTabCtrl)
    //}}AFX_VIRTUAL

protected:
    //{{AFX_MSG(CSheetTabCtrl)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETTABCTRL_H__6E61A73E_BBC0_4252_A608_27CBF593137A__INCLUDED_)
