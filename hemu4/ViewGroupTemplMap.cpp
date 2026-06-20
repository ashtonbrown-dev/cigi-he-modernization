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

#include "globals.h"
#include "MainFrm.h"
#include "ViewGroup.h"
#include "ViewGroupTemplMap.h"

void CViewGroupTemplMap::CreateViewGroups(CMainFrame *mainframe,
                                          CViewGroup **viewGroupArray)
{
    POSITION pos = GetStartPosition();

    while (pos) {
        int id = 0;
        TEMPL_VIEWGROUP *templ = NULL;

        GetNextAssoc(pos, id, templ);

        if (viewGroupArray[id] == NULL)
            viewGroupArray[id] = new CViewGroup(templ);

        HTREEITEM hitem = mainframe->AddItemToViewTree(viewGroupArray[id]->GetName(),
                                                       IMG_VIEWGROUP,
                                                       MAKE_TREE_DATA(viewGroupArray[id]->GetGroupID(), OBJECT_TYPE_VIEWGROUP, 0));
        viewGroupArray[id]->SetHtree(hitem);
    }
}

/// Handles the parsing of "VIEWGROUP" blocks when encountered in the Views.def file.
void CViewGroupTemplMap::ParseTemplateBlock(CString &str, const CString &token, int *linenum)
{
    if (token == "VIEWGROUP") {
        TEMPL_VIEWGROUP *templ = new TEMPL_VIEWGROUP;
        templ->ReadText(str, linenum);
        SetAt(templ->GroupID, templ);
    }
}
