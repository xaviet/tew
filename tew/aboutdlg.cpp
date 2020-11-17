// aboutdlg.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "aboutdlg.h"
#include "afxdialogex.h"
#include "config.h"

#pragma comment(lib,"version.lib")

// aboutdlg 对话框

IMPLEMENT_DYNAMIC(caboutdlg, CDialogEx)

caboutdlg::caboutdlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(caboutdlg::IDD, pParent)
	, m_pos(0)
{

}

caboutdlg::~caboutdlg()
{
}

void caboutdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_dlgabouteditversion, m_editversion);
	DDX_Control(pDX, IDC_editthanks, m_thanks);
}


BEGIN_MESSAGE_MAP(caboutdlg, CDialogEx)
		ON_WM_TIMER()
END_MESSAGE_MAP()


// aboutdlg 消息处理程序


BOOL caboutdlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ÔÚ´ËÌí¼Ó¶îÍâµÄ³õÊ¼»¯
	m_editversion.SetWindowText(m_csversion);
	m_csthks=THANKS;
	// 默认回调函数OnTimer
	SetTimer(DLGTHANKSTIMER,150,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// Òì³£: OCX ÊôÐÔÒ³Ó¦·µ»Ø FALSE
}

void caboutdlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
	// 实现thanks内容左移
	m_pos++;
	if(m_pos>m_csthks.GetLength())
	{
		m_pos=0;
	}
	CString t_cs;
	t_cs=m_csthks+THANKS;
	m_thanks.SetWindowText((t_cs.Right(t_cs.GetLength()-m_pos)).Left(29));
	m_thanks.UpdateData(FALSE);
}