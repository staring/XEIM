/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// NormalSMS.cpp : implementation file
//

#include "stdafx.h"
#include "client02.h"
#include "client02Dlg.h"
#include "NormalSMS.h"
#include "ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNormalSMS dialog


CNormalSMS::CNormalSMS(CWnd* pParent /*=NULL*/)
	: CDialog(CNormalSMS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNormalSMS)
	m_strRecv = _T("");
	m_strSign = _T("");
	m_strSMS = _T("");
	//}}AFX_DATA_INIT
}


void CNormalSMS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNormalSMS)
	DDX_Text(pDX, IDC_EDIT1, m_strRecv);
	DDX_Text(pDX, IDC_EDIT2, m_strSign);
	DDX_Text(pDX, IDC_EDIT3, m_strSMS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNormalSMS, CDialog)
	//{{AFX_MSG_MAP(CNormalSMS)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNormalSMS message handlers

void CNormalSMS::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CNormalSMS::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if (m_strRecv.IsEmpty())
	{
		MessageBox("请输入短信接收者手机号码！", "操作有误", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (m_strSMS.IsEmpty())
	{
		MessageBox("请输入短信内容！", "操作有误", MB_OK | MB_ICONINFORMATION);
		return;
	}

	// 添加一个符号，后面算法需要
	m_strRecv += ",";

	// 读取配置文件

	// 建立文件名
	char buf[MAX_PATH];
	GetProgramDirectory(buf);
	strcat_s(buf, MAX_PATH*sizeof(char),"setting.ini");

	// 检查用户设置文件
	char szFileName[_MAX_PATH];
	GetProgramDirectory(szFileName);
	strcat_s(szFileName, MAX_PATH*sizeof(char), "info.ini");

	CIniReader iniReader(szFileName);

	CClient02Dlg *pDlg = (CClient02Dlg*)AfxGetMainWnd();
	pDlg->m_strCellNUM = m_strSign;
	iniReader.setKey(m_strSign, "cell_number", "xeim_user_info");

	CDialog::OnOK();
}


LPSTR CNormalSMS::ChangeCODE(LPCTSTR lpText)
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

	return newTmp;
}

BOOL CNormalSMS::IsPhoneNumber(CString strNUM)
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

BOOL CNormalSMS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 设置背景颜色
	m_brBk2.CreateSolidBrush(RGB(236, 243, 246));

	CClient02Dlg *pDlg = (CClient02Dlg*)AfxGetMainWnd();
	m_strSign = pDlg->m_strCellNUM;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CNormalSMS::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
