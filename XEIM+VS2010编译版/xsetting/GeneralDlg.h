#include "afxwin.h"
#if !defined(AFX_GENERALDLG_H__C38DEE76_B0F6_492E_8F9B_9EB1C77A0EF4__INCLUDED_)
#define AFX_GENERALDLG_H__C38DEE76_B0F6_492E_8F9B_9EB1C77A0EF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CGeneralDlg dialog

class CGeneralDlg : public CDialog
{
// Construction
public:
	
void GetProgramDirectory(char *buf)
{
	ASSERT(NULL != buf);
	
	char str[MAX_PATH];
	GetModuleFileName(NULL,str,MAX_PATH);

	int len = strlen(str);
	int nIdx = 0;

	for (int i=0; i<len; i++)
	{
		if (str[i] == '\\')
			nIdx = i;
		buf[i] = str[i];
	}
	buf[nIdx+1] = NULL;
}
	void Init_Ini();
	void UpdateIni();
	CGeneralDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGeneralDlg)
	enum { IDD = IDD_GENERAL };
	BOOL	m_bAutoRun;
	BOOL	m_bRemember;
	BOOL	m_bAutoLog;

	//}}AFX_DATA
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeneralDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGeneralDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_HotKey;
	afx_msg void OnEnSetfocusEditHotkey();
	afx_msg void OnEnKillfocusEditHotkey();
	afx_msg void OnEnChangeEditHotkey();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALDLG_H__C38DEE76_B0F6_492E_8F9B_9EB1C77A0EF4__INCLUDED_)
