/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#include "StdAfx.h"
#include "xtype.h"
#include "xdef.h"

//*******************************************************************************************
// XEIM 通讯消息结构
//*******************************************************************************************
XMSG::XMSG()
{
	m_nDataLength = 0;
	m_szData = NULL;
	m_szBuffer = NULL;
	m_nMessage = XEIMMESSAGE;
}

// 从数据包还原成 XMSG
XMSG::XMSG(const char* szBuffer, int nBufferLength)
{
	m_nDataLength = 0;
	m_szData = NULL;
	m_szBuffer = NULL;
	m_nMessage = XEIMMESSAGE;

	int nIdx = 0;
	// 确保该缓冲区是消息包数据
	try
	{
		memcpy((char*)&m_nMessage, szBuffer, sizeof(m_nMessage));
		if (XEIMMESSAGE == (m_nMessage & XEIMMESSAGE))
		{
		//	AfxMessageBox("This data is not a XEIM XMSG data. XMSG::XMSG");
		//	return;
			nIdx = sizeof(m_nMessage);
			memcpy((char*)&m_nDataLength, szBuffer+nIdx, sizeof(m_nDataLength));
			nIdx += sizeof(m_nDataLength);
			m_szData = new char[m_nDataLength];
			memcpy(m_szData, szBuffer+nIdx, m_nDataLength);
		}
	}
	catch(...)
	{
	//	AfxMessageBox("Error in the XMSG.");
		printf("");
	}
}

// XMSG 所装的数据
char *XMSG::GetData()
{
	return m_szData;
}

// 装入数据
void XMSG::SetData(const char* szData, int nDataLength)
{
	m_nDataLength = nDataLength;
	m_szData = new char[nDataLength];
	memcpy(m_szData, szData, nDataLength);
}

// XMSG 所装的数据长度
int XMSG::GetDataLenght()
{
	return m_nDataLength;
}

// XMSG 整体数据包
char *XMSG::GetBuffer()
{
	int nIdx = 0;
	m_szBuffer = new char[GetBufferLength()];
	memcpy(m_szBuffer, (char*)&m_nMessage, sizeof(m_nMessage));
	nIdx = sizeof(m_nMessage);
	memcpy(m_szBuffer+nIdx, (char*)&m_nDataLength, sizeof(m_nDataLength));
	nIdx += sizeof(m_nDataLength);
	memcpy(m_szBuffer+nIdx, m_szData, m_nDataLength);

	return m_szBuffer;
}

// XMSG 整体数据长度
int XMSG::GetBufferLength()
{
	int nLen = sizeof(m_nMessage);
	nLen += sizeof(m_nDataLength);
	nLen += m_nDataLength;

	return nLen;
}

XMSG::~XMSG()
{
	if (m_szData)
	{
		delete m_szData;
		m_szData = NULL;
	}
	if (m_szBuffer)
	{
		delete m_szBuffer;
		m_szData = NULL;
	}
}
//*******************************************************************************************
// End XEIM 通讯消息结构
//*******************************************************************************************





//*******************************************************************************************
// XEIM 帐号详细信息 通讯消息结构
//*******************************************************************************************
XUserDetail::XUserDetail()
{
	// 数据唯一标识
	m_nDataID = 0x111cfff;

	ZeroMemory((char*)m_szUID, sizeof(m_szUID));
	ZeroMemory((char*)m_szUNickName, sizeof(m_szUNickName));
	ZeroMemory((char*)m_szUPassword, sizeof(m_szUPassword));
	ZeroMemory((char*)m_szUGroup, sizeof(m_szUGroup));
	m_nUStatus = STATUS_OFFLINE;
}

XUserDetail::XUserDetail(const XUserDetail &xUserDetail)
{
	// 数据唯一标识
	m_nDataID = xUserDetail.m_nDataID;

	strcpy(m_szUPassword, xUserDetail.m_szUPassword);
	strcpy(m_szUID, xUserDetail.m_szUID);
	strcpy(m_szUNickName, xUserDetail.m_szUNickName);
	strcpy(m_szUGroup, xUserDetail.m_szUGroup);

	m_nUStatus = xUserDetail.m_nUStatus;
}

