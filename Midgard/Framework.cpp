// FRAMEWORK.CPP

#include <sstream>
#include <cwchar>
#include <functional>
#include "Framework.h"
//#include "Muspelheim.h"
#include "Niflheim.h"
//#include "Game.h"
#include "Helheimr.h"
#include "Platform.h"
#include "Loki.h"
#include "Volstagg.h"
#include "Vanaheimr.h"
//#include "ConsoleCommandManager.h"
//#include "ConsoleParameterList.h"
#include <glm/gtc/matrix_transform.hpp>

#pragma comment(lib, "Niflheim.lib")
#pragma comment(lib, "Muspelheim.lib")
#pragma comment(lib, "Helheimr.lib")
#pragma comment(lib, "Vanaheimr.lib")
#pragma comment(lib, "Valhalla.lib")

class camera {
	glm::vec3 m_pos;
	glm::quat m_orient;
public:
	camera( void ) = default;
	camera( const camera & ) = default;

	camera( const glm::vec3 &pos ) : m_pos( pos ) {}
	camera( const glm::vec3 &pos, const glm::quat &orient ) : m_pos( pos ), m_orient( orient ) {}

	camera &operator =( const camera & ) = default;

	const glm::vec3 &position( void ) const { return m_pos; }
	const glm::quat &orientation( void ) const { return m_orient; }

	glm::mat4 view( void ) const { return glm::translate( glm::mat4_cast( m_orient ), m_pos ); }

	void translate( const glm::vec3 &v ) { m_pos += v * m_orient; }
	void translate( float x, float y, float z ) { m_pos += glm::vec3( x, y, z ) * m_orient; }

	void rotate( float angle, const glm::vec3 &axis ) { m_orient *= glm::angleAxis( angle, axis * m_orient ); }
	void rotate( float angle, float x, float y, float z ) { m_orient *= glm::angleAxis( angle, glm::vec3( x, y, z ) * m_orient ); }

	void yaw( float angle ) { rotate( angle, 0.0f, 1.0f, 0.0f ); }
	void pitch( float angle ) { rotate( angle, 1.0f, 0.0f, 0.0f ); }
	void roll( float angle ) { rotate( angle, 0.0f, 0.0f, 1.0f ); }
};

