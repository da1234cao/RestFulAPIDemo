#pragma once
#include <afxwin.h>

#define ID_NOTHING_MGS WM_USER + 1001

class control : public CWnd
{
public:
	control();
	LRESULT control::nothing(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

