/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// DLG_ERP_NEW_PROJECT.cpp : implementation file
//

#include "stdafx.h"
#include "client02.h"
#include "client02Dlg.h"
#include "DLG_ERP_NEW_PROJECT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DLG_ERP_NEW_PROJECT dialog


DLG_ERP_NEW_PROJECT::DLG_ERP_NEW_PROJECT(CWnd* pParent /*=NULL*/)
	: CDialog(DLG_ERP_NEW_PROJECT::IDD, pParent)
{
	//{{AFX_DATA_INIT(DLG_ERP_NEW_PROJECT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bModify = FALSE;

	
	m_strProject = _T("");
	m_strNumber = _T("");
	m_strDetail = _T("");
	m_strCreator = _T("");
}


void DLG_ERP_NEW_PROJECT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DLG_ERP_NEW_PROJECT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DLG_ERP_NEW_PROJECT, CDialog)
	//{{AFX_MSG_MAP(DLG_ERP_NEW_PROJECT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLG_ERP_NEW_PROJECT message handlers

HBRUSH DLG_ERP_NEW_PROJECT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL DLG_ERP_NEW_PROJECT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitUI();

	// 用户修改项目信息
	if (m_bModify)
	{
		SetWindowText("项目修改");
		SetDlgItemText(IDC_EDIT1, m_strProject);
		SetDlgItemText(IDC_EDIT2, m_strNumber);
		SetDlgItemText(IDC_EDIT3, m_strCreator);
		SetDlgItemText(IDC_DETAIL, m_strDetail);
		SetDlgItemText(IDOK, "保存(&S)");
	}
	else
	{
		CString strNickName = ((CClient02Dlg*)AfxGetMainWnd())->m_strUNickName;
		SetDlgItemText(IDC_EDIT3, strNickName);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DLG_ERP_NEW_PROJECT::InitUI()
{
	// 文本框背景颜色
	m_editBKColor = RGB(255,255,255); // white text
	// 静态文本颜色
	m_textcolor = RGB(55,55,56);
	// 设置背景颜色
	m_brBk2.CreateSolidBrush(RGB(255, 255, 255));
}

void DLG_ERP_NEW_PROJECT::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
