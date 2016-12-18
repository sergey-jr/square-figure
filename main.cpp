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
		HDC hdc = GetDC(hWnd); //�������� DC(�������� ����������) ��� ���������
		HPEN hpen1; //��������� ������ ����
		HGDIOBJ hpenOld, hbrushOld;
		HBRUSH hbrush; //��������� �����
		//SetWindowText(hWnd, "TASK44");
		hpen1 = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); //���������� ���� � �������� ������, ������� � ������
		hpenOld = (HPEN)SelectObject(hdc, hpen1);
		hbrush = CreateSolidBrush(RGB(255, 255, 255));
		hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
		MoveToEx(hdc, 0, 100, NULL);
		for (auto i = head; i != NULL; i = i->next)
			Rectangle(hdc, i->x1 + 20, i->y1 + 20, i->x3 + 20, i->y3 + 20);
		hpen1 = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); //���������� ���� � �������� ������, ������� � ������
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
	int nCmdShow) // ����� ����������� ������
{

	TCHAR szClassName[] = "My class"; // ������ � ������ ������
	HWND hMainWnd; // ������ ���������� �������� ������
	MSG msg; // ����� ��������� ��������� MSG ��� ��������� ���������
	WNDCLASSEX wc; // ������ ���������, ��� ��������� � ������ ������ WNDCLASSEX
	wc.cbSize = sizeof(wc); // ������ ��������� (� ������)
	wc.style = CS_HREDRAW | CS_VREDRAW; // ����� ������ ������
	wc.lpfnWndProc = WndProc; // ��������� �� ���������������� �������
	wc.lpszMenuName = NULL; // ��������� �� ��� ���� (� ��� ��� ���)
	wc.lpszClassName = szClassName; // ��������� �� ��� ������
	wc.cbWndExtra = NULL; // ����� ������������� ������ � ����� ���������
	wc.cbClsExtra = NULL; // ����� ������������� ������ ��� �������� ���������� ����������
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE("\\res\Icon.ico")); // ��������� �����������
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // ���������� ��������� ����������� (� ����)
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // ���������� �������
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ���������� ����� ��� �������� ���� ����
	wc.hInstance = hInst; // ��������� �� ������, ���������� ��� ����, ������������ ��� ������
	if (!RegisterClassEx(&wc)) {
		// � ������ ���������� ����������� ������:
		MessageBox(NULL, "�� ���������� ���������������� �����!", "������", MB_OK);
		return NULL; // ����������, �������������, ������� �� WinMain
	}
	// �������, ��������� ������:
	hMainWnd = CreateWindow(
		szClassName, // ��� ������
		"TASK 44", // ��� ������ (�� ��� ������)
		WS_OVERLAPPEDWINDOW | WS_VSCROLL, // ������ ����������� ������
		CW_USEDEFAULT, // ������� ������ �� ��� �
		NULL, // ������� ������ �� ��� � (��� ������ � �, �� ������ �� �����)
		CW_USEDEFAULT, // ������ ������
		NULL, // ������ ������ (��� ������ � ������, �� ������ �� �����)
		(HWND)NULL, // ���������� ������������� ����
		NULL, // ���������� ����
		HINSTANCE(hInst), // ���������� ���������� ����������
		NULL); // ������ �� ������� �� WndProc
	if (!hMainWnd) {
		// � ������ ������������� �������� ������ (�������� ��������� � ��):
		MessageBox(NULL, "�� ���������� ������� ����!", "������", MB_OK);
		return NULL;
	}
	ShowWindow(hMainWnd, nCmdShow); // ���������� ������
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
	UpdateWindow(hMainWnd); // ��������� ������
	while (GetMessage(&msg, NULL, NULL, NULL)) { // ��������� ��������� �� �������, ���������� ��-�����, ��
		TranslateMessage(&msg); // �������������� ���������
		DispatchMessage(&msg); // ������� ��������� ������� ��
	}
	return NULL; // ���������� ��� ������ �� ����������
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hDC; // ������ ���������� ���������� ������ �� ������
	PAINTSTRUCT ps; // ���������, ���-��� ���������� � ���������� ������� (�������, ���� � ��)
	RECT rect; // ���-��, ������������ ������ ���������� �������
	COLORREF colorText = RGB(0, 0, 0); // ����� ���� ������
	switch (uMsg) {
	case WM_PAINT: // ���� ����� ����������, ��:
		hDC = BeginPaint(hWnd, &ps); // �������������� �������� ����������
		GetClientRect(hWnd, &rect); // �������� ������ � ������ ������� ��� ���������
		SetTextColor(hDC, colorText); // ������������� ���� ������������ ����������
		DrawText(hDC, mainMessage, -1, &rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER); // ������ �����
		ob->plot(hWnd);
		EndPaint(hWnd, &ps); // ����������� ��������
		break;
	case WM_DESTROY: // ���� ������ ���������, ��:
		PostQuitMessage(NULL); // ���������� WinMain() ��������� WM_QUIT
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam); // ���� ������� ������
	}
	return NULL; // ���������� ��������
}