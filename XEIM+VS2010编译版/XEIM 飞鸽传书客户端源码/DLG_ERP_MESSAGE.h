/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#if !defined(AFX_DLG_ERP_MESSAGE_H__92ED709D_A16D_4197_840D_99D0D5AA7FF6__INCLUDED_)
#define AFX_DLG_ERP_MESSAGE_H__92ED709D_A16D_4197_840D_99D0D5AA7FF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLG_ERP_MESSAGE.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DLG_ERP_MESSAGE dialog

class DLG_ERP_MESSAGE : public CDialog
{
// Construction
public:
	vector<string> m_vecMessage;
	vector<string>::iterator m_iPointer;
	void SetMessage(CString strMsg);
	DLG_ERP_MESSAGE(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DLG_ERP_MESSAGE)
	enum { IDD = IDD_DIALOG7 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DLG_ERP_MESSAGE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DLG_ERP_MESSAGE)
	virtual void OnOK();
	afx_msg void OnBack();
	afx_msg void OnNext();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_ERP_MESSAGE_H__92ED709D_A16D_4197_840D_99D0D5AA7FF6__INCLUDED_)
