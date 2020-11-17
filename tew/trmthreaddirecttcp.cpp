// trmthreaddirecttcp.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "trmthreaddirecttcp.h"


// trmthreaddirecttcp

IMPLEMENT_DYNCREATE(ctrmthreaddirecttcp, CWinThread)

ctrmthreaddirecttcp::ctrmthreaddirecttcp()
	:m_ptrmdlg(0)
	,m_pitemdata(0)
	,m_ptrmmode(0)
	,m_modecomm(0)
	,m_lineno(0)
	,m_timer(0)
	,m_rxtime(0)
	,m_lasttxtime(0)
	,m_waittime(0)
	,m_cmdrxflag(0)
	,m_ngflag(0)
	,m_matchkeyvalue(0)
	,m_mode(0)
{
	m_rxpacket.len=0;
	m_txpacket.len=0;
	m_commandfunctionarray[0].f=NULL;
}

ctrmthreaddirecttcp::~ctrmthreaddirecttcp()
{
}

BOOL ctrmthreaddirecttcp::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	// init normal
	if(trmmodecomminit(m_modecomm)!=FALSE)
	{
		// init错误,cleat quit
		//  app dlg log list add
		m_pitemdata->information=m_pitemdata->comment+_T(" init fail");
		m_ptrmdlg->SendMessage(WM_trmdlgctrladdloglist,(WPARAM)m_pitemdata->information.GetBuffer(0));
		m_pitemdata->information.ReleaseBuffer();
		// fault+1
		trmfaultcounter();
		m_pitemdata=NULL;
		m_ptrmmode=NULL;
		m_ptrmdlg->SendMessage(WM_CLOSE,1,1);
		m_ptrmdlg=NULL;
		return FALSE;
	}
	else
	{
		// clear line number
		m_pitemdata->lineno=0;
		// command init
		trminitcommandfunctionarray();
		// 发送消息入队,运行开始
		this->PostThreadMessage(WM_trmdirecttcprun,0,0);
		this->PostThreadMessage(WM_trmdirecttcptimer,0,0);
		this->PostThreadMessage(WM_trmdirecttcptx,0,0);
		this->PostThreadMessage(WM_trmdirecttcprx,0,0);
		m_pitemdata->nginformation.Empty();
		//trmsendtrmdlgcstring(_T("direct tcp init completed"));
	}
	return TRUE;
}

int ctrmthreaddirecttcp::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(ctrmthreaddirecttcp, CWinThread)
END_MESSAGE_MAP()

// ctrmthreaddirecttcp 消息处理程序

// init command function array
// 命令原语
//commentline:
//	[comment]tcprx:tcp rx;[1]=key value;[2]=target value;[3]=mask;[4]=start position;[5]==peroid;[6]=save value;
//	[comment]tcptx:tcp tx;[1]=tx value;
//	[comment]wait:wait second;[1]=seconds;[2]=message;
//	[comment]rxbuffclear:rx buff clear;
//	[comment]msg:terminal message;[1]=cmd(add/check/chkclr/clear);[2]=message;
//	[comment]module:[1]=module name;[2]=flag[begin,end];
//	[comment]sleep:sleep second;[1]=seconds;[2]=message;
int ctrmthreaddirecttcp::trminitcommandfunctionarray()
{
	// 注释行分解参数数组初始化
	for(int t_i=0;t_i<DIRECTTCPCMD;t_i++)
	{
		m_para[t_i].Empty();
	}
	m_commandfunctionarray[0].primitives=_T("tcprx");
	m_commandfunctionarray[0].f=&ctrmthreaddirecttcp::primitivestcprx;
	m_commandfunctionarray[1].primitives=_T("tcptx");
	m_commandfunctionarray[1].f=&ctrmthreaddirecttcp::primitivestcptx;
	m_commandfunctionarray[2].primitives=_T("wait");
	m_commandfunctionarray[2].f=&ctrmthreaddirecttcp::primitiveswait;
	m_commandfunctionarray[3].primitives=_T("rxbuffclear");
	m_commandfunctionarray[3].f=&ctrmthreaddirecttcp::primitivesrxbuffclear;
	m_commandfunctionarray[4].primitives=_T("msg");
	m_commandfunctionarray[4].f=&ctrmthreaddirecttcp::primitivesrxmsg;
	m_commandfunctionarray[5].primitives=_T("module");
	m_commandfunctionarray[5].f=&ctrmthreaddirecttcp::primitivesmodule;
	m_commandfunctionarray[6].primitives=_T("sleep");
	m_commandfunctionarray[6].f=&ctrmthreaddirecttcp::primitivessleep;
	return 0;
}

