// GeneralDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xsetting.h"
#include "GeneralDlg.h"
#include "INI.h"
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralDlg dialog

CGeneralDlg::CGeneralDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneralDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGeneralDlg)
	m_bAutoRun = FALSE;
	m_bRemember = FALSE;
	m_bAutoLog = FALSE;
	//}}AFX_DATA_INIT

}


void CGeneralDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralDlg)
	DDX_Check(pDX, IDC_CHECK1, m_bAutoRun);
	DDX_Check(pDX, IDC_CHECK2, m_bRemember);
	DDX_Check(pDX, IDC_CHECK3, m_bAutoLog);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_HOTKEY, m_HotKey);
}


BEGIN_MESSAGE_MAP(CGeneralDlg, CDialog)
	//{{AFX_MSG_MAP(CGeneralDlg)
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS(IDC_EDIT_HOTKEY, &CGeneralDlg::OnEnSetfocusEditHotkey)
	ON_EN_KILLFOCUS(IDC_EDIT_HOTKEY, &CGeneralDlg::OnEnKillfocusEditHotkey)
	ON_EN_CHANGE(IDC_EDIT_HOTKEY, &CGeneralDlg::OnEnChangeEditHotkey)
	ON_WM_CHAR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralDlg message handlers

void CGeneralDlg::UpdateIni()
{
	UpdateData(TRUE);

	char szFileName[_MAX_PATH];
	GetProgramDirectory(szFileName);
	strcat(szFileName, "info.ini");

	CIniReader iniReader(szFileName);

	// 自动登录
	if (m_bAutoLog)
		iniReader.setKey("1", "autorun", "xeim_user_info");
	else
		iniReader.setKey("0", "autorun", "xeim_user_info");

	// 记住密码
	if (m_bRemember)
		iniReader.setKey("1", "remember_me", "xeim_user_info");
	else
	{
		iniReader.setKey("0", "remember_me", "xeim_user_info");
		iniReader.setKey("", "password", "xeim_user_info");
	}

	// 开机自启动
	CRegistry regMyReg( NULL );
	if ( regMyReg.Open("Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
		HKEY_CURRENT_USER) )
	{
		if ( regMyReg["xeim"].Exists() )
		{
			// Value does exist
			if (!m_bAutoRun)
				regMyReg["xeim"].Delete();
		}
		else
		{
			if (m_bAutoRun)
			{
				char szFileName[_MAX_PATH];
				::GetModuleFileName(NULL, szFileName, _MAX_PATH);
				regMyReg["xeim"] = szFileName;
			}
			// Value does not exist
		}
		regMyReg.Close();
	}
	else
	{
		::MessageBox(NULL, "Unable to open key!", "Error", MB_OK | MB_ICONHAND);
	}

	DestroyWindow();
}

void CGeneralDlg::Init_Ini()
{
	char szFileName[_MAX_PATH];
	GetProgramDirectory(szFileName);
	strcat(szFileName, "info.ini");

	CIniReader iniReader(szFileName);
	if (iniReader.sectionExists("xeim_user_info"))
	{
		// 判断是否‘记住密码’
		CString value = iniReader.getKeyValue("remember_me", "xeim_user_info");
		int iValue = atoi((LPCTSTR)value);
		if (iValue == 1)
		{
			m_bRemember = TRUE;
		}
		// 判断是否‘自动登录’
		value = iniReader.getKeyValue("autorun", "xeim_user_info");
		iValue = atoi((LPCTSTR)value);
		if (iValue == 1)
		{
			m_bAutoLog = TRUE;
		}
	}
	// 注册表里的开机启动项
	CRegistry regMyReg( NULL );
	if ( regMyReg.Open("Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
		HKEY_CURRENT_USER) )
	{
		if ( regMyReg["xeim"].Exists() )
		{
			m_bAutoRun = TRUE;
		}
		regMyReg.Close();
	}
	else
	{
		::MessageBox(NULL, "Unable to open key!", "Error", MB_OK | MB_ICONHAND);
	}

	//快捷键
	m_HotKey.SetWindowText("Ctrl + Alt + Q");

	UpdateData(FALSE);
}

void CGeneralDlg::OnOK() 
{
	// TODO: Add extra validation here
//	DestroyWindow();
//	CDialog::OnOK();
}

void CGeneralDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
//	DestroyWindow();
//	CDialog::OnCancel();
}

void CGeneralDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::PostNcDestroy();
	delete this;
}

void CGeneralDlg::OnEnSetfocusEditHotkey()
{
	// TODO: 在此添加控件通知处理程序代码
	m_HotKey.SetWindowText("Ctrl + Alt + ");
}

void CGeneralDlg::OnEnKillfocusEditHotkey()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CGeneralDlg::OnEnChangeEditHotkey()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//当前焦点在快捷键编辑框上
	
	
}

void CGeneralDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACE("OnChar\r\n");
	/*if ( this->GetFocus()->m_hWnd == m_HotKey.m_hWnd)
	{
		CString str;
		if ( nChar >= 'A' && nChar <= 'Z')
		{
			str.Format("Ctrl + Alt + %c", nChar);
		}
		else if (nChar >= 'a' && nChar <= 'z')
		{
			str.Format("Ctrl + Alt + %c", nChar - 32);
		}
		else if ( nChar >= VK_F1 && nChar <= VK_F12)
		{
		}
		else
		{
			str = "Ctrl + Alt + ";
		}

		m_HotKey.SetWindowText(str);
	}*/

	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CGeneralDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	

	return CDialog::PreTranslateMessage(pMsg);
}
