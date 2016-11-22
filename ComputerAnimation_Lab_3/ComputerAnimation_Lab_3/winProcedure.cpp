#include <windows.h>
#include "WinProcedure.h"
#include "Assimp/Importer.hpp"
#include "Assimp/Scene.h"
#include "Assimp/Postprocess.h"
#include <ctime>

// private static varible
HWND WinProcedure::hwnd = NULL;
UserInput* WinProcedure::pUserInput = NULL;
Dx11Displayer* WinProcedure::pDisplayer = NULL;
Controller* WinProcedure::pController = NULL;
int WinProcedure::screenX = 0;
int WinProcedure::screenY = 0;
std::vector<Object*> WinProcedure::objects;


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
	LPWSTR text;
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

void WinProcedure::setController(Controller* pCtrl)
{
	this->pController = pCtrl;
}

HWND WinProcedure::getHWND()
{
	return hwnd;
};

void WinProcedure::initial()
{
	Object* pNewObject = NULL;

	Assimp::Importer importer;
	const aiScene* pAiScene = importer.ReadFile("./model/ground.nff", aiProcess_Triangulate | \
		aiProcess_GenSmoothNormals | \
		aiProcess_GenUVCoords | \
		aiProcess_SplitLargeMeshes
	);
	const aiMesh* pAiMesh = pAiScene->mMeshes[0];

	Mesh* pMesh = new Mesh(pDisplayer->getDevice(), pDisplayer->getContext(), pAiMesh);
	pMesh->readTextureFromFile(L"./model/throwPillow.jpg");

	Assimp::Importer importer2;
	const aiScene* pAiScene2 = importer2.ReadFile("./model/ball.nff", aiProcess_Triangulate | \
		aiProcess_GenSmoothNormals | \
		aiProcess_GenUVCoords | \
		aiProcess_SplitLargeMeshes
	);
	const aiMesh* pAiMesh2 = pAiScene2->mMeshes[0];

	Mesh* pMesh2 = new Mesh(pDisplayer->getDevice(), pDisplayer->getContext(), pAiMesh2);
	pMesh2->readTextureFromFile(L"./model/throwPillow.jpg");


	pNewObject = createObjectFromMesh(pMesh,
		0, -5, 0,
		0, 0, 0);
	pNewObject->Flag = Object::GROUND;
	objects.push_back(pNewObject);
	srand(clock());

	for(int i = 0; i < 30; i++)
	{
		pNewObject = createObjectFromMesh(pMesh2,
			9-(rand() % 17 + 1), (rand() % 17 + 1), 9-(rand() % 17 + 1),
			0, 0, 0);
		pNewObject->Flag = Object::BALL;
		pNewObject->motion.gravity.y = -0.005;
		pNewObject->motion.speed.x = 0.2 - 0.4 / (rand() % 6 + 1);
		pNewObject->motion.speed.y = 0.2 - 0.4 / (rand() % 6 + 1);
		pNewObject->motion.speed.z = 0.2 - 0.4 / (rand() % 6 + 1);
		pNewObject->id = i+1;

		objects.push_back(pNewObject);
	}
}

Object* WinProcedure::createObjectFromMesh(Mesh* pMesh,
	float x, float y, float z, 
	float angleX, float angleY, float angleZ)
{

	Object* pNewObject = NULL;
	
	pNewObject = new Object();
	pNewObject->pMesh = pMesh;
	pNewObject->pos.x = x;
	pNewObject->pos.y = y;
	pNewObject->pos.z = z;
	pNewObject->angle.x = angleX;
	pNewObject->angle.y = angleY;
	pNewObject->angle.z = angleZ;

	return pNewObject;
}

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
			render60();
		}
	}
	return (int)msg.wParam;
}

void WinProcedure::render60()
{
	static ULONGLONG Timer = 0;
	ULONGLONG cur = GetTickCount64();
	if(Timer == 0)
		Timer = cur;
	if(cur - Timer > 1000 / 120.0)
	{
		Timer = cur;
		pController->next(&objects);
		pDisplayer->render(&objects);
	}
}