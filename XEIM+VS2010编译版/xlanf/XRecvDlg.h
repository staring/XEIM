#if !defined(AFX_XRECVDLG_H__A958AC93_368B_4586_9874_92F05323AAE9__INCLUDED_)
#define AFX_XRECVDLG_H__A958AC93_368B_4586_9874_92F05323AAE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XRecvDlg.h : header file
//
#include "MyHyperLink.h"
#include "xdlldef.h"
#include "XFrame.h"

/////////////////////////////////////////////////////////////////////////////
// CXRecvDlg dialog

class CXRecvDlg : public CDialog
{
// Construction
public:
	void SendRefuseMsg();
	void CancelFile(DWORD dwPort);
	// 文件接收回调函数
	static DWORD WINAPI ProcRecvFile(LPVOID lParam);

	LPSTR FileSaveAs();
	CBrush m_brBkColor;
	void AddFile(LPXFILEINFO pFileInfo);
	XFILEINFO m_xFileInfo;
	DWORD xxxtmp;
	DWORD m_dwID;
	DWORD m_dwHeight;
	SOCKET m_sockListen;
	DWORD m_dwFileLength;
	DWORD m_dwRecvLength;

	// 线程开关
	DWORD m_bRunning;
	void AdjustWindow(DWORD dwDlgID);
	CXRecvDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXRecvDlg)
	enum { IDD = IDD_RECV };
	CMyHyperLink	m_staticSaveAs;
	CMyHyperLink	m_staticSave;
	CMyHyperLink	m_staticRefuse;
	CMyHyperLink	m_staticCancel;
	CProgressCtrl	m_process;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXRecvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXRecvDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnChildFire(WPARAM wparam,LPARAM lparam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnDestCancel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecvCancel(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XRECVDLG_H__A958AC93_368B_4586_9874_92F05323AAE9__INCLUDED_)
