// tewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "tewDlg.h"
#include "trmDlg.h"
#include "aboutdlg.h"
#include "nodepropertydlg.h"
#include "afxdialogex.h"
#include "dlgdutmsg.h"
#include "dlgproductresult.h"
#include "dlgprocessing.h"
#include "tcpserver.h"
#include "udp.h"
#include "dlgconfig.h"
#include "mmsystem.h"
#pragma comment(lib,"winmm")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtewDlg 对话框

CtewDlg::CtewDlg(CWnd* pParent /*=NULL*/)
  : CDialogEx(CtewDlg::IDD, pParent)
  , m_modctrlscaleflag(0)
  , m_pcuritemdata(0)
  , m_timer(0)
  , m_pmysql(0)
  , m_hAccTable(0)
  , m_pmenu(0)
  , m_ptoolbarctrl(0)
  , m_hsemaphore(0)
  , m_ptcpserver(0)
  , m_pudp(0)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  m_dlocalver.vmah = 0;
  //m_configflag.loop=0;
  m_ctrlpositionarray[0][0] = 0;
  m_ptrmtabpageitemdataarray[0] = NULL;
}

void CtewDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_cfgtree, m_cfgtree);
  DDX_Control(pDX, IDC_trmtab, m_trmtab);
  DDX_Control(pDX, IDC_loglist, m_loglist);
}

BEGIN_MESSAGE_MAP(CtewDlg, CDialogEx)
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_COMMAND(ID_dlgexit, &CtewDlg::Ondlgexit)
  ON_WM_SIZE()
  ON_WM_GETMINMAXINFO()
  ON_COMMAND(ID_ctrlloglistclear, &CtewDlg::Onctrlloglistclear)
  ON_MESSAGE(WM_appdlgctrladdloglist, &CtewDlg::Onappdlgctrladdloglist)
  ON_MESSAGE(WM_appdlgctrlsetstatusbar, &CtewDlg::Onappdlgctrlsetstatusbar)
  ON_MESSAGE(WM_appdlgnodepropertymodify, &CtewDlg::Onappdlgnodepropertymodify)
  ON_MESSAGE(WM_appdlgtrmdlgclose, &CtewDlg::Onappdlgtrmdlgclose)
  ON_NOTIFY(NM_RCLICK, IDC_loglist, &CtewDlg::OnNMRClickloglist)
  ON_WM_DROPFILES()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_COMMAND(ID_dlgopenfile, &CtewDlg::OnDlgopenfile)
  ON_COMMAND(ID_dlgopendir, &CtewDlg::OnDlgopendir)
  ON_COMMAND(ID_dlgpause, &CtewDlg::OnDlgpause)
  ON_COMMAND(ID_dlgstart, &CtewDlg::OnDlgstart)
  ON_COMMAND(ID_dlgstop, &CtewDlg::OnDlgstop)
  ON_COMMAND(ID_dlghelpdoc, &CtewDlg::OnDlghelpdoc)
  ON_COMMAND(ID_about, &CtewDlg::OnAbout)
  ON_COMMAND(ID_dlgexplorer, &CtewDlg::OnDlgexplorer)
  ON_NOTIFY(NM_RCLICK, IDC_cfgtree, &CtewDlg::OnNMRClickcfgtree)
  ON_COMMAND(ID_dlgenablenode, &CtewDlg::Ondlgenablenode)
  ON_COMMAND(ID_dlgdisablenode, &CtewDlg::Ondlgdisablenode)
  ON_COMMAND(ID_ctrlcfgtreeproperty, &CtewDlg::Onctrlcfgtreeproperty)
  ON_NOTIFY(NM_DBLCLK, IDC_cfgtree, &CtewDlg::OnNMDblclkcfgtree)
  ON_COMMAND(ID_dlgsaveas, &CtewDlg::Ondlgsaveas)
  ON_COMMAND(ID_dlgsave, &CtewDlg::Ondlgsave)
  ON_COMMAND(ID_dlginittcpport, &CtewDlg::Ondlginittcpport)
  ON_COMMAND(ID_dlgedit, &CtewDlg::Ondlgedit)
  ON_NOTIFY(TVN_SELCHANGED, IDC_cfgtree, &CtewDlg::OnTvnSelchangedcfgtree)
  ON_NOTIFY(TCN_SELCHANGE, IDC_trmtab, &CtewDlg::OnTcnSelchangetrmtab)
  ON_COMMAND(ID_dlglog, &CtewDlg::Ondlglog)
  ON_COMMAND(ID_dlgcsv, &CtewDlg::Ondlgcsv)
  ON_MESSAGE(WM_appdlgtrmdlgtrmfaultcounter, &CtewDlg::Onappdlgtrmdlgtrmfaultcounter)
  ON_MESSAGE(WM_appdlgsetsavevalue, &CtewDlg::Onappdlgsetsavevalue)
  ON_MESSAGE(WM_appdlggetsavevalue, &CtewDlg::Onappdlggetsavevalue)
  ON_MESSAGE(WM_appdlgsetcsv, &CtewDlg::Onappdlgsetcsv)
  ON_MESSAGE(WM_appdlgreplacelabel, &CtewDlg::Onappdlgreplacelabel)
  ON_MESSAGE(WM_appdlgmodule, &CtewDlg::Onappdlgmodule)
  ON_WM_TIMER()
  ON_MESSAGE(WM_appdlgovertime, &CtewDlg::Onappdlgovertime)
  ON_COMMAND(ID_dlgconfig, &CtewDlg::Ondlgconfig)
  ON_MESSAGE(WM_appdlgconfig, &CtewDlg::Onappdlgconfig)
  ON_MESSAGE(WM_appdlgmsg, &CtewDlg::Onappdlgmsg)
  ON_MESSAGE(WM_appdlgtcpservermsg, &CtewDlg::Onappdlgtcpservermsg)
  ON_MESSAGE(WM_appdlgctrlstop, &CtewDlg::Onappdlgctrlstop)
  ON_COMMAND(ID_dlgopenpath, &CtewDlg::Ondlgopenpath)
  ON_COMMAND(IDR_ACCELERATOR, &CtewDlg::OnIdrAccelerator)
END_MESSAGE_MAP()


// CtewDlg 消息处理程序

BOOL CtewDlg::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // 设置此对话框的图标.当应用程序主窗口不是对话框时,框架将自动
  //  执行此操作
  SetIcon(m_hIcon, TRUE);			// 设置大图标
  SetIcon(m_hIcon, FALSE);		// 设置小图标

  // TODO: 在此添加额外的初始化代码
  // 初始化配置标志
  m_configflag.loop = 0;
  m_configflag.ngstop = 0;
  m_configflag.zlogdir = _T("");
  // 初始化app环境,包含界面初始化
  initall();
  // menu deny
  setallcmddeny();
  // app ready
  CString t_cs = _T("[tew]ready");
  this->SendMessage(WM_appdlgctrladdloglist, (WPARAM)t_cs.GetBuffer(0), 0);
  t_cs.ReleaseBuffer();
  // 默认回调函数OnTimer
  SetTimer(APPDLG1STIMER, 1000, NULL);
  SetTimer(APPDLG60STIMER, 60000, NULL);
  // menu ready
  setcmdinready();
  // start sound
  PlaySound(MAKEINTRESOURCE(IDR_waveerror), AfxGetApp()->m_hInstance, SND_RESOURCE | SND_ASYNC | SND_NODEFAULT);
  //载入快捷键资源,其中IDR_ACCELERATOR为快捷键表的ID
  m_hAccTable = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));

  return TRUE;  // 除非将焦点设置到控件,否则返回 TRUE
}

// config
afx_msg LRESULT CtewDlg::Onappdlgconfig(WPARAM wParam, LPARAM lParam)
{
  // 指针传递,无需参数赋值
  m_mapappcfg[_T("loop")] = m_configflag.loop == 1 ? _T("1") : _T("0");
  m_mapappcfg[_T("ngstop")] = m_configflag.ngstop == 1 ? _T("1") : _T("0");
  m_mapappcfg[_T("logpath")] = m_configflag.zlogdir;

  return 0;
}

// ctrl log list 增加内容
afx_msg LRESULT CtewDlg::Onappdlgctrladdloglist(WPARAM wParam, LPARAM lParam)
{
  CString	t_csdate;
  // 获取时间
  t_csdate = getsystemdatetime();
  CString	t_linecount;
  t_linecount.Format(_T("%d"), m_loglist.GetItemCount() + 1);
  m_loglist.InsertItem(0, t_linecount);
  m_loglist.SetItemText(0, 1, t_csdate);
  m_loglist.SetItemText(0, 2, (LPCTSTR)wParam);
  // dlg改变后重绘状态栏,包含分隔比例
  if (IsWindow(m_statusbarctrl.m_hWnd))
  {
    // 添加状态栏,v_i为1创建,0为重绘
    initdlgviewstatusbar(FALSE);
  }
  // dlg改变后重绘log list,包含分隔比例
  if (IsWindow(m_loglist.m_hWnd))
  {
    // 重绘log list
    repaintloglist();
  }
  CString	t_cslog = (LPCTSTR)wParam;
  t_csdate += _T("\t") + t_cslog + _T("\n");
  m_csfapplogfile.WriteString(t_csdate);
  return 0;
}

// log list clear
void CtewDlg::Onctrlloglistclear()
{
  m_loglist.DeleteAllItems();
  // log list界面向下扩展覆盖部分status bar空间,因此重绘dlg
  CRect t_rect;
  GetClientRect(&t_rect);
  repaintctrl(t_rect.Width(), t_rect.Height());
}

// set ctrl statusbar
//	第npane个分栏加入pcstring
//	wParam:字符串指针;lParam:状态栏编号
afx_msg LRESULT CtewDlg::Onappdlgctrlsetstatusbar(WPARAM wParam, LPARAM lParam)
{
  assert(lParam < STATUSBARSEPARATES);
  // 第npane分栏加入pstr
  m_statusbarctrl.SetText((LPCTSTR)wParam, lParam, 0);
  return 0;
}

// trm msg
afx_msg LRESULT CtewDlg::Onappdlgmsg(WPARAM wParam, LPARAM lParam)
{
  struct treeitemdata* t_itemdata = reinterpret_cast<treeitemdata*>(wParam);
  int t_i = FALSE;
  assert(t_itemdata);
  switch (lParam)
  {
  case 1:
    // add
    m_trmmsg += t_itemdata->information;
    t_i = TRUE;
    break;
  case 2:
    // check
    if (m_trmmsg.Find(t_itemdata->information) != -1)
    {
      t_i = TRUE;
    }
    else
    {
      t_i = FALSE;
    }
    break;
  case 3:
    // chkclr
    if (m_trmmsg.Find(t_itemdata->information) != -1)
    {
      m_trmmsg.Replace(t_itemdata->information, _T(""));
      t_i = TRUE;
    }
    else
    {
      t_i = FALSE;
    }
    break;
  case 4:
    // clear
    m_trmmsg.Empty();
    t_i = TRUE;
    break;
  default:
    break;
  }
  if (!t_i)
  {
    this->SendMessage(WM_appdlgnodepropertymodify, wParam, 1);
  }
  else
  {
    this->SendMessage(WM_appdlgnodepropertymodify, wParam, 0);
  }
  t_itemdata->msgoperate = t_i;
  return 0;
}

// module begin or end
afx_msg LRESULT CtewDlg::Onappdlgmodule(WPARAM wParam, LPARAM lParam)
{
  struct treeitemdata* t_itemdata = reinterpret_cast<treeitemdata*>(wParam);
  struct treeitemdata* t_rootid = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(m_cfgtree.GetRootItem()));
  assert(t_itemdata);
  CString t_result;
  if (lParam == 0)
  {
    // 模块开始 初值
    t_result.Format(_T("%d"), t_rootid->faultcounter);
  }
  else
  {
    // 模块结束 差值
    int t_i;
    if (m_mapmodulefield.find(t_itemdata->information) != m_mapmodulefield.end())
    {
      t_i = _ttoi(m_mapmodulefield[t_itemdata->information]);
    }
    else
    {
      t_i = 0;
    }
    t_i = t_rootid->faultcounter - t_i;
    t_result.Format(_T("%d"), t_i);
  }
  m_mapmodulefield[t_itemdata->information] = t_result;
  return 0;
}

// modify node property 
afx_msg LRESULT CtewDlg::Onappdlgnodepropertymodify(WPARAM wParam, LPARAM lParam)
{
  CString t_cs;
  struct treeitemdata* t_itemdata = reinterpret_cast<treeitemdata*>(wParam);
  assert(t_itemdata);
  // 属性修改dlg节点使能/去使能check为只读
  m_cfgtree.SetItemImage(t_itemdata->node, t_itemdata->runstatus, t_itemdata->runstatus);
  if ((lParam == 1) && (m_cfgtree.GetItemText(t_itemdata->node)[0] == _T('<')))
  {
    return 0;
  }
  if (t_itemdata->faultcounter > 0)
  {
    t_cs.Format(_T("[NG:%d]"), t_itemdata->faultcounter);
    t_cs += t_itemdata->comment;
    m_cfgtree.SetItemText(t_itemdata->node, t_cs);
  }
  else if (t_itemdata->faultcounter == 0)
  {
    t_cs = t_itemdata->comment;
    m_cfgtree.SetItemText(t_itemdata->node, t_cs);
  }
  // lParam为1表示有msg check动作,修改相应节点显示
  if (lParam == 1)
  {
    if (m_cfgtree.GetItemText(t_itemdata->node)[0] != _T('<'))
    {
      int t_i = 6;
      CString t_cst, t_csdisp;
      t_cst = t_itemdata->information;
      t_cst = t_cst.Left(t_cst.GetLength() - 1);
      if (t_cst.GetLength() > t_i)
      {
        t_cst = t_cst.Left(t_i);
      }
      t_csdisp = _T("<") + t_cst + _T(">") + t_cs;
      m_cfgtree.SetItemText(t_itemdata->node, t_csdisp);
    }
  }
  return 0;
}

// get timer
afx_msg LRESULT CtewDlg::Onappdlgovertime(WPARAM wParam, LPARAM lParam)
{
  struct treeitemdata* t_itemdata = reinterpret_cast<treeitemdata*>(wParam);
  assert(t_itemdata);
  t_itemdata->overtime = this->m_timer;
  return 0;
}

// replace label
afx_msg LRESULT CtewDlg::Onappdlgreplacelabel(WPARAM wParam, LPARAM lParam)
{
  struct treeitemdata* t_itemdata = reinterpret_cast<treeitemdata*>(wParam);
  assert(t_itemdata);
  if (t_itemdata->fieldname[0] != _T('_'))
  {
    // 获取message字符串
    if (!(m_mapdutmessage.find(t_itemdata->fieldname) != m_mapdutmessage.end()))
    {
      CString t_cs;
      t_cs.Format(_T("配置文件存在未定义的替换标志(%s),请检查"), t_itemdata->fieldname);
      AfxMessageBox(t_cs);
      t_itemdata->fieldvalue.Empty();
    }
    else
    {
      t_itemdata->fieldvalue = m_mapdutmessage[t_itemdata->fieldname];
    }
  }
  else
  {
    // 获取save变量字符串
    int t_i;
    t_i = _ttoi(t_itemdata->fieldname.Right(t_itemdata->fieldname.GetLength() - 1));
    if ((t_i >= 0) && (t_i < SAVEVALUECOUNTER))
    {
      t_itemdata->fieldvalue = m_savevalue[t_i];
    }
    else
    {
      AfxMessageBox(_T("获取save变量参数异常"));
    }
  }
  return 0;
}

// 出现错误
afx_msg LRESULT CtewDlg::Onappdlgtrmdlgtrmfaultcounter(WPARAM wParam, LPARAM lParam)
{
  struct treeitemdata* t_itemdata = reinterpret_cast<treeitemdata*>(wParam);
  assert(t_itemdata);
  HTREEITEM t_node = t_itemdata->node;
  for (; t_node != NULL;)
  {
    t_itemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_node));
    t_itemdata->faultcounter++;
    t_itemdata->runstatus = iconrwf;
    this->SendMessage(WM_appdlgnodepropertymodify, (WPARAM)t_itemdata, NULL);
    t_node = m_cfgtree.GetParentItem(t_node);
  }
  // 出现错误,启动声音提示
  PlaySound(MAKEINTRESOURCE(IDR_waveerror), AfxGetApp()->m_hInstance, SND_RESOURCE | SND_ASYNC | SND_NODEFAULT);
  // ng 触发stop
  if (m_mapappcfg[_T("ngstop")] == _T("1"))
  {
    // 禁用循环执行标志
    m_configflag.loop = FALSE;
    // 获取ID_dlgstart菜单状态
    UINT v_i = m_pmenu->GetSubMenu(1)->GetMenuState(ID_dlgstart, MF_BYCOMMAND);
    // 判断菜单状态是否灰化
    if (v_i & MF_GRAYED)
    {
      // stop
      this->PostMessage(WM_appdlgctrlstop, NULL, NULL);
    }
  }
  return 0;
}

// 设置存储值
afx_msg LRESULT CtewDlg::Onappdlgsetsavevalue(WPARAM wParam, LPARAM lParam)
{
  struct treeitemdata* t_itemdata = reinterpret_cast<treeitemdata*>(wParam);
  assert(t_itemdata);
  assert(lParam < SAVEVALUECOUNTER);
  assert(lParam >= 0);
  m_savevalue[lParam] = t_itemdata->information;
  return 0;
}
// ctrl stop
afx_msg LRESULT CtewDlg::Onappdlgctrlstop(WPARAM wParam, LPARAM lParam)
{
  this->OnDlgstop();
  return 0;
}

