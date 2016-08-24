// CONSOLE.CPP

// TODO List
// 
// 1) Make the initialize method handle errors correctly
// 2) Figure out how to make the overlay height and the buffer height match
// 3) Change color support to be able to color each line individually

#include "Console.h"
#include "KTX.h"
#include "Logger.h"
#include "MessageManager.h"
#include <sstream>

// TODO: This will become a console variable
unsigned int cv_MinimumConsoleMessageLength = 0;

// TODO: Change this to accept %width and %height instead
// TODO: Make this handle init errors
bool Console::Initialize( unsigned int width, unsigned int height, float heightPercent )
{
	GLuint vertexShader = OpenGLInterface::LoadShader("Media/Shaders/console.overlay.vs.glsl", GL_VERTEX_SHADER, true);
	GLuint fragmentShader = OpenGLInterface::LoadShader("Media/Shaders/console.overlay.fs.glsl", GL_FRAGMENT_SHADER, true);

	m_HeightPercent = heightPercent;

	// Calculate the overlay size in clip space where height(0) => 1.0 && height(1.0) => -1.0
	m_ClipSize = 1.0f - m_HeightPercent * 2.0f;

	font_texture = KTX::load("Media/Textures/cp437_9x16.ktx");

	// Extract the font width and height
	glGetTexLevelParameteriv( GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_WIDTH, &m_FontWidth );
	glGetTexLevelParameteriv( GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_HEIGHT, &m_FontHeight );

	// Set up the window dimensions.  This call may also update the buffer size.
	SetWindowSize( width, height );

	m_OverlayProgram = OpenGLInterface::CreateProgram();
	OpenGLInterface::AttachShader(m_OverlayProgram, vertexShader);
	OpenGLInterface::AttachShader(m_OverlayProgram, fragmentShader);
	OpenGLInterface::LinkProgram(m_OverlayProgram);

	OpenGLInterface::DeleteShader(fragmentShader);
	OpenGLInterface::DeleteShader(vertexShader);

	// glCreateVertexArrays(1, &vao);
	OpenGLInterface::GenVertexArrays(1, &vao);
	OpenGLInterface::BindVertexArray(vao);

	vertexShader = OpenGLInterface::LoadShader("Media/Shaders/console.text2d.vs.glsl", GL_VERTEX_SHADER, true);
	fragmentShader = OpenGLInterface::LoadShader("Media/Shaders/console.text2d.fs.glsl", GL_FRAGMENT_SHADER, true);

	m_RenderTextProgram = OpenGLInterface::CreateProgram();
	OpenGLInterface::AttachShader(m_RenderTextProgram, vertexShader);
	OpenGLInterface::AttachShader(m_RenderTextProgram, fragmentShader);
	OpenGLInterface::LinkProgram(m_RenderTextProgram);

	OpenGLInterface::DeleteShader(fragmentShader);
	OpenGLInterface::DeleteShader(vertexShader);

	m_FontScalarLocationId = OpenGLInterface::GetUniformLocation( m_RenderTextProgram, "fontScalar" );

	// glCreateVertexArrays(1, &vao);
	OpenGLInterface::GenVertexArrays(1, &text_vao);
	OpenGLInterface::BindVertexArray(text_vao);

	OpenGLInterface::ActiveTexture( GL_TEXTURE0 );
	glGenTextures( 1, &text_buffer );
	glBindTexture( GL_TEXTURE_2D, text_buffer );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

	// This call is supposedly the equivalent of calling something like this:
	//glTexImage2D( GL_TEXTURE_2D, 0, GL_R8UI, 1024, 1024, 0, GL_RED, GL_UNSIGNED_BYTE, NULL );
	// but I never got it working.  Some of the parameters are guess work.
	OpenGLInterface::TexStorage2D( GL_TEXTURE_2D, 1, GL_R8UI, 1024, 1024 );

	OpenGLInterface::ActiveTexture( GL_TEXTURE2 );
	glGenTextures( 1, &text_color_buffer );
	glBindTexture( GL_TEXTURE_1D, text_color_buffer );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

	// See above comments
	OpenGLInterface::TexStorage1D( GL_TEXTURE_1D, 1, GL_R32UI, 1024 );

	// Register for the correct messages
	MessageManager::GetInstance()->Register( this, Message::LOG_INFO );
	MessageManager::GetInstance()->Register( this, Message::LOG_WARN );
	MessageManager::GetInstance()->Register( this, Message::LOG_ERROR );

	return true;
}
 
