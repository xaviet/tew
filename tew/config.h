// config.h
//	基本配置头文件

// 特别提醒:
//	本项目中的注释可能需要结合代码才能明白

#ifndef xa_config
#define xa_config

#include <locale.h>	  // 设置中文环境
#include "initfile.h"
#include <map>
#include <assert.h> 
using namespace std;

// thanks
#define THANKS _T("\
xujin@whnec.com.cn  \
chenjuan@whnec.com.cn(before)  \
lanming@whnec.com.cn  \
kangyalong@whnec.com.cn(before)  \
chenyaoyao@whnec.com.cn(before)  \
heyun@whnec.com.cn(before)  \
leiqiang@whnec.com.cn(before)  \
maqiang@whnec.com.cn  \
wuyong@whnec.com.cn  \
xuhao@whnec.com.cn(before)  \
yuzhimin@whnec.com.cn(before)  \
zhangjian@whnec.com.cn  \
zhengziyao@whnec.com.cn  \
")

// For test
#define xavietex 0
// 维护端口
#define MNTPORT 50013
// tcpserver prompt
#define TCPSERVERPROMPT _T("tew> ")
// udp process port
#define UDPPORT 50014
// trm delay time
#define TRMDELAY 0
//trmthread end of scriptfile delay time
#define ENDOFSCRIPTFILEDELAY 50
//trmthread end delay time
#define TRMTHREADENDDELAY 500
//app end delay time
#define APPENDDELAY 2000
// cmp/cmpn错误再次比较时间
#define FAULTRETRYDELAY 50
// cmp/cmpn错误再次比较次数
#define FAULTRETRYTIMES 80
// 脚本行执行超时时间计数,秒
#define OVERTIMR 900
// directtcp tx packet inteval second
#define TXPACKETINTV 1
// 常用宏定义,供模块间调用
// 应用程序对话框指针
#define P_TEWAPP	((CtewApp*)AfxGetApp())
// 应用程序对话框指针
#define P_TEWDLG	((CtewDlg*)m_pMainWnd)

// 回车换行
#define RETURNNEWLINE _T("\r\n")
// ctrl-d
#define CTRL_D 0x4
// app运行配置文件,文件初始内容定义"initfile.h"
// app运行初始化文件名称
#define APP_CFG_DAT	 _T("app_cfg.dat")
// 终端方式定义文件名称
#define MODE_COMM_DAT	_T("mode_comm.dat")
// log目录名称
#define LOG_PATH	_T("zlog\\")
// csv文件名称
#define CSV_FILE	_T("total.csv")
// 保存测试结果记录,包含ng行描述
#define RESULT_FILE	_T("total.txt")
// app log文件名称
#define APP_LOG_FILE _T("tew.log")
// trm log文件ext名称
#define TRM_LOG_EXT	_T(".log")
// dut message file
#define DUT_MSG_FILE _T("\\生产测试模板\\lastdutmsg.dat")
// product test result path
#define PRODUCT_RESULT_PATH _T("D:\\生产测试结果\\")
// default comment
#define DEF_COMMENT	_T("//")
// replace label
#define REPLACE_LABEL	_T("$_")
// replace value
#define REPLACE_VALUE	_T("$__")
// save value number
#define SAVEVALUECOUNTER	1024
// sshv2 definer
#define SSHV2DEF	_T("sshv2:")
// sshv2 definer
#define DIRECTTCPDEF	_T("directtcp:")
// grep definer
#define GREPDEF	_T("grep:") 

// edit控件能力:256M-1
#define	EDITCTRLLIMIT 268435455
// tab ctrl max page number
#define TABCTRLMAXPAGENUMBER 256
// path max length
#define PATHMAXLENGTH 256
// dlg最小size X&Y
#define DLGMINX 800
#define DLGMINY 600
// 状态栏分隔数量
#define STATUSBARSEPARATES 4 
// 工具栏图标计数
#define TOOLBARBUTTONCOUNTER 6
// 工具栏图标array
#define TOOLBARICONARRAY {IDB_openfile,IDB_opendir,IDB_start,IDB_pause,IDB_stop,IDB_helpdoc}
// 工具栏操作IDarray
#define TOOLBARIDARRAY {ID_dlgopenfile,ID_dlgopendir,ID_dlgstart,ID_dlgpause,ID_dlgstop,ID_dlghelpdoc}
// cfgterr图标计数
#define CFGTREEIMAGELIST 7
// cfgterr图标define
#define iconready		0
#define iconrwn			1
#define iconrwf			2
#define iconwaitting	3
#define iconcwn			4
#define iconcwf			5
#define icondisable		6
// dlg控件数组
#define DLGCTRLARRAY {IDC_cfgtree,IDC_trmtab,IDC_loglist}
// dlg控件数组
#define DLGCTRLSIZEARRAY	{(float)0,(float)0,(float)0.3,(float)1\
							,(float)0.3,(float)0,(float)1,(float)0.7\
							,(float)0.3,(float)0.7,(float)1,(float)1}
