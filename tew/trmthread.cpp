// trmthread.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "trmthread.h"

// ctrmthread

IMPLEMENT_DYNCREATE(ctrmthread, CWinThread)

	ctrmthread::ctrmthread()
	: m_ptrmdlg(0)
	, m_pitemdata(0)
	, m_matchnormal(0)
	, m_matchstrict(0)
	, m_lineno(0)
	, m_ngtimes(0)
	, m_cmdbegintime(0)
	, m_hgpib(0)
	, m_ptrmmode(0)
	, m_modecomm(0)
	, m_modecom(0)
	, m_modegpib(0)
	, m_commentflag(0)
	, m_sshv2rc(0)
	, m_psession(NULL)
	, m_pchannel(NULL)
	, m_sshv2auth(AUTH_NONE)
	, m_sftp_session(0)
	, m_sftp_handle(0)
	, m_ngmon(1)
{
	m_commandfunctionarray[0].f=NULL;
}

ctrmthread::~ctrmthread()
{
}


BOOL ctrmthread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	assert(m_ptrmdlg);
	assert(m_pitemdata); 
	// 延迟
	Sleep(TRMDELAY);
	// clear nginformation
	m_pitemdata->nginformation.Empty();
	// trm 测试
	if(xavietex)
	{
		// For test
		//  随机种子
		srand((unsigned)(m_pitemdata->ptrdlg%10));
		int t_irand=10*(rand()%10+1);
		for(int t_i=0;t_i<t_irand;t_i++)
		{
			CString t_cs=_T("");
			t_cs.Format(m_pitemdata->comment+_T(": %d/%d")+RETURNNEWLINE,t_i,t_irand);
			trmsendtrmdlgcstring(t_cs);
			Sleep(100);
		}
	}
	else
	{
		// set value
/*		if(m_pitemdata->message.GetLength()>0)
		{
			CString t_message=m_pitemdata->message;
			CString t_cs;
			t_message+=_T(" ");
			for(;t_message.Find(_T("="))!=-1;)
			{
				t_message.TrimLeft();
				t_cs=t_message.Left(t_message.Find(_T(" ")));
				int t_i=_ttoi(t_cs.Left(t_cs.Find(_T("="))));
				t_message=t_message.Right(t_message.GetLength()-t_message.Find(_T(" "))-1);
				assert((t_i>=0)&&(t_i<SAVEVALUECOUNTER));
				if((t_i>=0)&&(t_i<SAVEVALUECOUNTER))
				{
					m_pitemdata->information=t_cs.Right(t_cs.GetLength()-t_cs.Find(_T("="))-1);
					m_ptrmdlg->SendMessage(WM_trmdlgsetsavevalue,(WPARAM)m_pitemdata,t_i);
				}
			}
		}
*/
		// init command function array
		trminitcommandfunctionarray();
		// init normal
		if(trmmodecomminit(m_modecomm)==TRUE)
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
			// 发送消息入队,运行开始
			//	initreturn and autologin
			this->PostThreadMessage(WM_trmthreadinitreturn,0,0);
		}
	}
	return TRUE;
}

// init command function array
// 命令原语
//commentline:
//	[comment][comment]:strict match;[1]=match string;
//	[comment]:normal match;[1]=match string;
//	[comment]wait:waiting secounds;[1]=dialog confim time(0:always/[1,):secounds);[2]=dialog string
//	[comment]cmp:include key string;[1]=match string;[2]=key string;[3]=start position;[4]=lengh;[5]=save value number[0,1023];
//	[comment]cmpn:no include key string;[1]=match string;[2]=key string;[3]=start position;[4]=lengh;[5]=save value number[0,1023];
//	[comment]save:save resualt;[1]=match string;[2]=save value number[0,1023];[3]=start position("":the first word);[4]=lengh;
//	[comment]saveconst:save resualt;[1]=const;[2]=save value number[0,1023];
//	[comment]count:count resualt;[1]=the first value[0,1023];[2]="<","="">";[3]=the second value[0,1023];[4]=save value number[0,1023];
//	[comment]countf:count resualt;[1]=the first value[0,1023];[2]="<","="">";[3]=the second value[0,1023];[4]=save value number[0,1023];
//	[comment]buffcut:cut buff;[1]=start position("":buff first buff byte);[2]=stop position("":last buff byte);
//	[comment]csv:save csv file;[1]=save value number[0,1023]OR newline label['n'(new line),''(no new line)];
//	[comment]ctrld:none;
//	[comment]module:[1]=module name;[2]=flag[begin,end];
//	[comment]sleep:sleep secounds;[1]=sleep confim time(0:always/[1,):secounds);[2]=sleep string
//	[comment]systemcmd:execute system command;[1]=cmd;[2~31]=input
//	[comment]msg:terminal message;[1]=cmd(add/check/chkclr/clear);[2]=message
//	[comment]ctrlstop:[1]=save value number[0,1023]
//	[comment]label:[1]=label name;
//	[comment]goto:[1]=label name;[2]=save value number[0,1023];[3]=match string;
//	[comment]goton:[1]=label name;[2]=save value number[0,1023];[3]=match string;
//	[comment]sftp:[1]=operate tx/rx;[2]=local path+file name;[3]=remote path+file name;
//	[comment]ngmon:[1]=on/off;
int ctrmthread::trminitcommandfunctionarray()
{
	// 注释行分解参数数组初始化
	for(int t_i=0;t_i<COMMENTPARSEPARA;t_i++)
	{
		m_para[t_i].Empty();
	}
	m_commandfunctionarray[0].primitives=m_ptrmmode->comment;
	m_commandfunctionarray[0].f=&ctrmthread::primitivesstrictmatch;
	m_commandfunctionarray[1].primitives=_T("");
	m_commandfunctionarray[1].f=&ctrmthread::primitivesnormalmatch;
	m_commandfunctionarray[2].primitives=_T("wait");
	m_commandfunctionarray[2].f=&ctrmthread::primitiveswaiting;
	m_commandfunctionarray[3].primitives=_T("cmp");
	m_commandfunctionarray[3].f=&ctrmthread::primitivescmp;
	m_commandfunctionarray[4].primitives=_T("cmpn");
	m_commandfunctionarray[4].f=&ctrmthread::primitivescmpn;
	m_commandfunctionarray[5].primitives=_T("save");
	m_commandfunctionarray[5].f=&ctrmthread::primitivessave;
	m_commandfunctionarray[6].primitives=_T("saveconst");
	m_commandfunctionarray[6].f=&ctrmthread::primitivessaveconst;
	m_commandfunctionarray[7].primitives=_T("count");
	m_commandfunctionarray[7].f=&ctrmthread::primitivescount;
	m_commandfunctionarray[8].primitives=_T("buffcut");
	m_commandfunctionarray[8].f=&ctrmthread::primitivesbuffcut;
	m_commandfunctionarray[9].primitives=_T("csv");
	m_commandfunctionarray[9].f=&ctrmthread::primitivescsv;
	m_commandfunctionarray[10].primitives=_T("ctrld");
	m_commandfunctionarray[10].f=&ctrmthread::primitivesctrld;
	m_commandfunctionarray[11].primitives=_T("module");
	m_commandfunctionarray[11].f=&ctrmthread::primitivesmodule;
	m_commandfunctionarray[12].primitives=_T("sleep");
	m_commandfunctionarray[12].f=&ctrmthread::primitivessleep;
	m_commandfunctionarray[13].primitives=_T("systemcmd");
	m_commandfunctionarray[13].f=&ctrmthread::primitivessystemcmd;
	m_commandfunctionarray[14].primitives=_T("msg");
	m_commandfunctionarray[14].f=&ctrmthread::primitivesmsg;
	m_commandfunctionarray[15].primitives=_T("ctrlstop");
	m_commandfunctionarray[15].f=&ctrmthread::primitivesctrlstop;
	m_commandfunctionarray[16].primitives=_T("label");
	m_commandfunctionarray[16].f=&ctrmthread::primitiveslabel;
	m_commandfunctionarray[17].primitives=_T("goto");
	m_commandfunctionarray[17].f=&ctrmthread::primitivesgoto;
	m_commandfunctionarray[18].primitives=_T("goton");
	m_commandfunctionarray[18].f=&ctrmthread::primitivesgoton;
	m_commandfunctionarray[19].primitives=_T("countf");
	m_commandfunctionarray[19].f=&ctrmthread::primitivescountf;
	m_commandfunctionarray[20].primitives=_T("sftp");
	m_commandfunctionarray[20].f=&ctrmthread::primitivessftp;
	m_commandfunctionarray[21].primitives=_T("ngmon");
	m_commandfunctionarray[21].f=&ctrmthread::primitivesngmon;
	return 0;
}

// 出现错误
int ctrmthread::trmfaultcounter()
{
	m_ptrmdlg->SendMessage(WM_trmdlgtrmfaultcounter,(WPARAM)m_pitemdata,NULL);
	return 0;
}

//	[comment]msg:terminal message;[1]=cmd(add/check/chkclr/clear);[2]=message
int ctrmthread::primitivesmsg()
{
	int t_result=FALSE;
	int t_mode=0;
	if(m_para[1]==_T("add"))
	{
		t_mode=1;
	}
	else if(m_para[1]==_T("check"))
	{
		t_mode=2;
	}
	else if(m_para[1]==_T("chkclr"))
	{
		t_mode=3;
	}
	else if(m_para[1]==_T("clear"))
	{
		t_mode=4;
	}
	m_pitemdata->msgoperate=FALSE;
	m_pitemdata->information=_T("[")+m_para[2]+_T("];");
	m_ptrmdlg->SendMessage(WM_trmdlgmsg,(WPARAM)m_pitemdata,t_mode);
	if(m_pitemdata->msgoperate==TRUE)
	{
		t_result=TRUE;
		m_matchnormal=1;
	}
	else
	{
		t_result=FALSE;
		m_matchnormal=0;
	}
	return t_result;
}

