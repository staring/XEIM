/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// client02Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "client02.h"
#include "client02Dlg.h"
#include "../xmsg/xtype.h"
#include "../xmsg/xdef.h"
#include "xfun.h"
#include "XLoginDlg.h"
#include "process.h"
#include "SetDlg.h"
#include "ini.h"
#include "NormalSMS.h"
#include "DLG_ERP_PROJECT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		XWM_USEROK			WM_USER + 0x68
#define		XWM_USERBAD			WM_USER + 0x69
#define		XWM_DISCONNECTED	WM_USER + 0x70
#define		XWM_USERINFO		WM_USER + 0x71
#define		XWM_USERLIST		WM_USER + 0x72
// 用户列表获取完毕
#define		XWM_USERLISTEND		WM_USER + 0x73
#define		XWM_USERSTATUS		WM_USER + 0x74
#define		XWM_MESSAGE			WM_USER + 0x75
#define		XWM_FILE			WM_USER + 0x76

// 用来通知主窗口，网络线程收到好友数量
#define		XWM_TOTAL_CONTACT	WM_USER + 0x77
// 用户收到服务器发来的离线消息
#define		XWM_MSGOFFLINE		WM_USER + 0x78
// 服务器发来了历史记录
#define		XWM_MSGHISTORY		WM_USER + 0x79
// 没有当天的消息
#define		XWM_MSGNOHISTORY	WM_USER + 0x80


//////////////////////////////////////////////////////////////////////////
// 定时器，用来等待一段时间后获取用户离线消息
#define		TIMER_GET_OFFLINEMSG		55
// 1*5 秒 // 隔多长时间获取离线消息
#define		TIMER_GET_OFFLINEMSG_TIME	1000*5

// 在登录成功后检测服务器是否有新版本客户端
#define		TIMER_CHECK_VERSION			66
// 在登录有多长时间发送版本检测
#define		TIMER_CHECK_VERSION_TIME	1000


// 短线后重新登陆
#define		TIMER_CONNECT				88
#define		TIMER_CONNECT_TIME			10000

// 定时显示内容
#define		TIMER_SHOW_INFO				99
#define		TIMER_SHOW_INFO_TIME		500
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

//XEIM DEBUG
//int kkkkk = 0;

DLG_ERP_PROJECT* CClient02Dlg::m_pDlg_ERP_Project = NULL;
DLG_ERP_SCHEDULE* CClient02Dlg::m_pDlg_ERP_Schedule = NULL;
DLG_ERP_WORK* CClient02Dlg::m_pDlg_ERP_Work = NULL;

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
public:
	
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
	ON_COMMAND(IDM_HELP, &CAboutDlg::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClient02Dlg dialog

CClient02Dlg::CClient02Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClient02Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClient02Dlg)
	m_strDebug = _T("");
	m_strCellNUM = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dwOnLine = 0;
	m_bLogedin = FALSE;
	m_bNetWorkInit = FALSE;

	m_pGroupMsg = NULL;
	m_erpDlg = NULL;
	m_pPluginBar = NULL;
}

CClient02Dlg::~CClient02Dlg(void)
{
	int nCount = m_mapUserList.size();
	map<HTREEITEM, CXChatDlg*>::iterator userListIter;
	for (userListIter = m_mapUserList.begin(); userListIter != m_mapUserList.end(); userListIter++)
	{
		CXChatDlg* pDlg = userListIter->second;
		if (pDlg)
		{
			delete pDlg;
			pDlg = NULL;
		}

	}
	m_mapUserList.clear();

	if (m_erpDlg)
	{
		delete m_erpDlg;
		m_erpDlg = NULL;
	}
}

