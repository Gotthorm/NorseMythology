// CONSOLEPARAMETERLIST.CPP

#include "ConsoleParameterList.h"
#include <cstdarg>
#include <assert.h>

ConsoleParameterList::ConsoleParameterList( int paramCount, ConsoleParameter::ParameterType... )
{
	va_list args;

	va_start( args, paramCount );

	for( int index = 0; index < paramCount; ++index )
	{
		ConsoleParameter::ParameterType paramType = va_arg( args, ConsoleParameter::ParameterType );

		switch( paramType )
		{
		case ConsoleParameter::ParameterType::STRING:
			m_List.push_back( new TypedConsoleParameter<std::string>( ConsoleParameter::ParameterType::STRING, "" ) );
			break;
		case ConsoleParameter::ParameterType::INT:
			m_List.push_back( new TypedConsoleParameter<int>( ConsoleParameter::ParameterType::INT, 0 ) );
			break;
		case ConsoleParameter::ParameterType::FLOAT:
			m_List.push_back( new TypedConsoleParameter<float>( ConsoleParameter::ParameterType::FLOAT, 0.0f ) );
			break;
		case ConsoleParameter::ParameterType::INVALID:
		default:
			assert( 0 );
			break;
		}
	}

	va_end( args );
}

ConsoleParameter::ParameterType ConsoleParameterList::GetParameterType( unsigned int index ) const
{
	if( index < m_List.size() )
	{
		return m_List[ index ]->GetType();
	}

	return ConsoleParameter::ParameterType::INVALID;
}

ConsoleParameter* ConsoleParameterList::GetParameterValue( unsigned int index ) const
{
	if( index < m_List.size() )
	{
		return m_List[ index ];
	}

	return nullptr;
}

bool ConsoleParameterList::SetParameterValue( unsigned int index, int value )
{
	if( index < m_List.size() )
	{
		TypedConsoleParameter<int>* param = dynamic_cast<TypedConsoleParameter<int>*>(m_List[ index ]);

		if( param != nullptr )
		{
			param->SetData( value );
			return true;
		}
	}

	return false;
}

bool ConsoleParameterList::SetParameterValue( unsigned int index, float value )
{
	if( index < m_List.size() )
	{
		TypedConsoleParameter<float>* param = dynamic_cast<TypedConsoleParameter<float>*>( m_List[ index ] );

		if( param != nullptr )
		{
			param->SetData( value );
			return true;
		}
	}

	return false;
}

//bool ConsoleParameterList::SetParameterValue( unsigned int index, const std::string* value )
//{
//	if( index < m_List.size() )
//	{
//		TypedConsoleParameter<const std::string>* param = dynamic_cast<TypedConsoleParameter<const std::string>*>( m_List[ index ] );
//
//		if( param != nullptr )
//		{
//			param->SetData( value );
//		}
//	}
//}

bool ConsoleParameterList::SetParameterValue( unsigned int index, unsigned int value )
{
	if( index < m_List.size() )
	{
		TypedConsoleParameter<unsigned int>* param = dynamic_cast<TypedConsoleParameter<unsigned int>*>( m_List[ index ] );

		if( param != nullptr )
		{
			param->SetData( value );
			return true;
		}
	}

	return false;
}
