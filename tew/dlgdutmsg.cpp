// dlgdutmsg.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "dlgdutmsg.h"
#include "afxdialogex.h"
#include "tewdlg.h"

// cdlgdutmsg 对话框

IMPLEMENT_DYNAMIC(cdlgdutmsg, CDialogEx)

cdlgdutmsg::cdlgdutmsg(CWnd* pParent /*=NULL*/)
	: CDialogEx(cdlgdutmsg::IDD, pParent)
	, m_ptewdlg(0)
	, m_iitemcounter(0)
	, m_nItem(0)
	, m_nSubItem(0)
	, m_bEditVisible(true)
{

}

cdlgdutmsg::~cdlgdutmsg()
{
}

void cdlgdutmsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_listdutmsg, m_listdutmsg);
	DDX_Control(pDX, IDC_dlgdutmsgedit, m_editmsg);
}


BEGIN_MESSAGE_MAP(cdlgdutmsg, CDialogEx)
	ON_EN_KILLFOCUS(IDC_dlgdutmsgedit, &cdlgdutmsg::OnEnKillfocusdlgdutmsgedit)
	ON_MESSAGE(WM_dlgdutmsgeditend,&cdlgdutmsg::Ondlgdutmsgeditend)
	ON_NOTIFY(NM_DBLCLK, IDC_listdutmsg, &cdlgdutmsg::OnNMDblclklistdutmsg)
	ON_NOTIFY(HDN_BEGINTRACK, 0, &cdlgdutmsg::OnHdnBegintracklistdutmsg)
END_MESSAGE_MAP()

