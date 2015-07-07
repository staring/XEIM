// XSendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xlanf.h"
#include "XSendDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXSendDlg dialog

CXSendDlg::CXSendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXSendDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXSendDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwID = 0;
	m_dwHeight = 55;
	m_dwFileProcess = 0;
	m_dwFileLength = 0;

	m_bRunning = FALSE;
	m_sockListen = INVALID_SOCKET;
	xxxtmp = 8888;
}


void CXSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXSendDlg)
	DDX_Control(pDX, IDC_STATIC_CANCEL, m_staticCancel);
	DDX_Control(pDX, IDC_PROGRESS1, m_process);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXSendDlg, CDialog)
	//{{AFX_MSG_MAP(CXSendDlg)
	ON_WM_TIMER()
	ON_MESSAGE(_HYPERLINK_EVENT,OnChildFire)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_DEST_ACCEPT, OnXDestAcceptFile)
	ON_MESSAGE(WM_SEND_NOTIFY, OnSendNotify)
	ON_MESSAGE(WM_SEND_CANCEL, OnSendCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXSendDlg message handlers

void CXSendDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CXSendDlg::AdjustWindow(DWORD dwDlgID)
{
	if (m_dwID > dwDlgID)
	{
		m_dwID = dwDlgID;
		SetTimer(28, 100+m_dwID*66, NULL);
	}
	else
	{
		m_dwID = dwDlgID;
		RECT rt;
		rt.top = m_dwHeight * m_dwID;
		rt.left = 0;
		rt.bottom = m_dwHeight + rt.top;
		rt.right = 170;
		MoveWindow(&rt);
	}
}

void CXSendDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	switch (nIDEvent)
	{
		case 55:// 发送进度
		{
			float fFileLen = m_dwFileLength;
			float fSentLen = m_dwFileProcess;
			float fPercent = fSentLen/fFileLen * 100;
			CString str;
			str.Format("%d%%", (int)fPercent);
			m_process.SetPos((int)fPercent);

			SetDlgItemText(IDC_STATIC8, str);

			float filelen = float(m_dwFileLength);
			filelen /= 1024;
			filelen /= 1024;
			float sentlen = float(m_dwFileProcess);
			sentlen /= 1024;
			sentlen /= 1024;
			str.Format("%.2fMB/%.2fMB", sentlen, filelen);
			SetDlgItemText(IDC_STATIC6, str);

			if (m_dwFileProcess == m_dwFileLength)
			{
				KillTimer(55);
				SetTimer(88, 1000, NULL);
			//	AfxMessageBox("asdfasdf");
			}
		}
		break;

		case 88: // 对话框关闭
		{
			GetParent()->SendMessage(WM_FILERECVEND, m_dwID);
			DestroyWindow();
		}
		break;

	case 28: // 对话框准备关闭
		{
			KillTimer(nIDEvent);
			RECT rt;
			rt.top = m_dwHeight * m_dwID + 38;
			rt.left = 0;
			rt.bottom = m_dwHeight + rt.top;
			rt.right = 170;
			MoveWindow(&rt);
			RedrawWindow();

			SetTimer(18, 100, NULL);
		}
		break;

	case 18:
		{
			KillTimer(nIDEvent);
			RECT rt;
			rt.top = m_dwHeight * m_dwID;
			rt.left = 0;
			rt.bottom = m_dwHeight + rt.top;
			rt.right = 170;
			MoveWindow(&rt);
			RedrawWindow();
		}
		break;

	default:
		CDialog::OnTimer(nIDEvent);
	}
}

BOOL CXSendDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_brBkColor.CreateSolidBrush(RGB(250, 250, 250));

//	SetTimer(55, 200, NULL);
	m_staticCancel.SetFireChild(TRUE);

	m_process.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC8)->ShowWindow(SW_HIDE);
	RedrawWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXSendDlg::AddFile(LPXFILEINFO pFileInfo)
{
//	AfxMessageBox(pFileInfo->szFullName);
	memcpy((char*)&m_xFileInfo, pFileInfo, sizeof(XFILEINFO));

	SetDlgItemText(IDC_FILE_NAME, m_xFileInfo.szShortName);

	SHFILEINFO shfi;
	SHGetFileInfo(m_xFileInfo.szFullName,FILE_ATTRIBUTE_NORMAL,&shfi,
		sizeof(shfi),SHGFI_ICON|SHGFI_USEFILEATTRIBUTES);

	if (NULL != shfi.hIcon)
	{
		GetDlgItem(IDC_STATIC_ICON)->SendMessage(STM_SETIMAGE, IMAGE_ICON, (LPARAM)shfi.hIcon);
	}
	else
	{
		MessageBox("Could not retrieve the file icon.");
	}

	WaitingForDownload(&m_xFileInfo);
}

