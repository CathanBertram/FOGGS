#include "GLUTCallbacks.h"
#include "OpenGL.h"

namespace GLUTCallbacks
{
	namespace
	{
		OpenGL* openGL = nullptr;
	}
	void Init(OpenGL* gl)
	{
		openGL = gl;
	}
	void Display()
	{
		if (openGL != nullptr)
		{
			openGL->Display();
		}
	}
}