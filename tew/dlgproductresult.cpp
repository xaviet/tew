// dlgproductresult.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "dlgproductresult.h"
#include "afxdialogex.h"
#include "tewdlg.h"

// cdlgproductresult 对话框

IMPLEMENT_DYNAMIC(cdlgproductresult, CDialogEx)

cdlgproductresult::cdlgproductresult(CWnd* pParent /*=NULL*/)
	: CDialogEx(cdlgproductresult::IDD, pParent)
	, m_ptewdlg(0)
	, m_iitemcounter(0)
{

}

cdlgproductresult::~cdlgproductresult()
{
}

void cdlgproductresult::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_listproductresult, m_listproductresult);
}


BEGIN_MESSAGE_MAP(cdlgproductresult, CDialogEx)
END_MESSAGE_MAP()

// init list
int cdlgproductresult::productresultinitlist()
{
	CRect t_rect;   
	// 获取列表视图控件的位置和大小   
	m_listproductresult.GetClientRect(&t_rect); 
	// 为列表视图控件添加全行选中和栅格风格   
	m_listproductresult.SetExtendedStyle((m_listproductresult.GetExtendedStyle())|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);  
	// 为列表视图控件添加3列,首项默认左对齐不可修改,因此不配置0列,应用仍按此顺序   
	m_listproductresult.InsertColumn(1,_T("item"),LVCFMT_LEFT,t_rect.Width()*2/5); 
	m_listproductresult.InsertColumn(2,_T("value"),LVCFMT_LEFT,t_rect.Width()*3/5); 
	// 赋初始值
	map<CString,CString>::iterator t_it;
	CString t_cs;
    for(t_it=((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage).begin();t_it!=((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage).end();++t_it)
	{
			m_listproductresult.InsertItem(m_iitemcounter,t_it->first); 
			if(t_it->second.GetLength()==0)
			{
				t_cs=_T("[N/A]");
			}
			else
			{
				t_cs=t_it->second;
			}
			m_listproductresult.SetItemText(m_iitemcounter,1,t_cs);
			m_iitemcounter++;
	}
	for(t_it=((static_cast<CtewDlg*>(m_ptewdlg))->m_mapmodulefield).begin();t_it!=((static_cast<CtewDlg*>(m_ptewdlg))->m_mapmodulefield).end();++t_it)
	{
			m_listproductresult.InsertItem(m_iitemcounter,t_it->first);  
			if(t_it->second==_T("0"))
			{
				t_cs=_T("OK");
			}
			else
			{
				t_cs=_T("NG[")+t_it->second+_T("]");
			}
			m_listproductresult.SetItemText(m_iitemcounter,1,t_cs);
			m_iitemcounter++;
	}
	return 0;
}

// cdlgproductresult 消息处理程序


BOOL cdlgproductresult::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ÔÚ´ËÌí¼Ó¶îÍâµÄ³õÊ¼»¯
	assert(m_ptewdlg);
	// init list
	productresultinitlist();
	return TRUE;  // return TRUE unless you set the focus to a control
	// Òì³£: OCX ÊôÐÔÒ³Ó¦·µ»Ø FALSE
}
