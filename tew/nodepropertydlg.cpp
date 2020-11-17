// cnodepropertydlg.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "nodepropertydlg.h"
#include "afxdialogex.h"


// cnodepropertydlg 对话框

IMPLEMENT_DYNAMIC(cnodepropertydlg, CDialogEx)

cnodepropertydlg::cnodepropertydlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(cnodepropertydlg::IDD, pParent)
	, m_pscripteditdlg(0)
	, m_pitemdata(0)
	, m_pappdlg(0)
{
}

cnodepropertydlg::~cnodepropertydlg()
{
}

void cnodepropertydlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_editlevel, m_level);
	DDX_Control(pDX, IDC_editpath, m_path);
	DDX_Control(pDX, IDC_editexitflag, m_exitflag);
	DDX_Control(pDX, IDC_editrunstatus, m_runstatus);
	DDX_Control(pDX, IDC_editptrdlg, m_ptrdlg);
	DDX_Control(pDX, IDC_editmessage, m_message);
	DDX_Control(pDX, IDC_checkenableflag, m_enableflag);
	DDX_Control(pDX, IDC_checkrunmtmode, m_runmtmode);
	DDX_Control(pDX, IDC_edittrmtabctrlnumber, m_trmtabctrlnumber);
	DDX_Control(pDX, IDC_editport, m_port);
	DDX_Control(pDX, IDC_edithost, m_host);
	DDX_Control(pDX, IDC_editfilename, m_filename);
	DDX_Control(pDX, IDC_editcomment, m_comment);
	DDX_Control(pDX, IDC_editloopcounter, m_editloopcounter);
	DDX_Control(pDX, IDC_editfileedit, m_editfileedit);
}


BEGIN_MESSAGE_MAP(cnodepropertydlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &cnodepropertydlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_editfileedit, &cnodepropertydlg::OnBnClickededitfileedit)
	ON_MESSAGE(WM_nodeprotertydlgscripteditdlgexit,&cnodepropertydlg::Ondlgscripteditdlgexit)
END_MESSAGE_MAP()



BOOL cnodepropertydlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ÔÚ´ËÌí¼Ó¶îÍâµÄ³õÊ¼»¯
	assert(m_pitemdata);
	assert(m_pitemdata->node);
	// SetWindowText:设置标题
	SetWindowText(_T("node property---")+m_pitemdata->comment);
	CString t_cs;
	// level
	t_cs.Format(_T("%d"),m_pitemdata->level);
	m_level.SetWindowText(t_cs);
	// exitflag
	t_cs.Format(_T("%d"),m_pitemdata->exitflag);
	m_exitflag.SetWindowText(t_cs);
	// trmtabctrlnumber
	t_cs.Format(_T("%d"),m_pitemdata->trmtabctrlnumber);
	m_trmtabctrlnumber.SetWindowText(t_cs);
	// port
	if(m_pitemdata->sshv2)
	{
		t_cs.Format(_T("sshv2:%d"),m_pitemdata->port);
	}
	else if(m_pitemdata->directtcp)
	{
		t_cs.Format(_T("directtcp:%d"),m_pitemdata->port);
	}
	else
	{
		t_cs.Format(_T("%d"),m_pitemdata->port);
	}
	m_port.SetWindowText(t_cs);
	// runstatus
	t_cs.Format(_T("%d"),m_pitemdata->runstatus);
	m_runstatus.SetWindowText(t_cs);
	// loopcounter
	t_cs.Format(_T("%d"),m_pitemdata->loopcounter);
	m_editloopcounter.SetWindowText(t_cs);
	// ptrdlg
	t_cs.Format(_T("%08X"),m_pitemdata->ptrdlg);
	m_ptrdlg.SetWindowText(t_cs);
	// enableflag
	m_enableflag.SetCheck(m_pitemdata->enableflag);
	m_enableflag.EnableWindow(FALSE);
	// runmtmode
	m_runmtmode.SetCheck(m_pitemdata->runmtmode);
	// comment
	m_comment.SetWindowText(m_pitemdata->comment);
	// filename
	m_filename.SetWindowText(m_pitemdata->filename);
	// host
	m_host.SetWindowText(m_pitemdata->host);
	// message
	m_message.SetWindowText(m_pitemdata->message);
	// path
	m_path.SetWindowText(m_pitemdata->path);
	// file exist,not modify m_runmtmode
	if(m_pitemdata->filename.GetLength()!=0)
	{
		// 无子节点的节点禁止编辑path
		m_path.SetReadOnly(TRUE);
	}
	else
	{
		// 有子节点的节点禁止编辑script file
		m_editfileedit.EnableWindow(FALSE);
	}
	// set ctrl
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// Òì³£: OCX ÊôÐÔÒ³Ó¦·µ»Ø FALSE
}

