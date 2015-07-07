#pragma once
#include "afxwin.h"
#include <string>
using namespace std;

// CSysMsg 对话框

class CSysMsg : public CDialog
{
	DECLARE_DYNAMIC(CSysMsg)

public:
	CSysMsg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSysMsg();

// 对话框数据
	enum { IDD = IDD_DLG_SYSTEM_MSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit m_SysMsg;
	virtual BOOL OnInitDialog();

	string sMsg;
};
