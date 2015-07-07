/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#ifndef XEIM_TYPE
#define	XEIM_TYPE

#include "xdef.h"
#include "winsock2.h"
class XUserInfo;

/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
//*******************************************************************************************
// XEIM 通讯消息结构
//*******************************************************************************************
class XMSG
{
public:
	int m_nMessage;		// 消息类型定义在 xdef.h 里
private:
	char *m_szData;		// 包含的数据
	char *m_szBuffer;	// 整体数据
	int m_nDataLength;
public:
	BOOL IsMsgType();
	XMSG();
	XMSG(const char* szBuffer, int nBufferLength); // 从数据包还原成 XMSG

	char *GetData();		// XMSG 所装的数据
	void SetData(const char* szData, int nDataLength); // 装入数据
	int GetDataLenght();	// XMSG 所装的数据长度

	char *GetBuffer();		// XMSG 整体数据包
	int GetBufferLength();	// XMSG 整体数据长度

	~XMSG();
};
//*******************************************************************************************
// End XEIM 通讯消息结构
//*******************************************************************************************




/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
//*******************************************************************************************
// XEIM 帐号详细信息 通讯消息结构
//*******************************************************************************************
class XUserDetail
{
	char	m_szUID[32];
	char	m_szUNickName[32];
	char	m_szUPassword[32];
	char	m_szUGroup[32];
	int		m_nUStatus;

public:
	int		m_nDataID;
	BOOL IsDataType();
	XUserDetail();
	XUserDetail(const XUserDetail &xUserDetail);
	void GetUserInfo(XUserInfo &uinfo);
	void SetUID(const char *szUID);
	void SetUNickName(const char *szUNickName);
	void SetUPassword(const char *szUPassword);
	void SetUGroup(const char *szUGroup);
	void OnLine();
	void OffLine();

	char *GetUID();
	char *GetUNickName();
	char *GetUPassword();
	char *GetUGroup();
	int	GetUStatus();

	BOOL operator == (const XUserDetail &xUserDetail);
	BOOL operator == (const char *szUID);
	~XUserDetail();
};
//*******************************************************************************************
// End XEIM 帐号详细信息 通讯消息结构
//*******************************************************************************************






/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
//*******************************************************************************************
// XEIM 用户信息
//*******************************************************************************************
class XUserInfo
{
	char	m_szUID[32];
	char	m_szUNickName[32];
	char	m_szUGroup[32];
	int		m_nUStatus;
public:
	BOOL UserHasMobile();
	void SetNumberAndSex(BOOL bNUM, BOOL bSex);
	void SetUID(const char *szUID);
	void SetUNickName(const char *szUNickName);
	void SetUGroup(const char *szUGroup); // 设置用户所在的部门名
	void OnLine();
	void OffLine();

	char *GetUID();
	char *GetUNickName();
	char *GetUGroup();		// 得到用户所在的部门名
	int	GetUStatus();

	BOOL operator == (const XUserInfo &xUserInfo);
	XUserInfo();
	~XUserInfo();
};
//*******************************************************************************************
// End XEIM 用户信息
//*******************************************************************************************


/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
//**********************************************************************************
// 在线消息 XXMSG_ONLINE
//**********************************************************************************
class XMSG_ONLINE
{
public:
	XMSG_ONLINE();
	XMSG_ONLINE(const char *buf);

	// 构造
	XMSG_ONLINE(const char *szid,
			const char *data,
			int len);

	// 设置 DATA
	void SetData(const char *szid,
			const char *szData,
			int len);
	void SetUID(const char *szid);

	// 获取 Data
	char * GetData();
	// 接受者用户ID
	char * GetUID();

	// 获取在线消息数据包
	char *GetBuffer();
	// 获取在线消息数据包长度
	int GetBufferLength();

