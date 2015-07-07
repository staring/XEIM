/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// client02Dlg.h : header file
//

#if !defined(AFX_CLIENT02DLG_H__30B9137D_CC8C_4377_9CF0_E0B7BD80A87C__INCLUDED_)
#define AFX_CLIENT02DLG_H__30B9137D_CC8C_4377_9CF0_E0B7BD80A87C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XChatDlg.h"
#include "XTreeCtrl.h"
#include "SystemTray.h"
#include "MyHyperLink.h"
#include "../XMSG/xtype.h"
#include "XPluginBar.h"	// Added by ClassView
#include "DLG_ERP_MESSAGE.h"
#include "DLG_ERP_SCHEDULE.h"
#include "DLG_ERP_PROJECT.h"
#include "DLG_ERP_WORK.h"
/////////////////////////////////////////////////////////////////////////////
// CClient02Dlg dialog


class CClient02Dlg : public CDialog
{
// Construction
public:
	void CheckForUpdate();
	CXPluginBar *m_pPluginBar;
	void ReConnect();
	BOOL SendSMS(LPCTSTR szRecv, LPCTSTR szSMS);
	void XCheckVersion();
	void XGetMsgOffLine();
	void AddDebugInfo(CString strInfo);
	LRESULT OnChildFire(WPARAM wparam,LPARAM lparam);
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	// 网络处理回调函数
	static void r(void* sock);
	static DLG_ERP_PROJECT* m_pDlg_ERP_Project;
	static DLG_ERP_SCHEDULE* m_pDlg_ERP_Schedule;
	static DLG_ERP_WORK* m_pDlg_ERP_Work;

	// 背景颜色
	CBrush m_brBk2;
	// 文本框背景颜色
	COLORREF m_editBKColor;
	COLORREF m_textcolor;

	void XSortUser(HTREEITEM hParent);
	BOOL Init();

	// 所有联系人的信息
	list<XUserInfo> m_listUserInfo;
	// 每个用户对应分配一个对话框
	map<HTREEITEM, CXChatDlg*> m_mapUserList;
	// 所有部门名称
	map<string, HTREEITEM> m_mapGroup;

	// 用户密码
	CString m_strUPSW;
	// 用户名
	CString m_strUID;
	// 用户昵称
	CString m_strUNickName;
	// 用户联系电话
	CString m_strCellNUM;

	// 系统消息对话框
	DLG_ERP_MESSAGE *m_erpDlg;

	// 最后的错误显示
	CString m_strLastError;

	//系统托盘
	CSystemTray m_sysTray;
//	CStdioFile m_inifile;
	CString m_strSvr;
	SOCKET m_sock;
	HTREEITEM m_hRoot;

	typedef struct
	{
		HWND hWnd;
		SOCKET sock;
	}TDATA, *LPTDATA;

	CClient02Dlg(CWnd* pParent = NULL);	// standard constructor
	~CClient02Dlg();

// Dialog Data
	//{{AFX_DATA(CClient02Dlg)
	enum { IDD = IDD_CLIENT02_DIALOG };
	CXTreeCtrl	m_xtreeUser;
	CString	m_strDebug;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClient02Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bLogedin;
	BOOL m_bNetWorkInit;
	DWORD m_dwOnLine;
	CXChatDlg *m_pGroupMsg;
	HICON m_hIcon;
	// 在线用户 treectrl 图标
	CImageList	m_ilUser;
	// [设置] 按钮
	CMyHyperLink	m_staticSettings;
	CMyHyperLink	m_staticSendSMS;

	//[菜单]按钮
	CMyHyperLink  m_staticMenu;

	// Generated message map functions
	//{{AFX_MSG(CClient02Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT XOnUserOK(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XOnUserBad(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XOnUserInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XOnDisconnected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XOnUserList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XOnListEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XOnUserStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XMsgRecvOffLine(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XOnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XOnTotalContact(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XOnFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XMsgHistory(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XMsgNoHistory(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg void OnDblclkUserlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTrayRestore();
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRclickUserlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	afx_msg void OnButton1();
	afx_msg void OnMenuitemLogout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnQuit();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	afx_msg void OnHelp();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENT02DLG_H__30B9137D_CC8C_4377_9CF0_E0B7BD80A87C__INCLUDED_)
