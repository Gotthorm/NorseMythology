// CONSOLECOMMANDMANAGER.CPP

#include "ConsoleCommandManager.h"
#include <iostream>

bool ConsoleCommandManager::RegisterCommand( const std::wstring& functionName, std::function<void( const ConsoleParameterList& )> callback, const ConsoleParameterList& params )
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

bool ConsoleCommandManager::GetParameterList( const std::wstring& functionName, ConsoleParameterList& parameterList )
{
	std::map<std::wstring, Command>::iterator it = m_CommandTable.find( functionName );

	if( it != m_CommandTable.end() )
	{
		parameterList = (*it).second.paramList;

		return true;
	}

	return false;
}

bool ConsoleCommandManager::ExecuteCommand( const std::wstring& functionName, const ConsoleParameterList& parameterList )
{
	std::map<std::wstring, Command>::iterator it = m_CommandTable.find( functionName );

	if( it != m_CommandTable.end() )
	{
		(*it).second.functionCallback( parameterList );

		return true;
	}

	// Error about unknown command?
	return false;
}