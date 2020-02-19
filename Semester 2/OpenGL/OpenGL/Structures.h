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

struct Vector4
{
	float x;
	float y;
	float z;
	float w;
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

struct TexCoords
{
	GLfloat u;
	GLfloat v;
};

struct Mesh
{
	Vertex* vertices;
	Color* colors;
	Vector3* normals;
	GLushort* indices;
	int vertexCount, colorCount, indexCount;
};

struct TexturedMesh
{
	Mesh* mesh;
	int texCoordCount;
	TexCoords* TexCoords;
};

struct Lighting
{
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
};

struct Material
{
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	GLfloat shininess;
};



