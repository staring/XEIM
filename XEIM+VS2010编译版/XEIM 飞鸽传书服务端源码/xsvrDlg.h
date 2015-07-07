/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// xsvrDlg.h : header file
//

#if !defined(AFX_XSVRDLG_H__BBBF4633_43E9_4615_BBCE_68DA3C0F4D80__INCLUDED_)
#define AFX_XSVRDLG_H__BBBF4633_43E9_4615_BBCE_68DA3C0F4D80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../xmsg/xtype.h"
#include "xserver.h"
#include "XTreeCtrl.h"
#include "SystemTray.h"
#include "XSplitDlg.h"
#include "XTaskDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CXsvrDlg dialog
//class SerialThread;
class CXsvrApp;

class CXsvrDlg : public CDialog
{
// Construction
public:
	BOOL _UI_SetProductInfo();
	LPCTSTR _ERP_GetUIDByName(LPCTSTR lpszName);
	BOOL _ERP_SendMessageToUserByName(LPCTSTR szName, LPCTSTR szMessage);
	void _ERP_SendMessageToUserList(CString strUsers, CString strMessage);
	// 把字符串时间转换成 CTime 时间
	void _ERP_StringToCTime(IN CString str, OUT CTime &ctm);
	void _ERP_INIT();
	void _ERP_CheckDatabase();
	void XUser_SendStatus();
	void XUser_SendContact();
//	void XCheckSMSList();


	// SMS 处理 ////////////////////////////////////////////////////////////
//	SerialThread* serialProcess;
	// SMS 线程是否正常
/*	BOOL m_bSMSActiveProccess;
	// COM1 端口正常
	DWORD m_dwSMSStep;
	CString	m_monitorSend;
	CString m_strSMSCenter;
	CString m_strCOMX;
	list<string> m_listSMS;
	// 发送状态
	BOOL m_bSuccessed;*/
	// SMS 处理 ////////////////////////////////////////////////////////////




	// ERP 处理 //////////////////////////////////////////////////////////////////////////
	DWORD m_dw_ERP_MONTH;
	DWORD m_dw_ERP_CURRENT_NUMBER;
	// ERP 处理 //////////////////////////////////////////////////////////////////////////
	


	// 用户联系人列表处理计时器
	BOOL m_bSendUserContact;
	list<SOCKET> m_listContactQuery;

	// 用户状态信息
	list<XUserInfo> m_listUserStatus;
	BOOL m_bSendUserStatus;

	// 线程同步
	CXsvrApp *m_pApp;

	void GetUIDMobileNUM(LPCTSTR lpszUID, CString &strNum);
	DWORD SendDataToClient(SOCKET sock, char *buf, DWORD len);
	// 重置 SOCKET 状态
	BOOL ResetSockState(SOCKET sock, LPPER_IO_OPERATION_DATA PerIoData);
	// 把在线消息添加到历史记录里
	void XDBMsgOnLine(LPCTSTR lpszSendUID, LPCTSTR lpszRecvUID, LPCTSTR lpszMsgBody);
	// 把离线消息添加到历史记录里
	void XDBMsgOffLine(LPCTSTR lpszSendUID, LPCTSTR lpszRecvUID, LPCTSTR lpszMsgBody);
	// 把短信消息记录到数据库
	void XDBMsgSMS(LPCTSTR lpszSendUID, LPCTSTR lpszRecvUID, LPCTSTR lpszMsgBody);

	// 用户登出 XEIM 后对数据库的处理
	void XDBUserLogout(LPCTSTR lpszUID);
	// 用户登录 XEIM 后对数据库的处理
	void XDBUserLogin(LPCTSTR lpszUID, LPCTSTR lpszVersion, LPCTSTR lpszIP);

	// 初始化程序对数据库进行读取
	void XDB_InitUserList();
	// 读取一个用户的详细信息
	void XDB_ReadUserDetail(LPCTSTR lpszUID, XUserDetail &xDetail);

	void XCheckUserInfo();
	void AddErrorInfo(CString strErr);
	_ConnectionPtr m_pConn;					// 数据库连接
	list<XUserDetail> m_listUserDetail;		// 用户详细信息
	map<SOCKET, XUserInfo> m_mapUserInfos;	// 在线用户摘要信息
	CImageList	m_ilUser;					// 在线用户 CTreeCtrl 图标

	BOOL XUserExisted(const char* szUID);
	void XUserAddNew(XUserDetail &detail, CString strMobile);
	void XUserUpdate(XUserDetail &detail, CString strMobile);
	BOOL XSendMsgToUser(const char* szUID, const char* szMSG);
	void XSendUserStatusToFriends(XUserInfo &xui);
	CXsvrDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXsvrDlg)
	enum { IDD = IDD_XSVR_DIALOG };
	CEdit	m_errOut;
	CListCtrl	m_lcUserList;
	CXTreeCtrl	m_tcUserTree;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXsvrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	XSplitDlg	*m_pSplitDlg;				// 窗口拆分
	xserver m_xserver;						// 服务器网络线程
//	CXTaskDlg m_dlgTask;					// 任务管理窗口

	HICON m_hIcon;
	HTREEITEM m_hRoot;						// 在线用户

	CString m_strDatabase;					// 数据库位置

	CSystemTray m_sysTray;					//系统托盘

	// Generated message map functions
	//{{AFX_MSG(CXsvrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT XOnConnected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XGetMsgOffLine(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XGetMsgHistory(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XMsgHistoryData(WPARAM wParam, LPARAM lParam);
//	afx_msg void XOnNormalSMS(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XOnRecordSMS(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT _ERP_OnRecordProject(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT _ERP_OnGetProject(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT _ERP_OnGetSchedule(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XEIM_USER_Login(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XMsgOnLine(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XMsgOffLine(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XMsgOffLineData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XOnLogoff(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XOnGetList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT XOnNewFile(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGetDispInfo (NMHDR* pnmhdr, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnTrayRestore();
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMenuabout();
	afx_msg void OnMenuquit();
	afx_msg void OnMenuUser();
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMenuTask();
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMsgSystem();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSVRDLG_H__BBBF4633_43E9_4615_BBCE_68DA3C0F4D80__INCLUDED_)
