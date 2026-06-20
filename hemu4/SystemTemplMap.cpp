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

#include "SystemTemplMap.h"

/// Handles the parsing of "COMPONENT" blocks when encountered in the System.def file.
void CSystemTemplMap::ParseTemplateBlock(CString &str, const CString &token, int *linenum)
{
    if (token == "COMPONENT") {
        TEMPL_COMPONENT component;
        component.ReadText(str, linenum);

        const unsigned int systemCompClass = 13;
        CComponent *newComponent = new CComponent(systemCompClass, &component);
        SetAt(newComponent->GetID(), newComponent);
    }
}
