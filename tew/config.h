// config.h
//	��������ͷ�ļ�

// �ر�����:
//	����Ŀ�е�ע�Ϳ�����Ҫ��ϴ����������

#ifndef xa_config
#define xa_config

#include <locale.h>	  // �������Ļ���
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
// ά���˿�
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
// cmp/cmpn�����ٴαȽ�ʱ��
#define FAULTRETRYDELAY 50
// cmp/cmpn�����ٴαȽϴ���
#define FAULTRETRYTIMES 80
// �ű���ִ�г�ʱʱ�����,��
#define OVERTIMR 900
// directtcp tx packet inteval second
#define TXPACKETINTV 1
// ���ú궨��,��ģ������
// Ӧ�ó���Ի���ָ��
#define P_TEWAPP	((CtewApp*)AfxGetApp())
// Ӧ�ó���Ի���ָ��
#define P_TEWDLG	((CtewDlg*)m_pMainWnd)

// �س�����
#define RETURNNEWLINE _T("\r\n")
// ctrl-d
#define CTRL_D 0x4
// app���������ļ�,�ļ���ʼ���ݶ���"initfile.h"
// app���г�ʼ���ļ�����
#define APP_CFG_DAT	 _T("app_cfg.dat")
// �ն˷�ʽ�����ļ�����
#define MODE_COMM_DAT	_T("mode_comm.dat")
// logĿ¼����
#define LOG_PATH	_T("zlog\\")
// csv�ļ�����
#define CSV_FILE	_T("total.csv")
// ������Խ����¼,����ng������
#define RESULT_FILE	_T("total.txt")
// app log�ļ�����
#define APP_LOG_FILE _T("tew.log")
// trm log�ļ�ext����
#define TRM_LOG_EXT	_T(".log")
// dut message file
#define DUT_MSG_FILE _T("\\��������ģ��\\lastdutmsg.dat")
// product test result path
#define PRODUCT_RESULT_PATH _T("D:\\�������Խ��\\")
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

// edit�ؼ�����:256M-1
#define	EDITCTRLLIMIT 268435455
// tab ctrl max page number
#define TABCTRLMAXPAGENUMBER 256
// path max length
#define PATHMAXLENGTH 256
// dlg��Сsize X&Y
#define DLGMINX 800
#define DLGMINY 600
// ״̬���ָ�����
#define STATUSBARSEPARATES 4 
// ������ͼ�����
#define TOOLBARBUTTONCOUNTER 6
// ������ͼ��array
#define TOOLBARICONARRAY {IDB_openfile,IDB_opendir,IDB_start,IDB_pause,IDB_stop,IDB_helpdoc}
// ����������IDarray
#define TOOLBARIDARRAY {ID_dlgopenfile,ID_dlgopendir,ID_dlgstart,ID_dlgpause,ID_dlgstop,ID_dlghelpdoc}
// cfgterrͼ�����
#define CFGTREEIMAGELIST 7
// cfgterrͼ��define
#define iconready		0
#define iconrwn			1
#define iconrwf			2
#define iconwaitting	3
#define iconcwn			4
#define iconcwf			5
#define icondisable		6
// dlg�ؼ�����
#define DLGCTRLARRAY {IDC_cfgtree,IDC_trmtab,IDC_loglist}
// dlg�ؼ�����
#define DLGCTRLSIZEARRAY	{(float)0,(float)0,(float)0.3,(float)1\
							,(float)0.3,(float)0,(float)1,(float)0.7\
							,(float)0.3,(float)0.7,(float)1,(float)1}
// �ؼ���Ե���
#define EDGEWIDTH	1
// toolbar���
#define TOOLBARWIDTH	40
// statusbar���
#define STATUSBARWIDTH	20
// ���ָ��߽��ж�
#define BORDERJUDGE	10

// �Զ�����Ϣ
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
#define WM_scripteditselectall (WM_USER+1300)	// Ԥ��100 
//  trm directtcp thread message
#define WM_trmdirecttcprun (WM_USER+1400)
#define WM_trmdirecttcptimer (WM_USER+1401)
#define WM_trmdirecttcptx (WM_USER+1402)
#define WM_trmdirecttcprx (WM_USER+1403)
#define WM_trmdirecttcpwait (WM_USER+1404)
#define WM_trmdirecttcpmsg (WM_USER+1405)

