/********************************************************************
created:	2002/09/18
created:	18:9:2002   23:44
filename: 	C:\Development c++\Serial communication\SerialApp\serialThread.cpp
file path:	C:\Development c++\Serial communication\SerialApp
file base:	serialThread
file ext:	cpp
author:		Chaiyasit T.

  purpose:	This class is used to hanle doing thread of serial communcation.
*********************************************************************/
#include "StdAfx.h"
#include "afxwin.h"
#include "serialCtl.hpp"
#include "resource.h"
#include "xsvrDlg.h"
#include "serialThread.hpp"

const unsigned short MAX_MESSAGE = 1024;

IMPLEMENT_DYNCREATE(SerialThread,CWinThread)
//SerialThread::SerialThread()
//----------------------------------------------------------------------------
//Constructor
//
SerialThread::SerialThread() : ptrDlg(NULL)
{
}

//SerialThread::~SerialThread()
//----------------------------------------------------------------------------
//Deconstructor
//
SerialThread::~SerialThread()
{
	ptrDlg = NULL;
}

//SerialThread::InitInstance()
//----------------------------------------------------------------------------
//Deconstructor
//
BOOL
SerialThread::InitInstance()
{
	return TRUE;
}

// SerialThread::Run()
//----------------------------------------------------------------------------
// Description: This is a virtual function that is called when thread process
//               is created to be one task.
//
static BOOL g_bInit = FALSE;


int SerialThread::Run()
{
	// Check signal controlling and status to open serial communication.
	// enter if there is command of openning and port has be closed before.
	// 先打开 COM1 端口
	if (! OpenCOM1Port())
		return -1;

	m_dwToDo = SMS_READ;

	char *mess = new char[MAX_MESSAGE];
	unsigned int lenBuff;
	unsigned long lenMessage;

	while(ptrDlg->m_bSMSActiveProccess)
	{
		ProcessWrite();

		// 别太急，先等等
		Sleep(1888);
		lenMessage = 0;
		memset(mess, 0, MAX_MESSAGE);
		lenBuff = MAX_MESSAGE;

	//	ptrDlg->AddErrorInfo("reading");
		if (SCC::serialCtl().read_scc(mess,lenBuff,lenMessage))
		{
			if (lenMessage > 0)
			{
				ProcessRead(mess);
			}
			else // 超时
			{
			//	ReadTimeOut(mess);
			}
		}
		else
		{
			ptrDlg->m_bSMSActiveProccess = FALSE;
		}
	} // while(ptrDlg->activeProccess == TRUE)
	delete [] mess;

	return 0;
}
/****************************End of file**************************************/

BOOL SerialThread::SendSMS(LPCTSTR lpszSMS)
{

	return FALSE;
}

LPSTR RestoreCode(LPCTSTR lpText)
{
	int na = strlen(lpText);
	// printf("%d---\n", na);
	char *temp = new char[1024];
	WCHAR nchar[1024];
	for (int i=0; i<na/4; i++)
	{
		sscanf(lpText+i*4, "%04x", &nchar[i]);
		// printf("%s\n", tmp+i*4);
		// printf("%04X\n", nchar[i]);

		// printf("%d---%d\n", dwMinSize);
		// memcpy((char*)temp, (char*)&nchar[i], 4);
	}
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)nchar,-1,NULL,0,NULL,FALSE);
	WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)nchar,-1,temp,dwMinSize,NULL,FALSE);

	return temp;
}

