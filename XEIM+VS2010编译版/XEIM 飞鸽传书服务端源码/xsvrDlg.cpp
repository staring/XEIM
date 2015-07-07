/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// xsvrDlg.cpp : implementation file
//

#include "stdafx.h"
#include <Windows.h>
#include "xsvr.h"
#include "xsvrDlg.h"
#include "XUserEditorDlg.h"
#include "../xmsg/xdef.h"
#include "INI.h"
#include <string>
using namespace std;
#include "SysMsg.h"

#pragma comment(lib, "Version.lib")

// SMS Processing //////////////////////////////////////////////////
#include "serialCtl.hpp"
#include "serialThread.hpp"
// SMS Processing //////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

// 这个计时器用来动态跟新数据库的
#define TIMER_CHECK_DATABASE		22
// 每隔多长时间与数据库进行同步
#define	CHECK_DATABASE_TIMES		10000 * 8


// 启动 SMS 发送功能
#define TIMER_START_SMS				33
// 启动前所等待时间
#define START_SMS_TIMES				100

// SMS 队列里的短信每隔一段时间发一次
#define TIMER_SEND_SMS				55
// 隔多长时间检查并发送一次
#define TIMES_FOR_CHECK				10000

// SMS 数据库内的情况，是否有新的数据录入
#define	TIMER_CHECK_SMSDB			88
#define	TIMER_CHECK_SMSDB_PERIOD	1000 * 5

// 发送用户联系人信息
#define	TIMER_SEND_CONTACT			99
#define	TIMER_SEND_CONTACT_PERIOD	100

// 发送用户状态给联系人
#define	TIMER_SEND_STATUS			100
#define TIMER_SEND_STATUS_PERIOD	1000

// 检测 ERP 任务表是否有所更新
#define TIMER_CHECK_ERP				108
#define TIMER_CHECK_ERP_PERIOD		2888
void GetProgramDirectory(char *buf)
{
	ASSERT(NULL != buf);
	
	char str[MAX_PATH];
	GetModuleFileName(NULL,str,MAX_PATH);

	int len = strlen(str);
	int nIdx = 0;

	for (int i=0; i<len; i++)
	{
		if (str[i] == '\\')
			nIdx = i;
		buf[i] = str[i];
	}
	buf[nIdx+1] = NULL;
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXsvrDlg dialog

CXsvrDlg::CXsvrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXsvrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXsvrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pConn = NULL;
	m_strDatabase = "user.mdb";
//	m_strSMSCenter = "13800755500";
//	m_strCOMX = "COM1";
//	m_bSuccessed = FALSE;

	m_pSplitDlg = NULL;

	m_pApp = NULL;

	m_bSendUserContact = FALSE;
	m_bSendUserStatus = FALSE;

}

void CXsvrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXsvrDlg)
	DDX_Control(pDX, IDC_EDIT1, m_errOut);
	DDX_Control(pDX, IDC_LIST1, m_lcUserList);
	DDX_Control(pDX, IDC_TREE1, m_tcUserTree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXsvrDlg, CDialog)
	//{{AFX_MSG_MAP(CXsvrDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(XM_CONNECTED, XOnConnected)
	ON_MESSAGE(XM_GETMSGOFFLINE, XGetMsgOffLine)
	ON_MESSAGE(XM_GETHISTORYDATA, XGetMsgHistory)
	ON_MESSAGE(XM_SENDHISTORYDATA, XMsgHistoryData)
	ON_MESSAGE(XM_RECORD_SMS, XOnRecordSMS)
	ON_MESSAGE(XM_RECORD_PROJECT, _ERP_OnRecordProject)
	ON_MESSAGE(XM_GET_PROJECT, _ERP_OnGetProject)
	ON_MESSAGE(XM_GET_SCHEDULE, _ERP_OnGetSchedule)
	ON_MESSAGE(XM_LOGIN, XEIM_USER_Login)
	ON_MESSAGE(XM_MESSAGE, XMsgOnLine)
	ON_MESSAGE(XM_OFFLINEMESSAGE, XMsgOffLine)
	ON_MESSAGE(XM_GETDATAOFFLINE, XMsgOffLineData)
	ON_MESSAGE(XM_LOGOFF, XOnLogoff)
	ON_MESSAGE(XM_GETLIST, XOnGetList)
	ON_MESSAGE(XM_NEWFILE, XOnNewFile)
	ON_NOTIFY (LVN_GETDISPINFO, IDC_LIST1, OnGetDispInfo)
	ON_WM_CLOSE()
	ON_COMMAND(ID_TRAYRESTORE, OnTrayRestore)
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_COMMAND(ID_MENUABOUT, OnMenuabout)
	ON_COMMAND(ID_MENUQUIT, OnMenuquit)
	ON_COMMAND(ID_MENU_USER, OnMenuUser)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_TIMER()
	ON_COMMAND(ID_MENU_TASK, OnMenuTask)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_COMMAND(ID_TRAYQUIT, OnCancel)
	//}}AFX_MSG_MAP
	ON_COMMAND(IDM_MSG_SYSTEM, &CXsvrDlg::OnMsgSystem)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXsvrDlg message handlers
// XEIM 服务器需要启动的功能都在这里启动
BOOL CXsvrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// 设置软件信息
	_UI_SetProductInfo();

	m_pApp = (CXsvrApp*)AfxGetApp();
	// 从数据库读取用户数据，并对用户界面进行操作
	XDB_InitUserList();

	// *****************************************************************
	// 一切就绪，启动服务器
	// *****************************************************************

	m_xserver.run(GetSafeHwnd());

	// *****************************************************************
	// 系统托盘图标创建
	// *****************************************************************
	char szTitle[512];
	LoadString(NULL, IDS_STRING_TITLE, szTitle, 512);
	if (!m_sysTray.Create(this, WM_ICON_NOTIFY, szTitle, m_hIcon, IDR_SYSTEMTRAY))
	{
		MessageBox("System tray creating error.");
		return FALSE;
	}

	// 窗口切分
	CRect rtTree;
	CRect rtList;
	m_tcUserTree.GetWindowRect(&rtTree);
	ScreenToClient(&rtTree);
	m_lcUserList.GetWindowRect(&rtList);
	ScreenToClient(&rtList);

	m_pSplitDlg = new XSplitDlg;
	m_pSplitDlg->Create(XSplitDlg::IDD, this);
	if (::IsWindow(m_pSplitDlg->GetSafeHwnd()))
	{
		m_pSplitDlg->ShowWindow(SW_SHOW);
		m_pSplitDlg->SetTowWindow(&m_tcUserTree, &m_lcUserList, 160);
		m_pSplitDlg->MoveWindow(rtTree.right, rtTree.top ,rtList.left-rtTree.right, rtList.bottom-12);
	}

	// 定时检测数据库里的用户情况
	SetTimer(TIMER_CHECK_DATABASE, CHECK_DATABASE_TIMES, NULL);

	// 定时检测数据库里的短信情况
	SetTimer(TIMER_CHECK_SMSDB, TIMER_CHECK_SMSDB_PERIOD, NULL);

	// 定时检测 ERP 数据表是否更新
	SetTimer(TIMER_CHECK_ERP, TIMER_CHECK_ERP_PERIOD, NULL);

	// XEIM DEBUG
	CString strOut;
	GetDlgItemText(IDC_EDIT1, strOut);
	strOut += "欢迎使用 XEIM 企业即时通讯系统\r\n";
	SetDlgItemText(IDC_EDIT1, strOut);
	// END XEIM DEBUG


	// SMS Processing ////////////////////////////////////////////////
/*	serialProcess = 
		(SerialThread*)AfxBeginThread(RUNTIME_CLASS(SerialThread), 
		THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	SetTimer(TIMER_START_SMS, START_SMS_TIMES, NULL);*/
	// SMS Processing ////////////////////////////////////////////////

	// Task managerment
//	m_dlgTask.Create(IDD_DIALOG1, this);

	// ERP 数据初始化
//	_ERP_INIT();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXsvrDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (SC_MINIMIZE == nID)
	{
		ShowWindow(SW_HIDE);
		return;
	}
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXsvrDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXsvrDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//*******************************************************************************************
// XEIM 飞鸽传书处理函数
//*******************************************************************************************
LRESULT CXsvrDlg::XOnConnected(WPARAM wParam, LPARAM lParam)
{
//	AfxMessageBox("XOnConnected");
	TRACE("XOnConnected.");

	return 0;
}




// XEIM 飞鸽传书用户从某台机器登录
LRESULT CXsvrDlg::XEIM_USER_Login(WPARAM wParam, LPARAM lParam)
{
	// 与该用户的通讯套接字
	SOCKET sock = (SOCKET)lParam;
	LPPER_IO_OPERATION_DATA PerIoData = (LPPER_IO_OPERATION_DATA)wParam;

	// 用户的登录信息
	XUserDetail *pUser = (XUserDetail*)PerIoData->DataBuf.buf;

	CString strIP;

	if (! pUser->IsDataType())
	{
		// XEIM DEBUG
		AddErrorInfo("数据包格式错误");
		delete pUser; // 记得释放内存
		return 0;
	}
	// XEIM DEBUG

	DWORD SendBytes;

	list<XUserDetail>::iterator iUser;
	for (iUser=m_listUserDetail.begin(); iUser!=m_listUserDetail.end(); iUser++)
	{
		// 用户名与密码是否合法
		if ((*iUser) == (*pUser))
		{
			iUser->OnLine();
			XUserInfo xui;
			iUser->GetUserInfo(xui);

			if (! m_bSendUserStatus)
			{
				m_bSendUserStatus = TRUE;
				SetTimer(TIMER_SEND_STATUS, TIMER_SEND_STATUS_PERIOD, NULL);
			}
			m_listUserStatus.push_back(xui);
		//	XSendUserStatusToFriends(xui);
			// 用户是否已在别的地方登录
			map<SOCKET, XUserInfo>::iterator iUserInfo;
			for (iUserInfo=m_mapUserInfos.begin(); iUserInfo!=m_mapUserInfos.end(); ++iUserInfo)
			{
				if(iUserInfo->second == xui)
				{
					// 断开旧的连接 这里需要重新处理
					shutdown(iUserInfo->first, SD_BOTH);
					// 从列表中删除
					m_mapUserInfos.erase(iUserInfo);
					// 把新的插入表中
					m_mapUserInfos.insert(pair<SOCKET, XUserInfo>(sock, xui));

					// 从在线用户树里把用户删除
					HTREEITEM hItem = m_tcUserTree.GetChildItem(m_hRoot);
					while (hItem)
					{
						if (xui.GetUNickName() == m_tcUserTree.GetItemText(hItem))
						{
							m_tcUserTree.DeleteItem(hItem);
							break;
						}
						hItem = m_tcUserTree.GetNextSiblingItem(hItem);
					}

					CString strErr;
					strErr.Format("用户在别处登录 - %s", xui.GetUID());
					AddErrorInfo(strErr);

					break;
				}

				
			}
			// 用户未登录
			if (iUserInfo == m_mapUserInfos.end())
			{
				m_mapUserInfos.insert(pair<SOCKET, XUserInfo>(sock, xui));
			}

			// 左侧在线用户数量
			CString strRoot;
			strRoot.Format("在线用户(%d)", m_mapUserInfos.size());
			m_tcUserTree.SetItemText(m_hRoot, strRoot);

			// 显示状态为在线
			int nItemCount = m_lcUserList.GetItemCount();
			for (int i=0; i<nItemCount; i++)
			{
				char buf[32];
				m_lcUserList.GetItemText(i, 1, buf, 32);
				if (! strcmp(buf, iUser->GetUID()))
				{
				//	m_lcUserList.SetItemText(i, 3, "在线");
					m_lcUserList.SetItem(i, 3, LVIF_TEXT|LVIF_IMAGE, "在线", I_IMAGECALLBACK, LVIS_STATEIMAGEMASK, 0, 0);
					m_lcUserList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
					m_lcUserList.SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);

					// 左侧树控件处理
					m_tcUserTree.SetRedraw(FALSE);
					HTREEITEM hTree;
					hTree = m_tcUserTree.InsertItem(iUser->GetUNickName(), 4, 4, m_hRoot);
					m_tcUserTree.SetItemData(hTree, i);
					m_tcUserTree.SetRedraw();

					break;
				}
			}
			// 答复用户消息
			XMSG msg;
			msg.m_nMessage = XEIMMSG_USEROK;
			XUserDetail *p = &(*iUser);
			msg.SetData((char*)p, sizeof(XUserDetail));

			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
			int nLen = msg.GetBufferLength();
			PerIoData->send = 0;
			memcpy(PerIoData->Buffer, msg.GetBuffer(), nLen);
			PerIoData->DataBuf.buf = PerIoData->Buffer;
			PerIoData->DataBuf.len = nLen;
			PerIoData->DataSize = DATA_BUFSIZE;

			if(SOCKET_ERROR == WSASend(sock, &(PerIoData->DataBuf), 1, &SendBytes, 0, &(PerIoData->Overlapped), NULL))
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					// XEIM DEBUG
					AddErrorInfo("WSASend error5\n");
					return 0;
				}
			}

			// XEIM DEBUG
			{
				CString strClient;
				sockaddr_in aa;
				int slen = sizeof(aa);
				int rval = 0;
				if (getpeername(sock,(sockaddr*)&aa,&slen) == SOCKET_ERROR)
				{
					strClient = "Unknow info.";
				}
				else
				{
					strIP = inet_ntoa(aa.sin_addr);
					strClient.Format("用户登录 - %s(%s:%d)", pUser->GetUID(), (LPCTSTR)strIP, ntohs(aa.sin_port));
				}
				AddErrorInfo(strClient);

			} // XEIM DEBUG

			// 用户名与密码合法
			break;

		} // End 用户名与密码是否合法
	}
	// 用户名与密码不合法
	if (iUser == m_listUserDetail.end()) 
	{
		XMSG msg;
		msg.m_nMessage = XEIMMSG_USERBAD;
		msg.SetData((char*)pUser, sizeof(XUserDetail));

		int nLen = msg.GetBufferLength();
		ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
		PerIoData->send = 0;
		memcpy(PerIoData->Buffer, msg.GetBuffer(), nLen);
		PerIoData->DataBuf.buf = PerIoData->Buffer;
		PerIoData->DataBuf.len = nLen;
		PerIoData->DataSize = DATA_BUFSIZE;
		if(SOCKET_ERROR == WSASend(sock, &(PerIoData->DataBuf), 1, &SendBytes, 0, &(PerIoData->Overlapped), NULL))
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				AddErrorInfo("WSASend error6\n");
				return 0;
			}
		}
	}

	XDBUserLogin(pUser->GetUID(), "XEIM 2.0", (LPCTSTR)strIP);

	// 清除，防止内存泄漏
	delete pUser;
	return 1;
}

