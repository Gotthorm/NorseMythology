
#pragma once  

#include <string>
#include <vector>
#include <queue>
#include "Alfheimr.h"
#include "Niflheim.h"
#include "Jotunheim.h"
#include "Muspelheim.h"

namespace Alfheimr
{
	// Prototypes
	class ConsoleParser;
	class ConsoleCommandManager;

	class ConsoleImplementation : public Console, public Niflheim::MessageClient
	{
	public:
		// 
		ConsoleImplementation( const std::weak_ptr<Niflheim::MessageManager>& messageManager, std::weak_ptr<Muspelheim::Renderer> const & renderer );
		virtual ~ConsoleImplementation();

		// Implementation of Console interface
		virtual bool Initialize( int width, unsigned int height, float verticalClipSize ) override;
		virtual bool GetMaximumLineCount( unsigned int & lineCount ) override;
		virtual bool SetMaximumLineCount( unsigned int lineCount ) override;
		virtual void UpdateWindowSize( unsigned int width, unsigned int height ) override;
		virtual bool GetTextScale( float & widthScale, float & heightScale ) override;
		virtual bool SetTextScale( float widthScale, float heightScale ) override;
		virtual void Render() override;
		virtual void SetVisible( bool visible ) override;
		virtual bool IsVisible() override { return m_IsVisible; }
		virtual bool RegisterCommand( std::wstring const & functionName, std::function<void( ParameterList const & )> callback, std::shared_ptr<const ParameterList> params ) override;

		// Implementation of MessageClient interface
		virtual void ReceiveMessage( const Niflheim::Message& message ) override;

	private:
		// Remove the default assignment operator
		ConsoleImplementation & operator =( const ConsoleImplementation & ) = delete;

		// Remove the copy constructor
		ConsoleImplementation( const ConsoleImplementation & ) = delete;

		//void CopyCacheToRenderBuffer();
		void RenderText( std::shared_ptr<Muspelheim::Renderer> const & renderer );

		void UpdateBufferSize();

		bool ProcessVisibilityToggle( unsigned int key );

		bool ProcessControlInput( unsigned int keyStroke );

		void ProcessTextInput( unsigned int keyValue, bool shifted );

		void ProcessKeystroke( unsigned int keyStroke );

		void ProcessLogMessage( Niflheim::Message::MessageType type, std::wstring* logMessage );

		void TextScale_Callback( const Alfheimr::ParameterList& paramList );
		void MaxLineCount_Callback( const Alfheimr::ParameterList& paramList );

		void ValidateScrollIndex();

		bool m_IsVisible = false;

		struct CacheEntry
		{
			CacheEntry( std::wstring const & messageString, unsigned int colorValue, unsigned int wrapCount )
				: m_MessageString( messageString )
				, m_ColorValue( colorValue )
				, m_WrapCount( wrapCount )
			{}

			CacheEntry()
				: m_MessageString( L"" )
				, m_ColorValue( 0 )
				, m_WrapCount( 1 )
			{}

			std::wstring m_MessageString;
			unsigned int m_ColorValue;
			unsigned int m_WrapCount;
		};
		Jotunheim::RingBuffer<CacheEntry> m_LineBuffer;
		unsigned int m_VirtualTotalLineCount = 0;

		// The console's window dimensions in characters
		unsigned int m_VirtualBufferWidth = 0;
		unsigned int m_VirtualBufferHeight = 0;

		// The number of lines a page up/down will move
		unsigned int m_VirtualPageSize = 0;

		// The parent window's dimensions in pixels
		unsigned int m_WindowWidth = 0;
		unsigned int m_WindowHeight = 0;

		// The default scalars for the text font
		float m_TextScale[ 2 ] = { 1.0f, 1.0f };

		//int m_FontScalarLocationId = -1;

		bool m_Dirty = false;
		float m_ClipSize = 0.0f;
		float m_HeightPercent = 0.0f;
		//int m_FontHeight = 0;
		//int m_FontWidth = 0;

		// This 2D buffer holds all of the text that is currently being displayed.
		// Each render frame it is copied to a texture to be processed by a shader.
		//char* m_ScreenTextBuffer = nullptr;

		// This 1D buffer holds a list of the colors of each line in the m_ScreenTextBuffer buffer.
		// Each render frame it is copied to a texture to be processed by a shader.
		//unsigned int* m_ScreenTextColorBuffer = nullptr;

		// TODO: This could be moved to an XML file and loaded dynamically?
		// Color constants: White, Yellow, Red, Blue
		unsigned int m_ColorTable[ 4 ] = { 0xFFFFFFFF, 0xFFFF00FF, 0xFF0000FF, 0x01BCFFFF };

		// TODO: Update these incorrect comments or refactor code
		// A dynamic index that tracks how many lines above the base position the current view is set to
		// When this value is non-zero the window will not auto-scroll when new text lines are posted,
		// unless the currently viewed lines have reached the limit of the cache and are being removed.
		int m_ScrollIndex = 0;

		// The number of lines that are wrapping which are currently visible 
		int m_LineWrapCount = 0;

		unsigned int m_AutoCompletionIndex = 0;

		// The main screen (surface) identifier
		unsigned char m_MainScreenID;

		// The command line text buffer
		std::wstring m_ConsoleTextBuffer;

		// The command line text buffer
		std::wstring m_CommandAutoCompleteString;

		// The command line history
		Jotunheim::RingBuffer<std::wstring> m_ConsoleTextHistory;
		int m_HistoryIndex = -1;

		// A text buffer that represents the entire text surface
		// The dimensions match the physical dimensions of the parent window
		// This buffer does not understand wrapping so data must be wrapped for it by the client that writes into it.
		Muspelheim::TextBuffer m_ScreenTextBuffer;

		std::weak_ptr<Muspelheim::Renderer> m_Renderer;

		std::shared_ptr<ConsoleCommandManager> m_CommandManager;

		std::unique_ptr<ConsoleParser> m_Parser;
	};

}