BOOL XUserDetail::IsDataType()
{
	// 数据唯一表示
	return 	m_nDataID == 0x111cfff;
}

void XUserDetail::SetUID(const char *szUID)
{
	strcpy(m_szUID, szUID);
}

// 用户昵称
void XUserDetail::SetUNickName(const char *szUNickName)
{
	strcpy(m_szUNickName, szUNickName);
}

// 用户密码
void XUserDetail::SetUPassword(const char *szUPassword)
{
	strcpy(m_szUPassword, szUPassword);
}

// 用户组信息
void XUserDetail::SetUGroup(const char *szUGroup)
{
	strcpy(m_szUGroup, szUGroup);
}

void XUserDetail::OnLine()
{
	m_nUStatus = STATUS_ONLINE;
}

void XUserDetail::OffLine()
{
	m_nUStatus = STATUS_OFFLINE;
}

// 返回用户ID
char *XUserDetail::GetUID()
{
	return (char*)m_szUID;
}

// 返回用户昵称
char *XUserDetail::GetUNickName()
{
	return (char*)m_szUNickName;
}

// 返回用户密码
char *XUserDetail::GetUPassword()
{
	return (char*)m_szUPassword;
}

// 返回用户分组
char *XUserDetail::GetUGroup()
{
	return (char*)m_szUGroup;
}

// 返回用户状态
int	XUserDetail::GetUStatus()
{
	return m_nUStatus;
}

// 从 XUserDetail 中提取 XUserInfo
void XUserDetail::GetUserInfo(XUserInfo &uinfo)
{
	uinfo.SetUID(m_szUID);
	uinfo.SetUNickName(m_szUNickName);
	uinfo.SetUGroup(m_szUGroup);

	if (m_nDataID == 10)
		uinfo.SetNumberAndSex(TRUE, FALSE);
	else
		uinfo.SetNumberAndSex(FALSE, FALSE);

	if (m_nUStatus == STATUS_ONLINE)
		uinfo.OnLine();
	else
		uinfo.OffLine();

}

BOOL XUserDetail::operator == (const XUserDetail &xUserDetail)
{
	int ret = strcmp(strlwr(m_szUID), strlwr((char*)xUserDetail.m_szUID));
	if (ret != 0)
		return FALSE;
	ret = strcmp(m_szUPassword, xUserDetail.m_szUPassword);
	if (ret != 0)
		return FALSE;

	return TRUE;
}

BOOL XUserDetail::operator == (const char *szUID)
{
	int ret = strcmp(strlwr((char*)m_szUID), strlwr((char*)szUID));
	if (ret != 0)
		return FALSE;

	return TRUE;
}

XUserDetail::~XUserDetail()
{
}
//*******************************************************************************************
// End XEIM 帐号详细信息 通讯消息结构
//*******************************************************************************************





//*******************************************************************************************
// XEIM 用户信息
//*******************************************************************************************
void XUserInfo::SetUID(const char *szUID)
{
	strcpy(m_szUID, strlwr((LPSTR)szUID));
}

void XUserInfo::SetUNickName(const char *szUNickName)
{
	strcpy(m_szUNickName, szUNickName);
}
// 用户组信息
void XUserInfo::SetUGroup(const char *szUGroup)
{
	strcpy(m_szUGroup, szUGroup);
}

void XUserInfo::OnLine()
{
	m_nUStatus = STATUS_ONLINE;
}

void XUserInfo::OffLine()
{
	m_nUStatus = STATUS_OFFLINE;
}

char *XUserInfo::GetUID()
{
	return (char*)m_szUID;
}

char *XUserInfo::GetUNickName()
{
	return (char*)m_szUNickName;
}