// 控件边缘宽度
#define EDGEWIDTH	1
// toolbar宽度
#define TOOLBARWIDTH	40
// statusbar宽度
#define STATUSBARWIDTH	20
// 鼠标指针边界判定
#define BORDERJUDGE	10

// 自定义消息
//	app DLG message
#define WM_appdlgctrladdloglist	(WM_USER+100)
#define WM_appdlgctrlsetstatusbar	(WM_USER+101)
#define WM_appdlgnodepropertymodify	(WM_USER+102)
#define WM_appdlgtrmdlgclose	(WM_USER+103)
#define WM_appdlgtrmdlgtrmfaultcounter (WM_USER+104)
#define WM_appdlgsetsavevalue (WM_USER+105)
#define WM_appdlggetsavevalue (WM_USER+106)
#define WM_appdlgsetcsv (WM_USER+107)
#define WM_appdlgreplacelabel (WM_USER+108)
#define WM_appdlgmodule	(WM_USER+109)
//  app DLG second counter
#define APPDLG1STIMER (WM_USER+200)
#define WM_appdlgovertime (WM_USER+201)
#define WM_appdlgconfig (WM_USER+202)
#define WM_appdlgmsg (WM_USER+203)
#define WM_appdlgtcpservermsg (WM_USER+204)
#define DLGTHANKSTIMER (WM_USER+205)
#define WM_appdlgctrlstop (WM_USER+206)
//	app DLG 60 seconds count
#define APPDLG60STIMER (WM_USER+207)
//	dlg node property message
#define WM_nodeprotertydlgscripteditdlgexit	(WM_USER+300)
//	dlg trm dialog message
#define WM_trmdlgresize	(WM_USER+400)
#define WM_trmdlgeditaddcstring (WM_USER+401)
#define WM_trmdlgstop (WM_USER+402)
#define WM_trmdlgtrmfaultcounter (WM_USER+403)
#define WM_trmdlgsetsavevalue (WM_USER+404)
#define WM_trmdlggetsavevalue (WM_USER+405)
#define WM_trmdlgsetcsv (WM_USER+406)
#define WM_trmdlgctrladdloglist	(WM_USER+407)
#define WM_trmdlgreplacelabel	(WM_USER+408)
#define WM_trmdlgmodule	(WM_USER+409)
#define WM_trmdlgovertime (WM_USER+410)
#define WM_trmdlgmsg (WM_USER+411)
#define WM_trmdlgctrlstop (WM_USER+412)
//  trm thread message
#define WM_trmthreadrun (WM_USER+500)
#define WM_trmthreadautologin (WM_USER+501)
#define WM_trmthreadinitreturn (WM_USER+502)
//  dlg waiting message
#define DLGWAITINGTIMER (WM_USER+600)
//  dlg system cmd message
#define DLGSYSTEMCMDTIMER (WM_USER+700)
#define WM_dlgsystemcmdeditaddcstring (WM_USER+701)
//	dlg dut message message
#define WM_dlgdutmsgeditend  (WM_USER+800)
//	dlg processing message
#define DLGPROCESSINGTIMER  (WM_USER+900)
//  tcp server thread message
#define WM_tcpserveraccept (WM_USER+1000)
#define WM_tcpserverrun (WM_USER+1001)
#define WM_tcpserverconnect (WM_USER+1002)
#define WM_tcpserverinittcpport (WM_USER+1003)
//	udp thread message
#define WM_udprx (WM_USER+1100)
//  cscriptedit message
#define WM_scripteditlastng (WM_USER+1200)
#define WM_scripteditnextng (WM_USER+1201)
#define WM_scripteditselectall (WM_USER+1300)	// 预留100 
//  trm directtcp thread message
#define WM_trmdirecttcprun (WM_USER+1400)
#define WM_trmdirecttcptimer (WM_USER+1401)
#define WM_trmdirecttcptx (WM_USER+1402)
#define WM_trmdirecttcprx (WM_USER+1403)
#define WM_trmdirecttcpwait (WM_USER+1404)
#define WM_trmdirecttcpmsg (WM_USER+1405)

//逗一逗调程序的
//#define TRUE FALSE 

