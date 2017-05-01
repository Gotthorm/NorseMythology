// Renderer.CPP

#include "Muspelheim.h"
#include "OpenGLRenderer.h"

#pragma comment(lib, "Helheimr.lib")

namespace Muspelheim
{
	Renderer* Renderer::Create()
	{
		return new OpenGLRenderer();
	}

	bool Renderer::Destroy( Renderer*& renderer )
	{
		return false;
	}
}