#pragma once
#include "afxwin.h"
#include "config.h"

// cdlgprocessing �Ի���

class cdlgprocessing : public CDialogEx
{
	DECLARE_DYNAMIC(cdlgprocessing)

public:
	cdlgprocessing(CWnd* pParent=NULL);   // ��׼���캯��
	virtual ~cdlgprocessing();

// �Ի�������
	enum { IDD=IDD_dlgprocessing };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
