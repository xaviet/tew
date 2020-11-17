// udp.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "udp.h"


// cudp

IMPLEMENT_DYNCREATE(cudp, CWinThread)

cudp::cudp()
  : m_notifyflag(0)
  , m_ptewdlg(0)
  , m_txudp(0)
  , m_rxudp(0)
{
  m_tewudp.udpdata.version.vmah = m_dnewestver.vmah = m_dlocalver.vmah = 0;
  m_txaddr = { 0 };
  m_rxaddr = { 0 };
}

cudp::~cudp()
{
}

BOOL cudp::InitInstance()
{
  // TODO: 在此执行任意逐线程初始化
  // get 版本信息
  m_dnewestver = m_dlocalver;
  // 将版本信息显示在状态栏
  settewdlgstatusbar();
  if (!initudp())
  {
    return FALSE;
  }
  // 初始化完成发出一个版本公告报文
  udpsendto();
  // 发送接收处理消息
  this->PostThreadMessage(WM_udprx, NULL, NULL);
  return TRUE;
}

// init udp
int cudp::initudp()
{
  if (!AfxSocketInit())
  {
    AfxMessageBox(_T("初始化套接字失败"));
    return FALSE;
  }
  int t_broad = TRUE;
  // 初始化tx udp
  m_txudp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (INVALID_SOCKET == m_txudp)
  {
    printf("\"tx socket\" error! error code is %d/n", WSAGetLastError());
    return FALSE;
  }
  setsockopt(m_txudp, SOL_SOCKET, SO_BROADCAST, (const char*)&t_broad, sizeof(t_broad));
  m_txaddr.sin_family = AF_INET;
  m_txaddr.sin_port = htons(UDPPORT);
  // 绑定用socket参数,绑定至host所在端口
  char t_name[255];
  gethostname(t_name, sizeof(t_name));
  PHOSTENT hostinfo = gethostbyname(t_name);
  m_txaddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*) * hostinfo->h_addr_list));
  if (SOCKET_ERROR == bind(m_txudp, (SOCKADDR*)&m_txaddr, sizeof(SOCKADDR)))
  {
    printf("\"tx bind\" error! error code is %d/n", WSAGetLastError());
    return FALSE;
  }
  // 收发用socket参数 
  m_txaddr.sin_addr.s_addr = INADDR_BROADCAST;
  // 初始化rx udp
  m_rxudp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (INVALID_SOCKET == m_rxudp)
  {
    printf("\"rx socket\" error! error code is %d/n", WSAGetLastError());
    return FALSE;
  }
  setsockopt(m_rxudp, SOL_SOCKET, SO_BROADCAST, (const char*)&t_broad, sizeof(t_broad));
  m_rxaddr.sin_family = AF_INET;
  m_rxaddr.sin_port = htons(UDPPORT);
  m_rxaddr.sin_addr.s_addr = 0;
  if (SOCKET_ERROR == bind(m_rxudp, (SOCKADDR*)&m_rxaddr, sizeof(SOCKADDR)))
  {
    printf("\"rx bind\" error! error code is %d/n", WSAGetLastError());
    return FALSE;
  }
  // 收发用socket参数 
  m_rxaddr.sin_addr.s_addr = INADDR_BROADCAST;
  return TRUE;
}

int cudp::ExitInstance()
{
  // TODO: 在此执行任意逐线程清理
  return CWinThread::ExitInstance();
}

// sendto
int cudp::udpsendto()
{
  struct tewudp t_tewudp;
  t_tewudp.udpdata.version = m_dnewestver;
  t_tewudp.check = UDPDATACHECK;
  t_tewudp.optcode = 0;
  for (int t_i = 0; t_i < 2; t_i++)
  {

    sendto(m_txudp, (char*)&t_tewudp, sizeof(t_tewudp), 0, (SOCKADDR*)&m_txaddr, sizeof(SOCKADDR));
    Sleep(100);
  }
  return TRUE;
}

