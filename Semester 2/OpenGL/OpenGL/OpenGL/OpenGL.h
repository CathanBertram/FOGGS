#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"
#include "GLUTCallbacks.h"

class OpenGL
{
public:
	void Display();
	void DrawPolygon();
	void DrawTriangle();
	void DrawTriangleFan();
	OpenGL(int argc, char* argv[]);

	~OpenGL(void);

};

