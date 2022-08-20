#include "pch.h"
#include "control.h"

BEGIN_MESSAGE_MAP(control, CWnd)
	ON_MESSAGE(ID_NOTHING_MGS, nothing)
END_MESSAGE_MAP()

control::control()
{
	// 创建一个不显示，但接收消息的窗口
	// 窗口不显示参考：https://codeantenna.com/a/s6NmJb6YD8
	// 消息接收参考：https://www.cnblogs.com/greatverve/archive/2012/11/04/mfc-message.html
	this->CreateEx(WS_EX_NOACTIVATE, AfxRegisterWndClass(0), _T("windowName"),
		WS_CAPTION | WS_OVERLAPPEDWINDOW | WS_EX_NOPARENTNOTIFY,//  | WS_VISIBLE,
		0, 0, 400, 350, NULL, NULL, NULL);
}

LRESULT control::nothing(WPARAM wp, LPARAM lp)
{
	MessageBox(TEXT("control receive message"));
	return 0;
}
