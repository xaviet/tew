#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// cdlgdutmsg 对话框

class cdlgdutmsg : public CDialogEx
{
	DECLARE_DYNAMIC(cdlgdutmsg)

public:
	cdlgdutmsg(CWnd* pParent=NULL);   // 标准构造函数
	virtual ~cdlgdutmsg();

// 对话框数据
	enum { IDD=IDD_dlgdutmsg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// dut message
	CListCtrl m_listdutmsg;
	// tew dialog point
	CDialogEx *m_ptewdlg;
	// init list
	int dutmsginitlist();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	// dut message edit
	CEdit m_editmsg;
	// list条目数量
	int m_iitemcounter;
	// 当前点击项  
	int m_nItem;
	// 当前点击子项  
	int m_nSubItem;     
	// Edit是否可见  
	bool m_bEditVisible; 
	// show edit
	void dutmsgShowEdit(bool v_bShow,int v_nItem,int v_nSubItem,CRect v_rcCtrl); 
	// dut msg check
	CString dutmsgcheck();
	afx_msg void OnEnKillfocusdlgdutmsgedit();
	// edit end
	afx_msg LRESULT Ondlgdutmsgeditend(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMDblclklistdutmsg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnBegintracklistdutmsg(NMHDR *pNMHDR, LRESULT *pResult);
};
