/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// ToolBarExCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBarExCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarExCtrl

CToolBarExCtrl::CToolBarExCtrl()
{
	m_buttonSpacing = CSize(20,20);
	m_backGroundColor = RGB (255,255,255);
	m_buttonSize = CSize(16,16);
	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_hotButton = -1;
	m_pushedButton = -1;
}

CToolBarExCtrl::~CToolBarExCtrl()
{
	for (int i = 0; i < buttons.GetSize(); i++)
		DestroyIcon(buttons[i].icon);
}


BEGIN_MESSAGE_MAP(CToolBarExCtrl, CStatic)
	//{{AFX_MSG_MAP(CToolBarExCtrl)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarExCtrl message handlers

void CToolBarExCtrl::SetButtonSpacing(CSize spacing)
{
	m_buttonSpacing = spacing;
	Invalidate();
}

void CToolBarExCtrl::SetButtonSize(CSize size)
{
	m_buttonSize = size;
	Invalidate();
}

void CToolBarExCtrl::AddButton(HICON icon, int commandId, LPCTSTR text)
{
	ButtonEx button;
	button.icon = icon;
	button.idCommand = commandId;
	button.fsState = BT_EX_ENABLED;
	button.fsStyle = BT_EX_BUTTON;
	button.tooltip = strdup(text);
	int nIndex = buttons.Add(button);
	rects.Add(CRect(0,0,0,0));
	m_ToolTip.AddTool(this,text,&CRect(10,10,30,30),commandId);
	Invalidate();
}

void CToolBarExCtrl::AddSeparator()
{
	ButtonEx button;
	button.icon = NULL;
	button.idCommand = NULL;
	button.fsState = BT_EX_ENABLED;
	button.fsStyle = BT_EX_SEPARATOR;
	button.tooltip = NULL;
	buttons.Add(button);
	rects.Add(CRect(0,0,0,0));
	Invalidate();
}

void CToolBarExCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC* pDCMem = new CDC;
	CBitmap bpMem;
	CBitmap *bmOld;
	CRect rcClient;

	// Get client rectangle
	GetClientRect(rcClient);

	// Create DC
	pDCMem->CreateCompatibleDC(&dc);

	// Create bitmap
	bpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	// Select bitmap
	bmOld = pDCMem->SelectObject(&bpMem);

	// Draw control background
	pDCMem->FillSolidRect(rcClient,m_backGroundColor);
	CRect rect(0,0,m_buttonSpacing.cx,m_buttonSpacing.cy);
	CRect btRect;
	for (int i = 0; i < buttons.GetSize(); i++)
	{
		if (buttons[i].fsStyle == BT_EX_BUTTON)
		{
			btRect = rect;
			CSize border = m_buttonSpacing - m_buttonSize;
			btRect.DeflateRect(border.cx/2,border.cy/2);
			int nFlags = DST_ICON;
			if (buttons[i].fsState == BT_EX_DISABLED)
				nFlags = nFlags | DSS_DISABLED;
			if (m_hotButton == i)
			{
				CRect focusRect = btRect;
				focusRect.InflateRect(2,2);
				if (m_pushedButton == i)
					pDCMem->FrameRect(&focusRect, &CBrush(RGB(0,0,0)));
				else
					pDCMem->FrameRect(&focusRect, &CBrush(RGB(128,128,128)));
			}
			pDCMem->DrawState(btRect.TopLeft(),m_buttonSize,buttons[i].icon,nFlags,(CBrush*)NULL);
			rects[i] = btRect;
			m_ToolTip.SetToolRect(this,buttons[i].idCommand,btRect);
			rect.OffsetRect(m_buttonSpacing.cx,0);
		}
		else
		{
			btRect = rect;
			CSize border = m_buttonSpacing - m_buttonSize;
			btRect.DeflateRect(2,border.cy/2);
			btRect.right = btRect.left + 3;
			pDCMem->DrawEdge(&btRect,EDGE_BUMP,BF_RECT);
			rect.OffsetRect(7,0);
		}
		if (rect.right > rcClient.right)
		{
			rect.left = 0;
			rect.top += m_buttonSpacing.cy;
			rect.right = m_buttonSpacing.cx;
			rect.bottom += m_buttonSpacing.cy;
		}
	}

	// Copy back buffer to the display
	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), pDCMem, 0, 0, SRCCOPY);
	
	// Select old objects
	pDCMem->SelectObject(bmOld);
	
	// Delete objects
	if (pDCMem->GetSafeHdc() != NULL)
	{
		pDCMem->DeleteDC();
	}
	delete pDCMem;
	if (bpMem.GetSafeHandle() != NULL)
	{
		bpMem.DeleteObject();
	}

}

