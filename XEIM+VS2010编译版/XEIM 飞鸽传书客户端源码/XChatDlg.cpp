/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
// XChatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "client02.h"
#include "client02Dlg.h"
#include "xfun.h"
#include "xclientdef.h"
#include "XChatDlg.h"
#include "XSMSDlg.h"
#include "../xmsg/xtype.h"

#pragma comment(lib, "xlanf.lib")

//__declspec(dllimport) int counter;

//extern "C" __declspec(dllimport) int test(LPCTSTR szFileName);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXChatDlg dialog


CXChatDlg::CXChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXChatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXChatDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDI_CHAT);
	m_bPlaying = FALSE;

	m_hFrame = NULL;
	SendFile = NULL;
	hInst = NULL;
	m_pSplit = NULL;

	m_bFile = FALSE;
	// 默认 0 不在线
	m_dwStatus = 0;
	m_pHistoryDlg = NULL;

	// 区分单人或部们群发信息 FALSE 指单人
	m_bGroup = FALSE;

	// 是否具有手机号码？
	m_bMobile = FALSE;

	m_bCurInputPos = TRUE;
}

CXChatDlg::~CXChatDlg(void)
{
	if (m_pSplit)
	{
		delete m_pSplit;
		m_pSplit = NULL;
	}

	if (m_pHistoryDlg)
	{
		delete m_pHistoryDlg;
		m_pHistoryDlg = NULL;
	}
}


void CXChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXChatDlg)
	DDX_Control(pDX, IDC_RICHEDIT1, m_richMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXChatDlg, CDialog)
	//{{AFX_MSG_MAP(CXChatDlg)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_SENDSMS, OnSendSMS)
	ON_BN_CLICKED(IDC_HISTORY, OnViewHistory)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SENDFILE, OnSendFile)
	ON_WM_SIZE()
	ON_MESSAGE(WM_NEW_FILE, OnDestCanceled)
	ON_MESSAGE(WM_REFUSE_FILE, OnDestRefused)
	ON_MESSAGE(WM_FILE_FINISH, XOnAllFileFinished)
	ON_MESSAGE(XM_EMDBLCLICKTREE, XDoubleClickUserList)
	ON_WM_SIZING()
	ON_WM_DRAWITEM()
	ON_MESSAGE(XM_SENDFILEADD, XOnAddFDlgSize)
	ON_BN_CLICKED(IDC_SHAKE, OnShake_Window)
	//}}AFX_MSG_MAP
	ON_NOTIFY(EN_MSGFILTER, IDC_RICHEDIT2, &CXChatDlg::OnEnMsgfilterRichedit2)
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY(EN_MSGFILTER, IDC_RICHEDIT1, &CXChatDlg::OnEnMsgfilterRichedit1)
	ON_COMMAND(IDM_CHAT_PASTE, &CXChatDlg::OnChatPaste)
	ON_COMMAND(IDM_CHAT_COPY, &CXChatDlg::OnChatCopy)
	ON_COMMAND(IDM_CHAT_CUT, &CXChatDlg::OnChatCut)
	ON_COMMAND(IDM_CHAT_SELALL, &CXChatDlg::OnChatSelAll)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXChatDlg message handlers

void CXChatDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

BOOL CXChatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CRect rt;
	GetClientRect(& rt);
	m_ptOldXY.x = rt.Width();
	m_ptOldXY.y = rt.Height();
	m_ptMin.x = rt.right-55;
	m_ptMin.y = rt.bottom;

	// 设置背景颜色
	m_brBk2.CreateSolidBrush(RGB(255, 255, 255));
//	m_brBk2.CreateSolidBrush(RGB(200, 200, 255));
//	m_brBk2.CreateSolidBrush(RGB(236, 243, 246));
	// TODO: Add extra initialization here
	m_textcolor=RGB(0,0,255);						// blue
	m_whitecolor=RGB(255,255,255);					// white text
	m_whitebrush.CreateSolidBrush(m_whitecolor);

	if (! m_bGroup)
	{
		CString str;
		str.Format("与 %s 对话", m_strUser);
		SetWindowText(str);
	}
	else
	{
		GetDlgItem(IDC_SENDSMS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SENDFILE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_HISTORY)->ShowWindow(SW_HIDE);
	}

	// 窗口切分
	CWnd *pEdit = (CWnd*)GetDlgItem(IDC_RICHEDIT2);
	CWnd *pRich = (CWnd*)GetDlgItem(IDC_RICHEDIT1);
	CRect rtTop;
	CRect rtBottom;
	pRich->GetWindowRect(&rtTop);
	ScreenToClient(&rtTop);
	pEdit->GetWindowRect(&rtBottom);
	ScreenToClient(&rtBottom);

	m_pSplit = new CSplitDlg;
	m_pSplit->Create(CSplitDlg::IDD, this);
	if (::IsWindow(m_pSplit->GetSafeHwnd()))
	{
		m_pSplit->ShowWindow(SW_SHOW);
		m_pSplit->SetTowWindow(pRich, pEdit, 100);
		m_pSplit->MoveWindow(rtTop.left, rtTop.bottom,rtTop.Width(), 5);
	}

	if (! m_bMobile)
	{
		GetDlgItem(IDC_SENDSMS)->ShowWindow(SW_HIDE);
	}

	HWND hWnd = GetDlgItem(IDC_RICHEDIT2)->m_hWnd;
	::SendMessage(hWnd, EM_SETEVENTMASK, 0, 
		(LPARAM)(ENM_KEYEVENTS | ENM_MOUSEEVENTS));

	hWnd = GetDlgItem(IDC_RICHEDIT1)->m_hWnd;
	::SendMessage(hWnd, EM_SETEVENTMASK, 0, 
		(LPARAM)(ENM_KEYEVENTS | ENM_MOUSEEVENTS));
		

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 查看历史记录
void CXChatDlg::OnViewHistory()
{
	if (! m_pHistoryDlg)
	{
		m_pHistoryDlg = new CViewHistoryDlg;
		m_pHistoryDlg->Create(CViewHistoryDlg::IDD, CWnd::FromHandle(::GetDesktopWindow()));
		m_pHistoryDlg->InitSetting(m_strUser, m_strUID, m_sock);
	}

	if (::IsWindow(m_pHistoryDlg->GetSafeHwnd()))
	{
		m_pHistoryDlg->ShowWindow(SW_SHOW);
		m_pHistoryDlg->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		m_pHistoryDlg->BringWindowToTop();
	}

//	XAddHistoryMsg("历史记录", "2008-10-20");
}

