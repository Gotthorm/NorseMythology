// MESSAGECLIENT.H

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <windows.h>

class Message
{
public:
	enum MessageType
	{
		LOG_INFO,
		LOG_WARN,
		LOG_ERROR,

		// Must be the last entry
		MESSAGETYPE_COUNT
	};

	enum
	{
		MAX_STRING_LENGTH = 512,
	};

	MessageType Type;

	union
	{
		float floatData;
		int intData;
		std::wstring* stringData;
	};
};

#endif // _MESSAGE_H_

