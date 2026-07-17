#include "stdafx.h"
#include "ExternalTabConfig.h"

namespace
{
const int MaxExternalTabCount = 32;

CString GetDirectoryName(const CString &path)
{
    const int slash = path.ReverseFind(_T('\\'));
    if (slash < 0)
        return _T("");

    return path.Left(slash);
}

CString GetModuleDirectory()
{
    TCHAR modulePath[MAX_PATH] = {0};
    GetModuleFileName(NULL, modulePath, MAX_PATH);

    CString directory = GetDirectoryName(modulePath);
    return directory.IsEmpty() ? _T(".") : directory;
}

BOOL DirectoryExists(const CString &path)
{
    const DWORD attributes = GetFileAttributes(path);
    return attributes != INVALID_FILE_ATTRIBUTES
           && (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

BOOL FileExists(const CString &path)
{
    const DWORD attributes = GetFileAttributes(path);
    return attributes != INVALID_FILE_ATTRIBUTES
           && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

CString GetApplicationDirectory()
{
    const CString moduleDirectory = GetModuleDirectory();
    if (DirectoryExists(moduleDirectory + _T("\\config")))
        return moduleDirectory;

    TCHAR currentDirectory[MAX_PATH] = {0};
    if (GetCurrentDirectory(MAX_PATH, currentDirectory) > 0) {
        const CString workingDirectory(currentDirectory);
        if (DirectoryExists(workingDirectory + _T("\\config")))
            return workingDirectory;
    }

    return moduleDirectory;
}

BOOL IsAbsolutePath(const CString &path)
{
    if (path.GetLength() >= 2 && path[1] == _T(':'))
        return TRUE;

    return !path.IsEmpty() && path[0] == _T('\\');
}

CString ResolveApplicationPath(const CString &applicationDirectory,
                               const CString &configuredPath)
{
    CString path(configuredPath);
    path.TrimLeft();
    path.TrimRight();
    if (path.IsEmpty())
        return path;

    CString combinedPath = path;
    if (!IsAbsolutePath(path))
        combinedPath = applicationDirectory + _T("\\") + path;

    TCHAR fullPath[MAX_PATH] = {0};
    const DWORD length = GetFullPathName(combinedPath, MAX_PATH, fullPath,
                                         NULL);
    if (length > 0 && length < MAX_PATH)
        return CString(fullPath);

    return combinedPath;
}

CString ReadProfileString(const CString &sectionName, LPCTSTR keyName,
                          LPCTSTR defaultValue, const CString &filePath)
{
    TCHAR value[2048] = {0};
    GetPrivateProfileString(sectionName, keyName, defaultValue, value,
                            sizeof(value) / sizeof(value[0]), filePath);

    CString result(value);
    result.TrimLeft();
    result.TrimRight();
    return result;
}

CString GetSectionName(const int index)
{
    CString sectionName;
    sectionName.Format(_T("ExternalTab%d"), index);
    return sectionName;
}
}

CExternalTabDefinition::CExternalTabDefinition()
    : LaunchOnSelect(TRUE), CloseWithHemu(TRUE)
{
}

void LoadExternalTabDefinitions(CExternalTabDefinitionArray &definitions,
                                CStringArray &diagnostics,
                                CString &startupTabTitle)
{
    definitions.RemoveAll();
    diagnostics.RemoveAll();
    startupTabTitle = _T("Messages");

    const CString applicationDirectory = GetApplicationDirectory();
    const CString configDirectory = applicationDirectory + _T("\\config");
    const CString defaultFilePath =
        configDirectory + _T("\\external_tabs.ini");
    const CString userFilePath =
        configDirectory + _T("\\external_tabs.user.ini");

    CString configFilePath = defaultFilePath;
    if (FileExists(userFilePath)) {
        configFilePath = userFilePath;
        diagnostics.Add(_T("External tabs: using external_tabs.user.ini."));
    } else if (!FileExists(defaultFilePath)) {
        diagnostics.Add(
            _T("External tabs: no configuration file was found."));
        return;
    }

    startupTabTitle = ReadProfileString(
        _T("ExternalTabs"), _T("StartupTab"), _T("Messages"),
        configFilePath);
    if (startupTabTitle.IsEmpty())
        startupTabTitle = _T("Messages");

    int count = GetPrivateProfileInt(_T("ExternalTabs"), _T("Count"), 0,
                                     configFilePath);
    if (count < 0) {
        diagnostics.Add(
            _T("External tabs: Count cannot be negative; no tabs loaded."));
        return;
    }

    if (count > MaxExternalTabCount) {
        CString diagnostic;
        diagnostic.Format(
            _T("External tabs: Count limited to %d entries."),
            MaxExternalTabCount);
        diagnostics.Add(diagnostic);
        count = MaxExternalTabCount;
    }

    for (int index = 0; index < count; ++index) {
        const CString sectionName = GetSectionName(index);
        if (!GetPrivateProfileInt(sectionName, _T("Enabled"), 1,
                                  configFilePath))
            continue;

        CExternalTabDefinition definition;
        definition.Title = ReadProfileString(sectionName, _T("Title"),
                                             _T(""), configFilePath);
        if (definition.Title.IsEmpty())
            definition.Title.Format(_T("External Tool %d"), index + 1);

        const CString executable = ReadProfileString(
            sectionName, _T("Executable"), _T(""), configFilePath);
        if (executable.IsEmpty()) {
            CString diagnostic;
            diagnostic.Format(
                _T("External tabs: [%s] has no Executable value."),
                (LPCTSTR)sectionName);
            diagnostics.Add(diagnostic);
            continue;
        }

        definition.Executable =
            ResolveApplicationPath(applicationDirectory, executable);
        definition.Arguments = ReadProfileString(
            sectionName, _T("Arguments"), _T(""), configFilePath);

        const CString workingDirectory = ReadProfileString(
            sectionName, _T("WorkingDirectory"), _T(""), configFilePath);
        if (workingDirectory.IsEmpty())
            definition.WorkingDirectory =
                GetDirectoryName(definition.Executable);
        else
            definition.WorkingDirectory = ResolveApplicationPath(
                applicationDirectory, workingDirectory);

        definition.LaunchOnSelect = GetPrivateProfileInt(
            sectionName, _T("LaunchOnSelect"), 1, configFilePath) ? TRUE
                                                                  : FALSE;
        definition.CloseWithHemu = GetPrivateProfileInt(
            sectionName, _T("CloseWithHemu"), 1, configFilePath) ? TRUE
                                                                  : FALSE;

        definitions.Add(definition);
    }
}
