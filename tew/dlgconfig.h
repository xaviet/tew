#pragma once
#include "afxwin.h"
#include "config.h"

// cdlgconfig �Ի���

class cdlgconfig : public CDialogEx
{
	DECLARE_DYNAMIC(cdlgconfig)

public:
	cdlgconfig(CWnd* pParent=NULL);   // ��׼���캯��
	virtual ~cdlgconfig();

// �Ի�������
	enum { IDD=IDD_dlgconfig };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ѭ��ִ��
	CButton m_checkloop;
	virtual BOOL OnInitDialog();
	// app dialog point
	CDialogEx *m_pappdlg;
	afx_msg void OnBnClickedOk();
	// tewdlg���ñ�־
	struct tewdlgconfigflag *m_pconfigflag;
	// ng stop flag
	CButton m_checkngstop;
	CMFCEditBrowseCtrl m_zlogdir;
};
