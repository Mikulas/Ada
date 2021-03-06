﻿#define XML_USE_STL

#include <windows.h>
#include <objidl.h>
#include <sstream>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

#include "OsmParser.h"
#include "Ada.h"
#include "Location.h"

using namespace Gdiplus;


const char g_szClassName[] = "myWindowClass";
const int ID_TIMER = 1;

Ada ada;

VOID OnPaint(HDC hdc)
{
	DrawMap("map.osm", hdc, &ada);
}

void DrawString(HDC hdc, stringstream* s, PointF p)
{
	Graphics graphics(hdc);
	Font font(L"Lucida Console", 12);
	SolidBrush brushBlack(Color::Black);

	WCHAR buffer[150];
	MultiByteToWideChar(CP_ACP, 0, s->str().c_str(), -1, buffer, 150);
	graphics.DrawString(buffer, s->str().length(), &font, p, NULL, &brushBlack);
}

VOID DrawMovement(HDC hdc, RECT* rcClient)
{
	if (ada.track.empty())
		return;

	Graphics graphics(hdc);
	Pen pen(Color(255, 20, 180, 20));
	SolidBrush brushWhite(Color::White);
	SolidBrush brush(Color::MidnightBlue);

	stringstream lat, lon, orientation, distance;
	lat << "Lat = " << ada.location.latitude << "°";
	lon << "Lon = " << ada.location.longitude << "°";
	orientation << "Ori ~ " << (ada.orientation * 180 / 3.1415) << "°";
	distance << "Dis = " << ada.getDistanceToNext() << "m";
	
	graphics.FillRectangle(&brushWhite, Rect(0, 0, 400, 50));

	DrawString(hdc, &lat, PointF(2, 2));
	DrawString(hdc, &lon, PointF(2, 20));
	DrawString(hdc, &orientation, PointF(150, 2));
	DrawString(hdc, &distance, PointF(150, 20));
	
	int radius = 8;
	graphics.FillEllipse(&brush, lon2screen(ada.track.top().longitude) - radius / 2, lat2screen(ada.track.top().latitude) - radius / 2, radius, radius);

	graphics.DrawLine(&pen, lon2screen(ada.location.longitude), lat2screen(ada.location.latitude), lon2screen(ada.location.longitude), lat2screen(ada.location.latitude) + 1);
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rcClient;
	UINT ret;

	switch(msg)
	{
		case WM_CREATE:
			ret = SetTimer(hwnd, ID_TIMER, 50, NULL);
			if(ret == 0)
				MessageBox(hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);
		break;
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			OnPaint(hdc);
			EndPaint(hwnd, &ps);
		break;
		case WM_TIMER:
			hdc = GetDC(hwnd);
			GetClientRect(hwnd, &rcClient);
			ada.step(1.0);
			DrawMovement(hdc, &rcClient);
			ReleaseDC(hwnd, hdc);
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	//Step 1: Registering the Window Class
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Scout", // title
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow | SW_MAXIMIZE);
	UpdateWindow(hwnd);

	// Step 3: The Message Loop
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