// 发送短信
void CXChatDlg::OnSendSMS()
{
	CXSMSDlg dlg(this);
	if (IDOK == dlg.DoModal())
	{
//		CString tts;
//		tts.Format("%d %s", dlg.m_strSMS.GetLength(), dlg.m_strSMS);
//		AfxMessageBox(tts);
		XMSG xmsg;
		xmsg.m_nMessage = XEIMMSG_SMS;
		XMSG_ONLINE omsg;

		CString strSMS;
		CString strNickName;
		AfxGetMainWnd()->GetDlgItemText(IDC_EDIT_NICKNAME, strNickName);
		strSMS.Format(_T("%s[%s]：%s"), strNickName, dlg.m_strNUM, dlg.m_strSMS);
		char *tmp = dlg.ChangeCODE((LPCTSTR)strSMS);

		XMSG xRecord;
		xRecord.m_nMessage = XEIMMSG_SMS_RECORD;
		XMSG_ONLINE xData;

		xData.SetData(m_strUID, (LPCTSTR)strSMS, strSMS.GetLength()+1);
		xRecord.SetData(xData.GetBuffer(), xData.GetBufferLength());
		send(m_sock, xRecord.GetBuffer(), xRecord.GetBufferLength(), 0);
		Sleep(268);

	//	AfxMessageBox(strSMS);
		DWORD dwLen = strlen(tmp);
		DWORD dwIndex = 0;
		DWORD dwMaxSMS = 70*4;
		char *buf = NULL;

		while (dwIndex < dwLen)
		{
			DWORD dwSendLen = dwMaxSMS;
			if (dwLen - dwIndex < dwMaxSMS)
			{
				dwSendLen = dwLen-dwIndex;
			}
			// Add 1 to contain NULL
			buf = new char[dwSendLen+3];
			if (strlen(buf) > 70*4+2)
			{
				CString str2;
				str2.Format("你所发的信息太长，有 %d 个字，请分几次发送，最大 70 个字符。", strlen(tmp)/4);
				MessageBox(str2, "警告", MB_ICONINFORMATION);
				return;
			}
			sprintf_s(buf, dwSendLen+3, "%02X", dwSendLen/2);
			memcpy(buf+2, tmp+dwIndex, dwSendLen);
			dwIndex += dwSendLen;
			buf[dwSendLen+2] = NULL;

			omsg.SetData(m_strUID, buf, dwSendLen+3);

			xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());
			send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);

			delete [] buf;
		}

		int iTotalTextLength = m_richMsg.GetWindowTextLength();
		m_richMsg.SetSel(iTotalTextLength, iTotalTextLength);
		CString str;
		str = "提交短信：";
		str += dlg.m_strSMS;
		str += "\r\n";
		m_richMsg.ReplaceSel(str);

		delete [] tmp;
	}
}

// 1. 发送消息，个人，部门，群发
void CXChatDlg::OnSend()
{
	// 发给一个人
	if (! m_bGroup)
	{
		if (m_dwStatus == STATUS_OFFLINE)
		{
			SendOffLineMsg();
		}
		else if(m_dwStatus == STATUS_ONLINE)
		{
			SendOnLineMsg();
		}
	}
	else
	{
		// 发给所有人
		CString strTitle;
		GetWindowText(strTitle);
		if (-1 != strTitle.Find(_T("发给所有人")))
		{
			SendAllMsg();
		}
		// 发给一个部门
		else
		{
			SendGroupMsg();
		}
	}
	// 把光标移动到 CEdit 消息输入框
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_RICHEDIT2);
	pEdit->SetFocus();
}

void CXChatDlg::OnCancel()
{
	ShowWindow(SW_HIDE);
//	CDialog::OnCancel();
}

void CXChatDlg::XAddMessage(const char *szMessage)
{
	CTime m_StartTime1 = CTime::GetCurrentTime();
	CString csStartTime;
	// 信息 发送者 和 发送时间
	csStartTime.Format(_T("%s - %s\r\n"), m_strUser,
		m_StartTime1.Format(_T("%H:%M:%S")));


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
	// 窗口抖动消息
	if (str == "---xxx---@#$@#$")
	{
		str = _T("[窗口抖动]\r\n");
		Shake_Window();
	}
	else	// 属于普通消息
	{
		int nIdx = 0;
		while (nIdx != -1)
		{
			nIdx = str.Find('\n', nIdx+1);
			str.Insert(nIdx+1, "  ");
		}
		str += "\r\n";
	}

	m_richMsg.ReplaceSel(str);
	m_richMsg.SetSel(iEndPos, m_richMsg.GetWindowTextLength());
	cf.crTextColor = RGB(0, 0, 0);
	m_richMsg.SetSelectionCharFormat(cf);
	m_richMsg.PostMessage(WM_VSCROLL, SB_BOTTOM,0);

	//////////////////////////////////////////////////////////////////////////
	// 播放消息提示声音
	char szDir[MAX_PATH];
	GetProgramDirectory((char*)szDir);
	strcat_s(szDir, MAX_PATH*sizeof(char), "sounds//");
	strcat_s(szDir, MAX_PATH*sizeof(char),"sound_2.wav");
	PlaySound(szDir, NULL, SND_FILENAME | SND_ASYNC);
}

HBRUSH CXChatDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
		return (HBRUSH)m_brBk2.GetSafeHandle();
		break;
