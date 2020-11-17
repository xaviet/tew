#pragma once

#include "config.h"
#include "afxsock.h"

// trmthreaddirecttcp

// 申明class ctrmthread,以便结构体commandfunction使用
class ctrmthreaddirecttcp;
// 命令原语
//	原语数量
#define DIRECTTCPCMD 7 
struct directtcpcmd
{
	// 命令原语
	CString primitives;
	// 函数指针
	int (ctrmthreaddirecttcp::*f)(void);
};

class ctrmthreaddirecttcp : public CWinThread
{
	DECLARE_DYNCREATE(ctrmthreaddirecttcp)

protected:
	ctrmthreaddirecttcp();           // 动态创建所使用的受保护的构造函数
	virtual ~ctrmthreaddirecttcp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	// trdlg point
	CDialogEx *m_ptrmdlg;
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
	// node itemdata
	struct treeitemdata *m_pitemdata;
	// script file handle
	CStdioFile m_csfscriptfile;
	// trm log file handle
	CStdioFile m_csflogfile;
	// 重载
	virtual int Run();
	// unicode hex to char
	int trmunicodehex2char(CString,char*);
	// char to unicode hex
	CString trmchar2unicodehex(char*,int);

protected:
	DECLARE_MESSAGE_MAP()

private:
	// rxbuff ok
	int trmrxbufferok();
	// mode_comm初始化
	int trmmodecomminit(int v_mode);
	// 注释行分解参数
	CString m_para[COMMENTPARSEPARA];
	// 通信参数结构
	struct directtcpcmd m_commandfunctionarray[DIRECTTCPCMD];
	// init command function array
	// 命令原语
	//commentline:
	//	[comment]tcprx:tcp rx;[1]=key value;[2]=target value;[3]=mask;[4]=start position;[5]==peroid;[6]=save value;
	//	[comment]tcptx:tcp tx;[1]=tx value;
	//	[comment]wait:wait second;[1]=seconds;[2]=message;
	//	[comment]rxbuffclear:rx buff clear;
	//	[comment]msg:terminal message;[1]=cmd(add/check/chkclr/clear);[2]=message;
	//	[comment]module:[1]=module name;[2]=flag[begin,end];
	//	[comment]sleep:sleep second;[1]=seconds;[2]=message;
	int trminitcommandfunctionarray();
	//	[comment]tcprx:tcp rx;[1]=key value;[2]=target value;[3]=mask;[4]=start position;[5]=peroid;[6]=save value;
	int primitivestcprx();
	//	[comment]tcptx:tcp tx;[1]=tx value;
	int primitivestcptx();
	//	[comment]wait:wait second;[1]=seconds;[2]=message;
	int primitiveswait();
	//	[comment]rxbuffclear:rx buuf clear;
	int primitivesrxbuffclear();
	//	[comment]msg:terminal message;[1]=cmd(add/check/chkclr/clear);[2]=message;
	int primitivesrxmsg();
	//	[comment]module:[1]=module name;[2]=flag[begin,end];
	int primitivesmodule();
	//	[comment]sleep:sleep second;[1]=seconds;[2]=message;
	int primitivessleep();
	// 分析注释行命令
	int trmparsecomentlinecommand();
	//  mode_comm结束
	int trmmodecommexit(int v_mode);
	// send v_str to trmdlg edit ctrl
	int trmsendtrmdlgcstring(CString v_str);
	// 出现错误
	int trmfaultcounter();
	// tcp rx
	int trmtcprx(void* v_recvalue,int v_buffsize);
	// tcp tx
	int trmtcptx(void* v_recvalue,DWORD v_buffsize);
	// clear socket buff
	int trmclearsocketbuff();
	// init log file
	int trmlogfileinit();
	// init script file
	int trmscriptfileinit();
	// init socket
	int trmsocketinit();
	// trm set nginformation
	int trmsetnginformation(CString);
	//	loop
	int ontrmdirecttcprun();
	//	script line
	int m_lineno;
	//	script line string
	CString m_linestring;
	// read script file one line
	//	TRUE good
	//	FALSE end
	int trmreadscriptfileline();
	//	parse script file one line
	int trmparsescriptfileline();
	//	tewDlg timer
	int m_timer;
	//	get tewDlg timer
	int trmgettewdlgtimer();
	//  tx & rx packet
	tewchars m_rxpacket;
	tewchars m_txpacket;
	//  last tx time
	int m_lasttxtime;
	//  tx packet
	int trmtxpacket();
	//  rx command flag
	int m_cmdrxflag;
	//  rx begin time
	int m_rxtime;
	//  rx packet
	int trmrxpacket();
	//  wait begin time
	int m_waittime;
	//  wait
	int trmwait();
	// 分析注释行
	//	m_para[0]命令
	//	m_para[1~9]参数
	int trmparsecomentline(CString);
	// 获取时间 v_type:0-format;1-dir used
	CString getsystemdatetime(int);
	// log msg
	int trmlogmsg(CString);
	// ng flag
	int m_ngflag;
	// rx buffer parse
	int trmrxbuffparse(char*,int);
	// recoder
	int trmrecoder(CString);
	// rx data match key value
	int m_matchkeyvalue;
	// rx judge
	int trmrxjudge();
	// chars in chars
	char* charschars(char*,int,char*,int);
	// target value judge
	int trmtargetvaluejudge(char*,int,char*,int,char*);
	// cstring to int
	int cstring2int(CString);
	//	save value
	int trmsavevalue(CString);
	//	trm thread exit
	int trmthreadexit();
	//	trm msg process
	int trmmsg();
	//  msg operate mode
	int m_mode;
};
