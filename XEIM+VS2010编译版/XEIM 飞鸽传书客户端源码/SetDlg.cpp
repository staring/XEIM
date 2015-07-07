/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// SetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "client02.h"
#include "SetDlg.h"
#include "client02Dlg.h"
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetDlg dialog


CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetDlg)
	m_bChecked = FALSE;
	//}}AFX_DATA_INIT
}


void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDlg)
	DDX_Check(pDX, IDC_CHECK1, m_bChecked);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDlg, CDialog)
	//{{AFX_MSG_MAP(CSetDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDlg message handlers

void CSetDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strOld = ((CClient02Dlg*)AfxGetMainWnd())->m_strSvr;
	GetDlgItemText(IDC_SVR, ((CClient02Dlg*)AfxGetMainWnd())->m_strSvr);
	if (((CClient02Dlg*)AfxGetMainWnd())->m_strSvr.IsEmpty())
	{
		MessageBox("请输入服务器IP或域名！", "操作错误", MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_SVR)->SetFocus();
		((CClient02Dlg*)AfxGetMainWnd())->m_strSvr = strOld;
		return;
	}
	CDialog::OnOK();
}

BOOL CSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	SetDlgItemText(IDC_SVR, ((CClient02Dlg*)AfxGetMainWnd())->m_strSvr);

	// 设置背景颜色
	m_brBk2.CreateSolidBrush(RGB(236, 243, 246));

	CRegistry regMyReg( NULL );
	if ( regMyReg.Open("Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
		HKEY_CURRENT_USER) )
	{
		if ( regMyReg["xeim"].Exists() )
		{
			m_bChecked = TRUE;
			UpdateData(FALSE);
		}
		else
		{
		}
		regMyReg.Close();
	}
	else
	{
		::MessageBox(NULL, "Unable to open key!", "Error", MB_OK | MB_ICONHAND);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}

void CSetDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	CRegistry regMyReg( NULL );
	if ( regMyReg.Open("Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
		HKEY_CURRENT_USER) )
	{
		if ( regMyReg["xeim"].Exists() )
		{
			// Value does exist
			if (m_bChecked)
			{
				m_bChecked = FALSE;
				regMyReg["xeim"].Delete();
			}
			else
			{
				m_bChecked = TRUE;
			}
			UpdateData(FALSE);
		}
		else
		{
			if (m_bChecked)
			{
				m_bChecked = FALSE;
				regMyReg["xeim"].Delete();
			}
			else
			{
				m_bChecked = TRUE;
				char szFileName[_MAX_PATH];
				::GetModuleFileName(NULL, szFileName, _MAX_PATH);
				regMyReg["xeim"] = szFileName;
			}
			UpdateData(FALSE);
			// Value does not exist
		}
		regMyReg.Close();
	}
	else
	{
		::MessageBox(NULL, "Unable to open key!", "Error", MB_OK | MB_ICONHAND);
	}
}

HBRUSH CSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
			pDC->SetBkColor(RGB(236, 243, 246));
			hbr = (HBRUSH) m_brBk2;
		}
		break;
	}

	return hbr;
}