/*	case CTLCOLOR_STATIC:
		{
		//	if (pWnd->GetDlgCtrlID() == IDC_EDIT_NICKNAME)
			{
				pDC->SetBkColor(m_editBKColor);
				pDC->SetTextColor(m_textcolor);
				hbr = (HBRUSH) m_brBk2;
			}
		}
		break;*/

	// TODO: Return a different brush if the default is not desired
	}
	return hbr;
}

void CXChatDlg::OnSendFile() 
{
	// TODO: Add your control notification handler code here
	if (! hInst)
	{
		hInst = LoadLibrary("xlanf.dll");
		if (hInst)
		{
			SendFile = (pSendFile)GetProcAddress(hInst, "SendFile");
			RecvFile = (pRecvFile)GetProcAddress(hInst, "RecvFile");
			if (SendFile)
			{
				m_hFrame = SendFile(m_hWnd, m_sock, (LPCTSTR)m_strUID);
			}
			else
			{
				MessageBox("fxdll.dll has some problem.");
				return;
			}

			if (! RecvFile)
			{
				MessageBox("fxdll.dll has some problem.");
				return;
			}
		}
	}
	else
	{
		if(SendFile)
		{
			m_hFrame = SendFile(m_hWnd, m_sock, m_strUID);
		}
	}
}

// 接受到文件
void CXChatDlg::XRecvFile(LPXFILEINFO pInfo)
{
	pInfo->sock = m_sock;
	strcpy_s(pInfo->szUID, 32*sizeof(char), (LPCTSTR)m_strUID);

	// 对方取消发送文件了
	if (pInfo->option == FILE_CANCEL)
	{
		::PostMessage(m_hFrame, WM_SEND_CANCEL, pInfo->dwPort, NULL);
		return;
	}
	if (pInfo->option == FILE_REFUSE)
	{
		::PostMessage(m_hFrame, WM_REFUSE_FILE, pInfo->dwPort, NULL);
		return;
	}

	// 对方向你发送文件
	if (! hInst)
	{
		hInst = LoadLibrary("xlanf.dll");
		if (hInst)
		{
			m_bFile = TRUE;
			ShowWindow(SW_SHOW);
			SendFile = (pSendFile)GetProcAddress(hInst, "SendFile");
			RecvFile = (pRecvFile)GetProcAddress(hInst, "RecvFile");

			if (RecvFile)
			{
				m_hFrame = RecvFile(GetSafeHwnd(), pInfo);
			}
			else
			{
				MessageBox("fxdll.dll has some problem.");
				return;
			}

			if (! SendFile)
			{
				MessageBox("fxdll.dll has some problem.");
				return;
			}
		}
	}
	else
	{
		if (RecvFile)
		{
			m_hFrame = RecvFile(GetSafeHwnd(), pInfo);
		}
	}
}


// 对方取消发送文件
LRESULT CXChatDlg::OnDestCanceled(WPARAM wParam, LPARAM lParam)
{
	// 模拟实际流程
//	LPXFILEINFO pInfo = (LPXFILEINFO)wParam;
	MessageBox("ttttttttttttt");
	return 1;
}



// 对方拒绝接收文件
LRESULT CXChatDlg::OnDestRefused(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(m_hFrame, WM_REFUSE_FILE, wParam, NULL);
	return 1;
}

void CXChatDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (cx == 0 || cy == 0)
		return;

	if (::IsWindow(GetSafeHwnd()))
	{
		int x = m_ptOldXY.x-cx;
		int y = m_ptOldXY.y-cy;

		CRect rtt;
		GetWindowRect(& rtt);

		if (cx < m_ptMin.x+2)
		{
			x = cx - m_ptMin.x-10;
			m_ptOldXY.x = m_ptMin.x+10;
			rtt.right = rtt.left + m_ptMin.x+10;
			MoveWindow(& rtt);
		}
		else
		{
			m_ptOldXY.x = cx;
		}

		m_ptOldXY.y = cy;

		AdjustSize(GetDlgItem(IDC_SEND), x, y, 1);
		AdjustSize(GetDlgItem(IDC_SENDFILE), x, y, 3);
		AdjustSize(GetDlgItem(IDC_HISTORY), x, y, 1);
		AdjustSize(GetDlgItem(IDC_SENDSMS), x, y, 3);
		AdjustSize(GetDlgItem(IDC_SHAKE), x, y, 3);
		AdjustSize(GetDlgItem(IDCANCEL), x, y, 1);

		AdjustSize(GetDlgItem(IDC_RICHEDIT2), x, y, 2);
		AdjustSize(&m_richMsg, x, y, 0);
		AdjustSize(m_pSplit, x, y, 2);

		CRect rtEdit;
		GetDlgItem(IDC_RICHEDIT2)->GetWindowRect(& rtEdit);
		ScreenToClient(& rtEdit);
		
		CRect rtRich;
		m_richMsg.GetWindowRect(& rtRich);
		ScreenToClient(& rtRich);
		
		if (rtRich.Height() < 94)
		{
			rtRich.bottom = 100;
			rtEdit.top = 100+5;
			GetDlgItem(IDC_RICHEDIT2)->MoveWindow(& rtEdit);
			m_richMsg.MoveWindow(& rtRich);
			m_pSplit->MoveWindow(rtRich.left, rtRich.bottom, rtRich.Width(), 5);
		}

		if (m_bFile)
		{
			CWnd *pFileDlg = CWnd::FromHandle(m_hFrame);
			if (::IsWindow(pFileDlg->GetSafeHwnd()))
			{
				CRect rtDlg;
				pFileDlg->GetWindowRect(& rtDlg);
				ScreenToClient(& rtDlg);
				pFileDlg->MoveWindow(cx-176, 6, rtDlg.Width(), rtDlg.Height()+10);
			}
		}

		m_richMsg.PostMessage(WM_VSCROLL, SB_BOTTOM,0);
	}
	RedrawWindow();
}

