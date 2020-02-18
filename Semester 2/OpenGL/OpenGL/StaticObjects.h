#pragma once
#include "SceneObject.h"

class StaticObjects : public SceneObject
{
public:
	StaticObjects(Mesh* mesh, float x, float y, float z);
	~StaticObjects();

	void Draw();
	void Update();
private:
	Vector3 _position;
};

