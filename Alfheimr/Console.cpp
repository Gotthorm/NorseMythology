// CONSOLE.CPP

#include "Alfheimr.h"
#include "Niflheim.h"
//#include "Vanaheimr.h"
#include "Helheimr.h"
#include <sstream>
#include <memory>
#include "ConsoleParser.h"
#include "ConsoleCommandManager.h"
//#include "Platform.h"
//#include "OpenGLInterface.h"


// TODO: This will become a console variable
unsigned int cv_MinimumConsoleMessageLength = 0;

std::wstring const consoleShaderName( L"Media/Shaders/console.overlay" );
std::wstring const text2DShaderName( L"Media/Shaders/console.text2d" );

namespace Alfheimr
{
	Console::Console( const std::weak_ptr<Niflheim::MessageManager>& messageManager ) : Niflheim::MessageClient( messageManager )
	{
	}

	// TODO: Make this handle init errors
	bool Console::Initialize( std::weak_ptr<Muspelheim::Renderer> const & renderer, int width, unsigned int height, float heightPercent )
	{
		std::shared_ptr<Niflheim::MessageManager> pMessageManager = m_MessageManager.lock();

		if ( nullptr == pMessageManager )
		{
			return false;
		}

		m_Renderer = renderer;

		{
			std::shared_ptr<Muspelheim::Renderer> pRenderer = m_Renderer.lock();

			if ( nullptr == pRenderer )
			{
				return false;
			}

			// Set up the main screen
			if ( false == pRenderer->CreateSurface( m_MainScreenID ) )
			{
				pMessageManager->Post( Niflheim::Message::LOG_ERROR, L"Failed to create console render surface" );
				return false;
			}
			else
			{
				pMessageManager->Post( Niflheim::Message::LOG_INFO, L"Console surface created" );

				unsigned int shaderId = pRenderer->LoadShader( consoleShaderName );

				if ( 0 == shaderId || false == pRenderer->SetSurfaceShader( m_MainScreenID, shaderId ) )
				{
					pMessageManager->Post( Niflheim::Message::LOG_WARN, L"Failed to load shader: " + consoleShaderName );
				}

				pMessageManager->Post( Niflheim::Message::LOG_INFO, L"Shader: " + consoleShaderName + L" loaded" );

				// Set background to the color orange
				//pRenderer->SetSurfaceColor( m_MainScreenID, glm::vec4( 0.8f, 0.3f, 0.3f, 1.0f ) );

				shaderId = pRenderer->LoadShader( text2DShaderName );

				if ( 0 == shaderId || false == pRenderer->SetSurfaceTextShader( m_MainScreenID, shaderId ) )
				{
					pMessageManager->Post( Niflheim::Message::LOG_WARN, L"Failed to load shader: " + text2DShaderName );
				}

				pMessageManager->Post( Niflheim::Message::LOG_INFO, L"Shader: " + text2DShaderName + L" loaded" );
			}
		}

		//GLuint vertexShader = OpenGLInterface::LoadShader( "Media/Shaders/console.overlay.vs.glsl", GL_VERTEX_SHADER, true );
		//GLuint fragmentShader = OpenGLInterface::LoadShader( "Media/Shaders/console.overlay.fs.glsl", GL_FRAGMENT_SHADER, true );

		m_HeightPercent = heightPercent;

		// Calculate the overlay size in clip space where height(0) => 1.0 && height(1.0) => -1.0
		m_ClipSize = 1.0f - m_HeightPercent * 2.0f;

		//m_FontTextureId = Vanaheimr::KTX::load( "Media/Textures/cp437_9x16.ktx" );

		// Extract the font width and height
		//glGetTexLevelParameteriv( GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_WIDTH, &m_FontWidth );
		//glGetTexLevelParameteriv( GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_HEIGHT, &m_FontHeight );

		// Set up the window dimensions.  This call may also update the buffer size.
		SetWindowSize( width, height );

		//m_OverlayProgram = OpenGLInterface::CreateProgram();
		//OpenGLInterface::AttachShader( m_OverlayProgram, vertexShader );
		//OpenGLInterface::AttachShader( m_OverlayProgram, fragmentShader );
		//OpenGLInterface::LinkProgram( m_OverlayProgram );

		//OpenGLInterface::DeleteShader( fragmentShader );
		//OpenGLInterface::DeleteShader( vertexShader );

		// glCreateVertexArrays(1, &vao);
		//OpenGLInterface::GenVertexArrays( 1, &m_OverlayVertexArrayObjectId );
		//OpenGLInterface::BindVertexArray( m_OverlayVertexArrayObjectId );

		//vertexShader = OpenGLInterface::LoadShader( "Media/Shaders/console.text2d.vs.glsl", GL_VERTEX_SHADER, true );
		//fragmentShader = OpenGLInterface::LoadShader( "Media/Shaders/console.text2d.fs.glsl", GL_FRAGMENT_SHADER, true );

		//m_RenderTextProgram = OpenGLInterface::CreateProgram();
		//OpenGLInterface::AttachShader( m_RenderTextProgram, vertexShader );
		//OpenGLInterface::AttachShader( m_RenderTextProgram, fragmentShader );
		//OpenGLInterface::LinkProgram( m_RenderTextProgram );

		//OpenGLInterface::DeleteShader( fragmentShader );
		//OpenGLInterface::DeleteShader( vertexShader );

		//m_FontScalarLocationId = OpenGLInterface::GetUniformLocation( m_RenderTextProgram, "fontScalar" );

		//// glCreateVertexArrays(1, &vao);
		//OpenGLInterface::GenVertexArrays( 1, &m_TextVertexArrayObjectId );
		//OpenGLInterface::BindVertexArray( m_TextVertexArrayObjectId );

		//OpenGLInterface::ActiveTexture( GL_TEXTURE0 );
		//glGenTextures( 1, &m_TextBufferTextureId );
		//glBindTexture( GL_TEXTURE_2D, m_TextBufferTextureId );
		//glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

		//// This call is supposedly the equivalent of calling something like this:
		////glTexImage2D( GL_TEXTURE_2D, 0, GL_R8UI, 1024, 1024, 0, GL_RED, GL_UNSIGNED_BYTE, NULL );
		//// but I never got it working.  Some of the parameters are guess work.
		//OpenGLInterface::TexStorage2D( GL_TEXTURE_2D, 1, GL_R8UI, 1024, 1024 );

		//OpenGLInterface::ActiveTexture( GL_TEXTURE2 );
		//glGenTextures( 1, &m_TextColorBufferTextureId );
		//glBindTexture( GL_TEXTURE_1D, m_TextColorBufferTextureId );
		//glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

		//// See above comments
		//OpenGLInterface::TexStorage1D( GL_TEXTURE_1D, 1, GL_R32UI, 1024 );

		// Register for the correct messages
		pMessageManager->Register( this, Niflheim::Message::LOG_INFO );
		pMessageManager->Register( this, Niflheim::Message::LOG_WARN );
		pMessageManager->Register( this, Niflheim::Message::LOG_ERROR );

		m_ConsoleTextBuffer.reserve( Niflheim::Message::MAX_STRING_LENGTH );

		m_Parser = new ConsoleParser( pMessageManager );
		assert( m_Parser );

		ConsoleCommandManager::Create();

		//REGISTER_COMMAND2( L"set_text_size", Console::SetTextScale, ConsoleParameter::ParameterType::FLOAT, ConsoleParameter::ParameterType::FLOAT );

		//ConsoleCommandManager::GetInstance()->TestRegisterCommand( L"set_text_size", std::bind( &Console::SetTextScale_Callback, this, std::placeholders::_1 ), ConsoleParameter::ParameterType::FLOAT, ConsoleParameter::ParameterType::FLOAT );

		//ConsoleCommandManager::FartRegisterCommand( L"set_text_size", ConsoleParameter::ParameterType::FLOAT, ConsoleParameter::ParameterType::FLOAT );

		//ConsoleCommandManager::GetInstance()->RegisterCommand( L"set_text_size", std::bind( &Console::SetTextScale_Callback, this, std::placeholders::_1 ), ConsoleParameterList( 2, ConsoleParameter::ParameterType::FLOAT, ConsoleParameter::ParameterType::FLOAT ) );
		ConsoleCommandManager::GetInstance()->RegisterCommand( L"set_text_size", std::bind( &Console::SetTextScale, this, std::placeholders::_1, std::placeholders::_2 ), ConsoleParameterList( 2, ConsoleParameter::ParameterType::FLOAT, ConsoleParameter::ParameterType::FLOAT ) );

		//ConsoleCommandManager::GetInstance()->RegisterCommand( L"set_text_size", L"Some sort of description", &Console::SetTextScale_Callback );

		return true;
	}