LRESULT CXsvrDlg::XMsgOnLine(WPARAM wParam, LPARAM lParam)
{
	// 发件人的套接字
	SOCKET sock = (SOCKET)lParam;
	LPPER_IO_OPERATION_DATA PerIoData = (LPPER_IO_OPERATION_DATA)wParam;

	DWORD SendBytes;
	XMSG xmsg;
	char *buf = (char*)PerIoData->DataBuf.buf;

	XMSG_ONLINE omsg(buf);

	map<SOCKET, XUserInfo>::iterator imap;
	// 搜索 收件人 的 套接字
	for (imap=m_mapUserInfos.begin();imap!=m_mapUserInfos.end(); ++imap)
	{
		if (! strcmp(imap->second.GetUID(), omsg.GetUID()))
		{
			// 发件人 UID : m_mapUserInfos[sock].GetUID()
			// 收件人 UID : imap->second.GetUID()
			// 信息内容 : XMSG_OFFLINE msgHistory(buf).GetData()

			omsg.SetUID(m_mapUserInfos[sock].GetUID());
			xmsg.m_nMessage = XEIMMSG_NEWMESSAGE;
			xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());
			LPPER_IO_OPERATION_DATA pIo = (LPPER_IO_OPERATION_DATA )GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA));
			ZeroMemory(&(pIo->Overlapped), sizeof(OVERLAPPED));
			memcpy(pIo->Buffer, xmsg.GetBuffer(), xmsg.GetBufferLength());
			pIo->DataBuf.buf = pIo->Buffer;
			pIo->DataBuf.len = xmsg.GetBufferLength();
			pIo->DataSize = xmsg.GetBufferLength();
			pIo->send = 1;
			if(SOCKET_ERROR == WSASend(imap->first, &(pIo->DataBuf), 1, &SendBytes, 0, &(pIo->Overlapped), NULL))
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					AddErrorInfo("WSASend error7\n");
					return 0;
				}
			}
			// 对消息进行记录
			XMSG_ONLINE msgHistory(buf);
			XDBMsgOnLine(m_mapUserInfos[sock].GetUID(), imap->second.GetUID(), msgHistory.GetData());
			break;
		}
	}

	// 恢复 SOCKET 的状态
	ResetSockState(sock, PerIoData);

	delete buf;
	return 1;
}

// 处理客户端发送的离线消息
LRESULT CXsvrDlg::XMsgOffLine(WPARAM wParam, LPARAM lParam)
{
	// 发件人的套接字
	SOCKET sock = (SOCKET)lParam;

	LPPER_IO_OPERATION_DATA PerIoData = (LPPER_IO_OPERATION_DATA)wParam;

	char *buf = (char*)PerIoData->DataBuf.buf;

	XMSG_OFFLINE omsg(buf);
	// 发件人 UID : m_mapUserInfos[sock].GetUID()
	// 收件人 UID : imap->second.GetUID() 或 omsg.GetUID()
	// 信息内容 : XMSG_OFFLINE msgHistory(buf).GetData()

	// 对消息进行记录
	XDBMsgOffLine(m_mapUserInfos[sock].GetUID(), omsg.GetUID(buf), omsg.GetData(buf));

	// 恢复 SOCKET 的状态
	ResetSockState(sock, PerIoData);

	delete [] buf;
	return 1;
}

LRESULT CXsvrDlg::XOnLogoff(WPARAM wParam, LPARAM lParam)
{
	SOCKET sIndex = (SOCKET)lParam;

	map<SOCKET, XUserInfo>::iterator iUserInfo;
	iUserInfo = m_mapUserInfos.find(sIndex);
	if (iUserInfo == m_mapUserInfos.end())
	{
	//	Sleep(50);
		return 0;
	}

	// 用户登出，对数据库进行标识
	XDBUserLogout(m_mapUserInfos[sIndex].GetUID());
	
	CString strText = m_mapUserInfos[sIndex].GetUNickName();
	m_mapUserInfos[sIndex].OffLine();
	if (! m_bSendUserStatus)
	{
		m_bSendUserStatus = TRUE;
		SetTimer(TIMER_SEND_STATUS, TIMER_SEND_STATUS_PERIOD, NULL);
	}

	m_listUserStatus.push_back(m_mapUserInfos[sIndex]);

	// 从在线用户树里把用户删除
	HTREEITEM hItem = m_tcUserTree.GetChildItem(m_hRoot);
	while (hItem)
	{
		if (strText == m_tcUserTree.GetItemText(hItem))
		{
			m_tcUserTree.DeleteItem(hItem);
			break;
		}
		hItem = m_tcUserTree.GetNextSiblingItem(hItem);
	}
	// 显示状态为离线
	int nItemCount = m_lcUserList.GetItemCount();
	for (int i=0; i<nItemCount; i++)
	{
		char buf[32];
		m_lcUserList.GetItemText(i, 1, buf, 32);
		if (! strcmp(buf, m_mapUserInfos[sIndex].GetUID()))
		{
			m_lcUserList.SetItemText(i, 3, "离线");
			m_lcUserList.SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
			m_lcUserList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			m_lcUserList.SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
			break;
		}
	}
	list<XUserDetail>::iterator iDetail;
	for (iDetail=m_listUserDetail.begin(); iDetail!=m_listUserDetail.end(); ++iDetail)
	{
		XUserInfo xui;
		iDetail->GetUserInfo(xui);
		if (m_mapUserInfos[sIndex] == xui)
		{
			iDetail->OffLine();
		}
	}

	// XEIM DEBUG
	CString strInfo;
	strInfo.Format("用户离线 - %s", m_mapUserInfos[sIndex].GetUID());
	AddErrorInfo(strInfo);
	// END XEIM DEBUG

	// 从列表里删除
	m_mapUserInfos.erase(sIndex);
	int nSize = m_mapUserInfos.size();
	CString strRoot;
	strRoot.Format("在线用户(%d)", nSize);
	m_tcUserTree.SetItemText(m_hRoot, strRoot);

	if (nSize == 0)
		m_tcUserTree.SetItemImage(m_hRoot, 1, 1);

	return 1;
}

// 用户请求用户列表（此模块将发送 在线 与 非在线 所有用户）
// 数据包大小是 1024 × 2 不可以超过这个数，否则会出现异常
LRESULT CXsvrDlg::XOnGetList(WPARAM wParam, LPARAM lParam)
{
	SOCKET sock = (SOCKET)lParam;
	LPPER_IO_OPERATION_DATA PerIoData = (LPPER_IO_OPERATION_DATA)wParam;
	// 恢复 SOCKET 的状态
	ResetSockState(sock, PerIoData);

	m_listContactQuery.push_back(sock);

	if (! m_bSendUserContact)
	{
	//	AddErrorInfo("分配资源");
		SetTimer(TIMER_SEND_CONTACT, TIMER_SEND_CONTACT_PERIOD, NULL);
		m_bSendUserContact = TRUE;
	}

	return 1;
} // end 用户请求用户列表（此模块将发送 在线 与 非在线 所有用户）

void CXsvrDlg::XSendUserStatusToFriends(XUserInfo &xui)
{
	// TODO: Add your control notification handler code here
	list<string> list_contact;
	// SQL 查询语句
	char * szSQL = "SELECT * FROM x_contact where uid='%s'";
	CString strSQL;
	strSQL.Format(szSQL, xui.GetUID());

	// ADO 操作模块
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));

	// 执行相应处理，避免表 为空时的异常
	try
	{
		pRs->Open((LPCTSTR)strSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		// 如果数据库是空白的
		if (! pRs->adoEOF)
		{
			// 读取并显示数据库里的信息

			while (!pRs->adoEOF)
			{
				variant_t var;

				// 获取用户昵称
				var = pRs->GetCollect(_variant_t("xucontact"));
				if (var.vt != VT_NULL)
				{
					list_contact.push_back((LPCTSTR)_bstr_t(var));
				}
				pRs->MoveNext();	
			}
		}
	}
	catch (_com_error e)
	{
		AddErrorInfo((LPCTSTR)e.Description());
	}
	// 释放 ADO 对象
	pRs->Close();
	pRs.Release();


	list<string>::iterator iContact;
	DWORD SendBytes;
	map<SOCKET, XUserInfo>::iterator iUserInfo;

	for (iUserInfo=m_mapUserInfos.begin(); iUserInfo!=m_mapUserInfos.end(); ++iUserInfo)
	{
		// 用户处于离线状态就略过
		if (iUserInfo->second.GetUStatus() == STATUS_OFFLINE)
			continue;

		string strUID = iUserInfo->second.GetUID();
		iContact = find(list_contact.begin(), list_contact.end(), strUID);
	
		if (iContact != list_contact.end())
		{
			LPPER_IO_OPERATION_DATA PerIoData;
			if ((PerIoData = (LPPER_IO_OPERATION_DATA )GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA))) == NULL)
			{
				CString strInfo;
				strInfo.Format("GlobalAlloc failed with error %d\n", GetLastError());
				AddErrorInfo(strInfo);
				return;
			}
			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
			PerIoData->hWnd = (HWND)GetSafeHwnd();
			PerIoData->send = 1;

			XMSG xmsg;
			xmsg.m_nMessage = XEIMMSG_USERSTATUS;
			xmsg.SetData((char*)&xui, sizeof(XUserInfo));

			memcpy(PerIoData->Buffer, xmsg.GetBuffer(), xmsg.GetBufferLength());
			PerIoData->DataBuf.buf = PerIoData->Buffer;
			PerIoData->DataBuf.len = xmsg.GetBufferLength();
			PerIoData->DataSize = xmsg.GetBufferLength();
			if(SOCKET_ERROR == WSASend(iUserInfo->first, &(PerIoData->DataBuf), 1, &SendBytes, 0, &(PerIoData->Overlapped), NULL))
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					CString str;
					str.Format("WSASend error with code: %d", WSAGetLastError());
					AddErrorInfo(str);
				}
			}
		}
	}
}


//做为解Url使用
char CharToInt(char ch){
	if(ch>='0' && ch<='9')return (char)(ch-'0');
	if(ch>='a' && ch<='f')return (char)(ch-'a'+10);
	if(ch>='A' && ch<='F')return (char)(ch-'A'+10);
	return -1;
}

char StrToBin(char *str){
	char tempWord[2];
	char chn;
	
	tempWord[0] = CharToInt(str[0]);                         //make the B to 11 -- 00001011
	tempWord[1] = CharToInt(str[1]);                         //make the 0 to 0  -- 00000000
	
	chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000
	
	return chn;
}

string UrlGB2312Decode(string str)
{
	string output="";
	char tmp[2];
	int i=0,len=str.length();
	
	while(i<len){
		if(str[i]=='%'){
			tmp[0]=str[i+1];
			tmp[1]=str[i+2];
			output += StrToBin(tmp);
			i=i+3;
		}
		else if(str[i]=='+'){
			output+=' ';
			i++;
		}
		else{
			output+=str[i];
			i++;
		}
	}
	
	return output;
}


BOOL IsPhoneNumber(CString strNUM)
{
	int nLen = strNUM.GetLength();
	if (nLen != 11)
	{
		return FALSE;
	}
	else
	{
		const char *tmp = (LPCTSTR)strNUM;
		int i=0;
		while (tmp[i])
		{
			if (isdigit(tmp[i]))
				i++;
			else
				return FALSE;
		}
	}
	return TRUE;
}


// SMS Processing //////////////////////////////////////////////////////////////////////////
LPSTR ChangeCODE(LPCTSTR lpText)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar (CP_ACP, 0, lpText, -1, NULL, 0);

	WCHAR *tmp = new WCHAR[dwMinSize];
	// Convert headers from ASCII to Unicode.
    MultiByteToWideChar (CP_ACP, 0, lpText, -1, tmp, dwMinSize);

	char *newTmp = new char[dwMinSize*4];
	memset(newTmp, 0, (dwMinSize-1)*4);

	for(DWORD i=0; i<dwMinSize-1; i++)
	{
		char buf[5];
		sprintf(buf, "%04X", tmp[i]);
		strcat(newTmp, buf);
	}
	char *retTmp = new char[dwMinSize*4+2];
	sprintf(retTmp, "%02X%s", strlen(newTmp)/2, newTmp);
	delete [] newTmp;
//	printf("%s", newTmp);
	return retTmp;
}


LPSTR SMS_Phone(LPCTSTR lpText)
{
	char *tmp = new char[15];
	tmp[0] = '6';
	tmp[1] = '8';
	for (int i=0; i<6; i++)
	{
		tmp[i*2+2] = lpText[i*2+1];
		tmp[i*2+3] = lpText[i*2];
	}
	tmp[12] = 'F';
	tmp[14] = NULL;

	char *ret = new char[20+8+6];
	sprintf(ret, "%s%s%s", "11000D91", tmp, "000800");
	delete [] tmp;

	return ret;
}
// SMS Processing //////////////////////////////////////////////////////////////////////////



