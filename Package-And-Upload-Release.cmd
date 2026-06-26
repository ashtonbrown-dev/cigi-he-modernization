@echo off
setlocal

cd /d "%~dp0"

echo.
echo CIGI HEMU package + upload
echo Repo folder: %CD%
echo.

powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0scripts\Package-And-Upload-Release.ps1"

echo.
pause
