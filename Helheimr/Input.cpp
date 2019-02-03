// INPUT.CPP

//#include "Platform.h"
//#include <assert.h>
#include <stdio.h>
#include "Helheimr.h"
#include <map>
#include <string>
#include "Niflheim.h"
#include "Platform.h"

namespace Helheimr
{
	Input::~Input()
	{
		Shutdown();
	}

	bool Input::Init()
	{
		memset( m_OldKeyStateBuffer, 0, m_KeyCodeCount * sizeof( char ) );
		memset( m_NewKeyStateBuffer, 0, m_KeyCodeCount * sizeof( char ) );

		m_MouseMovementDeltaX = 0.0f;
		m_MouseMovementDeltaY = 0.0f;

		return true;
	}

	void Input::Shutdown()
	{
	}

	void Input::ProcessEvent( unsigned int eventData )
	{
		unsigned short keyValue = eventData & MASK_KEY_VALUE;
		bool keyRelease = ( eventData & MODIFIER_KEY_PRESS ) == 0;

		m_NewKeyStateBuffer[ keyValue ] = keyRelease ? 0 : 1;

		// Broadcast key strokes
		if( keyRelease )
		{
			unsigned int inputData = keyValue;

			if( GetKeyDown( KEY_SHIFT ) || GetKeyDown( KEY_CAPS_LOCK ) )
			{
				inputData |= MODIFIER_KEY_SHIFT;
			}

			std::shared_ptr<Niflheim::MessageManager> messageManager = m_MessageManager.lock();
			if( messageManager )
			{
				messageManager->Post( Niflheim::Message::KEY_STROKES, inputData );
			}
		}

		if( eventData & MASK_MOUSE_DELTA_X )
		{
			char delta = ( char )( ( eventData & MASK_MOUSE_DELTA_X ) >> 24 );
			m_MouseMovementDeltaX += delta * m_MouseSpeed;
		}

		if( eventData & MASK_MOUSE_DELTA_Y )
		{
			char delta = (char)( ( eventData & MASK_MOUSE_DELTA_Y ) >> 16 );
			m_MouseMovementDeltaY += delta * m_MouseSpeed;
		}
	}

	void Input::AdvanceFrame()
	{
		memcpy( m_OldKeyStateBuffer, m_NewKeyStateBuffer, m_KeyCodeCount * sizeof( char ) );

		//if( m_MouseMovementDeltaX || m_MouseMovementDeltaY )
		//{
		//	wchar_t stringBuffer[ Platform::kMaxStringLength ];
		//	std::swprintf( stringBuffer, Platform::kMaxStringLength, L"Mouse deltas detected (%i, %i)\n", m_MouseMovementDeltaX, m_MouseMovementDeltaY );
		//	OutputDebugString( stringBuffer );
		//}

		m_MouseMovementDeltaX = 0.0f;
		m_MouseMovementDeltaY = 0.0f;
	}

	bool Input::GetKeyUp( KeyCode key ) const
	{
		return ( ( m_OldKeyStateBuffer[ key ] != 0 ) && ( m_NewKeyStateBuffer[ key ] == 0 ) );
	}

	bool Input::GetKeyDown( KeyCode key ) const
	{
		return ( m_NewKeyStateBuffer[ key ] != 0 );
	}

	void Input::AddMessaging( const std::weak_ptr<Niflheim::MessageManager>& messageManager )
	{
		m_MessageManager = messageManager;
	}

	bool Input::GetMouse( int& deltaX, int& deltaY ) const
	{
		deltaX = static_cast<int>( m_MouseMovementDeltaX );
		deltaY = static_cast<int>( m_MouseMovementDeltaY );

		return true;
	}

}