void CClient02Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClient02Dlg)
	DDX_Control(pDX, IDC_USERLIST, m_xtreeUser);
	DDX_Control(pDX, IDC_SETTINGS, m_staticSettings);
	DDX_Control(pDX, IDC_STATIC_MENU, m_staticMenu);
	DDX_Control(pDX, IDC_STATIC18, m_staticSendSMS);
	DDX_Text(pDX, IDC_EDIT1, m_strDebug);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClient02Dlg, CDialog)
	//{{AFX_MSG_MAP(CClient02Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(XWM_USEROK, XOnUserOK)
	ON_MESSAGE(XWM_USERBAD, XOnUserBad)
	ON_MESSAGE(XWM_USERINFO, XOnUserInfo)
	ON_MESSAGE(XWM_DISCONNECTED, XOnDisconnected)
	ON_MESSAGE(XWM_USERLIST, XOnUserList)
	ON_MESSAGE(XWM_USERLISTEND, XOnListEnd)
	ON_MESSAGE(XWM_USERSTATUS, XOnUserStatus)
	ON_MESSAGE(XWM_MSGOFFLINE, XMsgRecvOffLine)
	ON_MESSAGE(XWM_MESSAGE, XOnMessage)
	ON_MESSAGE(XWM_TOTAL_CONTACT, XOnTotalContact)
	ON_MESSAGE(XWM_FILE, XOnFile)
	ON_MESSAGE(XWM_MSGHISTORY, XMsgHistory)
	ON_MESSAGE(XWM_MSGNOHISTORY, XMsgNoHistory)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_USERLIST, OnDblclkUserlist)
	ON_COMMAND(ID_TRAYRESTORE, OnTrayRestore)
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_USERLIST, OnRclickUserlist)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_COMMAND(ID_TRAYQUIT, OnCancel)
	ON_MESSAGE(_HYPERLINK_EVENT,OnChildFire)
	ON_COMMAND(ID_MENUITEM_LOGOUT, OnMenuitemLogout)
	//}}AFX_MSG_MAP
	ON_COMMAND(IDM_QUIT, &CClient02Dlg::OnQuit)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(IDM_HELP, &CClient02Dlg::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClient02Dlg message handlers

BOOL CClient02Dlg::OnInitDialog()
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

	// TODO: Add extra initialization here

	// 读取配置文件
	CheckForUpdate();
//	return FALSE;
	// 建立文件名
	char buf[MAX_PATH];
	GetProgramDirectory(buf);
	strcat_s(buf,MAX_PATH*sizeof(char), "setting.ini");

	// 检查用户设置文件
	char szFileName[_MAX_PATH];
	GetProgramDirectory(szFileName);
	strcat_s(szFileName, MAX_PATH*sizeof(char), "info.ini");

	CIniReader iniReader(szFileName);
	if (iniReader.sectionExists("xeim_server_info"))
	{
		m_strSvr = iniReader.getKeyValue("server_addr", "xeim_server_info");
		m_strCellNUM = iniReader.getKeyValue("cell_number", "xeim_user_info");
	}
	else
	{
		iniReader.setKey("", "server_addr", "xeim_server_info");
		iniReader.setKey("", "cell_number", "xeim_user_info");
	}

	if (m_strSvr.IsEmpty())
	{
		CSetDlg dlg;
		if (IDOK == dlg.DoModal())
		{
		//	m_inifile.WriteString(m_strSvr);
			iniReader.setKey(m_strSvr, "server_addr", "xeim_server_info");
		}
		else
		{
			AfxMessageBox("找不到服务器，无法启动！");
			OnCancel();
			return TRUE;
		}
	}

	// 启动登录对话框
	CXLoginDlg dlg(GetDesktopWindow());
	if (IDOK == dlg.DoModal())
	{
	}
	else
	{
		OnCancel();
		return TRUE;
	}

	GetDlgItem(IDC_STATIC_INFO)->ShowWindow(SW_HIDE);
	// 设置背景颜色
	m_brBk2.CreateSolidBrush(RGB(255, 255, 255));
	// 文本框背景颜色
	m_editBKColor = RGB(255,255,255); // white text
	// 静态文本颜色
	m_textcolor = RGB(55,55,56);

	// 设置在线用户列表------------------------------------------------------------------
	CBitmap bm;
	m_ilUser.Create (16, 16, ILC_COLOR32 , 5,5);
	bm.LoadBitmap(IDB_BITMAP1);
//	m_ilUser.Add(& bm, RGB(255,0,255));
	m_ilUser.Add(& bm, RGB(25, 79, 149));
	m_xtreeUser.SetImageList(& m_ilUser, TVSIL_NORMAL);
//	m_hRoot = m_xtreeUser.InsertItem("联系人(0/0)", 1, 1);

	m_staticSettings.SetFireChild(TRUE);
	m_staticSendSMS.SetFireChild(TRUE);
	m_staticMenu.SetFireChild(TRUE);

	// XEIM USER
//	GetDlgItem(IDC_STATIC18)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
	CRect rtWnd;
	GetWindowRect(&rtWnd);
//	ScreenToClient(&rtWnd);
	MoveWindow(rtWnd.left, rtWnd.top, rtWnd.Width(), rtWnd.Height()-105);

//	CRect rtStatic;
//	GetDlgItem(IDC_STATIC28)->GetWindowRect(&rtStatic);
//	ScreenToClient(&rtStatic);
//	GetDlgItem(IDC_STATIC28)->MoveWindow(rtStatic.left, rtStatic.top-105, rtStatic.Width(), rtStatic.Height());
	// XEIM DEBUG
	m_strDebug += "调试信息：";
	UpdateData(FALSE);

	// 系统托盘图标创建------------------------------------------------------------------
	char szTitle[512] = {0};
//	LoadString(NULL, IDS_STRING_TITLE, szTitle, 512);
	sprintf_s(szTitle, 512, "XEIM -- %s", m_strUID.GetBuffer());
	m_strUID.ReleaseBuffer();

	if (!m_sysTray.Create(this, WM_ICON_NOTIFY, szTitle, m_hIcon, IDR_SYSTEMTRAY))
	{
		MessageBox("System tray creating error.");
		return FALSE;
	}

	// 创建插件栏
	m_pPluginBar = new CXPluginBar;
	m_pPluginBar->Create(CXPluginBar::IDD, this);
	if (::IsWindow(m_pPluginBar->GetSafeHwnd()))
	{
		CRect newPos;
		GetDlgItem(IDC_STATIC_MENU)->GetWindowRect(& newPos);
		ScreenToClient(& newPos);
		long nDist = (newPos.bottom - newPos.top);
		newPos.top += nDist;
		newPos.bottom += nDist;
		m_pPluginBar->MoveWindow(& newPos);
		m_pPluginBar->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_MENU)->ShowWindow(SW_SHOW);
	}

	//窗口标题
	SetWindowText("客户端");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClient02Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClient02Dlg::OnPaint() 
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
HCURSOR CClient02Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CClient02Dlg::Init()
{
	if (! m_bNetWorkInit)
	{
		sockaddr_in dA;

		int slen = sizeof(sockaddr_in);
		memset(&dA,0,sizeof(dA));

		dA.sin_family = AF_INET;
		unsigned long inaddr;
	//	inaddr = inet_addr("116.30.134.23");
	//	inaddr = inet_addr("127.0.0.1");
	//	AfxMessageBox(m_strSvr);
		inaddr = inet_addr(m_strSvr);
		if (inaddr == INADDR_NONE)
		{
			hostent *host=NULL;
			host = gethostbyname(m_strSvr);
			if (host == NULL)
			{
			//	AfxMessageBox("gethostbyname");
				m_strLastError = "找不到服务器，请确保服务器地址正确。";
				return FALSE;
			}
			in_addr in;
			memcpy(&in, host->h_addr_list[0], host->h_length);
			char k[128];
			strcpy_s(k,128*sizeof(char), inet_ntoa(in));
			inaddr = inet_addr(k);
		}

		if (inaddr != INADDR_NONE)
		{
			memcpy(&dA.sin_addr,&inaddr,sizeof(inaddr));
		}
		else
		{
			AfxMessageBox("inaddr != INADDR_NONE");
			return FALSE;
		}
		dA.sin_port = htons(8188);

		m_sock = socket(AF_INET,SOCK_STREAM,0);
		if (INVALID_SOCKET == m_sock)
		{
			m_strLastError = "socket 创建套接字失败。";
			return FALSE;
		}

		if (connect(m_sock,(sockaddr*)&dA,slen) < 0)
		{
			m_strLastError = "连接服务器失败，请确保服务器已启动 XEIM 服务端。";
			return FALSE;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//KeepAlive实现  短线检测
		TCP_KEEPALIVE2 inKeepAlive = {0}; //输入参数 
		unsigned long ulInLen = sizeof(TCP_KEEPALIVE2);  
		TCP_KEEPALIVE2 outKeepAlive = {0}; //输出参数 
		unsigned long ulOutLen = sizeof(TCP_KEEPALIVE2);  
		unsigned long ulBytesReturn = 0;  

		//设置socket的keep alive为5秒，并且发送次数为3次   
		inKeepAlive.onoff = 1;  
		inKeepAlive.keepaliveinterval = 5000; //两次KeepAlive探测间的时间间隔 
		inKeepAlive.keepalivetime = 5000; //开始首次KeepAlive探测前的TCP空闭时间 

		if (WSAIoctl((unsigned int)m_sock,
		  SIO_KEEPALIVE_VALS,
		  (LPVOID)&inKeepAlive, ulInLen,
		  (LPVOID)&outKeepAlive, ulOutLen,
		  &ulBytesReturn, NULL, NULL)
		  == SOCKET_ERROR)
		{
			CString strError;
			strError.Format("WSAIoctl failed. error code %d", WSAGetLastError());
			AfxMessageBox(strError);
			return FALSE;
		}

		TDATA *pData = new TDATA;
		pData->hWnd = m_hWnd;
		pData->sock = m_sock;
		_beginthread(r, 4026, (void*)pData);

		m_bNetWorkInit = TRUE;
	}
	return TRUE;
}

// 接收用户列表专用函数
BOOL RecvUserList(HWND hWnd, SOCKET sock)
{
	// 主窗口的句柄，用来输出调试信息
	CClient02Dlg *pDlg = (CClient02Dlg*)AfxGetMainWnd();

	int nRecv=0;
	// 由于是
	int nSize = sizeof(XMSG)+sizeof(int);
	char *tmp = new char[nSize];

	// 接受好友数量
	nRecv = recv(sock, (char*)tmp, sizeof(int)*3, 0);
	if (nRecv == 0 || nRecv == -1)
	{
	//	AfxMessageBox("--- Disconnected !\r\n\r\n");
		::PostMessage(hWnd, XWM_DISCONNECTED, NULL, sock);
		// 自然死
		_endthread();
		return FALSE;
	}
	XMSG msg(tmp, sizeof(int)*3);
	delete [] tmp;
	int nLen = 0;
	memcpy((char*)&nLen, msg.GetData(), sizeof(int));
	// 通知主窗口，有多少个好友
	::PostMessage(hWnd, XWM_TOTAL_CONTACT, nLen, sock);

	// 开始接收好友列表数据块
	int nUsers = 10;
	int nTimes = nLen / nUsers;
	int nRemind = nLen % nUsers;

	for (int i=0; i<(nTimes+1); i++)
	{
		int nCounter=0;
		int nRecvBufSize = 0;

		int nRecvSize=0;
		char *buf = NULL;

		if (i==nTimes)
		{
			nRecvSize = nRemind*sizeof(XUserInfo);
		}
		else
		{
			nRecvSize = nUsers*sizeof(XUserInfo);
		}

		buf = new char[nRecvSize];
		ZeroMemory(buf, nRecvSize);

		while (nCounter < nRecvSize)
		{
			if (nCounter==0)
				nRecvBufSize = nRecvSize;
			else
				nRecvBufSize = nRecvSize-nCounter;

			nRecv = recv(sock, buf+nCounter, nRecvBufSize, 0);
			if (nRecv == 0 || nRecv == -1)
			{
				CString str;
				str.Format("recv failed with error:%d", WSAGetLastError());
				pDlg->AddDebugInfo(str);
				::PostMessage(hWnd, XWM_DISCONNECTED, (LPARAM)buf, sock);
				// 自然死
				_endthread();
				return FALSE;
			}
			nCounter += nRecv;
		}

		char *nBuf = new char[nCounter+sizeof(int)];

		if (i==nTimes)
		{
			memcpy(nBuf, (char*)&nRemind, sizeof(int));
		}
		else
		{
			memcpy(nBuf, (char*)&nUsers, sizeof(int));
		}

		memcpy(nBuf+sizeof(int), buf, nCounter);
		delete [] buf;
		::PostMessage(hWnd, XWM_USERLIST, (WPARAM)nBuf, sock);
	}

	::PostMessage(hWnd, XWM_USERLISTEND, NULL, NULL);
	return TRUE;
}


// 程序启动后接收用户离线消息
BOOL RecvMsgOffLine(HWND hWnd, SOCKET sock)
{

	// 发送数据请求消息给服务器
	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_GETDATAOFFLINE;
	send(sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);

	// 主窗口的句柄，用来输出调试信息
	CClient02Dlg *pDlg = (CClient02Dlg*)AfxGetMainWnd();

	int nRecv=0;
	// 由于是
	int nSize = sizeof(XMSG)+sizeof(int);
	char *tmp = new char[nSize];

	// 接受离线消息长度
	nRecv = recv(sock, (char*)tmp, sizeof(int)*3, 0);
	if (nRecv == 0 || nRecv == -1)
	{
	//	AfxMessageBox("--- Disconnected !\r\n\r\n");
		::PostMessage(hWnd, XWM_DISCONNECTED, NULL, sock);
		// 自然死
	//	_endthread();
		return FALSE;
	}
	XMSG msg(tmp, sizeof(int)*3);
	delete [] tmp;
	int nLen = 0;
	memcpy((char*)&nLen, msg.GetData(), sizeof(int));

	if (nLen == 0)
	{
		// 如果没有离线消息的话，就把大权还给别人
		return TRUE;
	}

	char * buf = new char[nLen];
	DWORD total = 0;
	while ((int)total < nLen)
	{
		if ((nLen - total) < 1000)
		{
			nRecv = recv(sock, buf+total, nLen-total, 0);
			if (nRecv == 0 || nRecv == -1)
			{
				CString str;
				str.Format("recv failed with error:%d", WSAGetLastError());
				pDlg->AddDebugInfo(str);
				::PostMessage(hWnd, XWM_DISCONNECTED, (LPARAM)buf, sock);
				// 自然死
				_endthread();
				return FALSE;
			}
		}
		else
		{
			nRecv = recv(sock, buf+total, 1000, 0);
			if (nRecv == 0 || nRecv == -1)
			{
				CString str;
				str.Format("recv failed with error:%d", WSAGetLastError());
				pDlg->AddDebugInfo(str);
				::PostMessage(hWnd, XWM_DISCONNECTED, (LPARAM)buf, sock);
				// 自然死
				_endthread();
				return FALSE;
			}
		}
		total += nRecv;
	}

	int readlen = 0;
	while (readlen <nLen)
	{
	//	XMSG_OFFLINEDATA xdata(buf+readlen);
		XMSG_OFFLINEDATA *pData = new XMSG_OFFLINEDATA(buf+readlen);
	//	CString str;
	//	str.Format("offline msgs %s.", pData->GetData());
	//	AfxMessageBox(pData->GetData());
		DWORD len = pData->GetBufferLength();
		char *buffer = new char[len];
		memcpy(buffer, pData->GetBuffer(), len);
		::PostMessage(hWnd, XWM_MSGOFFLINE, (WPARAM)buffer, NULL);
		readlen += len;
		delete pData;
	}

	delete [] buf;

	return TRUE;
}

// 开始接受历史记录
BOOL RecvMsgHistory(HWND hWnd, SOCKET sock, LPCTSTR szUID, DWORD bufLen)
{
	// 发送数据请求消息给服务器
	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_SENDHISTORYDATA;
	xmsg.SetData(szUID, bufLen);

	send(sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);

	// 主窗口的句柄，用来输出调试信息
	CClient02Dlg *pDlg = (CClient02Dlg*)AfxGetMainWnd();

	int nRecv=0;
	// 由于是
	int nSize = sizeof(XMSG)+sizeof(int)+50;
	char *tmp = new char[nSize];

	// 接受离线消息长度
	nRecv = recv(sock, (char*)tmp, nSize, 0);
	if (nRecv == 0 || nRecv == -1)
	{
	//	AfxMessageBox("--- Disconnected !\r\n\r\n");
		::PostMessage(hWnd, XWM_DISCONNECTED, NULL, sock);
		// 自然死
	//	_endthread();
		return FALSE;
	}

	XMSG msg(tmp, nSize);
	delete [] tmp;
	int nLen = 0;
	char *buf2 = msg.GetData();
	char *bufUID = buf2+sizeof(int);
	memcpy((char*)&nLen, buf2, sizeof(int));

	if (nLen == 0)
	{
		// 如果没有离线消息的话，就把大权还给别人
		::PostMessage(hWnd, XWM_MSGNOHISTORY, NULL, (LPARAM)bufUID);
		return TRUE;
	}

	char * buf = new char[nLen];
	// 零时不处理
	if (buf == NULL)
		return TRUE;

	try{
	DWORD total = 0;
	while ((int)total < nLen)
	{
		if ((nLen - total) < 1000)
		{
			nRecv = recv(sock, buf+total, nLen-total, 0);
			if (nRecv == 0 || nRecv == -1)
			{
				CString str;
				str.Format("recv failed with error:%d", WSAGetLastError());
				pDlg->AddDebugInfo(str);
				::PostMessage(hWnd, XWM_DISCONNECTED, (LPARAM)buf, sock);
				// 自然死
				_endthread();
				return FALSE;
			}
		}
		else
		{
			nRecv = recv(sock, buf+total, 1000, 0);
			if (nRecv == 0 || nRecv == -1)
			{
				CString str;
				str.Format("recv failed with error:%d", WSAGetLastError());
				pDlg->AddDebugInfo(str);
				::PostMessage(hWnd, XWM_DISCONNECTED, (LPARAM)buf, sock);
				// 自然死
				_endthread();
				return FALSE;
			}
		}
		total += nRecv;
	}
	}
	catch (...)
	{
		AfxMessageBox("接收异常");
	}

	int readlen = 0;
	while (readlen < nLen)
	{
		XMSG_OFFLINEDATA odata(buf+readlen);
		DWORD len = odata.GetBufferLength();
		char *buffer = new char[len];
		memcpy(buffer, odata.GetBuffer(), len);
		::PostMessage(hWnd, XWM_MSGHISTORY, (WPARAM)buffer, (LPARAM)bufUID);
		readlen += len;
	}
	delete [] buf;

	return TRUE;
}

void CClient02Dlg::r(void* p)
{
	CClient02Dlg *pDlg = (CClient02Dlg*)(AfxGetApp()->GetMainWnd());

	if(NULL == p)
		_endthread();

	TDATA *pData = (TDATA*)p;

	sockaddr_in aa;
	int slen = sizeof(aa);
	char *buf;
	int rval = 0;

	getpeername(pData->sock,(sockaddr*)&aa,&slen);
//	printf("OK , connected with %s:%u...\r\n\r\n",inet_ntoa(aa.sin_addr),ntohs(aa.sin_port));
	while(1)
	{
		int nBufSize = 1024 * 2;
		buf = new char[nBufSize];
		rval = recv(pData->sock,buf,nBufSize,0);
		if (rval == 0 || rval == -1)
		{
		//	AfxMessageBox("--- Disconnected !\r\n\r\n");
//			::PostMessage(pData->hWnd, XWM_DISCONNECTED, (LPARAM)buf, rval);
			delete[] buf;
			::PostMessage(pData->hWnd, XWM_DISCONNECTED, NULL, rval);
			// 自然死
			delete pData;
			_endthread();
		}
	//	CString strInfo;
	//	strInfo.Format("%d bytes received.", rval);
	//	pDlg->AddDebugInfo(strInfo);
		XMSG xmsg(buf, rval);
		delete buf;
		HWND hWnd = pData->hWnd;

		switch(xmsg.m_nMessage)
		{
			// 收到工作管理信息
		case XEIMMSG_SCHEDULE:
			{
				CString strData = xmsg.GetData();
			//	AfxMessageBox(strData);
				if (m_pDlg_ERP_Project)
				m_pDlg_ERP_Project->AddSchedule(strData);
			/*	if (m_pDlg_ERP_Work)
					m_pDlg_ERP_Work->AddSchedule(strData);*/
			}
			break;
		// 收到项目管理信息
		case XEIMMSG_PROJECT:
		{
		//	AfxMessageBox(xmsg.GetData());
			CString strData = xmsg.GetData();

			if (m_pDlg_ERP_Project)
				m_pDlg_ERP_Project->AddProject(strData);
		}
		break;

		// 开始接收聊天历史数据
		case XEIMMSG_RECVHISTORYDATA:
		{
			//	CString sttr;
			//	sttr.Format("%d %s", xmsg.GetBufferLength(), xmsg.GetData()+50);
			//	AfxMessageBox(sttr);
			RecvMsgHistory(pData->hWnd, pData->sock, xmsg.GetData(), xmsg.GetBufferLength());
		}
		break;

		// 开始接收离线数据
		case XEIMMSG_GETMSGOFFLINE:
			{
				RecvMsgOffLine(pData->hWnd, pData->sock);
			}
			break;
		case XEIMMSG_USEROK:
			{
				XUserDetail *pDetail = new XUserDetail;
				memcpy(pDetail, xmsg.GetData(), xmsg.GetDataLenght());
				::PostMessage(pData->hWnd, XWM_USEROK, (LPARAM)pDetail, pData->sock);
				RecvUserList(pData->hWnd, pData->sock);
			}
			break;
		case XEIMMSG_USERBAD:
			::PostMessage(pData->hWnd, XWM_USERBAD, /*(LPARAM)buf*/0, pData->sock);
			break;
		case XEIMMSG_USERINFO:
			{
			//	char *tmp = new char[xmsg.GetBufLen()];
			//	memcpy(tmp, xmsg.GetBuf(), xmsg.GetBufLen());
				::PostMessage(pData->hWnd, XWM_USERINFO, /*(LPARAM)tmp*/0, pData->sock);
			}
			break;

			// 接收到用户列表数据
/*		case XEIMMSG_USERLIST:
			{
				// 要 delete buf 释放内存
				char *buf = new char[xmsg.GetDataLenght()];
				memcpy(buf, xmsg.GetData(), xmsg.GetDataLenght());
				::PostMessage(pData->hWnd, XWM_USERLIST, (WPARAM)buf, pData->sock);
			}
			break; // XEIMMSG_USERLIST
*/
		case XEIMMSG_USERSTATUS:
			{
				char *buf = new char[xmsg.GetDataLenght()];
				memcpy(buf, xmsg.GetData(), xmsg.GetDataLenght());
				::PostMessage(pData->hWnd, XWM_USERSTATUS, (WPARAM)buf, pData->sock);
			}
			break; // XEIMMSG_USERSTATUS

		case XEIMMSG_NEWMESSAGE:
			{
				char *buf = new char[xmsg.GetDataLenght()];
				memcpy(buf, xmsg.GetData(), xmsg.GetDataLenght());
				::PostMessage(pData->hWnd, XWM_MESSAGE, (WPARAM)buf, pData->sock);
			}
			break; // XEIMMSG_MESSAGE

			// 收到一个新的文件
		case XEIMMSG_FILE:
			{
			//	AfxMessageBox("XEIMMSG_FILE");//
				char *buf = new char[xmsg.GetDataLenght()];
				memcpy(buf, xmsg.GetData(), xmsg.GetDataLenght());
				::PostMessage(pData->hWnd, XWM_FILE, (WPARAM)buf, pData->sock);
			}
			break; // XEIMMSG_FILE
		}
	}
	delete pData;
}

LRESULT CClient02Dlg::XOnUserOK(WPARAM wParam, LPARAM lParam)
{
	//m_xtreeUser.ShowWindow(SW_SHOW);
//	GetDlgItem(IDC_USERLIST)->ShowWindow(SW_SHOW);
//	AfxMessageBox("User OK.");
	XUserDetail *pDetail = (XUserDetail*)wParam;
//	SetWindowText(pDetail->GetUNickName());
	SetDlgItemText(IDC_EDIT_NICKNAME, pDetail->GetUNickName());
	m_strUNickName = pDetail->GetUNickName();
	SetDlgItemText(IDC_EDIT_USERNAME, pDetail->GetUID());

	// 向服务器请求用户列表
	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_GETLIST;
	send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);

	m_bLogedin = TRUE;
	if (pDetail)
	{
		delete pDetail;
		pDetail = NULL;
	}
	return 1;
}

