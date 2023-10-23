#include <windows.h>
#include <windowsx.h>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN); // Функция GetDlgItem() по ID ресурса дочернего окна возвращает HWND соответствующего дочернего окна
	HWND hPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LRESULT)hIcon);
		SetWindowText(hLogin, "Введите имя пользователя");
		SetWindowText(hPassword, "Введите пароль");
		
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
			//HWND hLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN); // Функция GetDlgItem() по ID ресурса дочернего окна возвращает HWND соответствующего дочернего окна
			//HWND hPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
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
			CHAR l_buffer[256] = {};
			CHAR l_default[256] = { "Введите имя пользователя" };
			SendMessage(hLogin, WM_GETTEXT, 256, (LPARAM)l_buffer);
			switch (HIWORD(wParam))
			{
			case EN_SETFOCUS: 
				if (*l_buffer == *l_default)
					SetWindowText(hLogin, "");
				break;
			case EN_KILLFOCUS: 
				if(l_buffer[0] == '\0') 
				{
					SetWindowText(hLogin, l_default);
				}
				break;
			}
			break;
		}
		case IDC_EDIT_PASSWORD:
		{
			CHAR p_buffer[256] = {};
			CHAR p_default[256] = { "Введите пароль" };
			SendMessage(hPassword, WM_GETTEXT, 256, (LPARAM)p_buffer);
			switch (HIWORD(wParam))
			{
			case EN_SETFOCUS:
				if (*p_buffer == *p_default)
					SetWindowText(hPassword, "");
				break;
			case EN_KILLFOCUS:
			{
				if (p_buffer[0] == '\0')
					SetWindowText(hPassword, p_default);

			}
				break;
			}
			break;
		}
		case IDOK: MessageBox(hwnd, "Была нажата кнопа ОК", "Info", MB_OK | MB_ICONINFORMATION);
			break;
		case IDCANCEL: EndDialog(hwnd, 0);
		}
		break;
	case WM_CLOSE: EndDialog(hwnd, 0);
	}
	return FALSE;
}