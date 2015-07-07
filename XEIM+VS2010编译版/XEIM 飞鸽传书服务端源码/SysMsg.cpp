// SysMsg.cpp : 实现文件
//

#include "stdafx.h"
#include "xsvr.h"
#include "SysMsg.h"


// CSysMsg 对话框

IMPLEMENT_DYNAMIC(CSysMsg, CDialog)

CSysMsg::CSysMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CSysMsg::IDD, pParent)
{

}

CSysMsg::~CSysMsg()
{
}

void CSysMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SYSMSG, m_SysMsg);
}


BEGIN_MESSAGE_MAP(CSysMsg, CDialog)
	ON_BN_CLICKED(IDOK, &CSysMsg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSysMsg 消息处理程序

void CSysMsg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sText;
	GetDlgItemText(IDC_EDIT_SYSMSG, sText);
	sMsg = (LPCTSTR)sText;

	OnOK();
}

BOOL CSysMsg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_SysMsg.SetWindowText(_T("在此编辑系统消息!"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
