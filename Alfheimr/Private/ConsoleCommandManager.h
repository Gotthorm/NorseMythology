// CONSOLECOMMANDS.H

#ifndef _CONSOLECOMMANDMANAGER_H_
#define _CONSOLECOMMANDMANAGER_H_

#include "Singleton.h"
#include <functional>
#include "ParameterListImplementation.h"
#include <vector>
#include <map>

class ConsoleCommandManager : public Singleton<ConsoleCommandManager>
{
public:

	bool RegisterCommand( std::wstring const & functionName, std::function<void( Alfheimr::ParameterList const & )> callback, std::shared_ptr<const Alfheimr::ParameterList> params );

	bool GetParameterList( std::wstring const & functionName, Alfheimr::ParameterListImplementation & parameterList );

	bool ExecuteCommand( std::wstring const & functionName, Alfheimr::ParameterList const & parameterList );

private:

	struct Command
	{
		std::wstring functionName;
		std::function<void( const Alfheimr::ParameterList& )> functionCallback;
		std::shared_ptr<const Alfheimr::ParameterList> paramList;
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

#endif // _CONSOLECOMMANDMANAGER_H_