LRESULT CXChatDlg::XOnAllFileFinished(WPARAM wParam, LPARAM lParam)
{
	if (hInst)
	{
		SetTimer(11, 188,  NULL);
		m_hFrame = NULL;

		m_ptOldXY.x -= 76;
		CRect rt;
		GetWindowRect(& rt);

		MoveWindow(rt.left, rt.top, rt.Width() - 76, rt.Height());
	}
	return 1;
}


void CXChatDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case 11:
		{
			m_ptOldXY.x -= 100;
			CRect rt;
			GetWindowRect(& rt);
			if (IsZoomed())
			{
				MoveWindow(rt.left, rt.top, rt.Width() + 76, rt.Height());
			}
			else
			{
				MoveWindow(rt.left, rt.top, rt.Width() - 100, rt.Height());
			}

			KillTimer(nIDEvent);
			if (! m_bFile)
			{
				if (hInst)
				{
					FreeLibrary(hInst);
					hInst = NULL;
				}
			}
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

// 这个宏 来自 EASYSIZE
#define EASYSIZE_MINSIZE(mx,my,s,r) \
	if(r->right-r->left < mx) \
      { if((s == WMSZ_BOTTOMLEFT) || \
	       (s == WMSZ_LEFT)|| (s == WMSZ_TOPLEFT) ) \
	        r->left = r->right-mx; \
        else r->right = r->left+mx; } \
	    if(r->bottom-r->top < my) \
        { if((s == WMSZ_TOP)||(s == WMSZ_TOPLEFT) || \
	      (s == WMSZ_TOPRIGHT)) \
		  r->top = r->bottom-my; \
         else r->bottom = r->top+my; }
// 控制窗口的最小值
void CXChatDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CDialog::OnSizing(fwSide, pRect);
	
	// TODO: Add your message handler code here
	EASYSIZE_MINSIZE(m_ptMin.x+10, m_ptMin.y, fwSide,pRect);
}

LRESULT CXChatDlg::XDoubleClickUserList(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RICHEDIT2)->SetFocus();
	return 1;
}


void CXChatDlg::FlashWindow()
{
	if(! (GetWindowLong(m_hWnd, GWL_STYLE) & WS_VISIBLE))
	{
		ShowWindow(SW_MINIMIZE);

		CWnd* pParentWnd = GetTopLevelParent();
		if ( HINSTANCE hUser = LoadLibrary( _T("User32") ) )
		{
			BOOL (WINAPI *pfnFlashWindowEx)(PFLASHWINFO pfwi);
			(FARPROC&)pfnFlashWindowEx = GetProcAddress( hUser, "FlashWindowEx" );
			if ( pfnFlashWindowEx )
			{
				FLASHWINFO pFWX;
				pFWX.cbSize		= sizeof(pFWX);
				pFWX.dwFlags	= FLASHW_ALL | FLASHW_TIMERNOFG;
				pFWX.uCount		= 3;
				pFWX.dwTimeout	= 0;
				pFWX.hwnd		= m_hWnd;//pParentWnd->GetSafeHwnd();
				(*pfnFlashWindowEx)( &pFWX );
			}
			FreeLibrary( hUser );
		}
	}
	else
	{
		if ( GetForegroundWindow() != GetTopLevelParent() )
		{
			//CWnd* pParentWnd = GetTopLevelParent();
			if ( HINSTANCE hUser = LoadLibrary( _T("User32") ) )
			{
				BOOL (WINAPI *pfnFlashWindowEx)(PFLASHWINFO pfwi);

				(FARPROC&)pfnFlashWindowEx = GetProcAddress( hUser, "FlashWindowEx" );
				if ( pfnFlashWindowEx )
				{
					FLASHWINFO pFWX;

					pFWX.cbSize		= sizeof(pFWX);
					pFWX.dwFlags	= FLASHW_ALL | FLASHW_TIMERNOFG;
					pFWX.uCount		= 3;
					pFWX.dwTimeout	= 0;
					pFWX.hwnd		= m_hWnd;//pParentWnd->GetSafeHwnd();

					(*pfnFlashWindowEx)( &pFWX );
				}
				FreeLibrary( hUser );
			}
		}
	}
}



BOOL CXChatDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message==WM_KEYDOWN)
	{   
		if(pMsg->wParam==VK_RETURN)
		{   
			if(GetKeyState(VK_CONTROL)<0)
			{
				OnSend();
				return TRUE;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);   
}

// 发送在线消息
void CXChatDlg::SendOnLineMsg()
{
	CString str;
	GetDlgItemText(IDC_RICHEDIT2, str);
	if(str.IsEmpty())
	{
		MessageBox("请输入消息内容！");
		return;
	}
	if (str.GetLength() > 1024 - 100)
	{
		CString str2;
		str2.Format("你所发的信息太长，请分几次发送，最大 1024 个字符。", str.GetLength());
		MessageBox(str2, "警告", MB_ICONINFORMATION);
		return;
	}

	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_MESSAGE;
	XMSG_ONLINE omsg;

	omsg.SetData(m_strUID, str, str.GetLength()+1);
	xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());
	send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);

	CString newstr;
	AfxGetMainWnd()->GetDlgItemText(IDC_EDIT_NICKNAME, newstr);
	CTime m_StartTime1 = CTime::GetCurrentTime();
	CString csStartTime;
	// 信息 发送者 和 发送时间
	csStartTime.Format(_T("%s - %s\r\n"), newstr,
		m_StartTime1.Format(_T("%H:%M:%S")));

	SetDlgItemText(IDC_RICHEDIT2, "");

	//////////////////////////////////////////////////////////////////////////
	int iTotalTextLength = m_richMsg.GetWindowTextLength();
	m_richMsg.SetSel(iTotalTextLength, iTotalTextLength);
	m_richMsg.ReplaceSel(csStartTime);
	int iStartPos = iTotalTextLength;
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
	cf.dwEffects = (unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
	cf.crTextColor = RGB(0, 0, 255);
	int iEndPos = m_richMsg.GetWindowTextLength();
	m_richMsg.SetSel(iStartPos, iEndPos);
	m_richMsg.SetSelectionCharFormat(cf);
	m_richMsg.SetSel(iEndPos, iEndPos);
	int nIdx = 0;
	while (nIdx != -1)
	{
		nIdx = str.Find('\n', nIdx+1);
		str.Insert(nIdx+1, "  ");
	//	cout << 
	}
	str += "\r\n";
	m_richMsg.ReplaceSel(str);
	m_richMsg.SetSel(iEndPos, m_richMsg.GetWindowTextLength());
	cf.crTextColor = RGB(0, 0, 0);
	m_richMsg.SetSelectionCharFormat(cf);
	m_richMsg.PostMessage(WM_VSCROLL, SB_BOTTOM,0);
	//////////////////////////////////////////////////////////////////////////

	// 播放消息提示声音
	char szDir[MAX_PATH];
	GetProgramDirectory((char*)szDir);
	strcat_s(szDir, MAX_PATH*sizeof(char),"sounds//");
	strcat_s(szDir, MAX_PATH*sizeof(char),"sound_1.wav");
	PlaySound(szDir, NULL, SND_FILENAME | SND_ASYNC);
}

