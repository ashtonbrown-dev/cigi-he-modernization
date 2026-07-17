#if !defined(AFX_EXTERNALTABCONFIG_H__INCLUDED_)
#define AFX_EXTERNALTABCONFIG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include <afxtempl.h>

class CExternalTabDefinition
{
public:
    CExternalTabDefinition();

    CString Title;
    CString Executable;
    CString WorkingDirectory;
    CString Arguments;
    BOOL LaunchOnSelect;
    BOOL CloseWithHemu;
};

typedef CArray<CExternalTabDefinition, CExternalTabDefinition &>
    CExternalTabDefinitionArray;

void LoadExternalTabDefinitions(CExternalTabDefinitionArray &definitions,
                                CStringArray &diagnostics,
                                CString &startupTabTitle);

#endif
