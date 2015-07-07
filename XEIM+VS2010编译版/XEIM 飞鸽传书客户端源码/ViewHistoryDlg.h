/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#if !defined(AFX_VIEWHISTORYDLG_H__187D44B3_6CBE_4CE3_8FDD_AB60573202C8__INCLUDED_)
#define AFX_VIEWHISTORYDLG_H__187D44B3_6CBE_4CE3_8FDD_AB60573202C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewHistoryDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewHistoryDlg dialog

class CViewHistoryDlg : public CDialog
{
// Construction
public:
	void AdjustSize(CWnd *pWnd, int x, int y, int nStyle);
	void XNoHistoryMsg(LPCTSTR szUID);
	void XAddHistoryMsg(LPCTSTR szMessage, LPCTSTR szUID, LPCTSTR szTime);
	CTime m_timeToday;
	void InitSetting(CString &strUNickName, CString &strUID, SOCKET sock);
	CString m_strUID;
	CString m_strUNickName;
	SOCKET m_sock;
	CViewHistoryDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewHistoryDlg)
	enum { IDD = IDD_HISTORY_VIEW };
	CRichEditCtrl	m_richMsg;
	CDateTimeCtrl	m_dateCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewHistoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CPoint m_ptOldXY;
	// 背景颜色
	CBrush m_brBk2;
	HICON m_hIcon;
	CPoint m_ptMinSize;

	// Generated message map functions
	//{{AFX_MSG(CViewHistoryDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBack();
	afx_msg void OnFront();
	afx_msg void OnCloseupDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWHISTORYDLG_H__187D44B3_6CBE_4CE3_8FDD_AB60573202C8__INCLUDED_)