// 读取存储值
afx_msg LRESULT CtewDlg::Onappdlggetsavevalue(WPARAM wParam, LPARAM lParam)
{
  struct treeitemdata* t_itemdata = reinterpret_cast<treeitemdata*>(wParam);
  assert(t_itemdata);
  assert(lParam < SAVEVALUECOUNTER);
  assert(lParam >= 0);
  t_itemdata->information = m_savevalue[lParam];
  return 0;
}

// setcsv
afx_msg LRESULT CtewDlg::Onappdlgsetcsv(WPARAM wParam, LPARAM lParam)
{
  if (wParam == NULL)
  {
    m_csfcsv.WriteString(_T("\n"));
  }
  else
  {
    CString t_cs = m_savevalue[lParam] + _T(",");
    m_csfcsv.WriteString(t_cs);
  }
  return 0;
}

// tcp server msg
afx_msg LRESULT  CtewDlg::Onappdlgtcpservermsg(WPARAM wParam, LPARAM lParam)
{
  CString t_cs, t_cstemp;
  t_cs.Empty();
  switch (lParam)
  {
  case 1:
    // msg
    t_cs = this->m_trmmsg;
    break;
  case 2:
    // curtrm
    for (int t_i = 0; t_i < TABCTRLMAXPAGENUMBER; t_i++)
    {
      if (m_ptrmtabpageitemdataarray[t_i] == NULL)
      {
        t_cs += _T("");
        break;
      }
      else
      {
        t_cstemp.Format(_T("[%s]\r\n\tlevel:\t\t%d\r\n\tfile name:\t%s\r\n\tline number:\t%d\r\n"), m_ptrmtabpageitemdataarray[t_i]->comment, m_ptrmtabpageitemdataarray[t_i]->level, m_ptrmtabpageitemdataarray[t_i]->filename, m_ptrmtabpageitemdataarray[t_i]->lineno);
        t_cs += t_cstemp;
      }
    }
    break;
  case 3:
    // value
    for (int t_i = 0; t_i < SAVEVALUECOUNTER; t_i++)
    {
      t_cstemp.Format(_T("[%d]\t["), t_i);
      t_cs += t_cstemp + m_savevalue[t_i] + _T("]\r\n");
    }
    break;
  default:
    break;
  }
  *((CString*)wParam) = t_cs;
  return 0;
}

// trm dialog close message
afx_msg LRESULT CtewDlg::Onappdlgtrmdlgclose(WPARAM wParam, LPARAM lParam)
{
  // 使用信号量进行PV操作
  WaitForSingleObject(m_hsemaphore, INFINITE);
  struct treeitemdata* t_itemdata = reinterpret_cast<treeitemdata*>(wParam);
  assert(t_itemdata->exitflag == 0);
  t_itemdata->exitflag = 1;
  if (t_itemdata->faultcounter == 0)
  {
    t_itemdata->runstatus = iconcwn;
  }
  else if (t_itemdata->faultcounter > 0)
  {
    t_itemdata->runstatus = iconcwf;
  }
  else
  {
    assert(0);
  }
  this->SendMessage(WM_appdlgnodepropertymodify, (WPARAM)t_itemdata, NULL);
  // 检查节点完成状态,延伸至父节点
  setuserdefinenodeexitflag(t_itemdata);
  volatile int t_ic;
  m_trmtab.DeleteItem(t_itemdata->trmtabctrlnumber);
  t_ic = m_trmtab.GetItemCount();
  volatile int t_i;
  for (t_i = t_itemdata->trmtabctrlnumber; m_ptrmtabpageitemdataarray[t_i + 1] != NULL; t_i++)
  {
    m_ptrmtabpageitemdataarray[t_i] = m_ptrmtabpageitemdataarray[t_i + 1];
    m_ptrmtabpageitemdataarray[t_i]->trmtabctrlnumber -= 1;
  }
  m_ptrmtabpageitemdataarray[t_i] = NULL;
  if (m_pcuritemdata != NULL)
  {
    if (m_pcuritemdata->trmtabctrlnumber != t_itemdata->trmtabctrlnumber)
    {
    }
    else
    {
      if (t_ic > 0)
      {
        setcurtreenode(m_ptrmtabpageitemdataarray[t_ic - 1]->node);
        settrmtabpage(t_ic - 1);
      }
      else if (t_ic == 0)
      {
        m_pcuritemdata = NULL;
      }
      else
      {
        assert(0);
      }
    }
  }
  // 使用信号量进行PV操作
  ReleaseSemaphore(m_hsemaphore, 1, NULL);
  runtewdlgstart(m_cfgtree.GetRootItem());
  return 0;
}

// 如果向对话框添加最小化按钮,则需要下面的代码
//  来绘制该图标.对于使用文档/视图模型的 MFC 应用程序,
//  这将由框架自动完成.
void CtewDlg::OnPaint()
{
  if (IsIconic())
  {
    CPaintDC dc(this); // 用于绘制的设备上下文

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

    // 使图标在工作区矩形中居中
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // 绘制图标
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialogEx::OnPaint();
  }
}

// 当用户拖动最小化窗口时系统调用此函数取得光标
//  显示.

HCURSOR CtewDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

// 初始化app环境,包含界面初始化
int	CtewDlg::initall()
{
  // 创建信号量,线程互斥
  m_hsemaphore = CreateSemaphore(NULL, 1, 1, NULL);
  // 初始化终端页面数组
  inittrmtabpageitemdataarray();
  // 初始化app_cfg.dat,不存在则创建并写入缺省内容
  initfile(APP_CFG_DAT, FILE_APP_CFG_DAT);
  // 初始化mode_comm.dat,不存在则创建并写入缺省内容
  initfile(MODE_COMM_DAT, FILE_MODE_COMM_DAT);
  // 初始化mapappcfg(app配置map)
  initmapappcfg();
  // open app log file
  if (m_csfapplogfile.Open(m_mapappcfg[_T("apppath")] + APP_LOG_FILE, CFile::modeCreate | CFile::shareDenyNone | CFile::modeNoTruncate | CFile::modeWrite))
  {
    m_csfapplogfile.SeekToEnd();
  }
  else
  {
    AfxMessageBox(m_mapappcfg[_T("apppath")] + APP_LOG_FILE + "读取失败");
    assert(0);
  }
  // 初始化对话框界面
  initdlgview();
  // 初始化tcp server port:60013,维护使用
  initdlgtcpserver();
  // 初始化database
  initdlgmysql();
  // 初始化 udp process
  initdlgudp();
  return 0;
}

