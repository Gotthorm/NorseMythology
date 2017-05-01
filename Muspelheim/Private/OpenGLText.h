#pragma once

#include "Muspelheim.h"
#include "OpenGLInterface.h"
#include <string>

namespace Muspelheim
{
	class OpenGLShader;

	class OpenGLText
	{
	public:
		OpenGLText() = default;

		~OpenGLText();

		// Called by the parent surface
		bool Init( unsigned short width, unsigned int height );

		bool SetShader( OpenGLShader* shader );

		// Called by the parent surface
		void Render();

		//bool LoadFont( const std::wstring& font ) override;

		bool DrawString( const std::wstring& str, unsigned short posX, unsigned short posY );

		//bool MoveCursor( unsigned short posX, unsigned short posY ) override;

		void Clear();

		GLuint GetFontWidth() { return static_cast<GLuint>( m_FontWidth ); }
		GLuint GetFontHeight() { return static_cast<GLuint>( m_FontHeight ); }

	private:
		bool SetSize( unsigned short width, unsigned int height );

		GLuint      text_buffer;
		GLuint      font_texture;
		GLuint      vao;

		GLint		m_FontWidth = 0;
		GLint		m_FontHeight = 0;

		//GLuint      text_program = 0;
		OpenGLShader* m_Shader = nullptr;
		//char *      screen_buffer = nullptr;;
		//int         cursor_x = 0;
		//int         cursor_y = 0;

		bool m_Dirty = true;

		char* m_ScreenBuffer = nullptr;
		unsigned short m_BufferWidth = 0;
		unsigned short m_BufferHeight = 0;
	};
}