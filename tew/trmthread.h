#pragma once

#include "trmdlg.h"
#include "config.h"
#include "afxsock.h"
#include "ni488.h"
#include "dlgwaiting.h"
#include "dlgsystemcmd.h"
#include "ssh2.h"
#include "libssh2_sftp.h"
#include "regex"

// ctrmthread

// 申明class ctrmthread,以便结构体commandfunction使用
class ctrmthread;
// 命令原语
//	原语数量
#define COMMANDPRIMITIVESQUANTITY	22  
struct commandfunction
{
	// 命令原语
	CString primitives;
	// 函数指针
	int (ctrmthread::*f)(void);
};
// 定义一个以对象调用函数指针的方法，其中：obj：调用成员函数指针的对象；pointer：成员函数的指针。
// 注意：因为".*"优先级较低，所以该符号的两端，均需加上括号
#define callmemfun(obj,pointer) ((obj).*(pointer))
// 定义一个以对象指针调用函数指针的方法，其中：pobj：调用成员函数指针的对象指针；pointer：成员函数的指针。
// 注意：因为"->*"优先级较低，所以该符号的两端，均需加上括号
#define callptrfun(obj,pointer) ((obj)->*(pointer))

class ctrmthread : public CWinThread
{
	DECLARE_DYNCREATE(ctrmthread)

protected:
	// 动态创建所使用的受保护的构造函数
	ctrmthread();          
	virtual ~ctrmthread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	// 重载
	virtual int Run();
	// trdlg point
	CDialogEx *m_ptrmdlg;
	// node itemdata
	struct treeitemdata *m_pitemdata;
	// 终端方式配置结构
	struct trmmode *m_ptrmmode;
	// 定义通信方式
	//	#define mode_tcp	1
	//	#define mode_com	2
	//	#define mode_gpib	3
	//	#define mode_sshv2	4
	//	#define mode_directtcp	5
	int m_modecomm;
	// ip
	CSocket m_modeip;
	// com
	HANDLE m_modecom;
	// gpib
	Addr4882_t m_modegpib;
	// gpib handle
	int m_hgpib;
	// comm tx buffer
	CString m_cstxbuff;
	// comm rx buffer
	CString m_csrxbuff;
	// script line计数
	int m_lineno;
	// script line内容
	CString m_linestring;
	// script file handle
	CStdioFile m_csfscriptfile;
	// trm log file handle
	CStdioFile m_csflogfile;
	// ctrl edit buffer part
	CString m_trmdlgeditbuffpart;
	/*
	 *  After each GPIB call, the application checks whether the call
	 *  succeeded. If an NI-488.2 call fails, the GPIB driver sets the
	 *  corresponding bit in the global status variable. If the call
	 *  failed, this procedure prints an error message, takes the board
	 *  offline and exits.
	 */
	void trmmodeGPIBCleanup(int v_ud, CString v_cs);
	// 出现错误
	int trmfaultcounter();

protected:
	DECLARE_MESSAGE_MAP()
	
private:
	// script line comment flag
	volatile  int m_commentflag;
	// 终端提示符匹配
	volatile int m_matchnormal;
	// 严格匹配,忽略终端提示符
	volatile int m_matchstrict;
	// send v_str to trmdlg edit ctrl
	int trmsendtrmdlgcstring(CString v_str);
	// send char to trmdlg edit ctrl
	int trmsendtrmdlgchar(wchar_t *v_wch);
	// script run
	int Ontrmthreadrun();
	// initreturn
	int Ontrmthreadinitreturn();
	// autologin
	int Ontrmthreadautologin(int v_i);
	// mode_comm初始化
	int trmmodecomminit(int v_mode);
	//  mode_comm结束
	int trmmodecommexit(int v_mode);
	// read script file one line
	int trmreadscriptfileline();
	// 判断脚本行类型
	int trmscriptlineiscomment(CString *v_pcsline);	
	// 终端receive
	int trmmodecommrx(int v_mode);
	// 终端transmit
	int trmmodecommtx(int v_mode);
	// rxbuff ok
	int trmrxbufferok();
	// 分析注释行
	//	m_para[0]命令
	//	m_para[1~9]参数
	int trmparsecomentline(CString v_linestring);
	// 分析注释行命令
	int trmparsecomentlinecommand();
	// 注释行分解参数
	CString m_para[COMMENTPARSEPARA];
	// replace "\\n" to 0x0a'\n',"\\r" to 0x0d'\r'
	int	trmparsereplacechar(CString *v_pcs);
	// ng times
	int m_ngtimes;
	// 替换字符串中变量符号,"$_"开头
	//	return TRUE:none
	//	return FALSE:exist
	int trmtxbuffgetstring4replacelabel(CString *v_line);
	// 格式化macXX.XX.XX.XX.XX.XX to XXXX.XXXX.XXXX
	CString trmmacformat(CString v_line);
	// 保留数字字符
	int trmcutnonnumber(CString *v_cs);
	// 获取时间 v_type:0-format;1-dir used
	CString getsystemdatetime(int v_type=0);
	// 命令开始时间
	int m_cmdbegintime;
	// 命令计时开始
	int trmcmdtimer();
	// 命令计时开始,return:true-overtime;false-normal
	int trmcmdovertimedetect();
	// trm set nginformation
	int trmsetnginformation(CString v_cs);
	// 命令原语
	//commentline:
	//	[comment][comment]:strict match;[1]=match string;
	int primitivesstrictmatch();
	//	[comment]:normal match;[1]=match string;
	int primitivesnormalmatch();
	//	[comment]wait:waiting secounds;[1]=dialog confim time(0:always/[1,):secounds);[2]=dialog string
	int primitiveswaiting();
	//	[comment]cmp:include key string;[1]=match string;[2]=key string;[3]=start position;[4]=lengh;[5]=save value number[0,1023];
	int primitivescmp();
	//	[comment]cmpn:no include key string;[1]=match string;[2]=key string;[3]=start position;[4]=lengh;[5]=save value number[0,1023];
	int primitivescmpn();
	//	[comment]save:save resualt;[1]=match string;[2]=save value number[0,1023];[3]=start position("":the first word);[4]=lengh;
	int primitivessave();
	//	[comment]saveconst:save resualt;[1]=const;[2]=save value number[0,1023];
	int primitivessaveconst();
	//	[comment]count:count resualt;[1]=the first value[0,1023];[2]="<","="">";[3]=the second value[0,1023];[4]=save value number[0,1023];
	int primitivescount();
	//	[comment]countf:count resualt;[1]=the first value[0,1023];[2]="<","="">";[3]=the second value[0,1023];[4]=save value number[0,1023];
	int primitivescountf();
	//	[comment]buffcut:cut buff;[1]=start position("":buff first buff byte);[2]=stop position("":last buff byte);
	int primitivesbuffcut();
	//	[comment]csv:save csv file;[1]=save value number[0,1023]OR newline label['n'(new line),''(no new line)];
	int primitivescsv();
	//	[comment]ctrld:none;
	int primitivesctrld();
	//	[comment]module:[1]=module name;[2]=flag[begin,end];
	int primitivesmodule();
	//	[comment]sleep:sleep secounds;[1]=sleep confim time(0:always/[1,):secounds);[2]=sleep string;
	int primitivessleep();
	//	[comment]systemcmd:execute system command;[1]=cmd;[2~31]=input;
	int primitivessystemcmd();
	//	[comment]msg:terminal message;[1]=cmd(add/check/chkclr/clear);[2]=message;
	int primitivesmsg();
	//	[comment]ctrlstop:[1]=save value number[0,1023];
	int primitivesctrlstop();
	//	[comment]label:[1]=label name;
	int primitiveslabel();
	//	[comment]goto:[1]=label name;[2]=save value number[0,1023];[3]=match string;
	int primitivesgoto();
	//	[comment]goton:[1]=label name;[2]=save value number[0,1023];[3]=match string;
	int primitivesgoton();
	//	[comment]sftp:[1]=operate tx/rx;[2]=local path+file name;[3]=remote path+file name;
	int primitivessftp();
	//	[comment]ngmon:[1]=on/off;
	int primitivesngmon();
	// 通信参数结构
	struct commandfunction m_commandfunctionarray[COMMANDPRIMITIVESQUANTITY];
	// init command function array
	int trminitcommandfunctionarray();
	// cmp and cmpn;0-cmp;1-cmpn
	int primitivescmpnandcmp(int v_flag);
	// sshv2 vlaue
	int m_sshv2rc;
	int m_sshv2auth;
	LIBSSH2_SESSION *m_psession;
    LIBSSH2_CHANNEL *m_pchannel;
	// sftp
	LIBSSH2_SFTP *m_sftp_session;
    LIBSSH2_SFTP_HANDLE *m_sftp_handle;
	int trmsftpprocess(CString v_cs);
	CString m_cskeyfilepub;
	CString m_cskeyfile;
	// unicode hex to char
	int trmunicodehex2char(CString,char*);
	// ngmon flag
	int m_ngmon;
};


