#include "HelloGL.h"



HelloGL::HelloGL(int argc, char* argv[])
{
	rotation = 0.0f;
	color1 = 1.0f;
	color2 = 0.0f;
	c1r = false;
	c2r = false;
	GLUTCallbacks::Init(this);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("simple OpenGL Program");
	glutDisplayFunc(GLUTCallbacks::Display);
	glutTimerFunc(1000/REFRESHRATE,GLUTCallbacks::Timer,1000/REFRESHRATE);
	glutKeyboardFunc(GLUTCallbacks::Keyboard);
	glutMainLoop();
}


HelloGL::~HelloGL()
{
}

void HelloGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	DrawPolygon();
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
	if (key == 'd')
	{
		rotation++;
	}
}

void HelloGL::Update()
{
	rotation += 89.0f;
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