bool Framework::Init( Platform::WindowHandle hWindow, const Platform::LaunchInfo& launchInfo )
{
	if( m_Initialized == false )
	{
		// Initialize the message manager
		m_MessageManager = std::make_shared<Niflheim::MessageManager>();
		PLATFORM_ASSERT( m_MessageManager != nullptr );

		// Initialize the log system
		m_Logger = new Niflheim::Logger( m_MessageManager );
		PLATFORM_ASSERT( m_Logger != nullptr );

		if( m_Logger == nullptr || m_Logger->Initialize( L"LogFile.txt" ) == false )
		{
			PLATFORM_DEBUG_MESSAGE( L"Cannot initialize the logger system\n" );
			return false;
		}

		m_MessageManager->Post( Niflheim::Message::LOG_INFO, launchInfo.applicationTitle );

		// Initialize the renderer
		m_Renderer = Muspelheim::Renderer::Create();
		PLATFORM_ASSERT( m_Renderer != nullptr );
		if( m_Renderer == nullptr || m_Renderer->Initialize( hWindow ) == false )
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Failed to initialize the graphics rendering system" ) );
			return false;
		}

		m_MessageManager->Post( Niflheim::Message::LOG_INFO, m_Renderer->GetVersionInformation() );

		// Initialize the input system
		Helheimr::Input::Create();
		if( Helheimr::Input::GetInstance()->Init() == false )
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Cannot initialize the input system" ) );
			return false;
		}
		else
		{
			RAWINPUTDEVICE Rid[ 2 ];

			Rid[ 0 ].usUsagePage = 0x01;
			Rid[ 0 ].usUsage = 0x02;		// Adds HID mouse
			Rid[ 0 ].dwFlags = 0;			// Change to RIDEV_NOLEGACY to ignore legacy keyboard messages
			Rid[ 0 ].hwndTarget = 0;

			Rid[ 1 ].usUsagePage = 0x01;
			Rid[ 1 ].usUsage = 0x06;		// Adds HID keyboard
			Rid[ 1 ].dwFlags = RIDEV_NOLEGACY;			// Change to RIDEV_NOLEGACY to ignore legacy keyboard messages
			Rid[ 1 ].hwndTarget = 0;

			if( RegisterRawInputDevices( Rid, 2, sizeof( Rid[0] ) ) == FALSE )
			{
				//registration failed. Call GetLastError for the cause of the error
				m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Cannot initialize the keyboard for the input system" ) );
				return false;
			}
		}

		// Setup main screen
		if( m_Renderer->CreateSurface( m_MainScreen ) == false )
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Failed to create main render surface" ) );
			return false;
		}
		else
		{
			unsigned int shaderId = m_Renderer->LoadShader( L"Media/Shaders/surface.background" );
			m_Renderer->SetSurfaceShader( m_MainScreen, shaderId );

			// Set background to the color orange
			m_Renderer->SetSurfaceColor( m_MainScreen, glm::vec4( 0.8f, 0.3f, 0.3f, 1.0f ) );

			shaderId = m_Renderer->LoadShader( L"Media/Shaders/text2d" );
			m_Renderer->SetSurfaceTextShader( m_MainScreen, shaderId );
		}

		//
		m_CameraManager = new Vanaheimr::CameraManager();
		m_CameraManager->AddCamera( Vanaheimr::CameraManager::FREE, L"Free" );

		std::weak_ptr<Muspelheim::LightObject> mainLight = m_Renderer->CreateSurfaceLightObject( m_MainScreen );

		mainLight.lock()->SetAmbient( 0, 0, 0, 1 );
		mainLight.lock()->SetDiffuse( 1, 1, 1, 1 );
		mainLight.lock()->SetSpecular( 1, 1, 1, 1 );
		mainLight.lock()->SetPosition( glm::vec4( 100.0f, 100.0f, 100.0f, 1 ) );

		const float moveScale = 10.0f;

		// Initialize the game
		m_Loki1 = new Loki( m_MainScreen );
		if( m_Loki1 && m_Loki1->Init( *m_Renderer ) )
		{
			if( m_Loki1->Load( "Media/Objects/Dragon.sbm" ) == false )
			{
				m_MessageManager->Post( Niflheim::Message::LOG_WARN, std::wstring( L"Failed to load Loki render object" ) );
			}
		}
		else
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Failed to create Loki render object" ) );
			return false;
		}
		m_Loki1->SetPosition( glm::vec4( moveScale, 0, moveScale, 1.0f ) );

		m_Loki2 = new Loki( m_MainScreen );
		if( m_Loki2 && m_Loki2->Init( *m_Renderer ) )
		{
			if( m_Loki2->Load( "Media/Objects/Dragon.sbm" ) == false )
			{
				m_MessageManager->Post( Niflheim::Message::LOG_WARN, std::wstring( L"Failed to load Loki render object" ) );
			}
		}
		else
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Failed to create Loki render object" ) );
			return false;
		}
		m_Loki2->SetPosition( glm::vec4( -moveScale, 0, moveScale, 1.0f ) );

		m_Loki3 = new Loki( m_MainScreen );
		if( m_Loki3 && m_Loki3->Init( *m_Renderer ) )
		{
			if( m_Loki3->Load( "Media/Objects/Dragon.sbm" ) == false )
			{
				m_MessageManager->Post( Niflheim::Message::LOG_WARN, std::wstring( L"Failed to load Loki render object" ) );
			}
		}
		else
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Failed to create Loki render object" ) );
			return false;
		}
		m_Loki3->SetPosition( glm::vec4( moveScale, 0, -moveScale, 1.0f ) );

		m_Loki4 = new Loki( m_MainScreen );
		if( m_Loki4 && m_Loki4->Init( *m_Renderer ) )
		{
			if( m_Loki4->Load( "Media/Objects/Dragon.sbm" ) == false )
			{
				m_MessageManager->Post( Niflheim::Message::LOG_WARN, std::wstring( L"Failed to load Loki render object" ) );
			}
		}
		else
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Failed to create Loki render object" ) );
			return false;
		}
		m_Loki4->SetPosition( glm::vec4( -moveScale, 0, -moveScale, 1.0f ) );

		m_Volstagg = new Volstagg( m_MainScreen );
		if( m_Volstagg && m_Volstagg->Init( *m_Renderer ) )
		{
			if( m_Volstagg->Load( "Media/Textures/Island.png" ) == false )
			{
				m_MessageManager->Post( Niflheim::Message::LOG_WARN, std::wstring( L"Failed to load Volstagg render object" ) );
			}
		}
		else
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Failed to create Volstagg render object" ) );
			return false;
		}

		
		//m_pGame = new Game();
		//PLATFORM_ASSERT( m_pGame != nullptr );
		//if( m_pGame == nullptr || m_pGame->Init() == false )
		//{
		//	m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Cannot initialize the game system" ) );
		//	return false;
		//}

		//m_pGame->EnableMouseCapture( false );

		// Initialize the stats related variables
		m_OldFrameTime = timeGetTime();
		m_OneSecondIntervalAccumulator = 0;
		m_UpdateAccumulator = 0;
		m_CurrentFPS = 0;

		m_hWindow = hWindow;
	}

	//Vanaheimr::Object3D newObject;
	//glm::vec3 forwardVector = newObject.GetForward();
	//newObject.SetRotation( glm::vec3(0, 1.5708, 0) );
	//forwardVector = newObject.GetForward();

	// Set the initialized flag as true and return it as the results
    return m_Initialized = true;
}

