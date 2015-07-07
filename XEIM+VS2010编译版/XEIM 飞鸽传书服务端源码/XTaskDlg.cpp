/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// XTaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xsvr.h"
#include "XTaskDlg.h"
#include "xsvrDlg.h"
#include <string>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTaskDlg dialog


CXTaskDlg::CXTaskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXTaskDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXTaskDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nUniqueID = 0;
}


void CXTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTaskDlg)
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_ctrlTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_ctrlDate);
	DDX_Control(pDX, IDC_TREE1, m_ctrlTree);
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
	DDX_Control(pDX, IDC_COMBO1, m_ctrlComboBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXTaskDlg, CDialog)
	//{{AFX_MSG_MAP(CXTaskDlg)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_TREE1, OnClickTree1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTaskDlg message handlers

void CXTaskDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CXTaskDlg::OnOK() 
{
	// TODO: Add extra validation here

	CString strContent;
	GetDlgItemText(IDC_EDIT1, strContent);
	if (strContent.IsEmpty())
	{
		MessageBox("请输入内容！","不能添加任务", MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_EDIT1)->SetFocus();
		return;
	}

	BOOL bChanged = FALSE;
	map<string, HTREEITEM>::iterator iGroup;
	for (iGroup = m_mapGroup.begin();
	iGroup != m_mapGroup.end(); iGroup ++)
	{
		if (CheckForChanging(iGroup->second) != 0)
		{
			bChanged = TRUE;
		}
	}

	if (! bChanged)
	{
		MessageBox("请选择一个以上的接收者！", "不能添加任务", MB_OK|MB_ICONWARNING);
		return;
	}

	// 默认只执行一次
	int nFrequency = 1;

	CString strFrequency;
	m_ctrlComboBox.GetWindowText(strFrequency);
	if (m_ctrlComboBox.GetCurSel() == 1)
	{
		nFrequency = 0;
	}

	CTime timeNow;
	CTime dateNow;
	m_ctrlTime.GetTime(timeNow);
	m_ctrlDate.GetTime(dateNow);
	CString strTime;
	strTime.Format("%s %s", dateNow.Format("%Y-%m-%d"), timeNow.Format("%H:%M:%S"));

	// 自动增加一
	m_nUniqueID ++;
	AddNewTask(strTime, strContent, nFrequency);

	SetDlgItemText(IDC_EDIT1, "");
	GetDlgItem(IDC_EDIT1)->SetFocus();

}

BOOL CXTaskDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CXsvrDlg *pDlg = (CXsvrDlg*)AfxGetMainWnd();
	// TODO: Add extra initialization here
	m_ctrlTree.ModifyStyle(0,TVS_CHECKBOXES);
	m_ctrlTree.SetItemHeight(16);
	// 用户 CTreeCtrl 图标
	m_ctrlTree.SetImageList(& pDlg->m_ilUser, TVSIL_NORMAL);

	Init_CtrlList();

	list<XUserDetail>::iterator iDetail;
	map<string, HTREEITEM>::iterator iGroup;
	XUserInfo xui;
	HTREEITEM hGroup;
	HTREEITEM hItem;
	list<string>::iterator iContact;

	for (iDetail = pDlg->m_listUserDetail.begin();
	iDetail != pDlg->m_listUserDetail.end(); ++iDetail)
	{
		iDetail->GetUserInfo(xui);
		
		// 先获取并保存部门信息。存在就跳过，不存在则保存。
		iGroup = m_mapGroup.find(xui.GetUGroup());
		if (iGroup == m_mapGroup.end())	// 不存在
		{
			hGroup = m_ctrlTree.InsertItem(xui.GetUGroup(), 1, 1);
			m_mapGroup.insert(pair<string,HTREEITEM>(xui.GetUGroup(), hGroup));
		}
		else // 该组已存在
		{
			hGroup = iGroup->second;
		}
		// 插入UI
		hItem = m_ctrlTree.InsertItem(xui.GetUID(), 2, 2, hGroup);
	}

	// 启动任务管理
	SetTimer(TIMER_SCAN_DATABASE, TIMER_SCAN_DATABASE_TEIM, NULL);
	SetTimer(TIMER_DATE_TIME, 1000, NULL);

	m_ctrlComboBox.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// XTask
XTask::XTask()
{
	nID = 0;
	nTime = 0;
	nFrequency = 0;
	szContent = NULL;
}

