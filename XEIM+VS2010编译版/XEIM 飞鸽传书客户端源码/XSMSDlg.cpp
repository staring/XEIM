/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// XSMSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "client02.h"
#include "XSMSDlg.h"
#include "xfun.h"
#include "client02Dlg.h"
#include "INI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXSMSDlg dialog


CXSMSDlg::CXSMSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXSMSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXSMSDlg)
	m_strSMS = _T("");
	m_strNUM = _T("");
	//}}AFX_DATA_INIT
}


void CXSMSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXSMSDlg)
	DDX_Text(pDX, IDC_EDIT1, m_strSMS);
	DDX_Text(pDX, IDC_EDIT_SENDER_NUM, m_strNUM);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXSMSDlg, CDialog)
	//{{AFX_MSG_MAP(CXSMSDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXSMSDlg message handlers
LPSTR CXSMSDlg::UnchangeCODE(LPCTSTR lpText)
{
	return NULL;
}

LPSTR CXSMSDlg::ChangeCODE(LPCTSTR lpText)
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
		sprintf_s(buf, 5*sizeof(char), "%04X", tmp[i]);
		strcat_s(newTmp, dwMinSize*4*sizeof(char), buf);
	}
//	char *retTmp = new char[dwMinSize*4+2];
//	sprintf(retTmp, "%02X%s", strlen(newTmp)/2, newTmp);
//	delete [] newTmp;
//	printf("%s", newTmp);
	return newTmp;
}

void CXSMSDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	if(m_strSMS.IsEmpty())
	{
		MessageBox("请输入消息内容！");
		return;
	}

	// 读取配置文件

	// 建立文件名
	char buf[MAX_PATH];
	GetProgramDirectory(buf);
	strcat_s(buf, MAX_PATH*sizeof(char),"setting.ini");

	// 检查用户设置文件
	char szFileName[_MAX_PATH];
	GetProgramDirectory(szFileName);
	strcat_s(szFileName, MAX_PATH*sizeof(char),"info.ini");

	CIniReader iniReader(szFileName);

	CClient02Dlg *pDlg = (CClient02Dlg*)AfxGetMainWnd();
	pDlg->m_strCellNUM = m_strNUM;
	iniReader.setKey(m_strNUM, "cell_number", "xeim_user_info");

	CDialog::OnOK();
}

void CXSMSDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CXSMSDlg::SendSMS(SOCKET sock)
{

}

BOOL CXSMSDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 设置背景颜色
	m_brBk2.CreateSolidBrush(RGB(236, 243, 246));
	// TODO: Add extra initialization here
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	pEdit->SetFocus();

	CClient02Dlg *pDlg = (CClient02Dlg*)AfxGetMainWnd();
//	m_strNUM = "3333";//
	m_strNUM = pDlg->m_strCellNUM;

	UpdateData(FALSE);
//	::SendMessage(GetSafeHwnd(), WM_NEXTDLGCTL, (WPARAM)pEdit->GetSafeHwnd(), TRUE);
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CXSMSDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
