#pragma once
#include "afxcmn.h"


// cdlgproductresult �Ի���

class cdlgproductresult : public CDialogEx
{
	DECLARE_DYNAMIC(cdlgproductresult)

public:
	cdlgproductresult(CWnd* pParent=NULL);   // ��׼���캯��
	virtual ~cdlgproductresult();

// �Ի�������
	enum { IDD=IDD_dlgproductresult };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	// tew dialog point
	CDialogEx *m_ptewdlg;
	// init list
	int productresultinitlist();
	virtual BOOL OnInitDialog();
	// product result
	CListCtrl m_listproductresult;
	// list��Ŀ����
	int m_iitemcounter;
};
