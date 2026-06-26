param(
[string]$DefaultVersion = "v0.1.1-alpha",
[string]$RepoFullName = "ashtonbrown-dev/cigi-he-modernization",
[string]$DefaultTargetRef = "main",
[switch]$Upload,
[switch]$NonInteractive
)

$ErrorActionPreference = "Stop"

function Wait-BeforeExit {
if (!$NonInteractive) {
    Read-Host "Press Enter to close"
}
}

function Fail($message) {
Write-Host ""
Write-Host "ERROR: $message" -ForegroundColor Red
Write-Host ""
Wait-BeforeExit
exit 1
}

function Copy-RequiredFile($fileName) {
$src = Join-Path $AppDir $fileName
$dst = Join-Path $StageDir $fileName

if (!(Test-Path $src)) {
    Fail "Required file missing from app folder: $fileName"
}

Copy-Item $src $dst -Force

}

function Copy-OptionalFile($fileName) {
$src = Join-Path $AppDir $fileName
$dst = Join-Path $StageDir $fileName

if (Test-Path $src) {
    Copy-Item $src $dst -Force
}

}

function Copy-RequiredFolder($folderName) {
$src = Join-Path $AppDir $folderName
$dst = Join-Path $StageDir $folderName

if (!(Test-Path $src)) {
    Fail "Required folder missing from app folder: $folderName"
}

Copy-Item $src $dst -Recurse -Force

}

function Copy-RepoOrAppFile($appFileName, $repoFileName, $stageFileName) {
$appSrc = Join-Path $AppDir $appFileName
$repoSrc = Join-Path $RepoRoot $repoFileName
$dst = Join-Path $StageDir $stageFileName

if (Test-Path $appSrc) {
    Copy-Item $appSrc $dst -Force
    return
}

if (Test-Path $repoSrc) {
    Copy-Item $repoSrc $dst -Force
    return
}

Fail "Required package document missing from app and repo root: $stageFileName"
}

