#include <windows.h>

#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

class GdiPlusGuard
{
public:
	GdiPlusGuard()
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;

		Gdiplus::GdiplusStartup(&_gdiplusToken, &gdiplusStartupInput, NULL);
	}
	~GdiPlusGuard()
	{
		Gdiplus::GdiplusShutdown(_gdiplusToken);
	}
private:
	ULONG_PTR _gdiplusToken;
};


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_WINDOWPOSCHANGED:
	{
		WINDOWPOS* wndPos = (WINDOWPOS*)lParam;
		int left = wndPos->x;
		int top = wndPos->y;
		int right = left + wndPos->cx;
		int bottom = top + wndPos->cy;
		RECT workArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&workArea, 0);

		if (left <= workArea.left)
		{
			left = workArea.left;
		}
		else if (right >= workArea.right)
		{
			left = workArea.right - wndPos->cx;
		}
		if (top <= workArea.top)
		{
			top = workArea.top;
		}
		else if (bottom >= workArea.bottom)
		{
			top = workArea.bottom - wndPos->cy;
		}

		MoveWindow(hwnd, left, top, wndPos->cx, wndPos->cy, FALSE);
	}
	break;
	case WM_LBUTTONDOWN:
		SendMessageW(hwnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		break;
	case WM_LBUTTONDBLCLK:
		SendMessageW(hwnd, WM_CLOSE, 0, 0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

wchar_t className[] = L"layeredWindow";

bool InitLayeredWnd(HINSTANCE hInstance, HBITMAP hBitmap, int w, int h, int nCmdShow)
{
	WNDCLASSEXW wcex = { 0 };
	wcex.cbSize = sizeof(wcex);

	wcex.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;

	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreatePatternBrush(hBitmap);//Î»Í¼»­Ë¢
	wcex.lpszClassName = className;

	if (!RegisterClassExW(&wcex))
	{
		DWORD dwError = GetLastError();
		return false;
	}

	HWND hwnd = CreateWindowExW(WS_EX_LAYERED, className, L"", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, w, h, NULL, NULL, hInstance, NULL);

	if (NULL == hwnd)
	{
		return false;
	}

	

	SetLayeredWindowAttributes(hwnd, RGB(255, 255, 255), 0, LWA_COLORKEY);

	UpdateWindow(hwnd);

	ShowWindow(hwnd, nCmdShow);

	return true;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (NULL == hBitmap)
	{
		DWORD dwError = GetLastError();
		return -1;
	}

	BITMAP bm;
	GetObject(hBitmap, sizeof(bm), &bm);

	int w = bm.bmWidth;
	int h = bm.bmHeight;

	if (!InitLayeredWnd(hInstance, hBitmap, w, h, nCmdShow))
	{
		return -1;
	}

	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return msg.wParam;
}