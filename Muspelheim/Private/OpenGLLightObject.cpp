// OpenGLLightObject.CPP

#include "Muspelheim.h"
#include "OpenGLRenderer.h"
#include "OpenGLLightObject.h"
#include "OpenGLInterface.h"
#include "OpenGLSurface.h"
#include <glm/gtc/type_ptr.hpp>

namespace Muspelheim
{
	OpenGLLightObject::OpenGLLightObject( GLenum lightID, std::weak_ptr<OpenGLSurface> surface )
		: m_LightID( lightID )
		, m_Surface( surface )
		, m_Ambient( 0.0f )
		, m_Diffuse( 0.0f )
		, m_Specular( 0.0f )
		, m_Emission( 0.0f )
	{
	}

	void OpenGLLightObject::SetAmbient( float r, float g, float b, float a )
	{
		m_Ambient = glm::vec4( r, g, b, a );
	}

	//
	void OpenGLLightObject::SetDiffuse( float r, float g, float b, float a )
	{
		m_Diffuse = glm::vec4( r, g, b, a );
	}

	//
	void OpenGLLightObject::SetSpecular( float r, float g, float b, float a )
	{
		m_Specular = glm::vec4( r, g, b, a );
	}

	//
	void OpenGLLightObject::SetEmission( float r, float g, float b, float a )
	{
		m_Emission = glm::vec4( r, g, b, a );
	}

	//
	bool OpenGLLightObject::Apply()
	{
		if( m_Enabled )
		{
			glEnable( GetID() );

			glLightfv( GetID(), GL_AMBIENT, glm::value_ptr( m_Ambient ) );
			glLightfv( GetID(), GL_DIFFUSE, glm::value_ptr( m_Diffuse ) );
			glLightfv( GetID(), GL_SPECULAR, glm::value_ptr( m_Specular ) );
			glLightfv( GetID(), GL_EMISSION, glm::value_ptr( m_Emission ) );
			glLightfv( GetID(), GL_POSITION, glm::value_ptr( GetPosition() ) );
		}
		else
		{
			glDisable( GetID() );
		}

		return m_Enabled;
	}
}