XTask::~XTask()
{
	if (szContent)
	{
		delete [] szContent;
		szContent = NULL;
	}
}

BOOL XTask::SetDate(LPCTSTR szDate)
{
	char *szStandard = "2008-10-10 12:12:12";
	int nLen = strlen(szStandard);

	int nDateLen = strlen(szDate);
	if (nLen != nDateLen)
	{
		AfxMessageBox("输入的日期格式错误！");
		return FALSE;
	}
	else
	{
		char b1[5]={0,0,0,0,0};
		char b2[3]={0,0,0};
		char b3[3]={0,0,0};

		for (int i=0; i<2; i++)
		{
			b1[i] = szDate[i];
			b1[2+i] = szDate[2+i];
			b2[i] = szDate[5+i];
			b3[i] = szDate[8+i];
		}

		int nYear = atoi(b1);
		int nMonth = atoi(b2);
		int nDay = atoi(b3);

		// 算法
		nDate = nYear * 10 + nMonth * 151 + nDay;
	}

	return TRUE;
}

BOOL XTask::SetTime(LPCTSTR szTime)
{
	char *szStandard = "2008-10-10 12:12:12";
	int nLen = strlen(szStandard);

	int nTimeLen = strlen(szTime);
	if (nLen != nTimeLen)
	{
		AfxMessageBox("输入的时间格式错误！");
		return FALSE;
	}
	else
	{
		char b1[3]={0,0,0};
		char b2[3]={0,0,0};
		char b3[3]={0,0,0};

		for (int i=0; i<2; i++)
		{
			b1[i] = szTime[11+i];
			b2[i] = szTime[14+i];
			b3[i] = szTime[17+i];
		}

		nTime = atoi(b1);
		nTime *= 60;
		nTime += atoi(b2);
	}

	return TRUE;
}

BOOL CXTaskDlg::AddNewTask(CString strTime, CString strContent, DWORD dwFrequency)
{
	CXsvrDlg *pDlg = (CXsvrDlg*)AfxGetMainWnd();

	char *sql = "INSERT INTO task_query("
		"x_task_id,"
		"x_content,"
		"x_activate_time,"
		"x_frequency) "
		"VALUES('%d','%s','%s','%d')";

	CString strSQLFilter = strContent;
	strSQLFilter.Replace("'", "''");
	CString strSQL;
	strSQL.Format(sql,
		m_nUniqueID,
		strSQLFilter,
		strTime,
		dwFrequency);

	try
	{
		_variant_t vAffected;
		pDlg->m_pConn->CursorLocation = adUseClient;
		pDlg->m_pConn->Execute((LPCTSTR)strSQL, &vAffected, adExecuteNoRecords);
	}
	catch (_com_error e)
	{
		pDlg->AddErrorInfo((LPCTSTR)_bstr_t(e.Description()));
		return FALSE;
	}

	int nIdx = 0;
	LVITEM lvi;

	CString tmp;
	tmp.Format("%d", m_nUniqueID);
	lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
	lvi.iItem = 0xff;
	lvi.iSubItem = 0;
	lvi.pszText = (LPTSTR)(LPCTSTR)(tmp);
	lvi.iImage = I_IMAGECALLBACK;
	nIdx = m_ctrlList.InsertItem(&lvi);
	m_ctrlList.SetItemText(nIdx, 1, strContent);
	m_ctrlList.SetItemText(nIdx, 2, strTime);
	CString strFrequency;
	strFrequency = dwFrequency==0?"每天":"1次";
	m_ctrlList.SetItemText(nIdx, 3, strFrequency);

	XTask *pTask = new XTask;
	pTask->nID=m_nUniqueID;
	pTask->szContent = new char[strContent.GetLength()+1];
	strcpy(pTask->szContent, strContent);
	pTask->SetDate(strTime);
	pTask->SetTime(strTime);
	pTask->nFrequency = dwFrequency;
	m_listTask.push_back(pTask);

	return TRUE;
}

