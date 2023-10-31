#define _CRT_SECURE_NO_WARNINGS
#define IDC_COMBO 1001
#define IDC_BUTTON_APPLY 1002

#include<Windows.h>
#include <cstdio>
#include <vector>
#include <string>



CONST CHAR* g_CURSOR[] = { "Busy.ani", "Normal Select.ani", "Working In Background.ani", "Move.ani" };

CONST CHAR g_sz_WINDOW_CLASS[] = "My window"; // Имя класса окна

std::vector<std::string> LoadCursorsFromDir(const std::string& dir);

void GetWindowParams(HWND hwnd);
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	// 1) Регистрация класса окна
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	// cb - count bytes (Количество байт)
	wc.cbWndExtra = 0; // Дополнительные байты окна 
	wc.cbClsExtra = 0; // Дополнительные байты класса окна
	wc.style = 0; // Стиль окна

	wc.hIcon = (HICON)LoadImage(hInstance, "game.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(hInstance, "global.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hCursor = LoadCursorFromFile("MyCursor.ani");
	wc.hbrBackground = HBRUSH(COLOR_WINDOW + 1);

	wc.hInstance = hInstance; // Экземпляр исполняемого файла программы в памяти
	// Функция WinMain() принимает hInstance  как параметр и поэтому к нему есть прямой доступ,
	// В любой другой функции 'hInstance' всегда можно получить при помощи функции  GetModuleHandle(NULL)
	wc.lpfnWndProc = WndProc; // Указатель на процедуру окна
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_sz_WINDOW_CLASS;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "ClassRegistration failed", "Error", MB_OK | MB_ICONERROR);
	}
	
	INT windowWidth = 3 * GetSystemMetrics(SM_CXSCREEN) / 4;
	INT windowHeight = 3 *GetSystemMetrics(SM_CYSCREEN) / 4;
	INT start_x =GetSystemMetrics(SM_CXSCREEN) / 8;
	INT start_y =GetSystemMetrics(SM_CYSCREEN) / 8;
	// 2) Создание окна
	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_WINDOW_CLASS, // Class name
		g_sz_WINDOW_CLASS, // Window name
		WS_OVERLAPPEDWINDOW, // У главгого окна весгда будет такой стиль
		start_x, start_y, // Позиция окна на экране
		windowWidth, windowHeight, // размер окна
		NULL, // Parent window
		NULL, // hMenu - для главного окна этот параметр содержит ID ресурса меню 
		      // Для дочернего окна, которое является элементом другого окна, в hMenu передается ID ресурса этого элемента
		hInstance,
		NULL
		);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation failed", "Error", MB_OK | MB_ICONERROR);
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	// 3) Запуск цикла окна
	MSG msg;
	while(GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
	static HCURSOR hCursor;
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hCombo = CreateWindowEx
		(
			NULL, "ComboBox", "", WS_CHILD | WS_VISIBLE | CBN_DROPDOWN | WS_VSCROLL, 
			10, 10, 200, 200, hwnd, (HMENU)IDC_COMBO, GetModuleHandle(NULL), NULL
		);
		/*for(int i = 0; i < sizeof(g_CURSOR) / sizeof(g_CURSOR[0]); i++)
		{
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)g_CURSOR[i]);
		}*/
		std::vector<std::string> cursors = LoadCursorsFromDir("starcraft-original\\*");
		for(int i = 0; i < cursors.size(); i++)
		{
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)cursors[i].c_str());
		}

		HWND hButton = CreateWindowEx(
			NULL, "Button", "Apply", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			220, 10, 100, 24, hwnd, (HMENU)IDC_BUTTON_APPLY, GetModuleHandle(NULL), NULL);

	}
		break;
	case WM_MOVE:
		GetWindowParams(hwnd);
	break;
	case WM_SIZE:
		GetWindowParams(hwnd);
		break;
	case WM_MOUSEMOVE:
		GetWindowParams(hwnd);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_APPLY:
		{
			HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
			int i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			CHAR sz_filename[FILENAME_MAX] = {};
			CHAR sz_filepath[_MAX_PATH] = "starcraft-original\\";
			SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)sz_filename);
			strcat(sz_filepath, sz_filename);
			hCursor = (HCURSOR)LoadImage(
				GetModuleHandle(NULL), sz_filepath, 
				IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE
			);
			SetClassLong(hwnd, GCL_HCURSOR, (LONG)hCursor);
			SetClassLong(GetDlgItem(hwnd, IDC_BUTTON_APPLY), GCL_HCURSOR, (LONG)hCursor);
			SetClassLong(GetDlgItem(hwnd, IDC_COMBO), GCL_HCURSOR, (LONG)hCursor);
		}
			break;
		}
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE: DestroyWindow(hwnd); break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return NULL;
}

void GetWindowParams(HWND hwnd)
{
	RECT wp;
	POINT p;
	GetCursorPos(&p);
	GetWindowRect(hwnd, &wp);
	int Width = wp.right - wp.left;
	int Height = wp.bottom - wp.top;
	CHAR sz_message[256] = {};
	sprintf(sz_message, "%s [X = %i, Y = %i, Width = %i, Height = %i. Cursor_X = %i, Cursor_Y = %i]",
		g_sz_WINDOW_CLASS, wp.left, wp.top, Width, Height, p.x, p.y);
	SetWindowText(hwnd, sz_message);
}

std::vector<std::string> LoadCursorsFromDir(const std::string& dir)
{
	std::vector<std::string> files;
	WIN32_FIND_DATA data;
	for(
		HANDLE hFind = FindFirstFile(dir.c_str(), &data);
		FindNextFile(hFind, &data);
		)
	{
		if(strcmp(strchr(data.cFileName, '.'), ".cur") == 0 ||
			strcmp(strchr(data.cFileName, '.'), ".ani") == 0)
			files.push_back(data.cFileName);
	}
	return files;

}

