#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "model.h"

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT3 cameraPos;
	XMFLOAT3 lightPos;
};


typedef class Dx11Displayer
{
	public:
		void render();// render a frame
		void render60();// render in 60fps

		Dx11Displayer(HWND);
		~Dx11Displayer();

	private:
		ID3D11Device*			pDx11Device;
		ID3D11DeviceContext*	pDx11DeviceContext;
		IDXGISwapChain*         pDx11SwapChain;
		ID3D11RenderTargetView* pDx11RenderTargetView;
		ID3D11Texture2D*        pDx11DepthStencil;
		ID3D11DepthStencilView* pDx11DepthStencilView;
		ID3D11VertexShader*     pDx11VertexShader;
		ID3D11PixelShader*      pDx11PixelShader;
		ID3D11InputLayout*      pDx11VertexLayout;
		ID3D11Buffer*           pDx11ConstantBuffer;

		HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

		D3D11_INPUT_ELEMENT_DESC layout[3] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		XMMATRIX                g_World;
		XMMATRIX                g_View;
		XMMATRIX                g_Projection;
		XMVECTOR				eyePos;
		XMVECTOR				eyeDirect;

		Mesh* mesh;
}DX11DISPLAYER, *PDX11DISPLAYER;

