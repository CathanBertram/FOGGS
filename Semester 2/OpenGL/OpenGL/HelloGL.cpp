#include "HelloGL.h"

HelloGL::HelloGL(int argc, char* argv[])
{
	InitGL(argc, argv);
	InitObjects();
	InitLight();

	//Initialise Other Variables
	rotation = 0.0f;
	color1 = 1.0f;
	color2 = 0.0f;
	c1r = false;
	c2r = false;
	
	glutMainLoop();
}

HelloGL::~HelloGL()
{
	delete camera;
	camera = nullptr;
	for (int i = 0; i < OBJ_NUM; i++)
	{
		delete objects[i];
		objects[i] = nullptr;
	}
	delete objects;

	
}

void HelloGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < OBJ_NUM; i++)
	{
		objects[i]->Draw();
	}
	glFlush();
	glutSwapBuffers();
}

void HelloGL::DrawTriangle()
{
	glPushMatrix();
	glRotatef(rotation, 0.0f, 0.0f, -1.0f);
	glBegin(GL_TRIANGLES);
	{

		glColor4f(color1, color1, color2, 0.0f);
		glVertex2f(-0.75, -0.5);
		glColor4f(color2, color1, color1, 0.0f);
		glVertex2f(0.75, -0.5);
		glColor4f(color1, color2, color1, 0.0f);
		glVertex2f(0.0, 0.5);

		glEnd();
	}
}

void HelloGL::DrawPolygon()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -2.0f);
	glRotatef(rotation, 0.0f, 0.0f, -1.0f);
	glBegin(GL_POLYGON);
	{
		glColor4f(color1, color1, color2, 0.0f);
		glVertex2f(-0.5, -0.5);
		glColor4f(color1, color2, color1, 0.0f);
		glVertex2f(0.5, -0.5);
		glColor4f(color2, color1, color1, 0.0f);
		glVertex2f(0.5, 0.5);
		glColor4f(color1, color2, color1, 0.0f);
		glVertex2f(-0.5, 0.5);
		glEnd();
	}
	glPopMatrix();
}

void HelloGL::Keyboard(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
		camera->eye.z += 1.0f;
	}
	else if (key == 's')
	{
		camera->eye.z -= 1.0f;
	}
	else if (key == 'a')
	{
		camera->center.x += 1.0f;
	}
	else if (key == 'd')
	{
		camera->center.x -= 1.0f;
	}
	switch (key)
	{
	case 'w':
		camera->eye.z += 1.0f;
		break;
	case 's':
		camera->eye.z -= 1.0f;
		break;
	case 'a':
		camera->center.x += 1.0f;
		break;
	case 'd':
		camera->center.x -= 1.0f;
		break;
	case 27:
		glutLeaveMainLoop();
		break;
	}
}

void HelloGL::KeyboardUp(unsigned char key, int x, int y)
{
}

void HelloGL::Special(int key, int x, int y)
{
}

void HelloGL::SpecialUp(int key, int x, int y)
{
}

void HelloGL::Mouse(int button, int state, int x, int y)
{
}

void HelloGL::Motion(int x, int y)
{
}

void HelloGL::PassiveMotion(int x, int y)
{
}

void HelloGL::InitObjects()
{
	//Initialise Camera
	camera = new Camera();
	//Initialise Cube
	TexturedMesh* cubeMesh = MeshLoader::LoadTex((char*)"OBJs/CubeLight.txt");
	Texture2D* cubeTexture = new Texture2D();
	cubeTexture->Load((char*)"Textures/stars.raw", 512, 512);
	
	for (int i = 0; i < OBJ_NUM; i++)
	{
		objects[i] = new FlyingObjects(cubeMesh, cubeTexture, ((rand() % 100) / 5.0f) - 10.0f, ((rand() % 100) / 5.0f) - 10.0f, -(rand() % 1200) / 10.0f, rand() % 20 + (-10), rand() % 20 + (-10), rand() % 20 + (-10), rand() % 10 + (-5));
	}
	//Initialise Camera Variables
	camera->eye.x = 0.0f; camera->eye.y = 0.0f; camera->eye.z = 5.0f;
	camera->center.x = 0.0f; camera->center.y = 0.0f; camera->center.z = 0.0f;
	camera->up.x = 0.0f; camera->up.y = 1.0f; camera->up.z = 0.0f;
}

void HelloGL::InitLight()
{
	_lightPos = new Vector4();
	_lightPos->x = 0.0;
	_lightPos->y = 0.0;
	_lightPos->z = 1.0;
	_lightPos->w = 0.0;

	_lightData = new Lighting();
	_lightData->Ambient.x = 0.2;
	_lightData->Ambient.y = 0.2;
	_lightData->Ambient.z = 0.2;
	_lightData->Ambient.w = 1.0;
	_lightData->Diffuse.x = 0.8;
	_lightData->Diffuse.y = 0.8;
	_lightData->Diffuse.z = 0.8;
	_lightData->Diffuse.w = 1.0;
	_lightData->Specular.x = 0.2;
	_lightData->Specular.y = 0.2;
	_lightData->Specular.z = 0.2;
	_lightData->Specular.w = 1.0;

}

void HelloGL::InitGL(int argc, char* argv[])
{
	//Initialise OpenGL Features
	GLUTCallbacks::Init(this);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("qubz");
	glutDisplayFunc(GLUTCallbacks::Display);
	glutTimerFunc(1000 / REFRESHRATE, GLUTCallbacks::Timer, 1000 / REFRESHRATE);
	glutKeyboardFunc(GLUTCallbacks::Keyboard);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, 800, 800);
	gluPerspective(120, 1, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//Z Rendering
	glEnable(GL_DEPTH_TEST);
	//Backface Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//Enable Texturing
	glEnable(GL_TEXTURE_2D);
	//Enable Lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void HelloGL::Update()
{
	glLoadIdentity();
	gluLookAt(camera->eye.x, camera->eye.y, camera->eye.z, 
			camera->center.x, camera->center.y, camera->center.z, 
			camera->up.x, camera->up.y, camera->up.z);

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(_lightData->Ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(_lightData->Diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(_lightData->Specular.x));

	glLightfv(GL_LIGHT0, GL_POSITION, &(_lightPos->x));

	for (int i = 0; i < OBJ_NUM; i++)
	{
		objects[i]->Update();
	}

	glutPostRedisplay();
}