	~XMSG_ONLINE();
private:
	// 发送的消息长度
	int m_nDataLen;
	// 发送的消息
	char *buffer;
	// 消息接收者
	char	m_szUID[16];

};
//**********************************************************************************
// End 在线消息
//**********************************************************************************

/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
//**********************************************************************************
// 离线消息 XXMSG_OFFLINE
//**********************************************************************************
class XMSG_OFFLINE
{
public:
	XMSG_OFFLINE();
	XMSG_OFFLINE(const char *buf);

	// 构造
	XMSG_OFFLINE(const char *szid,
			const char *data,
			int len);

	// 设置 DATA
	void SetData(const char *szid,
			const char *szData,
			int len);
	void SetUID(const char *szid);

	// 获取 Data
	char * GetData();
	// 从数据里获取 Data
	char * GetData(char *buf);
	// 接受者用户ID
	char * GetUID();
	// 从缓冲区获取UID
	char * GetUID(char *buf);
	DWORD GetDataLength();
	// 获取消息数据的长度
	DWORD GetDataLength(char *buf);

	// 获取在线消息数据包
	char *GetBuffer();
	// 获取在线消息数据包长度
	int GetBufferLength();

	~XMSG_OFFLINE();
private:
	// 发送的消息长度
	int m_nDataLen;
	// 发送的消息
	char *buffer;
	// 消息接收者
	char	m_szUID[16];

};
//**********************************************************************************
// End 离线消息
//**********************************************************************************


/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
//**********************************************************************************
// 服务器发给客户端的离线消息 XMSG_OFFLINEDATA
//**********************************************************************************
class XMSG_OFFLINEDATA
{
public:
	DWORD GetDataLen(const char *buf);
	// 构造
	XMSG_OFFLINEDATA();
	XMSG_OFFLINEDATA(const char *buf);
	XMSG_OFFLINEDATA(const char *szid,const char *data,int len);


//	void SetData(const char *szid, const char *szdate, const char *buf, int len);
	void SetUID(const char *szid);
	void SetDate(const char *szdate);
	void SetData(const char *buf);


	char * GetData();// 获取 Data
	char * GetUID();// 接受者用户ID
	char * GetDate();// 离线消息的发送日期
	char * GetBuffer();// 获取在线消息数据包
	int GetBufferLength();// 获取在线消息数据包长度


	~XMSG_OFFLINEDATA();
private:

	int		m_nDataLen;		// 发送的消息长度
	char	*buffer;		// 发送的消息
	char	m_szUID[50];	// 消息接收者
	char	m_szDate[28];	// 离线发送消息日期
};
//**********************************************************************************
// End 服务器发给客户端的离线消息 XMSG_OFFLINEDATA
//**********************************************************************************

/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
typedef struct
{
	HWND hWnd;
	SOCKET sock;
}TDATA, *LPTDATA;

typedef struct {
	OVERLAPPED Overlapped;
	WSABUF		DataBuf;
	CHAR Buffer[DATA_BUFSIZE];
	DWORD send;
	DWORD DataSize;
	HWND	hWnd;
} PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

typedef struct {
	SOCKET Sock;
} PER_HEANDLE_DATA, *LPPER_HANDLE_DATA;


/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
//定义结构及宏 
typedef struct 
{
	u_long onoff;
	u_long keepalivetime;
	u_long keepaliveinterval;
}TCP_KEEPALIVE2;

class XMSG_SMS
{
public:
	DWORD m_dwIndex;
	XMSG_SMS();
	XMSG_SMS(const char *buf);
	~XMSG_SMS();
	void SetData(LPCTSTR szNUM, LPCTSTR buf, DWORD bufLen);
	char * GetData();
	char * GetNUM();
	char * GetBuffer();
	DWORD GetBufferLength();
private:

	char m_szNUM[12];
	DWORD m_dwBufLen;
	DWORD m_dwTotal;
	char *m_buf;
};

void GetProgramDirectory(char *buf);

#endif // XEIM_TYPE