// 分析注释行命令
int ctrmthreaddirecttcp::trmparsecomentlinecommand()
{
	int t_i;
	int t_return=FALSE;
	for(t_i=0;t_i<DIRECTTCPCMD;t_i++)
	{
		if(m_para[0].CompareNoCase(m_commandfunctionarray[t_i].primitives)==0)
		{
			// 调用函数成员
			t_return=((this)->*(m_commandfunctionarray[t_i].f))();
			break;
		}
		else if(t_i==(DIRECTTCPCMD-1))
		{
			// 未能解释的命令,skip
			t_return=TRUE;
		}
	}
	return t_return;
}

//	[comment]tcprx:tcp rx;[1]=key value;[2]=target value;[3]=mask;[4]=start position;[5]=peroid;[6]=save value;
int ctrmthreaddirecttcp::primitivestcprx()
{
	m_matchkeyvalue=0;
	m_cmdrxflag=1;
	m_rxtime=m_timer;
	return FALSE;
}

//	[comment]tcptx:tcp tx;[1]=tx value;
int ctrmthreaddirecttcp::primitivestcptx()
{
	m_txpacket.len=m_para[1].GetLength();
	m_txpacket.len=trmunicodehex2char(m_para[1],m_txpacket.data);
	return FALSE;
}

//	[comment]wait:wait second;[1]=seconds;[2]=message;
int ctrmthreaddirecttcp::primitiveswait()
{
	m_waittime=m_timer;
	//trmlogmsg(_T("begin"));
	this->PostThreadMessage(WM_trmdirecttcpwait,0,0);
	return FALSE;
}

//	[comment]sleep:sleep second;[1]=seconds;[2]=message;
int ctrmthreaddirecttcp::primitivessleep()
{
	int t_sleep;
	CString t_cs;
	t_sleep=_ttoi(m_para[1])*1000;
	t_cs=_T("\r\nsleep ")+m_para[1]+_T("s,do ")+m_para[2]+_T("\r\n");
	trmrecoder(t_cs);
	Sleep(t_sleep);
	this->PostThreadMessage(WM_trmdirecttcprun,0,0);
	return FALSE;
}

//	[comment]rxbuffclear:rx buff clear;
int ctrmthreaddirecttcp::primitivesrxbuffclear()
{
	trmclearsocketbuff();
	this->PostThreadMessage(WM_trmdirecttcprun,0,0);
	trmlogmsg(_T("OK"));
	return FALSE;
}

//	[comment]msg:terminal message;[1]=cmd(add/check/chkclr/clear);[2]=message;
int ctrmthreaddirecttcp::primitivesrxmsg()
{
	m_mode=0;
	if(m_para[1]==_T("add"))
	{
		m_mode=1;
	}
	else if(m_para[1]==_T("check"))
	{
		m_mode=2;
	}
	else if(m_para[1]==_T("chkclr"))
	{
		m_mode=3;
	}
	else if(m_para[1]==_T("clear"))
	{
		m_mode=4;
	}
	m_pitemdata->msgoperate=FALSE;
	m_pitemdata->information=_T("[")+m_para[2]+_T("];");
	this->PostThreadMessage(WM_trmdirecttcpmsg,0,0);
	CString t_cs=m_para[1]+_T(":")+m_pitemdata->information;
	trmrecoder(t_cs);
	return FALSE;
}

//	[comment]module:[1]=module name;[2]=flag[begin,end];
int ctrmthreaddirecttcp::primitivesmodule()
{
	if(m_para[1].GetLength()>0)
	{
		m_pitemdata->information=m_para[1];
		int t_i=0;
		if(m_para[2].CompareNoCase(_T("begin"))==0)
		{
			// 模块开始
			t_i=0;
		}
		else if(m_para[2].CompareNoCase(_T("end"))==0)
		{
			//模块结束
			t_i=1;
		}
		m_ptrmdlg->SendMessage(WM_trmdlgmodule,(WPARAM)m_pitemdata,t_i);
	}
	else
	{
		assert(m_para[1].GetLength()>0);
	}
	return FALSE;
}

