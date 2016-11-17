#pragma once
#include <windows.h>
#include "userInput.h"
#include "dx11Displayer.h"

class WinProcedure
{
	public:
		int startLoop();
		void setUserInput(UserInput*);
		void setDisplayer(Dx11Displayer*);
		WinProcedure(HINSTANCE hInstance, int nCmdShow, int screenX, int screenY);
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		HWND getHWND();
	private:
		static HWND hwnd;
		static UserInput* pUserInput;
		static Dx11Displayer* pDisplayer;
		static int screenX;
		static int screenY;
};




