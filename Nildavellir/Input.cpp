// INPUT.CPP

#include "Platform.h"
//#include <assert.h>
//#include <windows.h>
#include <stdio.h>
#include "Input.h"
//#include "TCHAR.H"
#include <map>
#include <string>
#include "MessageManager.h"

//std::map<string, bool> myKey;
//bool Input::GetRawKeyboardData(LPARAM lParam)
//{
//	char buffer[sizeof(RAWINPUT)];
//	UINT size = sizeof(RAWINPUT);
//
//	GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER));
//
//	RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer);
//	if(raw->header.dwType == RIM_TYPEKEYBOARD)
//	{
//		const RAWKEYBOARD& rawKeyboard = raw->data.keyboard;
//
//		unsigned int scanCode = rawKeyboard.MakeCode;
//		unsigned int flags = rawKeyboard.Flags;
//		const bool E0 = ((flags & RI_KEY_E0) != 0);
//		const bool E1 = ((flags & RI_KEY_E1) != 0);
//		const bool KeyDown = !((flags & RI_KEY_BREAK) != 0);
//
//		UINT key = (scanCode << 16) | (E0 << 24);
//		char stringBuffer[32];
//		GetKeyNameText((LONG)key, stringBuffer, 32);
//		if(KeyDown) // Press
//		{
//			myKey[stringBuffer] = true;
//		}
//		else // Release
//		{
//			myKey[stringBuffer] = false;
//		}
//	}
//	return true;
//}

/*
bool KEYBOARD_INPUT::KeyPressed(string key, int mode)
{
	if(mode == ONLYONCE)
	{
		if(myKey[key] && pressed_onlyonce[key] == false)
		{
			pressed_onlyonce[key] = true;
			return true;
		}
		if(!myKey[key])
		{
			pressed_onlyonce[key] = false;
			return false;
		}
	}

	else if(mode == CONTINUOUS)
	{
		if(myKey[key] == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	else if(mode == TOGGLE)
	{
		if(myKey[key] && pressed_toggle[key] == false)
		{
			pressed_toggle[key] = true;
			released_toggle[key] = !released_toggle[key];
			return released_toggle[key];
		}
		else if(!myKey[key])
		{
			pressed_toggle[key] = false;
			return released_toggle[key];
		}
	}
	return false;
}
*/

Input::Input()
{
}

Input::~Input()
{
}

bool Input::Init()
{
	ZeroMemory(&m_OldKeyStateBuffer, sizeof(m_OldKeyStateBuffer));
	ZeroMemory(&m_NewKeyStateBuffer, sizeof(m_NewKeyStateBuffer));

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
	Rid.usUsage = 0x06;			// Adds HID keyboard
	Rid.dwFlags = 0;			// Change to RIDEV_NOLEGACY to ignore legacy keyboard messages
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

void Input::ProcessEvent(LPARAM lParam)
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

	LPBYTE lpb = new BYTE[dwSize];
	PLATFORM_ASSERT( lpb != nullptr );

	if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
	{
		MessageManager::GetInstance()->Post( Message::LOG_ERROR, std::wstring( L"GetRawInputData does not return correct size!" ) );
	}

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		m_NewKeyStateBuffer[raw->data.keyboard.VKey] = (raw->data.keyboard.Flags & RI_KEY_BREAK) ? 0 : 1;
		//TCHAR stringBuffer[256];
		//_stprintf_s(stringBuffer, 256, _T(" Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n"),
		//	raw->data.keyboard.MakeCode,
		//	raw->data.keyboard.Flags,
		//	raw->data.keyboard.Reserved,
		//	raw->data.keyboard.ExtraInformation,
		//	raw->data.keyboard.Message,
		//	raw->data.keyboard.VKey);

		//OutputDebugString(stringBuffer);
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

	delete [] lpb;
}

void Input::AdvanceFrame()
{
	memcpy(m_OldKeyStateBuffer, m_NewKeyStateBuffer, sizeof(m_NewKeyStateBuffer));
}

bool Input::GetKeyUp(KeyCode key)
{
	return (m_OldKeyStateBuffer[key] != 0 && m_NewKeyStateBuffer[key] == 0);
}

bool Input::GetKeyDown(KeyCode key)
{
	return (m_NewKeyStateBuffer[key] != 0);
}

//void Input::ClearKeyboard()
//{
//	ZeroMemory( &m_KeyStateBuffer, sizeof( m_KeyStateBuffer ) );
//}
//
//void Input::ClearMouse()
//{
//	ZeroMemory( &m_MouseState, sizeof( m_MouseState ) );
//}
//
//bool Input::GetKey( KeyCode nKey )
//{
//	unsigned char nIndex;
//
//	switch( nKey )
//	{
//	case KEY_ARROW_RIGHT: 
//		nIndex = DIK_RIGHT;
//		break;
//	case KEY_ARROW_LEFT:
//		nIndex = DIK_LEFT;
//		break;
//	case KEY_ARROW_UP:
//		nIndex = DIK_UP;
//		break;
//	case KEY_ARROW_DOWN:
//		nIndex = DIK_DOWN;
//		break;
//	case KEY_W: 
//		nIndex = DIK_W;
//		break;
//	case KEY_A: 
//		nIndex = DIK_A;
//		break;
//	case KEY_S: 
//		nIndex = DIK_S;
//		break;
//	case KEY_D: 
//		nIndex = DIK_D;
//		break;
//	case KEY_TILDA:
//		nIndex = DIK_GRAVE;
//		break;
//	default:
//		return false;
//	}
//
//	return ( ( m_KeyStateBuffer[ nIndex ] & 0x80 ) != 0 );
//}
//
//bool Input::GetMouse( long& deltaX, long& deltaY )
//{
//	deltaX = m_MouseState.lX;
//	deltaY = m_MouseState.lY;
//
//	return true;
//}
//
//void Input::EnableMouseCapture( bool bEnable )
//{
//	if( bEnable == false )
//	{
//		if( m_bMouseCaptureEnabled && m_pDirectInputMouse != NULL)
//		{
//			// Unacquire mouse
//			m_pDirectInputMouse->Unacquire();
//		}
//	}
//
//	m_bMouseCaptureEnabled = bEnable;
//}
