/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// DLG_ERP_WORK.cpp : implementation file
//

#include "stdafx.h"
#include "client02.h"
#include "DLG_ERP_WORK.h"
#include "client02Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DLG_ERP_WORK dialog


DLG_ERP_WORK::DLG_ERP_WORK(CWnd* pParent /*=NULL*/)
	: CDialog(DLG_ERP_WORK::IDD, pParent)
{
	//{{AFX_DATA_INIT(DLG_ERP_WORK)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DLG_ERP_WORK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DLG_ERP_WORK)
	DDX_Control(pDX, IDC_LIST1, m_lcWorks);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DLG_ERP_WORK, CDialog)
	//{{AFX_MSG_MAP(DLG_ERP_WORK)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLG_ERP_WORK message handlers

HBRUSH DLG_ERP_WORK::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL DLG_ERP_WORK::OnInitDialog() 
{
	CDialog::OnInitDialog();

	DWORD dwStyle = m_lcWorks.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_lcWorks.SetExtendedStyle(dwStyle);

	m_lcWorks.InsertColumn(0, "发起人", LVCFMT_LEFT, 52);
	m_lcWorks.InsertColumn(1, "主题", LVCFMT_LEFT, 88);
	m_lcWorks.InsertColumn(2, "知会人", LVCFMT_LEFT, 188);
	m_lcWorks.InsertColumn(3, "备注", LVCFMT_LEFT, 288);

	// TODO: Add extra initialization here
	// 文本框背景颜色
	m_editBKColor = RGB(255,255,255); // white text
	// 静态文本颜色
	m_textcolor = RGB(55,55,56);
	// 设置背景颜色
	m_brBk2.CreateSolidBrush(RGB(255, 255, 255));

//	CString strProjectTitle;
//	strProjectTitle.Format("编号：[%s] - %s ", m_strNumber, m_strProject);
	SetDlgItemText(IDC_PRJ_NUMBER, m_strNumber);
	SetDlgItemText(IDC_PRJ_SUBJECT, m_strProject);
	SetDlgItemText(IDC_PRO_DETAIL, m_strProDetail);

	GetScheduleFromSrv();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL DLG_ERP_WORK::GetScheduleFromSrv()
{
	// 获取主程序与服务器通讯套接字
	SOCKET sock = ((CClient02Dlg*)AfxGetMainWnd())->m_sock;

	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_GET_SCHEDULE;
	xmsg.SetData((LPCTSTR)m_strNumber, m_strNumber.GetLength()+1);

	send(sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
	return TRUE;
}

void DLG_ERP_WORK::AddSchedule(CString strData)
{
//	AfxMessageBox("asdf");
	CString strNewData = strData;
	int nIdx = strNewData.Find('\"');

	CString strSubject = strNewData.Left(nIdx);

	CString strMembers = strNewData.Right(strNewData.GetLength()-nIdx-1);
	nIdx = strMembers.Find('\"');
	strNewData = strMembers;
	strMembers = strNewData.Left(nIdx);

	CString strDetail = strNewData.Right(strNewData.GetLength()-nIdx-1);
	nIdx = strDetail.Find('\"');
	strNewData = strDetail;
	strDetail = strNewData.Left(nIdx);

	CString strCreator = strNewData.Right(strNewData.GetLength()-nIdx-1);


	LVITEM lvi;

	lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
	lvi.iItem = 0xff;
	lvi.iSubItem = 0;
	lvi.pszText = (LPTSTR)(LPCTSTR)strCreator;
	lvi.iImage = I_IMAGECALLBACK;		// There are 8 images in the image list
	nIdx = m_lcWorks.InsertItem(&lvi);

	m_lcWorks.SetItemText(nIdx, 1, strSubject);
//	m_lcWorks.SetItemText(nIdx, 1, (LPCTSTR)(((CClient02Dlg*)AfxGetMainWnd())->m_strUNickName));
	m_lcWorks.SetItemText(nIdx, 2, strMembers);
	m_lcWorks.SetItemText(nIdx, 3, strDetail);
}

void DLG_ERP_WORK::OnOK() 
{
	// TODO: Add extra validation here
//	GetScheduleFromSrv();
	DLG_ERP_SCHEDULE *pDlg_ERP_Schedule = ((CClient02Dlg*)AfxGetMainWnd())->m_pDlg_ERP_Schedule;
	// 工作管理对话框
	if (pDlg_ERP_Schedule == NULL)
	{
	//	CDialog::OnOK();
		pDlg_ERP_Schedule = new DLG_ERP_SCHEDULE;
		pDlg_ERP_Schedule->m_strNumber = m_strNumber;
		pDlg_ERP_Schedule->m_strProject = m_strProject;
		pDlg_ERP_Schedule->DoModal();

		delete pDlg_ERP_Schedule;
		pDlg_ERP_Schedule = NULL;
	}
//	CDialog::OnOK();
}

void DLG_ERP_WORK::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
