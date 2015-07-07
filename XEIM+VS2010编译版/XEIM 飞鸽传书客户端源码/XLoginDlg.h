/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#if !defined(AFX_XLOGINDLG_H__FE84C3F0_40F6_4F2B_BB81_5C0C6A6E4FA5__INCLUDED_)
#define AFX_XLOGINDLG_H__FE84C3F0_40F6_4F2B_BB81_5C0C6A6E4FA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XLoginDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CXLoginDlg dialog

class CXLoginDlg : public CDialog
{
// Construction
public:
	void UpdateIniFile(LPCTSTR lpszUser, LPCTSTR lpszPass);
	CXLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXLoginDlg)
	enum { IDD = IDD_DIALOG1 };
	BOOL	m_bRemember;
	BOOL	m_bAuto;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CBrush m_brush;

	// Generated message map functions
	//{{AFX_MSG(CXLoginDlg)
	afx_msg void OnSetting();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnDestroy();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XLOGINDLG_H__FE84C3F0_40F6_4F2B_BB81_5C0C6A6E4FA5__INCLUDED_)