// unicode hex to char;v_str char[65535]
int ctrmthreaddirecttcp::trmunicodehex2char(CString v_cs,char* v_str)
{
	int t_len=0;
	if(((v_cs.GetLength())%2)==1)
	{
		v_cs=_T("0")+v_cs;
	}
	t_len=v_cs.GetLength();
	WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)v_cs,-1,v_str,t_len,0,false); 
	for(int v_i=0;v_i<t_len;v_i++)
	{
		if((*(v_str+v_i)>='a')&&(*(v_str+v_i)<='f'))
		{
			*(v_str+v_i)=*(v_str+v_i)-'a'+10;
		}
		else if((*(v_str+v_i)>='0')&&(*(v_str+v_i)<='9'))
		{
			*(v_str+v_i)=*(v_str+v_i)-'0';
		}
		else
		{
			assert(0);
		}
	}
	for(int v_i=0;v_i<(t_len/2);v_i++)
	{
		*(v_str+v_i)=((*(v_str+2*v_i))<<4)+*(v_str+2*v_i+1);
	}
	t_len/=2;
	return t_len;
}

// char to unicode hex,v_str char[65535]
CString ctrmthreaddirecttcp::trmchar2unicodehex(char* v_str,int v_strlen)
{
	CString t_cs;
	int t_len=v_strlen;
	char t_ch[BUFFERSIZE*2]={0};
	for(int v_i=0;v_i<t_len;v_i++)
	{
		unsigned char t_ch1=0;
		unsigned char t_ch2=0;
		t_ch1=*(v_str+v_i);
		t_ch2=t_ch1>>4;
		*(t_ch+2*v_i)=t_ch2;
		t_ch2=t_ch1&0x0f;
		*(t_ch+2*v_i+1)=t_ch2;;
	}
	t_len*=2;
	for(int v_i=0;v_i<t_len;v_i++)
	{
		if((*(t_ch+v_i)>=0xa)&&(*(t_ch+v_i)<=0xf))
		{
			*(t_ch+v_i)=*(t_ch+v_i)-0xa+'a';
		}
		else if((*(t_ch+v_i)>=0x0)&&(*(t_ch+v_i)<=0x9))
		{
			*(t_ch+v_i)=*(t_ch+v_i)+'0';
		}
	}	
	t_cs.Empty();
	t_cs=CString(t_ch,t_len);
	return t_cs;
}

//	get tewDlg timer
int ctrmthreaddirecttcp::trmgettewdlgtimer()
{
	m_ptrmdlg->SendMessage(WM_trmdlgovertime,(WPARAM)m_pitemdata,NULL);
	m_timer=m_pitemdata->overtime;
	this->PostThreadMessage(WM_trmdirecttcptimer,0,0);
	return TRUE;
}

// rxbuff ok
int ctrmthreaddirecttcp::trmrxbufferok()
{
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
	t_readfds.fd_array[0]=m_modeip.m_hSocket;  //SOCKET句柄
	t_iRet=select(t_ifds,&t_readfds,t_pwritefds,t_pexceptfds,&t_timeout);
	return t_iRet;
}

// init log file
int ctrmthreaddirecttcp::trmlogfileinit()
{
	// trm log file handle
	if(m_csflogfile.Open(m_pitemdata->logpath+m_pitemdata->filename+TRM_LOG_EXT,CFile::modeWrite|CFile::modeCreate|CFile::shareDenyNone|CFile::modeNoTruncate))
	{
		m_csflogfile.SeekToEnd();
		CString t_csmsg;
		t_csmsg.Format(_T("%d"),m_pitemdata->port);
		t_csmsg=_T("\n[")+m_pitemdata->host+_T(":")+t_csmsg+_T("]\n");
		m_csflogfile.WriteString(t_csmsg);
	}
	else
	{
		assert(0);
		return TRUE;
	}
	return FALSE;
}

// init script file
int ctrmthreaddirecttcp::trmscriptfileinit()
{
	// script file handle
	if(m_csfscriptfile.Open(m_pitemdata->path+m_pitemdata->filename,CFile::modeRead|CFile::shareDenyWrite))
	{
		m_csfscriptfile.SeekToBegin();
	}
	else
	{
		assert(0);
		return TRUE;
	}  
	return FALSE;
}

