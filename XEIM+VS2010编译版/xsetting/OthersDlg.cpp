// OthersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xsetting.h"
#include "FrameDlg.h"
#include "OthersDlg.h"
#include "INI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COthersDlg dialog


COthersDlg::COthersDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COthersDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COthersDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COthersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COthersDlg)
	DDX_Control(pDX, IDC_EXPLORER1, m_webBrowser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COthersDlg, CDialog)
	//{{AFX_MSG_MAP(COthersDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COthersDlg message handlers

BOOL COthersDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	char szFileName[_MAX_PATH];
	GetProgramDirectory(szFileName);
	strcat(szFileName, "info.ini");
	CIniReader iniReader(szFileName);
	CString value;
	if (iniReader.sectionExists("xeim_server_info"))
	{
		// 获取设置服务器
		value = iniReader.getKeyValue("server_setting", "xeim_server_info");
		if (value.IsEmpty())
		{
			value = "http://www.freeeim.com/index.asp";
			iniReader.setKey(value, "server_setting", "xeim_server_info");
		}
	}

	CRect rt;
	GetWindowRect(&rt);
	GetParent()->ScreenToClient(&rt);
	rt.right -= 10;
	HRESULT hr = CoInitialize(NULL);
	HINSTANCE hInst = LoadLibrary( _T("OLEACC.DLL") );
	CString strParam;
	CFrameDlg *pDlg = (CFrameDlg*)GetParent();
	strParam.Format("?uid=%s&upsw=%s", pDlg->m_strUID, pDlg->m_strPSW);
	value += strParam;
//	AfxMessageBox(value);
//	m_webBrowser.Create(NULL,NULL,WS_BORDER|WS_VISIBLE,rt,this,NULL);
//	m_webBrowser.MoveWindow(rt, TRUE);
	m_webBrowser.Navigate((LPCTSTR)value, NULL,NULL,NULL,NULL);	return TRUE;  // return TRUE unless you set the focus to a control
//	m_webBrowser.ShowScrollBar(SB_BOTH, FALSE);

	              // EXCEPTION: OCX Property Pages should return FALSE
}

#include <mshtml.h>
// For brevity, this code adds the attribute to all documents.
void COthersDlg::OnDocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT FAR* URL) 
{
	HRESULT hr = S_OK;
	IWebBrowser2* pWB = NULL;

	hr = pDisp->QueryInterface(IID_IWebBrowser2, reinterpret_cast<void**>(&pWB));
	
	IDispatch* pDocDisp = NULL;
	hr = pWB->get_Document(&pDocDisp);
	
	if (pDocDisp)
	{
		VARIANT v;
		VariantInit(&v);

		IHTMLDocument2* pDoc = NULL;
		hr = pDocDisp->QueryInterface(IID_IHTMLDocument2, reinterpret_cast<void **>(&pDoc));

		IHTMLElement* pElement = NULL;
		hr = pDoc->get_body(&pElement);

		IHTMLBodyElement* pBodyElement = NULL;
		hr = pElement->QueryInterface(IID_IHTMLBodyElement, (void**)&pBodyElement);

		if (pBodyElement)
		{
			pBodyElement->put_scroll(::SysAllocString(L"auto"));
			pBodyElement->Release();
		}
		pElement->Release();
		pDoc->Release();
		pDocDisp->Release();
	}
	pWB->Release();
}

BEGIN_EVENTSINK_MAP(COthersDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(COthersDlg)
	ON_EVENT(COthersDlg, IDC_EXPLORER1, 259 /* DocumentComplete */, OnDocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
