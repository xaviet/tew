// trmdlg.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "trmdlg.h"
#include "afxdialogex.h"


// ctrmdlg 对话框

IMPLEMENT_DYNAMIC(ctrmdlg, CDialogEx)

ctrmdlg::ctrmdlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ctrmdlg::IDD, pParent)
	, m_pitemdata(0)
	, m_pctrmthread(0)
	, m_ptewdlg(0)
	, m_modecomm(0)
	, m_stopflag(0)
{
	m_trmmode.autologin=0;
}

ctrmdlg::~ctrmdlg()
{
}

void ctrmdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_trmdlgedit, m_edittrmdlg);
}


BEGIN_MESSAGE_MAP(ctrmdlg, CDialogEx)
	ON_MESSAGE(WM_trmdlgresize,&ctrmdlg::Ontrmdlgresize)
	ON_WM_SIZE()
	ON_MESSAGE(WM_trmdlgeditaddcstring,&ctrmdlg::Ontrmdlgeditaddcstring)
	ON_MESSAGE(WM_trmdlgstop,&ctrmdlg::Ontrmdlgstop)
	ON_MESSAGE(WM_trmdlgtrmfaultcounter, &ctrmdlg::Ontrmdlgtrmfaultcounter)
	ON_MESSAGE(WM_trmdlgsetsavevalue,&ctrmdlg::Ontrmdlgsetsavevalue)
	ON_MESSAGE(WM_trmdlggetsavevalue,&ctrmdlg::Ontrmdlggetsavevalue)
	ON_MESSAGE(WM_trmdlgsetcsv,&ctrmdlg::Ontrmdlgsetcsv)
	ON_MESSAGE(WM_trmdlgctrladdloglist,&ctrmdlg::Ontrmdlgctrladdloglist)
	ON_MESSAGE(WM_trmdlgreplacelabel,&ctrmdlg::Ontrmdlgreplacelabel)
	ON_MESSAGE(WM_trmdlgmodule, &ctrmdlg::Ontrmdlgmodule)
	ON_MESSAGE(WM_trmdlgovertime, &ctrmdlg::Ontrmdlgovertime)
	ON_MESSAGE(WM_trmdlgmsg, &ctrmdlg::Ontrmdlgmsg)
	ON_MESSAGE(WM_trmdlgctrlstop, &ctrmdlg::Ontrmdlgctrlstop)
END_MESSAGE_MAP()

// 重绘edit控件
int ctrmdlg::resizeedittrmdlg()
{
	this->MoveWindow(&m_rc);
	if(m_edittrmdlg)
	{
		CRect rc; 
		this->GetClientRect(&rc); 
		rc.top+=1; 
		rc.bottom-=1; 
		rc.left+=1; 
		rc.right-=1; 
		m_edittrmdlg.MoveWindow(&rc);

	}
	return 0;
}

// trmdlg status changed
int ctrmdlg::trmstatuschanged(int v_i)
{
	m_pitemdata->runstatus=v_i;
	m_ptewdlg->SendMessage(WM_appdlgnodepropertymodify,(WPARAM)m_pitemdata);
	return 0;
}

