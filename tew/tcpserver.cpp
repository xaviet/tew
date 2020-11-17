// tcpserver.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "tcpserver.h"


// ctcpserver

IMPLEMENT_DYNCREATE(ctcpserver, CWinThread)

ctcpserver::ctcpserver()
	: m_ptewdlg(0)
	, m_tcpcloseflag(0)
	, m_tcpconnectflag(0)
{
}

ctcpserver::~ctcpserver()
{
}

BOOL ctcpserver::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	// 初始化socket
	if( AfxSocketInit()==FALSE)
	{ 
		AfxMessageBox(_T("Unable to AfxSocketInit()")); 
		return FALSE; 
	}
	if (!m_tcpserver.Socket())
	{
		AfxMessageBox(_T("Unable to Socket()"));
		return FALSE; 
	}
	if (!m_tcpserver.Bind(MNTPORT))
    {     
        AfxMessageBox(_T("Unable to Bind that port, please try another port"));     
        return FALSE; 
    }
	if(m_tcpserver.Listen()==FALSE) 
	{ 
		AfxMessageBox(_T("Unable to Listen on that port,please try another port")); 
		m_tcpserver.Close(); 
		return FALSE; 
	}
	// 发送消息入队,运行开始
	this->PostThreadMessage(WM_tcpserveraccept,0,0);
	return TRUE;
}

int ctcpserver::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

// 终端receive
int ctcpserver::tcpserverrx()
{
	// rxbuff ok
	// select 参数
	int t_iRet=0;
	int t_ifds=0;
	fd_set t_readfds;
	fd_set FAR *t_pwritefds=NULL;
	fd_set FAR *t_pexceptfds=NULL;
	struct timeval t_timeout;
	// 超时时间1毫秒
	t_timeout.tv_sec=0;
	t_timeout.tv_usec=1000;  
	t_readfds.fd_count=1;
	t_readfds.fd_array[0]=m_serversocket.m_hSocket;  //SOCKET句柄
	t_iRet=select(t_ifds,&t_readfds,t_pwritefds,t_pexceptfds,&t_timeout);
	if(t_iRet==1)
	{
		// rx
		char t_recValue[BUFFERSIZE]={0};
		m_serversocket.Receive((void *)t_recValue,BUFFERSIZE);
		//对于出现大于127的字符使用'?'替代,以避免stdiofile.readstring异常
		int t_i=0;
		for(;;)
		{
			if((t_recValue[t_i]==NULL)||(t_recValue[t_i]==BUFFERSIZE))
			{
				break;
			}
			else if((t_recValue[t_i]>127)||(t_recValue[t_i]<0))
			{
				t_recValue[t_i]='?';
			}
			t_i++;
		}
		// char[]转CString
		m_rxbuff+=CString(t_recValue);
		// process backspace
		for(;(t_i=m_rxbuff.Find(_T('\b')))!=-1;)
		{
			m_rxbuff=m_rxbuff.Left(t_i-1)+m_rxbuff.Right(m_rxbuff.GetLength()-t_i-1);
		}
	}
	return 0;
}

// 终端transmit
int ctcpserver::tcpservertx()
{
	char t_sndValue[BUFFERSIZE]={0};
	DWORD t_dwWritetcpserver;
	WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)m_txbuff,-1,t_sndValue,BUFFERSIZE,0,false); 
	t_dwWritetcpserver=strlen(t_sndValue);
	m_serversocket.Send(t_sndValue,t_dwWritetcpserver);
	return 0;
}

BEGIN_MESSAGE_MAP(ctcpserver, CWinThread)
END_MESSAGE_MAP()

// ctcpserver 消息处理程序

// WM_tcpserverrun
int ctcpserver::ontcpserveraccept()
{
	if(m_tcpserver.Accept(m_serversocket))
	{
		// send prompt
		m_txbuff=_T("tew ver:")+m_ver+_T("\r\n\r\n");
		// m_txbuff=_T("产品名称:武汉日电C-Node嵌入式通信管理软件V3.0\r\n版权所有@武汉日电光通行工业有限公司\r\n\r\n");
		m_txbuff+=TCPSERVERPROMPT;
		tcpservertx();
		// clear rx buff
		m_rxbuff.Empty();
		// set tcp connect flag
		m_tcpconnectflag=1;
		this->PostThreadMessage(WM_tcpserverconnect,0,0);
	}
	else
	{
		this->PostThreadMessage(WM_tcpserveraccept,0,0);
	}
	return 0;
}

