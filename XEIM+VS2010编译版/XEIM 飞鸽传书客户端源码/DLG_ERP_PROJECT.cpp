/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// DLG_ERP_PROJECT.cpp : implementation file
//

#include "stdafx.h"
#include "client02.h"
#include "DLG_ERP_PROJECT.h"
#include "DLG_ERP_WORK.h"
#include "DLG_ERP_NEW_PROJECT.h"
#include "client02Dlg.h"
#include "../xmsg/xtype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DLG_ERP_PROJECT dialog


DLG_ERP_PROJECT::DLG_ERP_PROJECT(CWnd* pParent /*=NULL*/)
	: CDialog(DLG_ERP_PROJECT::IDD, pParent)
{
	//{{AFX_DATA_INIT(DLG_ERP_PROJECT)
//	m_strRemark = _T("");
//	m_strProjectTitle = _T("");
	m_bAddNew = FALSE;
	m_dwMaxNumber = 0;

	m_hItemCurSel = 0;
	//}}AFX_DATA_INIT
}


void DLG_ERP_PROJECT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DLG_ERP_PROJECT)
	DDX_Control(pDX, IDC_LIST2, m_lcWorks);
	DDX_Control(pDX, IDC_LIST1, m_lcAllProject);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DLG_ERP_PROJECT, CDialog)
	//{{AFX_MSG_MAP(DLG_ERP_PROJECT)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	ON_BN_CLICKED(IDC_BTN_NEW_PROJECT, OnBtnNewProject)
	ON_BN_CLICKED(IDC_BTN_NEW_WORK, OnBtnNewWork)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLG_ERP_PROJECT message handlers

void DLG_ERP_PROJECT::OnOK() 
{
	// TODO: Add extra validation here
	if (m_bAddNew)
	{
		ZoomOut();
		// 添加(&A)
		SetDlgItemText(IDOK, "保存(&S)");
		return;
	}

	// 确保所有必需的信息都填入
	if (! EnsureDataComplete())
		return;

	// 项目表好获取
//	CTime tmNow = CTime::GetCurrentTime();
//	CString strNumber;
//	strNumber.Format("%s%03d", tmNow.Format("%y%m"), 21);

//	CString strUID = ((CClient02Dlg*)AfxGetMainWnd())->m_strUID;
//	CString strAll;
//	strAll.Format("%s\"%s\"%s", strUID, m_strProjectTitle, m_strRemark);
/*

	if (SendProjectToSrv(strAll))
	{
	//	AddProject(strAll);
		MessageBox("记录添加成功！");

		// 保存(&S)
		SetDlgItemText(IDOK, "添加(&A)");

		// 调整窗口大小
		ZoomIn();
	}*/
}

