#include <Windows.h>
#include <iostream>



int MessageLoop(HWND hwnd)
{
	MSG msg;

	while (GetMessageW(&msg, hwnd, 0, 0)) //HWND hwnd:Ҫ��������Ϣ�Ĵ��ھ�����ô��ڱ������ڵ�ǰ�̡߳�
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void main()
{
	HWND hwnd = FindWindowW(L"WeChatLoginWndForPC", L"��¼");

	if (NULL == hwnd)
	{
		return;
	}

	MessageLoop(hwnd);
}