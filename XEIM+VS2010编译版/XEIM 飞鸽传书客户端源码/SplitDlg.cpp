/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// SplitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "client02.h"
#include "SplitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitDlg dialog


CSplitDlg::CSplitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSplitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSplitDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSplitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplitDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSplitDlg, CDialog)
	//{{AFX_MSG_MAP(CSplitDlg)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitDlg message handlers

void CSplitDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	return;

	SetCursor(LoadCursor(NULL, IDC_SIZENS));
	GetParent()->ScreenToClient(&point);
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
			CRect rtTop;
			m_wndTop->GetWindowRect(&rtTop);
			GetParent()->ScreenToClient(&rtTop);

			// 右侧窗口
			CRect rtBottom;
			m_wndBottom->GetWindowRect(&rtBottom);
			GetParent()->ScreenToClient(&rtBottom);

			CRect rtParent;
			GetParent()->GetWindowRect(&rtParent);
			GetParent()->ScreenToClient(&rtParent);
			int nHalf = rtParent.Width()/2;

			apt.y -= m_ptDown.y;
			if (apt.y < (int)m_nLeftMin)
			{
				rtTop.bottom = m_nLeftMin;
				rtBottom.top = rc.Height() + m_nLeftMin;
				m_wndTop->MoveWindow(rtTop);
				m_wndBottom->MoveWindow(rtBottom);
				MoveWindow(rtTop.left, m_nLeftMin, rc.Width(), rc.Height());
		//		CString str;
		//		str.Format("apt.y < (int)m_nLeftMin %d, %d", apt.y, m_ptDown.y);
		//		GetParent()->SetWindowText(str);
			}
			else if (rtParent.bottom-apt.y-42 < 38)
			{
				rtTop.bottom = rtParent.bottom-42-rc.Height()-38;
				rtBottom.top =rtParent.bottom-42-38;
				m_wndTop->MoveWindow(rtTop);
				m_wndBottom->MoveWindow(rtBottom);
				MoveWindow(rc.left, rtTop.bottom, rc.Width(), rc.Height());
			//	CString str;
			//	str.Format("rtParent.bottom-apt.y-42 < 38");
			//	GetParent()->SetWindowText(str);
			}
			else
			{
				rtTop.bottom = apt.y;
				rtBottom.top = apt.y + rc.Height();

				m_wndTop->MoveWindow(rtTop);
				m_wndBottom->MoveWindow(rtBottom);
				MoveWindow(rc.left, apt.y, rc.Width(), rc.Height());
			//	CString str;
			//	str.Format("y:%d, %d, %d, %d, %d, %d, %d", apt.y, rtParent.right, rtParent.bottom,
			//		rtBottom.Width(), rtBottom.Height(),
			//		rtBottom.right, rtBottom.bottom);
			//	GetParent()->SetWindowText(str);
			}
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}


LRESULT CSplitDlg::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	// TODO: Add your message handler code here and/or call default
//	SetDlgItemText(IDC_STATIC, "hover");
	RedrawWindow();
	return 1;
}


LRESULT CSplitDlg::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
//	SetDlgItemText(IDC_STATIC, "leave");
	m_bTracking = FALSE;
	RedrawWindow();
	return 0;
}

HBRUSH CSplitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CSplitDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCursor(LoadCursor(NULL, IDC_SIZENS));
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

void CSplitDlg::OnLButtonUp(UINT nFlags, CPoint point) 
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

BOOL CSplitDlg::OnInitDialog() 
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

void CSplitDlg::SetTowWindow(CWnd *pWnd1, CWnd *pWnd2, DWORD dwLeftMin)
{
	m_wndTop = pWnd1;
	m_wndBottom = pWnd2;
	m_nLeftMin = dwLeftMin;
}