// init socket
int ctrmthreaddirecttcp::trmsocketinit()
{
	CString t_cs;
	// 初始化socket
	if( AfxSocketInit()==FALSE)
	{ 
		assert(0);
	}
	// 初始化 CSocket 对象, 因为客户端不需要绑定任何端口和地址, 所以用默认参数即可
	if(!m_modeip.Create())
	{
		t_cs.Format(_T("CSocket create faild: %d"),m_modeip.GetLastError());
		trmsendtrmdlgcstring(t_cs);
		return TRUE;
	}
	// 连接指定的地址和端口
	if(!m_modeip.Connect(m_pitemdata->host,m_pitemdata->port))
	{
		m_modeip.Close();
		t_cs=m_pitemdata->host+_T(" Connect FAIL!");
		trmsendtrmdlgcstring(t_cs);
		return TRUE;
	}
	return FALSE;
}

// mode_comm初始化
int ctrmthreaddirecttcp::trmmodecomminit(int v_mode)
{
	int t_rt=FALSE;
	CString t_cs;
	t_cs.Empty();
	//使用信号量进行PV操作
	//WaitForSingleObject(m_ptewdlg->m_hsemaphore,INFINITE);
	// init log file
	t_rt+=trmlogfileinit();
	switch(v_mode)
	{
	case mode_directtcp:
		t_rt+=trmsocketinit();
		break;
	default:
		break;
	} 
	// 将错误写入nginformation
	if(t_cs.GetLength()>0)
	{
		trmsetnginformation(t_cs);
	}
	t_rt+=trmscriptfileinit();
	//使用信号量进行PV操作
	//ReleaseSemaphore(m_ptewdlg->m_hsemaphore,1,NULL);
	return t_rt;
}

//  mode_comm结束
int ctrmthreaddirecttcp::trmmodecommexit(int v_mode)
{
	switch(v_mode)
	{
	case mode_directtcp:
		trmclearsocketbuff();
		m_modeip.Close();
		// 网络质量不好，还需要延长
		Sleep(TRMTHREADENDDELAY);
		break;
	default:
		break;
	}
	m_csfscriptfile.Close();
	m_csflogfile.Close();
	return FALSE;
}

// tcprx
int ctrmthreaddirecttcp::trmtcprx(void* v_recvalue,int v_buffsize)
{
	int t_rt=0;
	if(trmrxbufferok()==1)
	{
		t_rt=m_modeip.Receive((void *)v_recvalue,BUFFERSIZE);
		if(t_rt==-1)
		{
			// socket 通信异常
			CString t_cs;
			t_cs.Format(_T("%s: %d"),_T("socket rx failed"),::GetLastError());
			trmsendtrmdlgcstring(t_cs);
			this->PostThreadMessage(WM_QUIT,0,0);
			trmfaultcounter();
		}
	}
	return t_rt;
}

// tcptx
int ctrmthreaddirecttcp::trmtcptx(void* v_recvalue,DWORD v_buffsize)
{
	m_modeip.Send(v_recvalue,v_buffsize);
	m_txpacket.len=0;
	m_lasttxtime=m_timer;
	return TRUE;
}

// clear socket buff
int ctrmthreaddirecttcp::trmclearsocketbuff()
{
	// 判断m_modeip的rxbuff是否有内容,清空后再关闭。否则close会发RST，而不是FIN
	char t_recValue[BUFFERSIZE]={0};
	trmtcprx(t_recValue,BUFFERSIZE);
	return TRUE;
}

// send cstring to trmdlg edit ctrl
int ctrmthreaddirecttcp::trmsendtrmdlgcstring(CString v_str)
{
	if(v_str.GetLength()>0)
	{
		CString t_cs=v_str;
		t_cs=_T("[rx ")+getsystemdatetime(0)+_T("]")+t_cs+_T("111\r\n");
		trmrecoder(t_cs);
	}
	return FALSE;
}