LPSTR SMS_Center_CODE(LPCTSTR lpText)
{
	char *tmp = new char[20];
	tmp[0] = '9';
	tmp[1] = '1';
	tmp[2] = '6';
	tmp[3] = '8';
	for (int i=0; i<6; i++)
	{
		tmp[i*2+4] = lpText[i*2+1];
		tmp[i*2+5] = lpText[i*2];
	}
	tmp[14] = 'F';
	tmp[16] = NULL;
	int len = strlen(tmp);
	char szLen[3];
	sprintf(szLen, "%02d", len/2);
	for(int i=0; i<18; i++)
	{
		tmp[19-i] = tmp[17-i];
	}
	tmp[0] = szLen[0];
	tmp[1] = szLen[1];

	return tmp;
}

BOOL CXsvrDlg::XSendMsgToUser(const char *szUID, const char *szMSG)
{
	XMSG xmsg;
	DWORD SendBytes;
	XMSG_ONLINE omsg;
	omsg.SetData("xx--11--", szMSG, strlen(szMSG)+1);

	map<SOCKET, XUserInfo>::iterator imap;

	for (imap=m_mapUserInfos.begin();
	imap!=m_mapUserInfos.end(); ++imap)
	{
		if (! _stricmp(imap->second.GetUID(), szUID))
		{
			xmsg.m_nMessage = XEIMMSG_NEWMESSAGE;
			xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());
			LPPER_IO_OPERATION_DATA pIo = (LPPER_IO_OPERATION_DATA )GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA));
			ZeroMemory(&(pIo->Overlapped), sizeof(OVERLAPPED));
			memcpy(pIo->Buffer, xmsg.GetBuffer(), xmsg.GetBufferLength());
			pIo->DataBuf.buf = pIo->Buffer;
			pIo->DataBuf.len = xmsg.GetBufferLength();
			pIo->DataSize = xmsg.GetBufferLength();
			pIo->send = 1;
			if(SOCKET_ERROR == WSASend(imap->first, &(pIo->DataBuf), 1, &SendBytes, 0, &(pIo->Overlapped), NULL))
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					AddErrorInfo("WSASend error3\n");
					return FALSE;
				}
			}
			break;
		}
	}
	// 用户不在线
	if (imap == m_mapUserInfos.end())
	{
		return FALSE;
	}

	return TRUE;
}

void CXsvrDlg::OnGetDispInfo (NMHDR* pnmhdr, LRESULT* pResult)
{
	LV_DISPINFO* pdi = (LV_DISPINFO *) pnmhdr;
	// Fill in the LV_ITEM structure with the image info.
	// When an item is selected, the image is set to the first
	// image (the new bitmap on the toolbar).
	// When it is not selected, the image index is equal to the
	// item number (that is, 0=new, 1=open, 2=save, and so on.)
	CString str;
	str = m_lcUserList.GetItemText(pdi->item.iItem, 3);
	if (strcmp((LPCTSTR)str, "离线"))
		pdi->item.iImage = 4;
	else
		pdi->item.iImage = 3;
}

void CXsvrDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	if (IDYES == MessageBox("如果退出，所有客户端将会断开！\r\n确定要退出 XEIM 沟通专家 吗？", "警告", MB_YESNO|MB_ICONQUESTION))
	{
		m_sysTray.HideIcon();
		m_sysTray.RemoveIcon();
		if (m_pSplitDlg)
		{
			delete m_pSplitDlg;
			m_pSplitDlg = NULL;
		}
		CDialog::OnCancel();
	}
}

void CXsvrDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);
//	CDialog::OnClose();
}

void CXsvrDlg::OnTrayRestore()
{
	ShowWindow(SW_SHOW);
//	m_sysTray.HideIcon();
}

LRESULT CXsvrDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	// Delegate all the work back to the default implementation in CSystemTray.
	return m_sysTray.OnTrayNotification(wParam, lParam);	
}

void CXsvrDlg::OnMenuabout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CXsvrDlg::OnMenuquit() 
{
	// TODO: Add your command handler code here

	OnCancel();
}

void CXsvrDlg::OnMenuUser() 
{
	// TODO: Add your command handler code here
	CXUserEditorDlg dlg;
	if (IDOK == dlg.DoModal())
	{
	}
}

// 修改用户信息
void CXsvrDlg::XUserUpdate(XUserDetail &detail,  CString strMobile)
{
	// SQL 语句
	char *sql="update userinfo set "
	"unickname='%s',"
	"upassword='%s',"
	"ugroup='%s',"
	"umobile='%s' "
	"WHERE uid='%s'";

//	char *t = new char[sizeof(XUserDetail)];
	TRACE("sizeof（XUserDetail） = %d \r\n", sizeof(XUserDetail));

	CString strSQL;
	strSQL.Format(sql,
		detail.GetUNickName(),
		detail.GetUPassword(),
		detail.GetUGroup(),
		strMobile,
		detail.GetUID());

//	AfxMessageBox(strSQL);
	try
	{
		_variant_t vAffected;
		m_pConn->CursorLocation = adUseClient;
		m_pConn->Execute((LPCTSTR)strSQL, &vAffected, adExecuteNoRecords);
	}
	catch (_com_error e)
	{
		MessageBox(e.Description());
		return;
	}
}

// 添加一个新的用户
void CXsvrDlg::XUserAddNew(XUserDetail &detail, CString strMobile)
{
	int len = sizeof (XUserDetail) + 64;
	char *t = new char[len];

	char *sql = "INSERT INTO userinfo("
			"uid,"
			"unickname,"
			"upassword,"
			"ugroup,"
			"umobile) "
			"VALUES('%s','%s','%s','%s','%s')";

	CString strSQL;
	strSQL.Format(sql,
		detail.GetUID(),
		detail.GetUNickName(),
		detail.GetUPassword(),
		detail.GetUGroup(),
		strMobile);

	try
	{
		_variant_t vAffected;
		m_pConn->CursorLocation = adUseClient;
		m_pConn->Execute((LPCTSTR)strSQL, &vAffected, adExecuteNoRecords);
	}
	catch (_com_error e)
	{
		MessageBox(e.Description());
		delete t;
		t = NULL;
		return;
	}

	m_listUserDetail.push_back(detail);

	// 防止内存泄漏
	delete t;

	LVITEM lvi;
	int nIdx = 0;

	lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
	lvi.iItem = 0xff;
	lvi.iSubItem = 0;
	lvi.pszText = detail.GetUNickName();
	lvi.iImage = I_IMAGECALLBACK;		// There are 8 images in the image list
	nIdx = m_lcUserList.InsertItem(&lvi);

	m_lcUserList.SetItemText(nIdx, 1, detail.GetUID());
	m_lcUserList.SetItemText(nIdx, 2, detail.GetUPassword());
	m_lcUserList.SetItemText(nIdx, 3, "离线");
	m_lcUserList.SetItemText(nIdx, 4, detail.GetUGroup());

//	XUserDetail *pUser = new XUserDetail(detail);

//	delete pUser;
}

BOOL CXsvrDlg::XUserExisted(const char *szUID)
{
	list<XUserDetail>::iterator iUserDetail =
		find(m_listUserDetail.begin(), m_listUserDetail.end(), strlwr((LPSTR)szUID));

	// 用户存在
	if (iUserDetail != m_listUserDetail.end())
		return TRUE;

	return FALSE;
}

void CXsvrDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CPoint pt(pNMListView->ptAction);

	CMenu menu;
	int nID;

	POINT p;
	p.x = pt.x;
	p.y = pt.y;

	char szUID[256];
	m_lcUserList.GetItemText(pNMListView->iItem, 1, szUID, 256);

	char szUStatus[16];
	m_lcUserList.GetItemText(pNMListView->iItem, 3, szUStatus, 16);

	POSITION pos = m_lcUserList.GetFirstSelectedItemPosition();
	if (pos != NULL)					//判断是否选种了某一行
	{
		menu.LoadMenu(IDM_LISTCTRL);				// our context menu
		CMenu* pContextMenu = menu.GetSubMenu(0);
		if (! strcmp(szUStatus, "在线"))
			pContextMenu->EnableMenuItem(1, MF_BYPOSITION|MF_GRAYED);

		::ClientToScreen(pNMHDR->hwndFrom, &p);
		nID = pContextMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD,
		p.x, p.y, this);
	}
	else
	{
		menu.LoadMenu(IDM_LISTAddNEW);				// our context menu
		CMenu* pContextMenu = menu.GetSubMenu(0);

		::ClientToScreen(pNMHDR->hwndFrom, &p);
		nID = pContextMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD,
		p.x, p.y, this);
	}

	switch (nID)
	{
	case ID_MENUADDNEW:					// 添加用户
		OnMenuUser();
		break;

	case ID_MENU_MODIFY:				// 修改用户
		OnDblclkList1(pNMHDR, pResult);
		break;

	case ID_MENU_DELETE:				// 删除用户
		{
			char *sql="DELETE FROM userinfo WHERE uid='%s'";
			char szAlert[272];
			CString strSQL;
			strSQL.Format(sql, szUID);
			sprintf(szAlert, "确定要删除用户 %s 吗？", szUID);
			if (IDYES == ::MessageBox(m_hWnd, szAlert, "删除用户", MB_YESNO|MB_ICONQUESTION))
			{
				try
				{
					_variant_t vAffected;
					m_pConn->CursorLocation = adUseClient;
					m_pConn->Execute((LPCTSTR)strSQL, &vAffected, adExecuteNoRecords);
				}
				catch(_com_error e)
				{
					MessageBox(e.Description());
				}
				m_lcUserList.DeleteItem(pNMListView->iItem);	
				list<XUserDetail>::iterator iUserDetail =
					find(m_listUserDetail.begin(), m_listUserDetail.end(), strlwr(szUID));
				m_listUserDetail.remove(*iUserDetail);
			}
		}
		break; // End 删除用户
	}
	// Now, based on the nID value, decide what to do with the menu selection
	// The menu is a temporary MFC object, no need to delete it.

	*pResult = 0;
}

void CXsvrDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	POSITION pos = m_lcUserList.GetFirstSelectedItemPosition();

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (NULL != pos)
	{
		char szUID[256];
		m_lcUserList.GetItemText(pNMListView->iItem, 1, szUID, 256);

		list<XUserDetail>::iterator iUserDetail =
					find(m_listUserDetail.begin(), m_listUserDetail.end(), strlwr(szUID));

		// 打开修改对话框
		CXUserEditorDlg dlg;
		dlg.SetData((*iUserDetail));
		CString strMobile;
		GetUIDMobileNUM((*iUserDetail).GetUID(), strMobile);
		dlg.SetMobile(strMobile);
	//	AfxMessageBox(strMobile);
		if (IDOK == dlg.DoModal())
		{
			m_lcUserList.SetItemText(pNMListView->iItem, 0, dlg.m_strNickName);
			m_lcUserList.SetItemText(pNMListView->iItem, 2, dlg.m_strPassword);
			m_lcUserList.SetItemText(pNMListView->iItem, 4, dlg.m_strGroup);

			// 更新内存中的用户列表
			list<XUserDetail>::iterator iUserDetail =
				find(m_listUserDetail.begin(), m_listUserDetail.end(), strlwr(szUID));

			iUserDetail->SetUNickName(dlg.m_strNickName);
			iUserDetail->SetUPassword(dlg.m_strPassword);
			iUserDetail->SetUGroup(dlg.m_strGroup);
		}
	}
	*pResult = 0;
}

// 转发用户文件消息
LRESULT CXsvrDlg::XOnNewFile(WPARAM wParam, LPARAM lParam)
{
	SOCKET sock = (SOCKET)lParam;
	LPPER_IO_OPERATION_DATA PerIoData = (LPPER_IO_OPERATION_DATA)wParam;

	DWORD SendBytes;
	XMSG xmsg;

	char *buf = (char*)PerIoData->DataBuf.buf;

	XMSG_ONLINE omsg(buf);

	map<SOCKET, XUserInfo>::iterator imap;
	for (imap=m_mapUserInfos.begin();
		imap!=m_mapUserInfos.end(); ++imap)
	{
		if (! strcmp(imap->second.GetUID(), omsg.GetUID()))
		{
			// 发送者 UID m_mapUserInfos[sock].GetUID()
			// 接收者 UID omsg.GetUID()

			omsg.SetUID(m_mapUserInfos[sock].GetUID());
			xmsg.m_nMessage = XEIMMSG_FILE;
			xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());
			LPPER_IO_OPERATION_DATA pIo = (LPPER_IO_OPERATION_DATA )GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA));
			ZeroMemory(&(pIo->Overlapped), sizeof(OVERLAPPED));
			memcpy(pIo->Buffer, xmsg.GetBuffer(), xmsg.GetBufferLength());
			pIo->DataBuf.buf = pIo->Buffer;
			pIo->DataBuf.len = xmsg.GetBufferLength();
			pIo->DataSize = xmsg.GetBufferLength();
			pIo->send = 1;
			if(SOCKET_ERROR == WSASend(imap->first, &(pIo->DataBuf), 1, &SendBytes, 0, &(pIo->Overlapped), NULL))
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					AddErrorInfo("WSASend error4\n");
					return 0;
				}
			}

			ResetSockState(sock, PerIoData);
			break;
		}
	}

	delete buf;

	return 1;
}

