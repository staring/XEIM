; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=4
Class1=CXlanfApp
LastClass=CXlanfApp
NewFileInclude2=#include "xlanf.h"
ResourceCount=3
NewFileInclude1=#include "stdafx.h"
Resource1=IDD_FRAME
Resource2=IDD_SEND
Class2=CXFrame
LastTemplate=CDialog
Class3=CXRecvDlg
Class4=CXSendDlg
Resource3=IDD_RECV

[CLS:CXlanfApp]
Type=0
HeaderFile=xlanf.h
ImplementationFile=xlanf.cpp
Filter=N
LastObject=CXlanfApp
BaseClass=CWinApp
VirtualFilter=AC

[DLG:IDD_FRAME]
Type=1
Class=CXFrame
ControlCount=1
Control1=IDC_FILE_TOTAL,static,1342308352

[DLG:IDD_SEND]
Type=1
Class=CXSendDlg
ControlCount=7
Control1=IDC_STATIC8,static,1342308352
Control2=IDC_STATIC6,static,1342308352
Control3=IDC_STATIC_ICON,static,1342177283
Control4=IDC_PROGRESS1,msctls_progress32,1350565888
Control5=IDC_STATIC_CANCEL,static,1342308352
Control6=IDC_FILE_NAME,static,1342308352
Control7=IDC_STATIC10,static,1342308352

[DLG:IDD_RECV]
Type=1
Class=CXRecvDlg
ControlCount=10
Control1=IDC_STATIC8,static,1342308352
Control2=IDC_STATIC6,static,1342308352
Control3=IDC_STATIC_ICON,static,1342177283
Control4=IDC_PROGRESS1,msctls_progress32,1350565888
Control5=IDC_STATIC_SAVE,static,1342308352
Control6=IDC_STATIC_REFUSE,static,1342308352
Control7=IDC_STATIC_CANCEL,static,1342308352
Control8=IDC_FILE_NAME,static,1342308352
Control9=IDC_STATIC_SAVEAS,static,1342308352
Control10=IDC_STATIC10,static,1342308352

[CLS:CXFrame]
Type=0
HeaderFile=XFrame.h
ImplementationFile=XFrame.cpp
BaseClass=CDialog
Filter=D
LastObject=CXFrame
VirtualFilter=dWC

[CLS:CXRecvDlg]
Type=0
HeaderFile=XRecvDlg.h
ImplementationFile=XRecvDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CXRecvDlg
VirtualFilter=dWC

[CLS:CXSendDlg]
Type=0
HeaderFile=XSendDlg.h
ImplementationFile=XSendDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CXSendDlg
VirtualFilter=dWC

