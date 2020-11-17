#pragma once
#include "afxwin.h"
#include "config.h"

//  派生cedit

#define MES_UNDO        _T("&Undo")
#define MES_CUT         _T("Cu&t")
#define MES_COPY        _T("&Copy")
#define MES_PASTE       _T("&Paste")
#define MES_DELETE      _T("&Delete")
#define MES_SELECTALL   _T("Select &All")

class cscriptedit : public CEdit
{
public:
    cscriptedit();
	int oneditinit();

protected:
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    DECLARE_MESSAGE_MAP()

private:
	// 当前编辑框选择位置
	int m_iposition;
	// NG array
	int m_ngarray[256];
	// 选择编辑框内容位置
	int setposition(int,int);
	// 定位上一个NG
	int onscripteditlastng();
	// 定位下一个NG
	int onscripteditnextng();
};

// cscripteditdlg 对话框

class cscripteditdlg : public CDialogEx
{
	DECLARE_DYNAMIC(cscripteditdlg)

public:
	cscripteditdlg(CWnd* pParent=NULL);   // 标准构造函数
	virtual ~cscripteditdlg();
	virtual BOOL OnInitDialog();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void PostNcDestroy();
// 对话框数据
	enum { IDD=IDD_scripteditdlg };

private:
	// edit 内容
	CString m_editcs;
	// 内容修改标志
	int m_modifyflag;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// new font
	CFont m_editFont;
	// script
	cscriptedit m_scriptedit;
	// script file name
	CString m_filename;
	// node property dlg point
	CDialogEx *m_pnodepropertydlg;
	//app dlg point
	CDialogEx *m_pappdlg;
	// read only flag
	int m_readonlyflag;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnCancel();
	afx_msg void OnEnChangescripteditor();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