// 发送离线消息
void CXChatDlg::SendOffLineMsg()
{
	CString str;
	GetDlgItemText(IDC_RICHEDIT2, str);
	if(str.IsEmpty())
	{
		MessageBox("请输入消息内容！");
		return;
	}
	if (str.GetLength() > 1024 * 2 - 100)
	{
		CString str2;
		str2.Format("你所发的信息太长，请分几次发送，最大 1024 个字符。", str.GetLength());
		MessageBox(str2, "警告", MB_ICONINFORMATION);
		return;
	}

	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_MSGOFFLINE;
	XMSG_OFFLINE omsg;
	omsg.SetData(m_strUID, str, str.GetLength()+1);
	xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());
	send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);

	CString newstr;
	AfxGetMainWnd()->GetDlgItemText(IDC_EDIT_NICKNAME, newstr);
	CTime m_StartTime1 = CTime::GetCurrentTime();
	CString csStartTime;
	// 信息 发送者 和 发送时间
	csStartTime.Format(_T("%s - %s\r\n"), newstr,
		m_StartTime1.Format(_T("%H:%M:%S")));

	SetDlgItemText(IDC_RICHEDIT2, "");

	//////////////////////////////////////////////////////////////////////////
	int iTotalTextLength = m_richMsg.GetWindowTextLength();
	m_richMsg.SetSel(iTotalTextLength, iTotalTextLength);
	m_richMsg.ReplaceSel(csStartTime);
	int iStartPos = iTotalTextLength;
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
	cf.dwEffects = (unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
	cf.crTextColor = RGB(0, 0, 255);
	int iEndPos = m_richMsg.GetWindowTextLength();
	m_richMsg.SetSel(iStartPos, iEndPos);
	m_richMsg.SetSelectionCharFormat(cf);
	m_richMsg.SetSel(iEndPos, iEndPos);
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
	//////////////////////////////////////////////////////////////////////////

	// 播放消息提示声音
	char szDir[MAX_PATH];
	GetProgramDirectory((char*)szDir);
	strcat_s(szDir, MAX_PATH*sizeof(char),"sounds//");
	strcat_s(szDir, MAX_PATH*sizeof(char),"sound_1.wav");
	PlaySound(szDir, NULL, SND_FILENAME | SND_ASYNC);
}

void CXChatDlg::SetUserStatus(DWORD dwStatus)
{
	m_dwStatus = dwStatus;

	// 如果窗口尚未创建
	if (! ::IsWindow(GetSafeHwnd()))
		return;

	CString str;
	if (m_dwStatus == STATUS_OFFLINE)
	{
		str.Format("离线消息 - 与 %s 对话", m_strUser);
		SetWindowText(str);
	}
	else if(m_dwStatus == STATUS_ONLINE)
	{
		str.Format("与 %s 对话", m_strUser);
		SetWindowText(str);
	}
}

void CXChatDlg::XAddOffLineMsg(LPCTSTR szMessage, LPCTSTR lpszTime)
{
	CString csStartTime;
	// 信息 发送者 和 发送时间
	csStartTime.Format(_T("%s - %s [离线消息]\r\n"), m_strUser, lpszTime);

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
}

void CXChatDlg::XAddHistoryMsg(LPCTSTR szMessage, LPCTSTR szUID, LPCTSTR szTime)
{
	if (m_pHistoryDlg)
	{
		m_pHistoryDlg->XAddHistoryMsg(szMessage, szUID, szTime);
	}
}

void CXChatDlg::XNoHistoryMsg(LPCTSTR szUID)
{
	if (m_pHistoryDlg)
	{
		m_pHistoryDlg->XNoHistoryMsg(szUID);
	}
}

// 群发部门消息
void CXChatDlg::SendGroupMsg()
{
	CClient02Dlg *pDlg = (CClient02Dlg*)AfxGetMainWnd();
	CString str;
	GetDlgItemText(IDC_RICHEDIT2, str);

	// 确保消息框有内容
	if( str.IsEmpty() )
	{
		MessageBox("请输入消息内容！");
		return;
	}

	// 消息框内容不能太长
	if (str.GetLength() > 1024 - 100)
	{
		CString str2;
		str2.Format("你所发的信息太长，请分几次发送，最大 1024 个字符。", str.GetLength());
		MessageBox(str2, "警告", MB_ICONINFORMATION);
		return;
	}

	list<XUserInfo>::iterator iUserInfo = pDlg->m_listUserInfo.begin();
	list<XUserInfo>::iterator _end = pDlg->m_listUserInfo.end();

	for (; iUserInfo != _end; iUserInfo ++)
	{
		if (m_strUID == (*iUserInfo).GetUGroup() )
		{
		//	MessageBox((*iUserInfo).GetUID());
			// 消息的
			if ((*iUserInfo).GetUStatus() == STATUS_ONLINE)
			{
				XMSG xmsg;
				XMSG_OFFLINE omsg;
				xmsg.m_nMessage = XEIMMSG_MESSAGE;
				omsg.SetData((*iUserInfo).GetUID(), str, str.GetLength()+1);
				xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());
				send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
			}
			else
			{
				XMSG xmsg;
				xmsg.m_nMessage = XEIMMSG_MSGOFFLINE;
				XMSG_OFFLINE omsg;
				omsg.SetData((*iUserInfo).GetUID(), str, str.GetLength()+1);
				xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());
				send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
			}
			Sleep(168);
		}
	}

