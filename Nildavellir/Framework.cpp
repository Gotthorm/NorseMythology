// FRAMEWORK.CPP

#include "Framework.h"
#include "Graphics.h"
#include "Game.h"
#include "Input.h"
#include "Logger.h"
#include <sstream>
#include <cwchar>
#include "MessageManager.h"

bool Framework::Init( Platform::WindowHandle hWindow, const Platform::LaunchInfo& launchInfo )
{
	if( m_Initialized == false )
	{
		// Initialize the message manager
		MessageManager::Create();

		// Initialize the log system
		m_pLogger = new Logger();
		PLATFORM_ASSERT( m_pLogger != nullptr );
		if( m_pLogger == nullptr || m_pLogger->Initialize( L"LogFile.txt" ) == false )
		{
			PLATFORM_DEBUG_MESSAGE( L"Cannot initialize the logger system\n" );
			return false;
		}

		MessageManager::GetInstance()->Post( Message::LOG_INFO, launchInfo.applicationTitle );

		// Initialize the graphics system
		m_pGraphics = new Graphics();
		PLATFORM_ASSERT( m_pGraphics != nullptr );
		if( m_pGraphics == nullptr || m_pGraphics->InitializeRenderingContext( hWindow ) == false )
		{
			MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Failed to initialize the graphics rendering context" ) );
			return false;
		}

		MessageManager::GetInstance()->Post( Message::LOG_INFO, m_pGraphics->GetVersionInformation() );

		if( m_pGraphics->InitializeSubSystems() == false )
		{
			MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Failed to initialize the graphics subsystems" ) );
			return false;
		}

		// Initialize the input system
		Input::Create();
		if( Input::GetInstance()->Init() == false )
		{
			MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Cannot initialize the input system" ) );
			return false;
		}

		// Initialize the game
		m_pGame = new Game();
		PLATFORM_ASSERT( m_pGame != nullptr );
		if( m_pGame == nullptr || m_pGame->Init() == false )
		{
			MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Cannot initialize the game system" ) );
			return false;
		}

		//m_pGame->EnableMouseCapture( false );

		// Initialize the stats related variables
		m_OldFrameTime = timeGetTime();
		m_OneSecondIntervalAccumulator = 0;
		m_UpdateAccumulator = 0;
		m_CurrentFPS = 0;
	}

	// Set the initialized flag as true and return it as the results
    return m_Initialized = true;
}

void Framework::Shutdown()
{
	if( m_pGraphics != nullptr )
	{
		m_pGraphics->Shutdown();
		delete m_pGraphics;
		m_pGraphics = nullptr;
	}
	Input::Destroy();
	if( m_pGame != nullptr )
	{
		m_pGame->Shutdown();
		delete m_pGame;
		m_pGame = nullptr;
	}
	// Try to always shutdown the logger 2nd last if possible
	if( m_pLogger != nullptr )
	{
		m_pLogger->Shutdown();
		delete m_pLogger;
		m_pLogger = nullptr;
	}
	// Try to always shutdown the message manager last if possible
	MessageManager::Destroy();
}

