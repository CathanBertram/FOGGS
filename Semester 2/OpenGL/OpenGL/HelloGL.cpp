#include "HelloGL.h"

Vertex HelloGL::vertices[] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,      // v0-v1-v2 (front)
				-1,-1, 1,   1,-1, 1,   1, 1, 1,      // v2-v3-v0

				1, 1, 1,   1,-1, 1,   1,-1,-1,      // v0-v3-v4 (right)
				1,-1,-1,   1, 1,-1,   1, 1, 1,      // v4-v5-v0

				1, 1, 1,   1, 1,-1,  -1, 1,-1,      // v0-v5-v6 (top)
				-1, 1,-1,  -1, 1, 1,   1, 1, 1,      // v6-v1-v0

				-1, 1, 1,  -1, 1,-1,  -1,-1,-1,      // v1-v6-v7 (left)
				-1,-1,-1,  -1,-1, 1,  -1, 1, 1,      // v7-v2-v1

				-1,-1,-1,   1,-1,-1,   1,-1, 1,      // v7-v4-v3 (bottom)
				1,-1, 1,  -1,-1, 1,  -1,-1,-1,      // v3-v2-v7

				1,-1,-1,  -1,-1,-1,  -1, 1,-1,      // v4-v7-v6 (back)
				-1, 1,-1,   1, 1,-1,   1,-1,-1 };    // v6-v5-v4

Color HelloGL::colors[] = { 1, 1, 1,   1, 1, 0,   1, 0, 0,      // v0-v1-v2 (front)
				1, 0, 0,   1, 0, 1,   1, 1, 1,      // v2-v3-v0

				1, 1, 1,   1, 0, 1,   0, 0, 1,      // v0-v3-v4 (right)
				0, 0, 1,   0, 1, 1,   1, 1, 1,      // v4-v5-v0

				1, 1, 1,   0, 1, 1,   0, 1, 0,      // v0-v5-v6 (top)
				0, 1, 0,   1, 1, 0,   1, 1, 1,      // v6-v1-v0

				1, 1, 0,   0, 1, 0,   0, 0, 0,      // v1-v6-v7 (left)
				0, 0, 0,   1, 0, 0,   1, 1, 0,      // v7-v2-v1

				0, 0, 0,   0, 0, 1,   1, 0, 1,      // v7-v4-v3 (bottom)
				1, 0, 1,   1, 0, 0,   0, 0, 0,      // v3-v2-v7

				0, 0, 1,   0, 0, 0,   0, 1, 0,      // v4-v7-v6 (back)
				0, 1, 0,   0, 1, 1,   0, 0, 1 };    // v6-v5-v4

HelloGL::HelloGL(int argc, char* argv[])
{
	//Initialise Camera
	camera = new Camera();
	//Initialise Cube
	for (int i = 0; i < CUBE_NUM; i++)
	{
		cube[i] = new Cube(((rand()%100)/5.0f)-10.0f,((rand()%100)/5.0f)-10.0f,(rand()%1200)/10.0f, rand() % 20 + (-10), rand() % 20 + (-10), rand() % 20 + (-10), rand() % 10 + (-5));
		//cube[i] = new Cube(i,i,-2);
	}
	//Initialise Camera Variables
	camera->eye.x = 0.0f; camera->eye.y = 0.0f; camera->eye.z = -5.0f;
	camera->center.x = 0.0f; camera->center.y = 0.0f; camera->center.z = 0.0f;
	camera->up.x = 0.0f; camera->up.y =1.0f; camera->up.z = 0.0f;
	//Initialise Other Variables
	rotation = 0.0f;
	color1 = 1.0f;
	color2 = 0.0f;
	c1r = false;
	c2r = false;
	//Initialise OpenGL Features
	GLUTCallbacks::Init(this);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("qubz");
	glutDisplayFunc(GLUTCallbacks::Display);
	glutTimerFunc(1000/REFRESHRATE,GLUTCallbacks::Timer,1000/REFRESHRATE);
	glutKeyboardFunc(GLUTCallbacks::Keyboard);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, 800, 800);
	gluPerspective(179, 1, 0, 1000);
	glMatrixMode(GL_MODELVIEW); 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glutMainLoop();
}

HelloGL::~HelloGL()
{
	delete camera;
	for (int i = 0; i < CUBE_NUM; i++)
	{
		delete cube[i];
	}
}

void HelloGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < CUBE_NUM; i++)
	{
	cube[i]->Draw();
	}
	glFlush();
	glutSwapBuffers();
}