//	[comment]systemcmd:execute system command;[1]=cmd;[2~31]=input
//	使用ftp注意:ftp -n禁止在初始连接时自动登录,否则密码输入无效
int ctrmthread::primitivessystemcmd()
{
	// 模态创建waiting对话框
	cdlgsystemcmd t_dlgsystemcmd;
	int t_result=TRUE;
	// init value 注释 参数数组初始化
	for(int t_i=0;t_i<COMMENTPARSEPARA;t_i++)
	{
		t_dlgsystemcmd.m_para[t_i]=m_para[t_i];
	}
	t_dlgsystemcmd.m_ptrmthread=(int)this;
	INT_PTR t_nResponse=t_dlgsystemcmd.DoModal();
	if(t_nResponse==IDOK)
	{
	}
	else if(t_nResponse==IDCANCEL)
	{
		t_result=FALSE;
	}
	trmsendtrmdlgcstring(m_csrxbuff);
	// 清空rxbuff
	m_csrxbuff.Empty();
	m_matchnormal=1;
	return t_result;
}

//	[comment]sftp:[1]=operate tx/rx;[2]=local path+file name;[3]=remote path+file name;
int ctrmthread::primitivessftp()
{
	// isn't sshv2 mode
	if(m_modecomm!=mode_sshv2)
	{
		trmsendtrmdlgcstring(_T("isn't sshv2 mode,sftp unused!!!"));
		m_matchnormal=1;
		return FALSE;
	}
	m_sftp_session = libssh2_sftp_init(m_psession);
	if (!m_sftp_session) 
	{
		trmsendtrmdlgcstring(_T("Unable to init SFTP session!!!"));
		m_matchnormal=1;
		return FALSE;
	}
	//m_matchnormal=1;
	return trmsftpprocess(m_para[1]);
}

//	[comment][comment]:strict match;[1]=match string;
int ctrmthread::primitivesstrictmatch()
{
	if(m_csrxbuff.Find(m_para[1])!=-1)
	{
		m_matchstrict=1;
		m_matchnormal=1;
		return TRUE;
	}
	else
	{
		m_matchstrict=0;
		m_matchnormal=0;
		return FALSE;
	}
	return 0;
}

//	[comment]:normal match;[1]=match string;
int ctrmthread::primitivesnormalmatch()
{
	if(m_csrxbuff.Find(m_para[1])!=-1)
	{
		m_matchnormal=1;
		return TRUE;
	}
	else
	{
		m_matchnormal=0;
		return FALSE;
	}
	return 0;
}

//	[comment]ngmon:[1]=on/off;
int ctrmthread::primitivesngmon()
{
	if(m_para[1].CompareNoCase(_T("off"))==0)
	{
		m_ngmon=0;
	}
	else
	{
		m_ngmon=1;
	}
	m_matchnormal=1;
	return TRUE;
}

//	[comment]wait:waiting secounds;[1]=dialog confim time(0:always/[1,):secounds);[2]=dialog string
int ctrmthread::primitiveswaiting()
{
	// 模态创建waiting对话框
	cdlgwaiting t_dlgwaiting;
	int t_result=TRUE;
	// init value
	t_dlgwaiting.m_csmsg=m_para[2];
	t_dlgwaiting.m_cssecond=m_para[1];
	INT_PTR t_nResponse=t_dlgwaiting.DoModal();
	if(t_nResponse==IDOK)
	{
	}
	else if(t_nResponse==IDCANCEL)
	{
		t_result=FALSE;
	}
	m_matchnormal=1;
	return t_result;
}

//	[comment]cmp:include key string;[1]=match string;[2]=key string;[3]=start position;[4]=lengh;[5]=save value number[0,1023];
int ctrmthread::primitivescmp()
{
	// cmp and cmpn;0-cmp;1-cmpn
	return primitivescmpnandcmp(0);
}

//	[comment]cmpn:no include key string;[1]=match string;[2]=key string;[3]=start position;[4]=lengh;[5]=save value number[0,1023];
int ctrmthread::primitivescmpn()
{
	// cmp and cmpn;0-cmp;1-cmpn
	return primitivescmpnandcmp(1);
}

//	[comment]save:save resualt;[1]=match string;[2]=save value number[0,1023];[3]=start position("":the first word);[4]=lengh;
int ctrmthread::primitivessave()
{
	CString t_cs=m_csrxbuff;
	CString t_csreault;
	int t_return=0;
	CString t_cstemp=GREPDEF;
	if(m_para[1].Find(t_cstemp)!=-1)
	{
		CString t_expcstring;
		cmatch t_mr;
		char t_expstring[BUFFERSIZE]={0};
		char t_contents[BUFFERSIZE]={0};
		t_expcstring=m_para[1].Right(m_para[1].GetLength()-t_cstemp.GetLength());
		trmunicodehex2char(t_expcstring,t_expstring);
		trmunicodehex2char(t_cs,t_contents);
		regex t_exp(t_expstring); 
		if(regex_search(t_contents,t_mr,t_exp))
		{
			m_para[1]=CString(t_mr[0].str().c_str());
		}
	}
	if(t_cs.Find(m_para[1])!=-1)
	{
		if(m_para[3].Find(t_cstemp)!=-1)
		{
			t_cs=t_cs.Right(t_cs.GetLength()-t_cs.Find(m_para[1])-m_para[1].GetLength());
			CString t_expcstring;
			cmatch t_mr;
			char t_expstring[BUFFERSIZE]={0};
			char t_contents[BUFFERSIZE]={0};
			t_expcstring=m_para[3].Right(m_para[3].GetLength()-t_cstemp.GetLength());
			trmunicodehex2char(t_expcstring,t_expstring);
			trmunicodehex2char(t_cs,t_contents);
			regex t_exp(t_expstring); 
			if(regex_search(t_contents,t_mr,t_exp))
			{
				t_cs=CString(t_mr[0].str().c_str());
			}
			m_matchnormal=1;
			t_return=TRUE;
			m_pitemdata->information=t_cs;
			int t_i=_ttoi(m_para[2]);
			if(t_i<SAVEVALUECOUNTER)
			{
				m_ptrmdlg->SendMessage(WM_trmdlgsetsavevalue,(WPARAM)m_pitemdata,t_i);
			}
			else
			{
				assert(0);
			}
		}
		else
		{
			// 匹配m_para[1]
			int t_ibegin,t_ilength;
			// get 起始位置
			if(m_para[3].GetLength()==0)
			{
				t_ibegin=0-m_para[1].GetLength()-1;
			}
			else
			{
				t_ibegin=0-_ttoi(m_para[3])-m_para[1].GetLength();
			}
			t_cs=t_cs.Right(t_cs.GetLength()-t_cs.Find(m_para[1])+t_ibegin+1);
			// get length
			if(m_para[4].GetLength()==0)
			{
				t_ilength=t_cs.GetLength();
			}
			else
			{
				t_ilength=_ttoi(m_para[4]);
			}	
			// 匹配目标size
			if((t_cs.GetLength()>t_ilength)||(m_para[4].GetLength()==0))
			{
				m_matchnormal=1;
				t_return=TRUE;
				t_cs=t_cs.Left(t_ilength);
				t_cs.TrimLeft();
				int t_ip=0;
				// 判断命令是否包含位置标示
				if((m_para[4].GetLength()==0)||(m_para[3].GetLength()==0))
				{
					// 不包含位置标示,按单词截断
					for(;t_ip<t_cs.GetLength();t_ip++)
					{
						if((t_cs[t_ip]!=_T(' '))&&(t_cs[t_ip]!=_T('\t'))&&(t_cs[t_ip]!=_T('\n'))&&(t_cs[t_ip]!=_T('\r')))
						{
							continue;
						}
						else
						{
							break;
						}
					}
				}
				else
				{
					// 包含位置表示,按位置截断
					t_ip=((t_ilength>t_cs.GetLength())?t_cs.GetLength():t_ilength);
				}
				t_cs=t_cs.Left(t_ip);
				m_pitemdata->information=t_cs;
				int t_i=_ttoi(m_para[2]);
				if(t_i<SAVEVALUECOUNTER)
				{
					m_ptrmdlg->SendMessage(WM_trmdlgsetsavevalue,(WPARAM)m_pitemdata,t_i);
				}
				else
				{
					assert(0);
				}
			}
			else
			{
				// 未匹配目标size
				m_matchnormal=0;
				t_return=FALSE;
			}
		}
	}
	else
	{
		// 未匹配m_para[1]
		m_matchnormal=0;
		t_return=FALSE;
	}
	return t_return;
}

//	[comment]saveconst:save resualt;[1]=const;[2]=save value number[0,1023];
int ctrmthread::primitivessaveconst()
{
	int t_i=_ttoi(m_para[2]);
	m_pitemdata->information=m_para[1];
	if(t_i<SAVEVALUECOUNTER)
	{
		m_ptrmdlg->SendMessage(WM_trmdlgsetsavevalue,(WPARAM)m_pitemdata,t_i);
	}
	else
	{
		assert(0);
	}
	m_matchnormal=1;
	return TRUE;
}

