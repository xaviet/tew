#pragma once
#include "afxwin.h"
#include "config.h"

// cdlgsystemcmd �Ի���

class cdlgsystemcmd : public CDialogEx
{
	DECLARE_DYNAMIC(cdlgsystemcmd)

public:
	cdlgsystemcmd(CWnd* pParent=NULL);   // ��׼���캯��
	virtual ~cdlgsystemcmd();

// �Ի�������
	enum { IDD=IDD_dlgsystemcmd };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ����ϵͳ���������Ϣ
	CEdit m_editsyscmdmsg;
	// cmd and ����
	CString m_para[COMMENTPARSEPARA];
	// output message
	CString m_csoutputbuff;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// system cmd dlg cedit add cstring
	afx_msg LRESULT Ondlgsystemcmdeditaddcstring(WPARAM wParam, LPARAM lParam);
	// trm thread point
	int m_ptrmthread;
private:
	// �����ܵ�����
	int m_retutn;
	SECURITY_ATTRIBUTES m_sa;
	HANDLE m_hInReadPipe,m_hInWritePipe,m_hOutReadPipe,m_hOutWritePipe;
	STARTUPINFO m_si;
	PROCESS_INFORMATION m_ProcessInformation;
	unsigned long m_lBytesRead;
	// ��ʼ�������ܵ�
	int systemcmdinitpipe();
	// �����������
	int m_paramno;
	// time click
	int m_timer;
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
