
// tew.cpp : ����Ӧ�ó��������Ϊ.
//

#include "stdafx.h"
#include "tew.h"
#include "tewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtewApp

BEGIN_MESSAGE_MAP(CtewApp, CWinApp)
END_MESSAGE_MAP()


// CtewApp ����

CtewApp::CtewApp()
{
	// TODO: �ڴ˴���ӹ������,
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CtewApp ����

CtewApp theApp;


// CtewApp ��ʼ��

BOOL CtewApp::InitInstance()
{
	CWinApp::InitInstance();


	// ���� shell ������,�Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ�.
	CShellManager *pShellManager=new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С,��Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ���,
	// �����޸�Ϊ��˾����֯��
	// �������Ļ���
	setlocale(LC_ALL,"chs");
	SetRegistryKey(_T("tew,xaviet@163.com"));

	// ����dlg color
	//SetDialogBkColor(RGB(188,197,230),RGB(13,125,188)); 

	CtewDlg dlg;
	m_pMainWnd=&dlg;
	INT_PTR t_nResponse=dlg.DoModal();
	if (t_nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (t_nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ������.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹر�,���Խ����� FALSE �Ա��˳�Ӧ�ó���,
	//  ����������Ӧ�ó������Ϣ��.
	return FALSE;
}
