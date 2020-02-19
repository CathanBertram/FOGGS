#pragma once
#include "SceneObject.h"

class FlyingObjects : public SceneObject
{
public:
	FlyingObjects(TexturedMesh* mesh, Texture2D* texture, float x, float y, float z, float rx, float ry, float rz, float rotate);
	~FlyingObjects();

	void Draw();
	void Update();

private:
	Vector3 _position;
	Vector3 _rotation;
	float rotation;
	float rotationSpeed;
};

