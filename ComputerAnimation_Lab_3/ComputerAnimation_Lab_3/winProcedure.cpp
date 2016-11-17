#include <windows.h>
#include "WinProcedure.h"

// private static varible
HWND WinProcedure::hwnd = NULL;
UserInput* WinProcedure::pUserInput = NULL;
Dx11Displayer* WinProcedure::pDisplayer = NULL;
int WinProcedure::screenX = 0;
int WinProcedure::screenY = 0;



//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------

WinProcedure::WinProcedure(HINSTANCE hInstance, int nCmdShow, int screenX, int screenY)
{
	this->screenX = screenX;
	this->screenY = screenY;

	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(nullptr, (LPCTSTR)IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"CA_LAB_3";
	wcex.hIconSm = LoadIcon(nullptr, (LPCTSTR)IDI_APPLICATION);
	if(!RegisterClassEx(&wcex))
		return;

	// Create window
	RECT rc = {0, 0, screenX, screenY};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	hwnd = CreateWindow(L"CA_LAB_3", L"Computer Animation Lab 3",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if(hwnd)
		ShowWindow(hwnd, nCmdShow);
}


//---------------------------------------------------------------------------------- -
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WinProcedure::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch(message)
	{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_MOUSEMOVE:
			pUserInput->mouse(hWnd, message, wParam, lParam);
			break;
		case WM_RBUTTONDOWN:
			pUserInput->mouse(hWnd, message, wParam, lParam);
			break;

		case WM_RBUTTONUP:
			pUserInput->mouse(hWnd, message, wParam, lParam);
			break;

		case WM_KEYUP:
			pUserInput->keyboard(hWnd, message, wParam, lParam);
			break;

		case WM_KEYDOWN:
			pUserInput->keyboard(hWnd, message, wParam, lParam);
			break;
			// Note that this tutorial does not handle resizing (WM_SIZE) requests,
			// so we created the window without the resize border.

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


void WinProcedure::setUserInput(UserInput* pUI)
{
	this->pUserInput = pUI;
}

void WinProcedure::setDisplayer(Dx11Displayer* pDP)
{
	this->pDisplayer = pDP;
}

HWND WinProcedure::getHWND()
{
	return hwnd;
};

int WinProcedure::startLoop()
{
	// Main message loop
	MSG msg = {0};
	while(WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			pDisplayer->render60();
		}
	}
	return (int)msg.wParam;
}
