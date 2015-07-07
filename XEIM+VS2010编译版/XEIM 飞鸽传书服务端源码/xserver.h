/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#ifndef XSVR_H
#define	XSVR_H

#include "../xmsg/xdef.h"
#include "winsock2.h"


#pragma comment(lib, "ws2_32")

class xserver
{
public:

	static CString m_strLastError;
	BOOL InitServer();

	// 专门用于关闭套接字
	static void XEIM_closesocket(DWORD tid, SOCKET sock);

	static BOOL IsHttpPost(LPCTSTR lpszBuffer);
	// 启动线程
	void run(HWND hWnd);

	xserver();
	~xserver();
private:

	// 服务器线程数量
	static DWORD m_dwThreads;
	// 线程句柄
	static HANDLE *m_phThread;
	// 数据处理线程
//	static void recv_proc(void* tdata);
	static DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortID);
	// 连接管理线程
	static void lisn_proc(void* tdata);
};
#endif // XSVR_H
