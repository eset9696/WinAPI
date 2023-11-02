#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<cstdio>
#include<string>


#define IDC_STATIC_EXPRESSION 1000
#define IDC_STATIC_NUMS 1001
#define IDC_BUTTON_0 1002
#define IDC_BUTTON_1 1003
#define IDC_BUTTON_2 1004
#define IDC_BUTTON_3 1005
#define IDC_BUTTON_4 1006
#define IDC_BUTTON_5 1007
#define IDC_BUTTON_6 1008
#define IDC_BUTTON_7 1009
#define IDC_BUTTON_8 1010
#define IDC_BUTTON_9 1011
#define IDC_BUTTON_PLUS 1012
#define IDC_BUTTON_MINUS 1013
#define IDC_BUTTON_MULT 1014
#define IDC_BUTTON_DIV 1015
#define IDC_BUTTON_CALC 1016
#define IDC_BUTTON_CLEAR 1017

CONST CHAR g_sz_WINDOW_CLASS[] = { "Calculator" };

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMSG, WPARAM wParam, LPARAM lParam);
std::string Calculate(const CHAR* expression);
void CStringCopy(CHAR* str1, const CHAR* str2);
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.style = 0;

	wc.hIcon = ExtractIcon(hInstance, "calc.ico", 0);
	wc.hIconSm = ExtractIcon(hInstance, "calc.ico", 0);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = HBRUSH(COLOR_WINDOW + 1);

	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_sz_WINDOW_CLASS;

	if (!RegisterClassEx(&wc)) MessageBox(NULL, "Class Registration failed", "Error", MB_OK | MB_ICONERROR);

	INT windowWidth = GetSystemMetrics(SM_CXSCREEN) / 6;
	INT windowHeight = GetSystemMetrics(SM_CYSCREEN) / 3;
	INT start_x = 5 * GetSystemMetrics(SM_CXSCREEN) / 12;
	INT start_y = GetSystemMetrics(SM_CYSCREEN) / 3;

	HWND hwnd = CreateWindowEx(
		NULL, g_sz_WINDOW_CLASS, g_sz_WINDOW_CLASS, WS_OVERLAPPEDWINDOW,
		start_x, start_y, windowWidth, windowHeight,
		NULL, NULL, hInstance, NULL);

	if(hwnd == NULL) MessageBox(NULL, "Window Creation failed", "Error", MB_OK | MB_ICONERROR);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;

	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL flag = false;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		INT windowWidth = GetSystemMetrics(SM_CXSCREEN) / 6;
		INT windowHeight = GetSystemMetrics(SM_CYSCREEN) / 3;
		INT start_button_x = 10;
		INT start_button_y = windowHeight - 90;
		INT buttonWidth = 50;
		INT buttonIndent = 10;
		INT buttonHeight = 30;
		INT buttons_x[4];
		INT buttons_y[4];
		for (int i = 0; i < 4; i++) buttons_x[i] = start_button_x + i * (buttonIndent + buttonWidth);
		for (int i = 0; i < 4; i++) buttons_y[i] = start_button_y - i * (buttonIndent + buttonHeight);

		int rows = 4;
		int cols = 3;
		int digit = 0;
		char sz_digit[2] = "";

		HWND hStaticText = CreateWindowExA(NULL, "Static", "", WS_CHILD | WS_VISIBLE,
			10, 10, 260, 40, hwnd, (HMENU)IDC_STATIC_EXPRESSION, GetModuleHandle(NULL), NULL);

		HWND hStaticNums = CreateWindowExA(NULL, "Static", "", WS_CHILD | WS_VISIBLE,
			10, 60, 260, 40, hwnd, (HMENU)IDC_STATIC_NUMS, GetModuleHandle(NULL), NULL);

		HWND hButton0 = CreateWindowExA(NULL, "Button", "0", WS_CHILD | WS_VISIBLE,
			buttons_x[0], buttons_y[0], 170, buttonHeight, hwnd, (HMENU)IDC_BUTTON_0, GetModuleHandle(NULL), NULL);

		for (int i = 1; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				digit++;
				sz_digit[0] = digit + 48;
				CreateWindowExA(NULL, "Button", sz_digit, WS_CHILD | WS_VISIBLE,buttons_x[j], buttons_y[i],
					buttonWidth, buttonHeight, hwnd, (HMENU)(IDC_BUTTON_1 + digit - 1), GetModuleHandle(NULL), NULL);
			}
		}

		std::string operators[] = { "+", "-", "*", "/" };
		int n = 0;
		for (int i = 3; i >= 0; i--)
		{
			CreateWindowExA(NULL, "Button", (LPCSTR)(operators[n].c_str()), WS_CHILD | WS_VISIBLE, buttons_x[3], buttons_y[i],
				buttonWidth, buttonHeight, hwnd, (HMENU)(IDC_BUTTON_PLUS + n), GetModuleHandle(NULL), NULL);
			n++;
		}

		HWND hButtonCalc = CreateWindowExA(NULL, "Button", "=", WS_CHILD | WS_VISIBLE,
			buttons_x[0], buttons_y[3] - 40, buttonWidth, buttonHeight, hwnd, (HMENU)IDC_BUTTON_CALC, GetModuleHandle(NULL), NULL);

		HWND hButtonClear = CreateWindowExA(NULL, "Button", "CE", WS_CHILD | WS_VISIBLE,
			buttons_x[1], buttons_y[3] - 40, buttonWidth, buttonHeight, hwnd, (HMENU)IDC_BUTTON_CLEAR, GetModuleHandle(NULL), NULL);

	}
		break;
	case WM_COMMAND:
	{
		CONST INT SIZE = 256;
		static CHAR sz_bufferExpression[SIZE] = {};
		CHAR sz_bufferDisplay[SIZE] = {};
		static BOOL flag_op = false;
		HWND hStaticHistory = GetDlgItem(hwnd, IDC_STATIC_EXPRESSION);
		HWND hStaticDisplay = GetDlgItem(hwnd, IDC_STATIC_NUMS);
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_1:
		{
			if(flag_op)
			{
				SendMessage(hStaticDisplay, WM_SETTEXT, 0, (LPARAM)"");
				flag_op = false;
			}
			SendMessage(hStaticDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_bufferDisplay);
			std::string str = "1";
			strcat(sz_bufferDisplay, (CHAR*)str.c_str());
			SendMessage(hStaticDisplay, WM_SETTEXT, 0, (LPARAM)sz_bufferDisplay);
		}
			break;
		case IDC_BUTTON_2:
		{
			if (flag_op)
			{
				SendMessage(hStaticDisplay, WM_SETTEXT, 0, (LPARAM)"");
				flag_op = false;
			}
			SendMessage(hStaticDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_bufferDisplay);
			std::string str = "2";
			strcat(sz_bufferDisplay, (CHAR*)str.c_str());
			SendMessage(hStaticDisplay, WM_SETTEXT, 0, (LPARAM)sz_bufferDisplay);
		}
		break;

		case IDC_BUTTON_PLUS:
		{
			flag_op = true;
			SendMessage(hStaticDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_bufferDisplay);
			std::string str = "+";
			strcat(strcat(sz_bufferExpression, sz_bufferDisplay), (CHAR*)str.c_str());
			SendMessage(hStaticHistory, WM_SETTEXT, 0, (LPARAM)sz_bufferExpression);
		}
		break;
		case IDC_BUTTON_CALC:
		{
			flag_op = true;
			SendMessage(hStaticDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_bufferDisplay);
			std::string str = "=";
			strcat(strcat(sz_bufferExpression, sz_bufferDisplay), (CHAR*)str.c_str());
			CHAR* expression = sz_bufferExpression;
			std::string res = Calculate(expression);
			SendMessage(hStaticDisplay, WM_SETTEXT, 0, (LPARAM)res.c_str());
			SendMessage(hStaticHistory, WM_SETTEXT, 0, (LPARAM)sz_bufferExpression);

		}
		break;
		}
	}
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE: DestroyWindow(hwnd); break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	
	return NULL;
}

std::string Calculate(const CHAR* expression)
{
	std::string result = "";
	CHAR buffer[256];CStringCopy(buffer, expression);
	CHAR delimeters[256] = "+-*/";
	INT operands[2] = {}; 
	int n = 0;
	for (char* pch = strtok(buffer, delimeters); pch; pch = strtok(NULL, delimeters)) {
		operands[n++] = atoi(pch);
	}
	int res = 0;
	if (strchr(expression, '+')) 
		res = operands[0] + operands[1];
	result = std::to_string(res);
	return result;
}
void CStringCopy(CHAR* str1, const CHAR* str2)
{
	for(int i = 0; str2[i]; i++)
	{
		str1[i] = str2[i];
	}
}