//	MessageBox(m_strUID);

	CString newstr;
	AfxGetMainWnd()->GetDlgItemText(IDC_EDIT_NICKNAME, newstr);
	CTime m_StartTime1 = CTime::GetCurrentTime();
	CString csStartTime;
	// 信息 发送者 和 发送时间
	csStartTime.Format(_T("%s - %s\r\n"), newstr,
		m_StartTime1.Format(_T("%H:%M:%S")));

	SetDlgItemText(IDC_RICHEDIT2, "");

	//////////////////////////////////////////////////////////////////////////
	int iTotalTextLength = m_richMsg.GetWindowTextLength();
	m_richMsg.SetSel(iTotalTextLength, iTotalTextLength);
	m_richMsg.ReplaceSel(csStartTime);
	int iStartPos = iTotalTextLength;
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
	cf.dwEffects = (unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
	cf.crTextColor = RGB(0, 0, 255);
	int iEndPos = m_richMsg.GetWindowTextLength();
	m_richMsg.SetSel(iStartPos, iEndPos);
	m_richMsg.SetSelectionCharFormat(cf);
	m_richMsg.SetSel(iEndPos, iEndPos);
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
	//////////////////////////////////////////////////////////////////////////
	// 播放消息提示声音
	char szDir[MAX_PATH];
	GetProgramDirectory((char*)szDir);
	strcat_s(szDir, MAX_PATH*sizeof(char),"sounds//");
	strcat_s(szDir, MAX_PATH*sizeof(char),"sound_1.wav");
	PlaySound(szDir, NULL, SND_FILENAME | SND_ASYNC);
}

void CXChatDlg::AdjustSize(CWnd *pWnd, int x, int y, int nStyle)
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
			rt.left -= x;

			rt.top -= y;
			rt.bottom -= y;
		}
		break;
	case 2:
		{
			rt.right -= x;

			rt.top -= y;
			rt.bottom -= y;
		}
		break;
	case 3:
		{
			rt.right -= x;
			rt.left -= x;
		}
		break;
	}

	pWnd->MoveWindow(rt.left, rt.top, rt.Width(), rt.Height());
}



void CXChatDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	RECT rect;
	rect= lpDrawItemStruct->rcItem;

	switch (nIDCtl)
	{
	case IDCANCEL:
	case IDC_SHAKE:
	case IDC_HISTORY:
	case IDC_SENDSMS:
	case IDC_SENDFILE:
	case IDC_SEND:
		{
		//	dc.Draw3dRect(&rect,RGB(236, 243, 246),RGB(236, 243, 246));
			dc.Draw3dRect(&rect,RGB(255, 255, 255),RGB(255, 255, 255));
		//	dc.FillSolidRect(&rect,RGB(236, 243, 246));
			dc.FillSolidRect(&rect,RGB(255, 255, 255));
			UINT state=lpDrawItemStruct->itemState;

			if((state & ODS_SELECTED))
			{
				dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);

			}
			else
			{
				dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);
			}

		//	dc.SetBkColor(RGB(236, 243, 246));
			dc.SetBkColor(RGB(255, 255, 255));
			//dc.SetBkColor(GetSysColor(COLOR_BTNFACE));
			dc.SetTextColor(RGB(25, 79, 149));


			TCHAR buffer[MAX_PATH];
			ZeroMemory(buffer,MAX_PATH );
			::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH);
			dc.DrawText(buffer,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

			dc.Detach();
		}
		break;

	}				

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

LRESULT CXChatDlg::XOnAddFDlgSize(WPARAM wParam, LPARAM lParam)
{
	if (IsZoomed())
	{
	//	AfxMessageBox("最大化");
		m_bFile = TRUE;
		m_ptOldXY.x += 176;
		CRect rt;
		GetWindowRect(& rt);
		MoveWindow(rt.left, rt.top, rt.Width()-10, rt.Height());
		MoveWindow(rt.left, rt.top, rt.Width()+10, rt.Height());
	}
	else
	{
		m_bFile = TRUE;
		m_ptOldXY.x += 176;
		CRect rt;
		GetWindowRect(& rt);
		MoveWindow(rt.left, rt.top, rt.Width() + 176, rt.Height());
	}
	return 1;
}


void CXChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);  
	if (SC_RESTORE == nID)//   还原
	{
		CRect rtEdit;
		GetDlgItem(IDC_RICHEDIT2)->GetWindowRect(& rtEdit);
		ScreenToClient(& rtEdit);
		
		CRect rtRich;
		m_richMsg.GetWindowRect(& rtRich);
		ScreenToClient(& rtRich);
		
		
		if (rtRich.Height() < 94)
		{
			rtRich.bottom = 100;
			rtEdit.top = 100+5;
			GetDlgItem(IDC_RICHEDIT2)->MoveWindow(& rtEdit);
			m_richMsg.MoveWindow(& rtRich);
			m_pSplit->MoveWindow(rtRich.left, rtRich.bottom, rtRich.Width(), 5);
		}
	}
	
}


