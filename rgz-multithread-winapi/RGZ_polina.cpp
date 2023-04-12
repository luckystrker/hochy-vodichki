#include <windows.h>
#include <vector>

#define TIME_COEF 40 // коэффициенты для отрисовки времени и скорости на графике
#define SPEED_COEF 30

using namespace std;

// прототипы функций

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//Процедура рисования линии
BOOL Line(HDC hdc, int x1, int y1, int x2, int y2);
// поточная функция, моделирующая работу двигателя
DWORD WINAPI EngineThread(LPVOID lpParam);
// поточная функция, моделирующая работу ограничителя
DWORD WINAPI StopperThread(LPVOID lpParam);
// функция, отрисовывающая график
void PaintAll();

struct point { // структура точки, содержащая две координаты - по X и по Y
	int x = 0, y = -100; 
};
vector<point> DrawPoints(10); // вектор, в котором хранятся точки для отрисовки (всего точек 10)

const int acceleration = 30;

int xView, yView, // координаты для отрисовки
	speed = 0, SpeedMax = 80, // сокрость и ограничитель максимальной скорости
	PointNum = 0; // количество отрисованных точек 
HANDLE hEngine, hStopper, hMtx,			// дескрипторы для: потоков моделирования двигателя и ограничителя, мьютекса,
	   hPauseEvent, hResumeEvent, hExitEvent; // события для остановки, запуска и выхода из потоков, и для смены ограничения
HWND hPauseBtn, hResumeBtn, hExitBtn, hStop1, hStop2, hStop3, hStop4; // дескрипторы кнопок
HINSTANCE hInst; // экземпляр программы
HDC hdc; //создаём контекст устройства
PAINTSTRUCT ps; //создаём экземпляр структуры графического вывода
HPEN hPen; //создаём перо
HWND hWnd; // главное окно нашей программы

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	MSG lpMsg;
	WNDCLASS w;
	// регистрация параметров окна
	w.lpszClassName = L"RGZ";
	w.hInstance = hInstance;
	w.lpfnWndProc = WndProc;
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hIcon = 0;
	w.lpszMenuName = 0;
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	w.style = CS_HREDRAW | CS_VREDRAW;
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;

	//Если не удалось зарегистрировать класс окна - выходим
	if (!RegisterClass(&w))
		return 0;

	//Создадим окно в памяти, заполнив аргументы CreateWindow
	hWnd = CreateWindow(L"RGZ",
		L"Полина Шипицына РГЗ",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		1024,
		512,
		(HWND)NULL,
		(HMENU)NULL,
		(HINSTANCE)hInstance,
		(HINSTANCE)NULL);
	// создаем мьютекс
	hMtx = CreateMutex(NULL, false, L"mutex");
	// задаем координаты по оси X для точек 
	for (int i = 0; i < 10; i++) DrawPoints[i].x = i*TIME_COEF;
	//Выводим окно из памяти на экран
	ShowWindow(hWnd, nCmdShow);
	//Обновим содержимое окна
	UpdateWindow(hWnd);

	//Цикл обработки сообщений

	while (GetMessage(&lpMsg, NULL, 0, 0)) { //Получаем сообщение из очереди
		TranslateMessage(&lpMsg); //Преобразует сообщения клавиш в символы
		DispatchMessage(&lpMsg); //Передаёт сообщение соответствующей функции окна
	}
	return(lpMsg.wParam);
}