void Framework::Shutdown()
{
	delete m_Loki1;
	m_Loki1 = nullptr;
	delete m_Loki2;
	m_Loki2 = nullptr;
	delete m_Loki3;
	m_Loki3 = nullptr;
	delete m_Loki4;
	m_Loki4 = nullptr;

	delete m_Volstagg;
	m_Volstagg = nullptr;

	if( m_Renderer != nullptr )
	{
		m_Renderer->Shutdown();
		delete m_Renderer;
		m_Renderer = nullptr;
	}
	Helheimr::Input::Destroy();
	//if( m_pGame != nullptr )
	//{
	//	m_pGame->Shutdown();
	//	delete m_pGame;
	//	m_pGame = nullptr;
	//}
	// Try to always shutdown the logger 2nd last if possible
	if( m_Logger != nullptr )
	{
		m_Logger->Shutdown();
		delete m_Logger;
		m_Logger = nullptr;
	}
	// Try to always shutdown the message manager last if possible
	m_MessageManager.reset();
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
		//if( Helheimr::Input::GetInstance()->GetKeyUp( Helheimr::Input::KEY_TILDA ) )
		//{
		//	// Toggle the console
		//	PLATFORM_ASSERT( m_pGraphics != nullptr );
		//	m_pGraphics->ToggleConsole();
		//}

		// Update the game
		//PLATFORM_ASSERT( m_pGame != nullptr );
		//m_pGame->Update();

		m_Loki1->Update();
		m_Loki2->Update();
		m_Loki3->Update();
		m_Loki4->Update();
		m_Volstagg->Update();
		m_CameraManager->Update( timeElapsed / 1000.0f, Helheimr::Input::GetInstance() );

		//PLATFORM_ASSERT( m_Renderer != nullptr );
		//m_pGraphics->UpdateConsole( timeElapsed / 1000.0f );

		// Update which is the current camera
		//m_Renderer->SetCamera( m_pGame->GetCurrentCamera() );

		// Render the scene
		const Vanaheimr::Camera* currentCamera = m_CameraManager->GetCurrentCamera();

		const glm::mat4& viewMatrix = currentCamera->GetMatrix();

		//// ###################################################
		//static camera internetCamera;
		//static glm::vec3 position;
		////static float currentYaw = 0;
		////static float currentPitch = 0;

		//if( Helheimr::Input::GetInstance()->GetKeyDown( Helheimr::Input::KEY_MOUSE_RIGHT ) )
		//{
		//	float mouseSpeed = 0.01f * timeElapsed;

		//	int deltaX;
		//	int deltaY;
		//	Helheimr::Input::GetInstance()->GetMouse( deltaX, deltaY );

		//	float targetPitch = (float)deltaY;
		//	float targetYaw = (float)-deltaX;

		//	float currentYaw = ( targetYaw * mouseSpeed );
		//	float currentPitch = ( targetPitch * mouseSpeed );

		//	//if( currentPitch > 89.0f )
		//	//	currentPitch = 89.0f;
		//	//if( currentPitch < -89.0f )
		//	//	currentPitch = -89.0f;

		//	internetCamera.pitch( currentPitch );
		//	internetCamera.yaw( currentYaw );

		//}
		//float arrowSpeed = 0.001f * timeElapsed;
		//if( Helheimr::Input::GetInstance()->GetKeyDown( Helheimr::Input::KEY_ARROW_DOWN ) )
		//{
		//	internetCamera.pitch( arrowSpeed );
		//}
		//else if( Helheimr::Input::GetInstance()->GetKeyDown( Helheimr::Input::KEY_ARROW_UP ) )
		//{
		//	internetCamera.pitch( -arrowSpeed );
		//}
		//if( Helheimr::Input::GetInstance()->GetKeyDown( Helheimr::Input::KEY_ARROW_LEFT ) )
		//{
		//	internetCamera.yaw( arrowSpeed );
		//}
		//else if( Helheimr::Input::GetInstance()->GetKeyDown( Helheimr::Input::KEY_ARROW_RIGHT ) )
		//{
		//	internetCamera.yaw( -arrowSpeed );
		//}



		//const glm::quat& viewQuat = internetCamera.orientation();

		//glm::mat4 identityMat = glm::mat4( 1.0f );
		//glm::mat4 rotMatrix = glm::mat4_cast( viewQuat );   //rotation is glm::quat
		//glm::mat4 transMatrix = glm::translate( identityMat, internetCamera.position() );
		//glm::mat4 viewMatrix = rotMatrix * glm::inverse( transMatrix );

		//glm::vec3 forward = glm::rotate( viewQuat, glm::vec3(0,0,1) );
		//glm::vec3 right = glm::cross( forward, glm::vec3( 0, 1, 0 ) );
		////glm::vec3 position = viewMatrix[ 3 ];

		//float MovementIncrement = 0.01f;

		//if( Helheimr::Input::GetInstance()->GetKeyDown( Helheimr::Input::KEY_W ) )
		//{
		//	//internetCamera.translate( glm::vec3( 0, 0, -MovementIncrement ) );
		//	position = position + ( forward * MovementIncrement );
		//}
		//else if( Helheimr::Input::GetInstance()->GetKeyDown( Helheimr::Input::KEY_S ) )
		//{
		//	//internetCamera.translate( glm::vec3( 0, 0, MovementIncrement ) );
		//	position = position - ( forward * MovementIncrement );
		//}
		//if( Helheimr::Input::GetInstance()->GetKeyDown( Helheimr::Input::KEY_A ) )
		//{
		//	//internetCamera.translate( glm::vec3( -MovementIncrement, 0, 0 ) );
		//	position = position - ( right * MovementIncrement );
		//}
		//else if( Helheimr::Input::GetInstance()->GetKeyDown( Helheimr::Input::KEY_D ) )
		//{
		//	//internetCamera.translate( glm::vec3( MovementIncrement, 0, 0 ) );
		//	position = position + ( right * MovementIncrement );
		//}

		//glm::mat4 viewMatrix99 = glm::lookAt( position, position + forward, glm::vec3(0,1,0) );

		// ####################################################

		m_Renderer->BeginRender( viewMatrix );

		// Render the current FPS
		wchar_t stringBuffer[ Platform::kMaxStringLength ];
		std::swprintf( stringBuffer, Platform::kMaxStringLength, L"%4u FPS", m_CurrentFPS );
		m_Renderer->DrawSurfaceString( m_MainScreen, stringBuffer, 10, 0, Muspelheim::Renderer::TEXT_RIGHT );

		glm::vec3 cameraForward = viewMatrix[ 2 ];
		std::swprintf( stringBuffer, Platform::kMaxStringLength, L"Camera Direction: %.3f, %.3f, %.3f", cameraForward.x, cameraForward.y, cameraForward.z );
		m_Renderer->DrawSurfaceString( m_MainScreen, stringBuffer, 40, 1, Muspelheim::Renderer::TEXT_RIGHT );

		glm::vec3 cameraPosition = currentCamera->GetPosition();
		std::swprintf( stringBuffer, Platform::kMaxStringLength, L"Camera Position: %.1f, %.1f, %.1f", cameraPosition.x, cameraPosition.y, cameraPosition.z );
		m_Renderer->DrawSurfaceString( m_MainScreen, stringBuffer, 40, 2, Muspelheim::Renderer::TEXT_RIGHT );

		m_Renderer->EndRender();

		// Clear input key releases and post message buffer
		Helheimr::Input::GetInstance()->AdvanceFrame();

		// Pump the message manager to broadcast all cached messages
		m_MessageManager->Update();
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
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Size query to GetRawInputData failed!" ) );
			return;
		}

		if( dwSize == 0 )
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Size query to GetRawInputData returned 0 size!" ) );
			return;
		}

		LPBYTE lpb = new BYTE[ dwSize ];
		PLATFORM_ASSERT( lpb != nullptr );

		if( GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof( RAWINPUTHEADER ) ) != dwSize )
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"GetRawInputData does not return correct size!" ) );
		}

		RAWINPUT* raw = (RAWINPUT*)lpb;
		unsigned int data = 0;

		if( raw->header.dwType == RIM_TYPEKEYBOARD )
		{
			data = raw->data.keyboard.VKey;
			if( ( raw->data.keyboard.Flags & RI_KEY_BREAK ) == 0 )
			{
				data |= Helheimr::Input::MODIFIER_KEY_PRESS;
			}
			else
			{
				// It seems the way we are handling input has intercepted the regular generation of the WM_CLOSE event (alt-F4)
				// So we intercept that key combination here and manually post the message.
				if( raw->data.keyboard.VKey == Helheimr::Input::KEY_F4 && Helheimr::Input::GetInstance()->GetKeyDown( Helheimr::Input::KEY_ALT ) )
				{
					PostMessage( m_hWindow, WM_CLOSE, 0, 0 );
				}
			}
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			if( raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN )
			{
				if( m_MouseCaptured == false )
				{
					SetCapture( m_hWindow );
					m_MouseCaptured = true;
					ShowCursor( FALSE );
				}

				//data = Helheimr::Input::KEY_MOUSE_RIGHT;
				//data |= Helheimr::Input::MODIFIER_KEY_PRESS;

			}
			else if( raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP )
			{
				if( m_MouseCaptured == true )
				{
					ReleaseCapture();
					m_MouseCaptured = false;
					ShowCursor( TRUE );

					// Place the cursor back to the center of the window
					RECT rect;
					if( GetWindowRect( m_hWindow, &rect ) )
					{
						int width = rect.right - rect.left;
						int height = rect.bottom - rect.top;

						SetCursorPos( width / 2, height / 2 );
					}
				}

				//data = Helheimr::Input::KEY_MOUSE_RIGHT;
			}

			// Ensure that the last X & Y are deltas and not absolute
			if( m_MouseCaptured && ( raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE ) == 0 )
			{
				long rawMouseX = raw->data.mouse.lLastX;
				long rawMouseY = raw->data.mouse.lLastY;

				// We will cap deltas to CHAR_MIN to CHAR_MAX for each axis
				if( rawMouseX < CHAR_MIN || rawMouseX > CHAR_MAX || rawMouseY < CHAR_MIN || rawMouseY > CHAR_MAX )
				{
					wchar_t stringBuffer[ Platform::kMaxStringLength ];
					std::swprintf( stringBuffer, Platform::kMaxStringLength, L"Mouse deltas exceeded max range (%i, %i)", rawMouseX, rawMouseY );

					m_MessageManager->Post( Niflheim::Message::LOG_ERROR, stringBuffer );

					if( rawMouseX < CHAR_MIN )
					{
						rawMouseX = CHAR_MIN;
					}
					else if( rawMouseX > CHAR_MAX )
					{
						rawMouseX = CHAR_MAX;
					}

					if( rawMouseY < CHAR_MIN )
					{
						rawMouseY = CHAR_MIN;
					}
					else if( rawMouseY > CHAR_MAX )
					{
						rawMouseY = CHAR_MAX;
					}
				}

				char signMouseX = ( rawMouseX < 0 ) ? 0x80 : 0;
				char signMouseY = ( rawMouseY < 0 ) ? 0x80 : 0;

				char mouseX = ( ( rawMouseX & CHAR_MAX ) | signMouseX );
				char mouseY = ( ( rawMouseY & CHAR_MAX ) | signMouseY );

				data |= ( mouseX << 24 );
				data |= ( mouseY << 16 );
			}
		}

		delete[] lpb;

		Helheimr::Input::GetInstance()->ProcessEvent( data );
	}
}