//��һ���������
//#define TRUE FALSE 

// ����tewdlg���ñ�־
struct tewdlgconfigflag
{
	int loop = 0;	// ѭ��ִ�б�־
	int ngstop = 0;	//ng����stop
	CString zlogdir = _T("");	//zlog���λ��
};

// ����user define tree�ؼ��ⲿ���ݣ�����ģ��ִ�з�ʽ
//	����ģ������ָʾ��ʽ
//	{level;path;comment;runmtmode;enableflag;message}
//	�ű�����ָʾ��ʽ
//	filename;host;[sshv2:]port;comment;runmtmode;enableflag;message;
struct treeitemdata
 {
	int level;	// level:��ʾ���в�νṹ,0��ʾroot(������).
	CString path;	// path;ģ��ű�·����ʹ�����·������Ҫ�����m_mapconfig[_T("lastpath")]��
	CString comment;	// comment:����
	int	runmtmode;	// runmtmode:0Ϊ˳��ִ��;1Ϊ����ִ��
	int enableflag;	// enableflag:0Ϊdisable;1Ϊenable
	int exitflag;	// exitflag:1Ϊexit
	int runstatus;	// cfgterrͼ��define
	CString filename;	// filename:��ʾ�ű��ļ���.*.script
	CString host;	// host;�ű��ļ�ִ�е�Ŀ��������
	int sshv2;	// use sshv2 mode
	int directtcp;	// use directtcp mode
	int	port;	// port:�ű��ļ�ִ�е�Ŀ�������˿�
	CString message;	// message:�ű�ִ�е���Ϊ��Ϣ
	int ptrdlg;	// pdlg:�ڵ��Ӧ�ն�dlgָ��
	int trmtabctrlnumber;	// tabctrlnumber:�ڵ��Ӧ�ն�tab ctrl���
	int loopcounter;	// loopcounter:ģ��ִ�д���
	HTREEITEM node;	// node:tree node handle
	CString information;	// information:�ڵ���Ϣ,���ն���Ϣ����
	int msgoperate;	// msgoperate:�ն���Ϣ����ִ�н��;TRUEΪok;FALSEΪng
	int faultcounter;	// faultcounter:�ڵ�������
	CString logpath;	// logpath: log file path
	CString fieldname;	// �ֶ���
	CString fieldvalue;	// �ֶ�ֵ
	int overtime;	// ����ִ��ʱ��
	CString nginformation;	// ������Ϣ
	int lineno;	// �ű�ִ������
 };

// ����ͨ�ŷ�ʽ
#define mode_tcp	1
#define mode_com	2
#define mode_gpib	3
#define mode_sshv2	4
#define mode_directtcp	5

// �����ն˷�ʽ����
struct trmmode
 {
	// ע�ͷ���
	CString comment;
	// ��ʼ���س�
	int initreturn;
	// �Զ���½
	int autologin;
	// �Զ���½ƥ���û���ʾ����
	CString displayuser;
	// �Զ���½ƥ��������ʾ����
	CString displaypwd;
	// �Զ���½�û�
	CString user;
	// �Զ���½����
	CString pwd;
	// �ն���ʾ��
	CString prompt;
	// ����
	int displayback;
	// �����
	CString reflectkey;
	// delay ms
	int delaytime;
	//  exit��Ϊ
	CString exitaction;
	//	����������ĩ����
	CString endofline;
	//	
	CString headflag;
	CString tailflag;
	CString heartmsg;
	int heartintv;
	// com����
	//  ������
	int BaudRate;
	//  ����λ
	int ByteSize;
	//  У�鷽ʽ
	//	#define NOPARITY	0
	//	#define ODDPARITY	1
	//	#define EVENPARITY	2
	//	#define MARKPARITY	3
	//	#define SPACEPARITY	4
	int Parity;
	//  ֹͣλ
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

// ע���зֽ����
#define COMMENTPARSEPARA 32        
// ����ָ�����
#define CONFIGDELIM _T(";")

// dlg ��ʱʱ��
#define DLGOVERTIME 300

// tew �汾�ṹ
struct tewversion
{
	int vmah;
	int vmal;
	int vlah;
	int vlal;
};

// udp load �ṹ
struct tewudpdata
{
	tewversion version;
};

// udp �ṹ
struct tewudp
{
	int check;
	int optcode;	// Ŀǰֻ�а汾����
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