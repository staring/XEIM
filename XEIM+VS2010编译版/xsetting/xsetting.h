// xsetting.h : main header file for the XSETTING DLL
//

#if !defined(AFX_XSETTING_H__41427F05_4385_4EBE_809A_30FE7A575B55__INCLUDED_)
#define AFX_XSETTING_H__41427F05_4385_4EBE_809A_30FE7A575B55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXsettingApp
// See xsetting.cpp for the implementation of this class
//

class CXsettingApp : public CWinApp
{
public:
	CXsettingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXsettingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXsettingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSETTING_H__41427F05_4385_4EBE_809A_30FE7A575B55__INCLUDED_)
