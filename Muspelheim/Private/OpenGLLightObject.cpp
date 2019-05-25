// OpenGLLightObject.CPP

#include "OpenGLRenderer.h"
#include "OpenGLLightObject.h"
#include "OpenGLInterface.h"
#include "OpenGLSurface.h"

namespace Muspelheim
{
	OpenGLLightObject::OpenGLLightObject( GLenum lightID, std::weak_ptr<OpenGLSurface> surface )
		: m_LightID( lightID )
		, m_Surface( surface )
	{
	}

	bool OpenGLLightObject::SetAmbient( float r, float g, float b, float a )
	{
		return false;
	}

	//
	bool OpenGLLightObject::SetDiffuse( float r, float g, float b, float a )
	{
		return false;
	}

	//
	bool OpenGLLightObject::SetSpecular( float r, float g, float b, float a )
	{
		return false;
	}

	//
	bool OpenGLLightObject::SetEmission( float r, float g, float b, float a )
	{
		return false;
	}
}