// WM_tcpserverconnect
int ctcpserver::ontcpserverconnect()
{
	if(m_tcpcloseflag)
	{
		tcpserverwaiting();
		m_tcpcloseflag=0;
	}
	else
	{
		tcpserverrx();
		if(m_rxbuff.GetLength()>0)
		{
			if((m_rxbuff[m_rxbuff.GetLength()-1]==_T('\r'))||(m_rxbuff[m_rxbuff.GetLength()-1]==_T('\n')))
			{
				// get return or newline
				m_rxbuff=m_rxbuff.Left(m_rxbuff.GetLength()-1);
				if((m_rxbuff[m_rxbuff.GetLength()-1]==_T('\r'))||(m_rxbuff[m_rxbuff.GetLength()-1]==_T('\n')))
				{
					m_rxbuff=m_rxbuff.Left(m_rxbuff.GetLength()-1);
				}
				if((m_rxbuff==_T("?"))||(m_rxbuff==_T("help")))
				{
					//m_txbuff=_T("tew ver:")+m_ver+_T("\r\n\t?/help\t\t\thelp\r\n\tmsg\t\t\t显示终端消息内容\r\n\tcurtrm\t\t\t当前终端状态\r\n\tvalue\t\t\t保存变量内容\r\n\tbye/exit/quit\t\t结束\r\n");	
					m_txbuff=_T("ver: 3.0\r\n\t?/help\t\t\thelp\r\n\tmsg\t\t\t显示终端消息内容\r\n\tcurtrm\t\t\t当前终端状态\r\n\tvalue\t\t\t保存变量内容\r\n\tbye/exit/quit\t\t结束\r\n");	
				}
				else if(m_rxbuff==_T("msg"))
				{
					m_ptewdlg->SendMessage(WM_appdlgtcpservermsg,(WPARAM)&m_txbuff,1);
				}
				else if(m_rxbuff==_T("curtrm"))
				{
					m_ptewdlg->SendMessage(WM_appdlgtcpservermsg,(WPARAM)&m_txbuff,2);
				}
				else if(m_rxbuff==_T("value"))
				{
					m_ptewdlg->SendMessage(WM_appdlgtcpservermsg,(WPARAM)&m_txbuff,3);
				}
				else if((m_rxbuff==_T("bye"))||(m_rxbuff==_T("exit"))||(m_rxbuff==_T("quit")))
				{
					tcpserverwaiting();
				}
				else
				{
					m_txbuff=_T("unknow command");
				}
				tcpservertx();	
				m_rxbuff.Empty();
				// send prompt
				m_txbuff=_T("\r\n");
				m_txbuff+=TCPSERVERPROMPT;
				tcpservertx();		
			}
		}
		// loop
		this->PostThreadMessage(WM_tcpserverconnect,0,0);
		// delay 0ms,交出cpu使用权
		Sleep(0);
	}
	return 0;
}

// waiting connect
int ctcpserver::tcpserverwaiting()
{
	m_txbuff=_T("bye!");
	// 关闭,进入accept
	m_serversocket.Close();
	m_tcpconnectflag=0;
	Sleep(1000);
	this->PostThreadMessage(WM_tcpserveraccept,0,0);
	return 0;
}

// tcp init port
int ctcpserver::ontcpserverinittcpport()
{
	if(m_tcpconnectflag)
	{
		m_tcpcloseflag=1;
	}
	return 0;
}

int ctcpserver::Run()
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	//	BOOL t_bRet; 
	MSG t_msg;
	while((GetMessage(&t_msg,NULL,0,0))!=0)
	{
		switch(t_msg.message)
		{
		case WM_tcpserveraccept:
			// WM_tcpserverrun
			ontcpserveraccept();
			break;
		case WM_tcpserverconnect:
			// WM_tcpserverconnect
			ontcpserverconnect();
			break;
		case WM_tcpserverinittcpport:
			// WM_tcpserverinittcpport
			ontcpserverinittcpport();
			break;
		default:
			break;
		}
/*		if(t_bRet==-1)
		{
			// handle the error and possibly exit  	
		}
		else
		{
			//TranslateMessage(&t_msg);
			//DispatchMessage(&t_msg);
		}*/
	}
	// 线程结束,清理
	m_tcpserver.Close();
	m_ptewdlg=NULL;
	return CWinThread::Run();
}
