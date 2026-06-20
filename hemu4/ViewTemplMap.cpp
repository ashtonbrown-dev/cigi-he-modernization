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

#include "CigiView.h"
#include "DataManager.h"
#include "globals.h"
#include "MainFrm.h"
#include "ViewGroup.h"
#include "ViewTemplMap.h"

void CViewTemplMap::CreateViews(CDataManager *dm,
                                CMainFrame *mainframe,
                                CViewGroup **viewGroupArray,
                                CViewMap *viewMap)
{
    POSITION pos = GetStartPosition();

    while (pos) {
        int id = 0;
        TEMPL_VIEW *templ = NULL;

        GetNextAssoc(pos, id, templ);

        CCigiView *view = new CCigiView(templ);

        // Find the group's tree node.
        // Use the template's data, since the view data might not be updated yet.
        HTREEITEM parent_node = NULL;
        int group_id = templ->GroupID;

        if (group_id) {
            CViewGroup *group = dm->GetViewGroup(group_id);

            if (group) {
                parent_node = group->GetHtree();
            } else {
                // If the .def file doesn't explicitly define a view group, create one.
                group = new CViewGroup(group_id);

                parent_node = mainframe->AddItemToViewTree(group->GetName(),
                                                           IMG_VIEWGROUP,
                                                           MAKE_TREE_DATA(group_id, OBJECT_TYPE_VIEWGROUP, 0));

                viewGroupArray[group_id] = group;
                viewGroupArray[group_id]->SetHtree(parent_node);
            }
        }

        HTREEITEM hitem = mainframe->AddItemToViewTree(view->GetName(),
                                                       IMG_VIEW,
                                                       MAKE_TREE_DATA(view->GetViewID(), OBJECT_TYPE_VIEW, 0),
                                                       parent_node);
        view->SetHtree(hitem);
        viewMap->SetAt(id, view);
    }
}

/// Handles the parsing of "VIEW" blocks when encountered in the Views.def file.
void CViewTemplMap::ParseTemplateBlock(CString &str, const CString &token, int *linenum)
{
    if (token == "VIEW") {
        TEMPL_VIEW *templ = new TEMPL_VIEW;
        templ->ReadText(str, linenum);
        SetAt(templ->ViewID, templ);
    }
}
