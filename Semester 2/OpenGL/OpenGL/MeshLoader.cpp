#include "MeshLoader.h"

#include <iostream>
#include <fstream>

using namespace std;

namespace MeshLoader
{
	void LoadVertices(ifstream& inFile, Mesh& mesh);
	void LoadColours(ifstream& inFile, Mesh& mesh);
	void LoadIndices(ifstream& inFile, Mesh& mesh);

	void LoadTexVertices(ifstream& inFile, TexturedMesh& mesh);
	void LoadTexColours(ifstream& inFile, TexturedMesh& mesh);
	void LoadTexIndices(ifstream& inFile, TexturedMesh& mesh);
	void LoadTexCoords(ifstream& inFile, TexturedMesh& mesh);
	
	void LoadVertices(ifstream& inFile, Mesh& mesh)
	{
		inFile >> mesh.vertexCount;

		if (mesh.vertexCount > 0)
		{
			mesh.vertices = new Vertex[mesh.vertexCount];

			for (int i = 0; i < mesh.vertexCount; i++)
			{
				inFile >> mesh.vertices[i].x;
				inFile >> mesh.vertices[i].y;
				inFile >> mesh.vertices[i].z;
			}
		}
	}

	void LoadColours(ifstream& inFile, Mesh& mesh)
	{
		inFile >> mesh.colorCount;

		if (mesh.colorCount > 0)
		{
			mesh.colors = new Color[mesh.colorCount];
			for (int i = 0; i < mesh.colorCount; i++)
			{
				inFile >> mesh.colors[i].r;
				inFile >> mesh.colors[i].g;
				inFile >> mesh.colors[i].b;
			}
		}
	}

	void LoadIndices(ifstream& inFile, Mesh& mesh)
	{
		inFile >> mesh.indexCount;

		if (mesh.indexCount > 0)
		{
			mesh.indices = new GLushort[mesh.indexCount];
			for (int i = 0; i < mesh.indexCount; i++)
			{
				inFile >> mesh.indices[i];
			}
		}
		
	}

	void LoadTexCoords(ifstream& inFile, TexturedMesh& mesh)
	{
		inFile >> mesh.texCoordCount;

		if (mesh.texCoordCount > 0)
		{
			mesh.TexCoords = new TexCoords[mesh.texCoordCount];
			for (int i = 0; i < mesh.texCoordCount; i++)
			{
				inFile >> mesh.TexCoords[i].u;
				inFile >> mesh.TexCoords[i].v;
			}
		}
	}

	Mesh* MeshLoader::Load(char* path)
	{
		Mesh* mesh = new Mesh();

		ifstream inFile;

		inFile.open(path);

		if (!inFile.good())
		{
			cerr << "Can't open texture file " << path << endl;
			return nullptr;
		}

		LoadVertices(inFile, *mesh);
		LoadColours(inFile, *mesh);
		LoadIndices(inFile, *mesh);

		return mesh;
	}

	TexturedMesh* MeshLoader::LoadTex(char* path)
	{
		TexturedMesh* texMesh = new TexturedMesh();
		texMesh->mesh = new Mesh();

		ifstream inFile;

		inFile.open(path);

		if (!inFile.good())
		{
			cerr << "Can't open texture file " << path << endl;
			return nullptr;
		}

		LoadVertices(inFile, *texMesh->mesh);
		LoadColours(inFile, *texMesh->mesh);
		LoadTexCoords(inFile, *texMesh);
		LoadIndices(inFile, *texMesh->mesh);

		return texMesh;
	}
}

