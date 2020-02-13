#pragma once

#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL\freeglut.h"


struct Vector3
{
	float x;
	float y;
	float z;
};

struct Camera
{
	Vector3 eye;
	Vector3 center;
	Vector3 up;
};

struct Color
{
	GLfloat r, g, b;
};


struct Vertex
{
	GLfloat x, y, z;
};

struct Mesh
{
	Vertex* vertices;
	Color* colors;
	GLushort* indices;
	int vertexCount, colorCount, indexCount;
};
