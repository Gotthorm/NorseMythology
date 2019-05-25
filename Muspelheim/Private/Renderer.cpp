// Renderer.CPP

#include "OpenGLRenderer.h"

#pragma comment(lib, "Helheimr.lib")

namespace Muspelheim
{
	std::shared_ptr<Renderer> Renderer::Create()
	{
		return std::make_shared<OpenGLRenderer>();
	}
}