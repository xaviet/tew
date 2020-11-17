#pragma once
#include "afxwin.h"
#include "config.h"
#include "scripteditdlg.h"

// cnodepropertydlg �Ի���

class cnodepropertydlg : public CDialogEx
{
	DECLARE_DYNAMIC(cnodepropertydlg)

public:
	cnodepropertydlg(CWnd* pParent=NULL);   // ��׼���캯��
	virtual ~cnodepropertydlg();

// �Ի�������
	enum { IDD=IDD_cfgtreenodeproperty };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	struct treeitemdata *m_pitemdata;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickededitfileedit();
	// app dialog point
	CDialogEx *m_pappdlg;
	// dlg scripteditdlg exit
	afx_msg LRESULT Ondlgscripteditdlgexit(WPARAM wParam, LPARAM lParam);

protected:
	// level
	CEdit m_level;
	// path
	CEdit m_path;
	// exitfalg
	CEdit m_exitflag;
	// runstatus
	CEdit m_runstatus;
	// ptrdlg
	CEdit m_ptrdlg;
	// message
	CEdit m_message;
	// enableflag
	CButton m_enableflag;
	// runmtmode
	CButton m_runmtmode;
	// trmtabctrlnumber
	CEdit m_trmtabctrlnumber;
	// port
	CEdit m_port;
	// host
	CEdit m_host;
	// filename
	CEdit m_filename;
	// comment
	CEdit m_comment;
	// fileedit
	CButton m_editfileedit;
	// loopcounter
	CEdit m_editloopcounter;

private:
	// ����޸�
	int paramentcheck(int v_i);
	// �ű��༭dlg point
	cscripteditdlg *m_pscripteditdlg;
};