// 初始化tcp server port:60013,维护使用
int CtewDlg::initdlgtcpserver()
{
  // 创建挂起的线程
  m_ptcpserver = AfxBeginThread(RUNTIME_CLASS(ctcpserver), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
  // 配置tcpserver参数
  (static_cast<ctcpserver*>(m_ptcpserver))->m_ptewdlg = this;
  (static_cast<ctcpserver*>(m_ptcpserver))->m_ver = this->getfileversion();
  // 恢复线程
  m_ptcpserver->ResumeThread();
  return 0;
}

// 初始化mysql
// For test
int CtewDlg::initdlgmysql()
{
  if (getdatabase())
  {
    //数据库连接句柄
    m_pmysql = mysql_init(NULL);;
    if (m_pmysql == NULL)
    {
      AfxMessageBox(_T("数据库(mysql)初始化失败"));
    }
    else
    {
      MYSQL_RES* t_pres;
      MYSQL_ROW t_rec;
      int t_errnotewdlg;
      // 不支持密码混编
      bool t_b = false;
      mysql_options(m_pmysql, MYSQL_SECURE_AUTH, &t_b);
      // mydb为你所创建的数据库，3306为端口号，可自行设定 
      mysql_real_connect(m_pmysql, "xa-think", "xa", "xa", "test", 3306, NULL, 0);
      t_errnotewdlg = mysql_errno(m_pmysql);
      if (t_errnotewdlg)
      {
        CString t_cs;
        t_cs = _T("数据库(mysql)连接失败,\n") + CString(mysql_error(m_pmysql));
        AfxMessageBox(t_cs);
      }
      mysql_query(m_pmysql, "select * from optindex");
      t_pres = mysql_store_result(m_pmysql);
      while (t_rec = mysql_fetch_row(t_pres))
      {
        CString t_cs;
        t_cs = CString(t_rec[0]) + _T("\t") + CString(t_rec[1]);
        AfxMessageBox(t_cs);
      }
    }
    //最好写到OnDestroy()函数中
    mysql_close(m_pmysql);
    m_pmysql = NULL;
  }
  return 0;
}

// 初始化终端页面数组
int CtewDlg::inittrmtabpageitemdataarray()
{
  for (int t_i = 0; t_i < TABCTRLMAXPAGENUMBER; t_i++)
  {
    m_ptrmtabpageitemdataarray[t_i] = NULL;
  }
  m_pcuritemdata = NULL;
  return 0;
}

// 初始化 udp process
int CtewDlg::initdlgudp()
{
  // 创建挂起的线程
  m_pudp = AfxBeginThread(RUNTIME_CLASS(cudp), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
  // 配置tcpserver参数
  (static_cast<cudp*>(m_pudp))->m_ptewdlg = this;
  // get local version
  this->getfileversion();
  (static_cast<cudp*>(m_pudp))->m_dlocalver = this->m_dlocalver;
  // 恢复线程
  m_pudp->ResumeThread();
  return 0;
}

// 初始化cfgtree ctrl
int	CtewDlg::initctrlcfgtree()
{
  initctrlcfgtreeimagelist();
  // 为树形控件设置图像序列，节点无图标需要申明TVSIL_STATE
  m_cfgtree.SetImageList(&m_cfgtreeimageList, TVSIL_NORMAL);
  return 0;
}

// 创建tree控件使用imagelist
int CtewDlg::initctrlcfgtreeimagelist()
{
  HICON t_hIcon[CFGTREEIMAGELIST];      // 图标句柄数组
  // 加载图标.并将它们的句柄保存到数组   
  t_hIcon[0] = theApp.LoadIcon(IDI_treeiconready);
  t_hIcon[1] = theApp.LoadIcon(IDI_runningwithnormal);
  t_hIcon[2] = theApp.LoadIcon(IDI_runningwithfault);
  t_hIcon[3] = theApp.LoadIcon(IDI_treeiconwaitting);
  t_hIcon[4] = theApp.LoadIcon(IDI_completewithnormal);
  t_hIcon[5] = theApp.LoadIcon(IDI_completewithfault);
  t_hIcon[6] = theApp.LoadIcon(IDI_disable);
  // 创建图像序列CImageList对象   
  m_cfgtreeimageList.Create(16, 16, ILC_COLOR32, CFGTREEIMAGELIST, CFGTREEIMAGELIST);
  // 白色透明处理
  m_cfgtreeimageList.SetBkColor(RGB(255, 255, 255));
  // 将图标添加到图像序列   
  for (int t_i = 0; t_i < CFGTREEIMAGELIST; t_i++)
  {
    m_cfgtreeimageList.Add(t_hIcon[t_i]);
  }
  return 0;
}

// 初始化trm tab ctrl
int	CtewDlg::initctrltrmtab()
{
  return 0;
}

// 初始化log list ctrl
int	CtewDlg::initctrlloglist()
{
  CRect t_rect;
  // 获取列表视图控件的位置和大小   
  m_loglist.GetClientRect(&t_rect);
  // 为列表视图控件添加全行选中和栅格风格   
  m_loglist.SetExtendedStyle((m_loglist.GetExtendedStyle()) | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
  // 为列表视图控件添加3列,首项默认左对齐不可修改,因此不配置0列,应用仍按此顺序   
  m_loglist.InsertColumn(1, _T("No."), LVCFMT_CENTER, t_rect.Width() / 8);
  m_loglist.InsertColumn(2, _T("Date"), LVCFMT_CENTER, t_rect.Width() * 3 / 8);
  m_loglist.InsertColumn(3, _T("Log"), LVCFMT_LEFT, t_rect.Width() / 2);
  return 0;
}

// 初始化app_cfg.dat&mode_comm.dat
int CtewDlg::initfile(CString v_filename, CString v_cs)
{
  CStdioFile t_csf;
  if (t_csf.Open(getappdir() + v_filename, CFile::modeRead | CFile::shareDenyNone))
  {
    t_csf.Close();
  }
  else
  {
    if (t_csf.Open(getappdir() + v_filename, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone))
    {
      t_csf.WriteString(v_cs);
      t_csf.Close();
    }
    else
    {
      AfxMessageBox(_T("配置文件创建失败.请检查目录访问权限或空间,尝试以管理员身份运行."));
      assert(0);
    }
  }
  return 0;
}

// 初始化log目录
int CtewDlg::pathinitlogpath(CString v_logpath)
{
  m_cslogpath = m_mapappcfg[_T("logpath")].GetLength() == 0 ? \
    m_mapappcfg[_T("lastpath")] + LOG_PATH :\
    m_mapappcfg[_T("logpath")];
  m_cscsvpath = m_cslogpath + m_csstarttime + _T("\\");
  struct treeitemdata* t_pitemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(m_cfgtree.GetRootItem()));
  t_pitemdata->logpath = m_cscsvpath;
  pathmkdir(m_cscsvpath);
  return 0;
}

// 初始化csv文件
int CtewDlg::pathinitcsvfile()
{
  if (m_csfcsv.Open(m_cscsvpath + CSV_FILE, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone))
  {
  }
  else
  {
    assert(0);
  }
  return 0;
}

// 判断目录是否存在
bool CtewDlg::pathgetfolderexists(CString v_cspath)
{
  DWORD t_attr;
  t_attr = GetFileAttributes(v_cspath);
  return (t_attr != (DWORD)(-1)) && (t_attr & FILE_ATTRIBUTE_DIRECTORY);
}

// 递归创建目录， 如果目录已经存在或者创建成功返回TRUE
bool CtewDlg::pathmkdir(CString v_cspath)
{
  int t_len = v_cspath.GetLength();
  if (t_len < 2)
  {
    return false;
  }
  if (_T('\\') == v_cspath[t_len - 1])
  {
    v_cspath = v_cspath.Left(t_len - 1);
    t_len = v_cspath.GetLength();
  }
  if (t_len <= 0)
  {
    return false;
  }
  if (t_len <= 3)
  {
    if (pathgetfolderexists(v_cspath))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  if (pathgetfolderexists(v_cspath))
  {
    return true;
  }
  CString t_Parent;
  t_Parent = v_cspath.Left(v_cspath.ReverseFind(_T('\\')));
  if (t_Parent.GetLength() <= 0)
  {
    return false;
  }
  bool t_Ret = pathmkdir(t_Parent);
  if (t_Ret)
  {
    SECURITY_ATTRIBUTES t_sa;
    t_sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    t_sa.lpSecurityDescriptor = NULL;
    t_sa.bInheritHandle = 0;
    t_Ret = (CreateDirectory(v_cspath, &t_sa) == TRUE);
    return t_Ret;
  }
  else
  {
    return false;

  }
}

// 初始化app配置map
int CtewDlg::initmapappcfg()
{
  /*
  initial following parameters in m_mapappcfg
  */
  m_mapappcfg.clear();
  CStdioFile t_csf;
  if (t_csf.Open(getappdir() + APP_CFG_DAT, CFile::modeRead | CFile::shareDenyNone))
  {
    // 由文件得到app配置map
    m_mapappcfg[_T("loop")] = m_configflag.loop == 1 ? _T("1") : _T("0");
    m_mapappcfg[_T("ngstop")] = m_configflag.ngstop == 1 ? _T("1") : _T("0");
    m_mapappcfg[_T("logpath")] = m_configflag.zlogdir;
    m_mapappcfg[_T("logpath")] = _T("");
    getfile2map(&t_csf, &m_mapappcfg); //把app_cfg.dat记进m_mapappcfg
    m_mapappcfg[_T("apppath")] = getappdir();
    t_csf.Close();
  }
  else
  {
    AfxMessageBox(getappdir() + APP_CFG_DAT + "读取失败");	//配置文件读取失败
    assert(0);
  }
  m_csmodecomm = m_mapappcfg[_T("apppath")] + MODE_COMM_DAT;
  //m_cslogpath=m_mapappcfg[_T("apppath")]+LOG_PATH;
  return 0;
}

// 获取应用程序路径,并转换成目录
CString CtewDlg::getappdir()
{
  CString t_apppath;
  GetModuleFileName(NULL, t_apppath.GetBuffer(_MAX_PATH), _MAX_PATH);
  t_apppath.ReleaseBuffer();
  return getdirfrompath(t_apppath);
}

// 获取mapdutmessage
int  CtewDlg::getdatafile2mapdutmessage()
{
  m_mapdutmessage.clear();
  CString t_csfile = m_mapappcfg[_T("apppath")] + DUT_MSG_FILE;
  CStdioFile t_csf;
  if (t_csf.Open(t_csfile, CFile::modeRead | CFile::shareDenyNone))
  {
    // 由文件得到app配置map
    getfile2map(&t_csf, &m_mapdutmessage);
    m_mapdutmessage[_T("000Date")] = m_csstarttime;
    t_csf.Close();
  }
  else
  {
    AfxMessageBox(_T("生产测试模板读取失败"));
    assert(0);
    return TRUE;
  }
  return 0;
}

// 从路径获取目录
CString CtewDlg::getdirfrompath(CString v_cspath)
{
  return v_cspath.Left(v_cspath.ReverseFind(_T('\\')) + 1);
}

// 从路径获取文件
CString CtewDlg::getfilefrompath(CString v_cspath)
{
  return v_cspath.Right(v_cspath.GetLength() - v_cspath.ReverseFind(_T('\\')) - 1);
}

// get dlg 界面控件间隙
int CtewDlg::getdlgctrlscale()
{
  m_pointns1.SetPoint(m_ctrlpositionarray[0][2] - EDGEWIDTH, m_ctrlpositionarray[0][0] + TOOLBARWIDTH + EDGEWIDTH);
  m_pointns2.SetPoint(m_ctrlpositionarray[2][0] + EDGEWIDTH, m_ctrlpositionarray[2][3] - STATUSBARWIDTH - EDGEWIDTH);
  m_pointwe1.SetPoint(m_ctrlpositionarray[1][0] + EDGEWIDTH, m_ctrlpositionarray[1][3] - EDGEWIDTH);
  m_pointwe2.SetPoint(m_ctrlpositionarray[2][2] - EDGEWIDTH, m_ctrlpositionarray[2][1] + EDGEWIDTH);
  return 0;
}

// resize后记录size to map
int  CtewDlg::getsize2mapappcfg()
{
  CRect t_rect;
  GetWindowRect(&t_rect);
  m_mapappcfg[_T("lastx")].Empty();
  m_mapappcfg[_T("lastx")].Format(_T("%d"), t_rect.Width());
  m_mapappcfg[_T("lasty")].Empty();
  m_mapappcfg[_T("lasty")].Format(_T("%d"), t_rect.Height());
  return 0;
}

// 替换字符串中变量符号,"$_"开头
//	return TRUE:exist
//	return FALSE:none
int CtewDlg::getstring4replacelabel(CString* v_line)
{
  int t_i;
  CString t_cslabel, t_cslabelid;
  t_i = v_line->Find(REPLACE_LABEL);
  if (t_i != -1 && (v_line->Find(REPLACE_VALUE) == -1))
  {
    int t_j;
    // 发现替换符号
    t_cslabelid = REPLACE_LABEL;
    t_j = t_cslabelid.GetLength();
    t_cslabel = v_line->Right(v_line->GetLength() - t_i - t_j);
    t_j = t_cslabel.GetLength();
    for (t_i = 0; t_i < t_j; t_i++)
    {
      if (!(((t_cslabel[t_i] >= _T('0')) && (t_cslabel[t_i] <= _T('9'))) || \
        ((t_cslabel[t_i] >= _T('a')) && (t_cslabel[t_i] <= _T('z'))) || \
        ((t_cslabel[t_i] >= _T('A')) && (t_cslabel[t_i] <= _T('Z')))))
      {
        break;
      }
    }
    t_cslabel = t_cslabel.Left(t_i);
    if (!(m_mapdutmessage.find(t_cslabel) != m_mapdutmessage.end()))
    {
      CString t_cs;
      t_cs.Format(_T("配置文件存在未定义的替换标志(%s),请检查"), t_cslabel);
      AfxMessageBox(t_cs);
    }
    else
    {
      v_line->Replace(REPLACE_LABEL + t_cslabel, m_mapdutmessage[t_cslabel]);
    }
    return TRUE;
  }
  return FALSE;
}

// 获取时间 v_type:0-format;1-dir used
CString  CtewDlg::getsystemdatetime(int v_type)
{
  CString t_cstime;
  SYSTEMTIME	t_sysTime;
  GetLocalTime(&t_sysTime);
  if (v_type == 0)
  {
    t_cstime.Format(_T("%04ld-%02ld-%02ld %02ld:%02ld:%02ld.%03ld"), t_sysTime.wYear, t_sysTime.wMonth, t_sysTime.wDay, t_sysTime.wHour, t_sysTime.wMinute, t_sysTime.wSecond, t_sysTime.wMilliseconds);
  }
  else if (v_type == 1)
  {
    t_cstime.Format(_T("%04ld%02ld%02ld%02ld%02ld%02ld%03ld"), t_sysTime.wYear, t_sysTime.wMonth, t_sysTime.wDay, t_sysTime.wHour, t_sysTime.wMinute, t_sysTime.wSecond, t_sysTime.wMilliseconds);
  }
  else
  {
    assert(0);
  }
  return t_cstime;
}

// 分析目录是否存在userdefine file,return 1 means yes,0 means no
int CtewDlg::getuserdefinefilefromdir(CString v_csdir)
{
  CFileFind t_f;
  CString t_cs;
  BOOL t_bcfg = t_f.FindFile(v_csdir + _T("\\*.cfg"));
  int t_i = 0;
  while (t_bcfg)
  {
    t_bcfg = t_f.FindNextFile();
    t_cs = t_f.GetFilePath();
    // the first cfg file
    if (t_i == 0)
    {
      m_mapappcfg[_T("lastfile")] = getfilefrompath(t_cs);
      m_mapappcfg[_T("lastpath")] = getdirfrompath(t_cs);
    }
    t_i++;
  }
  t_f.Close();
  return t_i;
}
// 解析用户define目录
int CtewDlg::getuserdefinefromdir(CString v_csdir)
{
  CString t_cs = _T("open ") + v_csdir;
  this->SendMessage(WM_appdlgctrladdloglist, (WPARAM)t_cs.GetBuffer(0), 0);
  t_cs.ReleaseBuffer();
  // 分析目录是否存在userdefine file,return 1 means yes,0 means no
  if (getuserdefinefilefromdir(v_csdir))
  {
    // 生成cfg tree
    getuserdefinecfgtree();
  }
  else
  {
    // clear lastfile item
    m_mapappcfg[_T("lastfile")] = _T("");
    HTREEITEM t_node;
    // set用户define root node
    struct treeitemdata* t_pid = reinterpret_cast<treeitemdata*>(getuserdefinedefaultitemdata());
    t_pid->path += _T("\\");
    t_pid->comment = t_pid->path;
    // 创建根节点
    t_node = m_cfgtree.InsertItem(t_pid->comment, t_pid->runstatus, t_pid->runstatus);
    // 配置节点携带信息,指针引用
    t_pid->node = t_node;
    m_cfgtree.SetItemData(t_node, (DWORD)t_pid);
    // 递归建立目录配置tree
    setuserdefinetree4path(v_csdir, t_pid);
    // 展开树控件,选择root node 
    setuserdefinetreeitem(m_cfgtree.GetRootItem(), 1);
    m_cfgtree.SelectItem(m_cfgtree.GetRootItem());
  }
  // 清空cfgtree和tabctrl
  this->SendMessage(ID_dlgstop, NULL, NULL);
  // menu open
  setcmdinopen();
  return 0;
}

// 递归建立目录配置tree
int CtewDlg::setuserdefinetree4path(CString v_cspath, treeitemdata* v_pid)
{
  CFileFind t_f;
  CString t_cs;
  BOOL t_bcfg = t_f.FindFile(v_cspath + _T("\\*.cfg"));
  int t_i = 0;
  while (t_bcfg)
  {
    t_bcfg = t_f.FindNextFile();
    t_cs = t_f.GetFilePath();
    t_i++;
    t_cs.MakeLower();
    // 存在cfg文件,取第一个cfg文件
    if (t_i == 1)
    {
      CStdioFile t_csf;
      t_csf.Open(t_cs, CFile::modeRead | CFile::shareDenyNone);
      t_csf.SeekToBegin();
      getuserdefinebyparseoldfile(&t_csf, v_pid);
      t_csf.Close();
    }
  }
  if (t_i > 1)
  {
    AfxMessageBox(v_cspath + _T(" 有多个cfg文件"));
  }
  // 未发现cfg文件,进入子目录
  if (t_i == 0)
  {
    // 清理,准备遍历目录
    t_f.Close();
    t_bcfg = t_f.FindFile(v_cspath + _T("\\*.*"));
    // 遍历目录
    while (t_bcfg)
    {
      t_bcfg = t_f.FindNextFile();
      t_cs = t_f.GetFilePath();
      // 判断目录非'.'以及是目录和非zlog目录
      if ((!t_f.IsDots()) && (t_f.IsDirectory()) && (t_cs.Find(_T("zlog")) == -1))
      {
        struct treeitemdata* t_pid = new treeitemdata;
        *t_pid = *v_pid;
        t_pid->level = v_pid->level + 1;
        t_pid->comment = t_cs;
        // 创建节点
        HTREEITEM t_node;
        t_node = m_cfgtree.InsertItem(t_pid->comment, t_pid->runstatus, t_pid->runstatus, v_pid->node, TVI_LAST);
        t_pid->path = t_cs + _T("\\");
        // 配置节点携带信息,指针引用
        t_pid->node = t_node;
        m_cfgtree.SetItemData(t_node, (DWORD)t_pid);
        // 递归遍历
        setuserdefinetree4path(t_cs, t_pid);
      }
    }
    t_f.Close();
  }
  return 0;
}

// parse userdefine with old file
int CtewDlg::getuserdefinebyparseoldfile(CStdioFile* v_csf, treeitemdata* v_pid)
{
  CString t_line, t_cs;
  for (; (v_csf->ReadString(t_line)) != NULL;)
  {
    // 忽略无用信息
    t_line.TrimLeft();
    t_line.TrimRight();
    if (getuserdefinelinebydelunuseinfo(t_line) == 1)
    {
      continue;
    }
    // t_line末尾补';'
    if (t_line[t_line.GetLength()] != _T(';'))
    {
      t_line += _T(";");
    }
    // 创建节点
    HTREEITEM t_node;
    struct treeitemdata* t_pid = new treeitemdata;
    *t_pid = *v_pid;
    t_pid->level = v_pid->level + 1;
    // 分析行参数
    // filename
    t_cs = t_line.Left(t_line.Find(_T(";")));
    t_pid->filename = t_cs;
    t_pid->comment = t_cs;
    t_line = t_line.Right(t_line.GetLength() - 1 - t_line.Find(_T(";")));
    // host
    t_cs = t_line.Left(t_line.Find(_T(";")));
    t_pid->host = t_cs;
    t_line = t_line.Right(t_line.GetLength() - 1 - t_line.Find(_T(";")));
    // port
    t_cs = t_line.Left(t_line.Find(_T(";")));
    if (t_cs.Find(SSHV2DEF) != -1)
    {
      t_pid->sshv2 = TRUE;
      t_pid->directtcp = FALSE;
      t_cs.Replace(SSHV2DEF, _T(""));
    }
    else if (t_cs.Find(DIRECTTCPDEF) != -1)
    {
      t_pid->sshv2 = FALSE;
      t_pid->directtcp = TRUE;
      t_cs.Replace(DIRECTTCPDEF, _T(""));
    }
    else
    {
      t_pid->sshv2 = FALSE;
      t_pid->directtcp = FALSE;
    }
    t_pid->port = _ttoi(t_cs);
    assert(t_pid->port);
    t_node = m_cfgtree.InsertItem(t_pid->comment, t_pid->runstatus, t_pid->runstatus, v_pid->node, TVI_LAST);
    // 配置节点携带信息,指针引用
    t_pid->node = t_node;
    m_cfgtree.SetItemData(t_node, (DWORD)t_pid);
  }
  return 0;
}

// 解析用户define文件
int CtewDlg::getuserdefinefromfile(CString v_csdir, CString v_csfile)
{
  CString t_cs = _T("open ") + v_csdir + v_csfile;
  this->SendMessage(WM_appdlgctrladdloglist, (WPARAM)t_cs.GetBuffer(0), 0);
  t_cs.ReleaseBuffer();
  t_cs = v_csdir + v_csfile;
  // SetWindowText:设置标题
  SetWindowText(_T("tew---") + t_cs);
  CStdioFile t_csf;
  if (t_csf.Open(t_cs, CFile::modeRead | CFile::shareDenyNone))
  {
    // parse userdefine
    getuserdefinebyparsefile(&t_csf, 0);
    t_csf.Close();
  }
  else
  {
    AfxMessageBox(_T("sorry,no find this file"));
  }
  return 0;
}

// parse userdefine with file
int CtewDlg::getuserdefinebyparsefile(CStdioFile* v_pcsf, HTREEITEM v_fathernode)
{
  CString t_line;
  HTREEITEM t_node;
  v_pcsf->SeekToBegin();
  t_node = v_fathernode;
  for (; (v_pcsf->ReadString(t_line)) != NULL;)
  {
    // 忽略无用信息
    t_line.TrimLeft();
    t_line.TrimRight();
    if (getuserdefinelinebydelunuseinfo(t_line) == 1)
    {
      continue;
    }
    // 替换字符串中变量符号,"$_"开头
    getstring4replacelabel(&t_line);
    t_node = getuserdefinebyparseline(t_line, t_node);
  }
  // 展开树控件,选择root node 
  setuserdefinetreeitem(m_cfgtree.GetRootItem(), 1);
  m_cfgtree.SelectItem(m_cfgtree.GetRootItem());
  // 清空cfgtree和tabctrl
  this->SendMessage(ID_dlgstop, NULL, NULL);
  return 0;
}

// parse userdefine with line
HTREEITEM CtewDlg::getuserdefinebyparseline(CString v_csline, HTREEITEM v_fathernode)
{
  struct treeitemdata* t_ptreeitemdata;
  // 分析携带信息
  if (v_fathernode == 0)
  {
    t_ptreeitemdata = reinterpret_cast<treeitemdata*>(getuserdefinedefaultitemdata());
    t_ptreeitemdata->level += 1;
  }
  else
  {
    struct treeitemdata* t_ptemp;
    t_ptreeitemdata = new treeitemdata;
    t_ptemp = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(v_fathernode));
    *t_ptreeitemdata = *t_ptemp;
    // 判断当前节点信息是否包含文件名
    if (t_ptemp->filename.GetLength() == 0)
    {
      // 确定下一节点level+1
      t_ptreeitemdata->level += 1;
    }
    else
    {
      // 判断是否需要更新终端节点携带信息
      if (getuserdefinenodetype(v_csline))
      {
        HTREEITEM t_node;
        int t_level;
        CString t_cs;
        // 去除左边'{'
        t_cs = v_csline.Right(v_csline.GetLength() - 1);
        // 获取层次,';'结束
        t_level = _ttoi(t_cs.Left(t_cs.Find(CONFIGDELIM)));

        if (t_level >= (t_ptreeitemdata->level - 1))
        {
          t_node = m_cfgtree.GetParentItem(v_fathernode);
          assert(t_node);
          t_node = m_cfgtree.GetParentItem(t_node);
          if (t_node != NULL)
          {
            // 获取父2层节点携带信息
            t_ptemp = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_node));
            *t_ptreeitemdata = *t_ptemp;
            // 确定下一节点level+2
            t_ptreeitemdata->level += 2;
          }
        }
        else
        {
          t_node = v_fathernode;
          for (; t_level < (t_ptreeitemdata->level + 1); t_level++)
          {
            t_node = m_cfgtree.GetParentItem(t_node);
          }
          t_ptemp = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_node));
          *t_ptreeitemdata = *t_ptemp;
          t_ptreeitemdata->level++;
        }
      }
    }
  }
  // 根据配置行内容更新携带信息
  getuserdefineitemdata(v_csline, t_ptreeitemdata);
  return setuserdefinetreenode(v_fathernode, t_ptreeitemdata);
}

// get node type by parse line;1 to has child;0 not
int CtewDlg::getuserdefinenodetype(CString v_csline)
{
  if (v_csline.Find(_T('{')) != 0)
  {
    // not has child
    return 0;
  }
  else
  {
    // has child
    return 1;
  }
}

// 从path获取id list
LPITEMIDLIST CtewDlg::GetItemIDListFromFilePath(CString v_strFilePath)
{
  CString t_strFilePath = v_strFilePath;
  if (t_strFilePath.GetLength() == 0)
  {
    return NULL;
  }
  // 得到桌面的目录
  LPSHELLFOLDER t_pDesktopFolder = NULL;
  HRESULT t_hr = SHGetDesktopFolder(&t_pDesktopFolder);
  if (FAILED(t_hr))
  {
    return NULL;
  }
  LPITEMIDLIST t_pItemIDList = NULL;
  t_hr = t_pDesktopFolder->ParseDisplayName(NULL, NULL, t_strFilePath.GetBuffer(0), NULL, &t_pItemIDList, NULL);
  t_strFilePath.ReleaseBuffer();
  t_pDesktopFolder->Release();
  if (FAILED(t_hr))
  {
    return NULL;
  }
  return t_pItemIDList;
}

