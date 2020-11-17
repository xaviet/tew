#pragma once

#include "tewDlg.h"
#include "afxsock.h"

// cudp

class cudp : public CWinThread
{
	DECLARE_DYNCREATE(cudp)

protected:
	cudp();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	// ���ذ汾
	struct tewversion m_dlocalver;
	// the newest ver
	struct tewversion m_dnewestver;

private:
	// ���汾��Ϣ��ʾ��״̬��
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
	// ���udp����
	int rxbufferok();
	// udp data
	struct tewudp m_tewudp;
	// sendto
	int udpsendto();
	// recvfrom();
	int udprecvfrom();
	// ��ʾ��־
	int m_notifyflag;
};


