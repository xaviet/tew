#pragma once
#include "afxwin.h"
#include "config.h"

//  ����cedit

#define MES_UNDO        _T("&Undo")
#define MES_CUT         _T("Cu&t")
#define MES_COPY        _T("&Copy")
#define MES_PASTE       _T("&Paste")
#define MES_DELETE      _T("&Delete")
#define MES_SELECTALL   _T("Select &All")

class cscriptedit : public CEdit
{
public:
    cscriptedit();
	int oneditinit();

protected:
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    DECLARE_MESSAGE_MAP()

private:
	// ��ǰ�༭��ѡ��λ��
	int m_iposition;
	// NG array
	int m_ngarray[256];
	// ѡ��༭������λ��
	int setposition(int,int);
	// ��λ��һ��NG
	int onscripteditlastng();
	// ��λ��һ��NG
	int onscripteditnextng();
};

// cscripteditdlg �Ի���

class cscripteditdlg : public CDialogEx
{
	DECLARE_DYNAMIC(cscripteditdlg)

public:
	cscripteditdlg(CWnd* pParent=NULL);   // ��׼���캯��
	virtual ~cscripteditdlg();
	virtual BOOL OnInitDialog();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void PostNcDestroy();
// �Ի�������
	enum { IDD=IDD_scripteditdlg };

private:
	// edit ����
	CString m_editcs;
	// �����޸ı�־
	int m_modifyflag;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// new font
	CFont m_editFont;
	// script
	cscriptedit m_scriptedit;
	// script file name
	CString m_filename;
	// node property dlg point
	CDialogEx *m_pnodepropertydlg;
	//app dlg point
	CDialogEx *m_pappdlg;
	// read only flag
	int m_readonlyflag;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnCancel();
	afx_msg void OnEnChangescripteditor();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
