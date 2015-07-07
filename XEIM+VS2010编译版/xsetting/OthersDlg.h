//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES
#if !defined(AFX_OTHERSDLG_H__31155E2B_799E_4795_89F4_C132239F03B4__INCLUDED_)
#define AFX_OTHERSDLG_H__31155E2B_799E_4795_89F4_C132239F03B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OthersDlg.h : header file
//
#include "webbrowser2.h"
/////////////////////////////////////////////////////////////////////////////
// COthersDlg dialog

class COthersDlg : public CDialog
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
	CWebBrowser2 m_webBrowser;
	COthersDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COthersDlg)
	enum { IDD = IDD_OTHERS };
//	CWebBrowser2	m_webBrowser2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COthersDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COthersDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT FAR* URL);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OTHERSDLG_H__31155E2B_799E_4795_89F4_C132239F03B4__INCLUDED_)