	Console::~Console()
	{
		// Deregister the message handlers
		std::shared_ptr<Niflheim::MessageManager> pMessageManager = m_MessageManager.lock();
		if ( nullptr != pMessageManager )
		{
			pMessageManager->Deregister( this, Niflheim::Message::LOG_INFO );
			pMessageManager->Deregister( this, Niflheim::Message::LOG_WARN );
			pMessageManager->Deregister( this, Niflheim::Message::LOG_ERROR );
		}

		SetVisible( false );

		m_MessageManager.reset();

		delete m_Parser;
		m_Parser = nullptr;

		ConsoleCommandManager::Destroy();

		m_Cache.clear();

		delete[] m_ScreenTextBuffer;
		m_ScreenTextBuffer = nullptr;

		delete[] m_ScreenTextColorBuffer;
		m_ScreenTextColorBuffer = nullptr;

		m_Renderer.reset();
		//OpenGLInterface::DeleteVertexArrays( 1, &m_OverlayVertexArrayObjectId );
		//OpenGLInterface::DeleteVertexArrays( 1, &m_TextVertexArrayObjectId );
		//OpenGLInterface::DeleteProgram( m_OverlayProgram );
		//OpenGLInterface::DeleteProgram( m_RenderTextProgram );
	}

	void Console::SetCacheSize( unsigned int cacheSize )
	{
		if( cacheSize != m_CacheSize )
		{
			std::vector<CacheEntry> tempCache;

			// Make a copy of the current cache
			if( m_Cache.size() > 0 )
			{
				tempCache.swap( m_Cache );
			}

			// Store the old cache attributes for the copying of existing data
			unsigned int oldIndex = m_CacheIndex;
			unsigned int oldSize = m_CacheSize;

			// The new cache size cannot be smaller than the buffer height so clamp it here
			if( cacheSize < m_VirtualBufferHeight )
			{
				cacheSize = m_VirtualBufferHeight;
			}

			// Reinitialize the cache
			m_Cache.resize( cacheSize );
			m_CacheSize = cacheSize;
			m_CacheIndex = 0;

			m_ScrollIndex = 0;
			m_MaxScrollIndex = cacheSize - m_VirtualBufferHeight;

			// Copy the contents of the old cache into the new cache
			for( unsigned int index = 0; index < cacheSize; ++index )
			{
				// I am not sure if this is needed on the entries that will be swapped, but just in case
				// we will call this on all entries in the new cache
				m_Cache[ index ].messageString.reserve( Niflheim::Message::MAX_STRING_LENGTH );

				// Copy the existing string from the old cache
				if( index < oldSize )
				{
					// We will be reset the cache index so the index we copy from will be the oldest entry in the old cache
					m_Cache[ index ].messageString.swap( tempCache[ oldIndex ].messageString );
					m_Cache[ index ].colorValue = tempCache[ oldIndex ].colorValue;

					// Move to the next oldest entry in the old cache
					oldIndex = ( oldIndex + 1 ) % oldSize;
				}
			}
		}
	}

