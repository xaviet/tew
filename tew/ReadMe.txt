================================================================================
MICROSOFT 基础类库: tew 项目概述
===============================================================================

应用程序向导已为您创建了这个 tew 应用程序.此应用程序不仅演示 Microsoft 基础类的基本使用方法,还可作为您编写应用程序的起点.

本文件概要介绍组成 tew 应用程序的每个文件的内容.

tew.vcxproj
这是使用应用程序向导生成的 VC++ 项目的主项目文件. 
它包含生成该文件的 Visual C++ 的版本信息,以及有关使用应用程序向导选择的平台、配置和项目功能的信息.

tew.vcxproj.filters
    这是使用“应用程序向导”生成的 VC++ 项目筛选器文件.
    它包含有关项目文件与筛选器之间的关联信息.在 IDE 中,通过这种关联,在特定节点下以分组形式显示具有相似扩展名的文件.例如,“.cpp”文件与“源文件”筛选器关联.

tew.h
这是应用程序的主要头文件.它包括其他项目特定的头文件(包括 Resource.h),并声明 CtewApp 应用程序类.

tew.cpp
这是包含应用程序类 CtewApp 的主要应用程序源文件.

tew.rc
这是程序使用的所有 Microsoft Windows 资源的列表.它包括 RES 子目录中存储的图标、位图和光标.此文件可以直接在 Microsoft Visual C++ 中进行编辑.项目资源位于 2052 中.

res\tew.ico
这是用作应用程序图标的图标文件.此图标包括在主要资源文件 tew.rc 中.

res\tew.rc2
此文件包含不在 Microsoft Visual C++ 中进行编辑的资源.您应该将不可由资源编辑器编辑的所有资源放在此文件中.


/////////////////////////////////////////////////////////////////////////////

应用程序向导创建一个对话框类:

tewDlg.h,tewDlg.cpp - 对话框
这些文件包含 CtewDlg 类.该类定义应用程序主对话框的行为.该对话框的模板位于 tew.rc 中,该文件可以在 Microsoft Visual C++ 中进行编辑.


/////////////////////////////////////////////////////////////////////////////

其他标准文件:

StdAfx.h,StdAfx.cpp
这些文件用于生成名为 tew.pch 的预编译头 (PCH) 文件和名为 StdAfx.obj 的预编译类型文件.

Resource.h
这是标准头文件,它定义新的资源 ID.
Microsoft Visual C++ 读取并更新此文件.

/////////////////////////////////////////////////////////////////////////////

其他注释:

应用程序向导使用“TODO:”指示应添加或自定义的源代码部分.

如果应用程序在共享的 DLL 中使用 MFC,则需要重新发布这些 MFC DLL；如果应用程序所用的语言与操作系统的当前区域设置不同,则还需要重新发布对应的本地化资源 MFC100XXX.DLL.有关这两个主题的更多信息,请参见 MSDN 文档中有关 Redistributing Visual C++ applications (重新发布 Visual C++ 应用程序)的章节.

/////////////////////////////////////////////////////////////////////////////

1.
终端实例停止方式:
TerminateThread(this->m_hThread,1)
上面机制无法制止内存泄漏

2.
m_hsemaphore=CreateSemaphore(NULL,1,1,NULL)
WaitForSingleObject
ReleaseSemaphore
使用信号量进行线程间控制
//使用信号量进行PV操作
WaitForSingleObject(m_hsemaphore,INFINITE);
//使用信号量进行PV操作
ReleaseSemaphore(m_hsemaphore,1,NULL);

3.
#include <locale.h> 
//设置中文环境
setlocale(LC_ALL,"chs"); 
解决unicode在cstdiofile读写

4.
通用命名方式:	[主体(控件)+]动作(行为)+目标(目的)
成员变量:		m_
指针:			p
句柄:			h
消息函数:		On
局部变量:		t_
参数:			v_
宏定义:			全部大写
宏函数:			同通用命名方式

5.
非模态对话框需要重载函数OnCanel，
并且在这个函数中调用DestroyWindow。
并且不能调用基类的OnCancel，因为
基类的OnCancel调用了EndDialog这个
函数，这个函数是针对模态对话框的。
还有一个必须重载的函数就是PostNcDestroy
，这也是一个虚函数，通常的非模态对
话框是用类的指针，通过new创建的，
这就需要在PostNcDestroy函数中delete
掉这个指针。

6.
m_edittrmdlg:	cstring+_T("\r\n")

7.
在用ReadFile和WriteFile读写串行口时，需要考虑超时问题。超时的作用是在指定的时间内没有读入或发送指定数量的字符，ReadFile或WriteFile的操作仍然会结束。
要查询当前的超时设置应调用GetCommTimeouts函数，该函数会填充一个COMMTIMEOUTS结构。调用SetCommTimeouts可以用某一个COMMTIMEOUTS结构的内容来设置超时。
读写串口的超时有两种：间隔超时和总超时。间隔超时是指在接收时两个字符之间的最大时延。总超时是指读写操作总共花费的最大时间。写操作只支持总超时，而读操作两种超时均支持。用COMMTIMEOUTS结构可以规定读写操作的超时。
COMMTIMEOUTS结构的定义为：
typedef struct _COMMTIMEOUTS {  
DWORD ReadIntervalTimeout; //读间隔超时
DWORD ReadTotalTimeoutMultiplier; //读时间系数
DWORD ReadTotalTimeoutConstant; //读时间常量
DWORD WriteTotalTimeoutMultiplier; // 写时间系数
DWORD WriteTotalTimeoutConstant; //写时间常量
} COMMTIMEOUTS,*LPCOMMTIMEOUTS;
COMMTIMEOUTS结构的成员都以毫秒为单位。总超时的计算公式是：
总超时＝时间系数×要求读/写的字符数＋时间常量
例如，要读入10个字符，那么读操作的总超时的计算公式为：
读总超时＝ReadTotalTimeoutMultiplier×10＋ReadTotalTimeoutConstant
可以看出：间隔超时和总超时的设置是不相关的，这可以方便通信程序灵活地设置各种超时。
如果所有写超时参数均为0，那么就不使用写超时。如果ReadIntervalTimeout为0，那么就不使用读间隔超时。如果ReadTotalTimeoutMultiplier 和 ReadTotalTimeoutConstant 都为0，则不使用读总超时。如果读间隔超时被设置成MAXDWORD并且读时间系数和读时间常量都为0，那么在读一次输入缓冲区的内容后读操作就立即返回，而不管是否读入了要求的字符。
在用重叠方式读写串口时，虽然ReadFile和WriteFile在完成操作以前就可能返回，但超时仍然是起作用的。在这种情况下，超时规定的是操作的完成时间，而不是ReadFile和WriteFile的返回时间

8.
利用查找统计行数
ctrl+shift+f
^:b*[^:b#/]+.*$

9.
gpib-32.obj:
multi device模式异常,需要开启ni的app才能正常运行
修改为traditional模式。

10.
//sound
#include <mmsystem.h>
//添加“winmm”库
#pragma comment(lib,"winmm")
//异步播放
sndPlaySound(_T("Error.wav"),SND_ASYNC|SND_NODEFAULT);
