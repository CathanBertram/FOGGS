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
	if (_texMesh->mesh->vertices !=nullptr && _texMesh->mesh->indices !=nullptr && _texMesh->TexCoords !=nullptr && _texMesh->mesh->normals !=nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, _texture->GetID());
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, _texMesh->mesh->vertices);
		glNormalPointer(GL_FLOAT, 0, _texMesh->mesh->normals);
		glTexCoordPointer(2, GL_FLOAT, 0, _texMesh->TexCoords);

		LoadMat();

		glMaterialfv(GL_FRONT, GL_AMBIENT, &(_material->Ambient.x));
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &(_material->Diffuse.x));
		glMaterialfv(GL_FRONT, GL_SPECULAR, &(_material->Specular.x));
		glMaterialf(GL_FRONT, GL_SHININESS, _material->Shininess);

		glPushMatrix();
		glTranslatef(_position.x, _position.y, _position.z);
		glRotatef(rotation, _rotation.x, _rotation.y,_rotation.z);
		glDrawElements(GL_TRIANGLES, _texMesh->mesh->indexCount, GL_UNSIGNED_SHORT, _texMesh->mesh->indices);
		glPopMatrix();

		glDisableClientState(GL_NORMAL_ARRAY);
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
	_position.z += 0.35f;
	if (_position.z >= 0.0f)
	{
		_position.z = -100.0f;
	}
}

void FlyingObjects::LoadMat()
{
	_material = new Material();
	_material->Ambient.x = 0.7; _material->Ambient.y = 0.7; _material->Ambient.z = 0.7;
	_material->Ambient.w = 1.0;
	_material->Diffuse.x = 0.7; _material->Diffuse.y = 0.7; _material->Diffuse.z = 0.7;
	_material->Diffuse.w = 1.0;
	_material->Specular.x = 1.0; _material->Specular.y = 1.0; _material->Specular.z = 1.0;
	_material->Specular.w = 1.0;
	_material->Shininess = 100.0f;
}
