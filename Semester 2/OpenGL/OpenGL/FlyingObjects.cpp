#include "FlyingObjects.h"

FlyingObjects::FlyingObjects(Mesh* mesh, float x, float y, float z, float rx, float ry, float rz, float rotate) : SceneObject(mesh)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
	_rotation.x = rx;
	_rotation.y = ry;
	_rotation.z = rz;
	rotation = rotate;
	rotationSpeed = rotate;
}


FlyingObjects::~FlyingObjects()
{
}

void FlyingObjects::Draw()
{
	if (_mesh->vertices !=nullptr && _mesh->colors !=nullptr && _mesh->indices!=nullptr)
	{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _mesh->vertices);
	glColorPointer(3, GL_FLOAT, 0, _mesh->colors);

	glPushMatrix();
	glTranslatef(_position.x, _position.y, _position.z);
	glRotatef(rotation, _rotation.x, _rotation.y,_rotation.z);
	glDrawElements(GL_TRIANGLES, _mesh->indexCount, GL_UNSIGNED_SHORT, _mesh->indices);
	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void FlyingObjects::Update()
{
	rotation += rotationSpeed;
	if (rotation >= 360.0f)
	{
		rotation = 0.0f;
	}
	_position.z -= 0.35f;
	if (_position.z <=-3.0f)
	{
		_position.z = 100.0f;
	}
}