LRESULT CClient02Dlg::XOnUserBad(WPARAM wParam, LPARAM lParam)
{
	if (IDYES == MessageBox("登录失败，用户名或密码错误！\n是否重新登陆？", "错误", MB_YESNO))
	{
		CXLoginDlg dlg;
		if (IDOK == dlg.DoModal())
		{
		}
		else
		{
			OnCancel();
			return 0;
		}
	}
	else
		OnCancel();

	return 1;
}

LRESULT CClient02Dlg::XOnUserInfo(WPARAM wParam, LPARAM lParam)
{
	char *buf = (char*)wParam;
	delete buf;
	return 1;
}

LRESULT CClient02Dlg::XOnUserList(WPARAM wParam, LPARAM lParam)
{
	DWORD dwCount;
	char *buf = (char*)wParam;
	memcpy((char*)&dwCount, buf, sizeof(DWORD));

	int nIdx = sizeof(DWORD);
	CString str;
	XUserInfo xui;
	HTREEITEM hItem;
	HTREEITEM hGroup;
	CString strGroupName;

	m_xtreeUser.SetRedraw(FALSE);
	for (int i=0; i<(int)dwCount; i++)
	{
		// 获取 XUserInfo 数据
		memcpy((char*)&xui, buf+nIdx, sizeof(XUserInfo));
		// 把索引指向下一个数据块
		nIdx += sizeof(XUserInfo);

		// 先获取并保存部门信息。存在就跳过，不存在则保存。
		map<string, HTREEITEM>::iterator iGroup;
		iGroup = m_mapGroup.find(xui.GetUGroup());//	AfxMessageBox(xui.GetUGroup());
		if (iGroup == m_mapGroup.end())	// 不存在
		{
			// 	m_hRoot = m_xtreeUser.InsertItem("联系人(0/0)", 1, 1);
			hGroup = m_xtreeUser.InsertItem(xui.GetUGroup(), 1, 1);
			m_mapGroup.insert(pair<string,HTREEITEM>(xui.GetUGroup(), hGroup));
			strGroupName = xui.GetUGroup();
		}
		else // 该组已存在
		{
			hGroup = iGroup->second;
			strGroupName = (iGroup->first).c_str();
		}


		// 把用户添加到联系人列表
		if (xui.GetUStatus() == STATUS_ONLINE)	// 在线用户
		{
			hItem = m_xtreeUser.InsertItem(xui.GetUNickName(), 4, 4, hGroup);
			m_xtreeUser.SetItemData(hItem, STATUS_ONLINE);
			m_xtreeUser.SetItemColor(hItem, RGB(25, 79, 149));
		//	m_xtreeUser.SetItemBold(hGroup, TRUE);
			m_xtreeUser.UpdateGroupStatus(hGroup, strGroupName);
			m_dwOnLine ++;
		}
		else		// 不在线用户
		{
			hItem = m_xtreeUser.InsertItem(xui.GetUNickName(), 3, 3, hGroup);
			m_xtreeUser.SetItemData(hItem, STATUS_OFFLINE);
			m_xtreeUser.SetItemColor(hItem, RGB(128,128,128));
			m_xtreeUser.UpdateGroupStatus(hGroup, strGroupName);
		}

		CXChatDlg *pdlg = new CXChatDlg;
		pdlg->m_strUser = xui.GetUNickName();
		pdlg->m_strUID = xui.GetUID();
		pdlg->m_bMobile = xui.UserHasMobile();
		pdlg->m_sock = m_sock;
		m_mapUserList.insert(pair<HTREEITEM,CXChatDlg*>(hItem, pdlg));
		m_listUserInfo.push_back(xui);

		XSortUser(hGroup);
	}
	str.Format("联系人(%d/%d)", m_dwOnLine, m_mapUserList.size());

	m_xtreeUser.SetRedraw();

	SetDlgItemText(IDC_STATIC_CONTACT, str);

	// 释放资源
	delete [] buf;
	return 1;
}

