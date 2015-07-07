/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#if !defined(AFX_DLG_ERP_NEW_PROJECT_H__A7686DE8_0D51_4C85_B2F1_9103232E7E31__INCLUDED_)
#define AFX_DLG_ERP_NEW_PROJECT_H__A7686DE8_0D51_4C85_B2F1_9103232E7E31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLG_ERP_NEW_PROJECT.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DLG_ERP_NEW_PROJECT dialog

class DLG_ERP_NEW_PROJECT : public CDialog
{
// Construction
public:
	void InitUI();
	BOOL m_bModify;

	COLORREF m_editBKColor;
	COLORREF m_textcolor;
	CBrush m_brBk2;

	CString m_strProject;
	CString m_strNumber;
	CString m_strDetail;
	CString m_strCreator;

	DLG_ERP_NEW_PROJECT(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DLG_ERP_NEW_PROJECT)
	enum { IDD = IDD_PROJECT_ADD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DLG_ERP_NEW_PROJECT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DLG_ERP_NEW_PROJECT)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_ERP_NEW_PROJECT_H__A7686DE8_0D51_4C85_B2F1_9103232E7E31__INCLUDED_)
