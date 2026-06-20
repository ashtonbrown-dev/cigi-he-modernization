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

#include "DatabaseTemplMap.h"
#include "ConfigTemplGlobalEnv.h"
#include "TerrainDB.h"

CDatabaseTemplMap::CDatabaseTemplMap(TEMPL_GLOBAL_ENV *i_GlobalEnvTempl) :
    m_GlobalEnvTempl(i_GlobalEnvTempl)
{
}

void CDatabaseTemplMap::CreateDatabases(CTerrainDB **databaseArray)
{
    POSITION pos = GetStartPosition();

    while (pos) {
        int id = 0;
        TEMPL_DATABASE *templ = NULL;

        GetNextAssoc(pos, id, templ);

        if (databaseArray[id] == NULL)
            databaseArray[id] = new CTerrainDB(templ);
    }
}

/// Handles the parsing of "DATABASE" and "GLOBAL" blocks when encountered in the Terrain.def file.
void CDatabaseTemplMap::ParseTemplateBlock(CString &str, const CString &token, int *linenum)
{
    if (token == "DATABASE") {
        TEMPL_DATABASE *templ = new TEMPL_DATABASE;
        templ->ReadText(str, linenum);
        SetAt(templ->DatabaseID, templ);
    } else if (token == "GLOBAL") {
        m_GlobalEnvTempl->ReadText(str, linenum);
    }
}
