#include <Windows.h>
#include <string>

#define  CAPTURE_TIMER 0x0001
const wchar_t className[] = L"capture_desktop";
const UINT time = 1000;


void CaptureDesktop(HWND hwnd)
{
	//抓取桌面
	HWND hDesktopWnd = GetDesktopWindow();
	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYSCREEN);
	HDC hDesktopDC = GetDC(hDesktopWnd);

	//拷贝图标指定窗口的设备上下文
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	int left = clientRect.left;
	int top = clientRect.top;
	int w = clientRect.right - left;
	int h = clientRect.bottom - top;
	HDC hdc = GetDC(hwnd);
	HDC hMemDc = CreateCompatibleDC(hdc);
	HBITMAP hMemBitmap = CreateCompatibleBitmap(hdc, w, h);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDc, hMemBitmap);
	SetStretchBltMode(hMemDc, HALFTONE);
	StretchBlt(hMemDc, left, top, w, h, hDesktopDC, 0, 0, cxScreen, cyScreen, SRCCOPY);
	BitBlt(hdc, 0, 0, w, h, hMemDc, 0, 0, SRCCOPY);
	//释放资源
	SelectObject(hMemDc, hOldBitmap);
	DeleteObject(hMemBitmap);
	DeleteObject(hMemDc);
	ReleaseDC(hwnd, hdc);
	ReleaseDC(hDesktopWnd, hDesktopDC);
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = NULL;
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_CREATE:
	{
		SetTimer(hwnd, CAPTURE_TIMER, time, NULL);
	}
	break;
	case WM_TIMER:
	{
		InvalidateRect(hwnd, NULL, TRUE);
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		//todo
		CaptureDesktop(hwnd);
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		KillTimer(hwnd, CAPTURE_TIMER);
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

bool RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex = { 0 };

	wcex.cbSize = sizeof(wcex);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = className;
	wcex.lpfnWndProc = WndProc;
	wcex.hbrBackground = NULL;// CreateSolidBrush(RGB(255, 255, 255));
	wcex.hCursor = LoadCursorW(NULL, IDC_HAND);
	wcex.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	
	return RegisterClassExW(&wcex);
}

bool CreateWnd(HINSTANCE hInstance, int left, int top, int w, int h, int nShowCmd)
{
	HWND hwnd = CreateWindowExW(0, className, L"Capture", WS_OVERLAPPEDWINDOW & (~WS_THICKFRAME), left, top, w, h, NULL, NULL, hInstance, NULL);

	if (NULL == hwnd)
	{
		return false;
	}

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	return true;
}

int MessageLoop()
{
	MSG msg;

	while (GetMessageW(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	if (!RegisterWndClass(hInstance))
	{
		return -1;
	}
	RECT workReaRec;
	SystemParametersInfoW(SPI_GETWORKAREA, 0, (LPVOID)&workReaRec, 0);
	int defaultW = 500;
	int defaultH = 500;
	int left = (workReaRec.right - workReaRec.left - defaultW) / 2;
	int top = (workReaRec.bottom - workReaRec.top - defaultH) / 2;
	if (!CreateWnd(hInstance, left, top, defaultW, defaultH, nShowCmd))
	{
		return -1;
	}

	return MessageLoop();
}