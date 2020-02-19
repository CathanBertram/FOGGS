#include "SceneObject.h"

SceneObject::SceneObject(TexturedMesh* mesh, Texture2D* texture)
{
	_texMesh = mesh;
}

SceneObject::SceneObject(Mesh* mesh)
{
	_mesh = mesh;
}


SceneObject::~SceneObject()
{
}

void SceneObject::Update()
{
}

void SceneObject::Draw()
{
}
