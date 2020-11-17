#pragma once
#include "afxwin.h"

// aboutdlg 对话框

class caboutdlg : public CDialogEx
{
	DECLARE_DYNAMIC(caboutdlg)

public:
	caboutdlg(CWnd* pParent=NULL);   // 标准构造函数
	virtual ~caboutdlg();

// 对话框数据
	enum { IDD=IDD_dlgabout };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// version
	CString m_csversion;
private:
	// version
	CEdit m_editversion;
	// thanks display position
	int m_pos;
public:
	// thanks
	CEdit m_thanks;
	CString m_csthks;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
