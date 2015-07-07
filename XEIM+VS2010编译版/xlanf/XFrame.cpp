// XFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xlanf.h"
#include "XFrame.h"
#include "XRecvDlg.h"
#include "XSendDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXFrame dialog

#define WM_RECV_FILE		WM_USER + 0x93

CXFrame::CXFrame(CWnd* pParent /*=NULL*/)
	: CDialog(CXFrame::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXFrame)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwFileTotal = 0;
	m_dwDlgTotal = 0;
	m_hParent = NULL;

	for (int i=0; i<6; i++)
	{
		m_dlg[i].pDlg = NULL;
		m_dlg[i].bSend = FALSE;
	}
}


void CXFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXFrame)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXFrame, CDialog)
	//{{AFX_MSG_MAP(CXFrame)
	ON_MESSAGE(WM_FILERECVEND, OnFileRecvEnd)
	ON_WM_TIMER()
	ON_MESSAGE(WM_RECV_FILE, OnDestRecv)
	ON_MESSAGE(WM_SEND_CANCEL, OnCancelFile)
	ON_MESSAGE(WM_REFUSE_FILE, OnRefuseFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXFrame message handlers

void CXFrame::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::PostNcDestroy();
	delete this;
}

void CXFrame::SendFile(LPXFILEINFO pFileInfo)
{
	m_dwFileTotal ++;
	if (m_dwDlgTotal < 6)
	{
		CXSendDlg *pDlg = new CXSendDlg;
		CWnd *pWnd = CWnd::FromHandle(m_hWnd);

		if (! pDlg->Create(CXSendDlg::IDD, pWnd))
		{
			AfxMessageBox("Send File Create failed.");
		}

		pDlg->AdjustWindow(m_dwDlgTotal);
		pDlg->ShowWindow(SW_SHOW);
		pDlg->AddFile(pFileInfo);

		m_dlg[m_dwDlgTotal].pDlg = pDlg;
		m_dlg[m_dwDlgTotal].bSend = TRUE;

		m_dwDlgTotal ++;
	}
	else
	{
		LPXFILEINFO p = new XFILEINFO;
		memcpy(p, pFileInfo, sizeof(XFILEINFO));
		m_sendfile.push_back(p);
	}

	CString str;
	str.Format("队列中有[%d]个文件", m_dwFileTotal);
	SetDlgItemText(IDC_FILE_TOTAL, str);
}

void CXFrame::RecvFile(LPXFILEINFO pFileInfo)
{
	m_dwFileTotal ++;
	if (m_dwDlgTotal < 6)
	{
		CXRecvDlg *pDlg = new CXRecvDlg;
		CWnd *pWnd = CWnd::FromHandle(m_hWnd);

		if (! pDlg->Create(CXRecvDlg::IDD, pWnd))
		{
			AfxMessageBox("RecvFile Create failed.");
		}

		pDlg->AdjustWindow(m_dwDlgTotal);
		pDlg->ShowWindow(SW_SHOW);
		pDlg->AddFile(pFileInfo);

		m_dlg[m_dwDlgTotal].pDlg = pDlg;
		m_dlg[m_dwDlgTotal].bSend = FALSE;

		m_dwDlgTotal ++;
	}
	else
	{
		LPXFILEINFO p = new XFILEINFO;
		memcpy(p, pFileInfo, sizeof(XFILEINFO));
		m_recvfile.push_back(p);
	}

	CString str;
	str.Format("队列中有[%d]个文件", m_dwFileTotal);
	SetDlgItemText(IDC_FILE_TOTAL, str);
}

