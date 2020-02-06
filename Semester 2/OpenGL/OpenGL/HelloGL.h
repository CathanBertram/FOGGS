#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL\freeglut.h"
#include "GLUTCallbacks.h"
#include "Structures.h"
#include "Cube.h"

#define REFRESHRATE 60
#define CUBE_NUM 10000

class HelloGL
{
public:
	HelloGL(int argc, char* argv[]);

	~HelloGL(void);

	void Display();

	/*void DrawCube();

	void DrawCubeArray();

	void DrawCubeArrayAlt();

	void DrawCubeIndexed();

	void DrawCubeIndexedAlt();*/

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

	Camera* camera;
	Cube* cube[CUBE_NUM];
};
