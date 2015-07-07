/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#if !defined(AFX_NORMALSMS_H__EF91CA6F_62F4_4CE9_9D3C_DCB282248E7A__INCLUDED_)
#define AFX_NORMALSMS_H__EF91CA6F_62F4_4CE9_9D3C_DCB282248E7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NormalSMS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNormalSMS dialog

class CNormalSMS : public CDialog
{
// Construction
public:
	BOOL IsPhoneNumber(CString strNUM);
	LPSTR ChangeCODE(LPCTSTR lpText);
	CNormalSMS(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNormalSMS)
	enum { IDD = IDD_NORMAL_SMS };
	CString	m_strRecv;
	CString	m_strSign;
	CString	m_strSMS;
	// 背景颜色
	CBrush m_brBk2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNormalSMS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNormalSMS)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NORMALSMS_H__EF91CA6F_62F4_4CE9_9D3C_DCB282248E7A__INCLUDED_)