LRESULT CXSendDlg::OnChildFire(WPARAM wparam,LPARAM lparam)
{
	if ((int)wparam == IDC_STATIC_CANCEL)
	{
		if (m_bRunning)
		{
			m_staticCancel.ShowWindow(SW_HIDE);
			m_bRunning = FALSE;
		}
		else
		{
			m_staticCancel.ShowWindow(SW_HIDE);
			SetDlgItemText(IDC_STATIC10, "正在取消发送...");
			SendCancelMsg();
			shutdown(m_sockListen, SD_BOTH);
			closesocket(m_sockListen);
			SetTimer(88, 1000, NULL);
		}
	}

	return 0;
}

HBRUSH CXSendDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	switch (nCtlColor)
	{

	case CTLCOLOR_STATIC:
		{
			switch (pWnd->GetDlgCtrlID())
			{
			case IDC_STATIC8:
			case IDC_STATIC6:
			case IDC_STATIC10:
				{
					pDC->SetTextColor(RGB(0,136,228));
					pDC->SetBkColor(RGB(250, 250, 250));
					return (HBRUSH)m_brBkColor;
				}

			case IDC_STATIC_ICON:
				return (HBRUSH)m_brBkColor.GetSafeHandle();

			case IDC_FILE_NAME:
				{
					pDC->SetBkColor(RGB(250, 250, 250));    // change the background
					pDC->SetTextColor(RGB(0,136,228));  // change the text color
					return (HBRUSH)m_brBkColor;
				}

			default:
				return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
			}
		}

	case CTLCOLOR_DLG:
		return (HBRUSH)m_brBkColor.GetSafeHandle();

	default:
		// TODO: Return a different brush if the default is not desired
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}

//////////////////////////////////////////////////////////////////////////

// 文件发送回调函数
DWORD WINAPI CXSendDlg::ProcSendFile(LPVOID lParam)
{
	CXSendDlg *pDlg = (CXSendDlg*)lParam;

//	LPXTCP_SendData pData = (LPXTCP_SendData)lParam;

	pDlg->m_sockListen = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == pDlg->m_sockListen)
	{
		// 其他错误处理
		CString str;
		str.Format("%d", WSAGetLastError());
		::MessageBox(NULL, str, "Error", MB_OK);
		return 0;
	} // end if (INVALID_SOCKET == sock)

	// 获取本地 IP
	hostent *host = gethostbyname(NULL);
	char ip[128];
	if (host != NULL)
	{
		sockaddr_in ssin;
		memcpy(& ssin.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
		strcpy(ip, inet_ntoa(ssin.sin_addr));
	}

	sockaddr_in sin;
	sin.sin_addr.s_addr = /*htonl(INADDR_ANY);*/inet_addr(ip);
	sin.sin_family = AF_INET;
	sin.sin_port =  INADDR_ANY;

	int len = sizeof(sin);
	int ret = bind(pDlg->m_sockListen, (PSOCKADDR)&sin, len);
	if (SOCKET_ERROR == ret)
	{
		::MessageBox(NULL, "bind failed.", "Error", MB_OK);
		return 0;
	}

	ret = listen(pDlg->m_sockListen, 1);
	if (SOCKET_ERROR == ret)
	{
		::MessageBox(NULL, "listen failed.\n", "Error", MB_OK);
		return 0;
	}

	// 把我随机绑定的端口告诉主程序
	len = sizeof(sin);
	getsockname(pDlg->m_sockListen, (PSOCKADDR)&sin, &len);//	AfxMessageBox(inet_ntoa(sin.sin_addr)); // 这时绑定的 IP 地址
	DWORD dwPort = ntohs(sin.sin_port);//	::PostMessage(pData->hWnd, X_WM_TCP_FILE_BIND_OK, (WPARAM)dwPort, NULL);

	::PostMessage(pDlg->m_hWnd, WM_SEND_NOTIFY, (WPARAM)dwPort, (LPARAM)ip);


	sockaddr_in sout;
	int oLen = sizeof(sout);
	SOCKET sAccept = accept(pDlg->m_sockListen, (PSOCKADDR)&sout, &oLen);
	if (sAccept == INVALID_SOCKET)
	{
		if (WSAGetLastError() == 10004)
		{
		}
		else
		{
			CString str;
			str.Format("accept failed %d", WSAGetLastError());
			::MessageBox(NULL, str, "Error", MB_OK);
		}
		return 0;
	}

	CFile file;
	CFileException e;
	file.Open(pDlg->m_xFileInfo.szFullName, CFile::modeRead|CFile::shareDenyNone/*CFile::shareExclusive*/, &e);
	if (e.m_cause != 0)
	{
		char *str = new char[1024];
		e.GetErrorMessage(str, 1024, NULL);
		::MessageBox(NULL, str, "Error", MB_OK);
		return 0;
	}

	::PostMessage(pDlg->m_hWnd, WM_DEST_ACCEPT, NULL, NULL);
	pDlg->m_bRunning = TRUE;
	// 1 发送文件信息
	XTCP_FileInfo fInfo;
	strcpy(fInfo.szFileName, file.GetFileName());
	fInfo.nFileLength = file.GetLength();
	pDlg->m_dwFileLength = fInfo.nFileLength;

	ret = send(sAccept, (char*)&fInfo, sizeof(fInfo), 0);
	if (SOCKET_ERROR == ret)
	{
		::MessageBox(NULL, "发送文件信息 send failed.", "Error", MB_OK);
		return 0;
	}

	// 2 发送文件数据
	char szBuf[1024];
	int nReadLen = 0;
	while(pDlg->m_bRunning)
	{
		nReadLen = file.Read(szBuf, 1024);
		ret = send(sAccept, szBuf, nReadLen, 0);
		if (SOCKET_ERROR == ret)
		{
		//	::MessageBox(NULL, "send failed.", "Error", MB_OK);
			pDlg->m_bRunning = FALSE;
			break;
		}
		pDlg->m_dwFileProcess += ret;
//		::PostMessage(pData->hWnd, WM_SEND_PROCESS, nReadLen, fInfo.nFileLength);
		if (nReadLen < 1024)
			break;
	}

	if (! pDlg->m_bRunning)
		pDlg->PostMessage(WM_SEND_CANCEL, 0, 0);
	shutdown(sAccept, SD_BOTH);
	closesocket(sAccept);
	closesocket(pDlg->m_sockListen);

	file.Close();

	// 3 通知主窗口文件接收完成
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CXSendDlg::WaitingForDownload(LPXFILEINFO pFileInfo)
{
	LPXTCP_SendData pData = new XTCP_SendData;
	strcpy(pData->szSendFileName, pFileInfo->szFullName);
	pData->hWnd = m_hWnd;

	// 线程创建
	DWORD tId;
	HANDLE hThread = CreateThread(NULL, 0, ProcSendFile, this, 0, &tId);
	if (hThread == 0)
	{
		::MessageBox(NULL, "线程创建错误！", "Error", MB_OK);
		return;
	}

	CloseHandle(hThread);
}

LRESULT CXSendDlg::OnXDestAcceptFile(WPARAM wParam, LPARAM lParam)
{
//	AfxMessageBox("WM_DEST_ACCEPT");
	GetDlgItem(IDC_STATIC8)->ShowWindow(SW_SHOW);
	m_process.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC10)->ShowWindow(SW_HIDE);

	SetTimer(55, 200, NULL);

	return 0;
}

