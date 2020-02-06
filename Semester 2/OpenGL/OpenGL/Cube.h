#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL\freeglut.h"
#include "Structures.h"
#include <fstream>
#include <iostream>

class Cube
{
public:
	Cube(float x, float y, float z, float rx, float ry, float rz, float rotate);
	~Cube();

	void Draw();
	void Update();
	static bool Load(char* path);

private:
	static Vertex* indexedVertices;
	static Color* indexedColors;
	static GLushort* indices;

	static int numVertices;
	static int numColors;
	static int numIndices;

	Vector3 _position;
	Vector3 _rotation;
	float rotation;
	float rotationSpeed;

};

