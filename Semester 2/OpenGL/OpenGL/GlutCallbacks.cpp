#include "GLUTCallbacks.h"
#include "HelloGL.h"

namespace GLUTCallbacks
{
	namespace
	{
		HelloGL* helloGL = nullptr;
	}

	void Init(HelloGL* gl)
	{
		helloGL = gl;
	}

	void Display()
	{
		if (helloGL != nullptr)
		{
			helloGL->Display();
		}
	}

	void Keyboard(unsigned char key, int x, int y)
	{
		helloGL->Keyboard(key, x, y);
	}

	void KeyboardUp(unsigned char key, int x, int y)
	{
		helloGL->KeyboardUp(key, x, y);
	}

	void Special(int key, int x, int y)
	{
		helloGL->Special(key, x, y);
	}

	void SpecialUp(int key, int x, int y)
	{
		helloGL->SpecialUp(key, x, y);
	}

	void Mouse(int button, int state, int x, int y)
	{
		helloGL->Mouse(button, state, x, y);
	}

	void Motion(int x, int y)
	{
		helloGL->Motion(x, y);
	}

	void PassiveMotion(int x, int y)
	{
		helloGL->PassiveMotion(x, y);
	}

	void Timer(int refreshRate)
	{
		int updateTime = glutGet(GLUT_ELAPSED_TIME);
		helloGL->Update();
		updateTime = glutGet(GLUT_ELAPSED_TIME) - updateTime;
		glutTimerFunc(refreshRate - updateTime, GLUTCallbacks::Timer, refreshRate);
	}
}