LRESULT CClient02Dlg::XOnListEnd(WPARAM wParam, LPARAM lParam)
{
	// 获取完用户好友之后，隔一点时间后请求获取用户离线消息
	SetTimer(TIMER_GET_OFFLINEMSG, TIMER_GET_OFFLINEMSG_TIME, NULL);
	// 检测客户端是否有新版本哦
	SetTimer(TIMER_CHECK_VERSION, TIMER_CHECK_VERSION_TIME, NULL);
//	AfxMessageBox("List end.");
	return 1;
}

LRESULT CClient02Dlg::XOnUserStatus(WPARAM wParam, LPARAM lParam)
{
	char *buf = (char*)wParam;
	XUserInfo xui;
	CString str;
	CString strUID;

	memcpy((char*)&xui, buf, sizeof(XUserInfo));

	map<HTREEITEM, CXChatDlg*>::iterator imap, _end;
	imap = m_mapUserList.begin();
	_end = m_mapUserList.end();

	// 用户的状态
	int iStatus = xui.GetUStatus();
	// 用户ID
	strUID = xui.GetUID();

	while (imap != _end)
	{
		if ((imap->second)->m_strUID == strUID)
		{
			// 用户登录了
			if (iStatus == STATUS_ONLINE)
			{
				// 如果用户已经在线了，就什么也不做
				if (m_xtreeUser.GetItemData(imap->first) == STATUS_ONLINE)
					break;

				m_dwOnLine ++;
				m_xtreeUser.SetItemImage(imap->first, 4, 4);
				m_xtreeUser.SetItemColor(imap->first, RGB(25, 79, 149));
				m_xtreeUser.SetItemData(imap->first, STATUS_ONLINE);

				m_mapUserList[imap->first]->SetUserStatus(STATUS_ONLINE);
				str.Format("联系人(%d/%d)", m_dwOnLine, m_mapUserList.size());
				SetDlgItemText(IDC_STATIC_CONTACT, str);
				HTREEITEM hParent = m_xtreeUser.GetParentItem(imap->first);

				// 先获取并保存部门信息。存在就跳过，不存在则保存。
				map<string, HTREEITEM>::iterator iGroup;
				for (iGroup=m_mapGroup.begin();iGroup!=m_mapGroup.end();++iGroup)
				{
					if (iGroup->second == hParent)
					{
						m_xtreeUser.UpdateGroupStatus(hParent, iGroup->first.c_str());
						break;
					}
				}
		//		m_xtreeUser.SetItemColor(hParent, RGB(180,180,0));
			//	m_xtreeUser.SetItemText(m_hRoot, str);
			} // END 用户登录了

			else // 用户离线了
			{
				m_dwOnLine --;
				m_xtreeUser.SetItemImage(imap->first, 3, 3);
				m_xtreeUser.SetItemData(imap->first, STATUS_OFFLINE);
				m_xtreeUser.SetItemColor(imap->first, RGB(128,128,128));
				m_mapUserList[imap->first]->SetUserStatus(STATUS_OFFLINE);
				str.Format("联系人(%d/%d)", m_dwOnLine, m_mapUserList.size());
				SetDlgItemText(IDC_STATIC_CONTACT, str);
				HTREEITEM hParent = m_xtreeUser.GetParentItem(imap->first);
				map<string, HTREEITEM>::iterator iGroup;
				for (iGroup=m_mapGroup.begin();iGroup!=m_mapGroup.end();++iGroup)
				{
					if (iGroup->second == hParent)
					{
						m_xtreeUser.UpdateGroupStatus(hParent, iGroup->first.c_str());
						break;
					}
				}
			//	m_xtreeUser.SetItemText(m_hRoot, str);
			} // END 用户离线了

			HTREEITEM hParent = m_xtreeUser.GetParentItem(imap->first);
			XSortUser(hParent);
			break;
		}

		imap ++;
	}

	// 联系人不在列表里，可能是服务器新添加的用户
	if (imap == _end)
	{
		HTREEITEM hGroup;
		HTREEITEM hItem;
		// 先获取并保存部门信息。存在就跳过，不存在则保存。
		map<string, HTREEITEM>::iterator iGroup;
		iGroup = m_mapGroup.find(xui.GetUGroup());//	AfxMessageBox(xui.GetUGroup());
		if (iGroup == m_mapGroup.end())	// 不存在
		{
			// 	m_hRoot = m_xtreeUser.InsertItem("联系人(0/0)", 1, 1);
			hGroup = m_xtreeUser.InsertItem(xui.GetUGroup(), 1, 1);
			m_mapGroup.insert(pair<string,HTREEITEM>(xui.GetUGroup(), hGroup));
		}
		else // 该组已存在
		{
			hGroup = iGroup->second;
		}

		// 把用户添加到联系人列表
		if (iStatus == STATUS_ONLINE)	// 在线用户
		{
			hItem = m_xtreeUser.InsertItem(xui.GetUNickName(), 4, 4, hGroup);
			m_xtreeUser.SetItemData(hItem, STATUS_ONLINE);
			m_dwOnLine ++;
		}
		else		// 不在线用户
		{
			hItem = m_xtreeUser.InsertItem(xui.GetUNickName(), 3, 3, hGroup);
			m_xtreeUser.SetItemData(hItem, STATUS_OFFLINE);
		}

		CXChatDlg *pdlg = new CXChatDlg;
		pdlg->m_strUser = xui.GetUNickName();
		pdlg->m_strUID = xui.GetUID();
		pdlg->m_sock = m_sock;
		m_mapUserList.insert(pair<HTREEITEM,CXChatDlg*>(hItem, pdlg));
		m_listUserInfo.push_back(xui);

		XSortUser(hGroup);
	}
	else
	{
		list<XUserInfo>::iterator iUserInfo = m_listUserInfo.begin();
		list<XUserInfo>::iterator _end = m_listUserInfo.end();
		while( iUserInfo != _end )
		{
			if (iUserInfo->GetUID() == strUID)
			{
				if (iStatus == STATUS_ONLINE)
				{
					iUserInfo->OnLine();
				}
				else
				{
					iUserInfo->OffLine();
				}
				break;
			}
			iUserInfo ++;
		}
	}

//	XSortUser();
	// Very inportant te delete buf
	delete [] buf;
	return 1;
}


