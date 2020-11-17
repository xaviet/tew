#pragma once
#include "afxwin.h"
#include "config.h"

// cdlgconfig 对话框

class cdlgconfig : public CDialogEx
{
	DECLARE_DYNAMIC(cdlgconfig)

public:
	cdlgconfig(CWnd* pParent=NULL);   // 标准构造函数
	virtual ~cdlgconfig();

// 对话框数据
	enum { IDD=IDD_dlgconfig };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 循环执行
	CButton m_checkloop;
	virtual BOOL OnInitDialog();
	// app dialog point
	CDialogEx *m_pappdlg;
	afx_msg void OnBnClickedOk();
	// tewdlg配置标志
	struct tewdlgconfigflag *m_pconfigflag;
	// ng stop flag
	CButton m_checkngstop;
	CMFCEditBrowseCtrl m_zlogdir;
};