	void Console::SetTextScale( float widthScale, float heightScale )
	{
		m_TextScale[ 0 ] = widthScale;
		m_TextScale[ 1 ] = heightScale;

		UpdateBufferSize();
	}

	void Console::SetWindowSize( unsigned int width, unsigned int height )
	{
		m_WindowWidth = width;
		m_WindowHeight = height;

		UpdateBufferSize();
	}

	void Console::UpdateBufferSize()
	{
		std::shared_ptr<Muspelheim::Renderer> pRenderer = m_Renderer.lock();

		unsigned int fontWidth, fontHeight;

		pRenderer->GetSurfaceFontSize( m_MainScreenID, fontWidth, fontHeight );

		//pRenderer->GetSurface( m_MainScreenID );

		unsigned int newBufferWidth = m_WindowWidth / unsigned int( m_TextScale[ 0 ] * fontWidth );
		unsigned int newBufferHeight = unsigned int( m_WindowHeight * m_HeightPercent ) / unsigned int( m_TextScale[ 1 ] * fontHeight );

		// Leave one line for the console input
		--newBufferHeight;

		if( newBufferWidth != m_VirtualBufferWidth || newBufferHeight != m_VirtualBufferHeight )
		{
			std::shared_ptr<Niflheim::MessageManager> pMessageManager = m_MessageManager.lock();

			std::wstringstream stringStream;

			if ( nullptr != pMessageManager )
			{
				stringStream << L"Console virtual buffer size changed from (" << m_VirtualBufferWidth << ", " << m_VirtualBufferHeight << ") to (" << newBufferWidth << ", " << newBufferHeight << ")";
				pMessageManager->Post( Niflheim::Message::LOG_INFO, stringStream.str() );

				// Clear the string stream
				std::wstringstream().swap( stringStream );
			}

			m_VirtualBufferWidth = newBufferWidth;
			m_VirtualBufferHeight = newBufferHeight;

			// The physical buffer width must be 16 byte aligned, so we round up the buffer size to the next multiple of 16
			m_BufferWidth = ( m_VirtualBufferWidth + 0x0F ) & ~0x0F;
			m_BufferHeight = m_VirtualBufferHeight + 1;	// Add the console input line 

			delete[] m_ScreenTextBuffer;
			m_ScreenTextBuffer = new char[ m_BufferWidth * m_BufferHeight ];

			delete[] m_ScreenTextColorBuffer;
			m_ScreenTextColorBuffer = new unsigned int[ m_BufferHeight ];

			if ( nullptr != pMessageManager )
			{
				stringStream << L"Console physical buffer size changed to (" << m_BufferWidth << ", " << m_BufferHeight << ")";
				pMessageManager->Post( Niflheim::Message::LOG_INFO, stringStream.str() );
			}

			// To enforce our rule that the cache size is at least as large as the screen
			if( m_CacheSize < m_VirtualBufferHeight )
			{
				SetCacheSize( m_VirtualBufferHeight );
			}
		}
	}

