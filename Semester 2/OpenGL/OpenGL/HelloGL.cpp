#include "HelloGL.h"

HelloGL::HelloGL(int argc, char* argv[])
{
	InitGL(argc, argv);
	InitObjects();

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
	for (int i = 0; i < CUBE_NUM; i++)
	{
		delete cube[i];
		cube[i] = nullptr;
	}
	delete cube;
}

void HelloGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < CUBE_NUM; i++)
	{
	cube[i]->Draw();
	cube2[i]->Draw();
	pyramid[i]->Draw();
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
	Mesh* cubeMesh = MeshLoader::Load((char*)"OBJs/cube.txt");
	Mesh* cube2Mesh = MeshLoader::Load((char*)"OBJs/cube2.txt");
	Mesh* pyramidMesh = MeshLoader::Load((char*)"OBJs/pyramid.txt");
	for (int i = 0; i < CUBE_NUM; i++)
	{
		cube[i] = new FlyingObjects(cubeMesh,((rand() % 100) / 5.0f) - 10.0f, ((rand() % 100) / 5.0f) - 10.0f, (rand() % 1200) / 10.0f, rand() % 20 + (-10), rand() % 20 + (-10), rand() % 20 + (-10), rand() % 10 + (-5));
		cube2[i] = new FlyingObjects(cube2Mesh, ((rand() % 100) / 5.0f) - 10.0f, ((rand() % 100) / 5.0f) - 10.0f, (rand() % 1200) / 10.0f, rand() % 20 + (-10), rand() % 20 + (-10), rand() % 20 + (-10), rand() % 10 + (-5));
		pyramid[i] = new FlyingObjects(pyramidMesh, ((rand() % 100) / 5.0f) - 10.0f, ((rand() % 100) / 5.0f) - 10.0f, (rand() % 1200) / 10.0f, rand() % 20 + (-10), rand() % 20 + (-10), rand() % 20 + (-10), rand() % 10 + (-5));
	}
	//Initialise Camera Variables
	camera->eye.x = 0.0f; camera->eye.y = 0.0f; camera->eye.z = -5.0f;
	camera->center.x = 0.0f; camera->center.y = 0.0f; camera->center.z = 0.0f;
	camera->up.x = 0.0f; camera->up.y = 1.0f; camera->up.z = 0.0f;
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
}

void HelloGL::Update()
{
	glLoadIdentity();
	gluLookAt(camera->eye.x, camera->eye.y, camera->eye.z, 
			camera->center.x, camera->center.y, camera->center.z, 
			camera->up.x, camera->up.y, camera->up.z);
	for (int i = 0; i < CUBE_NUM; i++)
	{
		cube[i]->Update();
		cube2[i]->Update();
		pyramid[i]->Update();
	}

	glutPostRedisplay();
}
