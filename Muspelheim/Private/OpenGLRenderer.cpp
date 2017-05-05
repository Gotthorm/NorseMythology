// OpenGLRenderer.CPP

#include "OpenGLRenderer.h"
#include "OpenGLInterface.h"
#include "OpenGLShader.h"
#include <sstream>
#include <codecvt>
#include "Niflheim.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "Niflheim.lib")
#pragma comment(lib, "Vanaheimr.lib")

namespace Muspelheim
{
	//
	bool OpenGLRenderer::Initialize( const Platform::WindowHandle& hWindow )
	{
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof( PIXELFORMATDESCRIPTOR ),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
			PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
			32,                        //Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                        //Number of bits for the depthbuffer
			8,                        //Number of bits for the stencilbuffer
			0,                        //Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		m_WindowHandleToDeviceContext = GetDC( hWindow );

		if( m_WindowHandleToDeviceContext != NULL )
		{
			int letWindowsChooseThisPixelFormat = ChoosePixelFormat( m_WindowHandleToDeviceContext, &pfd );

			if( letWindowsChooseThisPixelFormat != 0 )
			{
				if( SetPixelFormat( m_WindowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd ) == TRUE )
				{
					HGLRC ourOpenGLRenderingContext = wglCreateContext( m_WindowHandleToDeviceContext );

					if( ourOpenGLRenderingContext != NULL )
					{
						if( wglMakeCurrent( m_WindowHandleToDeviceContext, ourOpenGLRenderingContext ) == TRUE )
						{
							return InitializeSubSystems();
						}
					}
				}
			}
		}

		return false;
	}

	//
	void OpenGLRenderer::Shutdown()
	{
		for( std::shared_ptr<OpenGLSurface> surface : m_Surfaces )
		{
			surface.reset();
		}
		m_Surfaces.clear();

		// All shader clients should be destroyed at this point
		for( OpenGLShader* shader : m_Shaders )
		{
			delete shader;
		}
		m_Shaders.clear();

		OpenGLRenderObject::SetRenderer( nullptr );
	}

	//
	void OpenGLRenderer::BeginRender( const glm::mat4& viewMatrix )
	{
		static const GLfloat one = 1.0f;

		m_ViewMatrix = viewMatrix;

		// Default background color
		VectorMath::vec4 backgroundColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		OpenGLInterface::ClearBufferfv( GL_COLOR, 0, backgroundColor );
		OpenGLInterface::ClearBufferfv( GL_DEPTH, 0, &one );

		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LEQUAL );
		//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		glViewport( 0, 0, m_Width, m_Height );

