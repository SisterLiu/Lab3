#include "model.h"

//---------------------------------------------------------------------------
//	Class Mesh
//---------------------------------------------------------------------------

Mesh::Mesh(aiMesh* pAiMesh)
{
	numVertex = pAiMesh->mNumVertices;
	numIndex = pAiMesh->mNumFaces * 3;

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
	if(pAiMesh->HasNormals)
		for(int i = 0; i < numVertex; i++)
		{
			pVertexs[i].normal.x = pAiMesh->mNormals[i].x;
			pVertexs[i].normal.y = pAiMesh->mNormals[i].y;
			pVertexs[i].normal.z = pAiMesh->mNormals[i].z;
		}

	//	Set vertex texture coordinate
	if(pAiMesh->HasTextureCoords)
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
	//	Create Vertex
	//------------------------------------------------------------------




}

Mesh::~Mesh()
{

}

















//---------------------------------------------------------------------------
//	Class Model
//---------------------------------------------------------------------------


