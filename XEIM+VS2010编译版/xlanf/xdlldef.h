#ifndef XEIM_TYPE
#define	XEIM_TYPE

#include <WinSock2.h>

#define		WM_DEST_ACCEPT			WM_USER + 0x66
#define		WM_SEND_PROCESS			WM_USER + 0x68
#define		WM_SEND_NOTIFY			WM_USER + 0x69
#define		WM_SEND_CANCEL			WM_USER + 0xa2

#define		WM_DEST_CANCEL			WM_USER + 0xa1
#define		WM_RECV_FILE			WM_USER + 0x93
#define		WM_REFUSE_FILE			WM_USER + 0xb8


#define		WM_FILERECVEND			WM_USER + 0x64
#define		WM_FILESENDEND			WM_USER + 0x65
#define		WM_NEW_FILE				WM_USER + 0x68
#define		WM_RECV_CANCEL			WM_USER + 0xa3
#define		WM_FILE_FINISH			WM_USER + 0xa5	// 所有文件处理完毕了
//define		WM_REFUSE_FILE			WM_USER + 0xb8
// 通知父窗口调整窗口
#define		XM_SENDFILEADD			WM_USER + 0x20
#define		XM_SENDFILEMIN			WM_USER + 0x21
// 通知绑定成功与绑定端口
#define		X_WM_TCP_FILE_BIND_OK	WM_USER + 0x22

// 文件接收完毕
#define		X_WM_TCP_FILE_COMPLETED	WM_USER + 0x23



/////////////////////////////////////////////////////////
// msg 标识定义规范
// XEIMMSG + 0x01 开始
/////////////////////////////////////////////////////////
#define	XEIMMESSAGE					0xef000000
#define XEIMMSG_LOGIN				XEIMMESSAGE + 0x01
#define	XEIMMSG_MESSAGE				XEIMMESSAGE + 0x02
#define	XEIMMSG_USEROK				XEIMMESSAGE + 0x03
#define	XEIMMSG_USERBAD				XEIMMESSAGE + 0x04
#define	XEIMMSG_GETLIST				XEIMMESSAGE + 0x05
#define XEIMMSG_USERINFO			XEIMMESSAGE + 0x06
#define XEIMMSG_LISTEND				XEIMMESSAGE + 0x07
#define XEIMMSG_USERLIST			XEIMMESSAGE + 0x08
#define XEIMMSG_USERSTATUS			XEIMMESSAGE + 0x09
#define	XEIMMSG_NEWMESSAGE			XEIMMESSAGE + 0x0a
#define XEIMMSG_HTTPPOST			XEIMMESSAGE + 0x0b

#define	XEIMMSG_NEWFILE				XEIMMESSAGE + 0x0c	// 接收到文件消息

// 文件发送线程传递的参数
typedef struct
{
	// 接收线程消息的窗口句柄
	HWND		hWnd;
	// 要发送的文件地址
	char		szSendFileName[MAX_PATH];
	// 接收方所使用的 TCP 端口
	int			nPort;

} XTCP_SendData, *LPXTCP_SendData;

// 文件接收线程传递的数据
typedef struct
{
	// 接收线程消息的窗口句柄
	HWND	hWnd;
	// 保存文件的地址
	char	szSaveFileName[MAX_PATH];
	DWORD dwPort;

} XTCP_RecvData, *LPXTCP_RecvData;

// 文件信息，作为参数传递
typedef struct 
{
	char szFullName[MAX_PATH];
	char szShortName[MAX_PATH];
	char szExt[16];
	char szUID[32];
	char szUIP[128];
	SOCKET sock;
	DWORD dwPort;
	DWORD option;

} XFILEINFO, *LPXFILEINFO;

#define FILE_NEW			1		// 用户新发邮件
#define FILE_REFUSE			2		// 用户拒绝接收文件
#define	FILE_CANCEL			3		// 用户取消发送


// 文件信息
typedef struct {
	// 文件长度
	int		nFileLength;
	// 文件名
	char	szFileName[MAX_PATH];

} XTCP_FileInfo, *LPXTCP_FileInfo;



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

#endif 