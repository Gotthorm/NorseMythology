// CONSOLE.CPP

// TODO List
// 
// 1) Make the initialize method handle errors correctly
// 2) Figure out how to make the overlay height and the buffer height match
// 3) Change color support to be able to color each line individually

#include "Console.h"
#include <math.h>
#include "KTX.h"
#include "Logger.h"
#include <codecvt>
#include "MessageManager.h"

// Static
//const unsigned int kDefaultCacheSize = 10;

Console::Console() 
	: cursor_x(0)
	, cursor_y(0)
	, m_IsVisible(false)
	, m_CacheSize(0)
	, m_CacheIndex(0)
	, screen_buffer(nullptr)
{
}

Console::~Console()
{
	Shutdown();
}

// TODO: Change this to accept %width and %height instead
// TODO: Make this handle init errors
bool Console::Initialize(unsigned int width, unsigned int height, float heightPercent)
{
	GLuint vertexShader = OpenGLInterface::LoadShader("Media/Shaders/console.overlay.vs.glsl", GL_VERTEX_SHADER, true);
	GLuint fragmentShader = OpenGLInterface::LoadShader("Media/Shaders/console.overlay.fs.glsl", GL_FRAGMENT_SHADER, true);

	m_HeightPercent = heightPercent;

	// Calculate the overlay size in clip space where height(0) => 1.0 && height(1.0) => -1.0
	m_ClipSize = 1.0f - m_HeightPercent * 2.0f;

	// We are using a 9x16 pixel bitmap font
	m_FontHeight = 16;
	m_FontWidth = 9;
	font_texture = KTX::load("Media/Textures/cp437_9x16.ktx");

	SetWindowSize(width, height);

	m_OverlayProgram = OpenGLInterface::CreateProgram();
	OpenGLInterface::AttachShader(m_OverlayProgram, vertexShader);
	OpenGLInterface::AttachShader(m_OverlayProgram, fragmentShader);
	OpenGLInterface::LinkProgram(m_OverlayProgram);

	OpenGLInterface::DeleteShader(fragmentShader);
	OpenGLInterface::DeleteShader(vertexShader);

	// glCreateVertexArrays(1, &vao);
	OpenGLInterface::GenVertexArrays(1, &vao);
	OpenGLInterface::BindVertexArray(vao);

	vertexShader = OpenGLInterface::LoadShader("Media/Shaders/text2d.vs.glsl", GL_VERTEX_SHADER, true);
	fragmentShader = OpenGLInterface::LoadShader("Media/Shaders/text2d.fs.glsl", GL_FRAGMENT_SHADER, true);

	m_RenderTextProgram = OpenGLInterface::CreateProgram();
	OpenGLInterface::AttachShader(m_RenderTextProgram, vertexShader);
	OpenGLInterface::AttachShader(m_RenderTextProgram, fragmentShader);
	OpenGLInterface::LinkProgram(m_RenderTextProgram);

	OpenGLInterface::DeleteShader(fragmentShader);
	OpenGLInterface::DeleteShader(vertexShader);

	// glCreateVertexArrays(1, &vao);
	OpenGLInterface::GenVertexArrays(1, &text_vao);
	OpenGLInterface::BindVertexArray(text_vao);

	// glCreateTextures(GL_TEXTURE_2D, 1, &text_buffer);
	glGenTextures(1, &text_buffer);
	glBindTexture(GL_TEXTURE_2D, text_buffer);
	//OpenGLInterface::TexStorage2D(GL_TEXTURE_2D, 1, GL_R8UI, width, height);
	OpenGLInterface::TexStorage2D(GL_TEXTURE_2D, 1, GL_R8UI, 512, 512);

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

	delete [] screen_buffer;
	screen_buffer = NULL;

	OpenGLInterface::DeleteVertexArrays(1, &vao);
	OpenGLInterface::DeleteVertexArrays(1, &text_vao);
	OpenGLInterface::DeleteProgram(m_OverlayProgram);
	OpenGLInterface::DeleteProgram(m_RenderTextProgram);
}

void Console::SetCacheSize(unsigned int cacheSize)
{
	std::vector<std::wstring> tempCache;

	// Make a copy of the current cache
	if (m_Cache.size() > 0)
	{
		tempCache.swap(m_Cache);
	}

	// Store the old cache attributes for the copying of existing data
	unsigned int oldIndex = m_CacheIndex;
	unsigned int oldSize = m_CacheSize;

	// The new cache cannot be smaller than the buffer height so clamp it here
	if (cacheSize < buffer_height)
	{
		cacheSize = buffer_height;
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
		m_Cache[index].reserve(Message::MAX_STRING_LENGTH);

		// Copy the existing string from the old cache
		if (index < oldSize)
		{
			// We will be reset the cache index so the index we copy from will be the oldest entry in the old cache
			m_Cache[index].swap(tempCache[oldIndex]);

			// Move to the next oldest entry in the old cache
			oldIndex = (oldIndex + 1) % oldSize;
		}
	}
}

void Console::SetWindowSize(unsigned int width, unsigned int height)
{
	buffer_width = width / m_FontWidth;
	buffer_height = ((unsigned int)(height * m_HeightPercent) / m_FontHeight) - 1;

	// Temp hack for testing, we clear the last 4 bits
	buffer_width = buffer_width >> 4;
	buffer_width = buffer_width << 4;
	//buffer_height = buffer_height >> 4;
	//buffer_height = buffer_height << 4;

	delete [] screen_buffer;
	screen_buffer = new char[buffer_width * buffer_height];

	SetCacheSize(buffer_height);
}

