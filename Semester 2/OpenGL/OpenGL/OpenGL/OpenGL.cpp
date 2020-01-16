#include "OpenGL.h"



OpenGL::OpenGL(int argc, char*argv[])
{
	GLUTCallbacks::Init(this);
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutCreateWindow("opengl");
	glutDisplayFunc(GLUTCallbacks::Display);
	glutMainLoop();
}

void OpenGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//DrawPolygon();
	//DrawTriangle();
	DrawTriangleFan();
	glFlush();
}

void OpenGL::DrawPolygon()
{
	glBegin(GL_POLYGON);
	{
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glVertex2f(-0.75, 0.5);
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex2f(0.75, 0.5);
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
		glVertex2f(0.75, -0.5);
		glEnd();
	}
}

void OpenGL::DrawTriangle()
{
	glBegin(GL_TRIANGLES);
	{
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glVertex2f(-0.75, 0.5);
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex2f(0.75, 0.5);
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
		glVertex2f(0.75, -0.5);
		glEnd();
	}
}

void OpenGL::DrawTriangleFan()
{
	glBegin(GL_TRIANGLE_STRIP);
	{
		glColor4f(0.5f, 0.3f, 1.0f, 1.0f);
		glVertex2f(-0.9, 0.5);
		glColor4f(1.0f, 0.5f, 0.3f, 1.0f);
		glVertex2f(-0.9, 0.3);
		glColor4f(0.3f, 0.6f, 0.4f, 1.0f);
		glVertex2f(-0.7, 0.5);
		glColor4f(1.0f, 0.9f, 0.8f, 1.0f);
		glVertex2f(-0.3, -0.4);
		glColor4f(0.1f, 0.3f, 0.0f, 1.0f);
		glVertex2f(0,-0.5);
		glColor4f(0.4f, 0.2f, 0.0f, 1.0f);
		glVertex2f(0.3, 0.3);
		glColor4f(0.4f, 0.7f, 0.1f, 1.0f);
		glVertex2f(0.7, 0.5);
		glEnd();
	}
}



OpenGL::~OpenGL(void)
{
}
