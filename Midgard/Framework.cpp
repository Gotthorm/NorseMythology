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
#include "Alfheimr.h"

//#include "ConsoleCommandManager.h"
//#include "ConsoleParameterList.h"
#include <glm/gtc/matrix_transform.hpp>

#pragma comment(lib, "Niflheim.lib")
#pragma comment(lib, "Muspelheim.lib")
#pragma comment(lib, "Helheimr.lib")
#pragma comment(lib, "Vanaheimr.lib")
#pragma comment(lib, "Valhalla.lib")
#pragma comment(lib, "Alfheimr.lib")

// Used for the macro NEXTRAWINPUTBLOCK
typedef __int64 QWORD;

const unsigned int MaxRawInputEntries = 32;
const float VSyncMouseSpeed = 0.1f;
const float DefaultMouseSpeed = 1.0f;

//void Camera::Update()
//{
//	m_mat4ViewMatrix = glm::mat4();
//
//	m_flYaw += 0.02f * g_pInput->GetMouseDeltaX() * g_pEngine->GetDeltaTime();
//	m_flPitch += 0.02f * g_pInput->GetMouseDeltaY() * g_pEngine->GetDeltaTime();
//
//	float flYawDegrees = glm::degrees( m_flYaw );
//	float flPitchDegrees = glm::degrees( m_flPitch );
//
//	if ( flYawDegrees < 0.0f )
//		m_flYaw = glm::radians( flYawDegrees + 360.0f );
//	else if ( flYawDegrees > 360.0f )
//		m_flYaw = glm::radians( flYawDegrees - 360.0f );
//
//	if ( flPitchDegrees < 0.0f )
//		m_flPitch = glm::radians( flPitchDegrees + 360.0f );
//	else if ( flPitchDegrees > 360.0f )
//		m_flPitch = glm::radians( flPitchDegrees - 360.0f );
//
//	m_mat4ViewMatrix = glm::rotate( m_mat4ViewMatrix, m_flYaw, Vector3f( 0.0f, 1.0f, 0.0f ) );
//
//	m_mat4ViewMatrix = glm::rotate( m_mat4ViewMatrix, m_flPitch, Vector3f( m_mat4ViewMatrix[ 0 ][ 0 ], m_mat4ViewMatrix[ 1 ][ 0 ], m_mat4ViewMatrix[ 2 ][ 0 ] ) );
//
//	m_mat4ViewMatrix = glm::translate( m_mat4ViewMatrix, GetPosition() );
//
//	m_vForward = Vector3f( m_mat4ViewMatrix[ 0 ][ 2 ], m_mat4ViewMatrix[ 1 ][ 2 ], m_mat4ViewMatrix[ 2 ][ 2 ] );
//	m_vUp = Vector3f( m_mat4ViewMatrix[ 0 ][ 1 ], m_mat4ViewMatrix[ 1 ][ 1 ], m_mat4ViewMatrix[ 2 ][ 1 ] );
//	m_vLeft = Vector3f( m_mat4ViewMatrix[ 0 ][ 0 ], m_mat4ViewMatrix[ 1 ][ 0 ], m_mat4ViewMatrix[ 2 ][ 0 ] );
//
//	m_vBackward = -m_vForward;
//	m_vRight = -m_vLeft;
//
//	if ( g_pInput->IsButtonPressed( "Forward" ) )
//	{
//		SetPosition( GetPosition() + m_vForward * 0.01f * g_pEngine->GetDeltaTime() );
//	}
//	else if ( g_pInput->IsButtonPressed( "Backward" ) )
//	{
//		SetPosition( GetPosition() + m_vBackward * 0.01f * g_pEngine->GetDeltaTime() );
//	}
//	if ( g_pInput->IsButtonPressed( "Left" ) )
//	{
//		SetPosition( GetPosition() + m_vLeft * 0.01f * g_pEngine->GetDeltaTime() );
//	}
//	else if ( g_pInput->IsButtonPressed( "Right" ) )
//	{
//		SetPosition( GetPosition() + m_vRight * 0.01f * g_pEngine->GetDeltaTime() );
//	}
//}

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

