#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include<Windows.h>
#include "resource.h"

CONST CHAR* g_sz_VALUES[] = { "This", "is", "my", "first", "List","box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, LPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, LPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcDelete(HWND hwnd, UINT uMsg, LPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, LPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = ExtractIconA(GetModuleHandle(NULL), "List.ico", 0);
		SendMessage(hwnd, WM_SETICON, 0, (LRESULT)hIcon);
		HWND hList = GetDlgItem(hwnd, IDC_LIST1);
		for(int i = 0; i < sizeof(g_sz_VALUES) / sizeof(g_sz_VALUES[0]); i++)
		{
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)g_sz_VALUES[i]);
		}
	}
		break;
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_ADD1:
		{
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hwnd, (DLGPROC)DlgProcAdd, 0);
		}
			break;
		case IDC_BUTTON_DELETE1:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			int i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG3), hwnd, (DLGPROC)DlgProcDelete, 0);
		}
		break;
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			int i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);
			CHAR sz_message[SIZE] = {};
			sprintf(sz_message, "Вы выбрали элемент %i со значением \"%s\".", i, sz_buffer);
			MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
	}
		break;
	case WM_CLOSE: EndDialog(hwnd, 0); break;
	}
	return FALSE;
}

BOOL CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, LPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = ExtractIconA(GetModuleHandle(NULL), "Add.ico", 0);
		SendMessage(hwnd, WM_SETICON, 0, (LRESULT)hIcon);
		HWND hStaticAdd = GetDlgItem(hwnd, IDC_STATIC_ADD);
		SendMessage(hStaticAdd, WM_SETTEXT, 0, (LPARAM)"Введите добавляемое значение:");
		SetFocus(GetDlgItem(hwnd, IDC_EDIT_ADD));
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[256] = {};
			HWND hParent = GetParent(hwnd);
			HWND hParentEdit = GetDlgItem(hParent, IDC_LIST1);
			HWND hEditAdd = GetDlgItem(hwnd, IDC_EDIT_ADD);
			SendMessage(hEditAdd, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			if(SendMessage(hParentEdit, LB_FINDSTRING, -1, (LPARAM)sz_buffer) == CB_ERR)
			{
				if (strlen(sz_buffer) == 0) break;
				SendMessage(hParentEdit, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
				EndDialog(hwnd, 0);
			}
			else
			{
				MessageBox(hwnd, "Такое значение уже есть", "Info", MB_OK | MB_ICONINFORMATION);
			}
		}
		break;
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
	}
	break;
	case WM_CLOSE: EndDialog(hwnd, 0); break; 
	}
	return FALSE;
}

BOOL CALLBACK DlgProcDelete(HWND hwnd, UINT uMsg, LPARAM wParam, LPARAM lParam)
{
	HWND hStatic = GetDlgItem(hwnd, IDC_STATIC_DELETE);
	CONST INT SIZE = 256;
	CHAR sz_buffer[SIZE] = {};
	HWND hParent = GetParent(hwnd);
	HWND hParentList = GetDlgItem(hParent, IDC_LIST1);
	int i = SendMessage(hParentList, LB_GETCURSEL, 0, 0);
	SendMessage(hParentList, LB_GETTEXT, i, (LPARAM)sz_buffer);
	CHAR sz_message[SIZE] = {};
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = ExtractIconA(GetModuleHandle(NULL), "Remove.ico", 0);
		SendMessage(hwnd, WM_SETICON, 0, (LRESULT)hIcon);
		sprintf(sz_message, "Вы действительно хотите удалить элемент \"%s\" ?", sz_buffer);
		SendMessage(hStatic, WM_SETTEXT, 0, (LPARAM)sz_message);
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

		case IDC_BUTTON_YES1:
		{
			SendMessage(hParentList, LB_DELETESTRING, i, 0);
			sprintf(sz_message, "Элемент \"%s\" удален из списка.", sz_buffer);
			MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
			EndDialog(hwnd, 0);
		}
		break;
		case IDC_BUTTON_NO1: EndDialog(hwnd, 0); break;
		}
	}
	break;
	case WM_CLOSE: EndDialog(hwnd, 0); break;
	}
	return FALSE;
}