#include "userInput.h"

int UserInput::mouse(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

int UserInput::keyboard(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void UserInput::setDisplayer(Dx11Displayer* pDP)
{
	this->pDisplayer = pDP;
}