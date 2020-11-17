#pragma once

#include "tewDlg.h"
#include "afxsock.h"

// ctcpserver

class ctcpserver : public CWinThread
{
	DECLARE_DYNCREATE(ctcpserver)

protected:
	ctcpserver();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~ctcpserver();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

private :
	// ά����tcp server
	CSocket m_tcpserver;
	// server socket
	CSocket m_serversocket;
	// �ն�receive
	int tcpserverrx();
	// �ն�transmit
	int tcpservertx();
	// rx buffer
	CString m_rxbuff;
	// tx buffer
	CString m_txbuff;
	// init tcp connect
	int m_tcpcloseflag;
	// waiting connect
	int tcpserverwaiting();
	// tcp init port
	int ontcpserverinittcpport();

public:
	// tew dlg point
	CtewDlg *m_ptewdlg;
	// WM_tcpserverrun
	int ontcpserveraccept();
	// WM_tcpserverconnect
	int ontcpserverconnect();
	// file version
	CString m_ver;
	// tcp connect flag
	int m_tcpconnectflag;
	virtual int Run();
};


