// INPUT.CPP

#include "Platform.h"
//#include <assert.h>
#include <stdio.h>
#include "Input.h"
#include <map>
#include <string>
#include "MessageManager.h"

Input::~Input()
{
	Shutdown();
}

bool Input::Init()
{
	memset( m_OldKeyStateBuffer, 0, m_KeyCodeCount * sizeof( char ) );
	memset( m_NewKeyStateBuffer, 0, m_KeyCodeCount * sizeof( char ) );

	return true;
}

void Input::Shutdown()
{
}

void Input::ProcessEvent( unsigned int eventData )
{
	unsigned short keyValue = eventData & MASK_KEY_VALUE;
	bool keyRelease = (eventData & MASK_KEY_PRESS) == 0;

	m_NewKeyStateBuffer[ keyValue ] = keyRelease ? 0 : 1;

	// Broadcast key strokes
	if( keyRelease )
	{
		unsigned int inputData = keyValue;

		if( GetKeyDown( KEY_SHIFT ) || GetKeyDown( KEY_CAPS_LOCK ) )
		{
			inputData |= MASK_KEY_SHIFT;
		}

		MessageManager::GetInstance()->Post( Message::KEY_STROKES, inputData );
	}
}

void Input::AdvanceFrame()
{
	memcpy( m_OldKeyStateBuffer, m_NewKeyStateBuffer, m_KeyCodeCount * sizeof( char ) );
}

bool Input::GetKeyUp( KeyCode key )
{
	return ( ( m_OldKeyStateBuffer[ key ] != 0 ) && ( m_NewKeyStateBuffer[ key ] == 0 ) );
}

bool Input::GetKeyDown( KeyCode key )
{
	return ( m_NewKeyStateBuffer[ key ] != 0 );
}