// 界面上的处理
void CXsvrDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here

	if (::IsWindow(m_lcUserList.GetSafeHwnd()))
	{
		CRect rtDebug;
		GetDlgItem(IDC_EDIT1)->GetWindowRect(&rtDebug);
		ScreenToClient(&rtDebug);

		CRect rt;
		m_lcUserList.GetWindowRect(&rt);
		ScreenToClient(&rt);

		CRect rt2;
		m_tcUserTree.GetWindowRect(&rt2);
		ScreenToClient(&rt2);

		CRect rt3;
		GetDlgItem(IDC_STATIC8)->GetWindowRect(&rt3);
		ScreenToClient(&rt3);

		CRect rtSplit;
		m_pSplitDlg->GetWindowRect(&rtSplit);
		ScreenToClient(&rtSplit);

		m_lcUserList.MoveWindow(rt.left, rt.top, rt.Width()+cx-rt.right, rt.Height()/*+cy-rt.bottom-31*/);
		m_tcUserTree.MoveWindow(rt2.left, rt2.top, rt2.Width(), rt2.Height()/* + cy-rt.bottom - 31*/);
		GetDlgItem(IDC_STATIC8)->MoveWindow(rt3.left+cx-rt3.right,
											rt3.top+cy-rt3.bottom-9,
											rt3.Width(),
											rt3.Height());

		GetDlgItem(IDC_EDIT1)->MoveWindow(rtDebug.left, rtDebug.top, rtDebug.Width()+cx-rtDebug.right, rtDebug.Height()+cy-rtDebug.bottom-31);
		m_pSplitDlg->MoveWindow(rtSplit.left, rtSplit.top, rtSplit.Width(), rtSplit.Height()/*+cy-rtSplit.bottom-31*/);
	}
}

void CXsvrDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);
//	EASYSIZE_MINSIZE(629,450,fwSide,pRect);
}

// 调试信息输出
void CXsvrDlg::AddErrorInfo(CString strErr)
{
	EnterCriticalSection(& m_pApp->g_cs);
	CStdioFile file;
	char buf[MAX_PATH];
	GetProgramDirectory(buf);
	// XEIM DEBUG
	CString strOut;
	GetDlgItemText(IDC_EDIT1, strOut);
	strOut += "\r\n";

	CTime m_StartTime1 = CTime::GetCurrentTime();
	CString strTime1 = m_StartTime1.Format(_T("%H:%M:%S - "));
	// 记录的时间
	strOut += strTime1;

	strOut += strErr;
	SetDlgItemText(IDC_EDIT1, strOut);

	CEdit* pEd=(CEdit*)GetDlgItem(IDC_EDIT1);
	pEd->PostMessage(WM_VSCROLL, SB_BOTTOM);


	CString fileName;
	fileName.Format("%s%s", buf, "sys.log");
	file.Open(fileName, CFile::modeWrite|CFile::shareDenyNone, NULL);
	file.SeekToEnd();
	file.WriteString("\r\n");
	file.WriteString(strTime1);
	file.WriteString(strErr);
	file.Close();
	// END XEIM DEBUG
	LeaveCriticalSection(& m_pApp->g_cs);
}

void CXsvrDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
		// 检查ERP 接口是否更新
	case TIMER_CHECK_ERP:
		{
			_ERP_CheckDatabase();
		}
		break;
	// 一次发送一次用户状态
	case TIMER_SEND_STATUS:
		{
			XSendUserStatusToFriends(m_listUserStatus.back());
			m_listUserStatus.pop_back();

			if (m_listUserStatus.size() == 0)
			{
				KillTimer(TIMER_SEND_STATUS);
				m_bSendUserStatus = FALSE;
			}
		}
		break;
	// 一次发送一个用户的联系人信息
	case TIMER_SEND_CONTACT:
		{
			XUser_SendContact();
		}
		break;

	// 检查数据库里是否修改用户信息
	case TIMER_CHECK_DATABASE:
		XCheckUserInfo();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CXsvrDlg::XCheckUserInfo()
{
	list<string> listUID;

	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));

	// SQL QUERY
	char * szSQL = "SELECT uid FROM userinfo";

	variant_t var;
	CString str;

	try
	{
		pRs->Open(szSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		// 如果数据库不是空白的
		// 读取并显示数据库里的用户信息
		if (! pRs->adoEOF)
		{
			while (! pRs->adoEOF)
			{
				// 获取用户 ID
				var = pRs->GetCollect(_variant_t("uid"));
				if (var.vt != VT_NULL)
				{
					listUID.push_back(strlwr((LPSTR)(LPCTSTR)_bstr_t(var)));
				}
				pRs->MoveNext();
			}
		}
		else
		{
			AddErrorInfo("暂无用户记录！");
			pRs->Close();
			pRs.Release();
			return;
		}
	}
	catch (_com_error e)
	{
		AddErrorInfo(e.ErrorMessage());
	}

	// Close it as soon as possible.
	pRs->Close();
	pRs.Release();


	// 与旧表进行对比，把删除或添加的进行处理
	list<XUserDetail>::iterator ilist;
	m_lcUserList.SetRedraw(FALSE);
	for (ilist=m_listUserDetail.begin(); ilist!=m_listUserDetail.end();++ilist)
	{
		// 用户被删除了？
		if (find(listUID.begin(), listUID.end(), (*ilist).GetUID()) == listUID.end())
		{
			// 从用户界面里把那个用户删除
			int nItemCount = m_lcUserList.GetItemCount();
			for (int i=0; i<nItemCount; i++)
			{
				CString strItemText;
				strItemText = m_lcUserList.GetItemText(i, 1);
				if (strItemText == (*ilist).GetUID())
				{
					m_lcUserList.DeleteItem(i);
					break;
				}
			}

			list<XUserDetail>::iterator itemp = ilist;
			-- itemp;

			m_listUserDetail.remove((*ilist));
			ilist=itemp;

		}

		// 用户没被更改！
		else
		{
			listUID.remove((*ilist).GetUID());
		}
	}

	// 把增加的用户添加到程序里
	int nSize = listUID.size();
	for(int i=0; i<nSize; i++)
	{
		XUserDetail detail;
		string sTemp;
		sTemp = listUID.front();
		listUID.pop_front();

		XDB_ReadUserDetail(sTemp.c_str(), detail);
		m_listUserDetail.push_back(detail);
	}

	m_lcUserList.SetRedraw();

	listUID.clear();
}

void CXsvrDlg::XDB_ReadUserDetail(LPCTSTR lpszUID, XUserDetail &xDetail)
{
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));
	char * szSQL = "";
	CString strSQL;
	strSQL.Format("SELECT unickname,upassword,ugroup FROM userinfo WHERE uid='%s'", lpszUID);

	variant_t var[5];
	CString str;
	int nIdx = 0;
	LVITEM lvi;

	try
	{
		pRs->Open((LPCTSTR)strSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		// 如果数据库是空白的
		if (pRs->adoEOF)
		{
			AddErrorInfo("XDB_ReadUserDetail 暂无用户记录！");

			pRs->Close();
			pRs.Release();
			return;
		}

		// 读取并显示数据库里的用户信息
		else
		{
			while (!pRs->adoEOF)
			{
				// 获取用户昵称
				var[0] = pRs->GetCollect(_variant_t("unickname"));
				if (var[0].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[0]);
					xDetail.SetUNickName((LPCTSTR)str);

					lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
					lvi.iItem = 0xff;
					lvi.iSubItem = 0;
					lvi.pszText = (LPTSTR)(LPCTSTR)(str);
					lvi.iImage = I_IMAGECALLBACK;		// There are 8 images in the image list
					nIdx = m_lcUserList.InsertItem(&lvi);
				}

				// 获取用户 ID
				xDetail.SetUID(lpszUID);
				m_lcUserList.SetItemText(nIdx, 1, lpszUID);

				// 获取用户密码
				var[2] = pRs->GetCollect(_variant_t("upassword"));
				if (var[2].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[2]);
					m_lcUserList.SetItemText(nIdx, 2, str);
					m_lcUserList.SetItemText(nIdx, 3, "离线");
					xDetail.SetUPassword((LPCTSTR)str);
				}

				// 获取用户所在分组
				var[3] = pRs->GetCollect(_variant_t("ugroup"));
				if (var[2].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[3]);
					xDetail.SetUGroup((LPCTSTR)str);
					m_lcUserList.SetItemText(nIdx, 4, str);
				}
				var[4] = pRs->GetCollect(_variant_t("umobile"));
				if (var[4].vt != VT_NULL)
				{
				//	str = (LPCTSTR)_bstr_t(var[4]);
					xDetail.m_nDataID = 10;
				}

				pRs->MoveNext();
			}
		}
	}
	catch (_com_error e)
	{
		AddErrorInfo(e.ErrorMessage());
	}

	// Close it as soon as possible.
	pRs->Close();
	pRs.Release();
}

void CXsvrDlg::XDB_InitUserList()
{
//	AddErrorInfo("XDB_InitUserList");
	// *****************************************************************
	// 用户 UI 初始化设置
	// *****************************************************************
	// 设置在线用户列表------------------------------------------------------------------
	CBitmap bm;
	m_ilUser.Create (16, 16, ILC_COLOR24 | ILC_MASK , 5,5);
	bm.LoadBitmap(IDB_BITMAP1);
	m_ilUser.Add(& bm, RGB(255,0,255));
	m_tcUserTree.SetImageList(& m_ilUser, TVSIL_NORMAL);
	m_lcUserList.SetImageList(& m_ilUser, LVSIL_SMALL);

	// Initial extended style for the list control on this dialog
	DWORD dwStyle = m_lcUserList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_lcUserList.SetExtendedStyle(dwStyle);

	m_lcUserList.InsertColumn(0, "昵称", LVCFMT_LEFT, 88);
	m_lcUserList.InsertColumn(1, "帐号", LVCFMT_LEFT, 88);
	m_lcUserList.InsertColumn(2, "密码", LVCFMT_LEFT, 88);
	m_lcUserList.InsertColumn(3, "状态", LVCFMT_LEFT, 55);
	m_lcUserList.InsertColumn(4, "分组", LVCFMT_LEFT, 80);
	m_lcUserList.InsertColumn(5, "手机", LVCFMT_LEFT, 80);

	m_hRoot = m_tcUserTree.InsertItem("在线用户(0)", 1, 1);

	// 检查用户设置文件
	char szFileName[_MAX_PATH];
	GetProgramDirectory(szFileName);
	strcat(szFileName, "xsetting.ini");

	// 使用的数据库类型
	int nDBType = -1;

	CIniReader iniReader(szFileName);
	if (iniReader.sectionExists("db_config"))
	{
		CString strDBType;
		strDBType = iniReader.getKeyValue("dbtype", "db_config");
		nDBType = atoi((LPCTSTR)strDBType);
	//	m_strSvr = iniReader.getKeyValue("server_addr", "xeim_server_info");
	//	iniReader.setKey(((CClient02Dlg*)AfxGetMainWnd())->m_strSvr, "server_addr", "xeim_server_info");
	}
	else
	{
		// 默认使用 Access
		nDBType = 0;
		iniReader.setKey("0", "dbtype", "db_config");
	}

	// *****************************************************************
	// 打开 user.mdb数据库，user.mdb 是ACCESS类型，改扩展名就行
	// *****************************************************************

	CString strConn;
	CString strError;

	switch (nDBType)
	{
	case 1://MySql
		
	case 2: // 使用 SQL
		{
			CString strServer="server";
			strServer = iniReader.getKeyValue("server", "db_config");
			CString strID="xeim";
			strID = iniReader.getKeyValue("ID", "db_config");
			CString strPassword="xeim";
			strPassword = iniReader.getKeyValue("password", "db_config");
			CString strDatabase="xeim";
			strDatabase = iniReader.getKeyValue("database", "db_config");

			/*	MS SQL 的连接语句	*/
			strConn = "Provider=SQLOLEDB.1;Persist Security Info=False;User ID="
			+strID+
			";Password="
			+strPassword+
			";Initial Catalog="
			+ strDatabase+
			";Data Source="
			+strServer;

			strError = "SQL 数据库连接错误！";
		}
		break;
	case 3: // 不支持
		{
			AddErrorInfo("数据库暂不支持！");
			CDialog::OnCancel();
			return;
		}
		break;

	default: // 使用 Access
		{
			strConn = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + m_strDatabase;
			strError = "Access 数据库连接错误！";
		}
	}

	HRESULT hr;

	try
	{
		// m_pConn 在构造函数里，必须初始化为 NULL

		CString m_sConn="Provider=Microsoft.Jet.OLEDB.4.0.1;Data Source=..\\bin\\user.mdb";
		m_pConn.CreateInstance("ADODB.Connection");

		HRESULT hr=m_pConn->Open((_bstr_t)m_sConn,"","",adConnectUnspecified);    
		if (FAILED(hr))
		{
			AfxMessageBox("不能连接数据库 source!");
			return ;
		}
	}
	catch(_com_error& e)
	{
		TRACE("打开数据库异常：%s\r\n", e.ErrorMessage());
		AddErrorInfo(strError);
		AfxMessageBox("CXsvrDlg.XDB_InitUserList.catch");
		// 当数据库连接失败时，直接退出系统。
	//	CDialog::OnCancel();
	//	return;
	}
	if (! m_pConn->State)
	{
		AfxMessageBox("数据库打开失败！");
		m_pConn->Release();
		return;
	}

	// *****************************************************************
	// 读取数据库并显示用户信息
	// *****************************************************************
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));
	char * szSQL = "SELECT unickname,uid,upassword,ugroup,umobile FROM userinfo";
	variant_t var[5];
	CString str;
	int nIdx = 0;
	XUserDetail *pUDetail;
	LVITEM lvi;

	try
	{
		hr = pRs->Open(szSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	//	if (SUCCEEDED(hr))
	//		AddErrorInfo("rs failed.");

		// 如果数据库是空白的
		if (pRs->adoEOF)
		{
			AddErrorInfo("暂无用户记录！");
			pRs->Close();
			pRs.Release();
			return;
		}
		else
		{
			// 读取并显示数据库里的用户信息		
			while (!pRs->adoEOF)
			{
				pUDetail = new XUserDetail;

				// 获取用户昵称
				var[0] = pRs->GetCollect(_variant_t("unickname"));
				if (var[0].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[0]);
					pUDetail->SetUNickName((LPCTSTR)str);

					lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
					lvi.iItem = 0xff;
					lvi.iSubItem = 0;
					lvi.pszText = (LPTSTR)(LPCTSTR)(str);
					lvi.iImage = I_IMAGECALLBACK;		// There are 8 images in the image list
					nIdx = m_lcUserList.InsertItem(&lvi);
				}

				// 获取用户 ID
				var[1] = pRs->GetCollect(_variant_t("uid"));
				if (var[1].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[1]);
					pUDetail->SetUID(strlwr((LPSTR)(LPCTSTR)str));
					m_lcUserList.SetItemText(nIdx, 1, str);
				}

				// 获取用户密码
				var[2] = pRs->GetCollect(_variant_t("upassword"));
				if (var[2].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[2]);
					m_lcUserList.SetItemText(nIdx, 2, str);
					m_lcUserList.SetItemText(nIdx, 3, "离线");
					pUDetail->SetUPassword((LPCTSTR)str);
				}

				// 获取用户所在分组
				var[3] = pRs->GetCollect(_variant_t("ugroup"));
				if (var[3].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[3]);
					pUDetail->SetUGroup((LPCTSTR)str);
					m_lcUserList.SetItemText(nIdx, 4, str);
				}
				// 用户是否有手机号
				var[4] = pRs->GetCollect(_variant_t("umobile"));
				if (var[4].vt != VT_NULL)
				{
					pUDetail->m_nDataID = 10;
					CString strb;
					strb.Format("%d", 10);
					m_lcUserList.SetItemText(nIdx, 5, strb);
				}
				else
				{
					pUDetail->m_nDataID = 5;
					CString strb;
					strb.Format("%d", 5);
					m_lcUserList.SetItemText(nIdx, 5, strb);
				}

				pRs->MoveNext();
				m_listUserDetail.push_back(*pUDetail);

				delete pUDetail;
			}
		}
	}
	catch (_com_error e)
	{
		AddErrorInfo(e.ErrorMessage());
	}

	pRs->Close();
	pRs.Release();
}

