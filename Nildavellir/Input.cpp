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

/*
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
	Rid[0].hwndTarget = 0;

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
	Rid[1].hwndTarget = 0;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE) 
	{
		//registration failed. Call GetLastError for the cause of the error
		return false;
	}
*/

	RAWINPUTDEVICE Rid;

	Rid.usUsagePage = 0x01;
	Rid.usUsage = 0x06;			
	Rid.dwFlags = RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
	Rid.hwndTarget = 0;

	if (RegisterRawInputDevices(&Rid, 1, sizeof(Rid)) == FALSE)
	{
		//registration failed. Call GetLastError for the cause of the error
		return false;
	}

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

		if( GetKeyDown( KEY_SHIFT ) )
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

