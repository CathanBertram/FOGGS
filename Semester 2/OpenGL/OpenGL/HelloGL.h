#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL\freeglut.h"
#include "GLUTCallbacks.h"

#define REFRESHRATE 60


class HelloGL
{
public:
	HelloGL(int argc, char* argv[]);

	~HelloGL(void);

	void Display();

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
};
