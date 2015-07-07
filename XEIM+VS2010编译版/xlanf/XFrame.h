#if !defined(AFX_XFRAME_H__E0145AE1_0CCE_4BCC_B0A7_7A0B57C6F7CA__INCLUDED_)
#define AFX_XFRAME_H__E0145AE1_0CCE_4BCC_B0A7_7A0B57C6F7CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XFrame.h : header file
//
#include "xdlldef.h"

#pragma warning(disable:4786)
#include <list>
#include <string>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CXFrame dialog

// 文件对话框及对话框类型
typedef struct
{
	CDialog *pDlg;
	BOOL	bSend;

} FILEDIALOG;


class CXFrame : public CDialog
{
// Construction
public:
	// 所有文件发送完毕
	void NoFile();
	// 一个文件处理完毕（发送或接受）
	void FileOK(DWORD dwID);

	// 发送一个文件
	void SendFile(LPXFILEINFO pFileInfo);

	// 接受一个文件
	void RecvFile(LPXFILEINFO pFileInfo);

	// 文件传输对话框，最多 6 个
	FILEDIALOG	m_dlg[6];

	// 调用者窗口句柄
	HWND		m_hParent;

	// 任务总数
	DWORD		m_dwFileTotal;
	// 对话框总数，必须小于 6
	DWORD		m_dwDlgTotal;

	list<XFILEINFO*> m_recvfile;
	list<XFILEINFO*> m_sendfile;

	CXFrame(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXFrame)
	enum { IDD = IDD_FRAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXFrame)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXFrame)
	afx_msg LRESULT OnFileRecvEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnDestRecv(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCancelFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRefuseFile(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XFRAME_H__E0145AE1_0CCE_4BCC_B0A7_7A0B57C6F7CA__INCLUDED_)
