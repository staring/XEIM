/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#if !defined(AFX_XPLUGINBAR_H__7E26647D_A57B_4B7C_A699_CC1E37488A6E__INCLUDED_)
#define AFX_XPLUGINBAR_H__7E26647D_A57B_4B7C_A699_CC1E37488A6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XPluginBar.h : header file
//
#include "ToolBarExCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CXPluginBar dialog

class CXPluginBar : public CDialog
{
	static BOOL CALLBACK EnumWindowsProc (HWND hwnd, LPARAM lParam);
// Construction
public:
	void CloseAllProgram();
	void Read_ini_Info(LPCTSTR lpszINI, LPCTSTR lpszPath);
	void AddPlugIn();
	CXPluginBar(CWnd* pParent = NULL);   // standard constructor
	map<DWORD, string> m_mapPlugin;
	list<DWORD> m_listThreadId;
// Dialog Data
	//{{AFX_DATA(CXPluginBar)
	enum { IDD = IDD_PLUGIN };
	CToolBarExCtrl	m_ctrlPluginBar;

	DWORD m_dwCurSel;
	DWORD m_dwBtnIdx;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXPluginBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXPluginBar)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XPLUGINBAR_H__7E26647D_A57B_4B7C_A699_CC1E37488A6E__INCLUDED_)