std::wstring const surfaceShaderName( L"Media/Shaders/surface.background" );
std::wstring const text2DShaderName( L"Media/Shaders/text2d" );
std::wstring const terrainDataName( L"Media/Textures/Island.png" );
std::wstring const avatarDataName( L"Media/Objects/Dragon.sbm" );

Framework::Framework()
	: m_pLogger( nullptr )
	, m_pGame( nullptr )
//	, m_pInput( nullptr )
	, m_pCameraManager( nullptr )
	, m_pConsole( nullptr )
	, m_WindowHandle( 0 )
	, m_MainScreenID( Muspelheim::InvalidSurface )
	, m_pRawInputBuffer( nullptr )
{

}

bool Framework::Init( Platform::WindowHandle hWindow, const Platform::LaunchInfo& launchInfo )
{
	if( false == m_Initialized )
	{
		// Initialize the message manager
		m_MessageManager = std::make_shared<Niflheim::MessageManager>();
		PLATFORM_ASSERT( nullptr != m_MessageManager );

		// Initialize the log system
		m_pLogger = new Niflheim::Logger( m_MessageManager );
		PLATFORM_ASSERT( nullptr != m_pLogger );

		if( nullptr == m_pLogger || false == m_pLogger->Initialize( L"Logs\\LogFile.txt" ) )
		{
			PLATFORM_DEBUG_MESSAGE( L"Failed to initialize the logger system\n" );
			return false;
		}

		m_MessageManager->Post( Niflheim::Message::LOG_INFO, launchInfo.applicationTitle );

		// Initialize the renderer
		m_Renderer = Muspelheim::Renderer::Create();
		PLATFORM_ASSERT( nullptr != m_Renderer );
		if( nullptr == m_Renderer || false == m_Renderer->Initialize( hWindow ) )
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, L"Failed to initialize the graphics rendering system" );
			return false;
		}
		m_Renderer->SetVSyncEnabled( false );

		m_MessageManager->Post( Niflheim::Message::LOG_INFO, m_Renderer->GetVersionInformation() );

		// Initialize the input system
		// TODO: Can we remove the singleton?
		Helheimr::Input::Create();
		if( false == Helheimr::Input::GetInstance()->Init() )
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, L"Failed to initialize the input system" );
			return false;
		}
		else
		{
			{
				RAWINPUTDEVICE Rid[ 2 ];

				Rid[ 0 ].usUsagePage = 0x01;
				Rid[ 0 ].usUsage = 0x02;			// Adds HID mouse
				Rid[ 0 ].dwFlags = 0;				// Change to RIDEV_NOLEGACY to ignore legacy mouse messages
				Rid[ 0 ].hwndTarget = 0;

				Rid[ 1 ].usUsagePage = 0x01;
				Rid[ 1 ].usUsage = 0x06;			// Adds HID keyboard
				Rid[ 1 ].dwFlags = RIDEV_NOLEGACY;	// Change to RIDEV_NOLEGACY to ignore legacy keyboard messages
				Rid[ 1 ].hwndTarget = 0;

				// This is a Windows only call
				// To receive WM_INPUT messages, an application must first register the raw input devices using RegisterRawInputDevices. 
				// By default, an application does not receive raw input.
				if ( FALSE == RegisterRawInputDevices( Rid, 2, sizeof( Rid[ 0 ] ) ) )
				{
					// Registration failed. Call GetLastError for the cause of the error
					m_MessageManager->Post( Niflheim::Message::LOG_ERROR, L"Cannot initialize the keyboard for the input system" );
					return false;
				}

				m_pRawInputBuffer = new BYTE[ sizeof( RAWINPUT ) * MaxRawInputEntries ];
			}

			m_MessageManager->Post( Niflheim::Message::LOG_INFO, L"Input system initialized" );
		}

		// Set up the main screen
		if( false == m_Renderer->CreateSurface( m_MainScreenID ) )
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, L"Failed to create main render surface" );
			return false;
		}
		else
		{
			m_MessageManager->Post( Niflheim::Message::LOG_INFO, L"Render surface created" );

			unsigned int shaderId = m_Renderer->LoadShader( surfaceShaderName );

			if ( 0 == shaderId || false == m_Renderer->SetSurfaceShader( m_MainScreenID, shaderId ) )
			{
				m_MessageManager->Post( Niflheim::Message::LOG_WARN, L"Failed to load shader: " + surfaceShaderName );
			}

			m_MessageManager->Post( Niflheim::Message::LOG_INFO, L"Shader: " + surfaceShaderName + L" loaded" );

			// Set background to the color orange
			m_Renderer->SetSurfaceColor( m_MainScreenID, glm::vec4( 0.8f, 0.3f, 0.3f, 1.0f ) );

			shaderId = m_Renderer->LoadShader( text2DShaderName );
			
			if ( 0 == shaderId || false == m_Renderer->SetSurfaceTextShader( m_MainScreenID, shaderId ) )
			{
				m_MessageManager->Post( Niflheim::Message::LOG_WARN, L"Failed to load shader: " + text2DShaderName );
			}

			m_MessageManager->Post( Niflheim::Message::LOG_INFO, L"Shader: " + text2DShaderName + L" loaded" );
		}

		//
		m_pConsole = new Alfheimr::Console( m_MessageManager );
		m_pConsole->Initialize( m_Renderer, 80, 80, 50.0f );

		//
		m_pCameraManager = new Vanaheimr::CameraManager();
		PLATFORM_ASSERT( nullptr != m_pCameraManager );

		m_pCameraManager->AddCamera( Vanaheimr::CameraManager::FREE, L"Free" );

		float const moveScale = 10.0f;

		// Initialize the game

		// Create and initialize the avatar
		m_pLoki1 = new Loki( m_MainScreenID );
		PLATFORM_ASSERT( nullptr != m_pCameraManager );
		if( m_pLoki1->Init( m_Renderer ) )
		{
			if( false == m_pLoki1->Load( avatarDataName ) )
			{
				m_MessageManager->Post( Niflheim::Message::LOG_WARN, L"Failed to load Loki render data: " + avatarDataName );
			}
		}
		else
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, L"Failed to initialize a Loki render object" );
			return false;
		}

		m_pLoki1->SetPosition( glm::vec3( moveScale, 0, moveScale ) );

		//m_Loki2 = new Loki( m_MainScreenID );
		//if( m_Loki2 && m_Loki2->Init( *m_Renderer ) )
		//{
		//	if( m_Loki2->Load( "Media/Objects/Dragon.sbm" ) == false )
		//	{
		//		m_MessageManager->Post( Niflheim::Message::LOG_WARN, std::wstring( L"Failed to load Loki render object" ) );
		//	}
		//}
		//else
		//{
		//	m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Failed to create Loki render object" ) );
		//	return false;
		//}
		//m_Loki2->SetPosition( glm::vec3( -moveScale, 0, moveScale ) );

		//m_Loki3 = new Loki( m_MainScreenID );
		//if( m_Loki3 && m_Loki3->Init( *m_Renderer ) )
		//{
		//	if( m_Loki3->Load( "Media/Objects/Dragon.sbm" ) == false )
		//	{
		//		m_MessageManager->Post( Niflheim::Message::LOG_WARN, std::wstring( L"Failed to load Loki render object" ) );
		//	}
		//}
		//else
		//{
		//	m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Failed to create Loki render object" ) );
		//	return false;
		//}
		//m_Loki3->SetPosition( glm::vec3( moveScale, 0, -moveScale ) );

		//m_Loki4 = new Loki( m_MainScreenID );
		//if( m_Loki4 && m_Loki4->Init( *m_Renderer ) )
		//{
		//	if( m_Loki4->Load( "Media/Objects/Dragon.sbm" ) == false )
		//	{
		//		m_MessageManager->Post( Niflheim::Message::LOG_WARN, std::wstring( L"Failed to load Loki render object" ) );
		//	}
		//}
		//else
		//{
		//	m_MessageManager->Post( Niflheim::Message::LOG_ERROR, std::wstring( L"Failed to create Loki render object" ) );
		//	return false;
		//}
		//m_Loki4->SetPosition( glm::vec3( -moveScale, 0, -moveScale ) );

		// Create and initialize the terrain loader
		m_pVolstagg = new Volstagg( m_MainScreenID );
		PLATFORM_ASSERT( nullptr != m_pVolstagg );
		if( m_pVolstagg->Init( *m_Renderer ) )
		{
			// Load the terrain data
			if( false == m_pVolstagg->Load( terrainDataName ) )
			{
				m_MessageManager->Post( Niflheim::Message::LOG_WARN, L"Failed to load Volstagg render data:" + terrainDataName );
			}
		}
		else
		{
			m_MessageManager->Post( Niflheim::Message::LOG_ERROR, L"Failed to initialize a Volstagg render object" );
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

		m_WindowHandle = hWindow;
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
	delete m_pLoki1;
	m_pLoki1 = nullptr;
	//delete m_Loki2;
	//m_Loki2 = nullptr;
	//delete m_Loki3;
	//m_Loki3 = nullptr;
	//delete m_Loki4;
	//m_Loki4 = nullptr;

	delete m_pVolstagg;
	m_pVolstagg = nullptr;

	delete m_pCameraManager;
	m_pCameraManager = nullptr;

	delete m_pConsole;
	m_pConsole = nullptr;

	Helheimr::Input::Destroy();
	//if( m_pGame != nullptr )
	//{
	//	m_pGame->Shutdown();
	//	delete m_pGame;
	//	m_pGame = nullptr;
	//}
	delete m_pRawInputBuffer;
	m_pRawInputBuffer = nullptr;

	m_Renderer.reset();

	m_MessageManager->Post( Niflheim::Message::LOG_INFO, L"System shutdown" );
	m_MessageManager->Update();

	// Try to always shutdown the logger 2nd last if possible
	if( nullptr != m_pLogger )
	{
		m_pLogger->Shutdown();
		delete m_pLogger;
		m_pLogger = nullptr;
	}
	// Try to always shutdown the message manager last if possible
	m_MessageManager.reset();
}

