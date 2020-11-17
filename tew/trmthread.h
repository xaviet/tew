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

// ����class ctrmthread,�Ա�ṹ��commandfunctionʹ��
class ctrmthread;
// ����ԭ��
//	ԭ������
#define COMMANDPRIMITIVESQUANTITY	22  
struct commandfunction
{
	// ����ԭ��
	CString primitives;
	// ����ָ��
	int (ctrmthread::*f)(void);
};
// ����һ���Զ�����ú���ָ��ķ��������У�obj�����ó�Ա����ָ��Ķ���pointer����Ա������ָ�롣
// ע�⣺��Ϊ".*"���ȼ��ϵͣ����Ը÷��ŵ����ˣ������������
#define callmemfun(obj,pointer) ((obj).*(pointer))
// ����һ���Զ���ָ����ú���ָ��ķ��������У�pobj�����ó�Ա����ָ��Ķ���ָ�룻pointer����Ա������ָ�롣
// ע�⣺��Ϊ"->*"���ȼ��ϵͣ����Ը÷��ŵ����ˣ������������
#define callptrfun(obj,pointer) ((obj)->*(pointer))

class ctrmthread : public CWinThread
{
	DECLARE_DYNCREATE(ctrmthread)

protected:
	// ��̬������ʹ�õ��ܱ����Ĺ��캯��
	ctrmthread();          
	virtual ~ctrmthread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	// ����
	virtual int Run();
	// trdlg point
	CDialogEx *m_ptrmdlg;
	// node itemdata
	struct treeitemdata *m_pitemdata;
	// �ն˷�ʽ���ýṹ
	struct trmmode *m_ptrmmode;
	// ����ͨ�ŷ�ʽ
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
	// script line����
	int m_lineno;
	// script line����
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
	// ���ִ���
	int trmfaultcounter();

protected:
	DECLARE_MESSAGE_MAP()
	
private:
	// script line comment flag
	volatile  int m_commentflag;
	// �ն���ʾ��ƥ��
	volatile int m_matchnormal;
	// �ϸ�ƥ��,�����ն���ʾ��
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
	// mode_comm��ʼ��
	int trmmodecomminit(int v_mode);
	//  mode_comm����
	int trmmodecommexit(int v_mode);
	// read script file one line
	int trmreadscriptfileline();
	// �жϽű�������
	int trmscriptlineiscomment(CString *v_pcsline);	
	// �ն�receive
	int trmmodecommrx(int v_mode);
	// �ն�transmit
	int trmmodecommtx(int v_mode);
	// rxbuff ok
	int trmrxbufferok();
	// ����ע����
	//	m_para[0]����
	//	m_para[1~9]����
	int trmparsecomentline(CString v_linestring);
	// ����ע��������
	int trmparsecomentlinecommand();
	// ע���зֽ����
	CString m_para[COMMENTPARSEPARA];
	// replace "\\n" to 0x0a'\n',"\\r" to 0x0d'\r'
	int	trmparsereplacechar(CString *v_pcs);
	// ng times
	int m_ngtimes;
	// �滻�ַ����б�������,"$_"��ͷ
	//	return TRUE:none
	//	return FALSE:exist
	int trmtxbuffgetstring4replacelabel(CString *v_line);
	// ��ʽ��macXX.XX.XX.XX.XX.XX to XXXX.XXXX.XXXX
	CString trmmacformat(CString v_line);
	// ���������ַ�
	int trmcutnonnumber(CString *v_cs);
	// ��ȡʱ�� v_type:0-format;1-dir used
	CString getsystemdatetime(int v_type=0);
	// ���ʼʱ��
	int m_cmdbegintime;
	// �����ʱ��ʼ
	int trmcmdtimer();
	// �����ʱ��ʼ,return:true-overtime;false-normal
	int trmcmdovertimedetect();
	// trm set nginformation
	int trmsetnginformation(CString v_cs);
	// ����ԭ��
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
	// ͨ�Ų����ṹ
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