int	XUserInfo::GetUStatus()
{
	return m_nUStatus;
}

// 返回用户分组
char *XUserInfo::GetUGroup()
{
	return (char*)m_szUGroup;
}

XUserInfo::XUserInfo()
{
	ZeroMemory((char*)m_szUID, sizeof(m_szUID));
	ZeroMemory((char*)m_szUNickName, sizeof(m_szUNickName));
	m_nUStatus = STATUS_OFFLINE;
}

BOOL XUserInfo::operator == (const XUserInfo &xUserInfo)
{
	int ret = strcmp(strlwr((char*)m_szUID), strlwr((char*)xUserInfo.m_szUID));
	if (ret != 0)
		return FALSE;

	return TRUE;
}

XUserInfo::~XUserInfo()
{
}
//*******************************************************************************************
// End XEIM 用户信息
//*******************************************************************************************



//**********************************************************************************
// 在线消息 XXMSG_ONLINE
//**********************************************************************************
XMSG_ONLINE::XMSG_ONLINE()
{
	m_nDataLen = 0;
	buffer = NULL;
	memset((char*)m_szUID, 0, sizeof(m_szUID));
}
XMSG_ONLINE::XMSG_ONLINE(const char *buf)
{
	m_nDataLen = 0;
	buffer = NULL;
	memset((char*)m_szUID, 0, sizeof(m_szUID));

	int i=0;
	memcpy(m_szUID, buf, sizeof(m_szUID));
	i = sizeof(m_szUID);

	memcpy((char*)&m_nDataLen, buf+i, sizeof(m_nDataLen));
	i += sizeof(m_nDataLen);

	buffer = new char[m_nDataLen];
	memcpy(buffer, buf+i, m_nDataLen);
}

XMSG_ONLINE::XMSG_ONLINE(const char *szid, const char *data, int len)
{
	m_nDataLen = len;
	buffer = new char[len];
	memcpy(buffer, data, len);
	memset((char*)m_szUID, 0, sizeof(m_szUID));
	memcpy(m_szUID, szid, sizeof(m_szUID));
}

// 设置 DATA
void XMSG_ONLINE::SetData(const char *szid, const char *szData, int len)
{
	m_nDataLen = len;
	buffer = new char[len];
	memcpy(buffer, szData, len);
	strcpy(m_szUID, szid);
}

void XMSG_ONLINE::SetUID(const char *szid)
{
	memset((char*)m_szUID, 0, sizeof(m_szUID));
	strcpy(m_szUID, szid);
}

// 获取 Data
char * XMSG_ONLINE::GetData()
{
	return buffer;
}

// 接受者用户ID
char * XMSG_ONLINE::GetUID()
{
	return (char*)m_szUID;
}

// 获取在线消息数据包
char *XMSG_ONLINE::GetBuffer()
{
	char *temp;
	int len = GetBufferLength();
	temp = new char[len];

	// 打包数据
	memcpy(temp, m_szUID, sizeof(m_szUID));		// ID
	int i = sizeof(m_szUID);
	memcpy(temp+i, (char*)&m_nDataLen, sizeof(m_nDataLen));	// Data Length
	i += sizeof(m_nDataLen);
	memcpy(temp+i, buffer, m_nDataLen);					// Data

	delete buffer;
	buffer = temp;

	return temp;
}

// 获取在线消息数据包长度
int XMSG_ONLINE::GetBufferLength()
{
	int total = sizeof(m_szUID)	// ID
			+ sizeof(m_nDataLen)	// Data Length
			+ m_nDataLen;			// Data

	return total;
}

XMSG_ONLINE::~XMSG_ONLINE()
{
	if (buffer)
	{
		delete buffer;
		buffer = NULL;
	}
}


//**********************************************************************************
// End 在线消息 XXMSG_ONLINE
//**********************************************************************************




