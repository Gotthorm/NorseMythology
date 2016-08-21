// MESSAGEMANAGER.H

#ifndef _MESSAGEMANAGER_H_
#define _MESSAGEMANAGER_H_

#include <windows.h>
#include "Singleton.h"
#include <string>
#include <vector>
#include "Message.h"
#include <array>

// Forward declarations
class MessageClient;

class MessageManager : public Singleton<MessageManager>
{
public:
	void Post( Message::MessageType type, int message );
	void Post( Message::MessageType type, float message );
	void Post( Message::MessageType type, const std::wstring& message );

	// Deliver all queued messages and update all internal data
	void Update();

	//void PostImmediate( LogType type );

	void Register( MessageClient* client, Message::MessageType type );
	void Deregister( MessageClient* client, Message::MessageType type );

	// Provide friend access to Singleton<MessageManager> specialization
	friend Singleton<MessageManager>; 

private:
	enum
	{
		// This system will reject more than 256 messages per update
		MAX_MESSAGES = 256,
	};

	MessageManager();

	void BroadcastWarning(const wchar_t* message);

	// A message table
	std::array<Message, MAX_MESSAGES> m_MessageTable;
	unsigned int m_MessageCount;

	// A string cache table
	std::array<std::wstring, MAX_MESSAGES> m_StringTable;

	// The table of registered message clients
	std::array<std::vector<MessageClient*>, Message::MESSAGETYPE_COUNT> m_RegisteredClients;
};

#endif // _MESSAGEMANAGER_H_

