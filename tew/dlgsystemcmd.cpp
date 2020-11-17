// dlgsystemcmd.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "dlgsystemcmd.h"
#include "afxdialogex.h"
#include "trmthread.h"

// cdlgsystemcmd 对话框

IMPLEMENT_DYNAMIC(cdlgsystemcmd, CDialogEx)

cdlgsystemcmd::cdlgsystemcmd(CWnd* pParent /*=NULL*/)
	: CDialogEx(cdlgsystemcmd::IDD, pParent)
	, m_retutn(TRUE)
	, m_lBytesRead(0)
	, m_paramno(0)
	, m_timer(0)
	, m_ptrmthread(0)
	, m_hInReadPipe(0)
	, m_hInWritePipe(0)
	, m_hOutReadPipe(0)
	, m_hOutWritePipe(0)
{

}

cdlgsystemcmd::~cdlgsystemcmd()
{
}

void cdlgsystemcmd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_editsystemcmdmsg, m_editsyscmdmsg);
}


BEGIN_MESSAGE_MAP(cdlgsystemcmd, CDialogEx)
	ON_WM_TIMER()
	ON_MESSAGE(WM_dlgsystemcmdeditaddcstring,&cdlgsystemcmd::Ondlgsystemcmdeditaddcstring)
END_MESSAGE_MAP()

// 初始化匿名管道
int cdlgsystemcmd::systemcmdinitpipe()
{
	m_retutn=TRUE;
	m_sa.nLength=sizeof(SECURITY_ATTRIBUTES);
	m_sa.lpSecurityDescriptor=NULL;
	m_sa.bInheritHandle=true;
	// 创建输入管道
	if(!CreatePipe(&m_hInReadPipe,&m_hInWritePipe,&m_sa,0))
	{
		AfxMessageBox(_T("Error on Create input Pipe()!"));
		m_retutn=FALSE;
	}
	// 创建输出管道
	if(!CreatePipe(&m_hOutReadPipe,&m_hOutWritePipe,&m_sa,0))
	{
		AfxMessageBox(_T("Error on Create output Pipe()!"));
		m_retutn=FALSE;
	}
	ZeroMemory(&m_si,sizeof(m_si));
	m_si.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
	m_si.wShowWindow=SW_HIDE;
	m_si.hStdInput=m_hInReadPipe;
	m_si.hStdOutput=m_hOutWritePipe;
	m_si.hStdError=m_hOutWritePipe;
	// 将匿名管道和cmd的输入输出关联
	if(!CreateProcess(NULL,m_para[1].GetBuffer(0),NULL,NULL,TRUE,NULL,NULL,NULL,&m_si,&m_ProcessInformation))
	{
		AfxMessageBox(_T("匿名管道和cmd的输入输出关联错误"));
		m_retutn=FALSE;
	}
	m_para[1].ReleaseBuffer();
	m_paramno=2;
	m_csoutputbuff.Empty();
	m_csoutputbuff=m_para[1]+_T("\r\n");
	this->SendMessage(WM_dlgsystemcmdeditaddcstring,(WPARAM)m_csoutputbuff.GetBuffer(0),NULL);
	m_csoutputbuff.ReleaseBuffer();
	return m_retutn;
}

// cdlgsystemcmd 消息处理程序


BOOL cdlgsystemcmd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ÔÚ´ËÌí¼Ó¶îÍâµÄ³õÊ¼»¯
	// SetWindowText:设置标题
	SetWindowText(_T("cmd---")+m_para[1]);
	if(systemcmdinitpipe()==FALSE)
	{
		// fail
		this->OnCancel();
	}
	// 默认回调函数OnTimer
	SetTimer(DLGSYSTEMCMDTIMER,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// Òì³£: OCX ÊôÐÔÒ³Ó¦·µ»Ø FALSE
}

// system cmd dlg cedit add cstring
afx_msg LRESULT cdlgsystemcmd::Ondlgsystemcmdeditaddcstring(WPARAM wParam, LPARAM lParam)
{
	// 显示最新,不闪烁
	int t_j=m_editsyscmdmsg.GetWindowTextLength();
	m_editsyscmdmsg.SetSel(t_j,t_j);
	m_editsyscmdmsg.ReplaceSel((LPCTSTR)wParam);
	m_editsyscmdmsg.ScrollWindow(0,0);
	return 0;
}

void cdlgsystemcmd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
	m_timer++;
	// 设置超时时间
	if(m_timer==DLGOVERTIME)
	{
		// fail
		this->OnCancel();
	}
	char t_msg[BUFFERSIZE]={0};
	// 输出管道是否有数据可读
	m_retutn=PeekNamedPipe(m_hOutReadPipe,t_msg,BUFFERSIZE,&m_lBytesRead,0,0);
	if(m_lBytesRead>0)
	{
		// 读取管道里的数据
		m_retutn=ReadFile(m_hOutReadPipe,t_msg,m_lBytesRead,&m_lBytesRead,0);
		if(!m_retutn)
		{
			// fail
			this->OnCancel();
		}
		m_csoutputbuff.Empty();
		m_csoutputbuff=CString(t_msg);
		this->SendMessage(WM_dlgsystemcmdeditaddcstring,(WPARAM)m_csoutputbuff.GetBuffer(0),NULL);
		m_csoutputbuff.ReleaseBuffer();
	}
	if(m_paramno<COMMENTPARSEPARA)
	{
		m_para[m_paramno]+=_T("\n");
		// back display
		m_csoutputbuff=m_para[m_paramno];
		this->SendMessage(WM_dlgsystemcmdeditaddcstring,(WPARAM)m_csoutputbuff.GetBuffer(0),NULL);
		WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)m_para[m_paramno],-1,t_msg,BUFFERSIZE,0,false); 
		m_lBytesRead=strlen(t_msg);
		WriteFile(m_hInWritePipe,t_msg,m_lBytesRead,&m_lBytesRead,0);
	}
	DWORD t_dwMark;
	GetExitCodeProcess(m_ProcessInformation.hProcess,&t_dwMark);
	if(STILL_ACTIVE==t_dwMark)  
	{
		// run
		m_paramno++;
	}
	else 
	{
		// exit
		this->OnOK();
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL cdlgsystemcmd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	if(message==WM_DESTROY)
	{
		// 终端退出,清理(例如退出配置模式)
		// get ctrl
		UpdateData(TRUE);
		CString t_cs;
		m_editsyscmdmsg.GetWindowText(t_cs);
		(reinterpret_cast<ctrmthread*>(this->m_ptrmthread))->m_csrxbuff=t_cs;
		//	close system cmd process
		WaitForSingleObject(m_ProcessInformation.hProcess,1000);
		TerminateProcess(m_ProcessInformation.hProcess,NULL);
		CloseHandle(m_hInReadPipe);
		CloseHandle(m_hInWritePipe);
		CloseHandle(m_hOutReadPipe);
		CloseHandle(m_hOutWritePipe);
	}
	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}