// 由文件匹配runmode
int ctrmdlg::trmgetmodefromfile()
{
	CStdioFile t_csf;
	CString t_cs;
	// 模式匹配标志
	int t_mode=0;
	// m_mapmode初始化
	trminitmapmode();
	if(t_csf.Open(m_ptewdlg->m_csmodecomm,CFile::modeRead|CFile::shareDenyNone))
	{
		t_csf.SeekToBegin();	
		CString t_line,t_filenametemp,t_hosttemp;
		int t_porttemp;

		for(;(t_csf.ReadString(t_line)!=NULL);)
		{
			t_line.TrimLeft();
			//忽略空行和注释行initappconfigcomment
			if((t_line.GetLength()==0)||(t_line.Find(DEF_COMMENT)==0))
			{
				continue;
			}
			t_line.TrimRight();
			t_line.Replace(_T(" "),_T(""));
			if(t_mode==0)
			{
				int i=t_line.Find(_T("["));
				int j=t_line.Find(_T("]"));
				if((i!=-1)&&(j!=-1)&&(i<j))
				{
					t_cs=t_line.Mid(i+1,j-i-1);
					int x=0;
					//获取文件名,';'结束
					t_filenametemp=t_cs.Left(t_cs.Find(CONFIGDELIM));
					if(t_filenametemp.GetLength()==0)
					{
						AfxMessageBox(_T("通信初始化模板文件参数错误"));
						break;
					}
					if((m_pitemdata->filename.Find(t_filenametemp))==-1)
					{
						continue;
					}
					else 
					{
						x++;
					}
					t_cs=t_cs.Right(t_cs.GetLength()-1-t_cs.Find(CONFIGDELIM));
					//获取主机名,';'结束
					t_hosttemp=t_cs.Left(t_cs.Find(CONFIGDELIM));
					if(((m_pitemdata->host.Find(t_hosttemp))!=-1)||(t_hosttemp.GetLength()==0))
					{
						x++;
					}
					t_cs=t_cs.Right(t_cs.GetLength()-1-t_cs.Find(CONFIGDELIM));
					//获取端口,';'结束,注意为空返回0,0也会返回0
					t_porttemp=_ttoi(t_cs.Left(t_cs.Find(CONFIGDELIM)));
					if((m_pitemdata->port==t_porttemp)||(t_porttemp==0))
					{
						x++;
					}
					if((t_porttemp==0)&&(t_cs.GetLength()==0))
					{
						AfxMessageBox(_T("通信初始化模板主机和端口参数错误"));
						break;
					}
					if(x==3)
					{
						t_mode=mode_tcp;
						if(t_hosttemp==_T("gpib"))
						{
							t_mode=mode_gpib;
						}
						else if(t_hosttemp==_T("com"))
						{
							t_mode=mode_com;
						}
						else
						{
							if(m_pitemdata->sshv2)
							{
								t_mode=mode_sshv2;
							}
							else if(m_pitemdata->directtcp)
							{
								t_mode=mode_directtcp;
							}
							else
							{
								t_mode=mode_tcp;
							}
						}
					}
				}
				continue;
			}
			else
			{
				if(t_line.Find(_T("["))!=-1)
				{
					break;
				}
				else
				{
					//分析初始化参数
					int y;
					CString t_cstr1,t_cstr2;
					y=t_line.Find(_T("="));
					if(y==-1)
					{
						AfxMessageBox(t_line+_T("\n通信模板初始化错误"));
						continue;
					}
					t_cstr1=t_line.Left(y);
					t_cstr2=t_line.Right(t_line.GetLength()-y-1);
					t_cstr2=t_cstr2.Left(t_cstr2.Find(CONFIGDELIM));
					m_mapmode[t_cstr1]=t_cstr2;
				}
			}
		}
		t_csf.Close();
	}
	else
	{
		AfxMessageBox(_T("通信模板Unload"));
	}
	if(t_mode!=0)
	{
		// 由map获取mode配置
		trmgetmodefrommap();
	}
	else
	{
		AfxMessageBox(_T("未能成功匹配通信方式模板"));
		assert(0);
	}

	return t_mode;
}

