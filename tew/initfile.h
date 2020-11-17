// initfile.h
//  初始化文件内容宏定义
//	FILE_APP_CFG_DAT对应app_cfg.dat
//	FILE_MODE_COMM_DAT对应mode_comm.dat
//	map索引CString定义

//	m_mapappcfg索引CString定义
#define MAPAPPCFG	{\
	_T("producttest"),\
	_T("database"),\
	_T("lasty"),\
	_T("lastx"),\
	_T("lastfile"),\
	_T("lastpath"),\
	_T("apppath")}
// app运行文件初始化内容
#define FILE_APP_CFG_DAT	(\
_T("\
apppath=;\n\
lastpath=;\n\
lastfile=;\n\
lastx=800;\n\
lasty=600;\n\
database=0;\n\
producttest=0;\n\
loop=0;\n\
ngstop=0;\n\
zlogdir=;\n")\
)

// 终端方式定义文件初始化内容
#define MAPMODECOMM	{\
	_T("comstopbits"),\
	_T("comparity"),\
	_T("combytesize"),\
	_T("combaudrate"),\
	_T("reflectkey"),\
	_T("displayback"),\
	_T("prompt"),\
	_T("pwd"),\
	_T("user"),\
	_T("displaypwd"),\
	_T("displayuser"),\
	_T("autologin"),\
	_T("initreturn"),\
	_T("comment")}
// 终端方式定义文件初始化内容
#define FILE_MODE_COMM_DAT	(\
_T("\
//[keytype;host;port]\n\
//comment=#;\n\
//initreturn=0;\n\
//autologin=0;\n\
//displayuser=login:;\n\
//displaypwd=Password:;\n\
//user=a;\n\
//pwd=a;\n\
//prompt=#;\n\
//displayback=0;\n\
//reflectkey={\"key1\";action1}{\"key2\";action2};\n\
//exitaction=exit;\n\
//delaytime=0;\n\
//endofline=\\r\\n;\n\
//headflag=00;\n\
//tailflag=00;\n\
//heartmsg=00;\n\
//heartintv=1000;\n\
//BaudRate=9600;\n\
//ByteSize=8;\n\
//Parity=0;\n\
//#define NOPARITY            0;\n\
//#define ODDPARITY           1;\n\
//#define EVENPARITY          2;\n\
//#define MARKPARITY          3;\n\
//#define SPACEPARITY         4;\n\
//StopBits=0;\n\
//#define ONESTOPBIT          0;\n\
//#define ONE5STOPBITS        1;\n\
//#define TWOSTOPBITS         2;\n\
[pn;;23;]\n\
	comment=#;\n\
	initreturn=0;\n\
	autologin=0;\n\
	displayuser=login:;\n\
	displaypwd=Password:;\n\
	user=a;\n\
	pwd=a;\n\
	prompt=#;\n\
[pn;;22;]\n\
	comment=#;\n\
	initreturn=0;\n\
	autologin=0;\n\
	displayuser=login:;\n\
	displaypwd=Password:;\n\
	user=root;\n\
	pwd=root;\n\
	prompt=#;\n\
	endofline=\\r;\n\
[pn;;3000;]\n\
	comment=//;\n\
	initreturn=0;\n\
	autologin=0;\n\
	displayuser=User:;\n\
	displaypwd=Password:;\n\
	user=admin;\n\
	pwd=cMPC_pxn;\n\
	prompt=#;\n\
	exitaction=exit\n\
[pn;com;;]\n\
	comment=#;\n\
	initreturn=1;\n\
	autologin=0;\n\
	prompt==>;\n\
	reflectkey={\"stop key:<\";};\n\
	endofline=\\r;\n\
	BaudRate=115200;\n\
	ByteSize=8;\n\
	Parity=0;\n\
	StopBits=0;\n\
[ixia;;23;]\n\
	comment=#;\n\
	prompt=Ixia>;\n\
	endofline=\\n;\n\
[1570;gpib;;]\n\
	prompt=;\n\
	comment=#;\n\
	displayback=1;\n\
	delaytime=1000;\n\
	endofline=\\n;\n\
[37718;gpib;;]\n\
	prompt=;\n\
	comment=#;\n\
	displayback=1;\n\
	delaytime=1000;\n\
	endofline=\\n;\n\
[37718;com;;]\n\
	comment=#;\n\
	initreturn=1;\n\
	autologin=0;\n\
	prompt=37718>;\n\
	displayback=0;\n\
	BaudRate=9600;\n\
	ByteSize=7;\n\
	Parity=1;\n\
	StopBits=0;\n\
[37718;;5001;]\n\
	comment=#;\n\
	initreturn=0;\n\
	autologin=0;\n\
	prompt=37718>;\n\
[directtcp;;1024;]\n\
	prompt=;\n\
	comment=#;\n\
	delaytime=2000;\n\
	endofline=\\n;\n\
	headflag=0a0a0a0a0b0b;\n\
	tailflag=0b0b0a0a0a0a;\n\
	heartmsg=e0;\n\
	heartintv=3000;\n\
[tew;;60013;]\n\
	comment=#;\n\
	initreturn=0;\n\
	autologin=0;\n\
	prompt=> ;\n")\
)