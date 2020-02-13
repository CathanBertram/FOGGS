#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL\freeglut.h"
#include "Structures.h"
#include <fstream>
#include <iostream>

class FlyingObjects
{
public:
	FlyingObjects(Mesh* mesh, float x, float y, float z, float rx, float ry, float rz, float rotate);
	~FlyingObjects();

	void Draw();
	void Update();
	static bool Load(char* path);

private:
	Vector3 _position;
	Vector3 _rotation;
	float rotation;
	float rotationSpeed;

	Mesh* _mesh;
};

