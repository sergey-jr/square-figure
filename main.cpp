#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <algorithm>
#include <windows.h>
using namespace std;
struct square
{
	double x1, y1, x2, y2, x3, y3, x4, y4;
	square *next;
	square() :next(NULL) {}
	double sq()
	{
		return (y3 - y1)*(x3 - x1);
	}
	friend ostream & operator<<(ostream & out, square ob)
	{
		out << "(" << ob.x1 << ", " << ob.y1 << ")" << endl;
		out << "(" << ob.x1 << ", " << ob.y2 << ")" << endl;
		out << "(" << ob.x2 << ", " << ob.y2 << ")" << endl;
		out << "(" << ob.x2 << ", " << ob.y1 << ")" << endl;
	}
	friend ifstream & operator >> (ifstream & in, square *ob)
	{
		in >> ob->x1 >> ob->y1 >> ob->x2 >> ob->y2 >> ob->x3 >> ob->y3 >> ob->x4 >> ob->y4;
		return in;
	}
};
class list
{
	square *head;
	bool check1(square *i, square *j)
	{
		return !(i->x1>j->x3 || i->y1>j->y3 || j->y1>i->y3 || j->x1>i->x3);
	}
	double insq(square *i, square *j)
	{
		double inx1, inx2, iny1, iny2;
		inx1 = max(i->x1, j->x1);
		inx2 = min(i->x3, j->x3);
		iny1 = max(i->y1, j->y1);
		iny2 = min(i->y3, j->y3);
		return (inx2 - inx1)*(iny2 - iny1);
	}
	double maxx()
	{
		double mx = head->x3;
		for (auto i = head; i != NULL; i = i->next)
		{
			if (mx < i->x3)
				mx = i->x3;
		}
		return mx;
	}
	double maxy()
	{
		double mx = head->y3;
		for (auto i = head; i != NULL; i = i->next)
		{
			if (mx < i->y3)
				mx = i->y3;
		}
		return mx;
	}
	double minx()
	{
		double mx = head->x1;
		for (auto i = head; i != NULL; i = i->next)
		{
			if (mx > i->x1)
				mx = i->x1;
		}
		return mx;
	}
	double miny()
	{
		double mx = head->y1;
		for (auto i = head; i != NULL; i = i->next)
		{
			if (mx > i->y1)
				mx = i->y1;
		}
		return mx;
	}
public:
	list() :head(NULL) {}
	void push(square *val)
	{
		if (head == NULL)
		{
			head = val;
		}
		else
		{
			square *tmp = head;
			while (tmp->next != NULL)
			{
				tmp = tmp->next;
			}
			tmp->next = val;
		}
	}
	void print()
	{
		square * tmp = head;
		while (tmp)
		{
			cout << tmp;
			cout << endl;
			tmp = tmp->next;
		}
	}
	size_t size()
	{
		square* tmp = head;
		size_t k = 0;
		while (tmp)
		{
			tmp = tmp->next;
			k++;
		}
		return k;
	}
	bool check()
	{
		int f = 1;
		for (auto i = head; i != NULL; i = i->next)
		{
			for (auto j = head; j != NULL; j = j->next)
			{
				if (i != j && !check1(i, j))
					f++;
			}
		}
		return ceil(f / 2.0) < size();
	}
	double squres()
	{
		double s = 0;
		square *tmp = head;
		while (tmp)
		{
			s += tmp->sq();
			tmp = tmp->next;
		}
		return s;
	}
	double insqures()
	{
		double s = 0;
		for (auto i = head; i != NULL; i = i->next)
		{
			for (auto j = head; j != NULL; j = j->next)
			{
				if (i != j && check1(i, j))
				{
					s += insq(i, j);
				}
			}
		}
		return s / 2.0;
	}
	void normalize()
	{
		double xc = minx(), cx = (maxx() - minx());
		double yc = miny(), cy = (maxy() - miny());
		for (auto i = head; i != NULL; i = i->next)
		{
			i->x1 = (i->x1 - xc) / cx;
			i->x1 *= 200;
			i->x3 = (i->x3 - xc) / cx;
			i->x3 *= 200;
			i->y1 = (i->y1 - yc) / cy;
			i->y1 *= 200;
			i->y3 = (i->y3 - yc) / cy;
			i->y3 *= 200;
		}
	}
	void plot(HWND hWnd)
	{
		HDC hdc = GetDC(hWnd); //получаем DC(контекст устройства) для рисования
		HPEN hpen1; //объявляем объект перо
		HGDIOBJ hpenOld, hbrushOld;
		HBRUSH hbrush; //объявляем кисть
		//SetWindowText(hWnd, "TASK44");
		hpen1 = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); //логическое перо с заданным стилем, шириной и цветом
		hpenOld = (HPEN)SelectObject(hdc, hpen1);
		hbrush = CreateSolidBrush(RGB(255, 255, 255));
		hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
		MoveToEx(hdc, 0, 100, NULL);
		for (auto i = head; i != NULL; i = i->next)
			Rectangle(hdc, i->x1 + 20, i->y1 + 20, i->x3 + 20, i->y3 + 20);
		hpen1 = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); //логическое перо с заданным стилем, шириной и цветом
		hpenOld = (HPEN)SelectObject(hdc, hpen1);
		for (auto i = head; i != NULL; i = i->next)
		{
			for (auto j = head; j != NULL; j = j->next)
			{
				if (i != j && check1(i, j))
				{
					double inx1 = max(i->x1, j->x1), inx2 = min(i->x3, j->x3), iny1 = max(i->y1, j->y1), iny2 = min(i->y3, j->y3);
					Rectangle(hdc, inx1 + 20, iny1 + 20, inx2 + 20, iny2 + 20);
				}
			}
		}
	}
};
char *itoc(double x, int redux)
{
	int k = 0;
	int wl = x * 100;
	int wa = x * 100;
	while (wa)
	{
		wa /= redux;
		k++;
	}
	char *res = new char[k+1];
	for (int i = 0; i < k+2; i++)
		res[i] = 0;
	int i = k;
	while (wl)
	{
		if (i != k - 2)
			{
				res[i] = (wl % redux) + '0';
				wl /= redux;
			}
		else
			res[i] = '.';
		
		if(wl)
		i--;
		
	}
	if (wl)
		res[i + 1] = wl%redux+'0';
	if (!wl) res = "0.00";
	return res;
}
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR *mainMessage = "";
list *ob;
int WINAPI WinMain(HINSTANCE hInst, 
	HINSTANCE hPrevInst,
	LPSTR lpCmdLine,
	int nCmdShow) // режим отображения окошка
{

	TCHAR szClassName[] = "My class"; // строка с именем класса
	HWND hMainWnd; // создаём дескриптор будущего окошка
	MSG msg; // создём экземпляр структуры MSG для обработки сообщений
	WNDCLASSEX wc; // создаём экземпляр, для обращения к членам класса WNDCLASSEX
	wc.cbSize = sizeof(wc); // размер структуры (в байтах)
	wc.style = CS_HREDRAW | CS_VREDRAW; // стиль класса окошка
	wc.lpfnWndProc = WndProc; // указатель на пользовательскую функцию
	wc.lpszMenuName = NULL; // указатель на имя меню (у нас его нет)
	wc.lpszClassName = szClassName; // указатель на имя класса
	wc.cbWndExtra = NULL; // число освобождаемых байтов в конце структуры
	wc.cbClsExtra = NULL; // число освобождаемых байтов при создании экземпляра приложения
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE("\\res\Icon.ico")); // декриптор пиктограммы
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // дескриптор маленькой пиктограммы (в трэе)
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // дескриптор курсора
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // дескриптор кисти для закраски фона окна
	wc.hInstance = hInst; // указатель на строку, содержащую имя меню, применяемого для класса
	if (!RegisterClassEx(&wc)) {
		// в случае отсутствия регистрации класса:
		MessageBox(NULL, "Не получилось зарегистрировать класс!", "Ошибка", MB_OK);
		return NULL; // возвращаем, следовательно, выходим из WinMain
	}
	// Функция, создающая окошко:
	hMainWnd = CreateWindow(
		szClassName, // имя класса
		"TASK 44", // имя окошка (то что сверху)
		WS_OVERLAPPEDWINDOW | WS_VSCROLL, // режимы отображения окошка
		CW_USEDEFAULT, // позиция окошка по оси х
		NULL, // позиция окошка по оси у (раз дефолт в х, то писать не нужно)
		CW_USEDEFAULT, // ширина окошка
		NULL, // высота окошка (раз дефолт в ширине, то писать не нужно)
		(HWND)NULL, // дескриптор родительского окна
		NULL, // дескриптор меню
		HINSTANCE(hInst), // дескриптор экземпляра приложения
		NULL); // ничего не передаём из WndProc
	if (!hMainWnd) {
		// в случае некорректного создания окошка (неверные параметры и тп):
		MessageBox(NULL, "Не получилось создать окно!", "Ошибка", MB_OK);
		return NULL;
	}
	ShowWindow(hMainWnd, nCmdShow); // отображаем окошко
	ifstream in("input.txt");
	int n;
	ob = new list;

	if (in.is_open())
	{
		in >> n;
		square * obj;
		for (int i = 0; i<n; i++)
		{
			obj = new square;
			in >> obj;
			ob->push(obj);
		}
		in.close();
		//ob->print();
		if (!ob->check())
		{
			char *left = itoc(0., 10);
			mainMessage=left;
		}
		else
		{
			int n = ob->squres() - ob->insqures();
			char *left= itoc(ob->squres() - ob->insqures(),10);
			mainMessage = left;
		}
		ob->normalize();
	}
	UpdateWindow(hMainWnd); // обновляем окошко
	while (GetMessage(&msg, NULL, NULL, NULL)) { // извлекаем сообщения из очереди, посылаемые фу-циями, ОС
		TranslateMessage(&msg); // интерпретируем сообщения
		DispatchMessage(&msg); // передаём сообщения обратно ОС
	}
	return NULL; // возвращаем код выхода из приложения
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hDC; // создаём дескриптор ориентации текста на экране
	PAINTSTRUCT ps; // структура, сод-щая информацию о клиентской области (размеры, цвет и тп)
	RECT rect; // стр-ра, определяющая размер клиентской области
	COLORREF colorText = RGB(0, 0, 0); // задаём цвет текста
	switch (uMsg) {
	case WM_PAINT: // если нужно нарисовать, то:
		hDC = BeginPaint(hWnd, &ps); // инициализируем контекст устройства
		GetClientRect(hWnd, &rect); // получаем ширину и высоту области для рисования
		SetTextColor(hDC, colorText); // устанавливаем цвет контекстного устройства
		DrawText(hDC, mainMessage, -1, &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER); // рисуем текст
		ob->plot(hWnd);
		EndPaint(hWnd, &ps); // заканчиваем рисовать
		break;
	case WM_DESTROY: // если окошко закрылось, то:
		PostQuitMessage(NULL); // отправляем WinMain() сообщение WM_QUIT
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam); // если закрыли окошко
	}
	return NULL; // возвращаем значение
}