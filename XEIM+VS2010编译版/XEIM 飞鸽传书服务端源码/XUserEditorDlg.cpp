/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// XUserEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xsvr.h"
#include "XUserEditorDlg.h"
#include "xsvrDlg.h"
#include "../xmsg/xtype.h"
#include <string>
#include <algorithm>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXUserEditorDlg dialog


CXUserEditorDlg::CXUserEditorDlg(CWnd* pParent /*=NULL*/)
: CDialog(CXUserEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXUserEditorDlg)
	m_strUID = _T("");
	m_strPassword = _T("");
	m_strNickName = _T("");
	m_strGroup = _T("");
	m_bModify = FALSE;
	m_strUMobile = _T("");
	//}}AFX_DATA_INIT
}


void CXUserEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXUserEditorDlg)
	DDX_Control(pDX, IDC_TREE1, m_treeUserList);
	DDX_Text(pDX, IDC_EDIT1, m_strUID);
	DDX_Text(pDX, IDC_EDIT2, m_strPassword);
	DDX_Text(pDX, IDC_EDIT3, m_strNickName);
	DDX_Text(pDX, IDC_EDIT4, m_strGroup);
	DDX_Text(pDX, IDC_EDIT_CELL, m_strUMobile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXUserEditorDlg, CDialog)
//{{AFX_MSG_MAP(CXUserEditorDlg)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, OnClickTree1)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE1, OnItemexpandingTree1)
	ON_BN_CLICKED(IDC_CHECK_ALL, OnCheckAll)
	ON_BN_CLICKED(IDC_CHECK_INVERSE, OnCheckInverse)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CXUserEditorDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXUserEditorDlg message handlers

void CXUserEditorDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	
	CXsvrDlg *pDlg = (CXsvrDlg*)AfxGetMainWnd();
	
	if (! m_bModify)
	{
		if (m_strUID.IsEmpty())
		{
			GetDlgItem(IDC_EDIT1)->SetFocus();
			AfxMessageBox("用户ID必须填写");
			return;
		}
		
		if (pDlg->XUserExisted((LPCTSTR)m_strUID))
		{
			GetDlgItem(IDC_EDIT1)->SetFocus();
			AfxMessageBox("用户ID已存在，请重新输入。");
			return;
		}
		
		if (m_strPassword.IsEmpty())
		{
			GetDlgItem(IDC_EDIT2)->SetFocus();
			AfxMessageBox("用户密码必须填写");
			return;
		}
		
		if (m_strNickName.IsEmpty())
		{
			m_strNickName = m_strUID;
		}

		if (m_strGroup.IsEmpty())
		{
			m_strGroup = "其他";
		}

		XUserDetail xUser;
		xUser.SetUID((LPCTSTR)m_strUID);
		xUser.SetUNickName((LPCTSTR)m_strNickName);
		xUser.SetUPassword((LPCTSTR)m_strPassword);
		xUser.SetUGroup((LPCTSTR)m_strGroup);

		// 交回给主程序处理
		pDlg->XUserAddNew(xUser, m_strUMobile);
	}
	else
	{
		XUserDetail xUser;
		xUser.SetUID((LPCTSTR)m_strUID);
		xUser.SetUNickName((LPCTSTR)m_strNickName);
		xUser.SetUPassword((LPCTSTR)m_strPassword);
		xUser.SetUGroup((LPCTSTR)m_strGroup);
		
		// 交回个主程序处理
		pDlg->XUserUpdate(xUser, m_strUMobile);
	}
	
	map<string, HTREEITEM>::iterator iGroup;
	for (iGroup = m_mapGroup.begin();
	iGroup != m_mapGroup.end(); iGroup ++)
	{
		CheckForChanging(iGroup->second);
	}
	
	string str;
	if (m_listContact.size() != 0)
	{
		int nSize = m_listContact.size();
		for (int i=0; i<nSize; i++)
		{
			str = m_listContact.front();
			m_listContact.pop_front();
			DeleteContact(str.c_str());
		}
	}
	
	m_listContact.clear();
	m_mapGroup.clear();
	CDialog::OnOK();
}

void CXUserEditorDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CXUserEditorDlg::SetData(XUserDetail &uDetail)
{
	m_strGroup = uDetail.GetUGroup();
	m_strNickName = uDetail.GetUNickName();
	m_strPassword = uDetail.GetUPassword();
	m_strUID = uDetail.GetUID();
	
	m_bModify = TRUE;
	
	return TRUE;
}

