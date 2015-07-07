/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#ifndef XEIM_DEF
#define XEIM_DEF

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
#define	XEIMMSG_NEWMESSAGE			XEIMMESSAGE + 0x0a	// 接收到文字消息
#define XEIMMSG_HTTPPOST			XEIMMESSAGE + 0x0b	// 接收到HTTP消息
#define	XEIMMSG_NEWFILE				XEIMMESSAGE + 0x0c	// 接收到文件消息
#define	XEIMMSG_FILE				XEIMMESSAGE + 0x0d	// 客户端接收到文件

// 客户端发送离线消息
#define XEIMMSG_MSGOFFLINE				XEIMMESSAGE + 0x0e
// 客户端端请求获取离线消息
#define XEIMMSG_GETMSGOFFLINE			XEIMMESSAGE + 0x0f
// 客户端请求获取离线消息数据
#define XEIMMSG_GETDATAOFFLINE			XEIMMESSAGE + 0x10

// 发送短信
#define XEIMMSG_SMS						XEIMMESSAGE + 0x11

// 客户端向服务器请求历史记录
#define XEIMMSG_GETHISTORYMSG			XEIMMESSAGE + 0x12
// 服务器通知客户端开始接受历史记录
#define XEIMMSG_RECVHISTORYDATA			XEIMMESSAGE + 0x13
// 客户端通知服务器开始发送历史记录数据
#define XEIMMSG_SENDHISTORYDATA			XEIMMESSAGE + 0x15
// 服务器开始向客户端发送历史记录数据
#define XEIMMSG_HISTORYDATA				XEIMMESSAGE + 0x16


// 客户端向服务器查询最新版本
#define XEIMMSG_CHECK_VERSION			XEIMMESSAGE + 0x17
// 服务器发送最新版号给客户端
#define XEIMMSG_VERSION					XEIMMESSAGE + 0x18


// 客户端发送普通短信
#define XEIMMSG_NORMAL_SMS				XEIMMESSAGE + 0x21

// 客户端发送的短信记录
#define XEIMMSG_SMS_RECORD				XEIMMESSAGE + 0x22

// 客户端通过程序提交短信
#define	XEIMMSG_APP_SMS					XEIMMESSAGE + 0x23

// 客户端新建项目
#define XEIMMSG_NEW_PROJECT				XEIMMESSAGE + 0x24
// 客户端请求获取项目
#define XEIMMSG_GET_PROJECT				XEIMMESSAGE + 0x25
// 服务器发送的项目
#define XEIMMSG_PROJECT					XEIMMESSAGE + 0x26
// 客户端请求获取工作列表
#define XEIMMSG_GET_SCHEDULE			XEIMMESSAGE + 0x27
// 服务器发送的工作
#define XEIMMSG_SCHEDULE				XEIMMESSAGE + 0x28

//////////////////////////////////////////////////////////////////////////
// 系统消息
//////////////////////////////////////////////////////////////////////////


#define XM_USER				WM_USER + 0x65

#define	XM_LOGIN			XM_USER + 0x01
#define	XM_LOGOFF			XM_USER + 0x02
#define XM_MESSAGE			XM_USER + 0x03
#define	XM_CONNECTED		XM_USER + 0x04
#define	XM_GETLIST			XM_USER + 0x05
#define XM_HTTPPOST			XM_USER + 0x06
// 文件消息处理
#define XM_NEWFILE			XM_USER + 0x07

// 双击User List TreeCtrl时发送的消息
#define	XM_EMDBLCLICKTREE	WM_USER + 0x08
// 处理用户请求的离线消息
#define XM_GETMSGOFFLINE	WM_USER + 0x09
// 离线消息
#define XM_OFFLINEMESSAGE	XM_USER + 0x10
// 离线消息数据
#define XM_GETDATAOFFLINE	XM_USER + 0x11

// 短信
#define XM_SMS				XM_USER + 0x12

// 客户端请求消息历史记录
#define XM_GETHISTORYDATA	XM_USER + 0x13

// 客户端通知服务器开始发送历史消息数据
#define XM_SENDHISTORYDATA	XM_USER + 0x15

// 客户端发送普通短信
#define XM_NORMAL_SMS		XM_USER + 0x16

// 记录客户端的短信
#define XM_RECORD_SMS		XM_USER + 0x18

// 记录客户端项目
#define XM_RECORD_PROJECT	XM_USER + 0x19

// 读取并发送客户端项目
#define XM_GET_PROJECT		XM_USER + 0x20

// 读取并发送客户端工作
#define XM_GET_SCHEDULE		XM_USER + 0x21

// XEIM 即时通讯系统采用的 TCP 通讯端口
#define PORT			8188

#define DATA_BUFSIZE	1024 * 2


#define	STATUS_ONLINE						1
#define	STATUS_OFFLINE						2


#define SIO_KEEPALIVE_VALS   _WSAIOW(IOC_VENDOR,4) 

#endif // XEIM_DEF