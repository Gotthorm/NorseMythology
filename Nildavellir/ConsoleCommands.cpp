// CONSOLECOMMANDS.CPP

#include "ConsoleCommands.h"

// Sample call back
// Framework::SetWindowSize( unsigned int, unsigned int );

// Sample register call
// ConsoleCommands::GetInstance()->RegisterCommand( Framework::SetWindowSize, "SetWindowSize", 2, ConsoleCommands::INT, ConsoleCommands::INT );

// Sample console command
// setwindowsize 640 480

// Parser looks up command "setwindowsize" and finds that this function requires two integer parameters

// Parser retrieves the required params and reports error if parameters are invalid

// How do I make the call now?
// pFunction();

bool ConsoleCommands::RegisterCommand( const std::wstring& functionName, std::function<void( const ConsoleParameterList& )> callback, const ConsoleParameterList& params )
{
	if( m_CommandTable.find( functionName ) == m_CommandTable.end() )
	{
		m_CommandTable[ functionName ].functionName = functionName;
		m_CommandTable[ functionName ].functionCallback = callback;
		m_CommandTable[ functionName ].paramList = params;

		//callback( params );

		return true;
	}

	return false;
}

bool ConsoleCommands::GetParameterList( const std::wstring& functionName, ConsoleParameterList& parameterList )
{
	std::map<std::wstring, Command>::iterator it = m_CommandTable.find( functionName );

	if( it != m_CommandTable.end() )
	{
		parameterList = (*it).second.paramList;

		return true;
	}

	return false;
}

bool ConsoleCommands::ExecuteCommand( const std::wstring& functionName, const ConsoleParameterList& parameterList )
{
	std::map<std::wstring, Command>::iterator it = m_CommandTable.find( functionName );

	if( it != m_CommandTable.end() )
	{
		(*it).second.functionCallback( parameterList );

		return true;
	}

	return false;
}
