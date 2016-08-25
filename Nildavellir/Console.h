// CONSOLE.H

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <Windows.h>
//#include <TCHAR.H>
#include "OpenGLInterface.h"
//#include <assert.h>
#include <string>
#include <vector>
#include "MessageClient.h"

/// \brief A window for logging and run time variable tweaking
///
/// The Console class controls a special window whose visibility is toggled with the '`' or '~' keys.
/// It is used for monitoring logged messages in real time and to also allow for querying and modifying run time variables.
class Console : public MessageClient
{
public:
	// Use the default implementation
	Console() = default;

	// Use the default implementation
	virtual ~Console() = default;

	/// \brief Create the console window.
	///
	/// Create and activate the console window.  
	/// The console will immediately begin receiving messages after this call. 
	/// \param width The desired width of the console window.  This would typically be the width of the parent window.
	/// \param height The desired height of the console window.  This would typically be the height of the parent window.
	/// \param heightPercent The vertical percentage of the given height that will be used by this console window. [0.0 > heightPercent <= 1.0]%
	/// \return Currently always returns true, needs to be fixed.
	bool Initialize( unsigned int width, unsigned int height, float heightPercent );

	/// \brief Close the console window
	///
	/// Closes the console window and cleans up all internal data
	void Shutdown();

	// Override MessageClient::ReceiveMessage
	virtual void ReceiveMessage( const Message& message ) override;

	/// \brief Set the console cache size
	///
	/// This sets the number of lines that are cached in the console window.
	/// The value set here will be the number of full lines without taking into account any wrapping.
	/// You can not set this size smaller than the height of the console.  If you do it will be clamped to the console line height.
	/// \param cacheSize The number of text lines that will be stored in the cache.
	void SetCacheSize( unsigned int cacheSize );

	/// \brief Notify the console of the current parent window size.
	///
	/// Call this whenever the parent window changes dimensions.
	/// The values used are typically the dimension of the parent windows client area dimensions.
	/// \param width
	/// \param height
	void SetWindowSize( unsigned int width, unsigned int height );

	/// \brief Set the text scale
	///
	/// Set the horizontal and vertical scale of the text.
	/// The default values are (1.0, 1.0)
	/// When you change these values the console will adjust the number of characters displayed both vertically and horizontally to match the new dimensions.
	/// \param widthScale
	/// \param heightScale
	void SetTextScale( float widthScale, float heightScale );

	/// \brief Render the console window
	///
	/// This is called by the graphics system every render frame.
	void Render();

	/// \brief Set the console visible or non visible.
	///
	/// Set the console visible or non visible. 
	/// \param visible
	void SetVisible( bool visible ) { m_IsVisible = visible; }

	/// \brief Get the visibility status of the console window.
	///
	/// Get the visibility status of the console window.
	/// \return Returns a boolean indicating whether the console window is visible or not.
	bool IsVisible() { return m_IsVisible; }

	/// \brief Update the console
	///
	/// Update the console with the amount of time that has elapsed in seconds, since the last update.
	/// \param timeElapsed The amount of time in seconds since the last update.
	void Update( float timeElapsed );

private:
	// Remove the default assignment operator
	Console & operator =( const Console & ) = delete;

	// Remove the copy constructor
	Console( const Console & ) = delete;

	void CopyCacheToRenderBuffer();

	void UpdateBufferSize();

	bool m_IsVisible = false;

	struct CacheEntry
	{
		std::wstring messageString;
		unsigned int colorValue;
	};
	std::vector<CacheEntry> m_Cache;

	unsigned int m_CacheIndex = 0;
	unsigned int m_CacheSize = 0;

	unsigned int m_BufferWidth = 0;
	unsigned int m_BufferHeight = 0;
	unsigned int m_VirtualBufferWidth = 0;
	unsigned int m_VirtualBufferHeight = 0;

	unsigned int m_WindowWidth = 0;
	unsigned int m_WindowHeight = 0;

	// The default scalars for the text font
	GLfloat m_TextScale[ 2 ] = { 1.0f, 1.0f };

	GLint m_FontScalarLocationId = -1;

	bool m_Dirty = false;
	GLfloat m_ClipSize = 0.0f;
	float m_HeightPercent = 0.0f;
	GLint m_FontHeight = 0;
	GLint m_FontWidth = 0;

	GLuint m_OverlayProgram;
	GLuint m_RenderTextProgram;

	GLuint m_TextBufferTextureId;
	GLuint m_TextColorBufferTextureId;
	GLuint m_FontTextureId;
	GLuint m_OverlayVertexArrayObjectId;
	GLuint m_TextVertexArrayObjectId;

	// This 2D buffer holds all of the text that is currently being displayed.
	// Each render frame it is copied to a texture to be processed by a shader.
	char* m_ScreenTextBuffer = nullptr;

	// This 1D buffer holds a list of the colors of each line in the m_ScreenTextBuffer buffer.
	// Each render frame it is copied to a texture to be processed by a shader.
	unsigned int* m_ScreenTextColorBuffer = nullptr;

	// TODO: This could be moved to an XML file and loaded dynamically?
	// Color constants: White, Yellow, Red, Blue
	unsigned int m_ColorTable[4] = { 0xFFFFFFFF, 0xFFFF00FF, 0xFF0000FF, 0x0000FFFF };

	// A dynamic index that tracks how many lines above the base position the current view is set to
	// When this value is non-zero the window will not auto-scroll when new text lines are posted,
	// unless the currently viewed lines have reached the limit of the cache and are being removed.
	unsigned int m_ScrollIndex = 0;
};

#endif // _CONSOLE_H_