	void Console::Render()
	{
		if( m_IsVisible )
		{
			std::shared_ptr<Muspelheim::Renderer> pRenderer = m_Renderer.lock();

			if ( nullptr != pRenderer )
			{
				//pRenderer->BeginRender( glm::mat4() );

				// Render the current FPS
				wchar_t stringBuffer[ Platform::kMaxStringLength ];
				std::swprintf( stringBuffer, Platform::kMaxStringLength, L"Some console text" );
				pRenderer->DrawSurfaceString( m_MainScreenID, stringBuffer, 0, 0, Muspelheim::Renderer::TEXT_LEFT );

				//std::swprintf( stringBuffer, Platform::kMaxStringLength, L"VSync: %s", ( vsync ? L"enabled" : L"disabled" ) );
				//m_Renderer->DrawSurfaceString( m_MainScreenID, stringBuffer, 40, 0, Muspelheim::Renderer::TEXT_RIGHT );

				//glm::vec3 const & cameraForward = viewMatrix[ 2 ];
				//std::swprintf( stringBuffer, Platform::kMaxStringLength, L"Camera Direction: %.3f, %.3f, %.3f", cameraForward.x, cameraForward.y, cameraForward.z );
				//m_Renderer->DrawSurfaceString( m_MainScreenID, stringBuffer, 40, 1, Muspelheim::Renderer::TEXT_RIGHT );

				//glm::vec3 const & cameraPosition = pCurrentCamera->GetPosition();
				//std::swprintf( stringBuffer, Platform::kMaxStringLength, L"Camera Position: %.1f, %.1f, %.1f", cameraPosition.x, cameraPosition.y, cameraPosition.z );
				//m_Renderer->DrawSurfaceString( m_MainScreenID, stringBuffer, 40, 2, Muspelheim::Renderer::TEXT_RIGHT );

				//pRenderer->EndRender();
			}

			// Render the semi transparent overlay
			//OpenGLInterface::UseProgram( m_OverlayProgram );
			//OpenGLInterface::VertexAttrib1f( 1, m_ClipSize );
			//glEnable( GL_BLEND );
			//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			//glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
			//glDisable( GL_BLEND );

			//// Render the text
			//OpenGLInterface::UseProgram( m_RenderTextProgram );
			//OpenGLInterface::VertexAttrib1f( 1, m_ClipSize );
			////OpenGLInterface::VertexAttrib4fv( 2, m_TextScale );

			//OpenGLInterface::Uniform2fv( m_FontScalarLocationId, 1, m_TextScale );
			//GLenum error = glGetError();

			//// Update the screen buffer with latest version of the cache data
			//CopyCacheToRenderBuffer();

			//OpenGLInterface::ActiveTexture( GL_TEXTURE0 );

			//glBindTexture( GL_TEXTURE_2D, m_TextBufferTextureId );

			//glTexSubImage2D(
			//	GL_TEXTURE_2D,			// Target to which the texture is bound
			//	0,						// Level of detail number, 0 is base
			//	0,						// Specifies a texel offset in the x direction within the texture array
			//	0,						// Specifies a texel offset in the y direction within the texture array
			//	m_BufferWidth,			// Specifies the width of the texture subimage
			//	m_BufferHeight,			// Specifies the height of the texture subimage
			//	GL_RED_INTEGER,			// Specifies the format of the pixel data.  GL_RED_INTEGER tells GL to keep the exact integer value rather than normalizing
			//	GL_UNSIGNED_BYTE,		// Specifies the data type of the pixel data
			//	m_ScreenTextBuffer		// Specifies a pointer to the image data in memory
			//);

			//OpenGLInterface::ActiveTexture( GL_TEXTURE2 );

			//glBindTexture( GL_TEXTURE_1D, m_TextColorBufferTextureId );

			//glTexSubImage1D(
			//	GL_TEXTURE_1D,			// Target to which the texture is bound
			//	0,						// Level of detail number, 0 is base
			//	0,						// Specifies a texel offset in the x direction within the texture array
			//	m_BufferHeight,			// Specifies the width of the texture subimage
			//	GL_RED_INTEGER,			// Specifies the format of the pixel data.  GL_RED_INTEGER tells GL to keep the exact integer value rather than normalizing
			//	GL_UNSIGNED_INT,		// Specifies the data type of the pixel data
			//	m_ScreenTextColorBuffer	// Specifies a pointer to the image data in memory
			//);

			//OpenGLInterface::ActiveTexture( GL_TEXTURE1 );

			//OpenGLInterface::BindTexture( GL_TEXTURE_2D_ARRAY, m_FontTextureId );

			//OpenGLInterface::BindVertexArray( m_TextVertexArrayObjectId );

			//// Render four vertices in a triangle strip format, which equates to a square composed of two triangles
			//glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
		}
	}