//	[comment]count:count resualt;[1]=the first value[0,1023];[2]="<","=",">","!=";[3]=the second value[0,1023];[4]=save value number[0,1023];
int ctrmthread::primitivescount()
{
	CString t_cs1,t_cs2;
	int t_i=_ttoi(m_para[1]);
	CString t_csresult;
	int i_result=0;
	m_pitemdata->information=m_para[1];
	if(t_i<SAVEVALUECOUNTER)
	{
		m_ptrmdlg->SendMessage(WM_trmdlggetsavevalue,(WPARAM)m_pitemdata,t_i);
		t_cs1=m_pitemdata->information;
	}
	else
	{
		assert(0);
	}
	t_i=_ttoi(m_para[3]);
	if(t_i<SAVEVALUECOUNTER)
	{
		m_ptrmdlg->SendMessage(WM_trmdlggetsavevalue,(WPARAM)m_pitemdata,t_i);
		t_cs2=m_pitemdata->information;
	}
	else
	{
		assert(0);
	}
	if(m_para[2]==_T("="))
	{
		if(t_cs1==t_cs2)
		{
			t_csresult=_T("OK");
			i_result=1;
		}
		else
		{
			t_csresult=_T("NG");
			i_result=0;
		}
	}
	else if(m_para[2]==_T("!="))
	{
		if(t_cs1==t_cs2)
		{
			t_csresult=_T("NG");
			i_result=0;
		}
		else
		{
			t_csresult=_T("OK");
			i_result=1;
		}
	}
	else if(m_para[2]==_T(">"))
	{
		// 保留数字字符
		trmcutnonnumber(&t_cs1);
		trmcutnonnumber(&t_cs2);
		if((t_cs1.GetLength()==0)||(t_cs2.GetLength()==0))
		{
			t_csresult=_T("NG");
			i_result=0;
		}
		else if(_ttoi(t_cs1)>_ttoi(t_cs2))
		{
			t_csresult=_T("OK");
			i_result=1;
		}
		else
		{
			t_csresult=_T("NG");
			i_result=0;
		}
	} 
	else if(m_para[2]==_T("<"))
	{
		// 保留数字字符
		trmcutnonnumber(&t_cs1);
		trmcutnonnumber(&t_cs2);
		if((t_cs1.GetLength()==0)||(t_cs2.GetLength()==0))
		{
			t_csresult=_T("NG");
			i_result=0;
		}
		else if(_ttoi(t_cs1)<_ttoi(t_cs2))
		{
			t_csresult=_T("OK");
			i_result=1;
		}
		else
		{
			t_csresult=_T("NG");
			i_result=0;
		}
	}
	if(m_para[4].GetLength()!=0)
	{
		t_i=_ttoi(m_para[4]);
		if(t_i<SAVEVALUECOUNTER)
		{
			m_pitemdata->information=t_csresult;
			m_ptrmdlg->SendMessage(WM_trmdlgsetsavevalue,(WPARAM)m_pitemdata,t_i);
		}
		else
		{
			assert(0);
		}
	}
	m_matchnormal=1;
	return i_result;
}

//	[comment]countf:count resualt;[1]=the first value[0,1023];[2]="<","="">";[3]=the second value[0,1023];[4]=save value number[0,1023];
int ctrmthread::primitivescountf()
{
	CString t_cs1,t_cs2;
	int t_i=_ttoi(m_para[1]);
	CString t_csresult;
	int i_result=0;
	m_pitemdata->information=m_para[1];
	if(t_i<SAVEVALUECOUNTER)
	{
		m_ptrmdlg->SendMessage(WM_trmdlggetsavevalue,(WPARAM)m_pitemdata,t_i);
		t_cs1=m_pitemdata->information;
	}
	else
	{
		assert(0);
	}
	t_i=_ttoi(m_para[3]);
	if(t_i<SAVEVALUECOUNTER)
	{
		m_ptrmdlg->SendMessage(WM_trmdlggetsavevalue,(WPARAM)m_pitemdata,t_i);
		t_cs2=m_pitemdata->information;
	}
	else
	{
		assert(0);
	}
	//	CString t_cs3;
	//	t_cs3.Format(_T("%f;%f"),_ttof(t_cs1),_ttof(t_cs2));
	//	AfxMessageBox(t_cs3);
	if(m_para[2]==_T("="))
	{
		if((t_cs1.GetLength()==0)||(t_cs2.GetLength()==0))
		{
			t_csresult=_T("NG");
			i_result=0;
		}
		else if(_ttof(t_cs1)==_ttof(t_cs2))
		{
			t_csresult=_T("OK");
			i_result=1;
		}
		else
		{
			t_csresult=_T("NG");
			i_result=0;
		}
	}
	else if(m_para[2]==_T("!="))
	{
		if((t_cs1.GetLength()==0)||(t_cs2.GetLength()==0))
		{
			t_csresult=_T("NG");
			i_result=0;
		}
		else if(_ttof(t_cs1)==_ttof(t_cs2))
		{
			t_csresult=_T("NG");
			i_result=0;
		}
		else
		{
			t_csresult=_T("OK");
			i_result=1;
		}
	}
	else if(m_para[2]==_T(">"))
	{
		if((t_cs1.GetLength()==0)||(t_cs2.GetLength()==0))
		{
			t_csresult=_T("NG");
			i_result=0;
		}
		else if(_ttof(t_cs1)>_ttof(t_cs2))
		{
			t_csresult=_T("OK");
			i_result=1;
		}
		else
		{
			t_csresult=_T("NG");
			i_result=0;
		}
	} 
	else if(m_para[2]==_T("<"))
	{
		if((t_cs1.GetLength()==0)||(t_cs2.GetLength()==0))
		{
			t_csresult=_T("NG");
			i_result=0;
		}
		else if(_ttof(t_cs1)<_ttof(t_cs2))
		{
			t_csresult=_T("OK");
			i_result=1;
		}
		else
		{
			t_csresult=_T("NG");
			i_result=0;
		}
	}
	if(m_para[4].GetLength()!=0)
	{
		t_i=_ttoi(m_para[4]);
		if(t_i<SAVEVALUECOUNTER)
		{
			m_pitemdata->information=t_csresult;
			m_ptrmdlg->SendMessage(WM_trmdlgsetsavevalue,(WPARAM)m_pitemdata,t_i);
		}
		else
		{
			assert(0);
		}
	}
	m_matchnormal=1;
	return i_result;
}

