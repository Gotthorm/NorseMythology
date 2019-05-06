// CONSOLECOMMANDMANAGER.CPP

#include "ConsoleCommandManager.h"
#include <iostream>

namespace Alfheimr
{
	bool ConsoleCommandManager::RegisterCommand( std::wstring const & functionName, std::function<void( ParameterList const & )> callback, std::shared_ptr<const ParameterList> params )
	{
		if ( m_CommandTable.find( functionName ) == m_CommandTable.end() )
		{
			m_CommandTable[ functionName ].functionName = functionName;
			m_CommandTable[ functionName ].functionCallback = callback;
			m_CommandTable[ functionName ].paramList = params;

			return true;
		}

		return false;
	}

	bool ConsoleCommandManager::GetParameterList( std::wstring const & functionName, ParameterListImplementation & parameterList )
	{
		std::map<std::wstring, Command>::iterator it = m_CommandTable.find( functionName );

		if ( it != m_CommandTable.end() )
		{
			ParameterListImplementation const * const pParameterData = dynamic_cast< ParameterListImplementation const * >( ( ( *it ).second.paramList ).get() );

			parameterList = *pParameterData;

			return true;
		}

		return false;
	}

	bool ConsoleCommandManager::ExecuteCommand( std::wstring const & functionName, ParameterList const & parameterList )
	{
		std::map<std::wstring, Command>::iterator it = m_CommandTable.find( functionName );

		if ( it != m_CommandTable.end() )
		{
			( *it ).second.functionCallback( parameterList );

			return true;
		}

		// Error about unknown command?
		return false;
	}
}