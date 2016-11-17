#pragma once
#include <windows.h>

class UserInput
{
	public:
		int mouse(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		int keyboard(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};


