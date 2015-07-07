/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// DLG_ERP_SCHEDULE.cpp : implementation file
//

#include "stdafx.h"
#include "client02.h"
#include "DLG_ERP_SCHEDULE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DLG_ERP_SCHEDULE dialog


DLG_ERP_SCHEDULE::DLG_ERP_SCHEDULE(CWnd* pParent /*=NULL*/)
	: CDialog(DLG_ERP_SCHEDULE::IDD, pParent)
{
	//{{AFX_DATA_INIT(DLG_ERP_SCHEDULE)
	m_strSubject = _T("");
	m_strType = _T("");
	m_strContent = _T("");
	m_strMembers = _T("");
	m_bPublic = FALSE;

	m_bModify = FALSE;
	//}}AFX_DATA_INIT
}


void DLG_ERP_SCHEDULE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DLG_ERP_SCHEDULE)
	DDX_Text(pDX, IDC_EDIT1, m_strSubject);
	DDX_Text(pDX, IDC_EDIT2, m_strType);
	DDX_Text(pDX, IDC_EDIT4, m_strContent);
	DDX_Text(pDX, IDC_EDIT3, m_strMembers);
	DDX_Check(pDX, IDC_CHECK1, m_bPublic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DLG_ERP_SCHEDULE, CDialog)
	//{{AFX_MSG_MAP(DLG_ERP_SCHEDULE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLG_ERP_SCHEDULE message handlers

// 向服务器提交新日程安排
void DLG_ERP_SCHEDULE::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

HBRUSH DLG_ERP_SCHEDULE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
		return (HBRUSH)m_brBk2.GetSafeHandle();
		break;
	case CTLCOLOR_STATIC:
		//	if (pWnd->GetDlgCtrlID() == IDC_EDIT_NICKNAME)
		{
			pDC->SetBkColor(m_editBKColor);
			pDC->SetTextColor(m_textcolor);
			hbr = (HBRUSH) m_brBk2;
		}
		break;
	default:
		break;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL DLG_ERP_SCHEDULE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitUI();

	if (m_bModify)
	{
		SetDlgItemText(IDC_EDIT3, m_strMembers);
		SetDlgItemText(IDOK, "保存(&S)");
		SetWindowText("修改工作");
	}

	SetDlgItemText(IDC_EDIT5, m_strProject);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DLG_ERP_SCHEDULE::InitUI()
{
	// 文本框背景颜色
	m_editBKColor = RGB(255,255,255); // white text
	// 静态文本颜色
	m_textcolor = RGB(55,55,56);
	// 设置背景颜色
	m_brBk2.CreateSolidBrush(RGB(255, 255, 255));
}
