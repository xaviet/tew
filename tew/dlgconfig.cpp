// dlconfig.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "dlgconfig.h"
#include "afxdialogex.h"

// cdlgconfig 对话框

IMPLEMENT_DYNAMIC(cdlgconfig, CDialogEx)

cdlgconfig::cdlgconfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(cdlgconfig::IDD, pParent)
	, m_pappdlg(0)
	, m_pconfigflag(0)
{

}

cdlgconfig::~cdlgconfig()
{
}

void cdlgconfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_checkloop, m_checkloop);
	DDX_Control(pDX, IDC_checkngstop, m_checkngstop);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_zlogdir);
}


BEGIN_MESSAGE_MAP(cdlgconfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &cdlgconfig::OnBnClickedOk)
END_MESSAGE_MAP()


// cdlgconfig 消息处理程序


BOOL cdlgconfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ÔÚ´ËÌí¼Ó¶îÍâµÄ³õÊ¼»¯
	assert(m_pappdlg);
	assert(m_pconfigflag);
	m_checkloop.SetCheck(m_pconfigflag->loop);
	m_checkngstop.SetCheck(m_pconfigflag->ngstop);
	m_zlogdir.SetWindowText(m_pconfigflag->zlogdir);
	// set ctrl
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// Òì³£: OCX ÊôÐÔÒ³Ó¦·µ»Ø FALSE
}


void cdlgconfig::OnBnClickedOk()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	// 保存config
	// get ctrl
	UpdateData(TRUE);
	m_pconfigflag->loop=m_checkloop.GetCheck()==1;
	m_pconfigflag->ngstop=m_checkngstop.GetCheck()==1;

	m_zlogdir.GetWindowText(m_pconfigflag->zlogdir);
	if(m_pconfigflag->zlogdir.GetLength()!=0)
		if(m_pconfigflag->zlogdir.Right(1)!=_T("\\") && m_pconfigflag->zlogdir.Right(1)!=_T("//"))
			m_pconfigflag->zlogdir.Append(_T("\\"));
	// 向app dlg发送修改消息
	m_pappdlg->SendMessage(WM_appdlgconfig,(WPARAM)m_pconfigflag,NULL);
	CDialogEx::OnOK();
}

