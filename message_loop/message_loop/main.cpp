#include <Windows.h>
#include <iostream>



int MessageLoop(HWND hwnd)
{
	MSG msg;

	while (GetMessageW(&msg, hwnd, 0, 0)) //HWND hwnd:要检索其消息的窗口句柄。该窗口必须属于当前线程。
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void main()
{
	HWND hwnd = FindWindowW(L"WeChatLoginWndForPC", L"登录");

	if (NULL == hwnd)
	{
		return;
	}

	MessageLoop(hwnd);
}