LRESULT CClient02Dlg::XOnMessage(WPARAM wParam, LPARAM lParam)
{
	char *buf = (char*)wParam;
	XMSG_ONLINE omsg(buf);

	char *data = omsg.GetData();
	CString strUID = omsg.GetUID();
	if (strUID == "xx--11--")
	{
		strUID.Format("%s", UrlGB2312Decode(omsg.GetData()).c_str());
//		::MessageBox(m_hWnd, strUID, "系统消息", MB_OK|MB_ICONINFORMATION);
		if (m_erpDlg == NULL) // 在第一次使用时才创建
		{
			m_erpDlg = new DLG_ERP_MESSAGE(this);
			m_erpDlg->Create(IDD_DIALOG7, GetDesktopWindow());
		}
		if (::IsWindow(m_erpDlg->GetSafeHwnd()))
		{
			m_erpDlg->ShowWindow(SW_SHOW);
			m_erpDlg->SetMessage(strUID);

			CString str;
			str.Format(_T("[消息(%d)]"), m_erpDlg->m_vecMessage.size());
			SetDlgItemText(IDC_STATIC_SYSMSG, str);
		}
		delete buf;
		return 0;
	}

	map<HTREEITEM, CXChatDlg*>::iterator imap;
	for (imap=m_mapUserList.begin(); imap!=m_mapUserList.end(); ++imap)
	{
		if ((imap->second)->m_strUID == strUID)
		{
			if (!::IsWindow((imap->second)->GetSafeHwnd()))
			{
				(imap->second)->Create(IDD_DIALOG2, GetDesktopWindow());
			}
			(imap->second)->XAddMessage(omsg.GetData());
			// 看看用户的当前状态
			DWORD dwStatus = m_xtreeUser.GetItemData(imap->first);
			(imap->second)->SetUserStatus(dwStatus);
			(imap->second)->FlashWindow();
			break;
		}
	}

	delete buf;
	return 1;
}

LRESULT CClient02Dlg::XOnFile(WPARAM wParam, LPARAM lParam)
{
	char *buf = (char*)wParam;
	XMSG_ONLINE omsg(buf);

	char *data = omsg.GetData();
	CString strUID = omsg.GetUID();

	// 特殊情况
	if (strUID == "xx--11--")
	{
		strUID.Format("%s", UrlGB2312Decode(omsg.GetData()).c_str());
		::MessageBox(m_hWnd, strUID, "系统消息", MB_OK|MB_ICONINFORMATION);
		delete buf;
		return 0;
	}

	map<HTREEITEM, CXChatDlg*>::iterator imap;
	for (imap=m_mapUserList.begin(); imap!=m_mapUserList.end(); ++imap)
	{
		if ((imap->second)->m_strUID == strUID)
		{
			if (!::IsWindow((imap->second)->GetSafeHwnd()))
			{
				(imap->second)->Create(IDD_DIALOG2, GetDesktopWindow());
			}
			(imap->second)->XRecvFile((LPXFILEINFO)omsg.GetData());
			(imap->second)->ShowWindow(SW_SHOW);
			break;
		}
	}

	delete buf;
	return 1;
}

void CClient02Dlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
//	AfxMessageBox("ONClose");
//	m_inifile.Close();
//	CDialog::OnClose();
	ShowWindow(SW_HIDE);
}

void CClient02Dlg::OnCancel()
{
	// TODO: Add extra cleanup here
//	AfxMessageBox("OnCancel");
	// 退出处理
	if (m_bLogedin)
	{
		if (IDYES != MessageBox("要退出 XEIM 沟通专家 吗？", "警告", MB_YESNO|MB_ICONQUESTION))
		{
			return;
		}
	}

	if (m_pPluginBar)
	{
		m_pPluginBar->CloseAllProgram();
		delete m_pPluginBar;
		m_pPluginBar = NULL;
	}

	shutdown(m_sock, SD_BOTH);
	closesocket(m_sock);
	m_sysTray.HideIcon();
	m_sysTray.RemoveIcon();
//	m_inifile.Close();
	CDialog::OnCancel();
}

void CClient02Dlg::OnDblclkUserlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	GetCursorPos(&pt);
	m_xtreeUser.ScreenToClient(&pt);
	HTREEITEM hItem = m_xtreeUser.HitTest(pt);

	// 如果用户双击 用户分组的 TREE ITEM 就 什么也不做返回
	map<string,HTREEITEM>::iterator iGroup;
	for(iGroup=m_mapGroup.begin();
		iGroup!=m_mapGroup.end(); ++iGroup)
	{
		if(iGroup->second == hItem)
		{
		//	CString itemText = m_xtreeUser.GetItemText(hItem);
		//	AfxMessageBox(itemText);
			return;
		}
	}

	// 用户选择了某个联系人
	if (m_hRoot != hItem)
	{
		if (!::IsWindow(m_mapUserList[hItem]->GetSafeHwnd()))
		{
			m_mapUserList[hItem]->Create(IDD_DIALOG2, GetDesktopWindow());
		}
//		m_mapUserList[hItem]->ShowWindow(SW_SHOW);
	}

	// 看看用户的当前状态
	DWORD dwStatus = m_xtreeUser.GetItemData(hItem);
	m_mapUserList[hItem]->SetUserStatus(dwStatus);

	// 为什么不直接 m_mapUserList[hItem]->ShowWindow(SW_NORMAL);
	// 因为直接那样显示，Chat窗口的焦点会失去，焦点回到Main窗口.
	// 可能是TreeCtrl 的问题
	m_mapUserList[hItem]->PostMessage(XM_EMDBLCLICKTREE);
	*pResult = 0;
}

int CALLBACK
CClient02Dlg::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   // lParamSort contains a pointer to the tree control.
   // The lParam of an item is just its handle, 
// as specified with <A HREF="_mfc_ctreectrl.3a3a.setitemdata.htm">SetItemData</A>
   CTreeCtrl* pmyTreeCtrl = (CTreeCtrl*) lParamSort;
  // CString    strItem1 = pmyTreeCtrl->GetItemText((HTREEITEM) lParam1);
  // CString    strItem2 = pmyTreeCtrl->GetItemText((HTREEITEM) lParam2);
  // AfxMessageBox(strItem2);
   DWORD dw1 = lParam1;
   DWORD dw2 = lParam2;
   //return strcmp(strItem2, strItem1);
   return dw2 < dw1;
}

void CClient02Dlg::XSortUser(HTREEITEM hParent)
{
	TVSORTCB tvs;

	// Sort the tree control's items using my
	// callback procedure.
	tvs.hParent = hParent;
	tvs.lpfnCompare = MyCompareProc;
	tvs.lParam = (LPARAM)&m_xtreeUser;

	m_xtreeUser.SortChildrenCB(&tvs);
//	AfxMessageBox("tttttttttttttt");
}

