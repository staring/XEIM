/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// XSplitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xsvr.h"
#include "XSplitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// XSplitDlg dialog


XSplitDlg::XSplitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(XSplitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(XSplitDlg)
	m_wndLeft = NULL;
	m_wndRight = NULL;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void XSplitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(XSplitDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(XSplitDlg, CDialog)
	//{{AFX_MSG_MAP(XSplitDlg)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// XSplitDlg message handlers

void XSplitDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	AfxGetMainWnd()->ScreenToClient(&point);
	CPoint apt = point;
	ClientToScreen(&apt);

	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	else
	{
		if (m_bCaptured)
		{
			// 中间分解线
			CRect rc;
			GetWindowRect(&rc);
			GetParent()->ScreenToClient(&rc);

			// 左侧窗口
			CRect rtLeft;
			m_wndLeft->GetWindowRect(&rtLeft);
			GetParent()->ScreenToClient(&rtLeft);

			// 右侧窗口
			CRect rtRight;
			m_wndRight->GetWindowRect(&rtRight);
			GetParent()->ScreenToClient(&rtRight);

			CRect rtParent;
			GetParent()->GetWindowRect(&rtParent);
			int nHalf = rtParent.Width()/2;

			apt.x -= m_ptDown.x;
			if (apt.x < (int)m_nLeftMin)
			{
				rtLeft.right = m_nLeftMin;
				rtRight.left = rc.Width() + m_nLeftMin;
				m_wndLeft->MoveWindow(rtLeft);
				m_wndRight->MoveWindow(rtRight);
				MoveWindow(m_nLeftMin, rc.top, rc.Width(), rc.Height());
			}
			else if (apt.x > nHalf)
			{
				rtLeft.right = nHalf;
				rtRight.left = nHalf + rc.Width();
				m_wndLeft->MoveWindow(rtLeft);
				m_wndRight->MoveWindow(rtRight);
				MoveWindow(nHalf, rc.top, rc.Width(), rc.Height());
			}
			else
			{
				rtLeft.right = apt.x;
				rtRight.left = apt.x + rc.Width();

				m_wndLeft->MoveWindow(rtLeft);
				m_wndRight->MoveWindow(rtRight);
				MoveWindow(apt.x, rc.top, rc.Width(), rc.Height());
			}
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}


LRESULT XSplitDlg::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	// TODO: Add your message handler code here and/or call default
//	SetDlgItemText(IDC_STATIC, "hover");
	RedrawWindow();
	return 1;
}


LRESULT XSplitDlg::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
//	SetDlgItemText(IDC_STATIC, "leave");
	m_bTracking = FALSE;
	RedrawWindow();
	return 0;
}

HBRUSH XSplitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
		if (m_bTracking)
		{
			if (m_bCaptured)
				return (HBRUSH)m_brBk2.GetSafeHandle();
			else
				return (HBRUSH)m_brBk3.GetSafeHandle();
		}
		else
			return (HBRUSH)m_brBk1.GetSafeHandle();
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
/*
UINT CAaaaaaaaaDlg::OnNcHitTest(CPoint point)
{
	UINT nResult = CDialog::OnNcHitTest(point);
	return nResult  == HTCLIENT ? HTCAPTION : nResult;
}
*/

void XSplitDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCursor(LoadCursor(NULL, IDC_SIZEWE));
//	SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	if(!m_bCaptured)
	{
		m_bCaptured=true;
		RedrawWindow();
		SetCapture();
		m_ptDown.x = point.x;
		m_ptDown.y = point.y;
	//	CRect rt;
	//	GetWindowRect(&rt);
	//	GetParent()->ScreenToClient(&rt);
	//	m_ptDown=rt.TopLeft();
	}
	
//	CDialog::OnLButtonDown(nFlags, point);
}

void XSplitDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	if(m_bCaptured)
	{
		m_bCaptured=false;
		RedrawWindow();
		ReleaseCapture();
	}

//	CDialog::OnLButtonUp(nFlags, point);
}

BOOL XSplitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_brBk1.CreateSolidBrush(RGB(236, 243, 246));
	m_brBk2.CreateSolidBrush(RGB(200, 200, 200));
	m_brBk3.CreateSolidBrush(RGB(100, 100, 100));

	m_bCaptured = FALSE;
	m_bTracking = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void XSplitDlg::SetTowWindow(CWnd *pWnd1, CWnd *pWnd2, DWORD dwLeftMin)
{
	m_wndLeft = pWnd1;
	m_wndRight = pWnd2;
	m_nLeftMin = dwLeftMin;
}
