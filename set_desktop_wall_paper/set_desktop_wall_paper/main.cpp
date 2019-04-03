#include <Windows.h>
#include <thread>
#include <iostream>


void ChangeDesktopWallPaperTask()
{
	static wchar_t* path[10] =
	{
		L"C:\\Users\\smart\\Desktop\\workspace\\window_message\\set_desktop_wall_paper\\set_desktop_wall_paper\\test1.bmp",
		L"C:\\Users\\smart\\Desktop\\workspace\\window_message\\set_desktop_wall_paper\\set_desktop_wall_paper\\test2.bmp",
		L"C:\\Users\\smart\\Desktop\\workspace\\window_message\\set_desktop_wall_paper\\set_desktop_wall_paper\\test3.bmp",
		L"C:\\Users\\smart\\Desktop\\workspace\\window_message\\set_desktop_wall_paper\\set_desktop_wall_paper\\test4.bmp",
		L"C:\\Users\\smart\\Desktop\\workspace\\window_message\\set_desktop_wall_paper\\set_desktop_wall_paper\\test5.bmp",
		L"C:\\Users\\smart\\Desktop\\workspace\\window_message\\set_desktop_wall_paper\\set_desktop_wall_paper\\test6.bmp",
		L"C:\\Users\\smart\\Desktop\\workspace\\window_message\\set_desktop_wall_paper\\set_desktop_wall_paper\\test7.bmp",
		L"C:\\Users\\smart\\Desktop\\workspace\\window_message\\set_desktop_wall_paper\\set_desktop_wall_paper\\test8.bmp",
		L"C:\\Users\\smart\\Desktop\\workspace\\window_message\\set_desktop_wall_paper\\set_desktop_wall_paper\\test9.bmp",
		L"C:\\Users\\smart\\Desktop\\workspace\\window_message\\set_desktop_wall_paper\\set_desktop_wall_paper\\test10.bmp"
	};

	int index = 0;
	while (true)
	{
		SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (LPVOID)path[index], SPIF_UPDATEINIFILE);

		index = index++ % 10;

		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}

int main()
{
	std::thread task(ChangeDesktopWallPaperTask);

	task.join();

	return 0;
}