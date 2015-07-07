#if !defined(AFX_FRAMEDLG_H__2338F6CA_801E_49CB_B572_3EDADB9B4E6D__INCLUDED_)
#define AFX_FRAMEDLG_H__2338F6CA_801E_49CB_B572_3EDADB9B4E6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrameDlg.h : header file
//
#include "CListBoxEBX.h"
#include "GeneralDlg.h"
#include "OthersDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CFrameDlg dialog

class CFrameDlg : public CDialog
{
// Construction
public:
	void SetUINFO(LPCTSTR szUID, LPCTSTR szPSW);
	CString m_strUID;
	CString m_strPSW;
	CWebBrowser2 m_webBrowser;
	DWORD m_dwSel;
	CGeneralDlg *m_pGenDlg;
	COthersDlg m_Others;
	CFrameDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFrameDlg)
	enum { IDD = IDD_FRAME };
	CListBoxEBX	m_listBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFrameDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	afx_msg void OnOk();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEDLG_H__2338F6CA_801E_49CB_B572_3EDADB9B4E6D__INCLUDED_)