// parse itemdata to get user define line
CString CtewDlg::getuserdefineline(HTREEITEM v_node)
{
  CString t_csline, t_cstemp;
  struct treeitemdata* t_itemdata;
  assert(v_node);
  t_itemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(v_node));
  assert(t_itemdata);
  if (t_itemdata->filename.GetLength() == 0)
  {
    // has child
    //	{level;path;comment;runmtmode;enableflag;message}
    t_csline = _T("{");
    // level
    t_cstemp.Format(_T("%d"), t_itemdata->level);
    t_csline += t_cstemp + CONFIGDELIM;
    // path
    if (m_cfgtree.GetParentItem(v_node) == 0)
    {
      // root node
      t_csline += t_itemdata->path + CONFIGDELIM;
    }
    else
    {
      // not root node
      t_cstemp = t_itemdata->path.Right(t_itemdata->path.GetLength() - (reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(m_cfgtree.GetParentItem(v_node))))->path.GetLength());
      t_csline += t_cstemp + CONFIGDELIM;
    }
    // comment
    t_csline += t_itemdata->comment + CONFIGDELIM;
    // runmtmode
    t_cstemp.Format(_T("%d"), t_itemdata->runmtmode);
    t_csline += t_cstemp + CONFIGDELIM;
    // enableflag
    t_cstemp.Format(_T("%d"), t_itemdata->enableflag);
    t_csline += t_cstemp + CONFIGDELIM;
    // message
    t_csline += t_itemdata->message + CONFIGDELIM;
    t_csline += _T("}\n");
  }
  else
  {
    // not has child
    //	filename;host;[sshv2:]port;comment;runmtmode;enableflag;message;
    t_csline = _T("\t");
    // filename
    t_csline += t_itemdata->filename + CONFIGDELIM;
    // host
    t_csline += t_itemdata->host + CONFIGDELIM;
    // port
    if (t_itemdata->sshv2)
    {
      t_cstemp.Format(_T("sshv2:%d"), t_itemdata->port);
    }
    else if (t_itemdata->directtcp)
    {
      t_cstemp.Format(_T("directtcp:%d"), t_itemdata->port);
    }
    else
    {
      t_cstemp.Format(_T("%d"), t_itemdata->port);
    }
    t_csline += t_cstemp + CONFIGDELIM;
    // comment
    t_csline += t_itemdata->comment + CONFIGDELIM;
    // runmtmode
    t_cstemp.Format(_T("%d"), t_itemdata->runmtmode);
    t_csline += t_cstemp + CONFIGDELIM;
    // enableflag
    t_cstemp.Format(_T("%d"), t_itemdata->enableflag);
    t_csline += t_cstemp + CONFIGDELIM;
    // message
    t_csline += t_itemdata->message + _T(";\n");
  }
  return t_csline;
}

// parse line to get user define itemdata
int CtewDlg::getuserdefineitemdata(CString v_csline, treeitemdata* v_ptreeitemdata)
{
  if (v_csline.Find(_T('{')) != 0)
  {
    // 获取文件名,';'结束
    v_ptreeitemdata->filename = v_csline.Left(v_csline.Find(CONFIGDELIM));
    v_csline = v_csline.Right(v_csline.GetLength() - 1 - v_csline.Find(CONFIGDELIM));
    // 获取主机名,';'结束
    v_ptreeitemdata->host = v_csline.Left(v_csline.Find(CONFIGDELIM));
    v_csline = v_csline.Right(v_csline.GetLength() - 1 - v_csline.Find(CONFIGDELIM));
    // 获取端口,';'结束
    CString t_csport = v_csline.Left(v_csline.Find(CONFIGDELIM));
    // get port by value
    if (t_csport.Find(REPLACE_VALUE) != -1)
    {
      CString t_cd = REPLACE_VALUE;
      int t_i = _ttoi(t_csport.Right(t_csport.GetLength() - t_cd.GetLength()));
      assert((t_i >= 0) && (t_i < SAVEVALUECOUNTER));
      if ((t_i >= 0) && (t_i < SAVEVALUECOUNTER))
      {
        t_csport = m_savevalue[t_i];
      }
    }
    if (t_csport.Find(SSHV2DEF) != -1)
    {
      v_ptreeitemdata->sshv2 = TRUE;
      v_ptreeitemdata->directtcp = FALSE;
      t_csport.Replace(SSHV2DEF, _T(""));
    }
    else if (t_csport.Find(DIRECTTCPDEF) != -1)
    {
      v_ptreeitemdata->sshv2 = FALSE;
      v_ptreeitemdata->directtcp = TRUE;
      t_csport.Replace(DIRECTTCPDEF, _T(""));
    }
    else
    {
      v_ptreeitemdata->sshv2 = FALSE;
      v_ptreeitemdata->directtcp = FALSE;
    }
    v_ptreeitemdata->port = _ttoi(t_csport);
    // 获取标题,';'结束
    v_csline = v_csline.Right(v_csline.GetLength() - 1 - v_csline.Find(CONFIGDELIM));
    if (v_csline.Find(CONFIGDELIM) > 0)
    {
      v_ptreeitemdata->comment = v_csline.Left(v_csline.Find(CONFIGDELIM));
    }
    else
    {
      v_ptreeitemdata->comment = v_ptreeitemdata->filename;
    }
    // 获取runmtmode,';'结束
    if (v_csline.Find(CONFIGDELIM) != -1)
    {
      v_csline = v_csline.Right(v_csline.GetLength() - 1 - v_csline.Find(CONFIGDELIM));
      if (((v_csline.Left(v_csline.Find(CONFIGDELIM))).GetLength()) != 0)
      {
        v_ptreeitemdata->runmtmode = _ttoi(v_csline.Left(v_csline.Find(CONFIGDELIM)));
      }
    }
    // 获取enableflag,';'结束
    if (v_csline.Find(CONFIGDELIM) != -1)
    {
      v_csline = v_csline.Right(v_csline.GetLength() - 1 - v_csline.Find(CONFIGDELIM));
      if (((v_csline.Left(v_csline.Find(CONFIGDELIM))).GetLength()) != 0)
      {
        v_ptreeitemdata->enableflag = _ttoi(v_csline.Left(v_csline.Find(CONFIGDELIM)));
      }
    }
    if (v_ptreeitemdata->enableflag == 0)
    {
      v_ptreeitemdata->runstatus = icondisable;
    }
    else
    {
      v_ptreeitemdata->runstatus = iconready;
    }
    // 获取信息,';'结束
    v_csline = v_csline.Right(v_csline.GetLength() - 1 - v_csline.Find(CONFIGDELIM));
    if (v_csline.Find(CONFIGDELIM) >= 0)
    {
      v_ptreeitemdata->message = v_csline.Left(v_csline.Find(CONFIGDELIM));
    }
    else
    {
      v_ptreeitemdata->message = _T("");
    }
    setmessage2savevalue(v_ptreeitemdata->message);
  }
  else
  {
    // 去除左边'{'
    v_csline = v_csline.Right(v_csline.GetLength() - 1);
    // 获取层次,';'结束
    v_ptreeitemdata->level = _ttoi(v_csline.Left(v_csline.Find(CONFIGDELIM)));
    // 获取路径,';'结束
    v_csline = v_csline.Right(v_csline.GetLength() - 1 - v_csline.Find(CONFIGDELIM));
    // level=0,取打开路径值
    if (v_ptreeitemdata->level != 0)
    {
      v_ptreeitemdata->path += (v_csline.Left(v_csline.Find(CONFIGDELIM)));
    }
    else
    {
      v_ptreeitemdata->path = m_mapappcfg[_T("lastpath")];
    }
    // 获取comment,';'结束
    if (v_csline.Find(CONFIGDELIM) != -1)
    {
      v_csline = v_csline.Right(v_csline.GetLength() - 1 - v_csline.Find(CONFIGDELIM));
      if (((v_csline.Left(v_csline.Find(CONFIGDELIM))).GetLength()) != 0)
      {
        v_ptreeitemdata->comment = (v_csline.Left(v_csline.Find(CONFIGDELIM)));
        if (v_ptreeitemdata->comment.GetLength() == 0)
        {
          v_ptreeitemdata->comment = v_ptreeitemdata->path;
        }
      }
    }
    // 获取runmtmode,';'结束
    if (v_csline.Find(CONFIGDELIM) != -1)
    {
      v_csline = v_csline.Right(v_csline.GetLength() - 1 - v_csline.Find(CONFIGDELIM));
      if (((v_csline.Left(v_csline.Find(CONFIGDELIM))).GetLength()) != 0)
      {
        v_ptreeitemdata->runmtmode = _ttoi(v_csline.Left(v_csline.Find(CONFIGDELIM)));
      }
    }
    // 获取enableflag,';'结束
    if (v_csline.Find(CONFIGDELIM) != -1)
    {
      v_csline = v_csline.Right(v_csline.GetLength() - 1 - v_csline.Find(CONFIGDELIM));
      if (((v_csline.Left(v_csline.Find(CONFIGDELIM))).GetLength()) != 0)
      {
        v_ptreeitemdata->enableflag = _ttoi(v_csline.Left(v_csline.Find(CONFIGDELIM)));
      }
    }
    if (v_ptreeitemdata->enableflag == 0)
    {
      v_ptreeitemdata->runstatus = icondisable;
    }
    else
    {
      v_ptreeitemdata->runstatus = iconready;
    }
    // 获取message,';'结束
    if (v_csline.Find(CONFIGDELIM) != -1)
    {
      v_csline = v_csline.Right(v_csline.GetLength() - 1 - v_csline.Find(CONFIGDELIM));
      if (((v_csline.Left(v_csline.Find(CONFIGDELIM))).GetLength()) != 0)
      {
        v_ptreeitemdata->message = (v_csline.Left(v_csline.Find(CONFIGDELIM)));
      }
      else
      {
        v_ptreeitemdata->message = _T("");
      }
      setmessage2savevalue(v_ptreeitemdata->message);
    }
    // clear file/host/port
    v_ptreeitemdata->filename = v_ptreeitemdata->host = _T("");
    v_ptreeitemdata->port = 0;
    v_ptreeitemdata->sshv2 = FALSE;
  }
  return 0;
}

// message to save value[0,1023]
int CtewDlg::setmessage2savevalue(CString v_message)
{
  /*  set to trmthread::InitInstance()
  */
  CString t_message = v_message;
  CString t_cs;
  t_message += _T(" ");
  for (; t_message.Find(_T("=")) != -1;)
  {
    t_message.TrimLeft();
    t_cs = t_message.Left(t_message.Find(_T(" ")));
    t_message = t_message.Right(t_message.GetLength() - t_message.Find(_T(" ")) - 1);
    assert((_ttoi(t_cs.Left(t_cs.Find(_T("=")))) >= 0) && (_ttoi(t_cs.Left(t_cs.Find(_T("=")))) < SAVEVALUECOUNTER));
    if ((_ttoi(t_cs.Left(t_cs.Find(_T("=")))) >= 0) && (_ttoi(t_cs.Left(t_cs.Find(_T("=")))) < SAVEVALUECOUNTER))
    {
      m_savevalue[_ttoi(t_cs.Left(t_cs.Find(_T("="))))] = t_cs.Right(t_cs.GetLength() - t_cs.Find(_T("=")) - 1);
    }
  }

  return 1;
}

// 文件内容忽略无用信息
int CtewDlg::getuserdefinelinebydelunuseinfo(CString v_line)
{
  // 忽略空行和注释行
  if ((v_line.GetLength() == 0) || (v_line.Find(DEF_COMMENT) == 0))
  {
    return 1;
  }
  return 0;
}


// 生成cfg tree
int CtewDlg::getuserdefinecfgtree()
{
  // 初始化dut message
  if (getproducttest())
  {
    m_mapdutmessage.clear();
    // 获取mapdutmessage
    if (getdatafile2mapdutmessage())
    {
      assert(0);
    }
    // 准备生产测试场景
    cdlgdutmsg t_dlgdutmsg;
    t_dlgdutmsg.m_ptewdlg = this;
    int t_nResponse = t_dlgdutmsg.DoModal();
    if (t_nResponse == IDOK)
    {
      CString t_csfile = m_mapappcfg[_T("apppath")] + DUT_MSG_FILE;
      CStdioFile t_csf;
      // 防止日期表项被修改
      m_mapdutmessage[_T("000Date")] = m_csstarttime;
      if (t_csf.Open(t_csfile, CFile::modeWrite | CFile::shareDenyNone | CFile::modeCreate))
      {
        getmap2file(&t_csf, &m_mapdutmessage);
        t_csf.Close();
      }
      else
      {
        AfxMessageBox(_T("生产测试模板写入失败"));
        assert(0);
      }
    }
  }
  // 清空树控件node
  setuserdefinetreeempty();
  // create tree
  getuserdefinefromfile(m_mapappcfg[_T("lastpath")], m_mapappcfg[_T("lastfile")]);
  return 0;
}

// get default itemdata
int CtewDlg::getuserdefinedefaultitemdata()
{
  treeitemdata* t_pitemdata = new treeitemdata;
  t_pitemdata->level = 0;
  t_pitemdata->path = m_mapappcfg[_T("lastpath")];
  if ((m_mapappcfg[_T("lastfile")]).GetLength() == 0)
  {
    t_pitemdata->comment = m_mapappcfg[_T("lastpath")];
  }
  else
  {
    t_pitemdata->comment = m_mapappcfg[_T("lastfile")];
  }
  t_pitemdata->runmtmode = 0;
  t_pitemdata->enableflag = 1;
  t_pitemdata->exitflag = 0;
  t_pitemdata->runstatus = 0;
  t_pitemdata->filename = _T("");
  t_pitemdata->host = _T("");
  t_pitemdata->port = 0;
  t_pitemdata->sshv2 = FALSE;
  t_pitemdata->directtcp = FALSE;
  t_pitemdata->message = _T("");
  t_pitemdata->ptrdlg = 0;
  t_pitemdata->trmtabctrlnumber = -1;
  t_pitemdata->loopcounter = 0;
  t_pitemdata->faultcounter = 0;
  t_pitemdata->logpath = m_cslogpath;
  return (int)t_pitemdata;
}

// get file version
CString CtewDlg::getfileversion()
{
  CString t_csversion;
  CString t_apppath;
  DWORD t_dwHandle;
  VS_FIXEDFILEINFO* t_pVi;
  GetModuleFileName(NULL, t_apppath.GetBuffer(_MAX_PATH), _MAX_PATH);
  t_apppath.ReleaseBuffer();
  int t_size = GetFileVersionInfoSize(t_apppath, &t_dwHandle);
  if (t_size > 0)
  {
    wchar_t* t_pbuffer = new wchar_t[t_size];
    if (GetFileVersionInfo(t_apppath, t_dwHandle, t_size, t_pbuffer))
    {
      if (VerQueryValue(t_pbuffer, _T("\\"), (LPVOID*)&t_pVi, (PUINT)&t_size))
      {
        int t_ivmsh, t_ivmsl, t_ivlsh, t_ivlsl;
        m_dlocalver.vmah = t_ivmsh = HIWORD(t_pVi->dwFileVersionMS);
        m_dlocalver.vmal = t_ivmsl = LOWORD(t_pVi->dwFileVersionMS);
        m_dlocalver.vlah = t_ivlsh = HIWORD(t_pVi->dwFileVersionLS);
        m_dlocalver.vlal = t_ivlsl = LOWORD(t_pVi->dwFileVersionLS);
        t_csversion.Format(_T("%d.%d.%d.%d"), t_ivmsh, t_ivmsl, t_ivlsh, t_ivlsl);
      }
    }
    delete[] t_pbuffer;
    t_pbuffer = NULL;
  }
  return t_csversion;
}

// 由配置map得到文件
int CtewDlg::getmap2file(CStdioFile* v_pcsf, map<CString, CString>* v_map)
{
  map<CString, CString>::iterator t_it;
  for (t_it = (*v_map).begin(); t_it != (*v_map).end(); ++t_it)
  {
    v_pcsf->WriteString(t_it->first + _T("=") + t_it->second + _T("\n"));
  }
  return 0;
}

// 由文件得到app配置map
int CtewDlg::getfile2map(CStdioFile* v_pcsf, map<CString, CString>* v_map)
{
  CString t_line, t_cs1, t_cs2;
  v_pcsf->SeekToBegin();
  for (; (v_pcsf->ReadString(t_line) != NULL);)
  {
    int t_i = 0;
    //删除行末注释部分
    t_i = t_line.Find(DEF_COMMENT);
    if (t_i > 0)
    {
      t_line.Left(t_i);
    }
    //跳过注释和空行
    if ((t_i == 0) || (t_line.GetLength() == 0))
    {
      continue;
    }
    t_i = t_line.Find(_T("="));
    if (t_i == -1)
    {
      AfxMessageBox(_T("app_cfg.dat\n格式错误:") + t_line);
      continue;
    }
    //去除空格/换行/回车/tab,""之间空格不去除
    t_cs1 = t_line.Left(t_i);
    t_cs1.TrimLeft();
    t_cs1.TrimRight();
    t_cs2 = t_line.Right(t_line.GetLength() - t_i - 1);
    t_cs2.TrimLeft();
    t_cs2.TrimRight();
    t_cs2.Replace(CONFIGDELIM, _T(""));
    (*v_map)[t_cs1] = t_cs2;
  }
  return 0;
}

// app配置map保存到文件
int CtewDlg::getcfgmapappcfg2file()
{
  CString t_csarray[] = MAPAPPCFG;
  CStdioFile t_csf;
  if (t_csf.Open(getappdir() + APP_CFG_DAT, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone))
  {
    int t_i;
    for (t_i = sizeof(t_csarray) / sizeof(*t_csarray); t_i > 0;)
    {
      t_i--;
      t_csf.WriteString(t_csarray[t_i] + _T("=") + m_mapappcfg[t_csarray[t_i]] + _T(";\n"));
    }
    t_csf.Close();
  }
  else
  {
    AfxMessageBox(_T("配置文件写入失败"));
    assert(0);
  }
  return 0;
}

