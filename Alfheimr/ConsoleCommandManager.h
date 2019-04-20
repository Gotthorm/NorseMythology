// CONSOLECOMMANDS.H

#ifndef _CONSOLECOMMANDMANAGER_H_
#define _CONSOLECOMMANDMANAGER_H_

#include "Singleton.h"
#include <functional>
#include "ConsoleParameterList.h"
#include <vector>
#include <map>

class ConsoleCommandManager : public Singleton<ConsoleCommandManager>
{
public:

	bool RegisterCommand( const std::wstring& functionName, std::function<void( const ConsoleParameterList& )> callback, const ConsoleParameterList& params );

	bool GetParameterList( const std::wstring& functionName, ConsoleParameterList& parameterList );

	bool ExecuteCommand( const std::wstring& functionName, const ConsoleParameterList& parameterList );

private:

	struct Command
	{
		std::wstring functionName;
		std::function<void( const ConsoleParameterList& )> functionCallback;
		ConsoleParameterList paramList;
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