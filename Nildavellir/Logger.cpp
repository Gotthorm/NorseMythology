#include "Logger.h"
#include <iomanip>
#include <ctime>
#include "MessageManager.h"

Logger::Logger()
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
		MessageManager::GetInstance()->Register( this, Message::LOG_INFO );
		MessageManager::GetInstance()->Register( this, Message::LOG_WARN );
		MessageManager::GetInstance()->Register( this, Message::LOG_ERROR );

		return true;
	}

	return false;
}

void Logger::Shutdown()
{
	if( m_FileOutput.is_open() )
	{
		m_FileOutput.close();

		MessageManager::GetInstance()->Deregister( this, Message::LOG_INFO );
		MessageManager::GetInstance()->Deregister( this, Message::LOG_WARN );
		MessageManager::GetInstance()->Deregister( this, Message::LOG_ERROR );
	}
}

void Logger::ReceiveMessage( const Message& message )
{
	std::time_t t = std::time( nullptr );
	std::tm tm = *std::localtime( &t );

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

	assert(message.stringData);

	// Write to file
	m_FileOutput << std::put_time( &tm, L"%c " ) << typePrefix << message.stringData->c_str() << std::endl;
}