//**********************************************************************************
// 离线消息 XMSG_OFFLINE
//**********************************************************************************
XMSG_OFFLINE::XMSG_OFFLINE()
{
	m_nDataLen = 0;
	buffer = NULL;
	memset((char*)m_szUID, 0, sizeof(m_szUID));
}

XMSG_OFFLINE::XMSG_OFFLINE(const char *buf)
{
	try
	{
		m_nDataLen = 0;
		buffer = NULL;
		memset((char*)m_szUID, 0, sizeof(m_szUID));

		int i=0;
		memcpy(m_szUID, buf, sizeof(m_szUID));
		i = sizeof(m_szUID);

		memcpy((char*)&m_nDataLen, buf+i, sizeof(m_nDataLen));
		i += sizeof(m_nDataLen);

		buffer = new char[m_nDataLen];
		memcpy(buffer, buf+i, m_nDataLen);
	}
	catch(...)
	{
		MessageBox(NULL, "XMSG_OFFLINE::XMSG_OFFLINE(const char *buf)", "catch(...)", MB_OK);
	}
}

XMSG_OFFLINE::XMSG_OFFLINE(const char *szid, const char *data, int len)
{
	m_nDataLen = len;
	buffer = new char[len];
	memcpy(buffer, data, len);
	memset((char*)m_szUID, 0, sizeof(m_szUID));
	strcpy(m_szUID, szid);
}

// 设置 DATA
void XMSG_OFFLINE::SetData(const char *szid, const char *szData, int len)
{
	m_nDataLen = len;
	buffer = new char[len];
	memcpy(buffer, szData, len);
	strcpy(m_szUID, szid);
}

void XMSG_OFFLINE::SetUID(const char *szid)
{
	memset((char*)m_szUID, 0, sizeof(m_szUID));
	strcpy(m_szUID, szid);
}

// 获取 Data
char * XMSG_OFFLINE::GetData()
{
	return buffer;
}

// 从数据里获取 Data
char * XMSG_OFFLINE::GetData(char *buf)
{
	int len = GetDataLength(buf);
	int offset = sizeof(m_szUID) + sizeof(m_nDataLen);
	buffer = buf+offset;
	return buffer;
}

// 接受者用户ID
char * XMSG_OFFLINE::GetUID()
{
	return (char*)m_szUID;
}

// 从缓冲区获取UID
char * XMSG_OFFLINE::GetUID(char *buf)
{
	memset((char*)m_szUID, 0, sizeof(m_szUID));
	memcpy(m_szUID, buf, sizeof(m_szUID));

	return (char*)m_szUID;
}

DWORD XMSG_OFFLINE::GetDataLength()
{
	return m_nDataLen;
}

// 获取消息数据的长度
DWORD XMSG_OFFLINE::GetDataLength(char *buf)
{
	try
	{
		memcpy((char*)&m_nDataLen, buf+sizeof(m_szUID), sizeof(m_nDataLen));
	}
	catch(...)
	{
		MessageBox(NULL, "XMSG_OFFLINE::GetDataLength", "catch(...)", MB_OK);
	}

	return m_nDataLen;
}

// 获取在线消息数据包
char *XMSG_OFFLINE::GetBuffer()
{
	char *temp;
	int len = GetBufferLength();
	temp = new char[len];

	// 打包数据
	memcpy(temp, m_szUID, sizeof(m_szUID));		// ID
	int i = sizeof(m_szUID);
	memcpy(temp+i, (char*)&m_nDataLen, sizeof(m_nDataLen));	// Data Length
	i += sizeof(m_nDataLen);
	memcpy(temp+i, buffer, m_nDataLen);					// Data

	delete buffer;
	buffer = temp;

	return temp;
}

// 获取在线消息数据包长度
int XMSG_OFFLINE::GetBufferLength()
{
	int total = sizeof(m_szUID)	// ID
			+ sizeof(m_nDataLen)	// Data Length
			+ m_nDataLen;			// Data

	return total;
}

XMSG_OFFLINE::~XMSG_OFFLINE()
{
	if (buffer)
	{
		delete buffer;
		buffer = NULL;
	}
}

