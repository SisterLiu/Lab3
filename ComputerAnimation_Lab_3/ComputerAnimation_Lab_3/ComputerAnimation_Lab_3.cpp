#include <windows.h>
#include "dx11Displayer.h"
#include "WinProcedure.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WinProcedure winPro(hInstance, nCmdShow, 1280, 800);
	Dx11Displayer caDisplayer(winPro.getHWND());
	UserInput userInput;

	winPro.setDisplayer(&caDisplayer);
	winPro.setUserInput(&userInput);

	return winPro.startLoop();
}



