#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <vector>
#include <windows.h>

struct MeshVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texture;
	DirectX::XMFLOAT3 normal;
};

class Mesh
{
	public:
		ID3D11Buffer* pDx11VertexBuffer;
		ID3D11Buffer* pDx11IndexBuffer;
		ID3D11Resource* pTexture;
		ID3D11ShaderResourceView* pDx11TextureView;
		unsigned int numVertex;
		unsigned int numIndex;
		DirectX::XMFLOAT3 posInParent;
		DirectX::XMFLOAT3 posRoot;
		DirectX::XMFLOAT3 angle;
		std::vector<Mesh> child;
		D3D_PRIMITIVE_TOPOLOGY layout = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		Mesh(ID3D11Device*, ID3D11DeviceContext*, const aiMesh*);
		~Mesh();
		void readTextureFromFile(LPWSTR file);

	private:
		ID3D11Device* pDx11Device;
		ID3D11DeviceContext* pDx11DeviceContext;
};


class Model
{

};

class Object
{
	public:
		Mesh mesh;
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 angle;
		float speed_x;
		float speed_y;
		float speed_z;
		float rotation_x;
		float rotation_y;
		float rotation_z;
	private:
		ID3D11Device* pDx11Device;
		ID3D11DeviceContext* pDx11DeviceContext;
};
