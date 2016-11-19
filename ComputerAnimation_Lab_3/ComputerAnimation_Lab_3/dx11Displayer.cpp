#include "dx11Displayer.h"

using namespace DirectX;

void Dx11Displayer::render()
{

	// Initialize the view matrix
	XMVECTOR Eye = eyePos;
	XMVECTOR At = eyePos + eyeDirect;
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);
	//
	// Clear the back buffer
	//
	pDx11DeviceContext->ClearRenderTargetView(pDx11RenderTargetView, Colors::MidnightBlue);
	pDx11DeviceContext->ClearDepthStencilView(pDx11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	ConstantBuffer cb;
	cb.mWorld = XMMatrixIdentity();
	cb.mView = XMMatrixTranspose(g_View);
	cb.mProjection = XMMatrixTranspose(g_Projection);
	XMStoreFloat3(&cb.cameraPos, eyePos);
	//XMStoreFloat3(&cb.lightPos, eyePos);
	cb.lightPos = XMFLOAT3(10.0, 10.0, 10.0);
	pDx11DeviceContext->UpdateSubresource(pDx11ConstantBuffer, 0, nullptr, &cb, 0, 0);

	// Render
	UINT stride = sizeof(MeshVertex);
	UINT offset = 0;
	pDx11DeviceContext->IASetVertexBuffers(0,1,&mesh->pDx11VertexBuffer, &stride, &offset);
	pDx11DeviceContext->IASetIndexBuffer(mesh->pDx11IndexBuffer, DXGI_FORMAT_R32_UINT,0);
	pDx11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDx11DeviceContext->VSSetConstantBuffers(0, 1, &pDx11ConstantBuffer);

	pDx11DeviceContext->DrawIndexed(mesh->numIndex, 0, 0);
	
	//
	// Present our back buffer to our front buffer
	//
	pDx11SwapChain->Present(0, 0);
}

void Dx11Displayer::render60()
{
	static ULONGLONG Timer = 0;
	ULONGLONG cur = GetTickCount64();
	if(Timer == 0)
		Timer = cur;
	if(cur - Timer > 1000 / 120.0)
	{
		Timer = cur;
		render();
	}
}

Dx11Displayer::Dx11Displayer(HWND hwnd)
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	// create the device
	DXGI_SWAP_CHAIN_DESC swapChainDescribe;
	ZeroMemory(&swapChainDescribe, sizeof(swapChainDescribe));
	swapChainDescribe.BufferCount = 1;
	swapChainDescribe.BufferDesc.Width = width;
	swapChainDescribe.BufferDesc.Height = height;
	swapChainDescribe.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescribe.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDescribe.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDescribe.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescribe.OutputWindow = hwnd;
	swapChainDescribe.SampleDesc.Count = 1;
	swapChainDescribe.SampleDesc.Quality = 0;
	swapChainDescribe.Windowed = TRUE;

	D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
	UINT               numLevelsRequested = 1;
	D3D_FEATURE_LEVEL  FeatureLevelsSupported;

	if(FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&FeatureLevelsRequested,
		numLevelsRequested,
		D3D11_SDK_VERSION,
		&swapChainDescribe,
		&pDx11SwapChain,
		&pDx11Device,
		&FeatureLevelsSupported,
		&pDx11DeviceContext)))
	{
		return;
	}

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = pDx11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if(FAILED(hr))
		return;

	hr = pDx11Device->CreateRenderTargetView(pBackBuffer, nullptr, &pDx11RenderTargetView);
	pBackBuffer->Release();
	if(FAILED(hr))
		return;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = pDx11Device->CreateTexture2D(&descDepth, nullptr, &pDx11DepthStencil);
	if(FAILED(hr))
		return;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = pDx11Device->CreateDepthStencilView(pDx11DepthStencil, &descDSV, &pDx11DepthStencilView);
	if(FAILED(hr))
		return;

	pDx11DeviceContext->OMSetRenderTargets(1, &pDx11RenderTargetView, pDx11DepthStencilView);

	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDx11DeviceContext->RSSetViewports(1, &vp);


	ID3DBlob* pVSBlob = nullptr;

	// Compile the vertex shader
	hr = CompileShaderFromFile(L"shader.fx", "VS", "vs_4_0", &pVSBlob);
	if(FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return;
	}

	// Create the vertex shader
	hr = pDx11Device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pDx11VertexShader);
	if(FAILED(hr))
	{
		pVSBlob->Release();
		return;
	}

	// Set the vertex shader
	pDx11DeviceContext->VSSetShader(pDx11VertexShader, nullptr, 0);

	// Create the input layout
	
	hr = pDx11Device->CreateInputLayout(layout, 3, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &pDx11VertexLayout);
	pVSBlob->Release();
	if(FAILED(hr))
		return;

	// Set the input layout
	pDx11DeviceContext->IASetInputLayout(pDx11VertexLayout);


	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(L"shader.fx", "PS", "ps_4_0", &pPSBlob);
	if(FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return;
	}

	// Create the pixel shader
	hr = pDx11Device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pDx11PixelShader);
	pPSBlob->Release();
	if(FAILED(hr))
		return;

	// Set the pixel shader
	pDx11DeviceContext->PSSetShader(pDx11PixelShader, nullptr, 0);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pDx11Device->CreateBuffer(&bd, nullptr, &pDx11ConstantBuffer);
	if(FAILED(hr))
		return;

	// Initialize the world matrix
	g_World = XMMatrixIdentity();

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, -10.0f, -10.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	eyeDirect = At - Eye;
	eyeDirect = XMVector4Normalize(eyeDirect);
	eyePos = Eye;
	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 1000.0f);

	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile("./model/bird.3ds", aiProcess_Triangulate);
	const aiMesh* pMesh = pScene->mMeshes[0];
	mesh = new Mesh(pDx11Device, pDx11DeviceContext, pMesh);

}

Dx11Displayer::~Dx11Displayer()
{
	if(pDx11Device)				pDx11Device->Release();
	if(pDx11DeviceContext)		pDx11DeviceContext->Release();
	if(pDx11SwapChain)			pDx11SwapChain->Release();
	if(pDx11RenderTargetView)	pDx11RenderTargetView->Release();
	if(pDx11DepthStencil)		pDx11DepthStencil->Release();
	if(pDx11DepthStencilView)	pDx11DepthStencilView->Release();
	if(pDx11VertexShader)		pDx11VertexShader->Release();
	if(pDx11PixelShader)		pDx11PixelShader->Release();
	if(pDx11VertexLayout)		pDx11VertexLayout->Release();
	if(pDx11ConstantBuffer)		pDx11ConstantBuffer->Release();
}

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT Dx11Displayer::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if(FAILED(hr))
	{
		if(pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if(pErrorBlob) pErrorBlob->Release();

	return S_OK;
}