void CToolBarExCtrl::RemoveButton(int nIndex, int commandId)
{
	if (nIndex > -1 && nIndex < buttons.GetSize())
	{
		buttons.RemoveAt(nIndex);
		rects.RemoveAt(nIndex);
		m_ToolTip.DelTool(this,nIndex);
	}
	else
	{
		for (int i = 0; i < buttons.GetSize(); i++)
		{
			if (buttons[i].idCommand == commandId)
			{
				buttons.RemoveAt(i);
				rects.RemoveAt(i);
				m_ToolTip.DelTool(this,i);
			}
		}
	}
	Invalidate();
}

void CToolBarExCtrl::SetBackGroundColor(COLORREF color)
{
	m_backGroundColor = color;
	Invalidate();
}

BOOL CToolBarExCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL res = CStatic::PreTranslateMessage(pMsg);
	m_ToolTip.RelayEvent(pMsg);
	return res;
}

void CToolBarExCtrl::EnableButton(int nId, bool bEnable)
{
	if (nId < 0 || nId > buttons.GetSize())
		return;
	if (bEnable)
		buttons[nId].fsState = BT_EX_ENABLED;
	else
		buttons[nId].fsState = BT_EX_DISABLED;
	Invalidate();
}

ButtonEx* CToolBarExCtrl::GetButton(int nIndex, int commandId)
{
	if (nIndex > -1 && nIndex < buttons.GetSize())
		return &buttons[nIndex];
	else
	{
		for (int i = 0; i < buttons.GetSize(); i++)
		{
			if (buttons[i].idCommand == commandId)
			{
				return &buttons[i];
			}
		}
	}
	return NULL;
}

void CToolBarExCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	TOOLINFO ti;
	BOOL bHit = m_ToolTip.HitTest(this,point,&ti);
	if (bHit)
	{
		for (int i = 0; i < buttons.GetSize(); i++)
		{
			if (buttons[i].idCommand == ti.uId && buttons[i].fsState)
			{
				m_hotButton = i;
				::SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(32649)));
			}
		}
	}
	else
	{
		m_hotButton = -1;
		::SetCursor(LoadCursor(NULL,IDC_ARROW));
	}
	Invalidate();
	CStatic::OnMouseMove(nFlags, point);
}

void CToolBarExCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	TOOLINFO ti;
	BOOL bHit = m_ToolTip.HitTest(this,point,&ti);
	if (bHit)
	{
		for (int i = 0; i < buttons.GetSize(); i++)
		{
			if (buttons[i].idCommand == ti.uId && buttons[i].fsState)
			{
				m_pushedButton = i;
				::SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(32649)));
			}
		}
	}
	else
	{
		m_pushedButton = -1;
		::SetCursor(LoadCursor(NULL,IDC_ARROW));
	}
	Invalidate();
	CStatic::OnLButtonDown(nFlags, point);
}

void CToolBarExCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	TOOLINFO ti;
	BOOL bHit = m_ToolTip.HitTest(this,point,&ti);
	if (bHit)
	{
		for (int i = 0; i < buttons.GetSize(); i++)
		{
			if (buttons[i].idCommand == ti.uId && buttons[i].fsState)
			{
				GetParent()->SendMessage(WM_COMMAND,(WPARAM)buttons[i].idCommand,(LPARAM)m_hWnd);
				::SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(32649)));
			}
		}
	}
	else
		::SetCursor(LoadCursor(NULL,IDC_ARROW));
	m_pushedButton = -1;
	Invalidate();
	CStatic::OnLButtonUp(nFlags, point);
}