// 窗口抖动
void CXChatDlg::Shake_Window()
{
//	Voice_AddText(_T("对方向你发送一个窗口抖动"));
	int ty=2;
	int nDelay = 28;
	CRect   m_rect;   
	GetWindowRect(&m_rect);  
	int recordy=m_rect.left;
	int recordx=m_rect.top;

//	RedrawWindow(&m_rect);
	for(int i=0;i<5;i++)
	{
		m_rect.left=recordy;
		m_rect.top=recordx;
		m_rect.top = m_rect.top + ty;
		m_rect.left = m_rect.left - ty;
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW );
		RedrawWindow(&m_rect);
		Sleep(nDelay);
		m_rect.top = m_rect.top -ty;
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW );
		Sleep(nDelay);
		m_rect.top = m_rect.top -2*ty;
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW );
		Sleep(nDelay);
		m_rect.left=m_rect.left+ty;
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW );
		Sleep(nDelay);
		m_rect.left=m_rect.left+2*ty;
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW );
		Sleep(nDelay);
		m_rect.top = m_rect.top + ty;  
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW );
		Sleep(nDelay);
		m_rect.top=m_rect.top+2*ty;
		::SetWindowPos(m_hWnd,NULL,m_rect.left,m_rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW ); 
		::SetWindowPos(m_hWnd,NULL,recordy,recordx,0,0,SWP_NOSIZE|SWP_SHOWWINDOW );
		Sleep(nDelay);
	}
}// End 窗口抖动

void CXChatDlg::OnShake_Window() 
{
	CString str = "---xxx---@#$@#$";

	XMSG xmsg;
	xmsg.m_nMessage = XEIMMSG_MESSAGE;
	XMSG_ONLINE omsg;

	omsg.SetData(m_strUID, str, str.GetLength()+1);
	xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());
	send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);

	Shake_Window();

	// 播放消息提示声音
	char szDir[MAX_PATH];
	GetProgramDirectory((char*)szDir);
	strcat_s(szDir, MAX_PATH*sizeof(char),"sounds//");
	strcat_s(szDir, MAX_PATH*sizeof(char),"sound_1.wav");
	PlaySound(szDir, NULL, SND_FILENAME | SND_ASYNC);
}

void CXChatDlg::SendAllMsg()
{
	CClient02Dlg *pDlg = (CClient02Dlg*)AfxGetMainWnd();
	CString str;
	GetDlgItemText(IDC_RICHEDIT2, str);

	// 确保消息框有内容
	if( str.IsEmpty() )
	{
		MessageBox("请输入消息内容！");
		return;
	}

	// 消息框内容不能太长
	if (str.GetLength() > 1024 - 100)
	{
		CString str2;
		str2.Format("你所发的信息太长，请分几次发送，最大 1024 个字符。", str.GetLength());
		MessageBox(str2, "警告", MB_ICONINFORMATION);
		return;
	}

	list<XUserInfo>::iterator iUserInfo = pDlg->m_listUserInfo.begin();
	list<XUserInfo>::iterator _end = pDlg->m_listUserInfo.end();

	while( iUserInfo != _end )
	{
		// 发送在线消息
		if ((*iUserInfo).GetUStatus() == STATUS_ONLINE)
		{
			XMSG xmsg;
			XMSG_OFFLINE omsg;
			xmsg.m_nMessage = XEIMMSG_MESSAGE;
			omsg.SetData((*iUserInfo).GetUID(), str, str.GetLength()+1);
			xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());
			send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
		//	AfxMessageBox((*iUserInfo).GetUID());
		}
		// 发送离线消息
		else
		{
			XMSG xmsg;
			xmsg.m_nMessage = XEIMMSG_MSGOFFLINE;
			XMSG_OFFLINE omsg;
			omsg.SetData((*iUserInfo).GetUID(), str, str.GetLength()+1);
			xmsg.SetData(omsg.GetBuffer(), omsg.GetBufferLength());
			send(m_sock, xmsg.GetBuffer(), xmsg.GetBufferLength(), 0);
		}
		Sleep(168);
		iUserInfo ++;
	}

//	MessageBox(m_strUID);

	CString newstr;
	AfxGetMainWnd()->GetDlgItemText(IDC_EDIT_NICKNAME, newstr);
	CTime m_StartTime1 = CTime::GetCurrentTime();
	CString csStartTime;
	// 信息 发送者 和 发送时间
	csStartTime.Format(_T("%s - %s\r\n"), newstr,
		m_StartTime1.Format(_T("%H:%M:%S")));

	SetDlgItemText(IDC_RICHEDIT2, "");

	//////////////////////////////////////////////////////////////////////////
	int iTotalTextLength = m_richMsg.GetWindowTextLength();
	m_richMsg.SetSel(iTotalTextLength, iTotalTextLength);
	m_richMsg.ReplaceSel(csStartTime);
	int iStartPos = iTotalTextLength;
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
	cf.dwEffects = (unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
	cf.crTextColor = RGB(0, 0, 255);
	int iEndPos = m_richMsg.GetWindowTextLength();
	m_richMsg.SetSel(iStartPos, iEndPos);
	m_richMsg.SetSelectionCharFormat(cf);
	m_richMsg.SetSel(iEndPos, iEndPos);
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
	//////////////////////////////////////////////////////////////////////////
	// 播放消息提示声音
	char szDir[MAX_PATH];
	GetProgramDirectory((char*)szDir);
	strcat_s(szDir, MAX_PATH*sizeof(char),"sounds//");
	strcat_s(szDir, MAX_PATH*sizeof(char),"sound_1.wav");
	PlaySound(szDir, NULL, SND_FILENAME | SND_ASYNC);
}