// 出现错误
int ctrmthreaddirecttcp::trmfaultcounter()
{
	CString t_nginfo;
	t_nginfo.Format(_T("%d"),m_lineno);
	t_nginfo=_T("[line:")+t_nginfo+_T("NG]")+getsystemdatetime(0)+_T(" comment[")+m_linestring+_T("]");
	trmsetnginformation(t_nginfo);
	m_ptrmdlg->SendMessage(WM_trmdlgtrmfaultcounter,(WPARAM)m_pitemdata,NULL);
	return 0;
}

// read and parse script file one line
//	TRUE good
//	FALSE end
int ctrmthreaddirecttcp::trmreadscriptfileline()
{
	if(m_csfscriptfile.ReadString(m_linestring)!=NULL)
	{
		m_linestring.MakeLower();
		m_lineno+=1;
		m_pitemdata->lineno=m_lineno;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//	parse script file one line
int ctrmthreaddirecttcp::trmparsescriptfileline()
{
	trmparsecomentline(m_linestring);
	return trmparsecomentlinecommand();
}

// 分析注释行
//	m_para[0]命令
//	m_para[1~9]参数
int ctrmthreaddirecttcp::trmparsecomentline(CString v_linestring)
{
	int t_i;
	m_para[0]=v_linestring;
	if(m_para[0].Right(1)!=CONFIGDELIM)
	{
		m_para[0]+=CONFIGDELIM;
	}
	for(t_i=0;((m_para[t_i].GetLength()!=0)&&(m_para[t_i].Find(CONFIGDELIM)!=-1)&&(t_i<(COMMENTPARSEPARA-1)));t_i++)
	{
		m_para[t_i+1]=m_para[t_i].Right(m_para[t_i].GetLength()-1-m_para[t_i].Find(CONFIGDELIM));
		m_para[t_i]=m_para[t_i].Left(m_para[t_i].Find(CONFIGDELIM));
	}
	m_para[0]=m_para[0].Right(m_para[0].GetLength()-m_ptrmmode->comment.GetLength()-m_para[0].Find(m_ptrmmode->comment));
	return FALSE;
}

//	loop
int ctrmthreaddirecttcp::ontrmdirecttcprun()
{
	// read and parse script file one line
	if(trmreadscriptfileline())
	{
		if(trmparsescriptfileline()==TRUE)
		{
			trmlogmsg(_T("UNKNOW"));
			this->PostThreadMessage(WM_trmdirecttcprun,0,0);
		}
	}
	else
	{
		// 脚本文件运行结束
		this->PostThreadMessage(WM_QUIT,0,0);
	}
	return TRUE;
}

//  tx packet
int ctrmthreaddirecttcp::trmtxpacket()
{
	if(m_lasttxtime==0)
	{
		m_lasttxtime=m_timer;
	}
	else
	{
		if(m_txpacket.len!=0)
		{
			if((m_timer-m_lasttxtime)>=(m_ptrmmode->delaytime/1000))
			{
				trmtcptx(m_txpacket.data,m_txpacket.len);
				this->PostThreadMessage(WM_trmdirecttcprun,0,0);
				trmlogmsg(_T("OK"));
			}
		}
		else if((m_timer-m_lasttxtime)>(m_ptrmmode->heartintv/1000))
		{
			m_txpacket.len=m_ptrmmode->heartmsg.GetLength();
			m_txpacket.len=trmunicodehex2char(m_ptrmmode->heartmsg,m_txpacket.data);
			trmtcptx(m_txpacket.data,m_txpacket.len);
		}
	}
	this->PostThreadMessage(WM_trmdirecttcptx,0,0);
	return TRUE;
}

// cstring to int
int ctrmthreaddirecttcp::cstring2int(CString v_cs)
{
	if(v_cs.GetLength()>0)
	{
		return _ttoi(v_cs);
	}
	return 0;
}

// rx buffer parse
// return: true		NG
// return: false	OK
int ctrmthreaddirecttcp::trmrxbuffparse(char* v_ch,int v_len)
{
	//	[comment]tcprx:tcp rx;[1]=key value;[2]=target value;[3]=mask;[4]=start position;[5]==peroid;[6]=save value;
	tewchars t_keyvalue;
	tewchars t_targetvalue;
	tewchars t_mask;
	int t_rt=FALSE;
	char *t_pch=v_ch;
	int t_len=v_len;
	char *t_ch=NULL;
	t_keyvalue.len=trmunicodehex2char(m_para[1],t_keyvalue.data);
	t_targetvalue.len=trmunicodehex2char(m_para[2],t_targetvalue.data);
	t_mask.len=trmunicodehex2char(m_para[3],t_mask.data);
	assert(t_targetvalue.len==t_mask.len);
	do
	{
		t_ch=charschars(t_pch,t_len,t_keyvalue.data,t_keyvalue.len);
		if(t_ch!=0)
		{
			int t_i=cstring2int(m_para[4]);
			t_len-=(t_ch-t_pch)-t_i;
			t_pch=t_ch+t_i+t_keyvalue.len;
			m_matchkeyvalue++;
			t_rt=trmtargetvaluejudge(t_pch,t_len,t_targetvalue.data,t_targetvalue.len,t_mask.data);
		}
	}while((t_ch!=NULL)&&(t_rt!=TRUE));
	return t_rt;
}

// target value judge
//	return:TRUE		NG
//	return:FALSE	OK
int ctrmthreaddirecttcp::trmtargetvaluejudge(char* v_rxdata,int v_rxlen,char* v_targetvalue,int v_targetvaluelen,char* v_mask)
{
	assert(v_rxlen>=v_targetvaluelen);
	for(int t_i=0;t_i<v_targetvaluelen;t_i++)
	{
		if(((*(v_rxdata+t_i))&(*(v_mask+t_i)))!=((*(v_targetvalue+t_i))&(*(v_mask+t_i))))
		{
			return TRUE;
		}
	}
	return FALSE;
}

// chars in chars
char* ctrmthreaddirecttcp::charschars(char* v_rxdata,int v_rxlen,char* v_keyvalue,int v_keyvaluelen)
{
	// no enough length
	if(v_rxlen<v_keyvaluelen)
	{
		return NULL;
	}
	for(int i=0;i<=(v_rxlen-v_keyvaluelen);i++)
	{
		if((*(v_rxdata+i))==(*v_keyvalue))
		{
			for(int j=1;j<v_keyvaluelen;j++)
			{
				if((*(v_rxdata+i+j))!=(*(v_keyvalue+j)))
				{
					break;
				}
				if(j==(v_keyvaluelen-1))
				{
					return v_rxdata+i;
				}
			}
		}
	}
	return NULL;
}

// trm set nginformation
int ctrmthreaddirecttcp::trmsetnginformation(CString v_cs)
{
	m_pitemdata->nginformation+=v_cs+_T("\n");
	return 0;
}

//  rx packet
int ctrmthreaddirecttcp::trmrxpacket()
{
	if(m_cmdrxflag==1)
	{
		int t_rxlen=0;
		char t_chrx[BUFFERSIZE]={0};
		CString t_cs;
		// rx command 
		m_ngflag=FALSE;
		t_rxlen=trmtcprx(t_chrx,BUFFERSIZE);
		if(t_rxlen!=0)
		{
			t_cs=trmchar2unicodehex(t_chrx,t_rxlen);
			m_ngflag=m_ngflag||trmrxbuffparse(t_chrx,t_rxlen);
			if(m_ngflag==TRUE)
			{
				trmsendtrmdlgcstring(t_cs);
			}
		}
		trmrxjudge();
	}
	else
	{
		trmclearsocketbuff();
	}
	this->PostThreadMessage(WM_trmdirecttcprx,0,0);
	return TRUE;
}

//	rx judge
int ctrmthreaddirecttcp::trmrxjudge()
{
	if(((m_timer-m_rxtime)>=cstring2int(m_para[5]))||(m_ngflag==TRUE))
	{
		// key value match==0:NG
		m_ngflag=m_ngflag||((m_matchkeyvalue==0)?TRUE:FALSE);
		m_cmdrxflag=0;
		this->PostThreadMessage(WM_trmdirecttcprun,0,0);
		CString t_cs;
		t_cs=m_ngflag?_T("NG"):_T("OK");
		trmlogmsg(t_cs);
		if(m_para[6].GetLength()>0)
		{
			trmsavevalue(t_cs);
		}
	}
	if(m_ngflag==TRUE)
	{
		trmfaultcounter();
	}
	return TRUE;
}

//	save value
int ctrmthreaddirecttcp::trmsavevalue(CString v_cs)
{
	int t_i=cstring2int(m_para[6]);
	m_pitemdata->information=v_cs;
	if(t_i<SAVEVALUECOUNTER)
	{
		m_ptrmdlg->SendMessage(WM_trmdlgsetsavevalue,(WPARAM)m_pitemdata,t_i);
	}
	else
	{
		assert(0);
	}
	return TRUE;
}

//  wait
int ctrmthreaddirecttcp::trmwait()
{
	if((m_timer-m_waittime)>=_ttoi(m_para[1]))
	{
		this->PostThreadMessage(WM_trmdirecttcprun,0,0);
		trmlogmsg(_T("OK"));
	}
	else
	{
		this->PostThreadMessage(WM_trmdirecttcpwait,0,0);
	}
	return TRUE;
}

// 获取时间 v_type:0-format;1-dir used
CString  ctrmthreaddirecttcp::getsystemdatetime(int v_type)
{
	CString t_cstime;
	SYSTEMTIME	t_sysTime; 
	GetLocalTime(&t_sysTime);
	if(v_type==0)
	{
		t_cstime.Format( _T("%04ld-%02ld-%02ld %02ld:%02ld:%02ld.%03ld"),t_sysTime.wYear,t_sysTime.wMonth,t_sysTime.wDay,t_sysTime.wHour,t_sysTime.wMinute,t_sysTime.wSecond,t_sysTime.wMilliseconds);
	}
	else if(v_type==1)
	{
	t_cstime.Format( _T("%04ld%02ld%02ld%02ld%02ld%02ld%03ld"),t_sysTime.wYear,t_sysTime.wMonth,t_sysTime.wDay,t_sysTime.wHour,t_sysTime.wMinute,t_sysTime.wSecond,t_sysTime.wMilliseconds);
	}
	else
	{
		assert(0);
	}
	return t_cstime;
}

// log msg
int ctrmthreaddirecttcp::trmlogmsg(CString v_csreault)
{
	CString t_csedit;
	t_csedit.Format(_T("%d"),m_lineno);
	t_csedit=_T("\r\n[line:")+t_csedit+v_csreault+_T("]")+getsystemdatetime(0)+_T(" comment[")+m_linestring+_T("]\r\n");
	trmrecoder(t_csedit);
	return TRUE;
}

// recoder
int ctrmthreaddirecttcp::trmrecoder(CString v_cs)
{
	m_csflogfile.WriteString(v_cs);
	m_csflogfile.Flush();
	m_ptrmdlg->SendMessage(WM_trmdlgeditaddcstring,(WPARAM)v_cs.GetBuffer(0));
	v_cs.ReleaseBuffer();
	return TRUE;
}

//	trm thread exit
int ctrmthreaddirecttcp::trmthreadexit()
{
	// 线程结束,清理,通知trmdlg
	trmmodecommexit(m_modecomm);
	m_pitemdata=NULL;
	m_ptrmmode=NULL;
	m_ptrmdlg->PostMessage(WM_CLOSE,1,0);
	m_ptrmdlg=NULL;
	return TRUE;
}

//	trm msg process
int ctrmthreaddirecttcp::trmmsg()
{
	m_ptrmdlg->SendMessage(WM_trmdlgmsg,(WPARAM)m_pitemdata,m_mode);
	if(m_pitemdata->msgoperate==TRUE)
	{
		this->PostThreadMessage(WM_trmdirecttcprun,0,0);
	}
	else
	{
		this->PostThreadMessage(WM_trmdirecttcpmsg,0,0);
	}	
	return TRUE;
}

int ctrmthreaddirecttcp::Run()
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	//	BOOL t_bRet; 
	MSG t_msg;
	while((/*t_bRet=*/GetMessage(&t_msg,NULL,0,0))!=0)
	{
		switch(t_msg.message)
		{
		case WM_trmdirecttcprun:
			ontrmdirecttcprun();
			break;
		case WM_trmdirecttcptimer:
			trmgettewdlgtimer();
			break;
		case WM_trmdirecttcptx:
			trmtxpacket();
			break;
		case WM_trmdirecttcprx:
			trmrxpacket();
			break;
		case WM_trmdirecttcpwait:
			trmwait();
			break;
		case WM_trmdirecttcpmsg:
			trmmsg();
			break;
		default:
			break;
		}
	}
	trmthreadexit();
	return CWinThread::Run();
}

