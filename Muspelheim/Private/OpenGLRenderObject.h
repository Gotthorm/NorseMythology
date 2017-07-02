#pragma once  

#include <memory>
#include "Muspelheim.h"
//#include <glm\glm.hpp>
//#include "VectorMath.h"
//#include "OpenGLText.h"

namespace Muspelheim
{
	class OpenGLSurface;
	class OpenGLShader;
	class OpenGLRenderer;

	class OpenGLRenderObject : public RenderObject
	{
	public:
		//
		OpenGLRenderObject( unsigned short id, std::weak_ptr<OpenGLSurface> surface );

		//
		~OpenGLRenderObject() = default;

		virtual bool SetShader( unsigned int shaderID ) override;
		virtual bool LoadData( unsigned int size, void* data, unsigned int objectCount ) override;
		virtual bool LoadTexture( const unsigned char* imageData, unsigned int height, unsigned int width, bool hasAlpha ) override;
		virtual bool Render() override;
		virtual void SetWindingOrderClockwise( bool clockwise ) override { m_WindingOrderClockwise = clockwise; }
		virtual void SetPolyLineMode( bool lineMode ) override { m_PolyLineMode = lineMode; }
		virtual void SetPolyBackFace( bool backFace ) override { m_PolyBackFace = backFace; }
		virtual void SetPolyMode( PolyMode polyMode ) override { m_PolyMode = polyMode; }

		//
		void Draw( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix );

		//
		static void SetRenderer( OpenGLRenderer* renderer ) { sm_Renderer = renderer; }

	private:
		bool m_WindingOrderClockwise = false;
		bool m_PolyLineMode = false;
		bool m_PolyBackFace = false;
		PolyMode m_PolyMode = TRIANGLES;

		std::weak_ptr<OpenGLSurface> m_Surface;
		OpenGLShader* m_Shader;
		unsigned short m_ID;
		unsigned int m_DataBuffer = 0;
		unsigned int m_ObjectCount = 0;
		unsigned int m_VertexArrayObject = 0;

		GLint m_UniformModelMatrix = -1;
		GLint m_UniformViewMatrix = -1;
		GLint m_UniformProjectionMatrix = -1;

		std::vector<unsigned int> m_LoadedTextures;

		static OpenGLRenderer* sm_Renderer;
	};
}



