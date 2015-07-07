// FrameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xsetting.h"
#include "FrameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrameDlg dialog


CFrameDlg::CFrameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFrameDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwSel = -1;
	m_pGenDlg = NULL;
	m_strPSW = _T("");
	m_strUID = _T("");
}


void CFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFrameDlg)
	DDX_Control(pDX, IDC_LIST1, m_listBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFrameDlg, CDialog)
	//{{AFX_MSG_MAP(CFrameDlg)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrameDlg message handlers

BOOL CFrameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

//	HRESULT hr = CoInitialize(NULL);
//	HINSTANCE hInst = LoadLibrary( _T("OLEACC.DLL") );
//	m_webBrowser.Create(NULL,NULL,WS_BORDER|WS_VISIBLE,CRect(4,35,478,300),this,NULL);
//	m_webBrowser.Navigate("http://www.samesay.com/",NULL,NULL,NULL,NULL);

	m_listBox.SetItemHeight(20);
	m_listBox.AddItem("基本设置", RGB(150,  100, 150),  RGB(255, 255,255));
	m_listBox.AddItem("其他", RGB(200, 150, 200),  RGB(255, 255,255));

	CRect rt;
	GetDlgItem(IDC_STATIC8)->GetWindowRect(&rt);
	ScreenToClient(&rt);

	m_Others.Create(COthersDlg::IDD, this);
	if (::IsWindow(m_Others.GetSafeHwnd()))
	{
		m_Others.MoveWindow(&rt);
		m_Others.ShowWindow(SW_HIDE);
	}

	m_pGenDlg = new CGeneralDlg;
	m_pGenDlg->Create(CGeneralDlg::IDD, this);
	if (::IsWindow(m_pGenDlg->GetSafeHwnd()))
	{
		m_pGenDlg->MoveWindow(&rt);
		m_pGenDlg->ShowWindow(SW_SHOW);
		m_pGenDlg->Init_Ini();
	}
	m_dwSel = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFrameDlg::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	DWORD sel = m_listBox.GetCurSel();
	if (m_dwSel == sel)
		return;

	m_listBox.SetBg(RGB(100, 150, 200), m_dwSel);
	m_listBox.SetBg(RGB(50,  100, 150), sel);
	m_dwSel = sel;
	CString str;
	m_listBox.GetText(sel, str);
	if (str == "基本设置")
	{
		if (m_pGenDlg)
		{
			if (::IsWindow(m_pGenDlg->GetSafeHwnd()))
			{
				m_pGenDlg->ShowWindow(SW_SHOW);
			}
		}

		if (m_Others.IsWindowVisible())
		{
			m_Others.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		if (m_pGenDlg)
		{
			if (::IsWindow(m_pGenDlg->GetSafeHwnd()))
			{
				m_pGenDlg->ShowWindow(SW_HIDE);
			}
		}

		if (::IsWindow(m_Others.GetSafeHwnd()))
		{
			m_Others.ShowWindow(SW_SHOW);
		}
	}
//	AfxMessageBox(str);
}

void CFrameDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	if (m_pGenDlg)
	{
		if (::IsWindow(m_pGenDlg->GetSafeHwnd()))
		{
			m_pGenDlg->UpdateIni();
		}
	}
	CDialog::OnOK();
}

void CFrameDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if (m_pGenDlg)
	{
		if (::IsWindow(m_pGenDlg->GetSafeHwnd()))
		{
			m_pGenDlg->DestroyWindow();
		}
	}
	CDialog::OnCancel();
}

void CFrameDlg::SetUINFO(LPCTSTR szUID, LPCTSTR szPSW)
{
	m_strUID = szUID;
	m_strPSW = szPSW;
}