// 初始化对话框界面
int CtewDlg::initdlgview()
{
  // 初始化cfg trm log 控件布局
  float t_ctrlscalearray[3][4] = DLGCTRLSIZEARRAY;
  int t_x = _ttoi(m_mapappcfg[_T("lastx")]);
  int t_y = _ttoi(m_mapappcfg[_T("lasty")]);
  for (int t_i = 0; t_i < 3; t_i++)
  {
    m_ctrlpositionarray[t_i][0] = (int)(t_ctrlscalearray[t_i][0] * (t_x - 8));
    m_ctrlpositionarray[t_i][1] = (int)(t_ctrlscalearray[t_i][1] * (t_y - 46));
    m_ctrlpositionarray[t_i][2] = (int)(t_ctrlscalearray[t_i][2] * (t_x - 8));
    m_ctrlpositionarray[t_i][3] = (int)(t_ctrlscalearray[t_i][3] * (t_y - 46));
  }
  //设置初始化窗口size为上次
  MoveWindow(CRect(0, 0, t_x, t_y));
  // 增加菜单栏
  initdlgviewmenu();
  // 增加工具栏
  initdlgviewtoolsbar();
  // 添加状态栏,V_i为1创建,0为重绘
  initdlgviewstatusbar(TRUE);
  // 初始化cfgtree ctrl
  initctrlcfgtree();
  // 初始化trm tab ctrl
  initctrltrmtab();
  // 初始化log list ctrl
  initctrlloglist();
  return 0;
}

// 增加菜单栏
int CtewDlg::initdlgviewmenu()
{
  // dlg菜单句柄
  HMENU t_hMenu;
  // 导入资源,创建菜单
  t_hMenu = LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_dlgmenu));
  // 添加到对话框
  ::SetMenu(this->GetSafeHwnd(), t_hMenu);
  // get菜单栏的指针
  m_pmenu = CMenu::FromHandle(t_hMenu);
  return 0;
}

// 增加工具栏
int CtewDlg::initdlgviewtoolsbar()
{
  // imagelist大小为32*32,颜色为32位
  m_toolbarimagelist.Create(32, 32, ILC_COLOR32 | ILC_MASK, TOOLBARBUTTONCOUNTER, TOOLBARBUTTONCOUNTER);
  CBitmap t_bmp;
  UINT t_bmparray[TOOLBARBUTTONCOUNTER] = TOOLBARICONARRAY;
  UINT t_idarray[TOOLBARBUTTONCOUNTER] = TOOLBARIDARRAY;
  int t_i;
  for (t_i = 0; t_i < TOOLBARBUTTONCOUNTER; t_i++)
  {
    // 使用现有位图为bmp赋值
    t_bmp.LoadBitmap(t_bmparray[t_i]);
    // 结合ILC_MASK,将RGB(255,255,255)也就是白色透明处理
    m_toolbarimagelist.Add(&t_bmp, RGB(255, 255, 255));
    t_bmp.DeleteObject();
  }
  // 创建工具栏控件,CreateEx函数创建的工具栏,默认有浮动按钮属性
  m_toolbar.CreateEx(this);
  // 按钮设置ID
  m_toolbar.SetButtons(t_idarray, TOOLBARBUTTONCOUNTER);
  // 函数第一个是按钮大小,第二个是图像大小,按钮必须比图像要大,具体是按钮的要比图像的宽大7或者以上,高6
  m_toolbar.SetSizes(CSize(40, 40), CSize(32, 32));
  // 获得工具条控制指针
  m_ptoolbarctrl = &(m_toolbar.GetToolBarCtrl());
  // 设置图像
  m_ptoolbarctrl->SetImageList(&m_toolbarimagelist);
  // 设置工具栏tip	
  if (m_tooltipctrl.Create(this))
  {
    CRect t_toolbuttonrect;
    for (t_i = 0; t_i < TOOLBARBUTTONCOUNTER; t_i++)
    {
      TBBUTTON t_btn;
      if ((m_ptoolbarctrl->GetButton(t_i, &t_btn)) && (t_btn.fsStyle == TBSTYLE_BUTTON))
      {
        m_ptoolbarctrl->GetItemRect(t_i, &t_toolbuttonrect);
        m_tooltipctrl.AddTool(m_ptoolbarctrl, t_btn.idCommand, t_toolbuttonrect, t_btn.idCommand);
      }
    }
    m_ptoolbarctrl->SetToolTips(&m_tooltipctrl);
  }
  if (NULL != m_ptoolbarctrl->GetToolTips())
  {
    m_ptoolbarctrl->GetToolTips()->UpdateTipText(_T("打开文件"), m_ptoolbarctrl, t_idarray[0]);
    m_ptoolbarctrl->GetToolTips()->UpdateTipText(_T("打开目录"), m_ptoolbarctrl, t_idarray[1]);
    m_ptoolbarctrl->GetToolTips()->UpdateTipText(_T("开始执行"), m_ptoolbarctrl, t_idarray[2]);
    m_ptoolbarctrl->GetToolTips()->UpdateTipText(_T("暂停执行"), m_ptoolbarctrl, t_idarray[3]);
    m_ptoolbarctrl->GetToolTips()->UpdateTipText(_T("停止执行"), m_ptoolbarctrl, t_idarray[4]);
    m_ptoolbarctrl->GetToolTips()->UpdateTipText(_T("帮助文档"), m_ptoolbarctrl, t_idarray[5]);
  }
  // 显示工具栏
  repainttoolstatus(0);
  return 0;
}
// 添加状态栏,v_i为1创建,0为重绘
int CtewDlg::initdlgviewstatusbar(int v_i)
{
  // 状态栏分割数量和比例
  CRect rect;
  GetClientRect(&rect);
  int t_iarray[STATUSBARSEPARATES];
  for (int t_i = 0; t_i < STATUSBARSEPARATES; t_i++)
  {
    t_iarray[t_i] = rect.Width() * (t_i + 1) / STATUSBARSEPARATES;
  }
  t_iarray[STATUSBARSEPARATES - 1] = -1;
  if (v_i)
  {
    m_statusbarctrl.Create(WS_CHILD | WS_VISIBLE | SBARS_TOOLTIPS | CCS_BOTTOM, rect, this, ID_dlgstatusbarctrl);
  }
  m_statusbarctrl.SetParts(STATUSBARSEPARATES, t_iarray);
  //重绘状态栏
  repainttoolstatus(ID_dlgstatusbarctrl);
  return 0;
}

// 重绘控件

//  重绘ctrl log list
int CtewDlg::repaintloglist()
{
  CRect t_rect;
  // 获取列表视图控件的位置和大小   
  m_loglist.GetClientRect(&t_rect);
  // 为列表视图控件添加3列,首项默认左对齐不可修改,因此不配置0列,应用仍按此顺序,but重绘时按0列开始计算 
  m_loglist.SetColumnWidth(0, t_rect.Width() / 8);
  m_loglist.SetColumnWidth(1, t_rect.Width() * 3 / 8);
  m_loglist.SetColumnWidth(2, t_rect.Width() / 2);
  return 0;
}

//	v_id:0表示工具栏,ID_dlgstatusbarctrl表示状态栏
int CtewDlg::repainttoolstatus(int v_id)
{
  RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, v_id);
  return 0;
}

//  重绘各控件
int CtewDlg::repaintctrl(int v_x, int v_y)
{
  // 忽略最小化
  if ((v_x != 0) && (v_y != 0))
  {
    // 窗口改变后重绘控件
    int t_ctrlarray[] = DLGCTRLARRAY;
    int t_i;
    CRect t_rect;
    CRect t_windowsrect;
    GetWindowRect(&t_windowsrect);
    float t_scalex = (float)t_windowsrect.Width() / (float)(_ttoi(m_mapappcfg[_T("lastx")]));
    float t_scaley = (float)t_windowsrect.Height() / (float)(_ttoi(m_mapappcfg[_T("lasty")]));
    for (t_i = 0; t_i < 3; t_i++)
    {
      CWnd* t_pWnd;
      // 获取控件句柄
      t_pWnd = GetDlgItem(t_ctrlarray[t_i]);
      // 判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建  
      if (t_pWnd != NULL)
      {
        m_ctrlpositionarray[t_i][0] = (int)((float)m_ctrlpositionarray[t_i][0] * t_scalex);
        m_ctrlpositionarray[t_i][1] = (int)((float)m_ctrlpositionarray[t_i][1] * t_scaley);
        m_ctrlpositionarray[t_i][2] = (int)((float)m_ctrlpositionarray[t_i][2] * t_scalex);
        m_ctrlpositionarray[t_i][3] = (int)((float)m_ctrlpositionarray[t_i][3] * t_scaley);
        switch (t_i)
        {
        case 0:
          t_rect.left = EDGEWIDTH;
          t_rect.top = TOOLBARWIDTH + EDGEWIDTH;
          t_rect.right = m_ctrlpositionarray[t_i][2] - EDGEWIDTH;
          t_rect.bottom = v_y - STATUSBARWIDTH - EDGEWIDTH;
          break;
        case 1:
          t_rect.left = m_ctrlpositionarray[t_i][0] + EDGEWIDTH;
          t_rect.top = m_ctrlpositionarray[t_i][1] + TOOLBARWIDTH + EDGEWIDTH;
          t_rect.right = v_x - EDGEWIDTH;
          t_rect.bottom = m_ctrlpositionarray[t_i][3] - EDGEWIDTH;
          break;
        case 2:
          t_rect.left = m_ctrlpositionarray[t_i][0] + EDGEWIDTH;
          t_rect.top = m_ctrlpositionarray[t_i][1] + EDGEWIDTH;
          t_rect.right = v_x - EDGEWIDTH;
          t_rect.bottom = v_y - STATUSBARWIDTH - EDGEWIDTH;
          break;
        }
        // 设置控件大小 
        t_pWnd->MoveWindow(&t_rect);
      }
    }
    // tabctrl关联dlg重绘/repaint ctrl trmdlg
    if (m_pcuritemdata != NULL)
    {
      repaintctrltrmdlg(m_pcuritemdata->node);
    }
    // dlg改变后重绘log list,包含分隔比例
    if (IsWindow(m_loglist.m_hWnd))
    {
      // 重绘log list
      repaintloglist();
    }
    // dlg改变后重绘状态栏,包含分隔比例
    if (IsWindow(m_statusbarctrl.m_hWnd))
    {
      // 添加状态栏,v_i为1创建,0为重绘
      initdlgviewstatusbar(FALSE);
    }
    // get dlg 界面控件间隙
    getdlgctrlscale();
  }
  return 0;
}

// tabctrl关联dlg重绘/repaint ctrl trmdlg
int CtewDlg::repaintctrltrmdlg(HTREEITEM v_node)
{
  struct treeitemdata* t_itemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(v_node));
  assert(t_itemdata);
  if (t_itemdata->ptrdlg != 0)
  {
    CRect t_tabdlgrect;
    // 终端tab control指针
    m_trmtab.GetClientRect(&t_tabdlgrect);
    t_tabdlgrect.top += 2;
    t_tabdlgrect.left += 2;
    t_tabdlgrect.bottom -= 25;
    t_tabdlgrect.right -= 2;
    (reinterpret_cast<ctrmdlg*>(t_itemdata->ptrdlg))->m_rc = t_tabdlgrect;
    (reinterpret_cast<ctrmdlg*>(t_itemdata->ptrdlg))->SendMessage(WM_trmdlgresize, NULL, NULL);
  }
  repainttoolstatus(ID_dlgstatusbarctrl);
  return 0;
}

// 重载系统消息处理,拦截onclose消息
BOOL CtewDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
  // TODO: 在此添加专用代码和/或调用基类
  if (message == WM_CLOSE)
  {
    // 询问是否关闭,发送关闭窗口消息,进入关闭对话框的代码
    if (AfxMessageBox(_T("确认关闭程序?"), MB_YESNO) == IDYES)
    {
      // tcp server exit
      m_ptcpserver->PostThreadMessage(WM_QUIT, 0, 0);
      // udp exit
      m_pudp->PostThreadMessage(WM_QUIT, 0, 0);
      // app exit
      CString t_cs = _T("[tew]close");
      this->SendMessage(WM_appdlgctrladdloglist, (WPARAM)t_cs.GetBuffer(0), 0);
      t_cs.ReleaseBuffer();
      // 遍历树,dlgstop
      setuserdefinetreeitem(m_cfgtree.GetRootItem(), 4);
      // app配置map保存到文件
      getcfgmapappcfg2file();
      // close app log file
      m_csfapplogfile.Close();
      // 等待清理完成
      Sleep(APPENDDELAY);
      return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
    }
    return TRUE;
  }
  else
  {
    return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
  }
}

// dialog exit
void CtewDlg::Ondlgexit()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  SendMessage(WM_CLOSE);
}

// dialog size
void CtewDlg::OnSize(UINT nType, int cx, int cy)
{
  CDialogEx::OnSize(nType, cx, cy);

  // TODO: ÔÚ´Ë´¦Ìí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂë
  if ((cx != 0) && (cy != 0))
  {
    // dlg改变后重绘各控件,注意会调用mapappcfg
    repaintctrl(cx, cy);
    // resize后记录size to map
    getsize2mapappcfg();
  }
}

// dlg最小size X&Y
void CtewDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
  // TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
  // set dlg size min
  lpMMI->ptMinTrackSize.x = DLGMINX;
  lpMMI->ptMinTrackSize.y = DLGMINY;
  CDialogEx::OnGetMinMaxInfo(lpMMI);
}

// log list menu popup
void CtewDlg::OnNMRClickloglist(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
  // TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
  CMenu menu;
  POINT pt = { 0 };
  GetCursorPos(&pt);// 得到鼠标点击位置 
  menu.LoadMenu(IDR_loglistmenu);// 菜单资源ID 
  // m_list是CListCtrl对象,这里的消息发到this 
  CMenu* popup = menu.GetSubMenu(0);
  popup->TrackPopupMenu(0, pt.x, pt.y, this);
  *pResult = 0;
}

// 遍历,exit
int CtewDlg::runtewdlgexit(HTREEITEM v_node)
{
  Sleep(100);
  runtewdlgclear(v_node);
  // menu open
  setcmdinopen();
  return 0;
}

// 遍历,clear
int CtewDlg::runtewdlgclear(HTREEITEM v_node)
{
  // close file
  m_csfcsv.Close();
  // 删除标签控件中所有的项
  if (m_trmtab.DeleteAllItems() == 0)
  {
    assert(0);
  }
  m_pcuritemdata = 0;
  return 0;
}

// 准备开始
int CtewDlg::runtewdlgperstart()
{
  // 清理报告字段
  m_mapmodulefield.clear();
  // 初始化log目录
  pathinitlogpath(m_cslogpath);
  // 初始化csv文件  -- m_cfgtree.GetRootItem()->logpath
  pathinitcsvfile();
  // 初始化终端页面数组
  inittrmtabpageitemdataarray();
  HTREEITEM t_node;
  t_node = m_cfgtree.GetRootItem();
  //	3-scan script filename
  setuserdefinetreeitem(t_node, 3);
  //	5-dlgstart icon&text初始化
  setuserdefinetreeitem(t_node, 5);
  //  6-trm log path init
  setuserdefinetreeitem(t_node, 6);
  // 遍历,并发
  runtewdlgstart(t_node);
  return 0;
}

