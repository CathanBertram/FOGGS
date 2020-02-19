#include "FlyingObjects.h"

FlyingObjects::FlyingObjects(TexturedMesh* mesh, Texture2D* texture, float x, float y, float z, float rx, float ry, float rz, float rotate) : SceneObject(mesh, texture)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
	_rotation.x = rx;
	_rotation.y = ry;
	_rotation.z = rz;
	rotation = rotate;
	rotationSpeed = rotate;
	_texture = texture;
}


FlyingObjects::~FlyingObjects()
{
}

void FlyingObjects::Draw()
{
	if (_texMesh->mesh->vertices !=nullptr && _texMesh->mesh->colors !=nullptr && _texMesh->mesh->indices!=nullptr && _texMesh->TexCoords !=nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, _texture->GetID());
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, _texMesh->mesh->vertices);
		glColorPointer(3, GL_FLOAT, 0, _texMesh->mesh->colors);

		glTexCoordPointer(2, GL_FLOAT, 0, _texMesh->TexCoords);

		glPushMatrix();
		glTranslatef(_position.x, _position.y, _position.z);
		glRotatef(rotation, _rotation.x, _rotation.y,_rotation.z);
		glDrawElements(GL_TRIANGLES, _texMesh->mesh->indexCount, GL_UNSIGNED_SHORT, _texMesh->mesh->indices);
		glPopMatrix();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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
