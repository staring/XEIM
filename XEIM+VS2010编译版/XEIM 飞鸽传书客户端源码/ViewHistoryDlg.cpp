/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// ViewHistoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "client02.h"
#include "ViewHistoryDlg.h"
#include "client02Dlg.h"
#include "../xmsg/xtype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewHistoryDlg dialog


CViewHistoryDlg::CViewHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewHistoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewHistoryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_CHAT);
}


void CViewHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewHistoryDlg)
	DDX_Control(pDX, IDC_RICHEDIT1, m_richMsg);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewHistoryDlg, CDialog)
	//{{AFX_MSG_MAP(CViewHistoryDlg)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDC_FRONT, OnFront)
	ON_NOTIFY(DTN_CLOSEUP, IDC_DATETIMEPICKER1, OnCloseupDatetimepicker1)
	ON_WM_CTLCOLOR()
	ON_WM_SIZING()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewHistoryDlg message handlers

void CViewHistoryDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CViewHistoryDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CViewHistoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CRect rt;
	GetClientRect(& rt);
	m_ptOldXY.x = rt.Width();
	m_ptOldXY.y = rt.Height();
	m_ptMinSize.x = rt.right;
	m_ptMinSize.y = rt.bottom;

	// 设置背景颜色
	m_brBk2.CreateSolidBrush(RGB(236, 243, 246));

	// TODO: Add extra initialization here
	m_timeToday = CTime::GetCurrentTime();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewHistoryDlg::InitSetting(CString &strUNickName, CString &strUID, SOCKET sock)
{
	CString str;
	CTime tToday = CTime::GetCurrentTime();

	str.Format("与 %s %s 的聊天记录", strUNickName, tToday.Format("%y年%m月%d日"));
	SetDlgItemText(IDC_STATIC2, str);
	m_strUID = strUID;
	m_strUNickName = strUNickName;
	m_sock = sock;

	// UID 50
	// DATE 28
	int sentLen = 50 + 28;
	char * buf = new char[sentLen];
	ZeroMemory(buf, sentLen);
	strcpy_s(buf, sentLen*sizeof(char), (LPCTSTR)m_strUID);
	CString strDate = tToday.Format("%Y-%m-%d");
	memcpy(buf+50, (LPCTSTR)strDate, strDate.GetLength()+1);

	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_GETHISTORYMSG;
	xmsg.SetData(buf, sentLen);
	delete [] buf;

	send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
}

void CViewHistoryDlg::OnBack() 
{
	// TODO: Add your control notification handler code here
	CTime nowTime;
	m_dateCtrl.GetTime(nowTime);
	DWORD dwDay = nowTime.GetDay();
	CTime newTime(nowTime.GetYear(),nowTime.GetMonth(),nowTime.GetDay()-1,0,0,0);
	m_dateCtrl.SetTime(& newTime);

//	CString strTime = newTime.Format("%Y-%m-%d");
//	AfxMessageBox(strTime);
	m_richMsg.SetSel(0, -1);
	m_richMsg.ReplaceSel("");

	// UID 50
	// DATE 28
	int sentLen = 50 + 28;
	char * buf = new char[sentLen];
	ZeroMemory(buf, sentLen);
	strcpy_s(buf, sentLen*sizeof(char), (LPCTSTR)m_strUID);
	CString strDate = newTime.Format("%Y-%m-%d");
	memcpy(buf+50, (LPCTSTR)strDate, strDate.GetLength()+1);

	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_GETHISTORYMSG;
	xmsg.SetData(buf, sentLen);
	delete [] buf;

	send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);

	CString str;

	str.Format("与 %s %s 的聊天记录", m_strUNickName, newTime.Format("%y年%m月%d日"));
	SetDlgItemText(IDC_STATIC2, str);
//	nowTime.
}

void CViewHistoryDlg::OnFront()
{
	// TODO: Add your control notification handler code here
	CTime nowTime;
	m_dateCtrl.GetTime(nowTime);
	DWORD dwDay = nowTime.GetDay();
	CTime newTime(nowTime.GetYear(),nowTime.GetMonth(),nowTime.GetDay()+1,0,0,0);
	m_dateCtrl.SetTime(& newTime);

//	CString strTime = newTime.Format("%Y-%m-%d");
//	AfxMessageBox(strTime);
	m_richMsg.SetSel(0, -1);
	m_richMsg.ReplaceSel("");

	// UID 50
	// DATE 28
	int sentLen = 50 + 28;
	char * buf = new char[sentLen];
	ZeroMemory(buf, sentLen);
	strcpy_s(buf, sentLen*sizeof(char), (LPCTSTR)m_strUID);
	CString strDate = newTime.Format("%Y-%m-%d");
	memcpy(buf+50, (LPCTSTR)strDate, strDate.GetLength()+1);

	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_GETHISTORYMSG;
	xmsg.SetData(buf, sentLen);
	delete [] buf;

	send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);

	CString str;

	str.Format("与 %s %s 的聊天记录", m_strUNickName, newTime.Format("%y年%m月%d日"));
	SetDlgItemText(IDC_STATIC2, str);
}

void CViewHistoryDlg::XNoHistoryMsg(LPCTSTR szUID)
{
	CTime nowTime;
	m_dateCtrl.GetTime(nowTime);

	m_richMsg.SetSel(0, -1);
	m_richMsg.ReplaceSel("");

	CString strMsg;
	strMsg.Format("与用户 %s 在 %s 无聊天记录。", m_strUNickName, nowTime.Format("%Y-%m-%d"));
	m_richMsg.ReplaceSel(strMsg);

}