	std::string ConvertWideCharToChar( const std::wstring& source )
	{
		size_t sourceLength = source.length();

		std::string results;
		results.reserve( sourceLength );

		for( unsigned int index = 0; index < source.length(); ++index )
		{
			wchar_t code = source[ index ];

			if( code == '\0' )
				break;

			if( code < 128 )
			{
				results += char( code );
			}
			else
			{
				results += '?';
				if( code >= 0xD800 && code <= 0xD8FF )
				{
					// lead surrogate, skip the next code unit, which is the trail
					++index;
				}
			}
		}

		return results;
	}

	void Console::CopyCacheToRenderBuffer()
	{
		if( m_Dirty )
		{
			// Fill the render buffer with the latest version of the cache

			// Clear the buffers
			memset( m_ScreenTextBuffer, 0, m_BufferWidth * m_BufferHeight * sizeof( char ) );
			memset( m_ScreenTextColorBuffer, 0xFF, m_BufferHeight * sizeof( unsigned int ) );

			unsigned int cacheIndex = m_CacheIndex;

			//
			unsigned int startIndex = m_ScrollIndex;
			if( m_ScrollIndex > cacheIndex )
			{
				cacheIndex = m_CacheSize - ( m_ScrollIndex - cacheIndex );
			}
			else
			{
				cacheIndex -= m_ScrollIndex;
			}

			for( int index = ( m_VirtualBufferHeight - 1 ); index >= 0; --index )
			{
				// Determine cache line index
				if( cacheIndex == 0 )
				{
					cacheIndex = m_CacheSize;
				}
				--cacheIndex;

				// Convert cached line from wchar to char text and copy into the render buffer
				std::string convertedString = ConvertWideCharToChar( m_Cache[ cacheIndex ].messageString );

				// For debugging the console window dimensions, etc
				// This will pad non empty lines to a minimum length
				if( cv_MinimumConsoleMessageLength > 0 )
				{
					size_t currentLength = convertedString.length();

					// We do not want to pad empty entries in the buffer
					if( currentLength > 0 )
					{
						while( currentLength < cv_MinimumConsoleMessageLength )
						{
							convertedString += ( '0' + currentLength % 10 );
							++currentLength;
						}
					}
				}

				// Handle wrapped lines
				size_t stringLength = convertedString.length();
				char* dst = m_ScreenTextBuffer + index * m_BufferWidth;
				while( stringLength > m_VirtualBufferWidth )
				{
					size_t subIndex = ( ( stringLength - 1 ) / m_VirtualBufferWidth ) * m_VirtualBufferWidth;
					int subLength = stringLength % m_VirtualBufferWidth;
					if( subLength == 0 )
					{
						subLength = m_VirtualBufferWidth;
					}
					memcpy( dst, convertedString.substr( subIndex ).c_str(), subLength );
					m_ScreenTextColorBuffer[ index ] = m_Cache[ cacheIndex ].colorValue;
					stringLength -= subLength;
					if( --index < 0 )
					{
						return;
					}
					dst = m_ScreenTextBuffer + index * m_BufferWidth;
				}

				memcpy( dst, convertedString.c_str(), stringLength );
				m_ScreenTextColorBuffer[ index ] = m_Cache[ cacheIndex ].colorValue;
			}

			std::string convertedString = ConvertWideCharToChar( m_ConsoleTextBuffer );
			memcpy( m_ScreenTextBuffer + m_VirtualBufferHeight * m_BufferWidth, convertedString.c_str(), convertedString.length() );
			m_ScreenTextColorBuffer[ m_VirtualBufferHeight ] = m_ColorTable[ 3 ];
		}
	}