void CXTaskDlg::Init_CtrlList()
{
	CXsvrDlg *pDlg = (CXsvrDlg*)AfxGetMainWnd();

	m_ctrlList.InsertColumn(0, "任务 ID", LVCFMT_LEFT, 66);
	m_ctrlList.InsertColumn(1, "发送内容", LVCFMT_LEFT, 88);
	m_ctrlList.InsertColumn(2, "任务时间", LVCFMT_LEFT, 128);
	m_ctrlList.InsertColumn(3, "重复次数", LVCFMT_LEFT, 98);

	// *****************************************************************
	// 读取数据库并显示用户信息
	// *****************************************************************
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));

	char * szSQL = "SELECT * FROM task_query";
	variant_t var[4];
	CString str;
	int nIdx = 0;
	LVITEM lvi;

	try
	{
		pRs->Open(szSQL,
			pDlg->m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

		// 如果数据库是空白的
		if (pRs->adoEOF)
		{
			pDlg->AddErrorInfo("暂无用户记录！");
			pRs->Close();
			pRs.Release();
			return;
		}
		else
		{
			// 读取并显示数据库里的用户信息		
			while (!pRs->adoEOF)
			{
				XTask *pTask = new XTask;
				var[0] = pRs->GetCollect(_variant_t("x_task_id"));
				if (var[0].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[0]);

					lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
					lvi.iItem = 0xff;
					lvi.iSubItem = 0;
					lvi.pszText = (LPTSTR)(LPCTSTR)(str);
					lvi.iImage = I_IMAGECALLBACK;
					nIdx = m_ctrlList.InsertItem(&lvi);
					pTask->nID = atoi((LPCTSTR)str);
					if ((int)m_nUniqueID < pTask->nID)
					{
						m_nUniqueID = pTask->nID;
					}
				}

				var[1] = pRs->GetCollect(_variant_t("x_content"));
				if (var[1].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[1]);
					m_ctrlList.SetItemText(nIdx, 1, str);
					pTask->szContent = new char[str.GetLength()+1];
					strcpy(pTask->szContent, (LPCTSTR)str);
				}

				var[2] = pRs->GetCollect(_variant_t("x_activate_time"));
				if (var[2].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[2]);
					m_ctrlList.SetItemText(nIdx, 2, str);
					pTask->SetDate(str);
					pTask->SetTime(str);
				}

				// 获取用户所在分组
				var[3] = pRs->GetCollect(_variant_t("x_frequency"));
				if (var[3].vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var[3]);
					pTask->nFrequency = atoi((LPCTSTR)str);
					if (pTask->nFrequency == 0)
					{
						m_ctrlList.SetItemText(nIdx, 3, "每天");
					}
					else
					{
						m_ctrlList.SetItemText(nIdx, 3, "1次");
					}
				}

				m_listTask.push_back(pTask);
				pRs->MoveNext();
			}
		}
	}
	catch (_com_error e)
	{
		pDlg->AddErrorInfo(e.ErrorMessage());
	}

	pRs->Close();
	pRs.Release();
}

void CXTaskDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == TIMER_SCAN_DATABASE)
	{
		// 先停止计时器，然后在 CheckForUpdate 中再启动计时器。
		KillTimer(TIMER_SCAN_DATABASE);
		CheckForUpdate();
	}
	else if (nIDEvent == TIMER_DATE_TIME)
	{
		CTime today;
		today = CTime::GetCurrentTime();
		CString strTime;
		SetDlgItemText(IDC_STATIC_TIME, today.Format(_T("%Y-%m-%d %H:%M:%S")));
	}

	CDialog::OnTimer(nIDEvent);
}

