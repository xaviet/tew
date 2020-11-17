
// tewDlg.h : ͷ�ļ�
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

// CtewDlg �Ի���
class CtewDlg : public CDialogEx
{
	// ����
public:
	CtewDlg(CWnd* pParent=NULL);	// ��׼���캯��

	// �Ի�������
	enum { IDD=IDD_TEW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
private:
	// ��ʼ��app����,���������ʼ��
	int	initall();
	// ��ʼ��app_cfg.dat & mode_comm.dat,�������򴴽���д��ȱʡ����
	int initfile(CString v_filename,CString v_cs);
	// ��ʼ��app����map
	int initmapappcfg();
	// ���ļ��õ�����map
	int getfile2map(CStdioFile *v_pcsf,map<CString,CString> *v_map);
	// ������map�õ��ļ�
	int getmap2file(CStdioFile *v_pcsf,map<CString,CString> *v_map);
	// app����map���浽�ļ�
	int getcfgmapappcfg2file();
	// ��ʼ���Ի������
	int initdlgview();
	// ���Ӳ˵���
	int initdlgviewmenu();
	// ���ӹ�����
	int initdlgviewtoolsbar();
	// ���״̬��,v_iΪ1����,0Ϊ�ػ�
	int initdlgviewstatusbar(int v_i);
	//	v_id:0��ʾ������,ID_dlgstatusbarctrl��ʾ״̬��
	int repainttoolstatus(int v_id);
	// �ػ�ctrl log list
	int repaintloglist();
	// �ػ���ؼ�
	int repaintctrl(int v_x,int v_y);
	// resize���¼size to map
	int getsize2mapappcfg();
	// ��ʼ��cfgtree ctrl
	int	initctrlcfgtree();
	// ��ʼ��trm tab ctrl
	int	initctrltrmtab();
	// ��ʼ��log list ctrl
	int	initctrlloglist();
	// ʹ�ܻ��ֹ�˵�
	int setmenuaction(int v_menu, int v_submenu, int v_flag);
	// get dlg ����ؼ���϶
	int getdlgctrlscale();
	// ��·����ȡĿ¼
	CString getdirfrompath(CString v_cspath);
	// ��·����ȡĿ¼
	CString getfilefrompath(CString v_cspath);
	// �����û�define�ļ�
	int getuserdefinefromfile(CString v_csdir,CString v_csfile);
	// �����û�defineĿ¼
	int getuserdefinefromdir(CString v_csdir);
	// ����Ŀ¼�Ƿ����userdefine file, return 1 means yes,0 means no
	int getuserdefinefilefromdir(CString v_csdir);
	// ����tree�ؼ�ʹ��imagelist
	int initctrlcfgtreeimagelist();
	// set�û�define node
	HTREEITEM setuserdefinetreenode(HTREEITEM v_fathernode,treeitemdata *v_ptreeitemdata);
	// �������ؼ��ڵ�
	//	0-ɾ��Я����Ϣָ�룬��սڵ�Я����Ϣ����
	//	1-չ���ڵ�
	//	2-save
	//	3-scan script filename
	//	4-dlgstop
	//	5-dlgstart icon&text��ʼ��
	//  6-trm log path init
	//	7-replace $_
	//	8-������Խ����¼,����ng������
	int setuserdefinetreeitem(HTREEITEM v_nodea,int v_i=0,CStdioFile *v_pcsf=0);
	// ������ؼ�node
	int setuserdefinetreeempty();
	// parse userdefine with file
	int getuserdefinebyparsefile(CStdioFile *v_pcsf,HTREEITEM v_fathernode);
	// parse userdefine with old file
	int getuserdefinebyparseoldfile(CStdioFile*v_csf,treeitemdata *v_pid);
	// parse userdefine with line
	HTREEITEM getuserdefinebyparseline(CString v_csline,HTREEITEM v_fathernode);
	// �ļ����ݺ���������Ϣ
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
	// tabctrl����dlg�ػ�/repaint ctrl trmdlg
	int repaintctrltrmdlg(HTREEITEM v_node);
	// ����,����
	int runtewdlgstart(HTREEITEM v_node);
	// ����,exit
	int runtewdlgexit(HTREEITEM v_node);
	// ����,clear
	int runtewdlgclear(HTREEITEM v_node);
	// ˢ�µ�ǰ�ڵ�
	int setcurtreenode(HTREEITEM v_node);
	// ģ�������
	int setmouseclick(CPoint v_pt);
	// ��ʼ���ն�ҳ������
	int inittrmtabpageitemdataarray();
	// ѡ��tabpage������trmdlg��cfgtree
	int settrmtabpage(int v_i);
	// ���ڵ����״̬,���������ڵ�
	int setuserdefinenodeexitflag(treeitemdata *v_ptreeitemdata);
	// ���ؼ�Я����Ϣenableflag
	//	v_flag:1-enable;0-disable
	int modifytreeallitemdatarunmode(HTREEITEM v_node,int v_flag);
	// ��ȡʱ�� v_type:0-format;1-dir used
	CString getsystemdatetime(int v_type=0);
	// ����cfg tree
	int getuserdefinecfgtree();
	// ��ʼ��logĿ¼
	int pathinitlogpath(CString v_logpath);
	// �ж�Ŀ¼�Ƿ����
	bool pathgetfolderexists(CString v_cs);
	// �ݹ鴴��Ŀ¼�� ���Ŀ¼�Ѿ����ڻ��ߴ����ɹ�����TRUE
	bool pathmkdir(CString v_cspath);
	// ��ʼ��csv�ļ�
	int pathinitcsvfile();
	// ׼����ʼ
	int runtewdlgperstart();
	// ��ȡ�������Գ���
	int getproducttest();
	// ��ȡdatabase����
	int getdatabase();
	// ��ȡmapdutmessage
	int getdatafile2mapdutmessage();
	// �������Խ��ȷ��
	int getproducttestresultmsg();
	// �����������Ա���
	int setproducttestresultmsg(CString v_csdirname);
	// �ݹ齨��Ŀ¼����tree
	int setuserdefinetree4path(CString v_cspath,treeitemdata *v_pid);
	// ������ʹ�ܻ��ֹ
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
	// ������Խ����¼,����ng������
	int setnginformation();
	// ��ʼ��tcp server port:MNTPORT,ά��ʹ��
	int initdlgtcpserver();
	// ִ�����ñ�־
	struct tewdlgconfigflag m_configflag;
	// �ն���Ϣ
	CString m_trmmsg;
	// ��path��ȡid list
	LPITEMIDLIST GetItemIDListFromFilePath(CString v_strFilePath);
	//���ݿ����Ӿ��
	MYSQL *m_pmysql;
	// ��ʼ��mysql
	int initdlgmysql();
	// ��ʼ�� udp process
	int initdlgudp();
	// local version
	struct tewversion m_dlocalver;
	// message to save value[0,1023]
	int setmessage2savevalue(CString v_message);

protected:
	// app����map
	map<CString,CString> m_mapappcfg; 
	// dlg�˵�point
	CMenu *m_pmenu;
	// ״̬���ؼ�ʵ��
	CStatusBarCtrl m_statusbarctrl;
	// ����������
	CToolBar m_toolbar;		// ������
	CImageList m_toolbarimagelist;	// ������ͼ����
	CToolBarCtrl *m_ptoolbarctrl;	// ����������ָ��
	CToolTipCtrl m_tooltipctrl;	// CToolTipCtrl�ؼ�,Ϊtoolbar�ṩtip
	// config tree ctrl
	CTreeCtrl m_cfgtree;
	// ����tree�ؼ�ʹ��imagelist 
	CImageList m_cfgtreeimageList;
	// trm tab ctrl
	CTabCtrl m_trmtab;
	// log list ctrl
	CListCtrl m_loglist;
	// cfg trm log �ؼ�����
	volatile int m_ctrlpositionarray[3][4];
	// dlg ����ؼ���϶
	CPoint m_pointns1,m_pointns2,m_pointwe1,m_pointwe2;
	// �޸�scale��Ϊflag,1 indicate x,2 indicate y,0 indicate none
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
	// ��ݼ�ȫ�ֱ���
	HACCEL m_hAccTable;
	// �ն�ҳ������
	struct treeitemdata *m_ptrmtabpageitemdataarray[TABCTRLMAXPAGENUMBER];
	// report����map
	map<CString,CString> m_mapmodulefield;
	// dut message
	map<CString,CString> m_mapdutmessage;
	// ִ�п�ʼʱ��
	CString m_csstarttime;
	// �ź���,�̼߳�ͨ��
	HANDLE m_hsemaphore;
	// ͨ�ŷ�ʽģ���ļ���
	CString m_csmodecomm;
	// ��ȡӦ�ó���Ŀ¼
	CString getappdir();
	// ����ϵͳ��Ϣ����,����onclose��Ϣ
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	// dialog exit
	afx_msg void Ondlgexit();
	// dialog size
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// dlg��Сsize X&Y
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	// log list clear
	afx_msg void Onctrlloglistclear();
	// log list menu popup
	afx_msg void OnNMRClickloglist(NMHDR *pNMHDR, LRESULT *pResult);
	// ctrl log list ��������
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
	// �ļ�/Ŀ¼֧����ק
	afx_msg void OnDropFiles(HDROP hDropInfo);
	// ��괦�ڿؼ���϶ʱ���϶��޸�scale
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	// press mouse lbutton set scale��Ϊflag
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// press mouse lbutton unset scale��Ϊflag
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	// ���ִ���
	afx_msg LRESULT Onappdlgtrmdlgtrmfaultcounter(WPARAM wParam, LPARAM lParam);
	// save value number[0,1023]
	CString m_savevalue[SAVEVALUECOUNTER];
	// ���ô洢ֵ
	afx_msg LRESULT Onappdlgsetsavevalue(WPARAM wParam, LPARAM lParam);
	// ��ȡ�洢ֵ
	afx_msg LRESULT Onappdlggetsavevalue(WPARAM wParam, LPARAM lParam);
	// setcsv
	afx_msg LRESULT Onappdlgsetcsv(WPARAM wParam, LPARAM lParam);
	// �滻�ַ����б�������,"$_"��ͷ
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
	// ��������
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
