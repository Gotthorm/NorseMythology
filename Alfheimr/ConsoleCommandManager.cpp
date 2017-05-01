// CONSOLECOMMANDMANAGER.CPP

#include "ConsoleCommandManager.h"
#include <iostream>

#include<tuple>
#define PARAM_COUNT(...) std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value
//#define FARTYPANTS_INIT(command, callback, ...) ConsoleCommandManager::GetInstance()->RegisterCommand( command, std::bind( &callback, this, std::placeholders::_1 ), ConsoleParameterList( PARAM_COUNT(__VA_ARGS__), __VA_ARGS__) ) );

#define MACRO(...) \
    int fart = std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value;

// Sample call back
// Framework::SetWindowSize( unsigned int, unsigned int );

// Sample register call
// ConsoleCommandManager::GetInstance()->RegisterCommand( Framework::SetWindowSize, "SetWindowSize", 2, ConsoleCommandManager::INT, ConsoleCommandManager::INT );

// Sample console command
// setwindowsize 640 480

// Parser looks up command "setwindowsize" and finds that this function requires two integer parameters

// Parser retrieves the required params and reports error if parameters are invalid

// How do I make the call now?
// pFunction();
//bool ConsoleCommandManager::TestRegisterCommand( const std::wstring& functionName, std::function<void( const ConsoleParameterList& )> callback, ConsoleParameter::ParameterType params... )
//{
//	//MACRO( params );
//	//int paramCount = sizeof...(params);
//
//	ConsoleParameterList paramList( PARAM_COUNT( params ), params );
//
//	if( m_CommandTable.find( functionName ) == m_CommandTable.end() )
//	{
//		m_CommandTable[ functionName ].functionName = functionName;
//		m_CommandTable[ functionName ].functionCallback = callback;
//		m_CommandTable[ functionName ].paramList = paramList;
//
//		//callback( params );
//
//		return true;
//	}
//
//	return false;
//}

bool ConsoleCommandManager::RegisterCommand( const std::wstring& functionName, std::function<void( const float&, const float& )> callback, const ConsoleParameterList& params )
{
	if( m_CommandTable.find( functionName ) == m_CommandTable.end() )
	{
		m_CommandTable[ functionName ].functionName = functionName;
		//m_CommandTable[ functionName ].functionCallback = callback;
		m_CommandTable[ functionName ].paramList = params;

		callback( 2.0f, 2.0f );

		return true;
	}

	return false;
}

//bool ConsoleCommandManager::RegisterCommand( const std::wstring& functionName, std::function<void( const ConsoleParameterList& )> callback, const ConsoleParameterList& params )
//{
//	if( m_CommandTable.find( functionName ) == m_CommandTable.end() )
//	{
//		m_CommandTable[ functionName ].functionName = functionName;
//		m_CommandTable[ functionName ].functionCallback = callback;
//		m_CommandTable[ functionName ].paramList = params;
//
//		//callback( params );
//
//		return true;
//	}
//
//	return false;
//}

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

/*
if( paramList.GetCount() == 2 )
{
if( paramList.GetParameterType( 0 ) == ConsoleParameter::ParameterType::FLOAT && paramList.GetParameterType( 1 ) == ConsoleParameter::ParameterType::FLOAT )
{
TypedConsoleParameter<float>* param1 = dynamic_cast<TypedConsoleParameter<float>*>( paramList.GetParameterValue( 0 ) );
TypedConsoleParameter<float>* param2 = dynamic_cast<TypedConsoleParameter<float>*>( paramList.GetParameterValue( 1 ) );
PLATFORM_ASSERT( param1 != nullptr && param2 != nullptr );

if( param1 != nullptr && param2 != nullptr )
{
float widthScale = param1->GetData();
float heightScale = param2->GetData();

SetTextScale( widthScale, heightScale );
}
}
}
*/

bool ConsoleCommandManager::ExecuteCommand( const std::wstring& functionName, const ConsoleParameterList& parameterList )
{
	std::map<std::wstring, Command>::iterator it = m_CommandTable.find( functionName );

	if( it != m_CommandTable.end() )
	{
		(*it).second.functionCallback( parameterList );

		return true;

		//const Command& command = ( *it ).second;

		//// Test the given parameters against the function signature
		//if( parameterList.GetCount() == command.paramList.GetCount() )
		//{
		//	for( int parameterIndex = 0; parameterIndex < parameterList.GetCount(); ++parameterIndex )
		//	{
		//		if( parameterList.GetParameterType( parameterIndex ) != command.paramList.GetParameterType( parameterIndex ) )
		//		{
		//			// Error about the incorrect parameter type?
		//			return false;
		//		}
		//	}

		//	// TODO BEGIN: Generalize this to handle all cases
		//	TypedConsoleParameter<float>* param1 = dynamic_cast<TypedConsoleParameter<float>*>( parameterList.GetParameterValue( 0 ) );
		//	TypedConsoleParameter<float>* param2 = dynamic_cast<TypedConsoleParameter<float>*>( parameterList.GetParameterValue( 1 ) );
		//	PLATFORM_ASSERT( param1 != nullptr && param2 != nullptr );

		//	float widthScale = param1->GetData();
		//	float heightScale = param2->GetData();

		//	( *it ).second.functionCallback( widthScale, heightScale );
		//	// TODO: END
		//}
		//else
		//{
		//	// Error about the incorrect number of parameters?
		//	return false;
		//}
	}

	// Error about unknown command?
	return false;
}