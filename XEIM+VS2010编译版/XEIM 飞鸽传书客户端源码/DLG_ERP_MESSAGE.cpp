/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// DLG_ERP_MESSAGE.cpp : implementation file
//

#include "stdafx.h"
#include "client02.h"
#include "DLG_ERP_MESSAGE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DLG_ERP_MESSAGE dialog


DLG_ERP_MESSAGE::DLG_ERP_MESSAGE(CWnd* pParent /*=NULL*/)
	: CDialog(DLG_ERP_MESSAGE::IDD, pParent)
{
	//{{AFX_DATA_INIT(DLG_ERP_MESSAGE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DLG_ERP_MESSAGE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DLG_ERP_MESSAGE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DLG_ERP_MESSAGE, CDialog)
	//{{AFX_MSG_MAP(DLG_ERP_MESSAGE)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLG_ERP_MESSAGE message handlers

void DLG_ERP_MESSAGE::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void DLG_ERP_MESSAGE::SetMessage(CString strMsg)
{
	int nSize = m_vecMessage.size();
	CTime ctNow = CTime::GetCurrentTime();
	CString strMessage;
	// 信息 发送者 和 发送时间
	strMessage.Format(_T("第[%d]条 - %s\r\n%s"), nSize+1, 
		ctNow.Format(_T("%H:%M:%S")),
		strMsg);

//	strMessage.Replace("#", "xxx");
	m_vecMessage.push_back((LPCTSTR)strMessage);
	// 显示消息
	SetDlgItemText(IDC_EDIT1, strMessage);
	// 显示消息条数
	CString strSize;
	strSize.Format("共有[%d]条系统消息。", nSize+1);
	SetDlgItemText(IDC_STATIC_SIZE, strSize);

	m_iPointer = m_vecMessage.end();
	m_iPointer --;

	// 如果消息不止一条
	if (nSize > 0)
	{
		GetDlgItem(IDC_BACK)->EnableWindow();
	}
}

void DLG_ERP_MESSAGE::OnBack() 
{
	// TODO: Add your control notification handler code here
	// 上一条记录
	m_iPointer --;

	string str = *m_iPointer;
	SetDlgItemText(IDC_EDIT1, str.c_str());
	
	if (! GetDlgItem(IDC_NEXT)->IsWindowEnabled())
	{
		GetDlgItem(IDC_NEXT)->EnableWindow(TRUE);
	}

	// 如果是第一条记录
	if (m_iPointer == m_vecMessage.begin())
	{
		GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
	}
}

void DLG_ERP_MESSAGE::OnNext() 
{
	// TODO: Add your control notification handler code here
	m_iPointer ++;

	string str = *m_iPointer;
	SetDlgItemText(IDC_EDIT1, str.c_str());
	if (! GetDlgItem(IDC_BACK)->IsWindowEnabled())
	{
		GetDlgItem(IDC_BACK)->EnableWindow(TRUE);
	}

	// 如果是最后一条
	m_iPointer ++;
	if (m_iPointer == m_vecMessage.end())
	{
		GetDlgItem(IDC_NEXT)->EnableWindow(FALSE);

	}

	m_iPointer --;
}

BOOL DLG_ERP_MESSAGE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 上一条按钮
	GetDlgItem(IDC_BACK)->EnableWindow(FALSE);
	// 下一条按钮
	GetDlgItem(IDC_NEXT)->EnableWindow(FALSE);

	// 开始时，无任何消息显示：暂无消息
	SetDlgItemText(IDC_EDIT1, "- 暂无消息");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
