#pragma once

#include "tewDlg.h"
#include "afxsock.h"

// cudp

class cudp : public CWinThread
{
	DECLARE_DYNCREATE(cudp)

protected:
	cudp();           // 动态创建所使用的受保护的构造函数
	virtual ~cudp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	// tew dlg point
	CtewDlg *m_ptewdlg;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
	// 本地版本
	struct tewversion m_dlocalver;
	// the newest ver
	struct tewversion m_dnewestver;

private:
	// 将版本信息显示在状态栏
	int settewdlgstatusbar();
	// init udp
	int initudp();
	// udpsocket
	SOCKET m_txudp;
	// udpsocket
	SOCKET m_rxudp;
	// udp addr
  SOCKADDR_IN m_txaddr = { 0 };
	// udp addr
	SOCKADDR_IN m_rxaddr = { 0 };
	// 检查udp接收
	int rxbufferok();
	// udp data
	struct tewudp m_tewudp;
	// sendto
	int udpsendto();
	// recvfrom();
	int udprecvfrom();
	// 提示标志
	int m_notifyflag;
};


