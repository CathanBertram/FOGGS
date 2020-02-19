#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL\freeglut.h"
#include "Structures.h"
#include <fstream>
#include <iostream>
#include "Structures.h"
#include "Texture2D.h"

class SceneObject
{
protected:
	TexturedMesh* _texMesh;
	Mesh* _mesh;
	Texture2D* _texture;

public:
	SceneObject(TexturedMesh* mesh, Texture2D* texture);
	SceneObject(Mesh* mesh);
	virtual ~SceneObject();

	virtual void Update();
	virtual void Draw();
};

