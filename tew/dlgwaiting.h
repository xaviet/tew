#pragma once

#include "config.h"
#include "afxwin.h"

// cdlgwaiting �Ի���

class cdlgwaiting : public CDialogEx
{
	DECLARE_DYNAMIC(cdlgwaiting)

public:
	cdlgwaiting(CWnd* pParent=NULL);   // ��׼���캯��
	virtual ~cdlgwaiting();

// �Ի�������
	enum { IDD=IDD_dlgwaiting };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