//	[comment]buffcut:cut buff;[1]=start position("":buff first buff byte);[2]=stop position("":last buff byte);
int ctrmthread::primitivesbuffcut()
{
	CString t_cs=m_csrxbuff;
	int t_return=0;
	int t_ipos=-1;
	// 获取rxbuff最后匹配位置,注意buff完整,可在buffcut前使用strictmarch
	for(;;)
	{
		if(t_cs.Find(m_para[1])!=-1)
		{
			t_ipos=t_cs.Find(m_para[1]);
			t_cs=t_cs.Right(t_cs.GetLength()-t_ipos-m_para[1].GetLength());
		}
		else
		{
			break;
		}
	}
	if(t_ipos!=-1)
	{	
		if(m_para[2].GetLength()!=0)
		{
			if(t_cs.Find(m_para[2])!=-1)
			{
				t_cs=t_cs.Left(t_cs.Find(m_para[2]));
				m_csrxbuff=t_cs;
				m_matchnormal=1;
				t_return=TRUE;
			}
			else
			{
				m_matchnormal=0;
				t_return=FALSE;
			}
		}
		else
		{
			m_csrxbuff=t_cs;
			m_matchnormal=1;
			t_return=TRUE;
		}
	}
	else
	{
		m_matchnormal=0;
		t_return=FALSE;
	}

	return t_return;
}
//	[comment]csv:save csv file;[1]=save value number[0,1023]OR newline label['n'(new line),''(no new line)];
int ctrmthread::primitivescsv()
{
	if((m_para[1].Find(_T("n"))!=-1)||(m_para[1].Find(_T("N"))!=-1))
	{
		m_ptrmdlg->SendMessage(WM_trmdlgsetcsv,NULL,NULL);
	}
	else
	{
		int t_i=_ttoi(m_para[1]);
		if(t_i<SAVEVALUECOUNTER)
		{
			m_ptrmdlg->SendMessage(WM_trmdlgsetcsv,(WPARAM)m_pitemdata,t_i);
		}
	}
	m_matchnormal=1;
	return TRUE;
}
//	[comment]ctrld:none;
int ctrmthread::primitivesctrld()
{
	char t_sndValue[1]={CTRL_D};
	DWORD t_dwWrite=1; 
	DWORD t_dwWritten=0;
	COMSTAT t_ComStat; 
	DWORD t_dwErrorFlags; 
	BOOL t_bWriteStat; 
	// 使用信号量进行PV操作
	//WaitForSingleObject(m_ptewdlg->m_hsemaphore,INFINITE);
	// sshv2 value
	int t_i=0;
	ssize_t t_wr=0;
	switch(m_modecomm)
	{
	case mode_tcp:
		m_modeip.Send(t_sndValue,t_dwWrite);
		break;
	case mode_com:
		//同步写串口 
		ClearCommError(m_modecom,&t_dwErrorFlags,&t_ComStat); 
		t_bWriteStat=WriteFile(m_modecom,t_sndValue,t_dwWrite,&t_dwWritten,NULL); 
		if(!t_bWriteStat) 
		{ 
			AfxMessageBox(_T("写串口失败!")); 
			assert(0);
		} 
		break;
	case mode_sshv2:
		do
		{
			t_i=libssh2_channel_write(m_pchannel,t_sndValue,t_dwWrite);
			if(t_i<0) 
			{
				AfxMessageBox(_T("libssh2_channel_write error!")); 
				// 出现错误
				trmfaultcounter();
				return TRUE;
			}
			t_wr+=t_i;
		}while((t_i>0)&&(t_wr<(int)t_dwWrite));	
		break;
	default:
		break;
	}
	CString t_csedit;
	t_csedit.Format(_T("%d"),m_lineno);
	t_csedit=_T("\r\n[line:")+t_csedit+_T("]tx[ctrl-d]\r\n");
	trmsendtrmdlgcstring(t_csedit);
	m_matchnormal=1;
	return TRUE;
}
//	[comment]ctrlstop:[1]=save value number[0,1023]
int ctrmthread::primitivesctrlstop()
{	
	CString t_cs;
	int t_i=_ttoi(m_para[1]);
	if(t_i<SAVEVALUECOUNTER)
	{
		m_ptrmdlg->SendMessage(WM_trmdlggetsavevalue,(WPARAM)m_pitemdata,t_i);
		t_cs=m_pitemdata->information;
		if(t_cs==_T("NG"))
		{
			m_ptrmdlg->PostMessage(WM_trmdlgctrlstop,NULL,NULL);
		}
	}
	else
	{
		assert(0);
	}
	m_matchnormal=1;
	return TRUE;
}
//	[comment]module:[1]=module name;[2]=flag[begin,end];
int ctrmthread::primitivesmodule()
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
		AfxMessageBox(_T("miss module name"));
		return FALSE;
	}
	m_matchnormal=1;
	return TRUE;
}
//	[comment]sleep:sleep secounds;[1]=sleep confim time(0:always/[1,):secounds);[2]=sleep string
int ctrmthread::primitivessleep()
{
	int t_sleep;
	CString t_cs;
	t_sleep=_ttoi(m_para[1])*1000;
	t_cs=_T("\r\nsleep ")+m_para[1]+_T("s,do ")+m_para[2]+_T("\r\n");
	trmsendtrmdlgcstring(t_cs);
	Sleep(t_sleep);
	m_matchnormal=1;
	return TRUE;
}
// cmp and cmpn;0-cmp;1-cmpn
int ctrmthread::primitivescmpnandcmp(int v_flag)
{
	// include key string;[1]=match string;[2]=key string;[3]=start position;[4]=lengh;[5]=save value number[0,1023];
	CString t_cs=m_csrxbuff;
	CString t_cstemp;
	CString t_csreault;
	int t_grepresult=FALSE;
	t_csreault.Empty();
	CString t_csgrepflag=GREPDEF;
	if(m_para[1].Find(t_csgrepflag)!=-1)
	{
		CString t_expcstring;
		cmatch t_mr;
		char t_expstring[BUFFERSIZE]={0};
		char t_contents[BUFFERSIZE]={0};
		t_expcstring=m_para[1].Right(m_para[1].GetLength()-t_csgrepflag.GetLength());
		trmunicodehex2char(t_expcstring,t_expstring);
		trmunicodehex2char(t_cs,t_contents);
		regex t_exp(t_expstring); 
		if(regex_search(t_contents,t_mr,t_exp))
		{
			m_para[1]=CString(t_mr[0].str().c_str());
		}
	}
	if(t_cs.Find(m_para[1])!=-1)
	{
		// 匹配m_para[1]
		int t_ibegin=0;
		int t_ilength=0;
		if(m_para[3].GetLength()==0)
		{
			t_ibegin=0-m_para[1].GetLength()-1;
		}
		else
		{
			t_ibegin=0-_ttoi(m_para[3])-m_para[1].GetLength();
		}
		t_cs=t_cs.Right(t_cs.GetLength()-t_cs.Find(m_para[1])+1+t_ibegin);
		if(m_para[4].GetLength()==0)
		{
			t_ilength=0;
		}
		else
		{
			t_ilength=_ttoi(m_para[4]);
			t_cs=t_cs.Left(t_ilength);
		}
		t_cstemp=GREPDEF;
		if(m_para[2].Find(t_cstemp)!=-1)
		{
			//regex_match 判断一个正则表达式(参数 e)是否匹配整个字符序列 str
			//bool ret = regex_match(str, expression);
			//regex_replace 在整个字符序列中查找正则表达式e的所有匹配
			//str = regex_replace(str, expression, string("表达式"));
			CString t_expcstring;
			char t_expstring[BUFFERSIZE]={0};
			char t_contents[BUFFERSIZE]={0};
			t_expcstring=m_para[2].Right(m_para[2].GetLength()-t_cstemp.GetLength());
			trmunicodehex2char(t_expcstring,t_expstring);
			trmunicodehex2char(t_cs,t_contents);
			regex t_exp(t_expstring); 
			if(regex_search(t_contents,t_exp))
			{
				t_grepresult=TRUE;
			}
			else
			{
				t_grepresult=FALSE;
			}
		}
		if((t_cs.Find(m_para[2])!=-1)||t_grepresult)
		{
			// 发现
			if(v_flag==1)
			{
				m_matchnormal=1;
				t_csreault=_T("NG");
			}
			else
			{
				// cmp normal
				m_matchnormal=1;
				t_csreault=_T("OK");
			}
		}
		else if((t_cs.GetLength()>=t_ilength)&&(t_ilength!=0))
		{
			// rx buff 完整，但未匹配
			if(v_flag==1)
			{
				m_matchnormal=1;
				t_csreault=_T("OK");
			}
			else
			{
				// cmp normal
				m_matchnormal=1;
				t_csreault=_T("NG");
			}
		}
		else
		{
			// 未发现，rx buff 不完整,延迟时间FAULTRETRYDELAY，重试次数FAULTRETRYTIMES
			if(m_ngtimes<FAULTRETRYTIMES)
			{
				Sleep(FAULTRETRYDELAY);
				m_ngtimes++;
				m_matchnormal=0;
			}
			else if(m_ngtimes==FAULTRETRYTIMES)
			{
				if((t_cs.Find(m_para[2])!=-1)||t_grepresult)
				{
					// 发现
					if(v_flag==1)
					{
						m_matchnormal=1;
						t_csreault=_T("NG");
					}
					else
					{
						// cmp normal
						m_matchnormal=1;
						t_csreault=_T("OK");
					}
				}
				else
				{
					// 未发现
					if(v_flag==1)
					{
						// cmpn normal
						m_matchnormal=1;
						t_csreault=_T("OK");
					}
					else
					{
						m_matchnormal=1;
						t_csreault=_T("NG");
					}
				}
				m_ngtimes=0;
			}
		}
		if((t_csreault.GetLength()!=0)&&(m_para[5].GetLength()!=0))
		{
			m_pitemdata->information=t_csreault;
			int t_i=_ttoi(m_para[5]);
			if(t_i<SAVEVALUECOUNTER)
			{
				m_ptrmdlg->SendMessage(WM_trmdlgsetsavevalue,(WPARAM)m_pitemdata,t_i);
			}
			else
			{
				assert(0);
			}
		}
	}
	else
	{
		// 未匹配m_para[1]
		m_matchnormal=0;
	}
	if(t_csreault==_T("NG"))
	{
		return FALSE;
	}
	else if(t_csreault==_T("OK"))
	{
		return TRUE;
	}
	return FALSE;
}

//	[comment]label:[1]=label name;
int ctrmthread::primitiveslabel()
{
	m_matchnormal=1;
	return TRUE;
}

//	[comment]goto:[1]=label name;[2]=save value number[0,1023];[3]=match string;
int ctrmthread::primitivesgoto()
{
	CString t_cs1,t_csline;
	int t_i=_ttoi(m_para[2]);
	CString t_csresult;
	// 获取保存变量值
	if(t_i<SAVEVALUECOUNTER)
	{
		m_ptrmdlg->SendMessage(WM_trmdlggetsavevalue,(WPARAM)m_pitemdata,t_i);
		t_cs1=m_pitemdata->information;
	}
	else
	{
		assert(0);
	}
	// 判断获取保存变量值是否匹配匹配字符串,忽略大小写
	if(t_cs1.CompareNoCase(m_para[3])==0)
	{
		// goto label
		m_csfscriptfile.SeekToBegin();
		int t_lineno=0;
		for(t_i=0;m_csfscriptfile.ReadString(t_csline)!=NULL;)
		{
			t_i++;
			if(t_csline.Find(_T("label;")+m_para[1])!=-1)
			{
				t_lineno=t_i;
				break;
			}
		}
		if(t_lineno!=0)
		{
			// find label name
			m_lineno=t_lineno;
			Sleep(1000);
		}
		else
		{
			// no find label name
			m_csfscriptfile.SeekToBegin();
			for(t_i=0;t_i<m_lineno;t_i++)
			{
				m_csfscriptfile.ReadString(t_csline);
			}
		}
	}
  m_matchstrict = 1;
	m_matchnormal=1;
  m_matchstrict = 1;
	return TRUE;
}

//	[comment]goton:[1]=label name;[2]=save value number[0,1023];[3]=match string;
//  其实和primitivesgoton一个模子,只是脑子有点晕,以后再优化
int ctrmthread::primitivesgoton()
{
	CString t_cs1,t_csline;
	int t_i=_ttoi(m_para[2]);
	CString t_csresult;
	// 获取保存变量值
	if(t_i<SAVEVALUECOUNTER)
	{
		m_ptrmdlg->SendMessage(WM_trmdlggetsavevalue,(WPARAM)m_pitemdata,t_i);
		t_cs1=m_pitemdata->information;
	}
	else
	{
		assert(0);
	}
	// 判断获取保存变量值是否匹配匹配字符串,忽略大小写
	if(t_cs1.CompareNoCase(m_para[3])!=0)
	{
		// goto label
		m_csfscriptfile.SeekToBegin();
		int t_lineno=0;
		for(t_i=0;m_csfscriptfile.ReadString(t_csline)!=NULL;)
		{
			t_i++;
			if(t_csline.Find(_T("label;")+m_para[1])!=-1)
			{
				t_lineno=t_i;
				break;
			}
		}
		if(t_lineno!=0)
		{
			// find label name
			m_lineno=t_lineno;
			Sleep(1000);
		}
		else
		{
			// no find label name
			m_csfscriptfile.SeekToBegin();
			for(t_i=0;t_i<m_lineno;t_i++)
			{
				m_csfscriptfile.ReadString(t_csline);
			}
		}
	}
  m_matchstrict = 1;
	m_matchnormal=1;
	return TRUE;
}

