// INPUT.HPP

#ifndef _INPUT_HPP_
#define _INPUT_HPP_

#include "Singleton.h"

class Input : public Singleton<Input>
{
public:
	enum KeyCode 
	{
		KEY_SHIFT		= 0x10,
		KEY_PAGE_UP		= 0x21,
		KEY_PAGE_DOWN	= 0x22,
		KEY_END			= 0x23,
		KEY_HOME		= 0x24,
		KEY_ARROW_LEFT	= 0x25,
		KEY_ARROW_UP	= 0x26,
		KEY_ARROW_RIGHT = 0x27,
		KEY_ARROW_DOWN	= 0x28,
		KEY_A			= 0x41,
		KEY_D			= 0x44,
		KEY_S			= 0x53,
		KEY_W			= 0x57,
		KEY_TILDA		= 0xC0,
	};

	//
	Input();

	//
	virtual ~Input();

	//
	bool Init();

	//
	void ProcessEvent(LPARAM lParam);

	// Indicates that one frame has elapsed
	void AdvanceFrame();

	bool GetKeyUp(KeyCode keyCode);

	bool GetKeyDown(KeyCode key);

	//
	//bool GetKey( KeyCode nKey );

	////
	//bool GetMouse( long& deltaX, long& deltaY );

	////
	//void EnableMouseCapture( bool bEnable );

private:
	//
	void Shutdown();

	//void ClearKeyboard();
	//void ClearMouse();

	//LPDIRECTINPUT8 m_pDirectInput;
	//LPDIRECTINPUTDEVICE8 m_pDirectInputKeyboard;
	//LPDIRECTINPUTDEVICE8 m_pDirectInputMouse;
	//HANDLE m_hMouseEvent;

	char m_NewKeyStateBuffer[256];
	char m_OldKeyStateBuffer[256];
	//DIMOUSESTATE2 m_MouseState;
	//bool m_bMouseCaptureEnabled;

	// Provide friend access to Singleton<Input> specialization
	friend Singleton<Input>;
};

#endif // _INPUT_HPP_