void CXTaskDlg::OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	UINT flag;
	BOOL bRoot;

	HTREEITEM hItem;
	GetCursorPos(&pt);
	m_ctrlTree.ScreenToClient(&pt);   

	hItem = m_ctrlTree.HitTest(pt, &flag);

	if (hItem == NULL)
	{
		*pResult = 0;
		return;
	}

	CString itemText = m_ctrlTree.GetItemText(hItem);

	// 判断选项是否为分组
	map<string, HTREEITEM>::iterator iGroup;
	iGroup = m_mapGroup.find((LPCTSTR)itemText);
	if (iGroup != m_mapGroup.end())
	{
		if (iGroup->second == hItem)
		{
			bRoot = TRUE;
			//		AfxMessageBox("ROOT");
		}
		else
		{
			bRoot = FALSE;
		}
	}
	else
	{
		bRoot = FALSE;
	} // end 判断选项是否为分组
	
	if (flag & TVHT_ONITEMSTATEICON)
	{
		//		AfxMessageBox("TVHT_ONITEMSTATEICON");
		if (bRoot)		// ROOT
		{
			if (! m_ctrlTree.GetCheck(hItem))
			{
				SetItemCheck(hItem, TRUE);
			}
			else
			{
				SetItemCheck(hItem, FALSE);
			}
		}
		else						// CHILD
		{
			if (m_ctrlTree.GetCheck(hItem))
			{
				//			AfxMessageBox("aaaaa");
				HTREEITEM hParent = m_ctrlTree.GetParentItem(hItem);
				if (m_ctrlTree.GetCheck(hParent))
					m_ctrlTree.SetCheck(hParent, FALSE);
			}
			else
			{
				SetParentCheck2(hItem);
			}
		}
		// 只有当用户选择方框时，才需要调用此函数，下面代码不需要。
		m_ctrlTree.SelectItem(hItem);
	}
	else if (flag & (TVHT_ONITEMICON | TVHT_ONITEMLABEL))
	{
		if (bRoot)
		{
			if (m_ctrlTree.GetCheck(hItem))
			{
				m_ctrlTree.SetCheck(hItem, FALSE);
				
				SetItemCheck(hItem, FALSE);
			}
			else
			{
				m_ctrlTree.SetCheck(hItem);
				
				SetItemCheck(hItem, TRUE);
			}
		}
		else
		{
			if (m_ctrlTree.GetCheck(hItem))
			{
				HTREEITEM hParent = m_ctrlTree.GetParentItem(hItem);
				if (hParent == NULL)
					return;
				if (m_ctrlTree.GetCheck(hParent))
					m_ctrlTree.SetCheck(hParent, FALSE);
				
				m_ctrlTree.SetCheck(hItem, FALSE);
				//	AfxMessageBox(m_treeUserList.GetItemText(hParent));
			}
			else
			{
				m_ctrlTree.SetCheck(hItem);
				SetParentCheck(hItem);
			}
		}
	}
	
	*pResult = 0;
}


void CXTaskDlg::SetItemCheck(HTREEITEM hParent, BOOL bCheck)
{
	HTREEITEM hChild = m_ctrlTree.GetChildItem(hParent);
	
	m_ctrlTree.SetCheck(hChild, bCheck);
	
	while(hChild = m_ctrlTree.GetNextSiblingItem(hChild))
	{
		m_ctrlTree.SetCheck(hChild, bCheck);
	}
}


void CXTaskDlg::SetParentCheck(HTREEITEM hItem)
{
	HTREEITEM hParent;
	BOOL bCheckParent = FALSE;

	if (hItem != NULL)
	{
		hParent = m_ctrlTree.GetParentItem(hItem);
		HTREEITEM hChild = m_ctrlTree.GetChildItem(hParent);

		while (hChild)
		{
			if (! m_ctrlTree.GetCheck(hChild))
				break;
			hChild = m_ctrlTree.GetNextSiblingItem(hChild);
		}
		if (hChild == NULL)
		{
			m_ctrlTree.SetCheck(hParent);
		}
	}
}

void CXTaskDlg::SetParentCheck2(HTREEITEM hItem)
{
	HTREEITEM hParent;
	int i=0;
	BOOL bCheckParent = FALSE;

	if (hItem != NULL)
	{
		hParent = m_ctrlTree.GetParentItem(hItem);
		HTREEITEM hChild = m_ctrlTree.GetChildItem(hParent);

		while (hChild)
		{
			if (hChild == hItem)
			{
				hChild = m_ctrlTree.GetNextSiblingItem(hChild);
				continue;
			}
			if (! m_ctrlTree.GetCheck(hChild))
				break;

			hChild = m_ctrlTree.GetNextSiblingItem(hChild);
		}
		if (hChild == NULL)
		{
			m_ctrlTree.SetCheck(hParent);
		}
	}
}


DWORD CXTaskDlg::CheckForChanging(HTREEITEM hParent)
{
	DWORD dwItemChanged = 0;
	HTREEITEM hChild = m_ctrlTree.GetChildItem(hParent);
	list<string>::iterator iContact;

	m_ctrlTree.SetCheck(hParent, FALSE);

	string str = m_ctrlTree.GetItemText(hChild);
	// 如果打勾了
	if (m_ctrlTree.GetCheck(hChild))
	{
		AddNewContact(str.c_str());
		m_ctrlTree.SetCheck(hChild, FALSE);

		dwItemChanged +=1;
	}

	while(hChild = m_ctrlTree.GetNextSiblingItem(hChild))
	{
		str = m_ctrlTree.GetItemText(hChild);
		// 如果打勾了
		if (m_ctrlTree.GetCheck(hChild))
		{
			string str = m_ctrlTree.GetItemText(hChild);
			AddNewContact(str.c_str());
			m_ctrlTree.SetCheck(hChild, FALSE);
			dwItemChanged += 1;
			SetParentCheck2(hChild);
		}
	}

	return dwItemChanged;
}