BOOL DLG_ERP_PROJECT::SendProjectToSrv(CString &str)
{
	// 获取主程序与服务器通讯套接字
	SOCKET sock = ((CClient02Dlg*)AfxGetMainWnd())->m_sock;

//	AfxMessageBox(str);

	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_NEW_PROJECT;
	xmsg.SetData((LPCTSTR)str, str.GetLength()+1);
//	XMSG_ONLINE omsg;

/*	CString strSMS;
	CString strNickName;
	AfxGetMainWnd()->GetDlgItemText(IDC_EDIT_NICKNAME, strNickName);
	strSMS.Format(_T("%s[%s]：%s"), strNickName, dlg.m_strNUM, dlg.m_strSMS);
	char *tmp = dlg.ChangeCODE((LPCTSTR)strSMS);

	XMSG xRecord;
	xRecord.m_nMessage = XEIMMSG_SMS_RECORD;
	XMSG_ONLINE xData;

	xData.SetData(m_strUID, (LPCTSTR)strSMS, strSMS.GetLength()+1);
	xRecord.SetData(xData.GetBuffer(), xData.GetBufferLength());*/
	send(sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
	return TRUE;
}

BOOL DLG_ERP_PROJECT::EnsureDataComplete()
{
	UpdateData(TRUE);
/*
	if (m_strProjectTitle.IsEmpty())
	{
		AfxMessageBox("请输入项目名称！");
		return FALSE;
	}
*/
	return TRUE;
}

BOOL DLG_ERP_PROJECT::GetProjectFromSrv()
{
	// 获取主程序与服务器通讯套接字
	SOCKET sock = ((CClient02Dlg*)AfxGetMainWnd())->m_sock;

//	AfxMessageBox(str);

	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_GET_PROJECT;
	xmsg.SetData("", 1);

	send(sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
	return TRUE;
}

BOOL DLG_ERP_PROJECT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitUI();

	// 向服务器请求项目列表
	GetProjectFromSrv();

//	ZoomIn();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH DLG_ERP_PROJECT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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


void DLG_ERP_PROJECT::AddProject(CString &strData)
{
	CString strNewData = strData;
	int nIdx = strNewData.Find('\"');

	CString strNumber = strNewData.Left(nIdx);

	CString strCreator = strNewData.Right(strNewData.GetLength()-nIdx-1);
	nIdx = strCreator.Find('\"');
	strNewData = strCreator;
	strCreator = strNewData.Left(nIdx);

	CString strProject = strNewData.Right(strNewData.GetLength()-nIdx-1);
	nIdx = strProject.Find('\"');
	strNewData = strProject;
	strProject = strNewData.Left(nIdx);

	CString strRemark = strNewData.Right(strNewData.GetLength()-nIdx-1);


	LVITEM lvi;

	lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
	lvi.iItem = 0xff;
	lvi.iSubItem = 0;
	lvi.pszText = (LPTSTR)(LPCTSTR)strNumber;
	lvi.iImage = I_IMAGECALLBACK;		// There are 8 images in the image list
	nIdx = m_lcAllProject.InsertItem(&lvi);

	m_lcAllProject.SetItemText(nIdx, 1, (LPCTSTR)(((CClient02Dlg*)AfxGetMainWnd())->m_strUNickName));
	m_lcAllProject.SetItemText(nIdx, 2, strProject);
	m_lcAllProject.SetItemText(nIdx, 3, strRemark);
}

void DLG_ERP_PROJECT::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lcAllProject.GetFirstSelectedItemPosition();

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (NULL != pos)
	{
		char szNumber[32];
		m_lcAllProject.GetItemText(pNMListView->iItem, 0, szNumber, 32);

		char szProject[1024];
		m_lcAllProject.GetItemText(pNMListView->iItem, 2, szProject, 1024);

		char szCreator[32];
		m_lcAllProject.GetItemText(pNMListView->iItem,1, szCreator, 32);

		char szDetail[1024];
		m_lcAllProject.GetItemText(pNMListView->iItem,3, szDetail, 1024);

		DLG_ERP_NEW_PROJECT dlg;
		dlg.m_bModify = TRUE; // 调用修改对话
		dlg.m_strProject = szProject;
		dlg.m_strNumber = szNumber;
		dlg.m_strCreator = szCreator;
		dlg.m_strDetail = szDetail;

		if (dlg.DoModal() == IDOK)
		{
		}
	}

	*pResult = 0;
}

void DLG_ERP_PROJECT::ZoomIn()
{
	if (! m_bAddNew)
	{
		CRect rtWnd;
		GetWindowRect(& rtWnd);
		rtWnd.bottom -= 188;
		MoveWindow(& rtWnd);
		m_bAddNew = !m_bAddNew;
	}
}

void DLG_ERP_PROJECT::AddSchedule(CString strData)
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
	m_lcWorks.SetItemText(nIdx, 2, strMembers);
	m_lcWorks.SetItemText(nIdx, 3, strDetail);
}

void DLG_ERP_PROJECT::ZoomOut()
{
	if (m_bAddNew)
	{
		CRect rtWnd;
		GetWindowRect(& rtWnd);
		rtWnd.bottom += 188;
		MoveWindow(& rtWnd);
		m_bAddNew = !m_bAddNew;
	}
}

// 获取指定项目编号的所有工作列表
BOOL DLG_ERP_PROJECT::GetScheduleFromSrv(CString strNumber)
{
	// 获取主程序与服务器通讯套接字
	SOCKET sock = ((CClient02Dlg*)AfxGetMainWnd())->m_sock;

	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_GET_SCHEDULE;
	xmsg.SetData((LPCTSTR)strNumber, strNumber.GetLength()+1);

	send(sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
	return TRUE;
}

void DLG_ERP_PROJECT::OnCancel() 
{
	// TODO: Add extra cleanup here
	

	CDialog::OnCancel();
}

void DLG_ERP_PROJECT::OnBtnWork() 
{
	// TODO: Add your control notification handler code here

	// 工作管理对话框
//	DLG_ERP_WORK *pDlg_ERP_Work;
/*
	pDlg_ERP_Work = new DLG_ERP_WORK;
	pDlg_ERP_Work->m_strNumber = m_strNumber;
	pDlg_ERP_Work->m_strProject = m_strProjectTitle;
	pDlg_ERP_Work->m_strProDetail = m_strRemark;

	((CClient02Dlg*)AfxGetMainWnd())->m_pDlg_ERP_Work = pDlg_ERP_Work;
	pDlg_ERP_Work->DoModal();

	delete pDlg_ERP_Work;
	pDlg_ERP_Work = NULL;
	((CClient02Dlg*)AfxGetMainWnd())->m_pDlg_ERP_Work = NULL;*/
}

void DLG_ERP_PROJECT::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here

	POSITION pos = m_lcAllProject.GetFirstSelectedItemPosition();

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (NULL != pos)
	{
		m_hItemCurSel = pNMListView->iItem;
		char szNumber[256];
		m_lcAllProject.GetItemText(pNMListView->iItem, 0, szNumber, 256);
		GetScheduleFromSrv(szNumber);
		m_lcWorks.DeleteAllItems();
	//	m_strNumber = szNumber;

	/*	char szTitle[1024];
		m_lcAllProject.GetItemText(pNMListView->iItem, 2, szTitle, 1024);
		m_strProjectTitle = szTitle;

		m_lcAllProject.GetItemText(pNMListView->iItem, 3, szTitle, 1024);
		m_strRemark = szTitle;*/
	}
	*pResult = 0;
}