void cnodepropertydlg::OnBnClickedOk()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	if(paramentcheck(0))
	{
		// 询问是否修改
		if(AfxMessageBox(_T("确认修改?"),MB_YESNO)==IDYES)
		{
			// app配置map保存到内存
			paramentcheck(1);
			// 向app dlg发送修改消息
			m_pappdlg->SendMessage(WM_appdlgnodepropertymodify,(WPARAM)m_pitemdata);
		}
	}
	CDialogEx::OnOK();
}

// dlg scripteditdlg exit
afx_msg LRESULT cnodepropertydlg::Ondlgscripteditdlgexit(WPARAM wParam, LPARAM lParam)
{
	if(m_pscripteditdlg!=NULL)
	{
		m_pscripteditdlg=NULL;
	}
	return 0;
}

// 检测修改
int cnodepropertydlg::paramentcheck(int v_i)
{
	int t_i=0;
	CString t_csport,t_csfilename,t_cshost,t_cscomment,t_csmessage,t_cspath;
	// get ctrl
	UpdateData(TRUE);
	m_port.GetWindowText(t_csport);
	m_filename.GetWindowText(t_csfilename);
	m_host.GetWindowText(t_cshost);
	m_comment.GetWindowText(t_cscomment);
	m_message.GetWindowText(t_csmessage);
	m_path.GetWindowText(t_cspath);
	if(v_i)
	{
		// set paraments
		m_pitemdata->enableflag=m_enableflag.GetCheck();
		m_pitemdata->runmtmode=m_runmtmode.GetCheck();
		if(t_csport.Find(SSHV2DEF)!=-1)
		{
			m_pitemdata->sshv2=TRUE;
			m_pitemdata->directtcp=FALSE;
			t_csport.Replace(SSHV2DEF,_T(""));
		}
		else if(t_csport.Find(DIRECTTCPDEF)!=-1)
		{
			m_pitemdata->sshv2=FALSE;
			m_pitemdata->directtcp=TRUE;
			t_csport.Replace(DIRECTTCPDEF,_T(""));
		}
		else
		{
			m_pitemdata->sshv2=FALSE;
			m_pitemdata->directtcp=FALSE;
		}
		m_pitemdata->port=_ttoi(t_csport);
		m_pitemdata->path=t_cspath;
		m_pitemdata->filename=t_csfilename;
		m_pitemdata->host=t_cshost;
		m_pitemdata->comment=t_cscomment;
		m_pitemdata->message=t_csmessage;
	}
	else
	{
		//check paraments
		int sshv2flag=0;
		int directtcpflag=0;
		if(t_csport.Find(SSHV2DEF)!=-1)
		{
			sshv2flag=TRUE;
			directtcpflag=FALSE;
		}
		else if(t_csport.Find(DIRECTTCPDEF)!=-1)
		{
			sshv2flag=FALSE;
			directtcpflag=TRUE;
		}
		else
		{
			sshv2flag=FALSE;
			directtcpflag=FALSE;
		}
		if((m_pitemdata->enableflag!=m_enableflag.GetCheck())\
			||(m_pitemdata->runmtmode!=m_runmtmode.GetCheck())\
			||(m_pitemdata->port!=_ttoi(t_csport))\
			||(m_pitemdata->filename!=t_csfilename)\
			||(m_pitemdata->host!=t_cshost)\
			||(m_pitemdata->comment!=t_cscomment)\
			||(m_pitemdata->message!=t_csmessage)\
			||(m_pitemdata->path!=t_cspath)\
			||(m_pitemdata->sshv2!=sshv2flag)\
			||(m_pitemdata->directtcp!=directtcpflag)\
			)
		{
			t_i=1;
			if(m_enableflag.GetCheck()==0)
			{
				m_pitemdata->runstatus=icondisable;
			}
			else
			{
				m_pitemdata->runstatus=iconready;
			}
		}
	}
	return t_i;
}

void cnodepropertydlg::OnBnClickededitfileedit()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë 
	// scripteditdlg是否存在
	if(m_pscripteditdlg==NULL)
	{
		// 初始化脚本编辑dlg
		m_pscripteditdlg=new cscripteditdlg;
		m_pscripteditdlg->m_filename=m_pitemdata->path+m_pitemdata->filename;
		m_pscripteditdlg->m_pnodepropertydlg=this;
		// 创建一个非模态对话框 
		m_pscripteditdlg->Create(IDD_scripteditdlg); 
		// 显示非模态对话框
		m_pscripteditdlg->ShowWindow(SW_SHOWNORMAL); 
	}
}