// 添加新的联系人
void CXTaskDlg::AddNewContact(LPCTSTR szUID)
{
	char buf[256];

	// 主框架句柄
	CXsvrDlg *pDlg = (CXsvrDlg*)GetParent();
	// SQL 查询
	sprintf(buf, "INSERT INTO task_receiver(x_task_id,x_receiver) VALUES('%d','%s')",
		m_nUniqueID+1, szUID);

	try
	{
		_variant_t vAffected;
		pDlg->m_pConn->CursorLocation = adUseClient;
		pDlg->m_pConn->Execute(buf, &vAffected, adExecuteNoRecords);
	}
	catch(_com_error e)
	{
		MessageBox(e.Description());
	}
}

void CXTaskDlg::DeleteReceiver(DWORD dwTaskID)
{
	CXsvrDlg *pDlg = (CXsvrDlg*)AfxGetMainWnd();
	
	char *sql="DELETE FROM task_receiver WHERE x_task_id='%d'";
	CString strSQL;
	strSQL.Format(sql, dwTaskID);

	try
	{
		_variant_t vAffected;
		pDlg->m_pConn->CursorLocation = adUseClient;
		pDlg->m_pConn->Execute((LPCTSTR)strSQL, &vAffected, adExecuteNoRecords);
	}
	catch(_com_error e)
	{
		MessageBox(e.Description(), "数据库错误", MB_ICONINFORMATION|MB_OK);
	}
}

void CXTaskDlg::DeleteTask(DWORD dwTaskID)
{
	CXsvrDlg *pDlg = (CXsvrDlg*)AfxGetMainWnd();

	// 从用户界面里把那个用户删除
	int nItemCount = m_ctrlList.GetItemCount();
	for (int i=0; i<nItemCount; i++)
	{
		CString strItemText;
		strItemText = m_ctrlList.GetItemText(i, 0);
		if (atoi(strItemText) == dwTaskID)
		{
			m_ctrlList.DeleteItem(i);
			break;
		}
	}

	char *sql="DELETE FROM task_query WHERE x_task_id='%d'";
	CString strSQL;
	strSQL.Format(sql, dwTaskID);

	try
	{
		_variant_t vAffected;
		pDlg->m_pConn->CursorLocation = adUseClient;
		pDlg->m_pConn->Execute((LPCTSTR)strSQL, &vAffected, adExecuteNoRecords);
	}
	catch(_com_error e)
	{
		MessageBox(e.Description(), "数据库错误", MB_ICONINFORMATION|MB_OK);
	}
}

void CXTaskDlg::CheckForUpdate()
{
	CXsvrDlg *pDlg = (CXsvrDlg*)AfxGetMainWnd();

	CTime today;
	today = CTime::GetCurrentTime();
	CString strNow = today.Format(_T("%Y-%m-%d %H:%M:%S"));

	char *buf = (LPSTR)(LPCTSTR)strNow;
	XTask task;
	task.SetDate(buf);
	task.SetTime(buf);

	list<XTask*>::iterator iLast = m_listTask.end();
	for (list<XTask*>::iterator iTask = m_listTask.begin();
		iTask != iLast;)
	{
		// 是时候执行任务了。
		if ( (task.nDate > (*iTask)->nDate)
			| ((task.nDate == (*iTask)->nDate)&&task.nTime >= (*iTask)->nTime))
		{
			// 日期已过
			pDlg->AddErrorInfo((*iTask)->szContent);

			XTask *p = (*iTask);
			m_listTask.remove(*iTask++);

			SendTask(p->nID, p->szContent);
			DeleteTask(p->nID);

			if (p->nFrequency == 0)
			{
				CTime timeTomorrow(today.GetYear(),
					today.GetMonth(),
					today.GetDay()+1,
					today.GetHour(),
					today.GetMinute(),
					today.GetSecond());
				CString strTomorrow = timeTomorrow.Format(_T("%Y-%m-%d %H:%M:%S"));
				AddNewTask(strTomorrow, p->szContent, 0);
				pDlg->AddErrorInfo(strTomorrow);
			}
			else
			{
				DeleteReceiver(p->nID);
			}

			delete p;
		}
		else
		{
			++ iTask;
		}
	}

	// 任务完成后，重新启动计时器
	SetTimer(TIMER_SCAN_DATABASE, TIMER_SCAN_DATABASE_TEIM, NULL);
}

