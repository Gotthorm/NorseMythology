// CONSOLE.CPP

#include "Alfheimr.h"
#include "Niflheim.h"
#include "Muspelheim.h"
#include "Vanaheimr.h"
#include "Helheimr.h"
//#include <sstream>
#include <memory>
#include "ConsoleParser.h"
#include "ConsoleCommandManager.h"
//#include "Platform.h"


// TODO: This will become a console variable
unsigned int cv_MinimumConsoleMessageLength = 0;

std::wstring const consoleShaderName( L"Media/Shaders/console.overlay" );
std::wstring const text2DShaderName( L"Media/Shaders/text2d" );

namespace Alfheimr
{
	Console::Console( const std::weak_ptr<Niflheim::MessageManager>& messageManager ) 
		: Niflheim::MessageClient( messageManager )
		, m_MaximumLineCount( 0 )
		, m_LineBuffer( 100 )
	{
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
			pMessageManager->Deregister( this, Niflheim::Message::KEY_STROKES );
		}

		SetVisible( false );

		m_MessageManager.reset();

		delete m_Parser;
		m_Parser = nullptr;

		ConsoleCommandManager::Destroy();

		//m_Cache.clear();

		delete[] m_ScreenTextBuffer;
		m_ScreenTextBuffer = nullptr;

		delete[] m_ScreenTextColorBuffer;
		m_ScreenTextColorBuffer = nullptr;

		std::shared_ptr<Muspelheim::Renderer> pRenderer = m_Renderer.lock();
		if ( nullptr != pRenderer )
		{
			pRenderer->DestroySurface( m_MainScreenID );
			pRenderer.reset();
		}
		m_Renderer.reset();

		//OpenGLInterface::DeleteVertexArrays( 1, &m_OverlayVertexArrayObjectId );
		//OpenGLInterface::DeleteVertexArrays( 1, &m_TextVertexArrayObjectId );
		//OpenGLInterface::DeleteProgram( m_OverlayProgram );
		//OpenGLInterface::DeleteProgram( m_RenderTextProgram );
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

		unsigned int fontWidth = 0;
		unsigned int fontHeight = 0;

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
				pRenderer->SetSurfaceColor( m_MainScreenID, glm::vec4( 0.8f, 0.3f, 0.3f, 0.7f ) );

				pRenderer->SetSurfaceVisible( m_MainScreenID, false );

				shaderId = pRenderer->LoadShader( text2DShaderName );

				if ( 0 == shaderId || false == pRenderer->SetSurfaceTextShader( m_MainScreenID, shaderId ) )
				{
					pMessageManager->Post( Niflheim::Message::LOG_WARN, L"Failed to load shader: " + text2DShaderName );
				}

