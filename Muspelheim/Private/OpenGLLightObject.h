#pragma once  

#include <memory>
#include "Muspelheim.h"
//#include <glm\glm.hpp>
//#include "VectorMath.h"
//#include "OpenGLText.h"

namespace Muspelheim
{
	class OpenGLSurface;
	class OpenGLRenderer;

	class OpenGLLightObject : public LightObject
	{
	public:
		//
		OpenGLLightObject( GLenum lightID, std::weak_ptr<OpenGLSurface> surface );

		//
		~OpenGLLightObject() = default;

		virtual void SetAmbient( float r, float g, float b, float a ) override;
		virtual void SetDiffuse( float r, float g, float b, float a ) override;
		virtual void SetSpecular( float r, float g, float b, float a ) override;
		virtual void SetEmission( float r, float g, float b, float a ) override;
		virtual bool GetEnabled() override { return m_Enabled; }
		virtual void SetEnabled( bool enabled ) override { m_Enabled = enabled; }

		GLenum GetID() { return m_LightID; }

		//
		bool Apply();

	private:
		bool m_Enabled = false;
		GLenum m_LightID;
		glm::vec4 m_Ambient;
		glm::vec4 m_Diffuse;
		glm::vec4 m_Specular;
		glm::vec4 m_Emission;

		std::weak_ptr<OpenGLSurface> m_Surface;
	};
}



