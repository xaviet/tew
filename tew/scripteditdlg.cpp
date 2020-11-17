// scriptedit.cpp : 实现文件
//

#include "stdafx.h"
#include "tew.h"
#include "scripteditdlg.h"
#include "afxdialogex.h"

//  派生cedit

BEGIN_MESSAGE_MAP(cscriptedit,CEdit)
    ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

cscriptedit::cscriptedit()
{
	m_iposition=-1;
	int t_i=0;
	// init ng array
	for(;t_i<256;t_i++)
	{
		m_ngarray[t_i]=-1;
	}
}

int cscriptedit::oneditinit()
{
	int t_i=0;
	int t_temppos=0;
	CString t_cs;
	this->GetWindowText(t_cs);
	t_i=0;
	// create ng array
	for(;;)
	{
		t_temppos=t_cs.Find(_T("(NG)"),t_temppos);
		if((t_temppos==-1))
		{
			// t_temppos位置后未发现
			break;
		}
		m_ngarray[t_i]=t_temppos;
		t_i++;
		t_temppos+=1;
	}
	m_iposition=-1;
	return 0;
}

// 选择编辑框内容位置
int cscriptedit::setposition(int v_startpos,int v_endpos)
{
	this->SetSel(v_startpos,v_endpos);
	this->SetFocus();
	return 0;
}

// 定位上一个NG
int cscriptedit::onscripteditlastng()
{
	if(m_iposition>0)
	{
		m_iposition-=1;
		setposition(m_ngarray[m_iposition],m_ngarray[m_iposition]+2);
	}
	else
	{
		AfxMessageBox(_T("已到头,未发现NG"));
	}
	return 0;
}

// 定位下一个NG
int cscriptedit::onscripteditnextng()
{
	if(m_ngarray[m_iposition+1]!=-1)
	{
		m_iposition+=1;
		setposition(m_ngarray[m_iposition],m_ngarray[m_iposition]+2);
	}
	else
	{
		AfxMessageBox(_T("已到尾,未发现NG"));
	}
	return 0;
}

void cscriptedit::OnContextMenu(CWnd* pWnd, CPoint point)
{
	SetFocus();
    CMenu menu;
    menu.CreatePopupMenu();
    BOOL bReadOnly = GetStyle() & ES_READONLY;
    DWORD flags = CanUndo() && !bReadOnly ? 0 : MF_GRAYED;
    menu.InsertMenu(0, MF_BYPOSITION | flags, EM_UNDO,MES_UNDO);
    menu.InsertMenu(1, MF_BYPOSITION | MF_SEPARATOR);
    DWORD sel = GetSel();
    flags = LOWORD(sel) == HIWORD(sel) ? MF_GRAYED : 0;
    menu.InsertMenu(2, MF_BYPOSITION | flags, WM_COPY,MES_COPY);
    flags = (flags == MF_GRAYED || bReadOnly) ? MF_GRAYED : 0;
    menu.InsertMenu(2, MF_BYPOSITION | flags, WM_CUT,MES_CUT);
    menu.InsertMenu(4, MF_BYPOSITION | flags, WM_CLEAR, MES_DELETE);
    flags = IsClipboardFormatAvailable(CF_TEXT) &&
	!bReadOnly ? 0 : MF_GRAYED;
    menu.InsertMenu(4, MF_BYPOSITION | flags, WM_PASTE,MES_PASTE);
    menu.InsertMenu(6, MF_BYPOSITION | MF_SEPARATOR);
    int len = GetWindowTextLength();
    flags = (!len || (LOWORD(sel) == 0 && HIWORD(sel) ==len)) ? MF_GRAYED : 0;
    menu.InsertMenu(7, MF_BYPOSITION | flags, WM_scripteditselectall,MES_SELECTALL);
	menu.InsertMenu(8, MF_BYPOSITION | MF_SEPARATOR);
	menu.InsertMenu(9, MF_BYPOSITION,WM_scripteditlastng,_T("last NG"));
	menu.InsertMenu(10, MF_BYPOSITION,WM_scripteditnextng,_T("next NG"));
	menu.InsertMenu(11, MF_BYPOSITION | MF_SEPARATOR);

	for(int t_i=12;t_i<12;t_i++){
	menu.InsertMenu(t_i, MF_BYPOSITION,WM_scripteditselectall+1,_T("ex"));
	}

    menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
}

BOOL cscriptedit::OnCommand(WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(wParam))
    {
    case EM_UNDO:
    case WM_CUT:
    case WM_COPY:
    case WM_CLEAR:
    case WM_PASTE:
        return SendMessage(LOWORD(wParam));
    case WM_scripteditselectall:
        return SendMessage(EM_SETSEL, 0, -1);
	case WM_scripteditlastng:
		return	onscripteditlastng();
	case WM_scripteditnextng:
		return	onscripteditnextng();
	case WM_scripteditselectall+1:
		return AfxMessageBox(_T("ex"));
    default:
        return CEdit::OnCommand(wParam, lParam);
    }
}

// cscripteditdlg 对话框

IMPLEMENT_DYNAMIC(cscripteditdlg, CDialogEx)

cscripteditdlg::cscripteditdlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(cscripteditdlg::IDD, pParent)
	, m_pnodepropertydlg(0)
	, m_pappdlg(0)
	, m_modifyflag(0)
	, m_readonlyflag(FALSE)
{

}

cscripteditdlg::~cscripteditdlg()
{
}

void cscripteditdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_scripteditor, m_scriptedit);
}


BEGIN_MESSAGE_MAP(cscripteditdlg, CDialogEx)
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_scripteditor, &cscripteditdlg::OnEnChangescripteditor)
END_MESSAGE_MAP()


