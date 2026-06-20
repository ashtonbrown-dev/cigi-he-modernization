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

#pragma once

#include "ConfigTemplView.h"
#include "TemplMap.h"

class CCigiView;
class CDataManager;
class CMainFrame;
class CViewGroup;

typedef CMap<int, int, CCigiView *, CCigiView *&> CViewMap;

class CViewTemplMap : public CTemplMap<TEMPL_VIEW>
{
public:
    CViewTemplMap() {}
    virtual ~CViewTemplMap() {}

    void CreateViews(CDataManager *dm,
                     CMainFrame *mainframe,
                     CViewGroup **viewGroupArray,
                     CViewMap *viewMap);

private:
    void ParseTemplateBlock(CString &str, const CString &token, int *linenum);
};