void CXChatDlg::OnEnMsgfilterRichedit2(NMHDR *pNMHDR, LRESULT *pResult)
{
	MSGFILTER *pMsgFilter = reinterpret_cast<MSGFILTER *>(pNMHDR);
	// TODO:  控件将不发送此通知，除非您重写
	// CDialog::OnInitDialog() 函数，以将 EM_SETEVENTMASK 消息发送
	// 到该控件，同时将 ENM_KEYEVENTS 或 ENM_MOUSEEVENTS 标志
	// “或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
	switch (pMsgFilter->msg)
	{
	case WM_RBUTTONUP:
		{
			m_bCurInputPos = TRUE;

			CMenu menu;
			menu.LoadMenu(IDM_CHAT);

			CMenu* pPopMenu = menu.GetSubMenu(0);
			CPoint point;
			GetCursorPos(&point);
			//是否选择了文本
			CRichEditCtrl* pREdit2 = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2);
			CString sSelTxt = pREdit2->GetSelText();
			if ( sSelTxt.GetLength() <= 0)
			{
				pPopMenu->EnableMenuItem(IDM_CHAT_COPY, MF_GRAYED | MF_BYCOMMAND | MF_DISABLED);
				pPopMenu->EnableMenuItem(IDM_CHAT_CUT, MF_GRAYED | MF_BYCOMMAND | MF_DISABLED);
			}

			CString sTemp;
			pREdit2->GetWindowText(sTemp);
			if ( sTemp.GetLength() <= 0)
			{
				pPopMenu->EnableMenuItem(IDM_CHAT_SELALL, MF_GRAYED | MF_BYCOMMAND | MF_DISABLED);
			}

			if ( !HasClipTextData() )
			{
				pPopMenu->EnableMenuItem(IDM_CHAT_PASTE, MF_GRAYED | MF_BYCOMMAND | MF_DISABLED);
			}

			pPopMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, 
				point.x, point.y, this);
		}
		break;
	}

	*pResult = 0;
}

void CXChatDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialog::OnRButtonDown(nFlags, point);
}

void CXChatDlg::OnEnMsgfilterRichedit1(NMHDR *pNMHDR, LRESULT *pResult)
{
	MSGFILTER *pMsgFilter = reinterpret_cast<MSGFILTER *>(pNMHDR);
	// TODO:  控件将不发送此通知，除非您重写
	// CDialog::OnInitDialog() 函数，以将 EM_SETEVENTMASK 消息发送
	// 到该控件，同时将 ENM_KEYEVENTS 或 ENM_MOUSEEVENTS 标志
	// “或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
	switch (pMsgFilter->msg)
	{
	case WM_RBUTTONUP:
		{
			m_bCurInputPos = FALSE;

			CMenu menu;
			menu.LoadMenu(IDM_CHAT);

			CMenu* pPopMenu = menu.GetSubMenu(0);
			CPoint point;
			GetCursorPos(&point);
			//是否选择了文本
			CRichEditCtrl* pREdit1 = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT1);
			CString sSelTxt = pREdit1->GetSelText();
			if ( sSelTxt.GetLength() <= 0)
			{
				pPopMenu->EnableMenuItem(IDM_CHAT_COPY, MF_GRAYED | MF_BYCOMMAND | MF_DISABLED);
			}

			CString sTemp;
			pREdit1->GetWindowText(sTemp);
			if ( sTemp.GetLength() <= 0)
			{
				pPopMenu->EnableMenuItem(IDM_CHAT_SELALL, MF_GRAYED | MF_BYCOMMAND | MF_DISABLED);
			}

			pPopMenu->EnableMenuItem(IDM_CHAT_CUT, MF_GRAYED | MF_BYCOMMAND | MF_DISABLED);
			pPopMenu->EnableMenuItem(IDM_CHAT_PASTE, MF_GRAYED | MF_BYCOMMAND | MF_DISABLED);

			pPopMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, 
				point.x, point.y, this);
		}
		break;
	}

	*pResult = 0;
}

//当前剪贴板中是否有文本数据
BOOL CXChatDlg::HasClipTextData()
{
	if ( OpenClipboard() )
	{
		if ( IsClipboardFormatAvailable(CF_TEXT) )
		{
			//有文本数据
			CloseClipboard();
			return TRUE;
		}
	}

	return FALSE;
}

//获取剪贴板文本数据
CString CXChatDlg::GetClipTextData()
{
	CString sText;

	if(OpenClipboard())
	{
		if(IsClipboardFormatAvailable(CF_TEXT))
		{
			HANDLE hClip;
			char *pBuf;
			hClip=GetClipboardData(CF_TEXT);
			pBuf=(char*)GlobalLock(hClip);
			GlobalUnlock(hClip);
			sText = pBuf;
		}
		CloseClipboard();
	} 
	
	return sText;
}

//设置剪贴板数据
BOOL  CXChatDlg::SetClipTextData(CString sText)
{
	if(OpenClipboard())
	{
		CString str;
		HANDLE hClip;
		char *pBuf;
		EmptyClipboard();
	
		hClip=GlobalAlloc(GMEM_MOVEABLE,sText.GetLength()+1);
		pBuf=(char*)GlobalLock(hClip);
		strcpy(pBuf,str);
		GlobalUnlock(hClip);
		SetClipboardData(CF_TEXT,hClip);
		CloseClipboard();
		return TRUE;
	} 

	return FALSE;
}

void CXChatDlg::OnChatPaste()
{
	// TODO: 在此添加命令处理程序代码
	CRichEditCtrl* pREdit = ((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2));
	pREdit->Paste();
}

void CXChatDlg::OnChatCopy()
{
	// TODO: 在此添加命令处理程序代码
	CRichEditCtrl* pREdit1 = ((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT1));
	CString str = pREdit1->GetSelText();
	if ( str.GetLength() > 0)
	{
		pREdit1->Copy();
		return;
	}
	
	CRichEditCtrl* pREdit2 = ((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2));
	str = pREdit2->GetSelText();
	if ( str.GetLength() > 0 )
	{
		pREdit2->Copy();
	}
}

void CXChatDlg::OnChatCut()
{
	// TODO: 在此添加命令处理程序代码
	CRichEditCtrl* pREdit = ((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2));
	pREdit->Cut();
}

void CXChatDlg::OnChatSelAll()
{
	// TODO: 在此添加命令处理程序代码
	if ( !m_bCurInputPos )
	{
		CRichEditCtrl* pREdit1 = ((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT1));
		pREdit1->SetSel(0, -1);
	}
	else
	{
		CRichEditCtrl* pREdit2 = ((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2));
		pREdit2->SetSel(0, -1);
	}
}