/*

Draw Cube Methods
void HelloGL::DrawCube()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -2.0f);
	glRotatef(rotation, -1.0f, 0.0f, 0.0f);
	glRotatef(rotation/2, 0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	{
		// face v0-v1-v2
		glColor3f(1, 1, 1);
		glVertex3f(1, 1, 1);
		glColor3f(1, 1, 0);
		glVertex3f(-1, 1, 1);
		glColor3f(1, 0, 0);
		glVertex3f(-1, -1, 1);
		// face v2-v3-v0
		glColor3f(1, 0, 0);
		glVertex3f(-1, -1, 1);
		glColor3f(1, 0, 1);
		glVertex3f(1, -1, 1);
		glColor3f(1, 1, 1);
		glVertex3f(1, 1, 1);
		// face v0-v3-v4
		glColor3f(1, 1, 1);
		glVertex3f(1, 1, 1);
		glColor3f(1, 0, 1);
		glVertex3f(1, -1, 1);
		glColor3f(0, 0, 1);
		glVertex3f(1, -1, -1);
		// face v4-v5-v0
		glColor3f(0, 0, 1);
		glVertex3f(1, -1, -1);
		glColor3f(0, 1, 1);
		glVertex3f(1, 1, -1);
		glColor3f(1, 1, 1);
		glVertex3f(1, 1, 1);
		// face v0-v5-v6
		glColor3f(1, 1, 1);
		glVertex3f(1, 1, 1);
		glColor3f(0, 1, 1);
		glVertex3f(1, 1, -1);
		glColor3f(0, 1, 0);
		glVertex3f(-1, 1, -1);
		// face v6-v1-v0
		glColor3f(0, 1, 0);
		glVertex3f(-1, 1, -1);
		glColor3f(1, 1, 0);
		glVertex3f(-1, 1, 1);
		glColor3f(1, 1, 1);
		glVertex3f(1, 1, 1);
		// face  v1-v6-v7
		glColor3f(1, 1, 0);
		glVertex3f(-1, 1, 1);
		glColor3f(0, 1, 0);
		glVertex3f(-1, 1, -1);
		glColor3f(0, 0, 0);
		glVertex3f(-1, -1, -1);
		// face v7-v2-v1
		glColor3f(0, 0, 0);
		glVertex3f(-1, -1, -1);
		glColor3f(1, 0, 0);
		glVertex3f(-1, -1, 1);
		glColor3f(1, 1, 0);
		glVertex3f(-1, 1, 1);
		// face v7-v4-v3
		glColor3f(0, 0, 0);
		glVertex3f(-1, -1, -1);
		glColor3f(0, 0, 1);
		glVertex3f(1, -1, -1);
		glColor3f(1, 0, 1);
		glVertex3f(1, -1, 1);
		// face v3-v2-v7
		glColor3f(1, 0, 1);
		glVertex3f(1, -1, 1);
		glColor3f(1, 0, 0);
		glVertex3f(-1, -1, 1);
		glColor3f(0, 0, 0);
		glVertex3f(-1, -1, -1);
		// face v4-v7-v6
		glColor3f(0, 0, 1);
		glVertex3f(1, -1, -1);
		glColor3f(0, 0, 0);
		glVertex3f(-1, -1, -1);
		glColor3f(0, 1, 0);
		glVertex3f(-1, 1, -1);
		// face v6-v5-v4
		glColor3f(0, 1, 0);
		glVertex3f(-1, 1, -1);
		glColor3f(0, 1, 1);
		glVertex3f(1, 1, -1);
		glColor3f(0, 0, 1);
		glVertex3f(1, -1, -1);

		glEnd();
	}
	glPopMatrix();
}

void HelloGL::DrawCubeArray()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -2.0f);
	glRotatef(rotation, -1.0f, 0.0f, 0.0f);
	glRotatef(rotation / 2, 0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 36; i++)
	{
		//glColor3f(colors[i].r, colors[i].g, colors[i].b);
		//glVertex3f(vertices[i].x,vertices[i].y,vertices[i].z);
		glColor3fv(&colors[i].r);
		glVertex3fv(&vertices[i].x);
	}
	glEnd();

	glPopMatrix();
}

void HelloGL::DrawCubeArrayAlt()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -2.0f);
	glRotatef(rotation, -1.0f, 0.0f, 0.0f);
	glRotatef(rotation / 2, 0.0f, 1.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}

void HelloGL::DrawCubeIndexed()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -2.0f);
	glRotatef(rotation, -1.0f, 0.0f, 0.0f);
	glRotatef(rotation / 2, 0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 36; i++)
	{
		glColor3f(indexedColors[indices[i]].r, indexedColors[indices[i]].g, indexedColors[indices[i]].b);
		glVertex3f(indexedVertices[indices[i]].x, indexedVertices[indices[i]].y, indexedVertices[indices[i]].z);
	}
	glEnd();
}

void HelloGL::DrawCubeIndexedAlt()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, indexedVertices);
	glColorPointer(3, GL_FLOAT, 0, indexedColors);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -2.0f);
	glRotatef(rotation, -1.0f, 0.0f, 0.0f);
	glRotatef(rotation / 2, 0.0f, 1.0f, 0.0f);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, indices);
	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}
*/

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
	case 'a':
		camera->center.x += 1.0f;
	case 'd':
		camera->center.x -= 1.0f;
	case 27:
		exit(0);
		break;
	}
}

void HelloGL::Update()
{
	glLoadIdentity();
	gluLookAt(camera->eye.x, camera->eye.y, camera->eye.z, camera->center.x, 
			camera->center.y, camera->center.z, 
			camera->up.x, camera->up.y, camera->up.z);
	for (int i = 0; i < CUBE_NUM; i++)
	{
	cube[i]->Update();
	}
	rotation += 1.0f;
	if (rotation>=360.0f)
	{
		rotation = 0.0f;
	}
	if (color1 >1.0f)
	{
		c1r = true;
	}
	if (color1 < 0.0f)
	{
		c1r =false;
	}
	if (color2 > 1.0f)
	{
		c2r = true;
	}
	if (color2 < 0.0f)
	{
		c2r = false;
	}

	if (c1r == true)
	{
		color1 -= 0.01f;
	}
	else
	{
		color1 += 0.01f;
	}
	if (c2r == true)
	{
		color2 -= 0.01f;
	}
	else
	{
		color2 += 0.01f;
	}

	glutPostRedisplay();
}
