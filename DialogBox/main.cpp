#include <windows.h>
#include <windowsx.h>
#include "resource.h"

CONST CHAR g_sz_LOGIN_INVITATION[] = "Введите имя пользователя";
CONST CHAR g_sz_PASSWORD_INVITATION[] = "Введите пароль";


BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN); // Функция GetDlgItem() по ID ресурса дочернего окна возвращает HWND соответствующего дочернего окна
	HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LRESULT)hIcon);
		SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_LOGIN_INVITATION);
		SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)g_sz_PASSWORD_INVITATION);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDC_BUTTON_COPY:
		{
			// 1) Создаем буфер чтения
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			// 2) Получаем обработчик текстового поля "LOGIN:"
			HWND hLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN); // Функция GetDlgItem() по ID ресурса дочернего окна возвращает HWND соответствующего дочернего окна
			HWND hPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
			HWND hStaticPassword = GetDlgItem(hwnd, IDC_STATIC_PASSWORD);
			// 3) Читаем тест из тестового поля "Login:" 
			SendMessage(hLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			// 4) Загружаем содержимое текстового буфера в поле "Password:":
			SendMessage(hPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			SendMessage(hStaticPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			Edit_SetText(hLogin, "");
		}
		break;
		case IDC_EDIT_LOGIN:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			if (HIWORD(wParam) == EN_SETFOCUS && strcmp(sz_buffer, g_sz_LOGIN_INVITATION) == 0)
				SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)"");
			if (HIWORD(wParam) == EN_KILLFOCUS && strcmp(sz_buffer, "") == 0)
				SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_LOGIN_INVITATION);
		}
		break;
		case IDC_EDIT_PASSWORD:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND pEditLogin = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
			SendMessage(hEditPassword, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			if (HIWORD(wParam) == EN_SETFOCUS && strcmp(sz_buffer, g_sz_PASSWORD_INVITATION) == 0)
				SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)"");
			if (HIWORD(wParam) == EN_KILLFOCUS && strcmp(sz_buffer, "") == 0)
				SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)g_sz_PASSWORD_INVITATION);
		}
		break;
		case IDOK: MessageBox(hwnd, "Была нажата кнопа ОК", "Info", MB_OK | MB_ICONINFORMATION);
			break;
		case IDCANCEL: EndDialog(hwnd, 0);
		}
		break;
	case WM_CLOSE: EndDialog(hwnd, 0);
	}
	return FALSE;
}