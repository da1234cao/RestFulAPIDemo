#include "pch.h"
#include "control.h"

BEGIN_MESSAGE_MAP(control, CWnd)
	ON_MESSAGE(ID_NOTHING_MGS, nothing)
END_MESSAGE_MAP()

control::control()
{
	// ����һ������ʾ����������Ϣ�Ĵ���
	// ���ڲ���ʾ�ο���https://codeantenna.com/a/s6NmJb6YD8
	// ��Ϣ���ղο���https://www.cnblogs.com/greatverve/archive/2012/11/04/mfc-message.html
	this->CreateEx(WS_EX_NOACTIVATE, AfxRegisterWndClass(0), _T("windowName"),
		WS_CAPTION | WS_OVERLAPPEDWINDOW | WS_EX_NOPARENTNOTIFY,//  | WS_VISIBLE,
		0, 0, 400, 350, NULL, NULL, NULL);
}

LRESULT control::nothing(WPARAM wp, LPARAM lp)
{
	MessageBox(TEXT("control receive message"));
	return 0;
}
