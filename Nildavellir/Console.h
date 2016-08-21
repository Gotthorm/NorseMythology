#pragma once

#include <Windows.h>
#include <TCHAR.H>
#include "OpenGLInterface.h"
#include <assert.h>
#include <string>
#include <vector>
#include "MessageClient.h"


class Console : public MessageClient
{
public:
	Console();

	virtual ~Console();

	// Create the console using the given height percent of the window [0.0 > heightPercent <= 1.0]%
	bool Initialize(unsigned int width, unsigned int height, float heightPercent);

	void Shutdown();

	// Override MessageClient::ReceiveMessage
	virtual void ReceiveMessage( const Message& message );

	// Set the history size of the console window data.
	// You cannot set this size smaller than the height of the console
	// Smaller values will be clamped to the current height.
	void SetCacheSize(unsigned int cacheSize);

	// Set the current window dimensions
	void SetWindowSize( unsigned short width, unsigned short height);

	//bool Init(unsigned int width, unsigned int height, unsigned int cacheSize);
	void Render();
	void SetVisible(bool visible) { m_IsVisible = visible; }
	bool IsVisible() { return m_IsVisible; }

	//void DrawText(std::wstring& str, int x, int y);
	//void print(const char* str);
	//void scroll(int lines);
	//void moveCursor(int x, int y);
	//void clear();

private:
	void CopyCacheToRenderBuffer();

	bool m_IsVisible;

	std::vector<std::wstring> m_Cache;

	unsigned int m_CacheIndex;

	bool m_Dirty;
	GLfloat m_ClipSize;
	float m_HeightPercent;
	unsigned int m_CacheSize;
	unsigned int m_FontHeight;
	unsigned int m_FontWidth;

	GLuint m_OverlayProgram;
	GLuint m_RenderTextProgram;

	GLuint      text_buffer;
	GLuint      font_texture;
	GLuint      vao;
	GLuint      text_vao;

	char *      screen_buffer;
	int         buffer_width;
	int         buffer_height;

	int         cursor_x;
	int         cursor_y;
};