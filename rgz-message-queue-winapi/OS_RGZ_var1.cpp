#pragma once
#include <windowsx.h>
#include "CustomMessage.h"

// глобальные переменные

LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\sample_mailslot");
HANDLE hFile, hProcess;
LPCWSTR childProc = L"1.exe";
HWND hEdit, hBtn, hExit;
int MessageCount = 0;


// запись в почтовый ящик

BOOL WriteSlot(HANDLE hSlot, LPCTSTR lpszMessage)
{
	BOOL fResult;
	DWORD cbWritten;

	fResult = WriteFile(hSlot,
		lpszMessage,
		(DWORD)(lstrlen(lpszMessage) + 1) * sizeof(TCHAR),
		&cbWritten,
		(LPOVERLAPPED)NULL);

	if (!fResult)
	{
		return FALSE;
	}
	return TRUE;
}

// открытие дочернего процесса

HANDLE CreateChildProcess(LPCWSTR szCmdline)
{
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bFuncRetn = FALSE;

	// Set up members of the PROCESS_INFORMATION structure. 

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure. 

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);

	// Create the child process. 

	bFuncRetn = CreateProcess(L"1.exe",
		NULL,     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

	if (bFuncRetn == 0)
	{
		DWORD err = GetLastError();
		MessageBoxA(NULL, "CreateProcess failed", "Error", MB_OK);
		return INVALID_HANDLE_VALUE;
	}
	else
	{
		CloseHandle(piProcInfo.hThread);
		return piProcInfo.hProcess;
	}
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd; // дескриптор окна
	MSG msg; // структура сообщения
	WNDCLASS w; // структура класса окна
	// Регистрация класса окна
	hProcess = CreateChildProcess(childProc);
	w.style = CS_HREDRAW | CS_VREDRAW;
	w.lpfnWndProc = WndProc; // имя оконной функции
	w.hInstance = hInstance;
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;
	w.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	w.lpszMenuName = NULL;
	w.lpszClassName = L"My Class";
	RegisterClass(&w);
	// Создание окна
	hwnd = CreateWindow(L"My Class", L"Окно пользователя",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow); // отображение
	UpdateWindow(hwnd);          // перерисовка
	// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam)
{
	HINSTANCE hInst;
	switch (Message)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lparam)->hInstance;
		hEdit = CreateWindow(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 10, 300, 20, hwnd, NULL, hInst, NULL);
		hBtn = CreateWindow(L"BUTTON",
			L"Send Message",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			420, 10, 140, 20,
			hwnd, (HMENU)1, NULL, NULL);
		hExit = CreateWindow(L"BUTTON",
			L"Exit from program",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			420, 60, 140, 20,
			hwnd, (HMENU)1, NULL, NULL);
		hFile = CreateFile(SlotName,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}
		break;
	case WM_COMMAND:
		if (lparam == (LPARAM)hBtn)    // если нажали на кнопку
		{
			CustomMessage m;
			const LPWSTR lpszText = new wchar_t[30];
			Edit_GetText(hEdit, lpszText, 30);
			MessageCount++;
			m.setID(MessageCount);
			m.setTime();
			m.setText(lpszText);
			m.DumpToStr();
			WriteSlot(hFile, m.StrText);
			delete[] lpszText;
		}
		else if (lparam == (LPARAM)hExit) {
			TerminateProcess(hProcess, 1);
			CloseHandle(hProcess);
			CloseHandle(hFile);
			ExitProcess(1);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, Message, wparam, lparam);
}