// 定义tewdlg配置标志
struct tewdlgconfigflag
{
	int loop = 0;	// 循环执行标志
	int ngstop = 0;	//ng触发stop
	CString zlogdir = _T("");	//zlog存放位置
};

// 定义user define tree控件外部数据，包含模块执行方式
//	配置模块运行指示方式
//	{level;path;comment;runmtmode;enableflag;message}
//	脚本工作指示方式
//	filename;host;[sshv2:]port;comment;runmtmode;enableflag;message;
struct treeitemdata
 {
	int level;	// level:表示并行层次结构,0表示root(不可用).
	CString path;	// path;模块脚本路径。使用相对路径。需要添加在m_mapconfig[_T("lastpath")]后
	CString comment;	// comment:标题
	int	runmtmode;	// runmtmode:0为顺序执行;1为并行执行
	int enableflag;	// enableflag:0为disable;1为enable
	int exitflag;	// exitflag:1为exit
	int runstatus;	// cfgterr图标define
	CString filename;	// filename:表示脚本文件名.*.script
	CString host;	// host;脚本文件执行的目标主机名
	int sshv2;	// use sshv2 mode
	int directtcp;	// use directtcp mode
	int	port;	// port:脚本文件执行的目标主机端口
	CString message;	// message:脚本执行的行为消息
	int ptrdlg;	// pdlg:节点对应终端dlg指针
	int trmtabctrlnumber;	// tabctrlnumber:节点对应终端tab ctrl编号
	int loopcounter;	// loopcounter:模块执行次数
	HTREEITEM node;	// node:tree node handle
	CString information;	// information:节点信息,供终端消息传递
	int msgoperate;	// msgoperate:终端消息传递执行结果;TRUE为ok;FALSE为ng
	int faultcounter;	// faultcounter:节点错误计数
	CString logpath;	// logpath: log file path
	CString fieldname;	// 字段名
	CString fieldvalue;	// 字段值
	int overtime;	// 命令执行时间
	CString nginformation;	// 错误信息
	int lineno;	// 脚本执行行数
 };

// 定义通信方式
#define mode_tcp	1
#define mode_com	2
#define mode_gpib	3
#define mode_sshv2	4
#define mode_directtcp	5

// 定义终端方式参数
struct trmmode
 {
	// 注释符号
	CString comment;
	// 初始化回车
	int initreturn;
	// 自动登陆
	int autologin;
	// 自动登陆匹配用户显示内容
	CString displayuser;
	// 自动登陆匹配密码显示内容
	CString displaypwd;
	// 自动登陆用户
	CString user;
	// 自动登陆密码
	CString pwd;
	// 终端提示符
	CString prompt;
	// 回显
	int displayback;
	// 反射键
	CString reflectkey;
	// delay ms
	int delaytime;
	//  exit行为
	CString exitaction;
	//	发送命令行末处理
	CString endofline;
	//	
	CString headflag;
	CString tailflag;
	CString heartmsg;
	int heartintv;
	// com参数
	//  波特率
	int BaudRate;
	//  数据位
	int ByteSize;
	//  校验方式
	//	#define NOPARITY	0
	//	#define ODDPARITY	1
	//	#define EVENPARITY	2
	//	#define MARKPARITY	3
	//	#define SPACEPARITY	4
	int Parity;
	//  停止位
	//	#define ONESTOPBIT	0
	//	#define ONE5STOPBITS	1
	//	#define TWOSTOPBITS	2
	int StopBits;
 };

// char buffer
#define CHARBUFFER 4096
// Size of read buffer
#define BUFFERSIZE	15000 
// IP
//  Size of read buffer
#define IPBUFFERSIZE	65536 
// COM
//  Size of read buffer
#define COMBUFFERSIZE	4096 
// GPIB
//  Size of read buffer
#define GPIBBUFFERSIZE	4096 
//  Board handle
#define GPIB0	0                

// 注释行分解参数
#define COMMENTPARSEPARA 32        
// 命令分隔符号
#define CONFIGDELIM _T(";")

// dlg 超时时间
#define DLGOVERTIME 300

// tew 版本结构
struct tewversion
{
	int vmah;
	int vmal;
	int vlah;
	int vlal;
};

// udp load 结构
struct tewudpdata
{
	tewversion version;
};

// udp 结构
struct tewudp
{
	int check;
	int optcode;	// 目前只有版本公告
	tewudpdata udpdata;
};

#define UDPDATACHECK 0x5a5a5a5a

// direct tcp packet
struct tewchars
{
	char data[BUFFERSIZE];
	int len;
};

#endif