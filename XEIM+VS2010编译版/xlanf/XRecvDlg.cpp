// XRecvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xlanf.h"
#include "XRecvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXRecvDlg dialog

CXRecvDlg::CXRecvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXRecvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXRecvDlg)
	//}}AFX_DATA_INIT
	m_dwID = 0;
	m_dwHeight = 55;
	m_dwFileLength = 0;
	m_dwRecvLength = 0;

	m_bRunning = FALSE;
	m_sockListen = INVALID_SOCKET;
	xxxtmp = 100;
}


void CXRecvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXRecvDlg)
	DDX_Control(pDX, IDC_STATIC_SAVEAS, m_staticSaveAs);
	DDX_Control(pDX, IDC_STATIC_SAVE, m_staticSave);
	DDX_Control(pDX, IDC_STATIC_REFUSE, m_staticRefuse);
	DDX_Control(pDX, IDC_STATIC_CANCEL, m_staticCancel);
	DDX_Control(pDX, IDC_PROGRESS1, m_process);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXRecvDlg, CDialog)
	//{{AFX_MSG_MAP(CXRecvDlg)
	ON_WM_TIMER()
	ON_MESSAGE(_HYPERLINK_EVENT,OnChildFire)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_DEST_CANCEL, OnDestCancel)
	ON_MESSAGE(WM_RECV_CANCEL, OnRecvCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXRecvDlg message handlers

void CXRecvDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::PostNcDestroy();
	delete this;
}