void Framework::Update()
{
	if( m_Initialized )
	{
		m_FrameTime.Update();

		ProcessPlatformInput();

		// Check for the console activation/deactivation
		if( Helheimr::Input::GetInstance()->GetKeyUp( Helheimr::Input::KEY_TILDA ) )
		{
			// Toggle the console
			m_pConsole->SetVisible( m_pConsole->IsVisible() == false );
		}
		m_pConsole->Update( m_FrameTime.Duration() );

		if( Helheimr::Input::GetInstance()->GetKeyUp( Helheimr::Input::KEY_F2 ) )
		{
			// Toggle the vsync
			PLATFORM_ASSERT( nullptr != m_Renderer );
			bool const enable = ( false == m_Renderer->GetVSyncEnabled() );
			m_Renderer->SetVSyncEnabled( enable );
			Helheimr::Input::GetInstance()->SetMouseSpeed( enable ? VSyncMouseSpeed : DefaultMouseSpeed );
		}

		// Update the game
		//PLATFORM_ASSERT( m_pGame != nullptr );
		//m_pGame->Update();

		m_pLoki1->Update();
		//m_Loki2->Update();
		//m_Loki3->Update();
		//m_Loki4->Update();
		m_pVolstagg->Update();
		m_pCameraManager->Update( m_FrameTime.Duration(), Helheimr::Input::GetInstance() );

		//PLATFORM_ASSERT( m_Renderer != nullptr );
		//m_pGraphics->UpdateConsole( timeElapsed / 1000.0f );

		// Update which is the current camera
		//m_Renderer->SetCamera( m_pGame->GetCurrentCamera() );

		// Render the scene
		Vanaheimr::Camera const * const pCurrentCamera = m_pCameraManager->GetCurrentCamera();

		glm::mat4 const & viewMatrix = pCurrentCamera->GetMatrix();

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

		bool vsync = m_Renderer->GetVSyncEnabled();

		m_Renderer->BeginRender( viewMatrix );

		// Render the current FPS
		wchar_t stringBuffer[ Platform::kMaxStringLength ];
		std::swprintf( stringBuffer, Platform::kMaxStringLength, L"%4u FPS", m_FrameTime.FPS() );
		m_Renderer->DrawSurfaceString( m_MainScreenID, stringBuffer, 10, 0, Muspelheim::Renderer::TEXT_RIGHT );

		std::swprintf( stringBuffer, Platform::kMaxStringLength, L"VSync: %s", ( vsync ? L"enabled" : L"disabled" ) );
		m_Renderer->DrawSurfaceString( m_MainScreenID, stringBuffer, 40, 0, Muspelheim::Renderer::TEXT_RIGHT );

		glm::vec3 const & cameraForward = viewMatrix[ 2 ];
		std::swprintf( stringBuffer, Platform::kMaxStringLength, L"Camera Direction: %.3f, %.3f, %.3f", cameraForward.x, cameraForward.y, cameraForward.z );
		m_Renderer->DrawSurfaceString( m_MainScreenID, stringBuffer, 40, 1, Muspelheim::Renderer::TEXT_RIGHT );

		glm::vec3 const & cameraPosition = pCurrentCamera->GetPosition();
		std::swprintf( stringBuffer, Platform::kMaxStringLength, L"Camera Position: %.1f, %.1f, %.1f", cameraPosition.x, cameraPosition.y, cameraPosition.z );
		m_Renderer->DrawSurfaceString( m_MainScreenID, stringBuffer, 40, 2, Muspelheim::Renderer::TEXT_RIGHT );

		m_pConsole->Render();

		m_Renderer->EndRender();

		// Clear input key releases and post message buffer
		Helheimr::Input::GetInstance()->AdvanceFrame();

		// Pump the message manager to broadcast all cached messages
		m_MessageManager->Update();
	}
}

