#pragma once

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

HINSTANCE hInst;
static HWND MessageLog;


HANDLE hSlot;
LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\sample_mailslot");

BOOL ReadSlot(HWND hwnd)
{
    DWORD cbMessage, cMessage, cbRead;
    BOOL fResult;
    LPTSTR lpszBuffer;
    TCHAR achID[80];
    DWORD cAllMessages;
    HANDLE hEvent;
    OVERLAPPED ov;

    cbMessage = cMessage = cbRead = 0;

    hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("Slot"));
    if (NULL == hEvent)
        return FALSE;
    ov.Offset = 0;
    ov.OffsetHigh = 0;
    ov.hEvent = hEvent;

    fResult = GetMailslotInfo(hSlot, // mailslot handle 
        (LPDWORD)NULL,               // no maximum message size 
        &cbMessage,                   // size of next message 
        &cMessage,                    // number of messages 
        (LPDWORD)NULL);              // no read time-out 

    if (!fResult)
    {
        return FALSE;
    }

    if (cbMessage == MAILSLOT_NO_MESSAGE)
    {
        return TRUE;
    }

    cAllMessages = cMessage;

    while (cMessage != 0)  // получаем все непрочитанные сообщения
    {
        // выделяем память под сообщение

        lpszBuffer = (LPTSTR)GlobalAlloc(GPTR, cbMessage);
        if (NULL == lpszBuffer)
            return FALSE;
        lpszBuffer[0] = '\0';

        fResult = ReadFile(hSlot,
            lpszBuffer,
            cbMessage,
            &cbRead,
            &ov);

        if (!fResult)
        {
            MessageBox(hwnd, L"ReadFile failed", L"", MB_OK);
            GlobalFree((HGLOBAL)lpszBuffer);
            return FALSE;
        }

        StringCbCat(lpszBuffer,
            cbMessage + 2,
            L"\n");

        // вывод сообщения в лог

        SendMessage(MessageLog, EM_SETSEL, -1, -1);
        SendMessage(MessageLog, EM_REPLACESEL, FALSE, (LPARAM)lpszBuffer);

        GlobalFree((HGLOBAL)lpszBuffer);

        fResult = GetMailslotInfo(hSlot,  // хэндл почтового ящика
            (LPDWORD)NULL,               // нет максимального размера сообщения
            &cbMessage,                   // размер следующего сообщения
            &cMessage,                    // количество сообщений 
            (LPDWORD)NULL);              // no read time-out 

        if (!fResult)
        {
            MessageBox(hwnd, L"GetMailslotInfo failed", L"", MB_OK);
            return FALSE;
        }
    }
    CloseHandle(hEvent);
    return TRUE;
}

BOOL WINAPI MakeSlot(LPCTSTR lpszSlotName)
{
    hSlot = CreateMailslot(lpszSlotName,
        0,                             // no maximum message size 
        MAILSLOT_WAIT_FOREVER,         // no time-out for operations 
        (LPSECURITY_ATTRIBUTES)NULL); // default security

    if (hSlot == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    return TRUE;
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd; // дескриптор окна
	MSG msg; // структура сообщения
	WNDCLASS w; // структура класса окна
	// Регистрация класса окна

	w.style = CS_HREDRAW | CS_VREDRAW;
	w.lpfnWndProc = WndProc; // имя оконной функции
	w.hInstance = hInstance;
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;
	w.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	w.lpszMenuName = NULL;
	w.lpszClassName = L"Child Class";
	RegisterClass(&w);
	// Создание окна
    MakeSlot(SlotName);
	hwnd = CreateWindow(L"Child Class", L"Дочернее Окно",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024, NULL, NULL, hInstance, NULL);
    SetTimer(hwnd, 3000, 3000, NULL);
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


	switch (Message)
	{
	case WM_CREATE:
	
		hInst = ((LPCREATESTRUCT)lparam)->hInstance;
		MessageLog = CreateWindowA("edit", "",
                     WS_DISABLED | WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL | ES_READONLY,
                     150, 20, 650, 850, hwnd, NULL, NULL, NULL);
        break;
    case WM_TIMER:
        if(wparam == 3000){
            ReadSlot(hwnd);
        }
        break;
	case WM_COMMAND:
	{
		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}

	return DefWindowProc(hwnd, Message, wparam, lparam);
}