//Функция окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	//Цикл обработки сообщений
	switch (message)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;

		hPauseEvent = CreateEvent(NULL, TRUE, FALSE, L"pause"); // создаем событие паузы
		hPauseBtn = CreateWindow(L"button", L"Pause", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_OVERLAPPED, 800, 20, 80, 30, hWnd, NULL, hInst, NULL); // создаем кнопку паузы

		hResumeEvent = CreateEvent(NULL, TRUE, FALSE, L"resume"); // создаем событие запуска
		hResumeBtn = CreateWindow(L"button", L"Resume", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_OVERLAPPED, 900, 20, 80, 30, hWnd, NULL, hInst, NULL); // создаем кнопку запуска

		hExitEvent = CreateEvent(NULL, TRUE, FALSE, L"resume");// создаем событие выхода
		hExitBtn = CreateWindow(L"button", L"Exit", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_OVERLAPPED, 800, 70, 180, 30, hWnd, NULL, hInst, NULL);// создаем кнопку выхода
		// создаем кнопки для изменение предела скорости
		hStop1 = CreateWindow(L"button", L"800 RPM", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_OVERLAPPED, 800, 120, 80, 30, hWnd, NULL, hInst, NULL);
		hStop2 = CreateWindow(L"button", L"1000 RPM", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_OVERLAPPED, 900, 120, 80, 30, hWnd, NULL, hInst, NULL);
		hStop3 = CreateWindow(L"button", L"1500 RPM", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_OVERLAPPED, 800, 170, 80, 30, hWnd, NULL, hInst, NULL);
		hStop4 = CreateWindow(L"button", L"2000 RPM", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_OVERLAPPED, 900, 170, 80, 30, hWnd, NULL, hInst, NULL);
		// запускаем потоки 
		hEngine = CreateThread(NULL, 0, &EngineThread, NULL, 0, NULL);
		hStopper = CreateThread(NULL, 0, &StopperThread, NULL, 0, NULL);
		break;
		// получение координат для рисования
	case WM_SIZE: 
		xView = LOWORD(lParam);
		yView = HIWORD(lParam);

		break;


		//сообщение рисования
	case WM_PAINT:
		PaintAll();
		break;
	case WM_COMMAND:
		if (lParam == (LPARAM)hPauseBtn) {
			SetEvent(hPauseEvent); // при нажатии на кнопку - включаем событие паузы
		}
		else if (lParam == (LPARAM)hResumeBtn) {
			SetEvent(hResumeEvent);// при нажатии на кнопку - включаем событие запуска
		}
		else if (lParam == (LPARAM)hExitBtn) {
			SetEvent(hExitEvent);//при нажатии на кнопку -включаем событие выхода и закрываем все открытые дескрипторы
			CloseHandle(hPauseEvent);
			CloseHandle(hResumeEvent);
			CloseHandle(hExitEvent);
			CloseHandle(hEngine);
			CloseHandle(hStopper);
			CloseHandle(hMtx);
			ExitProcess(1);
		}
		else if (lParam == (LPARAM)hStop1) {
			SpeedMax = 80;//при нажатии на кнопку - меняем предел скорости
		}
		else if (lParam == (LPARAM)hStop2) {
			SpeedMax = 100;
		}
		else if (lParam == (LPARAM)hStop3) {
			SpeedMax = 150;
		}
		else if (lParam == (LPARAM)hStop4) {
			SpeedMax = 200;
		}
		break;
		//сообщение выхода - разрушение окна
	case WM_DESTROY:
		DeleteObject(hPen); //не забываем уничтожать перья
		CloseHandle(hPauseEvent);//так же закрываем все открытые дескрипторы
		CloseHandle(hResumeEvent);
		CloseHandle(hExitEvent);
		CloseHandle(hEngine);
		CloseHandle(hStopper);
		CloseHandle(hMtx);
		PostQuitMessage(0); //Посылаем сообщение выхода с кодом 0 - нормальное завершение
		break;
	default:
		return(DefWindowProc(hWnd, message, wParam, lParam)); //освобождаем очередь приложения от нераспознаных
	}
	return 0;
}

//Функция рисования линии
BOOL Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
	return LineTo(hdc, x2, y2); //нарисовать линию
}


DWORD WINAPI EngineThread(LPVOID lpParam) {
	HANDLE m1 = OpenMutex(SYNCHRONIZE, false, L"mutex"); // открываем заранее созданный мьютекс
	while (true) { // заходим в бесконечный цикл
		if (WaitForSingleObject(hExitEvent, 0) == WAIT_OBJECT_0) { // если было открыто событие выхода 
			CloseHandle(m1); // - закрываем дескриптор мьютекса и выходим из потока
			return 0;
		}
		WaitForSingleObject(m1, INFINITE); // ждем, пока в поток поступит мьютекс 
		speed += acceleration; // увеличиваем скорость
		ReleaseMutex(m1); // отпускаем мьютекс дальше 
		if (WaitForSingleObject(hPauseEvent, 0) == WAIT_OBJECT_0) { // если было открыто событие паузы 
			MessageBox(hWnd, L"Paused", L"Message", MB_OK); //уведомляем пользователя о нажатии паузы
			WaitForSingleObject(hResumeEvent, INFINITE); // ждем, пока не откроется событие запуска
			MessageBox(hWnd, L"Resumed", L"da", MB_OK); // уведомляем пользователя о запуске
			ResetEvent(hPauseEvent); // выключаем события паузы и пуска
			ResetEvent(hResumeEvent);
		}
		Sleep(1000); // ждем одну секунду
	}
}