void Framework::ResizeWindow( unsigned short width, unsigned short height )
{
	if( m_Initialized )
	{
		PLATFORM_ASSERT( nullptr != m_Renderer );
		m_Renderer->SetWindowSize( width, height );
	}
}

// Encapsulate the platform dependent input processing and send results to Helheimr as generic events
void Framework::ProcessPlatformInput()
{
	unsigned int inputEventData = 0;
	unsigned int requestedInputDataSize = 0;

	if ( 0 != GetRawInputBuffer( NULL, &requestedInputDataSize, sizeof( RAWINPUTHEADER ) ) )
	{
		m_MessageManager->Post( Niflheim::Message::LOG_ERROR, L"Failed to read from raw input buffer" );
	}
	else if ( 0 != requestedInputDataSize )
	{
		// The requestedInputDataSize doesnt appear to be of any use since the min size needed tends to be much larger that this size.
		// So we just use the predetermined size instead and let it fail if the size is too small.
		requestedInputDataSize = MaxRawInputEntries * sizeof( RAWINPUTHEADER );

		unsigned int const rawInputCount = GetRawInputBuffer( reinterpret_cast< RAWINPUT* >( m_pRawInputBuffer ), &requestedInputDataSize, sizeof( RAWINPUTHEADER ) );
		if ( rawInputCount == -1 )
		{
			if ( 0x7A == GetLastError() )
			{
				m_MessageManager->Post( Niflheim::Message::LOG_ERROR, L"Raw input buffer is too small" );
			}
			else
			{
				m_MessageManager->Post( Niflheim::Message::LOG_ERROR, L"Unknown error retrieving the raw input data" );
			}
			return;
		}

		RAWINPUT const * pRawInput = reinterpret_cast<RAWINPUT*>( m_pRawInputBuffer );
		for ( unsigned int i = 0; i < rawInputCount; ++i )
		{
			switch ( pRawInput->header.dwType )
			{
				case RIM_TYPEMOUSE:
				{
					if ( pRawInput->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN )
					{
						if ( m_MouseCaptured == false )
						{
							SetCapture( m_WindowHandle );
							m_MouseCaptured = true;
							ShowCursor( FALSE );
						}

						//data = Helheimr::Input::KEY_MOUSE_RIGHT;
						//data |= Helheimr::Input::MODIFIER_KEY_PRESS;

					}
					else if ( pRawInput->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP )
					{
						if ( m_MouseCaptured == true )
						{
							ReleaseCapture();
							m_MouseCaptured = false;
							ShowCursor( TRUE );

							// Place the cursor back to the center of the window
							RECT rect;
							if ( GetWindowRect( m_WindowHandle, &rect ) )
							{
								int width = rect.right - rect.left;
								int height = rect.bottom - rect.top;

								SetCursorPos( width / 2, height / 2 );
							}
						}

						//data = Helheimr::Input::KEY_MOUSE_RIGHT;
					}

					// Ensure that the last X & Y are deltas and not absolute
					if ( m_MouseCaptured && ( pRawInput->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE ) == 0 )
					{
						long rawMouseX = pRawInput->data.mouse.lLastX;
						long rawMouseY = pRawInput->data.mouse.lLastY;

						// We will cap deltas to CHAR_MIN to CHAR_MAX for each axis
						if ( rawMouseX < CHAR_MIN || rawMouseX > CHAR_MAX || rawMouseY < CHAR_MIN || rawMouseY > CHAR_MAX )
						{
							wchar_t stringBuffer[ Platform::kMaxStringLength ];
							std::swprintf( stringBuffer, Platform::kMaxStringLength, L"Mouse deltas exceeded max range (%i, %i)", rawMouseX, rawMouseY );

							m_MessageManager->Post( Niflheim::Message::LOG_ERROR, stringBuffer );

							if ( rawMouseX < CHAR_MIN )
							{
								rawMouseX = CHAR_MIN;
							}
							else if ( rawMouseX > CHAR_MAX )
							{
								rawMouseX = CHAR_MAX;
							}

							if ( rawMouseY < CHAR_MIN )
							{
								rawMouseY = CHAR_MIN;
							}
							else if ( rawMouseY > CHAR_MAX )
							{
								rawMouseY = CHAR_MAX;
							}
						}

						// Test
						unsigned char mouseX = ( rawMouseX & UCHAR_MAX );
						unsigned char mouseY = ( rawMouseY & UCHAR_MAX );

						inputEventData |= ( mouseX << 24 );
						inputEventData |= ( mouseY << 16 );
					}
				}
				break;
				case RIM_TYPEKEYBOARD:
				{
					char stringBuffer[ 256 ];
					sprintf( stringBuffer, "VKey %u Message %u Flags %u Extra %u\n", pRawInput->data.keyboard.VKey, pRawInput->data.keyboard.Message, pRawInput->data.keyboard.Flags, pRawInput->data.keyboard.ExtraInformation );
					OutputDebugStringA( stringBuffer );

					inputEventData = pRawInput->data.keyboard.VKey;

					if ( ( pRawInput->data.keyboard.Flags & RI_KEY_BREAK ) == 0 )
					{
						inputEventData |= Helheimr::Input::MODIFIER_KEY_PRESS;
						//OutputDebugStringA( "Key Press\n" );
					}
					else
					{
						// It seems the way we are handling input has intercepted the regular generation of the WM_CLOSE event (alt-F4)
						// So we intercept that key combination here and manually post the message.
						if ( pRawInput->data.keyboard.VKey == Helheimr::Input::KEY_F4 && Helheimr::Input::GetInstance()->GetKeyDown( Helheimr::Input::KEY_ALT ) )
						{
							PostMessage( m_WindowHandle, WM_CLOSE, 0, 0 );
						}
					}
				}
				break;
			}
			pRawInput = NEXTRAWINPUTBLOCK( pRawInput );
		}

		Helheimr::Input::GetInstance()->ProcessEvent( inputEventData );
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