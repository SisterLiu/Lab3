#include "model.h"
#include "Wincodec.h"
#include "WICTextureLoader.h"

//---------------------------------------------------------------------------
//	Class Mesh
//---------------------------------------------------------------------------

Mesh::Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const aiMesh* pAiMesh)
{
	numVertex = pAiMesh->mNumVertices;
	numIndex = pAiMesh->mNumFaces * 3;

	this->pDx11Device = pDevice;
	this->pDx11DeviceContext = pDeviceContext;

	//	Set the vertex & index buffer
	MeshVertex* pVertexs = new MeshVertex[numVertex];
	unsigned int* pIndexs = new unsigned int[numIndex];
	
	//------------------------------------------------------------------
	//	Vertex
	//------------------------------------------------------------------
	//	Set vertex position
	for(int i = 0; i < numVertex; i++)
	{
		pVertexs[i].pos.x = pAiMesh->mVertices[i].x;
		pVertexs[i].pos.y = pAiMesh->mVertices[i].y;
		pVertexs[i].pos.z = pAiMesh->mVertices[i].z;
	}

	//	Set vertex normal
	if(pAiMesh->HasNormals())
		for(int i = 0; i < numVertex; i++)
		{
			pVertexs[i].normal.x = pAiMesh->mNormals[i].x;
			pVertexs[i].normal.y = pAiMesh->mNormals[i].y;
			pVertexs[i].normal.z = pAiMesh->mNormals[i].z;
		}

	//	Set vertex texture coordinate
	if(pAiMesh->HasTextureCoords(0))
	for(int i = 0; i < numVertex; i++)
	{
		pVertexs[i].texture.x = pAiMesh->mTextureCoords[0][i].x;
		pVertexs[i].texture.y = pAiMesh->mTextureCoords[0][i].y;
	}

	//------------------------------------------------------------------
	//	Index
	//------------------------------------------------------------------
	//	Set Index
	for(int i = 0; i < pAiMesh->mNumFaces; i++)
	{
		pIndexs[i * 3 + 0] = pAiMesh->mFaces[i].mIndices[0];
		pIndexs[i * 3 + 1] = pAiMesh->mFaces[i].mIndices[1];
		pIndexs[i * 3 + 2] = pAiMesh->mFaces[i].mIndices[2];
	}

	//------------------------------------------------------------------
	//	Create Vertex Buffer
	//------------------------------------------------------------------
	//	Set vertex buffer describe
	D3D11_BUFFER_DESC vertexBufferDescribe;
	ZeroMemory(&vertexBufferDescribe,sizeof(vertexBufferDescribe));
	vertexBufferDescribe.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDescribe.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescribe.ByteWidth = sizeof(MeshVertex) * numVertex;
	vertexBufferDescribe.CPUAccessFlags = 0;
	
	//	Set vertex buffer data
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = pVertexs;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	//	Create Vertex Buffer
	pDx11Device->CreateBuffer(&vertexBufferDescribe,&vertexBufferData,&pDx11VertexBuffer);

	//------------------------------------------------------------------
	//	Create Index Buffer
	//------------------------------------------------------------------
	//	Set index buffer describe
	D3D11_BUFFER_DESC indexBufferDescribe;
	ZeroMemory(&indexBufferDescribe, sizeof(indexBufferDescribe));
	indexBufferDescribe.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDescribe.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescribe.ByteWidth = sizeof(unsigned int) * numIndex;
	indexBufferDescribe.CPUAccessFlags = 0;

	//	Set index buffer data
	D3D11_SUBRESOURCE_DATA indexBufferData;
	ZeroMemory(&indexBufferData, sizeof(indexBufferData));
	indexBufferData.pSysMem = pIndexs;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	//	Create Vertex Buffer
	pDx11Device->CreateBuffer(&indexBufferDescribe, &indexBufferData, &pDx11IndexBuffer);

	//------------------------------------------------------------------
	//	Release vertex & index buffer
	//------------------------------------------------------------------
	delete[] pVertexs;
	delete[] pIndexs;

}

Mesh::~Mesh()
{
	if(pDx11VertexBuffer) pDx11VertexBuffer->Release();
	if(pDx11IndexBuffer) pDx11IndexBuffer->Release();
	if(pTexture) pTexture->Release();
	if(pDx11TextureView) pDx11TextureView->Release();
}

void Mesh::readTextureFromFile(LPWSTR file)
{
	CreateWICTextureFromFile(pDx11Device,
		pDx11DeviceContext,
		file,
		&pTexture,
		&pDx11TextureView);
}















//---------------------------------------------------------------------------
//	Class Model
//---------------------------------------------------------------------------