void CXsvrDlg::XDBUserLogin(LPCTSTR lpszUID, LPCTSTR lpszVersion, LPCTSTR lpszIP)
{
	ASSERT(lpszUID != NULL);
	ASSERT(lpszVersion != NULL);
	ASSERT(lpszIP != NULL);

	// SQL 语句
	char *sql="update userinfo set "
	"ustatus='%s',"
	"uclient='%s',"
	"uipaddress='%s' "
	"WHERE uid='%s'";

	CString strSQL;
	strSQL.Format(sql,
		"1",
		lpszVersion,
		lpszIP,
		lpszUID);
//	AfxMessageBox(t);
//	return;

	try
	{
		_variant_t vAffected;
		m_pConn->CursorLocation = adUseClient;
		m_pConn->Execute((LPCTSTR)strSQL, &vAffected, adExecuteNoRecords);
	}
	catch (_com_error e)
	{
		MessageBox(e.Description());
		return;
	}
}

void CXsvrDlg::XDBUserLogout(LPCTSTR lpszUID)
{
	// SQL 语句
	char *sql="update userinfo set "
	"ustatus='%s',"
	"uclient='%s' "
	"WHERE uid='%s'";

	CString strSQL;
	strSQL.Format(sql,
		"0",
		"",
		lpszUID);

	try
	{
		_variant_t vAffected;
		m_pConn->CursorLocation = adUseClient;
		m_pConn->Execute((LPCTSTR)strSQL, &vAffected, adExecuteNoRecords);
	}
	catch (_com_error e)
	{
		MessageBox(e.Description());
		return;
	}
}

// 把发送的消息记录到数据库
void CXsvrDlg::XDBMsgSMS(LPCTSTR lpszSendUID, // 发送者UID
							 LPCTSTR lpszRecvUID, // 接收者UID
							 LPCTSTR lpszMsgBody // 消息内容
							 )
{
	CTime today;
	today = CTime::GetCurrentTime();

	CString strSendUID = lpszSendUID;
	CString strRecvUID = lpszRecvUID;
	CString strMSG = lpszMsgBody;
	strMSG.Replace("'", "''");
	
	char *sql = "INSERT INTO sms_list("
			"senduid,"
			"recvuid,"
			"xsmsbody,"
			"xposttime) "
			"VALUES('%s','%s','%s','%s')";

	CString strSQL;
	strSQL.Format(sql,
		lpszSendUID,
		lpszRecvUID,
		strMSG,
		today.Format(_T("%Y-%m-%d %H:%M:%S")));

	try
	{
		_variant_t vAffected;
		m_pConn->CursorLocation = adUseClient;
		m_pConn->Execute((LPCTSTR)strSQL, &vAffected, adExecuteNoRecords);
	}
	catch (_com_error e)
	{
		AddErrorInfo((LPCTSTR)_bstr_t(e.Description()));
		return;
	}

}

// 添加在线消息到历史记录
void CXsvrDlg::XDBMsgOnLine(LPCTSTR lpszSendUID, // 发送者UID
							 LPCTSTR lpszRecvUID, // 接收者UID
							 LPCTSTR lpszMsgBody // 消息内容
							 )
{
/*	CString strMsg;
	strMsg.Format("在线消息添加到历史记录\r\n%s to %s : %s",
		lpszSendUID,
		lpszRecvUID,
		lpszMsgBody);

	AddErrorInfo(strMsg);*/

	CTime today;
	today = CTime::GetCurrentTime();
	
	CString strSendUID = lpszSendUID;
	CString strRecvUID = lpszRecvUID;
	CString strMSG = lpszMsgBody;
	strMSG.Replace("'", "''");

	char *sql = "INSERT INTO msg_list("
			"senduid,"
			"recvuid,"
			"xmsgbody,"
			"xposttime,"
			"xread) "
			"VALUES('%s','%s','%s','%s','1')";

	CString strSQL;
	strSQL.Format(sql,
		lpszSendUID,
		lpszRecvUID,
		strMSG,
		today.Format(_T("%Y-%m-%d %H:%M:%S")));

	try
	{
		_variant_t vAffected;
		m_pConn->CursorLocation = adUseClient;
		m_pConn->Execute((LPCTSTR)strSQL, &vAffected, adExecuteNoRecords);
	}
	catch (_com_error e)
	{
		AddErrorInfo((LPCTSTR)_bstr_t(e.Description()));
		return;
	}
}


// 添加离线线消息到历史记录
void CXsvrDlg::XDBMsgOffLine(LPCTSTR lpszSendUID, // 发送者UID
							 LPCTSTR lpszRecvUID, // 接收者UID
							 LPCTSTR lpszMsgBody // 消息内容
							 )
{
/*	CString strMsg;
	strMsg.Format("离线消息添加到历史记录\r\n%s to %s : %s",
		lpszSendUID,
		lpszRecvUID,
		lpszMsgBody);

	AddErrorInfo(strMsg);*/

	CTime today;
	today = CTime::GetCurrentTime();
	// TEST
	CString strSendUID = lpszSendUID;
	CString strRecvUID = lpszRecvUID;
	CString strMSG = lpszMsgBody;
	strMSG.Replace("'", "''");

	char *sql = "INSERT INTO msg_list("
			"senduid,"
			"recvuid,"
			"xmsgbody,"
			"xposttime,"
			"xread) "
			"VALUES('%s','%s','%s','%s','0')";


	CString strSQL;
	strSQL.Format(sql,
		lpszSendUID,
		lpszRecvUID,
		strMSG,
		today.Format(_T("%Y-%m-%d %H:%M:%S")));

	try
	{
		_variant_t vAffected;
		m_pConn->CursorLocation = adUseClient;
		m_pConn->Execute((LPCTSTR)strSQL, &vAffected, adExecuteNoRecords);
	}
	catch (_com_error e)
	{
		AddErrorInfo((LPCTSTR)_bstr_t(e.Description()));
		return;
	}
}

// 向客户端发送离线消息数据
LRESULT CXsvrDlg::XMsgOffLineData(WPARAM wParam, LPARAM lParam)
{
//	AddErrorInfo("发送离线消息数据");

	LPPER_IO_OPERATION_DATA		PerIoData	= (LPPER_IO_OPERATION_DATA)wParam;
	SOCKET						sock		= (SOCKET)lParam;

	// 从数据库里读取离线消息数据
//	char * szSQL = "SELECT TOP 10 senduid, xmsgbody, xposttime, xread, recvuid FROM msg_list where recvuid='%s' AND xread='0' ORDER BY xitems";
	char * szSQL = "SELECT * FROM msg_list where recvuid='%s' AND xread='0' ORDER BY xitems";

	CString strSQL;
	strSQL.Format(szSQL, m_mapUserInfos[sock].GetUID());

	// ADO 操作模块
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));
	// 执行相应处理，避免表 为空时的异常

	// 所有数据保存于此缓冲区
	char *hugeBuf=NULL;
	// 所有数据的长度
	DWORD dwLen = 0;
	// 离线消息数量
	DWORD dwTotal = 0;
	// 如果用户无离线消息，bHasMsg = FALSE;
	BOOL bHasMsg = TRUE;

	try
	{
		pRs->Open((LPCTSTR)strSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		// 如果数据库不是空白的
		if (! pRs->adoEOF)
		{
			// 读取并显示数据库里的信息

			list<XUserDetail>::iterator iDetail;

			while (!pRs->adoEOF)
			{
				XMSG_OFFLINEDATA *pData = new XMSG_OFFLINEDATA;
				variant_t var[3];

				// 获取用户昵称
				var[0] = pRs->GetCollect(_variant_t("senduid"));
				if (var[0].vt != VT_NULL)
				{
					pData->SetUID((LPCTSTR)_bstr_t(var[0]));
				}

				// 获取消息内容
				var[1] = pRs->GetCollect(_variant_t("xmsgbody"));
				if (var[1].vt != VT_NULL)
				{
					pData->SetData((LPCTSTR)_bstr_t(var[1]));
				}

				// 获取发送日期
				var[2] = pRs->GetCollect(_variant_t("xposttime"));
				if (var[2].vt != VT_NULL)
				{
					pData->SetDate((LPCTSTR)_bstr_t(var[2]));
				}

				// 1L 的使用说明可参考 XEIM 参考资料 2008121301.txt
				pRs->PutCollect(_variant_t("xread"), "1");
				pRs->Update();

				pRs->MoveNext();

				// 消息数量递增
				dwTotal ++;

				if (dwLen == 0)
				{
					dwLen = pData->GetBufferLength();
					hugeBuf = new char[dwLen];
					memcpy(hugeBuf, pData->GetBuffer(), dwLen);
				}
				else
				{
					DWORD len1 = pData->GetBufferLength();
					DWORD newLen = dwLen + len1;

					char *newBuf = new char[newLen];
					// 先保存老的
					memcpy(newBuf, hugeBuf, dwLen);
					// 再保存新的
					memcpy(newBuf+dwLen, pData->GetBuffer(), len1);
					// 旧资源释放
					delete hugeBuf;

					// 更新
					hugeBuf = newBuf;
					dwLen = newLen;
				}

				delete pData;
			}
		}
		else
		{
			// XEIM DEBUG
		/*	CString strInfo;
			strInfo.Format("用户[ %s ]无离线消息.", m_mapUserInfos[sock].GetUID());
			AddErrorInfo(strInfo);*/
			bHasMsg = FALSE;
		}
	}
	catch (_com_error e)
	{
		AddErrorInfo((LPCTSTR)e.Description());
	}
	// 释放 ADO 对象
	pRs->Close();
	pRs.Release();

	XMSG xLength;
	xLength.m_nMessage = XEIMMSG_GETMSGOFFLINE;
	xLength.SetData((char*)&dwLen, sizeof(int));

	// 发送离线消息的长度
	SendDataToClient(sock, xLength.GetBuffer(), xLength.GetBufferLength());

	DWORD sentBytes=0;
	while (sentBytes < dwLen)
	{
		DWORD ret = 0;
		if ((dwLen - sentBytes) < 1000)
		{
			ret = SendDataToClient(sock, hugeBuf+sentBytes, dwLen-sentBytes);
			ret = dwLen - sentBytes;
		}
		else
		{
			ret = SendDataToClient(sock, hugeBuf+sentBytes, 1000);
			ret = 1000;
		}
		sentBytes += ret;
	}

	delete hugeBuf;
	hugeBuf = NULL;

	// 恢复 SOCKET 的状态
	ResetSockState(sock, PerIoData);

	// XEIM DEBUG
	if (! bHasMsg)
		return 0;
	CString strInfo;
	strInfo.Format("已向用户[ %s ]发送 %d 条离线消息", m_mapUserInfos[sock].GetUID(), dwTotal);
	AddErrorInfo(strInfo);
	// END XEIM DEBUG
	return 1;
}

// 客户端请求消息历史记录
LRESULT CXsvrDlg::XGetMsgHistory(WPARAM wParam, LPARAM lParam)
{
	DWORD						SendBytes	= 0;
	LPPER_IO_OPERATION_DATA		PerIoData	= (LPPER_IO_OPERATION_DATA)wParam;
	SOCKET						sock		= (SOCKET)lParam;
	char *buf = PerIoData->DataBuf.buf;

	int nSize = 10;
	XMSG xLength;
	xLength.m_nMessage = XEIMMSG_RECVHISTORYDATA;
	xLength.SetData(buf, PerIoData->DataBuf.len);

	SendDataToClient(sock, xLength.GetBuffer(), xLength.GetBufferLength());

	// XEIM DEBUG
	CString strInfo;
	strInfo.Format("通知客户端[ %s ]开始接收历史消息数据.", m_mapUserInfos[sock].GetUID());
	AddErrorInfo(strInfo);

	// 恢复 SOCKET 的状态
	ResetSockState(sock, PerIoData);
	return 1;
}