void CViewHistoryDlg::XAddHistoryMsg(LPCTSTR szMessage, LPCTSTR szUID, LPCTSTR szTime)
{
	CString strNickName;
	if (! strcmp((LPCTSTR)m_strUID, szUID))
	{
		strNickName = m_strUNickName;
	}
	else
	{
		CClient02Dlg *pDlg = (CClient02Dlg*)AfxGetMainWnd();
		strNickName = pDlg->m_strUNickName;
	}
	CString csStartTime;
	// 信息 发送者 和 发送时间
	csStartTime.Format(_T("%s - %s \r\n"), strNickName, szTime);

	//////////////////////////////////////////////////////////////////////////
	int iTotalTextLength = m_richMsg.GetWindowTextLength();
	m_richMsg.SetSel(iTotalTextLength, iTotalTextLength);
	m_richMsg.ReplaceSel(csStartTime);
	int iStartPos = iTotalTextLength;
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
	cf.dwEffects = (unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
	cf.crTextColor = RGB(0, 128, 0);
	int iEndPos = m_richMsg.GetWindowTextLength();
	m_richMsg.SetSel(iStartPos, iEndPos);
	m_richMsg.SetSelectionCharFormat(cf);
	m_richMsg.SetSel(iEndPos, iEndPos);
	CString str = szMessage;
	int nIdx = 0;
	while (nIdx != -1)
	{
		nIdx = str.Find('\n', nIdx+1);
		str.Insert(nIdx+1, "  ");
	}
	str += "\r\n";
	m_richMsg.ReplaceSel(str);
	m_richMsg.SetSel(iEndPos, m_richMsg.GetWindowTextLength());
	cf.crTextColor = RGB(0, 0, 0);
	m_richMsg.SetSelectionCharFormat(cf);
	m_richMsg.PostMessage(WM_VSCROLL, SB_BOTTOM,0);
	iEndPos = m_richMsg.GetWindowTextLength();
	m_richMsg.SetSel(iEndPos, iEndPos);
}

void CViewHistoryDlg::OnCloseupDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	CTime nowTime;
	m_dateCtrl.GetTime(nowTime);
//	DWORD dwDay = nowTime.GetDay();
//	CTime newTime(nowTime.GetYear(),nowTime.GetMonth(),nowTime.GetDay()+1,0,0,0);
//	m_dateCtrl.SetTime(& newTime);

//	CString strTime = newTime.Format("%Y-%m-%d");
//	AfxMessageBox(strTime);
	m_richMsg.SetSel(0, -1);
	m_richMsg.ReplaceSel("");

	// UID 50
	// DATE 28
	int sentLen = 50 + 28;
	char * buf = new char[sentLen];
	ZeroMemory(buf, sentLen);
	strcpy_s(buf, sentLen*sizeof(char), (LPCTSTR)m_strUID);
	CString strDate = nowTime.Format("%Y-%m-%d");
	memcpy(buf+50, (LPCTSTR)strDate, strDate.GetLength()+1);

	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_GETHISTORYMSG;
	xmsg.SetData(buf, sentLen);
	delete [] buf;

	send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);

	CString str;

	str.Format("与 %s %s 的聊天记录", m_strUNickName, nowTime.Format("%y年%m月%d日"));
	SetDlgItemText(IDC_STATIC2, str);


	*pResult = 0;
}

HBRUSH CViewHistoryDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
		return (HBRUSH)m_brBk2.GetSafeHandle();
		break;
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkColor(RGB(236, 243, 246));
			hbr = (HBRUSH) m_brBk2;
		}
		break;
	}

	return hbr;
}

// 这个宏 来自 EASYSIZE
#define EASYSIZE_MINSIZE(mx,my,s,r) if(r->right-r->left < mx) { if((s == WMSZ_BOTTOMLEFT)||(s == WMSZ_LEFT)||(s == WMSZ_TOPLEFT)) r->left = r->right-mx; else r->right = r->left+mx; } if(r->bottom-r->top < my) { if((s == WMSZ_TOP)||(s == WMSZ_TOPLEFT)||(s == WMSZ_TOPRIGHT)) r->top = r->bottom-my; else r->bottom = r->top+my; }
// 控制窗口的最小值
void CViewHistoryDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CDialog::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
	EASYSIZE_MINSIZE(m_ptMinSize.x, m_ptMinSize.y, fwSide,pRect);
}

void CViewHistoryDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (cx == 0 || cy == 0)
		return;
	
	if (::IsWindow(m_dateCtrl.GetSafeHwnd()))
	{
		int x = m_ptOldXY.x-cx;
		int y = m_ptOldXY.y-cy;

		m_ptOldXY.x = cx;
		m_ptOldXY.y = cy;
		AdjustSize(&m_dateCtrl, x, y, 3);
		AdjustSize(GetDlgItem(IDC_FRONT), x, y, 3);
		AdjustSize(GetDlgItem(IDC_BACK), x, y, 3);
		AdjustSize(GetDlgItem(IDC_RICHEDIT1), x, y, 0);
	}
}


void CViewHistoryDlg::AdjustSize(CWnd *pWnd, int x, int y, int nStyle)
{
	CRect rt;
	pWnd->GetWindowRect(& rt);
	ScreenToClient(& rt);

	switch (nStyle)
	{
	case 0:
		{
			rt.right -= x;
			rt.bottom -= y;
		}
		break;
	case 1:
		{
			rt.right -= x;
			rt.bottom -= y;
			rt.left -= x;
			rt.top -= y;
		}
		break;
	case 2:
		{
			rt.right -= x;
			rt.bottom -= y;
			rt.top -= y;
		}
		break;
	case 3:
		{
			rt.left -= x;
			rt.right -= x;
		}
		break;
	}

	pWnd->MoveWindow(rt.left, rt.top, rt.Width(), rt.Height());
}
