/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#if !defined(AFX_DLG_ERP_SCHEDULE_H__62B21B31_D46C_41B1_A369_732DA760EBF0__INCLUDED_)
#define AFX_DLG_ERP_SCHEDULE_H__62B21B31_D46C_41B1_A369_732DA760EBF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLG_ERP_SCHEDULE.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DLG_ERP_SCHEDULE dialog

class DLG_ERP_SCHEDULE : public CDialog
{
// Construction
public:
	void InitUI();
	DLG_ERP_SCHEDULE(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DLG_ERP_SCHEDULE)
	enum { IDD = IDD_SCHEDULE };
	CString	m_strSubject;
	CString	m_strType;
	CString	m_strContent;
	CString	m_strMembers;
	CString m_strNumber; // 项目编号
	CString m_strProject; // 项目名称
	BOOL	m_bPublic;

	BOOL m_bModify;		// 修改工作还是新建工作
	// 文本框背景颜色
	COLORREF m_editBKColor;
	COLORREF m_textcolor;
	// 背景颜色
	CBrush m_brBk2;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DLG_ERP_SCHEDULE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DLG_ERP_SCHEDULE)
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_ERP_SCHEDULE_H__62B21B31_D46C_41B1_A369_732DA760EBF0__INCLUDED_)