BOOL CXUserEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if (m_bModify)
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		SetDlgItemText(IDOK, "修改(&M)");
	}
	
	m_treeUserList.ModifyStyle(0,TVS_CHECKBOXES);
	m_treeUserList.SetItemHeight(16);
	
	// 获取用户的联系人
	GetUserContact();
	
	CXsvrDlg *pDlg = (CXsvrDlg*)GetParent();
	// 用户 CTreeCtrl 图标
	m_treeUserList.SetImageList(& pDlg->m_ilUser, TVSIL_NORMAL);

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
			hGroup = m_treeUserList.InsertItem(xui.GetUGroup(), 1, 1);
			m_mapGroup.insert(pair<string,HTREEITEM>(xui.GetUGroup(), hGroup));
		}
		else // 该组已存在
		{
			hGroup = iGroup->second;
		}
		// 插入UI
		hItem = m_treeUserList.InsertItem(xui.GetUID(), 2, 2, hGroup);
		iContact = find(m_listContact.begin(), m_listContact.end(), xui.GetUID());
		
		if (iContact != m_listContact.end())
		{
			m_treeUserList.SetCheck(hItem);
			//	SetParentCheck(hItem);
		}
		
	}
	for (iGroup = m_mapGroup.begin(); iGroup != m_mapGroup.end(); ++iGroup)
	{
		HTREEITEM hChild = m_treeUserList.GetChildItem(iGroup->second);
		/*	CString strc;
		strc.Format("%s %d", iGroup->first.c_str(), hChild);
		AfxMessageBox(strc);
		*/
		//	m_treeUserList.Expand(iGroup->second, TVE_EXPAND);
		SetParentCheck(hChild);
	}
	
	//	CString strbb;
	//	strbb.Format("%d", m_mapGroup.size());
	//	AfxMessageBox(strbb);
	// 对该用户的好友进行打勾
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CXUserEditorDlg::OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	UINT flag;
	BOOL bRoot;
	
	HTREEITEM hItem;
	GetCursorPos(&pt);
	m_treeUserList.ScreenToClient(&pt);   
	
	hItem = m_treeUserList.HitTest(pt, &flag);
	
	if (hItem == NULL)
	{
		*pResult = 0;
		return;
	}
	
	CString itemText = m_treeUserList.GetItemText(hItem);
	
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
			if (! m_treeUserList.GetCheck(hItem))
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
			if (m_treeUserList.GetCheck(hItem))
			{
				//			AfxMessageBox("aaaaa");
				HTREEITEM hParent = m_treeUserList.GetParentItem(hItem);
				if (m_treeUserList.GetCheck(hParent))
					m_treeUserList.SetCheck(hParent, FALSE);
			}
			else
			{
				SetParentCheck2(hItem);
			}
		}
		// 只有当用户选择方框时，才需要调用此函数，下面代码不需要。
		m_treeUserList.SelectItem(hItem);
	}
	else if (flag & (TVHT_ONITEMICON | TVHT_ONITEMLABEL))
	{
		if (bRoot)
		{
			if (m_treeUserList.GetCheck(hItem))
			{
				m_treeUserList.SetCheck(hItem, FALSE);
				
				SetItemCheck(hItem, FALSE);
			}
			else
			{
				m_treeUserList.SetCheck(hItem);
				
				SetItemCheck(hItem, TRUE);
			}
		}
		else
		{
			if (m_treeUserList.GetCheck(hItem))
			{
				HTREEITEM hParent = m_treeUserList.GetParentItem(hItem);
				if (hParent == NULL)
					return;
				if (m_treeUserList.GetCheck(hParent))
					m_treeUserList.SetCheck(hParent, FALSE);
				
				m_treeUserList.SetCheck(hItem, FALSE);
				//	AfxMessageBox(m_treeUserList.GetItemText(hParent));
			}
			else
			{
				m_treeUserList.SetCheck(hItem);
				SetParentCheck(hItem);
			}
		}
	}
	
	*pResult = 0;
}


void CXUserEditorDlg::SetItemCheck(HTREEITEM hParent, BOOL bCheck)
{
	HTREEITEM hChild = m_treeUserList.GetChildItem(hParent);
	
	m_treeUserList.SetCheck(hChild, bCheck);
	
	while(hChild = m_treeUserList.GetNextSiblingItem(hChild))
	{
		m_treeUserList.SetCheck(hChild, bCheck);
	}
}


void CXUserEditorDlg::SetParentCheck(HTREEITEM hItem)
{
	HTREEITEM hParent;
	BOOL bCheckParent = FALSE;

	if (hItem != NULL)
	{
		hParent = m_treeUserList.GetParentItem(hItem);
		HTREEITEM hChild = m_treeUserList.GetChildItem(hParent);

		while (hChild)
		{
			if (! m_treeUserList.GetCheck(hChild))
				break;
			hChild = m_treeUserList.GetNextSiblingItem(hChild);
		}
		if (hChild == NULL)
		{
			m_treeUserList.SetCheck(hParent);
		}
	}
}