void Framework::ResizeWindow( unsigned short width, unsigned short height )
{
	if( m_Initialized )
	{
		PLATFORM_ASSERT( m_Renderer != nullptr );
		m_Renderer->SetWindowSize( width, height );
	}
}

//void Framework::UpdateCamera( unsigned int timeElapsed )
//{
//	// nTimeDelta is the number of milliseconds that has elapsed since the last frame
//#if 1
//	if( m_pInput != NULL )
//	{
//		float deltaTime = timeElapsed / 1000.0f;
//
//		if( m_pInput->GetKey( Input::KEY_MOUSE_RIGHT ) )
//		{
//			long deltaX;
//			long deltaY;
//			m_pInput->GetMouse( deltaX, deltaY );
//
//			//camera->ProcessMouseMovement((float)deltaX, (float)-deltaY);
//			m_CameraTargetPitch = (float)-deltaY;
//			m_CameraTargetYaw = (float)-deltaX;
//		}
//
//		const float mouseSpeed = 0.25f;
//
//		m_CameraCurrentYaw += ( m_CameraTargetYaw * mouseSpeed );
//		m_CameraCurrentPitch -= ( m_CameraTargetPitch * mouseSpeed );
//
//		m_CameraTargetPitch = m_CameraTargetYaw = 0.0f;
//
//		Matrix3 theYawMatrix( Vector3::UNIT_Y, Math::DEG_TO_RAD * m_CameraCurrentYaw );
//		Matrix3 thePitchMatrix( Vector3::UNIT_X, Math::DEG_TO_RAD * m_CameraCurrentPitch );
//		Vector3 theUpVector = thePitchMatrix * Vector3::UNIT_Y;
//
//		Vector3 orientationMatrix = ( theYawMatrix * thePitchMatrix ) * Vector3::UNIT_Z;
//		m_CameraOrientation[ 0 ] = orientationMatrix[ 0 ];
//		m_CameraOrientation[ 1 ] = orientationMatrix[ 1 ];
//		m_CameraOrientation[ 2 ] = orientationMatrix[ 2 ];
//
//		Vector3 rightVector = orientationMatrix.Cross( theUpVector );
//
//		// 10 units per second
//		float MovementIncrement = 100.0f * ( timeElapsed / 1000.0f );
//
//		if( m_pInput->GetKey( Input::KEY_W ) )
//		{
//			//camera->ProcessKeyboard(FORWARD, deltaTime);
//			m_CameraPosition = m_CameraPosition + ( m_CameraOrientation * MovementIncrement );
//		}
//		else if( m_pInput->GetKey( Input::KEY_S ) )
//		{
//			//camera->ProcessKeyboard(BACKWARD, deltaTime);
//			m_CameraPosition = m_CameraPosition - ( m_CameraOrientation * MovementIncrement );
//		}
//
//		if( m_pInput->GetKey( Input::KEY_A ) )
//		{
//			//camera->ProcessKeyboard(LEFT, deltaTime);
//			m_CameraPosition = m_CameraPosition - ( rightVector * MovementIncrement );
//		}
//		else if( m_pInput->GetKey( Input::KEY_D ) )
//		{
//			//camera->ProcessKeyboard(RIGHT, deltaTime);
//			m_CameraPosition = m_CameraPosition + ( rightVector * MovementIncrement );
//		}
//	}
//#else
//	//if (g_nRequestedCameraMode == FLY)
//	{
//		if( m_pInput->GetKey( Input::KEY_MOUSE_RIGHT ) )
//		{
//			long deltaX;
//			long deltaY;
//			m_pInput->GetMouse( deltaX, deltaY );
//			//g_CameraPanActive = true;
//			m_CameraTargetPitch = (float)-deltaY;
//			m_CameraTargetYaw = (float)-deltaX;
//		}
//
//		m_CameraCurrentYaw += ( m_CameraTargetYaw * 0.75f );
//		m_CameraCurrentPitch -= ( m_CameraTargetPitch * 0.75f );
//
//		m_CameraTargetPitch = m_CameraTargetYaw = 0.0f;
//
//		Matrix3 theYawMatrix( Vector3::UNIT_Y, Math::DEG_TO_RAD * m_CameraCurrentYaw );
//		Matrix3 thePitchMatrix( Vector3::UNIT_X, Math::DEG_TO_RAD * m_CameraCurrentPitch );
//		Vector3 theUpVector = thePitchMatrix * Vector3::UNIT_Y;
//
//		Vector3 orientationMatrix = ( theYawMatrix * thePitchMatrix ) * Vector3::UNIT_Z;
//		m_CameraOrientation[ 0 ] = orientationMatrix[ 0 ];
//		m_CameraOrientation[ 1 ] = orientationMatrix[ 1 ];
//		m_CameraOrientation[ 2 ] = orientationMatrix[ 2 ];
//
//		Vector3 rightVector = orientationMatrix.Cross( theUpVector );
//
//		// 10 units per second
//		float MovementIncrement = 100.0f * ( timeElapsed / 1000.0f );
//
//		if( m_pInput != NULL )
//		{
//			if( m_pInput->GetKey( Input::KEY_W ) )
//			{
//				m_CameraPosition = m_CameraPosition + ( m_CameraOrientation * MovementIncrement );
//			}
//			else if( m_pInput->GetKey( Input::KEY_S ) )
//			{
//				m_CameraPosition = m_CameraPosition - ( m_CameraOrientation * MovementIncrement );
//			}
//
//			if( m_pInput->GetKey( Input::KEY_A ) )
//			{
//				m_CameraPosition = m_CameraPosition - ( rightVector * MovementIncrement );
//			}
//			else if( m_pInput->GetKey( Input::KEY_D ) )
//			{
//				m_CameraPosition = m_CameraPosition + ( rightVector * MovementIncrement );
//			}
//		}
//	}
//#endif
//}