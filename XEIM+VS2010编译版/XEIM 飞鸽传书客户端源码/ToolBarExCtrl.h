/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#if !defined(AFX_TOOLBAREXCTRL_H__0223C89F_6CAA_4C80_BDF9_3672B21CD2D1__INCLUDED_)
#define AFX_TOOLBAREXCTRL_H__0223C89F_6CAA_4C80_BDF9_3672B21CD2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarExCtrl.h : header file
//
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////

#define BT_EX_ENABLED	1
#define BT_EX_DISABLED	0
#define BT_EX_HOT		2

#define BT_EX_BUTTON		1
#define BT_EX_SEPARATOR		2


// CToolBarExCtrl window
typedef struct _BUTTONEX {
	HICON icon;		// button image
	int idCommand;  // command to be sent when button pressed
	BYTE fsState;   // button state
	BYTE fsStyle;   // button style
	LPSTR tooltip;	// button tooltip
} ButtonEx;


class CToolBarExCtrl : public CStatic
{
// Construction
public:
	CToolBarExCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarExCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	ButtonEx* GetButton(int nIndex, int commandId);
	void EnableButton(int nId, bool bEnable);
	void SetBackGroundColor(COLORREF color);
	void RemoveButton(int nIndex, int commandId);
	void AddSeparator();
	void AddButton(HICON icon,int commandId, LPCTSTR text);
	void SetButtonSize(CSize size);
	void SetButtonSpacing(CSize spacing);
	virtual ~CToolBarExCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolBarExCtrl)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int m_pushedButton;
	int m_hotButton;
	CToolTipCtrl m_ToolTip;
	COLORREF m_backGroundColor;
	CArray <ButtonEx,ButtonEx&> buttons;
	CArray <CRect,CRect> rects;
	CSize m_buttonSize;
	CSize m_buttonSpacing;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBAREXCTRL_H__0223C89F_6CAA_4C80_BDF9_3672B21CD2D1__INCLUDED_)
