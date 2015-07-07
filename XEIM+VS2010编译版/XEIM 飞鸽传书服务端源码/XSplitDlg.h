/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#if !defined(AFX_XSPLITDLG_H__A775F1C5_116D_4193_A661_8E11803296F6__INCLUDED_)
#define AFX_XSPLITDLG_H__A775F1C5_116D_4193_A661_8E11803296F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XSplitDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// XSplitDlg dialog

class XSplitDlg : public CDialog
{
// Construction
public:
	void SetTowWindow(CWnd *pWnd1, CWnd *pWnd2, DWORD dwLeftMin);
	BOOL m_bTracking;
	CBrush m_brBk1;
	CBrush m_brBk2;
	CBrush m_brBk3;
	BOOL m_bCaptured;
	CPoint m_ptDown;
	CWnd *m_wndLeft;
	DWORD m_nLeftMin;
	CWnd *m_wndRight;
	XSplitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(XSplitDlg)
	enum { IDD = IDD_SPLITDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(XSplitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(XSplitDlg)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam) ;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSPLITDLG_H__A775F1C5_116D_4193_A661_8E11803296F6__INCLUDED_)
