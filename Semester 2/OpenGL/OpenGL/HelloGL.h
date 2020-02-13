#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL\freeglut.h"
#include "GLUTCallbacks.h"
#include "Structures.h"
#include "FlyingObjects.h"
#include "MeshLoader.h"

#define REFRESHRATE 60
#define CUBE_NUM 200

class HelloGL
{
public:
	HelloGL(int argc, char* argv[]);

	~HelloGL(void);

	void Display();

	//Draw Shape Functions
	void DrawTriangle();

	void DrawPolygon();

	//Inpout Functions
	void Keyboard(unsigned char key, int x, int y);

	void KeyboardUp(unsigned char key, int x, int y);

	void Special(int key, int x, int y);

	void SpecialUp(int key, int x, int y);

	void Mouse(int button, int state, int x, int y);

	void Motion(int x, int y);

	void PassiveMotion(int x, int y);

	//Init Functions
	void InitObjects();
	void InitGL(int argc, char* argv[]);

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
	FlyingObjects* cube[CUBE_NUM];
	FlyingObjects* cube2[CUBE_NUM];
	FlyingObjects* pyramid[CUBE_NUM];
};
