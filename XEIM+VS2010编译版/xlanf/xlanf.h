// xlanf.h : main header file for the XLANF DLL
//

#if !defined(AFX_XLANF_H__4FA3A855_4DF4_45C3_9454_EC80A6F66379__INCLUDED_)
#define AFX_XLANF_H__4FA3A855_4DF4_45C3_9454_EC80A6F66379__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "XFrame.h"

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")


#pragma warning(disable: 4786)
#include <map>
using namespace std;


/////////////////////////////////////////////////////////////////////////////
// CXlanfApp
// See xlanf.cpp for the implementation of this class
//

class CXlanfApp : public CWinApp
{
public:
	BOOL m_bInitSocket;
	// Winsock 初始化
	BOOL InitWinSock();
	// Winsock 反初始化
	void UninitWinSock();

	void UninitFrame(HWND hWnd);
	map<HWND, CXFrame*> m_mapInstance;

	BOOL IsNewInstance(HWND hWnd);
	CXFrame *InitFrame(HWND hParent);
	void AddWndSize(HWND hWnd, int nSize);

	CXlanfApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXlanfApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXlanfApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XLANF_H__4FA3A855_4DF4_45C3_9454_EC80A6F66379__INCLUDED_)