	void Console::ReceiveMessage( const Niflheim::Message& message )
	{
		if( message.Type == Niflheim::Message::KEY_STROKES )
		{
			ProcessKeystroke( message.uintData );
		}
		else
		{
			ProcessLogMessage( message.Type, message.stringData );
		}
	}

	void Console::Update( float timeElapsed )
	{
		// When active we will process user input for the window
		if( m_IsVisible )
		{
			if( m_ScrollIndex < m_MaxScrollIndex && Helheimr::Input::GetInstance()->GetKeyUp( Helheimr::Input::KEY_ARROW_UP ) )
			{
				if( ++m_ScrollIndex > m_MaxScrollIndex )
				{
					m_ScrollIndex = m_MaxScrollIndex;
				}
			}
			if( m_ScrollIndex > 0 && Helheimr::Input::GetInstance()->GetKeyUp( Helheimr::Input::KEY_ARROW_DOWN ) )
			{
				--m_ScrollIndex;
			}
			if( m_ScrollIndex > 0 && Helheimr::Input::GetInstance()->GetKeyUp( Helheimr::Input::KEY_END ) )
			{
				m_ScrollIndex = 0;
			}
			// TODO: Broken
			//if( Input::GetInstance()->GetKeyUp( Input::KEY_HOME ) )
			//{
			//	m_ScrollIndex = m_MaxScrollIndex;
			//}
		}
	}

