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

		virtual bool SetAmbient( float r, float g, float b, float a ) override;
		virtual bool SetDiffuse( float r, float g, float b, float a ) override;
		virtual bool SetSpecular( float r, float g, float b, float a ) override;
		virtual bool SetEmission( float r, float g, float b, float a ) override;
		virtual bool GetEnabled() override { return m_Enabled; }
		virtual void SetEnabled( bool enabled ) override { m_Enabled = enabled; }

	private:
		bool m_Enabled = false;
		GLenum m_LightID;
		std::weak_ptr<OpenGLSurface> m_Surface;
	};
}