BOOL SerialThread::ProcessRead(LPCTSTR szMess)
{
	if (strstr(szMess, "AT\r\r\nOK"))
	{
	//	CString str;
	//	str.Format("%0x-%0x-%0x-%0x-%0x%0x-O=%0x", (char*)szMess[0], (char*)szMess[1],(char*)szMess[2],(char*)szMess[3],(char*)szMess[4],(char*)szMess[5], 'O');
	//	ptrDlg->AddErrorInfo(str);
		ptrDlg->AddErrorInfo("已启动短信发送功能");
	}
	else if (strstr(szMess, "AT+CMGF=0\r\r\nOK"))
	{
		ptrDlg->AddErrorInfo("CMGF 模式启动成功");
	}
	else if (strstr(szMess, "AT+CLIP=1\r\r\nOK"))
	{
		ptrDlg->AddErrorInfo("来电显示已开启");
	}
	else if (strstr(szMess, "RING"))
	{
	//	ptrDlg->AddErrorInfo(szMess);
		m_listMess.push_front("ATH\r");
		char *p;
		if ( (p = strchr(szMess, '\"')) )
		{
			char *tmp = strchr(p+1,'\"');
			p[tmp-p] = NULL;
//			ptrDlg->AddErrorInfo("快接电话");
			char *buf = new char[1024];
			sprintf(buf, "来电 - %s", p+1);
			ptrDlg->AddErrorInfo(buf);
		}
	}
	else if (strstr(szMess, "ATH\r\r\nOK"))
	{
		ptrDlg->AddErrorInfo("来电已被挂断");
	}
	else if (strstr(szMess, "AT+CMGS="))
	{
	}
	else if (strstr(szMess, "+CMGS:"))
	{
		ptrDlg->AddErrorInfo("已成功发出一条短信");
	}
	else if (strstr(szMess, " \"SM\","))
	{
		char *p = strchr(szMess,  ',');
		if (p)
		{
			CString str;
			str.Format("接受并删除短信 - %s", p+1);
			// Delete SMS
			CString command;
			// STL 的顺序，后进先出
			command.Format("AT+CMGD=%s\r", p+1);
			m_listMess.push_front((LPCTSTR)command);

			command.Format("AT+CMGR=%s\r", p+1);
			m_listMess.push_front((LPCTSTR)command);

		//	ptrDlg->AddErrorInfo(str);
		}
	}
	else if (strstr(szMess, "+CMTI"))
	{
	}
	else if (strstr(szMess, "AT+CMGL"))
	{
	//	ptrDlg->AddErrorInfo(szMess);
	}
	else if (strstr(szMess, "AT+CNMI=2"))
	{
	}
	else if (strstr(szMess, "ATE1\r"))
	{
	}
	else if (strstr(szMess, "+CMGR:"))
	{
		char *p = strstr(szMess, "0891");
		int nLen;
		sscanf(p+56, "%02X", (char*)&nLen);
		char abc[512];
		ZeroMemory(abc, 512);
		memcpy(abc, p+58, nLen*2);
		
		CString str;
	//	str.Format("%s Len:%d", RestoreCode(abc), nLen*2);

		// 解码电话号码 ******************************
		char eft[12];
		ZeroMemory(&eft, 12);
		for (int i=0; i<6; i++)
		{
			int bidx = i*2;

			eft[bidx] = p[26+bidx+1];
			eft[bidx+1] = p[26+bidx];
		}
		eft[11] = NULL;
		// 解码电话号码 ******************************

		str.Format("收到短信 - 发送者号码(%s):%s", eft, RestoreCode(abc));
		ptrDlg->XDBMsgSMS(eft, "XEIM_SYSTEM", RestoreCode(abc));

		ptrDlg->AddErrorInfo(str);
	}
	else if (strstr(szMess, "AT+CMGD"))
	{
	//	ptrDlg->AddErrorInfo(szMess);
	}
	else if (strstr(szMess, "+CMGL:"))
	{
		ptrDlg->AddErrorInfo(szMess);
	//	char *p = strstr(szMess, "0891");
	//	char *tmp2 = strstr(p, "\r");
	//	p[tmp2-p] = NULL;

	//	ptrDlg->AddErrorInfo(p);

		char *p = strstr(szMess, "0891");
		char *pID = strstr(szMess, "+CMGL:");
		while (p)
		{
			char szID[8];
			ZeroMemory(szID, 8);
			for (int i=0; i<7; i++)
			{
				szID[i] = pID[7+i];
				if (pID[8+i] == ',')
					break;
			}

			CString command;
			// Delete SMS
			// 存在问题10.
			// STL 的顺序，后进先出
			command.Format("AT+CMGD=%s\r", szID);
			m_listMess.push_front((LPCTSTR)command);
	//		ptrDlg->AddErrorInfo(command);

			int nLen;
			sscanf(p+56, "%02X", (char*)&nLen);

			char abc[1024*2];
			ZeroMemory(abc, 1024*2);
			memcpy(abc, p+58, nLen*2);

			CString str;
			str.Format("%s Len:%d", RestoreCode(abc), nLen*2);
			str.Format("收到短信 - %s", RestoreCode(abc));
			ptrDlg->XDBMsgSMS("SMS_SYSTEM", "XEIM_SYSTEM", RestoreCode(abc));
			ptrDlg->AddErrorInfo(str);

			pID = strstr(p, "+CMGL:");
		//	p=strstr(p, "+CMGL:");
			p = NULL;
			if (pID)
				p=strstr(pID, "0891");
		}
	//	Sleep(2000);
	}

	else
	{
		CString str;
		str.Format("ProcessRead:%s", szMess);
		ptrDlg->AddErrorInfo(str);
	}

	return TRUE;
}