// mode_comm初始化
int ctrmthread::trmmodecomminit(int v_mode)
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
	CString t_cs;
	t_cs.Empty();
	char t_username[64]={0};
	char t_password[64]={0};
	//	char t_chkeyfilepub[256]={0};	
	char *t_ch;
	// socket vlaue
	struct sockaddr_storage t_addr;
	socklen_t t_addr_len=sizeof(t_addr);
	char t_dhost[64]={0};
	//	int t_dport;
	CString t_csshost;
	CString t_cssport;
	TCHAR t_straddr[32]={0};          
	DWORD t_addrlen=32;  
	char t_shost[64]={0};
	//	int t_sport;
	//使用信号量进行PV操作
	//WaitForSingleObject(m_ptewdlg->m_hsemaphore,INFINITE);
	// get host by value
	if(m_pitemdata->host.Find(REPLACE_VALUE)!=-1)
	{
		CString t_cd=REPLACE_VALUE;
		int t_i=_ttoi(m_pitemdata->host.Right(m_pitemdata->host.GetLength()-t_cd.GetLength()));
		assert((t_i>=0)&&(t_i<SAVEVALUECOUNTER));
		if((t_i>=0)&&(t_i<SAVEVALUECOUNTER))
		{
			m_ptrmdlg->SendMessage(WM_trmdlggetsavevalue,(WPARAM)m_pitemdata,t_i);
			m_pitemdata->host=m_pitemdata->information;
		}
	}
	switch(v_mode)
	{
	case mode_sshv2:
		// 初始化socket
		if( AfxSocketInit()==FALSE)
		{ 
			assert(0);
		}
		trmsendtrmdlgcstring(_T("libssh2_init begin!"));
		Sleep(1);
		// 初始化libssh2
		m_sshv2rc = 0;// libssh2_init(0);
		//AfxMessageBox(_T("libssh2_init end!"));
		trmsendtrmdlgcstring(_T("libssh2_init end!"));
		if(m_sshv2rc!=0) 
		{
			t_cs.Format(_T("libssh2 initialization failed (%d)\n"),m_sshv2rc);
			trmsendtrmdlgcstring(t_cs);
			return TRUE;
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
		// Create a session instance  
		m_psession=libssh2_session_init();
		if(!m_psession) 
		{
			t_cs.Format(_T("Could not initialize SSH session!\n"));
			trmsendtrmdlgcstring(t_cs);
			return TRUE;
		}
		// start it up. This will trade welcome banners, exchange keys, and setup crypto, compression, and MAC layers
		m_sshv2rc=libssh2_session_handshake(m_psession,m_modeip);
		if(m_sshv2rc) 
		{
			t_cs.Format(_T("Error when starting up SSH session: %d\n"),m_sshv2rc);
			trmsendtrmdlgcstring(t_cs);
			return TRUE;
		}
		// At this point we havn't yet authenticated.  The first thing to do
		// is check the hostkey's fingerprint against our known hosts Your app
		// may have it hard coded, may go to a file, may present it to the
		// user, that's your call
		libssh2_hostkey_hash(m_psession,LIBSSH2_HOSTKEY_HASH_SHA1);
		// check what authentication methods are available
		// cstring to char*,get user
		WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)(this->m_ptrmmode->user),-1,t_username,64,0,false); 
		t_ch=libssh2_userauth_list(m_psession,t_username,strlen(t_username));
		if(strstr(t_ch,"password"))
		{
			m_sshv2auth|=AUTH_PASSWORD;
		}
		if(strstr(t_ch,"publickey"))
		{
			m_sshv2auth|=AUTH_PUBLICKEY;
		}
		// auth
		// cstring to char*,get password
		WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)(this->m_ptrmmode->pwd),-1,t_password,64,0,false); 
		if (m_sshv2auth&AUTH_PASSWORD) 
		{
			if(libssh2_userauth_password(m_psession,t_username,t_password)) 
			{

				t_cs.Format(_T("Authentication by password failed.\n"));
				trmsendtrmdlgcstring(t_cs);
				return TRUE;
			}
		} 
		else if(m_sshv2auth&AUTH_PUBLICKEY) 
		{
			char t_chkeyfile[256]={0};
			char t_chkeyfilepub[256]={0};
			m_cskeyfilepub=this->m_pitemdata->path+_T("Identity.pub");
			m_cskeyfile=this->m_pitemdata->path+_T("Identity");
			// cstring to char*,get keyfile
			WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)(m_cskeyfilepub),-1,t_chkeyfilepub,256,0,false); 
			WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)(m_cskeyfile),-1,t_chkeyfile,256,0,false); 
			if(libssh2_userauth_publickey_fromfile(m_psession,t_username,t_chkeyfilepub,t_chkeyfile,t_password)) 
			{
				t_cs.Format(_T("\tAuthentication by public key failed!\n"));
				trmsendtrmdlgcstring(t_cs);
				return TRUE;
			}
			trmsendtrmdlgcstring(_T("\tAuthentication by public key succeeded.\n"));
		} 
		else
		{
			t_cs.Format(_T("No supported authentication methods found!\n"));
			trmsendtrmdlgcstring(t_cs);
			return TRUE;
		}
		// get host and port
		// get dhost
		WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)(m_pitemdata->host),-1,t_dhost,64,0,false); 
		// get dport
		//		t_dport=m_pitemdata->port;
		// get shost and sport
		if(getsockname(m_modeip.m_hSocket,(sockaddr*)&t_addr,&t_addr_len))
		{
			t_cs.Format(_T("getsockname failed\n"));
			trmsendtrmdlgcstring(t_cs);
			return TRUE;
		}
		WSAAddressToString((sockaddr*)&t_addr,t_addr_len,NULL,t_straddr,&t_addrlen);
		t_cs.Format(_T("%s"),t_straddr);
		t_cssport=t_cs.Right(t_cs.GetLength()-t_cs.Find(_T(':'))-1);
		t_csshost=t_cs.Left(t_cs.Find(_T(':')));
		WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)(t_csshost),-1,t_shost,64,0,false); 		
		//		t_sport=_ttoi(t_cssport);
		/* Request a shell */ 
		m_pchannel=libssh2_channel_open_session(m_psession);
		if(!m_pchannel) 
		{
			t_cs.Format(_T("Unable to open a session\n"));
			trmsendtrmdlgcstring(t_cs);
			return TRUE;
		}
		// Some environment variables may be set,
		// It's up to the server which ones it'll allow though
		libssh2_channel_setenv(m_pchannel,"FOO","bar");
		// Request a terminal with 'vanilla' terminal emulation
		// See /etc/termcap for more options
		if(libssh2_channel_request_pty(m_pchannel,"ansi")) 
		{
			t_cs.Format(_T("Failed requesting pty\n"));
			trmsendtrmdlgcstring(t_cs);
			return TRUE;
		}
		// Open a SHELL on that pty */ 
		if(libssh2_channel_shell(m_pchannel)) 
		{
			t_cs.Format(_T("Unable to request shell on allocated pty\n"));
			trmsendtrmdlgcstring(t_cs);
			return TRUE;
		}
		// Blocking mode may be (en|dis)abled with: 
		libssh2_channel_set_blocking(m_pchannel,TRUE);
		break;
	case mode_tcp:
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
		break;
	case mode_com:
		t_cs.Format(_T("%d"),m_pitemdata->port);
		t_cs=m_pitemdata->host+t_cs;
		// 打开串口
		m_modecom=CreateFile(t_cs,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);  
		if(m_modecom==(HANDLE)-1)
		{
			t_cs+=_T("失败");
			trmsendtrmdlgcstring(t_cs);
			return TRUE;
		}
		else
		{
			DCB t_wdcb;
			GetCommState(m_modecom,&t_wdcb); // 设置串口；
			t_wdcb.BaudRate=m_ptrmmode->BaudRate;
			// wdcb.fBinary=TRUE;
			// wdcb.fParity=TRUE;
			t_wdcb.ByteSize=m_ptrmmode->ByteSize;
			t_wdcb.Parity=m_ptrmmode->Parity;
			t_wdcb.StopBits=m_ptrmmode->StopBits;
			// 缓冲区设置
			SetupComm(m_modecom,BUFFERSIZE,BUFFERSIZE);
			SetCommState(m_modecom,&t_wdcb);
			// 清空缓存
			PurgeComm(m_modecom,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
		}
		COMMTIMEOUTS TimeOuts; 
		// 设定读超时 
		TimeOuts.ReadIntervalTimeout=MAXDWORD; 
		TimeOuts.ReadTotalTimeoutMultiplier=0; 
		TimeOuts.ReadTotalTimeoutConstant=0; 
		// 设定写超时 
		TimeOuts.WriteTotalTimeoutMultiplier=500; 
		TimeOuts.WriteTotalTimeoutConstant=2000; 
		// 设置超时
		SetCommTimeouts(m_modecom,&TimeOuts);
		break;
	case mode_gpib:
		/*		// gpib控制器地址为0
		SendIFC(GPIB0);
		if(ibsta & ERR)
		{
		trmmodeGPIBCleanup(GPIB0,_T("Unable to open board(GPIB)"));
		return TRUE;
		}
		m_modegpib=m_pitemdata->port;
		DevClear(GPIB0,m_modegpib);
		if (ibsta & ERR)
		{
		trmmodeGPIBCleanup(GPIB0,_T("Unable to clear devices(GPIB)"));
		return TRUE;
		}
		break;
		*/
		/*****************************************************************************
		* Initialization - Done only once at the beginning of your application.
		*****************************************************************************/
		m_modegpib=m_pitemdata->port;
		m_hgpib=ibdev(		/* Create a unit descriptor handle         */
			GPIB0,			/* Board Index (GPIB0=0, GPIB1=1, ...) */
			m_modegpib,		/* Device primary address                  */
			0,				/* Device secondary address                */
			T10s,			/* Timeout setting (T10s=10 seconds)     */
			1,				/* Assert EOI line at end of write         */
			0);				/* EOS termination mode                    */
		if (ibsta & ERR) 
		{
			/* Check for GPIB Error                    */
			t_cs=_T("Unable to open board(GPIB ibdev)");
			trmmodeGPIBCleanup(GPIB0,t_cs);
			return TRUE;
		}
		/* Clear the device                        */
		ibclr(m_hgpib);                 
		if (ibsta & ERR) 
		{
			t_cs=_T("Unable to open board(GPIB ibclr)");
			trmmodeGPIBCleanup(GPIB0,t_cs);
			return TRUE;
		}
	default:
		break;
	} 
	// 将错误写入nginformation
	if(t_cs.GetLength()>0)
	{
		trmsetnginformation(t_cs);
	}
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
	//使用信号量进行PV操作
	//ReleaseSemaphore(m_ptewdlg->m_hsemaphore,1,NULL);
	return FALSE;
}

