// CONSOLECOMMANDS.H

#pragma once

#include "Singleton.h"
#include <functional>
#include "ParameterListImplementation.h"
#include <vector>
#include <map>

namespace Alfheimr
{
	class ConsoleCommandManager : public Singleton<ConsoleCommandManager>
	{
	public:

		bool RegisterCommand( std::wstring const & functionName, std::function<void( ParameterList const & )> callback, std::shared_ptr<const ParameterList> params );

		bool GetParameterList( std::wstring const & functionName, ParameterListImplementation & parameterList );

		bool ExecuteCommand( std::wstring const & functionName, ParameterList const & parameterList );

	private:

		struct Command
		{
			std::wstring functionName;
			std::function<void( const ParameterList& )> functionCallback;
			std::shared_ptr<const ParameterList> paramList;
		};

		// Use the default implementation
		ConsoleCommandManager() = default;

		// Use the default implementation
		virtual ~ConsoleCommandManager() = default;

		// Remove the default assignment operator
		ConsoleCommandManager & operator =( const ConsoleCommandManager & ) = delete;

		// Remove the copy constructor
		ConsoleCommandManager( const ConsoleCommandManager & ) = delete;

		std::map<std::wstring, Command> m_CommandTable;

		// Provide friend access to Singleton<ConsoleCommand> specialization
		friend Singleton<ConsoleCommandManager>;
	};
}