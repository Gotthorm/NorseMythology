// INPUT.HPP

#ifndef _INPUT_HPP_
#define _INPUT_HPP_

#include "Singleton.h"
#include <string>

class Input : public Singleton<Input>
{
public:
	enum KeyCode
	{
		KEY_BACKSPACE		= 0x08,
		KEY_RETURN			= 0x0D,
		KEY_SHIFT			= 0x10,
		KEY_CONTROL			= 0x11,
		KEY_ALT				= 0x12,
		KEY_CAPS_LOCK		= 0x14,
		KEY_SPACE			= 0x20,
		KEY_PAGE_UP			= 0x21,
		KEY_PAGE_DOWN		= 0x22,
		KEY_END				= 0x23,
		KEY_HOME			= 0x24,
		KEY_ARROW_LEFT		= 0x25,
		KEY_ARROW_UP		= 0x26,
		KEY_ARROW_RIGHT		= 0x27,
		KEY_ARROW_DOWN		= 0x28,
		KEY_0				= 0x30,
		KEY_1				= 0x31,
		KEY_2				= 0x32,
		KEY_3				= 0x33,
		KEY_4				= 0x34,
		KEY_5				= 0x35,
		KEY_6				= 0x36,
		KEY_7				= 0x37,
		KEY_8				= 0x38,
		KEY_9				= 0x39,
		KEY_A				= 0x41,
		KEY_D				= 0x44,
		KEY_S				= 0x53,
		KEY_W				= 0x57,
		KEY_Z				= 0x5A,
		KEY_NUMPAD_0		= 0x60,
		KEY_NUMPAD_9		= 0x69,
		KEY_F4				= 0x73,
		KEY_TILDA			= 0xC0,
		KEY_SEMICOLON		= 0xBA,
		KEY_EQUALS			= 0xBB,
		KEY_COMMA			= 0xBC,
		KEY_MINUS			= 0xBD,
		KEY_PERIOD			= 0xBE,
		KEY_FORWARD_SLASH	= 0xBF,
		KEY_SINGLE_QUOTE	= 0xDE,
	};

	enum 
	{
		MASK_KEY_VALUE		= 0x00FF,
		MASK_KEY_PRESS		= 0x0100,
		MASK_KEY_SHIFT		= 0x0200,
		MASK_KEY_CONTROL	= 0x0400,
	};

	// Use the default implementation
	Input() = default;

	//
	virtual ~Input();

	//
	bool Init();

	//
	void ProcessEvent(unsigned int eventData);

	// Indicates that one frame has elapsed
	void AdvanceFrame();

	bool GetKeyUp(KeyCode keyCode);

	bool GetKeyDown(KeyCode key);

	//
	//bool GetKey( KeyCode nKey );

	//
	//bool GetMouse( long& deltaX, long& deltaY );

	//
	//void EnableMouseCapture( bool bEnable );

private:
	// Remove the default assignment operator
	Input & operator =( const Input & ) = delete;

	// Remove the copy constructor
	Input( const Input & ) = delete;

	//
	void Shutdown();

	static const unsigned int m_KeyCodeCount = 256;

	char m_NewKeyStateBuffer[ m_KeyCodeCount ];
	char m_OldKeyStateBuffer[ m_KeyCodeCount ];

	// Provide friend access to Singleton<Input> specialization
	friend Singleton<Input>;
};

#endif // _INPUT_HPP_