// 获取mode配置
int ctrmdlg::trmgetmodefrommap()
{
	if(m_mapmode.find(_T("comment"))!=m_mapmode.end())
	{
		m_trmmode.comment=m_mapmode[_T("comment")];
	}
	if(m_mapmode.find(_T("initreturn"))!=m_mapmode.end())
	{
		m_trmmode.initreturn=_ttoi(m_mapmode[_T("initreturn")]);
	}
	if(m_mapmode.find(_T("autologin"))!=m_mapmode.end())
	{
		m_trmmode.autologin=_ttoi(m_mapmode[_T("autologin")]);
	}
	if(m_mapmode.find(_T("displayuser"))!=m_mapmode.end())
	{
		m_trmmode.displayuser=m_mapmode[_T("displayuser")];
	}
	if(m_mapmode.find(_T("displaypwd"))!=m_mapmode.end())
	{
		m_trmmode.displaypwd=m_mapmode[_T("displaypwd")];
	}
	if(m_mapmode.find(_T("user"))!=m_mapmode.end())
	{
		m_trmmode.user=m_mapmode[_T("user")];
	}	
	if(m_mapmode.find(_T("pwd"))!=m_mapmode.end())
	{
		m_trmmode.pwd=m_mapmode[_T("pwd")];
	}
	if(m_mapmode.find(_T("prompt"))!=m_mapmode.end())
	{
		m_trmmode.prompt=m_mapmode[_T("prompt")];
	}	
	if(m_mapmode.find(_T("displayback"))!=m_mapmode.end())
	{
		m_trmmode.displayback=_ttoi(m_mapmode[_T("displayback")]);
	}
	if(m_mapmode.find(_T("reflectkey"))!=m_mapmode.end())
	{
		m_trmmode.reflectkey=m_mapmode[_T("reflectkey")];
	}	
	if(m_mapmode.find(_T("exitaction"))!=m_mapmode.end())
	{
		m_trmmode.exitaction=m_mapmode[_T("exitaction")];
	}
	if(m_mapmode.find(_T("BaudRate"))!=m_mapmode.end())
	{
		m_trmmode.BaudRate=_ttoi(m_mapmode[_T("BaudRate")]);
	}	
	if(m_mapmode.find(_T("ByteSize"))!=m_mapmode.end())
	{
		m_trmmode.ByteSize=_ttoi(m_mapmode[_T("ByteSize")]);
	}
	if(m_mapmode.find(_T("Parity"))!=m_mapmode.end())
	{
		m_trmmode.Parity=_ttoi(m_mapmode[_T("Parity")]);
	}	
	if(m_mapmode.find(_T("StopBits"))!=m_mapmode.end())
	{
		m_trmmode.StopBits=_ttoi(m_mapmode[_T("StopBits")]);
	}
	if(m_mapmode.find(_T("delaytime"))!=m_mapmode.end())
	{
		m_trmmode.delaytime=_ttoi(m_mapmode[_T("delaytime")]);
	}
	if(m_mapmode.find(_T("headflag"))!=m_mapmode.end())
	{
		m_trmmode.headflag=m_mapmode[_T("headflag")];
	}
	if(m_mapmode.find(_T("tailflag"))!=m_mapmode.end())
	{
		m_trmmode.tailflag=m_mapmode[_T("tailflag")];
	}
	if(m_mapmode.find(_T("heartmsg"))!=m_mapmode.end())
	{
		m_trmmode.heartmsg=m_mapmode[_T("heartmsg")];
	}
	if(m_mapmode.find(_T("heartintv"))!=m_mapmode.end())
	{
		m_trmmode.heartintv=_ttoi(m_mapmode[_T("heartintv")]);
	}
	if(m_mapmode.find(_T("endofline"))!=m_mapmode.end())
	{
		m_trmmode.endofline=m_mapmode[_T("endofline")];
		// 检测字符串
		if(m_trmmode.endofline.Find(_T("\\\\"))==-1)
		{
			// 字符串不含有_T("\\\\"),替换为特殊字符
			//	tcp和串口通信
			m_trmmode.endofline.Replace(_T("\\n"),_T("\n"));
			m_trmmode.endofline.Replace(_T("\\r"),_T("\r"));
		}
		else
		{
			// 字符串含有_T("\\\\"),替换为_T("\\")
			//	gpib通信
			m_trmmode.endofline.Replace(_T("\\\\"),_T("\\"));
		}
	}
	return 0;
}

// m_mapmode初始化
int ctrmdlg::trminitmapmode()
{
	m_mapmode.clear();
	m_mapmode[_T("comment")]=_T("#");
	m_mapmode[_T("initreturn")]=_T("0");
	m_mapmode[_T("autologin")]=_T("0");
	m_mapmode[_T("displayuser")]=_T("login:");
	m_mapmode[_T("displaypwd")]=_T("Password:");
	m_mapmode[_T("user")]=_T("a");
	m_mapmode[_T("pwd")]=_T("a");
	m_mapmode[_T("prompt")]=_T("$");
	m_mapmode[_T("displayback")]=_T("0");
	m_mapmode[_T("reflectkey")]=_T("");
	m_mapmode[_T("exitaction")]=_T("");
	m_mapmode[_T("BaudRate")]=_T("9600");
	m_mapmode[_T("ByteSize")]=_T("8");
	m_mapmode[_T("Parity")]=_T("0");
	m_mapmode[_T("StopBits")]=_T("0");
	m_mapmode[_T("delaytime")]=_T("0");
	m_mapmode[_T("endofline")]=RETURNNEWLINE;
	m_mapmode[_T("headflag")]=_T("00");
	m_mapmode[_T("tailflag")]=_T("00");
	m_mapmode[_T("heartmsg")]=_T("00");
	m_mapmode[_T("heartintv")]=_T("1000");
	return 0;
}