				pMessageManager->Post( Niflheim::Message::LOG_INFO, L"Shader: " + text2DShaderName + L" loaded" );
			}

			pRenderer->GetSurfaceFontSize( m_MainScreenID, fontWidth, fontHeight );
		}

		m_HeightPercent = heightPercent;

		// Calculate the overlay size in clip space where height(0) => 1.0 && height(1.0) => -1.0
		m_ClipSize = 1.0f - m_HeightPercent * 2.0f;

		// Set up the window dimensions.  This call may also update the buffer size.
		SetWindowSize( width, height );

		cv_MinimumConsoleMessageLength = width / fontWidth;

		// Register for the correct messages
		pMessageManager->Register( this, Niflheim::Message::LOG_INFO );
		pMessageManager->Register( this, Niflheim::Message::LOG_WARN );
		pMessageManager->Register( this, Niflheim::Message::LOG_ERROR );
		pMessageManager->Register( this, Niflheim::Message::KEY_STROKES );

		m_ConsoleTextBuffer.reserve( Niflheim::Message::MAX_STRING_LENGTH );

		m_Parser = new ConsoleParser( pMessageManager );
		assert( m_Parser );

		ConsoleCommandManager::Create();

		//REGISTER_COMMAND2( L"set_text_size", Console::SetTextScale, ConsoleParameter::ParameterType::FLOAT, ConsoleParameter::ParameterType::FLOAT );

		//ConsoleCommandManager::GetInstance()->TestRegisterCommand( L"set_text_size", std::bind( &Console::SetTextScale_Callback, this, std::placeholders::_1 ), ConsoleParameter::ParameterType::FLOAT, ConsoleParameter::ParameterType::FLOAT );

		//ConsoleCommandManager::FartRegisterCommand( L"set_text_size", ConsoleParameter::ParameterType::FLOAT, ConsoleParameter::ParameterType::FLOAT );

		//ConsoleCommandManager::GetInstance()->RegisterCommand( L"set_text_size", std::bind( &Console::SetTextScale_Callback, this, std::placeholders::_1 ), ConsoleParameterList( 2, ConsoleParameter::ParameterType::FLOAT, ConsoleParameter::ParameterType::FLOAT ) );
		//ConsoleCommandManager::GetInstance()->RegisterCommand( L"set_text_size", std::bind( &Console::SetTextScale, this, std::placeholders::_1, std::placeholders::_2 ), ConsoleParameterList( 2, ConsoleParameter::ParameterType::FLOAT, ConsoleParameter::ParameterType::FLOAT ) );

		//ConsoleCommandManager::GetInstance()->RegisterCommand( L"set_text_size", L"Some sort of description", &Console::SetTextScale_Callback );

		return true;
	}

	void Console::SetMaximumLineCount( unsigned int lineCount )
	{
		// 
		if ( lineCount != m_MaximumLineCount )
		{
			while ( lineCount < m_MaximumLineCount )
			{
				m_LineBuffer.Pop();
			}

			lineCount = m_MaximumLineCount;
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

		// Determine what the current display dimensions are in character units
		unsigned int newBufferWidth = m_WindowWidth / unsigned int( m_TextScale[ 0 ] * fontWidth );
		unsigned int newBufferHeight = unsigned int( m_WindowHeight * m_HeightPercent ) / unsigned int( m_TextScale[ 1 ] * fontHeight );

		// Leave one line for the console input
		--newBufferHeight;

		if( newBufferWidth != m_VirtualBufferWidth || newBufferHeight != m_VirtualBufferHeight )
		{
			std::wstring message;

			std::shared_ptr<Niflheim::MessageManager> pMessageManager = m_MessageManager.lock();

			if ( nullptr != pMessageManager )
			{
				message = L"Console virtual buffer size changed from (" + std::to_wstring( m_VirtualBufferWidth ) + L") to (" + std::to_wstring( newBufferWidth ) + L", " + std::to_wstring( newBufferHeight ) + L")";
				pMessageManager->Post( Niflheim::Message::LOG_INFO, message );
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
				message = L"Console physical buffer size changed to (" + std::to_wstring( m_BufferWidth ) + L", " + std::to_wstring( m_BufferHeight ) + L")";
				pMessageManager->Post( Niflheim::Message::LOG_INFO, message );
			}

			// To enforce our rule that the cache size is at least as large as the screen
			if( m_MaximumLineCount < m_VirtualBufferHeight )
			{
				SetMaximumLineCount( m_VirtualBufferHeight );
			}

			m_MaxScrollIndex = m_LineBuffer.Size() - m_VirtualBufferHeight + 2;
			if ( m_MaxScrollIndex < 0 )
			{
				m_MaxScrollIndex = 0;
			}
		}
	}

	void Console::Render()
	{
		if( IsVisible() )
		{
			std::shared_ptr<Muspelheim::Renderer> pRenderer = m_Renderer.lock();

			if ( nullptr != pRenderer )
			{
				// Update the screen buffer with latest version of the cache data
				RenderText( pRenderer );
			}
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

	bool Console::ProcessVisibilityToggle( unsigned int key )
	{
		// Check for the console activation/deactivation
		if ( Helheimr::Input::KEY_TILDA == key )
		{
			// Toggle the console
			SetVisible( IsVisible() == false );

			return true;
		}

		return false;
	}

	bool Console::ProcessControlInput( unsigned int key )
	{
		bool processed = false;

		// When active we will process user input for the window
		if ( IsVisible() )
		{
			processed = true;

			if ( Helheimr::Input::KEY_ARROW_UP == key )
			{
				++m_ScrollIndex;
			}
			else if ( Helheimr::Input::KEY_ARROW_DOWN == key )
			{
				--m_ScrollIndex;
			}
			else if ( Helheimr::Input::KEY_END == key )
			{
				m_ScrollIndex = 0;
			}
			//if( Helheimr::Input::KEY_HOME == key )
			//{
			//	m_ScrollIndex = m_MaxScrollIndex;
			//}
			else
			{
				processed = false;
			}

			int const maxScrollIndex = m_LineBuffer.Size() - m_VirtualBufferHeight + 2;

			if ( Helheimr::Input::KEY_ARROW_UP == key )
			{
				++m_ScrollIndex;
			}
			else if ( Helheimr::Input::KEY_ARROW_DOWN == key )
			{
				--m_ScrollIndex;
			}

			if ( Helheimr::Input::KEY_END == key )
			{
				m_ScrollIndex = 0;
			}
			else if ( m_ScrollIndex > maxScrollIndex )
			{
				m_ScrollIndex = maxScrollIndex;
			}
			else if ( m_ScrollIndex < 0 )
			{
				m_ScrollIndex = 0;
			}
			// TODO: Broken
			//if( Input::GetInstance()->GetKeyUp( Input::KEY_HOME ) )
			//{
			//	m_ScrollIndex = m_MaxScrollIndex;
			//}
		}

		return processed;
	}

	void Console::ProcessTextInput( unsigned int keyValue, bool shifted )
	{
		// The text processing only occurs when we the console is visible
		if ( IsVisible() )
		{
			wchar_t character = 0;
			if ( keyValue >= Helheimr::Input::KEY_A && keyValue <= Helheimr::Input::KEY_Z )
			{
				character = ( shifted ? L'A' : L'a' ) + ( keyValue - Helheimr::Input::KEY_A );
			}
			else if ( keyValue >= Helheimr::Input::KEY_0 && keyValue <= Helheimr::Input::KEY_9 )
			{
				if ( shifted )
				{
					switch ( keyValue )
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
			else if ( keyValue >= Helheimr::Input::KEY_NUMPAD_0 && keyValue <= Helheimr::Input::KEY_NUMPAD_9 )
			{
				character = L'0' + ( keyValue - Helheimr::Input::KEY_NUMPAD_0 );
			}
			else if ( keyValue == Helheimr::Input::KEY_SPACE )
			{
				character = L' ';
			}
			else if ( keyValue == Helheimr::Input::KEY_BACKSPACE )
			{
				if ( m_ConsoleTextBuffer.length() > 0 )
				{
					m_ConsoleTextBuffer.pop_back();
				}
			}
			else if ( keyValue == Helheimr::Input::KEY_SEMICOLON )
			{
				character = shifted ? L':' : L';';
			}
			else if ( keyValue == Helheimr::Input::KEY_EQUALS )
			{
				character = shifted ? L'+' : L'=';
			}
			else if ( keyValue == Helheimr::Input::KEY_COMMA )
			{
				character = shifted ? L'<' : L',';
			}
			else if ( keyValue == Helheimr::Input::KEY_MINUS )
			{
				character = shifted ? L'_' : L'-';
			}
			else if ( keyValue == Helheimr::Input::KEY_PERIOD )
			{
				character = shifted ? L'>' : L'.';
			}
			else if ( keyValue == Helheimr::Input::KEY_FORWARD_SLASH )
			{
				character = shifted ? L'?' : L'/';
			}
			else if ( keyValue == Helheimr::Input::KEY_SINGLE_QUOTE )
			{
				character = shifted ? L'"' : L'\'';
			}

			if ( character != 0 )
			{
				m_ConsoleTextBuffer += character;
			}
			else if ( keyValue == Helheimr::Input::KEY_RETURN )
			{
				// Send the command line string to the parser
				m_Parser->Execute( m_ConsoleTextBuffer );

				// Clear the string
				m_ConsoleTextBuffer.clear();
			}
		}
	}

	void Console::Update( std::shared_ptr<Helheimr::Input> const & input, float timeElapsed )
	{
	}

	void Console::SetVisible( bool visible )
	{
		if( m_IsVisible != visible )
		{
			m_IsVisible = visible;

			std::shared_ptr<Muspelheim::Renderer> pRenderer = m_Renderer.lock();

			if ( nullptr != pRenderer )
			{
				pRenderer->SetSurfaceVisible( m_MainScreenID, m_IsVisible );
			}

			std::shared_ptr<Niflheim::MessageManager> pMessageManager = m_MessageManager.lock();

			if ( nullptr != pMessageManager )
			{
				if ( m_IsVisible )
				{
					m_ScrollIndex = 0;
				}
			}
		}
	}

	void Console::ProcessKeystroke( unsigned int keyStroke )
	{
		unsigned short keyValue = ( Helheimr::Input::MASK_KEY_VALUE & keyStroke );

		bool shifted = ( Helheimr::Input::MODIFIER_KEY_SHIFT & keyStroke ) > 0;

		// We will ignore key releases to support key repeat
		bool keyPressed = ( Helheimr::Input::MODIFIER_KEY_PRESS & keyStroke ) != 0;

		if ( keyPressed )
		{
			if ( false == ProcessVisibilityToggle( keyValue ) )
			{
				if ( false == ProcessControlInput( keyValue ) )
				{
					ProcessTextInput( keyValue, shifted );
				}
			}
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

		if ( m_LineBuffer.Full() )
		{
			m_LineBuffer.Pop();
		}
		m_LineBuffer.Push( *logMessage );

		++m_ScrollIndex;

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

	void Console::RenderText( std::shared_ptr<Muspelheim::Renderer> const & renderer )
	{
		int const startIndex = ( m_VirtualBufferHeight - 3 );
		int const endIndex = 0;
		int const lineBufferSize = m_LineBuffer.Size() - 1;
		int lineIndex = m_ScrollIndex;
		for ( int index = startIndex; index >= endIndex; --index )
		{
			std::wstring const & outputString = m_LineBuffer[ lineBufferSize - lineIndex ];
			++lineIndex;

			// For debugging the console window dimensions, etc
			// This will pad non empty lines to a minimum length
			//if ( cv_MinimumConsoleMessageLength > 0 )
			//{
			//	// TODO: This returns the reserve size, not the actual string length
			//	size_t currentLength = outputString.length();

			//	// We do not want to pad empty entries in the buffer
			//	if ( currentLength > 0 )
			//	{
			//		while ( currentLength < cv_MinimumConsoleMessageLength )
			//		{
			//			outputString += ( '0' + currentLength % 10 );
			//			++currentLength;
			//		}
			//	}
			//}

			renderer->DrawSurfaceString( m_MainScreenID, outputString, 0, index, Muspelheim::Renderer::TEXT_LEFT );
		}

		renderer->DrawSurfaceString( m_MainScreenID, m_ConsoleTextBuffer, 0, startIndex + 1, Muspelheim::Renderer::TEXT_LEFT );
	}
}