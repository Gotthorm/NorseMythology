#pragma once  

#include <Windows.h>
#include <glm\glm.hpp>
#include "Muspelheim.h"
#include "OpenGLSurface.h"
#include "OpenGLRenderObject.h"
#include "OpenGLLightObject.h"
#include <gl\GL.h>
#include <vector>

namespace Muspelheim
{
	class Surface;
	class OpenGLShader;

	class OpenGLRenderer : public Renderer
	{
	public:
		//
		OpenGLRenderer() = default;

		//
		virtual ~OpenGLRenderer();

		//
		virtual bool Initialize( Platform::WindowHandle const & hWindow ) override;

		//
		virtual void BeginRender( glm::mat4 const & viewMatrix ) override;

		//
		virtual void EndRender() override;

		//
		virtual void SetWindowSize( unsigned short width, unsigned short height ) override;

		//
		virtual std::wstring GetVersionInformation() override;

		//
		virtual void AddMessaging( std::weak_ptr<Niflheim::MessageManager> const & messageManager ) override;

		//
		virtual unsigned int LoadShader( std::wstring const & shaderName ) override;

		//
		virtual unsigned int LoadTexture( unsigned char const * imageData, unsigned int height, unsigned int width, bool hasAlpha ) override;

		//
		virtual bool GetVSyncEnabled() override;

		//
		virtual void SetVSyncEnabled( bool enable ) override;

		//
		OpenGLShader* GetShader( unsigned int shaderID );

		// ########## Surfaces ##########

		//
		virtual bool CreateSurface( SurfaceID& surfaceID ) override;

		//
		virtual bool DestroySurface( SurfaceID surfaceID ) override;

		// 
		virtual bool GetSurfaceDepth( SurfaceID surfaceID, unsigned char& depth ) override;

		//
		virtual bool SetSurfaceDepth( SurfaceID surfaceID, unsigned char depth ) override;

		//
		virtual bool GetSurfaceColor( SurfaceID surfaceID, glm::vec4& color ) override;

		//
		virtual bool SetSurfaceColor( SurfaceID surfaceID, glm::vec4 const & color ) override;

		//
		virtual bool GetSurfaceVisible( SurfaceID surfaceID, bool& visible ) override;

		//
		virtual bool SetSurfaceVisible( SurfaceID surfaceID, bool visible ) override;

		//
		virtual bool DrawSurfaceString( SurfaceID surfaceID, std::wstring const & textString, unsigned int color, unsigned short posX, unsigned short posY, TextAlignment alignment ) override;

		//
		virtual bool DrawSurfaceStringBuffer( SurfaceID surfaceID, TextBuffer const & textBuffer ) override;

		//
		virtual std::shared_ptr<RenderObject> CreateSurfaceRenderObject( SurfaceID surfaceID ) override;

		//
		virtual std::shared_ptr<LightObject> CreateSurfaceLightObject( SurfaceID surfaceID ) override;

		//
		virtual bool SetSurfaceShader( SurfaceID surfaceID, unsigned int shaderID ) override;

		//
		virtual bool SetSurfaceTextShader( SurfaceID surfaceID, unsigned int shaderID ) override;

		//
		virtual bool GetSurfaceFontSize( SurfaceID surfaceID, unsigned int & width, unsigned int & height ) override;

		//
		virtual bool SetSurfaceClipping( SurfaceID surfaceID, float x, float y, float width, float height ) override;

		//
		virtual bool SetSurfaceTextScale( SurfaceID surfaceID, float widthScale, float heightScale ) override;

	private:
		//
		bool InitializeSubSystems();

		HDC m_WindowHandleToDeviceContext;
		GLuint m_VertexArrayObject;
		GLushort m_Width;
		GLushort m_Height;

		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;

		std::vector<std::shared_ptr<OpenGLSurface>> m_Surfaces;
		std::vector<OpenGLShader*> m_Shaders;
		//std::vector<std::shared_ptr<OpenGLRenderObject>> m_RenderObjects;
		std::weak_ptr<Niflheim::MessageManager> m_MessageManager;

		typedef std::shared_ptr<OpenGLLightObject> LightPointer;
		typedef std::pair<GLenum, LightPointer> LightEntry;

		GLint m_MaxLights;
		std::vector<LightEntry> m_Lights;
	};
}