		//static inline mat4 perspective( float fovy, float aspect, float n, float f )
		m_ProjectionMatrix = glm::perspective( 45.0f, (float)m_Width / (float)m_Height, 0.1f, 10000.0f );
	}

	//
	void OpenGLRenderer::EndRender()
	{
		// Iterate through the surfaces in current depth order
		for( std::shared_ptr<OpenGLSurface> surface : m_Surfaces )
		{
			surface->RenderAll( m_ViewMatrix, m_ProjectionMatrix );
		}

		SwapBuffers( m_WindowHandleToDeviceContext );
	}

	std::wstring OpenGLRenderer::GetVersionInformation()
	{
		// Determine what version of OpenGL is initialized
		const char* glVersion = (const char*)glGetString( GL_VERSION );
		CHAR tempString[ 256 ];
		sprintf_s( tempString, 256, "OpenGL information: %s", glVersion );

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring versionString = converter.from_bytes( tempString );

		return versionString;
	}

	bool OpenGLRenderer::InitializeSubSystems()
	{
		// Initialize the OpenGL functions we will be using
		// This must be called after we make our OpenGL rendering context above
		OpenGLInterface::Initialize();

		RECT rect;
		HWND window = WindowFromDC( m_WindowHandleToDeviceContext );

		if( GetClientRect( window, &rect ) == FALSE )
		{
			std::shared_ptr<Niflheim::MessageManager> messageManager = m_MessageManager.lock();
			if( messageManager )
			{
				messageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Failed to extract the initial window size" ) );
			}
		}

		unsigned int width = rect.right - rect.left;
		unsigned int height = rect.bottom - rect.top;

		//if( m_Console.Initialize( width, height, 0.75f ) == false )
		//{
		//	MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Failed to initialize the console system" ) );
		//}
		//else
		//{
		//	// Set the console cache size to the number of lines needed in the log history
		//	// TODO: Make this a console variable
		//	m_Console.SetCacheSize( 128 );
		//}

		SetWindowSize( width, height );

		// Initialize the 2D text system
		//m_Text2D.init( 128, 50 );

		// TODO: What is this for?
		OpenGLInterface::GenVertexArrays( 1, &m_VertexArrayObject );
		OpenGLInterface::BindVertexArray( m_VertexArrayObject );

		OpenGLRenderObject::SetRenderer( this );

		// Determine the max number of lights if not using shaders
		glGetIntegerv( GL_MAX_LIGHTS, &m_MaxLights );
		PLATFORM_ASSERT( m_MaxLights > 0 );

		for( int lightIndex = 0; lightIndex < m_MaxLights; ++lightIndex )
		{
			m_Lights.push_back( std::make_pair<GLenum, LightPointer>( GL_LIGHT0 + lightIndex, nullptr ) );
		}

		// Ensure lighting is enabled
		glEnable( GL_LIGHTING );

		// Set the global ambient light level
		glLightModelfv( GL_LIGHT_MODEL_AMBIENT, glm::value_ptr( glm::vec4( 0.2, 0.2, 0.2, 1 ) ) );

		return true;
	}

	void OpenGLRenderer::SetWindowSize( unsigned short width, unsigned short height )
	{
		m_Width = width;
		m_Height = height;

		// TODO: Notify all surfaces

		std::wstringstream s;
		s << L"SetWindowSize( " << width << L", " << height << L" )";
		std::shared_ptr<Niflheim::MessageManager> messageManager = m_MessageManager.lock();
		if( messageManager )
		{
			messageManager->Post( Niflheim::Message::LOG_INFO, s.str() );
		}
	}

	void OpenGLRenderer::AddMessaging( const std::weak_ptr<Niflheim::MessageManager>& messageManager )
	{
		m_MessageManager = messageManager;
	}

	unsigned int OpenGLRenderer::LoadShader( const std::wstring& shaderName )
	{
		OpenGLShader* shader = new OpenGLShader();

		if( shader->Load( shaderName ) )
		{
			m_Shaders.push_back( shader );

			return (unsigned int)(m_Shaders.size());
		}

		return 0;
	}

	unsigned int OpenGLRenderer::LoadTexture( const unsigned char* imageData, unsigned int height, unsigned int width, bool hasAlpha )
	{
		unsigned int textureID = 0;

		if( imageData )
		{
			glGenTextures( 1, &textureID );

			glBindTexture( GL_TEXTURE_2D, textureID );

			glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

			GLint format = hasAlpha ? GL_RGBA : GL_RGB;

			glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData );

			OpenGLInterface::GenerateMipmap( GL_TEXTURE_2D );
		}

		return textureID;
	}

	OpenGLShader* OpenGLRenderer::GetShader( unsigned int shaderID )
	{
		if( shaderID > 0 && shaderID <= m_Shaders.size() )
		{
			return m_Shaders[ shaderID - 1 ];
		}

		return nullptr;
	}

	//
	bool OpenGLRenderer::CreateSurface( SurfaceID& surfaceID )
	{
		std::shared_ptr<OpenGLSurface> newSurface = std::make_shared<OpenGLSurface>();

		if( newSurface )
		{
			if( newSurface->Init( newSurface, m_Width, m_Height ) )
			{
				m_Surfaces.push_back( newSurface );

				PLATFORM_ASSERT( m_Surfaces.size() < UCHAR_MAX );
				surfaceID = static_cast<SurfaceID>(m_Surfaces.size());

				return true;
			}
		}

		std::shared_ptr<Niflheim::MessageManager> messageManager = m_MessageManager.lock();
		if( messageManager )
		{
			messageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Failed to create new surface" ) );
		}

		return false;
	}

	//
	bool OpenGLRenderer::SetSurfaceDepth( SurfaceID surfaceID, unsigned char depth )
	{
		if( surfaceID > 0 && m_Surfaces.size() >= surfaceID )
		{
			m_Surfaces[ surfaceID - 1 ]->SetDepth( depth );

			return true;
		}

		return false;
	}

	// 
	bool OpenGLRenderer::GetSurfaceDepth( SurfaceID surfaceID, unsigned char& depth )
	{
		if( surfaceID > 0 && m_Surfaces.size() >= surfaceID )
		{
			depth = m_Surfaces[ surfaceID - 1 ]->GetDepth();

			return true;
		}

		return false;
	}

	//
	bool OpenGLRenderer::GetSurfaceColor( SurfaceID surfaceID, glm::vec4& color )
	{
		if( surfaceID > 0 && m_Surfaces.size() >= surfaceID )
		{
			color = m_Surfaces[ surfaceID - 1 ]->GetColor();

			return true;
		}

		return false;
	}

	//
	bool OpenGLRenderer::SetSurfaceColor( SurfaceID surfaceID, const glm::vec4& color )
	{
		if( surfaceID > 0 && m_Surfaces.size() >= surfaceID )
		{
			m_Surfaces[ surfaceID - 1 ]->SetColor( color );

			return true;
		}

		return false;
	}

	//
	bool OpenGLRenderer::SetSurfaceShader( SurfaceID surfaceID, unsigned int shaderID )
	{
		if( surfaceID > 0 && m_Surfaces.size() >= surfaceID )
		{
			if( shaderID > 0 && shaderID <= m_Shaders.size() )
			{
				m_Surfaces[ surfaceID - 1 ]->SetShader( m_Shaders[ shaderID - 1 ] );

				return true;
			}
		}

		return false;
	}

	//
	bool OpenGLRenderer::SetSurfaceTextShader( SurfaceID surfaceID, unsigned int shaderID )
	{
		if( surfaceID > 0 && m_Surfaces.size() >= surfaceID )
		{
			if( shaderID > 0 && shaderID <= m_Shaders.size() )
			{
				m_Surfaces[ surfaceID - 1 ]->SetTextShader( m_Shaders[ shaderID - 1 ] );

				return true;
			}
		}

		return false;
	}
	//
	bool OpenGLRenderer::DrawSurfaceString( SurfaceID surfaceID, const std::wstring& textString, unsigned short posX, unsigned short posY, TextAlignment alignment )
	{
		if( surfaceID > 0 && m_Surfaces.size() >= surfaceID )
		{
			return m_Surfaces[ surfaceID - 1 ]->DrawString( textString, posX, posY, alignment );
		}

		return false;
	}

	//
	std::shared_ptr<RenderObject> OpenGLRenderer::CreateSurfaceRenderObject( SurfaceID surfaceID )
	{
		if( surfaceID > 0 && m_Surfaces.size() >= surfaceID )
		{
			return m_Surfaces[ surfaceID - 1 ]->CreateRenderObject();
		}

		return nullptr;
	}

	std::shared_ptr<LightObject> OpenGLRenderer::CreateSurfaceLightObject( SurfaceID surfaceID )
	{
		if( surfaceID > 0 && m_Surfaces.size() >= surfaceID )
		{
			// Ensure that we have not already reached the maximum number of lights
			for( int lightIndex = 0; lightIndex < m_MaxLights; ++lightIndex )
			{
				if( m_Lights[ lightIndex ].second == nullptr )
				{
					m_Lights[ lightIndex ].second = std::make_shared<OpenGLLightObject>( m_Lights[ lightIndex ].first, m_Surfaces[ surfaceID - 1 ] );

					m_Surfaces[ surfaceID - 1 ]->AddLightObject( m_Lights[ lightIndex ].second );

					return m_Lights[ lightIndex ].second;
				}
			}
		}

		return nullptr;
	}
}