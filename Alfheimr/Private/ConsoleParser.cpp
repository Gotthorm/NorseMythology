// CONSOLEPARSER.CPP

#include "ConsoleParser.h"
#include "Niflheim.h"
#include <sstream>
#include "ConsoleCommandManager.h"
#include "ParameterListImplementation.h"

// Some command brain storming
// log <string>
// log_warn <string>
// log_error <string>
// set <variablename> [newvalue]
// quit
// restart
// reload 

namespace Alfheimr
{
	ConsoleParser::ConsoleParser( std::weak_ptr<Niflheim::MessageManager> const & messageManager )
		: m_MessageManager( messageManager )
	{
	}

	void ConsoleParser::Execute( const std::wstring& commandLine )
	{
		std::wstring commandBuffer = commandLine;
		std::wstring commandToken;

		if ( GetCommandToken( commandBuffer, commandToken ) )
		{
			std::shared_ptr<Niflheim::MessageManager> pMessageManager = m_MessageManager.lock();

			ParameterListImplementation paramList;
			if ( ConsoleCommandManager::GetInstance()->GetParameterList( commandToken, paramList ) )
			{
				int paramCount = paramList.GetCount();

				std::vector<std::wstring> tokenList;

				int tokenCount = Tokenize( commandBuffer, tokenList );

				// We are going to allow excess parameters, we will ignore the extras but generate a warning
				if ( tokenCount > paramCount && nullptr != pMessageManager )
				{
					std::wstringstream stringStream;
					stringStream << L"Excess parameters for console command: \"" << commandToken << L"\" ";
					stringStream << L"Was expecting: " << paramCount << L" and found: " << tokenCount;
					pMessageManager->Post( Niflheim::Message::LOG_WARN, stringStream.str() );
				}

				if ( tokenCount >= paramCount )
				{
					for ( int paramIndex = 0; paramIndex < paramCount; ++paramIndex )
					{
						try
						{
							switch ( paramList.GetType( paramIndex ) )
							{
							case ParameterList::ParameterType::FLOAT:
							{
								float floatParam = std::stof( tokenList[ paramIndex ] );
								paramList.SetValue( paramIndex, floatParam );
								break;
							}
							case ParameterList::ParameterType::INT:
							{
								int intParam = std::stoi( tokenList[ paramIndex ] );
								paramList.SetValue( paramIndex, intParam );
								break;
							}
							case ParameterList::ParameterType::UINT:
							{
								unsigned int uintParam = unsigned int( std::stoi( tokenList[ paramIndex ] ) );
								paramList.SetValue( paramIndex, uintParam );
								break;
							}
							case ParameterList::ParameterType::BOOL:
							{
								std::wstring stringValue = tokenList[ paramIndex ];
								std::transform( stringValue.begin(), stringValue.end(), stringValue.begin(), ::tolower );
								if ( 0 == stringValue.compare( L"true" ) || 0 == stringValue.compare( L"1" ) )
								{
									paramList.SetValue( paramIndex, true );
								}
								else if ( 0 == stringValue.compare( L"false" ) || 0 == stringValue.compare( L"0" ) )
								{
									paramList.SetValue( paramIndex, false );
								}
								else
								{
									throw std::invalid_argument( "Invalid boolean" );
								}
								break;
							}
							default:
								break;
							}
						}
						catch ( std::invalid_argument )
						{
							if ( nullptr != pMessageManager )
							{
								std::wstring errorMessage = L"Invalid argument: ";
								errorMessage += tokenList[ paramIndex ];
								pMessageManager->Post( Niflheim::Message::LOG_ERROR, errorMessage.c_str() );
							}
							return;
						}
					}
				}

				// When there is zero tokens we treat the call as a "get" instead of a "set"
				if ( 0 == tokenCount )
				{
					paramList.Clear();
					paramCount = 0;
				}

				if ( tokenCount >= paramCount )
				{
					if ( nullptr != pMessageManager )
					{
						std::wstringstream stringStream;
						stringStream << L"Executed Console Command: \"" << commandToken;
						for ( std::wstring token : tokenList )
						{
							stringStream << L" " << token;
						}
						stringStream << "\"";
						pMessageManager->Post( Niflheim::Message::LOG_INFO, stringStream.str() );
					}

					if ( false == ConsoleCommandManager::GetInstance()->ExecuteCommand( commandToken, paramList ) )
					{
						pMessageManager->Post( Niflheim::Message::LOG_ERROR, L"Console system command execute error" );
					}
				}
				else
				{
					if ( nullptr != pMessageManager )
					{
						std::wstring errorMessage = L"Insufficient parameters for command: ";
						errorMessage += commandToken;
						pMessageManager->Post( Niflheim::Message::LOG_ERROR, errorMessage.c_str() );
					}
				}
			}
			else if ( nullptr != pMessageManager )
			{
				std::wstring errorMessage = L"Unrecognized command: ";
				errorMessage += commandToken;
				pMessageManager->Post( Niflheim::Message::LOG_ERROR, errorMessage.c_str() );
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

		while ( inputStream >> bufferString )
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

		if ( lastPos != std::wstring::npos )
		{
			// Find first "non-delimiter".
			std::wstring::size_type pos = commandBuffer.find_first_of( L" ", lastPos );

			if ( pos != std::wstring::npos )
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
}