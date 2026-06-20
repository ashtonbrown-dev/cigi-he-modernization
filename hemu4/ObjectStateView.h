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

#if !defined(AFX_OBJECTSTATEVIEW_H__69799C87_FD1C_4A45_B6D9_F5ADCDBA57CE__INCLUDED_)
#define AFX_OBJECTSTATEVIEW_H__69799C87_FD1C_4A45_B6D9_F5ADCDBA57CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectStateView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjectStateView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "EntityStateView.h"
#include "ViewStateView.h"
#include "ViewGroupStateView.h"
#include "WeatherStateView.h"
#include "SkyStateView.h"
#include "TerrainStateView.h"
#include "globals.h"

const int VIEW_COUNT = 6;   //NUM_OBJECT_TYPES;

class CObjectStateView : public CFormView
{
public:
    void RefreshActiveView(void);
    void RefreshAllViews(void);
    int SetActiveView(const int view);
    int GetActiveViewNum(void);
    CBaseStateView *GetActiveView(void);
    CEntityStateView &GetEntityStateView(void);
    CViewStateView &GetViewStateView(void);
    CViewGroupStateView &GetViewGroupStateView(void);
    CWeatherStateView &GetWeatherStateView(void);
    CSkyStateView &GetSkyStateView(void);
    CTerrainStateView &GetTerrainStateView(void);

    //{{AFX_DATA(CObjectStateView)
    enum { IDD = IDD_BLANK_STATE_VIEW };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CObjectStateView)
public:
    virtual void OnInitialUpdate();
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

protected:
    BOOL m_Initialized;
    int m_ActiveViewNum;
    CBaseStateView *m_Views[VIEW_COUNT];
    DLGTEMPLATE *m_DlgTempl[VIEW_COUNT];
    CEntityStateView        m_EntityView;
    CViewStateView          m_ViewView;
    CViewGroupStateView     m_ViewgroupView;
    CWeatherStateView       m_WeatherView;
    CSkyStateView           m_SkyView;
    CTerrainStateView       m_TerrainView;

    CObjectStateView();           // protected constructor used by dynamic creation
    virtual ~CObjectStateView();
    DECLARE_DYNCREATE(CObjectStateView)
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext &dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CObjectStateView)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

inline void CObjectStateView::RefreshActiveView()
{
    if ((unsigned int)m_ActiveViewNum < VIEW_COUNT)
        m_Views[m_ActiveViewNum]->RefreshView();
}

inline void CObjectStateView::RefreshAllViews(void)
{
    int i;
    for (i = 0; i < VIEW_COUNT; i++)
        m_Views[i]->RefreshView();
}

inline int CObjectStateView::GetActiveViewNum(void)
{
    return m_ActiveViewNum;
}

inline CBaseStateView *CObjectStateView::GetActiveView(void)
{
    if (m_ActiveViewNum < 0)
        return NULL;
    else
        return m_Views[m_ActiveViewNum];
}

inline CEntityStateView &CObjectStateView::GetEntityStateView(void)
{
    return m_EntityView;
}

inline CViewStateView &CObjectStateView::GetViewStateView(void)
{
    return m_ViewView;
}

inline CViewGroupStateView &CObjectStateView::GetViewGroupStateView(void)
{
    return m_ViewgroupView;
}

inline CWeatherStateView &CObjectStateView::GetWeatherStateView(void)
{
    return m_WeatherView;
}

inline CSkyStateView &CObjectStateView::GetSkyStateView(void)
{
    return m_SkyView;
}

inline CTerrainStateView &CObjectStateView::GetTerrainStateView(void)
{
    return m_TerrainView;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTSTATEVIEW_H__69799C87_FD1C_4A45_B6D9_F5ADCDBA57CE__INCLUDED_)
