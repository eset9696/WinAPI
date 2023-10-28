#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include <cstdio>
//#include"resource.h"

CONST CHAR g_sz_WINDOW_CLASS[] = "My window"; // Имя класса окна
void GetSizeAndPosition(HWND hwnd);
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
	// 2) Создание окна
	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_WINDOW_CLASS, // Class name
		g_sz_WINDOW_CLASS, // Window name
		WS_OVERLAPPEDWINDOW, // У главгого окна весгда будет такой стиль
		CW_USEDEFAULT, CW_USEDEFAULT, // Позиция окна на экране
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
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_MOVE:
		GetSizeAndPosition(hwnd);
	break;
	case WM_SIZE:
		GetSizeAndPosition(hwnd);
		break;
	case WM_COMMAND:
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE: DestroyWindow(hwnd); break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return NULL;
}

void GetSizeAndPosition(HWND hwnd)
{
	RECT wp;
	GetWindowRect(hwnd, &wp);
	int Width = wp.right - wp.left;
	int Height = wp.bottom - wp.top;
	CHAR sz_message[256] = {};
	sprintf(sz_message, "%s X = %i, Y = %i, SIZE: %i, %i.", g_sz_WINDOW_CLASS, wp.left, wp.top, Width, Height);
	SetWindowText(hwnd, sz_message);
}