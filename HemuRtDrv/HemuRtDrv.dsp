# Microsoft Developer Studio Project File - Name="HemuRtDrv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=HEMURTDRV - WIN32 RTSS RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HemuRtDrv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HemuRtDrv.mak" CFG="HEMURTDRV - WIN32 RTSS RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HemuRtDrv - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HemuRtDrv - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "HemuRtDrv - Win32 RTSS Release" (based on "Win32 (x86) Application")
!MESSAGE "HemuRtDrv - Win32 RTSS Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HemuRtDrv - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /W3 /GX /Ox /I "$(RtxSdkDir)\include" /D "WIN32" /D "NDEBUG" /D "RELEASE" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Ox /I "..\coordcnv" /I ".." /D "WIN32" /D "NDEBUG" /D "RELEASE" /D "NO_RTX" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 rtapi_w32.lib libc.lib oldnames.lib kernel32.lib ws2_32.lib mswsock.lib advapi32.lib /nologo /pdb:none /machine:I386 /nodefaultlib /libpath:"$(RtxSdkDir)\lib" /subsystem:console,4.00
# ADD LINK32 libc.lib oldnames.lib kernel32.lib ws2_32.lib mswsock.lib advapi32.lib user32.lib /nologo /pdb:none /machine:I386 /nodefaultlib /out:"Release/HemuDrv.exe" /libpath:"c:\apis\ccu" /libpath:"$(RtxSdkDir)\lib" /subsystem:console,4.00
# SUBTRACT LINK32 /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\Release\hemudrv.exe c:\stealthviewer	copy .\Release\hemudrv.exe ..\hemu4
# End Special Build Tool

!ELSEIF  "$(CFG)" == "HemuRtDrv - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /I "$(RtxSdkDir)\include" /D "WIN32" /D "_DEBUG" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /I "..\coordcnv" /I ".." /D "WIN32" /D "_DEBUG" /D "NO_RTX" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 rtapi_w32.lib libc.lib oldnames.lib kernel32.lib ws2_32.lib mswsock.lib advapi32.lib /nologo /incremental:no /pdb:"HemuRtDrv___Win32_RTSS_Debug/HemuRtDrv.pdb" /debug /debugtype:both /machine:I386 /nodefaultlib /pdbtype:sept /libpath:"$(RtxSdkDir)\lib" /subsystem:console,4.00
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 libc.lib oldnames.lib kernel32.lib ws2_32.lib mswsock.lib advapi32.lib user32.lib LIBCMTD.LIB /nologo /incremental:no /pdb:"HemuRtDrv___Win32_RTSS_Debug/HemuRtDrv.pdb" /debug /debugtype:both /machine:I386 /nodefaultlib /out:"Debug/HemuDrv.exe" /pdbtype:sept /libpath:"$(RtxSdkDir)\lib" /libpath:"c:\apis\ccu" /subsystem:console,4.00
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\Debug\hemudrv.exe c:\stealthviewer	copy .\Debug\hemudrv.exe ..\hemu4
# End Special Build Tool

!ELSEIF  "$(CFG)" == "HemuRtDrv - Win32 RTSS Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "HemuRtDrv___Win32_RTSS_Release"
# PROP BASE Intermediate_Dir "HemuRtDrv___Win32_RTSS_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "HemuRtDrv___Win32_RTSS_Release"
# PROP Intermediate_Dir "HemuRtDrv___Win32_RTSS_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /ML /W3 /GX /Ox /I "$(RtxSdkDir)\include" /D "WIN32" /D "UNDER_RTSS" /D "NDEBUG" /D "RELEASE" /YX /FD /c
# ADD CPP /nologo /ML /W3 /GX /Ox /I "$(RtxSdkDir)\include" /I "..\coordcnv" /I ".." /D "WIN32" /D "UNDER_RTSS" /D "NDEBUG" /D "RELEASE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 startupCRT.obj rtxlibc.lib oldnames.lib rtapi_rtss.lib rtx_rtss.lib /nologo /entry:"_RtapiProcessEntryCRT@8" /pdb:none /machine:I386 /nodefaultlib:"libc" /nodefaultlib:"libcd" /nodefaultlib:"libcmt" /nodefaultlib:"libcmtd" /nodefaultlib /out:"HemuRtDrv___Win32_RTSS_Release/HemuRtDrv.rtss" /libpath:"$(RtxSdkDir)\lib" /driver /subsystem:native,4.00 /align:0x20
# ADD LINK32 startupCRT.obj rtxlibc.lib oldnames.lib rtapi_rtss.lib rtx_rtss.lib rtxtcpip.lib /nologo /entry:"_RtapiProcessEntryCRT@8" /pdb:none /machine:I386 /nodefaultlib:"libc" /nodefaultlib:"libcd" /nodefaultlib:"libcmt" /nodefaultlib:"libcmtd" /nodefaultlib /out:"HemuRtDrv___Win32_RTSS_Release\HemuRtDrv.rtss" /libpath:"$(RtxSdkDir)\lib" /libpath:"c:\apis\ccu" /driver /subsystem:native,4.00 /align:0x20
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\HemuRtDrv___Win32_RTSS_Release\hemurtdrv.rtss c:\stealthviewer	copy .\HemuRtDrv___Win32_RTSS_Release\hemurtdrv.rtss ..\hemu4
# End Special Build Tool