// 客户端请求消息历史记录数据
LRESULT CXsvrDlg::XMsgHistoryData(WPARAM wParam, LPARAM lParam)
{
	LPPER_IO_OPERATION_DATA		PerIoData	= (LPPER_IO_OPERATION_DATA)wParam;
	SOCKET						sock		= (SOCKET)lParam;
	char *buf2 = PerIoData->DataBuf.buf;

	char * buf = buf2;
	char * szdate = buf2 + 50;

	// 从数据库里读取离线消息数据
	char * szSQL = "SELECT recvuid, senduid, xmsgbody, xposttime FROM msg_list "
		"where ((senduid='%s' AND recvuid='%s') "
		"OR (senduid='%s' AND recvuid='%s')) "
		"AND xposttime LIKE '%s%%' ORDER BY xitems";

	CString strSQL;
	strSQL.Format(szSQL,
		m_mapUserInfos[sock].GetUID(),
		buf,
		buf,
		m_mapUserInfos[sock].GetUID(),
		szdate);

	// ADO 操作模块
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));
	// 执行相应处理，避免表 为空时的异常

	// 所有数据保存于此缓冲区
	char *hugeBuf=NULL;
	// 所有数据的长度
	DWORD dwLen = 0;
	// 离线消息数量
	DWORD dwTotal = 0;
	// 有聊天历史
	BOOL bHasHistory = FALSE;

	try
	{
		pRs->Open((LPCTSTR)strSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		// 如果数据库不是空白的
		if (! pRs->adoEOF)
		{
			// 读取并显示数据库里的信息

			list<XUserDetail>::iterator iDetail;

			while (!pRs->adoEOF)
			{
				XMSG_OFFLINEDATA *pData = new XMSG_OFFLINEDATA;
				variant_t var[3];

				// 获取用户昵称
				var[0] = pRs->GetCollect(_variant_t("senduid"));
				if (var[0].vt != VT_NULL)
				{
					if (! strcmp((LPCTSTR)_bstr_t(var[0]), buf))
						pData->SetUID((LPCTSTR)_bstr_t(var[0]));
					else
						pData->SetUID(m_mapUserInfos[sock].GetUID());
				}

				// 获取消息内容
				var[1] = pRs->GetCollect(_variant_t("xmsgbody"));
				if (var[1].vt != VT_NULL)
				{
					pData->SetData((LPCTSTR)_bstr_t(var[1]));
				}

				// 获取发送日期
				var[2] = pRs->GetCollect(_variant_t("xposttime"));
				if (var[2].vt != VT_NULL)
				{
					pData->SetDate((LPCTSTR)_bstr_t(var[2]));
				}

				pRs->MoveNext();

				// 消息数量递增
				dwTotal ++;

				if (dwLen == 0)
				{
					dwLen = pData->GetBufferLength();
					hugeBuf = new char[dwLen];
					memcpy(hugeBuf, pData->GetBuffer(), dwLen);
				}
				else
				{
					DWORD len1 = pData->GetBufferLength();
					DWORD newLen = dwLen + len1;

					char *newBuf = new char[newLen];
					// 先保存老的
					memcpy(newBuf, hugeBuf, dwLen);
					// 再保存新的
					memcpy(newBuf+dwLen, pData->GetBuffer(), len1);
					// 旧资源释放
					delete hugeBuf;

					// 更新
					hugeBuf = newBuf;
					dwLen = newLen;
				}

				delete pData;
			}
			bHasHistory = TRUE;
		}
		else
		{
			// XEIM DEBUG
			CString strInfo;
			strInfo.Format("无历史历史记录 - %s - %s", m_mapUserInfos[sock].GetUID(), szdate);
			AddErrorInfo(strInfo);
		}
	}
	catch (_com_error e)
	{
		AddErrorInfo((LPCTSTR)e.Description());
	}
	// 释放 ADO 对象
	pRs->Close();
	pRs.Release();

	int sentLen = 50+sizeof(DWORD);
	char *sendBuf = new char[50+sizeof(DWORD)];
	memcpy(sendBuf, (char*)&dwLen, sizeof(DWORD));
	memcpy(sendBuf+sizeof(DWORD), buf, 50);

	XMSG xLength;
	xLength.m_nMessage = XEIMMSG_HISTORYDATA;
	xLength.SetData(sendBuf, sentLen);
	delete sendBuf;

	// 发送离线消息的长度
	SendDataToClient(sock, xLength.GetBuffer(), xLength.GetBufferLength());

	DWORD sentBytes=0;
	while (sentBytes < dwLen)
	{
		DWORD ret = 0;
		if ((dwLen - sentBytes) < 1000)
		{
			ret = SendDataToClient(sock, hugeBuf+sentBytes, dwLen-sentBytes);
			ret = dwLen - sentBytes;
		}
		else
		{
			ret = SendDataToClient(sock, hugeBuf+sentBytes, 1000);
			ret = 1000;
		}
		sentBytes += ret;
	}

	delete hugeBuf;
	hugeBuf = NULL;

	// 恢复 SOCKET 的状态
	ResetSockState(sock, PerIoData);

	// XEIM DEBUG
	if (! bHasHistory)
		return 0;

	CString strInfo;
	strInfo.Format("已向用户[ %s ]发送 %s 的历史消息，共 %d 条 数据长度：%d", m_mapUserInfos[sock].GetUID(), szdate, dwTotal, dwLen);
	AddErrorInfo(strInfo);
	// END XEIM DEBUG

	return 1;
}

// 用户请求获取离线消息
LRESULT CXsvrDlg::XGetMsgOffLine(WPARAM wParam, LPARAM lParam)
{
	DWORD						SendBytes	= 0;
	LPPER_IO_OPERATION_DATA		PerIoData	= (LPPER_IO_OPERATION_DATA)wParam;
	SOCKET						sock		= (SOCKET)lParam;

	int nSize = 10;
	XMSG xLength;
	xLength.m_nMessage = XEIMMSG_GETMSGOFFLINE;
	xLength.SetData((char*)&nSize, sizeof(int));

	SendDataToClient(sock, xLength.GetBuffer(), xLength.GetBufferLength());

	// 恢复 SOCKET 的状态
	ResetSockState(sock, PerIoData);

	// 添加到日志，包括消息数量与用户请求时间
	return 1;
}

// 恢复 SOCKET 的状态
BOOL CXsvrDlg::ResetSockState(SOCKET sock, LPPER_IO_OPERATION_DATA PerIoData)
{
	ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
	PerIoData->DataBuf.len					= DATA_BUFSIZE;
	PerIoData->DataBuf.buf					= PerIoData->Buffer;
	PerIoData->send							= 1;

	DWORD						Flags		= 0;
	DWORD						RecvBytes	= 0;

	if (SOCKET_ERROR
		== WSARecv(sock, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags, &(PerIoData->Overlapped),NULL)
		)
	{
		if (ERROR_IO_PENDING != WSAGetLastError())
		{
			if (WSAGetLastError() != 10058)
			{
				CString strErr;
				strErr.Format("CXsvrDlg::ResetSockState WSARecv 函数错误，代码：%d\n", WSAGetLastError());
				AddErrorInfo(strErr);
				return FALSE;
			}
		}
	}

	return TRUE;
}

DWORD CXsvrDlg::SendDataToClient(SOCKET sock, char *buf, DWORD len)
{
	DWORD SendBytes=0;
	LPPER_IO_OPERATION_DATA pIo
		= (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA));

	ZeroMemory(&(pIo->Overlapped), sizeof(OVERLAPPED));

	memcpy(pIo->Buffer, buf, len);
	pIo->DataBuf.buf = pIo->Buffer;
	pIo->DataBuf.len = len;
	pIo->DataSize = len;

	pIo->send = 2;
	if(SOCKET_ERROR == WSASend(sock, &(pIo->DataBuf), 1, &SendBytes, 0, &(pIo->Overlapped), NULL))
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			CString strErr;
			strErr.Format("CXsvrDlg::XOnGetList WSASend10 failed with error:%d", WSAGetLastError());
			AddErrorInfo(strErr);
			return 0;
		}
	}

	return SendBytes;
}
/*
#include <afxsock.h>		// MFC socket extensions
#import "C:\\WINDOWS\\system32\\msxml.dll" rename("XMLHTTPRequest","MyXMLHTTPRequest")
using namespace MSXML;
*/
//把str编码为网页中的 GB2312 url encode ,英文不变，汉字双字节  如%3D%AE%88
string UrlGB2312(char * str)
{
	string dd;
	size_t len = strlen(str);
	for (size_t i=0;i<len;i++)
	{
		if(isalnum((BYTE)str[i]))
		{
			char tempbuff[2];
			sprintf(tempbuff,"%c",str[i]);
			dd.append(tempbuff);
		}
		else if (isspace((BYTE)str[i]))
		{
			dd.append("+");
		}
		else
		{
			char tempbuff[4];
			sprintf(tempbuff,"%%%X%X",((BYTE*)str)[i] >>4,((BYTE*)str)[i] %16);
			dd.append(tempbuff);
		}
	}
	return dd;
}



void CXsvrDlg::GetUIDMobileNUM(LPCTSTR lpszUID, CString &strNum)
{
	char * szSQL = "SELECT umobile FROM userinfo where uid='%s'";
	CString strSQL;
	strSQL.Format(szSQL, lpszUID);

	// ADO 操作模块
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));
	// 执行相应处理，避免表 为空时的异常

	try
	{
		pRs->Open((LPCTSTR)strSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		if (! pRs->adoEOF)
		{
			// 读取并显示数据库里的信息
			while (!pRs->adoEOF)
			{
				variant_t var;

				// 获取用户昵称
				var = pRs->GetCollect(_variant_t("umobile"));
				if (var.vt != VT_NULL)
				{
					strNum = (LPCTSTR)_bstr_t(var);
				//	AfxMessageBox(strNum);
				}
				pRs->MoveNext();
			}
		}
		else
		{
			strNum = "";
		}
	}
	catch (_com_error e)
	{
		AddErrorInfo((LPCTSTR)e.Description());
	}
	// 释放 ADO 对象
	pRs->Close();
	pRs.Release();
}

// 读取并发送客户端工作
LRESULT CXsvrDlg::_ERP_OnGetSchedule(WPARAM wParam, LPARAM lParam)
{
	// 发件人的套接字
	SOCKET sock = (SOCKET)lParam;
	LPPER_IO_OPERATION_DATA PerIoData = (LPPER_IO_OPERATION_DATA)wParam;

	// 数据库 表[xeim_project]查询语句。
	char * szSQL = "SELECT * FROM xeim_schedule where xeim_number='%s'";
	CString strSQL;

	char *buf = (char*)PerIoData->DataBuf.buf;

	// m_mapUserInfos[sock].GetUID() 请求这套接字关联的 ID。
	strSQL.Format(szSQL, buf);
//	AddErrorInfo(strSQL);

	variant_t var[4];

	int nIdx = 0;

	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));

	try
	{
		pRs->Open((LPCTSTR)strSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		// 如果数据库是空白的
		if (pRs->adoEOF)
		{
			AddErrorInfo("暂无用户记录！");
		//	pRs->Close();
		//	pRs.Release();
		}
		else
		{
			CString strSubject;
			CString strData;
			CString strMembers;
			CString strCreator;

			CString strAll;

			// 读取并显示数据库里的信息
			while (!pRs->adoEOF)
			{
				// 工作的主题
				var[0] = pRs->GetCollect(_variant_t("xeim_subject"));
				if (var[0].vt != VT_NULL)
				{
					strSubject = (LPCTSTR)_bstr_t(var[0]);
				}

				// 工作描述内容
				var[1] = pRs->GetCollect(_variant_t("xeim_data"));
				if (var[1].vt != VT_NULL)
				{
					strData = (LPCTSTR)_bstr_t(var[1]);
				}

				// 工作知会人员
				var[2] = pRs->GetCollect(_variant_t("xeim_members"));
				if (var[2].vt != VT_NULL)
				{
					strMembers = (LPCTSTR)_bstr_t(var[2]);
				}

				// 工作发起者
				var[3] = pRs->GetCollect(_variant_t("xeim_uid"));
				if (var[3].vt != VT_NULL)
				{
					strCreator = (LPCTSTR)_bstr_t(var[3]);
				}

				/*// 任务完成时间
				var[3] = pRs->GetCollect(_variant_t("xeim_posttime"));
				if (var[3].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[3]);
				}*/

				strAll.Format("%s\"%s\"%s\"%s", strSubject, strMembers, strData, strCreator);
			//	AddErrorInfo(strAll);

				XMSG xProject;
				xProject.m_nMessage = XEIMMSG_SCHEDULE;
				xProject.SetData((LPCTSTR)strAll, strAll.GetLength()+1);
				SendDataToClient(sock, xProject.GetBuffer(), xProject.GetBufferLength());
			//	AddErrorInfo("XEIMMSG_SCHEDULE");
				Sleep(20);
				pRs->MoveNext();
			}
		}
	}
	catch (_com_error e)
	{
		AddErrorInfo(e.ErrorMessage());
	}

	pRs->Close();
	pRs.Release();

	// 恢复 SOCKET 的状态
	ResetSockState(sock, PerIoData);
	return 1;
}