void CXFrame::FileOK(DWORD dwID)
{
	m_dwFileTotal --;
	m_dwDlgTotal --;

	for (int i=dwID; i<m_dwDlgTotal; i++)
	{
		if (m_dlg[i+1].bSend)
		{
			((CXSendDlg*)m_dlg[i+1].pDlg)->AdjustWindow(i);
		}
		else
		{
			((CXRecvDlg*)m_dlg[i+1].pDlg)->AdjustWindow(i);
		}
	
		m_dlg[i].bSend = m_dlg[i+1].bSend;
		m_dlg[i].pDlg = m_dlg[i+1].pDlg;
	}

	m_dlg[m_dwDlgTotal].pDlg = NULL;
	m_dlg[m_dwDlgTotal].bSend = FALSE;

	if (m_recvfile.size() > 0)
	{
		LPXFILEINFO p = m_recvfile.front();
		m_recvfile.pop_front();

		if (m_dwDlgTotal < 6)
		{
			CXRecvDlg *pDlg;
			pDlg = new CXRecvDlg;
			CWnd *pWnd = CWnd::FromHandle(m_hWnd);
			if (! pDlg->Create(CXRecvDlg::IDD, pWnd))
			{
				AfxMessageBox("Recv Dialog Creating failed.");
			}

			pDlg->AdjustWindow(m_dwDlgTotal);
			pDlg->ShowWindow(SW_SHOW);
			pDlg->AddFile(p);

			delete p;
			m_dlg[m_dwDlgTotal].pDlg = pDlg;
			m_dlg[m_dwDlgTotal].bSend = FALSE;

			m_dwDlgTotal ++;
		}
		else
		{
			m_recvfile.push_back(p);
		}
	}
	else
	{
		if (m_sendfile.size() > 0)
		{
			LPXFILEINFO p = m_sendfile.front();
			m_sendfile.pop_front();
			
			if (m_dwDlgTotal < 6)
			{
				CXSendDlg *pDlg;
				pDlg = new CXSendDlg;
				CWnd *pWnd = CWnd::FromHandle(m_hWnd);
				if (! pDlg->Create(CXSendDlg::IDD, pWnd))
				{
					AfxMessageBox("Send Dialog Creating failed.");
				}

				pDlg->AdjustWindow(m_dwDlgTotal);
				pDlg->ShowWindow(SW_SHOW);
				pDlg->AddFile(p);

				delete p;

				m_dlg[m_dwDlgTotal].pDlg = pDlg;
				m_dlg[m_dwDlgTotal].bSend = TRUE;

				m_dwDlgTotal ++;
			}
			else
			{
				m_recvfile.push_back(p);
			}
		}
	}

	CString str;
	str.Format("队列中有[%d]个文件", m_dwFileTotal);
	SetDlgItemText(IDC_FILE_TOTAL, str);


	// 如果文件列表发完了，就做退出处理。
	if (m_dwFileTotal == 0)
	{
		NoFile();
	}
}


void CXFrame::NoFile()
{
	SetTimer(88, 300, NULL);
}

LRESULT CXFrame::OnFileRecvEnd(WPARAM wParam, LPARAM lParam)
{
	FileOK(wParam);
	return 0;
}

void CXFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 88)
	{
		KillTimer(nIDEvent);
		ShowWindow(SW_HIDE);
		::SendMessage(m_hParent, WM_FILE_FINISH, 0, 0);
	//	((CXlanfApp*)AfxGetApp())->AddWndSize(m_hParent, -100);
		SetTimer(66, 133, NULL);
	}
	else if (nIDEvent == 66)
	{
		KillTimer(nIDEvent);
	//	((CXlanfApp*)AfxGetApp())->AddWndSize(m_hParent, -80);
		((CXlanfApp*)AfxGetApp())->UninitFrame(m_hParent);
		DestroyWindow();
	}
	else
	{
		CDialog::OnTimer(nIDEvent);
	}
}

LRESULT CXFrame::OnDestRecv(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox("CXFrame::OnDestRecv");
	return 0;
}

LRESULT CXFrame::OnCancelFile(WPARAM wParam, LPARAM lParam)
{
//	AfxMessageBox("OnCancelFile");
//	m_dwFileTotal --;
//	m_dwDlgTotal --;

	for (int i=0; i<m_dwDlgTotal; i++)
	{
		if (! m_dlg[i].bSend)
		{
			((CXRecvDlg*)m_dlg[i].pDlg)->CancelFile(wParam);
		/*	CString str;
			str.Format("%d", wParam);
			AfxMessageBox(str);*/
			//m_dlg[i].pDlg->ShowWindow(SW_HIDE);
	//		((CXSendDlg*)m_dlg[i+1].pDlg)->AdjustWindow(i);
		}
//		else
//		{
//			((CXRecvDlg*)m_dlg[i+1].pDlg)->AdjustWindow(i);
		//	AfxMessageBox("TTT");
//		}
		
	//	m_dlg[i] = m_dlg[i+1];
	}

//	m_dlg[m_dwDlgTotal].pDlg = NULL;

/*	CString str;
	str.Format("队列中有[%d]个文件", m_dwFileTotal);
	SetDlgItemText(IDC_FILE_TOTAL, str);


	// 如果文件列表发完了，就做退出处理。
	if (m_dwFileTotal == 0)
	{
		NoFile();
	}*/

	return 0;
}

LRESULT CXFrame::OnRefuseFile(WPARAM wParam, LPARAM lParam)
{
	for (int i=0; i<m_dwDlgTotal; i++)
	{
		if (m_dlg[i].bSend)
		{
		//	CString str;
		//	str.Format("OnRefuseFile %d", wParam);
		//	AfxMessageBox(str);
			((CXSendDlg*)m_dlg[i].pDlg)->RefuseFile(wParam);
		}
	}

	return 0;
}