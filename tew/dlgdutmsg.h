#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// cdlgdutmsg �Ի���

class cdlgdutmsg : public CDialogEx
{
	DECLARE_DYNAMIC(cdlgdutmsg)

public:
	cdlgdutmsg(CWnd* pParent=NULL);   // ��׼���캯��
	virtual ~cdlgdutmsg();

// �Ի�������
	enum { IDD=IDD_dlgdutmsg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	// list��Ŀ����
	int m_iitemcounter;
	// ��ǰ�����  
	int m_nItem;
	// ��ǰ�������  
	int m_nSubItem;     
	// Edit�Ƿ�ɼ�  
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