/*
*  After each GPIB call, the application checks whether the call
*  succeeded. If an NI-488.2 call fails, the GPIB driver sets the
*  corresponding bit in the global status variable. If the call
*  failed, this procedure prints an error message, takes the board
*  offline and exits.
*/
void ctrmthread::trmmodeGPIBCleanup(int v_ud, CString v_cs)
{
	ibonl(v_ud,0);
	m_trmdlgeditbuffpart=v_cs;
	trmsendtrmdlgcstring(m_trmdlgeditbuffpart);
}

//  mode_comm结束
int ctrmthread::trmmodecommexit(int v_mode)
{
	switch(v_mode)
	{
	case mode_sshv2:
		if(m_pchannel)
		{
			libssh2_channel_free(m_pchannel);
			libssh2_session_disconnect(m_psession,"Client disconnecting normally");
			libssh2_session_free(m_psession); 
			m_modeip.Close();
			// 网络质量不好，还需要延长
			Sleep(TRMTHREADENDDELAY);
			libssh2_exit();
		}
		break;
	case mode_tcp:
		// 判断m_modeip的rxbuff是否有内容,清空后再关闭。否则close会发RST，而不是FIN
		if(trmrxbufferok()==1)
		{
			char t_recValue[BUFFERSIZE]={0};
			if(m_modeip.Receive((void *)t_recValue,BUFFERSIZE)==-1)
			{
				// socket 通信异常
				CString t_cs;
				t_cs.Format(_T("%s: %d"),_T("socket rx failed"),::GetLastError());
				trmsendtrmdlgcstring(t_cs);
				this->PostThreadMessage(WM_QUIT,0,0);
				trmfaultcounter();
			}
		}
		m_modeip.Close();
		// 网络质量不好，还需要延长
		Sleep(TRMTHREADENDDELAY);
		break;
	case mode_com:
		CloseHandle(m_modecom);
		break;
	case mode_gpib:
		ibonl(GPIB0,0);
		m_modegpib=0;
		m_hgpib=0;
		break;
	default:
		break;
	}
	m_csfscriptfile.Close();
	m_csflogfile.Close();
	return FALSE;
}

// 终端receive
int ctrmthread::trmmodecommrx(int v_mode)
{
	char t_recValue[BUFFERSIZE]={0};
	DWORD t_wCount;
	BOOL t_bReadStat;
	// 使用信号量进行PV操作
	//WaitForSingleObject(m_ptewdlg->m_hsemaphore,INFINITE);
	switch(v_mode)
	{
	case mode_sshv2:
		// 判断m_modeip的rxbuff是否有内容
		if(trmrxbufferok()==1)
		{
			t_wCount=static_cast<DWORD>(libssh2_channel_read(m_pchannel,t_recValue,BUFFERSIZE));
			if(LIBSSH2_ERROR_EAGAIN==t_wCount)
			{
				m_trmdlgeditbuffpart=_T("error:LIBSSH2_ERROR_EAGAIN\n");
				trmsendtrmdlgcstring(m_trmdlgeditbuffpart); 
				// 出现错误
				trmfaultcounter();
				return TRUE;;
			}
			else if((static_cast<long>(t_wCount))<0) 
			{
				m_trmdlgeditbuffpart=_T("libssh2_channel_read error!\n");
				trmsendtrmdlgcstring(m_trmdlgeditbuffpart); 
				// 出现错误
				trmfaultcounter();
				return TRUE;;
			}
			if(libssh2_channel_eof(m_pchannel)) 
			{
				m_trmdlgeditbuffpart=_T("The server disconnected!\n");
				trmsendtrmdlgcstring(m_trmdlgeditbuffpart); 
				// 出现错误
				trmfaultcounter();
				return TRUE;
			}
		}
		break;
	case mode_tcp: 
		// 判断m_modeip的rxbuff是否有内容
		if(trmrxbufferok()==1)
		{
			if(m_modeip.Receive((void *)t_recValue,BUFFERSIZE)==-1)
			{
				// socket 通信异常
				CString t_cs;
				t_cs.Format(_T("%s: %d"),_T("socket rx failed"),::GetLastError());
				trmsendtrmdlgcstring(t_cs);
				this->PostThreadMessage(WM_QUIT,0,0);
				trmfaultcounter();
				return TRUE;
			}
		}
		break;
	case mode_com:
		t_bReadStat=ReadFile(m_modecom,t_recValue,BUFFERSIZE,&t_wCount,NULL);
		if(!t_bReadStat) 
		{ 
			m_trmdlgeditbuffpart=_T("读串口失败!");
			trmsendtrmdlgcstring(m_trmdlgeditbuffpart); 
			// 出现错误
			trmfaultcounter();
			return TRUE;
		} 
		break;
	case mode_gpib:
		/*		Receive(GPIB0,m_modegpib,t_recValue,BUFFERSIZE,STOPend);
		if (ibsta & ERR)
		{
		trmmodeGPIBCleanup(GPIB0,_T("Unable to read from a device(GPIB)"));
		return 1;
		}
		t_recValue[ibcntl]='\0';
		*/
		/* Read up to 100 bytes from the device    */
		ibrd(m_hgpib,t_recValue,BUFFERSIZE);     
		if (ibsta & ERR) 
		{
			trmmodeGPIBCleanup(GPIB0,_T("Unable to open board(GPIB ibrd)"));
			// 出现错误
			trmfaultcounter();
			return TRUE;
		}
		break;
	default:
		break;
	}
	// 使用信号量进行PV操作
	// ReleaseSemaphore(m_ptewdlg->m_hsemaphore,1,NULL);
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
	m_trmdlgeditbuffpart.Empty();
	m_trmdlgeditbuffpart=CString(t_recValue);
	// 收到内容则显示，避免edit控件过快刷新
	if(m_trmdlgeditbuffpart.GetLength()>0)
	{
		trmsendtrmdlgcstring(m_trmdlgeditbuffpart);

	}
	m_csrxbuff+=m_trmdlgeditbuffpart;	
	return 0;
}

// 终端transmit
int ctrmthread::trmmodecommtx(int v_mode)
{
	char t_sndValue[BUFFERSIZE]={0};
	DWORD t_dwWrite; 
	DWORD t_dwWritten=0;
	COMSTAT t_ComStat; 
	DWORD t_dwErrorFlags; 
	BOOL t_bWriteStat; 
	// sshv2 value
	int t_i=0;
	ssize_t t_wr=0;
	// 显示发送信息
	CString t_csedit;
	t_csedit.Format(_T("%d"),m_lineno);
	t_csedit=_T("\r\n[line:")+t_csedit+_T("]")+getsystemdatetime(0)+_T(" tx[")+m_cstxbuff+_T("]\r\n");
	trmsendtrmdlgcstring(t_csedit);
	// end of line处理:\r\n or \n
	m_cstxbuff+=m_ptrmmode->endofline;
	// cstring to char*
	WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)m_cstxbuff,-1,t_sndValue,BUFFERSIZE,0,false); 
	t_dwWrite=strlen(t_sndValue);
	// 使用信号量进行PV操作
	//WaitForSingleObject(m_ptewdlg->m_hsemaphore,INFINITE);
	switch(v_mode)
	{
	case mode_sshv2:
		t_wr=0;
		do
		{
			t_i=libssh2_channel_write(m_pchannel,t_sndValue,t_dwWrite);
			if(t_i<0) 
			{
				AfxMessageBox(_T("libssh2_channel_write error!")); 
				// 出现错误
				trmfaultcounter();
				return TRUE;
			}
			t_wr+=t_i;
		}while((t_i>0)&&(t_wr<(int)t_dwWrite));		
		break;
	case mode_tcp:
		m_modeip.Send(t_sndValue,t_dwWrite);
		break;
	case mode_com:
		//同步写串口 
		ClearCommError(m_modecom,&t_dwErrorFlags,&t_ComStat); 
		t_bWriteStat=WriteFile(m_modecom,t_sndValue,t_dwWrite,&t_dwWritten,NULL); 
		if(!t_bWriteStat) 
		{ 
			AfxMessageBox(_T("写串口失败!")); 
			// 出现错误
			trmfaultcounter();
			return TRUE;
		} 
		break;
	case mode_gpib:
		/*		Send(GPIB0,m_modegpib,t_sndValue,t_dwWrite,NULLend);
		//ibwrt(m_hgpib,sndValue,dwWrite);
		if (ibsta & ERR)
		{
		trmmodeGPIBCleanup(GPIB0,_T("Unable to write to devices(GPIB)"));
		return TRUE;
		}
		*/
		/* Send the identification query command   */
		ibwrt(m_hgpib,t_sndValue,t_dwWrite);     
		if (ibsta & ERR) 
		{
			trmmodeGPIBCleanup(GPIB0,_T("Unable to open board(GPIB ibwrt)"));
			// 出现错误
			trmfaultcounter();
			return TRUE;
		}
		break;
	default:
		break;
	}
	// 使用信号量进行PV操作
	//ReleaseSemaphore(m_ptewdlg->m_hsemaphore,1,NULL);
	if(m_ptrmmode->displayback)
	{
		m_trmdlgeditbuffpart=m_cstxbuff;
		trmsendtrmdlgcstring(m_trmdlgeditbuffpart);
	}
	return FALSE;
}

// unicode hex to char;v_str char[65535]
int ctrmthread::trmunicodehex2char(CString v_cs,char* v_str)
{
	int t_len=0;
	t_len=v_cs.GetLength();
	WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)v_cs,-1,v_str,t_len,0,false); 
	return t_len;
}

// trm set nginformation
int ctrmthread::trmsetnginformation(CString v_cs)
{
	m_pitemdata->nginformation+=v_cs+_T("\n");
	return 0;
}