	void Console::SetVisible( bool visible )
	{
		if( m_IsVisible != visible )
		{
			m_IsVisible = visible;

			std::shared_ptr<Niflheim::MessageManager> pMessageManager = m_MessageManager.lock();

			if ( nullptr != pMessageManager )
			{
				if ( visible )
				{
					m_ScrollIndex = 0;

					pMessageManager->Register( this, Niflheim::Message::KEY_STROKES );
				}
				else
				{
					pMessageManager->Deregister( this, Niflheim::Message::KEY_STROKES );
				}
			}
		}
	}

	void Console::ProcessKeystroke( unsigned int keyStroke )
	{
		unsigned short keyValue = ( Helheimr::Input::MASK_KEY_VALUE & keyStroke );

		bool shifted = ( Helheimr::Input::MODIFIER_KEY_SHIFT & keyStroke ) > 0;

		wchar_t character = 0;
		if( keyValue >= Helheimr::Input::KEY_A && keyValue <= Helheimr::Input::KEY_Z )
		{
			character = ( shifted ? L'A' : L'a' ) + ( keyValue - Helheimr::Input::KEY_A );
		}
		else if( keyValue >= Helheimr::Input::KEY_0 && keyValue <= Helheimr::Input::KEY_9 )
		{
			if( shifted )
			{
				switch( keyValue )
				{
				case Helheimr::Input::KEY_1:
					character = L'!';
					break;
				case Helheimr::Input::KEY_2:
					character = L'@';
					break;
				case Helheimr::Input::KEY_3:
					character = L'#';
					break;
				case Helheimr::Input::KEY_4:
					character = L'$';
					break;
				case Helheimr::Input::KEY_5:
					character = L'%';
					break;
				case Helheimr::Input::KEY_6:
					character = L'^';
					break;
				case Helheimr::Input::KEY_7:
					character = L'&';
					break;
				case Helheimr::Input::KEY_8:
					character = L'*';
					break;
				case Helheimr::Input::KEY_9:
					character = L'(';
					break;
				case Helheimr::Input::KEY_0:
					character = L')';
					break;
				}
			}
			else
			{
				character = L'0' + ( keyValue - Helheimr::Input::KEY_0 );
			}
		}
		else if( keyValue >= Helheimr::Input::KEY_NUMPAD_0 && keyValue <= Helheimr::Input::KEY_NUMPAD_9 )
		{
			character = L'0' + ( keyValue - Helheimr::Input::KEY_NUMPAD_0 );
		}
		else if( keyValue == Helheimr::Input::KEY_SPACE )
		{
			character = L' ';
		}
		else if( keyValue == Helheimr::Input::KEY_BACKSPACE )
		{
			if( m_ConsoleTextBuffer.length() > 0 )
			{
				m_ConsoleTextBuffer.pop_back();
			}
		}
		else if( keyValue == Helheimr::Input::KEY_SEMICOLON )
		{
			character = shifted ? L':' : L';';
		}
		else if( keyValue == Helheimr::Input::KEY_EQUALS )
		{
			character = shifted ? L'+' : L'=';
		}
		else if( keyValue == Helheimr::Input::KEY_COMMA )
		{
			character = shifted ? L'<' : L',';
		}
		else if( keyValue == Helheimr::Input::KEY_MINUS )
		{
			character = shifted ? L'_' : L'-';
		}
		else if( keyValue == Helheimr::Input::KEY_PERIOD )
		{
			character = shifted ? L'>' : L'.';
		}
		else if( keyValue == Helheimr::Input::KEY_FORWARD_SLASH )
		{
			character = shifted ? L'?' : L'/';
		}
		else if( keyValue == Helheimr::Input::KEY_SINGLE_QUOTE )
		{
			character = shifted ? L'"' : L'\'';
		}

		if( character != 0 )
		{
			m_ConsoleTextBuffer += character;
		}
		else if( keyValue == Helheimr::Input::KEY_RETURN )
		{
			// Send the command line string to the parser
			m_Parser->Execute( m_ConsoleTextBuffer );

			// Clear the string
			m_ConsoleTextBuffer.clear();
		}
	}

