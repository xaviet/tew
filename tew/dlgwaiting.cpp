// dlgwaiting.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "dlgwaiting.h"
#include "afxdialogex.h"

// cdlgwaiting 对话框

IMPLEMENT_DYNAMIC(cdlgwaiting, CDialogEx)

cdlgwaiting::cdlgwaiting(CWnd* pParent /*=NULL*/)
	: CDialogEx(cdlgwaiting::IDD, pParent)
	, m_escapetime(0)
	, m_isecond(0)
{

}

cdlgwaiting::~cdlgwaiting()
{
}

void cdlgwaiting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_dlgwaitingmsg, m_dlgwaitingmsg);
	DDX_Control(pDX, IDC_dlgwaittingtime, m_DLGWAITINGTIMER);
}

BEGIN_MESSAGE_MAP(cdlgwaiting, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// cdlgwaiting 消息处理程序

BOOL cdlgwaiting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ÔÚ´ËÌí¼Ó¶îÍâµÄ³õÊ¼»¯
	// message
	m_dlgwaitingmsg.SetWindowText(m_csmsg);
	// time
	m_DLGWAITINGTIMER.SetWindowText(m_cssecond);
	// set ctrl
	UpdateData(FALSE);
	m_isecond=_ttoi(m_cssecond);
	assert(m_isecond>=0);
	// 默认回调函数OnTimer
	SetTimer(DLGWAITINGTIMER,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// Òì³£: OCX ÊôÐÔÒ³Ó¦·µ»Ø FALSE
}

// Timer回调函数
void cdlgwaiting::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
	if(m_isecond==1)
	{
		this->OnOK();
	}
	else if(m_isecond>1)
	{
		m_isecond--;
		m_cssecond.Format(_T("%d"),m_isecond);
	}
	else if(m_isecond==0)
	{
		m_escapetime++;
		m_cssecond.Format(_T("%d"),m_escapetime);
	}
	// time
	m_DLGWAITINGTIMER.SetWindowText(m_cssecond);
	// set ctrl
	UpdateData(FALSE);
	CDialogEx::OnTimer(nIDEvent);
}