void CXRecvDlg::AdjustWindow(DWORD dwDlgID)
{
//	AfxMessageBox("CXRecvDlg Adjust Wnd");
	if (m_dwID > dwDlgID)
	{
		m_dwID = dwDlgID;
		SetTimer(28, 100+m_dwID*100, NULL);
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

void CXRecvDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case 55:
		{
			float fFileLen = m_dwFileLength;
			float fSentLen = m_dwRecvLength;
			float fPercent = fSentLen/fFileLen * 100;
			CString str;
			str.Format("%d%%", (int)fPercent);
			m_process.SetPos((int)fPercent);

			SetDlgItemText(IDC_STATIC8, str);

			float filelen = float(m_dwFileLength);
			filelen /= 1024;
			filelen /= 1024;
			float sentlen = float(m_dwRecvLength);
			sentlen /= 1024;
			sentlen /= 1024;
			str.Format("%.2fMB/%.2fMB", sentlen, filelen);
			SetDlgItemText(IDC_STATIC6, str);

			if (m_dwRecvLength == m_dwFileLength)
			{
				KillTimer(55);
				SetTimer(88, 1000, NULL);
			//	AfxMessageBox("asdfasdf");
			}

		}
		break;

	case 88:
		{
			KillTimer(nIDEvent);

			GetParent()->SendMessage(WM_FILERECVEND, m_dwID);
			DestroyWindow();
		}
		break;

	case 28:
		{
			KillTimer(nIDEvent);

			RECT rt;
			rt.top = m_dwHeight * m_dwID + 38;
			rt.bottom = m_dwHeight + rt.top;
			rt.left = 0;
			rt.right = 170;
			MoveWindow(&rt);

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

BOOL CXRecvDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_brBkColor.CreateSolidBrush(RGB(236, 243, 246));

	m_staticRefuse.SetFireChild(TRUE);
	m_staticSave.SetFireChild(TRUE);
	m_staticSaveAs.SetFireChild(TRUE);
	m_staticCancel.SetFireChild(TRUE);

	m_staticCancel.ShowWindow(SW_HIDE);

	m_process.ShowWindow(SW_HIDE);

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// 直接保存功能暂时没做好
	m_staticSave.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 接受文件，然后等待用户确认是否接收 [保存] [另存] [拒收]
void CXRecvDlg::AddFile(LPXFILEINFO pFileInfo)
{
	memcpy((char*)&m_xFileInfo, pFileInfo, sizeof(XFILEINFO));

//	CString str;
//	str.Format("%d", m_xFileInfo.dwPort);
//	AfxMessageBox(str);
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
}

LRESULT CXRecvDlg::OnChildFire(WPARAM wparam,LPARAM lparam)
{
	m_staticRefuse.ShowWindow(SW_HIDE);
	m_staticSave.ShowWindow(SW_HIDE);
	m_staticSaveAs.ShowWindow(SW_HIDE);

	switch ((int)wparam)
	{
	case IDC_STATIC_SAVEAS:
		{
			char *szFilePath = FileSaveAs();
			if (szFilePath) // return NULL
			{
				SetTimer(55, 100, NULL);
				m_staticCancel.ShowWindow(SW_SHOW);
				delete szFilePath;
			}
			else
			{
				m_staticRefuse.ShowWindow(SW_SHOW);
				m_staticSave.ShowWindow(SW_SHOW);
				m_staticSaveAs.ShowWindow(SW_SHOW);
			}
			return 1;
		}
	case IDC_STATIC_SAVE:
		{
	//		m_staticCancel.ShowWindow(SW_SHOW);
	//		SetTimer(55, 100, NULL);
			return 1;
		}
	case IDC_STATIC_REFUSE:
		{
			m_staticRefuse.ShowWindow(SW_HIDE);
			m_staticSave.ShowWindow(SW_HIDE);
			m_staticSaveAs.ShowWindow(SW_HIDE);
			SetDlgItemText(IDC_STATIC10, "文件已拒收...");
			SendRefuseMsg();
			SetTimer(88, 1000, NULL);
			return 1;
		}
	case IDC_STATIC_CANCEL:
		{
	//		m_staticCancel.ShowWindow(SW_HIDE);
	//		m_bRunning = FALSE;
			return 1;
		}
	}

	return 0;
}

HBRUSH CXRecvDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
					pDC->SetTextColor(RGB(99,97,78));
					pDC->SetBkColor(RGB(236, 243, 246));
					return (HBRUSH)m_brBkColor;
				//	pDC->SetBkMode(TRANSPARENT);
				//	return (HBRUSH)::GetStockObject(NULL_BRUSH);
				}

			case IDC_STATIC_ICON:
				return (HBRUSH)m_brBkColor.GetSafeHandle();

			case IDC_FILE_NAME:
				{
					pDC->SetBkColor(RGB(236, 243, 246));    // change the background
					pDC->SetTextColor(RGB(99,97,78));  // change the text color
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

LPSTR CXRecvDlg::FileSaveAs()
{
	char *szBuf = new char[MAX_PATH];

	CString strFileExt;
	strFileExt.Format("*.%s|*.%s|All Files(*.*)|*.*||", m_xFileInfo.szExt, m_xFileInfo.szExt);
	CFileDialog dlgFileSaveAs(FALSE, NULL, m_xFileInfo.szShortName,
	OFN_HIDEREADONLY |
	OFN_OVERWRITEPROMPT,
	strFileExt,this);

	int structsize=0;
	DWORD dwVersion,dwWindowsMajorVersion,dwWindowsMinorVersion;
	//检测目前的操作系统，GetVersion具体用法详见MSDN
	dwVersion = GetVersion();
	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	// 如果运行的操作系统是Windows NT/2000
	if (dwVersion < 0x80000000)
	structsize =88;//显示新的文件对话框
	else
	//运行的操作系统Windows 95/98
	structsize =76;//显示老的文件对话框
	dlgFileSaveAs.m_ofn.lStructSize=structsize;

	// 按了保存
	if (IDOK == dlgFileSaveAs.DoModal())
	{
		strcpy(szBuf, dlgFileSaveAs.GetPathName());
		LPXTCP_RecvData pData = new XTCP_RecvData;
		pData->dwPort = m_xFileInfo.dwPort;
		strcpy(pData->szSaveFileName, dlgFileSaveAs.GetPathName());
		strcpy(m_xFileInfo.szFullName, dlgFileSaveAs.GetPathName());
		pData->hWnd = m_hWnd;

		// 线程创建
		DWORD tId;
		HANDLE hThread = CreateThread(NULL, 0, ProcRecvFile, this/*pData*/, 0, &tId);
		if (hThread == 0)
		{
			::MessageBox(NULL, "线程创建错误！", "Error", MB_OK);
			return 0;
		}
		CloseHandle(hThread);

		GetDlgItem(IDC_STATIC10)->ShowWindow(SW_HIDE);
		m_process.ShowWindow(SW_SHOW);

		return szBuf;
	}
	return NULL;

}

//////////////////////////////////////////////////////////////////////////

// 文件接收回调函数
// return 0 之前，可能没释放内存 pData
DWORD WINAPI CXRecvDlg::ProcRecvFile(LPVOID lParam)
{
	CXRecvDlg *pDlg = (CXRecvDlg*)lParam;
//	LPXTCP_RecvData pData = (LPXTCP_RecvData)lParam;

	pDlg->m_sockListen = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == pDlg->m_sockListen)
	{
		// 其他错误处理
		CString str;
		str.Format("%d", WSAGetLastError());
		::MessageBox(NULL, str, "Error", MB_OK);
		return 0;
	} // end if (INVALID_SOCKET == sock)

	sockaddr_in sin;
	sin.sin_addr.s_addr = inet_addr(pDlg->m_xFileInfo.szUIP);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(pDlg->m_xFileInfo.dwPort);
	int len = sizeof(sin);

	CFile file;
	CFileException e;
	file.Open(pDlg->m_xFileInfo.szFullName, CFile::modeWrite|CFile::modeCreate|CFile::shareExclusive, &e);
	if (e.m_cause != 0)
	{
		char *str = new char[1024];
		e.GetErrorMessage(str, 1024, NULL);
		::MessageBox(NULL, str, "Error", MB_OK);
		return 0;
	}

	int ret = connect(pDlg->m_sockListen, (PSOCKADDR)&sin, len);
	if (SOCKET_ERROR == ret)
	{
		int error = WSAGetLastError();
		CString str;
		str.Format("connect failed:%d", error);
		::MessageBox(NULL, str, "Error", MB_OK);
		return 0;
	}

	// 1 接收文件信息
	XTCP_FileInfo xInfo;
	ret = recv(pDlg->m_sockListen, (char*)&xInfo, sizeof(XTCP_FileInfo), 0);
	if (ret == SOCKET_ERROR )
	{
		CString err;
		err.Format("接收文件信息 recv failed:%d", WSAGetLastError());
		::MessageBox(NULL, err, "Error", MB_OK);
		return 0;
	}

	int nTotal = 0;
	char szBuf[1024];
	pDlg->m_dwFileLength = xInfo.nFileLength;
	pDlg->m_bRunning = TRUE;
	while ((nTotal < xInfo.nFileLength) && pDlg->m_bRunning)
	{
		ret = recv(pDlg->m_sockListen, szBuf, 1024, 0);
		if (SOCKET_ERROR == ret)
		{
			CString str;
			str.Format("%d", WSAGetLastError());
			AfxMessageBox(str);
			break;
		}
		if (ret == 0)
		{
			pDlg->PostMessage(WM_DEST_CANCEL, 0, 0);
			break;
		}
		file.Write(szBuf, ret);
		nTotal += ret;
		pDlg->m_dwRecvLength = nTotal;
	}

	if (! pDlg->m_bRunning)
	{
		pDlg->PostMessage(WM_RECV_CANCEL);
	}
	closesocket(pDlg->m_sockListen);
	file.Close();

	// 3 通知主窗口文件发送成功
//	Sleep(2000);
	return 0;
}

LRESULT CXRecvDlg::OnRecvCancel(WPARAM wParam, LPARAM lParam)
{
//	SetTimer(88, 1000, NULL);
	AfxMessageBox("CXRecvDlg::OnRecvCancel");

	return 0;
}
//////////////////////////////////////////////////////////////////////////

void CXRecvDlg::CancelFile(DWORD dwPort)
{
	if (dwPort == m_xFileInfo.dwPort)
	{
		m_bRunning = FALSE;
		m_staticRefuse.ShowWindow(SW_HIDE);
		m_staticSave.ShowWindow(SW_HIDE);
		m_staticSaveAs.ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_STATIC10, "已取消发送...");
		SetTimer(88, 1000, NULL);
	}
}

void CXRecvDlg::SendRefuseMsg()
{
//	HWND hChatDlg = ::GetParent(GetParent()->m_hWnd);
//	AfxMessageBox(pInfo->szFullName);
//	::PostMessage(hChatDlg, WM_REFUSE_FILE, m_xFileInfo.dwPort, NULL);
//	AfxMessageBox(m_xFileInfo.szFullName);
	m_xFileInfo.option = FILE_REFUSE;
	XMSG xmsg;
	xmsg.m_nMessage = /*XEIMMSG_MESSAGE;*/XEIMMSG_NEWFILE;
	XMSG_ONLINE omsg;
	omsg.SetData(m_xFileInfo.szUID, (char*)&m_xFileInfo, sizeof (XFILEINFO));
	xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());

//	AfxMessageBox((char*)lParam);
	send(m_xFileInfo.sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
}

LRESULT CXRecvDlg::OnDestCancel(WPARAM wParam, LPARAM lParam)
{
	m_bRunning = FALSE;
	m_staticCancel.ShowWindow(SW_HIDE);
	m_process.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC10)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC8)->ShowWindow(SW_HIDE);
	SetDlgItemText(IDC_STATIC10, "对方终止发送...");
	SetTimer(88, 1000, NULL);

	return 0;
}