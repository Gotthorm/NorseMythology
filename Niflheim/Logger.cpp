#include <iomanip>
#include <time.h>
#include <assert.h>
#include "Niflheim.h"

namespace Niflheim
{
	Logger::Logger( const std::weak_ptr<MessageManager>& messageManager ) : MessageClient( messageManager )
	{
	}

	Logger::~Logger()
	{
		Shutdown();
	}

	bool Logger::Initialize( const std::wstring& fileName )
	{
		Shutdown();

		// First ensure the log file can be opened
		m_FileOutput.open( fileName );

		// Check the results to ensure the file was actually opened successfully
		if( ( m_FileOutput.rdstate() & std::ifstream::failbit ) == 0 )
		{
			// Register for the correct messages
			std::shared_ptr<MessageManager> messageManager = m_MessageManager.lock();
			if( messageManager )
			{
				messageManager->Register( this, Message::LOG_INFO );
				messageManager->Register( this, Message::LOG_WARN );
				messageManager->Register( this, Message::LOG_ERROR );
			}

			return true;
		}

		return false;
	}

	void Logger::Shutdown()
	{
		if( m_FileOutput.is_open() )
		{
			m_FileOutput.close();

			std::shared_ptr<MessageManager> messageManager = m_MessageManager.lock();
			if( messageManager )
			{
				messageManager->Deregister( this, Message::LOG_INFO );
				messageManager->Deregister( this, Message::LOG_WARN );
				messageManager->Deregister( this, Message::LOG_ERROR );
			}
		}
	}

	void Logger::ReceiveMessage( const Message& message )
	{
		struct tm timeStructure;
		time_t timeValue;

		// Get time as 64-bit integer.  
		time( &timeValue );

		errno_t errorCode = localtime_s( &timeStructure, &timeValue );

		if( errorCode == 0 )
		{
			errorCode = asctime_s( m_TimeStampBuffer, TIMESTAMP_BUFFER_SIZE, &timeStructure );
		
			// asctime_s appends the string with a carriage return which we do not want
			m_TimeStampBuffer[ TIMESTAMP_BUFFER_SIZE - 2 ] = ' ';
		}

		if( errorCode != 0 )
		{
			strcpy_s( m_TimeStampBuffer, TIMESTAMP_BUFFER_SIZE, "00/00/00 00:00:00 " );
		}

		std::wstring typePrefix;

		switch( message.Type )
		{
		case Message::LOG_INFO:
			typePrefix = L"INFO: ";
			break;
		case Message::LOG_WARN:
			typePrefix = L"WARNING: ";
			break;
		case Message::LOG_ERROR:
			typePrefix = L"ERROR: ";
			break;
		default:
			// Invalid message type
			return;
		}

		assert( message.stringData );

		// Write to file
		m_FileOutput << m_TimeStampBuffer << typePrefix << message.stringData->c_str() << std::endl;
	}
}