// dlgprocessing.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "dlgprocessing.h"
#include "afxdialogex.h"


// cdlgprocessing 对话框

IMPLEMENT_DYNAMIC(cdlgprocessing, CDialogEx)

cdlgprocessing::cdlgprocessing(CWnd* pParent /*=NULL*/)
	: CDialogEx(cdlgprocessing::IDD, pParent)
	, m_flag(0)
	, m_endflag(NULL)
{

}

cdlgprocessing::~cdlgprocessing()
{
}

void cdlgprocessing::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_dlgprocessingeditor, m_edit);
}


BEGIN_MESSAGE_MAP(cdlgprocessing, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// cdlgprocessing 消息处理程序


void cdlgprocessing::OnOK()
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	//CDialogEx::OnOK();
}


BOOL cdlgprocessing::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ÔÚ´ËÌí¼Ó¶îÍâµÄ³õÊ¼»¯
	m_edit.SetWindowText(_T("processing"));
	// set ctrl
	UpdateData(FALSE);
	// 默认回调函数OnTimer
	SetTimer(DLGPROCESSINGTIMER,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// Òì³£: OCX ÊôÐÔÒ³Ó¦·µ»Ø FALSE
}


void cdlgprocessing::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
	if((*m_flag)==m_endflag)
	{
		// processing end
		this->SendMessage(WM_CLOSE,0,0);
	}
	else
	{
		// set ctrl
		UpdateData(TRUE);
		m_edit.GetWindowText(m_csedit);
		if(m_csedit.Find(_T(".........."))!=-1)
		{
			m_edit.SetWindowText(_T("processing"));
		}
		else
		{
			m_edit.SetWindowText(m_csedit+_T("."));
		}
		// set ctrl
		UpdateData(FALSE);
	}
	CDialogEx::OnTimer(nIDEvent);
}