void Framework::Update()
{
	if( m_Initialized )
	{
		// Get the current time in milliseconds since the computer was turned on
		unsigned int newFrameTime = Platform::GetTime();

		// Calculate the amount of milliseconds since the last update
		unsigned int timeElapsed = newFrameTime - m_OldFrameTime;

		// If someone's computer has been running for 49 days, the counter may wrap over
		if( newFrameTime < m_OldFrameTime )
		{
			timeElapsed = newFrameTime + ( UINT_MAX - m_OldFrameTime );
		}

		// Calculate the current FPS value
		{
			// Update the 1 second accumulator
			m_OneSecondIntervalAccumulator += timeElapsed;

			// Increment the update accumulator;
			++m_UpdateAccumulator;

			const unsigned int kOneSecond = 1000;

			if( m_OneSecondIntervalAccumulator >= kOneSecond )
			{
				unsigned int secondElapsed = 0;

				while( m_OneSecondIntervalAccumulator >= kOneSecond )
				{
					++secondElapsed;
					m_OneSecondIntervalAccumulator -= kOneSecond;
				}

				m_CurrentFPS = m_UpdateAccumulator / secondElapsed;

				m_UpdateAccumulator = 0;

#if 0
				// TEMP: For debugging
				static int counter = 0;
				std::wstringstream s;
				s << L"One second has elapsed : " << counter++ << "#";
				size_t length = s.str().length();
				for( ; length < 150; ++length )
				{
					s << ( length  % 10 );
					//length = s.str().length();
				}
				MessageManager::GetInstance()->Post( Message::LOG_INFO, s.str() );
				//MessageManager::GetInstance()->Post( Message::LOG_ERROR, s.str() );
#endif
			}
		}

		// Update the old time for the next update
		m_OldFrameTime = newFrameTime;

		// Check for the console activation/deactivation
		if( Input::GetInstance()->GetKeyUp( Input::KEY_TILDA ) )
		{
			// Toggle the console
			PLATFORM_ASSERT( m_pGraphics != nullptr );
			m_pGraphics->ToggleConsole();
		}

		// Update the game
		PLATFORM_ASSERT( m_pGame != nullptr );
		m_pGame->Update();

		PLATFORM_ASSERT( m_pGraphics != nullptr );
		m_pGraphics->UpdateConsole( timeElapsed / 1000.0f );

		// Render the current FPS
		wchar_t stringBuffer[ Platform::kMaxStringLength ];
		std::swprintf( stringBuffer, Platform::kMaxStringLength, L"%4u FPS", m_CurrentFPS );
		PLATFORM_ASSERT( m_pGraphics != nullptr );
		m_pGraphics->DisplayText( stringBuffer, 110, 0 );

		// Update which is the current camera
		m_pGraphics->SetCamera( m_pGame->GetCurrentCamera() );

		// Render the scene
		m_pGraphics->Render();

		// Clear input key releases and post message buffer
		Input::GetInstance()->AdvanceFrame();

		// Pump the message manager to broadcast all cached messages
		MessageManager::GetInstance()->Update();
	}
}

// This method is responsible for removing the platform dependence from the input data
void Framework::ProcessInputEvent( Platform::LongParam lParam )
{
	if( m_Initialized )
	{
		UINT dwSize;

		if( GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof( RAWINPUTHEADER ) ) != 0 )
		{
			MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Size query to GetRawInputData failed!" ) );
			return;
		}

		if( dwSize == 0 )
		{
			MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"Size query to GetRawInputData returned 0 size!" ) );
			return;
		}

		LPBYTE lpb = new BYTE[ dwSize ];
		PLATFORM_ASSERT( lpb != nullptr );

		if( GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof( RAWINPUTHEADER ) ) != dwSize )
		{
			MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"GetRawInputData does not return correct size!" ) );
		}

		RAWINPUT* raw = (RAWINPUT*)lpb;
		unsigned int data = 0;

		if( raw->header.dwType == RIM_TYPEKEYBOARD )
		{
			data = raw->data.keyboard.VKey;
			if( ( raw->data.keyboard.Flags & RI_KEY_BREAK ) == 0 )
			{
				data |= Input::MASK_KEY_PRESS;
			}

#if 0
			wchar_t stringBuffer[ 256 ];
			wsprintf( stringBuffer, L" Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x\n",
				raw->data.keyboard.MakeCode,
				raw->data.keyboard.Flags,
				raw->data.keyboard.Reserved,
				raw->data.keyboard.ExtraInformation,
				raw->data.keyboard.Message,
				raw->data.keyboard.VKey );

			OutputDebugString( stringBuffer );
#endif
		}
		//else if (raw->header.dwType == RIM_TYPEMOUSE)
		//{
		//	TCHAR stringBuffer[256];
		//	_stprintf_s(stringBuffer, 256, _T("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
		//		raw->data.mouse.usFlags,
		//		raw->data.mouse.ulButtons,
		//		raw->data.mouse.usButtonFlags,
		//		raw->data.mouse.usButtonData,
		//		raw->data.mouse.ulRawButtons,
		//		raw->data.mouse.lLastX,
		//		raw->data.mouse.lLastY,
		//		raw->data.mouse.ulExtraInformation);

		//	OutputDebugString(stringBuffer);
		//}

		delete[] lpb;

		Input::GetInstance()->ProcessEvent( data );
	}
}

void Framework::ResizeWindow( unsigned short width, unsigned short height )
{
	if( m_Initialized )
	{
		PLATFORM_ASSERT( m_pGraphics != nullptr );
		m_pGraphics->SetWindowSize( width, height );
	}
}