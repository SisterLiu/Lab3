#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <vector>

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
		ID3D11Texture2D* pTexture;
		ID3D11ShaderResourceView* pDx11TextureView;
		unsigned int numVertex;
		unsigned int numIndex;
		DirectX::XMFLOAT3 posInParent;
		DirectX::XMFLOAT3 posRoot;
		DirectX::XMFLOAT3 angle;
		std::vector<Mesh> child;
		D3D_PRIMITIVE_TOPOLOGY layout = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		Mesh(aiMesh*);
		~Mesh();
};


class Model
{

};