//	sftp process
int ctrmthread::trmsftpprocess(CString v_cs)
{
	int t_rc;
	struct timeval t_timeout;
	char *t_ptr;
	char t_buff[BUFFERSIZE];
	int t_result=FALSE;
	CString t_cs;
	CStdioFile t_csf;
	fd_set t_fd;
	// cstring to char*
	char t_remotepathfile[PATHMAXLENGTH];
	WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)m_para[3],-1,t_remotepathfile,PATHMAXLENGTH,0,false); 
	if(v_cs.CompareNoCase(_T("tx"))==0)
	{
		m_sftp_handle=libssh2_sftp_open(m_sftp_session,t_remotepathfile,LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC,LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);
		if(!m_sftp_handle) 
		{
			t_cs=_T("sftp remote file open fail!");
			t_result=FALSE;
		}
		else
		{
			if(t_csf.Open(m_para[2],CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
			{
				do 
				{
					UINT t_sftpiobytes;
					t_sftpiobytes=t_csf.Read(t_buff,BUFFERSIZE);
					if((t_sftpiobytes==0)||(t_sftpiobytes>=0x80000000))
					{
						/* end of file */ 
						break;
					}
					t_ptr=t_buff;
					do 
					{
						/* write data in a loop until we block */ 
						t_rc=libssh2_sftp_write(m_sftp_handle,t_ptr,t_sftpiobytes);
						if(t_rc<0)
						{
							break;
						}
						t_ptr+=t_rc;
						t_sftpiobytes-=t_rc;
					}while(t_sftpiobytes);
				}while(t_rc>0);
				t_cs=_T("\nsftp tx completed!\n");
				t_csf.Close();
				t_result=TRUE;
			}
			else
			{
				t_cs=_T("sftp local file open fail!");
				t_result=FALSE;
			}
		}
		libssh2_sftp_close(m_sftp_handle);
		libssh2_sftp_shutdown(m_sftp_session);
	}
	else if(v_cs.CompareNoCase(_T("rx"))==0)
	{
		m_sftp_handle=libssh2_sftp_open(m_sftp_session,t_remotepathfile,LIBSSH2_FXF_READ,0);
		if(!m_sftp_handle) 
		{
			if(libssh2_session_last_errno(m_psession)!=LIBSSH2_ERROR_EAGAIN) 
			{
				t_cs=_T("Unable to open file with SFTP!!!\n");
				t_result=FALSE;
			}
			else 
			{
				t_cs=_T("non-blocking open!!!\n");	 
				t_result=FALSE;
			}
		}
		else
		{
			if(t_csf.Open(m_para[2],CFile::modeWrite|CFile::modeCreate|CFile::typeBinary|CFile::shareDenyNone))
			{
				do 
				{
					do 
					{
						/* read in a loop until we block */ 
						t_rc=libssh2_sftp_read(m_sftp_handle,t_buff,BUFFERSIZE);
						if(t_rc>0) 
						{
							t_ptr=t_buff;
							t_csf.Write(t_ptr,t_rc);
						}
					}while(t_rc>0);
					if(t_rc!=LIBSSH2_ERROR_EAGAIN) 
					{
						/* error or end of file */ 
						t_cs=_T("\nsftp rx completed!\n");
						t_result=TRUE;
						break;
					}
					t_timeout.tv_sec=3;
					t_timeout.tv_usec=0;
					FD_ZERO(&t_fd);
					FD_SET(m_modeip,&t_fd); 
					/* wait for readable or writeable */ 
					t_rc=select(m_modeip+1,&t_fd,&t_fd,NULL,&t_timeout);
					if(t_rc<=0) 
					{
						/* negative is error
						0 is timeout */ 
						t_cs=_T("SFTP download timed out!!!%d\n");
						t_result=FALSE;
						break;
					}
				}while(1);
				t_csf.Close();
			}
			else
			{
				t_cs=_T("sftp local file open fail!");
				t_result=FALSE;
			}
		}
		libssh2_sftp_close(m_sftp_handle);
		libssh2_sftp_shutdown(m_sftp_session);
	}
	else
	{
		t_cs=_T("sftp unknow parament!!!");
		t_result=FALSE;
	}
	trmsendtrmdlgcstring(t_cs);
	m_matchnormal=1;
	return t_result;
}

// 替换字符串中变量符号,"$_"开头
//	return TRUE:none
//	return FALSE:exist
//	$_host替换为m_pitemdata->host
//	$__[digit]替换为save变量[digit]
//	macformat$__macaddr更换格式为XXXX.XXXX.XXXX
int ctrmthread::trmtxbuffgetstring4replacelabel(CString *v_pline)
{
	int t_rt=TRUE;
	CString t_cslabel,t_cslabelid;
	v_pline->Replace(_T("$_host"),m_pitemdata->host);
	for(;;)
	{
		int t_i;
		t_i=v_pline->Find(REPLACE_LABEL);
		if(t_i!=-1)
		{
			int t_j;
			// 发现替换符号
			t_cslabelid=REPLACE_LABEL;
			t_j=t_cslabelid.GetLength();
			t_cslabel=v_pline->Right(v_pline->GetLength()-t_i-t_j);
			t_j=t_cslabel.GetLength();
			for(t_i=0;t_i<t_j;t_i++)
			{
				if(!(((t_cslabel[t_i]>=_T('0'))&&(t_cslabel[t_i]<=_T('9')))||\
					((t_cslabel[t_i]>=_T('a'))&&(t_cslabel[t_i]<=_T('z')))||\
					((t_cslabel[t_i]>=_T('A'))&&(t_cslabel[t_i]<=_T('Z')))||\
					(t_cslabel[t_i]==_T('_'))))
				{
					break;
				}
			}
			// get replace label key
			m_pitemdata->fieldname=t_cslabel.Left(t_i);
			m_ptrmdlg->SendMessage(WM_trmdlgreplacelabel,(WPARAM)m_pitemdata,0);			
			if((v_pline->Find(_T("macformat"))!=-1)||(v_pline->Find(_T("MACFORMAT"))!=-1))
			{
				// 存在macformat指示
				v_pline->Replace(_T("macformat"),_T(""));
				v_pline->Replace(REPLACE_LABEL+m_pitemdata->fieldname,trmmacformat(m_pitemdata->fieldvalue));
			}
			else
			{
				v_pline->Replace(REPLACE_LABEL+m_pitemdata->fieldname,m_pitemdata->fieldvalue);
			}
			t_rt=FALSE;
		}
		else
		{
			break;
		}
	}
	return t_rt;
}

// 格式化macXX.XX.XX.XX.XX.XX to XXXX.XXXX.XXXX
CString ctrmthread::trmmacformat(CString v_line)
{
	CString t_cs;
	t_cs=v_line;
	TCHAR* t_tch=t_cs.GetBuffer();
	t_tch[11]=t_tch[5]=_T('.');
	t_cs.ReleaseBuffer();
	t_cs.Replace(_T(":"),_T(""));
	return t_cs;
}

// autologin
int ctrmthread::Ontrmthreadautologin(int v_i)
{
	// autologin
	if(m_ptrmmode->autologin==1)
	{
		switch(v_i)
		{
		case 0:
			// user
			// read rx buff
			trmmodecommrx(m_modecomm);
			if(m_csrxbuff.Find(m_ptrmmode->displayuser)!=-1)
			{
				// tx user
				m_cstxbuff=m_ptrmmode->user+RETURNNEWLINE;
				trmmodecommtx(m_modecomm);
				// autologin password
				this->PostThreadMessage(WM_trmthreadautologin,1,0);
				m_csrxbuff.Empty();
			}
			else
			{
				// autologin user
				this->PostThreadMessage(WM_trmthreadautologin,0,0);
			}
			break;
		case 1:
			// password
			// read rx buff
			trmmodecommrx(m_modecomm);
			if(m_csrxbuff.Find(m_ptrmmode->displaypwd)!=-1)
			{
				// tx password
				m_cstxbuff=m_ptrmmode->pwd+RETURNNEWLINE;
				trmmodecommtx(m_modecomm);
				// run
				this->PostThreadMessage(WM_trmthreadrun,0,0);
				m_csrxbuff.Empty();
			}
			else
			{
				// autologin user
				this->PostThreadMessage(WM_trmthreadautologin,1,0);
			}
			break;
		default:
			break;
		}
	}
	else
	{
		// run
		this->PostThreadMessage(WM_trmthreadrun,0,0);
	}
	Sleep(TRMDELAY+m_ptrmmode->delaytime);
	return 0;
}

// initreturn
int ctrmthread::Ontrmthreadinitreturn()
{
	// initreturn
	if(m_ptrmmode->initreturn==1)
	{
		// tx return;
		m_cstxbuff=m_ptrmmode->endofline;
		trmmodecommtx(m_modecomm);				
	}
	// autologin
	this->PostThreadMessage(WM_trmthreadautologin,0,0);
	return 0;
}

// script run
int ctrmthread::Ontrmthreadrun()
{
	// 匹配完成或script开始需读取一行scriptfile
	if((m_matchnormal!=0)||(m_lineno==0))
	{
		// read script file one line
		if(trmreadscriptfileline())
		{
			// 命令计时开始
			trmcmdtimer();
			// 忽略无用信息
			m_linestring.TrimLeft();
			m_linestring.TrimRight();
			// 判断脚本行类型:TRUE 软件命令;FALSE 下发脚本
			m_commentflag=trmscriptlineiscomment(&m_linestring);
			if(m_commentflag==TRUE)
			{
				// 注释行分解参数数组初始化
				for(int t_i=0;t_i<COMMENTPARSEPARA;t_i++)
				{
					m_para[t_i].Empty();
				}
				//	分析注释行
				trmparsecomentline(m_linestring);
			}
			// 第一行为下发脚本，直接下发
			if((m_lineno==0)&&(m_commentflag==FALSE))
			{
				m_matchnormal=m_matchstrict=1;
			}
		}
		else
		{
			// 延时,获取rxbuff
			// 超过延时，需要在脚本文件中增加匹配命令，以确认命令下发完成然后关闭连接
			Sleep(ENDOFSCRIPTFILEDELAY);
			// gpib命令查询方式需拿取rx buffer,或者不为gpib时也拿取rx buffer
			if((m_modecomm!=mode_gpib)||((m_commentflag==FALSE)&&(m_modecomm==mode_gpib)&&(m_linestring.Find(_T('?'))!=-1)&&(m_linestring.GetLength()>0)))
			{
				// read rx buff
				trmmodecommrx(m_modecomm);	
			}
			// 脚本文件运行结束
			this->PostThreadMessage(WM_QUIT,0,0);
			return 0;
		}
	}
	// 未匹配以及gpib方式需拿取rx buffer
	if((m_matchnormal==0)&&(m_modecomm!=mode_gpib))
	{
		// read rx buff
		trmmodecommrx(m_modecomm);	
	}
	// 命令计时开始,return:true-overtime;false-normal
	int t_overtimeflag=trmcmdovertimedetect();
	// 判断脚本行类型
	if(m_commentflag)
	{
		int t_result;
		// 注释行类型
		if(t_overtimeflag==TRUE)
		{
			// 超时判ng，下一条
			t_result=FALSE;
			m_matchnormal=1;
		}
		else
		{
			// normal
			t_result=trmparsecomentlinecommand();
		}
		if(m_matchnormal==1)
		{
			CString t_csedit,t_csreault;
			if(t_result==TRUE)
			{
				t_csreault=_T("(OK)");
			}
			else if(t_result==FALSE)
			{
				if(m_ngmon)
				{
					t_csreault=_T("(NG)");
					// 出现错误
					trmfaultcounter();
				}
				else
				{
					t_csreault=_T("(off)");
				}
				// 写入nginformation
				CString t_nginfo;
				t_nginfo.Format(_T("%d"),m_lineno);
				t_nginfo=_T("[line:")+t_nginfo+t_csreault+_T("]")+getsystemdatetime(0)+_T(" comment[")+m_linestring+_T("]");
				trmsetnginformation(t_nginfo);
			}
			t_csedit.Format(_T("%d"),m_lineno);
			t_csedit=_T("\r\n[line:")+t_csedit+t_csreault+_T("]")+getsystemdatetime(0)+_T(" comment[")+m_linestring+_T("]\r\n");
			trmsendtrmdlgcstring(t_csedit);
		}
	}
	else
	{
		if(t_overtimeflag==TRUE)
		{
			// 超时，下发
			m_matchstrict=1;
		}
		// 下发脚本类型
		if(m_matchstrict==1)
		{
			// 下发脚本 
			m_cstxbuff=m_linestring;
			trmmodecommtx(m_modecomm);	
			m_csrxbuff.Empty();
			m_matchstrict=0;
			m_matchnormal=1;
		}
		else
		{
			// 终端提示符匹配判断
			if(m_csrxbuff.Find(m_ptrmmode->prompt)!=-1)
			{
				// 匹配终端提示符
				m_matchstrict=0;
				m_matchnormal=1;
				// 下发脚本 
				m_cstxbuff=m_linestring;
				trmmodecommtx(m_modecomm);	
				m_csrxbuff.Empty();
			}
			else
			{
				// 未能匹配终端提示符
				m_matchstrict=0;
				m_matchnormal=0;
			}
		}
	}
	Sleep(TRMDELAY+m_ptrmmode->delaytime);
	// gpib命令查询方式需拿取rx buffer
	if((m_commentflag==FALSE)&&(m_modecomm==mode_gpib)&&(m_linestring.Find(_T('?'))!=-1)&&(m_linestring.GetLength()>0))
	{
		// read rx buff
		trmmodecommrx(m_modecomm);	
	}
	this->PostThreadMessage(WM_trmthreadrun,0,0);
	return 0;
}

// 获取时间 v_type:0-format;1-dir used
CString  ctrmthread::getsystemdatetime(int v_type)
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

// replace "\\n" to 0x0a'\n'
// replace "\\r" to 0x0d'\r'
int	ctrmthread::trmparsereplacechar(CString *v_pcs)
{
	v_pcs->Replace(_T("\\n"),_T("\n"));
	v_pcs->Replace(_T("\\n"),_T("\n"));
	return 0;
}

// 命令计时开始
int ctrmthread::trmcmdtimer()
{
	m_ptrmdlg->SendMessage(WM_trmdlgovertime,(WPARAM)m_pitemdata,NULL);
	m_cmdbegintime=m_pitemdata->overtime;
	return 0;
}

// 命令计时开始,return:true-overtime;false-normal
int ctrmthread::trmcmdovertimedetect()
{
	m_ptrmdlg->SendMessage(WM_trmdlgovertime,(WPARAM)m_pitemdata,NULL);
	if((m_pitemdata->overtime-m_cmdbegintime)>OVERTIMR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 保留数字字符
int ctrmthread::trmcutnonnumber(CString *v_cs)
{
	CString t_cs=*v_cs;
	int t_i=0;
	for(;t_cs[t_i]<_T('0')||t_cs[t_i]>_T('9');t_i++)
	{
		if(t_i==t_cs.GetLength())
		{
			break;
		}
	}
	if(t_i==0)
	{
	}
	else
	{
		if(t_cs[t_i-1]==_T('-'))
		{
			t_i-=1;
		}
	}
	t_cs=t_cs.Right(t_cs.GetLength()-t_i);
	if(t_cs.GetLength()>0)
	{
		if(t_cs[0]==_T('-'))
		{
			t_i=1;
		}
		else
		{
			t_i=0;
		}
		for(;!(t_cs[t_i]<_T('0')||t_cs[t_i]>_T('9'));t_i++)
		{
			if(t_i==t_cs.GetLength())
			{
				break;
			}
		}
	}
	*v_cs=t_cs.Left(t_i);
	return 0;
}

// 分析注释行
//	m_para[0]命令
//	m_para[1~9]参数
int ctrmthread::trmparsecomentline(CString v_linestring)
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
		// replace "\\n" to 0x0a'\n',"\\r" to 0x0d'\r'
		trmparsereplacechar(&(m_para[t_i]));
	}
	m_para[0]=m_para[0].Right(m_para[0].GetLength()-m_ptrmmode->comment.GetLength()-m_para[0].Find(m_ptrmmode->comment));
	return FALSE;
}

// 分析注释行命令
//	亲爱的维护者：
//	如果你尝试了对这段程序进行‘优化’，
//	并认识到这种企图是大错特错，请增加
//	下面这个计数器，用来对后来人进行警告：
//	浪费在这里的总时间=1小时
int ctrmthread::trmparsecomentlinecommand()
{
	int t_i;
	int t_return=0;
	for(t_i=0;t_i<COMMANDPRIMITIVESQUANTITY;t_i++)
	{
		if(m_para[0].CompareNoCase(m_commandfunctionarray[t_i].primitives)==0)
		{
			// 调用函数成员
			t_return=((this)->*(m_commandfunctionarray[t_i].f))();
			break;
		}
		else if(t_i==(COMMANDPRIMITIVESQUANTITY-1))
		{
			// 未能解释的命令,skip
			t_return=TRUE;
			m_matchnormal=1;
		}
	}
	return t_return;
}

// read script file one line
//	TRUE good
//	FALSE end
int ctrmthread::trmreadscriptfileline()
{
	if(m_csfscriptfile.ReadString(m_linestring)!=NULL)
	{
		m_lineno+=1;
		m_pitemdata->lineno=m_lineno;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// rxbuff ok
int ctrmthread::trmrxbufferok()
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

// 判断脚本行类型
//	TRUE 软件命令 
//	FALSE 下发脚本
int ctrmthread::trmscriptlineiscomment(CString *v_pcsline)
{
	// 替换字符串中变量符号,"$_"开头
	trmtxbuffgetstring4replacelabel(v_pcsline);
	if(v_pcsline->Find(m_ptrmmode->comment)!=0)
	{
		//	FALSE 下发脚本
		if(m_matchstrict==1)
		{
			m_matchnormal=1;
		}
		else
		{
			m_matchnormal=0;
		}
		return FALSE;
	}
	else
	{
		//	TRUE 软件命令 
		m_matchnormal=0;
		return TRUE;
	}
}

int ctrmthread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

// send cstring to trmdlg edit ctrl
int ctrmthread::trmsendtrmdlgcstring(CString v_str)
{
	m_csflogfile.WriteString(v_str);
	m_csflogfile.Flush();
	CString t_cs=v_str;
	m_ptrmdlg->SendMessage(WM_trmdlgeditaddcstring,(WPARAM)t_cs.GetBuffer(0));
	t_cs.ReleaseBuffer();
	return FALSE;
}

// send char to trmdlg edit ctrl
int ctrmthread::trmsendtrmdlgchar(wchar_t *v_wch)
{
	CString t_strtemp;
	wchar_t *t_pChar =v_wch;
	t_strtemp=CString(t_pChar);
	trmsendtrmdlgcstring(t_strtemp);
	return 0;
}

BEGIN_MESSAGE_MAP(ctrmthread, CWinThread)
END_MESSAGE_MAP()

// ctrmthread 消息处理程序

int ctrmthread::Run()
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	//	BOOL t_bRet; 
	MSG t_msg;
	while((/*t_bRet=*/GetMessage(&t_msg,NULL,0,0))!=0)
	{
		switch(t_msg.message)
		{
		case WM_trmthreadinitreturn: 	
			Ontrmthreadinitreturn();
			break;
		case WM_trmthreadautologin:
			Ontrmthreadautologin(t_msg.wParam);
			break;
		case WM_trmthreadrun:
			Ontrmthreadrun();
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
	// 线程结束,清理,通知trmdlg
	trmmodecommexit(m_modecomm);
	m_pitemdata=NULL;
	m_ptrmmode=NULL;
	m_ptrmdlg->SendMessage(WM_CLOSE,1,0);
	m_ptrmdlg=NULL;
	return CWinThread::Run();
}