// terminal work
int ctrmdlg::trmwork()
{
	assert(m_pitemdata);
	// 由文件匹配runmode@
	m_modecomm=trmgetmodefromfile();
	trmstatuschanged(iconrwn);
	// 启动trmthread线程  
	threadbegintrmthread();
	return 0;
}

// 启动trmthread线程  
int ctrmdlg::threadbegintrmthread()
{
	// 创建挂起的线程
	if(m_pitemdata->directtcp==TRUE)
	{
		m_pctrmthread=AfxBeginThread(RUNTIME_CLASS(ctrmthreaddirecttcp),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED,NULL);
	}
	else
	{
		m_pctrmthread=AfxBeginThread(RUNTIME_CLASS(ctrmthread),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED,NULL);
	}
	// 配置trmthread参数
	settrmthreadpara();
	// 恢复线程
	m_pctrmthread->ResumeThread();
	return 0;
}

// 配置trmthread参数
int ctrmdlg::settrmthreadpara()
{
	// mode_comm初始化
	if(m_pitemdata->directtcp==TRUE)
	{
		(static_cast<ctrmthreaddirecttcp*>(m_pctrmthread))->m_ptrmdlg=this;
		(static_cast<ctrmthreaddirecttcp*>(m_pctrmthread))->m_ptrmmode=&m_trmmode;
		(static_cast<ctrmthreaddirecttcp*>(m_pctrmthread))->m_pitemdata=m_pitemdata;
		m_pitemdata->faultcounter=0;
		(static_cast<ctrmthreaddirecttcp*>(m_pctrmthread))->m_modecomm=m_modecomm;
	}
	else
	{
		(static_cast<ctrmthread*>(m_pctrmthread))->m_ptrmdlg=this;
		(static_cast<ctrmthread*>(m_pctrmthread))->m_ptrmmode=&m_trmmode;
		(static_cast<ctrmthread*>(m_pctrmthread))->m_pitemdata=m_pitemdata;
		m_pitemdata->faultcounter=0;
		(static_cast<ctrmthread*>(m_pctrmthread))->m_modecomm=m_modecomm;
	}
	return 0;
}

// ctrmdlg 消息处理程序
/* 致终于来到这里的勇敢的人：
    你是被上帝选中的人，英勇的、不辞劳苦的、不眠不修的来修改
我们这最棘手的代码的编程勇士。你，我们的救世主，人中之龙，
我要对你说：永远不要放弃，永远不要对自己失望，永远不要逃走，辜负了自己。
永远不要哭啼，永远不要说再见。永远不要说谎来伤害自己。 */
BOOL ctrmdlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// edit控件能力:256M-1
	m_edittrmdlg.SetLimitText(EDITCTRLLIMIT);
	// 设置新字体
	//m_editFont.CreatePointFont(120,_T("宋体"));
	//m_edittrmdlg.SetFont(&m_editFont);
	// TODO:  ÔÚ´ËÌí¼Ó¶îÍâµÄ³õÊ¼»¯
	assert(m_ptewdlg);
	// terminal work
	trmwork();
	return TRUE;  // return TRUE unless you set the focus to a control
	// Òì³£: OCX ÊôÐÔÒ³Ó¦·µ»Ø FALSE
}

// appdlg stop command
afx_msg LRESULT ctrmdlg::Ontrmdlgstop(WPARAM wParam, LPARAM lParam)
{
	assert(m_pctrmthread);
	// quit trm thread
	m_pctrmthread->PostThreadMessage(WM_QUIT,0,0);
	m_stopflag=1;
	return 0;
}

// trm msg
afx_msg LRESULT ctrmdlg::Ontrmdlgmsg(WPARAM wParam, LPARAM lParam)
{
	//  trm msg
	m_ptewdlg->SendMessage(WM_appdlgmsg,wParam,lParam);
	return 0;
}

// module begin or end
afx_msg LRESULT ctrmdlg::Ontrmdlgmodule(WPARAM wParam, LPARAM lParam)
{
	// module begin or end
	m_ptewdlg->SendMessage(WM_appdlgmodule,wParam,lParam);
	return 0;
}

