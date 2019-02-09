#include <windows.h>
#include <gl\GL.h>
#include "Utility.h"

// wglGetProcAddress will not return function pointers from any OpenGL functions that are directly exported by the OpenGL32.DLL itself. 
// This means the old ones from OpenGL version 1.1. 
// Fortunately those functions can be obtained by the Win32's GetProcAddress. 
// On the other hand GetProcAddress will not work for the functions for which wglGetProcAddress works. 
// So in order to get the address of any GL function one can try with wglGetProcAddress and if it fails, try again with the Win32's GetProcAddress:

void* Utility::GetOpenGLFunctionAddress(const char* name)
{
	void* pointer = (void*)wglGetProcAddress(name);

	// Apparently the returned error codes from wglGetProcAddress can be -1, NULL, 1, 2, or 3
	if (pointer == NULL || (pointer == (void*)0x1) || (pointer == (void*)0x2) || (pointer == (void*)0x3) || (pointer == (void*)-1))
	{
		HMODULE module = LoadLibraryA("opengl32.dll");
		pointer = (void*)GetProcAddress(module, name);
	}

	return pointer;
}