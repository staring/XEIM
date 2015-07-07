#include "StdAfx.h"
#include "xdlldef.h"


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
	memcpy((char*)&m_nMessage, szBuffer, sizeof(m_nMessage));
	if (XEIMMESSAGE != (m_nMessage & XEIMMESSAGE))
	{
		printf("This data is not a XEIM XMSG data. XMSG::XMSG");
		return;
	}
	nIdx = sizeof(m_nMessage);
	memcpy((char*)&m_nDataLength, szBuffer+nIdx, sizeof(m_nDataLength));
	nIdx += sizeof(m_nDataLength);
	m_szData = new char[m_nDataLength];
	memcpy(m_szData, szBuffer+nIdx, m_nDataLength);
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
	strcpy(m_szUID, szid);
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