void CXTaskDlg::SendTask(DWORD dwTaskID, LPCTSTR szContent)
{
	CXsvrDlg *pDlg = (CXsvrDlg*)AfxGetMainWnd();

	// *****************************************************************
	// 读取消息接收者
	// *****************************************************************
	_RecordsetPtr pRs;

	char * szSQL = "SELECT * FROM task_receiver WHERE x_task_id='%d'";
	CString strSQL;
	strSQL.Format(szSQL, dwTaskID);

	variant_t var;
	CString str;

	try
	{
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->Open((LPCTSTR)strSQL,
			pDlg->m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

	//	AfxMessageBox(strSQL);
		// 如果数据库是空白的
		if (pRs->adoEOF)
		{
			pDlg->AddErrorInfo("暂无用户记录！");
			pRs->Close();
			pRs.Release();
			return;
		}
		else
		{
			// 读取并显示数据库里的用户信息		
			while (!pRs->adoEOF)
			{
				var = pRs->GetCollect(_variant_t("x_receiver"));
				if (var.vt != VT_NULL)
				{
					str = (LPCTSTR)_bstr_t(var);
					SendTaskToUser(str, szContent);
					CString strInfo;
					strInfo.Format("向用户 %s 发送定时消息：%s", str, szContent);
					pDlg->AddErrorInfo(strInfo);
				}

				pRs->MoveNext();
			}
		}
	}
	catch (_com_error e)
	{
		pDlg->AddErrorInfo(e.ErrorMessage());
	}

	pRs->Close();
	pRs.Release();
}

void CXTaskDlg::SendTaskToUser(LPCTSTR lpszUID, LPCTSTR lpszContent)
{
	LPCTSTR lpszSystem = "xx--11--";

	CXsvrDlg *pDlg = (CXsvrDlg*)AfxGetMainWnd();

	SOCKET sock = GetUserSocket(lpszUID);

	if (INVALID_SOCKET == sock)
	{
		// 用户不在线

		// 对消息进行记录
		pDlg->XDBMsgOffLine(lpszSystem, lpszUID, lpszContent);
	}
	else
	{
		// 用户在线
		XMSG xmsg;
		XMSG_ONLINE omsg;
		DWORD SendBytes;

		omsg.SetData(lpszSystem, lpszContent, strlen(lpszContent)+1);
		xmsg.m_nMessage = XEIMMSG_NEWMESSAGE;
		xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());

		LPPER_IO_OPERATION_DATA pIo = (LPPER_IO_OPERATION_DATA )GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA));
		ZeroMemory(&(pIo->Overlapped), sizeof(OVERLAPPED));
		memcpy(pIo->Buffer, xmsg.GetBuffer(), xmsg.GetBufferLength());
		pIo->DataBuf.buf = pIo->Buffer;
		pIo->DataBuf.len = xmsg.GetBufferLength();
		pIo->DataSize = xmsg.GetBufferLength();
		pIo->send = 1;
		if(SOCKET_ERROR == WSASend(sock, &(pIo->DataBuf), 1, &SendBytes, 0, &(pIo->Overlapped), NULL))
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				pDlg->AddErrorInfo("WSASend error7\n");
				return;
			}
		}

		// 对消息进行记录
		pDlg->XDBMsgOnLine(lpszSystem, lpszUID, lpszContent);
	}
}

SOCKET CXTaskDlg::GetUserSocket(LPCTSTR lpszUID)
{
	CXsvrDlg *pDlg = (CXsvrDlg*)AfxGetMainWnd();

	map<SOCKET, XUserInfo>::iterator imap;
	imap = pDlg->m_mapUserInfos.begin();

	map<SOCKET, XUserInfo>::iterator _END;
	_END = pDlg->m_mapUserInfos.end();

	// 搜索 收件人 的 套接字
	for (; imap!=_END; ++imap)
	{
		if (! strcmp(imap->second.GetUID(), lpszUID))
		{
			return imap->first;
		}
	}

	// 没找到的话，返回 INVALID_SOCKET
//	if (imap == _END)
//	{
		return INVALID_SOCKET;
//	}
}
