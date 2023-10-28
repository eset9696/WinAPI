#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include <cstdio>
//#include"resource.h"

CONST CHAR g_sz_WINDOW_CLASS[] = "My window"; // ��� ������ ����
void GetSizeAndPosition(HWND hwnd);
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	// 1) ����������� ������ ����
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	// cb - count bytes (���������� ����)
	wc.cbWndExtra = 0; // �������������� ����� ���� 
	wc.cbClsExtra = 0; // �������������� ����� ������ ����
	wc.style = 0; // ����� ����

	wc.hIcon = (HICON)LoadImage(hInstance, "game.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(hInstance, "global.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hCursor = LoadCursorFromFile("MyCursor.ani");
	wc.hbrBackground = HBRUSH(COLOR_WINDOW + 1);

	wc.hInstance = hInstance; // ��������� ������������ ����� ��������� � ������
	// ������� WinMain() ��������� hInstance  ��� �������� � ������� � ���� ���� ������ ������,
	// � ����� ������ ������� 'hInstance' ������ ����� �������� ��� ������ �������  GetModuleHandle(NULL)
	wc.lpfnWndProc = WndProc; // ��������� �� ��������� ����
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_sz_WINDOW_CLASS;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "ClassRegistration failed", "Error", MB_OK | MB_ICONERROR);
	}
	
	INT windowWidth = 3 * GetSystemMetrics(SM_CXSCREEN) / 4;
	INT windowHeight = 3 *GetSystemMetrics(SM_CYSCREEN) / 4;
	// 2) �������� ����
	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_WINDOW_CLASS, // Class name
		g_sz_WINDOW_CLASS, // Window name
		WS_OVERLAPPEDWINDOW, // � �������� ���� ������ ����� ����� �����
		CW_USEDEFAULT, CW_USEDEFAULT, // ������� ���� �� ������
		windowWidth, windowHeight, // ������ ����
		NULL, // Parent window
		NULL, // hMenu - ��� �������� ���� ���� �������� �������� ID ������� ���� 
		      // ��� ��������� ����, ������� �������� ��������� ������� ����, � hMenu ���������� ID ������� ����� ��������
		hInstance,
		NULL
		);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation failed", "Error", MB_OK | MB_ICONERROR);
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	// 3) ������ ����� ����
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