void CClient02Dlg::OnTrayRestore()
{
	ShowWindow(SW_SHOW);
//	m_sysTray.HideIcon();
}

LRESULT CClient02Dlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	// Delegate all the work back to the default implementation in CSystemTray.
	return m_sysTray.OnTrayNotification(wParam, lParam);	
}

LRESULT CClient02Dlg::OnChildFire(WPARAM wparam, LPARAM lparam)
{
	switch ((int)wparam)
	{
	case IDC_SETTINGS:
		{
			HINSTANCE hInst;
			hInst = LoadLibrary("xsetting.dll");
			if (hInst)
			{
				typedef void(*pTest)(LPCTSTR, LPCTSTR);
				pTest test2 = (pTest)GetProcAddress(hInst, "test2");
				if (test2)
				{
					test2((LPCTSTR)m_strUID, (LPCTSTR)m_strUPSW);
				}
				else
				{
					AfxMessageBox("找不到 xsetting.dll");
					return 0;
				}
				FreeLibrary(hInst);
				hInst = NULL;
			}
		}
		break;
	case IDC_STATIC_SYSMSG:
		{
			OnButton1();
		}
		break;
	case IDC_STATIC_MENU:
		{
			//菜单
			CMenu menu;
			menu.LoadMenu(IDM_CHAT);

			CMenu* pPopMenu = menu.GetSubMenu(1);
			CRect rc;
			GetDlgItem(IDC_STATIC_MENU)->GetClientRect(rc);
			ClientToScreen(rc);
			CPoint pt;
			GetCursorPos(&pt);
			
			pPopMenu->TrackPopupMenu(TPM_BOTTOMALIGN |TPM_RIGHTBUTTON, 
				rc.left, pt.y, this);
		}
		break;
	default:
		break;
	}

	return 1;
}

void CClient02Dlg::AddDebugInfo(CString strInfo)
{
	m_strDebug+= "\r\n";
	m_strDebug += strInfo;
	UpdateData(FALSE);
	CEdit* pEd=(CEdit*)GetDlgItem(IDC_EDIT1);
	pEd->PostMessage(WM_VSCROLL, SB_BOTTOM);
}


LRESULT CClient02Dlg::XOnTotalContact(WPARAM wParam, LPARAM lParam)
{
	CString str;
	str.Format("联系人(0/%d)", wParam);
	SetDlgItemText(IDC_STATIC_CONTACT, str);
	return 1;
}


// 向服务器请求用户的离线消息
void CClient02Dlg::XGetMsgOffLine()
{
//	AfxMessageBox("XGetMsgOffLine");
	// 向服务器请求用户列表
	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_GETMSGOFFLINE;
	send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
//	m_bLogedin = TRUE;

}

void CClient02Dlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TIMER_GET_OFFLINEMSG:
		{
			XGetMsgOffLine();
			KillTimer(nIDEvent);
		}
		break;
	case TIMER_CHECK_VERSION:
		{
			XCheckVersion();
			KillTimer(TIMER_CHECK_VERSION);
		}
		break;
	case TIMER_CONNECT:
		{
			KillTimer(TIMER_CONNECT);
			ReConnect();
		}
		break;
	case TIMER_SHOW_INFO:
		{
			KillTimer(TIMER_SHOW_INFO);
			SetDlgItemText(IDC_STATIC_INFO, "与服务器断开连接！\r\n\r\n正在尝试重新连接.");
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

// 无当天历史消息
LRESULT CClient02Dlg::XMsgNoHistory(WPARAM wParam, LPARAM lParam)
{
	CString strUID = (char*)lParam;

	map<HTREEITEM, CXChatDlg*>::iterator imap;
	for (imap=m_mapUserList.begin(); imap!=m_mapUserList.end(); ++imap)
	{
		if ((imap->second)->m_strUID == strUID)
		{
			if (!::IsWindow((imap->second)->GetSafeHwnd()))
			{
				(imap->second)->Create(IDD_DIALOG2, GetDesktopWindow());
			}

			(imap->second)->XNoHistoryMsg(strUID);
			// 看看用户的当前状态
		//	DWORD dwStatus = m_xtreeUser.GetItemData(imap->first);
		//	(imap->second)->SetUserStatus(dwStatus);
		//	(imap->second)->FlashWindow();
			break;
		}
	}
	return 1;
}
// 开始接受历史消息
LRESULT CClient02Dlg::XMsgHistory(WPARAM wParam, LPARAM lParam)
{
	char *buf = (char*)wParam;
	XMSG_OFFLINEDATA omsg(buf);

	char *data = omsg.GetData();
	CString strUID = (char*)lParam;

	map<HTREEITEM, CXChatDlg*>::iterator imap;
	for (imap=m_mapUserList.begin(); imap!=m_mapUserList.end(); ++imap)
	{
		if ((imap->second)->m_strUID == strUID)
		{
			if (!::IsWindow((imap->second)->GetSafeHwnd()))
			{
				(imap->second)->Create(IDD_DIALOG2, GetDesktopWindow());
			}

			(imap->second)->XAddHistoryMsg(omsg.GetData(), omsg.GetUID(), omsg.GetDate());
			// 看看用户的当前状态
		//	DWORD dwStatus = m_xtreeUser.GetItemData(imap->first);
		//	(imap->second)->SetUserStatus(dwStatus);
		//	(imap->second)->FlashWindow();
			break;
		}
	}

	delete [] buf;
	return 1;
}

// 用户收到服务器发来的离线消息
LRESULT CClient02Dlg::XMsgRecvOffLine(WPARAM wParam, LPARAM lParam)
{
	char *buf = (char*)wParam;
	XMSG_OFFLINEDATA omsg(buf);

	char *data = omsg.GetData();
	CString strUID = omsg.GetUID();
	if (strUID == "xx--11--")
	{
		strUID.Format("%s", UrlGB2312Decode(omsg.GetData()).c_str());
	//	::MessageBox(m_hWnd, strUID, "系统消息", MB_OK|MB_ICONINFORMATION);
		if (m_erpDlg == NULL) // 在第一次使用时才创建
		{
			m_erpDlg = new DLG_ERP_MESSAGE(this);
			m_erpDlg->Create(IDD_DIALOG7, GetDesktopWindow());
		}
		if (::IsWindow(m_erpDlg->GetSafeHwnd()))
		{
			m_erpDlg->ShowWindow(SW_SHOW);
			m_erpDlg->SetMessage(strUID);
		}
		delete buf;
		return 0;
	}

	map<HTREEITEM, CXChatDlg*>::iterator imap;
	for (imap=m_mapUserList.begin(); imap!=m_mapUserList.end(); ++imap)
	{
		if ((imap->second)->m_strUID == strUID)
		{
			if (!::IsWindow((imap->second)->GetSafeHwnd()))
			{
				(imap->second)->Create(IDD_DIALOG2, GetDesktopWindow());
			}
			(imap->second)->XAddOffLineMsg(omsg.GetData(), omsg.GetDate());
			// 看看用户的当前状态
			DWORD dwStatus = m_xtreeUser.GetItemData(imap->first);
			(imap->second)->SetUserStatus(dwStatus);
			(imap->second)->FlashWindow();
			break;
		}
	}

	delete [] buf;
	return 1;
}

// 1. 处理用户选择右键菜单
void CClient02Dlg::OnRclickUserlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DWORD SelectionMade = 0;
	CString strGroup;
	CPoint pt;
	GetCursorPos( &pt);
	m_xtreeUser.ScreenToClient( &pt);
	HTREEITEM hItem = m_xtreeUser.HitTest(pt);
	m_xtreeUser.ClientToScreen( &pt);

	CMenu add;
	VERIFY(add.CreatePopupMenu());

	if (NULL == hItem)
	{
	}
	else
	{
		// 如果用户双击 用户分组的 TREE ITEM 就 什么也不做返回
		map<string,HTREEITEM>::iterator iGroup, _end;
		_end = m_mapGroup.end();
		for(iGroup=m_mapGroup.begin();
			iGroup!=_end; ++iGroup)
		{
			// 选择了一个部门
			if(iGroup->second == hItem)
			{
				strGroup = iGroup->first.c_str();

				VERIFY(add.AppendMenu(MF_STRING, 22, _T("部门群发")));
				VERIFY(add.AppendMenu(MF_STRING, 55, _T("发给所有人")));
				SelectionMade = add.TrackPopupMenu(TPM_LEFTALIGN |
					TPM_LEFTBUTTON |
					TPM_NONOTIFY |
					TPM_RETURNCMD, pt.x, pt.y, this);
				add.DestroyMenu();
				break;
			}
		}
		if (iGroup == m_mapGroup.end())
		{
			// 用户选择了某个联系人
			VERIFY(add.AppendMenu(MF_STRING, 33, _T("发消息")));
			SelectionMade = add.TrackPopupMenu(TPM_LEFTALIGN |
				TPM_LEFTBUTTON |
				TPM_NONOTIFY |
				TPM_RETURNCMD,
				pt.x, pt.y, this);

			add.DestroyMenu();
		}

	}

	switch(SelectionMade)
	{
		// ..... do something here .....
		// 群发
		case 22:
		{
			if (m_pGroupMsg == NULL)
			{
				m_pGroupMsg = new CXChatDlg;
				m_pGroupMsg->m_bGroup = TRUE;
				m_pGroupMsg->m_sock = m_sock;
			}
			if (!::IsWindow(m_pGroupMsg->GetSafeHwnd()))
			{
				m_pGroupMsg->Create(IDD_DIALOG2, GetDesktopWindow());
			}
			CString strTitle;
			strTitle.Format("%s - 部门群发", strGroup);
			m_pGroupMsg->SetWindowText(strTitle);
			m_pGroupMsg->m_strUID = strGroup;

			// 为什么不直接 m_mapUserList[hItem]->ShowWindow(SW_NORMAL);
			// 因为直接那样显示，Chat窗口的焦点会失去，焦点回到Main窗口.
			// 可能是TreeCtrl 的问题
			m_pGroupMsg->PostMessage(XM_EMDBLCLICKTREE);
		}
	//	PostMessage(WM_EMSENDALL);
		break;

		// 发送消息
		case 33:									
		{
			// 用户选择了某个联系人
			if (m_hRoot != hItem)
			{
				if (!::IsWindow(m_mapUserList[hItem]->GetSafeHwnd()))
				{
					m_mapUserList[hItem]->Create(IDD_DIALOG2, GetDesktopWindow());
				}
			}
			// 看看用户的当前状态
			DWORD dwStatus = m_xtreeUser.GetItemData(hItem);
			m_mapUserList[hItem]->SetUserStatus(dwStatus);
			
			// 为什么不直接 m_mapUserList[hItem]->ShowWindow(SW_NORMAL);
			// 因为直接那样显示，Chat窗口的焦点会失去，焦点回到Main窗口.
			// 可能是TreeCtrl 的问题
			m_mapUserList[hItem]->PostMessage(XM_EMDBLCLICKTREE);
		}
		break;

		case 55:
		{
			if (m_pGroupMsg == NULL)
			{
				m_pGroupMsg = new CXChatDlg;
				m_pGroupMsg->m_bGroup = TRUE;
				m_pGroupMsg->m_sock = m_sock;
			}
			if (!::IsWindow(m_pGroupMsg->GetSafeHwnd()))
			{
				m_pGroupMsg->Create(IDD_DIALOG2, GetDesktopWindow());
			}
			CString strTitle = _T("发给所有人");
			m_pGroupMsg->SetWindowText(strTitle);
			m_pGroupMsg->m_strUID = strGroup;

			// 为什么不直接 m_mapUserList[hItem]->ShowWindow(SW_NORMAL);
			// 因为直接那样显示，Chat窗口的焦点会失去，焦点回到Main窗口.
			// 可能是TreeCtrl 的问题
			m_pGroupMsg->PostMessage(XM_EMDBLCLICKTREE);
		}
	}

	*pResult = 0;
}

