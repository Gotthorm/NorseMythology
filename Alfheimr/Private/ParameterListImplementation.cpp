// PARAMETERLISTIMPLEMENTATION.CPP

#include "ParameterListImplementation.h"
#include <cstdarg>
#include <assert.h>
#include "Parameter.h"

namespace Alfheimr
{
	std::shared_ptr<const ParameterList> ParameterList::Create( int paramCount, ParameterType... )
	{
		std::shared_ptr<ParameterListImplementation> paramList = std::make_shared<ParameterListImplementation>();
	
		va_list args;

		va_start( args, paramCount );

		for ( int index = 0; index < paramCount; ++index )
		{
			ParameterType paramType = va_arg( args, ParameterType );

			switch ( paramType )
			{
			case ParameterType::STRING:
				paramList->m_List.push_back( new TypedParameter<std::wstring>( ParameterType::STRING, L"" ) );
				break;
			case ParameterType::INT:
				paramList->m_List.push_back( new TypedParameter<int>( ParameterType::INT, 0 ) );
				break;
			case ParameterType::UINT:
				paramList->m_List.push_back( new TypedParameter<unsigned int>( ParameterType::UINT, 0U ) );
				break;
			case ParameterType::FLOAT:
				paramList->m_List.push_back( new TypedParameter<float>( ParameterType::FLOAT, 0.0f ) );
				break;
			case ParameterType::BOOL:
				paramList->m_List.push_back( new TypedParameter<bool>( ParameterType::BOOL, false ) );
				break;
			case ParameterType::INVALID:
			default:
				assert( 0 );
				break;
			}
		}

		va_end( args );

		return paramList;
	}

	ParameterListImplementation::ParameterType ParameterListImplementation::GetType( unsigned int index ) const
	{
		if ( index < m_List.size() )
		{
			return m_List[ index ]->GetType();
		}

		return ParameterType::INVALID;
	}

	bool ParameterListImplementation::GetValue( unsigned int index, int & param ) const
	{
		if ( index < m_List.size() )
		{
			if ( ParameterType::INT == m_List[ index ]->GetType() )
			{
				TypedParameter<int>* pValue = dynamic_cast< TypedParameter<int>* >( m_List[ index ] );
				assert( nullptr != pValue );

				if ( nullptr != pValue )
				{
					param = pValue->GetData();
					return true;
				}
			}
		}

		return false;
	}

	bool ParameterListImplementation::GetValue( unsigned int index, bool & param ) const
	{
		if ( index < m_List.size() )
		{
			if ( ParameterType::BOOL == m_List[ index ]->GetType() )
			{
				TypedParameter<bool>* pValue = dynamic_cast< TypedParameter<bool>* >( m_List[ index ] );
				assert( nullptr != pValue );

				if ( nullptr != pValue )
				{
					param = pValue->GetData();
					return true;
				}
			}
		}

		return false;
	}

	bool ParameterListImplementation::GetValue( unsigned int index, float & param ) const
	{
		if ( index < m_List.size() )
		{
			if ( ParameterType::FLOAT == m_List[ index ]->GetType() )
			{
				TypedParameter<float>* pValue = dynamic_cast< TypedParameter<float>* >( m_List[ index ] );
				assert( nullptr != pValue );

				if ( nullptr != pValue )
				{
					param = pValue->GetData();
					return true;
				}
			}
		}

		return false;
	}

	bool ParameterListImplementation::GetValue( unsigned int index, unsigned int & param ) const
	{
		if ( index < m_List.size() )
		{
			if ( ParameterType::UINT == m_List[ index ]->GetType() )
			{
				TypedParameter<unsigned int>* pValue = dynamic_cast< TypedParameter<unsigned int>* >( m_List[ index ] );
				assert( nullptr != pValue );

				if ( nullptr != pValue )
				{
					param = pValue->GetData();
					return true;
				}
			}
		}

		return false;
	}

	bool ParameterListImplementation::GetValue( unsigned int index, std::wstring & param ) const
	{
		if ( index < m_List.size() )
		{
			if ( ParameterType::STRING == m_List[ index ]->GetType() )
			{
				TypedParameter<std::wstring>* pValue = dynamic_cast< TypedParameter<std::wstring>* >( m_List[ index ] );
				assert( nullptr != pValue );

				if ( nullptr != pValue )
				{
					param = pValue->GetData();
					return true;
				}
			}
		}

		return false;
	}

	bool ParameterListImplementation::SetValue( unsigned int index, int value )
	{
		if ( index < m_List.size() )
		{
			TypedParameter<int>* param = dynamic_cast< TypedParameter<int>* >( m_List[ index ] );

			if ( param != nullptr )
			{
				param->SetData( value );
				return true;
			}
		}

		return false;
	}

	bool ParameterListImplementation::SetValue( unsigned int index, bool value )
	{
		if ( index < m_List.size() )
		{
			TypedParameter<bool>* param = dynamic_cast< TypedParameter<bool>* >( m_List[ index ] );

			if ( param != nullptr )
			{
				param->SetData( value );
				return true;
			}
		}

		return false;
	}

	bool ParameterListImplementation::SetValue( unsigned int index, float value )
	{
		if ( index < m_List.size() )
		{
			TypedParameter<float>* param = dynamic_cast< TypedParameter<float>* >( m_List[ index ] );

			if ( param != nullptr )
			{
				param->SetData( value );
				return true;
			}
		}

		return false;
	}

	bool ParameterListImplementation::SetValue( unsigned int index, std::wstring const & value )
	{
		if ( index < m_List.size() )
		{
			TypedParameter<std::wstring>* param = dynamic_cast< TypedParameter<std::wstring>* >( m_List[ index ] );

			if ( param != nullptr )
			{
				param->SetData( value );
				return true;
			}
		}

		return false;
	}

	bool ParameterListImplementation::SetValue( unsigned int index, unsigned int value )
	{
		if ( index < m_List.size() )
		{
			TypedParameter<unsigned int>* param = dynamic_cast< TypedParameter<unsigned int>* >( m_List[ index ] );

			if ( param != nullptr )
			{
				param->SetData( value );
				return true;
			}
		}

		return false;
	}

	void ParameterListImplementation::Clear()
	{
		m_List.clear();
	}
}
