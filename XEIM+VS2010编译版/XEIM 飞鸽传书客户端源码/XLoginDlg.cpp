/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// XLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "client02.h"
#include "XLoginDlg.h"
#include "client02Dlg.h"
#include "process.h"
#include "SetDlg.h"
#include "../xmsg/xtype.h"
#include "../xmsg/xdef.h"
#include "xfun.h"
#include "INI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXLoginDlg dialog

CXLoginDlg::CXLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXLoginDlg)
	m_bRemember = FALSE;
	m_bAuto = FALSE;
	//}}AFX_DATA_INIT
}


void CXLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXLoginDlg)
	DDX_Check(pDX, IDC_CHECK1, m_bRemember);
	DDX_Check(pDX, IDC_CHECK2, m_bAuto);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CXLoginDlg)
	ON_BN_CLICKED(IDC_SETTING, OnSetting)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_WM_DESTROY()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXLoginDlg message handlers

void CXLoginDlg::OnSetting() 
{
	// TODO: Add your control notification handler code here
	CSetDlg dlg(this);
	if (IDOK == dlg.DoModal())
	{
		// 检查用户设置文件
		char szFileName[_MAX_PATH];
		GetProgramDirectory(szFileName);
		strcat_s(szFileName, MAX_PATH*sizeof(char),"info.ini");

		CIniReader iniReader(szFileName);
		if (iniReader.sectionExists("xeim_server_info"))
		{
		//	m_strSvr = iniReader.getKeyValue("server_addr", "xeim_server_info");
			iniReader.setKey(((CClient02Dlg*)AfxGetMainWnd())->m_strSvr, "server_addr", "xeim_server_info");
		}
		else
		{
			iniReader.setKey("", "server_addr", "xeim_server_info");
		}
	}
}

void CXLoginDlg::OnOK() 
{
	// TODO: Add extra validation here
	ShowWindow(SW_HIDE);
	CString user;
	CString pass;
	GetDlgItemText(IDC_EDIT1, user);
	GetDlgItemText(IDC_EDIT2, pass);


	if (((CClient02Dlg*)AfxGetMainWnd())->Init())
	{
		SOCKET m_sock = ((CClient02Dlg*)AfxGetMainWnd())->m_sock;
		XMSG message;
		XUserDetail xLogin;
	//	xAcc.SetAccount((LPCTSTR)user, (LPCTSTR)pass);
		xLogin.SetUID((LPCTSTR)user);
		xLogin.SetUPassword((LPCTSTR)pass);
		xLogin.SetUNickName("");
		xLogin.OnLine();

		message.m_nMessage = XEIMMSG_LOGIN;
		message.SetData((char*)&xLogin, sizeof(xLogin));
	//	message.SetBuf((char*)&xAcc, sizeof(XMSG_UserINFO));

		int ret = ::send(m_sock, (LPCTSTR)message.GetBuffer(), message.GetBufferLength(), 0);
		if (SOCKET_ERROR == ret || 0 == ret)
		{
			AfxMessageBox("发送不成功！");
			return;
		}
		// 按登录后要更新系统配置文件
		UpdateIniFile((LPCTSTR)user, (LPCTSTR)pass);

		((CClient02Dlg*)AfxGetMainWnd())->m_strUID = user;
		((CClient02Dlg*)AfxGetMainWnd())->m_strUPSW = pass;

		CDialog::OnOK();
		return;
	}
	else
	{
		CString strError;
		strError.Format("%s", ((CClient02Dlg*)AfxGetMainWnd())->m_strLastError);
		::MessageBox(GetSafeHwnd(), strError, "登录失败", MB_OK | MB_ICONERROR);
		ShowWindow(SW_SHOW);
		return;
	}

	CDialog::OnCancel();
}

BOOL CXLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