// 读取并发送客户端项目
LRESULT CXsvrDlg::_ERP_OnGetProject(WPARAM wParam, LPARAM lParam)
{
	// 发件人的套接字
	SOCKET sock = (SOCKET)lParam;
	LPPER_IO_OPERATION_DATA PerIoData = (LPPER_IO_OPERATION_DATA)wParam;


	// 数据库 表[xeim_project]查询语句。
	char * szSQL = "SELECT * FROM xeim_project where xeim_creator='%s'";
	CString strSQL;

	// m_mapUserInfos[sock].GetUID() 请求这套接字关联的 ID。
	strSQL.Format(szSQL, m_mapUserInfos[sock].GetUID());

	variant_t var[4];

	int nIdx = 0;

	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));

	try
	{
		pRs->Open((LPCTSTR)strSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		// 如果数据库是空白的
		if (pRs->adoEOF)
		{
			AddErrorInfo("暂无用户记录！");
		//	pRs->Close();
		//	pRs.Release();
		}
		else
		{
			CString strCreator;
			CString strProject;
			CString strRemark;
			CString strNumber;

			CString strAll;

			// 读取并显示数据库里的信息
			while (!pRs->adoEOF)
			{
				// 任务发起者
				var[0] = pRs->GetCollect(_variant_t("xeim_creator"));
				if (var[0].vt != VT_NULL)
				{
					strCreator = (LPCTSTR)_bstr_t(var[0]);
				}

				// 项目名称
				var[1] = pRs->GetCollect(_variant_t("xeim_project"));
				if (var[1].vt != VT_NULL)
				{
					strProject = (LPCTSTR)_bstr_t(var[1]);
				}

				// 项目详细描述信息
				var[2] = pRs->GetCollect(_variant_t("xeim_remark"));
				if (var[2].vt != VT_NULL)
				{
					strRemark = (LPCTSTR)_bstr_t(var[2]);
				}

				var[3] = pRs->GetCollect(_variant_t("xeim_number"));
				if (var[3].vt != VT_NULL)
				{
					strNumber = (LPCTSTR)_bstr_t(var[3]);
				}

				/*// 任务完成时间
				var[3] = pRs->GetCollect(_variant_t("xeim_posttime"));
				if (var[3].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[3]);
				}*/
				
				strAll.Format("%s\"%s\"%s\"%s", strNumber, strCreator, strProject, strRemark);
			//	AddErrorInfo(strAll);

				XMSG xProject;
				xProject.m_nMessage = XEIMMSG_PROJECT;
				xProject.SetData((LPCTSTR)strAll, strAll.GetLength()+1);
				SendDataToClient(sock, xProject.GetBuffer(), xProject.GetBufferLength());
				Sleep(20);
				pRs->MoveNext();
			}
		}
	}
	catch (_com_error e)
	{
		AddErrorInfo(e.ErrorMessage());
	}

	pRs->Close();
	pRs.Release();

	// 恢复 SOCKET 的状态
	ResetSockState(sock, PerIoData);
	return 1;
}

// 记录项目名称
LRESULT CXsvrDlg::_ERP_OnRecordProject(WPARAM wParam, LPARAM lParam)
{
	// 发件人的套接字
	SOCKET sock = (SOCKET)lParam;
	LPPER_IO_OPERATION_DATA PerIoData = (LPPER_IO_OPERATION_DATA)wParam;

	char *buf = (char*)PerIoData->DataBuf.buf;
	CString strNewData = buf;
//	AddErrorInfo(strNewData);

	// 判断当前日期
	CTime tmToday = CTime::GetCurrentTime();
	DWORD dwMonth = tmToday.GetMonth();

	// 项目编号计算
	if (dwMonth != m_dw_ERP_MONTH)
	{
		m_dw_ERP_MONTH = dwMonth;
		m_dw_ERP_CURRENT_NUMBER = 1;
	}
	else
	{
		m_dw_ERP_CURRENT_NUMBER ++;
	}

	// 项目编号
	CString strNumber;
	strNumber.Format("%s%03d", tmToday.Format("%y%m"), m_dw_ERP_CURRENT_NUMBER);

	// 项目发起者
	CString strCreator;
	int nIdx = strNewData.Find('\"');
	strCreator = strNewData.Left(nIdx);

	CString strProject = strNewData.Right(strNewData.GetLength()-nIdx-1);
	nIdx = strProject.Find('\"');
	strNewData = strProject;
	strProject = strNewData.Left(nIdx);

	CString strRemark = strNewData.Right(strNewData.GetLength()-nIdx-1);

//	CString strSendUID = lpszSendUID;
//	CString strRecvUID = lpszRecvUID;
//	CString strMSG = lpszMsgBody;
//	strMSG.Replace("'", "''");

	char *sql = "INSERT INTO xeim_project("
		"xeim_number,"
		"xeim_creator,"
		"xeim_project,"
		"xeim_remark,"
		"xeim_time_created) "
		"VALUES('%s','%s','%s','%s','%s')";

	CString strSQL;
	strSQL.Format(sql, strNumber, strCreator,
		strProject,
		strRemark,
		tmToday.Format(_T("%Y-%m-%d %H:%M:%S")));

//	AddErrorInfo(strRemark);

	try
	{
		_variant_t vAffected;
		m_pConn->CursorLocation = adUseClient;
		m_pConn->Execute((LPCTSTR)strSQL, &vAffected, adExecuteNoRecords);
	}
	catch (_com_error e)
	{
		AddErrorInfo((LPCTSTR)_bstr_t(e.Description()));
		return 0;
	}

	// 恢复 SOCKET 的状态
	ResetSockState(sock, PerIoData);

	CString strAll;
	strAll.Format("%s\"%s\"%s\"%s", strNumber, strCreator, strProject, strRemark);

	XMSG xProject;
	xProject.m_nMessage = XEIMMSG_PROJECT;
	xProject.SetData((LPCTSTR)strAll, strAll.GetLength()+1);
	SendDataToClient(sock, xProject.GetBuffer(), xProject.GetBufferLength());

	// 在 xserver 里分配了内存，这里释放。
	delete [] buf;
	return 1;
}

// 记录短信
LRESULT CXsvrDlg::XOnRecordSMS(WPARAM wParam, LPARAM lParam)
{
	// 发件人的套接字
	SOCKET sock = (SOCKET)lParam;
	LPPER_IO_OPERATION_DATA PerIoData = (LPPER_IO_OPERATION_DATA)wParam;

//	DWORD SendBytes;
	XMSG xmsg;
	char *buf = (char*)PerIoData->DataBuf.buf;

	XMSG_ONLINE omsg(buf);
//	AddErrorInfo(omsg.GetData());


	XDBMsgSMS(m_mapUserInfos[sock].GetUID(), omsg.GetUID(), omsg.GetData());
//	XDBMsgOnLine(m_mapUserInfos[sock].GetUID(), imap->second.GetUID(), msgHistory.GetData());


	// 恢复 SOCKET 的状态
	ResetSockState(sock, PerIoData);

//	delete buf;
	return 1;
}


void CXsvrDlg::OnMenuTask() 
{
	// TODO: Add your command handler code here
/*	if (! m_dlgTask.IsWindowVisible())
	{
		m_dlgTask.ShowWindow(SW_SHOW);
	}*/
}

void CXsvrDlg::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	GetCursorPos(&pt);
	m_tcUserTree.ScreenToClient(&pt);
	HTREEITEM hItem = m_tcUserTree.HitTest(pt);

	if (hItem == m_hRoot)
	{
	}
	else
	{
		DWORD dwItem = m_tcUserTree.GetItemData(hItem);

		char szUID[256];
		m_lcUserList.GetItemText(dwItem, 1, szUID, 256);

		list<XUserDetail>::iterator iUserDetail =
					find(m_listUserDetail.begin(), m_listUserDetail.end(), strlwr(szUID));

		// 打开修改对话框
		CXUserEditorDlg dlg;
		dlg.SetData((*iUserDetail));
		CString strMobile;
		GetUIDMobileNUM((*iUserDetail).GetUID(), strMobile);
		dlg.SetMobile(strMobile);
		if (IDOK == dlg.DoModal())
		{
			m_lcUserList.SetItemText(dwItem, 0, dlg.m_strNickName);
			m_lcUserList.SetItemText(dwItem, 2, dlg.m_strPassword);
			m_lcUserList.SetItemText(dwItem, 4, dlg.m_strGroup);

			m_tcUserTree.SetItemText(hItem, dlg.m_strNickName);

			// 更新内存中的用户列表
			list<XUserDetail>::iterator iUserDetail =
				find(m_listUserDetail.begin(), m_listUserDetail.end(), strlwr(szUID));

			iUserDetail->SetUNickName(dlg.m_strNickName);
			iUserDetail->SetUPassword(dlg.m_strPassword);
			iUserDetail->SetUGroup(dlg.m_strGroup);
		}
	}
	
	*pResult = 0;
}

void CXsvrDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}




// 发送用户状态
void CXsvrDlg::XUser_SendStatus()
{
}

// 发送用户联系人
void CXsvrDlg::XUser_SendContact()
{
	SOCKET sock = m_listContactQuery.back();
	string strUID = m_mapUserInfos[sock].GetUID();

	m_listContactQuery.pop_back();

	// 如果所有请求都处理完了，则释放系统资源
	if (m_listContactQuery.size() == 0)
	{
	//	AddErrorInfo("资源已释放");
		KillTimer(TIMER_SEND_CONTACT);
		m_bSendUserContact = FALSE;
	}

	list<string> list_contact;

	// SQL 查询
	char * szSQL = "SELECT xucontact FROM x_contact where uid='%s'";

	CString strSQL;
	strSQL.Format(szSQL, strUID.c_str());

	// ADO 操作模块
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));
		// 执行相应处理，避免表 为空时的异常
	try
	{
		pRs->Open((LPCTSTR)strSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		// 如果数据库是空白的
		if (! pRs->adoEOF)
		{
			// 读取并显示数据库里的信息
			list<XUserDetail>::iterator iDetail;
			while (!pRs->adoEOF)
			{
				variant_t var;

				// 获取用户昵称
				var = pRs->GetCollect(_variant_t("xucontact"));
				if (var.vt != VT_NULL)
				{
					iDetail = find(m_listUserDetail.begin(), m_listUserDetail.end(), strlwr((LPSTR)(LPCTSTR)_bstr_t(var)));	
					if (iDetail != m_listUserDetail.end())
					{
						list_contact.push_back((LPCTSTR)_bstr_t(var));
					}
				}
				pRs->MoveNext();
			}
		}
		else
		{
			AddErrorInfo("该联系人数据表无任何记录！");
		}
	}
	catch (_com_error e)
	{
		AddErrorInfo((LPCTSTR)e.Description());
		return;
	}

	// 释放 ADO 对象
	pRs->Close();
	pRs.Release();

	list_contact.sort();

	list<XUserDetail>::iterator iDetail;
	list<string>::iterator iContact;

	XMSG xmsg;
	XUserInfo xui;

	// 此用户所具有的联系人个数
	int nSize = list_contact.size();

	// 如果用户过多，数据包会过大，所以要分包发送。
	// 这里是分包算法，每20个用户打成一个包
	int nUsers	= 20;
	int nSndTime = nSize / nUsers;
	int nRemind = nSize % nUsers; // 取模运算，别忘记了

	// 把用户信息打包发送
	iContact = list_contact.begin();

	XMSG xLength;
	xLength.m_nMessage = XEIMMSG_USERLIST;
	xLength.SetData((char*)&nSize, sizeof(int));
	SendDataToClient(sock, xLength.GetBuffer(), xLength.GetBufferLength());

	for (int j=0; j<nSndTime; j++)
	{
		char *buf = new char[sizeof(XUserInfo)*nUsers];

		ZeroMemory(buf, sizeof(XUserInfo)*nUsers);
		int nIdx = 0;
		for (int i=0; i<nUsers; i++)
		{
			iDetail = find(m_listUserDetail.begin(), m_listUserDetail.end(), strlwr((char*)(iContact->c_str())));
			iDetail->GetUserInfo(xui);
			memcpy(buf+nIdx, (char*)&xui, sizeof(XUserInfo));
			nIdx += sizeof(XUserInfo);
			iContact++;
		}

		SendDataToClient(sock, buf, sizeof(XUserInfo)*nUsers);

		delete buf;
	}

	if (nRemind)
	{
		// 数据块大小
		int nCount = sizeof(XUserInfo) * nRemind;// + sizeof(DWORD);
		// 用户个数及用户信息块
		char *buf = new char[nCount];
		ZeroMemory(buf, nCount);
		int nIdx = 0;

		for (int i=0; i<nRemind; i++)
		{
			iDetail = find(m_listUserDetail.begin(), m_listUserDetail.end(), iContact->c_str());
			iDetail->GetUserInfo(xui);
			memcpy(buf+nIdx, (char*)&xui, sizeof(XUserInfo));
			nIdx += sizeof(XUserInfo);
			iContact++;
		}

		SendDataToClient(sock, buf, nCount);

		delete buf;
	}

	// XEIM DEBUG
/*	CString taab;
	taab.Format("发送用户[ %s ]的联系人信息，共 %d 个联系人. ",
		m_mapUserInfos[sock].GetUID(), list_contact.size());

	AddErrorInfo(taab);*/
	// END XEIM DEBUG

	list_contact.clear();
}

