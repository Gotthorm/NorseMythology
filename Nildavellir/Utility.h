#pragma once

namespace Utility
{
	// Given the name of an OpenGL function, it returns a function pointer
	void* GetOpenGLFunctionAddress(const char* name);
}
