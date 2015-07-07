# Microsoft Developer Studio Project File - Name="client02" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=client02 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "client02.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "client02.mak" CFG="client02 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "client02 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "client02 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "client02 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Release/client.exe"

!ELSEIF  "$(CFG)" == "client02 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "client02 - Win32 Release"
# Name "client02 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\client02.cpp
# End Source File
# Begin Source File

SOURCE=.\client02.rc
# End Source File
# Begin Source File

SOURCE=.\client02Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DLG_ERP_MESSAGE.cpp
# End Source File
# Begin Source File

SOURCE=.\DLG_ERP_NEW_PROJECT.cpp
# End Source File
# Begin Source File

SOURCE=.\DLG_ERP_PROJECT.cpp
# End Source File
# Begin Source File

SOURCE=.\DLG_ERP_SCHEDULE.cpp
# End Source File
# Begin Source File

SOURCE=.\DLG_ERP_WORK.cpp
# End Source File
# Begin Source File

SOURCE=.\INI.cpp
# End Source File
# Begin Source File

SOURCE=.\MyHyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\NormalSMS.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry.cpp

!IF  "$(CFG)" == "client02 - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "client02 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SplitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SystemTray.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarExCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewHistoryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XChatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\xfun.cpp
# End Source File
# Begin Source File

SOURCE=.\XLoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\XMSG\XMSG.cpp
# End Source File
# Begin Source File

SOURCE=.\XPluginBar.cpp
# End Source File
# Begin Source File

SOURCE=.\XSMSDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\XMSG\xtype.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\client02.h
# End Source File
# Begin Source File

SOURCE=.\client02Dlg.h
# End Source File
# Begin Source File

SOURCE=.\DLG_ERP_MESSAGE.h
# End Source File
# Begin Source File

SOURCE=.\DLG_ERP_NEW_PROJECT.h
# End Source File
# Begin Source File

SOURCE=.\DLG_ERP_PROJECT.h
# End Source File
# Begin Source File

SOURCE=.\DLG_ERP_SCHEDULE.h
# End Source File
# Begin Source File

SOURCE=.\DLG_ERP_WORK.h
# End Source File
# Begin Source File

SOURCE=.\INI.h
# End Source File
# Begin Source File

SOURCE=.\MyHyperLink.h
# End Source File
# Begin Source File

SOURCE=.\NormalSMS.h
# End Source File
# Begin Source File

SOURCE=.\RegEntry.h
# End Source File
# Begin Source File

SOURCE=.\Registry.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SetDlg.h
# End Source File
# Begin Source File

SOURCE=.\SplitDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SystemTray.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarExCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ViewHistoryDlg.h
# End Source File
# Begin Source File

SOURCE=.\XChatDlg.h
# End Source File
# Begin Source File

SOURCE=.\xclientdef.h
# End Source File
# Begin Source File

SOURCE=..\XMSG\xdef.h
# End Source File
# Begin Source File

SOURCE=.\xdlldef.h
# End Source File
# Begin Source File

SOURCE=.\xfun.h
# End Source File
# Begin Source File

SOURCE=.\XLoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\XPluginBar.h
# End Source File
# Begin Source File

SOURCE=.\XSMSDlg.h
# End Source File
# Begin Source File

SOURCE=.\XTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\XMSG\xtype.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\chat.ico
# End Source File
# Begin Source File

SOURCE=.\res\client02.ico
# End Source File
# Begin Source File

SOURCE=.\res\client02.rc2
# End Source File
# Begin Source File

SOURCE=.\res\clock.bmp
# End Source File
# Begin Source File

SOURCE=.\res\default1.bin
# End Source File
# Begin Source File

SOURCE=.\res\localuser.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo01.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MAINFRAME_CONTACT.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MAINFRAME_LINE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MAINFRAME_LINE_2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MAINFRAME_USER.bmp
# End Source File
# Begin Source File

SOURCE=.\res\user.ico
# End Source File
# Begin Source File

SOURCE=.\res\usericon.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
