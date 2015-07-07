; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CXsvrDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "xsvr.h"

ClassCount=6
Class1=CXsvrApp
Class2=CXsvrDlg
Class3=CAboutDlg

ResourceCount=9
Resource1=IDM_LISTCTRL
Resource2=IDR_MAINFRAME
Resource3=IDR_SYSTEMTRAY
Resource4=IDD_XSVR_DIALOG
Resource5=IDD_USER_EDITOR
Class4=CXUserEditorDlg
Resource6=IDR_MENU1
Resource7=IDD_SPLITDLG
Resource8=IDD_ABOUTBOX
Class5=XSplitDlg
Class6=CXTaskDlg
Resource9=IDM_LISTAddNEW

[CLS:CXsvrApp]
Type=0
HeaderFile=xsvr.h
ImplementationFile=xsvr.cpp
Filter=N

[CLS:CXsvrDlg]
Type=0
HeaderFile=xsvrDlg.h
ImplementationFile=xsvrDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST1

[CLS:CAboutDlg]
Type=0
HeaderFile=xsvrDlg.h
ImplementationFile=xsvrDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=3
Control1=IDC_STATIC,static,1342177283
Control2=IDOK,button,1342373889
Control3=IDC_STATIC,static,1342308352

[DLG:IDD_XSVR_DIALOG]
Type=1
Class=CXsvrDlg
ControlCount=4
Control1=IDC_STATIC8,static,1342308352
Control2=IDC_LIST1,SysListView32,1350631425
Control3=IDC_TREE1,SysTreeView32,1350631427
Control4=IDC_EDIT1,edit,1352730692

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=ID_MENU_USER
Command2=ID_MENUQUIT
Command3=ID_MENUABOUT
CommandCount=3

[MNU:IDR_SYSTEMTRAY]
Type=1
Class=?
Command1=ID_TRAYRESTORE
Command2=ID_TRAYQUIT
CommandCount=2

[DLG:IDD_USER_EDITOR]
Type=1
Class=CXUserEditorDlg
ControlCount=16
Control1=IDC_EDIT1,edit,1350631552
Control2=IDC_EDIT2,edit,1350631552
Control3=IDC_EDIT3,edit,1350631552
Control4=IDC_EDIT4,edit,1350631552
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_TREE1,SysTreeView32,1350631431
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC_CELL,static,1342308352
Control14=IDC_EDIT_CELL,edit,1350631552
Control15=IDC_CHECK_ALL,button,1342275587
Control16=IDC_CHECK_INVERSE,button,1342275587

[CLS:CXUserEditorDlg]
Type=0
HeaderFile=XUserEditorDlg.h
ImplementationFile=XUserEditorDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[MNU:IDM_LISTCTRL]
Type=1
Class=?
Command1=ID_MENU_MODIFY
Command2=ID_MENU_DELETE
CommandCount=2

[MNU:IDM_LISTAddNEW]
Type=1
Class=?
Command1=ID_MENUADDNEW
CommandCount=1

[DLG:IDD_SPLITDLG]
Type=1
Class=XSplitDlg
ControlCount=0

[CLS:XSplitDlg]
Type=0
HeaderFile=XSplitDlg.h
ImplementationFile=XSplitDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_MENU_DELETE

[CLS:CXTaskDlg]
Type=0
HeaderFile=XTaskDlg.h
ImplementationFile=XTaskDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CXTaskDlg
VirtualFilter=dWC