//	SetDlgItemText(IDC_EDIT1, "aaa");
//	SetDlgItemText(IDC_EDIT2, "118899");
	m_brush.CreateSolidBrush(RGB(255,255,255));

	// 检查用户设置文件
	char szFileName[_MAX_PATH];
	GetProgramDirectory(szFileName);
	strcat_s(szFileName, MAX_PATH*sizeof(char),"info.ini");

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
			m_bAuto = TRUE;
		}
		// 自动读取用户名
		value = iniReader.getKeyValue("username", "xeim_user_info");
		SetDlgItemText(IDC_EDIT1, value);

		// 如果是记住密码，就把密码读取出来
		if (m_bRemember)
		{
			value = iniReader.getKeyValue("password", "xeim_user_info");
			SetDlgItemText(IDC_EDIT2, value);
		}
		else
		{
			m_bAuto = FALSE;
			GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
		}

		UpdateData(FALSE);
	}
	else
	{
		iniReader.setKey("0", "remember_me", "xeim_user_info");
		iniReader.setKey("0", "autorun", "xeim_user_info");
		iniReader.setKey("", "username", "xeim_user_info");
		iniReader.setKey("", "password", "xeim_user_info");
	}

	// 查找用户名及密码
	if (m_bAuto)
		OnOK();

//	GetDlgItem(IDC_CHECK1)->ShowWindow(SW_HIDE);
//	GetDlgItem(IDC_CHECK2)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CXLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return m_brush;
}

// 点了记住密码一下
void CXLoginDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (! m_bRemember)
	{
		GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
		m_bAuto = FALSE;
		UpdateData();
	}
	else
	{
		GetDlgItem(IDC_CHECK2)->EnableWindow();
	}
}

// 点了自动登录一下
void CXLoginDlg::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CXLoginDlg::UpdateIniFile(LPCTSTR lpszUser, LPCTSTR lpszPass)
{
	// 检查用户设置文件
	char szFileName[_MAX_PATH];
	GetProgramDirectory(szFileName);
	strcat_s(szFileName, MAX_PATH*sizeof(char),"info.ini");

	CIniReader iniReader(szFileName);
	if (m_bRemember)
	{
		iniReader.setKey("1", "remember_me", "xeim_user_info");
		iniReader.setKey(lpszPass, "password", "xeim_user_info");
	}
	else
	{
		iniReader.setKey("0", "remember_me", "xeim_user_info");
		iniReader.setKey("", "password", "xeim_user_info");
	}
	if (m_bAuto)
	{
		iniReader.setKey("1", "autorun", "xeim_user_info");
	}
	else
	{
		iniReader.setKey("0", "autorun", "xeim_user_info");
	}
	iniReader.setKey(lpszUser, "username", "xeim_user_info");
//	iniReader.setKey(lpszPass, "password", "xeim_user_info");
}

void CXLoginDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CXLoginDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDCtl)
	{
	case IDC_SETTING:
	case IDOK:
		{
			CDC dc;

			dc.Attach(lpDrawItemStruct->hDC);
			RECT rect;
			rect= lpDrawItemStruct->rcItem;
			
			dc.Draw3dRect(&rect,RGB(236, 243, 246),RGB(236, 243, 246));
		//	dc.Draw3dRect(&rect,RGB(255, 255, 255),RGB(255, 255, 255));
			dc.FillSolidRect(&rect,RGB(236, 243, 246));
		//	dc.FillSolidRect(&rect,RGB(255, 255, 255));
			UINT state=lpDrawItemStruct->itemState;

			if((state & ODS_SELECTED))
			{
				dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);

			}
			else
			{
				dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);
			}

			dc.SetBkColor(RGB(236, 243, 246));
		//	dc.SetBkColor(RGB(255, 255, 255));
			//dc.SetBkColor(GetSysColor(COLOR_BTNFACE));
			dc.SetTextColor(RGB(25, 79, 149));


			TCHAR buffer[MAX_PATH];
			ZeroMemory(buffer,MAX_PATH );
			::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH);
			dc.DrawText(buffer,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			
			dc.Detach();
		}
		break;
	}
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
