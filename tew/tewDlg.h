
// tewDlg.h : 头文件
//

#pragma once

#include "config.h"
#include "afxcmn.h"
#include "dlgdutmsg.h"
#include "dlgproductresult.h"
#include "afxsock.h"
//mysql support
#include "mysql.h"
#pragma comment(lib,"libmySQL.lib") 

// CtewDlg 对话框
class CtewDlg : public CDialogEx
{
	// 构造
public:
	CtewDlg(CWnd* pParent=NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD=IDD_TEW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
private:
	// 初始化app环境,包含界面初始化
	int	initall();
	// 初始化app_cfg.dat & mode_comm.dat,不存在则创建并写入缺省内容
	int initfile(CString v_filename,CString v_cs);
	// 初始化app配置map
	int initmapappcfg();
	// 由文件得到配置map
	int getfile2map(CStdioFile *v_pcsf,map<CString,CString> *v_map);
	// 由配置map得到文件
	int getmap2file(CStdioFile *v_pcsf,map<CString,CString> *v_map);
	// app配置map保存到文件
	int getcfgmapappcfg2file();
	// 初始化对话框界面
	int initdlgview();
	// 增加菜单栏
	int initdlgviewmenu();
	// 增加工具栏
	int initdlgviewtoolsbar();
	// 添加状态栏,v_i为1创建,0为重绘
	int initdlgviewstatusbar(int v_i);
	//	v_id:0表示工具栏,ID_dlgstatusbarctrl表示状态栏
	int repainttoolstatus(int v_id);
	// 重绘ctrl log list
	int repaintloglist();
	// 重绘各控件
	int repaintctrl(int v_x,int v_y);
	// resize后记录size to map
	int getsize2mapappcfg();
	// 初始化cfgtree ctrl
	int	initctrlcfgtree();
	// 初始化trm tab ctrl
	int	initctrltrmtab();
	// 初始化log list ctrl
	int	initctrlloglist();
	// 使能或禁止菜单
	int setmenuaction(int v_menu, int v_submenu, int v_flag);
	// get dlg 界面控件间隙
	int getdlgctrlscale();
	// 从路径获取目录
	CString getdirfrompath(CString v_cspath);
	// 从路径获取目录
	CString getfilefrompath(CString v_cspath);
	// 解析用户define文件
	int getuserdefinefromfile(CString v_csdir,CString v_csfile);
	// 解析用户define目录
	int getuserdefinefromdir(CString v_csdir);
	// 分析目录是否存在userdefine file, return 1 means yes,0 means no
	int getuserdefinefilefromdir(CString v_csdir);
	// 创建tree控件使用imagelist
	int initctrlcfgtreeimagelist();
	// set用户define node
	HTREEITEM setuserdefinetreenode(HTREEITEM v_fathernode,treeitemdata *v_ptreeitemdata);
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
	int setuserdefinetreeitem(HTREEITEM v_nodea,int v_i=0,CStdioFile *v_pcsf=0);
	// 清空树控件node
	int setuserdefinetreeempty();
	// parse userdefine with file
	int getuserdefinebyparsefile(CStdioFile *v_pcsf,HTREEITEM v_fathernode);
	// parse userdefine with old file
	int getuserdefinebyparseoldfile(CStdioFile*v_csf,treeitemdata *v_pid);
	// parse userdefine with line
	HTREEITEM getuserdefinebyparseline(CString v_csline,HTREEITEM v_fathernode);
	// 文件内容忽略无用信息
	int getuserdefinelinebydelunuseinfo(CString v_line);
	// get default itemdata
	int getuserdefinedefaultitemdata();
	// parse line to get user define itemdata
	int getuserdefineitemdata(CString v_csline,treeitemdata *v_ptreeitemdata);
	// parse node itemdata to get user define line
	CString getuserdefineline(HTREEITEM v_node);
	// get node type by parse line;1 to has child;0 notnode
	int getuserdefinenodetype(CString v_csline);
	// set cfg tree property
	int setuserdefinenodeproperty(HTREEITEM v_node);
	// save user define file
	int setuserdefinefile(CString v_filename);
	// tabctrl关联dlg重绘/repaint ctrl trmdlg
	int repaintctrltrmdlg(HTREEITEM v_node);
	// 遍历,并发
	int runtewdlgstart(HTREEITEM v_node);
	// 遍历,exit
	int runtewdlgexit(HTREEITEM v_node);
	// 遍历,clear
	int runtewdlgclear(HTREEITEM v_node);
	// 刷新当前节点
	int setcurtreenode(HTREEITEM v_node);
	// 模拟鼠标点击
	int setmouseclick(CPoint v_pt);
	// 初始化终端页面数组
	int inittrmtabpageitemdataarray();
	// 选中tabpage，关联trmdlg和cfgtree
	int settrmtabpage(int v_i);
	// 检查节点完成状态,延伸至父节点
	int setuserdefinenodeexitflag(treeitemdata *v_ptreeitemdata);
	// 树控件携带信息enableflag
	//	v_flag:1-enable;0-disable
	int modifytreeallitemdatarunmode(HTREEITEM v_node,int v_flag);
	// 获取时间 v_type:0-format;1-dir used
	CString getsystemdatetime(int v_type=0);
	// 生成cfg tree
	int getuserdefinecfgtree();
	// 初始化log目录
	int pathinitlogpath(CString v_logpath);
	// 判断目录是否存在
	bool pathgetfolderexists(CString v_cs);
	// 递归创建目录， 如果目录已经存在或者创建成功返回TRUE
	bool pathmkdir(CString v_cspath);
	// 初始化csv文件
	int pathinitcsvfile();
	// 准备开始
	int runtewdlgperstart();
	// 获取生产测试场景
	int getproducttest();
	// 获取database场景
	int getdatabase();
	// 获取mapdutmessage
	int getdatafile2mapdutmessage();
	// 生产测试结果确认
	int getproducttestresultmsg();
	// 生成生产测试报告
	int setproducttestresultmsg(CString v_csdirname);
	// 递归建立目录配置tree
	int setuserdefinetree4path(CString v_cspath,treeitemdata *v_pid);
	// 工具栏使能或禁止
	int settoolbaraction(int v_id,BOOL v_flag);
	// deny all command
	int setallcmddeny();
	// ready
	int setcmdinready();
	// open
	int setcmdinopen();
	// start
	int setcmdinstart();
	// get file version
	CString getfileversion();
	// 保存测试结果记录,包含ng行描述
	int setnginformation();
	// 初始化tcp server port:MNTPORT,维护使用
	int initdlgtcpserver();
	// 执行配置标志
	struct tewdlgconfigflag m_configflag;
	// 终端消息
	CString m_trmmsg;
	// 从path获取id list
	LPITEMIDLIST GetItemIDListFromFilePath(CString v_strFilePath);
	//数据库连接句柄
	MYSQL *m_pmysql;
	// 初始化mysql
	int initdlgmysql();
	// 初始化 udp process
	int initdlgudp();
	// local version
	struct tewversion m_dlocalver;
	// message to save value[0,1023]
	int setmessage2savevalue(CString v_message);

protected:
	// app配置map
	map<CString,CString> m_mapappcfg; 
	// dlg菜单point
	CMenu *m_pmenu;
	// 状态栏控件实例
	CStatusBarCtrl m_statusbarctrl;
	// 工具栏变量
	CToolBar m_toolbar;		// 工具栏
	CImageList m_toolbarimagelist;	// 工具栏图标链
	CToolBarCtrl *m_ptoolbarctrl;	// 工具条控制指针
	CToolTipCtrl m_tooltipctrl;	// CToolTipCtrl控件,为toolbar提供tip
	// config tree ctrl
	CTreeCtrl m_cfgtree;
	// 创建tree控件使用imagelist 
	CImageList m_cfgtreeimageList;
	// trm tab ctrl
	CTabCtrl m_trmtab;
	// log list ctrl
	CListCtrl m_loglist;
	// cfg trm log 控件布局
	volatile int m_ctrlpositionarray[3][4];
	// dlg 界面控件间隙
	CPoint m_pointns1,m_pointns2,m_pointwe1,m_pointwe2;
	// 修改scale行为flag,1 indicate x,2 indicate y,0 indicate none
	int m_modctrlscaleflag;
	// current node itemdata
	struct treeitemdata *m_pcuritemdata;
	// log path
	CString m_cslogpath;
	// csv path
	CString m_cscsvpath;
	// csv file
	CStdioFile m_csfcsv;
	// app log file
	CStdioFile m_csfapplogfile;
	// APPDLG1STIMER
	volatile int m_timer;

public:
	// 快捷键全局变量
	HACCEL m_hAccTable;
	// 终端页面数组
	struct treeitemdata *m_ptrmtabpageitemdataarray[TABCTRLMAXPAGENUMBER];
	// report配置map
	map<CString,CString> m_mapmodulefield;
	// dut message
	map<CString,CString> m_mapdutmessage;
	// 执行开始时间
	CString m_csstarttime;
	// 信号量,线程间通信
	HANDLE m_hsemaphore;
	// 通信方式模板文件名
	CString m_csmodecomm;
	// 获取应用程序目录
	CString getappdir();
	// 重载系统消息处理,拦截onclose消息
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	// dialog exit
	afx_msg void Ondlgexit();
	// dialog size
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// dlg最小size X&Y
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	// log list clear
	afx_msg void Onctrlloglistclear();
	// log list menu popup
	afx_msg void OnNMRClickloglist(NMHDR *pNMHDR, LRESULT *pResult);
	// ctrl log list 增加内容
	afx_msg LRESULT Onappdlgctrladdloglist(WPARAM wParam, LPARAM lParam);
	// set ctrl statusbar 
	afx_msg LRESULT Onappdlgctrlsetstatusbar(WPARAM wParam, LPARAM lParam);
	// modify node property 
	afx_msg LRESULT Onappdlgnodepropertymodify(WPARAM wParam, LPARAM lParam);
	// trm dialog close message
	afx_msg LRESULT Onappdlgtrmdlgclose(WPARAM wParam, LPARAM lParam);
	// trm msg
	afx_msg LRESULT Onappdlgmsg(WPARAM wParam, LPARAM lParam);
	// ctrl stop
	afx_msg LRESULT Onappdlgctrlstop(WPARAM wParam, LPARAM lParam);
	// tcp server msg
	afx_msg LRESULT Onappdlgtcpservermsg(WPARAM wParam, LPARAM lParam);
	// 文件/目录支持拖拽
	afx_msg void OnDropFiles(HDROP hDropInfo);
	// 鼠标处于控件间隙时可拖动修改scale
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	// press mouse lbutton set scale行为flag
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// press mouse lbutton unset scale行为flag
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	// 出现错误
	afx_msg LRESULT Onappdlgtrmdlgtrmfaultcounter(WPARAM wParam, LPARAM lParam);
	// save value number[0,1023]
	CString m_savevalue[SAVEVALUECOUNTER];
	// 设置存储值
	afx_msg LRESULT Onappdlgsetsavevalue(WPARAM wParam, LPARAM lParam);
	// 读取存储值
	afx_msg LRESULT Onappdlggetsavevalue(WPARAM wParam, LPARAM lParam);
	// setcsv
	afx_msg LRESULT Onappdlgsetcsv(WPARAM wParam, LPARAM lParam);
	// 替换字符串中变量符号,"$_"开头
	int getstring4replacelabel(CString *v_line);
	// replace label
	afx_msg LRESULT Onappdlgreplacelabel(WPARAM wParam, LPARAM lParam);
	// module begin or end
	afx_msg LRESULT Onappdlgmodule(WPARAM wParam, LPARAM lParam);
	// get timer
	afx_msg LRESULT Onappdlgovertime(WPARAM wParam, LPARAM lParam);
	// tcp server point
	CWinThread *m_ptcpserver;
	// udp
	CWinThread *m_pudp;
	// config
	afx_msg LRESULT Onappdlgconfig(WPARAM wParam, LPARAM lParam);
	// 界面命令
	afx_msg void OnDlgopenfile();
	afx_msg void OnDlgopendir();
	afx_msg void OnDlgpause();
	afx_msg void OnDlgstart();
	afx_msg void OnDlgstop();
	afx_msg void OnDlghelpdoc();
	afx_msg void OnAbout();
	afx_msg void OnDlgexplorer();
	afx_msg void OnNMRClickcfgtree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Ondlgenablenode();
	afx_msg void Ondlgdisablenode();
	afx_msg void Onctrlcfgtreeproperty();
	afx_msg void OnNMDblclkcfgtree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Ondlgsaveas();
	afx_msg void Ondlgsave();
	afx_msg void Ondlgedit();
	afx_msg void Ondlginittcpport();
	afx_msg void OnTvnSelchangedcfgtree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangetrmtab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Ondlglog();
	afx_msg void Ondlgcsv();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();
	afx_msg void Ondlgconfig();
	afx_msg void Ondlgopenpath();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnIdrAccelerator();
	virtual void OnCancel();
};
