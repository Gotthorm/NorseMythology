// MathLibrary.h - Contains declaration of Function class  
#pragma once  

#ifdef MAKE_DLL  
#define EXPORT __declspec(dllexport)   
#else  
#define EXPORT __declspec(dllimport)   
#endif 

#include <string>
#include <vector>
#include <array>
#include <memory>
#include <fstream>

namespace Niflheim
{
	class EXPORT MessageManager;

	/// \brief Generic message object
	///
	/// All systems can communicate with each other internally by sending message.
	/// These messages are very simple and are managed by the MessageManager.
	class EXPORT Message
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
			KEY_STROKES,

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
			int uintData;				///< A generic unsigned integer value
			std::wstring* stringData;	///< The storage for this string is handled inside of the MessageManager
		};

	private:
		// Remove the default assignment operator
		Message & operator =( const Message & ) = delete;

		// Remove the copy constructor
		Message( const Message & ) = delete;
	};

	/// \brief The abstract class for receiving messages.
	///
	/// The MessageClient is the base class for any object that wants to receive messages.
	/// Classes that derive from MessageClient must implement the ReceiveMessage method.
	class EXPORT MessageClient
	{
	public:
		MessageClient( const std::weak_ptr<MessageManager>& messageManager ) : m_MessageManager( messageManager ) {}

		virtual ~MessageClient() { }

		/// \brief The message handler.
		/// 
		/// This is by the messaging system whenever a message is posted that matches any of the types registed by this instance.
		/// \param message A reference to the posted message.
		virtual void ReceiveMessage( const Message& message ) = 0;

	protected:
		std::weak_ptr<MessageManager> m_MessageManager;
	};

	class EXPORT MessageManager
	{
	public:
		//
		MessageManager();

		// 
		virtual ~MessageManager();

		void Post( Message::MessageType type, unsigned int message );
		void Post( Message::MessageType type, float message );
		void Post( Message::MessageType type, const std::wstring& message );

		// Deliver all queued messages and update all internal data
		void Update();

		//void PostImmediate( LogType type );

		void Register( MessageClient* client, Message::MessageType type );
		void Deregister( MessageClient* client, Message::MessageType type );

	private:
		enum
		{
			// This system will reject more than 256 messages per update
			MAX_MESSAGES = 256,
		};

		// Remove the default assignment operator
		MessageManager & operator =( const MessageManager & ) = delete;

		// Remove the copy constructor
		MessageManager( const MessageManager & ) = delete;

		//
		void BroadcastWarning( const wchar_t* message );

		// A message table
		std::array<Message, MAX_MESSAGES> m_MessageTable;
		unsigned int m_MessageCount;

		// A string cache table
		std::array<std::wstring, MAX_MESSAGES> m_StringTable;

		// The table of registered message clients
		std::array<std::vector<MessageClient*>, Message::MESSAGETYPE_COUNT> m_RegisteredClients;
	};

	class EXPORT Logger : public MessageClient
	{
	public:
		Logger( const std::weak_ptr<MessageManager>& messageManager );

		virtual ~Logger();

		bool Initialize( const std::wstring& fileName );

		void Shutdown();

		// Override MessageClient::ReceiveMessage
		virtual void ReceiveMessage( const Message& message );

	private:
// There is a warning generated here for not exporting the m_FileOutput attribute here
// Since it is private and never exposed, I assume this is a bullshit warning.
// I would like to find a better solution to this.
#pragma warning( push )
#pragma warning( disable : 4251)
		std::wofstream m_FileOutput;
#pragma warning( pop ) 

		enum { TIMESTAMP_BUFFER_SIZE = 26 };
		char m_TimeStampBuffer[ TIMESTAMP_BUFFER_SIZE ];
	};
}