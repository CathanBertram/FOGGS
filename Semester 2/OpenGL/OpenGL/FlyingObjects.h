#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL\freeglut.h"
#include "Structures.h"
#include <fstream>
#include <iostream>
#include "SceneObject.h"

class FlyingObjects : public SceneObject
{
public:
	FlyingObjects(Mesh* mesh, float x, float y, float z, float rx, float ry, float rz, float rotate);
	~FlyingObjects();

	void Draw();
	void Update();

private:
	Vector3 _position;
	Vector3 _rotation;
	float rotation;
	float rotationSpeed;
};