void CXUserEditorDlg::SetParentCheck2(HTREEITEM hItem)
{
	HTREEITEM hParent;
	int i=0;
	BOOL bCheckParent = FALSE;

	if (hItem != NULL)
	{
		hParent = m_treeUserList.GetParentItem(hItem);
		HTREEITEM hChild = m_treeUserList.GetChildItem(hParent);

		while (hChild)
		{
			if (hChild == hItem)
			{
				hChild = m_treeUserList.GetNextSiblingItem(hChild);
				continue;
			}
			if (! m_treeUserList.GetCheck(hChild))
				break;

			hChild = m_treeUserList.GetNextSiblingItem(hChild);
		}
		if (hChild == NULL)
		{
			m_treeUserList.SetCheck(hParent);
		}
	}
}

void CXUserEditorDlg::OnItemexpandingTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (TVE_EXPAND == pNMTreeView->action)
		m_treeUserList.SetItemImage(pNMTreeView->itemNew.hItem, 0, 0);
	else
		m_treeUserList.SetItemImage(pNMTreeView->itemNew.hItem, 1, 1);
	*pResult = 0;
}

// 获取用户好友信息
void CXUserEditorDlg::GetUserContact()
{
	// 主框架句柄
	CXsvrDlg *pDlg = (CXsvrDlg*)GetParent();
	// SQL 查询
	char * szSQL = "SELECT * FROM x_contact where uid='%s'";
	char buf[512];
	sprintf(buf, szSQL, (LPCTSTR)m_strUID);
	
	// ADO 操作模块
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));
	
	// 执行相应处理，避免表 为空时的异常
	try
	{
		pRs->Open(buf,
			pDlg->m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		
		// 如果数据库是空白的
		if (! pRs->adoEOF)
		{
			// 读取并显示数据库里的信息
			
			while (!pRs->adoEOF)
			{
				variant_t var;
				
				// 获取用户昵称
				var = pRs->GetCollect(_variant_t("xucontact"));
				if (var.vt != VT_NULL)
				{
					m_listContact.push_back((LPCTSTR)_bstr_t(var));
					//		AfxMessageBox((LPCTSTR)_bstr_t(var));
				}
				pRs->MoveNext();
			}
		}
		else
		{
			//			AfxMessageBox("联系人数据表无任何记录！");
		}
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	
	pRs->Close();
	pRs.Release();
} // end 获取用户好友信息


void CXUserEditorDlg::CheckForChanging(HTREEITEM hParent)
{
	HTREEITEM hChild = m_treeUserList.GetChildItem(hParent);
	list<string>::iterator iContact;
	
	string str = m_treeUserList.GetItemText(hChild);
	// 如果打勾了
	if (m_treeUserList.GetCheck(hChild))
	{
		iContact = find(m_listContact.begin(), m_listContact.end(), str);
		// 该好友已存在
		if (iContact != m_listContact.end())
		{
			// 把它从联系人列表里删除
			m_listContact.erase(iContact);
		}
		else
		{
			AddNewContact(str.c_str());
		}
	}
	
	while(hChild = m_treeUserList.GetNextSiblingItem(hChild))
	{
		str = m_treeUserList.GetItemText(hChild);
		// 如果打勾了
		if (m_treeUserList.GetCheck(hChild))
		{
			string str = m_treeUserList.GetItemText(hChild);
			iContact = find(m_listContact.begin(), m_listContact.end(), str);
			// 该好友已存在
			if (iContact != m_listContact.end())
			{
				// 把它从联系人列表里删除
				m_listContact.erase(iContact);
			}
			else
			{
				AddNewContact(str.c_str());
			}
		}
	}
}

// 把联系人删除
void CXUserEditorDlg::DeleteContact(LPCTSTR szUID)
{
	char buf[256];
	
	// 主框架句柄
	CXsvrDlg *pDlg = (CXsvrDlg*)GetParent();
	// SQL 查询
	sprintf(buf, "DELETE FROM x_contact WHERE xucontact='%s' AND uid='%s'", szUID, (LPCTSTR)m_strUID);

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

// 添加新的联系人
void CXUserEditorDlg::AddNewContact(LPCTSTR szUID)
{
	char buf[256];
	
	// 主框架句柄
	CXsvrDlg *pDlg = (CXsvrDlg*)GetParent();
	// SQL 查询
	sprintf(buf, "INSERT INTO x_contact(uid,xucontact) VALUES('%s','%s')",
		(LPCTSTR)m_strUID, szUID);
	//	AfxMessageBox(buf);
	
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

	// 判断我是否也是对方的好友
	sprintf(buf, "SELECT * FROM x_contact WHERE uid='%s' AND xucontact='%s'", szUID, (LPCTSTR)m_strUID);
	
	// ADO 操作模块
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));
	
	try
	{
		pRs->Open(buf,
			pDlg->m_pConn.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
		
		// 如果数据库是空白的
		if (pRs->adoEOF)
		{
			//	AfxMessageBox("aaaa");
			pRs->AddNew(); 
			pRs->Fields->GetItem("uid")->Value=_variant_t(szUID);
			pRs->Fields->GetItem("xucontact")->Value=_variant_t((LPCTSTR)m_strUID); 
			pRs->Update();
			// 读取并显示数据库里的信息
			
			/*			while (!pRs->adoEOF)
			{
			variant_t var;
			
			  // 获取用户昵称
			  var = pRs->GetCollect(_variant_t("xucontact"));
			  if (var.vt != VT_NULL)
			  {
			  m_listContact.push_back((LPCTSTR)_bstr_t(var));
			  //		AfxMessageBox((LPCTSTR)_bstr_t(var));
			  }
			  pRs->MoveNext();
		}*/
		}
		else
		{
			//			AfxMessageBox("联系人数据表无任何记录！");
		}
	}
	catch(_com_error e)
	{
		MessageBox(e.Description());
	}
	pRs->Close();
	pRs.Release();
}

// 全选
void CXUserEditorDlg::OnCheckAll() 
{
	// TODO: Add your control notification handler code here
	if (((CButton*)GetDlgItem(IDC_CHECK_ALL))->GetCheck())
	{
		UncheckAll();
		InverseAll();
	}
	else
	{
		UncheckAll();
	}
}

// 反选
void CXUserEditorDlg::OnCheckInverse() 
{
	// TODO: Add your control notification handler code here
	if (((CButton*)GetDlgItem(IDC_CHECK_INVERSE))->GetCheck())
	{
		InverseAll();
	}
	else
	{
		InverseAll();
	}
}

void CXUserEditorDlg::UncheckAll()
{
	map<string, HTREEITEM>::iterator iGroup;
	for (iGroup = m_mapGroup.begin();
	iGroup != m_mapGroup.end(); iGroup ++)
	{
		if (m_treeUserList.GetCheck(iGroup->second))
		{
			m_treeUserList.SetCheck(iGroup->second, FALSE);
		}

		HTREEITEM hChild = m_treeUserList.GetChildItem(iGroup->second);

		string str = m_treeUserList.GetItemText(hChild);
		// 如果打勾了
		if (m_treeUserList.GetCheck(hChild))
		{
			m_treeUserList.SetCheck(hChild, FALSE);
		}

		while(hChild = m_treeUserList.GetNextSiblingItem(hChild))
		{
			str = m_treeUserList.GetItemText(hChild);
			// 如果打勾了
			if (m_treeUserList.GetCheck(hChild))
			{
				m_treeUserList.SetCheck(hChild, FALSE);
			}
		}
	}
}

void CXUserEditorDlg::InverseAll()
{
	BOOL bCheck;
	map<string, HTREEITEM>::iterator iGroup;
	map<string, HTREEITEM>::iterator _END;
	_END = m_mapGroup.end();

	for (iGroup = m_mapGroup.begin();
	iGroup != _END; iGroup ++)
	{
		bCheck = TRUE;
		HTREEITEM hChild = m_treeUserList.GetChildItem(iGroup->second);

		string str = m_treeUserList.GetItemText(hChild);
		// 如果打勾了
		if (m_treeUserList.GetCheck(hChild))
		{
			bCheck = FALSE;
			m_treeUserList.SetCheck(hChild, FALSE);
		}
		else
		{
			m_treeUserList.SetCheck(hChild);
		}

		while(hChild = m_treeUserList.GetNextSiblingItem(hChild))
		{
			str = m_treeUserList.GetItemText(hChild);
			// 如果打勾了
			if (m_treeUserList.GetCheck(hChild))
			{
				bCheck = FALSE;
				m_treeUserList.SetCheck(hChild, FALSE);
			}
			else
			{
				m_treeUserList.SetCheck(hChild);
			}
		}

		if (bCheck == FALSE)
		{
		//	AfxMessageBox(m_treeUserList.GetItemText(iGroup->second));
		}
		m_treeUserList.SetCheck(iGroup->second, bCheck);
	}
}

void CXUserEditorDlg::SetMobile(LPCTSTR szMobile)
{
	m_strUMobile = szMobile;
}

void CXUserEditorDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