void DLG_ERP_PROJECT::InitUI()
{
	// 文本框背景颜色
	m_editBKColor = RGB(255,255,255); // white text
	// 静态文本颜色
	m_textcolor = RGB(55,55,56);
	// 设置背景颜色
	m_brBk2.CreateSolidBrush(RGB(255, 255, 255));


	// Initial extended style for the list control on this dialog
	DWORD dwStyle = m_lcAllProject.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_lcAllProject.SetExtendedStyle(dwStyle);

	m_lcAllProject.InsertColumn(0, "编号", LVCFMT_LEFT, 52);
	m_lcAllProject.InsertColumn(1, "发起者", LVCFMT_LEFT, 88);
	m_lcAllProject.InsertColumn(2, "名称", LVCFMT_LEFT, 188);
	m_lcAllProject.InsertColumn(3, "备注", LVCFMT_LEFT, 288);

	dwStyle = m_lcWorks.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_lcWorks.SetExtendedStyle(dwStyle);

	m_lcWorks.InsertColumn(0, "发起人", LVCFMT_LEFT, 52);
	m_lcWorks.InsertColumn(1, "主题", LVCFMT_LEFT, 88);
	m_lcWorks.InsertColumn(2, "知会人", LVCFMT_LEFT, 188);
	m_lcWorks.InsertColumn(3, "备注", LVCFMT_LEFT, 288);
}

void DLG_ERP_PROJECT::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lcAllProject.GetFirstSelectedItemPosition();

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (NULL != pos)
	{
		char szProject[1024];
		m_lcAllProject.GetItemText(m_hItemCurSel, 2, szProject, 1024);

		char szSubject[1024];
		m_lcWorks.GetItemText(pNMListView->iItem, 1, szSubject, 1024);

		char szMembers[1024];
		m_lcWorks.GetItemText(pNMListView->iItem, 2, szMembers, 1024);

	//	OnBtnWork();
	//	DLG_ERP_SCHEDULE *pDlg_ERP_Schedule = ((CClient02Dlg*)AfxGetMainWnd())->m_pDlg_ERP_Schedule;
		// 工作管理对话框
		DLG_ERP_SCHEDULE dlg;
		dlg.m_bModify = TRUE;
		dlg.m_strProject = szProject;
		dlg.m_strSubject = szSubject;
		dlg.m_strMembers = szMembers;
		if (IDOK == dlg.DoModal())
		{
		}
	/*	if (pDlg_ERP_Schedule == NULL)
		{
			pDlg_ERP_Schedule = new DLG_ERP_SCHEDULE;
			pDlg_ERP_Schedule->DoModal();

			delete pDlg_ERP_Schedule;
			pDlg_ERP_Schedule = NULL;
		}*/
	}
	*pResult = 0;
}

void DLG_ERP_PROJECT::OnBtnNewProject() 
{
	// TODO: Add your control notification handler code here
	DLG_ERP_NEW_PROJECT dlg;
	if (dlg.DoModal() == IDOK)
	{
	}
}

void DLG_ERP_PROJECT::OnBtnNewWork() 
{
	// TODO: Add your control notification handler code here

	char szProject[1024];
	m_lcAllProject.GetItemText(m_hItemCurSel, 2, szProject, 1024);
	// 工作管理对话框
	DLG_ERP_SCHEDULE dlg;
	dlg.m_strProject = szProject;
	if (IDOK == dlg.DoModal())
	{
	}
}
