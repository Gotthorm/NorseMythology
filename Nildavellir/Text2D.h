#pragma once

#include "Platform.h"
#include "OpenGLInterface.h"

class Text2D
{
public:
	Text2D() : cursor_x(0), cursor_y(0)	{ }

	void init(int width, int height, const char* font = nullptr);
	void Shutdown();
	void draw();

	void drawText( const wchar_t* str, int x, int y);
	void print(const char* str);
	void scroll(int lines);
	void moveCursor(int x, int y);
	void clear();

private:
	void CopyWideCharBufferToCharBuffer( const wchar_t*, char* );

	GLuint      text_buffer;
	GLuint      font_texture;
	GLuint      vao;

	GLint		m_FontWidth;
	GLint		m_FontHeight;

	GLuint      text_program;
	char *      screen_buffer;
	int         buffer_width;
	int         buffer_height;
	bool        dirty;
	int         cursor_x;
	int         cursor_y;
};