// recvfrom();
int cudp::udprecvfrom()
{
  if (rxbufferok())
  {
    // udp data
    struct tewudp t_tewudp;
    int t_len = sizeof(SOCKADDR);
    recvfrom(m_rxudp, (char*)&t_tewudp, sizeof(t_tewudp), 0, (SOCKADDR*)&m_rxaddr, &t_len);
    if ((t_tewudp.check) == UDPDATACHECK)
    {
      if ((t_tewudp.udpdata.version.vmah > m_dnewestver.vmah) || \
        ((t_tewudp.udpdata.version.vmah == m_dnewestver.vmah) && (t_tewudp.udpdata.version.vmal > m_dnewestver.vmal)) || \
        ((t_tewudp.udpdata.version.vmah == m_dnewestver.vmah) && (t_tewudp.udpdata.version.vmal == m_dnewestver.vmal) && (t_tewudp.udpdata.version.vlah > m_dnewestver.vlah)) || \
        ((t_tewudp.udpdata.version.vmah == m_dnewestver.vmah) && (t_tewudp.udpdata.version.vmal == m_dnewestver.vmal) && (t_tewudp.udpdata.version.vlah == m_dnewestver.vlah) && (t_tewudp.udpdata.version.vlal > m_dnewestver.vlal)))
      {
        m_dnewestver = t_tewudp.udpdata.version;
        // 将版本信息显示在状态栏
        settewdlgstatusbar();
        if ((t_tewudp.optcode == 0xffffffff) && (m_notifyflag == 0))
        {
          AfxMessageBox(_T("存在关键版本，注意更新"));
          m_notifyflag = 1;
        }
      }
      else if ((t_tewudp.udpdata.version.vmah != m_dnewestver.vmah) || (t_tewudp.udpdata.version.vmal != m_dnewestver.vmal) || (t_tewudp.udpdata.version.vlah != m_dnewestver.vlah) || (t_tewudp.udpdata.version.vlal != m_dnewestver.vlal))
      {
        // 发送一次本地版本
        udpsendto();
      }
    }
  }
  else
  {
    // 如果rxbuff为空，休息3秒再接收
    Sleep(3000);
  }
  // 发送接收处理消息
  this->PostThreadMessage(WM_udprx, NULL, NULL);
  return 0;
}

// 检查udp接收
int cudp::rxbufferok()
{
  // select 参数
  int t_iRet = 0;
  int t_ifds = 0;
  fd_set t_readfds;
  fd_set FAR* t_pwritefds = NULL;
  fd_set FAR* t_pexceptfds = NULL;
  struct timeval t_timeout;
  // 超时时间1毫秒
  t_timeout.tv_sec = 0;
  t_timeout.tv_usec = 1000;
  t_readfds.fd_count = 1;
  t_readfds.fd_array[0] = m_rxudp;  //SOCKET句柄
  t_iRet = select(t_ifds, &t_readfds, t_pwritefds, t_pexceptfds, &t_timeout);
  return t_iRet;
}

// 将版本信息显示在状态栏
int cudp::settewdlgstatusbar()
{
  // 状态栏展示
  CString t_cs;
  t_cs.Format(_T("版本(当前%d.%d.%d.%d;最新%d.%d.%d.%d)"), m_dlocalver.vmah, m_dlocalver.vmal, m_dlocalver.vlah, m_dlocalver.vlal, m_dnewestver.vmah, m_dnewestver.vmal, m_dnewestver.vlah, m_dnewestver.vlal);
  m_ptewdlg->SendMessage(WM_appdlgctrlsetstatusbar, (WPARAM)t_cs.GetBuffer(0), 3);
  t_cs.ReleaseBuffer();
  return 0;
}

BEGIN_MESSAGE_MAP(cudp, CWinThread)
END_MESSAGE_MAP()


// cudp 消息处理程序


int cudp::Run()
{
  // TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
  // 线程结束,清理 
  MSG t_msg;
  while ((GetMessage(&t_msg, NULL, 0, 0)) != 0)
  {
    switch (t_msg.message)
    {
    case WM_udprx:
      udprecvfrom();
      break;
    default:
      break;
    }
  }
  m_ptewdlg = NULL;
  return CWinThread::Run();
}
