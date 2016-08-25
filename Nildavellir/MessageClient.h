// MESSAGECLIENT.H

#ifndef _MESSAGECLIENT_H_
#define _MESSAGECLIENT_H_

#include <windows.h>
#include "Message.h"

/// \brief The abstract class for receiving messages.
///
/// The MessageClient is the base class for any object that wants to receive messages.
/// Classes that derive from MessageClient must implement the ReceiveMessage method.
class MessageClient
{
public:
	virtual ~MessageClient() { }

	/// \brief The message handler.
	/// 
	/// This is by the messaging system whenever a message is posted that matches any of the types registed by this instance.
	/// \param message A reference to the posted message.
	virtual void ReceiveMessage( const Message& message ) = 0;
};

#endif // _MESSAGECLIENT_H_

