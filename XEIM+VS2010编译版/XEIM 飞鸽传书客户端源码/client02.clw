; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CClient02Dlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "client02.h"

ClassCount=17
Class1=CClient02App
Class2=CClient02Dlg
Class3=CAboutDlg

ResourceCount=18
Resource1=IDD_PLUGIN
Resource2=IDR_MAINFRAME
Resource3=IDD_NORMAL_SMS
Class4=CXLoginDlg
Resource4=IDD_PROJECT_ADD
Class5=CXChatDlg
Resource5=IDD_SCHEDULE
Class6=CSetDlg
Class7=CXTreeCtrl
Resource6=IDD_ABOUTBOX
Resource7=IDD_DIALOG6
Class8=CSplitDlg
Resource8=IDD_CLIENT02_DIALOG
Class9=CXSMSDlg
Resource9=IDD_PROJECT_NEW
Class10=CViewHistoryDlg
Resource10=IDD_DIALOG5
Resource11=IDD_DIALOG7
Class11=CNormalSMS
Resource12=IDD_HISTORY_VIEW
Class12=CXPluginBar
Resource13=IDD_DIALOG4
Class13=DLG_ERP_MESSAGE
Resource14=IDD_DIALOG2
Class14=DLG_ERP_SCHEDULE
Resource15=IDD_DIALOG3
Class15=DLG_ERP_PROJECT
Resource16=IDD_DIALOG1
Class16=DLG_ERP_WORK
Resource17=IDD_WORK
Class17=DLG_ERP_NEW_PROJECT
Resource18=IDR_SYSTEMTRAY

[CLS:CClient02App]
Type=0
HeaderFile=client02.h
ImplementationFile=client02.cpp
Filter=N

[CLS:CClient02Dlg]
Type=0
HeaderFile=client02Dlg.h
ImplementationFile=client02Dlg.cpp
Filter=D
LastObject=ID_MENUITEM_LOGOUT
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=client02Dlg.h
ImplementationFile=client02Dlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_CLIENT02_DIALOG]
Type=1
Class=CClient02Dlg
ControlCount=17
Control1=IDC_STATIC28,static,1342308352
Control2=IDC_USERLIST,SysTreeView32,1342242819
Control3=IDC_STATIC,static,1342177294
Control4=IDC_EDIT_NICKNAME,edit,1342244992
Control5=IDC_EDIT_USERNAME,edit,1342244992
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC_CONTACT,static,1342308352
Control9=IDC_SETTINGS,static,1342308352
Control10=IDC_STATIC18,static,1342308352
Control11=IDC_EDIT1,edit,1352728644
Control12=IDC_STATIC,static,1342177294
Control13=IDC_STATIC,static,1342177294
Control14=IDC_STATIC,static,1342177294
Control15=IDC_BUTTON1,button,1342242816
Control16=IDC_STATIC_INFO,static,1342308353
Control17=IDC_STATIC,static,1342177294

[DLG:IDD_DIALOG1]
Type=1
Class=CXLoginDlg
ControlCount=9
Control1=IDC_EDIT1,edit,1350631552
Control2=IDC_EDIT2,edit,1350631584
Control3=IDOK,button,1342242827
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_SETTING,button,1342242827
Control7=IDC_STATIC,static,1342177294
Control8=IDC_CHECK1,button,1342242819
Control9=IDC_CHECK2,button,1342242819

[CLS:CXLoginDlg]
Type=0
HeaderFile=XLoginDlg.h
ImplementationFile=XLoginDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CXLoginDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG2]
Type=1
Class=CXChatDlg
ControlCount=8
Control1=IDCANCEL,button,1342242827
Control2=IDC_SEND,button,1342242827
Control3=IDC_SENDFILE,button,1342242827
Control4=IDC_RICHEDIT1,RICHEDIT,1352730692
Control5=IDC_SENDSMS,button,1342373899
Control6=IDC_HISTORY,button,1342242827
Control7=IDC_RICHEDIT2,RICHEDIT,1352732740
Control8=IDC_SHAKE,button,1342242827

[CLS:CXChatDlg]
Type=0
HeaderFile=XChatDlg.h
ImplementationFile=XChatDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CXChatDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG3]
Type=1
Class=CSetDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_SVR,edit,1350631552
Control5=IDC_CHECK1,button,1342242819

[CLS:CSetDlg]
Type=0
HeaderFile=SetDlg.h
ImplementationFile=SetDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CSetDlg
VirtualFilter=dWC

[CLS:CXTreeCtrl]
Type=0
HeaderFile=XTreeCtrl.h
ImplementationFile=XTreeCtrl.cpp
BaseClass=CTreeCtrl
Filter=W
LastObject=CXTreeCtrl
VirtualFilter=GWC

[MNU:IDR_SYSTEMTRAY]
Type=1
Class=?
Command1=ID_TRAYRESTORE
Command2=ID_MENUITEM_LOGOUT
Command3=ID_TRAYQUIT
CommandCount=3

[DLG:IDD_DIALOG4]
Type=1
Class=CSplitDlg
ControlCount=0

[CLS:CSplitDlg]
Type=0
HeaderFile=SplitDlg.h
ImplementationFile=SplitDlg.cpp
BaseClass=CDialog
Filter=W
LastObject=CSplitDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG5]
Type=1
Class=CXSMSDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1352732740
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_SENDER_NUM,edit,1350639744

