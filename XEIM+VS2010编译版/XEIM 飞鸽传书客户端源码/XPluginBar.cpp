/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// XPluginBar.cpp : implementation file
//

#include "stdafx.h"
#include "client02.h"
#include "XPluginBar.h"
#include "client02Dlg.h"
#include "xfun.h"
#include "ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXPluginBar dialog


CXPluginBar::CXPluginBar(CWnd* pParent /*=NULL*/)
	: CDialog(CXPluginBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXPluginBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwCurSel = -1;
	m_dwBtnIdx = 300;
}


void CXPluginBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXPluginBar)
	DDX_Control(pDX, IDC_PLUGIN, m_ctrlPluginBar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXPluginBar, CDialog)
	//{{AFX_MSG_MAP(CXPluginBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXPluginBar message handlers

BOOL CXPluginBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	AddPlugIn();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXPluginBar::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CXPluginBar::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CXPluginBar::AddPlugIn()
{
	char buf[MAX_PATH];
	GetProgramDirectory(buf);
	strcat_s(buf, MAX_PATH*sizeof(char),"plugin\\");

	char path[MAX_PATH];
	strcpy_s(path, MAX_PATH*sizeof(char),buf);
	strcat_s(path, MAX_PATH*sizeof(char),"*.ini");

	int   i=0;
	CFileFind findINI;
	CString strDisplay;

	if (findINI.FindFile(path))
	{
		while(findINI.FindNextFile())
		{
			if (! findINI.IsDirectory())
			{
				strDisplay = findINI.GetFilePath();
				Read_ini_Info(strDisplay, buf);
				i++;
			}   
		}
		if (! findINI.IsDirectory())
		{
			strDisplay = findINI.GetFilePath();
			Read_ini_Info(strDisplay, buf);
		}
	}
}

void CXPluginBar::Read_ini_Info(LPCTSTR lpszINI, LPCTSTR lpszPath)
{
	CString strAppName;
	CString strAppDir;
	CString strAppIcon;
	CString strAppVer;
	CString strAppAbout;

	CIniReader iniReader(lpszINI);
	if (iniReader.sectionExists("PLUGIN_INFO"))
	{
		strAppName = iniReader.getKeyValue("appname", "PLUGIN_INFO");
		strAppDir = iniReader.getKeyValue("appboot", "PLUGIN_INFO");
		strAppIcon = iniReader.getKeyValue("appicon", "PLUGIN_INFO");
		strAppVer = iniReader.getKeyValue("appvers", "PLUGIN_INFO");
		strAppAbout = iniReader.getKeyValue("appabout", "PLUGIN_INFO");
	}

	if (strAppDir.IsEmpty())
		return;

	CString strApp = lpszPath + strAppDir;

	m_dwBtnIdx ++;
	m_mapPlugin.insert(pair<DWORD, string>(m_dwBtnIdx, (LPCTSTR)strApp));
	
	SHFILEINFO shfi;
	SHGetFileInfo(strApp,FILE_ATTRIBUTE_NORMAL,&shfi,
		sizeof(shfi),SHGFI_ICON|SHGFI_SMALLICON|SHGFI_USEFILEATTRIBUTES);

/*	SHFILEINFO sfi;
	ZeroMemory(&sfi,sizeof(SHFILEINFO));
	SHGetFileInfo(strApp,0,&sfi,sizeof(SHFILEINFO),SHGFI_ICON);*/
//	g_icon = sfi.hIcon;
/*	if (NULL != sfi.hIcon)
	{
		m_ctrlPluginBar.AddButton(sfi.hIcon,m_dwBtnIdx,strAppName);
	}*/

	if (NULL != shfi.hIcon)
	{
		m_ctrlPluginBar.AddButton(shfi.hIcon,m_dwBtnIdx,strAppName);
	}
	else
	{
	}
}

BOOL CXPluginBar::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	int nCmd = (int)wParam;
	if (nCmd <= (int)m_dwBtnIdx && nCmd > 300)
	{
		CClient02Dlg *pDlg = (CClient02Dlg*)AfxGetMainWnd();
	//	m_ctrlPluginBar.EnableButton(m_dwCurSel,TRUE);
	//	m_dwCurSel = nCmd-300;
	//	m_ctrlPluginBar.EnableButton(m_dwCurSel,FALSE);
	//	AfxMessageBox(m_mapPlugin[nCmd].c_str());
		CString strCommand;
		CString strApp;
		strApp = m_mapPlugin[nCmd].c_str();
		if (-1 != strApp.Find("schedule"))
		{
			strCommand.Format(_T("%s %s %s"), strApp, pDlg->m_strUID, pDlg->m_strUPSW);
		}
		else
		{
			strCommand = strApp;
		}

	//	WinExec(strCommand, SW_SHOW);

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
		(LPSTR)(LPCTSTR)strCommand,
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
		m_listThreadId.push_back(piProcInfo.dwProcessId);
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CXPluginBar::CloseAllProgram()
{
	list<DWORD>::iterator it = m_listThreadId.begin();
	for (; it != m_listThreadId.end(); ++it)
	{
	//	PostThreadMessage(*it, WM_CLOSE, 0, 0);
		//Enumerate all visible processes and add them to the listbox
	//	CString str;
	//	str.Format("%d ----", *it);
	//	AfxMessageBox(str);
		EnumWindows (EnumWindowsProc, (LPARAM)(*it)/*pointer to the dialog box*/);
	}

	m_mapPlugin.clear();
	m_listThreadId.clear();
}



BOOL CALLBACK CXPluginBar::EnumWindowsProc (HWND hWnd, LPARAM lParam)
{
//	CXPluginBar *pDlg = (CXPluginBar *)lParam;
//	int nItem = 0;

	//Make sure that the window is visible
	TCHAR szWindowText [MAX_PATH];
	if (!::IsWindowVisible (hWnd))
		return TRUE;

	//Get the text on the title bar
	::GetWindowText (hWnd, szWindowText, MAX_PATH);

	//If the window is Process Manager than don't display it
/*	if (_tcsstr (_T("Program Manager"), szWindowText))
		return TRUE;*/

/*	DWORD dwTID;
	GetWindowThreadProcessId(hWnd, &dwTID);*/
//	CString strTID;
//	strTID.Format("%d", dwTID);
	//Get process ID
	DWORD dwProcessID;
	GetWindowThreadProcessId (hWnd, &dwProcessID);
	if (dwProcessID == (DWORD)lParam)
	{
	//	AfxMessageBox("asdfasdf");
		::PostMessage(hWnd, WM_CLOSE, 0, 0);
	}

/*	CString stt;
	stt.Format("%d %s", dwProcessID, szWindowText);
	AfxMessageBox(stt);*/

	//Get the name of the executable file
/*	CString strEXEName = pDlg->GetEXEName (dwProcessID);
*/
	//Add the info to the list control
/*	nItem = pDlg->m_List.InsertItem (0, szWindowText);
	pDlg->m_List.SetItemText (nItem, 1, strEXEName);
	pDlg->m_List.SetItemText(nItem, 2, strTID);
*/
	return TRUE;
}