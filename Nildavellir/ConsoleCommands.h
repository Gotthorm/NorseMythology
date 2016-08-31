// CONSOLECOMMANDS.H

#ifndef _CONSOLECOMMANDS_H_
#define _CONSOLECOMMANDS_H_

#include "Singleton.h"
#include <functional>
#include "ConsoleParameterList.h"
#include <vector>
#include <map>

//#include <tuple>

//#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
//#define SUM(...)  (sum(NUMARGS(__VA_ARGS__), __VA_ARGS__))

//#define NUMARGS(...) std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value
//#define REGISTER(...)  (ConsoleCommands::GetInstance()->RegisterCommand(NUMARGS(__VA_ARGS__), __VA_ARGS__))


class ConsoleCommands : public Singleton<ConsoleCommands>
{
public:

	bool RegisterCommand( const std::wstring& functionName, std::function<void( const ConsoleParameterList& )> callback, const ConsoleParameterList& params );

	bool GetParameterList( const std::wstring& functionName, ConsoleParameterList& parameterList );

	bool ExecuteCommand( const std::wstring& functionName, const ConsoleParameterList& parameterList );

private:
	// const std::wstring& functionName, std::function<void( ConsoleParameterList& )> callback, ConsoleParameterList params
	struct Command
	{
		std::wstring functionName;
		std::function<void( const ConsoleParameterList& )> functionCallback;
		ConsoleParameterList paramList;
	};

	// Use the default implementation
	ConsoleCommands() = default;

	// Use the default implementation
	virtual ~ConsoleCommands() = default;

	// Remove the default assignment operator
	ConsoleCommands & operator =( const ConsoleCommands & ) = delete;

	// Remove the copy constructor
	ConsoleCommands( const ConsoleCommands & ) = delete;

	std::map<std::wstring, Command> m_CommandTable;

	// Provide friend access to Singleton<ConsoleCommand> specialization
	friend Singleton<ConsoleCommands>;
};

#endif // _CONSOLECOMMANDS_H_