HBRUSH CClient02Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
		return (HBRUSH)m_brBk2.GetSafeHandle();
		break;
	case CTLCOLOR_STATIC:
		{
		//	if (pWnd->GetDlgCtrlID() == IDC_EDIT_NICKNAME)
			{
				pDC->SetBkColor(m_editBKColor);
				pDC->SetTextColor(m_textcolor);
				hbr = (HBRUSH) m_brBk2;
			}
		}
		break;

	//	else
	//		return (HBRUSH)m_brBk1.GetSafeHandle();
	// TODO: Return a different brush if the default is not desired
	}
	return hbr;
}

void CClient02Dlg::XCheckVersion()
{
	// 向服务器查询最新版的客户端
	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_CHECK_VERSION;
	send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
}

void CClient02Dlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

// 发送普通消息
void CClient02Dlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	// 向服务器发送普通短信
/*	CNormalSMS dlg(this);
	if (IDOK == dlg.DoModal())
	{
	//	XMSG xmsg;
	//	xmsg.m_nMessage = XEIMMSG_NORMAL_SMS;

		CString strSMS;
		CString strNickName;
	//	CString strNUM = dlg.m_strRecv;
		AfxGetMainWnd()->GetDlgItemText(IDC_EDIT_NICKNAME, strNickName);
		strSMS.Format(_T("%s：%s"), dlg.m_strSign, dlg.m_strSMS);

		char *tmp = dlg.ChangeCODE((LPCTSTR)strSMS);

		// 统计收信人号码，并逐个发送
		CString m_strInput = dlg.m_strRecv;
		m_strInput += ",";
//		AfxMessageBox(m_strInput);
		int nIdx = 0;
		CString strTmp;
		nIdx = m_strInput.Find(',');
		while (-1 != (nIdx=m_strInput.Find(',')))
		{
			strTmp = m_strInput.Left(nIdx);
			strTmp.TrimLeft();
			strTmp.TrimRight();
			if (! strTmp.IsEmpty())
			{
				if (dlg.IsPhoneNumber(strTmp))
				{
					XMSG xmsg;
					xmsg.m_nMessage = XEIMMSG_SMS_RECORD;
					XMSG_ONLINE omsg;

					omsg.SetData(strTmp, (LPCTSTR)strSMS, strSMS.GetLength()+1);
					xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());
					send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
					Sleep(268);
					SendSMS(strTmp, tmp);
				}
			}

			int len = m_strInput.GetLength();
			strTmp = m_strInput.Right(len - nIdx -1);
			m_strInput = strTmp;
			nIdx = m_strInput.Find(',');
		}
*/
	/*	DWORD dwLen = strlen(tmp);
		DWORD dwIndex = 0;
		DWORD dwMaxSMS = 70*4;
		char *buf = NULL;

		while (dwIndex < dwLen)
		{
			XMSG_SMS sms;
			DWORD dwSendLen = dwMaxSMS;
			if (dwLen - dwIndex < dwMaxSMS)
			{
				dwSendLen = dwLen-dwIndex;
			}
			// Add 1 to contain NULL
			buf = new char[dwSendLen+3];
			if (strlen(buf) > 70*4+2)
			{
				CString str2;
				str2.Format("你所发的信息太长，有 %d 个字，请分几次发送，最大 70 个字符。", strlen(tmp)/4);
				MessageBox(str2, "警告", MB_ICONINFORMATION);
				return;
			}
			sprintf(buf, "%02X", dwSendLen/2);
			memcpy(buf+2, tmp+dwIndex, dwSendLen);
			dwIndex += dwSendLen;
			buf[dwSendLen+2] = NULL;

			sms.SetData((LPCTSTR)strNUM, buf, dwSendLen+3);
			CString strInfo;
			strInfo.Format("%d", sms.GetBufferLength());

			xmsg.SetData(sms.GetData(), sms.GetBufferLength());
			send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);

			delete [] buf;
		}
*/
/*		delete [] tmp;
	}*/
/*	if (m_pDlg_ERP_Project == NULL)
	{
		m_pDlg_ERP_Project = new DLG_ERP_PROJECT;
		m_pDlg_ERP_Project->DoModal();

		delete m_pDlg_ERP_Project;
		m_pDlg_ERP_Project = NULL;
	}*/

	if (m_erpDlg == NULL) // 在第一次使用时才创建
	{
		m_erpDlg = new DLG_ERP_MESSAGE(this);
		m_erpDlg->Create(IDD_DIALOG7, GetDesktopWindow());
	}
	if (::IsWindow(m_erpDlg->GetSafeHwnd()))
	{
		m_erpDlg->ShowWindow(SW_SHOW);
	//	m_erpDlg->SetMessage(strUID);
	}
}

BOOL CClient02Dlg::SendSMS(LPCTSTR szRecv, LPCTSTR szSMS)
{
	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_NORMAL_SMS;

	CString strNUM = szRecv;
	char *tmp = (LPSTR)szSMS;
	DWORD dwLen = strlen(tmp);
	DWORD dwIndex = 0;
	DWORD dwMaxSMS = 70*4;
	char *buf = NULL;

	while (dwIndex < dwLen)
	{
		XMSG_SMS sms;
		DWORD dwSendLen = dwMaxSMS;
		if (dwLen - dwIndex < dwMaxSMS)
		{
			dwSendLen = dwLen-dwIndex;
		}
		// Add 1 to contain NULL
		buf = new char[dwSendLen+3];
		if (strlen(buf) > 70*4+2)
		{
			CString str2;
			str2.Format("你所发的信息太长，有 %d 个字，请分几次发送，最大 70 个字符。", strlen(tmp)/4);
			MessageBox(str2, "警告", MB_ICONINFORMATION);
			return FALSE;
		}
		sprintf_s(buf, dwSendLen+3, "%02X", dwSendLen/2);
		memcpy(buf+2, tmp+dwIndex, dwSendLen);
		dwIndex += dwSendLen;
		buf[dwSendLen+2] = NULL;

		sms.SetData((LPCTSTR)strNUM, buf, dwSendLen+3);
		CString strInfo;
		strInfo.Format("%d", sms.GetBufferLength());

		xmsg.SetData(sms.GetData(), sms.GetBufferLength());
		send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);

		delete [] buf;
	}

	return TRUE;
}