BOOL XMSG::IsMsgType()
{
	return (XEIMMESSAGE == (m_nMessage & XEIMMESSAGE));
}

//**********************************************************************************
// End 离线消息 XMSG_OFFLINE
//**********************************************************************************




//**********************************************************************************
// 离线消息 XMSG_OFFLINEDATA
//**********************************************************************************
XMSG_OFFLINEDATA::XMSG_OFFLINEDATA()
{
	m_nDataLen = 0;
	buffer = NULL;
	memset((char*)m_szUID, 0, sizeof(m_szUID));
}

XMSG_OFFLINEDATA::XMSG_OFFLINEDATA(const char *buf)
{
	m_nDataLen = 0;
	buffer = NULL;
	memset((char*)m_szDate, 0, sizeof(m_szDate));
	memset((char*)m_szUID, 0, sizeof(m_szUID));

	int i=0;
	memcpy(m_szUID, buf, sizeof(m_szUID));
	i = sizeof(m_szUID);

	memcpy(m_szDate, buf+i, sizeof(m_szDate));
	i += sizeof(m_szDate);

	memcpy((char*)&m_nDataLen, buf+i, sizeof(m_nDataLen));
	i += sizeof(m_nDataLen);

	buffer = new char[m_nDataLen];
	ZeroMemory(buffer, m_nDataLen);

	memcpy(buffer, buf+i, m_nDataLen);
}

XMSG_OFFLINEDATA::XMSG_OFFLINEDATA(const char *szid, const char *data, int len)
{
	m_nDataLen = len;
	buffer = new char[len];
	memcpy(buffer, data, len);
	memset((char*)m_szUID, 0, sizeof(m_szUID));
	memcpy(m_szUID, szid, sizeof(m_szUID));
}

// 设置 DATA
/*void XMSG_OFFLINEDATA::SetData(const char *szid, const char *szdate, const char *buf, int len)
{
	m_nDataLen = len;
	buffer = new char[len];
	memcpy(buffer, buf, len);
	strcpy(m_szUID, szid);
	strcpy(m_szDate, szdate);
}*/

void XMSG_OFFLINEDATA::SetData(const char *buf)
{
	m_nDataLen = strlen(buf)+1;
	buffer = new char[m_nDataLen];
	memcpy(buffer, buf, m_nDataLen);
}

void XMSG_OFFLINEDATA::SetUID(const char *szid)
{
	memset((char*)m_szUID, 0, sizeof(m_szUID));
	memcpy(m_szUID, szid, sizeof(m_szUID));
}

void XMSG_OFFLINEDATA::SetDate(const char *szdate)
{
	memset((char*)m_szDate, 0, sizeof(m_szDate));
	strcpy(m_szDate, szdate);
}

// 获取 Data
char * XMSG_OFFLINEDATA::GetData()
{
	return buffer;
}

// 接受者用户ID
char * XMSG_OFFLINEDATA::GetUID()
{
	return (char*)m_szUID;
}

// 离线消息的发送日期
char * XMSG_OFFLINEDATA::GetDate()
{
	return (char*)m_szDate;
}

// 获取在线消息数据包
char *XMSG_OFFLINEDATA::GetBuffer()
{
	int len = GetBufferLength();
	char *temp;
	temp = new char[len];

	// 打包数据
	memcpy(temp, m_szUID, sizeof(m_szUID));		// ID
	int i = sizeof(m_szUID);

	memcpy(temp+i, m_szDate, sizeof(m_szDate));	// Date 日期
	i += sizeof(m_szDate);

	memcpy(temp+i, (char*)&m_nDataLen, sizeof(m_nDataLen));	// Data Length
	i += sizeof(m_nDataLen);

	memcpy(temp+i, buffer, m_nDataLen);					// Data

	delete buffer;
	buffer = temp;

	return buffer;
}

