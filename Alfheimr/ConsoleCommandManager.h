// CONSOLECOMMANDS.H

#ifndef _CONSOLECOMMANDMANAGER_H_
#define _CONSOLECOMMANDMANAGER_H_

#include "Singleton.h"
#include <functional>
#include "ConsoleParameterList.h"
#include <vector>
#include <map>

#include <tuple>

//#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
//#define SUM(...)  (sum(NUMARGS(__VA_ARGS__), __VA_ARGS__))

//#define NUMARGS(...) std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value
//#define REGISTER(...)  (ConsoleCommandManager::GetInstance()->RegisterCommand(NUMARGS(__VA_ARGS__), __VA_ARGS__))

// EXAMPLE: REGISTER_COMMAND( L"set_text_size", Console::SetTextSize, ConsoleParameter::ParameterType::FLOAT, ConsoleParameter::ParameterType::FLOAT )
//#define REGISTER_COMMAND( commandString, callbackFunction, ... ) ConsoleCommandManager::GetInstance()->RegisterCommand( commandString, \
//	std::bind( &callbackFunction, this, std::placeholders::_1 ), \
//	ConsoleParameterList( std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value, __VA_ARGS__ ) );

#define REGISTER_COMMAND1( commandString, callbackFunction, param1 ) ConsoleCommandManager::GetInstance()->RegisterCommand( commandString, \
	std::bind( &callbackFunction, this, std::placeholders::_1 ), \
	ConsoleParameterList( 1, param1 ) );

#define REGISTER_COMMAND2( commandString, callbackFunction, param1, param2 ) ConsoleCommandManager::GetInstance()->RegisterCommand( commandString, \
	std::bind( &callbackFunction, this, std::placeholders::_1, std::placeholders::_2 ), \
	ConsoleParameterList( 2, param1, param2 ) );

// 	ConsoleParameterList( sizeof((int[]){__VA_ARGS__})/sizeof(int), __VA_ARGS__ ) );

class ConsoleCommandManager : public Singleton<ConsoleCommandManager>
{
public:

	//void AddCommand( const std::wstring& functionName,  )

	//bool RegisterCommand( const std::wstring& functionName, std::function<void( const ConsoleParameterList& )> callback, const ConsoleParameterList& params );
	bool RegisterCommand( const std::wstring& functionName, std::function<void( const float&, const float& )> callback, const ConsoleParameterList& params );

	//template < typename FN >
	//bool RegisterCommand( const std::wstring& functionName, const std::wstring& functionDescription, FN&& fn );

	bool GetParameterList( const std::wstring& functionName, ConsoleParameterList& parameterList );

	bool ExecuteCommand( const std::wstring& functionName, const ConsoleParameterList& parameterList );

	//bool TestRegisterCommand( const std::wstring& functionName, std::function<void( const ConsoleParameterList& )> callback, ConsoleParameter::ParameterType params... );

	//template<typename Func>
	//static bool FartRegisterCommand( const std::wstring& functionName, ConsoleParameter::ParameterType params... ) 
	//{ 
	//	//this; 
	//	return false; 
	//}

private:
	// const std::wstring& functionName, std::function<void( ConsoleParameterList& )> callback, ConsoleParameterList params
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

// Templated methods must be defined in the header
//template < typename FN >
//bool ConsoleCommandManager::RegisterCommand( const std::wstring& functionName, const std::wstring& functionDescription, FN&& fn )
//{
//	return false;
//}

#endif // _CONSOLECOMMANDMANAGER_H_