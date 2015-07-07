// xlanf.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "xlanf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CXlanfApp

BEGIN_MESSAGE_MAP(CXlanfApp, CWinApp)
	//{{AFX_MSG_MAP(CXlanfApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXlanfApp construction

CXlanfApp::CXlanfApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bInitSocket = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CXlanfApp object

CXlanfApp theApp;

// Winsock 初始化
BOOL CXlanfApp::InitWinSock()
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock)
	{
		int lastError = WSAGetLastError();
		if (WSANOTINITIALISED == lastError)
		{
			WSADATA data;
			WSAStartup(0x0202, &data);
			m_bInitSocket = TRUE;
			return TRUE;
		}
	}
	else
	{
		closesocket(sock);
		return TRUE;
	}

	return FALSE;
}

// 反初始化 Winsock
void CXlanfApp::UninitWinSock()
{
	if (m_bInitSocket)
	{
		WSACleanup();
//		MessageBox(NULL, "Uninit", "", MB_OK);
	}
}

// 增加窗口宽度
void CXlanfApp::AddWndSize(HWND hWnd, int nSize)
{
	::PostMessage(hWnd, XM_SENDFILEADD, NULL, NULL);
	return;
	RECT rtWnd;
	::GetWindowRect(hWnd, &rtWnd);
	RECT rtMove;
	rtMove.left = rtWnd.left;
	rtMove.top = rtWnd.top;
	rtMove.right = rtWnd.right - rtWnd.left;
	rtMove.bottom = rtWnd.bottom - rtWnd.top;
	::MoveWindow(hWnd, rtWnd.left, rtWnd.top, rtMove.right+nSize, rtMove.bottom, TRUE);
}

CXFrame *CXlanfApp::InitFrame(HWND hParent)
{
	if (! InitWinSock())
	{
		MessageBox(hParent, "winsock 初始化失败", "网络错误", MB_OK|MB_ICONWARNING);
		return NULL;
	}

	if (IsNewInstance(hParent))
	{
		AddWndSize(hParent, 180);

		CXFrame *pFrame = new CXFrame;
		CWnd *pWnd = CWnd::FromHandle(hParent);
		pFrame->Create(CXFrame::IDD, pWnd);
		pFrame->m_hParent = hParent;

		RECT rt;
		GetClientRect(hParent, &rt);
		if (::IsWindow(pFrame->GetSafeHwnd()))
		{
			pFrame->MoveWindow(rt.right-180, 10, 170, rt.bottom-20, TRUE);
			pFrame->ShowWindow(SW_SHOW);
			pFrame->UpdateWindow();
			m_mapInstance.insert(pair<HWND, CXFrame*>(hParent, pFrame));

			return pFrame;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		map<HWND, CXFrame*>::iterator it;
		it = m_mapInstance.find(hParent);
		if (it != m_mapInstance.end())
		{
			return it->second;
		}
		else
			return NULL;
	}
}

BOOL CXlanfApp::IsNewInstance(HWND hWnd)
{
	map<HWND, CXFrame*>::iterator it;
	it = m_mapInstance.find(hWnd);

	BOOL bNew;
	bNew = (it == m_mapInstance.end());

	return bNew;
}

void CXlanfApp::UninitFrame(HWND hWnd)
{
	UninitWinSock();

	map<HWND, CXFrame*>::iterator it;
	it = m_mapInstance.find(hWnd);
	if (it != m_mapInstance.end())
	{
		m_mapInstance.erase(it);
	}
	SendMessage(hWnd, WM_FRAME_END, NULL, NULL);
}

extern "C" __declspec(dllexport)
HWND AddFile(HWND hWnd, LPCTSTR szFullName, LPCTSTR szShortName, LPCTSTR szExt, BOOL bSend)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CXFrame *frame = theApp.InitFrame(hWnd);

	XFILEINFO xFile;
	strcpy(xFile.szShortName, szShortName);
	strcpy(xFile.szFullName, szFullName);
	strcpy(xFile.szExt, szExt);

	if (bSend)
		frame->SendFile(& xFile);
	else
		frame->RecvFile(& xFile);

	return frame->GetSafeHwnd();
}

extern "C" __declspec(dllexport)
HWND AddFile1(HWND hWnd, LPXFILEINFO pInfo, BOOL bSend)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CXFrame *frame = theApp.InitFrame(hWnd);

//	XFILEINFO xFile;
//	strcpy(xFile.szShortName, szShortName);
//	strcpy(xFile.szFullName, szFullName);
//	strcpy(xFile.szExt, szExt);

/*	if (bSend)
		frame->SendFile(pInfo);
	else
		frame->RecvFile(pInfo);
*/
	return frame->GetSafeHwnd();
}

// 打开要发送的文件
BOOL XOpenFile(LPXFILEINFO pInfo)
{
	// TODO: Add your control notification handler code here
	// 模拟发送网络消息
//	PostMessage(WM_SEND_FILE, NULL, NULL);
	// 文件打开操作
	CFileDialog dlgFileOpen(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
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
	dlgFileOpen.m_ofn.lStructSize=structsize;
	TCHAR lpstrFilename[MAX_PATH] = _T("");
	dlgFileOpen.m_ofn.lpstrFile=lpstrFilename;

	if(dlgFileOpen.DoModal()==IDOK)
	{
		// Add file to fileinfo list.
	//	XFILEINFO fInfo;
	//	strcpy(fInfo.szFullName, "c:\\windows\\notepad.exe");Outlook.pst
		strcpy(pInfo->szFullName, dlgFileOpen.GetPathName());
		strcpy(pInfo->szShortName, dlgFileOpen.GetFileName());
		strcpy(pInfo->szExt, dlgFileOpen.GetFileExt());
		return TRUE;
	}
/*	XFILEINFO fInfo;
	strcpy(fInfo.szFullName, "c:\\windows\\notepad.exe");
	strcpy(fInfo.szShortName, "notepad.exe");
	strcpy(fInfo.szExt, "exe");*/

//	m_hFrame = AddFile(m_hWnd, "c:\\windows\\notepad.exe", "notepad.exe", "exe", TRUE);

	return FALSE;
}

// 给主程序调用的接口，用于发送文件
extern "C" __declspec(dllexport)
HWND SendFile(HWND hParent, SOCKET sock, LPSTR szUID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	XFILEINFO fInfo;
	if (XOpenFile(& fInfo))
	{
		CXFrame *frame = theApp.InitFrame(hParent);
		fInfo.sock = sock;
		strcpy(fInfo.szUID, szUID);
		frame->SendFile(& fInfo);
	//	AfxMessageBox("asdfafd");
		return frame->GetSafeHwnd();
	}
	else
	{
		return FALSE;
	}
}


extern "C" __declspec(dllexport)
HWND RecvFile(HWND hParent, LPXFILEINFO pInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

//if (AfxMessageBox("File reced.", MB_YESNO) == IDYES)
//{
//	Sleep(1000);
	CXFrame *frame = theApp.InitFrame(hParent);
//	
//	if (frame->GetSafeHwnd())
	if (frame == NULL)
	{
		AfxMessageBox("bADLY ERROR");
		return NULL;
	}
	frame->RecvFile(pInfo);
	return frame->GetSafeHwnd();

	return NULL;
}

BOOL CXlanfApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinApp::InitInstance();
}
