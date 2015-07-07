#if !defined(AFX_XSENDDLG_H__FA93385A_0F2B_4843_B88E_538CAC462E14__INCLUDED_)
#define AFX_XSENDDLG_H__FA93385A_0F2B_4843_B88E_538CAC462E14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XSendDlg.h : header file
//
#include "MyHyperLink.h"
#include "xdlldef.h"

/////////////////////////////////////////////////////////////////////////////
// CXSendDlg dialog

class CXSendDlg : public CDialog
{
// Construction
public:
	void RefuseFile(DWORD dwPort);
	void SendCancelMsg();

	void WaitingForDownload(LPXFILEINFO pFileInfo);

	// 文件发送回调函数
	static DWORD WINAPI ProcSendFile(LPVOID lParam);

	void AddFile(LPXFILEINFO pFileInfo);

	XFILEINFO m_xFileInfo;

	// 文件长度
	DWORD m_dwFileLength;
	// 当前发送的长度
	DWORD m_dwFileProcess;
	// 线程开关
	DWORD m_bRunning;
	SOCKET m_sockListen;

	DWORD xxxtmp;

	DWORD m_dwID;
	CBrush m_brBkColor;
	DWORD m_dwHeight;
	void AdjustWindow(DWORD dwDlgID);
	CXSendDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXSendDlg)
	enum { IDD = IDD_SEND };
	CMyHyperLink	m_staticCancel;
	CProgressCtrl	m_process;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXSendDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXSendDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnChildFire(WPARAM wparam,LPARAM lparam);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnXDestAcceptFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileProcess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSendNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSendCancel(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSENDDLG_H__FA93385A_0F2B_4843_B88E_538CAC462E14__INCLUDED_)
