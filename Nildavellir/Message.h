// MESSAGE.H

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>

/// \brief Generic message object
///
/// All systems can communicate with each other internally by sending message.
/// These messages are very simple and are managed by the MessageManager.
class Message
{
public:
	// Use the default implementation
	Message() = default;

	// Use the default implementation
	virtual ~Message() = default;

	/// The list of message types.
	enum MessageType
	{
		LOG_INFO,
		LOG_WARN,
		LOG_ERROR,

		// Must be the last entry
		MESSAGETYPE_COUNT
	};

	/// The maximum length of string that a message may contain.
	enum
	{
		MAX_STRING_LENGTH = 512,
	};

	MessageType Type;

	/// \union
	/// \brief The storage parameters for the allowed message data types.
	union
	{
		float floatData;			///< A generic float value
		int intData;				///< A generic integer value
		std::wstring* stringData;	///< The storage for this string is handled inside of the MessageManager
	};

private:
	// Remove the default assignment operator
	Message & operator =( const Message & ) = delete;

	// Remove the copy constructor
	Message( const Message & ) = delete;
};

#endif // _MESSAGE_H_