// 遍历,并行和顺序化兼容,节点信息runmtflag表示本层次串并执行方式
//	写这段代码的时候，只有老天和我知道它是干嘛的
//	现在，只有老天知道
int CtewDlg::runtewdlgstart(HTREEITEM v_node)
{
  int t_ireturn = 0;
  if (v_node != NULL)
  {
    struct treeitemdata* t_pitemdata;
    HTREEITEM t_hChildItem;
    t_pitemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(v_node));
    if (t_pitemdata != NULL)
    {
      if ((t_pitemdata->enableflag == 0) || (t_pitemdata->exitflag == 1))
      {
        if ((t_pitemdata->enableflag == 0) && (xavietex))
        {
          CString t_cs = _T("disable ") + t_pitemdata->comment;
          this->SendMessage(WM_appdlgctrladdloglist, (WPARAM)t_cs.GetBuffer(0), 0);
          t_cs.ReleaseBuffer();
        }
        t_pitemdata->exitflag = 1;
        // 访问兄弟节点
        t_hChildItem = m_cfgtree.GetNextSiblingItem(v_node);
        t_ireturn = runtewdlgstart(t_hChildItem);
        return t_ireturn;
      }
      else
      {
        CString t_cs = _T("start ") + t_pitemdata->comment;
        this->SendMessage(WM_appdlgctrladdloglist, (WPARAM)t_cs.GetBuffer(0), 0);
        t_cs.ReleaseBuffer();
        if ((t_pitemdata->filename.GetLength() != 0) && (t_pitemdata->runstatus == iconready))
        {
          // 使用信号量进行PV操作
          WaitForSingleObject(m_hsemaphore, INFINITE);
          // 创建trmdlg
          ctrmdlg* t_trmdlg = new ctrmdlg;
          t_pitemdata->ptrdlg = (int)t_trmdlg;
          (reinterpret_cast<ctrmdlg*>(t_pitemdata->ptrdlg))->m_pitemdata = t_pitemdata;
          t_pitemdata->trmtabctrlnumber = m_trmtab.GetItemCount();
          m_ptrmtabpageitemdataarray[t_pitemdata->trmtabctrlnumber] = t_pitemdata;
          m_trmtab.InsertItem(t_pitemdata->trmtabctrlnumber, t_pitemdata->comment);
          t_trmdlg->m_ptewdlg = this;
          // 创建一个非模态对话框 
          t_trmdlg->Create(IDD_trmdlg, &m_trmtab);
          // 刷新当前节点
          setcurtreenode(v_node);
          // 终端实例界面重绘
          if (m_pcuritemdata != NULL)
          {
            repaintctrltrmdlg(m_pcuritemdata->node);
          }
          t_ireturn = 1;
          // 修改当前和父节点图标
          HTREEITEM t_node;
          t_node = m_cfgtree.GetParentItem(v_node);
          for (; t_node != NULL;)
          {
            struct treeitemdata* t_pid;
            t_pid = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_node));
            if (t_pid->runstatus == iconready)
            {
              t_pid->runstatus = iconrwn;
              this->SendMessage(WM_appdlgnodepropertymodify, (WPARAM)t_pid, NULL);
            }
            t_node = m_cfgtree.GetParentItem(t_node);
          }
          // 使用信号量进行PV操作
          ReleaseSemaphore(m_hsemaphore, 1, NULL);
        }
      }
      if (t_pitemdata->runmtmode == 0)
      {
        // 顺序化方式
        // 注意禁止空节点，既非脚本节点不存在脚本节点的子节点
        t_hChildItem = m_cfgtree.GetChildItem(v_node);
        t_ireturn = runtewdlgstart(t_hChildItem);
      }
      else if (t_pitemdata->runmtmode == 1)
      {
        // 并行
        t_hChildItem = m_cfgtree.GetChildItem(v_node);
        runtewdlgstart(t_hChildItem);
        t_hChildItem = m_cfgtree.GetNextSiblingItem(v_node);
        t_ireturn = runtewdlgstart(t_hChildItem);
      }
      else
      {
        assert(0);
      }
    }
  }
  // run over 
  if ((NULL == v_node) && (m_trmtab.GetItemCount() == 0))
  {
    // 保存测试结果记录,包含ng行描述
    setnginformation();
    runtewdlgexit(v_node);
    if (getproducttest())
    {
      // 生产测试结果确认
      getproducttestresultmsg();
    }
    if (this->m_mapappcfg[_T("loop")] == _T("1"))
    {
      // start
      OnDlgstart();
    }
  }
  return t_ireturn;
}

// 生产测试结果确认
int CtewDlg::getproducttestresultmsg()
{
  // 准备生产测试结果确认
  cdlgproductresult t_dlgproductresult;
  t_dlgproductresult.m_ptewdlg = this;
  CString t_csdirname;
  int t_nResponse = t_dlgproductresult.DoModal();
  if (t_nResponse == IDOK)
  {
    // 创建测试报告
    if ((m_mapdutmessage[_T("002ProductSN")].GetLength() != 0) || (m_mapdutmessage[_T("003SN1")].GetLength() != 0))
    {
      // 报告文件路径
      t_csdirname = m_mapdutmessage[_T("002ProductSN")];
      if (t_csdirname.GetLength() == 0)
      {
        t_csdirname = m_mapdutmessage[_T("003SN1")];
      }
      // 002ProductSN or 003SN1 生成生产测试报告
      setproducttestresultmsg(t_csdirname);
      // 009SN2 生成生产测试报告
      if (((m_mapdutmessage[_T("002ProductSN")].GetLength() == 0) && (m_mapdutmessage[_T("009SN2")].GetLength() != 0)))
      {
        // 报告文件路径
        t_csdirname = m_mapdutmessage[_T("009SN2")];
        // 009SN2 生成生产测试报告
        setproducttestresultmsg(t_csdirname);
      }
    }
    else
    {
      AfxMessageBox(_T("ProductSN or SN1 error"));
    }
  }
  else if (t_nResponse == IDCANCEL)
  {
  }
  return 0;
}

// 保存测试结果记录,包含ng行描述
int CtewDlg::setnginformation()
{
  CStdioFile t_csf;
  if (t_csf.Open(m_cscsvpath + RESULT_FILE, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone))
  {
    setuserdefinetreeitem(m_cfgtree.GetRootItem(), 8, &t_csf);
    t_csf.Close();
  }
  else
  {
    assert(0);
  }
  return 0;
}

// 生成生产测试报告
int CtewDlg::setproducttestresultmsg(CString v_csdirname)
{
  CString t_csresultpath;
  t_csresultpath = PRODUCT_RESULT_PATH + v_csdirname + _T("\\");
  // 创建目录
  pathmkdir(t_csresultpath);
  // 计算文件数量t_i
  CFileFind t_f;
  CString t_cs;
  BOOL t_bcfg = t_f.FindFile(t_csresultpath + _T("*.txt"));
  int t_i = 0;
  while (t_bcfg)
  {
    t_bcfg = t_f.FindNextFile();
    t_i++;
  }
  t_f.Close();
  // make file name
  CString t_csresultfile;
  t_csresultfile.Format(_T("%d"), t_i);
  t_csresultfile = t_csresultpath + v_csdirname + _T("_") + t_csresultfile + _T(".txt");
  CStdioFile t_csf;
  // file operate
  t_csf.Open(t_csresultfile, CFile::modeWrite | CFile::shareDenyNone | CFile::modeCreate);
  // write
  map<CString, CString>::iterator t_it;
  CString t_csvalue;
  for (t_it = m_mapdutmessage.begin(); t_it != m_mapdutmessage.end(); ++t_it)
  {
    if (t_it->second.GetLength() == 0)
    {
      t_csvalue = _T("[N/A]");
    }
    else
    {
      t_csvalue = t_it->second;
    }
    t_csf.WriteString(t_it->first + _T("=") + t_csvalue + _T("\n"));
  }
  for (t_it = m_mapmodulefield.begin(); t_it != m_mapmodulefield.end(); ++t_it)
  {
    if (t_it->second == _T("0"))
    {
      t_csvalue = _T("OK");
    }
    else
    {
      t_csvalue = _T("NG[") + t_it->second + _T("]");
    }
    t_csf.WriteString(t_it->first + _T("=") + t_csvalue + _T("\n"));
    t_csf.Flush();
  }
  t_csf.Close();
  return 0;
}

// 获取生产测试场景
int CtewDlg::getproducttest()
{
  if (m_mapappcfg[_T("producttest")] == _T("1"))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

// 获取database场景
int CtewDlg::getdatabase()
{
  if (m_mapappcfg[_T("database")] == _T("1"))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

// 刷新当前节点
int CtewDlg::setcurtreenode(HTREEITEM v_node)
{
  assert(v_node);
  volatile struct treeitemdata* t_pitemdataold;
  struct treeitemdata* t_pitemdatanew;
  if (m_pcuritemdata == NULL)
  {
    m_pcuritemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(v_node));
  }
  t_pitemdataold = m_pcuritemdata;
  t_pitemdatanew = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(v_node));
  assert(t_pitemdataold);
  assert(t_pitemdatanew);
  if (t_pitemdatanew->ptrdlg != NULL)
  {
    if (t_pitemdataold->ptrdlg != NULL)
    {
      (reinterpret_cast<ctrmdlg*>(t_pitemdataold->ptrdlg))->ShowWindow(SW_HIDE);
    }
    repaintctrltrmdlg(t_pitemdatanew->node);
    (reinterpret_cast<ctrmdlg*>(t_pitemdatanew->ptrdlg))->ShowWindow(SW_SHOW);
    m_pcuritemdata = t_pitemdatanew;
    m_trmtab.SetCurSel(m_pcuritemdata->trmtabctrlnumber);
  }
  return 0;
}

// 使能或禁止菜单
//  menu表示从0开始的主菜单序号
//  submenu表示子菜单ID号,为0表示操作对象为主菜单
//  flag为MF_GRAYED或MF_ENABLED
//  返回TRUE正常或FALSE异常
int CtewDlg::setmenuaction(int v_menu, int v_submenu, int v_flag)
{
  assert(v_menu >= 0);
  if (v_submenu == 0)
  {
    m_pmenu->EnableMenuItem(v_menu, MF_BYPOSITION | v_flag);	//禁用主菜单
    return TRUE;
  }
  else
  {
    m_pmenu->GetSubMenu(v_menu)->EnableMenuItem(v_submenu, v_flag);	//menu表示从0开始的主菜单序号,submenu表示子菜单ID号,MF_GRAYED表示禁用并且为灰色
    return TRUE;
  }
  return FALSE;
}

// 工具栏使能或禁止
// id表示按钮ID号
// flag为false或ture
// 返回TRUE正常或FALSE异常
int CtewDlg::settoolbaraction(int v_id, BOOL v_flag)
{
  return m_ptoolbarctrl->EnableButton(v_id, v_flag);
}

// deny all command
int CtewDlg::setallcmddeny()
{
  setmenuaction(0, ID_dlgopenfile, MF_GRAYED);
  setmenuaction(0, ID_dlgopendir, MF_GRAYED);
  setmenuaction(0, ID_dlgsave, MF_GRAYED);
  setmenuaction(0, ID_dlgsaveas, MF_GRAYED);
  setmenuaction(1, ID_dlgstart, MF_GRAYED);
  setmenuaction(1, ID_dlgpause, MF_GRAYED);
  setmenuaction(1, ID_dlgstop, MF_GRAYED);
  setmenuaction(2, ID_dlgenablenode, MF_GRAYED);
  setmenuaction(2, ID_dlgdisablenode, MF_GRAYED);
  setmenuaction(2, ID_dlgquarynode, MF_GRAYED);
  settoolbaraction(ID_dlgopenfile, false);
  settoolbaraction(ID_dlgopendir, false);
  settoolbaraction(ID_dlgstart, false);
  settoolbaraction(ID_dlgpause, false);
  settoolbaraction(ID_dlgstop, false);
  return 0;
}

// ready
int CtewDlg::setcmdinready()
{
  setmenuaction(0, ID_dlgopenfile, MF_ENABLED);
  setmenuaction(0, ID_dlgopendir, MF_ENABLED);
  setmenuaction(0, ID_dlgsave, MF_GRAYED);
  setmenuaction(0, ID_dlgsaveas, MF_GRAYED);
  setmenuaction(1, ID_dlgstart, MF_GRAYED);
  setmenuaction(1, ID_dlgpause, MF_GRAYED);
  setmenuaction(1, ID_dlgstop, MF_GRAYED);
  setmenuaction(2, ID_dlgenablenode, MF_GRAYED);
  setmenuaction(2, ID_dlgdisablenode, MF_GRAYED);
  setmenuaction(2, ID_dlgquarynode, MF_GRAYED);
  settoolbaraction(ID_dlgopenfile, true);
  settoolbaraction(ID_dlgopendir, true);
  settoolbaraction(ID_dlgstart, false);
  settoolbaraction(ID_dlgpause, false);
  settoolbaraction(ID_dlgstop, false);
  return 0;
}

// open
int CtewDlg::setcmdinopen()
{
  setmenuaction(0, ID_dlgopenfile, MF_ENABLED);
  setmenuaction(0, ID_dlgopendir, MF_ENABLED);
  setmenuaction(0, ID_dlgsave, MF_ENABLED);
  setmenuaction(0, ID_dlgsaveas, MF_ENABLED);
  setmenuaction(1, ID_dlgstart, MF_ENABLED);
  setmenuaction(1, ID_dlgpause, MF_GRAYED);
  setmenuaction(1, ID_dlgstop, MF_GRAYED);
  setmenuaction(2, ID_dlgenablenode, MF_ENABLED);
  setmenuaction(2, ID_dlgdisablenode, MF_ENABLED);
  setmenuaction(2, ID_dlgquarynode, MF_ENABLED);
  settoolbaraction(ID_dlgopenfile, true);
  settoolbaraction(ID_dlgopendir, true);
  settoolbaraction(ID_dlgstart, true);
  settoolbaraction(ID_dlgpause, false);
  settoolbaraction(ID_dlgstop, false);
  return 0;
}

// start
int CtewDlg::setcmdinstart()
{
  setmenuaction(0, ID_dlgopenfile, MF_GRAYED);
  setmenuaction(0, ID_dlgopendir, MF_GRAYED);
  setmenuaction(0, ID_dlgsave, MF_GRAYED);
  setmenuaction(0, ID_dlgsaveas, MF_GRAYED);
  setmenuaction(1, ID_dlgstart, MF_GRAYED);
  setmenuaction(1, ID_dlgpause, MF_ENABLED);
  setmenuaction(1, ID_dlgstop, MF_ENABLED);
  setmenuaction(2, ID_dlgenablenode, MF_GRAYED);
  setmenuaction(2, ID_dlgdisablenode, MF_GRAYED);
  setmenuaction(2, ID_dlgquarynode, MF_GRAYED);
  settoolbaraction(ID_dlgopenfile, false);
  settoolbaraction(ID_dlgopendir, false);
  settoolbaraction(ID_dlgstart, false);
  settoolbaraction(ID_dlgpause, true);
  settoolbaraction(ID_dlgstop, true);
  return 0;
}

// set用户define node
HTREEITEM CtewDlg::setuserdefinetreenode(HTREEITEM v_fathernode, treeitemdata* v_ptreeitemdata)
{
  // 节点句柄 
  assert(v_ptreeitemdata);
  HTREEITEM t_hnode;
  struct treeitemdata* t_pitemdata;
  if (v_fathernode)
  {
    // 非root节点
    t_pitemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(v_fathernode));
    // cmp node level 
    assert((t_pitemdata->level >= 0) && (v_ptreeitemdata->level >= 0));
    // target node level match
    if ((t_pitemdata->level + 1) == v_ptreeitemdata->level)
    {
      // 根据当前节点信息是否包含文件名确定下一节点level
      if (t_pitemdata->filename.GetLength() != 0)
      {
        t_hnode = m_cfgtree.GetParentItem(m_cfgtree.GetParentItem(v_fathernode));
        t_hnode = setuserdefinetreenode(t_hnode, v_ptreeitemdata);
      }
      else
      {
        // 创建非根节点
        t_hnode = m_cfgtree.InsertItem(v_ptreeitemdata->comment, v_ptreeitemdata->runstatus, v_ptreeitemdata->runstatus, v_fathernode, TVI_LAST);
        // 配置节点携带信息,指针引用
        v_ptreeitemdata->node = t_hnode;
        m_cfgtree.SetItemData(t_hnode, (DWORD)v_ptreeitemdata);
      }
    }
    // target node level big
    else if ((t_pitemdata->level + 1) < v_ptreeitemdata->level)
    {
      // 根据当前节点信息是否包含文件名确定下一节点level
      if (t_pitemdata->filename.GetLength() != 0)
      {
        t_hnode = m_cfgtree.GetParentItem(m_cfgtree.GetParentItem(v_fathernode));
      }
      else
      {
        struct treeitemdata* t_ptemp;
        t_ptemp = new treeitemdata;
        *t_ptemp = *t_pitemdata;
        t_ptemp->level += 1;
        t_hnode = m_cfgtree.InsertItem(t_ptemp->comment, t_ptemp->runstatus, t_ptemp->runstatus, v_fathernode, TVI_LAST);
        t_ptemp->node = t_hnode;
        m_cfgtree.SetItemData(t_hnode, (DWORD)t_ptemp);
      }
      t_hnode = setuserdefinetreenode(t_hnode, v_ptreeitemdata);
    }
    // target node level small or equal
    else
    {
      t_hnode = v_fathernode;
      t_pitemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_hnode));
      if (t_pitemdata->filename.GetLength() != 0)
      {
        t_hnode = m_cfgtree.GetParentItem(t_hnode);
        t_pitemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_hnode));
        if (v_ptreeitemdata->filename.GetLength() == 0)
        {
          t_hnode = m_cfgtree.GetParentItem(t_hnode);
          t_pitemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_hnode));
        }
      }
      for (; (t_pitemdata->level) >= (v_ptreeitemdata->level);)
      {
        t_hnode = m_cfgtree.GetParentItem(t_hnode);
        t_pitemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_hnode));
      }
      t_hnode = setuserdefinetreenode(t_hnode, v_ptreeitemdata);
    }
  }
  else
  {
    // 配置根节点携带信息,指针引用
    if (v_ptreeitemdata->level != 0)
    {
      t_pitemdata = reinterpret_cast<treeitemdata*>(getuserdefinedefaultitemdata());
      // 创建根节点
      t_hnode = m_cfgtree.InsertItem(t_pitemdata->comment, t_pitemdata->runstatus, t_pitemdata->runstatus);
      t_pitemdata->node = t_hnode;
      m_cfgtree.SetItemData(t_hnode, (DWORD)t_pitemdata);
      t_hnode = setuserdefinetreenode(t_hnode, v_ptreeitemdata);
    }
    else
    {
      // 创建根节点
      t_hnode = m_cfgtree.InsertItem(v_ptreeitemdata->comment, v_ptreeitemdata->runstatus, v_ptreeitemdata->runstatus);
      v_ptreeitemdata->node = t_hnode;
      t_pitemdata = v_ptreeitemdata;
      m_cfgtree.SetItemData(t_hnode, (DWORD)t_pitemdata);
    }
  }
  return t_hnode;
}

