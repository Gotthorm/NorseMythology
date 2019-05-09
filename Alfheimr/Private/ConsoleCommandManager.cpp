// CONSOLECOMMANDMANAGER.CPP

#include <algorithm>
#include "ConsoleCommandManager.h"
#include "ParameterListImplementation.h"

namespace Alfheimr
{
	bool ConsoleCommandManager::RegisterCommand( std::wstring const & functionName, std::function<void( ParameterList const & )> callback, std::shared_ptr<const ParameterList> params )
	{
		if ( m_CommandTable.find( functionName ) == m_CommandTable.end() )
		{
			m_CommandTable[ functionName ].functionName = functionName;
			m_CommandTable[ functionName ].functionCallback = callback;
			m_CommandTable[ functionName ].paramList = params;

			// The command list is always sorted
			auto commandIterator = std::upper_bound( m_CommandList.cbegin(), m_CommandList.cend(), functionName );
			m_CommandList.insert( commandIterator, functionName );

			return true;
		}

		return false;
	}

	bool ConsoleCommandManager::GetParameterList( std::wstring const & functionName, ParameterListImplementation & parameterList ) const
	{
		std::map<std::wstring, Command>::const_iterator it = m_CommandTable.find( functionName );

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

	bool ConsoleCommandManager::GetCommand( std::wstring const & searchName, std::wstring & functionName, unsigned int & matchIndex ) const
	{
		auto initialPosition = std::upper_bound( m_CommandList.cbegin(), m_CommandList.cend(), searchName );

		if ( initialPosition != m_CommandList.cend() )
		{
			unsigned int const searchLength = static_cast< unsigned int >( searchName.length() );

			if ( 0 == searchName.compare( ( *initialPosition ).substr( 0, searchLength ) ) )
			{
				// Initial match
				auto currentPosition = initialPosition;

				int currentIndex = 0;

				while ( currentIndex < matchIndex )
				{
					++currentIndex;
					currentPosition = std::next( currentPosition );

					if ( currentPosition == m_CommandList.cend() || 0 != searchName.compare( ( *currentPosition ).substr( 0, searchLength ) ) )
					{
						// We ran out of matches before satisfying the match index, so we wrap to the beginning
						matchIndex = 0;
						currentPosition = initialPosition;
						break;
					}
				}

				functionName = *currentPosition;

				return true;
			}
		}

		return false;
	}
}