void CXsvrDlg::_ERP_CheckDatabase()
{
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));

	if (!pRs || !m_pConn) 
	{
		return;
	}

	// 数据库 表[xeim_interface]查询语句。
	char * szSQL = "SELECT * FROM xeim_interface where xeim_checked='n'";

	variant_t var[6];
	CString str;
	int nIdx = 0;

	try
	{
		pRs->Open(szSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		// 如果数据库是空白的
		if (pRs->adoEOF)
		{
		//	AddErrorInfo("暂无用户记录！");
		}
		else
		{
			// 读取并显示数据库里的信息
			while (!pRs->adoEOF)
			{
				CString strMessage;
				// 需发送的消息类型
				int nMessageType = -1;
				enum
				{
					MESSAGE_INSTANT=1,
						MESSAGE_TIMING=2
				};

				// 任务是否已处理
			/*	var[4] = pRs->GetCollect(_variant_t("xeim_checked"));
				if (var[4].vt != VT_NULL){str = (LPCTSTR)_bstr_t(var[4]);*/

				// 任务发起者
			/*	var[0] = pRs->GetCollect(_variant_t("xeim_sender"));
				if (var[0].vt != VT_NULL){str = (LPCTSTR)_bstr_t(var[0]);}*/

				// 任务需处理内容
				var[2] = pRs->GetCollect(_variant_t("xeim_data"));
				if (var[2].vt != VT_NULL)
				{
					strMessage = (LPCTSTR)_bstr_t(var[2]);
				}

				// 任务类型
				var[5] = pRs->GetCollect(_variant_t("xeim_type"));
				if (var[5].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[5]);
				}

				// 如果是即时消息
				char *pTmp = strupr((LPSTR)(LPCTSTR)str);
				if (! strcmp(pTmp, "XINSTANT") )
					nMessageType = MESSAGE_INSTANT;
				// 如果是定时消息
				else if (! strcmp(pTmp, "XTIMING") )
					nMessageType = MESSAGE_TIMING;
				else
					nMessageType = MESSAGE_INSTANT;

				// 任务完成时间
				var[3] = pRs->GetCollect(_variant_t("xeim_posttime"));
				if (var[3].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[3]);
					if (nMessageType == MESSAGE_TIMING)
					{
					//	AddErrorInfo(str);
						CTime timeCurrent = CTime::GetCurrentTime();

						CTime ctmSend;
						_ERP_StringToCTime(str, ctmSend);

						if (timeCurrent >= ctmSend)
						{
							// 确定时间已经到了，下一步就可以发送提醒消息了。
							nMessageType = MESSAGE_INSTANT;
						//	AddErrorInfo(strTemp);
						}
						else // 消息通知时间还没到，什么也不做。
						{
						//	strTemp.Format("%d, %d", nMinute, timeCurrent.GetMinute());
						//	AddErrorInfo(strTemp);
						}
					}
				}

				// 如果消息是即时消息，或者定时消息是到时了，才执行这个动作。
				if (nMessageType == MESSAGE_INSTANT)
				{
					// 任务处理人员
					var[1] = pRs->GetCollect(_variant_t("xeim_receiver"));
					if (var[1].vt != VT_NULL)
					{
						str = (LPCTSTR)_bstr_t(var[1]);
						_ERP_SendMessageToUserList(str, strMessage);
					/*	
						str += ",";
						//////////////////////////////////////////////////////////////////////////
						CString strTmp;
						nIdx = str.Find(',');
						while (-1 != (nIdx=str.Find(',')))
						{
							strTmp = str.Left(nIdx);
							strTmp.TrimLeft();
							strTmp.TrimRight();
							if (! strTmp.IsEmpty())
							{
								char * szUID = strlwr((LPSTR)(LPCTSTR)strTmp);
							//	AddErrorInfo(szUID);
								if (! XSendMsgToUser(szUID, (LPCTSTR)strMessage))
								{
									// 如果用户不在线或者发送不成功，就把消息保存在数据库，下次登录再发
									XDBMsgOffLine("xx--11--", szUID, (LPCTSTR)strMessage);
								}
							}

							int len = str.GetLength();
							strTmp = str.Right(len - nIdx -1);
							str = strTmp;
							nIdx = str.Find(',');

						} // while (-1 != (nIdx=str.Find(',')))*/

					} // if (var[1].vt != VT_NULL)

					// 对消息进行标识，让数据库下次不再查询这条记录。
					pRs->PutCollect(_variant_t("xeim_checked"), "y");
					pRs->Update();

				} // if (nMessageType == MESSAGE_INSTANT)

				pRs->MoveNext();
			}
		}
	}
	catch (_com_error e)
	{
		AddErrorInfo(e.ErrorMessage());
	}

	pRs->Close();
	pRs.Release();
}

// 1. 初始化项目管理的项目编号
// 2. 获取当前的月份
void CXsvrDlg::_ERP_INIT()
{
	CTime tmMonth = CTime::GetCurrentTime();
	m_dw_ERP_MONTH = tmMonth.GetMonth();
	m_dw_ERP_CURRENT_NUMBER = 1;

	// 从数据库获取最后的项目记录，以免程序重启后，项目重复
	char *szSQL = "SELECT xeim_number FROM xeim_project WHERE [xeim_index] = (select MAX([xeim_index]) FROM xeim_project)";

	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));
	variant_t var;

	try
	{
		pRs->Open(szSQL,
			m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		// 如果数据库是空白的
		if (pRs->adoEOF)
		{
		//	AddErrorInfo("暂无用户记录！");
		}
		else // 读取并显示数据库里的信息
		{
			// 读取最大的项目编号
			var = pRs->GetCollect(_variant_t("xeim_number"));
			if (var.vt != VT_NULL)
			{
				CString str = (LPCTSTR)_bstr_t(var);
			//	AddErrorInfo(str.Right(3));
				m_dw_ERP_CURRENT_NUMBER = atoi(str.Right(3));
			}
		}
	}
	catch (_com_error e)
	{
		AddErrorInfo(e.ErrorMessage());
	}

	pRs->Close();
	pRs.Release();
}

// 1. 把时间字符串转换成 CTime 格式时间。
// return void
void CXsvrDlg::_ERP_StringToCTime(IN CString str, OUT CTime &ctm)
{
	char *szPtr = (LPSTR)(LPCTSTR)str;

	// 日期
	int nYear;
	int nMonth;
	int nDay;
	// 时间
	int nHour;
	int nMin;
	int nSec;

	nYear = atoi(szPtr);
	nMonth = atoi(szPtr+5);
	if (szPtr[5+1] == '-')
	{
		nDay = atoi(szPtr+7);
		if (szPtr[7+1] == ' ')
			szPtr = szPtr + 9;
		else
			szPtr = szPtr + 10;
	}
	else
	{
		nDay = atoi(szPtr+8);
		if (szPtr[8+1] == ' ')
			szPtr = szPtr + 10;
		else
			szPtr = szPtr + 11;
	}

	nHour = atoi(szPtr);
	nMin = atoi(szPtr+3);
	nSec = atoi(szPtr+6);

	// CTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,
	// int nDST = -1);
	ctm = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
}

// 1. 把消息[strMessage]发给指定的所有用户[strUsers]（可能是一个，可能是多个）。
// 2. 如果用户在线的话，马上发送，如果用户离线，先记录入到数据库，等用户在线再发。
// return void
// 备注：strUser 这里用的是用户姓名，因为 ERP 系统的特殊需求。
void CXsvrDlg::_ERP_SendMessageToUserList(CString strUsers, CString strMessage)
{
	int nIdx;
	CString str = strUsers;
	str += ",";
	//////////////////////////////////////////////////////////////////////////
	CString strTmp;
	nIdx = str.Find(',');
	while (-1 != (nIdx=str.Find(',')))
	{
		strTmp = str.Left(nIdx);
		strTmp.TrimLeft();
		strTmp.TrimRight();
		if (! strTmp.IsEmpty())
		{
			LPCTSTR szName = (LPCTSTR)strTmp;

			if (! _ERP_SendMessageToUserByName(szName, (LPCTSTR)strMessage))
			{
				LPCTSTR pUID = _ERP_GetUIDByName(szName);

				// 如果用户不在线或者发送不成功，就把消息保存在数据库，下次登录再发
				if (pUID != NULL)
				{
					AddErrorInfo(pUID);
					XDBMsgOffLine("xx--11--", pUID, (LPCTSTR)strMessage);
				}
			}
		}

		int len = str.GetLength();
		strTmp = str.Right(len - nIdx -1);
		str = strTmp;
		nIdx = str.Find(',');

	} // while (-1 != (nIdx=str.Find(',')))
}

// 1. 根据所给的用户姓名[szName]来发送消息[szMessage]给该用户。
// return 成功返回[TRUE]，失败返回[FALSE]。
BOOL CXsvrDlg::_ERP_SendMessageToUserByName(LPCTSTR szName, LPCTSTR szMessage)
{
	XMSG xmsg;
	DWORD SendBytes;
	XMSG_ONLINE omsg;
	omsg.SetData("xx--11--", szMessage, strlen(szMessage)+1);

	map<SOCKET, XUserInfo>::iterator imap;

	for (imap=m_mapUserInfos.begin();
	imap!=m_mapUserInfos.end(); ++imap)
	{
		if (! _stricmp(imap->second.GetUNickName(), szName))
		{
			xmsg.m_nMessage = XEIMMSG_NEWMESSAGE;
			xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());
			LPPER_IO_OPERATION_DATA pIo = (LPPER_IO_OPERATION_DATA )GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA));
			ZeroMemory(&(pIo->Overlapped), sizeof(OVERLAPPED));
			memcpy(pIo->Buffer, xmsg.GetBuffer(), xmsg.GetBufferLength());
			pIo->DataBuf.buf = pIo->Buffer;
			pIo->DataBuf.len = xmsg.GetBufferLength();
			pIo->DataSize = xmsg.GetBufferLength();
			pIo->send = 1;
			if(SOCKET_ERROR == WSASend(imap->first, &(pIo->DataBuf), 1, &SendBytes, 0, &(pIo->Overlapped), NULL))
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					AddErrorInfo("WSASend error3\n");
					return FALSE;
				}
			}
			break;
		}
	}
	// 用户不在线
	if (imap == m_mapUserInfos.end())
	{
		return FALSE;
	}

	return TRUE;
}

// 1. 根据给定的用户姓名[lpszName]来获取用户 UID。
// return 如果在用户列表里找到了该用户姓名，则返回该用户 UID，否则返回 NULL。
LPCTSTR CXsvrDlg::_ERP_GetUIDByName(LPCTSTR lpszName)
{
	list<XUserDetail>::iterator iUser;
	for (iUser=m_listUserDetail.begin(); iUser!=m_listUserDetail.end(); iUser++)
	{
		// 找到了该用户姓名。
		if (! _stricmp((*iUser).GetUNickName(), lpszName))
		{
			return (*iUser).GetUID();
		}
	}

	return NULL;
}

// 获取程序版本信息
// return FALSE if failed.
BOOL CXsvrDlg::_UI_SetProductInfo()
{
	CHAR szAppPath[MAX_PATH] ={0};
	::GetModuleFileName(NULL, szAppPath, MAX_PATH);

	INT nVersionLen = ::GetFileVersionInfoSize(szAppPath,NULL);
	if (nVersionLen <= 0)
	{
		return FALSE;
	}

	CHAR* pBuffer = new CHAR[nVersionLen];
	if (!pBuffer)  
	{
		return FALSE;
	}

	if ( !::GetFileVersionInfo(szAppPath, NULL, nVersionLen, pBuffer) )
	{
		return FALSE;
	}

	UINT dwBytes;
	CHAR* lpBuffer;
	CHAR szSubBlock[256];

	CString strProductName;
	CString strVersion;
	CString strCopyright;

	// 为了方便访问。
	struct LANGANDCODEPAGE {
	WORD wLanguage;
	WORD wCodePage;
	} *lpTranslate;
	// Read the list of languages and code pages.
	UINT   cbTranslate;   
	::VerQueryValue(pBuffer, _T("\\VarFileInfo\\Translation"),
	(LPVOID*)&lpTranslate, &cbTranslate);

	// 产品版本
	_snprintf( szSubBlock, 256, _T("\\StringFileInfo\\%04x%04x\\ProductVersion"),
		lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);
	if(::VerQueryValue(pBuffer, szSubBlock, (LPVOID*)&lpBuffer, &dwBytes))
	{
		strVersion = lpBuffer;
	}

	// 产品名称
	_snprintf(szSubBlock, 256, _T("\\StringFileInfo\\%04x%04x\\ProductName"),
		lpTranslate[0].wLanguage, lpTranslate[0].wCodePage );   
	if(::VerQueryValue(pBuffer, szSubBlock, (LPVOID*)&lpBuffer, &dwBytes))
	{
		strProductName = lpBuffer;
	}

	// 产品版权
	_snprintf(szSubBlock, 256, _T("\\StringFileInfo\\%04x%04x\\LegalCopyright"),
		lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);
	if(::VerQueryValue(pBuffer, szSubBlock, (LPVOID*)&lpBuffer, &dwBytes))
	{
		strCopyright = lpBuffer;
	}

	CString strSoftInfo;
	strSoftInfo.Format(_T("%s v%s %s"), strProductName, strVersion, strCopyright);

	SetDlgItemText(IDC_STATIC8, strSoftInfo);
	delete [] pBuffer;
	return TRUE;
}

void CXsvrDlg::OnMsgSystem()
{
	// TODO: 在此添加命令处理程序代码
	CSysMsg sysDlg;
	string sText;
	if ( IDOK != sysDlg.DoModal())
	{
		return;
	}

	sText = sysDlg.sMsg;

	//在线用户总数
	int nOnlineCount = m_mapUserInfos.size();
	if ( nOnlineCount <= 0 )
	{
		return;
	}

	map<SOCKET, XUserInfo>::iterator it = m_mapUserInfos.begin();
	for ( ; it != m_mapUserInfos.end(); it++)
	{
		SOCKET sock = it->first;
		XUserInfo info = it->second;
		//发送系统消息
		XMSG xmsg;
		DWORD SendBytes;
		XMSG_ONLINE omsg;
//		const char* szMessage = "系统消息\r\n";
		omsg.SetData("xx--11--", sText.c_str(), strlen(sText.c_str())+1);

		{
			xmsg.m_nMessage = XEIMMSG_NEWMESSAGE;
			xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());
			LPPER_IO_OPERATION_DATA pIo = (LPPER_IO_OPERATION_DATA )GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA));
			ZeroMemory(&(pIo->Overlapped), sizeof(OVERLAPPED));
			memcpy(pIo->Buffer, xmsg.GetBuffer(), xmsg.GetBufferLength());
			pIo->DataBuf.buf = pIo->Buffer;
			pIo->DataBuf.len = xmsg.GetBufferLength();
			pIo->DataSize = xmsg.GetBufferLength();
			pIo->send = 1;
			if(SOCKET_ERROR == WSASend(sock, &(pIo->DataBuf), 1, &SendBytes, 0, &(pIo->Overlapped), NULL))
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					AddErrorInfo("WSASend error3\n");
					return;
				}
			}
		}
	}
}