// 清空树控件node
int CtewDlg::setuserdefinetreeempty()
{
  // clear tree from root node
  m_pcuritemdata = 0;
  setuserdefinetreeitem(m_cfgtree.GetRootItem(), 0);
  if (m_cfgtree.DeleteAllItems() == 0)
  {
    assert(0);
  }
  return 0;
}

// 如果这段代码好用，那它是xaviet写的。
//	如果不好用，我不知道是谁写的。
// 遍历树控件节点
//	0-删除携带信息指针，清空节点携带信息引用
//	1-展开节点
//	2-save
//	3-scan script filename
//	4-dlgstop
//	5-dlgstart icon&text初始化
//  6-trm log path init
//	7-replace $_
//	8-保存测试结果记录,包含ng行描述
int CtewDlg::setuserdefinetreeitem(HTREEITEM v_nodea, int v_i, CStdioFile* v_pcsf)
{
  struct treeitemdata* t_pid;
  CString t_cs, t_cstemp;
  if (v_nodea != NULL)
  {
    t_pid = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(v_nodea));
    if (t_pid != NULL)
    {
      switch (v_i)
      {
      case 0:
        //	0-删除携带信息指针，清空节点携带信息引用
        delete t_pid;
        m_cfgtree.SetItemData(v_nodea, NULL);
        break;
      case 1:
        //	1-展开节点
        m_cfgtree.Expand(v_nodea, TVE_EXPAND);
        break;
      case 2:
        //	2-save
        assert(v_pcsf);
        v_pcsf->WriteString(getuserdefineline(v_nodea));
        break;
      case 3:
        //	3-scan script filename
        if (t_pid->filename.GetLength() != 0)
        {
          CFileFind t_fFind;
          CString t_cspath = t_pid->path + t_pid->filename;
          if (t_fFind.FindFile(t_cspath))
          {
          }
          else
          {
            t_pid->enableflag = 0;
            CString t_csmsg;
            t_csmsg.Format(_T("%s is not exist,\ndisable node %s!"), t_cspath, t_pid->comment);
            AfxMessageBox(t_csmsg);
          }
          t_fFind.Close();
        }
        break;
      case 4:
        //	4-dlgstop
        if ((t_pid->ptrdlg != 0) && (t_pid->exitflag != 1))
        {
          (reinterpret_cast<ctrmdlg*>(t_pid->ptrdlg))->SendMessage(WM_trmdlgstop, 0, 0);
          t_pid->exitflag = 1;
          t_pid->trmtabctrlnumber = -1;
        }
        break;
      case 5:
        //	5-dlgstart
        t_pid->exitflag = 0;
        t_pid->faultcounter = 0;
        if (t_pid->enableflag == 0)
        {
          t_pid->runstatus = icondisable;
        }
        else
        {
          t_pid->runstatus = iconready;
        }
        this->SendMessage(WM_appdlgnodepropertymodify, (WPARAM)t_pid, NULL);
        break;
      case 6:
        //  6-trm log path init
        if (t_pid->node != m_cfgtree.GetRootItem())
        {
          struct treeitemdata* t_pfatherid;
          CString t_csdiff;
          t_pfatherid = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(m_cfgtree.GetParentItem(t_pid->node)));
          t_csdiff = t_pid->path.Right(t_pid->path.GetLength() - t_pfatherid->path.GetLength());
          t_pid->logpath = t_pfatherid->logpath + t_csdiff;
        }
        pathmkdir(t_pid->logpath);
        break;
      case 7:
        //	7-replace $_
        if (t_pid->filename.GetLength() != 0)
        {
          getstring4replacelabel(&(t_pid->host));
        }
        break;
      case 8:
        //	8-保存测试结果记录,包含ng行描述,忽略disable节点
        if (t_pid->enableflag == 1)
        {
          t_cstemp.Format(_T("%d"), t_pid->level);
          t_cs = _T("{") + t_cstemp + _T(";") + t_pid->comment + _T(";");
          t_cstemp.Format(_T("NG(%d)"), t_pid->faultcounter);
          t_cs += t_cstemp + _T(";") + t_pid->filename + _T(";}\n");
          t_cstemp.Empty();
          for (int t_i = 0; t_i < t_pid->level; t_i++)
          {
            t_cstemp += _T("  ");
          }
          t_cs = t_cstemp + t_cs;
          if (t_pid->nginformation.GetLength() > 0)
          {
            t_cs += t_pid->nginformation;
          }
          v_pcsf->WriteString(t_cs);
          v_pcsf->Flush();
        }
        break;
      default:
        break;
      }
    }
    if (m_cfgtree.ItemHasChildren(v_nodea) != NULL)
    {
      HTREEITEM t_hChildItem = m_cfgtree.GetChildItem(v_nodea);
      while (t_hChildItem != NULL)
      {
        setuserdefinetreeitem(t_hChildItem, v_i, v_pcsf); //递归遍历子节点     
        t_hChildItem = m_cfgtree.GetNextSiblingItem(t_hChildItem);
      }
    }
  }
  return 0;
}

// 模拟鼠标点击
int CtewDlg::setmouseclick(CPoint v_pt)
{
  CPoint t_opt;
  GetCursorPos(&t_opt);
  SetCursorPos(v_pt.x, v_pt.y);
  // mouse_event   发送模拟鼠标消息到该矩形内一点。
  mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
  mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
  SetCursorPos(t_opt.x, t_opt.y);
  return 0;
}

// 选中tabpage，关联trmdlg和cfgtree
int CtewDlg::settrmtabpage(int v_i)
{
  if (m_pcuritemdata != NULL)
  {
    if (v_i != m_pcuritemdata->trmtabctrlnumber)
    {
      setcurtreenode(m_ptrmtabpageitemdataarray[v_i]->node);
      CRect t_rcItem;
      // 取得该ITEM的坐标。CTreeCtrl::GetItemRect
      m_cfgtree.GetItemRect(m_pcuritemdata->node, &t_rcItem, TRUE);
      // 判断坐标在窗口范围
      CRect t_rcdlg;
      m_cfgtree.GetClientRect(&t_rcdlg);
      if (t_rcItem.bottom < t_rcdlg.bottom)
      {
        // 转换到屏幕坐标   CTreeCtrl::ClientToScreen
        CPoint t_pt;
        t_pt.x = t_rcItem.left + t_rcItem.Width() / 2;
        t_pt.y = t_rcItem.top + t_rcItem.Height() / 2;
        ::ClientToScreen(m_cfgtree.m_hWnd, &t_pt);
        // 模拟鼠标点击
        setmouseclick(t_pt);
      }
    }
  }
  return 0;
}


// save user define file
int  CtewDlg::setuserdefinefile(CString v_filename)
{
  // 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
  m_mapappcfg[_T("lastfile")] = getfilefrompath(v_filename);
  // path包含文件名
  m_mapappcfg[_T("lastpath")] = getdirfrompath(v_filename);
  CStdioFile t_csf;
  if (t_csf.Open(v_filename, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone))
  {
    // 遍历树控件节点,0-删除携带信息指针,1-展开节点,2-save
    setuserdefinetreeitem(m_cfgtree.GetRootItem(), 2, &t_csf);
    t_csf.Close();
  }
  else
  {
    //assert(0);
    AfxMessageBox(_T("生产测试环境请不要修改测试配置!"));
  }
  return 0;
}

// 检查节点完成状态,延伸至父节点
int  CtewDlg::setuserdefinenodeexitflag(treeitemdata* v_ptreeitemdata)
{
  HTREEITEM t_node;
  //	int t_runstatus=iconrwn;
    // 获取第一个兄弟节点
  t_node = m_cfgtree.GetParentItem(v_ptreeitemdata->node);
  if (t_node != NULL)
  {
    int t_exitflag = 1;
    treeitemdata* t_pid;
    t_node = m_cfgtree.GetChildItem(t_node);
    assert(t_node);
    for (; t_node != NULL;)
    {
      t_pid = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_node));
      if ((t_pid->exitflag != 1) && (t_pid->enableflag == 1))
      {
        t_exitflag = 0;
      }
      t_node = m_cfgtree.GetNextSiblingItem(t_node);
    }
    t_node = m_cfgtree.GetParentItem(v_ptreeitemdata->node);
    t_pid = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_node));
    assert(t_pid);
    if (t_exitflag == 1)
    {
      t_pid->exitflag = 1;
      setuserdefinenodeexitflag(t_pid);
      // 刷新节点图标
      if (t_pid->faultcounter > 0)
      {
        t_pid->runstatus = iconcwf;
      }
      else
      {
        t_pid->runstatus = iconcwn;
      }
    }
    else if (t_exitflag == 0)
    {
      t_pid->exitflag = 0;
      // 刷新节点图标
      if (t_pid->faultcounter > 0)
      {
        t_pid->runstatus = iconrwf;
      }
      else
      {
        t_pid->runstatus = iconrwn;
      }
    }
    else
    {
      assert(0);
    }
    this->SendMessage(WM_appdlgnodepropertymodify, (WPARAM)t_pid);
  }
  return 0;
}

// 树控件携带信息enableflag
//	v_flag:1-enable;0-disable
int CtewDlg::modifytreeallitemdatarunmode(HTREEITEM v_node, int v_flag)
{
  if (v_node != NULL)
  {
    if ((m_cfgtree.GetItemData(v_node)) != NULL)
    {
      // 修改节点信息中runmode
      if (v_flag == TRUE)
      {

        HTREEITEM t_node = v_node;
        for (; t_node != NULL;)
        {
          (reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_node)))->enableflag = TRUE;
          m_cfgtree.SetItemImage(t_node, iconready, iconready);
          t_node = m_cfgtree.GetParentItem(t_node);
        }
      }
      else if (v_flag == FALSE)
      {
        (reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(v_node)))->enableflag = FALSE;
        m_cfgtree.SetItemImage(v_node, icondisable, icondisable);
      }
      else
      {
        assert(0);
      }
    }
    if (m_cfgtree.ItemHasChildren(v_node) != NULL)
    {
      HTREEITEM t_hChildItem = m_cfgtree.GetChildItem(v_node);
      while (t_hChildItem != NULL)
      {
        modifytreeallitemdatarunmode(t_hChildItem, v_flag); //递归遍历子节点     
        t_hChildItem = m_cfgtree.GetNextSiblingItem(t_hChildItem);
      }
    }
  }
  return 0;
}

// set cfg tree property
int CtewDlg::setuserdefinenodeproperty(HTREEITEM v_node)
{
  // 获取当前选中节点的句柄   
  HTREEITEM t_hItem;
  if (v_node == 0)
  {
    t_hItem = m_cfgtree.GetSelectedItem();
  }
  else
  {
    t_hItem = v_node;
  }
  assert(t_hItem);
  // 检查节点有效,可以进行节点属性编辑
  if (t_hItem != 0)
  {
    // 模态创建节点属性对话框
    cnodepropertydlg t_nodepropertydlg;
    // 获取选中节点信息赋值给节点属性对话框实例的公共变量m_pitemdata
    t_nodepropertydlg.m_pitemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_hItem));
    t_nodepropertydlg.m_pappdlg = this;
    if (t_nodepropertydlg.DoModal() == IDOK)
    {
    }
  }
  return 0;
}

// 文件/目录支持拖拽
void CtewDlg::OnDropFiles(HDROP hDropInfo)
{
  // TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
  CString t_filename;
  int t_DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);
  if (t_DropCount == 1)
  {
    DragQueryFile(hDropInfo, 0, t_filename.GetBuffer(_MAX_PATH), _MAX_PATH);
    t_filename.ReleaseBuffer();
    if (PathIsDirectory(t_filename) || PathIsRoot(t_filename))
    {
      getuserdefinefromdir(t_filename);
    }
    else
    {
      // 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
      m_mapappcfg[_T("lastfile")] = getfilefrompath(t_filename);
      // path包含文件名
      m_mapappcfg[_T("lastpath")] = getdirfrompath(t_filename);
      // 清空树控件node
      setuserdefinetreeempty();
      getuserdefinefromfile(m_mapappcfg[_T("lastpath")], m_mapappcfg[_T("lastfile")]);
    }
  }
  CDialogEx::OnDropFiles(hDropInfo);
}

// 鼠标处于控件间隙时可拖动修改scale
void CtewDlg::OnMouseMove(UINT nFlags, CPoint point)
{
  // TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
  if (m_modctrlscaleflag)
  {
    if (((point.x <= m_ctrlpositionarray[0][0] + BORDERJUDGE) || (point.x >= m_ctrlpositionarray[2][2] - BORDERJUDGE)) || ((point.y <= m_ctrlpositionarray[0][1] + BORDERJUDGE) || (point.y >= m_ctrlpositionarray[2][3] - BORDERJUDGE)))
    {
      SendMessage(WM_LBUTTONUP, 0, 0);
    }
  }
  else
  {
    // 光标类型 
    LPWSTR t_cursortype;
    // 鼠标处于控件间隙时可拖动修改scale
    //  列
    if ((point.x >= m_pointns1.x) && (point.x <= m_pointns2.x) && (point.y >= m_pointns1.y) && (point.y <= m_pointns2.y))
    {
      t_cursortype = IDC_SIZEWE;
    }
    //	行
    else if ((point.x >= m_pointwe1.x) && (point.x <= m_pointwe2.x) && (point.y >= m_pointwe1.y) && (point.y <= m_pointwe2.y))
    {
      t_cursortype = IDC_SIZENS;
    }
    else
    {
      t_cursortype = IDC_ARROW;
    }
    SetCursor(LoadCursor(NULL, t_cursortype));
    ShowCursor(TRUE);
  }
  CDialogEx::OnMouseMove(nFlags, point);
}

// press mouse lbutton set scale行为flag
/*	如果你能弄清楚这一段并完善它,求求你务必发邮件至xaviet@163.com告诉我,不胜感激*/
void CtewDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
  // TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
  // 光标类型 
  LPWSTR t_cursortype;
  // 鼠标处于控件间隙时可拖动修改scale,修改时隐藏主要控件
  //  修改scale行为flag,1indicate x,2 indicate y,0 indicate none
  //  修改列宽
  if ((point.x >= m_pointns1.x) && (point.x <= m_pointns2.x) && (point.y >= m_pointns1.y) && (point.y <= m_pointns2.y))
  {
    t_cursortype = IDC_SIZEWE;
    m_modctrlscaleflag = 1;
    GetDlgItem(IDC_cfgtree)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_trmtab)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_loglist)->ShowWindow(SW_HIDE);
    UpdateWindow();
  }
  //	修改行高
  else if ((point.x >= m_pointwe1.x) && (point.x <= m_pointwe2.x) && (point.y >= m_pointwe1.y) && (point.y <= m_pointwe2.y))
  {
    t_cursortype = IDC_SIZENS;
    m_modctrlscaleflag = 2;
    GetDlgItem(IDC_cfgtree)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_trmtab)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_loglist)->ShowWindow(SW_HIDE);
    UpdateWindow();
  }
  else
  {
    t_cursortype = IDC_ARROW;
    m_modctrlscaleflag = 0;
  }
  SetCursor(LoadCursor(NULL, t_cursortype));
  ShowCursor(TRUE);
  CDialogEx::OnLButtonDown(nFlags, point);
}

// press mouse lbutton unset scale行为flag
void CtewDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
  // TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
  CRect t_rect;
  GetClientRect(&t_rect);
  // 修改scale行为flag,1indicate x,2 indicate y,0 indicate none
  //  修改完成后显示主要控件
  if (m_modctrlscaleflag)
  {
    if (m_modctrlscaleflag == 1)
    {
      if ((point.x >= m_ctrlpositionarray[0][0] + BORDERJUDGE) && (point.x <= m_ctrlpositionarray[2][2] - BORDERJUDGE))
      {
        m_ctrlpositionarray[1][0] = m_ctrlpositionarray[2][0] = m_ctrlpositionarray[0][2] = point.x;
      }
    }
    else if (m_modctrlscaleflag == 2)
    {
      if ((point.y >= m_ctrlpositionarray[0][1] + BORDERJUDGE) && (point.y <= m_ctrlpositionarray[2][3] - BORDERJUDGE))
      {
        m_ctrlpositionarray[1][3] = m_ctrlpositionarray[2][1] = point.y;
      }
    }
    repaintctrl(t_rect.Width(), t_rect.Height());
    GetDlgItem(IDC_cfgtree)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_trmtab)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_loglist)->ShowWindow(SW_SHOW);
    UpdateWindow();
  }
  SetCursor(LoadCursor(NULL, IDC_ARROW));
  ShowCursor(TRUE);
  m_modctrlscaleflag = 0;
  CDialogEx::OnLButtonUp(nFlags, point);
}

