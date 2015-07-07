// xsetting.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "xsetting.h"
#include "FrameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CXsettingApp

BEGIN_MESSAGE_MAP(CXsettingApp, CWinApp)
	//{{AFX_MSG_MAP(CXsettingApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXsettingApp construction

CXsettingApp::CXsettingApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CXsettingApp object

CXsettingApp theApp;


extern "C" __declspec(dllexport)
void test()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CFrameDlg dlg;
	dlg.DoModal();
}

extern "C" __declspec(dllexport)
void test2(LPCTSTR szUID, LPCTSTR szPSW)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CFrameDlg dlg;
	dlg.SetUINFO(szUID, szPSW);
//	AfxMessageBox(szUID);
	dlg.DoModal();
}

BOOL CXsettingApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	return CWinApp::InitInstance();
}