// init list
int cdlgdutmsg::dutmsginitlist()
{
	CRect t_rect;   
	// 获取列表视图控件的位置和大小   
	m_listdutmsg.GetClientRect(&t_rect); 
	// 为列表视图控件添加全行选中和栅格风格   
	m_listdutmsg.SetExtendedStyle((m_listdutmsg.GetExtendedStyle())|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);  
	// 为列表视图控件添加3列,首项默认左对齐不可修改,因此不配置0列,应用仍按此顺序   
	m_listdutmsg.InsertColumn(1,_T("item"),LVCFMT_LEFT,t_rect.Width()*2/5); 
	m_listdutmsg.InsertColumn(2,_T("value"),LVCFMT_LEFT,t_rect.Width()*3/5); 
	// 赋初始值
	map<CString,CString>::iterator t_it;
    for(t_it=(static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage.begin();t_it!=((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage).end();++t_it)
	{
			m_listdutmsg.InsertItem(m_iitemcounter,t_it->first);  
			m_listdutmsg.SetItemText(m_iitemcounter,1,t_it->second);
			m_iitemcounter++;
	}
	UpdateData(FALSE);
	return 0;
}

// show edit
void cdlgdutmsg::dutmsgShowEdit(bool v_bShow,int v_nItem,int v_nSubItem,CRect v_rcCtrl)  
{  
	m_bEditVisible=v_bShow;  
	if (m_editmsg.m_hWnd==NULL)  
	{  
		m_editmsg.Create(ES_AUTOHSCROLL|WS_CHILD|ES_LEFT|ES_WANTRETURN|WS_BORDER,CRect(0,0,0,0),this,IDC_dlgdutmsgedit);  
		m_editmsg.ShowWindow(SW_HIDE);  
		// dlg已指定字体，此处font设置好似无效
		//CFont tpFont;  
		//tpFont.CreateStockObject(OEM_FIXED_FONT);  
		//m_editmsg.SetFont(&tpFont);  
		//tpFont.DeleteObject();  
	}  
	if (v_bShow==TRUE)  
	{  
		CString t_cstrItem=m_listdutmsg.GetItemText(m_nItem,m_nSubItem);  
		m_editmsg.MoveWindow(&v_rcCtrl);  
		m_editmsg.ShowWindow(SW_SHOW);  
		m_editmsg.SetWindowText(t_cstrItem);  
		m_editmsg.SetFocus();  
		m_editmsg.SetSel(0,-1);   
	}  
	else 
	{
		m_editmsg.ShowWindow(SW_HIDE);  
	}  
}

// dut msg check
// 晕了,以后再改
CString cdlgdutmsg::dutmsgcheck()
{
	CString t_cs;
	int t_i=0;
	t_cs.Empty();
	// 遍历map,check role
	map<CString,CString>::iterator t_it;
    for(t_it=((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage).begin();t_it!=((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage).end();++t_it)
	{
		if(t_it->first.Find(_T("ProductSN"))!=-1)
		{
/*格式不稳定,关闭校验			
			if(t_it->second.GetLength()>0)
			{
				if((t_it->second[0]==_T('A'))&&(t_it->second.GetLength()==12))
				{
					for(t_i=1;t_i<12;t_i++)
					{
						if((t_it->second[t_i]<_T('0'))||(t_it->second[t_i]>_T('9')))
						{
							t_cs+=_T("[")+t_it->first+_T("]");
						}
					}
				}
				else
				{
					t_cs+=_T("[")+t_it->first+_T("]");
				}
			}
*/		}
		else if(t_it->first.Find(_T("SN"))!=-1)
		{
/*格式不稳定,关闭校验			
			if(t_it->second.GetLength()>0)
			{
				if((t_it->second[0]==_T('A'))&&(t_it->second.GetLength()==12))
				{
					for(t_i=1;t_i<12;t_i++)
					{
						if((t_it->second[t_i]<_T('0'))||(t_it->second[t_i]>_T('9')))
						{
							t_cs+=_T("[")+t_it->first+_T("]");
						}
					}
				}
				else
				{
					t_cs+=_T("[")+t_it->first+_T("]");
				}
			}
*/		}
		else if(t_it->first.Find(_T("PN"))!=-1)
		{
/*格式不稳定,关闭校验			
			if(t_it->second.GetLength()>0)
			{
				if((t_it->second.GetLength()==10))
				{
					// 需求是去掉PN字段中的'.'
//					if((t_it->second[3]==_T('.'))&&(t_it->second[9]==_T('.')))
//					{
						for(t_i=0;t_i<10;t_i++)
						{
//							if(!((t_i==3)||(t_i==9)))
//							{
								if((t_it->second[t_i]<_T('0'))||(t_it->second[t_i]>_T('9')))
								{
									t_cs+=_T("[")+t_it->first+_T("]");
								}
//							}
						}
//					}
//					else
//					{
//						t_cs+=_T("[")+t_it->first+_T("]");
//					}
				}
				else
				{
					t_cs+=_T("[")+t_it->first+_T("]");
				}
			}
*/		}
		else if(t_it->first.Find(_T("EthMac"))!=-1)
		{
			if(t_it->second.GetLength()>0)
			{
				if(t_it->second.GetLength()==17)
				{
					if((t_it->second[2]==_T(':'))&&(t_it->second[5]==_T(':'))&&(t_it->second[8]==_T(':'))&&(t_it->second[11]==_T(':'))&&(t_it->second[14]==_T(':')))
					{
						for(t_i=0;t_i<17;t_i++)
						{
							if(!((t_i==2)||(t_i==5)||(t_i==8)||(t_i==11)||(t_i==14)))
							{
								if(!((t_it->second[t_i]>=_T('0'))&&(t_it->second[t_i]<=_T('9'))||(t_it->second[t_i]>=_T('A'))&&(t_it->second[t_i]<=_T('F'))))
								{
									t_cs+=_T("[")+t_it->first+_T("]");
								}
							}
						}
					}
					else
					{
						t_cs+=_T("[")+t_it->first+_T("]");
					}
				}
				else
				{
					t_cs+=_T("[")+t_it->first+_T("]");
				}
			}
		}
		else if((t_it->first.Find(_T("UBoot"))!=-1)||(t_it->first.Find(_T("RootFS"))!=-1))
		{
			if(t_it->second.GetLength()>0)
			{
				if(t_it->second.GetLength()==10)
				{
					if((t_it->second[4]==_T('-'))&&(t_it->second[7]==_T('-')))
					{
					}
					else
					{
						t_cs+=_T("[")+t_it->first+_T("]");
					}
				}
				else
				{
					t_cs+=_T("[")+t_it->first+_T("]");
				}
			}
		}
	}
	return t_cs;
}

// cdlgdutmsg 消息处理程序

BOOL cdlgdutmsg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ÔÚ´ËÌí¼Ó¶îÍâµÄ³õÊ¼»¯
	assert(m_ptewdlg);
	// init list
	dutmsginitlist();
	return TRUE;  // return TRUE unless you set the focus to a control
	// Òì³£: OCX ÊôÐÔÒ³Ó¦·µ»Ø FALSE
}


void cdlgdutmsg::OnOK()
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	UpdateData(TRUE);
	int t_i=0;
	// get dut msg map
	for(;t_i<m_iitemcounter;t_i++)
	{
		((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage)[m_listdutmsg.GetItemText(t_i,0)]=m_listdutmsg.GetItemText(t_i,1);
	}
	// dut msg check
	CString t_cs=dutmsgcheck();
	if(t_cs.GetLength()!=0)
	{
		// check ng
		t_cs=_T("error\n")+t_cs;
		AfxMessageBox(t_cs);
	}
	else
	{
/*		// update dutmsg
		map<CString,CString>::iterator t_it;
		for(t_it=(static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage.begin();t_it!=((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage).end();++t_it)
		{
			m_listdutmsg.InsertItem(m_iitemcounter,t_it->first);  
			m_listdutmsg.SetItemText(m_iitemcounter,1,t_it->second);
			m_iitemcounter++;
		}
		UpdateData(FALSE);
		Sleep(1000);
*/		// check ok
		CDialogEx::OnOK();
	}
}


void cdlgdutmsg::OnEnKillfocusdlgdutmsgedit()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	this->SendMessage(WM_dlgdutmsgeditend,0,0);  
}

// edit end
afx_msg LRESULT cdlgdutmsg::Ondlgdutmsgeditend(WPARAM wParam, LPARAM lParam)
{
	CString t_strText;  
	CString t_cs; 
	m_editmsg.GetWindowText(t_strText); 
	t_cs=m_listdutmsg.GetItemText(m_nItem,0);
	if(t_cs.Find(_T("EthMac"))!=-1)
	{
		// mac地址中的_T('-') replace to _T(':')
		t_strText.Replace(_T('-'),_T(':'));
	}
	else if(t_cs.Find(_T("PN"))!=-1)
	{
		// PN中的_T('.') replace to _T('')
		t_strText.Replace(_T("."),_T(""));
	}
	else if((t_strText.Find(_T("1P"))!=-1)&&(t_strText.Find(_T("2P"))!=-1))
	{
		//判断格式包含GS信息
		CString t_cs1,t_cs2,t_cs3;
		t_strText=t_strText.Right(t_strText.GetLength()-t_strText.Find(_T("1P"))-2);
		t_cs1=t_strText.Left(t_strText.Find(_T("2P")));
		t_strText=t_strText.Right(t_strText.GetLength()-t_strText.Find(_T("2P"))-2);
		t_cs2=t_strText.Left(t_strText.Find(_T("S")));
		t_strText=t_strText.Right(t_strText.GetLength()-t_strText.Find(_T("S"))-1);
		t_strText=t_strText.Left(11);
		t_cs3=t_cs1+_T("-")+t_cs2;
		if(t_cs.Find(_T("SN1"))!=-1)
		{
			((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage)[_T("003SN1")]=t_strText;
			((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage)[_T("004PN1")]=t_cs3;
			m_listdutmsg.SetItemText(m_nItem+1,m_nSubItem,t_cs3);
		}
		else if(t_cs.Find(_T("SN2"))!=-1)
		{
			((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage)[_T("009SN2")]=t_strText;
			((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage)[_T("010PN2")]=t_cs3;
			m_listdutmsg.SetItemText(m_nItem+1,m_nSubItem,t_cs3);
		}
    else if (t_cs.Find(_T("SN3")) != -1)
    {
      ((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage)[_T("024SN3")] = t_strText;
      ((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage)[_T("025PN3")] = t_cs3;
      m_listdutmsg.SetItemText(m_nItem + 1, m_nSubItem, t_cs3);
    }
    else if (t_cs.Find(_T("SN4")) != -1)
    {
      ((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage)[_T("026SN4")] = t_strText;
      ((static_cast<CtewDlg*>(m_ptewdlg))->m_mapdutmessage)[_T("027PN4")] = t_cs3;
      m_listdutmsg.SetItemText(m_nItem + 1, m_nSubItem, t_cs3);
    }
	}
	m_listdutmsg.SetItemText(m_nItem,m_nSubItem,t_strText);  
	dutmsgShowEdit(false,-1,-1,CRect());  

	return 0;
}

void cdlgdutmsg::OnNMDblclklistdutmsg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate=reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	CRect t_rcCtrl;  
	LVHITTESTINFO t_lvhti;
	POINT t_point;
	// 得到鼠标点击位置 
	GetCursorPos(&t_point);
	m_listdutmsg.ScreenToClient(&t_point);
	t_lvhti.pt=t_point;  
	m_nItem=m_listdutmsg.SubItemHitTest(&t_lvhti);  
	if(m_nItem!=-1)   
	{
		m_nSubItem=t_lvhti.iSubItem;  
		m_listdutmsg.GetSubItemRect(m_nItem,m_nSubItem,LVIR_LABEL,t_rcCtrl);  
		CRect t_rclist;  
		m_listdutmsg.GetWindowRect(&t_rclist);
		this->ScreenToClient(&t_rclist);
		t_rcCtrl.top+=t_rclist.top;
		t_rcCtrl.bottom+=t_rclist.top+3;
		t_rcCtrl.left+=t_rclist.left+3;
		t_rcCtrl.right+=t_rclist.left;
		if(t_lvhti.iSubItem==1)  
		{
			dutmsgShowEdit(TRUE,m_nItem,m_nSubItem,t_rcCtrl);  
		}
	}
	*pResult=0;
}

void cdlgdutmsg::OnHdnBegintracklistdutmsg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr=reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	if (m_editmsg.m_hWnd != NULL && m_bEditVisible)  
	{
		this->SendMessage(WM_dlgdutmsgeditend,0,0);  
	}
	*pResult=0;
}
