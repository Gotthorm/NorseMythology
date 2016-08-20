// GAME.CPP

#include <assert.h>
#include <windows.h>
#include <stdio.h>
#include "Game.h"
#include "TCHAR.H"
#include "Camera.h"
//#include "Input.h"


Game::Game() : m_pCurrentCamera( NULL )//, m_pInput( NULL )
{
}

Game::~Game()
{
	//assert( m_pInput == NULL );
	assert( m_pCurrentCamera == NULL );
}

bool Game::Init( HINSTANCE hInstance, HWND hWindow )
{
	//assert( m_pInput == NULL );
	//m_pInput = new Input();
	//assert( m_pInput != NULL );

 //   if( m_pInput->Init( hInstance, hWindow ) == false )
	//{
 //       OutputDebugString( _T("Cannot initialize the input system\n") );
 //       return false;
	//}

	//m_pCurrentCamera = new Camera();

	//m_pCurrentCamera->Set( D3DXVECTOR3( -5.0f, 0.5f, 0.0f ), 0.0f, 0.0f );

	static const float data[] = 
	{ 
		 0.25, -0.25, 0.5, 1.0,
		-0.25, -0.25, 0.5, 1.0,
		 0.25,  0.25, 0.5, 1.0 
	};

	OpenGLInterface::CreateBuffers(1, &m_BufferName);

	OpenGLInterface::NamedBufferStorage(m_BufferName, 1024 * 1024, NULL, GL_MAP_WRITE_BIT);

	OpenGLInterface::BindBuffer(GL_ARRAY_BUFFER, m_BufferName);

	OpenGLInterface::BufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data);

	return true;
}

void Game::Shutdown()
{
	if( m_pCurrentCamera )
	{
		delete m_pCurrentCamera;
		m_pCurrentCamera = NULL;
	}

	//if( m_pInput )
	//{
	//	m_pInput->Shutdown();
	//	delete m_pInput;
	//	m_pInput = NULL;
	//}
}

void Game::Update()
{
	//m_pInput->Update();

	//if (m_pInput)
	//{
	//	if (m_pInput->GetKey(Input::KEY_TILDA))
	//	{

	//	}
	//}

	//if( m_pInput && m_pCurrentCamera )
	//{
	//	if( m_pInput->GetKey( Input::KEY_ARROW_UP ) || m_pInput->GetKey( Input::KEY_W ) )
	//	{
	//		//m_pCurrentCamera->Move( 0.01f );
	//	}
	//	if( m_pInput->GetKey( Input::KEY_ARROW_DOWN ) || m_pInput->GetKey( Input::KEY_S ) )
	//	{
	//		//m_pCurrentCamera->Move( -0.01f );
	//	}
	//	if( m_pInput->GetKey( Input::KEY_ARROW_LEFT ) || m_pInput->GetKey( Input::KEY_A ) )
	//	{
	//		//m_pCurrentCamera->Slide( 0.01f, 0.0f );
	//	}
	//	if( m_pInput->GetKey( Input::KEY_ARROW_RIGHT ) || m_pInput->GetKey( Input::KEY_D ) )
	//	{
	//		//m_pCurrentCamera->Slide( -0.01f, 0.0f );
	//	}

	//	long deltaX;
	//	long deltaY;

	//	if( m_pInput->GetMouse( deltaX, deltaY ) )
	//	{
	//		//TCHAR theString[ 256 ];
	//		//_stprintf( theString, L"Mouse X( %d )  Mouse Y( %d )\n", deltaX, deltaY );
	//		//OutputDebugString( theString );

	//		//m_pCurrentCamera->Rotate( deltaX * 0.001f, deltaY * 0.001f );
	//	}
	//}
}

void Game::EnableMouseCapture( bool bEnable ) 
{ 
	//m_pInput->EnableMouseCapture( bEnable ); 
}