// get timer
afx_msg LRESULT ctrmdlg::Ontrmdlgovertime(WPARAM wParam, LPARAM lParam)
{
	// get timer
	m_ptewdlg->SendMessage(WM_appdlgovertime,wParam,lParam);
	return 0;
}

// replace label
afx_msg LRESULT ctrmdlg::Ontrmdlgreplacelabel(WPARAM wParam, LPARAM lParam)
{
	// replace label
	m_ptewdlg->SendMessage(WM_appdlgreplacelabel,wParam,lParam);
	return 0;
}

// trm dlg resize
afx_msg LRESULT ctrmdlg::Ontrmdlgresize(WPARAM wParam, LPARAM lParam)
{
	// 重绘edit控件
	resizeedittrmdlg();
	return 0;
}

// 出现错误
afx_msg LRESULT ctrmdlg::Ontrmdlgtrmfaultcounter(WPARAM wParam, LPARAM lParam)
{
	m_ptewdlg->SendMessage(WM_appdlgtrmdlgtrmfaultcounter,wParam,lParam);
	return 0;
}

// 设置存储值
afx_msg LRESULT ctrmdlg::Ontrmdlgsetsavevalue(WPARAM wParam, LPARAM lParam)
{
	m_ptewdlg->SendMessage(WM_appdlgsetsavevalue,wParam,lParam);
	return 0;
}

// 读取存储值
afx_msg LRESULT ctrmdlg::Ontrmdlggetsavevalue(WPARAM wParam, LPARAM lParam)
{
	m_ptewdlg->SendMessage(WM_appdlggetsavevalue,wParam,lParam);
	return 0;
}

// setcsv
afx_msg LRESULT ctrmdlg::Ontrmdlgsetcsv(WPARAM wParam, LPARAM lParam)
{
	m_ptewdlg->SendMessage(WM_appdlgsetcsv,wParam,lParam);
	return 0;
}

// log list add
afx_msg LRESULT ctrmdlg::Ontrmdlgctrladdloglist(WPARAM wParam, LPARAM lParam)
{
	m_ptewdlg->SendMessage(WM_appdlgctrladdloglist,wParam,lParam);
	return 0;
}

// new cdialogex,重载该虚函数删除指针
void ctrmdlg::PostNcDestroy()
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	delete this;
	CDialogEx::PostNcDestroy();
}


void ctrmdlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ÔÚ´Ë´¦Ìí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂë
	resizeedittrmdlg();
}

// trm dlg cedit add cstring
afx_msg LRESULT ctrmdlg::Ontrmdlgeditaddcstring(WPARAM wParam, LPARAM lParam)
{
	// 显示最新,不闪烁
	int t_j=m_edittrmdlg.GetWindowTextLength();
	m_edittrmdlg.SetSel(t_j,t_j);
	m_edittrmdlg.ReplaceSel((LPCTSTR)wParam);
	m_edittrmdlg.ScrollWindow(0,0);
	return 0;
}

// ctrl stop
afx_msg LRESULT ctrmdlg::Ontrmdlgctrlstop(WPARAM wParam, LPARAM lParam)
{
	m_ptewdlg->SendMessage(WM_appdlgctrlstop,wParam,lParam);
	return 0;
}

void ctrmdlg::OnCancel()
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	DestroyWindow();
	//CDialogEx::OnCancel();
}

BOOL ctrmdlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	if(message==WM_CLOSE)
	{
		// 终端退出,清理(例如退出配置模式)
		if(m_pctrmthread!=NULL)
		{
			CString t_cs;
			t_cs.Format(_T("(ng[%d])"),m_pitemdata->faultcounter);
			t_cs=_T("complete ")+m_pitemdata->comment+t_cs;
			m_ptewdlg->SendMessage(WM_appdlgctrladdloglist,(WPARAM)t_cs.GetBuffer(0),0);
			t_cs.ReleaseBuffer();
			// trm 自行完成
			if(m_stopflag!=1)
			{
				m_ptewdlg->SendMessage(WM_appdlgtrmdlgclose,(WPARAM)m_pitemdata,0);
			}
			// 说实在的,我也不知道为什么这样
			m_pitemdata->ptrdlg=NULL;
			m_pitemdata=NULL;
			m_ptewdlg=NULL;
			m_pctrmthread=NULL;
		}
	}
	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}
