#pragma once
#include "afxwin.h"
#include "config.h"

// cdlgsystemcmd 对话框

class cdlgsystemcmd : public CDialogEx
{
	DECLARE_DYNAMIC(cdlgsystemcmd)

public:
	cdlgsystemcmd(CWnd* pParent=NULL);   // 标准构造函数
	virtual ~cdlgsystemcmd();

// 对话框数据
	enum { IDD=IDD_dlgsystemcmd };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 调用系统命令相关信息
	CEdit m_editsyscmdmsg;
	// cmd and 参数
	CString m_para[COMMENTPARSEPARA];
	// output message
	CString m_csoutputbuff;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// system cmd dlg cedit add cstring
	afx_msg LRESULT Ondlgsystemcmdeditaddcstring(WPARAM wParam, LPARAM lParam);
	// trm thread point
	int m_ptrmthread;
private:
	// 匿名管道参数
	int m_retutn;
	SECURITY_ATTRIBUTES m_sa;
	HANDLE m_hInReadPipe,m_hInWritePipe,m_hOutReadPipe,m_hOutWritePipe;
	STARTUPINFO m_si;
	PROCESS_INFORMATION m_ProcessInformation;
	unsigned long m_lBytesRead;
	// 初始化匿名管道
	int systemcmdinitpipe();
	// 命令参数计数
	int m_paramno;
	// time click
	int m_timer;
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
