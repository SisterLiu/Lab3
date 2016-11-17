#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

typedef class Dx11Displayer
{
	public:
		void render();// render a frame
		void render60();// render in 60fps

		Dx11Displayer(HWND);
		~Dx11Displayer();

	private:
		
		
}DX11DISPLAYER, *PDX11DISPLAYER;