	void Console::ProcessLogMessage( Niflheim::Message::MessageType type, std::wstring* logMessage )
	{
		unsigned int colorValue;

		switch( type )
		{
		case Niflheim::Message::LOG_INFO:
			colorValue = m_ColorTable[ 0 ]; // White
			break;
		case Niflheim::Message::LOG_WARN:
			colorValue = m_ColorTable[ 1 ]; // Yellow
			break;
		case Niflheim::Message::LOG_ERROR:
			colorValue = m_ColorTable[ 2 ]; // Red
			break;
		default:
			// Invalid message type?
			return;
		}

		assert( logMessage );

		// Write to the cache string entry
		m_Cache[ m_CacheIndex ].messageString.assign( *logMessage, 0, Niflheim::Message::MAX_STRING_LENGTH );

		// Set cache entry color?
		m_Cache[ m_CacheIndex ].colorValue = colorValue;

		// Update the index to the next available line in the cache
		m_CacheIndex = ( m_CacheIndex + 1 ) % m_CacheSize;

		// If a scroll is in progress, we adjust it so that the text doesnt appear to scroll
		if( m_ScrollIndex > 0 )
		{
			m_ScrollIndex = ( m_ScrollIndex + 1 ) % m_CacheSize;
		}

		m_Dirty = true;
	}

	void Console::SetTextScale_Callback( const ConsoleParameterList& paramList )
	{
		if( paramList.GetCount() == 2 )
		{
			if( paramList.GetParameterType( 0 ) == ConsoleParameter::ParameterType::FLOAT && paramList.GetParameterType( 1 ) == ConsoleParameter::ParameterType::FLOAT )
			{
				TypedConsoleParameter<float>* param1 = dynamic_cast<TypedConsoleParameter<float>*>( paramList.GetParameterValue( 0 ) );
				TypedConsoleParameter<float>* param2 = dynamic_cast<TypedConsoleParameter<float>*>( paramList.GetParameterValue( 1 ) );
				assert( param1 != nullptr && param2 != nullptr );

				if( param1 != nullptr && param2 != nullptr )
				{
					float widthScale = param1->GetData();
					float heightScale = param2->GetData();

					SetTextScale( widthScale, heightScale );
				}
			}
		}
	}
}