// 获取在线消息数据包长度
int XMSG_OFFLINEDATA::GetBufferLength()
{
	int total = sizeof(m_szUID)		// ID
			+ sizeof(m_szDate)		// Date
			+ sizeof(m_nDataLen)	// Data Length
			+ m_nDataLen;			// Data

	return total;
}


DWORD XMSG_OFFLINEDATA::GetDataLen(const char *buf)
{
	int i=0;
	i = sizeof(m_szUID);
	i += sizeof(m_szDate);
	memcpy((char*)&m_nDataLen, buf+i, sizeof(m_nDataLen));
	i += sizeof(m_nDataLen);
	i += m_nDataLen;

	return i;
}

XMSG_OFFLINEDATA::~XMSG_OFFLINEDATA()
{
	if (buffer)
	{
		delete buffer;
		buffer = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

XMSG_SMS::XMSG_SMS()
{
	ZeroMemory((char*)m_szNUM, sizeof(m_szNUM));
	m_buf = NULL;
	m_dwBufLen = 0;
	m_dwIndex = 255;
	m_dwTotal = 0;
}

XMSG_SMS::XMSG_SMS(const char *buf)
{
	ZeroMemory((char*)m_szNUM, sizeof(m_szNUM));
	m_buf = NULL;
	m_dwBufLen = 0;
	m_dwIndex = 255;
	m_dwTotal = 0;

	DWORD idx = 0;
	memcpy((char*)&m_dwIndex, buf, sizeof(DWORD));
	idx += sizeof(DWORD);

	memcpy(m_szNUM, buf+idx, sizeof(m_szNUM));
	idx += sizeof(m_szNUM);

	memcpy((char*)&m_dwBufLen, buf+idx, sizeof(DWORD));
	idx += sizeof(DWORD);

	m_buf = new char [m_dwBufLen];
	memcpy(m_buf, buf+idx, m_dwBufLen);
}

XMSG_SMS::~XMSG_SMS()
{
	if (m_buf)
	{
		delete [] m_buf;
		m_buf = NULL;
	}
}

void XMSG_SMS::SetData(LPCTSTR szNUM, LPCTSTR buf, DWORD bufLen)
{
	memcpy(m_szNUM, szNUM, sizeof(m_szNUM));
	m_buf = new char[bufLen];
	m_dwBufLen = bufLen;
	strcpy(m_buf, buf);

	m_dwTotal = sizeof(m_szNUM);
	m_dwTotal += sizeof(DWORD)*2;
	m_dwTotal += m_dwBufLen;
}

char *XMSG_SMS::GetData()
{
	DWORD idx=0;

	char *tmp = new char [m_dwTotal];

	memcpy(tmp, (char*)&m_dwIndex, sizeof(DWORD));
	idx += sizeof(DWORD);

	memcpy(tmp+idx, m_szNUM, sizeof(m_szNUM));
	idx += sizeof(m_szNUM);

	memcpy(tmp+idx, (char*)&m_dwBufLen, sizeof(DWORD));
	idx += sizeof(DWORD);

	memcpy(tmp+idx, m_buf, m_dwBufLen);

	delete [] m_buf;
	m_buf = tmp;

	return tmp;
}

char * XMSG_SMS::GetBuffer()
{
	return m_buf;
}

DWORD XMSG_SMS::GetBufferLength()
{
	return m_dwTotal;
}

char * XMSG_SMS::GetNUM()
{
	return (char*)m_szNUM;
}
//////////////////////////////////////////////////////////////////////////

//**********************************************************************************
// End 离线消息 XMSG_OFFLINEDATA
//**********************************************************************************


void XUserInfo::SetNumberAndSex(BOOL bNUM, BOOL bSex)
{
	char n1;
	char n2;
	n1 = (char)bNUM;
	n2 = (char)bSex;

	m_szUID[29] = n1;
	m_szUID[30] = n2;
}

BOOL XUserInfo::UserHasMobile()
{
	char n1 = (int)m_szUID[29];
	return n1==1;
}