!ELSEIF  "$(CFG)" == "HemuRtDrv - Win32 RTSS Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "HemuRtDrv___Win32_RTSS_Debug"
# PROP BASE Intermediate_Dir "HemuRtDrv___Win32_RTSS_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "HemuRtDrv___Win32_RTSS_Debug"
# PROP Intermediate_Dir "HemuRtDrv___Win32_RTSS_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /I "$(RtxSdkDir)\include" /D "WIN32" /D "_DEBUG" /D "UNDER_RTSS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /I "$(RtxSdkDir)\include" /I "..\coordcnv" /I ".." /D "WIN32" /D "_DEBUG" /D "UNDER_RTSS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 startupCRT.obj rtxlibc.lib oldnames.lib rtapi_rtss.lib rtx_rtss.lib /nologo /entry:"_RtapiProcessEntryCRT@8" /incremental:no /debug /debugtype:both /machine:I386 /nodefaultlib:"libc" /nodefaultlib:"libcd" /nodefaultlib:"libcmt" /nodefaultlib:"libcmtd" /nodefaultlib /out:"HemuRtDrv___Win32_RTSS_Debug/HemuRtDrv.rtss" /pdbtype:sept /libpath:"$(RtxSdkDir)\lib" /driver /subsystem:native,4.00 /align:0x20
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 startupCRT.obj rtxlibc.lib oldnames.lib rtapi_rtss.lib rtx_rtss.lib rtxtcpip.lib /nologo /entry:"_RtapiProcessEntryCRT@8" /incremental:no /debug /debugtype:both /machine:I386 /nodefaultlib:"libc" /nodefaultlib:"libcd" /nodefaultlib:"libcmt" /nodefaultlib:"libcmtd" /nodefaultlib /out:"HemuRtDrv___Win32_RTSS_Debug/HemuRtDrv.rtss" /pdbtype:sept /libpath:"$(RtxSdkDir)\lib" /libpath:"c:\apis\ccu" /driver /subsystem:native,4.00 /align:0x20
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\HemuRtDrv___Win32_RTSS_Debug\hemurtdrv.rtss c:\stealthviewer	copy .\HemuRtDrv___Win32_RTSS_Debug\hemurtdrv.rtss ..\hemu4
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "HemuRtDrv - Win32 Release"
# Name "HemuRtDrv - Win32 Debug"
# Name "HemuRtDrv - Win32 RTSS Release"
# Name "HemuRtDrv - Win32 RTSS Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\cas.cpp
# End Source File
# Begin Source File

SOURCE=..\cigi3.cpp
# End Source File
# Begin Source File

SOURCE=..\common.cpp
# End Source File
# Begin Source File

SOURCE=..\coordcnv\coordcnv.c
# End Source File
# Begin Source File

SOURCE=.\Fly.cpp
# End Source File
# Begin Source File

SOURCE=..\hemumsg.cpp
# End Source File
# Begin Source File

SOURCE=.\HemuRtDrv.cpp
# End Source File
# Begin Source File

SOURCE=..\RTXCompatibility.cpp
# End Source File
# Begin Source File

SOURCE=..\RTXSharedBufferQ.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\cigi3.h
# End Source File
# Begin Source File

SOURCE=..\cigi3types.h
# End Source File
# Begin Source File

SOURCE=..\common.h
# End Source File
# Begin Source File

SOURCE=..\coordcnv\coordcnv.h
# End Source File
# Begin Source File

SOURCE=.\Fly.h
# End Source File
# Begin Source File

SOURCE=..\hemumsg.h
# End Source File
# Begin Source File

SOURCE=..\LinkedList.h
# End Source File
# Begin Source File

SOURCE=..\RTXCompatibility.h
# End Source File
# Begin Source File

SOURCE=..\RTXSharedBufferQ.h
# End Source File
# Begin Source File

SOURCE=..\RTXSharedObj.h
# End Source File
# Begin Source File

SOURCE=..\SharedEntityObj.h
# End Source File
# Begin Source File

SOURCE=..\SharedViewGroupObj.h
# End Source File
# Begin Source File

SOURCE=..\SharedViewObj.h
# End Source File
# Begin Source File

SOURCE=..\simpleq.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE="..\Change History.txt"
# End Source File
# End Target
# End Project
