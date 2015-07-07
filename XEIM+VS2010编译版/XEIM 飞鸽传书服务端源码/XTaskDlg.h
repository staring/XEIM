/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#if !defined(AFX_XTASKDLG_H__4E25B26A_9D48_493D_A2D8_3D02EC61FFC1__INCLUDED_)
#define AFX_XTASKDLG_H__4E25B26A_9D48_493D_A2D8_3D02EC61FFC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XTaskDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXTaskDlg dialog
#define TIMER_SCAN_DATABASE			22
#define TIMER_SCAN_DATABASE_TEIM	2000	// 5 秒

#define TIMER_DATE_TIME				11


/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
class XTask
{
public:
	BOOL SetTime(LPCTSTR szTime);
	BOOL SetDate(LPCTSTR szDate);
	char *szContent;
	int nID;
	int nDate;
	int nTime;
	int nFrequency;

	XTask();
	~XTask();
};

class CXTaskDlg : public CDialog
{
// Construction
public:
	SOCKET GetUserSocket(LPCTSTR lpszUID);
	void SendTaskToUser(LPCTSTR lpszUID, LPCTSTR lpszContent);
	void SendTask(DWORD dwTaskID, LPCTSTR szContent);
	void CheckForUpdate();
	
	void DeleteTask(DWORD dwTaskID);
	void DeleteReceiver(DWORD dwTaskID);

	DWORD m_nUniqueID;
	list <XTask*> m_listTask;				// 任务列表
	// 所有部门名称
	map<string, HTREEITEM> m_mapGroup;

	void AddNewContact(LPCTSTR szUID);
	void Init_CtrlList();
	DWORD CheckForChanging(HTREEITEM hParent);
	void SetItemCheck(HTREEITEM hParent, BOOL bCheck);
	void SetParentCheck(HTREEITEM hItem);
	void SetParentCheck2(HTREEITEM hItem);
	BOOL AddNewTask(CString strTime, CString strContent, DWORD dwFrequency);
	CXTaskDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXTaskDlg)
	enum { IDD = IDD_DIALOG1 };
	CDateTimeCtrl	m_ctrlTime;
	CDateTimeCtrl	m_ctrlDate;
	CTreeCtrl	m_ctrlTree;
	CListCtrl	m_ctrlList;
	CComboBox	m_ctrlComboBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTaskDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXTaskDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XTASKDLG_H__4E25B26A_9D48_493D_A2D8_3D02EC61FFC1__INCLUDED_)
