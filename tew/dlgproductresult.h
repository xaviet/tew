#pragma once
#include "afxcmn.h"


// cdlgproductresult 对话框

class cdlgproductresult : public CDialogEx
{
	DECLARE_DYNAMIC(cdlgproductresult)

public:
	cdlgproductresult(CWnd* pParent=NULL);   // 标准构造函数
	virtual ~cdlgproductresult();

// 对话框数据
	enum { IDD=IDD_dlgproductresult };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	// tew dialog point
	CDialogEx *m_ptewdlg;
	// init list
	int productresultinitlist();
	virtual BOOL OnInitDialog();
	// product result
	CListCtrl m_listproductresult;
	// list条目数量
	int m_iitemcounter;
};