void Console::Render()
{
	if (m_IsVisible)
	{
		// Render overlay

		OpenGLInterface::UseProgram(m_OverlayProgram);

		OpenGLInterface::VertexAttrib1f(1, m_ClipSize);

		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisable(GL_BLEND);

		// Render text
		const GLfloat color[] = {1.0f, 0.0f, 1.0f, 1.0f};

		OpenGLInterface::UseProgram(m_RenderTextProgram);

		OpenGLInterface::VertexAttrib1f(1, m_ClipSize);
		OpenGLInterface::VertexAttrib4fv(2, color);

		OpenGLInterface::ActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, text_buffer);

		// Update the screen buffer with latest version of the cache data
		CopyCacheToRenderBuffer();

		glTexSubImage2D(
			GL_TEXTURE_2D,		// Target to which the texture is bound
			0,					// Level of detail number, 0 is base
			0,					// Specifies a texel offset in the x direction within the texture array
			0,					// Specifies a texel offset in the y direction within the texture array
			buffer_width,		// Specifies the width of the texture subimage
			buffer_height,		// Specifies the height of the texture subimage
			GL_RED_INTEGER,		// Specifies the format of the pixel data.  GL_RED_INTEGER tells GL to keep the exact integer value rather than normalizing
			GL_UNSIGNED_BYTE,	// Specifies the data type of the pixel data
			screen_buffer		// Specifies a pointer to the image data in memory
		);

		OpenGLInterface::ActiveTexture(GL_TEXTURE1);

		OpenGLInterface::BindTexture(GL_TEXTURE_2D_ARRAY, font_texture);

		OpenGLInterface::BindVertexArray(text_vao);

		// Render four vertices in a triangle strip format, which equates to a square composed of two triangles
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}

void Console::CopyCacheToRenderBuffer()
{
	if (m_Dirty)
	{
		// Fill the render buffer with the latest version of the cache

		// Clear the buffer
		memset(screen_buffer, 0, buffer_width * buffer_height);

		// Setup converter
		// For now we convert each line from wchar to char
		// TODO: Create a render buffer that supports unicode
		typedef std::codecvt_utf8<wchar_t> convert_type;
		std::wstring_convert<convert_type, wchar_t> converter;

		//
		unsigned int cacheIndex = m_CacheIndex;
		unsigned int cacheEntriesLeft = m_CacheSize;

		for (int index = (buffer_height - 1); index >= 0, cacheEntriesLeft > 0; --index)
		{
			// Determine cache line index
			if (cacheIndex == 0)
			{
				cacheIndex = m_CacheSize;
			}
			--cacheIndex;
			--cacheEntriesLeft;

			// Convert cached line from wchar to char text and copy into the render buffer
			std::string converted_str = converter.to_bytes(m_Cache[cacheIndex]);
			char* dst = screen_buffer + index * buffer_width;
			strcpy(dst, converted_str.c_str());
		}
	}
}

void Console::ReceiveMessage( const Message& message )
{
	unsigned int colorIndex;

	switch( message.Type )
	{
	case Message::LOG_INFO:
		colorIndex = 0;
		break;
	case Message::LOG_WARN:
		colorIndex = 1;
		break;
	case Message::LOG_ERROR:
		colorIndex = 2;
		break;
	default:
		// Invalid message type
		return;
	}

	// Write to the cache string entry
	m_Cache[ m_CacheIndex ].assign( *message.stringData, 0, Message::MAX_STRING_LENGTH );

	// Set cache entry color?

	// Update the index to the next available line in the cache
	m_CacheIndex = ( m_CacheIndex + 1 ) % m_CacheSize;

	m_Dirty = true;
}

//void Console::DrawText(std::wstring& str, int x, int y)
//{
//	//std::wstring str = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes("some string");
//
//	//setup converter
//	typedef std::codecvt_utf8<wchar_t> convert_type;
//	std::wstring_convert<convert_type, wchar_t> converter;
//
//	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
//	std::string converted_str = converter.to_bytes(str);
//
//	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//	//VersionString = converter.from_bytes(versionString);
//
//	char* dst = screen_buffer + y * buffer_width + x;
//	strcpy(dst, converted_str.c_str());
//	dirty = true;
//}
//
//void Console::scroll(int lines)
//{
//	const char* src = screen_buffer + lines * buffer_width;
//	char * dst = screen_buffer;
//
//	memmove(dst, src, (buffer_height - lines) * buffer_width);
//
//	dirty = true;
//}
//
//void Console::print(const char* str)
//{
//	const char* p = str;
//	char c;
//	char* dst = screen_buffer + cursor_y * buffer_width + cursor_x;
//
//	while (*p != 0)
//	{
//		c = *p++;
//		if (c == '\n')
//		{
//			cursor_y++;
//			cursor_x = 0;
//			if (cursor_y >= buffer_height)
//			{
//				cursor_y--;
//				scroll(1);
//			}
//			dst = screen_buffer + cursor_y * buffer_width + cursor_x;
//		}
//		else
//		{
//			*dst++ = c;
//			cursor_x++;
//			if (cursor_x >= buffer_width)
//			{
//				cursor_y++;
//				cursor_x = 0;
//				if (cursor_y >= buffer_height)
//				{
//					cursor_y--;
//					scroll(1);
//				}
//				dst = screen_buffer + cursor_y * buffer_width + cursor_x;
//			}
//		}
//	}
//
//	dirty = true;
//}
//
//void Console::moveCursor(int x, int y)
//{
//	cursor_x = x;
//	cursor_y = y;
//}
//
//void Console::clear()
//{
//	memset(screen_buffer, 0, buffer_width * buffer_height);
//	dirty = true;
//	cursor_x = 0;
//	cursor_y = 0;
//}
