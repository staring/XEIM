/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// XTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "xsvr.h"
#include "XTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTreeCtrl

CXTreeCtrl::CXTreeCtrl()
{
}

CXTreeCtrl::~CXTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CXTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CXTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTreeCtrl message handlers

void CXTreeCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CXTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT iFlags;
	UINT TVIF;
	HTREEITEM hItem = HitTest(point, &iFlags);
	if((hItem != NULL) && ItemHasChildren(hItem))
	{
		SetRedraw(FALSE);
		if((iFlags & TVHT_ONITEMICON))
		{
			TVIF = GetItemState(hItem, TVIF_STATE) & TVIS_EXPANDED ? TVE_COLLAPSE : TVE_EXPAND;
			Expand(hItem, TVIF);
			if (TVIF == TVE_COLLAPSE)
				SetItemImage(hItem, 1, 1);
			else
				SetItemImage(hItem, 0, 0);
			UpdateData(FALSE);
		}
		SetRedraw();
	}
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CXTreeCtrl::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (TVE_EXPAND == pNMTreeView->action)
		SetItemImage(pNMTreeView->itemNew.hItem, 0, 0);
	else
		SetItemImage(pNMTreeView->itemNew.hItem, 1, 1);
	*pResult = 0;
}

void CXTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CTreeCtrl::OnRButtonDown(nFlags, point);
}