[CLS:CXSMSDlg]
Type=0
HeaderFile=XSMSDlg.h
ImplementationFile=XSMSDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CXSMSDlg
VirtualFilter=dWC

[DLG:IDD_HISTORY_VIEW]
Type=1
Class=CViewHistoryDlg
ControlCount=5
Control1=IDC_RICHEDIT1,RICHEDIT,1352730692
Control2=IDC_DATETIMEPICKER1,SysDateTimePick32,1342242848
Control3=IDC_STATIC2,static,1342308352
Control4=IDC_BACK,button,1342242816
Control5=IDC_FRONT,button,1342242816

[CLS:CViewHistoryDlg]
Type=0
HeaderFile=ViewHistoryDlg.h
ImplementationFile=ViewHistoryDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CViewHistoryDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG6]
Type=1
Class=?
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1352732740
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT3,edit,1352732740

[DLG:IDD_NORMAL_SMS]
Type=1
Class=CNormalSMS
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1352732740
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT3,edit,1352732740

[CLS:CNormalSMS]
Type=0
HeaderFile=NormalSMS.h
ImplementationFile=NormalSMS.cpp
BaseClass=CDialog
Filter=D
LastObject=CNormalSMS
VirtualFilter=dWC

[DLG:IDD_PLUGIN]
Type=1
Class=CXPluginBar
ControlCount=1
Control1=IDC_PLUGIN,static,1342308608

[CLS:CXPluginBar]
Type=0
HeaderFile=XPluginBar.h
ImplementationFile=XPluginBar.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CXPluginBar

[DLG:IDD_DIALOG7]
Type=1
Class=DLG_ERP_MESSAGE
ControlCount=4
Control1=IDC_EDIT1,edit,1352730692
Control2=IDC_BACK,button,1342242816
Control3=IDC_NEXT,button,1342242816
Control4=IDC_STATIC_SIZE,static,1342308352

[CLS:DLG_ERP_MESSAGE]
Type=0
HeaderFile=DLG_ERP_MESSAGE.h
ImplementationFile=DLG_ERP_MESSAGE.cpp
BaseClass=CDialog
Filter=D
LastObject=DLG_ERP_MESSAGE
VirtualFilter=dWC

[DLG:IDD_SCHEDULE]
Type=1
Class=DLG_ERP_SCHEDULE
ControlCount=20
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342177294
Control9=IDC_STATIC,static,1342308352
Control10=IDC_DATETIMEPICKER1,SysDateTimePick32,1342242848
Control11=IDC_DATETIMEPICKER2,SysDateTimePick32,1342242857
Control12=IDC_DATETIMEPICKER3,SysDateTimePick32,1342242848
Control13=IDC_DATETIMEPICKER4,SysDateTimePick32,1342242857
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT3,edit,1350631552
Control16=IDC_CHECK1,button,1342242819
Control17=IDC_STATIC,static,1342308352
Control18=IDC_EDIT4,edit,1352732740
Control19=IDC_STATIC,static,1342308352
Control20=IDC_EDIT5,edit,1350633600

[CLS:DLG_ERP_SCHEDULE]
Type=0
HeaderFile=DLG_ERP_SCHEDULE.h
ImplementationFile=DLG_ERP_SCHEDULE.cpp
BaseClass=CDialog
Filter=D
LastObject=DLG_ERP_SCHEDULE
VirtualFilter=dWC

[DLG:IDD_PROJECT_NEW]
Type=1
Class=DLG_ERP_PROJECT
ControlCount=6
Control1=IDC_BTN_NEW_PROJECT,button,1342242817
Control2=IDC_LIST1,SysListView32,1350631425
Control3=IDC_STATIC,static,1342308352
Control4=IDC_LIST2,SysListView32,1350631425
Control5=IDC_BTN_NEW_WORK,button,1342242816
Control6=IDC_STATIC,static,1342308352

[CLS:DLG_ERP_PROJECT]
Type=0
HeaderFile=DLG_ERP_PROJECT.h
ImplementationFile=DLG_ERP_PROJECT.cpp
BaseClass=CDialog
Filter=D
LastObject=DLG_ERP_PROJECT
VirtualFilter=dWC

[DLG:IDD_WORK]
Type=1
Class=DLG_ERP_WORK
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_LIST1,SysListView32,1350631425
Control5=IDC_STATIC,static,1342308352
Control6=IDC_PRJ_NUMBER,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_PRJ_SUBJECT,edit,1350631552
Control9=IDC_PRO_DETAIL,edit,1352728644

[CLS:DLG_ERP_WORK]
Type=0
HeaderFile=DLG_ERP_WORK.h
ImplementationFile=DLG_ERP_WORK.cpp
BaseClass=CDialog
Filter=D
LastObject=DLG_ERP_WORK
VirtualFilter=dWC

[DLG:IDD_PROJECT_ADD]
Type=1
Class=DLG_ERP_NEW_PROJECT
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT2,edit,1350633600
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT3,edit,1350633600
Control9=IDC_STATIC,static,1342308352
Control10=IDC_DETAIL,edit,1352728644

[CLS:DLG_ERP_NEW_PROJECT]
Type=0
HeaderFile=DLG_ERP_NEW_PROJECT.h
ImplementationFile=DLG_ERP_NEW_PROJECT.cpp
BaseClass=CDialog
Filter=D
LastObject=DLG_ERP_NEW_PROJECT
VirtualFilter=dWC

