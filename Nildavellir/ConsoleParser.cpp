// CONSOLEPARSER.CPP

#include "ConsoleParser.h"
#include "MessageManager.h"
#include <sstream>

// Some command brain storming
// log <string>
// log_warn <string>
// log_error <string>
// set <variablename> [newvalue]
// quit
// restart
// reload 

enum
{
	LOG,
	LOG_WARN,
	LOG_ERROR,
};

void ConsoleParser::Execute( const std::wstring& commandLine )
{
	std::vector<std::wstring> tokens; // Create vector to hold our words

	if( Tokenize( commandLine, tokens ) > 0 )
	{
		std::unordered_map<std::wstring, unsigned int>::const_iterator commandToken = m_CommandTable.find( tokens[ 0 ] );

		if( commandToken == m_CommandTable.end() )
		{
			std::wstring warningMessage = L"Unrecognized command: ";
			warningMessage += tokens[ 0 ];
			MessageManager::GetInstance()->Post( Message::LOG_WARN, warningMessage.c_str() );
		}
	}
}

int ConsoleParser::Tokenize( const std::wstring& inputString, std::vector<std::wstring>& tokens )
{
	// For convienence we tally the token count and use as the return value
	// This way we can also append to non-empty vectors if needed
	int tokenCount = 0;

	// Have a temp buffer string
	std::wstring bufferString; 

	// Insert the string into a stream
	std::wstringstream inputStream( inputString );

	while( inputStream >> bufferString )
	{
		++tokenCount;
		tokens.push_back( bufferString );
	}

	return tokenCount;
}