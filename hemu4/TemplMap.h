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

#include "StdAfx.h"

template <class TEMPL_TYPE>
class CTemplMap : public CMap<int, int, TEMPL_TYPE *, TEMPL_TYPE *>
{
public:
    CTemplMap() {}
    virtual ~CTemplMap() {}

    void LoadTemplates(CStdioFile &file);
    void RemoveTemplates();
    void WriteTemplates(CStdioFile &file);
    static void AFXAPI SerializeElements(CArchive &ar, TEMPL_TYPE **pElements, int nCount);

private:
    virtual void ParseTemplateBlock(CString &str, const CString &token, int *linenum) = 0;
};

template <class TEMPL_TYPE>
void CTemplMap<TEMPL_TYPE>::LoadTemplates(CStdioFile &file)
{
    // Find the end of the file.
    file.SeekToEnd();
    ULONGLONG end = file.GetPosition();
    file.SeekToBegin();

    while (file.GetPosition() != end) {
        // Get the entity block.
        int fileline = 0;
        CString str = ReadBlock(file, &fileline);

        // Read and process each line.
        int linenum = 0;
        CString temp, token;
        BOOL readstatus = ReadLine(str, &linenum, token, temp);

        while ((readstatus == TRUE) && (token != "}")) {
            ParseTemplateBlock(str, token, &linenum);
            readstatus = ReadLine(str, &linenum, token, temp);
        }
    }
}

template <class TEMPL_TYPE>
void CTemplMap<TEMPL_TYPE>::RemoveTemplates()
{
    POSITION pos = GetStartPosition();

    while (pos) {
        int id = 0;
        TEMPL_TYPE *templ = NULL;

        GetNextAssoc(pos, id, templ);

        if (templ) {
            // Remove the entry from the map.
            RemoveKey(id);

            // Delete the object.
            delete templ;
        }
    }
}

template <class TEMPL_TYPE>
void CTemplMap<TEMPL_TYPE>::WriteTemplates(CStdioFile &file)
{
    POSITION pos = GetStartPosition();

    while (pos) {
        int temp;
        TEMPL_TYPE *templ = NULL;

        GetNextAssoc(pos, temp, templ);

        if (templ)
            templ->WriteText(file);
    }
}

template <class TEMPL_TYPE>
void AFXAPI CTemplMap<TEMPL_TYPE>::SerializeElements(CArchive &ar, TEMPL_TYPE **pElements, int nCount)
{
    if (ar.IsStoring()) {
        for (int i = 0; i < nCount; i++) {
            if (pElements[i])
                pElements[i]->Serialize(ar);
        }
    } else {
        for (int i = 0; i < nCount; i++) {
            pElements[i] = new TEMPL_TYPE;
            pElements[i]->Serialize(ar);
        }
    }
}
