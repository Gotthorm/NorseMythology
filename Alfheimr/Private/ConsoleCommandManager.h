// CONSOLECOMMANDMANAGER.H

#pragma once

#include <functional>
#include <memory>
#include <map>
#include <set>

namespace Alfheimr
{
	class ParameterList;
	class ParameterListImplementation;

	class ConsoleCommandManager
	{
	public:
		// Use default implementation
		ConsoleCommandManager() = default;
		virtual ~ConsoleCommandManager() = default;

		bool RegisterCommand( std::wstring const & functionName, std::function<void( ParameterList const & )> callback, std::shared_ptr<const ParameterList> params );

		bool GetParameterList( std::wstring const & functionName, ParameterListImplementation & parameterList ) const;

		bool ExecuteCommand( std::wstring const & functionName, ParameterList const & parameterList );

		bool GetCommand( std::wstring const & searchName, std::wstring & functionName, unsigned int & matchIndex ) const;

	private:

		struct Command
		{
			std::wstring functionName;
			std::function<void( const ParameterList& )> functionCallback;
			std::shared_ptr<const ParameterList> paramList;
		};

		// Remove the default assignment operator
		ConsoleCommandManager & operator =( const ConsoleCommandManager & ) = delete;

		// Remove the copy constructor
		ConsoleCommandManager( const ConsoleCommandManager & ) = delete;

		std::map<std::wstring, Command> m_CommandTable;
		std::set<std::wstring> m_CommandList;
	};
}