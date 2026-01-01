#define _PHYSX_DEBUG
#include <winsock2.h>
#include <windows.h>
#include <Overlay/Overlay.h>
//#include <Overlay/Overlay.cpp>
#include <Common/Data.h>
#include <Utils/Utils.h>
#include <Hack/Hack.h>
#include <Common/Offset.h>
#include <thread>
#include <DMALibrary/Memory/Memory.h>
#include <cstdint>
#include <Utils/w3c/w3c.h>
#include <Utils/MachineCodeGenerator.h>
#include "resource.h"
#include "resource1.h"
//#include"Main.h"



#include <tchar.h>  // 必须包含的声明
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS



FGameData GameData;
using namespace std;

void SetConsoleStyle()
{
	SetConsoleOutputCP(CP_UTF8);

	SetConsoleTitle(L"LD模型漏打");

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 14;

	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);

	auto hStdin = ::GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	::GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	::SetConsoleMode(hStdin, mode);
}

int DENGLU;

int Refresh()
{
	while (true)
	{
		mem.RefreshAll();
		Sleep(1000 * 60 * 15);
	}
}

BOOL WINAPI ConsoleHandler(DWORD event) {
	if (event == CTRL_CLOSE_EVENT) {
		HWND Progman = FindWindowA("Progman", NULL);
		HWND TrayWnd = FindWindowA("Shell_TrayWnd", NULL);
		ShowWindow(Progman, SW_SHOW);
		ShowWindow(TrayWnd, SW_SHOW);
		return TRUE;
	}
	return FALSE;
}

bool DeleteFile(const std::string& file_path) {
	std::wstring wfile_path = std::wstring(file_path.begin(), file_path.end());
	return DeleteFileW(wfile_path.c_str()) != 0;
}






// 设置控制台文本颜色
void SetConsoleColor(WORD color) {
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleOutput, color);
}

// 显示菜单并高亮选中项
void DisplayMenu(const std::vector<std::string>& menuItems, int selectedItem, const std::string g) {
	system("cls");  // 清除屏幕
	printf("%s\n", g.c_str());
	for (int i = 0; i < menuItems.size(); ++i) {
		if (i == selectedItem) {
			SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);  // 设置红色高亮
		}
		else {
			SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);  // 设置白色
		}
		printf("%s\n", menuItems[i].c_str());
	}
}










bool EnsureDirectoryExists(const std::string& dir) {
	if (CreateDirectoryA(dir.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
		return true;
	}
	return false;
}





int main() {



	if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE))
	{
		return -1;
	}

	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL)
	{
		ShowWindow(hwnd, SW_SHOW);//SW_SHOW显示   SW_HIDE隐藏
	}


	
	std::string HWID = MachineCodeGenerator::GenerateMachineCode();
	ShowWindow(hwnd, SW_SHOW);


	SetConsoleStyle();

	if (HWID == "") exit(0);

	Utils::Log(0, U8("LD模型漏打-----助手加载成功......"));

	
	Config::Load();

	std::thread HackThread(Hack::Init);

	Overlay::Init(0, DrawMain);
	while (true)
	{
		if (GameData.Config.Window.Main) {
			Sleep(500);
			continue;
		}
		break;
	}

	return 0;
}