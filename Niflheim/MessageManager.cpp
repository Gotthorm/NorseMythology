// MathLibrary.cpp : Defines the exported functions for the DLL application.  
// Compile by using: cl /EHsc /DMATHLIBRARY_EXPORTS /LD MathLibrary.cpp  

#include "Niflheim.h"  
#include <assert.h>

namespace Niflheim
{
	MessageManager::MessageManager() : m_MessageCount( 0 )
	{
		// Set up the string cache
		for( unsigned int index = 0; index < MAX_MESSAGES; ++index )
		{
			m_StringTable[ index ].resize( Message::MAX_STRING_LENGTH );
		}
	}

	MessageManager::~MessageManager()
	{
		// Assert if any clients are still registered
		for( int typeIndex = 0; typeIndex < Message::MessageType::MESSAGETYPE_COUNT; ++typeIndex )
		{
			assert( m_RegisteredClients[ typeIndex ].size() == 0 );
		}
	}

	void MessageManager::Post( Message::MessageType type, unsigned int message )
	{
		if( m_MessageCount < MAX_MESSAGES )
		{
			m_MessageTable[ m_MessageCount ].Type = type;
			m_MessageTable[ m_MessageCount ].uintData = message;

			++m_MessageCount;
		}
		else
		{
			// Broadcast immediate warning message
			BroadcastWarning( L"Exceeded maximum message count!" );
		}
	}

	void MessageManager::Post( Message::MessageType type, float message )
	{
		if( m_MessageCount < MAX_MESSAGES )
		{
			m_MessageTable[ m_MessageCount ].Type = type;
			m_MessageTable[ m_MessageCount ].floatData = message;

			++m_MessageCount;
		}
		else
		{
			// Broadcast immediate warning message
			BroadcastWarning( L"Exceeded maximum message count!" );
		}
	}

	void MessageManager::Post( Message::MessageType type, const std::wstring& message )
	{
		if( m_MessageCount < MAX_MESSAGES )
		{
			m_MessageTable[ m_MessageCount ].Type = type;

			// Check the length of the string and broadcast a warning if we will be truncating it
			size_t stringLength = message.size();
			if( stringLength > Message::MAX_STRING_LENGTH )
			{
				// Broadcast warning
				BroadcastWarning( L"Exceeded maximum message string length!" );

				stringLength = Message::MAX_STRING_LENGTH;
			}

			// Copy the string to the string cache table using the same index as the message
			wcsncpy_s( &m_StringTable[ m_MessageCount ][ 0 ], Message::MAX_STRING_LENGTH, message.c_str(), stringLength );

			// Ensure there is a null terminator in the string after truncating it
			if( message.size() > Message::MAX_STRING_LENGTH )
			{
				m_StringTable[ m_MessageCount ][ Message::MAX_STRING_LENGTH - 1 ] = 0;
			}

			m_MessageTable[ m_MessageCount ].stringData = &m_StringTable[ m_MessageCount ];

			++m_MessageCount;
		}
		else
		{
			// Broadcast immediate warning message
			BroadcastWarning( L"Exceeded maximum message count!" );
		}
	}

	void MessageManager::Update()
	{
		// Broadcast all messages
		for( unsigned int messageIndex = 0; messageIndex < m_MessageCount; ++messageIndex )
		{
			int typeIndex = m_MessageTable[ messageIndex ].Type;

			const std::vector<MessageClient*>& clientList = m_RegisteredClients[ typeIndex ];

			for( std::vector<MessageClient*>::const_iterator it = clientList.begin(); it != clientList.end(); ++it )
			{
				( (MessageClient*)( *it ) )->ReceiveMessage( m_MessageTable[ messageIndex ] );
			}
		}

		m_MessageCount = 0;
	}

	void MessageManager::Register( MessageClient* client, Message::MessageType type )
	{
		assert( client );
		assert( type < (int)m_RegisteredClients.size() );

		std::vector<MessageClient*>& clientList = m_RegisteredClients[ type ];

		std::vector<MessageClient*>::iterator it = std::find( clientList.begin(), clientList.end(), client );

		if( it == clientList.end() )
		{
			// Add the client
			clientList.push_back( client );
		}
	}

	void MessageManager::Deregister( MessageClient* client, Message::MessageType type )
	{
		assert( client );
		assert( type < (int)m_RegisteredClients.size() );

		std::vector<MessageClient*>& clientList = m_RegisteredClients[ type ];

		std::vector<MessageClient*>::iterator it = std::find( clientList.begin(), clientList.end(), client );

		if( it != clientList.end() )
		{
			// Add the client
			clientList.erase( it );
		}
	}

	void MessageManager::BroadcastWarning( const wchar_t* message )
	{
		Message warningMessage;
		std::wstring warningString( message );

		const std::vector<MessageClient*>& clientList = m_RegisteredClients[ Message::LOG_WARN ];

		warningMessage.Type = Message::LOG_WARN;
		warningMessage.stringData = &warningString;

		for( std::vector<MessageClient*>::const_iterator it = clientList.begin(); it != clientList.end(); ++it )
		{
			( (MessageClient*)( *it ) )->ReceiveMessage( warningMessage );
		}
	}
}