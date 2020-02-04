#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL\freeglut.h"
#include "GLUTCallbacks.h"

#define REFRESHRATE 60

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

class HelloGL
{
public:
	HelloGL(int argc, char* argv[]);

	~HelloGL(void);

	void Display();

	void DrawCube();

	void DrawCubeArray();

	void DrawCubeArrayAlt();

	void DrawCubeIndexed();

	void DrawCubeIndexedAlt();

	void DrawTriangle();

	void DrawPolygon();

	void Keyboard(unsigned char key, int x, int y);

	void Update();

private:
	float rotation;
	float color1;
	float color2;
	bool c1r;
	bool c2r;

	static Vertex vertices[];
	static Color colors[];

	static Vertex indexedVertices[];
	static Color indexedColors[];
	static GLushort indices[];

	Camera* camera;
};
