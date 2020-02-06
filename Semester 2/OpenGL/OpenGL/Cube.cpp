#include "Cube.h"

Vertex* Cube::indexedVertices = nullptr; 

/*Color Cube::indexedColors[] = { 1, 0, 1,   1, 1, 1,   // v0,v1,
				1, 1, 1,   1, 1, 1,   // v2,v3
				0, 1, 1,   1, 1, 1,   // v4,v5
				1, 1, 1,   1, 1, 0 }; //v6,v7*/

Color* Cube::indexedColors = nullptr;

GLushort* Cube::indices = nullptr; 

int Cube::numVertices = 0;
int Cube::numColors = 0;
int Cube::numIndices = 0;

Cube::Cube(float x, float y, float z, float rx, float ry, float rz, float rotate)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
	_rotation.x = rx;
	_rotation.y = ry;
	_rotation.z = rz;
	rotation = rotate;
	rotationSpeed = rotate;

}


Cube::~Cube()
{
}

void Cube::Draw()
{
	if (indexedVertices !=nullptr && indexedColors !=nullptr && indices!=nullptr)
	{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, indexedVertices);
	glColorPointer(3, GL_FLOAT, 0, indexedColors);

	glPushMatrix();
	glTranslatef(_position.x, _position.y, _position.z);
	glRotatef(rotation, _rotation.x, _rotation.y,_rotation.z);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, indices);
	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void Cube::Update()
{
	rotation += rotationSpeed;
	if (rotation >= 360.0f)
	{
		rotation = 0.0f;
	}
	_position.z -= 0.35f;
	if (_position.z <=-5.0f)
	{
		_position.z = 100.0f;
	}
}

bool Cube::Load(char* path)
{
	std::ifstream inFile;
	inFile.open(path);
	if (!inFile.good())
	{
		std::cerr << "Cant open text file" << path << std::endl;
		return false;
	}

	inFile >> numVertices;
	indexedVertices = new Vertex[numVertices];
	for (int i = 0; i < numVertices; i++)
	{
		inFile >> indexedVertices[i].x;
		inFile >> indexedVertices[i].y;
		inFile >> indexedVertices[i].z;
	}

	inFile >> numColors;
	indexedColors = new Color[numColors];
	for (int i = 0; i < numVertices; i++)
	{
		inFile >> indexedColors[i].r;
		inFile >> indexedColors[i].g;
		inFile >> indexedColors[i].b;
	}
	
	inFile >> numIndices;
	indices = new GLushort[numIndices];
	for (int i = 0; i < numIndices; i++)
	{
		inFile >> indices[i];
	}

	inFile.close();
	return true;
}
