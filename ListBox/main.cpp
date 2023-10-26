#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include<Windows.h>
#include "resource.h"

CONST CHAR* g_sz_VALUES[] = { "This", "is", "my", "first", "List","box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, LPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc2(HWND hwnd, UINT uMsg, LPARAM wParam, LPARAM lParam);

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

			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), NULL, (DLGPROC)DlgProc2, 0);
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
			sprintf(sz_message, "Вы выбрали элемент %i со значением \"%s\"", i, sz_buffer);
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

BOOL CALLBACK DlgProc2(HWND hwnd, UINT uMsg, LPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		
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
			HWND hParent = GetDlgItem(hwnd, IDC_LIST1);
			HWND hEditAdd = GetDlgItem(hwnd, IDC_EDIT_ADD);
			SendMessage(hEditAdd, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hParent, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
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