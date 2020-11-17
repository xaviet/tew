#pragma once
#include "config.h"
#include "afxwin.h"
#include "trmthread.h"
#include "trmthreaddirecttcp.h"
#include "tewDlg.h"

// ctrmdlg 对话框

class ctrmdlg : public CDialogEx
{
	DECLARE_DYNAMIC(ctrmdlg)

public:
	ctrmdlg(CWnd* pParent=NULL);   // 标准构造函数
	virtual ~ctrmdlg();

// 对话框数据
	enum { IDD=IDD_trmdlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	// 重绘edit控件
	int resizeedittrmdlg();
	virtual void PostNcDestroy();
	// terminal work
	int trmwork();
	// trm thread point
	CWinThread *m_pctrmthread;
	// 启动trmthread线程  
	int threadbegintrmthread();
	// trmdlg status changed
	int trmstatuschanged(int v_i);
	// 由文件匹配runmode
	int trmgetmodefromfile();
	// m_mapmode初始化
	int trminitmapmode();
	// 获取mode配置
	int trmgetmodefrommap();
	 // 配置trmthread参数
	int settrmthreadpara();
	// appdlg stop command
	int m_stopflag;

protected:
	// 终端模式配置泛型
	map<CString,CString> m_mapmode; 

public:
	// edit 控件字体
	CFont m_editFont;
	// 终端内容
	CEdit m_edittrmdlg;
	// dlg size
	CRect m_rc;
	// 终端方式配置结构
	struct trmmode m_trmmode;
	// node itemdata
	struct treeitemdata *m_pitemdata;
	// tew dialog point
	CtewDlg *m_ptewdlg;
	// 定义通信方式
	//	#define mode_tcp	1
	//	#define mode_com	2
	//	#define mode_gpib	3
	//	#define mode_sshv2	4
	//	#define mode_directtcp	5
	int m_modecomm;
	// appdlg stop command
	afx_msg LRESULT Ontrmdlgstop(WPARAM wParam, LPARAM lParam);
	// trm dlg resize 
	afx_msg LRESULT Ontrmdlgresize(WPARAM wParam, LPARAM lParam);
	// trm dlg cedit add cstring
	afx_msg LRESULT Ontrmdlgeditaddcstring(WPARAM wParam, LPARAM lParam);
	// 出现错误
	afx_msg LRESULT Ontrmdlgtrmfaultcounter(WPARAM wParam, LPARAM lParam);
	// 设置存储值
	afx_msg LRESULT Ontrmdlgsetsavevalue(WPARAM wParam, LPARAM lParam);
	// 读取存储值
	afx_msg LRESULT Ontrmdlggetsavevalue(WPARAM wParam, LPARAM lParam);
	// setcsv
	afx_msg LRESULT Ontrmdlgsetcsv(WPARAM wParam, LPARAM lParam);
	// log list add
	afx_msg LRESULT Ontrmdlgctrladdloglist(WPARAM wParam, LPARAM lParam);
	// replace label
	afx_msg LRESULT Ontrmdlgreplacelabel(WPARAM wParam, LPARAM lParam);
	// module begin or end
	afx_msg LRESULT Ontrmdlgmodule(WPARAM wParam, LPARAM lParam);
	// get timer
	afx_msg LRESULT Ontrmdlgovertime(WPARAM wParam, LPARAM lParam);
	// trm msg
	afx_msg LRESULT Ontrmdlgmsg(WPARAM wParam, LPARAM lParam);
	// ctrl stop
	afx_msg LRESULT Ontrmdlgctrlstop(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnCancel();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
