#pragma once

#include "tewDlg.h"
#include "afxsock.h"

// ctcpserver

class ctcpserver : public CWinThread
{
	DECLARE_DYNCREATE(ctcpserver)

protected:
	ctcpserver();           // 动态创建所使用的受保护的构造函数
	virtual ~ctcpserver();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

private :
	// 维护用tcp server
	CSocket m_tcpserver;
	// server socket
	CSocket m_serversocket;
	// 终端receive
	int tcpserverrx();
	// 终端transmit
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