void SerialThread::ReadTimeOut(LPCTSTR mess)
{
	// 发送 AT 指令后，读数据超时，可能找不到 SMS Moderm
	if (ptrDlg->m_dwSMSStep == SMS_AT)
	{
		ptrDlg->m_dwSMSStep = -1;
		ptrDlg->AddErrorInfo("找不到 SMS Device，XEIM 系统将不能发送短信。");
		ptrDlg->m_bSMSActiveProccess = FALSE;
	}
}



BOOL SerialThread::OpenCOM1Port()
{
	if ( SCC::serialCtl().getStatusPort() == FALSE )
	{
		DCB configSerial_;
		configSerial_.ByteSize = 8;
		configSerial_.StopBits = ONESTOPBIT;
		configSerial_.Parity = NOPARITY;
		configSerial_.BaudRate = CBR_9600;

		// open port by calling api function of class serialCtl.
		if (! SCC::serialCtl().openPort(configSerial_,"COM1"))
		{
			ptrDlg->m_bSMSActiveProccess = FALSE;
			m_bSMSOpenPortActivate = FALSE;
			ptrDlg->AddErrorInfo("无法连接 COM1，请确定其他程序没有占用此端口。");
			// 结束线程
			return FALSE;
		}
		else
		{
			m_bSMSOpenPortActivate = TRUE;
			ptrDlg->AddErrorInfo("COM1 9600 已连接，正在查找 SMS Device...");
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL SerialThread::ProcessWrite()
{
	if (m_listMess.size())
	{
		m_dwToDo = SMS_READ;
		unsigned long len;
	//	ptrDlg->AddErrorInfo("write_scc");

		string str = m_listMess.front();
		m_listMess.pop_front();

		SCC::serialCtl().write_scc(str.c_str(),
			str.size(),len);

	}
	return TRUE;
}

void SerialThread::ProcessClose()
{
	// Check status and signal controlling to close serial communication.
	if (ptrDlg->m_dwSMSStep == SMS_CLOSE)
	{
		if (SCC::serialCtl().closePort() == TRUE)
		{
			// Show message that close when performming of closing port okay.
			ptrDlg->AddErrorInfo("SMS Device 已关闭！");
		}
	}
}

BOOL SerialThread::ProcessWrite(LPCTSTR lpszMess, DWORD dwLen)
{
//	m_dwToDo = SMS_READ;
//	DWORD len;

//	CString str;
//	str.Format("ProcessWrite:%s", lpszMess);
//	ptrDlg->AddErrorInfo(str);

//	SCC::serialCtl().write_scc(lpszMess, dwLen, len);
	m_listMess.push_back(lpszMess);

	return TRUE;
}

/*
string DecodeChinese(string InputStr)   
{
	wchar_t   Buf[300];
	for(int   i=0;i<InputStr.Length();i=i+4)
	{
		Buf[i/4]=StrToInt("0x"+InputStr.SubString(i+1,4));
	}   
	Buf[InputStr.Length()/4]=0;
	return WideCharToString(Buf);
}*/