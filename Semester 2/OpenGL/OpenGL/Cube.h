#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL\freeglut.h"
#include "Structures.h"

class Cube
{
public:
	Cube(float x, float y, float z, float rx, float ry, float rz, float rotate);
	~Cube();

	void Draw();
	void Update();

private:
	static Vertex indexedVertices[];
	static Color indexedColors[];
	static GLushort indices[];

	Vector3 _position;
	Vector3 _rotation;
	float rotation;
	float rotationSpeed;

};