DWORD WINAPI StopperThread(LPVOID lpParam) {
	HANDLE m2 = OpenMutex(SYNCHRONIZE, false, L"mutex");// открываем заранее созданный мьютекс
	int SpeedBuff; // создаем переменную-буфер для скорости 
	while (true) {// заходим в бесконечный цикл
		if (WaitForSingleObject(hExitEvent, 0) == WAIT_OBJECT_0) { // если было открыто событие выхода 
			CloseHandle(m2); // - закрываем дескриптор мьютекса и выходим из потока
			return 0;
		}
		WaitForSingleObject(m2, INFINITE); // ждем, пока в поток поступит мьютекс 
		if (speed > SpeedMax) speed = SpeedMax; // если скорость больше той, что была задана - уменьшаем ее до максимального значения
		SpeedBuff = speed; // записываем скорость в буферную переменную
		ReleaseMutex(m2); // отпускаем мьютекс дальше
		if (PointNum < 10) { // проверка для отрисовки - если было нарисовано до десяти точек, то переписываем значение Y (скорости) в точке 
			DrawPoints[PointNum].y = SpeedBuff; 
			PointNum++; 
		}
		else { 
			for (int i = 0; i < 9; i++) { // иначе - делаем сдвиг влево на 1
				DrawPoints[i].y = DrawPoints[i + 1].y;
			}
			DrawPoints[9].y = SpeedBuff; // присваиваем последней точке значение скорости 
		}
		PaintAll();// отрисовываем график заново
		if (WaitForSingleObject(hPauseEvent, 0) == WAIT_OBJECT_0) { // если было открыто событие паузы 
			WaitForSingleObject(hResumeEvent, INFINITE); // ждем, пока не откроется событие запуска
			ResetEvent(hPauseEvent); // выключаем события паузы и пуска
			ResetEvent(hResumeEvent);
		}
		Sleep(1000); // ожидаем 1 секунду
	}
}

void PaintAll() { 
	InvalidateRect(hWnd, NULL, true); // очищаем область для рисования 
	hdc = BeginPaint(hWnd, &ps); 
	SetMapMode(hdc, MM_ISOTROPIC); //логические единицы отображаем, как физические
	SetWindowExtEx(hdc, 500, 500, NULL); //Длина осей
	SetViewportExtEx(hdc, xView, -yView, NULL); //Определяем область вывода
	SetViewportOrgEx(hdc, xView / 6, yView / 2, NULL); //Начало координат

	//Рисуем оси координат
	Line(hdc, 0, 220, 0, -220);//ось У
	Line(hdc, -100, -100, 500, -100);//ось Х

	MoveToEx(hdc, 0, -100, NULL); //перемещаемся в начало координат
	//Создание красного пера
	hPen = CreatePen(1, 2, RGB(255, 25, 0));
	SelectObject(hdc, hPen);

	// рисуем наш график
	for (auto it : DrawPoints) {
		LineTo(hdc, it.x, it.y);
	}

	//Делаем перо снова чёрным
	hPen = CreatePen(1, 1, RGB(0, 0, 0));
	SelectObject(hdc, hPen);

	for (int i = 0; i < 10; i++)
	{
		Line(hdc, i * TIME_COEF, -97, i * TIME_COEF, -103);
		Line(hdc, -3, i * SPEED_COEF - 100, 3, i * SPEED_COEF - 100);
	}

	ValidateRect(hWnd, NULL); //Обновляем экран
	EndPaint(hWnd, &ps);
}