LRESULT CXSendDlg::OnFileProcess(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CXSendDlg::OnSendNotify(WPARAM wParam, LPARAM lParam)
{
	m_xFileInfo.dwPort = (DWORD)wParam;
	strcpy(m_xFileInfo.szUIP, (char*)lParam);

	XMSG xmsg;
	xmsg.m_nMessage = /*XEIMMSG_MESSAGE;*/XEIMMSG_NEWFILE;
	XMSG_ONLINE omsg;
	omsg.SetData(m_xFileInfo.szUID, (char*)&m_xFileInfo, sizeof (XFILEINFO));
	xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());

//	AfxMessageBox((char*)lParam);
	send(m_xFileInfo.sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
//	SendMsgToDest(&m_xFileInfo);

	return 0;
}

LRESULT CXSendDlg::OnSendCancel(WPARAM wParam, LPARAM lParam)
{
//	AfxMessageBox("发送取消");
	SetTimer(88, 1000, NULL);

	return 0;
}

void CXSendDlg::SendCancelMsg()
{
	// 模拟实际流程
//	LPXFILEINFO pInfo = new XFILEINFO;
//	memcpy(pInfo, (char*)&m_xFileInfo, sizeof(XFILEINFO));
//	HWND hChatDlg = ::GetParent(GetParent()->m_hWnd);
//	AfxMessageBox(pInfo->szFullName);
//	::PostMessage(hChatDlg, WM_SEND_CANCEL, (WPARAM)pInfo, NULL);
//	::PostMessage(WM_SEND_CANCEL, 0, 0);
	m_xFileInfo.option = FILE_CANCEL;
	XMSG xmsg;
	xmsg.m_nMessage = /*XEIMMSG_MESSAGE;*/XEIMMSG_NEWFILE;
	XMSG_ONLINE omsg;
	omsg.SetData(m_xFileInfo.szUID, (char*)&m_xFileInfo, sizeof (XFILEINFO));
	xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());

//	AfxMessageBox((char*)lParam);
	send(m_xFileInfo.sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
}

void CXSendDlg::RefuseFile(DWORD dwPort)
{
//	AfxMessageBox("Hello.");
	if (dwPort == m_xFileInfo.dwPort)
	{
		m_bRunning = FALSE;
		m_process.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC8)->ShowWindow(SW_HIDE);
		m_staticCancel.ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_STATIC10, "对方拒收文件...");
		SetTimer(88, 1000, NULL);
	}
}
