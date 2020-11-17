#pragma once
#include "afxwin.h"
#include "config.h"

// cdlgprocessing 对话框

class cdlgprocessing : public CDialogEx
{
	DECLARE_DYNAMIC(cdlgprocessing)

public:
	cdlgprocessing(CWnd* pParent=NULL);   // 标准构造函数
	virtual ~cdlgprocessing();

// 对话框数据
	enum { IDD=IDD_dlgprocessing };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// processing flag
	int *m_flag;
	int m_endflag;
	// message
	CEdit m_edit;
	CString m_csedit;
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
