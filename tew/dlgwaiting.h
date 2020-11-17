#pragma once

#include "config.h"
#include "afxwin.h"

// cdlgwaiting 对话框

class cdlgwaiting : public CDialogEx
{
	DECLARE_DYNAMIC(cdlgwaiting)

public:
	cdlgwaiting(CWnd* pParent=NULL);   // 标准构造函数
	virtual ~cdlgwaiting();

// 对话框数据
	enum { IDD=IDD_dlgwaiting };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	// 
	int m_escapetime;
	DECLARE_MESSAGE_MAP()
public:
	// message
	CEdit m_dlgwaitingmsg;
	CString m_csmsg;
	// time
	CEdit m_DLGWAITINGTIMER;
	CString m_cssecond;
	int m_isecond;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