void Console::Shutdown()
{
	MessageManager::GetInstance()->Deregister( this, Message::LOG_INFO );
	MessageManager::GetInstance()->Deregister( this, Message::LOG_WARN );
	MessageManager::GetInstance()->Deregister( this, Message::LOG_ERROR );

	m_Cache.clear();

	delete[] screen_buffer;
	screen_buffer = nullptr;

	delete[] screen_color_buffer;
	screen_color_buffer = nullptr;

	OpenGLInterface::DeleteVertexArrays(1, &vao);
	OpenGLInterface::DeleteVertexArrays(1, &text_vao);
	OpenGLInterface::DeleteProgram(m_OverlayProgram);
	OpenGLInterface::DeleteProgram(m_RenderTextProgram);
}

void Console::SetCacheSize(unsigned int cacheSize)
{
	std::vector<CacheEntry> tempCache;

	// Make a copy of the current cache
	if (m_Cache.size() > 0)
	{
		tempCache.swap(m_Cache);
	}

	// Store the old cache attributes for the copying of existing data
	unsigned int oldIndex = m_CacheIndex;
	unsigned int oldSize = m_CacheSize;

	// The new cache size cannot be smaller than the buffer height so clamp it here
	if (cacheSize < m_BufferHeight)
	{
		cacheSize = m_BufferHeight;
	}

	// Reinitialize the cache
	m_Cache.resize(cacheSize);
	m_CacheSize = cacheSize;
	m_CacheIndex = 0;

	// Copy the contents of the old cache into the new cache
	for (unsigned int index = 0; index < cacheSize; ++index)
	{
		// I am not sure if this is needed on the entries that will be swapped, but just in case
		// we will call this on all entries in the new cache
		m_Cache[index].messageString.reserve(Message::MAX_STRING_LENGTH);

		// Copy the existing string from the old cache
		if (index < oldSize)
		{
			// We will be reset the cache index so the index we copy from will be the oldest entry in the old cache
			m_Cache[ index ].messageString.swap( tempCache[ oldIndex ].messageString );
			m_Cache[ index ].colorValue = tempCache[ oldIndex ].colorValue;

			// Move to the next oldest entry in the old cache
			oldIndex = (oldIndex + 1) % oldSize;
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
	unsigned int newBufferWidth = m_WindowWidth / unsigned int( m_TextScale[ 0 ] * m_FontWidth );
	unsigned int newBufferHeight = unsigned int( m_WindowHeight * m_HeightPercent ) / unsigned int( m_TextScale[ 1 ] * m_FontHeight );

	if( newBufferWidth != m_VirtualBufferWidth || newBufferHeight != m_VirtualBufferHeight )
	{
		std::wstringstream stringStream;
		stringStream << L"Console virtual buffer size changed from (" << m_VirtualBufferWidth << ", " << m_VirtualBufferHeight << ") to (" << newBufferWidth << ", " << newBufferHeight << ")";
		MessageManager::GetInstance()->Post( Message::LOG_INFO, stringStream.str() );
		
		// Clear the string stream
		std::wstringstream().swap( stringStream );

		m_VirtualBufferWidth = newBufferWidth;
		m_VirtualBufferHeight = newBufferHeight;

		// The physical buffer width must be 16 byte aligned, so we round up the buffer size to the next multiple of 16
		m_BufferWidth = ( m_VirtualBufferWidth + 0x0F ) & ~0x0F;
		m_BufferHeight = m_VirtualBufferHeight;

		delete[] screen_buffer;
		screen_buffer = new char[ m_BufferWidth * m_BufferHeight ];

		delete[] screen_color_buffer;
		screen_color_buffer = new unsigned int[ m_BufferHeight ];

		stringStream << L"Console physical buffer size changed to (" << m_BufferWidth << ", " << m_BufferHeight << ")";
		MessageManager::GetInstance()->Post( Message::LOG_INFO, stringStream.str() );
	}
}

void Console::Render( float timeElapsed )
{
	if (m_IsVisible)
	{
		// Render the semi transparent overlay
		OpenGLInterface::UseProgram( m_OverlayProgram );
 		OpenGLInterface::VertexAttrib1f( 1, m_ClipSize );
 		glEnable( GL_BLEND );
 		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
 		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
 		glDisable( GL_BLEND );

		// Render the text
		OpenGLInterface::UseProgram( m_RenderTextProgram );
		OpenGLInterface::VertexAttrib1f( 1, m_ClipSize );
		//OpenGLInterface::VertexAttrib4fv( 2, m_TextScale );

		OpenGLInterface::Uniform2fv( m_FontScalarLocationId, 1, m_TextScale );
		GLenum error = glGetError();

		// Update the screen buffer with latest version of the cache data
		CopyCacheToRenderBuffer();

		OpenGLInterface::ActiveTexture( GL_TEXTURE0 );

		glBindTexture( GL_TEXTURE_2D, text_buffer );

		glTexSubImage2D(
			GL_TEXTURE_2D,			// Target to which the texture is bound
			0,						// Level of detail number, 0 is base
			0,						// Specifies a texel offset in the x direction within the texture array
			0,						// Specifies a texel offset in the y direction within the texture array
			m_BufferWidth,			// Specifies the width of the texture subimage
			m_BufferHeight,			// Specifies the height of the texture subimage
			GL_RED_INTEGER,			// Specifies the format of the pixel data.  GL_RED_INTEGER tells GL to keep the exact integer value rather than normalizing
			GL_UNSIGNED_BYTE,		// Specifies the data type of the pixel data
			screen_buffer			// Specifies a pointer to the image data in memory
		);

		OpenGLInterface::ActiveTexture( GL_TEXTURE2 );

		glBindTexture( GL_TEXTURE_1D, text_color_buffer );

		glTexSubImage1D(
			GL_TEXTURE_1D,			// Target to which the texture is bound
			0,						// Level of detail number, 0 is base
			0,						// Specifies a texel offset in the x direction within the texture array
			m_BufferHeight,			// Specifies the width of the texture subimage
			GL_RED_INTEGER,			// Specifies the format of the pixel data.  GL_RED_INTEGER tells GL to keep the exact integer value rather than normalizing
			GL_UNSIGNED_INT,		// Specifies the data type of the pixel data
			screen_color_buffer		// Specifies a pointer to the image data in memory
		);

		OpenGLInterface::ActiveTexture( GL_TEXTURE1 );

		OpenGLInterface::BindTexture(GL_TEXTURE_2D_ARRAY, font_texture);

		OpenGLInterface::BindVertexArray(text_vao);

		// Render four vertices in a triangle strip format, which equates to a square composed of two triangles
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
	if (m_Dirty)
	{
		// Fill the render buffer with the latest version of the cache

		// Clear the buffers
		memset( screen_buffer, 0, m_BufferWidth * m_BufferHeight * sizeof( char ) );
		memset( screen_color_buffer, 0xFF, m_BufferHeight * sizeof( unsigned int ) );

		//
		unsigned int cacheIndex = m_CacheIndex;

		for( int index = ( m_BufferHeight - 1 ); index >= 0; --index )
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
						convertedString += ('0' + currentLength  % 10);
						++currentLength;
					}
				}
			}

			// Handle wrapped lines
			size_t stringLength = convertedString.length();
			char* dst = screen_buffer + index * m_BufferWidth;
			while( stringLength > m_VirtualBufferWidth )
			{
				size_t subIndex = ( ( stringLength - 1 ) / m_VirtualBufferWidth ) * m_VirtualBufferWidth;
				int subLength = stringLength % m_VirtualBufferWidth;
				if( subLength == 0 )
				{
					subLength = m_VirtualBufferWidth;
				}
				memcpy( dst, convertedString.substr( subIndex ).c_str(), subLength );
				screen_color_buffer[ index ] = m_Cache[ cacheIndex ].colorValue;
				stringLength -= subLength;
				if( --index < 0 )
				{
					return;
				}
				dst = screen_buffer + index * m_BufferWidth;
			}

			memcpy( dst, convertedString.c_str(), stringLength );
			screen_color_buffer[ index ] = m_Cache[ cacheIndex ].colorValue;
		}
	}
}

void Console::ReceiveMessage( const Message& message )
{
	unsigned int colorValue;

	switch( message.Type )
	{
	case Message::LOG_INFO:
		colorValue = m_ColorTable[ 0 ]; // White
		break;
	case Message::LOG_WARN:
		colorValue = m_ColorTable[ 1 ]; // Yellow
		break;
	case Message::LOG_ERROR:
		colorValue = m_ColorTable[ 2 ]; // Red
		break;
	default:
		// Invalid message type
		return;
	}

	// Write to the cache string entry
	m_Cache[ m_CacheIndex ].messageString.assign( *message.stringData, 0, Message::MAX_STRING_LENGTH );

	// Set cache entry color?
	m_Cache[ m_CacheIndex ].colorValue = colorValue;

	// Update the index to the next available line in the cache
	m_CacheIndex = ( m_CacheIndex + 1 ) % m_CacheSize;

	m_Dirty = true;
}