// cscripteditdlg 消息处理程序


BOOL cscripteditdlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 设置新字体
	m_editFont.CreatePointFont(120,_T("宋体"));
	m_scriptedit.SetFont(&m_editFont);
	m_editFont.DeleteObject();
	// TODO:  ÔÚ´ËÌí¼Ó¶îÍâµÄ³õÊ¼»¯
	assert(m_filename.GetLength());
	// edit控件能力:256M-1
	m_scriptedit.SetLimitText(EDITCTRLLIMIT);
	// SetWindowText:设置标题
	SetWindowText(_T("script---")+m_filename);
	// 根据m_readonly标志设置edit状态
	m_scriptedit.SetReadOnly(m_readonlyflag);
	// open file
	CStdioFile t_csf;
	CString t_cs;
	if(t_csf.Open(m_filename,CFile::modeRead|CFile::shareDenyNone))
	{
		// 全部内容需要删除_T('\r')，行末添加_T('\r\n')作用于edit控件内容的换行显示
		t_csf.SeekToBegin();
		// edit控件内的回车输入结果为_T('\r\n')
		// 第一行不作修改
		t_csf.ReadString(t_cs);
		t_cs.Remove(_T('\r'));
		m_editcs+=t_cs;
		// 第一行之后行尾整理"\r\n"
		while(t_csf.ReadString(t_cs))
		{
			t_cs.GetLength();
			t_cs.Remove(_T('\r'));
			t_cs=RETURNNEWLINE+t_cs;
			m_editcs+=t_cs;
		}
		t_csf.Close();
		// set ctrl
		m_scriptedit.SetWindowText(m_editcs);
		UpdateData(FALSE);
		// 由于焦点原因,cedit控件内容是全部选中,重载PreTranslateMessage使之可以接受TAB键输入
	}
	else
	{
		t_cs.Format(_T("%s open fail!"),m_filename);
		AfxMessageBox(t_cs);
	}
	// init edit
	m_scriptedit.oneditinit();
	return TRUE;  // return TRUE unless you set the focus to a control
	// Òì³£: OCX ÊôÐÔÒ³Ó¦·µ»Ø FALSE
}

// 重载系统消息处理,拦截onclose消息
BOOL cscripteditdlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	if(message==WM_CLOSE)
	{
		// 询问是否关闭,发送关闭窗口消息,进入关闭对话框的代码
		CString t_cs;
		// open file
		CStdioFile t_csf;
		if((m_modifyflag==1)&&(m_readonlyflag==FALSE))
		{
			if(AfxMessageBox(_T("确认保存修改?\n点击否将关闭."),MB_YESNO)==IDYES)
			{
				if(t_csf.Open(m_filename,CFile::modeWrite|CFile::modeCreate|CFile::shareDenyNone))
				{
					UpdateData(TRUE);
					m_scriptedit.GetWindowText(t_cs);
					// 删除全部_T('\r')，stdiofile控件写将_T('\n')转换为_T('\r\n')
					t_cs.Remove(_T('\r'));
					t_csf.WriteString(t_cs);
					t_csf.Close();
				}
				else
				{
					t_cs.Format(_T("%s open fail!"),m_filename);
					AfxMessageBox(t_cs);
				}
			}
		}
		if(m_pnodepropertydlg!=NULL)
		{
			// 告知nodepropertydlg本dlg结束
			m_pnodepropertydlg->SendMessage(WM_nodeprotertydlgscripteditdlgexit);
		}
	}	
	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}

// new cdialogex,重载该虚函数删除指针
void cscripteditdlg::PostNcDestroy()
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	delete this;
	CDialogEx::PostNcDestroy();
}


void cscripteditdlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ÔÚ´Ë´¦Ìí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂë
	// 窗口改变后重绘控件
	CWnd *t_pWnd;
	CRect t_rect;
	// 获取控件句柄  
	t_pWnd=GetDlgItem(IDC_scripteditor);   
	// 判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建  
	if(t_pWnd!=NULL)
	{  
		t_rect.left=EDGEWIDTH;
		t_rect.top=EDGEWIDTH;	
		t_rect.right=cx-EDGEWIDTH;
		t_rect.bottom=cy-EDGEWIDTH;	
		t_pWnd->MoveWindow(&t_rect);
	}	
}


void cscripteditdlg::OnCancel()
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	DestroyWindow();
	//CDialogEx::OnCancel();
}


void cscripteditdlg::OnEnChangescripteditor()
{
	// TODO:  Èç¹û¸Ã¿Ø¼þÊÇ RICHEDIT ¿Ø¼þ£¬Ëü½«²»
	// ·¢ËÍ´ËÍ¨Öª£¬³ý·ÇÖØÐ´ CDialogEx::OnInitDialog()
	// º¯Êý²¢µ÷ÓÃ CRichEditCtrl().SetEventMask()£¬
	// Í¬Ê±½« ENM_CHANGE ±êÖ¾¡°»ò¡±ÔËËãµ½ÑÚÂëÖÐ¡£

	// TODO:  ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
 	m_modifyflag=1;
}


BOOL cscripteditdlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_TAB)   
	{   
		HWND hWnd=::SetFocus(m_scriptedit.m_hWnd);
		if(hWnd!= m_scriptedit.m_hWnd)   
		{
			::SetFocus(hWnd);   
		}
		else   
		{   
			int t_istart,t_iend;  
			m_scriptedit.GetSel(t_istart,t_iend);   
			m_scriptedit.SetSel(t_istart,t_istart);   
			m_scriptedit.ReplaceSel(_T("\t"));   
			return TRUE;   
		}   
	} 
	return CDialogEx::PreTranslateMessage(pMsg);
}