// 界面命令
void CtewDlg::OnDlgopenfile()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  // 设置过滤器 
  CString t_lastcfgpath;
  TCHAR t_szFilter[] = _T("配置文件(*.cfg)|*.cfg|所有文件(*.*)|*.*||");
  // 构造打开文件对话框   
  CFileDialog t_fileDlg(TRUE, NULL, NULL, 0, t_szFilter);
  //设置执行配置文件初始路径,如果上次路径为空,使用应用程序路径
  t_lastcfgpath = m_mapappcfg[_T("lastpath")];
  if (t_lastcfgpath.GetLength() == 0)
  {
    t_lastcfgpath = m_mapappcfg[_T("apppath")];
  }
  // default path
  t_fileDlg.m_ofn.lpstrInitialDir = t_lastcfgpath;
  // dlg title
  t_fileDlg.m_ofn.lpstrTitle = _T("打开...");
  // 显示打开文件对话框
  if (IDOK == t_fileDlg.DoModal())
  {
    CStdioFile t_csf;
    // 判断所打开文件存在
    if (t_csf.Open(t_fileDlg.GetPathName(), CFile::modeRead | CFile::shareDenyNone))
    {
      t_csf.Close();
      // 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
      m_mapappcfg[_T("lastfile")] = t_fileDlg.GetFileName();
      // path包含文件名
      m_mapappcfg[_T("lastpath")] = getdirfrompath(t_fileDlg.GetPathName());
      // 生成cfg tree
      getuserdefinecfgtree();
    }
    else
    {
      AfxMessageBox(_T("sorry,no find this file"));
    }
  }
  // menu open
  setcmdinopen();
}


void CtewDlg::OnDlgopendir()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  // 存放选择的目录路径
  TCHAR t_wcpath[MAX_PATH];
  ZeroMemory(t_wcpath, sizeof(t_wcpath));
  CString t_path;
  BROWSEINFO t_bi;
  t_bi.hwndOwner = m_hWnd;
  // 打开上次目录
//	t_bi.pidlRoot=GetItemIDListFromFilePath(m_mapappcfg[_T("lastpath")]);   
  t_bi.pidlRoot = NULL;
  t_bi.pszDisplayName = t_wcpath;
  t_bi.lpszTitle = _T("选择目录");
  t_bi.ulFlags = 0;
  t_bi.lpfn = NULL;
  t_bi.lParam = 0;
  t_bi.iImage = 0;
  // 弹出选择目录对话框
  LPITEMIDLIST t_lp = SHBrowseForFolder(&t_bi);
  int t_i = SHGetPathFromIDList(t_lp, t_wcpath);
  t_path.Format(_T("%s"), t_wcpath);
  if (t_lp && t_i)
  {
    m_mapappcfg[_T("lastpath")] = t_path;
    // SetWindowText:设置标题
    SetWindowText(_T("tew---") + t_path);
    // 清空树控件node
    setuserdefinetreeempty();
    getuserdefinefromdir(t_path);
  }
}


void CtewDlg::OnDlgpause()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  AfxMessageBox(_T("OnDlgpause"));
}


void CtewDlg::OnDlgstart()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  HTREEITEM t_node;
  t_node = m_cfgtree.GetRootItem();
  struct treeitemdata* t_id;
  t_id = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_node));
  // 检查root是否enable
  if (t_id->enableflag == 0)
  {
    AfxMessageBox(_T("root节点未激活"));
  }
  else
  {
    INT_PTR t_nResponse = 0;
    // 获取时间,关联测试时间和log目录
    m_csstarttime = this->getsystemdatetime(1);
    // 清空终端消息
    m_trmmsg.Empty();
    if (getproducttest())
    {
      m_mapdutmessage.clear();
      // 获取mapdutmessage
      if (getdatafile2mapdutmessage())
      {
        assert(0);
      }
      // 准备生产测试场景
      cdlgdutmsg t_dlgdutmsg;
      t_dlgdutmsg.m_ptewdlg = this;
      t_nResponse = t_dlgdutmsg.DoModal();
      if (t_nResponse == IDOK)
      {
        CString t_csfile = m_mapappcfg[_T("apppath")] + DUT_MSG_FILE;
        CStdioFile t_csf;
        // 防止日期表项被修改
        m_mapdutmessage[_T("000Date")] = m_csstarttime;
        if (t_csf.Open(t_csfile, CFile::modeWrite | CFile::shareDenyNone | CFile::modeCreate))
        {
          getmap2file(&t_csf, &m_mapdutmessage);
          t_csf.Close();
        }
        else
        {
          AfxMessageBox(_T("生产测试模板写入失败"));
          assert(0);
        }
        // 遍历树控件node,替换$_
        HTREEITEM t_node;
        t_node = m_cfgtree.GetRootItem();
        //	7-replace $_
        setuserdefinetreeitem(t_node, 7);
      }
      else if (t_nResponse == IDCANCEL)
      {
      }
    }
    // 准备开始
    if ((t_nResponse == IDOK) || (!getproducttest()))
    {
      runtewdlgperstart();
      // menu start
      setcmdinstart();
    }
  }
}


void CtewDlg::OnDlgstop()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  if (this->m_mapappcfg[_T("loop")] == _T("0"))
  {
    // 遍历树,dlgstop
    int i = 1;
    i += 1;
    setuserdefinetreeitem(m_cfgtree.GetRootItem(), 4);
    // menu open
    setcmdinopen();
    // 模态创建节点属性对话框
    cdlgprocessing t_dlgprocessing;
    // 获取选中节点信息赋值给节点属性对话框实例的公共变量m_pitemdata
    t_dlgprocessing.m_endflag = NULL;
    t_dlgprocessing.m_flag = &(m_pcuritemdata->ptrdlg);
    if (t_dlgprocessing.DoModal() == IDOK)
    {
    }
    if (getproducttest())
    {
      // 生产测试结果确认
      getproducttestresultmsg();
    }
    // 保存测试结果记录,包含ng行描述
    setnginformation();
    // run clear
    runtewdlgclear(m_cfgtree.GetRootItem());
  }
  else
  {
    AfxMessageBox(_T("终止前请取消环境配置菜单中循环执行"));
  }
}


void CtewDlg::OnDlghelpdoc()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  AfxMessageBox(_T("OnDlghelpdoc"));
}


void CtewDlg::OnAbout()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  caboutdlg t_aboutdlg;
  t_aboutdlg.m_csversion = this->getfileversion();
  if (t_aboutdlg.DoModal() == IDOK)
  {
  }
}


void CtewDlg::OnDlgexplorer()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  ShellExecute(NULL, _T("open"), m_mapappcfg[_T("lastpath")], NULL, NULL, SW_SHOW);
}


void CtewDlg::OnNMRClickcfgtree(NMHDR* pNMHDR, LRESULT* pResult)
{
  // TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
  CMenu menu;
  POINT t_pt = { 0 };
  // 得到鼠标点击位置 
  GetCursorPos(&t_pt);
  m_cfgtree.ScreenToClient(&t_pt);
  UINT t_uFlags;
  HTREEITEM hItem = m_cfgtree.HitTest(t_pt, &t_uFlags);
  if ((hItem != NULL) && (TVHT_ONITEM & t_uFlags))
  {
    m_cfgtree.Select(hItem, TVGN_CARET);
    m_cfgtree.ClientToScreen(&t_pt);
    menu.LoadMenu(IDR_cfgtreemenu);//菜单资源ID 
    //m_list是CListCtrl对象,这里的消息发到this 
    menu.GetSubMenu(0)->TrackPopupMenu(0, t_pt.x, t_pt.y, this);
  }
  *pResult = 0;
}


void CtewDlg::Ondlgenablenode()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  HTREEITEM t_node = m_cfgtree.GetSelectedItem();
  if (t_node)
  {
    struct treeitemdata* t_pitemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_node));
    if (t_node != m_cfgtree.GetRootItem())
    {
      // non root node展开节点
      m_cfgtree.Expand(t_node, TVE_EXPAND);
    }
    t_pitemdata->enableflag = 1;
    t_pitemdata->runstatus = 0;
    modifytreeallitemdatarunmode(t_pitemdata->node, t_pitemdata->enableflag);
  }
}


void CtewDlg::Ondlgdisablenode()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  HTREEITEM t_node = m_cfgtree.GetSelectedItem();
  if (t_node)
  {
    if (t_node != m_cfgtree.GetRootItem())
    {
      // non root node收起节点
      //m_cfgtree.Expand(t_node,TVE_COLLAPSE);
    }
    struct treeitemdata* t_pitemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_node));
    assert(t_pitemdata);
    t_pitemdata->enableflag = 0;
    t_pitemdata->runstatus = 6;
    modifytreeallitemdatarunmode(t_pitemdata->node, t_pitemdata->enableflag);
    t_node = m_cfgtree.GetParentItem(t_node);
    // 向上计算disable状态
    if (t_node != NULL)
    {
      t_node = m_cfgtree.GetChildItem(t_node);
      int t_enflag = 0;
      assert(t_pitemdata);
      for (; (t_node != NULL) || (t_enflag == 0);)
      {
        t_pitemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_node));
        if (t_pitemdata->enableflag == 1)
        {
          // enable则跳出循环
          t_enflag = 1;
          break;
        }
        else
        {
          if (m_cfgtree.GetNextSiblingItem(t_node) == NULL)
          {
            // 无兄弟节点,disable父节点
            t_node = m_cfgtree.GetParentItem(t_node);
            if (t_node != NULL)
            {
              t_pitemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_node));
              t_pitemdata->enableflag = 0;
              t_pitemdata->runstatus = 6;
              modifytreeallitemdatarunmode(t_pitemdata->node, t_pitemdata->enableflag);
              // 循环变量设为第一个兄弟节点
              for (; m_cfgtree.GetPrevSiblingItem(t_node) != NULL;)
              {
                t_node = m_cfgtree.GetPrevSiblingItem(t_node);
              }
            }
            else
            {
              // 无父节点则跳出循环
              break;
            }
          }
          else
          {
            // // 循环变量设为下一个兄弟节点
            t_node = m_cfgtree.GetNextSiblingItem(t_node);
          }
        }
      }
    }
  }
}

void CtewDlg::Onctrlcfgtreeproperty()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë 
  // set cfg tree property
  setuserdefinenodeproperty(0);
}


void CtewDlg::OnNMDblclkcfgtree(NMHDR* pNMHDR, LRESULT* pResult)
{
  // TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë 
  // set cfg tree property
  setuserdefinenodeproperty(0);
  // TRUE is 双击不收起树节点 
  *pResult = TRUE;
}


void CtewDlg::Ondlgsaveas()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  // 设置过滤器 
  CString t_lastcfgpath;
  TCHAR t_szFilter[] = _T("配置文件(*.cfg)|*.cfg|所有文件(*.*)|*.*||");
  // 构造打开文件对话框   
  CFileDialog t_fileDlg(FALSE, _T("*.cfg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, t_szFilter, NULL);
  //设置执行配置文件初始路径,如果上次路径为空,使用应用程序路径
  t_lastcfgpath = m_mapappcfg[_T("lastpath")];
  if (t_lastcfgpath.GetLength() == 0)
  {
    t_lastcfgpath = m_mapappcfg[_T("apppath")];
  }
  // default path
  t_fileDlg.m_ofn.lpstrInitialDir = t_lastcfgpath;
  // dlg title
  t_fileDlg.m_ofn.lpstrTitle = _T("另存为...");
  // 显示打开文件对话框
  if (IDOK == t_fileDlg.DoModal())
  {
    CStdioFile t_csf;
    // save user define file
    setuserdefinefile(t_fileDlg.GetPathName());
    // 生成cfg tree
    getuserdefinecfgtree();
  }
}

void CtewDlg::Ondlginittcpport()
{
  if (m_ptcpserver && ((static_cast<ctcpserver*>(m_ptcpserver))->m_tcpconnectflag == 1))
  {
    m_ptcpserver->PostThreadMessage(WM_tcpserverinittcpport, NULL, NULL);
  }
}

void CtewDlg::Ondlgsave()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  if (m_mapappcfg[_T("lastfile")].GetLength() > 0)
  {
    CString t_csfilename;
    int t_iflag = 0;
    t_csfilename = m_mapappcfg[_T("lastpath")] + m_mapappcfg[_T("lastfile")];
    CStdioFile t_csf;
    t_csf.Open(t_csfilename, CFile::modeRead | CFile::shareDenyNone);
    CString t_csline;
    for (; t_csf.ReadString(t_csline) != NULL;)
    {
      if (getstring4replacelabel(&t_csline))
      {
        t_iflag = 1;
        break;
      }
    }
    t_csf.Close();
    if (t_iflag)
    {
      AfxMessageBox(_T("配置文件含有替换标识,禁止保存!"));
    }
    else
    {
      // save user define file
      setuserdefinefile(t_csfilename);
      // 生成cfg tree
      getuserdefinecfgtree();
    }
  }
  else
  {
    this->Ondlgsaveas();
  }
}


void CtewDlg::Ondlgedit()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  // 当前节点信息
  HTREEITEM t_hItem;
  t_hItem = m_cfgtree.GetSelectedItem();
  // 检查节点有效,可以进行节点属性编辑
  if (t_hItem)
  {
    // 获取选中节点信息赋值给节点属性对话框实例的公共变量m_pitemdata
    struct treeitemdata* t_pitemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_hItem));
    if (t_pitemdata->filename.GetLength() != 0)
    {
      // 初始化脚本编辑dlg
      cscripteditdlg* t_pscripteditdlg = new cscripteditdlg;
      t_pscripteditdlg->m_filename = t_pitemdata->path + t_pitemdata->filename;
      t_pscripteditdlg->m_pappdlg = this;
      // 创建一个非模态对话框 
      t_pscripteditdlg->Create(IDD_scripteditdlg);
      // 显示非模态对话框
      t_pscripteditdlg->ShowWindow(SW_SHOWNORMAL);
    }
  }
}


void CtewDlg::OnTvnSelchangedcfgtree(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
  // TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
  HTREEITEM t_hItem;
  t_hItem = m_cfgtree.GetSelectedItem();
  setcurtreenode(t_hItem);
  *pResult = 0;
}


void CtewDlg::OnTcnSelchangetrmtab(NMHDR* pNMHDR, LRESULT* pResult)
{
  // TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
  int t_sel;
  t_sel = m_trmtab.GetCurSel();
  // 选中tabpage，关联trmdlg和cfgtree
  settrmtabpage(t_sel);
  *pResult = 0;
}


void CtewDlg::Ondlglog()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  HTREEITEM t_hItem;
  t_hItem = m_cfgtree.GetSelectedItem();
  if (t_hItem)
  {
    struct treeitemdata* t_pid = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_hItem));
    CString t_cslogfile = t_pid->logpath + t_pid->filename + TRM_LOG_EXT;
    CStdioFile t_csdlogfile;
    // 初始化脚本编辑dlg
    cscripteditdlg* t_pscripteditdlg = new cscripteditdlg;
    t_pscripteditdlg->m_filename = t_cslogfile;
    t_pscripteditdlg->m_pappdlg = this;
    // edit ctrl readonly
    t_pscripteditdlg->m_readonlyflag = TRUE;
    // 创建一个非模态对话框 
    t_pscripteditdlg->Create(IDD_scripteditdlg);
    // 显示非模态对话框
    t_pscripteditdlg->ShowWindow(SW_SHOWNORMAL);
  }
}


void CtewDlg::Ondlgcsv()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  // 取前三行
  CString t_cs, t_csline;
  CStdioFile t_csfcsv;
  if (t_csfcsv.Open(m_cscsvpath + CSV_FILE, CFile::modeRead | CFile::shareDenyNone))
  {
    for (int t_i = 0; (t_i < 3) && (t_csfcsv.ReadString(t_csline)); t_i++)
    {
      t_cs += t_csline + _T("\n");
    }
    t_csfcsv.Close();
  }
  else
  {
    t_cs = _T("csv file fail!");
  }
  AfxMessageBox(t_cs);
}


void CtewDlg::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
  switch (nIDEvent)
  {
  case APPDLG1STIMER:
    m_timer++;
    break;
  case APPDLG60STIMER:
    break;
  default:
    break;
  }
  CDialogEx::OnTimer(nIDEvent);
}

// 重载OnOK,避免回车关闭对话框
void CtewDlg::OnOK()
{
  // TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
  //CDialogEx::OnOK();
}


void CtewDlg::Ondlgconfig()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  // 模态创建节点属性对话框
  cdlgconfig t_dlgconfig;
  // 指针传递
  t_dlgconfig.m_pconfigflag = &m_configflag;
  t_dlgconfig.m_pappdlg = this;
  if (t_dlgconfig.DoModal() == IDOK)
  {
  }
}

// 打开节点文件夹
void CtewDlg::Ondlgopenpath()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
  HTREEITEM t_node = m_cfgtree.GetSelectedItem();
  struct treeitemdata* t_pitemdata = reinterpret_cast<treeitemdata*>(m_cfgtree.GetItemData(t_node));
  assert(t_pitemdata);
  ShellExecute(NULL, _T("open"), t_pitemdata->path, NULL, NULL, SW_SHOW);
}


BOOL CtewDlg::PreTranslateMessage(MSG* pMsg)
{
  // TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
  if (m_hAccTable)
  {
    if (::TranslateAccelerator(m_hWnd, m_hAccTable, pMsg))
    {
      return true;
    }
  }
  // 把Esc和Enter按键事件消息过滤掉，否则该消息会导致对应应用程序调用OnOK（）方法，结束应用程序  
  if (pMsg->message == WM_KEYDOWN)
  {
    switch (pMsg->wParam)
    {
    case VK_ESCAPE: //Esc按键事件  
      return true;
    case VK_RETURN: //Enter按键事件  
      return true;
    default:
      break;
    }
  }
  return CDialogEx::PreTranslateMessage(pMsg);
}

// 好似不用
void CtewDlg::OnIdrAccelerator()
{
  // TODO: ÔÚ´ËÌí¼ÓÃüÁî´¦Àí³ÌÐò´úÂë
}


void CtewDlg::OnCancel()
{
  // TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
  CDialogEx::OnCancel();
}
