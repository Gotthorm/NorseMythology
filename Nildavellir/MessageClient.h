// MESSAGECLIENT.H

#ifndef _MESSAGECLIENT_H_
#define _MESSAGECLIENT_H_

#include <windows.h>
#include "Message.h"

class MessageClient
{
public:

	virtual void ReceiveMessage( const Message& message ) = 0;

private:
	// A messageId queue

	// A message string queue
};

#endif // _MESSAGECLIENT_H_

