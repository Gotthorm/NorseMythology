// CONSOLEPARSER.CPP

#include "ConsoleParser.h"
#include "Niflheim.h"
#include <sstream>
#include "ConsoleCommandManager.h"

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
	std::wstring commandBuffer = commandLine;
	std::wstring commandToken;

	if( GetCommandToken( commandBuffer, commandToken ) )
	{
		ConsoleParameterList paramList;
		if( ConsoleCommandManager::GetInstance()->GetParameterList( commandToken, paramList ) )
		{
			int paramCount = paramList.GetCount();

			std::vector<std::wstring> tokenList;

			int tokenCount = Tokenize( commandBuffer, tokenList );

			// We are going to allow excess parameters, we will ignore the extras but generate a warning
			if( tokenCount > paramCount )
			{
				std::wstringstream stringStream;
				stringStream << L"Excess parameters for console command: \"" << commandToken << L"\" ";
				stringStream << L"Was expecting: " << paramCount << L" and found: " << tokenCount;
				Niflheim::MessageManager::GetInstance()->Post( Niflheim::Message::LOG_WARN, stringStream.str() );
			}

			if( tokenCount >= paramCount )
			{
				for( int paramIndex = 0; paramIndex < paramCount; ++paramIndex )
				{
					try
					{
						switch( paramList.GetParameterType( paramIndex ) )
						{
						case ConsoleParameter::ParameterType::FLOAT:
						{
							float floatParam = std::stof( tokenList[ paramIndex ] );
							paramList.SetParameterValue( paramIndex, floatParam );
							break;
						}
						case ConsoleParameter::ParameterType::INT:
						{
							int intParam = std::stoi( tokenList[ paramIndex ] );
							paramList.SetParameterValue( paramIndex, intParam );
							break;
						}
						case ConsoleParameter::ParameterType::UINT:
						{
							unsigned int uintParam = unsigned int( std::stoi( tokenList[ paramIndex ] ) );
							paramList.SetParameterValue( paramIndex, uintParam );
							break;
						}
						default:
							break;
						}
					}
					catch( std::invalid_argument )
					{
						std::wstring errorMessage = L"Invalid argument: ";
						errorMessage += tokenList[ paramIndex ];
						Niflheim::MessageManager::GetInstance()->Post( Niflheim::Message::LOG_ERROR, errorMessage.c_str() );
						return;
					}
				}

				if( ConsoleCommandManager::GetInstance()->ExecuteCommand( commandToken, paramList ) )
				{
					std::wstringstream stringStream;
					stringStream << L"Executed Console Command: \"" << commandToken;
					for( int paramIndex = 0; paramIndex < paramCount; ++paramIndex )
					{
						stringStream << L" " << tokenList[ paramIndex ];
					}
					stringStream << "\"";
					Niflheim::MessageManager::GetInstance()->Post( Niflheim::Message::LOG_INFO, stringStream.str() );
				}
			}
			else
			{
				std::wstring errorMessage = L"Insufficient parameters for command: ";
				errorMessage += commandToken;
				Niflheim::MessageManager::GetInstance()->Post( Niflheim::Message::LOG_ERROR, errorMessage.c_str() );
			}
		}
		else
		{
			std::wstring errorMessage = L"Unrecognized command: ";
			errorMessage += commandToken;
			Niflheim::MessageManager::GetInstance()->Post( Niflheim::Message::LOG_ERROR, errorMessage.c_str() );
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

bool ConsoleParser::GetCommandToken( std::wstring& commandBuffer, std::wstring& token )
{
	// Skip delimiters at beginning.
	std::wstring::size_type lastPos = commandBuffer.find_first_not_of( L" ", 0 );

	if( lastPos != std::wstring::npos )
	{
		// Find first "non-delimiter".
		std::wstring::size_type pos = commandBuffer.find_first_of( L" ", lastPos );

		if( pos != std::wstring::npos )
		{
			// Found a token, add it to the vector.
			token = commandBuffer.substr( lastPos, pos - lastPos );

			// Skip delimiters.  Note the "not_of"
			lastPos = commandBuffer.find_first_not_of( L" ", pos );

			// Return the remainder of the command buffer
			commandBuffer = commandBuffer.substr( lastPos );
		}
		else
		{
			// Found a token, add it to the vector.
			token = commandBuffer.substr( lastPos );

			// Return the remainder of the command buffer
			commandBuffer = L"";
		}

		return true;
	}

	return false;
}