void CClient02Dlg::ReConnect()
{
	if (Init())
	{
		SOCKET m_sock = ((CClient02Dlg*)AfxGetMainWnd())->m_sock;
		XMSG message;
		XUserDetail xLogin;
	//	xAcc.SetAccount((LPCTSTR)user, (LPCTSTR)pass);
		xLogin.SetUID((LPCTSTR)m_strUID);
		xLogin.SetUPassword((LPCTSTR)m_strUPSW);
		xLogin.SetUNickName("");
		xLogin.OnLine();

		message.m_nMessage = XEIMMSG_LOGIN;
		message.SetData((char*)&xLogin, sizeof(xLogin));

		int ret = send(m_sock, (LPCTSTR)message.GetBuffer(), message.GetBufferLength(), 0);
		if (SOCKET_ERROR == ret || 0 == ret)
		{
			AfxMessageBox("发送不成功！");
			return;
		}
		GetDlgItem(IDC_EDIT_NICKNAME)->EnableWindow();
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow();
		GetDlgItem(IDC_STATIC18)->EnableWindow();
		GetDlgItem(IDC_SETTINGS)->EnableWindow();
		GetDlgItem(IDC_STATIC_INFO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_USERLIST)->ShowWindow(SW_SHOW);
	}
	else
	{
		CString strError;
		strError.Format("与服务器断开连接！\r\n\r\n无法连接：%s", m_strLastError);
		SetDlgItemText(IDC_STATIC_INFO, strError);
		// 要设置重新连接册数 5 次足有吧
		SetTimer(TIMER_CONNECT, TIMER_CONNECT_TIME, NULL);
	}
}


LRESULT CClient02Dlg::XOnDisconnected(WPARAM wParam, LPARAM lParam)
{
	
	m_bNetWorkInit = FALSE;
	// 在线用户计数器
	m_dwOnLine = 0;

	map<HTREEITEM, CXChatDlg*>::iterator imap;
	for (imap=m_mapUserList.begin(); imap!=m_mapUserList.end(); ++imap)
	{
		CXChatDlg *pDlg = (CXChatDlg*)imap->second;
		delete pDlg;
		pDlg = NULL;
	}
	// 所有联系人的信息
	m_listUserInfo.clear();
	// 每个用户对应分配一个对话框
	m_mapUserList.clear();
	// 所有部门名称
	m_mapGroup.clear();

	m_xtreeUser.SetRedraw(FALSE);
	m_xtreeUser.DeleteAllItems();
	m_xtreeUser.SetRedraw();

	SetDlgItemText(IDC_STATIC_CONTACT, "联系人(0/0)");

	if (lParam == 0)
	{
		closesocket(m_sock);
		if (IDYES == ::MessageBox(GetSafeHwnd(), "用户在其他地方登录！是否重新登陆？", "登录", MB_YESNO))
		{
			ShowWindow(SW_HIDE);
			m_xtreeUser.ShowWindow(SW_SHOW);
			// 启动登录对话框
			CXLoginDlg dlg(GetDesktopWindow());
			if (IDOK == dlg.DoModal())
			{
				ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_INFO)->ShowWindow(SW_HIDE);
			}
			else
			{
				CDialog::OnOK();
			}
		}
		else
		{
			CDialog::OnOK();
		}
	//	CDialog::OnOK();
	}
	else
	{
		GetDlgItem(IDC_STATIC_INFO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_NICKNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC18)->EnableWindow(FALSE);
		GetDlgItem(IDC_SETTINGS)->EnableWindow(FALSE);
		GetDlgItem(IDC_USERLIST)->ShowWindow(SW_HIDE);
		// 设置重新登陆时间
		SetTimer(TIMER_CONNECT, TIMER_CONNECT_TIME, NULL);
		SetTimer(TIMER_SHOW_INFO, TIMER_SHOW_INFO_TIME, NULL);
	}
	return 1;
}

// 1. 检查程序是否有得更新
void CClient02Dlg::CheckForUpdate()
{
	CString strCommand;
	strCommand.Format("%s", AfxGetApp()->m_lpCmdLine);
//	AfxMessageBox(strCommand);
	if (-1 != strCommand.Find(_T("-update")))
	{
	//	AfxMessageBox("asdf");
		return;
	}

	// 检查用户设置文件
	char szFileName[_MAX_PATH];
	GetProgramDirectory(szFileName);
	strcat_s(szFileName, MAX_PATH*sizeof(char), "xver.ini");

	CString strError;
	CString strOldVer;
	CString strNewVer;
	CString strUpdateDir;
	CString strFileName;

	CIniReader iniReader(szFileName);
	if (iniReader.sectionExists("ver_info"))
	{
		// 当前软件的版本号
		strOldVer = iniReader.getKeyValue("xver", "ver_info");
		// 远程更新服务器的地址
		strUpdateDir = iniReader.getKeyValue("xdir", "ver_info");
		// 要更新的软件名称
		strFileName = iniReader.getKeyValue("xfile", "ver_info");
	}
	else
	{
		iniReader.setKey("1", "xver", "ver_info");
		iniReader.setKey("", "xdir", "ver_info");
		iniReader.setKey("", "xfile", "ver_info");
	}

//	AfxMessageBox(strUpdateDir+strFileName+strOldVer);
	if (strUpdateDir.IsEmpty()
		|| strFileName.IsEmpty()
		|| strOldVer.IsEmpty() )
		return;

	// 检查用户设置文件
//	char szFileName[_MAX_PATH];
	ZeroMemory(szFileName, _MAX_PATH);
	GetProgramDirectory(szFileName);
//	strcat(szFileName, m_strFileName);

	CString strRemote;
	strRemote.Format(_T("%s\\%s"), strUpdateDir, _T("xver.ini"));

	CIniReader iniRemote(strRemote);
	if (iniRemote.sectionExists("ver_info"))
	{
		// 当前软件的远程版本号
		strNewVer = iniRemote.getKeyValue("xver", "ver_info");
	}

	DWORD dwNewVer = atoi((LPCTSTR)strNewVer);
	DWORD dwOldVer = atoi((LPCTSTR)strOldVer);


	if (dwNewVer > dwOldVer)
	{
		iniReader.setKey(strNewVer, "xver", "ver_info");
	//	AfxMessageBox("有新版本");
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );

		// Start the child process. 
		if( !CreateProcess( NULL, // No module name (use command line)
			_T("xUpdate.exe"), // Command line
			NULL, // Process handle not inheritable
			NULL, // Thread handle not inheritable
			FALSE, // Set handle inheritance to FALSE
			0, // No creation flags
			NULL, // Use parent's environment block
			NULL, // Use parent's starting directory 
			&si, // Pointer to STARTUPINFO structure
			&pi) // Pointer to PROCESS_INFORMATION structure
			)
		{
			AfxMessageBox(_T("启动更新程序失败！")/* "CreateProcess failed (%d).\n", GetLastError() */);
			return;
		}

		// Wait until child process exits.
	//	WaitForSingleObject( pi.hProcess, INFINITE );

		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
		ExitProcess(-1);
	}
//	AfxMessageBox("无新版本");
}

// 1. 注销程序，当前解决方案是退出并重启程序
void CClient02Dlg::OnMenuitemLogout() 
{
	if (IDYES == AfxMessageBox(_T("确定要注销吗？"), MB_YESNO|MB_ICONQUESTION))
	{
		ShowWindow(SW_HIDE);
		char szFileName[_MAX_PATH];
		::GetModuleFileName(NULL, szFileName, _MAX_PATH);

		char szCommand[_MAX_PATH];
		sprintf_s(szCommand, MAX_PATH*sizeof(char),_T("%s %s"), szFileName, _T("-update"));
		PROCESS_INFORMATION piProcInfo; 
		STARTUPINFO siStartInfo;
		// Set up members of STARTUPINFO structure.
		ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
		siStartInfo.cb = sizeof(STARTUPINFO); 
		siStartInfo.lpReserved = NULL;
		siStartInfo.lpReserved2 = NULL; 
		siStartInfo.cbReserved2 = 0;
		siStartInfo.lpDesktop = NULL; 
		siStartInfo.dwFlags = 0;
		
		// Create the child process.
		CreateProcess(
			NULL,
			szCommand,
			NULL, // process security attributes
			NULL, // primary thread security attributes
			0, // handles are inherited
			0, // creation flags
			NULL, // use parent's environment
			NULL, // use parent's current directory
			&siStartInfo, // STARTUPINFO pointer
			&piProcInfo); // receives PROCESS_INFORMATION

		CloseHandle(piProcInfo.hThread);
		CloseHandle(piProcInfo.hProcess);

		m_bLogedin = FALSE;
		OnCancel();
	}
}


void CClient02Dlg::OnQuit()
{
	// TODO: 在此添加命令处理程序代码
	OnCancel();
}

void CClient02Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	

	CDialog::OnMouseMove(nFlags, point);
}


void CClient02Dlg::OnHelp()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlg;
	dlg.DoModal();
}