try {
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$RepoRoot = Split-Path -Parent $ScriptDir

$AppDir = Join-Path $RepoRoot "app"
$DistDir = Join-Path $RepoRoot "dist"

if (!(Test-Path $AppDir)) {
    Fail "app folder not found: $AppDir"
}

if (!(Test-Path $DistDir)) {
    New-Item -ItemType Directory -Path $DistDir | Out-Null
}

Write-Host ""
Write-Host "CIGI HEMU release packager/uploader" -ForegroundColor Cyan
Write-Host "Repo: $RepoRoot"
Write-Host "App:  $AppDir"
Write-Host "Dist: $DistDir"
Write-Host ""

if ($NonInteractive) {
    $Tag = $DefaultVersion
    $TargetRef = $DefaultTargetRef
}
else {
    $Tag = Read-Host "Release tag [$DefaultVersion]"
    if ([string]::IsNullOrWhiteSpace($Tag)) {
        $Tag = $DefaultVersion
    }

    $TargetRef = Read-Host "Target ref for new release tag [$DefaultTargetRef]"
    if ([string]::IsNullOrWhiteSpace($TargetRef)) {
        $TargetRef = $DefaultTargetRef
    }
}

$PackageName = "CIGI-HEMU-$Tag-win32"
$StageDir = Join-Path $DistDir $PackageName
$ZipPath = Join-Path $DistDir "$PackageName.zip"
$NotesPath = Join-Path $DistDir "RELEASE_NOTES_$Tag.md"
$ReleaseTitle = "CIGI HEMU $Tag"

Write-Host ""
Write-Host "Cleaning old local package for this version..." -ForegroundColor Cyan

if (Test-Path $StageDir) {
    Remove-Item $StageDir -Recurse -Force
}

if (Test-Path $ZipPath) {
    Remove-Item $ZipPath -Force
}

New-Item -ItemType Directory -Path $StageDir | Out-Null

Write-Host "Copying release files..." -ForegroundColor Cyan

Copy-RequiredFolder "config"
Copy-RequiredFolder "scenarios"

Copy-RequiredFile "Hemu4.exe"
Copy-RequiredFile "HemuDrv.exe"
Copy-RequiredFile "DummyIG4.exe"
Copy-RepoOrAppFile "LICENSE.txt" "license.txt" "LICENSE.txt"
Copy-RepoOrAppFile "README_RUN_FIRST.txt" "READ_ME_FIRST.txt" "README_RUN_FIRST.txt"

Copy-OptionalFile "Hemu4.chm"
Copy-OptionalFile "start_DummyIG4.bat"
Copy-OptionalFile "start_Hemu4.bat"
Copy-OptionalFile "stop_cigi.bat"

Copy-OptionalFile "mfc140.dll"
Copy-OptionalFile "msvcp140.dll"
Copy-OptionalFile "vcruntime140.dll"
Copy-OptionalFile "vcruntime140_1.dll"
Copy-OptionalFile "concrt140.dll"

$readmeInPackage = Join-Path $StageDir "README_RELEASE_PACKAGE.txt"

@"
CIGI HEMU $Tag

Quick start:

1. Run start_DummyIG4.bat if present, or launch DummyIG4.exe manually.
2. Run start_Hemu4.bat if present, or launch Hemu4.exe manually.
3. Load a sample scenario from the scenarios folder.
4. Press Run/Play to start scenario motion.

Notes:

* CIGI 4.0 remains the known-good runtime path.
* CIGI 3.3 support is early alpha functionality and is not yet feature complete.
* CIGI 3.0, 3.1, and 3.2 are not supported runtime targets in this package.
* This package was generated from the local app folder.
"@ | Set-Content -Path $readmeInPackage -Encoding UTF8

@"

# CIGI HEMU $Tag

This is a Windows Win32 alpha package of CIGI HEMU.

## Included

* Hemu4.exe
* HemuDrv.exe
* DummyIG4.exe
* Default config files
* Sample scenarios
* Launch/stop helper batch files when present
* Runtime DLLs if they were present in the app folder

## Runtime support

CIGI 4.0 remains the known-good runtime path.

CIGI 3.3 support is present as early alpha functionality and is not yet feature complete. CIGI 3.0, 3.1, and 3.2 are not supported runtime targets in this package.

## Suggested smoke test

1. Run start_DummyIG4.bat.
2. Run start_Hemu4.bat, or launch Hemu4.exe.
3. Select CIGI 4.0.
4. Load app/scenarios/MyOwnship.sf4.
5. Press Run/Play.
6. Confirm the heartbeat updates and the entity moves.
7. Exit Hemu4 and confirm HemuDrv.exe exits.
"@ | Set-Content -Path $NotesPath -Encoding UTF8

   Write-Host "Creating ZIP..." -ForegroundColor Cyan
   Compress-Archive -Path $StageDir -DestinationPath $ZipPath -Force

   if (!(Test-Path $ZipPath)) {
   Fail "ZIP was not created: $ZipPath"
   }

   $ZipInfo = Get-Item $ZipPath

   Write-Host ""
   Write-Host "Package ready:" -ForegroundColor Green
   Write-Host "  Folder: $StageDir"
   Write-Host "  ZIP:    $ZipPath"
   Write-Host "  Size:   $([Math]::Round($ZipInfo.Length / 1MB, 2)) MB"
   Write-Host ""

   if ($NonInteractive -and !$Upload) {
   Write-Host "Upload was not requested. Packaging complete." -ForegroundColor Yellow
   exit 0
   }

   Write-Host "Checking GitHub CLI..." -ForegroundColor Cyan

   $gh = Get-Command gh -ErrorAction SilentlyContinue
   if ($null -eq $gh) {
       Fail "GitHub CLI 'gh' was not found. Install it and run: gh auth login"
   }

   & gh auth status --hostname github.com
   if ($LASTEXITCODE -ne 0) {
       Fail "GitHub CLI is not logged in. Run: gh auth login"
   }

   Write-Host "About to upload to GitHub Releases:" -ForegroundColor Yellow
   Write-Host "  Repo:   $RepoFullName"
   Write-Host "  Tag:    $Tag"
   Write-Host "  Target: $TargetRef"
   Write-Host "  Asset:  $ZipPath"
   Write-Host ""

   if (!$NonInteractive) {
   $Confirm = Read-Host "Type UPLOAD to continue"
   if ($Confirm -ne "UPLOAD") {
   Write-Host "Upload cancelled."
   Wait-BeforeExit
   exit 0
   }
   }

   $oldErrorActionPreference = $ErrorActionPreference
   $ErrorActionPreference = "Continue"
   try {
       & gh release view $Tag --repo $RepoFullName *> $null
       $releaseViewExitCode = $LASTEXITCODE
   }
   finally {
       $ErrorActionPreference = $oldErrorActionPreference
   }

   $ReleaseExists = ($releaseViewExitCode -eq 0)

   if ($ReleaseExists) {
   Fail "Release already exists for $Tag. Refusing to overwrite existing release or assets."
   }

   Write-Host ""
   Write-Host "Release does not exist. Creating new DRAFT release..." -ForegroundColor Cyan

    & gh release create $Tag $ZipPath --repo $RepoFullName --title $ReleaseTitle --notes-file $NotesPath --target $TargetRef --draft --prerelease
    if ($LASTEXITCODE -ne 0) {
        Fail "gh release create failed"
    }

   Write-Host ""
   Write-Host "Done. Release asset uploaded." -ForegroundColor Green
   Write-Host "Release: https://github.com/$RepoFullName/releases/tag/$Tag"
   Write-Host ""

   Wait-BeforeExit
   }
   catch {
   Fail $_.Exception.Message
   }
