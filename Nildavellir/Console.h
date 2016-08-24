// CONSOLE.H

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

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
	// Use the default implementation
	Console() = default;

	// Use the default implementation
	virtual ~Console() = default;

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
	void SetWindowSize( unsigned int width, unsigned int height);

	// Set the text scale
	void SetTextScale( float widthScale, float heightScale );

	void Render( float timeElapsed );
	void SetVisible(bool visible) { m_IsVisible = visible; }
	bool IsVisible() { return m_IsVisible; }

private:
	// Remove the default assignment operator
	Console & operator =( const Console & ) = delete;

	// Remove the copy constructor
	Console( const Console & ) = delete;

	void CopyCacheToRenderBuffer();

	void UpdateBufferSize();

	bool m_IsVisible = false;

	std::vector<std::wstring> m_Cache;

	unsigned int m_CacheIndex = 0;
	unsigned int m_CacheSize = 0;

	unsigned int m_BufferWidth = 0;
	unsigned int m_BufferHeight = 0;
	unsigned int m_VirtualBufferWidth = 0;
	unsigned int m_VirtualBufferHeight = 0;

	unsigned int m_WindowWidth = 0;
	unsigned int m_WindowHeight = 0;

	GLfloat m_TextScale[ 2 ] = { 1.0f, 1.0f };

	bool m_Dirty = false;
	GLfloat m_ClipSize = 0.0f;
	float m_HeightPercent = 0.0f;
	GLint m_FontHeight = 0;
	GLint m_FontWidth = 0;

	GLuint m_OverlayProgram;
	GLuint m_RenderTextProgram;

	GLuint      text_buffer;
	GLuint      font_texture;
	GLuint      vao;
	GLuint      text_vao;

	char* screen_buffer = nullptr;

	int cursor_x = 0;
	int cursor_y = 0;
};

#endif // _CONSOLE_H_